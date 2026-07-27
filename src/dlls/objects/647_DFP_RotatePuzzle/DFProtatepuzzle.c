#include "common.h"
#include "dlls/objects/332_FXEmit.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/math.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 unk19;               //Stored to objData->unkF during setup, but otherwise unused
    s16 unk1A;
    s16 unk1C;
    s16 gamebitUnused;
    s16 gamebitSpin;        //Puzzle wheel rotates when this gamebit is set
} DFP_RotatePuzzle_Setup;

typedef struct {
    f32 initialY;           //Stores the wheel's initial Y during setup, but otherwise unused
    u32 soundHandle;        //For the flame sound loop
    s16 gamebitSolved;      //Gamebit to set when all the flames are extinguished
    s16 gamebitSpin;        //Puzzle wheel rotates when this gamebit is set
    s16 spinTimer;          //The wheel rotates in steps when this is nonzero (moving 3/4ths of the time, stopping 1/4th of the time)
    u8 solved;              //Whether the puzzle is finished
    s8 unkF;                //Stores objSetup->unk19 during setup, but otherwise unused
    u8 numExtinguished;     //The number of flames extinguished
} DFP_RotatePuzzle_Data;

/*0x0*/ static s16 dTimers[] = {    //Timers tracking how long each statue has been blowing at the flames
    0, 0, 0, 0
};

/*0x0*/ static Object* sFlameFXEmits[4];    //The puzzle wheel's four flames

static int DFP_RotatePuzzle_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);
static s32 DFP_RotatePuzzle_createFlameFXEmit(Object* self, u8 index);
static s32 DFP_RotatePuzzle_checkIfFlameExtinguished(Object* self, u8 statueIdx);

// offset: 0x0 | ctor
void DFP_RotatePuzzle_ctor(void* dll) { }

// offset: 0xC | dtor
void DFP_RotatePuzzle_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFP_RotatePuzzle_obj_Setup(Object* self, DFP_RotatePuzzle_Setup* objSetup, s32 reset) {
    DFP_RotatePuzzle_Data* objData;
    u32 i;
    
    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = DFP_RotatePuzzle_animCallback;
    
    objData = self->data;
    objData->initialY = self->srt.transl.y;
    objData->unkF = objSetup->unk19;
    objData->gamebitSolved = BIT_DFPT_Load_Block_Slide_Puzzle_1;
    objData->gamebitSpin = objSetup->gamebitSpin;
    objData->spinTimer = 1;
    
    //Set all the statues' flame-blowing timers to 0
    dTimers[0] = 0;
    dTimers[1] = 0;
    dTimers[2] = 0;
    dTimers[3] = 0;
    
    //Check if the puzzle's already solved, otherwise create 4 flames
    if (mainGetBits(objData->gamebitSolved)) {
        objData->solved = TRUE;
    } else {
        for (i = 0; i < ARRAYCOUNT(sFlameFXEmits); i++) {
            DFP_RotatePuzzle_createFlameFXEmit(self, i);
        }
    }
    
    self->stateFlags |= OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x114 | func: 1 | export: 1
void DFP_RotatePuzzle_obj_Control(Object* self) {
    #define MIN_FLAME_BLOW_TIME 100
    #define MAX_FLAME_BLOW_TIME 220
    
    DFP_RotatePuzzle_Data* objData;
    s16 isSpinning;
    SRT fxTransform;
    s16 i;

    objData = self->data;
    
    //Do nothing when the puzzle's finished
    if (objData->solved) {
        return;
    }
    
    //Start flame sound loop
    if (objData->soundHandle == 0) {
        dll_amSfx->Play(self, SOUND_99E_Flame_Crackling_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
    }
    
    //Check if all the flames have been blown out
    if (objData->numExtinguished == ARRAYCOUNT(sFlameFXEmits)) {
        //Stop flame sound loop
        if (objData->soundHandle != 0) {
            dll_amSfx->Stop(objData->soundHandle);
            objData->soundHandle = 0;
        }
        
        dll_amSfx->Play(self, SOUND_798_Puzzle_Solved, MAX_VOLUME, NULL, NULL, 0, NULL);
        objData->solved = TRUE;
        mainSetBits(objData->gamebitSolved, TRUE);
    }
    
    //Spin after a gamebit is set (when Kyte pulls the nearby perch switch)
    {
        isSpinning = mainGetBits(objData->gamebitSpin);
        if (isSpinning && (objData->solved == FALSE) && 
            (objData->spinTimer > 50) //Spin 3/4ths of the time, but lock in place for a beat
        ) {
            self->srt.yaw += ((s16) gUpdateRateF * 30);
        }

        if ((objData->spinTimer != 0) && isSpinning) {
            objData->spinTimer -= (s16) gUpdateRateF;
            if (objData->spinTimer <= 0) {
                objData->spinTimer = 200;
                dll_amSfx->Play(self, SOUND_88B_Pulley_Operating, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
        }
    }
    
    //Move the flames to match the wheel's rotation
    {
        fxTransform.roll = 0;
        fxTransform.pitch = 0;
        fxTransform.transl.x = 0.0f;
        fxTransform.transl.y = 0.0f;
        fxTransform.transl.z = 0.0f;
        fxTransform.scale = 1.0f;
        
        for (i = 0; i < ARRAYCOUNT_S(sFlameFXEmits); i++) {
            if (sFlameFXEmits[i] != NULL) {
                sFlameFXEmits[i]->srt.transl.x = 0.0f;
                sFlameFXEmits[i]->srt.transl.y = 60.0f;
                sFlameFXEmits[i]->srt.transl.z = 93.0f;
                fxTransform.yaw = self->srt.yaw + (i * (M_90_DEGREES - 1)); //@bug?: not exact multiple of 90 degrees
                mathRotateRPY(&fxTransform, sFlameFXEmits[i]->srt.transl.f);
                sFlameFXEmits[i]->srt.transl.x += self->srt.transl.x;
                sFlameFXEmits[i]->srt.transl.y += self->srt.transl.y;
                sFlameFXEmits[i]->srt.transl.z += self->srt.transl.z;
            }
        }
    }
    
    /*
        Check if the statues have blown out a flame.
        
        Starts with the statue just to the left of the perch switch, and then checks 
        the rest of the statues proceeding anticlockwise around the room (viewed from above).
    */ 
    {
        //1st statue (on the left of the perch switch, viewed from the centre of the room)
        if (mainGetBits(BIT_DFP_RotatePuzzle_Statue_1_Blowing) || 
            (dTimers[0] && (dTimers[0] < MAX_FLAME_BLOW_TIME))
        ) {
            if (dTimers[0] >= MIN_FLAME_BLOW_TIME) {
                DFP_RotatePuzzle_checkIfFlameExtinguished(self, 0);
            }

            dTimers[0] += gUpdateRateF;
            if (dTimers[0] >= MAX_FLAME_BLOW_TIME) {
                dTimers[0] = 0;
            }
        }
        
        //2nd statue (on the left as you first walk into the room)
        if (mainGetBits(BIT_DFP_RotatePuzzle_Statue_2_Blowing) || 
            (dTimers[1] && (dTimers[1] < MAX_FLAME_BLOW_TIME))
        ) {
            if (dTimers[1] >= MIN_FLAME_BLOW_TIME) {
                DFP_RotatePuzzle_checkIfFlameExtinguished(self, 1);
            }
            dTimers[1] += gUpdateRateF;
            if (dTimers[1] >= MAX_FLAME_BLOW_TIME) {
                dTimers[1] = 0;
            }
        }
        
        //3rd statue (on the right as you first walk into the room, positioned on the upper walkway)
        if (mainGetBits(BIT_DFP_RotatePuzzle_Statue_3_Blowing) || 
            (dTimers[2] && (dTimers[2] < MAX_FLAME_BLOW_TIME))
        ) {
            if (dTimers[2] >= MIN_FLAME_BLOW_TIME) {
                DFP_RotatePuzzle_checkIfFlameExtinguished(self, 2);
            }
            dTimers[2] += gUpdateRateF;
            if (dTimers[2] >= MAX_FLAME_BLOW_TIME) {
                dTimers[2] = 0;
            }
        }
        
        //4th statue (on the right of the perch switch, viewed from the centre of the room)
        if (mainGetBits(BIT_DFP_RotatePuzzle_Statue_4_Blowing) || 
            (dTimers[3] && (dTimers[3] < MAX_FLAME_BLOW_TIME))
        ) {
            if (dTimers[3] >= MIN_FLAME_BLOW_TIME) {
                DFP_RotatePuzzle_checkIfFlameExtinguished(self, 3);
            }
            dTimers[3] += gUpdateRateF;
            if (dTimers[3] >= MAX_FLAME_BLOW_TIME) {
                dTimers[3] = 0;
            }
        }
    }
}

// offset: 0x670 | func: 2 | export: 2
void DFP_RotatePuzzle_obj_Update(Object* self) { }

// offset: 0x67C | func: 3 | export: 3
void DFP_RotatePuzzle_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x694 | func: 4 | export: 4
void DFP_RotatePuzzle_obj_Free(Object* self, s32 onlySelf) {
    DFP_RotatePuzzle_Data* objData;
    s16 i;

    objData = self->data;
    
    //Free the flames
    for (i = 0; i < ARRAYCOUNT_S(sFlameFXEmits); i++) {
        if ((sFlameFXEmits[i] != NULL) && (onlySelf == FALSE)) {
            objFreeObject(sFlameFXEmits[i]);
        }
    }
    
    //Free soundhandles
    if (objData->soundHandle != 0) {
        dll_amSfx->Stop(objData->soundHandle);
        objData->soundHandle = 0;
    }
}

// offset: 0x760 | func: 5 | export: 5
u32 DFP_RotatePuzzle_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x770 | func: 6 | export: 6
u32 DFP_RotatePuzzle_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DFP_RotatePuzzle_Data);
}

// offset: 0x784 | func: 7
int DFP_RotatePuzzle_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    return 0;
}

// offset: 0x7A0 | func: 8
s32 DFP_RotatePuzzle_createFlameFXEmit(Object* self, u8 index) {
    FXEmit_Setup* fxSetup;

    fxSetup = objAllocSetup(sizeof(FXEmit_Setup), OBJ_FXEmit);
    fxSetup->base.loadDistance = 0xFF;
    fxSetup->base.fadeDistance = 0xFF;
    fxSetup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
    fxSetup->base.fadeFlags = OBJSETUP_FADE_MANUAL;
    fxSetup->base.x = self->srt.transl.x;
    fxSetup->base.y = self->srt.transl.y;
    fxSetup->base.z = self->srt.transl.z;
    fxSetup->toggleGamebit = NO_GAMEBIT;
    fxSetup->disableGamebit = NO_GAMEBIT;
    fxSetup->yaw = 0;
    fxSetup->flagConfig = 1;
    fxSetup->pitch = 0;
    fxSetup->roll = 0;
    fxSetup->rollSpeed = 0;
    fxSetup->pitchSpeed = 0;
    fxSetup->yawSpeed = 0;
    fxSetup->activationRange = 0;
    fxSetup->bank = 1;
    fxSetup->indexInBank = 17;
    fxSetup->fxRate = 1;
    fxSetup->interval = 0xFF;
    sFlameFXEmits[index] = objSetupObject(&fxSetup->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, self->parent);
    
    return 1;
}

// offset: 0x8A0 | func: 9
s32 DFP_RotatePuzzle_checkIfFlameExtinguished(Object* self, u8 statueIdx) {
    // Define angular ranges to determine where the wheel is currently "pointing"
    // (NOTE: they're a little off from being rotationally symmetrical, which likely contributes to how inconsistent the puzzle feels!)
    #define ANGLE_ZONE_0_MIN 24000                          //131.835 degrees
    #define ANGLE_ZONE_0_MAX (ANGLE_ZONE_0_MIN + 2000)      //142.822 degrees

    #define ANGLE_ZONE_1_MIN -10000                         //-54.931 degrees      
    #define ANGLE_ZONE_1_MAX (ANGLE_ZONE_1_MIN + 3000)      //-38.452 degrees

    #define ANGLE_ZONE_2_MIN 6000                           //32.958 degrees
    #define ANGLE_ZONE_2_MAX (ANGLE_ZONE_2_MIN + 3000)      //49.438 degrees

    #define ANGLE_ZONE_3_MIN -26000                         //-142.822 degrees
    #define ANGLE_ZONE_3_MAX (ANGLE_ZONE_3_MIN + 2000)      //-131.836 degrees

    //Wheel's Z-axis pointing towards the statue to the right of the perch switch
    #define IN_ANGLE_ZONE_0 (ANGLE_ZONE_0_MAX > self->srt.yaw) && (self->srt.yaw > ANGLE_ZONE_0_MIN) 

    //Wheel's Z-axis pointing towards the statue on the left as you walk in
    #define IN_ANGLE_ZONE_1 (ANGLE_ZONE_1_MIN < self->srt.yaw) && (self->srt.yaw < ANGLE_ZONE_1_MAX) 

    //Wheel's Z-axis pointing towards the statue on the right as you walk in
    #define IN_ANGLE_ZONE_2 (ANGLE_ZONE_2_MAX > self->srt.yaw) && (self->srt.yaw > ANGLE_ZONE_2_MIN) 

    //Wheel's Z-axis pointing towards the statue to the left of the perch switch
    #define IN_ANGLE_ZONE_3 (ANGLE_ZONE_3_MIN < self->srt.yaw) && (self->srt.yaw < ANGLE_ZONE_3_MAX) 

    DFP_RotatePuzzle_Data* objData;
    
    objData = self->data;
    
    if (statueIdx == 0) {
        if ((sFlameFXEmits[1] != NULL) && IN_ANGLE_ZONE_0) {
            objFreeObject(sFlameFXEmits[1]);
            sFlameFXEmits[1] = NULL;
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->numExtinguished++;
        } else if ((sFlameFXEmits[3] != NULL) && IN_ANGLE_ZONE_1) {
            objFreeObject(sFlameFXEmits[3]);
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            sFlameFXEmits[3] = NULL; //@typo: zeroed after the sound call, unlike the rest (doesn't affect anything though)
            objData->numExtinguished++;
        } else if ((sFlameFXEmits[2] != NULL) && IN_ANGLE_ZONE_2) {
            objFreeObject(sFlameFXEmits[2]);
            sFlameFXEmits[2] = NULL;
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->numExtinguished++;
        } else if ((sFlameFXEmits[0] != NULL) && IN_ANGLE_ZONE_3) {
            objFreeObject(sFlameFXEmits[0]);
            sFlameFXEmits[0] = NULL;
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->numExtinguished++;
        }
    } else if (statueIdx == 1) {
        if ((sFlameFXEmits[2] != NULL) && IN_ANGLE_ZONE_0) {
            objFreeObject(sFlameFXEmits[2]);
            sFlameFXEmits[2] = NULL;
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->numExtinguished++;
        } else if ((sFlameFXEmits[0] != NULL) && IN_ANGLE_ZONE_1) {
            objFreeObject(sFlameFXEmits[0]);
            sFlameFXEmits[0] = NULL;
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->numExtinguished++;
        } else if ((sFlameFXEmits[3] != NULL) && IN_ANGLE_ZONE_2) {
            objFreeObject(sFlameFXEmits[3]);
            sFlameFXEmits[3] = NULL;
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->numExtinguished++;
        } else if ((sFlameFXEmits[1] != NULL) && IN_ANGLE_ZONE_3) {
            objFreeObject(sFlameFXEmits[1]);
            sFlameFXEmits[1] = NULL;
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->numExtinguished++;
        }
    } else if (statueIdx == 2) {
        if ((sFlameFXEmits[3] != NULL) && IN_ANGLE_ZONE_0) {
            objFreeObject(sFlameFXEmits[3]);
            sFlameFXEmits[3] = NULL;
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->numExtinguished++;
        } else if ((sFlameFXEmits[1] != NULL) && IN_ANGLE_ZONE_1) {
            objFreeObject(sFlameFXEmits[1]);
            sFlameFXEmits[1] = NULL;
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->numExtinguished++;
        } else if ((sFlameFXEmits[0] != NULL) && IN_ANGLE_ZONE_2) {
            objFreeObject(sFlameFXEmits[0]);
            sFlameFXEmits[0] = NULL;
            // dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL); //@bug: missing sound
            objData->numExtinguished++;
        } else if ((sFlameFXEmits[2] != NULL) && IN_ANGLE_ZONE_3) {
            objFreeObject(sFlameFXEmits[2]);
            sFlameFXEmits[2] = NULL;
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->numExtinguished++;
        }
    } else if (statueIdx == 3) {
        if ((sFlameFXEmits[0] != NULL) && IN_ANGLE_ZONE_0) {
            objFreeObject(sFlameFXEmits[0]);
            sFlameFXEmits[0] = NULL;
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->numExtinguished++;
        } else if ((sFlameFXEmits[2] != NULL) && IN_ANGLE_ZONE_1) {
            objFreeObject(sFlameFXEmits[2]);
            sFlameFXEmits[2] = NULL;
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->numExtinguished++;
        } else if ((sFlameFXEmits[1] != NULL) && IN_ANGLE_ZONE_2) {
            objFreeObject(sFlameFXEmits[1]);
            sFlameFXEmits[1] = NULL;
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->numExtinguished++;
        } else if ((sFlameFXEmits[3] != NULL) && IN_ANGLE_ZONE_3) {
            objFreeObject(sFlameFXEmits[3]);
            sFlameFXEmits[3] = NULL;
            dll_amSfx->Play(self, SOUND_99B_Flame_Extinguish, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->numExtinguished++;
        }
    }
    
    return 1;
}
