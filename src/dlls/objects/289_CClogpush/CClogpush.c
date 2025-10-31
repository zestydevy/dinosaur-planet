#include "sys/objtype.h"
#include "sys/main.h"
#include "functions.h"
#include "dll.h"

typedef struct {
/*00*/ u8 state;
/*01*/ u8 unk1;
/*04*/ Object *obj;
} CClogpush_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebit1;
/*1A*/ s16 gamebit2;
/*1C*/ s16 gamebit3;
/*1E*/ s16 unk1E;
/*20*/ u8 unk20;
/*21*/ u8 objType; // 11
/*22*/ u8 objectSeqIndex;
/*23*/ u8 flags;
} CClogpush_Setup;

typedef enum {
    STATE_0,
    STATE_1,
    STATE_2,
    STATE_3,
    STATE_4
} CClogpush_State;

static void CClogpush_func_4E0(Object *self, Object *arg1);

// offset: 0x0 | ctor
void CClogpush_ctor(void *dll) { }

// offset: 0xC | dtor
void CClogpush_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CClogpush_setup(Object *self, CClogpush_Setup *objsetup, s32 arg2) {
    CClogpush_Data *objdata;

    objdata = self->data;
    objdata->state = STATE_0;
    objdata->obj = NULL;
    self->unkB0 |= 0x4000;
}

// offset: 0x40 | func: 1 | export: 1
void CClogpush_control(Object *self) {
    f32 distance;
    CClogpush_Setup *setup;
    CClogpush_Data *objdata;

    distance = 100.0f;
    objdata = self->data;
    setup = (CClogpush_Setup*)self->setup;

    if (!objdata->obj) {
        objdata->obj = obj_get_nearest_type_to(setup->objType, self, &distance);

        if (!objdata->obj)
            return;

        if (setup->gamebit2 == -1) {
            objdata->unk1 = FALSE;
        } else {
            objdata->unk1 = main_get_bits(setup->gamebit2);
        }
        if (objdata->unk1 && setup->unk1E != -1) {
            objdata->state = STATE_1;
        } else {
            objdata->state = STATE_2;
        }
    }

    self->srt.transl.x = objdata->obj->srt.transl.x;
    self->srt.transl.y = objdata->obj->srt.transl.y;
    self->srt.transl.z = objdata->obj->srt.transl.z;
    self->srt.yaw = objdata->obj->srt.yaw;
    self->srt.roll = objdata->obj->srt.roll;
    self->srt.pitch = objdata->obj->srt.pitch;

    switch (objdata->state) {
    case STATE_1:
        CClogpush_func_4E0(self, objdata->obj);
        gDLL_3_Animation->vtbl->func20(self, setup->unk1E);
        gDLL_3_Animation->vtbl->func17(setup->objectSeqIndex, self, setup->unk20);
        objdata->state = STATE_4;
        break;
    case STATE_2:
        if (objdata->unk1 && !(setup->flags & 1)) {
            CClogpush_func_4E0(self, objdata->obj);
            objdata->state = STATE_4;
            break;
        } else if (setup->gamebit1 != -1 && !main_get_bits(setup->gamebit1)) {
            CClogpush_func_4E0(self, objdata->obj);
            objdata->state = STATE_3;
            break;
        }

        if ((self->unkAF & 0x1) && (( (setup->gamebit3 == -1)) || ((gDLL_1_UI->vtbl->func_DF4(setup->gamebit3) != 0)))) {
            if (setup->flags & 2) {
                main_set_bits(setup->gamebit1, 0);
            }
            if (setup->gamebit2 != -1) {
                main_set_bits(setup->gamebit2, 1);
            }
            self->unkAF |= 0x8;
            objdata->unk1 = TRUE;
            gDLL_3_Animation->vtbl->func17(setup->objectSeqIndex, self, -1);
            break;
        }
        objdata->obj->unkAF |= 0x20;
        self->unkAF &= ~0x8;
        break;
    case STATE_3:
        if (main_get_bits(setup->gamebit1)) {
            objdata->state = STATE_2;
        }
        break;
    case STATE_4:
        break;
    }
}

// offset: 0x400 | func: 2 | export: 2
void CClogpush_update(Object *self) {
    if (self->def->flags & 1 && self->unk74) {
        func_80036438(self);
    }
}

// offset: 0x458 | func: 3 | export: 3
void CClogpush_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4AC | func: 4 | export: 4
void CClogpush_free(Object *self, s32 a1) { }

// offset: 0x4BC | func: 5 | export: 5
u32 CClogpush_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x4CC | func: 6 | export: 6
u32 CClogpush_get_data_size(Object *self, u32 a1) {
    return sizeof(CClogpush_Data);
}

// offset: 0x4E0 | func: 7
void CClogpush_func_4E0(Object *self, Object *arg1) {
    arg1->unkAF &= ~0x20;
    self->unkAF |= 0x8;
}
