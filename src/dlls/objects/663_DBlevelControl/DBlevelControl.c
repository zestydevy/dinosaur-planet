#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/objtype.h"
#include "dlls/objects/210_player.h"
#include "sys/objects.h"
#include "game/gamebits.h"

/*0x0*/ static s16 _data_0[3] = {
    BIT_DB_Gave_Pointback_Egg_One,
    BIT_DB_Gave_Pointback_Egg_Two,
    BIT_DB_Gave_Pointback_Egg_Three
};
/*0x8*/ static s16 _data_8[3] = {
    BIT_DB_Tricky_Drained_Pool_One,
    BIT_DB_Tricky_Drained_Pool_Two,
    BIT_DB_Tricky_Drained_Pool_Three
};
/*0x10*/ static s16 _data_10[3] = {
    BIT_DB_Egg_Flood_Level_One,
    BIT_DB_Egg_Flood_Level_Two,
    BIT_DB_Egg_Flood_Level_Three
};
/*0x18*/ static s16 _data_18[3] = {
    BIT_3C5,
    BIT_3C6,
    BIT_3C7
};
/*0x20*/ static s16 _data_20[3] = {
    BIT_DB_Unlock_Act_Two,
    BIT_DB_Unlock_Act_Three,
    BIT_DB_Unlock_Act_Four
};

/*0x0*/ static const char str_0[] = " NO EGGS %i ";
/*0x10*/ static const char str_10[] = "Flood Level  %i ";
/*0x24*/ static const char str_24[] = "";
/*0x28*/ static const char str_28[] = "";
/*0x2C*/ static const char str_2C[] = "";

typedef struct {
/*00*/ u8 mapSetup;
/*01*/ u8 eggsOrDiamonds;
/*02*/ u8 floodLevel;
/*03*/ u8 unk3;
} DBlevelControl_Data;

// offset: 0x0 | ctor
void DBlevelControl_ctor(void *self) { }

// offset: 0xC | dtor
void DBlevelControl_dtor(void *self) { }

// offset: 0x18 | func: 0 | export: 0
void DBlevelControl_setup(Object *self, ObjSetup *setup, s32 arg2) {
    s32 i;
    DBlevelControl_Data *objdata;

    objdata = self->data;
    objdata->eggsOrDiamonds = 0;
    objdata->floodLevel = 0;
    objdata->unk3 = 10;
    objdata->mapSetup = 1;

    for (i = 0; i < 3; i++) {
        if (main_get_bits(_data_8[i])) {
            objdata->floodLevel++;
        }
    }
    self->unk0xb0 |= 0x6000;
    obj_add_object_type(self, OBJTYPE_52);
}

// offset: 0xE4 | func: 1 | export: 1
void DBlevelControl_control(Object *self) {
    s32 i;
    s32 count;
    DBlevelControl_Data *objdata;
    Object *player;

    objdata = self->data;

    if (main_get_bits(BIT_DB_Picked_Up_Egg)) {
        count = 0;

        for (i = 0; i < 3; i++) {
            count += main_get_bits(_data_18[i]); // ?
        }

        if (objdata->eggsOrDiamonds < count) {
            main_set_bits(_data_0[objdata->eggsOrDiamonds], 1);
            main_set_bits(BIT_DB_Picked_Up_Egg, 0);
            main_set_bits(BIT_DB_Eggs_Or_Diamonds, ++objdata->eggsOrDiamonds);
        }
    }

    if (main_get_bits(BIT_DB_Update_Flood_Level)) {
        main_set_bits(_data_10[objdata->floodLevel], 1);
        main_set_bits(BIT_DB_Update_Flood_Level, 0);
        objdata->floodLevel++;
    }

    gDLL_6_AMSFX->vtbl->func_16E0();

    if (main_get_bits(_data_20[objdata->mapSetup-1])) {
        do {
            objdata->mapSetup++;
        } while (main_get_bits(_data_20[objdata->mapSetup-1]));
    }
    if (objdata->mapSetup != gDLL_29_Gplay->vtbl->get_map_setup(self->mapID)) {
        gDLL_29_Gplay->vtbl->set_map_setup(self->mapID, objdata->mapSetup);
    }

    player = get_player();
    if (((DLL_210_Player*)(player->dll))->vtbl->func38(player, 2)) {
        main_set_bits(BIT_21F_Spirit_Collected, 1);
    }
}

// offset: 0x360 | func: 2 | export: 2
void DBlevelControl_update(Object *self) { }

// offset: 0x36C | func: 3 | export: 3
void DBlevelControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x384 | func: 4 | export: 4
void DBlevelControl_free(Object *self, s32 arg1) {
   obj_free_object_type(self, OBJTYPE_52);
}

// offset: 0x3C4 | func: 5 | export: 5
u32 DBlevelControl_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x3D4 | func: 6 | export: 6
u32 DBlevelControl_get_data_size(Object *self, u32 arg1) {
    return sizeof(DBlevelControl_Data);
}
