#include "PR/ultratypes.h"
#include "common.h"
#include "sys/menu.h"

#define Y_UNDERGROUND 50.0f
#define YAW_WIN 32500
#define YAW_SPEED_MAX 80.0f

typedef struct {
    ObjSetup base;
} DBSH_Symbol_Setup;

typedef struct {
    Object* krystal;        //Phantom Krystal opponent
    f32 unk4;               //Set to 0 in setup, but otherwise unused
    f32 yawSpeed;           //Rate of change of rotation (increased by A button presses)
    f32 baseY;              //Y value of the symbol when it's fully out of the ground
    s32 yaw;                //Overall rotation value, applied via a sequence function
    s32 pushSeq;            //Used to control the overall rotation of the symbol/sequence
    s16 unused18;
    s16 delayTimer;         //Causes the control function to return early
    s16 state;              //State Machine value
    s16 magicFxTimer;       //Delay between creating pink magic particles
    s16 timeLeft;           //Time left before failing the Test of Strength (in frames)
    u32 soundHandle;        //For controlling the magic hum sound loop
    s8 testActive;          //Boolean, whether animCallback function should allow pushing yet
} DBSH_Symbol_Data;

typedef enum {
    DBSH_Symbol_STATE_Initial = 0,
    DBSH_Symbol_STATE_Rising_Up = 1,
    DBSH_Symbol_STATE_Start_Test = 2,
    DBSH_Symbol_STATE_Sinking_Down = 3
} DBSH_Symbol_States;

/*0x0*/ static u32 dAnimOffsetSabre = 0;   //Keeps track of Sabre's leading foot (L/R)
/*0x4*/ static u32 dAnimOffsetKrystal = 0; //Keeps track of Krystal's leading foot (L/R)

static int DBSH_Symbol_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3);

// offset: 0x0 | ctor
void DBSH_Symbol_ctor(void *dll) { }

// offset: 0xC | dtor
void DBSH_Symbol_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBSH_Symbol_setup(Object* self, DBSH_Symbol_Setup* objSetup, s32 arg2) {
    DBSH_Symbol_Data* objData = self->data;
    
    objData->unk4 = 0.0f;
    objData->yawSpeed = 0.0f;
    objData->yaw = 0;
    objData->baseY = self->srt.transl.y;
    self->srt.transl.y -= Y_UNDERGROUND; //start off underground
    objData->state = DBSH_Symbol_STATE_Initial;
    objData->delayTimer = 0;
    objData->magicFxTimer = 0;
    self->srt.scale = 0.25f;
    objData->krystal = NULL;
    objData->soundHandle = 0;
    objData->testActive = FALSE;
    self->animCallback = DBSH_Symbol_anim_callback;
}

// offset: 0x94 | func: 1 | export: 1
void DBSH_Symbol_control(Object* self) {
    /*0x8*/ static u8 dPlayHissSound = TRUE;
    
    DBSH_Symbol_Data* objData = self->data;

    if (main_get_bits(BIT_16A) == 0) {
        objData->timeLeft = 4000; //66.666 seconds
        objData->state = DBSH_Symbol_STATE_Initial;
        objData->krystal = NULL;
        main_set_bits(BIT_DB_Shrine_Test_of_Strength_Lose, 0);
        dAnimOffsetSabre = 0;
        dAnimOffsetKrystal = 0;
        return;
    }

    //Do nothing while waiting for delay to elapse
    if (objData->delayTimer != 0) {
        objData->delayTimer -= gUpdateRate;
        if (objData->delayTimer <= 0) {
            objData->delayTimer = 0;
        }
        return;
    }

    if (objData->state == DBSH_Symbol_STATE_Initial) {
        objData->magicFxTimer = 10;
        objData->state = DBSH_Symbol_STATE_Rising_Up;
        return;
    }
    
    //Beginning the test
    if (objData->state == DBSH_Symbol_STATE_Start_Test) {
        //Advance state (Deferred until after sequences/anim_callback function are finished)
        objData->state = DBSH_Symbol_STATE_Sinking_Down;
        objData->delayTimer = 20;

        //Start the Test of Strength's sequence
        //(Causes control function to yield, and anim_callback function takes over)
        objData->pushSeq = gDLL_3_Animation->vtbl->func17(0, self, -1);

        //Start a UI countdown (66 seconds)
        func_8000F64C(0x1D, 66);
        func_8000F6CC();
        return;
    }
    
    //Rising out of ground at start of test
    if (objData->state == DBSH_Symbol_STATE_Rising_Up) {
        if (dPlayHissSound) {
            dPlayHissSound = FALSE;
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_358_Reverse_Static_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
        }

        if (self->srt.transl.y < objData->baseY) {
            //Rise up
            self->srt.transl.y += 0.5f * gUpdateRateF;
        } else {
            //Advance state once fully out of ground
            objData->state = DBSH_Symbol_STATE_Start_Test;
            objData->delayTimer = 20;
            dPlayHissSound = TRUE;
        }
        
        //Create glowing magic flare particles under the symbol's points
        objData->magicFxTimer -= gUpdateRate;
        if (objData->magicFxTimer <= 10) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_279, NULL, 0, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_279, NULL, 0, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_279, NULL, 0, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_279, NULL, 0, -1, NULL);
        }
        return;
    } 
    
    //Sinking into ground at end of test (runs after sequences/anim_callback are finished) 
    if (objData->state == DBSH_Symbol_STATE_Sinking_Down) {
        if (dPlayHissSound) {
            dPlayHissSound = FALSE;
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_358_Reverse_Static_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
            return;
        }
        
        //Descend underground
        if (self->srt.transl.y > (objData->baseY - Y_UNDERGROUND)) {
            self->srt.transl.y -= 0.5f * gUpdateRateF;

            //Set win/lose gamebit once underground
            if ((self->srt.transl.y <= (objData->baseY - Y_UNDERGROUND)) && (objData->timeLeft <= 0)) {
                main_set_bits(BIT_DB_Shrine_Test_of_Strength_Lose, 1);
            } else if (self->srt.transl.y <= (objData->baseY - Y_UNDERGROUND)) {
                main_set_bits(BIT_DB_Shrine_Test_of_Strength_Win, 1);
            }
            
            //Create glowing magic flare particles under the symbol's points
            objData->magicFxTimer -= gUpdateRate;
            if (objData->magicFxTimer <= 10) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_279, NULL, 0, -1, NULL);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_279, NULL, 0, -1, NULL);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_279, NULL, 0, -1, NULL);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_279, NULL, 0, -1, NULL);
            }
            
            //Stop magic hum sound loop
            if (objData->soundHandle != 0) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
                objData->soundHandle = 0;
            }
        }

        return;
    }
}

// offset: 0x5F8 | func: 2 | export: 2
void DBSH_Symbol_update(Object *self) { }

// offset: 0x604 | func: 3 | export: 3
void DBSH_Symbol_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
}

// offset: 0x64C | func: 4 | export: 4
void DBSH_Symbol_free(Object* self, s32 arg1) {
    func_8000FA2C();
}

// offset: 0x68C | func: 5 | export: 5
u32 DBSH_Symbol_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x69C | func: 6 | export: 6
u32 DBSH_Symbol_get_data_size(Object *self, u32 a1) {
    return sizeof(DBSH_Symbol_Data);
}

// offset: 0x6B0 | func: 7
int DBSH_Symbol_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
    static s32 sPrevYaw;
    
    DBSH_Symbol_Data* objData;
    Object* player;
    Object** objects;
    s32 i;
    s32 index;
    s32 count;

    objData = self->data;
    player = get_player();
    
    animData->unk7A = -1;
    animData->unk62 = 0;

    for (i = 0; i < animData->unk98; i++) {
        if (animData->unk8E[i] == 1U) {
            objData->testActive = TRUE;
        }
    }
    
    if ((u8)objData->testActive == FALSE) {
        return 0;
    }

    //TEST OF STRENGTH GAMEPLAY

    //Create glowing pink magic particles
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_27A, NULL, 0, -1, NULL);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_27A, NULL, 0, -1, NULL);
    
    //Play magic humming loop
    if (objData->soundHandle == 0) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_357_Magic_Hum_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->set_pitch(objData->soundHandle, 0.8f);
    }

    //Find phantom Krystal
    if (objData->krystal == NULL) {
        for (objects = get_world_objects(&index, &count); index < count; index++) {
            objData->krystal = objects[index];
            if (objData->krystal->id == OBJ_DBSH_Krystal) {
                break;
            }
        }
    }
    
    //Decrement test timer
    objData->timeLeft -= (s16) gUpdateRateF;
    
    //Handle pushing behaviour, looping for each frame skipped
    //(@bug: becomes much more difficult to win at smoother framerates)
    for (i = 0; i < gUpdateRate; i++) {
        if (objData->krystal == NULL) {
            return 0;
        }
        
        //Handle losing (time running out)
        if (objData->timeLeft <= 0) {
            gDLL_3_Animation->vtbl->func18(objData->pushSeq);
            func_8000FA2C();
        }
        
        //Handle tapping A button
        if (joy_get_released_buffered(0, i) & A_BUTTON) {
            //@bug: harder at smoother FPS, since it won't increase as much per press
            objData->yawSpeed += 18.8f;

            //5% chance of playing random Sabre effort sound
            if (rand_next(0, 20) == 0) {
                gDLL_6_AMSFX->vtbl->play(
                    NULL, 
                    SOUND_710_Sabre_Test_of_Strength_1 + rand_next(0, 2), 
                    0x1E, 
                    NULL, 
                    NULL, 
                    0, 
                    NULL
                );
            }
        }
        
        //Limit rotation speed in positive direction (player push)
        if (objData->yawSpeed > YAW_SPEED_MAX) {
            objData->yawSpeed = YAW_SPEED_MAX;
        }

        //Apply rotation speed
        if ((objData->yaw <= YAW_WIN) && (objData->yaw >= 0)) {
            objData->yaw += objData->yawSpeed;
        }
        
        //Handle winning
        if (objData->yaw > YAW_WIN) {
            objData->yaw = YAW_WIN;
            gDLL_3_Animation->vtbl->func18(objData->pushSeq);
            func_8000FA2C();
            objData->magicFxTimer = 10;
            objData->delayTimer = 20;
            func_80023D30(player, 0, 0.0f, 0);
            break;
        }
        
        //Rotate sequence using push yaw
        gDLL_3_Animation->vtbl->func28(objData->pushSeq, objData->yaw);
        
        //Handle being stopped (when at initial rotation)
        if (objData->yaw < 0) {
            objData->yaw = 0;
            
            if (objData->yawSpeed < 0.0f) {
                objData->yawSpeed = 0.0f;
            }
            
            //Use stopped animation for Sabre/Krystal
            func_80023D30(player, dAnimOffsetSabre + 0x4A, 0.0f, 0);
            func_80023D30(objData->krystal, dAnimOffsetKrystal + 0x4A, 1.0f, 0);
            
            sPrevYaw = objData->yaw;

            if (objData->yawSpeed > -300.0f) {
                objData->yawSpeed -= 10.1f;
            }

            return 0;
        }

        //Apply Krystal's counter-push
        if (objData->yawSpeed > -YAW_SPEED_MAX) {
            objData->yawSpeed -= 1.8f + (0.012f * objData->yawSpeed);
        }
    
        //Handle player anim progress/looping
        if (func_80024108(player, ((f32) objData->yaw - (f32) sPrevYaw) / 7500.0f, gUpdateRateF, NULL) != 0) {
            dAnimOffsetSabre = 1 - dAnimOffsetSabre;
            if ((((f32) objData->yaw - (f32) sPrevYaw) / 7500.0f) < 0.0f) {
                //Start at end of animation
                func_80023D30(player, dAnimOffsetSabre + 0x4A, 1.0f, 0);
            } else {
                //Start at beginning of animation
                func_80023D30(player, dAnimOffsetSabre + 0x4A, 0.0f, 0);
            }
        }
        
        //Handle phantom Krystal anim progress/looping
        if (objData->krystal && (func_80024108(objData->krystal, -((f32) objData->yaw - (f32) sPrevYaw) / 7500.0f, gUpdateRateF, NULL) != 0)) {
            dAnimOffsetKrystal = 1 - dAnimOffsetKrystal;
            if ((((f32) objData->yaw - (f32) sPrevYaw) / 7500.0f) < 0.0f) {
                //Start at beginning of animation
                func_80023D30(objData->krystal, dAnimOffsetKrystal + 0x4A, 0.0f, 0);
            } else {
                //Start at end of animation
                func_80023D30(objData->krystal, dAnimOffsetKrystal + 0x4A, 1.0f, 0);
            }
        }
        
        sPrevYaw = objData->yaw;         
    }

    //Adjust magic hum sound loop's pitch wrt. yaw
    gDLL_6_AMSFX->vtbl->set_pitch(objData->soundHandle, ((f32) objData->yaw / 97500.0f) + 0.8f);

    return 0;
}
