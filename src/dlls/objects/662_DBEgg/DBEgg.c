#include "common.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "dlls/objects/210_player.h"

typedef struct {
    u8 _unk0[0x10C - 0x00];
    u8 unk10C;
    u8 unk10D;
    s16 unk10E;
} DBEgg_Data;

typedef struct {
    ObjSetup base;
    u32 _unk18;
    s16 unk1C;
} DBEgg_Setup;

/*0x0*/ static u32 _data_0[] = {
    0x00000001, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void dll_662_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_662_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_662_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/662_DBEgg/dll_662_setup.s")

// offset: 0x7C | func: 1 | export: 1
void dll_662_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/662_DBEgg/dll_662_control.s")

// offset: 0xD28 | func: 2 | export: 2
void dll_662_update(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/662_DBEgg/dll_662_update.s")

// offset: 0xED0 | func: 3 | export: 3
void dll_662_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DBEgg_Data* objdata = self->data;
    s32 temp;

    if (visibility != 0) {
        temp = objdata->unk10C;

        if ((temp != 0xC) && (temp != 4) && (temp != 0xB)) {
            draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0xF44 | func: 4 | export: 4
void dll_662_free(Object* self, s32 a1) {
    obj_free_object_type(self, 0x26);
}

// offset: 0xF84 | func: 5 | export: 5
u32 dll_662_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xF94 | func: 6 | export: 6
u32 dll_662_get_data_size(Object *self, u32 a1) {
    return sizeof(DBEgg_Data);
}

// offset: 0xFA8 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/662_DBEgg/dll_662_func_FA8.s")

// offset: 0x1240 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/662_DBEgg/dll_662_func_1240.s")

// offset: 0x1670 | func: 9
void dll_662_func_1670(Object* self) {
    Object* sp2C;
    DBEgg_Data* objdata;
    DBEgg_Setup* setup;

    sp2C = get_player();
    objdata = self->data;
    setup = (DBEgg_Setup*)self->setup;
    obj_free_object_type(self, 0x26);
    objdata->unk10C = 3;
    main_set_bits(0x3C4, 1U);
    self->unkAF |= 8;
    main_set_bits((s32) setup->unk1C, 1U);
    obj_send_mesg(sp2C, 0x7000AU, self, (void* )0x10000);
}

// offset: 0x1744 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/662_DBEgg/dll_662_func_1744.s")

// offset: 0x1954 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/662_DBEgg/dll_662_func_1954.s")
