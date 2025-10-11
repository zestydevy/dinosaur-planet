#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "game/objects/object.h"

#include "dlls/objects/214_animobj.h"

// offset: 0x0 | ctor
void animobj_ctor(void *dll) {
}

// offset: 0xC | dtor
void animobj_dtor(void *dll) {
}

// offset: 0x18 | func: 0 | export: 0
void animobj_create(Object *self, AnimObjSetup *setup, s32 arg2) {
    AnimObjState *state;

    obj_set_update_priority(self, 0x64);
    state = self->state;
    if (!setup->sequenceIdBitfield){
    }

    state->unk76 = setup->unk1A;
    state->unk7A = -1;
    state->unk24 = 1.0f / (setup->unk24 + 1.0f);
    state->unk28 = -1;

    if (self->unk0xdc == 0 && setup->sequenceIdBitfield != 1){
        gDLL_3_Animation->vtbl->func6((s32)state, (ObjSetup*)setup);
        self->unk0xdc = setup->sequenceIdBitfield + 1;
    } else {
        if (self->unk0xdc != 0 && (setup->sequenceIdBitfield + 1 != self->unk0xdc)){
            gDLL_3_Animation->vtbl->func8((s32)state);
            if (setup->sequenceIdBitfield != -1){
                gDLL_3_Animation->vtbl->func6((s32)state, (ObjSetup*)setup);
            }
            self->unk0xdc = setup->sequenceIdBitfield + 1;
        }
    }

    if (self->ptr0x64){
        self->ptr0x64->unk3A = 0x64;
        self->ptr0x64->unk3B = 0x96;
    }
}

// offset: 0x19C | func: 1 | export: 1
void animobj_update(Object *self) {
    s32 index;
    AnimObjState *state;
    AnimObjSetup *setup;
    s8 new_var;
    Object *object;
    Object *matchObject;
    s32 matches;
    s32 count;
    Object **objects;

    setup = (AnimObjSetup *) self->setup;
    if (!setup || setup->sequenceIdBitfield == -1){
        return;
    }

    index = gDLL_3_Animation->vtbl->func4(self, delayByte);
    if (!index || self->unk0xb4 != -2){
        return;
    }

    state = self->state;
    new_var = state->unk63;
    matchObject = 0;
    objects = get_world_objects(&index, &count);
    matches = 0;
    for (index = 0; index < count; index++){
        object = objects[index];
        if (new_var == object->unk0xb4){
            matchObject = object;
        }

        if (object->unk0xb4 == -2 && object->group == 0x10) {
            state = object->state;    
            if (new_var == state->unk63){
                matches++;
            }
        }
    }

    if (matches < 2 && matchObject && matchObject->unk0xb4 != -1){
        matchObject->unk0xb4 = -1;
        gDLL_3_Animation->vtbl->func18(new_var);
    }
    self->unk0xb4 = -1;
}

// offset: 0x318 | func: 2 | export: 2
void animobj_func_318(Object *self) {
}

// offset: 0x324 | func: 3 | export: 3
void animobj_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x378 | func: 4 | export: 4
void animobj_destroy(Object *self, s32 arg1) {
    AnimObjState *state;
    s32 i;

    state = self->state;
    gDLL_3_Animation->vtbl->func8((s32)state);
    for (i = 0; i < 4; i++){
        if (state->unk34[i]){
            gDLL_6_AMSFX->vtbl->func_A1C(state->unk34[i]);
        }
    }

    gDLL_5_AMSEQ2->vtbl->func1(self, 0xFFFF, 0, 0, 0);
    if (state->unk30 != 0){
        gDLL_6_AMSFX->vtbl->func_A1C(state->unk30);
    }
}

// offset: 0x484 | func: 5 | export: 5
u32 animobj_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_8;
}

// offset: 0x494 | func: 6 | export: 6
u32 animobj_get_state_size(Object *self, s32 arg1) {
    return sizeof(AnimObjState);
}
