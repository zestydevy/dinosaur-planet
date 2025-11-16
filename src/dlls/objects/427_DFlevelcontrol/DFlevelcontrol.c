#include "dll.h"
#include "functions.h"
#include "sys/objects.h"
#include "game/gamebits.h"
#include "dlls/objects/210_player.h"

typedef struct {
/*00*/ u8 state;
/*01*/ u8 unk1;
/*02*/ u8 mapID;
/*03*/ u8 unk3;
} DFlevelcontrol_Data;

static void DFlevelcontrol_func_388(Object *self);
static void DFlevelcontrol_func_420(Object *self, s32 arg1);

// offset: 0x0 | ctor
void DFlevelcontrol_ctor(void *dll) { }

// offset: 0xC | dtor
void DFlevelcontrol_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFlevelcontrol_setup(Object *self, ObjSetup *setup, s32 arg2) {
    DFlevelcontrol_Data *objdata;

    objdata = self->data;
    if (main_get_bits(BIT_10D)) {
        objdata->state = 2;
    } else {
        objdata->state = 0;
    }
    objdata->unk1 = main_get_bits(BIT_342);
    main_set_bits(BIT_8DE, 1-objdata->unk1);
    objdata->mapID = -1;
}

// offset: 0xC0 | func: 1 | export: 1
void DFlevelcontrol_control(Object *self) {
    DFlevelcontrol_Data *objdata;
    Object *player;

    objdata = self->data;
    player = get_player();
    gDLL_6_AMSFX->vtbl->func_16E0();
    if (objdata->mapID != MAP_DISCOVERY_FALLS) {
        if (map_get_map_id_from_xz_ws(player->srt.transl.x, player->srt.transl.z) != MAP_DISCOVERY_FALLS) {
            return;
        }
        DFlevelcontrol_func_388(self);
    }
    objdata->mapID = map_get_map_id_from_xz_ws(player->srt.transl.x, player->srt.transl.z);
    if ((objdata->unk1 == 0) && (main_get_bits(BIT_342))) {
        main_set_bits(BIT_Kyte_Flight_Curve, 0x46);
        main_set_bits(BIT_8DE, 0);
        objdata->unk1 = 1;
    }
    switch (objdata->state) {
    case 0:
        if (main_get_bits(BIT_DF_Shrine_Door_Light_Activated_One) &&
            main_get_bits(BIT_DF_Shrine_Door_Light_Activated_Two) &&
            main_get_bits(BIT_DF_Shrine_Door_Light_Activated_Three) &&
            main_get_bits(BIT_DF_Shrine_Door_Light_Activated_Four)) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 11, 1);
            objdata->state++;
        }
        break;
    case 1:
        if (gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, 11)) {
            main_set_bits(BIT_4A1, 1);
            objdata->state++;
        }
        break;
    }
}

// offset: 0x2F4 | func: 2 | export: 2
void DFlevelcontrol_update(Object *self) { }

// offset: 0x300 | func: 3 | export: 3
void DFlevelcontrol_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x354 | func: 4 | export: 4
void DFlevelcontrol_free(Object *self, s32 a1) { }

// offset: 0x364 | func: 5 | export: 5
u32 DFlevelcontrol_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x374 | func: 6 | export: 6
u32 DFlevelcontrol_get_data_size(Object *self, u32 a1) {
    return sizeof(DFlevelcontrol_Data);
}

// offset: 0x388 | func: 7
void DFlevelcontrol_func_388(Object *self) {
    Object *player;

    player = get_player();
    DFlevelcontrol_func_420(self, 1);
    if (((DLL_210_Player*)player->dll)->vtbl->func38(player, PLAYER_SPIRIT_1)) {
        main_set_bits(BIT_DF_Returned_From_Shrine_With_Spirit, 1);
    }
}

// offset: 0x420 | func: 8
void DFlevelcontrol_func_420(Object *self, s32 arg1) {
    Object *player;

    player = get_player();
    func_80000860(self, player, 0x35, 0);
    func_80000860(self, player, 0x36, 0);
    func_80000860(self, player, 0x37, 0);
    func_80000450(self, player, 0x24, 0, 0, 0);
}
