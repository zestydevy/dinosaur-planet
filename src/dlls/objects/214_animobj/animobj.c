#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "game/objects/object.h"

#include "dlls/objects/214_animobj.h"
#include "types.h"

// offset: 0x0 | ctor
void animobj_ctor(void *dll) { }

// offset: 0xC | dtor
void animobj_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void animobj_setup(Object *self, AnimObjSetup *setup, s32 arg2) {
    AnimObj_Data *objdata;

    obj_set_update_priority(self, 0x64);
    objdata = self->data;
    if (!setup->sequenceIdBitfield){
    }

    objdata->unk76 = setup->unk1A;
    objdata->unk7A = -1;
    objdata->unk24 = 1.0f / (setup->unk24 + 1.0f);
    objdata->unk28 = -1;

    if (self->unkDC == 0 && setup->sequenceIdBitfield != 1){
        gDLL_3_Animation->vtbl->func6(objdata, setup);
        self->unkDC = setup->sequenceIdBitfield + 1;
    } else {
        if (self->unkDC != 0 && (setup->sequenceIdBitfield + 1 != self->unkDC)){
            gDLL_3_Animation->vtbl->func8(objdata);
            if (setup->sequenceIdBitfield != -1){
                gDLL_3_Animation->vtbl->func6(objdata, setup);
            }
            self->unkDC = setup->sequenceIdBitfield + 1;
        }
    }

    if (self->ptr0x64){
        self->ptr0x64->unk3A = 0x64;
        self->ptr0x64->unk3B = 0x96;
    }
}

// offset: 0x19C | func: 1 | export: 1
void animobj_control(Object *self) {
    s32 index;
    AnimObj_Data *objdata;
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

    index = gDLL_3_Animation->vtbl->func4(self, gUpdateRate);
    if (!index || self->unkB4 != -2){
        return;
    }

    objdata = self->data;
    new_var = objdata->unk63;
    matchObject = 0;
    objects = get_world_objects(&index, &count);
    matches = 0;
    for (index = 0; index < count; index++){
        object = objects[index];
        if (new_var == object->unkB4){
            matchObject = object;
        }

        if (object->unkB4 == -2 && object->group == 0x10) {
            objdata = object->data;    
            if (new_var == objdata->unk63){
                matches++;
            }
        }
    }

    if (matches < 2 && matchObject && matchObject->unkB4 != -1){
        matchObject->unkB4 = -1;
        gDLL_3_Animation->vtbl->func18(new_var);
    }
    self->unkB4 = -1;
}

// offset: 0x318 | func: 2 | export: 2
void animobj_update(Object *self) { }

// offset: 0x324 | func: 3 | export: 3
void animobj_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x378 | func: 4 | export: 4
void animobj_free(Object *self, s32 arg1) {
    AnimObj_Data *objdata;
    s32 i;

    objdata = self->data;
    gDLL_3_Animation->vtbl->func8(objdata);
    for (i = 0; i < 4; i++){
        if (objdata->unk34[i]){
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk34[i]);
        }
    }

    gDLL_5_AMSEQ2->vtbl->func1(self, 0xFFFF, 0, 0, 0);
    if (objdata->unk30 != 0){
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk30);
    }
}

// offset: 0x484 | func: 5 | export: 5
u32 animobj_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_8;
}

// offset: 0x494 | func: 6 | export: 6
u32 animobj_get_data_size(Object *self, s32 arg1) {
    return sizeof(AnimObj_Data);
}
