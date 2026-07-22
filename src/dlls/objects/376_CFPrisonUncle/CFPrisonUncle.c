#include "common.h"
#include "game/objects/interaction_arrow.h"
#include "macros.h"
#include "sys/objexpr.h"
#include "sys/gfx/model.h"
#include "sys/objanim.h"
#include "sys/objmsg.h"

static int CFPrisonUncle_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);
static void CFPrisonUncle_animateHeadJoints(Object* self, Object* player);
static void CFPrisonUncle_resetHeadJoint(Object* self);

typedef struct {
    Object* perchObj;
    HeadAnimation headAnimators[2]; 
    u32 voiceLines[2]; 
    u32 unk54;
    u32 soundHandle;
    s16 talkWaitTimer;
    s8 voiceLineIdx;
    s8 freed;
    s8 magicDropped;
    s32 pad; 
    u8 _unk68[0x94 - 0x68];
} CFPrisonUncle_Data;

typedef struct{
    ObjSetup base;
    s16 _unk18;
    s8 unk1A;
    s8 unk1B;
    s16 unk1C;
    s16 _unk1E;
    s16 _unk20;
    s16 _unk22;
    s16 unk24;
    s8 _unk26;
    s8 unk27;
    s16 _unk28;
    s16 _unk2A;
    s16 unk2C;
} BoneDust_Setup;

/*0x0*/ static s16 dCloudRunnerChirps[] = {
    SOUND_C2_CloudRunner_Chirp_Double, 
    SOUND_C3_CloudRunner_Chirp, 
    SOUND_C4_CloudRunner_Chirp_Descending, 
    SOUND_C5_CloudRunner_Chirp_Warble
};

// offset: 0x0 | ctor
void CFPrisonUncle_ctor(void *dll) { }

// offset: 0xC | dtor
void CFPrisonUncle_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFPrisonUncle_obj_Setup(Object* self, ObjSetup* setup, s32 reset) {
    CFPrisonUncle_Data* objData;

    objInitMesgQueue(self, 1);
    objData = self->data;
    self->animCallback = CFPrisonUncle_animCallback;

    //@bug: should this be 0x1CF and 0x1D0? 0x1D1 is silent, and an earlier recording of 0x1CF was heard in the One Hour Footage.
    objData->voiceLines[0] = SOUND_1D0_CRF_Uncle_Get_Me_Out;
    objData->voiceLines[1] = SOUND_1D1; 

    objData->talkWaitTimer = 0;
    objData->magicDropped = FALSE;

    if (mainGetBits(BIT_CRF_CloudRunner_Uncle_Freed)) {
        mainSetBits(BIT_CRF_CloudRunner_Uncle_Escaped, TRUE);
    }
}

// offset: 0xC4 | func: 1 | export: 1
void CFPrisonUncle_obj_Control(Object* self) {
    CFPrisonUncle_Data* objData;
    s32 i;
    Object* messageSender;
    Object* player;
    s32 initialIndex;
    s32 objectsCount;
    Object** objects;
    u32 messageID;
    void* messageArg;
    SeqJoint* jawJoint;
    
    objData = self->data;
    if (!objData || mainGetBits(BIT_CRF_CloudRunner_Uncle_Escaped)){
        return;
    }
    
    if (objRecvMesg(self, &messageID, &messageSender, &messageArg)) {
        objData->perchObj = NULL;
    }
    
    if (objData->perchObj == NULL) {
        objects = objGetObjects(&initialIndex, &objectsCount);
        for (i = initialIndex; i < objectsCount; i++){
            if (objects[i]->controlNo == OBJCONTROL_CFPerch) {
                objData->perchObj = objects[i];
                i = objectsCount;
            }
        }
    }
    
    objData->freed = mainGetBits(BIT_CRF_CloudRunner_Uncle_Freed);
    if (objData->freed == FALSE) {
        player = objGetPlayer();
        
        if (objData->soundHandle) {
            objAnimSet(self, 0, 0.105f, 0);
            CFPrisonUncle_animateHeadJoints(self, player);

            jawJoint = objExpr_func_80034804(self, 1);
            if (mathRnd(0, 8) != 0) {
                jawJoint->pitch = -M_15_DEGREES;
            } else {
                jawJoint->pitch = 0;
            }

            if (dll_amSfx->IsPlaying(objData->soundHandle) == 0) {
                dll_amSfx->Stop(objData->soundHandle);
                objData->soundHandle = 0;
                jawJoint->pitch = 0;
            }
        } else {
            CFPrisonUncle_resetHeadJoint(self);

            if (mathRnd(0, 30) == 0) { 
                objExpr_func_80034B94(self, &objData->headAnimators[1], dCloudRunnerChirps[mathRnd(0, 3)]);
            }
        }
        
        if (self->unkAF & ARROW_FLAG_1_Interacted) {
            CFPrisonUncle_animateHeadJoints(self, player);

            jawJoint = objExpr_func_80034804(self, 1);
            jawJoint->pitch = -M_15_DEGREES;

            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
        } else {
            objExpr_func_80034BC0(self, &objData->headAnimators[1]);

            if ((objData->soundHandle == 0) && (objData->talkWaitTimer <= 0) && 
                player && (vec3Distance(&self->globalPosition, &player->globalPosition) < 200.0f)
            ) {
                dll_amSfx->Play(self, objData->voiceLines[objData->voiceLineIdx], MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
                
                if (mathRnd(0, 100) < 50) {
                    objData->voiceLineIdx++;
                } else {
                    objData->voiceLineIdx--;
                }

                if (objData->voiceLineIdx < 0) {
                    objData->voiceLineIdx = ARRAYCOUNT_S(objData->voiceLines) - 1;
                } else if (objData->voiceLineIdx > (ARRAYCOUNT_S(objData->voiceLines) - 1)) {
                    objData->voiceLineIdx = 0;
                }

                objData->talkWaitTimer = mathRnd(1000, 5000);
            }

            if (objData->talkWaitTimer > 0) {
                objData->talkWaitTimer -= gUpdateRate;
            }

            objAnimAdvance(self, 0.005f, gUpdateRate, NULL);
        }
    } else {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        if (self->seqSlot == SEQSLOT_NONE) {
            gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        }
    }
}

// offset: 0x590 | func: 2 | export: 2
void CFPrisonUncle_obj_Update(Object *self) { }

// offset: 0x59C | func: 3 | export: 3
void CFPrisonUncle_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    s32 boneIdx;
    ModelInstance* modelInstance;
    MtxF* mtx;
    ObjDef* objDef;
    CFPrisonUncle_Data* objData;
 
    objData = self->data;  
    
    //Draw just the Perch (and not the CloudRunner Uncle) after the Uncle's escaped
    if (mainGetBits(BIT_CRF_CloudRunner_Uncle_Escaped)) {

        if (objData->perchObj && trackObjVisCheck(objData->perchObj)) {
            objprintDrawModel(objData->perchObj, gdl, mtxs, vtxs, pols, 1.0f);
        }

        return;
    }
    
    //Draw CloudRunner Uncle and Perch after the uncle's been freed
    if (mainGetBits(BIT_CRF_CloudRunner_Uncle_Freed) && visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);

        if (objData->perchObj && trackObjVisCheck(objData->perchObj)) {
            objprintDrawModel(objData->perchObj, gdl, mtxs, vtxs, pols, 1.0f);
        }

        return;
    }
    
    if (!objData || !objData->perchObj){
        return;
    }
        
    if (objData->freed == FALSE) {
        if (!visibility) {
            return;
        }

        //Draw perch
        objprintDrawModel(objData->perchObj, gdl, mtxs, vtxs, pols, 1.0f);

        //Attach CloudRunner Uncle to the perch
        {
            objDef = objData->perchObj->def;
            modelInstance = objData->perchObj->modelInsts[objData->perchObj->modelInstIdx];

            if (objDef->numAttachPoints != 0) {
                boneIdx = objDef->pAttachPoints->bones[self->modelInstIdx];
                mtx = (MtxF*)(((f32*)modelInstance->matrices[modelInstance->unk34 & 1]) + (boneIdx << 4));
                
                self->srt.transl.x = self->globalPosition.x = mtx->m[3][0] + gWorldX;
                self->srt.transl.y = self->globalPosition.y = mtx->m[3][1];
                self->srt.transl.z = self->globalPosition.z = mtx->m[3][2] + gWorldZ;
                
                self->srt.yaw = 0;
                self->srt.pitch = 0;
                self->srt.roll = 0;
            }
        }

        //Draw CloudRunner Uncle
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    } else {
        //Draw the Perch and the CloudRunner Uncle after the Uncle's been freed
        if (trackObjVisCheck(objData->perchObj)) {
            objprintDrawModel(objData->perchObj, gdl, mtxs, vtxs, pols, 1.0f);
        }

        if (visibility) {
            objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0x8C0 | func: 4 | export: 4
void CFPrisonUncle_obj_Free(Object *self, s32 onlySelf) { }

// offset: 0x8D0 | func: 5 | export: 5
u32 CFPrisonUncle_obj_GetModelFlags(Object* self) {
    return MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x8E0 | func: 6 | export: 6
u32 CFPrisonUncle_obj_GetDataSize(Object *self, u32 offsetAddr) {
    return sizeof(CFPrisonUncle_Data);
}

// offset: 0x8F4 | func: 7
int CFPrisonUncle_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    CFPrisonUncle_Data* objData;
    BoneDust_Setup* dustSetup;

    objData = self->data;

    if (objData->magicDropped) {
        return 0;
    } 
    
    if (animData->lastMessage == 2) {
        objData->magicDropped = TRUE;
        
        dustSetup = (BoneDust_Setup*)objAllocSetup(sizeof(BoneDust_Setup), OBJ_BoneDust); 
        dustSetup->unk1A = 2;
        dustSetup->unk2C = -1;
        dustSetup->unk1C = -1;
        dustSetup->base.x = self->srt.transl.x;
        dustSetup->base.y = self->srt.transl.y + 15.0f;
        dustSetup->base.z = self->srt.transl.z;
        dustSetup->unk24 = -1;
        dustSetup->base.loadFlags = OBJSETUP_LOAD_MAIN;
        dustSetup->base.fadeFlags = OBJSETUP_FADE_CAMERA;
        dustSetup->base.loadDistance = 0x28;
        dustSetup->base.fadeDistance = 0xFF;
        dustSetup->unk27 = 0;
        objSetupObject(&dustSetup->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, NULL);
    }

    return 0;
}

// offset: 0x9F8 | func: 8
void CFPrisonUncle_resetHeadJoint(Object* self) {
    SeqJoint* headJoint = objExpr_func_80034804(self, 0);
    headJoint->pitch = 0;
    headJoint->yaw = 0;
}

// offset: 0xA3C | func: 9
void CFPrisonUncle_animateHeadJoints(Object* self, Object* player) {
    CFPrisonUncle_Data *objData = self->data;
    
    objExpr_func_80032CF8(self, player, objData->headAnimators, 35);
}
