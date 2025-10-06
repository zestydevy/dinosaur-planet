#include "PR/ultratypes.h"
#include "dlls/objects/210_player.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/print.h"
#include "dll.h"
#include "functions.h"

// size:0x3
typedef struct {
    u8 _unk0[3];
} VFP_LevelControl_State;

static void VFP_LevelControl_func_8EC(Object *self);
static void VFP_LevelControl_func_A08(Object *self);
static void VFP_LevelControl_func_AAC(Object *self);

// offset: 0x0 | ctor
void VFP_LevelControl_ctor(void *dll) { }

// offset: 0xC | dtor
void VFP_LevelControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void VFP_LevelControl_create(Object* self, ObjCreateInfo* createInfo, s32 a2) {
    u8 mapSetupID;

    obj_add_object_type(self, 0xA);
    gDLL_29_Gplay->vtbl->set_map_setup((s32) self->mapID, 1);
    mapSetupID = gDLL_29_Gplay->vtbl->get_map_setup((s32) self->mapID);
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
    self->unk0xb0 |= 0x6000;
}

// offset: 0x24C | func: 1 | export: 1
void VFP_LevelControl_update(Object* self) {
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
            _data_0 -= (s16)delayFloat;
            if (_data_0 <= 0) {
                _data_0 = 0;
                func_80000860(self, self, 0x105U, 0);
                func_80000860(self, self, 0x106U, 0);
                func_80000860(self, self, 0x107U, 0);
                func_80000450(self, self, 0x166U, 0, 0, 0);
                func_80000450(self, self, 0x167U, 0, 0, 0);
                func_80000450(self, self, 0x168U, 0, 0, 0);
                func_80000450(self, self, 0x169U, 0, 0, 0);
                func_80000450(self, self, 0x174U, 0, 0, 0);
                func_80000450(self, self, 0x178U, 0, 0, 0);
                set_gplay_bitstring(0x123, 1U);
            }
        }
        VFP_LevelControl_func_8EC(self);
    case 0:
        return;
    case 2:
        if (_data_0 != 0) {
            _data_0 -= (s16)delayFloat;
            if (_data_0 <= 0) {
                _data_0 = 0;
                set_gplay_bitstring(0x4E9, 1U);
                func_80000860(self, self, 0x105U, 0);
                func_80000860(self, self, 0x106U, 0);
                func_80000860(self, self, 0x107U, 0);
                func_80000450(self, self, 0x166U, 0, 0, 0);
                func_80000450(self, self, 0x167U, 0, 0, 0);
                func_80000450(self, self, 0x168U, 0, 0, 0);
                func_80000450(self, self, 0x169U, 0, 0, 0);
                func_80000450(self, self, 0x174U, 0, 0, 0);
                func_80000450(self, self, 0x178U, 0, 0, 0);
                set_gplay_bitstring(0x83B, 1U);
                set_gplay_bitstring(0x22B, 1U);
            }
        }
        VFP_LevelControl_func_A08(self);
        break;
    case 3:
        if (_data_0 != 0) {
            _data_0 -= (s16)delayFloat;
            if (_data_0 <= 0) {
                _data_0 = 0;
                set_gplay_bitstring(0x4E9, 1U);
                set_gplay_bitstring(0x63C, 1U);
                func_80000860(self, self, 0x105U, 0);
                func_80000860(self, self, 0x106U, 0);
                func_80000860(self, self, 0x107U, 0);
                func_80000450(self, self, 0x166U, 0, 0, 0);
                func_80000450(self, self, 0x167U, 0, 0, 0);
                func_80000450(self, self, 0x168U, 0, 0, 0);
                func_80000450(self, self, 0x169U, 0, 0, 0);
                func_80000450(self, self, 0x174U, 0, 0, 0);
                set_gplay_bitstring(0x83C, 1U);
            }
        }
        VFP_LevelControl_func_AAC(self);
        break;
    }
}

// offset: 0x864 | func: 2 | export: 2
void VFP_LevelControl_func_864(Object *self) { }

// offset: 0x870 | func: 3 | export: 3
void VFP_LevelControl_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x888 | func: 4 | export: 4
void VFP_LevelControl_destroy(Object *obj, s32 a1) {
    obj_free_object_type(obj, 0xA);
}

// offset: 0x8C8 | func: 5 | export: 5
u32 VFP_LevelControl_func_8C8(Object *self) {
    return 0;
}

// offset: 0x8D8 | func: 6 | export: 6
u32 VFP_LevelControl_get_state_size(Object *self, u32 a1) {
    return sizeof(VFP_LevelControl_State);
}

// offset: 0x8EC | func: 7
static void VFP_LevelControl_func_8EC(Object *self) {
    /*0x4*/ static u8 _data_4 = 1;

    Object *player = get_player();

    if (_data_4 != 0) {
        set_gplay_bitstring(0x2D, 1);
        set_gplay_bitstring(0x1D7, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func14.withTwoArgs((s32)player, 0x14);
        _data_4 = 0;
    }

    if (get_gplay_bitstring(0x8F5) == 0 && get_gplay_bitstring(0x51F) == 0 && 
            get_gplay_bitstring(0x520) == 0 && get_gplay_bitstring(0x522) != 0) {
        set_gplay_bitstring(0x8F5, 1);
    }
}

// offset: 0xA08 | func: 8
static void VFP_LevelControl_func_A08(Object *self) {
    /*0x8*/ static u8 _data_8 = 1;

    Object *player = get_player();

    if (_data_8 != 0) {
        set_gplay_bitstring(0x2D, 1);
        set_gplay_bitstring(0x1D7, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func14.withTwoArgs((s32)player, 0x14);
        _data_8 = 0;
    }
}

// offset: 0xAAC | func: 9
static void VFP_LevelControl_func_AAC(Object *self) {
    /*0xC*/ static u8 _data_C = 1;

    Object *player = get_player();

    if (_data_C != 0) {
        set_gplay_bitstring(0x2D, 1);
        set_gplay_bitstring(0x1D7, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func14.withTwoArgs((s32)player, 0x14);
        _data_C = 0;
    }
}
