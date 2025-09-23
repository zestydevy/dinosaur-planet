#include "PR/ultratypes.h"

#include "dlls/engine/28_screen_fade.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/engine/31_flash.h"
#include "game/objects/object.h"
#include "sys/asset_thread.h"
#include "sys/dll.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/fs.h"
#include "sys/menu.h"
#include "sys/memory.h"
#include "variables.h"
#include "functions.h"
#include "dll.h"

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
    0x0000, 0x0000, 0x076e, 0x08ec, 0x04fe, 0x00df, 0x00e0, 0x00e1, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7, 0x00e8, 
    0x00e9, 0x00ea, 0x00eb, 0x0492, 0x0000, 0x05d0, 0x0000, 0x00ed, 0x00ed, 0x00ed, 0x00f0, 0x0000, 0x0229, 0x00ee, 0x0000, 0x00ef, 
    0x0000, 0x0000, 0x0000, 0x03ee, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0349, 0x0000, 0x0492, 0x0492, 
    0x0547, 0x0000, 0x05d0, 0x0000, 0x076f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
// Map of map ID -> bit table key
static u16 sMapObjGroupBitKeys[120] = {
    0x03e0, 0x03e0, 0x05db, 0x08ed, 0x0500, 0x07ce, 0x0480, 0x0452, 0x0452, 0x047b, 0x04ae, 0x0405, 0x0458, 0x036a, 0x04a6, 0x045a, 
    0x047c, 0x0000, 0x042e, 0x0493, 0x0000, 0x05d1, 0x0000, 0x03ad, 0x03ad, 0x03ad, 0x0517, 0x0373, 0x0443, 0x03b7, 0x0421, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0397, 0x0000, 0x0000, 0x0000, 0x0473, 0x0000, 0x0000, 0x0000, 0x04a3, 0x0000, 0x0000, 0x0493, 0x0493, 
    0x0548, 0x0000, 0x05d1, 0x0601, 0x05dc, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x03e0, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
static Vec3f data_1E0 = { -14149.352f, -82.0f, -15569.178f };

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
void gplay_checkpoint(Vec3f *param1, s16 param2, s32 param3, s32 param4);
void gplay_start_loaded_game(void);
static void gplay_start_game(void);
static void gplay_reset_state(void);
void gplay_set_map_setup(s32 param1, s32 param2);
void gplay_world_load_obj_group_bits(s32 param1);

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
    sState.save.unk0.file.isEmpty = 1;
    sState.save.unk0.file.timePlayed = -1.0f;
    // Save twice to erase both copies
    gplay_save_game();
    gplay_save_game();
}

// regalloc
// https://decomp.me/scratch/74sZJ
#ifndef NON_MATCHING
void gplay_init_save(s8 idx, char *filename);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/29_gplay/gplay_init_save.s")
#else
void gplay_init_save(s8 idx, char *filename) {
    GplayStruct6 *struct6;
    GplayStruct12 *struct12;
    GplayStruct13 *struct13;
    GplayStruct11 *struct11;
    Vec3f vec;
    s32 i;
    char *dst;
    char var;

    vec = data_1E0;

    gplay_reset_state();

    sSavegameIdx = idx;
    sState.save.unk0.file.playerno = PLAYER_KRYSTAL;

    for (i = 0; i < 2; i++) {

        sState.save.unk0.file.players[i].health = 12;
        sState.save.unk0.file.players[i].healthMax = 12;
        sState.save.unk0.file.players[i].magicMax = 25;
        sState.save.unk0.file.players[i].magic = 0;

        sState.save.unk0.file.unk0x18[i].unk0x0 = 4;
        sState.save.unk0.file.unk0x18[i].unk0x1 = 5;

        struct13 = &sState.save.unk0x179c[i]; // v1
        struct6 = &sState.save.unk0x16F4[i]; // a1
        struct6->unk0x0 = -1;
        struct6->unk0x2 = -1;
        struct6->unk0x4 = -1;
        struct6->unk0x6 = -1;
        struct6->unk0x8 = -1;
        struct6->unk0xa = -1;
        struct6->unk0xc = -1;
        struct6->unk0xe = -1;
        struct6->unk0x12 = -1;

        struct12 = &sState.save.unk0x171C[i]; // a0
        struct12->unk0x0 = 43000.0f;
        struct12->unk0x4 = -1;
        struct12->unk0x6 = -1;
        struct12->unk0x8 = -1;
        struct12->unk0xa = -1;
        struct12->unk0xc = -1;
        struct12->unk0xe = -1;
        struct12->unk0x3d = -1;
        struct12->unk0x3e = -1;
        struct12->unk0x3f = -1;
        struct12->unk0x3c = 1;

        struct13->unk0x0[3] = -1;
        struct13->unk0x0[2] = -1;
        struct13->unk0x0[1] = -1;
        struct13->unk0x0[0] = -1;
    }

    for (i = 0; i < 120; i++) {
        if (sMapSetupBitKeys[i] != 0) {
            gDLL_29_Gplay->vtbl->set_map_setup(i, 1);
        }
    }

    set_gplay_bitstring(0x4E5, 1);

    if (filename != NULL) {
        dst = &sState.save.unk0.file.name[0];

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

    gplay_checkpoint(&vec, 0, 0, 0);
    // Save twice to init both copies
    gplay_save_game();
    gplay_save_game();
}
#endif

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
            } else if (copy1Ptr->asSave.unk0.file.timePlayed < sSavegame->asSave.unk0.file.timePlayed) {
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
        sState.save.unk0.file.numTimesSaved += 1;

        bcopy(&sState.save.unk0.file, &sSavegame->asSave.unk0.file, sizeof(Savefile));

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
            sSavegameIdx + (sSavegame->asSave.unk0.file.numTimesSaved % 2) * MAX_SAVEGAMES, 
            sizeof(FlashStruct), 
            TRUE);
        
        if (sRestartSave != NULL) {
            bcopy(&sState.save.unk0, &sRestartSave->unk0, sizeof(GplayStruct9));
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
        bcopy(&sState.save.unk0, &sSavegame->asSave.unk0, sizeof(GplayStruct9));
    } else {
        if (func_8001EBE0() != 0) {
            sState.save.unk0x16F4[sState.save.unk0.file.playerno].unk0x10 |= 1;
        } else {
            sState.save.unk0x16F4[sState.save.unk0.file.playerno].unk0x10 &= ~1;
        }

        sState.save.playerLocations[sState.save.unk0.file.playerno].vec.x = position->x;
        sState.save.playerLocations[sState.save.unk0.file.playerno].vec.y = position->y;
        sState.save.playerLocations[sState.save.unk0.file.playerno].vec.z = position->z;
        sState.save.playerLocations[sState.save.unk0.file.playerno].rotationY = yaw >> 8;
        sState.save.playerLocations[sState.save.unk0.file.playerno].mapLayer = mapLayer;

        // Copy save state to active savegame
        //
        // Does not persist savegame to flash until the player actually selects the save button,
        // this just simply stages the data.
        bcopy(&sState.save, &sSavegame->asSave, sizeof(Savegame));
    }

    if (sRestartSave != NULL) {
        bcopy(&sState.save.unk0, &sRestartSave->unk0, sizeof(GplayStruct9));
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
        sRestartSave->unk0x16F4[sRestartSave->unk0.file.playerno].unk0x10 |= 1;
    } else {
        sRestartSave->unk0x16F4[sRestartSave->unk0.file.playerno].unk0x10 &= ~1;
    }

    sRestartSave->playerLocations[sRestartSave->unk0.file.playerno].vec.x = position->x;
    sRestartSave->playerLocations[sRestartSave->unk0.file.playerno].vec.y = position->y;
    sRestartSave->playerLocations[sRestartSave->unk0.file.playerno].vec.z = position->z;
    sRestartSave->playerLocations[sRestartSave->unk0.file.playerno].rotationY = (u8)(yaw >> 8);
    sRestartSave->playerLocations[sState.save.unk0.file.playerno].mapLayer = mapLayer;
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
        sState.save.playerLocations[sState.save.unk0.file.playerno].vec.x,
        sState.save.playerLocations[sState.save.unk0.file.playerno].vec.y,
        sState.save.playerLocations[sState.save.unk0.file.playerno].vec.z);
    
    if (menu_get_current() != MENU_TITLE_SCREEN) {
        menu_set(MENU_GAMEPLAY);
    }

    gDLL_28_ScreenFade->vtbl->fade_reversed(40, SCREEN_FADE_BLACK);
}

GameState *gplay_get_state(void) {
    return &sState;
}

u8 gplay_get_playerno(void) {
    return sState.save.unk0.file.playerno;
}

void gplay_set_playerno(u8 playerno) {
    sState.save.unk0.file.playerno = playerno;
}

PlayerStats *gplay_get_player_stats(void) {
    return &sState.save.unk0.file.players[sState.save.unk0.file.playerno];
}

PlayerLocation *gplay_get_player_saved_location(void) {
    return &sState.save.playerLocations[sState.save.unk0.file.playerno];
}

GplayStruct11 *gplay_func_F30(void) {
    return &sState.save.unk0.file.unk0x18[sState.save.unk0.file.playerno];
}

// gplayGetCurrentPlayerLactions ?
GplayStruct6 *gplay_func_F60(void) {
    if (sState.save.unk0.file.playerno >= PLAYER_NUM_PLAYERS) {
        return &sState.save.unk0x16F4[0];
    }

    return &sState.save.unk0x16F4[sState.save.unk0.file.playerno];
}

// gplayGetCurrentPlayerEnvactions ?
GplayStruct12 *gplay_func_FA8(void) {
    if (sState.save.unk0.file.playerno >= PLAYER_NUM_PLAYERS) {
        return &sState.save.unk0x171C[0];
    }

    return &sState.save.unk0x171C[sState.save.unk0.file.playerno];
}

GplayStruct13 *gplay_func_FE8(void) {
    return &sState.save.unk0x179c[sState.save.unk0.file.playerno];
}

void gplay_add_time(s32 uid, f32 time) {
    s32 i;

    if (sState.save.unk0.file.timeSaveCount == MAX_TIMESAVES) {
        return;
    }

    time *= 20.0f;
    time += sState.save.unk0.file.timePlayed;

    for (i = 0; i < sState.save.unk0.file.timeSaveCount; i++) {
        if (uid == sState.save.unk0.file.timeSaves[i].uid) {
            // Found existing time save for the given UID
            break;
        }
    }

    if (i == sState.save.unk0.file.timeSaveCount) {
        // Time save doesn't exist yet for the given UID, add one
        sState.save.unk0.file.timeSaveCount += 1;
    }

    // Set time save expiry
    sState.save.unk0.file.timeSaves[i].uid = uid;
    sState.save.unk0.file.timeSaves[i].time = time;      
}

s32 gplay_did_time_expire(s32 uid) {
    s32 i;

    for (i = 0; i < sState.save.unk0.file.timeSaveCount; i++) {
        if (uid == sState.save.unk0.file.timeSaves[i].uid) {
            return FALSE;
        }
    }

    return TRUE;
}

f32 gplay_get_time_remaining(s32 uid) {
    s32 i;

    for (i = 0; i < sState.save.unk0.file.timeSaveCount; i++) {
        if (uid == sState.save.unk0.file.timeSaves[i].uid) {
            return sState.save.unk0.file.timeSaves[i].time - sState.save.unk0.file.timePlayed;
        }
    }

    return 0.0f;
}

void gplay_tick(void) {
    s32 i;

    // Increment time played
    sState.save.unk0.file.timePlayed += delayFloat;

    // Process time saves
    i = 0;
    while (i < sState.save.unk0.file.timeSaveCount) {
        if (sState.save.unk0.file.timeSaves[i].time < sState.save.unk0.file.timePlayed) {
            // Expire time save
            sState.save.unk0.file.timeSaveCount = sState.save.unk0.file.timeSaveCount - 1;

            sState.save.unk0.file.timeSaves[i].uid = sState.save.unk0.file.timeSaves[sState.save.unk0.file.timeSaveCount].uid;
            sState.save.unk0.file.timeSaves[i].time = sState.save.unk0.file.timeSaves[sState.save.unk0.file.timeSaveCount].time;
        } else {
            i++;
        }
    }
}

s16 gplay_get_num_saved_objects(void) {
    return sState.save.unk0.file.numSavedObjects;
}

void gplay_set_num_saved_objects(s32 num) {
    sState.save.unk0.file.numSavedObjects = num;
}

SavedObject *gplay_get_saved_objects(void) {
    return sState.save.unk0.file.savedObjects;
}

u32 gplay_get_time_played(void) {
    return (u32)sState.save.unk0.file.timePlayed;
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

    set_gplay_bitstring(sMapSetupBitKeys[mapID], setupID);

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
            sMapSetup.setupID = get_gplay_bitstring(sMapSetupBitKeys[mapID]);
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
        sMapObjGroups.groupBits = get_gplay_bitstring(sMapObjGroupBitKeys[mapID]);
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

    sAllMapObjGroups[mapID] = get_gplay_bitstring(sMapObjGroupBitKeys[mapID]);
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

        groupBits = get_gplay_bitstring(sMapObjGroupBitKeys[mapID]);

        newGroupBits = groupBits;
        if (status != 0) {
            newGroupBits |= (1 << group);
        } else {
            newGroupBits &= ~(1 << group);
        }

        set_gplay_bitstring(sMapObjGroupBitKeys[mapID], newGroupBits);

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
    return &sState.save.unk0.file.unk0x20[sState.save.unk0.file.playerno];
}

GplayStruct14 *gplay_func_19B8(void) {
    return &sState.save.unk0.file.unk0x188[sState.save.unk0.file.playerno];
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