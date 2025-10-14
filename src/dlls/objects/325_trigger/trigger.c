#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "game/gamebits.h"
#include "sys/asset_thread.h"
#include "sys/camera.h"
#include "sys/dll.h"
#include "sys/objmsg.h"
#include "sys/gfx/model.h"
#include "sys/gfx/gx.h"
#include "sys/gfx/texture.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "dll.h"
#include "dll_def.h"
#include "types.h"
#include "functions.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/210_player.h"

static const char str_1[] = "triggers: unknown trigger object %d.\n";
static const char str_2[] = "TRIGGER: warning Script overflow\n";
static const char str_3[] = "Trigger [%d], Gamplay Vulnerable";
static const char str_4[] = "Trigger [%d], Music Action,       Action Num [%d] Free";
static const char str_5[] = "Trigger [%d], Music Action,       Action Num [%d] Set";
static const char str_6[] = "Trigger [%d], Sound FX,           Action Num [%d],Handle Num [%d]";
static const char str_7[] = "Trigger [%d], Camera,             Action [%d], Camera Num [%d], PassDir [%d]";
static const char str_8[] = "Trigger [%d], Track Sky On";
static const char str_9[] = "Trigger [%d], Track Sky Off";
static const char str_10[] = "Trigger [%d], Track AntiAlias On";
static const char str_11[] = "Trigger [%d], Track AntiAlias Off";
static const char str_12[] = "Trigger [%d], Track SkyObjects On";
static const char str_13[] = "Trigger [%d], Track SkyObjects Off";
static const char str_14[] = "Trigger [%d], Track Dome On";
static const char str_15[] = "Trigger [%d], Track Dome Off";
static const char str_16[] = "Trigger [%d], Track MrSheen On %d";
static const char str_17[] = "Trigger [%d], Track MrSheen Off";
static const char str_18[] = "Trigger [%d], Environment Effect, Action Num [%d], Range [%d]";
static const char str_19[] = "Trigger [%d], Lighting,           Action      [%d], Range [%d], PassDir [%d]";
static const char str_20[] = "Trigger [%d], Anim Sequence,      SequenceID [%d], Activate";
static const char str_21[] = "Trigger [%d], Anim Sequence,      SequenceID [%d], Flag = 1";
static const char str_22[] = "Trigger [%d], Anim Sequence,      SequenceID [%d], Flag = 0";
static const char str_23[] = "Trigger [%d], Trigger,            Local ID   [%d]";
static const char str_24[] = "Storyboard disabled, please remove trigger\n";
static const char str_25[] = "Trigger [%d], LOD Model [%d]";
static const char str_26[] = "Trigger [%d], Setup Point,        Level      [%d], SetupPoint [%d]";
static const char str_27[] = "Trigger [%d], Bits\n";
static const char str_28[] = "Trigger [%d], Object Load\n";
static const char str_29[] = "Trigger [%d], Object Free\n";
static const char str_30[] = "Trigger [%d], Object Toggle\n";
static const char str_31[] = "Trigger [%d], Tex Load\n";
static const char str_32[] = "Trigger [%d], Tex Free\n";
static const char str_33[] = "TRIGGER: warning DLL not loaded\n";
static const char str_34[] = "Script [%d], Subscript [%d]\n";
static const char str_35[] = "Trigger [%d], Object Load\n";
static const char str_36[] = "Trigger [%d], Object Free\n";
static const char str_37[] = "Restart Set [%d]\n";
static const char str_38[] = "Restart Clear [%d]\n";
static const char str_39[] = "Restart Goto [%d]\n";
static const char str_40[] = "killing sidekick\n";
static const char str_41[] = "findobj %i \n";
static const char str_42[] = "Trigger [%d], amSfxWaterFallsSetFlags,   Action [%d], PassDir [%d]";

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
/*0C*/ s32 elapsedTicks; // for TriggerTime
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
    TRG_CMD_E = 0xE, // not implemented
    TRG_CMD_F = 0xF,
    TRG_CMD_LOD_MODEL = 0x10,
    TRG_CMD_11 = 0x11, // TRG_CMD_TRICKY_?
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

static ModelInstance *sPointModel = NULL;
static ModelInstance *sPlaneModel = NULL;

static s16 sPointModelRefCount;
static s16 sPlaneModelRefCount;

/*static*/ void trigger_process_commands(Object *self, Object *activator, s8 dir, s32 activatorDistSquared);
/*static*/ void trigger_func_1754(u8 param1, u8 param2);
/*static*/ void trigger_func_1764(u16 param1);
/*static*/ void trigger_func_17FC(u16 param1);
/*static*/ void trigger_func_1868(u16 param1);
/*static*/ void trigger_func_1920(u16 param1);

static void trigger_point_setup(Object *obj, Trigger_Setup *setup);
/*static*/ void trigger_point_update(Object *self, Object *activator);

static void trigger_cylinder_setup(Object *obj, Trigger_Setup *setup);
/*static*/ void trigger_cylinder_update(Object *self, Object* activator);

static void trigger_plane_setup(Object *obj, Trigger_Setup *setup);
/*static*/ void trigger_plane_update(Object *self, Object *activator);

static void trigger_area_setup(Object *self, Trigger_Setup *setup);
/*static*/ void trigger_area_update(Object *self, Object *activator);
/*static*/ s32 trigger_func_273C(Object *self, Vec3f *vec);
static void trigger_func_2884(Object *self, f32 *ox, f32 *oy, f32 *oz);
/*static*/ void trigger_func_29C0(u16 localID, Object *activator, s8 dir, s32 activatorDistSquared);

static void trigger_curve_setup(Object *self, Trigger_Setup *setup);
/*static*/ void trigger_curve_update(Object *self, Object *activator);

void trigger_ctor(void *dll) { }

void trigger_dtor(void *dll) { }

void trigger_setup(Object *self, Trigger_Setup *setup, s32 param3) {
    Trigger_Data *objdata;
    s32 s0;
    s32 i;
    TriggerCommand *cmd;

    obj_add_object_type(self, OBJTYPE_2);
    obj_set_update_priority(self, 40);

    objdata = (Trigger_Data*)self->data;

    switch (setup->base.objId) {
        case OBJ_TriggerPoint:
            trigger_point_setup(self, setup);
            break;
        case OBJ_TriggerPlane:
            if ((setup && setup) && setup){} // fakematch

            objdata->conditionBitFlagIDs[0] = setup->conditionBitFlagIDs[0];
            trigger_plane_setup(self, setup);
            break;
        case OBJ_TriggerCylinder:
            trigger_cylinder_setup(self, setup);
            break;
        case OBJ_TriggerArea:
            trigger_area_setup(self, setup);
            break;
        case OBJ_TriggerBits:
            objdata->conditionBitFlagIDs[0] = setup->conditionBitFlagIDs[0];
            objdata->conditionBitFlagIDs[1] = setup->conditionBitFlagIDs[1];
            objdata->conditionBitFlagIDs[2] = setup->conditionBitFlagIDs[2];
            objdata->conditionBitFlagIDs[3] = setup->conditionBitFlagIDs[3];
            break;
        case OBJ_TriggerCurve:
            trigger_curve_setup(self, setup);
            break;
        case OBJ_TriggerTime:
        case OBJ_TriggerButton:
        case OBJ_TriggerSetup:
            break;
        default:
            // "triggers: unknown trigger object %d.\n"
            break;
    }

    objdata->bitFlagID = setup->bitFlagID;
    s0 = main_get_bits(objdata->bitFlagID);

    cmd = setup->commands;

    for (i = 0; i < 8; i++, cmd++) {
        objdata->soundHandles[i] = 0;
        objdata->scripts[i] = NULL;

        if (cmd->id == TRG_CMD_SCRIPT) {
            // bug? Why does this hijack the variable for the bit flag value?
            // Is it the responsibility of the script to handle the previous state?
            s0 = cmd->param1 + 81; // scripts are DLLs 81-83
            if (s0 < 84 && param3 == 0) {
                objdata->scripts[i] = dll_load_deferred(s0, 0);
            }
            // "TRIGGER: warning Script overflow\n"
        }
    }

    if (s0 == 1) {
        // Activator previously "entered" the trigger, restore this state
        objdata->flags |= TRG_RESTORE_ENTERED_STATE;
    }
    objdata->flags |= TRG_FIRST_TICK;
}

// needs trigger_process_commands to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_control.s")
#else
void trigger_control(Object* self) {
    Trigger_Data* objdata;
    Trigger_Setup* setup;
    Object* player;
    Object* temp_v0_2;
    Object* sidekick;
    Object* activatorObj;
    s32 i;
    s32 b_allBitsSet;
    s32 b_foundActivatorObj;
    f32 maxObjSearchDist;

    objdata = (Trigger_Data*)self->data;
    setup = (Trigger_Setup*)self->setup;
    
    maxObjSearchDist = 200.0f;
   
    player = get_player();
    if (player != NULL) {
        temp_v0_2 = ((DLL_210_Player*)player->dll)->vtbl->func7(player);
        if (temp_v0_2 != NULL) {
            player = temp_v0_2;
        }
    }
    
    sidekick = get_sidekick();
    
    if ((player != NULL) || (sidekick != NULL)) {
        if (objdata->flags & TRG_RESTORE_ENTERED_STATE) {
            trigger_process_commands(self, player, 1, 0);
            objdata->flags &= ~TRG_RESTORE_ENTERED_STATE;
            objdata->flags |= TRG_ACTIVATOR_ENTERED;
            return;
        }
        
        b_foundActivatorObj = TRUE;
        if (setup->activatorObjType >= 3) {
            activatorObj = obj_get_nearest_type_to(setup->activatorObjType, self, &maxObjSearchDist);
            if (activatorObj == NULL) {
                b_foundActivatorObj = FALSE;
            }
        } else {
            switch (setup->activatorObjType) {
            case 0:
                activatorObj = player;
                if (player == NULL) {
                    b_foundActivatorObj = FALSE;
                }
                break;
            case 1:
                activatorObj = sidekick;
                if (sidekick == NULL) {
                    b_foundActivatorObj = FALSE;
                }
                break;
            case 2:
                activatorObj = gDLL_2_Camera->vtbl->func2();
                break;
            }
        }
        
        if (b_foundActivatorObj) {
            if (objdata->flags & TRG_FIRST_TICK) {
                switch (setup->activatorObjType) {
                default:
                    objdata->activatorPrevPos.x = activatorObj->positionMirror2.x;
                    objdata->activatorPrevPos.y = activatorObj->positionMirror2.y;
                    objdata->activatorPrevPos.z = activatorObj->positionMirror2.z;
                    break;
                case 2:
                    // Camera
                    objdata->activatorPrevPos.x = activatorObj->srt.transl.x;
                    objdata->activatorPrevPos.y = activatorObj->srt.transl.y;
                    objdata->activatorPrevPos.z = activatorObj->srt.transl.z;
                    break;
                case 0:
                case 1:
                    // Player/sidekick
                    objdata->activatorPrevPos.x = activatorObj->positionMirror3.x;
                    objdata->activatorPrevPos.y = activatorObj->positionMirror3.y;
                    objdata->activatorPrevPos.z = activatorObj->positionMirror3.z;
                    break;
                }
                
                objdata->flags &= ~TRG_FIRST_TICK;
            } else {
                objdata->activatorPrevPos.x = objdata->activatorCurrPos.x;
                objdata->activatorPrevPos.y = objdata->activatorCurrPos.y;
                objdata->activatorPrevPos.z = objdata->activatorCurrPos.z;
            }
            
            switch (setup->activatorObjType) {
            case 0:
            case 1:
                // Player/sidekick
                objdata->activatorCurrPos.x = activatorObj->positionMirror.x;
                objdata->activatorCurrPos.y = activatorObj->positionMirror.y;
                objdata->activatorCurrPos.z = activatorObj->positionMirror.z;
                break;
            default:
            case 2:
                // Camera/other
                objdata->activatorCurrPos.x = activatorObj->srt.transl.x;
                objdata->activatorCurrPos.y = activatorObj->srt.transl.y;
                objdata->activatorCurrPos.z = activatorObj->srt.transl.z;
                break;
            }
        }
        
        switch (setup->base.objId) {
        case OBJ_TriggerPoint:
            if (b_foundActivatorObj) {
                trigger_point_update(self, activatorObj);
            }
            break;
        case OBJ_TriggerCylinder:
            if (b_foundActivatorObj) {
                trigger_cylinder_update(self, activatorObj);
            }
            break;
        case OBJ_TriggerPlane:
            b_allBitsSet = TRUE;
            if (objdata->conditionBitFlagIDs[0] >= 0) {
                if (main_get_bits(objdata->conditionBitFlagIDs[0]) == 0) {
                    b_allBitsSet = FALSE;
                }
            }
            if (b_allBitsSet && b_foundActivatorObj) {
                trigger_plane_update(self, activatorObj);
            }
            break;
        case OBJ_TriggerTime:
            objdata->elapsedTicks += delayByte;
            if (objdata->elapsedTicks >= setup->timerDuration) {
                trigger_process_commands(self, NULL, 1, 0);
            }
            break;
        case OBJ_TriggerArea:
            if (b_foundActivatorObj) {
                trigger_area_update(self, activatorObj);
            }
            break;
        case OBJ_TriggerSetup:
            trigger_process_commands(self, player, 1, 0);
            if (ret1_8001454c() != 0) {
                obj_destroy_object(self);
            }
            break;
        case OBJ_TriggerBits:
            b_allBitsSet = TRUE;
            for (i = 0; i < 4 && b_allBitsSet; i++) {
                if (objdata->conditionBitFlagIDs[i] >= 0) {
                    if (main_get_bits(objdata->conditionBitFlagIDs[i]) == 0) {
                        b_allBitsSet = FALSE;
                    }
                }
            }
            if (b_allBitsSet) {
                trigger_process_commands(self, player, 1, 0);
            }
            break;
        case OBJ_TriggerCurve:
            if (b_foundActivatorObj) {
                trigger_curve_update(self, activatorObj);
            }
            break;
        }
    }
}
#endif

void trigger_update(Object *self) { }

void trigger_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

void trigger_free(Object *self, s32 param2) {
    Trigger_Setup *setup;
    Trigger_Data *objdata;
    u8 i;

    setup = (Trigger_Setup*)self->setup;
    objdata = (Trigger_Data*)self->data;

    for (i = 0; i < 8; i++) {
        if (objdata->soundHandles[i] != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandles[i]);
        }
        if (objdata->scripts[i] != NULL) {
            dll_unload(objdata->scripts[i]);
        }

        objdata->soundHandles[i] = 0;
        objdata->scripts[i] = NULL;
    }

    switch (setup->base.objId) {
        case OBJ_TriggerPoint:
        case OBJ_TriggerButton: // ??? is this a bug?
            if (sPointModel != NULL) {
                sPointModelRefCount -= 1;
                if (sPointModelRefCount == 0) {
                    destroy_model_instance(sPointModel);
                    sPointModel = NULL;
                }
            }
            break;
        case OBJ_TriggerPlane:
            if (sPlaneModel != NULL) {
                sPlaneModelRefCount -= 1;
                if (sPlaneModelRefCount == 0) {
                    destroy_model_instance(sPlaneModel);
                    sPlaneModel = NULL;
                }
            }
            break;
    }

    obj_free_object_type(self, OBJTYPE_2);
}

u32 trigger_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

u32 trigger_get_data_size(Object *self, u32 param2) {
    return sizeof(Trigger_Data);
}

// godspeed https://decomp.me/scratch/Imc8a
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_process_commands.s")
#else
// dir represents the positional status of the activator relative to the trigger:
// 2  = in
// 1  = entered
// 0  = n/a
// -1 = exited
// -2 = out
void trigger_process_commands(Object *self, Object *activator, s8 dir, s32 activatorDistSquared) {
    Trigger_Setup* setup; // sp+74
    Trigger_Data* objdata; // sp+70
    //Object* player;
    s32 pad;
    TriggerCommand *cmd;
    s32 temp_a1;
    u8 i;
    Object* var_v0_2;
    Object* sidekick;
    s32 pad2;

    objdata = (Trigger_Data*)self->data;
    setup = (Trigger_Setup*)self->setup;
    
    for (i = 0, cmd = setup->commands; i < 8; i++, cmd++) {
        if (cmd->id == 0) {
            // No command assigned to this slot
            continue;
        }

        if ((objdata->flags & TRG_RESTORE_ENTERED_STATE) && !(cmd->condition & CMD_COND_RESTORE)) {
            continue;
        }
        
        if (!(cmd->condition & CMD_COND_CONTINUOUS)) {
            // Not continuous, check for entry/exit tick
            if (dir == 1) {
                // Entered
                if (!(cmd->condition & CMD_COND_IN)) {
                    continue;
                }
                if ((objdata->flags & TRG_ACTIVATOR_ENTERED) && !(cmd->condition & CMD_COND_RE_ENTER)) {
                    continue;
                }
            } else if (dir == -1) {
                // Exited
                if (!(cmd->condition & CMD_COND_OUT)) {
                    continue;
                }
                if ((objdata->flags & TRG_ACTIVATOR_EXITED) && !(cmd->condition & CMD_COND_RE_EXIT)) {
                    continue;
                }
            } else {
                continue;
            }
        } else {
            // Continuous, check if in/out
            if (cmd->condition & CMD_COND_IN) {
                if (dir < 0) {
                    continue;
                }
            } else if (cmd->condition & CMD_COND_OUT) {
                if (dir > 0) {
                    continue;
                }
            }
        }
        
        switch (cmd->id) {                  /* switch 1 */
        case TRG_CMD_HAZARD: 
            // "Trigger [%d], Gamplay Vulnerable"
            switch (cmd->param1) {              /* switch 2 */
            case 0:                         /* switch 2 */
            case 1:                         /* switch 2 */
            case 2:                         /* switch 2 */
            case 3:                         /* switch 2 */
            case 4:                         /* switch 2 */
            case 5:                         /* switch 2 */
            case 6:                         /* switch 2 */
                break;
            case 7: {                         /* switch 2 */
                    s32 mesgID; // sp+50
                    if (dir == 1) {
                        mesgID = 0x80;
                    } else {
                        mesgID = 0x81;
                    }
                    obj_send_mesg_many_nearby(OBJ_Swoop,           6000.0f, 0, self, mesgID, 0);
                    obj_send_mesg_many_nearby(OBJ_GP_ChimneySwipe, 6000.0f, 0, self, mesgID, 0);
                }
                break;
            case 8: {                        /* switch 2 */
                    Object *player = get_player();
                    if (player != NULL) {
                        ((DLL_210_Player*)player->dll)->vtbl->func67(player, 9, 0.0f);
                    }
                }
                break;
            case 9: {                        /* switch 2 */
                    Object *player = get_player();
                    if (player != NULL) {
                        ((DLL_210_Player*)player->dll)->vtbl->func67(player, 10, 0.0f);
                    }
                }
                break;
            case 10: {                       /* switch 2 */
                    Object *player = get_player();
                    if (player != NULL) {
                        ((DLL_210_Player*)player->dll)->vtbl->func67(player, 11, 0.0f);
                    }
                }
                break;
            }
            break;
        case TRG_CMD_MUSIC_ACTION: 
            if ((dir < 0) && (gDLL_5_AMSEQ->vtbl->func2(self, (cmd->param2 | (cmd->param1 << 8))) != 0)) {
                // "Trigger [%d], Music Action,       Action Num [%d] Free"
                gDLL_5_AMSEQ2->vtbl->func1(self, (cmd->param2 | (cmd->param1 << 8)), 0, 0, 0);
            } else {
                // "Trigger [%d], Music Action,       Action Num [%d] Set"
                gDLL_5_AMSEQ2->vtbl->func0(self, (cmd->param2 | (cmd->param1 << 8)), 0, 0, 0);
            }
            break;
        case TRG_CMD_SOUND: 
            // "Trigger [%d], Sound FX,           Action Num [%d],Handle Num [%d]"
            if (dir >= 0) {
                gDLL_6_AMSFX->vtbl->func_10D0(self, (cmd->param2 | (cmd->param1 << 8)), &objdata->soundHandles[i]);
            } else {
                if (objdata->soundHandles[i] != 0) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandles[i]);
                    objdata->soundHandles[i] = 0;
                }
            }
            break;
        case TRG_CMD_CAMERA_ACTION: 
            // "Trigger [%d], Camera,             Action [%d], Camera Num [%d], PassDir [%d]"
            gDLL_2_Camera->vtbl->func8(cmd->param1, cmd->param2);
            break;
        case TRG_CMD_TRACK: 
            // "Trigger [%d], Track Sky On"
            // "Trigger [%d], Track Sky Off"
            // "Trigger [%d], Track AntiAlias On"
            // "Trigger [%d], Track AntiAlias Off"
            // "Trigger [%d], Track SkyObjects On"
            // "Trigger [%d], Track SkyObjects Off"
            // "Trigger [%d], Track Dome On"
            // "Trigger [%d], Track Dome Off"
            // "Trigger [%d], Track MrSheen On %d"
            // "Trigger [%d], Track MrSheen Off"
            switch (cmd->param1) {              /* switch 3 */
            case 0:                         /* switch 3 */
                if ((s32) cmd->param2 >= 2) {
                    cmd->param2 = 1;
                }
                func_80041C6C(cmd->param2);
                if (cmd->param2 != 0) {

                }
                break;
            case 1:                         /* switch 3 */
                if ((s32) cmd->param2 >= 2) {
                    cmd->param2 = 1;
                }
                func_80041CA8((s32) cmd->param2);
                if (cmd->param2 != 0) {

                }
                break;
            case 2:                         /* switch 3 */
                if ((s32) cmd->param2 >= 2) {
                    cmd->param2 = 1;
                }
                func_80041CE4((s32) cmd->param2);
                if (cmd->param2 != 0) {

                }
                break;
            case 3:                         /* switch 3 */
                if ((s32) cmd->param2 >= 2) {
                    cmd->param2 = 1;
                }
                gDLL_12_Minic->vtbl->func6(cmd->param2);
                if (cmd->param2 != 0) {

                }
                break;
            case 4:                         /* switch 3 */
                gDLL_16->vtbl->func2(cmd->param2);
                if (cmd->param2 != 0) {

                }
                break;
            case 5:                         /* switch 3 */
                func_8005CA5C((u32) cmd->param2);
                break;
            case 6:                         /* switch 3 */
                if ((s32) cmd->param2 > 0) {
                    func_8001EBD0(1);
                } else {
                    func_8001EBD0(0);
                }
                break;
            case 7:                         /* switch 3 */
                if ((s32) cmd->param2 > 0) {
                    func_80041E24(1);
                } else {
                    func_80041E24(0);
                }
                break;
            }
            break;
        case TRG_CMD_5: 
            if ((objdata->radiusSquared != 0.0f) && (activatorDistSquared != 0) && (setup->base.objId == OBJ_TriggerPlane)) {

            }
            break;
        case TRG_CMD_ENV_FX:
            // "Trigger [%d], Environment Effect, Action Num [%d], Range [%d]"
            func_80000860(self, activator, (cmd->param2 | (cmd->param1 << 8)), activatorDistSquared);
            break;
        case TRG_CMD_LIGHTING:
            // "Trigger [%d], Lighting,           Action      [%d], Range [%d], PassDir [%d]"
            func_80000450(self, activator, (cmd->param2 | (cmd->param1 << 8)), dir, activatorDistSquared, 0);
            break;
        case TRG_CMD_ANIM_SEQ:
            switch (cmd->param1) {
                case 0:
                case 3:
                    // "Trigger [%d], Anim Sequence,      SequenceID [%d], Activate"
                    gDLL_3_Animation->vtbl->func1(cmd->param2, 0); // should be 3 params?
                    break;
                case 1:
                    // "Trigger [%d], Anim Sequence,      SequenceID [%d], Flag = 1"
                    gDLL_3_Animation->vtbl->func2(cmd->param2, 1);
                    break;
                case 2:
                    // "Trigger [%d], Anim Sequence,      SequenceID [%d], Flag = 0"
                    gDLL_3_Animation->vtbl->func2(cmd->param2, 0);
                    break;
            }
            break;
        case TRG_CMD_TRIGGER:
            // "Trigger [%d], Trigger,            Local ID   [%d]"
            trigger_func_29C0((cmd->param2 | (cmd->param1 << 8)), activator, dir, activatorDistSquared);
            break;
        // case TRG_CMD_?
            // "Storyboard disabled, please remove trigger\n"
        case TRG_CMD_LOD_MODEL:
            // "Trigger [%d], LOD Model [%d]"
            func_80023A18(get_player(), (s32) cmd->param1);
            break;
        case TRG_CMD_F:
            // "Trigger [%d], Setup Point,        Level      [%d], SetupPoint [%d]"
            // ?
            trigger_func_1754(cmd->param1, cmd->param2);
            break;
        case TRG_CMD_FLAG:
            // "Trigger [%d], Bits\n"
            trigger_func_1764((cmd->param2 | (cmd->param1 << 8)));
            break;
        case TRG_CMD_FLAG_TOGGLE:
            trigger_func_17FC((cmd->param2 | (cmd->param1 << 8)));
            break;
        case TRG_CMD_ENABLE_OBJ_GROUP:
            // "Trigger [%d], Object Load\n"
            gDLL_29_Gplay->vtbl->set_obj_group_status((s32) self->mapID, cmd->param2 | (cmd->param1 << 8), 1);
            break;
        case TRG_CMD_DISABLE_OBJ_GROUP:
            // "Trigger [%d], Object Free\n"
            gDLL_29_Gplay->vtbl->set_obj_group_status((s32) self->mapID, cmd->param2 | (cmd->param1 << 8), 0);
            break;
        case TRG_CMD_TOGGLE_OBJ_GROUP:
            // "Trigger [%d], Object Toggle\n"
            temp_a1 = (cmd->param2 | (cmd->param1 << 8));
            gDLL_29_Gplay->vtbl->set_obj_group_status((s32) self->mapID, temp_a1, gDLL_29_Gplay->vtbl->get_obj_group_status((s32) self->mapID, temp_a1) ^ 1);
            break;
        case TRG_CMD_TEXTURE_LOAD:
            // "Trigger [%d], Tex Load\n"
            trigger_func_1868((cmd->param2 | (cmd->param1 << 8)));
            break;
        case TRG_CMD_TEXTURE_FREE:
            // "Trigger [%d], Tex Free\n"
            trigger_func_1920((cmd->param2 | (cmd->param1 << 8)));
            break;
        case TRG_CMD_SET_MAP_SETUP:
            gDLL_29_Gplay->vtbl->set_map_setup((s32) self->mapID, cmd->param2 | (cmd->param1 << 8));
            break;
        case TRG_CMD_SCRIPT:
            // "TRIGGER: warning DLL not loaded\n"
            // "Script [%d], Subscript [%d]\n"
            if (objdata->scripts[i] != NULL) {
                objdata->scripts[i]->vtbl->subscripts[cmd->param2](self, activator, dir, activatorDistSquared);
            }
            break;
        case TRG_CMD_WORLD_ENABLE_OBJ_GROUP:
            // "Trigger [%d], Object Load\n"
            gDLL_29_Gplay->vtbl->set_obj_group_status((s32) cmd->param2, (s32) cmd->param1, 1);
            break;
        case TRG_CMD_WORLD_DISABLE_OBJ_GROUP:
            // "Trigger [%d], Object Free\n"
            gDLL_29_Gplay->vtbl->set_obj_group_status((s32) cmd->param2, (s32) cmd->param1, 0);
            break;
        case TRG_CMD_KYTE_FLIGHT_GROUP:
            main_set_bits(BIT_Kyte_Flight_Curve, cmd->param2 | (cmd->param1 << 8));
            break;
        case TRG_CMD_KYTE_TALK_SEQ:
            main_set_bits(BIT_488, cmd->param2 | (cmd->param1 << 8));
            break;
        case TRG_CMD_WORLD_SET_MAP_SETUP:
            gDLL_29_Gplay->vtbl->set_map_setup((s32) cmd->param2, (s32) cmd->param1);
            break;
        case TRG_CMD_11:
            // Tricky related?
            main_set_bits(BIT_4E2, cmd->param2 | (cmd->param1 << 8));
            break;
        case TRG_CMD_SAVE_GAME:
            gDLL_29_Gplay->vtbl->checkpoint(&self->srt.transl, (s16) ((s16) self->srt.yaw >> 8), (s32) cmd->param2, func_80048498());
            break;
        case TRG_CMD_MAP_LAYER:
            if (cmd->param1 == 0) {
                func_80047374();
            } else {
                func_800473BC();
            }
            break;
        case TRG_CMD_RESTART:
            switch (cmd->param1) {            /* switch 4; irregular */
            case 0:                         /* switch 4 */
                // "Restart Set [%d]\n"
                gDLL_29_Gplay->vtbl->restart_set(&self->srt.transl, self->srt.yaw, func_80048498());
                break;
            case 1:                         /* switch 4 */
                // "Restart Clear [%d]\n"
                gDLL_29_Gplay->vtbl->restart_clear();
                break;
            case 2:                         /* switch 4 */
                // "Restart Goto [%d]\n"
                gDLL_29_Gplay->vtbl->restart_goto();
                break;
            }
            break;
        case TRG_CMD_SIDEKICK:
            sidekick = get_sidekick();
            if (sidekick != NULL) {
                switch (cmd->param1) {       /* switch 5; irregular */
                case 0:                     /* switch 5 */
                    ((DLL_ISidekick *)sidekick->dll)->vtbl->func23(sidekick);
                    break;
                case 1:                     /* switch 5 */
                    // "killing sidekick\n"
                    obj_destroy_object(get_sidekick());
                    break;
                case 2:                     /* switch 5 */
                    // "findobj %i \n"
                    var_v0_2 = obj_get_nearest_type_to(0x34, sidekick, NULL);
                    if (var_v0_2 == NULL) {
                        var_v0_2 = obj_get_nearest_type_to(0x33, sidekick, NULL);
                    }
                    if (var_v0_2 != NULL) {
                        ((DLL_ISidekick *)sidekick->dll)->vtbl->func22(sidekick, var_v0_2);
                    }
                    break;
                }
            }
            break;
        case TRG_CMD_WATER_FALLS_FLAGS:
        case TRG_CMD_WATER_FALLS_FLAGS2:
            // "Trigger [%d], amSfxWaterFallsSetFlags,   Action [%d], PassDir [%d]"
            gDLL_6_AMSFX->vtbl->water_falls_set_flags(cmd->param1);
            break;
        }
    }

    if (dir > 0) {
        // In
        objdata->flags |= TRG_ACTIVATOR_ENTERED;
        main_set_bits(objdata->bitFlagID, 1);
    } else if (dir < 0) {
        // Out
        objdata->flags |= TRG_ACTIVATOR_EXITED;
    }
}
#endif

void trigger_func_1754(u8 param1, u8 param2) { }

void trigger_func_1764(u16 param1) {
    s32 _stack_pad[2];
    s32 entry;
    u32 value;

    entry = param1 & 0x3FFF;
    param1 >>= 14;
    value = main_get_bits(entry);

    if (param1 == 0) {
        value = 0;
    } else if (param1 == 1) {
        value = -1;
    } else if (param1 == 2) {
        value = ~value;
    }

    main_set_bits(entry, value);
}

void trigger_func_17FC(u16 param1) {
    s32 _stack_pad[2];
    s32 entry;
    u32 value;

    entry = param1 & 0x1FFF;
    param1 >>= 13;

    value = main_get_bits(entry);
    value ^= (1 << param1);
    main_set_bits(entry, value);
}

void trigger_func_1868(u16 param1) {
    s32 *ptr;
    s32 *ptr2;
    Texture *tex;

    ptr = func_800213A0(param1 + 2);
    if (ptr != NULL) {
        for (ptr2 = ptr; *ptr2 != -1; ptr2++) {
            tex = func_8003E960(*ptr2);
            if (tex == NULL) {
                func_80012584(50, 3, NULL, (ObjSetup*)*ptr2, 0, 0, 0, 0);
            }
        }
    }
}

void trigger_func_1920(u16 param1) {
    s32 *ptr;
    s32 *ptr2;
    Texture *tex;

    ptr = func_800213A0(param1 + 2);
    if (ptr != NULL) {
        for (ptr2 = ptr; *ptr2 != -1; ptr2++) {
            tex = func_8003E960(*ptr2);
            if (tex != NULL) {
                texture_destroy(tex);
            }
        }
    }
}

static void trigger_point_setup(Object *self, Trigger_Setup *setup) {
    Trigger_Data *objdata;
    ModelInstance *modelInstance;
    Model *model;
    Vtx *vertex;
    u8 _stack_pad[4];
    f32 x,y,z;
    f32 radius;
    f32 modelRadius;

    if (sPointModel == NULL) {
        sPointModel = func_80017D2C(85, 0);
        sPointModelRefCount = 1;
    } else {
        sPointModelRefCount += 1;
    }

    objdata = (Trigger_Data*)self->data;
    radius = setup->sizeX << 1;
    objdata->radiusSquared = radius * radius;

    self->srt.roll = 0;
    self->srt.pitch = 0;
    self->srt.yaw = setup->rotationY << 8;

    modelInstance = sPointModel;
    model = modelInstance->model;
    vertex = &model->vertices[1];
    x = (f32)vertex->v.ob[0];
    y = (f32)vertex->v.ob[1];
    z = (f32)vertex->v.ob[2];
    modelRadius = sqrtf((x * x) + (y * y) + (z * z));
    
    self->srt.scale = radius / modelRadius;
}

// needs trigger_process_commands to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_point_update.s")
#else
void trigger_point_update(Object *self, Object *activator) {
    Trigger_Setup *setup;
    Trigger_Data *objdata;
    f32 prevDist;
    f32 currDist;
    f32 diffX;
    f32 diffY;
    f32 diffZ;
    s32 dir;

    objdata = (Trigger_Data*)self->data;
    setup = (Trigger_Setup*)self->setup;

    diffX = objdata->activatorPrevPos.x - self->positionMirror.x;
    diffY = objdata->activatorPrevPos.y - self->positionMirror.y;
    diffZ = objdata->activatorPrevPos.z - self->positionMirror.z;
    prevDist = diffX * diffX + diffY * diffY + diffZ * diffZ;

    if (setup->localID > 0) {
        currDist = gDLL_26_Curves->vtbl->curves_func_14f4(7, setup->localID,
            objdata->activatorCurrPos.x, objdata->activatorCurrPos.y, objdata->activatorCurrPos.z,
            &self->srt.transl.x, &self->srt.transl.y, &self->srt.transl.z);
        
            if (currDist < 0.0f) {
                currDist = -currDist;
            }

            if (self->parent != NULL) {
                inverse_transform_point_by_object(self->positionMirror.x, self->positionMirror.y, self->positionMirror.z,
                    &self->srt.transl.x, &self->srt.transl.y, &self->srt.transl.z, 
                    self->parent);
            }
    } else {
        diffX = objdata->activatorCurrPos.x - self->positionMirror.x;
        diffY = objdata->activatorCurrPos.y - self->positionMirror.y;
        diffZ = objdata->activatorCurrPos.z - self->positionMirror.z;
        currDist = diffX * diffX + diffY * diffY + diffZ * diffZ;
    }

    if (currDist < objdata->radiusSquared) {
        // in
        if (prevDist < objdata->radiusSquared) {
            dir = 2; // in
        } else {
            dir = 1; // entered
        }
    } else {
        // out
        if (prevDist < objdata->radiusSquared) {
            dir = -1; // exited
        } else {
            dir = -2; // out
        }
    }

    trigger_process_commands(self, activator, dir, currDist);
}
#endif

static void trigger_cylinder_setup(Object *self, Trigger_Setup *setup) {
    Trigger_Data *objdata;

    objdata = (Trigger_Data*)self->data;
    objdata->radiusSquared = setup->sizeX << 1;
    objdata->radiusSquared *= objdata->radiusSquared;
}

// needs trigger_process_commands to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_cylinder_update.s")
#else
void trigger_cylinder_update(Object* self, Object* activator) {
    Trigger_Data* objdata;
    Trigger_Setup *setup;
    f32 lengthSquared;
    f32 lengthSquared2;
    f32 diffX;
    f32 diffY2;
    f32 diffY;
    f32 diffZ;
    f32 unk3BTimes2;
    s32 dir;

    objdata = self->data;
    setup = (Trigger_Setup*)self->setup;
    unk3BTimes2 = setup->sizeY << 1;
    
    diffX = objdata->activatorPrevPos.x - self->positionMirror.x;
    diffY = objdata->activatorPrevPos.y - self->positionMirror.y;
    diffZ = objdata->activatorPrevPos.z - self->positionMirror.z;
    
    lengthSquared = (diffX * diffX) + (diffZ * diffZ);

    diffX = objdata->activatorCurrPos.x - self->positionMirror.x;
    diffY2 = objdata->activatorCurrPos.y - self->positionMirror.y;
    diffZ = objdata->activatorCurrPos.z - self->positionMirror.z;
    
    lengthSquared2 = (diffX * diffX) + (diffZ * diffZ);
    
    if (lengthSquared2 == lengthSquared) {
        if (((diffY == diffY2) && (!objdata)) && (!objdata)) {} // partial fakematch
    }

    if (lengthSquared2 < objdata->radiusSquared && (diffY2 < 0.0f ? -diffY2 : diffY2) < unk3BTimes2) {
        if (lengthSquared < objdata->radiusSquared && (diffY < 0.0f ? -diffY : diffY) < unk3BTimes2) {
            dir = 2;
        } else {
            dir = 1;
        }
    } else {
        if (lengthSquared < objdata->radiusSquared && (diffY < 0.0f ? -diffY : diffY) < unk3BTimes2) {
            dir = -1;
        } else {
            dir = -2;
        }
    }
    trigger_process_commands(self, activator, dir, lengthSquared2);
}
#endif

static void trigger_plane_setup(Object *self, Trigger_Setup *setup) {
    Trigger_Data *objdata;
    ModelInstance *modelInstance;
    Model *model;
    Vtx *vertex;
    SRT srt; // sp+a8
    MtxF mtx; // sp+68
    f32 ox; // sp+64
    f32 oy; // sp+60
    f32 oz; // sp+5c
    f32 temp;
    s32 i;
    f32 minX; // sp+50
    f32 minZ; // sp+4c
    f32 minY; // sp+48
    f32 maxY; // sp+44
    f32 maxX; // sp+40
    f32 maxZ; // sp+3c
    f32 swapTemp;
    
    if (sPlaneModel == NULL) {
        sPlaneModel = func_80017D2C(78, 0);
        sPlaneModelRefCount = 1;
    } else {
        sPlaneModelRefCount += 1;
    }

    objdata = (Trigger_Data*)self->data;

    self->srt.yaw = setup->rotationY << 8 << 2;
    self->srt.pitch = setup->rotationX << 8;
    temp = setup->sizeX * 0.0625f;
    self->srt.scale = self->def->scale * temp;

    srt.yaw = self->srt.yaw;
    srt.pitch = self->srt.pitch;
    srt.roll = self->srt.roll;
    srt.transl.x = 0.0f;
    srt.transl.y = 0.0f;
    srt.transl.z = 0.0f;
    srt.scale = 1.0f;
    matrix_from_srt(&mtx, &srt);

    vec3_transform(&mtx, 0.0f, 0.0f, 1.0f, &ox, &oy, &oz);
    objdata->lookVector.x = ox;
    objdata->lookVector.y = oy;
    objdata->lookVector.z = oz;
    objdata->lookVectorNegDot = -((self->positionMirror.x * ox) + (self->positionMirror.y * oy) + (self->positionMirror.z * oz));

    modelInstance = sPlaneModel;
    model = modelInstance->model;
    vertex = &model->vertices[1];
    minX = maxX = (f32)vertex->v.ob[0];
    minY = maxY = (f32)vertex->v.ob[1];
    minZ = maxZ = (f32)vertex->v.ob[2];

    for (i = 1; i < model->vertexCount; i++, vertex++) {
        if (vertex->v.ob[0] < minX) {
            minX = vertex->v.ob[0];
        }
        if (vertex->v.ob[0] > maxX) {
            maxX = vertex->v.ob[0];
        }

        if (vertex->v.ob[1] < minY) {
            minY = vertex->v.ob[1];
        }
        if (vertex->v.ob[1] > maxY) {
            maxY = vertex->v.ob[1];
        }

        if (vertex->v.ob[2] < minZ) {
            minZ = vertex->v.ob[2];
        }
        if (vertex->v.ob[2] > maxZ) {
            maxZ = vertex->v.ob[2];
        }
    }

    vec3_transform(&mtx, minX, minY, minZ, &minX, &minY, &minZ);
    vec3_transform(&mtx, maxX, maxY, maxZ, &maxX, &maxY, &maxZ);

    if (maxX < minX) {
        swapTemp = minX;
        minX = maxX;
        maxX = swapTemp;
    }
    if (maxY < minY) {
        swapTemp = maxY;
        maxY = minY;
        minY = swapTemp;
    }
    if (maxZ < minZ) {
        swapTemp = maxZ;
        maxZ = minZ;
        minZ = swapTemp;
    }

    objdata->planeMin.x = self->positionMirror.x + self->srt.scale * minX;
    objdata->planeMin.y = self->positionMirror.y + self->srt.scale * minY;
    objdata->planeMin.z = self->positionMirror.z + self->srt.scale * minZ;
    objdata->planeMax.x = self->positionMirror.x + self->srt.scale * maxX;
    objdata->planeMax.y = self->positionMirror.y + self->srt.scale * maxY;
    objdata->planeMax.z = self->positionMirror.z + self->srt.scale * maxZ;
    objdata->radiusSquared = (self->srt.scale * 145.0f) * (self->srt.scale * 145.0f);
}

// needs trigger_process_commands to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_plane_update.s")
#else
void trigger_plane_update(Object *self, Object *activator) {
    Trigger_Data *objdata;
    f32 prevDist;
    f32 currDist;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f12_2;
    f32 temp_f14_2;
    f32 temp_f16_2;
    f32 temp_f18_2;
    f32 temp_f2;
    s32 dir;

    objdata = (Trigger_Data*)self->data;
    
    prevDist = (objdata->lookVector.x * objdata->activatorPrevPos.x) + (objdata->lookVector.y * objdata->activatorPrevPos.y) + (objdata->lookVector.z * objdata->activatorPrevPos.z) + objdata->lookVectorNegDot;
    currDist = (objdata->lookVector.x * objdata->activatorCurrPos.x) + (objdata->lookVector.y * objdata->activatorCurrPos.y) + (objdata->lookVector.z * objdata->activatorCurrPos.z) + objdata->lookVectorNegDot;
    
    if (currDist < 0.0f) {
        if (prevDist < 0.0f) {
            dir = 2;
        } else {
            dir = 1;
        }
    } else {
        if (prevDist < 0.0f) {
            dir = -1;
        } else {
            dir = -2;
        }
    }
    if (dir == 1 || dir == -1) {
        temp_f0_2 = objdata->activatorCurrPos.x - objdata->activatorPrevPos.x;
        temp_f12_2 = objdata->activatorCurrPos.y - objdata->activatorPrevPos.y;
        temp_f14_2 = objdata->activatorCurrPos.z - objdata->activatorPrevPos.z;
        
        temp_f2 = ((((-objdata->lookVector.x * objdata->activatorPrevPos.x) - (objdata->lookVector.y * objdata->activatorPrevPos.y)) - (objdata->lookVector.z * objdata->activatorPrevPos.z)) - objdata->lookVectorNegDot) / 
            ((objdata->lookVector.x * temp_f0_2) + (objdata->lookVector.y * temp_f12_2) + (objdata->lookVector.z * temp_f14_2));
        
        temp_f16_2 = objdata->activatorPrevPos.x + (temp_f2 * temp_f0_2);
        temp_f0_3 = objdata->activatorPrevPos.y + (temp_f2 * temp_f12_2);
        temp_f18_2 = objdata->activatorPrevPos.z + (temp_f2 * temp_f14_2);
        
        if ((objdata->planeMin.x <= temp_f16_2) && (temp_f16_2 <= objdata->planeMax.x) && 
            (objdata->planeMin.y <= temp_f0_3) && (temp_f0_3 <= objdata->planeMax.y) && 
            (objdata->planeMin.z <= temp_f18_2) && (temp_f18_2 <= objdata->planeMax.z)) {
            trigger_process_commands(self, activator, dir, currDist);
        }
    }
}
#endif

static void trigger_area_setup(Object *self, Trigger_Setup *setup) {
    self->srt.yaw = setup->rotationY << 8;
    self->srt.pitch = setup->rotationX << 8;
    self->srt.roll = 0;
}

// needs trigger_process_commands to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_area_update.s")
#else
void trigger_area_update(Object *self, Object *activator) {
    Trigger_Data *objdata;
    s32 sp20;
    s32 temp_v0;

    objdata = (Trigger_Data*)self->data;
    
    sp20 = trigger_func_273C(self, &objdata->activatorCurrPos);
    temp_v0 = trigger_func_273C(self, &objdata->activatorPrevPos);
    
    if (sp20 != 0) {
        if (temp_v0 == 0) {
            trigger_process_commands(self, activator, 1, 0);
        } else {
            trigger_process_commands(self, activator, 2, 0);
        }
    } else {
        if (temp_v0 != 0) {
            trigger_process_commands(self, activator, -1, 0);
        } else {
            trigger_process_commands(self, activator, -2, 0);
        }
    }
}
#endif

s32 trigger_func_273C(Object *self, Vec3f *vec) {
    f32 x;
    f32 y;
    f32 z;
    Trigger_Setup *setup;

    setup = (Trigger_Setup*)self->setup;
    x = vec->x;
    y = vec->y;
    z = vec->z;
    
    trigger_func_2884(self, &x, &y, &z);
    
    if (x < 0.0f) {
        x = -x;
    }
    if (y < 0.0f) {
        y = -y;
    }
    if (z < 0.0f) {
        z = -z;
    }
    
    if (x <= (setup->sizeX * 2) && y <= (setup->sizeY * 2) && z <= (setup->sizeZ * 2)) {
        return 1;
    }
    
    return 0;
}

static void trigger_func_2884(Object *self, f32 *ox, f32 *oy, f32 *oz) {
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f16;
    f32 temp_f2;
    
    sp2C = fsin16_precise(-self->srt.yaw);
    sp28 = fcos16_precise(-self->srt.yaw);
    sp24 = fsin16_precise(-self->srt.pitch);
    temp_f0 = fcos16_precise(-self->srt.pitch);
    
    temp_f2 = (*ox) - self->positionMirror.x;
    temp_f16 = (*oy) - self->positionMirror.y;
    temp_f12 = (*oz) - self->positionMirror.z;
    
    *ox = (temp_f2 * sp28) - (temp_f12 * sp2C);
    temp_f12 = (temp_f2 * sp2C) + (temp_f12 * sp28);
    *oy = (temp_f16 * temp_f0) - (temp_f12 * sp24);
    *oz = (temp_f16 * sp24) + (temp_f12 * temp_f0);
}

// needs trigger_process_commands to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_func_29C0.s")
#else
void trigger_func_29C0(u16 localID, Object *activator, s8 dir, s32 activatorDistSquared) {
    Object** objects;
    Object* obj;
    Trigger_Setup* objsetup;
    s32 i;
    s32 numObjs;
    
    objects = get_world_objects(&i, &numObjs);
    while (i < numObjs) {
        obj = objects[i];
        objsetup = (Trigger_Setup*)obj->setup;
        
        if (objsetup != NULL) {
            switch (objsetup->base.objId) {
            case OBJ_TriggerPoint:
            case OBJ_TriggerPlane:
            case OBJ_TriggerArea:
            case OBJ_TriggerTime:
            case OBJ_TriggerButton:
            case OBJ_TriggerSetup:
            case OBJ_TriggerBits:
                if (0) { } // fakematch
            case OBJ_TriggerCylinder:
                if (objsetup->localID == localID) {
                    trigger_process_commands(obj, activator, dir, activatorDistSquared);
                }
                break;
            }
        }
        
        i++;
    }
}
#endif

static void trigger_curve_setup(Object *self, Trigger_Setup *setup) { }

// needs trigger_process_commands to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/325_trigger/trigger_curve_update.s")
#else
void trigger_curve_update(Object *self, Object *activator) {
    Trigger_Data* objdata;
    Trigger_Setup *setup;
    s32 sp3C;
    f32 dist;
    s32 sp34;
    s32 temp_v0;
    s32 temp_v0_2;

    sp34 = 0x17;
    objdata = (Trigger_Data*)self->data;
    setup = (Trigger_Setup*)self->setup;
    
    temp_v0 = gDLL_26_Curves->vtbl->curves_func_1e4(objdata->activatorCurrPos.x, objdata->activatorCurrPos.y, objdata->activatorCurrPos.z, &sp34, 1, setup->localID);
    sp3C = gDLL_26_Curves->vtbl->curves_func_291c(temp_v0, objdata->activatorCurrPos.x, objdata->activatorCurrPos.y, objdata->activatorCurrPos.z, &dist);
    temp_v0_2 = gDLL_26_Curves->vtbl->curves_func_291c(temp_v0, objdata->activatorPrevPos.x, objdata->activatorPrevPos.y, objdata->activatorPrevPos.z, &dist);
    
    if (sp3C != 0) {
        if (temp_v0_2 == 0) {
            trigger_process_commands(self, activator, 1, dist);
        } else {
            trigger_process_commands(self, activator, 2, dist);
        }
    } else {
        if (temp_v0_2 != 0) {
            trigger_process_commands(self, activator, -1, dist);
        } else {
            trigger_process_commands(self, activator, -2, dist);
        }
    }
}
#endif
