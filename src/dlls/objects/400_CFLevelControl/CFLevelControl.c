#include "dll.h"
#include "functions.h"
#include "sys/main.h"
#include "sys/print.h"
#include "sys/rand.h"
#include "game/gamebits.h"
#include "macros.h"
#include "sys/dll.h"
#include "sys/objtype.h"
#include "sys/gfx/projgfx.h"
#include "dlls/engine/53.h"
#include "dlls/objects/373_CFCloudBaby.h"

// CFGuardian/CFSupTreasureCh
DLL_INTERFACE(DLL_CFGuardianCFSupTreasureCh) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s32 (*func7)(Object*); // returns whether a flag is unset
};

#define GAMEBITS_COUNT(gamebitArray) (sizeof(gamebitArray) / sizeof(s16))

typedef struct {
/*00*/ u8 _unk0;
/*01*/ u8 flags;
/*02*/ u8 _unk2;
/*03*/ u8 _unk3;
} CFLevelControl_Data;

enum CFLevelControl_Flags {
    CFLEVELCONTROL_FLAG_1 = 1
};

/*0x0*/ static u32 sTriggerPassed = 0;
/*0x4*/ static DLL_IProjgfx *sDLL190 = NULL;
/*0x8*/ static s16 _data_8[5] = {
    BIT_Play_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_One,
    BIT_Play_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Two,
    BIT_Play_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Three,
    BIT_Play_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Four,
    BIT_Play_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Five
};
/*0x14*/ static s16 _data_14[5] = {
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_One,
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Two,
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Three,
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Four,
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Five
};
/*0x20*/ static s16 _data_20[4] = {
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_One,
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Two,
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Three,
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Four
};
/*0x28*/ static s16 _data_28[1] = { NO_GAMEBIT }; // unused
/*0x2C*/ static s16 _data_2C[2] = { BIT_4DC, BIT_SpellStone_CRF_Activated };
/*0x30*/ static s16 _data_30[1] = { BIT_30A };
/*0x34*/ static s16 _data_34[2] = { BIT_8D1, BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Five };
/*0x38*/ static s16 _data_38[1] = { BIT_477 };
/*0x3C*/ static s16 _data_3C[1] = { BIT_Play_Seq_02C6_CF_Sharpclaw_Only_Four_Chests_Left };
/*0x40*/ static s16 _data_40[1] = { BIT_528 };
/*0x44*/ static s16 _data_44[1] = { BIT_4BE };
/*0x48*/ static s16 _data_48[1] = { BIT_4C1 };
/*0x4C*/ static s16 _data_4C[1] = { BIT_4BE }; // unused
/*0x50*/ static s16 _data_50[1] = { BIT_4C1 }; // unused

// size:0x8
typedef struct {
/*00*/ s32 uID;
/*04*/ s16 gamebit;
/*06*/ u8 objgroup;
} Data54;
#define DATA_54_COUNT 3
enum ObjectUIDs {
    UID_CloudBaby_1 = 0x29b3, // CFCloudBaby
    UID_CloudBaby_2 = 0x236a, // CFCloudBaby
    UID_CloudBaby_3 = 0x29fa, // CFCloudBaby
    UID_Guardian = 0x2a4f, // CFGuardian
    UID_Chest_1 = 0x32cfa, // CFSupTreasureCh
    UID_Chest_2 = 0x31d36, // CFSupTreasureCh
    UID_Chest_3 = 0x31d0c, // CFSupTreasureCh
    UID_Chest_With_Baby = 0x31d0d // CFSupTreasureCh
};
/*0x54*/ static Data54 _data_54[3] = {
    //uID              gamebit  objgroup
    { UID_CloudBaby_1, BIT_50C, 22 },
    { UID_CloudBaby_2, BIT_50D, 20 },
    { UID_CloudBaby_3, BIT_50E, 21 }
};

// size:0x14
typedef struct {
/*00*/ s32 uID;
/*04*/ f32 distance;
/*08*/ s32 mapID;
/*0C*/ s16 gamebit1;
/*0E*/ u8 objgroup;
/*0F*/ u8 status;
/*10*/ s16 gamebit2;
/*12*/ u8 unk12;
} Data6C;
#define DATA_6C_COUNT 5
/*0x6C*/ static Data6C _data_6C[DATA_6C_COUNT] = {
    //uID                  dist    mapID                     gamebit1                                               objgroup  status  gamebit2     ?
    { UID_Guardian,        800.0f, MAP_CLOUDRUNNER_DUNGEON,  BIT_4E0,                                               3,        0,      NO_GAMEBIT,  0  },
    { UID_Chest_1,         400.0f, MAP_CLOUDRUNNER_TREASURE, BIT_Play_Seq_02C7_Scales_Takes_Baby_Cloudrunner_Away,  5,        0,      BIT_8CC,     10 },
    { UID_Chest_2,         400.0f, MAP_CLOUDRUNNER_TREASURE, BIT_Play_Seq_02C7_Scales_Takes_Baby_Cloudrunner_Away,  6,        0,      BIT_8CC,     11 },
    { UID_Chest_3,         400.0f, MAP_CLOUDRUNNER_TREASURE, BIT_Play_Seq_02C7_Scales_Takes_Baby_Cloudrunner_Away,  7,        0,      BIT_8CC,     12 },
    { UID_Chest_With_Baby, 400.0f, MAP_CLOUDRUNNER_TREASURE, BIT_Play_Seq_02C7_Scales_Takes_Baby_Cloudrunner_Away,  8,        0,      BIT_8CC,     13 }
};

// size: 0x8
typedef struct {
/*00*/ u8 bitValue; // value of bit in BIT_CF_ObjGroup_Bits
/*01*/ u8 gamebitCount;
/*02*/ u8 objgroup;
/*04*/ s16 *gamebits;
} DataD0;
#define DATA_D0_COUNT 8
/*0xD0*/ static DataD0 _data_D0[DATA_D0_COUNT] = {
    //value gamebit count              objgroup   gamebits
    { 1,    GAMEBITS_COUNT(_data_8),   5,         _data_8  },
    { 0,    GAMEBITS_COUNT(_data_14),  5,         _data_14 },
    { 1,    GAMEBITS_COUNT(_data_30),  3,         _data_30 },
    { 0,    GAMEBITS_COUNT(_data_38),  3,         _data_38 },
    { 1,    GAMEBITS_COUNT(_data_44),  25,        _data_44 },
    { 0,    GAMEBITS_COUNT(_data_48),  25,        _data_48 },
    { 1,    GAMEBITS_COUNT(_data_3C),  22,        _data_3C },
    { 0,    GAMEBITS_COUNT(_data_40),  22,        _data_40 }
};
/*0x110*/ static u32 _data_110 = DATA_D0_COUNT;

// size: 0x6
typedef struct {
/*00*/ s16 gamebit1;
/*02*/ s16 gamebit2;
/*04*/ s16 objgroup;
} Data114;
#define DATA_114_COUNT 1
/*0x114*/ static Data114 _data_114[DATA_114_COUNT] = {
    //gamebit1  gamebit2  objgroup
    { BIT_524,  BIT_525,  22 }
};

/*0x11C*/ static s16 _data_11C[] = {
    0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff,
    0x0003, 0x2cfa, 0x0003, 0x1d36, 0x0003, 0x1d0c,
    0x0003, 0x1d0d
};

static s32 CFLevelControl_func_39C(s16, s32);
static void CFLevelControl_func_3F0(Object *, DataD0 *, s32);
static void CFLevelControl_func_67C(Object *, DataD0 *, s32);
static s32 CFLevelControl_func_82C(s16, s16 *, s32);
static void CFLevelControl_func_914(void);
static void CFLevelControl_func_9EC(Data54 *, s32);
static void CFLevelControl_func_AE8(Data114 *, s32);
static void CFLevelControl_func_BB8(Data6C *, s32);
static void CFLevelControl_func_DC0(Data6C *, s32);
static Vec3f *CFLevelControl_get_position_of_saved_obj(s32);
static void CFLevelControl_func_1000(void);
static s32 CFLevelControl_func_10BC(Object *);

// offset: 0x0 | ctor
void CFLevelControl_ctor(void *dll) { }

// offset: 0xC | dtor
void CFLevelControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFLevelControl_setup(Object *self, ObjSetup *setup, s32 arg2) {
    CFLevelControl_Data *objdata;

    objdata = self->data;
    objdata->flags = CFLEVELCONTROL_FLAG_1;
    sTriggerPassed = main_get_bits(BIT_CF_Entrance_Trigger_Passed);
    CFLevelControl_func_67C(self, _data_D0, _data_110);
    sDLL190 = dll_load_deferred(DLL_ID_190, 1);
    create_temp_dll(DLL_ID_53);
}

// offset: 0xCC | func: 1 | export: 1
void CFLevelControl_control(Object *self) {
    CFLevelControl_Data *objdata;

    objdata = self->data;
    if (objdata->flags & CFLEVELCONTROL_FLAG_1) {
        CFLevelControl_func_DC0(_data_6C, DATA_6C_COUNT);
        objdata->flags &= ~CFLEVELCONTROL_FLAG_1;
    }
    CFLevelControl_func_3F0(self, _data_D0, _data_110);
    if (CFLevelControl_func_82C(BIT_30A, _data_20, GAMEBITS_COUNT(_data_20))) {
        main_set_bits(BIT_33D, 1);
    }
    CFLevelControl_func_82C(BIT_4BE, _data_34, GAMEBITS_COUNT(_data_34));
    CFLevelControl_func_82C(BIT_4DD, _data_2C, GAMEBITS_COUNT(_data_2C));
    CFLevelControl_func_914();
    CFLevelControl_func_9EC(_data_54, DATA_54_COUNT);
    CFLevelControl_func_BB8(_data_6C, DATA_6C_COUNT);
    if (!main_get_bits(BIT_Play_Seq_02C6_CF_Sharpclaw_Only_Four_Chests_Left)) {
        CFLevelControl_func_AE8(_data_114, DATA_114_COUNT);
    }
    CFLevelControl_func_10BC(self);
    CFLevelControl_func_1000();
    diPrintf(" Layer NO %i : ", map_get_layer());
}

// offset: 0x2B4 | func: 2 | export: 2
void CFLevelControl_update(Object *self) { }

// offset: 0x2C0 | func: 3 | export: 3
void CFLevelControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x314 | func: 4 | export: 4
void CFLevelControl_free(Object *self, s32 a1) {
    remove_temp_dll(DLL_ID_53);
    if (sDLL190) {
        dll_unload(sDLL190);
    }
}

// offset: 0x378 | func: 5 | export: 5
u32 CFLevelControl_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x388 | func: 6 | export: 6
u32 CFLevelControl_get_data_size(Object *self, u32 a1) {
    return sizeof(CFLevelControl_Data);
}

//*0x0*/ static const char str_0[] = " Layer NO %i : ";
/*0x10*/ static const char str_10[] = " Loading Group %i \n";
/*0x24*/ static const char str_24[] = " Cnt %i V %i ";
/*0x34*/ static const char str_34[] = " cnt %i v %i ";
/*0x44*/ static const char str_44[] = " RESETING GROUP LOAD ";
/*0x5C*/ static const char str_5C[] = " %i";

// offset: 0x39C | func: 7
s32 CFLevelControl_func_39C(s16 gamebit, s32 objgroup) {
    return (main_get_bits(gamebit) >> objgroup) & 1;
}

// offset: 0x3F0 | func: 8
void CFLevelControl_func_3F0(Object *self, DataD0 *data, s32 count) {
    s32 i, j, k;
    s16 *gamebits;
    s32 dothing1;
    s32 dothing2;
    DataD0 *d;

    for (i = 0; i < count; i++) {
        d = &data[i];
        gamebits = d->gamebits;
        dothing1 = FALSE;
        dothing2 = FALSE;
        for (j = 0; j < d->gamebitCount; j++) {
            if (gamebits[j] == NO_GAMEBIT) {
                continue;
            }
            if (main_get_bits(gamebits[j])) {
                gamebits[j] = NO_GAMEBIT;
                dothing1 = TRUE;
            }
        }
        if (dothing1) {
            if (CFLevelControl_func_39C(BIT_CF_ObjGroup_Bits, d->objgroup) != d->bitValue) {
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, d->objgroup, d->bitValue);
                dothing2 = TRUE;
            }
        }
        if (dothing2 && d->objgroup == 5) {
            k = 0;
            if (d->bitValue == 1) {
                for (j = 0; j < 31; j++) {
                    if ((CFLevelControl_func_39C(BIT_CF_ObjGroup_Bits, j)) && (j != d->objgroup)) {
                        _data_11C[k] = j;
                        k++;
                        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, j, 0);
                    }
                }
                _data_11C[k] = -1;
            } else {
                while (_data_11C[k] != -1 && k < 31) {
                    gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, _data_11C[k], 1);
                    k++;
                }
            }
        }
    }
}

// offset: 0x67C | func: 9
void CFLevelControl_func_67C(Object *self, DataD0 *data, s32 count) {
    s32 i, j;
    s16 *gamebits;
    s32 delta;
    DataD0 *d;

    for (i = 0; i < count; i++) {
        d = &data[i];
        gamebits = d->gamebits;
        delta = 0;
        for (j = 0; j < d->gamebitCount; j++) {
            if (gamebits[j] == NO_GAMEBIT) {
                continue;
            } else if (main_get_bits(gamebits[j])) {
                gamebits[j] = NO_GAMEBIT;
                delta++;
            }
        }
        i++;
        d = &data[i];
        gamebits = d->gamebits;
        for (j = 0; j < d->gamebitCount; j++) {
            if (gamebits[j] == NO_GAMEBIT) {
                continue;
            } else if (main_get_bits(gamebits[j])) {
                gamebits[j] = NO_GAMEBIT;
                delta--;
            }
        }
        if (delta != 0) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, d->objgroup, 1);
        }
    }
}

// offset: 0x82C | func: 10
s32 CFLevelControl_func_82C(s16 gamebit, s16 *gamebits, s32 gamebitsCount) {
    s32 i;
    s32 count;

    count = 0;
    if (main_get_bits(gamebit)) {
        return 1;
    }
    for (i = 0; i < gamebitsCount; i++) {
        if (!main_get_bits(*gamebits)) {
            return 0;
        }
        count++;
        if (1) {}
        main_get_bits(*gamebits);
        gamebits++;
    }
    if (gamebitsCount == count) {
        main_set_bits(gamebit, 1);
        return 1;
    }
    return 0;
}

// offset: 0x914 | func: 11
void CFLevelControl_func_914(void) {
    if (main_get_bits(BIT_Played_Seq_0041_Scales_Kills_The_Queen) && !main_get_bits(BIT_CF_Floor_Destroyed)) {
        if (main_get_bits(BIT_Kyte_Flight_Curve) != 0x11) {
            main_set_bits(BIT_Kyte_Flight_Curve, 0x11);
            main_set_bits(BIT_454, 1);
            STUBBED_PRINTF(" KYTE TRAPPED \n ");
        }
    } else if (main_get_bits(BIT_454)) {
        main_set_bits(BIT_454, 0);
        STUBBED_PRINTF(" KYTE ESCAPPED \n ");
    }
}

// /*0x60*/ static const char str_60[] = " KYTE TRAPPED \n ";
// /*0x74*/ static const char str_74[] = " KYTE ESCAPPED \n ";
// /*0x88*/ static const char str_88[] = " CRAP IS BOLLOX ";
// /*0x9C*/ static const char str_9C[] = " Freeing Baby ";
// /*0xAC*/ static const char str_AC[] = " CRAP IS BOLLOX ";
// /*0xC0*/ static const char str_C0[] = " CRAP IS BOLLOX ";
// /*0xD4*/ static const char str_D4[] = " CRAP IS BOLLOX ";
// /*0xE8*/ static const char str_E8[] = " Couldn't find romdef %i ";
//*0x104*/ static const char str_104[] = " STart Seq Val %i ";

// offset: 0x9EC | func: 12
void CFLevelControl_func_9EC(Data54 *data, s32 count) {
    Object *cloudbaby;

    while (count--) {
        STUBBED_PRINTF(" CRAP IS BOLLOX ");
        if (main_get_bits(data->gamebit)) {
            cloudbaby = func_800211B4(data->uID);
            if (cloudbaby && (((DLL_373_CFCloudBaby*)cloudbaby->dll)->vtbl->func7(cloudbaby))) {
                STUBBED_PRINTF(" Freeing Baby ");
                gDLL_29_Gplay->vtbl->set_obj_group_status(cloudbaby->mapID, data->objgroup, 0);
                main_set_bits(data->gamebit, 0);
            }
        }
        data++;
    }
}

// offset: 0xAE8 | func: 13
void CFLevelControl_func_AE8(Data114 *data, s32 count) {
    while (count--) {
        STUBBED_PRINTF(" CRAP IS BOLLOX ");
        if (main_get_bits(data->gamebit1) && !main_get_bits(data->gamebit2)) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_CLOUDRUNNER_FORTRESS, data->objgroup, 1);
            main_set_bits(data->gamebit1, 0);
        }
        data++;
    }
}

// offset: 0xBB8 | func: 14
void CFLevelControl_func_BB8(Data6C *data, s32 count) {
    Vec3f *objPos;
    Object *chestOrGuardian;
    f32 distance;

    while (count--) {
        STUBBED_PRINTF(" CRAP IS BOLLOX ");
        if ((data->gamebit2 == NO_GAMEBIT) || (main_get_bits(data->gamebit2))) {
            objPos = CFLevelControl_get_position_of_saved_obj(data->uID);
            if (objPos) {
                distance = func_80001884(objPos->x, objPos->y, objPos->z);
                obj_get_nearest_type(OBJTYPE_53, objPos, &distance);
                chestOrGuardian = func_800211B4(data->uID);
                if (chestOrGuardian) {
                    if (data->distance < distance && ((data->gamebit1 == NO_GAMEBIT) || (!main_get_bits(data->gamebit1))) && (((DLL_CFGuardianCFSupTreasureCh*)chestOrGuardian->dll)->vtbl->func7(chestOrGuardian))) {
                        data->status = 0;
                        gDLL_29_Gplay->vtbl->set_obj_group_status(chestOrGuardian->mapID, data->objgroup, data->status);
                    }
                } else if (distance < data->distance || ((data->gamebit1 != NO_GAMEBIT) && (main_get_bits(data->gamebit1)))) {
                    if (!data->status) {}
                    data->status = 1;
                    gDLL_29_Gplay->vtbl->set_obj_group_status(data->mapID, data->objgroup, data->status);
                }
            }
        }
        data++;
    }
}

// offset: 0xDC0 | func: 15
void CFLevelControl_func_DC0(Data6C *data, s32 count) {
    ObjSetup *setup;
    SRT transform;

    while (count--) {
        STUBBED_PRINTF(" CRAP IS BOLLOX ");
        setup = func_80044448(data->uID, NULL, NULL, NULL, NULL);
        if (setup) {
            if (data->unk12) {
                ((DLL_53*)(gTempDLLInsts[1]))->vtbl->func7(data->unk12, &transform);
                setup->x = transform.transl.x;
                setup->y = transform.transl.y;
                setup->z = transform.transl.z;
            }
            map_save_object(setup, data->mapID, setup->x, setup->y, setup->z);
        }
        data++;
    }
}

// offset: 0xEC8 | func: 16
Vec3f *CFLevelControl_get_position_of_saved_obj(s32 uID) {
    s16 numSavedObjs;
    SavedObject *savedObjs;
    s32 i;

    numSavedObjs = gDLL_29_Gplay->vtbl->get_num_saved_objects();
    savedObjs = gDLL_29_Gplay->vtbl->get_saved_objects();
    for (i = 0; i < numSavedObjs; i++) {
        if (uID == savedObjs[i].uID) {
            return (Vec3f*)&savedObjs[i].x;
        }
    }
    STUBBED_PRINTF(" Couldn't find romdef %i ", uID);
    return NULL;
}

// offset: 0x1000 | func: 17
void CFLevelControl_func_1000(void) {
    diPrintf(" STart Seq Val %i ", main_get_bits(BIT_Play_Seq_02C6_CF_Sharpclaw_Only_Four_Chests_Left));
    if (gDLL_29_Gplay->vtbl->get_obj_group_status(MAP_CLOUDRUNNER_FORTRESS, 23) && main_get_bits(BIT_CF_Free_Cloudrunner_From_Chest)) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_CLOUDRUNNER_FORTRESS, 23, 0);
    }
}

// /*0x118*/ static const char str_118[] = " you have Passed ";
// /*0x12C*/ static const char str_12C[] = " Lighting Flash ";

// offset: 0x10BC | func: 18
s32 CFLevelControl_func_10BC(Object *self) {
    s32 rand;
    SRT transform;

    transform.roll = 0;
    transform.pitch = 0;
    transform.yaw = 0;
    transform.transl.x = 300.0f;
    transform.transl.y = -100.0f;
    transform.transl.z = 0.0f;
    transform.scale = 2.0f;
    if (main_get_bits(BIT_CF_Entrance_Trigger_Passed) != sTriggerPassed) {
        gDLL_28_ScreenFade->vtbl->func3(10, SCREEN_FADE_WHITE, 0.3f);
        sDLL190->vtbl->func0(self, 2, &transform, 1, -1, 4, 0);
        sDLL190->vtbl->func0(self, 2, &transform, 1, -1, 4, 0);
        sDLL190->vtbl->func0(self, 2, &transform, 1, -1, 4, 0);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_73_Thunder, MAX_VOLUME, NULL, NULL, 0, NULL);
        STUBBED_PRINTF(" you have Passed ");
        sTriggerPassed = main_get_bits(BIT_CF_Entrance_Trigger_Passed);
    } else if ((rand_next(0, 100) == 0) && (main_get_bits(BIT_577))) {
        rand = rand_next(5, 10);
        gDLL_28_ScreenFade->vtbl->func3(rand, SCREEN_FADE_WHITE, 0.1f + rand * 0.05f);
        sDLL190->vtbl->func0(self, 2, NULL, 1, -1, 4, 0);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_73_Thunder, 77 + rand * 10 , NULL, NULL, 0, NULL);
        STUBBED_PRINTF(" Lighting Flash ");
    }
    return 1;
}
