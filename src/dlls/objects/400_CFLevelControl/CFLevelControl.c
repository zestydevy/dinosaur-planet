#include "sys/main.h"
#include "sys/map_enums.h"
#include "sys/print.h"
#include "sys/rand.h"
#include "sys/objprint.h"
#include "game/gamebits.h"
#include "sys/dll.h"
#include "sys/objtype.h"
#include "sys/gfx/projgfx.h"
#include "dlls/engine/53_movelib.h"
#include "dlls/objects/373_CFCloudBaby.h"
#include "dlls/objects/common/cf_can_unload.h"
#include "macros.h"
#include "dll.h"

typedef struct {
/*00*/ u8 _unk0;
/*01*/ u8 flags;
/*02*/ u8 _unk2;
/*03*/ u8 _unk3;
} CFLevelControl_Data;

enum CFLevelControl_Flags {
    CFLEVELCONTROL_IsFirstTick = 1
};

/*0x0*/ static u32 sTriggerPassed = 0;
/*0x4*/ static DLL_IProjgfx *sLightningProjgfx = NULL;
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
/*0x3C*/ static s16 sTreasRoboStartSeqBits[1] = { BIT_Play_Seq_02C6_CF_Sharpclaw_Only_Four_Chests_Left };
/*0x40*/ static s16 sTreasRoboEndSeqBits[1] = { BIT_Play_Seq_042D_CF_Baby_CloudRunner_Trapped_In_Chest };
/*0x44*/ static s16 sCourtyardWaterSeqStartBits[1] = { BIT_CRF_Throne_Room_Quest_Complete };
/*0x48*/ static s16 sCourtyardWaterSeqEndBits[1] = { BIT_Played_Seq_02B2_CF_Courtyard_Water_Drains };
/*0x4C*/ static s16 _data_4C[1] = { BIT_CRF_Throne_Room_Quest_Complete }; // unused
/*0x50*/ static s16 _data_50[1] = { BIT_Played_Seq_02B2_CF_Courtyard_Water_Drains }; // unused

// size:0x8
typedef struct {
/*00*/ s32 uID;
/*04*/ s16 gamebit;
/*06*/ u8 objgroup;
} BabyCRObjGroupToggle;
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
/*0x54*/ static BabyCRObjGroupToggle sBabyCRObjGroupToggles[3] = {
    //uID              gamebit  objgroup
    { UID_CloudBaby_1, BIT_CRF_Unload_Dock_Baby_CloudRunner_ObjGroup, 22 },
    { UID_CloudBaby_2, BIT_CRF_Unload_Cliff_Baby_CloudRunner_ObjGroup, 20 },
    { UID_CloudBaby_3, BIT_CRF_Unload_Courtyard_Baby_CloudRunner_ObjGroup, 21 }
};

// size:0x14
typedef struct {
/*00*/ s32 uID;
/*04*/ f32 distance;
/*08*/ s32 mapID;
/*0C*/ s16 toggleBit;
/*0E*/ u8 objgroup;
/*0F*/ u8 status; // current objgroup status
/*10*/ s16 enabledBit;
/*12*/ u8 createPointID; // create point curve ID to reset obj to on levelcontrol init
} DistObjGroupToggle;
/*0x6C*/ static DistObjGroupToggle sDistObjGroupToggles[] = {
    //uID                  dist    mapID                     toggleBit                                              objgroup  status  enabledBit   createPointID
    { UID_Guardian,        800.0f, MAP_CLOUDRUNNER_DUNGEON,  BIT_Play_Seq_0057_CF_PowerRoom_WindLifts_Activating,   3,        0,      NO_GAMEBIT,  0  },
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
} SeqObjGroupToggle;
/*0xD0*/ static SeqObjGroupToggle sSeqObjGroupToggles[] = {
    //status gamebit count                            objgroup   gamebits
    { 1,     ARRAYCOUNT(sBabyCRStartPerchLandBits),   5,         sBabyCRStartPerchLandBits }, // Enable throne room objgroup when cfbaby perch seq starts
    { 0,     ARRAYCOUNT(sBabyCREndPerchLandBits),     5,         sBabyCREndPerchLandBits },   // Disable throne room objgroup when cfbaby perch seq ends
    { 1,     ARRAYCOUNT(sTreasureRoomStartSeqBits),   3,         sTreasureRoomStartSeqBits },
    { 0,     ARRAYCOUNT(sTreasureRoomEndSeqBits),     3,         sTreasureRoomEndSeqBits },
    { 1,     ARRAYCOUNT(sCourtyardWaterSeqStartBits), 25,        sCourtyardWaterSeqStartBits },
    { 0,     ARRAYCOUNT(sCourtyardWaterSeqEndBits),   25,        sCourtyardWaterSeqEndBits },
    { 1,     ARRAYCOUNT(sTreasRoboStartSeqBits),      22,        sTreasRoboStartSeqBits },
    { 0,     ARRAYCOUNT(sTreasRoboEndSeqBits),        22,        sTreasRoboEndSeqBits }
};
/*0x110*/ static u32 sNumSeqObjGroupToggles = ARRAYCOUNT(sSeqObjGroupToggles);

// size: 0x6
typedef struct {
/*00*/ s16 enableBit;
/*02*/ s16 disableBit;
/*04*/ s16 objgroup;
} SimpleObjGroupToggle;
/*0x114*/ static SimpleObjGroupToggle sSimpleObjGroupToggles[] = {
    //enableBit                    disableBit                    objgroup
    { BIT_CRF_Enable_ObjGroup_22,  BIT_CRF_Disable_ObjGroup_22,  22 }
};

// @bug: This array is not large enough to list all potentially 31 enabled objgroups.
//       While this doesn't happen in practice, if too many are enabled then this will be overrun.
/*0x11C*/ static s16 sPrevEnabledObjGroups[] = {
    -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, -1
};
/*0x140*/ static s32 data_140[] = { // unused
    UID_Chest_1,
    UID_Chest_2,
    UID_Chest_3,
    UID_Chest_With_Baby
};

static void CFLevelControl_doSeqObjGroupToggling(Object* self, SeqObjGroupToggle* toggles, s32 count);
static void CFLevelControl_restoreSeqObjGroupToggles(Object* self, SeqObjGroupToggle* toggles, s32 count);
static s32 CFLevelControl_setBitAfterRequiredBits(s16 bit, s16* requiredBits, s32 requiredBitsCount);
static void CFLevelControl_kyteTrappedControl(void);
static void CFLevelControl_doBabyCRObjGroupToggling(BabyCRObjGroupToggle* t, s32 count);
static void CFLevelControl_doSimpleObjGroupToggling(SimpleObjGroupToggle* t, s32 count);
static void CFLevelControl_doDistBasedObjGroupToggling(DistObjGroupToggle* t, s32 count);
static void CFLevelControl_resetObjPositions(DistObjGroupToggle* t, s32 count);
static Vec3f* CFLevelControl_getPositionOfSavedObj(s32);
static void CFLevelControl_treasRoboControl(void);
static s32 CFLevelControl_entranceControl(Object* self);

// offset: 0x0 | ctor
void CFLevelControl_ctor(void* dll) { }

// offset: 0xC | dtor
void CFLevelControl_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFLevelControl_obj_Setup(Object* self, ObjSetup* setup, s32 arg2) {
    CFLevelControl_Data* objdata = self->data;
    
    objdata->flags = CFLEVELCONTROL_IsFirstTick;
    sTriggerPassed = mainGetBits(BIT_CF_Entrance_Trigger_Passed);
    CFLevelControl_restoreSeqObjGroupToggles(self, sSeqObjGroupToggles, sNumSeqObjGroupToggles);
    sLightningProjgfx = dllLoad(DLL_ID_190, 1);
    mainCreateTempDLL(DLL_ID_MOVELIB);
}

// offset: 0xCC | func: 1 | export: 1
void CFLevelControl_obj_Control(Object* self) {
    CFLevelControl_Data* objdata = self->data;

    if (objdata->flags & CFLEVELCONTROL_IsFirstTick) {
        CFLevelControl_resetObjPositions(sDistObjGroupToggles, ARRAYCOUNT(sDistObjGroupToggles));
        objdata->flags &= ~CFLEVELCONTROL_IsFirstTick;
    }
    CFLevelControl_doSeqObjGroupToggling(self, sSeqObjGroupToggles, sNumSeqObjGroupToggles);
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
    CFLevelControl_kyteTrappedControl();
    CFLevelControl_doBabyCRObjGroupToggling(sBabyCRObjGroupToggles, ARRAYCOUNT(sBabyCRObjGroupToggles));
    CFLevelControl_doDistBasedObjGroupToggling(sDistObjGroupToggles, ARRAYCOUNT(sDistObjGroupToggles));
    // Disable Galleon dock objgroup 22 once the CFTreasRobo segment starts
    if (!mainGetBits(BIT_Play_Seq_02C6_CF_Sharpclaw_Only_Four_Chests_Left)) {
        CFLevelControl_doSimpleObjGroupToggling(sSimpleObjGroupToggles, ARRAYCOUNT(sSimpleObjGroupToggles));
    }
    CFLevelControl_entranceControl(self);
    CFLevelControl_treasRoboControl();
    diPrintf(" Layer NO %i : ", mapGetLayer());
}

// offset: 0x2B4 | func: 2 | export: 2
void CFLevelControl_obj_Update(Object* self) { }

// offset: 0x2C0 | func: 3 | export: 3
void CFLevelControl_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x314 | func: 4 | export: 4
void CFLevelControl_obj_Free(Object* self, s32 a1) {
    mainRemoveTempDLL(DLL_ID_MOVELIB);
    if (sLightningProjgfx) {
        dllFree(sLightningProjgfx);
    }
}

// offset: 0x378 | func: 5 | export: 5
u32 CFLevelControl_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x388 | func: 6 | export: 6
u32 CFLevelControl_obj_GetDataSize(Object* self, u32 a1) {
    return sizeof(CFLevelControl_Data);
}

// offset: 0x39C | func: 7
static s32 CFLevelControl_getObjGroupStatus(s16 groupBit, s32 group) {
    return (mainGetBits(groupBit) >> group) & 1;
}

// offset: 0x3F0 | func: 8
static void CFLevelControl_doSeqObjGroupToggling(Object* self, SeqObjGroupToggle* toggles, s32 count) {
    s32 i, j, k;
    s16* bits;
    s32 bitWasSet;
    s32 statusChanged;
    SeqObjGroupToggle* t;

    for (i = 0; i < count; i++) {
        t = &toggles[i];
        bits = t->bits;
        bitWasSet = FALSE;
        statusChanged = FALSE;
        // Check if a bit in the list was set. Once set, ignore it going forward.
        for (j = 0; j < t->bitCount; j++) {
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
            if (CFLevelControl_getObjGroupStatus(BIT_CF_ObjGroup_Bits, t->objgroup) != t->status) {
                STUBBED_PRINTF(" Loading Group %i \n", t->objgroup);
                dll_gplay->set_obj_group_status(self->mapID, t->objgroup, t->status);
                statusChanged = TRUE;
            }
        }
        // For the throne room specifically, disable *all* other objgroups during the sequence of
        // a baby CloudRunner landing on their perch. Restore the objgroups after once we go to
        // disable the throne room objgroup.
        if (statusChanged && t->objgroup == 5) { // objgroup 5 == throne room
            k = 0;
            if (t->status == 1) {
                // Disable everything except the throne room
                for (j = 0; j < 31; j++) {
                    if ((CFLevelControl_getObjGroupStatus(BIT_CF_ObjGroup_Bits, j)) && (j != t->objgroup)) {
                        sPrevEnabledObjGroups[k] = j;
                        k++;
                        STUBBED_PRINTF(" Cnt %i V %i ", k, j);
                        dll_gplay->set_obj_group_status(self->mapID, j, 0);
                    }
                }
                sPrevEnabledObjGroups[k] = -1; // terminate list
            } else {
                // Restore previously enabled groups
                while (sPrevEnabledObjGroups[k] != -1 && k < 31) {
                    STUBBED_PRINTF(" cnt %i v %i ", k, sPrevEnabledObjGroups[k]);
                    dll_gplay->set_obj_group_status(self->mapID, sPrevEnabledObjGroups[k], 1);
                    k++;
                }
            }
        }
    }
}

// offset: 0x67C | func: 9
static void CFLevelControl_restoreSeqObjGroupToggles(Object* self, SeqObjGroupToggle* toggles, s32 count) {
    s32 i, j;
    s16* gamebits;
    s32 delta;
    SeqObjGroupToggle* t;

    STUBBED_PRINTF(" RESETING GROUP LOAD ");

    for (i = 0; i < count; i++) {
        // Check enable bits
        t = &toggles[i];
        gamebits = t->bits;
        delta = 0;
        for (j = 0; j < t->bitCount; j++) {
            if (gamebits[j] == NO_GAMEBIT) {
                continue;
            } else if (mainGetBits(gamebits[j])) {
                gamebits[j] = NO_GAMEBIT;
                delta++;
            }
        }
        // Check disable bits
        // Note: it's expected that the toggles array always has two entries per objgroup, one
        // for enabling the group and one for disabling the group, in that order.
        i++;
        t = &toggles[i];
        gamebits = t->bits;
        for (j = 0; j < t->bitCount; j++) {
            if (gamebits[j] == NO_GAMEBIT) {
                continue;
            } else if (mainGetBits(gamebits[j])) {
                gamebits[j] = NO_GAMEBIT;
                delta--;
            }
        }
        // delta will be non-zero if an enable bit is set but not the disable bit for the same objgroup.
        if (delta != 0) {
            dll_gplay->set_obj_group_status(self->mapID, t->objgroup, 1);
        }
    }
}

/*0x5C*/ static const char str_5C[] = " %i";

// offset: 0x82C | func: 10
static s32 CFLevelControl_setBitAfterRequiredBits(s16 bit, s16* requiredBits, s32 requiredBitsCount) {
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
static void CFLevelControl_kyteTrappedControl(void) {
    if (mainGetBits(BIT_Played_Seq_0041_Scales_Kills_The_Queen) && !mainGetBits(BIT_CF_Floor_Destroyed)) {
        if (mainGetBits(BIT_Kyte_Flight_Curve) != 0x11) {
            mainSetBits(BIT_Kyte_Flight_Curve, 0x11); // throne room flight group inside the forcefield bubble
            mainSetBits(BIT_Kyte_Trapped, 1);
            STUBBED_PRINTF(" KYTE TRAPPED \n "); // guessed location
        }
    } else if (mainGetBits(BIT_Kyte_Trapped)) {
        mainSetBits(BIT_Kyte_Trapped, 0);
        STUBBED_PRINTF(" KYTE ESCAPPED \n "); // guessed location
    }
}

/*0x88*/ static const char str_88[] = " CRAP IS BOLLOX ";

// offset: 0x9EC | func: 12
static void CFLevelControl_doBabyCRObjGroupToggling(BabyCRObjGroupToggle* t, s32 count) {
    Object* cloudbaby;

    while (count--) {
        if (mainGetBits(t->gamebit)) {
            cloudbaby = objGetObjectByUID(t->uID);
            if (cloudbaby && (((DLL_373_CFCloudBaby*)cloudbaby->dll)->vtbl->IsRescuedTimerDone(cloudbaby))) {
                dll_gplay->set_obj_group_status(cloudbaby->mapID, t->objgroup, 0);
                STUBBED_PRINTF(" Freeing Baby ");
                mainSetBits(t->gamebit, 0);
            }
        }
        t++;
    }
}

/*0xAC*/ static const char str_AC[] = " CRAP IS BOLLOX ";
/*0xC0*/ static const char str_C0[] = " CRAP IS BOLLOX ";
/*0xD4*/ static const char str_D4[] = " CRAP IS BOLLOX ";

// offset: 0xAE8 | func: 13
static void CFLevelControl_doSimpleObjGroupToggling(SimpleObjGroupToggle* t, s32 count) {
    while (count--) {
        if (mainGetBits(t->enableBit) && !mainGetBits(t->disableBit)) {
            dll_gplay->set_obj_group_status(MAP_CLOUDRUNNER_FORTRESS, t->objgroup, 1);
            mainSetBits(t->enableBit, 0);
        }
        t++;
    }
}

// offset: 0xBB8 | func: 14
static void CFLevelControl_doDistBasedObjGroupToggling(DistObjGroupToggle* t, s32 count) {
    Vec3f* objPos;
    Object* chestOrGuardian;
    f32 distance;

    while (count--) {
        if ((t->enabledBit == NO_GAMEBIT) || (mainGetBits(t->enabledBit))) {
            objPos = CFLevelControl_getPositionOfSavedObj(t->uID);
            if (objPos) {
                // Compare distance of whichever is closer: the camera or CFTreasRobo
                distance = camDistance(objPos->x, objPos->y, objPos->z);
                objGetNearestType(OBJTYPE_CFTreasRobo, objPos, &distance);
                chestOrGuardian = objGetObjectByUID(t->uID);
                if (chestOrGuardian) {
                    if (t->distance < distance && ((t->toggleBit == NO_GAMEBIT) || (!mainGetBits(t->toggleBit))) && (((DLL_ICFCanUnload*)chestOrGuardian->dll)->vtbl->CanUnload(chestOrGuardian))) {
                        // Moved far away and obj should unload, disable objgroup
                        t->status = 0;
                        dll_gplay->set_obj_group_status(chestOrGuardian->mapID, t->objgroup, t->status);
                    }
                } else {
                    if (distance < t->distance || ((t->toggleBit != NO_GAMEBIT) && (mainGetBits(t->toggleBit)))) {
                        // Moved close and obj should load, enable objgroup
                        if (!t->status) {}
                        t->status = 1;
                        dll_gplay->set_obj_group_status(t->mapID, t->objgroup, t->status);
                    }
                }
            }
        }
        t++;
    }
}

// offset: 0xDC0 | func: 15
static void CFLevelControl_resetObjPositions(DistObjGroupToggle* t, s32 count) {
    ObjSetup* setup;
    SRT transform;

    while (count--) {
        setup = mapFindObjSetup(t->uID, NULL, NULL, NULL, NULL);
        if (setup) {
            if (t->createPointID) {
                // Reset setup position to create point curve
                ((DLL_53_movelib*)(gTempDLLInsts[1]))->vtbl->func7(t->createPointID, &transform);
                setup->x = transform.transl.x;
                setup->y = transform.transl.y;
                setup->z = transform.transl.z;
            }
            mapSaveObject(setup, t->mapID, setup->x, setup->y, setup->z);
        }
        t++;
    }
}

// offset: 0xEC8 | func: 16
static Vec3f* CFLevelControl_getPositionOfSavedObj(s32 uID) {
    s16 numSavedObjs;
    SavedObject* savedObjs;
    s32 i;

    numSavedObjs = dll_gplay->get_num_saved_objects();
    savedObjs = dll_gplay->get_saved_objects();
    for (i = 0; i < numSavedObjs; i++) {
        if (uID == savedObjs[i].uID) {
            return (Vec3f*)&savedObjs[i].x;
        }
    }
    STUBBED_PRINTF(" Couldn't find romdef %i ", uID); // guessed location
    return NULL;
}

// offset: 0x1000 | func: 17
static void CFLevelControl_treasRoboControl(void) {
    diPrintf(" STart Seq Val %i ", mainGetBits(BIT_Play_Seq_02C6_CF_Sharpclaw_Only_Four_Chests_Left));
    // Disable CFTreasRobo objgroup once the baby in the chest is freed
    if (dll_gplay->get_obj_group_status(MAP_CLOUDRUNNER_FORTRESS, 23) && mainGetBits(BIT_CF_Free_Cloudrunner_From_Chest)) {
        dll_gplay->set_obj_group_status(MAP_CLOUDRUNNER_FORTRESS, 23, 0);
    }
}

// offset: 0x10BC | func: 18
static s32 CFLevelControl_entranceControl(Object* self) {
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
        // Do a sneaky lightning flash to distract from hiding the stairwell while the real thing loads 
        gDLL_28_ScreenFade->vtbl->func3(10, SCREEN_FADE_WHITE, 0.3f);
        sLightningProjgfx->vtbl->func0(self, 2, &transform, 1, -1, 4, 0);
        sLightningProjgfx->vtbl->func0(self, 2, &transform, 1, -1, 4, 0);
        sLightningProjgfx->vtbl->func0(self, 2, &transform, 1, -1, 4, 0);
        dll_amSfx->Play(self, SOUND_73_Thunder, MAX_VOLUME, NULL, NULL, 0, NULL);
        STUBBED_PRINTF(" you have Passed "); // guessed location
        sTriggerPassed = mainGetBits(BIT_CF_Entrance_Trigger_Passed);
    } else if ((mathRnd(0, 100) == 0) && (mainGetBits(BIT_CRF_Lightning_Enabled))) {
        rand = mathRnd(5, 10);
        gDLL_28_ScreenFade->vtbl->func3(rand, SCREEN_FADE_WHITE, 0.1f + rand * 0.05f);
        sLightningProjgfx->vtbl->func0(self, 2, NULL, 1, -1, 4, 0);
        dll_amSfx->Play(self, SOUND_73_Thunder, 77 + rand * 10 , NULL, NULL, 0, NULL);
        STUBBED_PRINTF(" Lighting Flash "); // guessed location
    }
    return 1;
}
