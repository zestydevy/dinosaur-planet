#include "PR/ultratypes.h"
#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "functions.h"
#include "dll.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
} WL_LevelControl_Data;

static void WL_LevelControl_setup1_tick(Object* self);
static void WL_LevelControl_setup2_tick(Object* self);
static void WL_LevelControl_setup3_tick(Object* self);
static void WL_LevelControl_setup4_tick(Object* self);
static void WL_LevelControl_setup5_tick(Object* self);
static void WL_LevelControl_setup6_tick(Object* self);
static void WL_LevelControl_setup7_tick(Object* self);

// offset: 0x0 | ctor
void WL_LevelControl_ctor(void *dll) { }

// offset: 0xC | dtor
void WL_LevelControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_LevelControl_setup(Object* self, ObjSetup* setup, s32 arg2) {
    WL_LevelControl_Data* objdata;

    obj_add_object_type(self, OBJTYPE_10);
    objdata = self->data;
    objdata->unk7 = 0;
    objdata->unk2 = 0x1E;
    switch (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID)) {
    case 0:
        break;
    case 1:
        gDLL_29_Gplay->vtbl->set_map_setup(MAP_SWAPSTONE_CIRCLE, 1);
        gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_SWAPSTONE_CIRCLE, 0, 1);
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        func_80000860(self, self, 0xA5, 0);
        func_80000860(self, self, 0xA6, 0);
        objdata->unk0 = -1;
        break;
    case 5:
        func_80000860(self, self, 0xE4, 0);
        func_80000860(self, self, 0xE5, 0);
        break;
    case 6:
        func_80000860(self, self, 0xA5, 0);
        func_80000860(self, self, 0xA6, 0);
        main_set_bits(BIT_Medium_Food_Bag, 1);
        break;
    case 7:
        func_80000860(self, self, 0xE4, 0);
        func_80000860(self, self, 0xE5, 0);
        objdata->unk4 = 0x2BC;
        objdata->unk6 = 0x1E;
        objdata->unk2 = objdata->unk6;
        break;
    default:
        break;
    }
}

// offset: 0x25C | func: 1 | export: 1
void WL_LevelControl_control(Object* self) {
    switch (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID)) {
    case 0:
        break;
    case 1:
        WL_LevelControl_setup1_tick(self);
        break;
    case 2:
        WL_LevelControl_setup2_tick(self);
        break;
    case 3:
        WL_LevelControl_setup3_tick(self);
        break;
    case 4:
        WL_LevelControl_setup4_tick(self);
        break;
    case 5:
        WL_LevelControl_setup5_tick(self);
        break;
    case 6:
        WL_LevelControl_setup6_tick(self);
        break;
    case 7:
        WL_LevelControl_setup7_tick(self);
        break;
    }
}

// offset: 0x394 | func: 2 | export: 2
void WL_LevelControl_update(Object *self) { }

// offset: 0x3A0 | func: 3 | export: 3
void WL_LevelControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x3F4 | func: 4 | export: 4
void WL_LevelControl_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_10);
    if (self){} // @fake
}

// offset: 0x43C | func: 5 | export: 5
u32 WL_LevelControl_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x44C | func: 6 | export: 6
u32 WL_LevelControl_get_data_size(Object *self, u32 a1) {
    return sizeof(WL_LevelControl_Data);
}

// offset: 0x460 | func: 7
static void WL_LevelControl_func_460(Object* self, WL_LevelControl_Data* objdata) {
    Object** objs;
    s32 numObjs;
    s32 i;
    s32 var_t0;

    if (main_get_bits(BIT_Play_Seq_00EF_Scales_Escapes_With_Kyte) == 0) {
        objs = obj_get_all_of_type(OBJTYPE_7, &numObjs);
        var_t0 = 1;
        
        for (i = 0; i < numObjs; i++) {
            if ((objs[i]->id == OBJ_WL_Galleon) || (objs[i]->id == OBJ_SB_Galleon)) {
                var_t0 = 0;
            }
        }

        if (var_t0 != 0) {
            if (gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, 1) == 0) {
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 1, 1);
            }
            main_set_bits(BIT_WL_Load_Unload_Galleon, 1);
            objdata->unk7 = 1;
            objdata->unk8 |= 1;
        }
    }
}

// offset: 0x648 | func: 8
static void WL_LevelControl_func_648(Object* self, WL_LevelControl_Data* objdata) {
    u32 sp2C;
    s32 numObjs;
    Object** objs;
    s32 i;

    sp2C = main_get_bits(BIT_WM_Near_Cave_Exit_Krystal_Side);
    if ((objdata->unk8 & 1) && (main_get_bits(BIT_WL_Load_Unload_Galleon) == 0)) {
        if (gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, 1) != 0) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 1, 0);
        }
        objdata->unk8 &= ~1;
    }
    if (!(objdata->unk8 & 2) && (sp2C != 0)) {
        objs = obj_get_all_of_type(OBJTYPE_7, &numObjs);
        for (i = 0; i < numObjs; i++) {
            if (objs[i]->id == OBJ_WL_Galleon) {
                i = numObjs; // break out of loop
            }
        }
        objdata->unk8 |= 2;
    }
}

// offset: 0x7A4 | func: 9
static void WL_LevelControl_setup1_tick(Object* self) {
    /*0x0*/ static u8 _data_0 = 0;

    WL_LevelControl_Data* objdata;
    u8 temp_t8;

    objdata = self->data;
    temp_t8 = 0;
    temp_t8 += main_get_bits(BIT_WM_Force_Field_1_Disabled);
    temp_t8 += main_get_bits(BIT_WM_Force_Field_2_Disabled);
    temp_t8 += main_get_bits(BIT_WM_Force_Field_3_Disabled);
    if ((temp_t8 == 3) && (_data_0 != 3)) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B89_Puzzle_Solved, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    _data_0 = temp_t8;
    switch (objdata->unk7) {
        case 1:
            if (objdata->unk9 == 1) {
                WL_LevelControl_func_648(self, objdata);
            }
            break;
        case 0:
            objdata->unk9 = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);
            switch (objdata->unk9) {
            case 1:
                WL_LevelControl_func_460(self, objdata);
                break;
            case 2:
                break;
            }
            break;
    }
}

// offset: 0x940 | func: 10
static void WL_LevelControl_setup2_tick(Object* self) {
    static u8 _data_4 = 1;
    
    Object* player;
    Object* temp_v0;

    player = get_player();
    if ((_data_4 != 0) && (main_get_bits(BIT_Play_Seq_0180_Release_Spirit_1) == 0)) {
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        player = get_player();
        main_set_bits(BIT_FC, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func14(player, 0x14);
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, 1, 1);
        func_80000860(self, self, 0x204, 0);
        func_80000860(self, self, 0x205, 0);
        func_80000860(self, self, 0x206, 0);
        _data_4 = 0;
    }
    if (main_get_bits(BIT_Medium_Food_Bag) != 0) {
        temp_v0 = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0xF);
        if ((temp_v0 != NULL) && (main_get_bits(BIT_Green_Apple_Count) == 0)) {
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
            ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        }
    }
}

// offset: 0xC50 | func: 11
static void WL_LevelControl_setup3_tick(Object* self) {
    static u8 _data_8 = 1;
    
    Object* player;

    if ((main_get_bits(BIT_Spirit_2_Release_Sabre) == 0) & _data_8) {
        func_80000860(self, self, 0xE4, 0);
        func_80000860(self, self, 0xE5, 0);
        func_80000450(self, self, 0x275, 0, 0, 0);
        func_80000450(self, self, 0x278, 0, 0, 0);
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        player = get_player();
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, 2, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func14(player, 0x14);
        _data_8 = 0;
    } else if (_data_8 != 0) {
        func_80000860(self, self, 0xE4, 0);
        func_80000860(self, self, 0xE5, 0);
        func_80000450(self, self, 0x275, 0, 0, 0);
        _data_8 = 0;
    }
}

// offset: 0xE34 | func: 12
static void WL_LevelControl_setup4_tick(Object* self) {
    /*0xC*/ static u8 _data_C = 1;
    /*0x10*/ static s32 _data_10 = -1;

    WL_LevelControl_Data* objdata;
    Object* player;
    s16 temp_v0_3;
    s16 var_s2;
    u8 i;

    var_s2 = 0;
    objdata = self->data;
    if ((_data_C != 0) && (main_get_bits(BIT_317) == 0)) {
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        main_set_bits(BIT_Set_During_Spirit_Release_1, 1);
        player = get_player();
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, 4, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func14(player, 0x14);
        _data_C = 0;
    }
    for (i = 0; i < 6; i++) {
        var_s2 +=  main_get_bits(i + BIT_WM_Killed_SharpClaw_2573);
    }
    temp_v0_3 = main_get_bits(BIT_2C4);
    if ((temp_v0_3 != 0) && (var_s2 != objdata->unk0)) {
        objdata->unk0 = var_s2;
        func_80000860(self, self, (objdata->unk0 + 0xF4), 0);
        func_80000450(self, self, ((objdata->unk0 / 2) + 0x13C), 0, 0, 0);
    } else if (temp_v0_3 != _data_10) {
        if (temp_v0_3 != 0) {
            func_80000860(self, self, (objdata->unk0 + 0xF4), 0);
            func_80000450(self, self, ((objdata->unk0 / 2) + 0x13C), 0, 0, 0);
        } else {
            func_80000860(self, self, 0xFB, 0);
            func_80000450(self, self, 0x144, 0, 0, 0);
        }
    }
    _data_10 = temp_v0_3;
}

// offset: 0x1118 | func: 13
static void WL_LevelControl_setup5_tick(Object* self) {
    /*0x14*/ static u8 _data_14 = 1;

    WL_LevelControl_Data* objdata;
    f32 sp40;
    Object* temp_v0;
    Object** var_a2;
    s32 sp34;
    s16 i;
    s16 temp_v0_2;
    ObjSetup *someObjsetup;
    Object* player;
    
    sp34 = 0;
    sp40 = 10000.0f;
    player = get_player();
    objdata = self->data;
    if ((_data_14 != 0) && (main_get_bits(BIT_318) == 0)) {
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        main_set_bits(BIT_Spell_Illusion, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, 8, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func14(player, 0x14);
        main_set_bits(BIT_2DD, 0);
        _data_14 = 0;
    }
    if (main_get_bits(BIT_2DB) != 0) {
        func_80059038(0x18, 0, 0);
    }
    if (main_get_bits(BIT_2DD) != 0) {
        main_set_bits(BIT_CFExplodeTunnel_Trigger_31B6F, 1);
        main_set_bits(BIT_2DD, 0);
        temp_v0 = obj_get_nearest_type_to(OBJTYPE_4, self, &sp40);
        if (temp_v0 != NULL) {
            obj_destroy_object(temp_v0);
        }
        objdata->unk2 = 0x1E;
    }
    if (main_get_bits(BIT_2F7) != 0) {
        var_a2 = obj_get_all_of_type(OBJTYPE_4, &sp34);
        for (i = 0; i < sp34; i++) {
            someObjsetup = var_a2[i]->setup;
            if ((someObjsetup->uID == 0x296E) || (someObjsetup->uID == 0x296F)) {
                obj_destroy_object(var_a2[i]);
            }
        }
        main_set_bits(BIT_2F7, 0);
    }
    if (main_get_bits(BIT_2EE) != 0) {
        temp_v0_2 = ((DLL_210_Player*)player->dll)->vtbl->func50(player);
        if ((temp_v0_2 != 0x40) && (temp_v0_2 != 0x1D7) && (main_get_bits(BIT_2F3) == 0)) {
            warpPlayer(WARP_WM_SABRE_KRAZOA_CORRIDOR, /*fadeToBlack=*/FALSE);
        }
        main_set_bits(BIT_2EE, 0);
    }
    if (main_get_bits(BIT_2FA) != 0) {
        if (main_get_bits(BIT_2F7) == 0) {
            main_set_bits(BIT_2F7, 1);
        }
        objdata->unk2 -= (s16)delayByte;
        if (objdata->unk2 <= 0) {
            objdata->unk2 = 0;
            main_set_bits(BIT_2FA, 0);
            main_set_bits(BIT_2F3, 1);
            objdata->unk2 = 0x1E;
        }
    }
}

// offset: 0x14FC | func: 14
static void WL_LevelControl_setup6_tick(Object* self) {
    /*0x18*/ static u8 _data_18 = 1;

    Object* player;
    Object* temp_v0;

    player = get_player();
    if ((_data_18 != 0) && (main_get_bits(BIT_Play_Seq_020D) == 0)) {
        temp_v0 = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0xF);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[10].withOneArg((s32)temp_v0);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 1);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 2);
        ((DLL_Unknown*)temp_v0->dll)->vtbl->func[11].withTwoArgs((s32)temp_v0, 4);
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, 0x20, 1);
        _data_18 = 0;
    }
}

// offset: 0x1788 | func: 15
static void WL_LevelControl_setup7_tick(Object* self) {
    /*0x1C*/ static u8 _data_1C = 1;

    WL_LevelControl_Data* objdata;
    Object* player;

    get_player();
    objdata = (WL_LevelControl_Data*)self->data;
    if ((_data_1C != 0) && (main_get_bits(BIT_Play_Seq_020D) == 0)) {
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        player = get_player();
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, 0x40, 1);
        ((DLL_210_Player*)player->dll)->vtbl->func14(player, 0x14);
        _data_1C = 0;
        objdata->unk2 = 1;
        func_80000860(self, self, 0x32, 0);
        func_80000860(self, self, 0x33, 0);
        main_set_bits(BIT_221, 1);
    }
    if (main_get_bits(BIT_36C) != 0) {
        if (objdata->unk4 > 0) {
            objdata->unk4 -= (s16)delayByte;
            if (objdata->unk2 != 0) {
                objdata->unk2 -= (s16)delayByte;
                if (objdata->unk2 <= 0) {
                    main_set_bits(BIT_36D, 1);
                    if (objdata->unk6 >= 0xB) {
                        objdata->unk6 = objdata->unk6 - 1;
                    }
                    objdata->unk2 = objdata->unk6;
                }
            }
        }
    }
    if (rand_next(0, 30) == 0) {
        main_set_bits(BIT_WM_Randorn_Door_OpenClose, 1);
    }
    if (rand_next(0, 10) == 0) {
        main_set_bits(BIT_WM_Randorn_Door_OpenClose, 0);
    }
}
