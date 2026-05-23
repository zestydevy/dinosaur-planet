#include "common.h"
#include "sys/gfx/animseq.h"
#include "sys/objtype.h"
#include "dlls/objects/541_DIMexplosion.h"

// offset: 0x0 | ctor
void DFbarrelanim_ctor(void *dll) { }

// offset: 0xC | dtor
void DFbarrelanim_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFbarrelanim_setup(Object* self, AnimObj_Setup* objSetup, s32 reset) {
    AnimObj_Data* objData;

    obj_set_update_priority(self, 100);
    objData = self->data;
    objData->eventGamebit = objSetup->unk1A;
    objData->unk7A = -1;

    objData->unk24 = 1.0f / (objSetup->unk24 + 1.0f);
    objData->unk28 = -1;

    if ((self->unkDC == 0) && (objSetup->sequenceIdBitfield != 1)) {
        gDLL_3_Animation->vtbl->init_curve(objData, objSetup);
        self->unkDC = objSetup->sequenceIdBitfield + 1;
    } else if ((self->unkDC) && ((objSetup->sequenceIdBitfield + 1) != self->unkDC)) {
        gDLL_3_Animation->vtbl->free_curve(objData);
        if (objSetup->sequenceIdBitfield != -1) {
            gDLL_3_Animation->vtbl->init_curve(objData, objSetup);
        }
        self->unkDC = objSetup->sequenceIdBitfield + 1;
    }
    
    if (self->shadow != NULL) {
        self->shadow->distFadeMaxOpacity = 100;
        self->shadow->distFadeMinOpacity = 150;
    }
}

// offset: 0x19C | func: 1 | export: 1
void DFbarrelanim_control(Object* self) {
    s32 index;
    AnimObj_Setup* objSetup;
    ObjSetup* explosion;
    Object* obj;
    Object* matchObj;
    Object** objects;
    s32 matches;
    s32 count;
    s32 i;
    s32 seqSlot;
    AnimObj_Data* objData;

    objSetup = (AnimObj_Setup*)self->setup;
    if ((objSetup == NULL) || (objSetup->sequenceIdBitfield == -1)) {
        return;
    }
    
    objData = self->data;

    index = gDLL_3_Animation->vtbl->tick_obj(self, gUpdateRate);
    if (index != 0) {
        matchObj = NULL;
        if (self->seqSlot == SEQSLOT_ANIMOBJ) {
            seqSlot = objData->seqSlot;
            objects = get_world_objects(&index, &count);

            for (matches = 0, index = 0; index < count; index++) {
                obj = objects[index];
                if (seqSlot == obj->seqSlot) {
                    matchObj = obj;
                }
                
                if (obj->seqSlot == SEQSLOT_ANIMOBJ && obj->controlNo == OBJTYPE_Door) {
                    objData = obj->data;    
                    if (seqSlot == objData->seqSlot) {
                        matches++;
                    }
                }
            }
            
            if ((matches < 2) && (matchObj != NULL) && (matchObj->seqSlot != SEQSLOT_NONE)) {
                matchObj->seqSlot = SEQSLOT_NONE;
                gDLL_3_Animation->vtbl->end_obj_sequence(seqSlot);
            }
            
            self->seqSlot = SEQSLOT_NONE;
            return;
        }
    }
    
    for (i = 0; i < objData->messageCount; i++) {        
        if (objData->messages[i] == 1) {
            explosion = obj_alloc_setup(sizeof(DIMExplosion_Setup), OBJ_DIMExplosion);
            self->srt.transl.y += 12.0f;
            explosion->x = self->srt.transl.x;
            explosion->y = self->srt.transl.y + 8.0f;
            explosion->z = self->srt.transl.z;
            obj_create(explosion, 5, self->mapID, -1, self->parent);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_355, NULL, 0, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_352, NULL, 0, -1, NULL);
            self->srt.transl.y -= 12.0f;
        }
    }
}

// offset: 0x454 | func: 2 | export: 2
void DFbarrelanim_update(Object *self) { }

// offset: 0x460 | func: 3 | export: 3
void DFbarrelanim_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4B4 | func: 4 | export: 4
void DFbarrelanim_free(Object* self, s32 arg1) {
    AnimObj_Data* objData;
    s32 i;

    objData = self->data;
    gDLL_3_Animation->vtbl->free_curve(objData);

    for (i = 0; i < 4; i++) {
        if (objData->sfxHandles[i]) {
            gDLL_6_AMSFX->vtbl->stop(objData->sfxHandles[i]);
        }
    }
    
    gDLL_5_AMSEQ2->vtbl->free(self, -1, 0, 0, 0);
    if (objData->unk30 != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->unk30);
    }
}

// offset: 0x5C0 | func: 5 | export: 5
u32 DFbarrelanim_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_8;
}

// offset: 0x5D0 | func: 6 | export: 6
u32 DFbarrelanim_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(AnimObj_Data);
}
