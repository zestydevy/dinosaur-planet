#include "common.h"
#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/objlib.h"

// offset: 0x0 | ctor
void SCAnimObj_ctor(void *dll) { }

// offset: 0xC | dtor
void SCAnimObj_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SCAnimObj_setup(Object* self, AnimObj_Setup* objSetup, s32 arg2) {
    AnimObj_Data* objData;

    obj_set_update_priority(self, OBJPRIORITY_ANIM);

    objData = self->data;
    
    objData->unk76 = objSetup->unk1A;
    objData->unk7A = -1;
    objData->unk24 = 1.0f / (objSetup->unk24 + 1.0f);
    objData->unk28 = -1;
    
    self->unkE0 = 0;
    
    if ((self->unkDC == 0) && (objSetup->sequenceIdBitfield != 1)) {
        gDLL_3_Animation->vtbl->func6(objData, objSetup);
        self->unkDC = objSetup->sequenceIdBitfield + 1;
    } else if ((self->unkDC != 0) && ((objSetup->sequenceIdBitfield + 1) != self->unkDC)) {
        gDLL_3_Animation->vtbl->func8(objData);
        if (objSetup->sequenceIdBitfield != -1) {
            gDLL_3_Animation->vtbl->func6(objData, objSetup);
        }
        self->unkDC = objSetup->sequenceIdBitfield + 1;
    }
    
    if (self->shadow != NULL) {
        self->shadow->distFadeMaxOpacity = 100;
        self->shadow->distFadeMinOpacity = 150;
    }
}

// offset: 0x1A0 | func: 1 | export: 1
void SCAnimObj_control(Object* self) {
    s32 index;
    Object** objects;
    Object* obj;
    s8 temp;
    s32 aIndex;
    Object* matchObj;
    AnimObj_Data* objData;
    s32 count;
    s32 matchCount;
    AnimObj_Setup* objSetup;

    objSetup = (AnimObj_Setup*)self->setup;
    objData = self->data;
    
    if ((objSetup == NULL) || (objSetup->sequenceIdBitfield == -1)) {
        return;
    }
        
    index = gDLL_3_Animation->vtbl->func4(self, gUpdateRateMirror);
    if ((index != 0) && (self->unkB4 == -2)) {
        temp = objData->unk63;
        matchObj = NULL;
        
        objects = get_world_objects(&index, &count);
        matchCount = 0;

        for (index = 0; index < count; index++) {
            obj = objects[index];

            if (temp == obj->unkB4) {
                matchObj = obj;
            }
            
            if ((obj->unkB4 == -2) && (obj->group == GROUP_UNK16)) {
                objData = obj->data;
                if (temp == objData->unk63) {
                    matchCount++;
                }  
            } 
        }

        
        if ((matchCount < 2) && (matchObj != NULL) && (matchObj->unkB4 != -1)) {
            matchObj->unkB4 = -1;
            gDLL_3_Animation->vtbl->func18(temp);
        }
        self->unkB4 = -1;
    }


    for (aIndex = 0; aIndex < objData->unk98; aIndex++) {
        switch (objData->unk8E[aIndex]) {
        case 0:
            self->unkE0 |= 1;
            if (0) {} //fake
            break;
        case 1:
            self->unkE0 &= ~1;
            break;
        case 2:
            self->unkE0 |= 2;
            break;
        case 3:
            self->unkE0 &= ~2;
            break;
        case 4:
            self->unkE0 |= 4;
            break;
        }
    }
}

// offset: 0x3F8 | func: 2 | export: 2
void SCAnimObj_update(Object *self) { }

// offset: 0x404 | func: 3 | export: 3
void SCAnimObj_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    static SRT sTransform;
    s32 i;

    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        
        if (self->unkE0 & 7) {
            func_80031F6C(self, 0, &sTransform.transl.x, &sTransform.transl.y, &sTransform.transl.z, 0);
        }
        
        if (self->unkE0 & 1) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_425, &sTransform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, 0);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_425, &sTransform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, 0);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_425, &sTransform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, 0);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_426, &sTransform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, 0);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_426, &sTransform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, 0);
        }
        
        if (self->unkE0 & 2) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_427, &sTransform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, 0);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_427, &sTransform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, 0);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_427, &sTransform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, 0);
        }
        
        if (self->unkE0 & 4) {
            for (i = 0; i < 20; i++) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_427, &sTransform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, 0);   
            }
            
            for (i = 0; i < 7; i++) {
                sTransform.transl.x += rand_next(-50, 50) / 10.0f;
                sTransform.transl.z += rand_next(-50, 50) / 10.0f;
                
                gDLL_24_Waterfx->vtbl->func_174C(sTransform.transl.x, self->srt.transl.y, sTransform.transl.z, 4.0f);
                gDLL_24_Waterfx->vtbl->func_1CC8(sTransform.transl.x, self->srt.transl.y, sTransform.transl.z, 0, 0.0f, 3);
            }
            
            gDLL_6_AMSFX->vtbl->play(self, SOUND_3D8_Water_Splash, MAX_VOLUME, 0, 0, 0, 0);
            
            self->unkE0 &= ~4;
        }
    }
}

// offset: 0x824 | func: 4 | export: 4
void SCAnimObj_free(Object* self, s32 arg1) {
    AnimObj_Data* objData;
    s32 i;

    objData = self->data;
    
    gDLL_3_Animation->vtbl->func8(objData);

    for (i = 0; i < 4; i++) {
        if (objData->unk34[i]) {
            gDLL_6_AMSFX->vtbl->stop(objData->unk34[i]);
        }
    }
    
    gDLL_5_AMSEQ2->vtbl->free(self, 0xFFFF, 0, 0, 0);
    
    if (objData->unk30 != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->unk30);
    }
}

// offset: 0x930 | func: 5 | export: 5
u32 SCAnimObj_get_model_flags(Object *self) {
    return MODFLAGS_8 | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0x940 | func: 6 | export: 6
u32 SCAnimObj_get_data_size(Object *self, u32 a1) {
    return sizeof(AnimObj_Data);
}
