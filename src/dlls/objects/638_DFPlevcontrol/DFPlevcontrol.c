#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/gfx/texture.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/214_animobj.h"

//Official name unknown, but VFPT's equivalent was named "VFlevcontrol.c"

/*0x0*/ static Texture* _data_0 = NULL;
/*0x4*/ static u32 _data_4[] = {
    0x01020300, 0x00000000, 0x00000000
};
/*0x10*/ static u32 _data_10 = 0x00000000;
/*0x14*/ static u32 _data_14 = 0x00820000;
/*0x18*/ static u32 _data_18 = 0x00000000;
/*0x1C*/ static u32 _data_1C = 0x01000000;
/*0x20*/ static u32 _data_20 = 0x01000000;
/*0x24*/ static u32 _data_24[] = {
    0x01000000, 0x00000000, 0x00000000
};

static int dll_638_func_62C(Object* self, Object *arg1, AnimObj_Data *arg2, s8 arg3);

typedef struct {
s16 unk0;   
u8 _unk2[4];
} DFPTLevelControl_Data;

// offset: 0x0 | ctor
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/638_DFPlevcontrol/dll_638_ctor.s")

// offset: 0xC | dtor
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/638_DFPlevcontrol/dll_638_dtor.s")

// offset: 0x18 | func: 0 | export: 0
void dll_638_setup(Object* self, s32 arg1, s32 arg2) {
    u8 mapSetup;

    obj_add_object_type(self, OBJTYPE_10);
    _data_0 = tex_load_deferred(0x46C);
    self->animCallback = dll_638_func_62C;
    gDLL_29_Gplay->vtbl->set_map_setup(self->mapID, 1);
    mapSetup = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);

    switch (mapSetup) { 
        case 0:
            func_80000860(self, self, 261, 0);
            func_80000860(self, self, 262, 0);
            func_80000860(self, self, 263, 0);
            gDLL_29_Gplay->vtbl->set_obj_group_status(0x32, 2, 1);
            break;
        case 1:
            func_80000860(self, self, 415, 0);
            main_set_bits(BIT_SpellStone_CRF, 1);
            break;
        case 2:
            func_80000860(self, self, 415, 0);
            main_set_bits(BIT_SpellStone_BWC, 1);
            main_set_bits(BIT_Spell_Grenade, 1);
            break;
        case 3:
            func_80000860(self, self, 415, 0);
            main_set_bits(BIT_SpellStone_KP, 1);
            main_set_bits(BIT_Spell_Grenade, 1);
            break;
    }
    self->unkB0 |= 0x6000;
}

// offset: 0x260 | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/638_DFPlevcontrol/dll_638_control.s")

// offset: 0x51C | func: 2 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/638_DFPlevcontrol/dll_638_update.s")

// offset: 0x528 | func: 3 | export: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/638_DFPlevcontrol/dll_638_print.s")

// offset: 0x540 | func: 4 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/638_DFPlevcontrol/dll_638_free.s")

// offset: 0x5C0 | func: 5 | export: 5
u32 dll_638_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x5D0 | func: 6 | export: 6
u32 dll_638_get_data_size(Object *self, u32 a1) {
    return sizeof(DFPTLevelControl_Data);
}

// offset: 0x5E4 | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/638_DFPlevcontrol/dll_638_func_5E4.s")

// offset: 0x62C | func: 8
static int dll_638_func_62C(Object* self, Object *arg1, AnimObj_Data *arg2, s8 arg3) {
    DFPTLevelControl_Data *objdata;
    DLL_210_Player *dll;
    Object *player;
    s16 delay;
    s16 timerValueOriginal;

    objdata = self->data;
    player = get_player();
    timerValueOriginal = objdata->unk0;
    if (timerValueOriginal > 0){
        delay = ((s32) gUpdateRateF);

        if (!(DLL_210_Player *) player->dll){} //@fake?

        objdata->unk0 = timerValueOriginal - (delay & 0xFFFF);
        ((DLL_210_Player *) player->dll)->vtbl->func72(player, 0x51E);
    }
    return 0;
}

// offset: 0x6C4 | func: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/638_DFPlevcontrol/dll_638_func_6C4.s")

// offset: 0xB2C | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/638_DFPlevcontrol/dll_638_func_B2C.s")

// offset: 0xECC | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/638_DFPlevcontrol/dll_638_func_ECC.s")
