/** @noop License Comment
 *  @file
 *  @copyright
 *  This Source Code is subject to the terms of the Mozilla Public License, v2.0.
 *  If a copy of the MPL was not distributed with this file You can obtain one at
 *  https://mozilla.org/MPL/2.0/
 *  
 *  @author David Mórász
 *  @date 2025
 */




#include "Mac/TDxWareReadingMethod.h"

#if PLATFORM_MAC

#include "SpaceMouseReader.h"
#include "SmButtons.h"

// connection to driver
// grabbed from https://github.com/blender/blender/blob/594f47ecd2d5367ca936cf6fc6ec8168c2b360d0/intern/ghost/intern/GHOST_NDOFManagerCocoa.mm

#include <dlfcn.h>
#include <stdint.h>

static TSharedPtr<FTDxWareReadingMethod> TDxWareReader;

// replicate just enough of the 3Dx API for our uses, not everything the driver provides

#pragma pack(push, 2)  // just this struct
struct ConnexionDeviceState {
    uint16_t version;
    uint16_t client;
    uint16_t command;
    int16_t param;
    int32_t value;
    uint64_t time;
    uint8_t report[8];
    uint16_t buttons8;  // obsolete! (pre-10.x drivers)
    int16_t axis[6];    // tx, ty, tz, rx, ry, rz
    uint16_t address;
    uint32_t buttons;
};
#pragma pack(pop)

#define kConnexionClientModeTakeOver 1
#define kConnexionMaskAll 0x3fff
#define kConnexionMaskAllButtons 0xffffffff
#define kConnexionCmdHandleButtons 2
#define kConnexionCmdHandleAxis 3
#define kConnexionCmdAppSpecific 10
#define kConnexionMsgDeviceState '3dSR'
#define kConnexionCtlGetDeviceID '3did'

// callback functions:
typedef void (*AddedHandler)(uint32_t);
typedef void (*RemovedHandler)(uint32_t);
typedef void (*MessageHandler)(uint32_t, uint32_t msg_type, void* msg_arg);

// driver functions:
typedef int16_t (*SetConnexionHandlers_ptr)(MessageHandler, AddedHandler, RemovedHandler, bool);
typedef int16_t (*InstallConnexionHandlers_ptr)(MessageHandler, AddedHandler, RemovedHandler);
typedef void (*CleanupConnexionHandlers_ptr)();

typedef uint16_t (*RegisterConnexionClient_ptr)(
    uint32_t signature,
    const char* name,
    uint16_t mode,
    uint32_t mask
);

typedef void (*SetConnexionClientButtonMask_ptr)(uint16_t clientID, uint32_t buttonMask);
typedef void (*UnregisterConnexionClient_ptr)(uint16_t clientID);

typedef int16_t (*ConnexionClientControl_ptr)(
    uint16_t clientID,
    uint32_t message,
    int32_t param,
    int32_t *result
);

#define DECLARE_FUNC(name) name##_ptr name = NULL

DECLARE_FUNC(SetConnexionHandlers);
DECLARE_FUNC(InstallConnexionHandlers);
DECLARE_FUNC(CleanupConnexionHandlers);
DECLARE_FUNC(RegisterConnexionClient);
DECLARE_FUNC(SetConnexionClientButtonMask);
DECLARE_FUNC(UnregisterConnexionClient);
DECLARE_FUNC(ConnexionClientControl);

void* load_func(void* module, const char* func_name)
{
    void* func = dlsym(module, func_name);
    FString FuncName {func_name};
    if(func)
    {
        UE_LOG(LogSmReader, Display, TEXT("3DxWare function loaded: %s"), *FuncName);
    }
    else
    {
        FString DlError {dlerror()};
        UE_LOG(LogSmReader, Error, TEXT("Couldn't load 3DxWare function: %s (%s)"), *FuncName, *DlError);
    }
    return func;
}

#define LOAD_FUNC(name) name = (name##_ptr)load_func(module, #name)

void* module;  // handle to the whole driver

bool load_driver_functions()
{
    if (TDxWareReader->bDriverLoaded) {
        return true;
    }

    // TODO: should we use local instead? It's supposedly already installed on the user's mac
    module = dlopen("/Library/Frameworks/3DconnexionClient.framework/3DconnexionClient", RTLD_LAZY | RTLD_LOCAL);

    if (module) {
        LOAD_FUNC(SetConnexionHandlers);

        if (SetConnexionHandlers != NULL) {
            TDxWareReader->bDriverLoaded = true;
            TDxWareReader->bHasNewDriver = true;
        }
        else {
            LOAD_FUNC(InstallConnexionHandlers);

            TDxWareReader->bDriverLoaded = (InstallConnexionHandlers != NULL);
        }

        if (TDxWareReader->bDriverLoaded) {
            LOAD_FUNC(CleanupConnexionHandlers);
            LOAD_FUNC(RegisterConnexionClient);
            LOAD_FUNC(SetConnexionClientButtonMask);
            LOAD_FUNC(UnregisterConnexionClient);
            LOAD_FUNC(ConnexionClientControl);

            TDxWareReader->bHasOldDriver = (SetConnexionClientButtonMask == NULL);
        }
    }

    return TDxWareReader->bDriverLoaded;
}

void unload_driver()
{
    dlclose(module);
}

void FTDxWareReadingMethod::DeviceAdded(uint32_t unused)
{
    // determine exactly which device is plugged in
    int32_t result;
    ConnexionClientControl(TDxWareReader->ClientID, kConnexionCtlGetDeviceID, 0, &result);
    auto vendorID = static_cast<uint16>(result >> 16);
    auto productID = static_cast<uint16>(result & 0xffff);
    
    UE_LOG(LogSmReader, Display, TEXT("A device got connected."));

    TDxWareReader->SeenDevices.Add({vendorID, productID});
}

void FTDxWareReadingMethod::DeviceRemoved(uint32_t unused)
{
    UE_LOG(LogSmReader, Display, TEXT("A device got removed."));
}

union FButtonBridge
{
    FButtonBits Output;
    uint32_t Input;
};

void FTDxWareReadingMethod::DeviceEvent(uint32_t unused, uint32_t msg_type, void* msg_arg)
{
    UE_LOG(LogSmReader, Display, TEXT("Device Event: %d %p"), msg_type, msg_arg);
    if (msg_type == kConnexionMsgDeviceState) {
        ConnexionDeviceState* s = (ConnexionDeviceState*)msg_arg;

        // device state is broadcast to all clients; only react if sent to us
        if (s->client == TDxWareReader->ClientID) {
            switch (s->command) {
                case kConnexionCmdHandleAxis: {
                    TDxWareReader->AccumulatedData.Translation = FVector(
                        static_cast<float>(s->axis[0]),
                        static_cast<float>(s->axis[1]),
                        static_cast<float>(s->axis[2])
                    );
                    TDxWareReader->AccumulatedData.Rotation = FRotator(
                        static_cast<float>(s->axis[3]),
                        static_cast<float>(s->axis[4]),
                        static_cast<float>(s->axis[5])
                    );
                    TDxWareReader->OnDataReceived.Broadcast();
                    break;
                }
                case kConnexionCmdHandleButtons: {
                    FButtonBridge bridge {};
                    bridge.Input = TDxWareReader->bHasOldDriver ? s->buttons8 : s->buttons;
                    TDxWareReader->AccumulatedData.Buttons |= bridge.Output;
                    TDxWareReader->OnDataReceived.Broadcast();
                    break;
                }
                case kConnexionCmdAppSpecific: {
                    UE_LOG(LogSmReader, Display, TEXT("app-specific command, param = %hd, value = %d"), s->param, s->value);
                    break;
                }
                default: {
                    UE_LOG(LogSmReader, Display, TEXT("mystery device command %d"), s->command);
                    break;
                }
            }
        }
    }
}

TSharedPtr<FTDxWareReadingMethod> FTDxWareReadingMethod::GetSingleton()
{
    if(!TDxWareReader)
    {
        TDxWareReader = MakeShared<FTDxWareReadingMethod>();
        TDxWareReader->Init();
    }
    
    return TDxWareReader;
}

FTDxWareReadingMethod::FTDxWareReadingMethod()
{
}

void FTDxWareReadingMethod::Init()
{
    if (!load_driver_functions())
    {
        UE_LOG(LogSmReader, Error, TEXT("Could not load 3DConnexion driver functions"));
        return;
    }

    uint16_t error;
    if (TDxWareReader->bHasNewDriver)
    {
        const bool separate_thread = false;  // TODO: blender has it false, wonder if they had a very good reason
        error = SetConnexionHandlers(
            FTDxWareReadingMethod::DeviceEvent,
            FTDxWareReadingMethod::DeviceAdded,
            FTDxWareReadingMethod::DeviceRemoved,
            separate_thread
        );
    }
    else
    {
        error = InstallConnexionHandlers(
            FTDxWareReadingMethod::DeviceEvent,
            FTDxWareReadingMethod::DeviceAdded,
            FTDxWareReadingMethod::DeviceRemoved
        );
    }

    if (error) {
        UE_LOG(LogSmReader, Error, TEXT("error %d while setting up handlers"), error);
        return;
    }

    // Pascal string *and* a four-letter constant. How old-skool.
    ClientID = RegisterConnexionClient(
        '\?\?\?\?', "\023com.epicgames.UE4Editor", // extracted from the Info.plist file of UE4 editor, yeah even the ????
        kConnexionClientModeTakeOver,
        kConnexionMaskAll
    );
    UE_LOG(LogSmReader, Display, TEXT("3DxWare assigned client ID: %d"), ClientID);

    if (!bHasOldDriver) {
        SetConnexionClientButtonMask(ClientID, kConnexionMaskAllButtons);
    }
}

FTDxWareReadingMethod::~FTDxWareReadingMethod()
{
    UE_LOG(LogSmReader, Warning, TEXT("Unregistering from 3DxWare"));
    if(bDriverLoaded)
    {
        UnregisterConnexionClient(ClientID);
        CleanupConnexionHandlers();
        unload_driver();
    }
}

void FTDxWareReadingMethod::Tick(FDataReadingOutput& Output, float DeltaSecs)
{
    FDataReadingMethod::Tick(Output, DeltaSecs);
    FProcessedDeviceOutput CurrData = AccumulatedData;
    bool bCurrMoved = bMoved;
    bMoved = false;

    if(bCurrMoved)
    {
        ApplyTranslation(
            Output,
            CurrData.Translation.X / GetAxisResolution(),
            CurrData.Translation.Y / GetAxisResolution(),
            CurrData.Translation.Z / GetAxisResolution(),
            DeltaSecs
        );
        ApplyRotation(
            Output,
            CurrData.Rotation.Pitch / GetAxisResolution(),
            CurrData.Rotation.Yaw / GetAxisResolution(),
            CurrData.Rotation.Roll / GetAxisResolution(),
            DeltaSecs
        );
        Output.MovementState->Move();
    }
    Output.ProcessedData->Buttons = Output.NormData->Buttons = CurrData.Buttons;

    TickMovementState(Output, DeltaSecs);
}

#endif
