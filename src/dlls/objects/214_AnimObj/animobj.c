#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "game/objects/object.h"
#include "sys/gfx/animseq.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "types.h"

// offset: 0x0 | ctor
void AnimObj_ctor(void* dll) { }

// offset: 0xC | dtor
void AnimObj_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void AnimObj_obj_Setup(Object* self, AnimObj_Setup* setup, s32 reset) {
    AnimObj_Data *objdata;

    objSetPriority(self, OBJPRIORITY_ANIM);
    objdata = self->data;
    if (!setup->sequenceIdBitfield){
    }

    objdata->eventGamebit = setup->unk1A;
    objdata->unk7A = -1;
    objdata->unk24 = 1.0f / (setup->camEaseDuration + 1.0f);
    objdata->unk28 = -1;

    if (self->unkDC == 0 && setup->sequenceIdBitfield != 1){
        dll_anim->init_curve(objdata, setup);
        self->unkDC = setup->sequenceIdBitfield + 1;
    } else {
        if (self->unkDC != 0 && (setup->sequenceIdBitfield + 1 != self->unkDC)){
            dll_anim->free_curve(objdata);
            if (setup->sequenceIdBitfield != -1){
                dll_anim->init_curve(objdata, setup);
            }
            self->unkDC = setup->sequenceIdBitfield + 1;
        }
    }

    if (self->shadow){
        self->shadow->distFadeMaxOpacity = 100;
        self->shadow->distFadeMinOpacity = 150;
    }
}

// offset: 0x19C | func: 1 | export: 1
void AnimObj_obj_Control(Object* self) {
    s32 index;
    AnimObj_Data *objdata;
    AnimObj_Setup *setup;
    s8 new_var;
    Object *object;
    Object *matchObject;
    s32 matches;
    s32 count;
    Object **objects;

    setup = (AnimObj_Setup*) self->setup;
    if (!setup || setup->sequenceIdBitfield == -1) {
        return;
    }

    index = dll_anim->tick_obj(self, gUpdateRate);
    if (!index || self->seqSlot != SEQSLOT_ANIMOBJ) {
        return;
    }

    objdata = self->data;
    new_var = objdata->seqSlot;
    matchObject = 0;
    objects = objGetObjects(&index, &count);
    matches = 0;
    for (index = 0; index < count; index++) {
        object = objects[index];
        if (new_var == object->seqSlot) {
            matchObject = object;
        }

        if (object->seqSlot == SEQSLOT_ANIMOBJ && object->controlNo == OBJCONTROL_AnimObj) {
            objdata = object->data;    
            if (new_var == objdata->seqSlot) {
                matches++;
            }
        }
    }

    if (matches < 2 && matchObject && matchObject->seqSlot != SEQSLOT_NONE) {
        matchObject->seqSlot = SEQSLOT_NONE;
        dll_anim->end_obj_sequence(new_var);
    }
    self->seqSlot = SEQSLOT_NONE;
}

// offset: 0x318 | func: 2 | export: 2
void AnimObj_obj_Update(Object* self) { }

// offset: 0x324 | func: 3 | export: 3
void AnimObj_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x378 | func: 4 | export: 4
void AnimObj_obj_Free(Object* self, s32 onlySelf) {
    AnimObj_Data *objdata;
    s32 i;

    objdata = self->data;
    dll_anim->free_curve(objdata);
    for (i = 0; i < 4; i++){
        if (objdata->sfxHandles[i]){
            dll_amSfx->Stop(objdata->sfxHandles[i]);
        }
    }

    gDLL_5_AMSEQ2->vtbl->free(self, 0xFFFF, 0, 0, 0);
    if (objdata->unk30 != 0){
        dll_amSfx->Stop(objdata->unk30);
    }
}

// offset: 0x484 | func: 5 | export: 5
u32 AnimObj_obj_GetModelFlags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_8;
}

// offset: 0x494 | func: 6 | export: 6
u32 AnimObj_obj_GetDataSize(Object* self, s32 offsetAddr) {
    return sizeof(AnimObj_Data);
}
