#include "dll.h"
#include "sys/main.h"
#include "sys/map_enums.h"
#include "sys/print.h"
#include "sys/rand.h"
#include "sys/objprint.h"
#include "game/gamebits.h"
#include "macros.h"
#include "sys/dll.h"
#include "sys/objtype.h"
#include "sys/gfx/projgfx.h"
#include "dlls/engine/53_movelib.h"
#include "dlls/objects/373_CFCloudBaby.h"

// CFGuardian/CFSupTreasureCh
DLL_INTERFACE(DLL_CFGuardianCFSupTreasureCh) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ s32 (*func7)(Object*); // returns whether a flag is unset
};

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
/*0x8*/ static s16 sBabyCRStartPerchLandBits[5] = {
    BIT_Play_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_One,
    BIT_Play_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Two,
    BIT_Play_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Three,
    BIT_Play_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Four,
    BIT_Play_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Five
};
/*0x14*/ static s16 sBabyCREndPerchLandBits[5] = {
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_One,
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Two,
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Three,
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Four,
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Five
};
// All excluding the treasure room CR
/*0x20*/ static s16 sFirstFourBabyCRPerchLandedBits[4] = {
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_One,
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Two,
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Three,
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Four
};
/*0x28*/ static s16 _data_28[1] = { NO_GAMEBIT }; // unused
/*0x2C*/ static s16 sEndCutsceneRequiredBits[2] = { 
    BIT_CRF_Player_Leaving_Via_Entrance_Bridge, 
    BIT_SpellStone_CRF_Activated 
};
/*0x30*/ static s16 sTreasureRoomStartSeqBits[1] = { BIT_CRF_Play_Seq_005D_Treasure_Room_Door_Opening_Sequences };
/*0x34*/ static s16 sThroneRoomQuestCompletionBits[2] = { 
    BIT_CRF_Activate_Kyte_Gold_Perch, 
    BIT_Played_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Five 
};
/*0x38*/ static s16 sTreasureRoomEndSeqBits[1] = { BIT_477_CRF_Treasure_Room_Doors_Opened };
/*0x3C*/ static s16 _data_3C[1] = { BIT_Play_Seq_02C6_CF_Sharpclaw_Only_Four_Chests_Left };
/*0x40*/ static s16 _data_40[1] = { BIT_528 };
/*0x44*/ static s16 sCourtyardWaterSeqStartBits[1] = { BIT_CRF_Throne_Room_Quest_Complete };
/*0x48*/ static s16 sCourtyardWaterSeqEndBits[1] = { BIT_4C1 };
/*0x4C*/ static s16 _data_4C[1] = { BIT_CRF_Throne_Room_Quest_Complete }; // unused
/*0x50*/ static s16 _data_50[1] = { BIT_4C1 }; // unused

// size:0x8
typedef struct {
/*00*/ s32 uID;
/*04*/ s16 gamebit;
/*06*/ u8 objgroup;
} Data54;
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
/*0x6C*/ static Data6C _data_6C[] = {
    //uID                  dist    mapID                     gamebit1                                               objgroup  status  gamebit2     ?
    { UID_Guardian,        800.0f, MAP_CLOUDRUNNER_DUNGEON,  BIT_4E0,                                               3,        0,      NO_GAMEBIT,  0  },
    { UID_Chest_1,         400.0f, MAP_CLOUDRUNNER_TREASURE, BIT_Play_Seq_02C7_Scales_Takes_Baby_Cloudrunner_Away,  5,        0,      BIT_8CC,     10 },
    { UID_Chest_2,         400.0f, MAP_CLOUDRUNNER_TREASURE, BIT_Play_Seq_02C7_Scales_Takes_Baby_Cloudrunner_Away,  6,        0,      BIT_8CC,     11 },
    { UID_Chest_3,         400.0f, MAP_CLOUDRUNNER_TREASURE, BIT_Play_Seq_02C7_Scales_Takes_Baby_Cloudrunner_Away,  7,        0,      BIT_8CC,     12 },
    { UID_Chest_With_Baby, 400.0f, MAP_CLOUDRUNNER_TREASURE, BIT_Play_Seq_02C7_Scales_Takes_Baby_Cloudrunner_Away,  8,        0,      BIT_8CC,     13 }
};

// size: 0x8
typedef struct {
/*00*/ u8 status; // status to set objgroup to
/*01*/ u8 bitCount;
/*02*/ u8 objgroup;
/*04*/ s16 *bits;
} DataD0;
/*0xD0*/ static DataD0 _data_D0[] = {
    //status gamebit count                            objgroup   gamebits
    { 1,     ARRAYCOUNT(sBabyCRStartPerchLandBits),   5,         sBabyCRStartPerchLandBits }, // Enable throne room objgroup when cfbaby perch seq starts
    { 0,     ARRAYCOUNT(sBabyCREndPerchLandBits),     5,         sBabyCREndPerchLandBits },   // Disable throne room objgroup when cfbaby perch seq ends
    { 1,     ARRAYCOUNT(sTreasureRoomStartSeqBits),   3,         sTreasureRoomStartSeqBits },
    { 0,     ARRAYCOUNT(sTreasureRoomEndSeqBits),     3,         sTreasureRoomEndSeqBits },
    { 1,     ARRAYCOUNT(sCourtyardWaterSeqStartBits), 25,        sCourtyardWaterSeqStartBits },
    { 0,     ARRAYCOUNT(sCourtyardWaterSeqEndBits),   25,        sCourtyardWaterSeqEndBits },
    { 1,     ARRAYCOUNT(_data_3C),                    22,        _data_3C },
    { 0,     ARRAYCOUNT(_data_40),                    22,        _data_40 }
};
/*0x110*/ static u32 _data_110 = ARRAYCOUNT(_data_D0);

// size: 0x6
typedef struct {
/*00*/ s16 gamebit1;
/*02*/ s16 gamebit2;
/*04*/ s16 objgroup;
} Data114;
/*0x114*/ static Data114 _data_114[] = {
    //gamebit1  gamebit2  objgroup
    { BIT_524,  BIT_525,  22 }
};

// @bug: This array is not large enough to list all potentially 31 enabled objgroups.
//       While this doesn't happen in practice, if too many are enabled then this will be overrun.
/*0x11C*/ static s16 sPrevEnabledObjGroups[] = {
    -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, -1,
    // other data?
    0x0003, 0x2cfa, 0x0003, 0x1d36, 0x0003, 0x1d0c,
    0x0003, 0x1d0d
};

static void CFLevelControl_func_3F0(Object *, DataD0 *, s32);
static void CFLevelControl_func_67C(Object *, DataD0 *, s32);
static s32 CFLevelControl_setBitAfterRequiredBits(s16 bit, s16 *requiredBits, s32 requiredBitsCount);
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
    sTriggerPassed = mainGetBits(BIT_CF_Entrance_Trigger_Passed);
    CFLevelControl_func_67C(self, _data_D0, _data_110);
    sDLL190 = dllLoad(DLL_ID_190, 1);
    mainCreateTempDLL(DLL_ID_MOVELIB);
}

// offset: 0xCC | func: 1 | export: 1
void CFLevelControl_control(Object *self) {
    CFLevelControl_Data *objdata;

    objdata = self->data;
    if (objdata->flags & CFLEVELCONTROL_FLAG_1) {
        CFLevelControl_func_DC0(_data_6C, ARRAYCOUNT(_data_6C));
        objdata->flags &= ~CFLEVELCONTROL_FLAG_1;
    }
    CFLevelControl_func_3F0(self, _data_D0, _data_110);
    // Open treasure room doors once the first four baby CloudRunners are rescued
    if (CFLevelControl_setBitAfterRequiredBits(BIT_CRF_Play_Seq_005D_Treasure_Room_Door_Opening_Sequences, 
            sFirstFourBabyCRPerchLandedBits, ARRAYCOUNT(sFirstFourBabyCRPerchLandedBits))) {
        mainSetBits(BIT_CRF_First_Four_Baby_CloudRunners_Rescued, 1);
    }
    // Check for throne room question completion (all baby CloudRunners rescued + Kyte activates the gold perch)
    CFLevelControl_setBitAfterRequiredBits(BIT_CRF_Throne_Room_Quest_Complete, 
        sThroneRoomQuestCompletionBits, ARRAYCOUNT(sThroneRoomQuestCompletionBits));
    // Enable final cutscene on the entrance bridge after level is completed
    CFLevelControl_setBitAfterRequiredBits(BIT_Play_Seq_0210_CF_Exit_Cutscene, 
        sEndCutsceneRequiredBits, ARRAYCOUNT(sEndCutsceneRequiredBits));
    CFLevelControl_func_914();
    CFLevelControl_func_9EC(_data_54, ARRAYCOUNT(_data_54));
    CFLevelControl_func_BB8(_data_6C, ARRAYCOUNT(_data_6C));
    if (!mainGetBits(BIT_Play_Seq_02C6_CF_Sharpclaw_Only_Four_Chests_Left)) {
        CFLevelControl_func_AE8(_data_114, ARRAYCOUNT(_data_114));
    }
    CFLevelControl_func_10BC(self);
    CFLevelControl_func_1000();
    diPrintf(" Layer NO %i : ", mapGetLayer());
}

// offset: 0x2B4 | func: 2 | export: 2
void CFLevelControl_update(Object *self) { }

// offset: 0x2C0 | func: 3 | export: 3
void CFLevelControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x314 | func: 4 | export: 4
void CFLevelControl_free(Object *self, s32 a1) {
    mainRemoveTempDLL(DLL_ID_MOVELIB);
    if (sDLL190) {
        dllFree(sDLL190);
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

// offset: 0x39C | func: 7
static s32 CFLevelControl_getObjGroupStatus(s16 groupBit, s32 group) {
    return (mainGetBits(groupBit) >> group) & 1;
}

// offset: 0x3F0 | func: 8
static void CFLevelControl_func_3F0(Object *self, DataD0 *data, s32 count) {
    s32 i, j, k;
    s16 *bits;
    s32 bitWasSet;
    s32 statusChanged;
    DataD0 *d;

    for (i = 0; i < count; i++) {
        d = &data[i];
        bits = d->bits;
        bitWasSet = FALSE;
        statusChanged = FALSE;
        // Check if a bit in the list was set. Once set, ignore it going forward.
        for (j = 0; j < d->bitCount; j++) {
            if (bits[j] == NO_GAMEBIT) {
                continue;
            }
            if (mainGetBits(bits[j])) {
                bits[j] = NO_GAMEBIT;
                bitWasSet = TRUE;
            }
        }
        // If a bit was just set, toggle the related objgroup if needed
        if (bitWasSet) {
            if (CFLevelControl_getObjGroupStatus(BIT_CF_ObjGroup_Bits, d->objgroup) != d->status) {
                STUBBED_PRINTF(" Loading Group %i \n", d->objgroup);
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, d->objgroup, d->status);
                statusChanged = TRUE;
            }
        }
        // For the throne room specifically, disable *all* other objgroups during the sequence of
        // a baby CloudRunner landing on their perch. Restore the objgroups after once we go to
        // disable the throne room objgroup.
        if (statusChanged && d->objgroup == 5) { // objgroup 5 == throne room
            k = 0;
            if (d->status == 1) {
                // Disable everything except the throne room
                for (j = 0; j < 31; j++) {
                    if ((CFLevelControl_getObjGroupStatus(BIT_CF_ObjGroup_Bits, j)) && (j != d->objgroup)) {
                        sPrevEnabledObjGroups[k] = j;
                        k++;
                        STUBBED_PRINTF(" Cnt %i V %i ", k, j);
                        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, j, 0);
                    }
                }
                sPrevEnabledObjGroups[k] = -1; // terminate list
            } else {
                // Restore previously enabled groups
                while (sPrevEnabledObjGroups[k] != -1 && k < 31) {
                    STUBBED_PRINTF(" cnt %i v %i ", k, sPrevEnabledObjGroups[k]);
                    gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, sPrevEnabledObjGroups[k], 1);
                    k++;
                }
            }
        }
    }
}

// offset: 0x67C | func: 9
static void CFLevelControl_func_67C(Object *self, DataD0 *data, s32 count) {
    s32 i, j;
    s16 *gamebits;
    s32 delta;
    DataD0 *d;

    STUBBED_PRINTF(" RESETING GROUP LOAD ");

    for (i = 0; i < count; i++) {
        d = &data[i];
        gamebits = d->bits;
        delta = 0;
        for (j = 0; j < d->bitCount; j++) {
            if (gamebits[j] == NO_GAMEBIT) {
                continue;
            } else if (mainGetBits(gamebits[j])) {
                gamebits[j] = NO_GAMEBIT;
                delta++;
            }
        }
        i++;
        d = &data[i];
        gamebits = d->bits;
        for (j = 0; j < d->bitCount; j++) {
            if (gamebits[j] == NO_GAMEBIT) {
                continue;
            } else if (mainGetBits(gamebits[j])) {
                gamebits[j] = NO_GAMEBIT;
                delta--;
            }
        }
        if (delta != 0) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, d->objgroup, 1);
        }
    }
}

/*0x5C*/ static const char str_5C[] = " %i";

// offset: 0x82C | func: 10
static s32 CFLevelControl_setBitAfterRequiredBits(s16 bit, s16 *requiredBits, s32 requiredBitsCount) {
    s32 i;
    s32 count;

    count = 0;
    if (mainGetBits(bit)) {
        return 1;
    }
    for (i = 0; i < requiredBitsCount; i++) {
        if (!mainGetBits(*requiredBits)) {
            return 0;
        }
        count++;
        if (1) {}
        mainGetBits(*requiredBits);
        requiredBits++;
    }
    if (requiredBitsCount == count) {
        mainSetBits(bit, 1);
        return 1;
    }
    return 0;
}

// offset: 0x914 | func: 11
static void CFLevelControl_func_914(void) {
    if (mainGetBits(BIT_Played_Seq_0041_Scales_Kills_The_Queen) && !mainGetBits(BIT_CF_Floor_Destroyed)) {
        if (mainGetBits(BIT_Kyte_Flight_Curve) != 0x11) {
            mainSetBits(BIT_Kyte_Flight_Curve, 0x11);
            mainSetBits(BIT_454, 1);
            STUBBED_PRINTF(" KYTE TRAPPED \n "); // guessed location
        }
    } else if (mainGetBits(BIT_454)) {
        mainSetBits(BIT_454, 0);
        STUBBED_PRINTF(" KYTE ESCAPPED \n "); // guessed location
    }
}

/*0x88*/ static const char str_88[] = " CRAP IS BOLLOX ";

// offset: 0x9EC | func: 12
static void CFLevelControl_func_9EC(Data54 *data, s32 count) {
    Object *cloudbaby;

    while (count--) {
        if (mainGetBits(data->gamebit)) {
            cloudbaby = objGetObjectByUID(data->uID);
            if (cloudbaby && (((DLL_373_CFCloudBaby*)cloudbaby->dll)->vtbl->IsRescuedTimerDone(cloudbaby))) {
                gDLL_29_Gplay->vtbl->set_obj_group_status(cloudbaby->mapID, data->objgroup, 0);
                STUBBED_PRINTF(" Freeing Baby ");
                mainSetBits(data->gamebit, 0);
            }
        }
        data++;
    }
}

/*0xAC*/ static const char str_AC[] = " CRAP IS BOLLOX ";
/*0xC0*/ static const char str_C0[] = " CRAP IS BOLLOX ";
/*0xD4*/ static const char str_D4[] = " CRAP IS BOLLOX ";

// offset: 0xAE8 | func: 13
static void CFLevelControl_func_AE8(Data114 *data, s32 count) {
    while (count--) {
        if (mainGetBits(data->gamebit1) && !mainGetBits(data->gamebit2)) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_CLOUDRUNNER_FORTRESS, data->objgroup, 1);
            mainSetBits(data->gamebit1, 0);
        }
        data++;
    }
}

// offset: 0xBB8 | func: 14
static void CFLevelControl_func_BB8(Data6C *data, s32 count) {
    Vec3f *objPos;
    Object *chestOrGuardian;
    f32 distance;

    while (count--) {
        if ((data->gamebit2 == NO_GAMEBIT) || (mainGetBits(data->gamebit2))) {
            objPos = CFLevelControl_get_position_of_saved_obj(data->uID);
            if (objPos) {
                distance = camDistance(objPos->x, objPos->y, objPos->z);
                objGetNearestType(OBJTYPE_CFTreasRobo, objPos, &distance);
                chestOrGuardian = objGetObjectByUID(data->uID);
                if (chestOrGuardian) {
                    if (data->distance < distance && ((data->gamebit1 == NO_GAMEBIT) || (!mainGetBits(data->gamebit1))) && (((DLL_CFGuardianCFSupTreasureCh*)chestOrGuardian->dll)->vtbl->func7(chestOrGuardian))) {
                        data->status = 0;
                        gDLL_29_Gplay->vtbl->set_obj_group_status(chestOrGuardian->mapID, data->objgroup, data->status);
                    }
                } else if (distance < data->distance || ((data->gamebit1 != NO_GAMEBIT) && (mainGetBits(data->gamebit1)))) {
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
static void CFLevelControl_func_DC0(Data6C *data, s32 count) {
    ObjSetup *setup;
    SRT transform;

    while (count--) {
        setup = mapFindObjSetup(data->uID, NULL, NULL, NULL, NULL);
        if (setup) {
            if (data->unk12) {
                ((DLL_53_movelib*)(gTempDLLInsts[1]))->vtbl->func7(data->unk12, &transform);
                setup->x = transform.transl.x;
                setup->y = transform.transl.y;
                setup->z = transform.transl.z;
            }
            mapSaveObject(setup, data->mapID, setup->x, setup->y, setup->z);
        }
        data++;
    }
}

// offset: 0xEC8 | func: 16
static Vec3f* CFLevelControl_get_position_of_saved_obj(s32 uID) {
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
    STUBBED_PRINTF(" Couldn't find romdef %i ", uID); // guessed location
    return NULL;
}

// offset: 0x1000 | func: 17
static void CFLevelControl_func_1000(void) {
    diPrintf(" STart Seq Val %i ", mainGetBits(BIT_Play_Seq_02C6_CF_Sharpclaw_Only_Four_Chests_Left));
    if (gDLL_29_Gplay->vtbl->get_obj_group_status(MAP_CLOUDRUNNER_FORTRESS, 23) && mainGetBits(BIT_CF_Free_Cloudrunner_From_Chest)) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_CLOUDRUNNER_FORTRESS, 23, 0);
    }
}

// offset: 0x10BC | func: 18
static s32 CFLevelControl_func_10BC(Object *self) {
    s32 rand;
    SRT transform;

    transform.roll = 0;
    transform.pitch = 0;
    transform.yaw = 0;
    transform.transl.x = 300.0f;
    transform.transl.y = -100.0f;
    transform.transl.z = 0.0f;
    transform.scale = 2.0f;
    if (mainGetBits(BIT_CF_Entrance_Trigger_Passed) != sTriggerPassed) {
        gDLL_28_ScreenFade->vtbl->func3(10, SCREEN_FADE_WHITE, 0.3f);
        sDLL190->vtbl->func0(self, 2, &transform, 1, -1, 4, 0);
        sDLL190->vtbl->func0(self, 2, &transform, 1, -1, 4, 0);
        sDLL190->vtbl->func0(self, 2, &transform, 1, -1, 4, 0);
        dll_amSfx->Play(self, SOUND_73_Thunder, MAX_VOLUME, NULL, NULL, 0, NULL);
        STUBBED_PRINTF(" you have Passed "); // guessed location
        sTriggerPassed = mainGetBits(BIT_CF_Entrance_Trigger_Passed);
    } else if ((mathRnd(0, 100) == 0) && (mainGetBits(BIT_577))) {
        rand = mathRnd(5, 10);
        gDLL_28_ScreenFade->vtbl->func3(rand, SCREEN_FADE_WHITE, 0.1f + rand * 0.05f);
        sDLL190->vtbl->func0(self, 2, NULL, 1, -1, 4, 0);
        dll_amSfx->Play(self, SOUND_73_Thunder, 77 + rand * 10 , NULL, NULL, 0, NULL);
        STUBBED_PRINTF(" Lighting Flash "); // guessed location
    }
    return 1;
}
