#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/print.h"
#include "dll.h"
#include "functions.h"
#include "types.h"

// size:0x3
typedef struct {
    u8 _unk0[3];
} VFP_LevelControl_Data;

static void VFP_LevelControl_func_8EC(Object *self);
static void VFP_LevelControl_func_A08(Object *self);
static void VFP_LevelControl_func_AAC(Object *self);

// offset: 0x0 | ctor
void VFP_LevelControl_ctor(void *dll) { }

// offset: 0xC | dtor
void VFP_LevelControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void VFP_LevelControl_setup(Object* self, ObjSetup* setup, s32 a2) {
    u8 mapSetupID;

    obj_add_object_type(self, OBJTYPE_10);
    gDLL_29_Gplay->vtbl->set_map_setup(self->mapID, 1);
    mapSetupID = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);
    switch (mapSetupID) {
    case 1:
    case 2:
        break;
    case 0:
        func_80000860(self, self, 0x105, 0);
        func_80000860(self, self, 0x106, 0);
        func_80000860(self, self, 0x107, 0);
        break;
    case 3:
        func_80000860(self, self, 0x105, 0);
        func_80000860(self, self, 0x106, 0);
        func_80000860(self, self, 0x107, 0);
        func_80000450(self, self, 0x166, 0, 0, 0);
        func_80000450(self, self, 0x167, 0, 0, 0);
        func_80000450(self, self, 0x168, 0, 0, 0);
        func_80000450(self, self, 0x169, 0, 0, 0);
        func_80000450(self, self, 0x174, 0, 0, 0);
        break;
    }
    self->unkB0 |= 0x6000;
}

// offset: 0x24C | func: 1 | export: 1
void VFP_LevelControl_control(Object* self) {
    /*0x0*/ static s16 _data_0 = 130;
    
    Object* player;
    u8 mapSetupID;

    player = get_player();
    map_get_map_id_from_xz_ws(player->srt.transl.x, player->srt.transl.z);
    diPrintf("ACT %d \n", gDLL_29_Gplay->vtbl->get_map_setup(self->mapID));
    mapSetupID = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);
    switch (mapSetupID) {
    case 1:
        if (_data_0 != 0) {
            _data_0 -= (s16)gUpdateRateF;
            if (_data_0 <= 0) {
                _data_0 = 0;
                func_80000860(self, self, 0x105, 0);
                func_80000860(self, self, 0x106, 0);
                func_80000860(self, self, 0x107, 0);
                func_80000450(self, self, 0x166, 0, 0, 0);
                func_80000450(self, self, 0x167, 0, 0, 0);
                func_80000450(self, self, 0x168, 0, 0, 0);
                func_80000450(self, self, 0x169, 0, 0, 0);
                func_80000450(self, self, 0x174, 0, 0, 0);
                func_80000450(self, self, 0x178, 0, 0, 0);
                main_set_bits(BIT_SpellStone_DIM, 1);
            }
        }
        VFP_LevelControl_func_8EC(self);
    case 0:
        return;
    case 2:
        if (_data_0 != 0) {
            _data_0 -= (s16)gUpdateRateF;
            if (_data_0 <= 0) {
                _data_0 = 0;
                main_set_bits(BIT_DB_Unlock_Act_Two, 1);
                func_80000860(self, self, 0x105, 0);
                func_80000860(self, self, 0x106, 0);
                func_80000860(self, self, 0x107, 0);
                func_80000450(self, self, 0x166, 0, 0, 0);
                func_80000450(self, self, 0x167, 0, 0, 0);
                func_80000450(self, self, 0x168, 0, 0, 0);
                func_80000450(self, self, 0x169, 0, 0, 0);
                func_80000450(self, self, 0x174, 0, 0, 0);
                func_80000450(self, self, 0x178, 0, 0, 0);
                main_set_bits(BIT_SpellStone_WC, 1);
                main_set_bits(BIT_SpellStone_DIM_Activated, 1);
            }
        }
        VFP_LevelControl_func_A08(self);
        break;
    case 3:
        if (_data_0 != 0) {
            _data_0 -= (s16)gUpdateRateF;
            if (_data_0 <= 0) {
                _data_0 = 0;
                main_set_bits(BIT_DB_Unlock_Act_Two, 1);
                main_set_bits(BIT_DB_Unlock_Act_Three, 1);
                func_80000860(self, self, 0x105, 0);
                func_80000860(self, self, 0x106, 0);
                func_80000860(self, self, 0x107, 0);
                func_80000450(self, self, 0x166, 0, 0, 0);
                func_80000450(self, self, 0x167, 0, 0, 0);
                func_80000450(self, self, 0x168, 0, 0, 0);
                func_80000450(self, self, 0x169, 0, 0, 0);
                func_80000450(self, self, 0x174, 0, 0, 0);
                main_set_bits(BIT_SpellStone_DR, 1);
            }
        }
        VFP_LevelControl_func_AAC(self);
        break;
    }
}

// offset: 0x864 | func: 2 | export: 2
void VFP_LevelControl_update(Object *self) { }

// offset: 0x870 | func: 3 | export: 3
void VFP_LevelControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x888 | func: 4 | export: 4
void VFP_LevelControl_free(Object *obj, s32 a1) {
    obj_free_object_type(obj, OBJTYPE_10);
}

// offset: 0x8C8 | func: 5 | export: 5
u32 VFP_LevelControl_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x8D8 | func: 6 | export: 6
u32 VFP_LevelControl_get_data_size(Object *self, u32 a1) {
    return sizeof(VFP_LevelControl_Data);
}

// offset: 0x8EC | func: 7
static void VFP_LevelControl_func_8EC(Object *self) {
    /*0x4*/ static u8 _data_4 = 1;

    Object *player = get_player();

    if (_data_4 != 0) {
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 20);
        _data_4 = 0;
    }

    if (main_get_bits(BIT_VFP_Flamethrower_Turned_Off) == 0 && 
            main_get_bits(BIT_VFP_Entered_Snake_Flame_Room_1) == 0 && 
            main_get_bits(BIT_VFP_Entered_Snake_Flame_Room_1_2) == 0 && 
            main_get_bits(BIT_VFP_Flamethrower_Turned_Off_2) != 0) {
        main_set_bits(BIT_VFP_Flamethrower_Turned_Off, 1);
    }
}

// offset: 0xA08 | func: 8
static void VFP_LevelControl_func_A08(Object *self) {
    /*0x8*/ static u8 _data_8 = 1;

    Object *player = get_player();

    if (_data_8 != 0) {
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 20);
        _data_8 = 0;
    }
}

// offset: 0xAAC | func: 9
static void VFP_LevelControl_func_AAC(Object *self) {
    /*0xC*/ static u8 _data_C = 1;

    Object *player = get_player();

    if (_data_C != 0) {
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 20);
        _data_C = 0;
    }
}
