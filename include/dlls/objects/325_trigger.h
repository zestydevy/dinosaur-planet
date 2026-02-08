#ifndef _DLLS_325_H
#define _DLLS_325_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/math.h"

typedef struct {
    u8 condition; // TriggerCommandConditionFlags enum
    u8 id;
    // For some commands, param1 and 2 are read as a single u16, but 
    // in code they are read individually and then combined with bit math.
    u8 param1;
    u8 param2;
} TriggerCommand;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ TriggerCommand commands[8];
/*38*/ s16 localID; // TODO: needs verification
/*3A*/ u8 sizeX; // unit depends on trigger type
/*3B*/ u8 sizeY; // unit depends on trigger type
/*3C*/ u8 sizeZ; // unit depends on trigger type
/*3D*/ u8 rotationY; // unit depends on trigger type
/*3E*/ u8 rotationX; // unit depends on trigger type
/*3F*/ u8 _unk3F[4];
/// The object type of the object that can activate the trigger.
/// If multiple instances of the given type exist in the scene, the one
/// closest to the trigger will be used.
/// Exceptions:
/// 0 - Always the primary player (even if multiple player instances exist)
/// 1 - Always the primary sidekick (even if multiple sidekick instances exist)
/// 2 - The camera(?)
/*43*/ u8 activatorObjType;
// Game bit flag to save the entered state of the trigger in.
// This flag will be set the first time the trigger is entered. If the flag is already
// set upon object creation, the entered state of the trigger will be restored, possibly
// running commands on the next update that support being restored.
/*44*/ s16 bitFlagID;
// Number of game ticks from object creation to wait before activating a 
// TriggerTime instance.
/*46*/ u16 timerDuration;
// Game bit flags that must be *all* set before the trigger can activate.
// Only supported by TriggerPlane and TriggerBits (plane only supports one flag to check).
// A negative ID indicates that there is no flag to check for that condition slot. 
/*48*/ s16 conditionBitFlagIDs[4];
} Trigger_Setup;

DLL_INTERFACE(DLL_TriggerScript) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    // Array length will vary
    void (*subscripts[1])(Object *trigger, Object *activator, s8 dir, s32 activatorDistSquared);
};

typedef struct {
/*00*/ u8 flags; // TriggerFlags enum
/*01*/ u8 _unk1[3];
/*04*/ f32 radiusSquared;
/*08*/ u8 _unk8[4];
/*0C*/ u32 elapsedTicks; // for TriggerTime
/*10*/ Vec3f lookVector; // for TriggerPlane
/*1C*/ f32 lookVectorNegDot;
/*20*/ Vec3f activatorPrevPos;
/*2C*/ Vec3f activatorCurrPos;
/*38*/ Vec3f planeMin;
/*44*/ Vec3f planeMax;
/*50*/ u8 _unk50[8];
/*58*/ s16 bitFlagID;
/*5A*/ s16 conditionBitFlagIDs[4];
/*62*/ u8 _unk62[2];
/*64*/ u32 soundHandles[8];
// Special "script" DLLs where each export is a "subscript".
/*84*/ DLL_TriggerScript *scripts[8];
} Trigger_Data;

typedef enum {
    // Activator entered at least once
    TRG_ACTIVATOR_ENTERED = 0x1,
    // Activator exited at least once
    TRG_ACTIVATOR_EXITED = 0x2,
    TRG_RESTORE_ENTERED_STATE = 0x4,
    TRG_FIRST_TICK = 0x40
} TriggerFlags;

typedef enum {
    // When activator is "inside" the trigger
    CMD_COND_IN = 0x1,
    // When activator is "outside" of the trigger
    CMD_COND_OUT = 0x2,
    // Command can be activated if trigger is entered more than once
    CMD_COND_RE_ENTER = 0x4,
    // Command can be activated if trigger is exited more than once
    CMD_COND_RE_EXIT = 0x8,
    // Command is activated every game tick the in/out conditions are met,
    // and not just on the initial entry/exit tick
    CMD_COND_CONTINUOUS = 0x10,
    // Activate the command when restoring the trigger's entered state
    CMD_COND_RESTORE = 0x20
} TriggerCommandConditionFlags;

typedef enum {
    TRG_CMD_HAZARD = 0x1, // "gameplay vulnerable"?
    TRG_CMD_2 = 0x2, // not implemented
    TRG_CMD_MUSIC_ACTION = 0x3,
    TRG_CMD_SOUND = 0x4,
    TRG_CMD_5 = 0x5,
    TRG_CMD_CAMERA_ACTION = 0x6,
    TRG_CMD_7 = 0x7, // not implemented
    TRG_CMD_TRACK = 0x8,
    TRG_CMD_9 = 0x9, // not implemented
    TRG_CMD_ENV_FX = 0xA,
    TRG_CMD_ANIM_SEQ = 0xB,
    TRG_CMD_TRIGGER = 0xC,
    TRG_CMD_LIGHTING = 0xD,
    TRG_CMD_STORYBOARD = 0xE, // not implemented
    TRG_CMD_SETUP_POINT = 0xF,
    TRG_CMD_LOD_MODEL = 0x10,
    TRG_CMD_TRICKY_TALK_SEQ = 0x11,
    TRG_CMD_FLAG = 0x12,
    TRG_CMD_ENABLE_OBJ_GROUP = 0x13,
    TRG_CMD_DISABLE_OBJ_GROUP = 0x14,
    TRG_CMD_TEXTURE_LOAD = 0x15,
    TRG_CMD_TEXTURE_FREE = 0x16,
    TRG_CMD_17 = 0x17, // not implemented
    TRG_CMD_SET_MAP_SETUP = 0x18,
    TRG_CMD_SCRIPT = 0x19,
    TRG_CMD_WORLD_ENABLE_OBJ_GROUP = 0x1A,
    TRG_CMD_WORLD_DISABLE_OBJ_GROUP = 0x1B,
    TRG_CMD_KYTE_FLIGHT_GROUP = 0x1C,
    TRG_CMD_KYTE_TALK_SEQ = 0x1D,
    TRG_CMD_WORLD_SET_MAP_SETUP = 0x1E,
    TRG_CMD_SAVE_GAME = 0x1F,
    TRG_CMD_MAP_LAYER = 0x20,
    TRG_CMD_FLAG_TOGGLE = 0x21,
    TRG_CMD_TOGGLE_OBJ_GROUP = 0x22,
    TRG_CMD_RESTART = 0x23,
    TRG_CMD_WATER_FALLS_FLAGS = 0x24,
    TRG_CMD_WATER_FALLS_FLAGS2 = 0x25,
    TRG_CMD_SIDEKICK = 0x26
} TriggerCommandID;

#endif
