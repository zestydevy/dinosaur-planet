#include "PR/os.h"
#include "PR/ultratypes.h"

#include "dlls/engine/28_screen_fade.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/engine/31_flash.h"
#include "game/gamebits.h"
#include "sys/asset_thread.h"
#include "sys/dll.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/fs.h"
#include "sys/menu.h"
#include "sys/memory.h"
#include "functions.h"
#include "dll.h"
#include "prevent_bss_reordering.h"

static const char str1[] = "gplayLoadGame error: savegame and backup failed to load.\n";
static const char str2[] = "gplayLoadOptions error: saveoptions failed to load.\n";
static const char str3[] = " WARNING : Not Enough Memort for Restart Point ";
static const char str4[] = "\nWARNING gplay : Restart Point Not Set \n";
static const char str5[] = "gameplay.c: gplayGetCurrentPlayerLactions: playerno overflow\n";
static const char str6[] = "gameplay.c: gplayGetCurrentPlayerEnvactions: playerno overflow\n";
static const char str7[] = "gplayAddTime: MAX_TIMESAVES reached\n";
static const char str8[] = "Error in gplaySetObjGroupStatus (mapno %d, groupno %d)\n";
static const char str9[] = "CHEAT OUT OF RANGE\n";
static const char str10[] = "CHEAT OUT OF RANGE\n";
static const char str11[] = "CHEAT OUT OF RANGE\n";
static const char str12[] = "CHEAT OUT OF RANGE\n";
static const char str13[] = "CINEMA OUT OF RANGE\n";
static const char str14[] = "CINEMA OUT OF RANGE\n";

// Map of map ID -> bit table key
static u16 sMapSetupBitKeys[120] = {
/*000*/ 0x0000, /*001*/ 0x0000, 
/*002*/ BIT_DR_Top_Map_Setup, // MAP_DRAGON_ROCK_TOP
/*003*/ BIT_KP_Map_Setup, // MAP_KRAZOA_PALACE
/*004*/ BIT_VFP_Map_Setup, // MAP_VOLCANO_FORCE_POINT_TEMPLE
/*005*/ BIT_Rolling_Demo_Map_Setup, // MAP_ROLLING_DEMO
/*006*/ BIT_DF_Map_Setup, // MAP_DISCOVERY_FALLS
/*007*/ BIT_SH_Map_Setup, // MAP_SWAPSTONE_HOLLOW
/*008*/ BIT_SH_Map_Setup, // MAP_SWAPSTONE_HOLLOW_2
/*009*/ BIT_GP_Map_Setup, // MAP_GOLDEN_PLAINS
/*010*/ BIT_SW_Map_Setup, // MAP_SNOWHORN_WASTES
/*011*/ BIT_WM_Map_Setup, // MAP_WARLOCK_MOUNTAIN
/*012*/ BIT_CF_Map_Setup, // MAP_CLOUDRUNNER_FORTRESS
/*013*/ BIT_WC_Map_Setup, // MAP_WALLED_CITY
/*014*/ BIT_SC_Map_Setup, // MAP_SWAPSTONE_CIRCLE
/*015*/ BIT_CF_Treasure_Map_Setup, // MAP_CLOUDRUNNER_TREASURE
/*016*/ BIT_CF_Dungeon_Map_Setup, // MAP_CLOUDRUNNER_DUNGEON
/*017*/ BIT_CF_TrapRooms_Map_Setup, // MAP_CLOUDRUNNER_TRAPROOMS
/*018*/ BIT_MMP_Map_Setup, // MAP_MOON_MOUNTAIN_PASS
/*019*/ BIT_DIM_Map_Setup, // MAP_DARK_ICE_MINES_1
/*020*/ 0x0000, 
/*021*/ BIT_DFP_Map_Setup, // MAP_DESERT_FORCE_POINT_TEMPLE_BOTTOM
/*022*/ 0x0000, 
/*023*/ BIT_IM_Map_Setup, // MAP_ICE_MOUNTAIN_1
/*024*/ BIT_IM_Map_Setup, // MAP_ICE_MOUNTAIN_2
/*025*/ BIT_IM_Map_Setup, // MAP_ICE_MOUNTAIN_3
/*026*/ BIT_AnimTest_Map_Setup, // MAP_ANIMTEST
/*027*/ 0x0000, 
/*028*/ BIT_Boss_Galadon_Map_Setup, // MAP_BOSS_GALADON
/*029*/ BIT_CC_Map_Setup, // MAP_CAPE_CLAW
/*030*/ 0x0000, 
/*031*/ BIT_DF_Shrine_Map_Setup, // MAP_SHRINE_DISCOVERY_FALLS
/*032*/ 0x0000, /*033*/ 0x0000, /*034*/ 0x0000, 
/*035*/ BIT_DB_Map_Setup, // MAP_DIAMOND_BAY
/*036*/ 0x0000, /*037*/ 0x0000, /*038*/ 0x0000, /*039*/ 0x0000, 
/*040*/ 0x0000, /*041*/ 0x0000, /*042*/ 0x0000, /*043*/ 0x0000, 
/*044*/ BIT_Boss_Drakor_Map_Setup, // MAP_BOSS_DRAKOR
/*045*/ 0x0000, 
/*046*/ BIT_DIM_Map_Setup, // MAP_DARK_ICE_MINES_CAVES
/*047*/ BIT_DIM_Map_Setup, // MAP_DARK_ICE_MINES_LAVA
/*048*/ BIT_Boss_Klanadack_Map_Setup, // MAP_BOSS_KLANADACK
/*049*/ 0x0000, 
/*050*/ BIT_DFP_Map_Setup, // MAP_DESERT_FORCE_POINT_TEMPLE_TOP
/*051*/ 0x0000, 
/*052*/ BIT_DR_Bottom_Map_Setup, // MAP_DRAGON_ROCK_BOTTOM
/*053*/ 0x0000, /*054*/ 0x0000, /*055*/ 0x0000, /*056*/ 0x0000, 
/*057*/ 0x0000, /*058*/ 0x0000, /*059*/ 0x0000, /*060*/ 0x0000, 
/*061*/ 0x0000, /*062*/ 0x0000, /*063*/ 0x0000, /*064*/ 0x0000, 
/*065*/ 0x0000, /*066*/ 0x0000, /*067*/ 0x0000, /*068*/ 0x0000, 
/*069*/ 0x0000, /*070*/ 0x0000, /*071*/ 0x0000, /*072*/ 0x0000, 
/*073*/ 0x0000, /*074*/ 0x0000, /*075*/ 0x0000, /*076*/ 0x0000, 
/*077*/ 0x0000, /*078*/ 0x0000, /*079*/ 0x0000, /*080*/ 0x0000, 
/*081*/ 0x0000, /*082*/ 0x0000, /*083*/ 0x0000, /*084*/ 0x0000, 
/*085*/ 0x0000, /*086*/ 0x0000, /*087*/ 0x0000, /*088*/ 0x0000, 
/*089*/ 0x0000, /*090*/ 0x0000, /*091*/ 0x0000, /*092*/ 0x0000, 
/*093*/ 0x0000, /*094*/ 0x0000, /*095*/ 0x0000, /*096*/ 0x0000, 
/*097*/ 0x0000, /*098*/ 0x0000, /*099*/ 0x0000, /*100*/ 0x0000, 
/*101*/ 0x0000, /*102*/ 0x0000, /*103*/ 0x0000, /*104*/ 0x0000, 
/*105*/ 0x0000, /*106*/ 0x0000, /*107*/ 0x0000, /*108*/ 0x0000, 
/*109*/ 0x0000, /*110*/ 0x0000, /*111*/ 0x0000, /*112*/ 0x0000, 
/*113*/ 0x0000, /*114*/ 0x0000, /*115*/ 0x0000, /*116*/ 0x0000, 
/*117*/ 0x0000, /*118*/ 0x0000, /*119*/ 0x0000
};
// Map of map ID -> bit table key
static u16 sMapObjGroupBitKeys[120] = {
/*000*/ BIT_Front_End_ObjGroup_Bits, // MAP_FRONT_END
/*001*/ BIT_Front_End_ObjGroup_Bits, // MAP_FRONT_END2
/*002*/ BIT_DR_Top_ObjGroup_Bits, // MAP_DRAGON_ROCK_TOP
/*003*/ BIT_KP_ObjGroup_Bits, // MAP_KRAZOA_PALACE
/*004*/ BIT_VFP_ObjGroup_Bits, // MAP_VOLCANO_FORCE_POINT_TEMPLE
/*005*/ BIT_Rolling_Demo_ObjGroup_Bits, // MAP_ROLLING_DEMO
/*006*/ BIT_DF_ObjGroup_Bits, // MAP_DISCOVERY_FALLS
/*007*/ BIT_SH_ObjGroup_Bits, // MAP_SWAPSTONE_HOLLOW
/*008*/ BIT_SH_ObjGroup_Bits, // MAP_SWAPSTONE_HOLLOW_2
/*009*/ BIT_GP_ObjGroup_Bits, // MAP_GOLDEN_PLAINS
/*010*/ BIT_SW_ObjGroup_Bits, // MAP_SNOWHORN_WASTES
/*011*/ BIT_WM_ObjGroup_Bits, // MAP_WARLOCK_MOUNTAIN
/*012*/ BIT_CF_ObjGroup_Bits, // MAP_CLOUDRUNNER_FORTRESS
/*013*/ BIT_WC_ObjGroup_Bits, // MAP_WALLED_CITY
/*014*/ BIT_SC_ObjGroup_Bits, // MAP_SWAPSTONE_CIRCLE
/*015*/ BIT_CF_Treasure_ObjGroup_Bits, // MAP_CLOUDRUNNER_TREASURE
/*016*/ BIT_CF_Dungeon_ObjGroup_Bits, // MAP_CLOUDRUNNER_DUNGEON
/*017*/ 0x0000, 
/*018*/ BIT_MMP_ObjGroup_Bits, // MAP_MOON_MOUNTAIN_PASS
/*019*/ BIT_DIM_ObjGroupBits, // MAP_DARK_ICE_MINES_1
/*020*/ 0x0000, 
/*021*/ BIT_DFP_ObjGroup_Bits, // MAP_DESERT_FORCE_POINT_TEMPLE_BOTTOM
/*022*/ 0x0000, 
/*023*/ BIT_IM_ObjGroup_Bits, // MAP_ICE_MOUNTAIN_1
/*024*/ BIT_IM_ObjGroup_Bits, // MAP_ICE_MOUNTAIN_2
/*025*/ BIT_IM_ObjGroup_Bits, // MAP_ICE_MOUNTAIN_3
/*026*/ BIT_AnimTest_ObjGroup_Bits, // MAP_ANIMTEST
/*027*/ BIT_DIM2_ObjGroup_Bits, // MAP_DARK_ICE_MINES_2
/*028*/ BIT_Boss_Galadon_ObjGroup_Bits, // MAP_BOSS_GALADON
/*029*/ BIT_CC_ObjGroup_Bits, // MAP_CAPE_CLAW
/*030*/ BIT_Galleon_Inside_ObjGroup_Bits, // MAP_INSIDE_GALLEON
/*031*/ 0x0000, /*032*/ 0x0000, /*033*/ 0x0000, /*034*/ 0x0000, 
/*035*/ BIT_DB_ObjGroup_Bits, // MAP_DIAMOND_BAY
/*036*/ 0x0000, /*037*/ 0x0000, /*038*/ 0x0000, 
/*039*/ BIT_DB_Shrine_ObjGroup_Bits, // MAP_SHRINE_DIAMOND_BAY
/*040*/ 0x0000, /*041*/ 0x0000, /*042*/ 0x0000, 
/*043*/ BIT_CF_RaceTrack_ObjGroup_Bits, // MAP_CLOUDRUNNER_RACETRACK
/*044*/ 0x0000, /*045*/ 0x0000, 
/*046*/ BIT_DIM_ObjGroupBits, // MAP_DARK_ICE_MINES_CAVES
/*047*/ BIT_DIM_ObjGroupBits, // MAP_DARK_ICE_MINES_LAVA
/*048*/ BIT_Boss_Klanadack_ObjGroup_Bits, // MAP_BOSS_KLANADACK
/*049*/ 0x0000, 
/*050*/ BIT_DFP_ObjGroup_Bits, // MAP_DESERT_FORCE_POINT_TEMPLE_TOP
/*051*/ BIT_SwapStone_Store_ObjGroup_Bits, // MAP_SWAPSTONE_STORE
/*052*/ BIT_DR_Bottom_ObjGroup_Bits, // MAP_DRAGON_ROCK_BOTTOM
/*053*/ 0x0000, /*054*/ 0x0000, /*055*/ 0x0000, /*056*/ 0x0000, 
/*057*/ 0x0000, /*058*/ 0x0000, /*059*/ 0x0000, 
/*060*/ BIT_Front_End_ObjGroup_Bits, // MOBILE_MAP_GALLEONSHIP
/*061*/ 0x0000, /*062*/ 0x0000, /*063*/ 0x0000, /*064*/ 0x0000, 
/*065*/ 0x0000, /*066*/ 0x0000, /*067*/ 0x0000, /*068*/ 0x0000, 
/*069*/ 0x0000, /*070*/ 0x0000, /*071*/ 0x0000, /*072*/ 0x0000, 
/*073*/ 0x0000, /*074*/ 0x0000, /*075*/ 0x0000, /*076*/ 0x0000, 
/*077*/ 0x0000, /*078*/ 0x0000, /*079*/ 0x0000, /*080*/ 0x0000, 
/*081*/ 0x0000, /*082*/ 0x0000, /*083*/ 0x0000, /*084*/ 0x0000, 
/*085*/ 0x0000, /*086*/ 0x0000, /*087*/ 0x0000, /*088*/ 0x0000, 
/*089*/ 0x0000, /*090*/ 0x0000, /*091*/ 0x0000, /*092*/ 0x0000, 
/*093*/ 0x0000, /*094*/ 0x0000, /*095*/ 0x0000, /*096*/ 0x0000, 
/*097*/ 0x0000, /*098*/ 0x0000, /*099*/ 0x0000, /*100*/ 0x0000, 
/*101*/ 0x0000, /*102*/ 0x0000, /*103*/ 0x0000, /*104*/ 0x0000, 
/*105*/ 0x0000, /*106*/ 0x0000, /*107*/ 0x0000, /*108*/ 0x0000, 
/*109*/ 0x0000, /*110*/ 0x0000, /*111*/ 0x0000, /*112*/ 0x0000, 
/*113*/ 0x0000, /*114*/ 0x0000, /*115*/ 0x0000, /*116*/ 0x0000, 
/*117*/ 0x0000, /*118*/ 0x0000, /*119*/ 0x0000
};
static Vec3f sPlayerStartPos = { -14149.352f, -82.0f, -15569.178f };

typedef struct {
    s8 mapID;
    s8 setupID;
} MapSetup;

typedef struct {
    s32 mapID;
    // Bitfield of the status of each object group
    s32 groupBits;
} MapObjGroupBits;

/*0x0*/    static GplaySaveFlash *sSavegame;
/*0x4*/    static Savegame *sRestartSave;
/*0x8*/    static GplayOptions *sGameOptions;
/*0x10*/   static GameState sState;
/*0x183C*/ static s8 sSavegameIdx; // current savegame index (slotno)
/*0x1840*/ static u8 bss_1840[40];
/*0x1868*/ static u32 sAllMapObjGroups[120]; // obj group bits for each map
/*0x1A48*/ static MapSetup sMapSetup;
/*0x1A50*/ static MapObjGroupBits sMapObjGroups; // obj group bits for current map

s32 gplay_load_save(s8 idx, u8 startGame);
void gplay_save_game(void);
void gplay_checkpoint(Vec3f *position, s16 yaw, s32 param3, s32 mapLayer);
void gplay_start_loaded_game(void);
static void gplay_start_game(void);
static void gplay_reset_state(void);
void gplay_set_map_setup(s32 mapID, s32 setupID);
void gplay_world_load_obj_group_bits(s32 mapID);

void gplay_ctor(DLLFile *self)  {
    sSavegame = (GplaySaveFlash*)mmAlloc(sizeof(GplaySaveFlash), COLOUR_TAG_YELLOW, NULL);
    gplay_reset_state();
    sRestartSave = NULL;
    sGameOptions = (GplayOptions*)mmAlloc(sizeof(GplayOptions), COLOUR_TAG_YELLOW, NULL);
    sMapSetup.mapID = -1;
    sMapObjGroups.mapID = -1;
}

void gplay_dtor(DLLFile *self)  {
    mmFree(sSavegame);
    if (sRestartSave != NULL) {
        mmFree(sRestartSave);
    }
    mmFree(sGameOptions);
}

void gplay_erase_save(s8 idx) {
    gplay_reset_state();
    sSavegameIdx = idx;
    sState.save.file.isEmpty = 1;
    sState.save.file.timePlayed = -1.0f;
    // Save twice to erase both copies
    gplay_save_game();
    gplay_save_game();
}

void gplay_init_save(s8 idx, char *filename) {
    u8 _pad[0x10];
    Vec3f startPos;
    s32 i;
    s32 k;
    char *dst;
    char var;

    startPos = sPlayerStartPos;

    gplay_reset_state();

    sSavegameIdx = idx;
    sState.save.file.playerno = PLAYER_KRYSTAL;

    for (i = 0; i < 2; i++) {

        sState.save.file.players[i].health = 12;
        sState.save.file.players[i].healthMax = 12;
        sState.save.file.players[i].magicMax = 25;
        sState.save.file.players[i].magic = 0;

        sState.save.file.unk18[i].unk0 = 4;
        sState.save.file.unk18[i].unk1 = 5;

        sState.save.map.unk16F4[i].unk0 = -1;
        sState.save.map.unk16F4[i].unk2 = -1;
        sState.save.map.unk16F4[i].unk4 = -1;
        sState.save.map.unk16F4[i].unk6 = -1;
        sState.save.map.unk16F4[i].unk8 = -1;
        sState.save.map.unk16F4[i].unkA = -1;
        sState.save.map.unk16F4[i].unkC = -1;
        sState.save.map.unk16F4[i].unkE = -1;
        sState.save.map.unk16F4[i].unk12 = -1;

        sState.save.map.unk171C[i].unk0 = 43000.0f;
        sState.save.map.unk171C[i].unk4 = -1;
        sState.save.map.unk171C[i].unk6 = -1;
        sState.save.map.unk171C[i].unk8 = -1;
        sState.save.map.unk171C[i].unkA = -1;
        sState.save.map.unk171C[i].unkC = -1;
        sState.save.map.unk171C[i].unkE = -1;
        sState.save.map.unk171C[i].unk3D = -1;
        sState.save.map.unk171C[i].unk3E = -1;
        sState.save.map.unk171C[i].unk3F = -1;
        sState.save.map.unk171C[i].unk3C = 1;

        for (k = 0; k < 4; k++) {
            sState.save.map.unk179C[i].unk0[k] = -1;
        }
    }

    for (i = 0; i < 120; i++) {
        if (sMapSetupBitKeys[i] != 0) {
            gDLL_29_Gplay->vtbl->set_map_setup(i, 1);
        }
    }

    main_set_bits(BIT_Tricky_Spawns, 1);

    if (filename != NULL) {
        dst = &sState.save.file.name[0];

        var = *filename;
        *dst = var;
        dst++;
        filename++;

        while (var != NULL) {
            var = *filename;
            *dst = var;
            dst++;
            filename++;
        }
    }

    gplay_checkpoint(&startPos, 0, 0, 0);
    // Save twice to init both copies
    gplay_save_game();
    gplay_save_game();
}

s32 gplay_load_save(s8 idx, u8 startGame) {
    u8 loadStatus0;
    u8 loadStatus1;
    GplaySaveFlash *copy1Ptr;
    s32 ret;

    if (idx != sSavegameIdx) {
        if (idx < MAX_SAVEGAMES) {
            sSavegameIdx = idx;

            // Depending on whether this savegame was saved an even or odd number of times determines
            // which copy is the main savegame and which is the backup. Load both and compare.
            copy1Ptr = (GplaySaveFlash*)mmAlloc(sizeof(GplaySaveFlash), COLOUR_TAG_YELLOW, NULL);

            loadStatus0 = gDLL_31_Flash->vtbl->load_game(&sSavegame->asFlash, idx,                 sizeof(FlashStruct), TRUE); // copy 0
            loadStatus1 = gDLL_31_Flash->vtbl->load_game(&copy1Ptr->asFlash,  idx + MAX_SAVEGAMES, sizeof(FlashStruct), TRUE); // copy 1

            if (!loadStatus0) {
                if (!loadStatus1) {
                    // both copies of the savegame failed to load
                    mmFree(copy1Ptr);
                    ret = 0;
                } else {
                    // copy 0 failed but copy 1 is good, use that one
                    mmFree(sSavegame);
                    ret = 2;
                    sSavegame = copy1Ptr;
                }
            } else if (!loadStatus1) {
                // copy 1 failed but copy 0 is good, use that one
                mmFree(copy1Ptr);
                ret = 2; // bug? shouldn't this be 1 since copy 0 is being returned?
            } else if (copy1Ptr->asSave.file.timePlayed < sSavegame->asSave.file.timePlayed) {
                // both copies are good but copy 0 is newer, use that one
                mmFree(copy1Ptr);
                ret = 1;
            } else {
                // both copies are good but copy 1 is newer, use that one
                mmFree(sSavegame);
                ret = 2;
                sSavegame = copy1Ptr;
            }
        } else {
            queue_load_file_region_to_ptr(
                (void**)&sSavegame->asFlash,
                SAVEGAME_BIN,
                idx * sizeof(FlashStruct) - (sizeof(FlashStruct) * 4),
                sizeof(FlashStruct));
            ret = 1;
        }
    } else {
        ret = 1;
    }

    if (startGame) {
        gplay_start_loaded_game();
    } else {
        bcopy(&sSavegame->asSave, &sState.save, sizeof(Savegame));
    }

    return ret;
}

void gplay_copy_save(s8 srcIdx, s8 dstIdx) {
    gplay_load_save(srcIdx, /*startGame*/FALSE);
    sSavegameIdx = dstIdx;
    // Save twice to make sure both alternate slots are for the new save
    gplay_save_game();
    gplay_save_game();
}

void gplay_save_game(void) {
    if (sSavegameIdx != -1) {
        sState.save.file.numTimesSaved += 1;

        // Always stage & save data in the Savefile struct
        bcopy(&sState.save.file, &sSavegame->asSave.file, sizeof(Savefile));

        // Alternate save location in flash every time the gamesave is saved
        //
        // In flash, there are 8 gamesave slots but only 4 can be distinct gamesaves
        // as each gamesave alternates between two slots (either idx + 0 or idx + 4).
        //
        // | 0 | savegame 0 alternate 0 |
        // | 1 | savegame 1 alternate 0 |
        // | 2 | savegame 2 alternate 0 |
        // | 3 | savegame 3 alternate 0 |
        // | 4 | savegame 0 alternate 1 |
        // | 5 | savegame 1 alternate 1 |
        // | 6 | savegame 2 alternate 1 |
        // | 7 | savegame 3 alternate 1 |
        gDLL_31_Flash->vtbl->save_game(
            &sSavegame->asFlash, 
            sSavegameIdx + (sSavegame->asSave.file.numTimesSaved % 2) * MAX_SAVEGAMES, 
            sizeof(FlashStruct), 
            TRUE);
        
        if (sRestartSave != NULL) {
            bcopy(&sState.save.chkpnt, &sRestartSave->chkpnt, sizeof(CheckpointSaveData));
        }
    }
}

u32 gplay_load_game_options(void) {
    u32 ret;
    s32 loadStatus;
    
    ret = 1;

    loadStatus = gDLL_31_Flash->vtbl->load_game(
        sGameOptions, 3, sizeof(GplayOptions), FALSE);
    
    if (!loadStatus) {
        // Failed to load
        bzero(sGameOptions, sizeof(GplayOptions));
        ret = 0;
        sGameOptions->volumeMusic = 0x7f;
        sGameOptions->volumeAudio = 0x7f;
        sGameOptions->unkA = 0x7f;
    }

    sGameOptions->languageID = 0;
    
    if (sGameOptions->screenOffsetX < -7) {
        sGameOptions->screenOffsetX = -7;
    }
    if (sGameOptions->screenOffsetX > 7) {
        sGameOptions->screenOffsetX = 7;
    }
    if (sGameOptions->screenOffsetY < -7) {
        sGameOptions->screenOffsetY = -7;
    }
    if (sGameOptions->screenOffsetY > 7) {
        sGameOptions->screenOffsetY = 7;
    }

    return ret;
}

void gplay_save_game_options(void) {
    gDLL_31_Flash->vtbl->save_game(sGameOptions, 3, sizeof(GplayOptions), FALSE);
}

GplayOptions *gplay_get_game_options(void) {
    return sGameOptions;
}

void gplay_func_94C(s32 param1) {

}

void gplay_checkpoint(Vec3f *position, s16 yaw, s32 param3, s32 mapLayer) {
    if ((param3 & 1) != 0) {
        bcopy(&sState.save.chkpnt, &sSavegame->asSave.chkpnt, sizeof(CheckpointSaveData));
    } else {
        if (func_8001EBE0() != 0) {
            sState.save.map.unk16F4[sState.save.file.playerno].unk10 |= 1;
        } else {
            sState.save.map.unk16F4[sState.save.file.playerno].unk10 &= ~1;
        }

        sState.save.map.playerLocations[sState.save.file.playerno].vec.x = position->x;
        sState.save.map.playerLocations[sState.save.file.playerno].vec.y = position->y;
        sState.save.map.playerLocations[sState.save.file.playerno].vec.z = position->z;
        sState.save.map.playerLocations[sState.save.file.playerno].rotationY = yaw >> 8;
        sState.save.map.playerLocations[sState.save.file.playerno].mapLayer = mapLayer;

        // Copy save state to active savegame
        //
        // Does not persist savegame to flash until the player actually selects the save button,
        // this just simply stages the data.
        bcopy(&sState.save, &sSavegame->asSave, sizeof(Savegame));
    }

    if (sRestartSave != NULL) {
        bcopy(&sState.save.chkpnt, &sRestartSave->chkpnt, sizeof(CheckpointSaveData));
    }
}

void gplay_start_loaded_game(void) {
    bcopy(&sSavegame->asSave, &sState.save, sizeof(Savegame));
    gplay_start_game();
}

void gplay_restart_set(Vec3f *position, s16 yaw, s32 mapLayer) {
    if (sRestartSave == NULL) {
        sRestartSave = (Savegame*)mmAlloc(sizeof(Savegame), COLOUR_TAG_YELLOW, NULL);
        if (sRestartSave == NULL) {
            return;
        }
    }

    bcopy(&sState.save, sRestartSave, sizeof(Savegame));

    if (func_8001EBE0() != 0) {
        sRestartSave->map.unk16F4[sRestartSave->file.playerno].unk10 |= 1;
    } else {
        sRestartSave->map.unk16F4[sRestartSave->file.playerno].unk10 &= ~1;
    }

    sRestartSave->map.playerLocations[sRestartSave->file.playerno].vec.x = position->x;
    sRestartSave->map.playerLocations[sRestartSave->file.playerno].vec.y = position->y;
    sRestartSave->map.playerLocations[sRestartSave->file.playerno].vec.z = position->z;
    sRestartSave->map.playerLocations[sRestartSave->file.playerno].rotationY = (u8)(yaw >> 8);
    sRestartSave->map.playerLocations[sState.save.file.playerno].mapLayer = mapLayer;
}

void gplay_restart_goto(void) {
    if (sRestartSave != NULL) {
        bcopy(sRestartSave, &sState.save, sizeof(Savegame));
        gplay_start_game();
    }
}

void gplay_restart_clear(void) {
    if (sRestartSave != NULL) {
        mmFree(sRestartSave);
        sRestartSave = NULL;
    }
}

s32 gplay_restart_is_set(void) {
    return sRestartSave != NULL;
}

static void gplay_start_game(void) {
    sMapSetup.mapID = -1;
    sMapObjGroups.mapID = -1;

    bzero(&sState.bitString, sizeof(sState.bitString));

    unpause();

    func_800142A0(
        sState.save.map.playerLocations[sState.save.file.playerno].vec.x,
        sState.save.map.playerLocations[sState.save.file.playerno].vec.y,
        sState.save.map.playerLocations[sState.save.file.playerno].vec.z);
    
    if (menu_get_current() != MENU_TITLE_SCREEN) {
        menu_set(MENU_GAMEPLAY);
    }

    gDLL_28_ScreenFade->vtbl->fade_reversed(40, SCREEN_FADE_BLACK);
}

GameState *gplay_get_state(void) {
    return &sState;
}

u8 gplay_get_playerno(void) {
    return sState.save.file.playerno;
}

void gplay_set_playerno(u8 playerno) {
    sState.save.file.playerno = playerno;
}

PlayerStats *gplay_get_player_stats(void) {
    return &sState.save.file.players[sState.save.file.playerno];
}

PlayerLocation *gplay_get_player_saved_location(void) {
    return &sState.save.map.playerLocations[sState.save.file.playerno];
}

GplayStruct11 *gplay_func_F30(void) {
    return &sState.save.file.unk18[sState.save.file.playerno];
}

// gplayGetCurrentPlayerLactions ?
GplayStruct6 *gplay_func_F60(void) {
    if (sState.save.file.playerno >= PLAYER_NUM_PLAYERS) {
        return &sState.save.map.unk16F4[0];
    }

    return &sState.save.map.unk16F4[sState.save.file.playerno];
}

// gplayGetCurrentPlayerEnvactions ?
GplayStruct12 *gplay_func_FA8(void) {
    if (sState.save.file.playerno >= PLAYER_NUM_PLAYERS) {
        return &sState.save.map.unk171C[0];
    }

    return &sState.save.map.unk171C[sState.save.file.playerno];
}

GplayStruct13 *gplay_func_FE8(void) {
    return &sState.save.map.unk179C[sState.save.file.playerno];
}

void gplay_add_time(s32 uid, f32 time) {
    s32 i;

    if (sState.save.file.timeSaveCount == MAX_TIMESAVES) {
        return;
    }

    time *= 20.0f;
    time += sState.save.file.timePlayed;

    for (i = 0; i < sState.save.file.timeSaveCount; i++) {
        if (uid == sState.save.file.timeSaves[i].uid) {
            // Found existing time save for the given UID
            break;
        }
    }

    if (i == sState.save.file.timeSaveCount) {
        // Time save doesn't exist yet for the given UID, add one
        sState.save.file.timeSaveCount += 1;
    }

    // Set time save expiry
    sState.save.file.timeSaves[i].uid = uid;
    sState.save.file.timeSaves[i].time = time;      
}

s32 gplay_did_time_expire(s32 uid) {
    s32 i;

    for (i = 0; i < sState.save.file.timeSaveCount; i++) {
        if (uid == sState.save.file.timeSaves[i].uid) {
            return FALSE;
        }
    }

    return TRUE;
}

f32 gplay_get_time_remaining(s32 uid) {
    s32 i;

    for (i = 0; i < sState.save.file.timeSaveCount; i++) {
        if (uid == sState.save.file.timeSaves[i].uid) {
            return sState.save.file.timeSaves[i].time - sState.save.file.timePlayed;
        }
    }

    return 0.0f;
}

void gplay_tick(void) {
    s32 i;

    // Increment time played
    sState.save.file.timePlayed += gUpdateRateF;

    // Process time saves
    i = 0;
    while (i < sState.save.file.timeSaveCount) {
        if (sState.save.file.timeSaves[i].time < sState.save.file.timePlayed) {
            // Expire time save
            sState.save.file.timeSaveCount = sState.save.file.timeSaveCount - 1;

            sState.save.file.timeSaves[i].uid = sState.save.file.timeSaves[sState.save.file.timeSaveCount].uid;
            sState.save.file.timeSaves[i].time = sState.save.file.timeSaves[sState.save.file.timeSaveCount].time;
        } else {
            i++;
        }
    }
}

s16 gplay_get_num_saved_objects(void) {
    return sState.save.file.numSavedObjects;
}

void gplay_set_num_saved_objects(s32 num) {
    sState.save.file.numSavedObjects = num;
}

SavedObject *gplay_get_saved_objects(void) {
    return sState.save.file.savedObjects;
}

u32 gplay_get_time_played(void) {
    return (u32)sState.save.file.timePlayed;
}

static void gplay_reset_state(void) {
    sSavegameIdx = -1;
    bzero(&sState, sizeof(GameState));
    bzero(sSavegame, sizeof(FlashStruct));
}

void gplay_func_1378(s32 param1, s32 param2) {
    bss_1840[param2 - 80] = param1;
}

void gplay_set_map_setup(s32 mapID, s32 setupID) {
    if (mapID >= 80) {
        mapID = bss_1840[mapID - 80];
    }

    main_set_bits(sMapSetupBitKeys[mapID], setupID);

    sMapSetup.mapID = mapID;
    sMapSetup.setupID = setupID;

    gplay_world_load_obj_group_bits(mapID);
}

u8 gplay_get_map_setup(s32 mapID) {
    if (mapID >= 80) {
        mapID = bss_1840[mapID - 80];
    }

    if (mapID != sMapSetup.mapID) {
        sMapSetup.mapID = mapID;

        if (mapID < 0 || mapID >= 120 || !sMapSetupBitKeys[mapID]) {
            sMapSetup.setupID = 0;
        } else {
            sMapSetup.setupID = main_get_bits(sMapSetupBitKeys[mapID]);
        }
    }

    return sMapSetup.setupID;
}

u8 gplay_get_obj_group_status(s32 mapID, s32 group) {
    if (mapID >= 80) {
        mapID = bss_1840[mapID - 80];
    }

    if (mapID != sMapObjGroups.mapID) {
        sMapObjGroups.mapID = mapID;
        sMapObjGroups.groupBits = main_get_bits(sMapObjGroupBitKeys[mapID]);
    }

    return (sMapObjGroups.groupBits >> group) & 1;
}

u16 gplay_get_obj_group_bit_key(s32 mapID) {
    return sMapObjGroupBitKeys[mapID];
}

void gplay_world_load_obj_group_bits(s32 mapID) {
    if (mapID >= 80) {
        mapID = bss_1840[mapID - 80];
    }

    sAllMapObjGroups[mapID] = main_get_bits(sMapObjGroupBitKeys[mapID]);
}

u32 gplay_world_get_obj_group_bits(s32 mapID) {
    if (mapID >= 80) {
        mapID = bss_1840[mapID - 80];
    }
    
    return sAllMapObjGroups[mapID];
}

void gplay_world_disable_all_obj_groups(s32 mapID) {
    if (mapID >= 80) {
        mapID = bss_1840[mapID - 80];
    }
    
    // Disable all obj groups for map
    sAllMapObjGroups[mapID] = 0;
}

void gplay_set_obj_group_status(s32 mapID, s32 group, s32 status) {
    s32 newGroupBits;
    s32 groupBits;
    s32 i;

    if (mapID >= 80) {
        mapID = bss_1840[mapID - 80];
    }

    if ((mapID < 120 && sMapObjGroupBitKeys[mapID] != 0) || (status >= 0 && 0)) {
        if (status == -1) {
            status = 1;
        }

        if (status == -2) {
            status = 0;
        }

        groupBits = main_get_bits(sMapObjGroupBitKeys[mapID]);

        newGroupBits = groupBits;
        if (status != 0) {
            newGroupBits |= (1 << group);
        } else {
            newGroupBits &= ~(1 << group);
        }

        main_set_bits(sMapObjGroupBitKeys[mapID], newGroupBits);

        sMapObjGroups.mapID = mapID;
        sMapObjGroups.groupBits = newGroupBits;

        if (status != 0) {
            if (((1 << group) & groupBits) == 0) {
                for (i = 0; i < 120; i++) {
                    if (sMapObjGroupBitKeys[i] == sMapObjGroupBitKeys[mapID]) {
                        sAllMapObjGroups[i] = sAllMapObjGroups[i] | (1 << group);
                    }
                }
            }
        } else {
            for (i = 0; i < 120; i++) {
                if (sMapObjGroupBitKeys[i] == sMapObjGroupBitKeys[mapID]) {
                    sAllMapObjGroups[i] = sAllMapObjGroups[i] & ~(1 << group);
                }
            }
        }
    }
}

GplayStruct14 *gplay_func_1974(void) {
    return &sState.save.file.unk20[sState.save.file.playerno];
}

GplayStruct14 *gplay_func_19B8(void) {
    return &sState.save.file.unk188[sState.save.file.playerno];
}

u32 gplay_is_cheat_unlocked(u8 cheatIdx) {
    if (cheatIdx >= 32) {
        return 0;
    }

    return sGameOptions->cheatsUnlocked & (1 << cheatIdx);
}

void gplay_unlock_cheat(u8 cheatIdx) {
    if (cheatIdx < 32) {
        sGameOptions->cheatsUnlocked |= (1 << cheatIdx);
    }
}

s32 gplay_is_cheat_active(u8 cheatIdx) {
    s32 ret;

    ret = FALSE;

    if (cheatIdx >= 32) {
        return FALSE;
    } 

    ret = (sGameOptions->cheatsUnlocked & (1 << cheatIdx)) != 0;
    if (ret) {
        return (sGameOptions->cheatsEnabled & (1 << cheatIdx)) != 0;
    }

    // hmm...
}

void gplay_set_cheat_enabled(u8 cheatIdx, u8 enabled) {
    if (cheatIdx < 32 && (sGameOptions->cheatsUnlocked & (1 << cheatIdx)) != 0) {
        if (enabled) {
            sGameOptions->cheatsEnabled |= (1 << cheatIdx);
        } else {
            sGameOptions->cheatsEnabled &= ~(1 << cheatIdx);
        }
    }
}

u32 gplay_is_cinema_unlocked(u8 cinemaIdx) {
    if (cinemaIdx >= 32) {
        return 0;
    }

    return sGameOptions->cinemasUnlocked & (1 << cinemaIdx);
}

void gplay_unlock_cinema(u8 cinemaIdx) {
    if (cinemaIdx < 32) {
        sGameOptions->cinemasUnlocked |= (1 << cinemaIdx);
    }
}
