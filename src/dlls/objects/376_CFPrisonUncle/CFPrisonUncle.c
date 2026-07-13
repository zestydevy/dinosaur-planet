#include "common.h"
#include "sys/objexpr.h"
#include "sys/gfx/model.h"
#include "sys/objanim.h"
#include "sys/objmsg.h"

static s32 dll_376_func_8F4(Object* self, Object* arg1, AnimObj_Data* arg2, s32 arg3);
static void dll_376_func_A3C(Object* self, Object* player);
static void dll_376_func_9F8(Object* self);

typedef struct {
    Object* unk0;
    HeadAnimation unk4; 
    HeadAnimation unk28;
    u32 unk4C[2]; 
    u32 unk54;
    u32 unk58;
    s16 unk5C;
    s8 unk5E;
    s8 unk5F;
    s8 unk60;
    s32 pad; 
    u8 _unk68[0x94 - 0x68];
} DLL376_Data;

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

/*0x0*/ static s16 _data_0[] = {
    0x00c2, 0x00c3, 0x00c4, 0x00c5, 0, 0, 0, 0
};

// offset: 0x0 | ctor
void dll_376_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_376_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_376_setup(Object* self, ObjSetup* setup, s32 arg2) {
    DLL376_Data* objData;

    objInitMesgQueue(self, 1);
    objData = self->data;
    self->animCallback = (void*)&dll_376_func_8F4;
    objData->unk4C[0] = 0x1D0;
    objData->unk4C[1] = 0x1D1;
    objData->unk5C = 0;
    objData->unk60 = 0;
    if (mainGetBits(BIT_4D)) {
        mainSetBits(BIT_50, 1);
    }
}

// offset: 0xC4 | func: 1 | export: 1
void dll_376_control(Object* self) {
    DLL376_Data* objData;
    s32 i;
    Object* messageSender;
    Object* player;
    s32 initialIndex;
    s32 objectsCount;
    Object** objects;
    u32 messageID;
    void* messageArg;
    s16* sp40;
    
    objData = self->data;
    if (!objData || mainGetBits(BIT_50)){
        return;
    }
    
    if (objRecvMesg(self, &messageID, &messageSender, &messageArg)) {
        objData->unk0 = NULL;
    }
    
    if (objData->unk0 == NULL) {
        objects = objGetObjects(&initialIndex, &objectsCount);
        for (i = initialIndex; i < objectsCount; i++){
            if (objects[i]->controlNo == OBJCONTROL_CFPerch) {
                objData->unk0 = objects[i];
                i = objectsCount;
            }
        }
    }
    
    objData->unk5F = mainGetBits(BIT_4D);
    if (objData->unk5F == 0) {
        player = objGetPlayer();
        
        if (objData->unk58) {
            objAnimSet(self, 0, 0.105f, 0);
            dll_376_func_A3C(self, player);
            sp40 = objExpr_func_80034804(self, 1);
            if (rand_next(0, 8) != 0) {
                *sp40 = -0xAAA;
            } else {
                *sp40 = 0;
            }
            if (gDLL_6_AMSFX->vtbl->is_playing(objData->unk58) == 0) {
                gDLL_6_AMSFX->vtbl->stop(objData->unk58);
                objData->unk58 = 0;
                *sp40 = 0;
            }
        } else {
            dll_376_func_9F8(self);
            if (rand_next(0, 0x1E) == 0) { 
                objExpr_func_80034B94(self, &objData->unk28, _data_0[rand_next(0, 3)]);
            }
        }
        
        if (self->unkAF & 1) {
            dll_376_func_A3C(self, player);
            *objExpr_func_80034804(self, 1) = -0xAAA;
            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
        } else {
            objExpr_func_80034BC0(self, &objData->unk28);
            if ((objData->unk58 == 0) && (objData->unk5C <= 0) && player && (vec3_distance(&self->globalPosition, &player->globalPosition) < 200.0f)) {
                gDLL_6_AMSFX->vtbl->play(self, objData->unk4C[objData->unk5E], MAX_VOLUME, &objData->unk58, NULL, 0, NULL);
                
                if (rand_next(0, 100) < 50) {
                    objData->unk5E++;
                } else {
                    objData->unk5E--;
                }

                if (objData->unk5E < 0) {
                    objData->unk5E = 1;
                } else if (objData->unk5E >= 2) {
                    objData->unk5E = 0;
                }
                objData->unk5C = rand_next(1000, 5000);
            }

            if (objData->unk5C > 0) {
                objData->unk5C -= gUpdateRate;
            }
            objAnimAdvance(self, 0.005f, gUpdateRate, NULL);
        }
    } else {
        self->unkAF |= 8;
        if (self->seqSlot == SEQSLOT_NONE) {
            gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        }
    }
}

// offset: 0x590 | func: 2 | export: 2
void dll_376_update(Object *self) { }

// offset: 0x59C | func: 3 | export: 3
void dll_376_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    s32 boneIdx;
    ModelInstance* modelInstance;
    MtxF* mtx;
    ObjDef* objDef;
    DLL376_Data* objData;
 
    objData = self->data;  
    
    if (mainGetBits(BIT_50)) {
        if (objData->unk0 && track_obj_vis_check(objData->unk0)) {
            objprintDrawModel(objData->unk0, gdl, mtxs, vtxs, pols, 1.0f);
        }
        return;
    }
    
    if (mainGetBits(BIT_4D) && visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        if (objData->unk0 && track_obj_vis_check(objData->unk0)) {
            objprintDrawModel(objData->unk0, gdl, mtxs, vtxs, pols, 1.0f);
        }
        return;
    }
    
    if (!objData || !objData->unk0){
        return;
    }
        
    if (objData->unk5F == 0) {
        if (!visibility) {
            return;
        }
        objprintDrawModel(objData->unk0, gdl, mtxs, vtxs, pols, 1.0f);
        objDef = objData->unk0->def;
        modelInstance = objData->unk0->modelInsts[objData->unk0->modelInstIdx];
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
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    } else {
        if (track_obj_vis_check(objData->unk0)) {
            objprintDrawModel(objData->unk0, gdl, mtxs, vtxs, pols, 1.0f);
        }
        if (visibility) {
            objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0x8C0 | func: 4 | export: 4
void dll_376_free(Object *self, s32 a1) { }

// offset: 0x8D0 | func: 5 | export: 5
u32 dll_376_get_model_flags(Object* self) {
    return MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x8E0 | func: 6 | export: 6
u32 dll_376_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL376_Data);
}

// offset: 0x8F4 | func: 7
s32 dll_376_func_8F4(Object* self, Object* arg1, AnimObj_Data* arg2, s32 arg3) {
    DLL376_Data* objData;
    BoneDust_Setup* dustSetup;

    objData = self->data;
    if (objData->unk60 != 0) {
        return 0;
    } else if (arg2->lastMessage == 2) {
        objData->unk60 = 1;
       
        
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
        objSetupObject((ObjSetup*)dustSetup, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, NULL);
    }
    return 0;
}

// offset: 0x9F8 | func: 8
void dll_376_func_9F8(Object* self) {
    s16* temp;
    temp = objExpr_func_80034804(self, 0);
    temp[0] = 0;
    temp[1] = 0;
}

// offset: 0xA3C | func: 9
void dll_376_func_A3C(Object* self, Object* player) {
    DLL376_Data *objData = self->data;
    
    objExpr_func_80032CF8(self, player, &objData->unk4, 0x23);
}
