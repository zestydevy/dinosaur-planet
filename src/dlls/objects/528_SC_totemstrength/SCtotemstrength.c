#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "macros.h"
#include "sys/fonts.h"
#include "sys/map_enums.h"

#include "dlls/objects/210_player.h"
#include "dlls/objects/519_SC_levelcontrol.h"

#define ANGLE_TO_PIT 0x1DDC 
#define ANGULAR_RANGE (ANGLE_TO_PIT*2)
#define ANGULAR_RANGE_F ((f32)ANGULAR_RANGE)

#define YAW_NEUTRAL -0x2900
#define YAW_LOSE (YAW_NEUTRAL + ANGLE_TO_PIT)
#define YAW_WIN  (YAW_NEUTRAL - ANGLE_TO_PIT)

#define YAW_SPEED_MAX 40.0f
#define YAW_SPEED_DELTA 2.0f

typedef struct {
    ObjSetup base;
} SCTotemStrength_Setup;

typedef struct {
    Object* lightFoot;         //Muscle LightFoot
    f32 unk4;                  //Set to 0 during initial state, but otherwise unused
    f32 yawSpeed;              //Rate of change of rotation (lowered by A button presses)
    s32 yaw;                   //Overall rotation value, applied via a sequence function
    s32 pushSeq;               //Used to control the overall rotation of the sequence
    s16 unused14;
    s16 state;                 //State Machine value
    u32 soundHandleCreak;      //For the wood creaking sound loop
    u32 soundHandleLightFoot;  //Sound handle for the LightFoot's random sounds
    u8 flags;                  //Tracks whether strength game is active, whether contestants can push, and whether to reset
    Vec3f home;                //Initial position
    f32 soundTimerKrystal;     //Random delay between effort sounds (shorter interval when Krystal losing)
    f32 soundTimerLF;          //Random delay between effort sounds (shorter interval when Krystal winning)
} SCTotemStrength_Data;

typedef enum {
    SCTotemStrength_FLAG_None = 0,
    SCTotemStrength_FLAG_Pushing_Enabled = 1,
    SCTotemStrength_FLAG_Strength_Game_Active = 2,
    SCTotemStrength_FLAG_Reset = 4
} SCTotemStrength_Flags;

typedef enum {
    SCTotemStrength_STATE_Initial = 0,
    SCTotemStrength_STATE_Pushing = 1,
    SCTotemStrength_STATE_Won = 2,
    SCTotemStrength_STATE_Lost = 3
} SCTotemStrength_States;

typedef enum {
    SCTotemStrength_SEQCMD_1_Enable_Pushing = 1,
    SCTotemStrength_SEQCMD_2_Initialise_Strength_Game = 2,
    SCTotemStrength_SEQCMD_3_Set_Level_State_3 = 3,
    SCTotemStrength_SEQCMD_4_Set_Level_State_4 = 4,
    SCTotemStrength_SEQCMD_5_Refill_Player_Health_Magic = 5
} SCTotemStrength_SeqCommands;

/*0x0*/ static u16 dSoundsLightFoot[3] = {
    SOUND_90F_Muscle_LightFoot_Cry, 
    SOUND_910_Muscle_LightFoot_Scream, 
    SOUND_911_Muscle_LightFoot_Oh_No
};

/*0x8*/ static u16 dSoundsKrystal[3] = {
    SOUND_D9_Krystal_Hurt_Agh, 
    SOUND_E1_Krystal_Ugh, 
    SOUND_E7_Krystal_Hyeh
};

// offset: 0x0 | ctor
void SCTotemStrength_ctor(void *dll) { }

// offset: 0xC | dtor
void SCTotemStrength_dtor(void *dll) { }

static int SCTotemStrength_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 a3);
static void SCTotemStrength_set_level_state(Object* self, u8 value);

// offset: 0x18 | func: 0 | export: 0
void SCTotemStrength_setup(Object* self, SCTotemStrength_Setup* objSetup, s32 arg2) {
    SCTotemStrength_Data* objData;

    objData = self->data;
    self->animCallback = SCTotemStrength_anim_callback;
    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);
    self->srt.yaw = YAW_NEUTRAL;
    objData->yaw = YAW_NEUTRAL;
    objData->state = SCTotemStrength_STATE_Initial;
    objData->lightFoot = NULL;

    objData->home.x = self->srt.transl.x;
    objData->home.y = self->srt.transl.y;
    objData->home.z = self->srt.transl.z;

    main_set_bits(BIT_SCTotemStrength_Inactive, 1);
}

// offset: 0xB0 | func: 1 | export: 1
void SCTotemStrength_control(Object* self) {
    SCTotemStrength_Data* objData;

    objData = self->data;
    
    //Do nothing if SwapStone Circle isn't in Setup 6
    if ((gDLL_29_Gplay)->vtbl->get_map_setup(MAP_SWAPSTONE_CIRCLE) != 6) {
        return;
    }

    //Handle flag: reset
    //(Used when switching back to `SCTotemStrength_control` from `SCTotemStrength_anim_callback`) 
    if (objData->flags & SCTotemStrength_FLAG_Reset) {
        objData->flags &= ~SCTotemStrength_FLAG_Reset;
        self->srt.transl.x = objData->home.x;
        self->srt.transl.y = objData->home.y;
        self->srt.transl.z = objData->home.z;
        self->srt.yaw = YAW_NEUTRAL;
        objData->yaw = self->srt.yaw;
        objData->lightFoot = NULL;
        main_set_bits(BIT_SCTotemStrength_Inactive, 1);
    }
    
    //Handle LightFoot Strength Trial's states
    if (objData->flags & SCTotemStrength_FLAG_Strength_Game_Active) {
        if (objData->state == SCTotemStrength_STATE_Initial) {
            self->srt.yaw = YAW_NEUTRAL;
            objData->yaw = self->srt.yaw;
            objData->state = SCTotemStrength_STATE_Pushing;
            objData->flags &= ~SCTotemStrength_FLAG_Pushing_Enabled;
            objData->unk4 = 0.0f;
            objData->yawSpeed = 0.0f;
            return;
        }
        
        if (objData->state == SCTotemStrength_STATE_Pushing) {
            objData->pushSeq = gDLL_3_Animation->vtbl->func17(0, self, -1);
            return;
        }
        
        if (objData->state == SCTotemStrength_STATE_Won) {
            objData->flags &= ~(SCTotemStrength_FLAG_Pushing_Enabled | SCTotemStrength_FLAG_Strength_Game_Active);
            objData->state = SCTotemStrength_STATE_Initial;
            objData->yaw = YAW_WIN;
            gDLL_6_AMSFX->vtbl->play(self, SOUND_798_Puzzle_Solved, MAX_VOLUME, 0, 0, 0, 0);
            gDLL_3_Animation->vtbl->func28(objData->pushSeq, objData->yaw);
            main_set_bits(BIT_LightFoot_Strength_Game_Won, 1);
            gDLL_6_AMSFX->vtbl->stop(objData->soundHandleCreak);
            objData->soundHandleCreak = 0;
            return;
        }
        
        if (objData->state == SCTotemStrength_STATE_Lost) {
            objData->flags &= ~(SCTotemStrength_FLAG_Pushing_Enabled | SCTotemStrength_FLAG_Strength_Game_Active);
            objData->state = SCTotemStrength_STATE_Initial;
            objData->yaw = YAW_LOSE;
            gDLL_6_AMSFX->vtbl->play(self, SOUND_912_Object_Refused, MAX_VOLUME, 0, 0, 0, 0);
            gDLL_2_Camera->vtbl->change_mode(3, 0);
            gDLL_3_Animation->vtbl->func28(objData->pushSeq, objData->yaw);
            main_set_bits(BIT_LightFoot_Strength_Game_Lost, 1);
            gDLL_6_AMSFX->vtbl->stop(objData->soundHandleCreak);
            objData->soundHandleCreak = 0;
            return;
        }
    }
}

// offset: 0x38C | func: 2 | export: 2
void SCTotemStrength_update(Object *self) { }

// offset: 0x398 | func: 3 | export: 3
void SCTotemStrength_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    //@bug?: doesn't check visibility argument
    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
}

// offset: 0x3E0 | func: 4 | export: 4
void SCTotemStrength_free(Object* self, s32 arg1) {
    SCTotemStrength_Data* objData = self->data;

    if (objData->soundHandleCreak != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->soundHandleCreak);
        objData->soundHandleCreak = 0;
    }
}

// offset: 0x448 | func: 5 | export: 5
u32 SCTotemStrength_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x458 | func: 6 | export: 6
u32 SCTotemStrength_get_data_size(Object *self, u32 a1) {
    return sizeof(SCTotemStrength_Data);
}

// offset: 0x46C | func: 7
static int SCTotemStrength_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
    static s32 sPrevYaw;

    s32 i;
    f32 pushFactor;
    f32 pushAbs;
    Object** objects;
    s32 index;
    s32 count;
    SCTotemStrength_Data* objData;
    f32 pushProgress;
    Object* player;

    objData = self->data;
    player = get_player();
    objData->flags |= SCTotemStrength_FLAG_Reset;
    main_set_bits(BIT_SCTotemStrength_Inactive, 0);
    
    //Handle sequence commands
    for (i = 0; i < animData->unk98; i++) {
        switch (animData->unk8E[i]) {
        case SCTotemStrength_SEQCMD_1_Enable_Pushing:
            objData->flags |= SCTotemStrength_FLAG_Pushing_Enabled;
            break;
        case SCTotemStrength_SEQCMD_2_Initialise_Strength_Game:
            objData->state = SCTotemStrength_STATE_Initial;
            objData->flags |= SCTotemStrength_FLAG_Strength_Game_Active;

            objects = get_world_objects(&index, &count);
            while (index < count) {
                objData->lightFoot = objects[index++];
                if (objData->lightFoot->id == OBJ_SC_musclelightf) {
                    index = count;
                }
            }
            
            func_80023D30(player, 0x401, 0.0f, 0);
            func_80023D30(objData->lightFoot, 0, 1.0f, 0);
            gDLL_3_Animation->vtbl->func19(0x5A, 3, 0, 0);
            break;
        case SCTotemStrength_SEQCMD_3_Set_Level_State_3:
            STUBBED_PRINTF("Enable music change\n");
            SCTotemStrength_set_level_state(self, 3);
            break;
        case SCTotemStrength_SEQCMD_4_Set_Level_State_4:
            SCTotemStrength_set_level_state(self, 4);
            break;
        case SCTotemStrength_SEQCMD_5_Refill_Player_Health_Magic:
            ((DLL_210_Player*)player->dll)->vtbl->set_health(player, 0x7F);
            ((DLL_210_Player*)player->dll)->vtbl->set_magic(player, 0xFF);
            //break;
        default:
            break;
        }
    }
    
    //Return early if the player/LightFoot can't push
    if (!(objData->flags & SCTotemStrength_FLAG_Pushing_Enabled)) {
        return 0;
    }
    
    animData->unk7A = -1;
    animData->unk62 = 0;

    //Find the Muscle LightFoot (@bug: doesn't check if already found, searches every frame)
    objects = get_world_objects(&index, &count);
    while (index < count) {
        objData->lightFoot = objects[index++];
        if (objData->lightFoot->id == OBJ_SC_musclelightf) {
            index = count; //index break
        }
    }
    
    //Start playing wood creaking loop
    if (objData->soundHandleCreak == 0) {
        objData->soundHandleCreak = gDLL_6_AMSFX->vtbl->play(self, SOUND_776_Wooden_Creaking_Loop, MAX_VOLUME, 0, 0, 0, 0);
        gDLL_6_AMSFX->vtbl->set_pitch(objData->soundHandleCreak, 0.8f);
    }

    //Handle pushing behaviour, looping for each frame skipped
    //(@bug: becomes much more difficult to win at smoother framerates)
    for (i = 0; i < gUpdateRate; i++) {
        if (objData->lightFoot == NULL) {
            return 0;
        }
        
        //Get push tValue (as value from 0 to 1, i.e. from losing to winning)
        pushFactor = (objData->yaw - YAW_LOSE) / -ANGULAR_RANGE_F;

        //Get push progress (value from -1 to 1, losing to winning)
        pushProgress = (2.0f * pushFactor) + -1.0f;
        
        //Calculate LightFoot's push factor
        if (pushProgress < 0/*.0f*/) {
            pushAbs = -pushProgress;
        } else {
            pushAbs = pushProgress;
        }
        pushProgress = (pushFactor * 1.7f) + 0.2f;
        pushFactor = pushAbs * pushProgress + 1.0f;
        
        //Handle tapping A button
        if (joy_get_released_buffered(0, i) & A_BUTTON) {
            //@bug: harder at smoother FPS, since it won't decrease as much per press
            objData->yawSpeed -= 2.5f;
        }
        if (objData->yawSpeed < -YAW_SPEED_MAX) {
            objData->yawSpeed = -YAW_SPEED_MAX;
        }

        //Apply rotation speed
        if ((objData->yaw >= YAW_WIN) && (objData->yaw <= YAW_LOSE)) {
            objData->yaw += objData->yawSpeed;
        }

        //Recalculate push progress wrt. rate of change of yaw (value from -1 to 1)
        pushProgress = ((f32)sPrevYaw - (f32)objData->yaw) / YAW_SPEED_MAX;

        //Handle winning
        if (objData->yaw < YAW_WIN) {
            sPrevYaw = YAW_NEUTRAL;
            objData->state = SCTotemStrength_STATE_Won;
            func_80023D30(player, 0, 0.0f, 0);
            gDLL_3_Animation->vtbl->func18(objData->pushSeq);
            return 4;
        }
    
        //Rotate sequence using push yaw
        gDLL_3_Animation->vtbl->func28(objData->pushSeq, objData->yaw);

        //Handle losing
        if (objData->yaw > YAW_LOSE) {
            sPrevYaw = YAW_NEUTRAL;
            objData->state = SCTotemStrength_STATE_Lost;
            func_80023D30(player, 0, 0.0f, 0);
            gDLL_3_Animation->vtbl->func18(objData->pushSeq);
            return 4;
        }
        
        //Apply the LightFoot's push
        if (objData->yawSpeed < YAW_SPEED_MAX) {
            objData->yawSpeed += 0.2f * pushFactor;
        }
        
        //Handle player anim progress/looping
        if (func_80024108(player, ((f32)sPrevYaw - (f32)objData->yaw) / 9500.0f, gUpdateRateF, 0) != 0) {
            if ((((f32)sPrevYaw - (f32)objData->yaw) / 9500.0f) < 0.0f) {
                //Start at end of animation
                func_80023D30(player, 0x401, 1.0f, 0);
            } else {
                //Start at beginning of animation
                func_80023D30(player, 0x401, 0.0f, 0);
            }
        }
        
        //Handle LightFoot anim progress/looping
        if (func_80024108(objData->lightFoot, -(((f32)sPrevYaw - (f32)objData->yaw) / 9500.0f), gUpdateRateF, 0) != 0) {
            if (-(((f32)sPrevYaw - (f32)objData->yaw) / 9500.0f) < 0.0f) {
                //Start at end of animation
                func_80023D30(objData->lightFoot, 0, 1.0f, 0);
            } else {
                //Start at beginning of animation
                func_80023D30(objData->lightFoot, 0, 0.0f, 0);
            }
        }

        //Store previous yaw value
        sPrevYaw = objData->yaw;
    }

    //Play random Krystal sounds
    objData->soundTimerKrystal -= gUpdateRateF;
    if (objData->soundTimerKrystal < 0.0f) {
        if (pushProgress < 0.0f) {
            //More frequent when Krystal is losing
            objData->soundTimerKrystal = rand_next(40, 100);
        } else {
            //Less frequent when Krystal is winning
            objData->soundTimerKrystal = rand_next(120, 240);
        }
        gDLL_6_AMSFX->vtbl->play(self, dSoundsKrystal[rand_next(0, 2)], MAX_VOLUME, 0, 0, 0, 0);
    }
    
    //Play random LightFoot sounds
    objData->soundTimerLF -= gUpdateRateF;
    if (objData->soundTimerLF < 0.0f) {
        if (pushProgress > 0.0f) {
            //More frequent when LightFoot is losing
            objData->soundTimerLF = rand_next(40, 100);
        } else {
            //Less frequent when LightFoot is winning
            objData->soundTimerLF = rand_next(120, 240);
        }
        gDLL_6_AMSFX->vtbl->play(self, dSoundsLightFoot[rand_next(0, 2)], MAX_VOLUME, &objData->soundHandleLightFoot, 0, 0, 0);
    }
    
    //Adjust the pitch/volume of the wood creaking sound loop wrt. push progress magnitude
    if (pushProgress < 0/*.0f*/) {
        pushProgress = -pushProgress;
    }
    gDLL_6_AMSFX->vtbl->set_pitch(objData->soundHandleCreak, (pushProgress * 0.3f) + 0.85f);
    gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandleCreak, (u8)(pushProgress * 0x40) + 0x20);

    return 0;
}

// offset: 0xF44 | func: 8
/**
  * Finds the `SC_levelcontrol` Object, and uses one of its functions to
  * set an overall state value for SwapStone Circle.
  */
void SCTotemStrength_set_level_state(Object* self, u8 value) {
    Object** objects;
    s32 index;
    s32 count;

    for (objects = get_world_objects(&index, &count); index < count; index++) {
        if ((self != objects[index]) && (objects[index]->id == OBJ_SC_levelcontrol)) {
            ((DLL_519_SC_Levelcontrol*)objects[index]->dll)->vtbl->func7(objects[index], value);
            return;
        }
    }
}
