#include "common.h"
#include "sys/objmsg.h"
#include "sys/objanim.h"
#include "sys/objtype.h"


typedef struct {
    u8 _unk0[0x404];
} DLL236_Data;

/*0x0*/ static u32 _data_0[] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffff0000
};
/*0x1C*/ static u32 _data_1C[] = {
    0x02060167, 0x01650206, 0x00000000, 0x00000000, 0x00000000
};

/*0x0*/ static u8 _bss_0[0x10];
/*0x10*/ static u8 _bss_10[0x10];

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/236_WG_Triffid/dll_236_func_0.s")

// offset: 0x5C | ctor
void dll_236_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/236_WG_Triffid/dll_236_ctor.s")

// offset: 0x9C | dtor
void dll_236_dtor(void *dll) { }

// offset: 0xA8 | func: 1 | export: 0
void dll_236_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/236_WG_Triffid/dll_236_setup.s")

// offset: 0x1EC | func: 2 | export: 1
void dll_236_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/236_WG_Triffid/dll_236_control.s")

// offset: 0x6C4 | func: 3 | export: 2
void dll_236_update(Object *self) { }

// offset: 0x6D0 | func: 4 | export: 3
void dll_236_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if ((visibility != 0) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x730 | func: 5 | export: 4
void dll_236_free(Object* self, s32 a1) {
    Baddie* sp24;
    Object* temp_a0;

    sp24 = self->data;
    obj_free_object_type(self, OBJTYPE_4);
    temp_a0 = self->linkedObject;
    if (temp_a0 != NULL) {
        obj_destroy_object(temp_a0);
        self->linkedObject = NULL;
    }
    gDLL_33_BaddieControl->vtbl->free(self, sp24, 0x20U);
}

// offset: 0x7D4 | func: 6 | export: 5
u32 dll_236_get_model_flags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_8;
}

// offset: 0x7E4 | func: 7 | export: 6
u32 dll_236_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL236_Data);
}

// offset: 0x7F8 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/236_WG_Triffid/dll_236_func_7F8.s")

// offset: 0xA20 | func: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/236_WG_Triffid/dll_236_func_A20.s")

// offset: 0xB44 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/236_WG_Triffid/dll_236_func_B44.s")

// offset: 0xC60 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/236_WG_Triffid/dll_236_func_C60.s")

// offset: 0xD64 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/236_WG_Triffid/dll_236_func_D64.s")

// offset: 0xEA0 | func: 13
s32 dll_236_func_EA0(s32 arg0, s32 arg1, s32 arg2) {
    return 0;
} 

