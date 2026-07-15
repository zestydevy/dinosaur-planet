#include "PR/os.h"
#include "common.h"
#include "dlls/objects/291_magicdust.h"
#include "game/objects/interaction_arrow.h"
#include "macros.h"
#include "sys/objlib.h"
#include "sys/objmsg.h"
#include "sys/objanim.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 survived;        //The EarthWalker animates and can be talked to when this is set
    s16 deadModAnimIdx; //This modAnimIdx is used when `objSetup->survived == FALSE`
    s16 pitch;
} DeadDino_Setup;

typedef struct {
    s32 _unk0;
    f32 animSpeedFlinch; //Animation playback speed when reacting to attacks
    f32 vanishTimer;     //Timer for the EarthWalker's vanishing animation: particles/sounds/MagicDust created as it progresses
    s16 blinkTimer;      //Randomised interval between blinks
    s16 cryHelpTimer;    //Randomised interval between cries for help
    u8 unk10;
    u8 vanished;         //Boolean: set when EarthWalker disappears, stops printing and removes collision
    u8 hitFlags;         //Set when reacting to attacks
    u8 blinkFrame;       //Frame index for eyelid texture animation
    u8 state;            //State Machine value
} DeadDino_Data;

typedef enum {
    STATE_0_Calling_for_Help,
    STATE_1_Waiting_for_Help,
    STATE_2_Talking,
    STATE_3_Dead
} DeadDino_States;

/*0x0*/ static Unk80026DF4 dJointHitSounds[] = {
    {0x0377, 0xffff, 0xffff, 0xffff, 0, 0, 0}, 
    {0x0377, 0xffff, 0xffff, 0xffff, 0, 0, 0}, 
    {0x0377, 0xffff, 0xffff, 0xffff, 0, 0, 0}, 
    {0x0677, 0xffff, 0xffff, 0xffff, 1, 0, 0}, 
    {0x0377, 0xffff, 0xffff, 0xffff, 0, 0, 0}, 
    {0x0377, 0xffff, 0xffff, 0xffff, 0, 0, 0}, 
    {0x0377, 0xffff, 0xffff, 0xffff, 0, 0, 0}, 
    {0x0377, 0xffff, 0xffff, 0xffff, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0},
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}, 
    {0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0}
};

static int WL_DeadDino_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);
static Object* WL_DeadDino_create_magic_dust(Object* self);

// offset: 0x0 | ctor
void WL_DeadDino_ctor(void *dll) { }

// offset: 0xC | dtor
void WL_DeadDino_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_DeadDino_setup(Object* self, DeadDino_Setup* objSetup, s32 reset) {
    DeadDino_Data* objData = self->data;
    ObjectHitInfo* objHits;
    
    objInitMesgQueue(self, 4);
    
    self->animCallback = WL_DeadDino_anim_callback;
    self->srt.yaw = objSetup->yaw << 8;
    self->srt.pitch = objSetup->pitch;
    
    if (mainGetBits(BIT_WM_EarthWalker_Died)) {
        self->objhitInfo->unk58 &= ~1;
        objData->vanished = TRUE;
    }
    
    objData->vanishTimer = 0.0f;
}

// offset: 0xD8 | func: 1 | export: 1
void WL_DeadDino_control(Object* self) {
    DeadDino_Setup* objSetup;
    s16 newModanimIdx;
    TextureAnimator* eyeL;
    TextureAnimator* eyeR;
    DeadDino_Data* objData;
    u8 prevBlinkFrame;
    u8 animate;
    u32 outMessageID;
    Object* player;
    f32 distanceToPlayer;
    f32 animSpeed;

    objSetup = (DeadDino_Setup*)self->setup;
    objData = self->data;
    
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    
    //Show LockIcon tutorial as Krystal approaches (when in LockIcon range)
    if ((self->unkAF & ARROW_FLAG_4_Highlighted) && (mainGetBits(BIT_Tutorial_ZLock_Talk) == FALSE)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
        mainSetBits(BIT_Tutorial_ZLock_Talk, TRUE);
        return;
    }
    
    //Remove collision after disappearing
    if (objData->vanished) {
        self->objhitInfo->unk58 &= ~1;
        return;
    }

    //Handle joint objHits
    if (func_80026DF4(self, dJointHitSounds, ARRAYCOUNT(dJointHitSounds), (objData->hitFlags & 1) ? TRUE : FALSE, &objData->animSpeedFlinch)) {
        objData->hitFlags |= 1;
        return;
    }
    objData->hitFlags &= ~1;
    
    while (objRecvMesg(self, &outMessageID, 0, 0)) {
    }
    
    //Handle changing/advancing animation
    {
        animate = TRUE;
        if (mainGetBits(BIT_WM_EarthWalker_Died) == FALSE) {
            if (objSetup->survived == FALSE) {
                //EarthWalker died during Scales' attack
                newModanimIdx = objSetup->deadModAnimIdx | 0x100;
                animSpeed = 0.0f;
            } else {
                //EarthWalker survived the attack
                self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                animSpeed = 0.005f;
                
                player = objGetPlayer();
                if (player != NULL) {
                    objAngleToObjectXZ(self, player, &distanceToPlayer);

                    if (objData->state == STATE_1_Waiting_for_Help) {
                        //Call out to the player at intervals
                        if ((distanceToPlayer > 140.0f) || (objData->cryHelpTimer <= 0)) {
                            objData->state = STATE_0_Calling_for_Help;
                        }
                        objData->cryHelpTimer -= gUpdateRate;

                        //Play a sequence when interacted with ("try not to move, the wizard will help you")
                        if (self->unkAF & ARROW_FLAG_1_Interacted) {
                            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
                            objData->state = STATE_2_Talking;
                            joyDisableButtons(0, A_BUTTON);
                        }
                    } else if (objData->state == STATE_0_Calling_for_Help) {
                        if (distanceToPlayer < 120.0f) {
                            gDLL_6_AMSFX->vtbl->play(self, SOUND_1FB_EarthWalker_Help, MAX_VOLUME, NULL, NULL, 0, NULL);
                            objData->state = STATE_1_Waiting_for_Help;
                            objData->cryHelpTimer = mathRnd(400, 600);
                        }
                    } else {
                        animSpeed = 0.0f;
                    }
                }

                newModanimIdx = 0x102;
            }
        } else if ((objData->vanishTimer == 0.0f) && mainGetBits(BIT_WM_EarthWalker_Died)) {
            objData->vanishTimer = 1.0f;
            gDLL_6_AMSFX->vtbl->play(self, SOUND_B21_Dissipating_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
            animate = FALSE;
        }
        
        //Advance/change animation
        if (animate && (objData->vanished == FALSE)) {
            if (newModanimIdx != self->curModAnimId) {
                objAnimSet(self, newModanimIdx, 0.0f, 0);
            }
            objAnimAdvance(self, animSpeed, gUpdateRateF, NULL);
        }
    }
    
    //Handle eyelids
    if (objData->vanishTimer) {
        prevBlinkFrame = objData->blinkFrame;
        if ((objSetup->survived == FALSE) || (objData->state >= STATE_2_Talking)) {
            objData->blinkFrame = 1;
        } else {
            objData->blinkTimer -= gUpdateRate;
            if (objData->blinkTimer < 0) {
                if (objData->blinkFrame != 0) {
                    objData->blinkFrame = 0;
                    objData->blinkTimer = mathRnd(60, 300);
                } else {
                    objData->blinkFrame = 1;
                    objData->blinkTimer = 10;
                }
            }
        }
        
        if (objData->blinkFrame != prevBlinkFrame) {
            eyeL = objExprGetTexAnimator(self, 5, 0);
            eyeR = objExprGetTexAnimator(self, 4, 0);
            if ((eyeL != NULL) && (eyeR != NULL)) {
                eyeL->frame = objData->blinkFrame << 8;
                eyeR->frame = objData->blinkFrame << 8;
            }
        }
    }
}

// offset: 0x61C | func: 2 | export: 2
void WL_DeadDino_update(Object *self) { }

// offset: 0x628 | func: 3 | export: 3
void WL_DeadDino_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DeadDino_Data* objdata;

    objdata = self->data;

    if (!visibility || objdata->vanished) {
        return;
    }

    objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);

    if (objdata->vanishTimer == 0) {
        return;
    }

    if (objdata->vanishTimer >= 1.8f) {
        gDLL_32->vtbl->func2(self, PARTICLE_330, NULL);
    } else {
        gDLL_32->vtbl->func2(self, PARTICLE_32F, &objdata->vanishTimer);
    }

    objdata->vanishTimer += (gUpdateRateF * 0.01f);
    if (objdata->vanishTimer >= 2.0f) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_B1F_Slow_Magic_Chimes, MAX_VOLUME, NULL, NULL, 0, NULL);
        objdata->vanished = TRUE;
        objdata->vanishTimer = 0.0f;
        WL_DeadDino_create_magic_dust(self);
    }
}

// offset: 0x7BC | func: 4 | export: 4
void WL_DeadDino_free(Object *self, s32 a1) { }

// offset: 0x7CC | func: 5 | export: 5
u32 WL_DeadDino_get_model_flags(Object* self) {
    return MODFLAGS_1;
}

// offset: 0x7DC | func: 6 | export: 6
u32 WL_DeadDino_get_data_size(Object *self, u32 a1) {
    return sizeof(DeadDino_Data);
}

// offset: 0x7F0 | func: 7
int WL_DeadDino_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    DeadDino_Data* objData;
    s32 i;

    objData = self->data;
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    animData->unk7A = -1;
    animData->unk62 = 0;
    
    for (i = 0; i < animData->messageCount; i++) {
        if (animData->messages[i] == 1) {
            objData->state = STATE_3_Dead;
            mainSetBits(BIT_WM_EarthWalker_Died, TRUE);
            animData->messages[i] = 0;
        }
    }
    
    if (objData->state == STATE_3_Dead) {
        animData->unk7A &= ~(ANIM7AFLAG_OVERRIDE_OPACITY | ANIM7AFLAG_OVERRIDE_MODEL);
    }

    return 0;
}

// offset: 0x8E4 | func: 8
void WL_DeadDino_func_8E4(void) {

}

// offset: 0x8EC | func: 9
static Object* WL_DeadDino_create_magic_dust(Object* self) {
    MagicDust_Setup* magicSetup;

    magicSetup = objAllocSetup(sizeof(MagicDust_Setup), OBJ_MagicDustMid);
    magicSetup->unk1A = 20;
    magicSetup->unk2C = -1;
    magicSetup->unk1C = -1;
    magicSetup->base.x = self->srt.transl.x;
    magicSetup->base.y = self->srt.transl.y + 40.0f;
    magicSetup->base.z = self->srt.transl.z;
    magicSetup->unk24 = -1;
    magicSetup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
    magicSetup->base.fadeFlags = OBJSETUP_FADE_MANUAL;
    magicSetup->base.loadDistance = 0xFF;
    magicSetup->base.fadeDistance = 0xFF;
    magicSetup->base.objId = OBJ_MagicDustMid;
    return objSetupObject((ObjSetup*)magicSetup, OBJINIT_FLAG4 | OBJINIT_STANDALONE, self->mapID, -1, self->parent);
}
