#include "common.h"
#include "sys/objtype.h"

typedef struct {
    Object *unk0;
} DLL499_Data;

// DLL 449 and object types 0x3E share this? maybe?
typedef struct {
    ObjSetup base;
    u8 _unk18[0x1B - 0x18];
    u8 unk1B;
} UnkObjType3E_Setup;

// offset: 0x0 | ctor
void dll_499_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_499_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_499_setup(Object* self, ObjSetup* setup, s32 arg2) {
    self->srt.flags |= 0x4000;
    self->unkB0 |= 0x2000;
    obj_add_object_type(self, OBJTYPE_61);
}

// offset: 0x74 | func: 1 | export: 1
void dll_499_control(Object* self) {
    DLL499_Data* objdata;
    s32 numObjs;
    Object* temp_a2;
    Object** objList;
    f32 dist;
    s32 i;
    UnkObjType3E_Setup *setup;
    UnkObjType3E_Setup *temp2;

    dist = 3.4028235e38f;
    objdata = self->data;
    if (objdata->unk0 != NULL) {
        self->srt.transl.x = objdata->unk0->srt.transl.x;
        self->srt.transl.y = objdata->unk0->srt.transl.y;
        self->srt.transl.z = objdata->unk0->srt.transl.z;
        self->srt.yaw = objdata->unk0->srt.yaw;
        obj_get_nearest_type_to_excluding_self(OBJTYPE_61, self, &dist);
        if (objdata->unk0->opacity < 128) {
            func_800267A4(self);
        } else {
            func_8002674C(self);
        }
        if ((objdata->unk0->opacity < 128) || (dist < 120.0f)) {
            self->unkB0 |= 0x4;
        } else {
            self->unkB0 &= ~0x4;
        }
        return;
    }
    objList = obj_get_all_of_type(OBJTYPE_62, &numObjs);
    setup = (UnkObjType3E_Setup*)self->setup;
    for (i = 0; i < numObjs; i++) {
        if (self != objList[i]) {
            temp2 = (UnkObjType3E_Setup*)objList[i]->setup;
            if (setup->unk1B == temp2->unk1B) {
                objdata->unk0 = objList[i];
                break;
            }
        }
    }
}


// offset: 0x21C | func: 2 | export: 2
void dll_499_update(Object *self) { }

// offset: 0x228 | func: 3 | export: 3
void dll_499_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x240 | func: 4 | export: 4
void dll_499_free(Object* self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_61);
}

// offset: 0x280 | func: 5 | export: 5
u32 dll_499_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x290 | func: 6 | export: 6
u32 dll_499_get_data_size(Object* self, u32 a1) {
    return 4U;
}
