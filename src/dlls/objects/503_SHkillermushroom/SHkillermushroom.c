#include "common.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "dlls/objects/210_player.h"


typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s32 _unk14;
    s32 _unk18;
    s32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    s32 _unk30;
    s32 unk34;
    s16 unk38;
    s16 unk3A;
    s32 unk3C;
} DLL503_Data;
typedef struct {
    ObjSetup base;
    u16 _unk18;
    u16 unk1A;
    s16 _unk1C;
    u8 _unk1E;
    u8 unk1F;
}DLL503_Setup;

static void dll_503_func_DE4(Object* arg0, DLL503_Data* arg1, s32 arg2);


/*0x0*/ static u32 _data_0[] = {
    0x00000000, 0x00040001, 0x00020003, 0x00050006, 0x00060009, 0x00000000
};
/*0x18*/ static u32 _data_18[] = {
    0x00000000, 0x00000000, 0x3c03126f, 0x3ccccccd, 0x3c9374bc, 0x3c75c28f, 0x3bc49ba6, 0x3c03126f, 
    0x3ba3d70a, 0x3c449ba6, 0x3ba3d70a, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_503_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_503_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_503_setup(Object* self, DLL503_Setup* setup, s32 arg2) {
    ObjectShadow* temp_v0;
    DLL503_Data* objdata;

    objdata = self->data;
    objdata->unk0 = 0;
    objdata->unk2C = 0; 
    objdata->unkC = self->srt.scale;
    objdata->unk38 =  setup->unk1A;
    if (objdata->unk38 < 0x708) {
        objdata->unk38 = 0x708U;
    }
    self->srt.transl.f[1] = setup->base.y - 2.0f;
    temp_v0 = self->shadow;
    if (temp_v0 != NULL) {
        temp_v0->flags |= 0x810;
    }
    if (arg2 == 0) {
        dll_503_func_DE4(self, objdata, 0);
    }
}

// offset: 0xC4 | func: 1 | export: 1
void dll_503_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/503_SHkillermushroom/dll_503_control.s")

// offset: 0xCDC | func: 2 | export: 2
void dll_503_update(Object *self) { }

// offset: 0xCE8 | func: 3 | export: 3
void dll_503_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DLL503_Data* objdata;

    objdata = self->data;
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_80031F6C(self, 0, &objdata->unk20, &objdata->unk24, &objdata->unk28, 0);
    }
}

// offset: 0xD70 | func: 4 | export: 4
void dll_503_free(Object* self, s32 a1) {
    gDLL_13_Expgfx->vtbl->func4(self);
}

// offset: 0xDB8 | func: 5 | export: 5
u32 dll_503_get_model_flags(Object* self) {
    DLL503_Setup* setup;

    setup = (DLL503_Setup*)self->setup;
    return (setup->unk1F << 0xB) | 0x400;
}
// offset: 0xDD0 | func: 6 | export: 6
u32 dll_503_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL503_Data);
}

// offset: 0xDE4 | func: 7
static void dll_503_func_DE4(Object* arg0, DLL503_Data* arg1, s32 arg2) {
    ObjSetup* setup;
    f32 temp_fv0;

    setup = arg0->setup;
    arg0->srt.roll = rand_next(-0x5DC, 0x5DC);
    arg0->srt.pitch = rand_next(-0x5DC, 0x5DC);
    arg0->srt.yaw = rand_next(-0x5DC, 0x5DC);
    arg0->opacity = 0xFF;
    arg0->srt.flags &= 0xBFFF;
    arg0->srt.transl.f[0] = setup->x;
    arg0->srt.transl.f[1] = setup->y;
    arg0->srt.transl.f[2] = setup->z;
    if (arg2 != 0) {
        arg0->srt.scale = 0.00001f;
        arg1->unk0 = 0.0f;
        arg1->unk8 = (rand_next(0, 0x64) + 200.0f);
        temp_fv0 = (rand_next(-0x64, 0x64) * 0.001f) + arg1->unkC;
        arg1->unk4 = temp_fv0;
        arg1->unk10 =(temp_fv0 / arg1->unk8);
    }
    func_8002674C(arg0);
    func_800264D0(arg0);
}
