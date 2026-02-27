#include "common.h"

// offset: 0x0 | ctor
void dll_260_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_260_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
typedef struct {
    s16 unk0;
    s16 pad2;
    s16 unk4;
    s16 unk6;
    f32 unk8;
    f32 unkC;
    s16 unk10;
    s16 unk12;
}DLL260_Setup;

void dll_260_setup(Object* self, ObjSetup* setup, s32 arg2) {
    DLL260_Setup* objdata; 

    objdata = self->data;
    objdata->unk0 = rand_next(-0x8000, 0x7FFF);
    objdata->unkC = ((f32) rand_next(0xFA0, 0x1388) * 0.01f);
    objdata->unk4 = rand_next(-0x8000, 0x7FFF);
    objdata->unk8 = 0.0f;
    objdata->unk6 = rand_next(0xE6, 0x1F4);
    objdata->unk10 = 0;
    objdata->unk12 = 0xA;
    self->opacity = 0xFF;
    func_800267A4(self);
    if (self->shadow != NULL) {
        self->shadow->flags |= 0x810;
        self->shadow->maxDistScale = self->shadow->scale * 2.5f;
    }
}

// offset: 0x13C | func: 1
void dll_260_func_13C(Object* arg0) {
    ObjSetup* setup;
    Object* temp_v0_3;
    s32 i;

    i = 6;
    while (i--) {
        setup = obj_alloc_create_info(0x24, 0x482);
        setup->x = arg0->srt.transl.x;
        setup->y = arg0->srt.transl.y;
        setup->z = arg0->srt.transl.z;
        setup->loadFlags = 1;
        setup->byte5 = 1;
        setup->byte6 = 0xFF;
        setup->fadeDistance = 0xFF;
        temp_v0_3 = obj_create(setup, 5U, -1, -1, NULL);
        if (temp_v0_3 != NULL) {
            temp_v0_3->srt.pitch = 0;
            temp_v0_3->srt.yaw = 0;
            temp_v0_3->speed.f[0] = (f32) rand_next(-0x32, 0x32) * 0.01f;
            temp_v0_3->speed.f[1] = (f32) rand_next(0, 0x1E) * 0.01f;
            temp_v0_3->speed.f[2] = (f32) rand_next(-0x32, 0x32) * 0.01f;
            temp_v0_3->unkC4 = arg0;
        }
    }
}

// offset: 0x2A4 | func: 2 | export: 1
void dll_260_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/260_Pollen/dll_260_control.s")

// offset: 0x5C0 | func: 3 | export: 2
void dll_260_update(Object* self) {
    ObjectHitInfo* temp_v0;

    temp_v0 = self->objhitInfo;
    if (temp_v0->unk9D != 0) {
        self->srt.transl.f[0] = temp_v0->unk34;
        self->srt.transl.f[1] = temp_v0->unk38;
        self->srt.transl.f[2] = temp_v0->unk3C;
        self->opacity = 0;
        self->speed.f[0] = 0.0f;
        self->speed.f[1] = 0.0f;
        self->speed.f[2] = 0.0f;
        func_800267A4(self);
    }
}

// offset: 0x634 | func: 4 | export: 3
void dll_260_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x688 | func: 5 | export: 4
void dll_260_free(Object* self, s32 a1) {
    camera_disable_y_offset();
}

// offset: 0x6C8 | func: 6 | export: 5
u32 dll_260_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x6D8 | func: 7 | export: 6
u32 dll_260_get_data_size(Object* self, u32 a1) {
    return 0x14U;
}

