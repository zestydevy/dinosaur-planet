#ifndef _DLLS_29_H
#define _DLLS_29_H

#include "PR/ultratypes.h"
#include "dlls/engine/31_flash.h"
#include "game/objects/object.h"
#include "sys/math.h"
#include "dll_def.h"
#include "unktypes.h"

typedef enum {
    PLAYER_NONE = -1,

    PLAYER_SABRE = 0,
    PLAYER_KRYSTAL = 1,

    PLAYER_NUM_PLAYERS = 2
} PlayerNo;

#define MAX_SAVEGAMES 4
#define MAX_TIMESAVES 256

// size: 0x10
typedef struct {
    /*0x0*/ Vec3f vec;
    /*0xC*/ s8 rotationY;
    /*0xD*/ s8 mapLayer;
    /*0xE*/ u8 unkE[2];
} PlayerLocation;

// size: 0x14
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
} GplayStruct6;

// size: 0xB4
typedef struct {
    f32 lifetime[30]; //lifetimes of each food bag item (in frames)
    u16 foodType[30]; //bitfield representation of foodID
} FoodbagContents; //0xB4

// size: 0x8
typedef struct {
    // Object UID.
    s32 uid;
    // Timestamp of when this time save expires.
    f32 time;
} TimeSave;

// size: 0x3
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 _unk2[1];
} GplayStruct11;

#define MAX_SCARABS 999
#define MAX_MAGIC 100
#define MAX_HEALTH 80

// size: 0xc
typedef struct {
    /*0x0*/ s8 unk0;
    /*0x1*/ s8 health;
    /*0x2*/ s8 healthMax; //only shows up to 104, code caps it at 80 (MAX_HEALTH).
    /*0x3*/ s8 unk3;
    /*0x4*/ s16 magic; //only mods when byte at "state"0x8bb is set.
    /*0x6*/ s16 magicMax; //capped at 100 (MAX_MAGIC).
    /*0x8*/ u16 scarabs; //capped at 999 (MAX_SCARABS).
    /*0xA*/ s8 scarabsLargestPayday; //seems to track the highest-value Scarab collected 
    /*0xB*/ s8 dusters;
} PlayerStats;

typedef struct {
    s32 uID;
    s16 mapID;
    s16 _unk6; // padding?
    f32 x;
    f32 y;
    f32 z;
} SavedObject;

// size: 0x13d4
typedef struct {
    PlayerStats players[2];
    GplayStruct11 unk18[2];
    u8 _unk1E[0x2];
    FoodbagContents unk20[2];
    FoodbagContents unk188[2];
    /*0x2F0*/char name[6]; // name of save
    u8 isEmpty; // whether this savefile is empty
    /*0x2f7*/u8 playerno; // currently active player (sabre or krystal)
    u8 numTimesSaved; // number of times this file has been saved to flash
    f32 timePlayed; // this is game time played, but what unit of measure?
    s16 numSavedObjects;
    s16 timeSaveCount;
    SavedObject savedObjects[100];
    TimeSave timeSaves[MAX_TIMESAVES];
    /*0x12D4*/u8 bitString[256];
} Savefile;

// size: 0x15d4
typedef struct {
    Savefile file;
    /*0x13d4*/u8 bitString[512];
} CheckpointSaveData;

// size: 0x40
typedef struct {
    f32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    /*0x10*/ s32 unk10;
    /*0x14*/ s32 unk14;
    /*0x18*/ s32 unk18;
    /*0x1C*/ s32 unk1C;
    /*0x20*/ s32 unk20;
    /*0x24*/ s32 unk24;
    /*0x28*/ s32 unk28;
    /*0x2C*/ s32 unk2C;
    /*0x30*/ s32 unk30;
    u8 _unk34[8];
    u8 unk3C;
    s8 unk3D;
    s8 unk3E;
    s8 unk3F;
} GplayStruct12;

// size: 0x8
typedef struct {
    s16 unk0[4];
} GplayStruct13;

// size: 0x17ac
typedef struct {
    CheckpointSaveData chkpnt;
    /*0x15d4*/u8 bitString[256];
    PlayerLocation playerLocations[2]; // saved locations of each player
    GplayStruct6 unk16F4[2];
    GplayStruct12 unk171C[2];
    GplayStruct13 unk179C[2];
} MapSaveData;

enum Languages {
    LANGUAGE_ENGLISH = 0,
    LANGUAGE_FRANCAIS = 1,
    LANGUAGE_DEUTSCH = 2,
    LANGUAGE_ESPANOL = 3,
    LANGUAGE_ITALIANO = 4,
    LANGUAGE_JAPANESE = 5 //Unused, but a Gametext string exists for it as an option
};

enum ZTargetModes {
    Z_TARGET_COMBO = 0,
    Z_TARGET_TAP = 1,
    Z_TARGET_HOLD = 2
};

/** Settings for anamorphic letterboxed widescreen (unimplemented, but presumably like in GoldenEye 64/Perfect Dark) */
enum ScreenSizeModes {
    SCREEN_SIZE_FULL = 0,
    SCREEN_SIZE_WIDE = 1,
    SCREEN_SIZE_CINEMA = 2
};

/** Changes the video aspect ratio */
enum ScreenRatioModes {
    SCREEN_RATIO_STANDARD = 0,
    SCREEN_RATIO_WIDESCREEN = 1
};

/** Changes the audio panning (unknown if actually implemented) */
enum AudioModes {
    AUDIO_STEREO = 0,
    AUDIO_SURROUND = 1,
    AUDIO_MONO = 2,
    AUDIO_HEADPHONES = 3
};

// size: 0x80
typedef struct {
/*00*/ u8 languageID;
/*01*/ u8 showSubtitles;
/*02*/ u8 showInstruments; //hides minimap
/*03*/ u8 zTargetMode; //0) combo, 1) tap, 2) hold
/*04*/ u8 screenSizeAnamorphic; //0) full, 1) wide, 2) cinema
/*05*/ u8 screenAspectRatio; //0) standard 4:3, 1) widescreen 16:9
/*06*/ u8 unk6;
/*07*/ u8 audioMode; //0) stereo, 1) surround, 2) mono, 3) headphones
/*08*/ u8 volumeMusic; //max 255
/*09*/ u8 volumeAudio; //max 127 (@bug: doesn't affect MP3s)
/*0A*/ u8 unkA; 
/*0B*/ s8 screenOffsetX;
/*0C*/ s8 screenOffsetY;
/*0D*/ s8 unkD;
/*0E*/ s8 unkE;
/*0F*/ s8 unkF;
/*10*/ u32 cheatsUnlocked; //bitfield
/*14*/ u32 cheatsEnabled; //bitfield
/*18*/ u32 cinemasUnlocked; //bitfield
/*1C*/ u8 _unk1C[0x64];
} GplayOptions;

// size: 0x17ac
typedef union {
    MapSaveData map;
    CheckpointSaveData chkpnt;
    Savefile file;
} Savegame;

// size: 0x182c
typedef struct {
    Savegame save;
    /*0x17ac*/u8 bitString[128]; // not persisted with savegame!
} GameState;

typedef union {
    FlashStruct asFlash;
    // Cannot exceed (sizeof(FlashStruct) - 0x10)
    Savegame asSave;
} GplaySaveFlash;

DLL_INTERFACE(DLL_29_gplay) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ void (*erase_save)(s8 idx);
    /*1*/ void (*init_save)(s8 idx, char *filename);
    /*2*/ s32 (*load_save)(s8 idx, u8 startGame);
    /*3*/ void (*copy_save)(s8 srcIdx, s8 dstIdx);
    /** Used by the Pause Menu to save the game */
    /*4*/ void (*save_game)(void);
    /*5*/ void (*func_94C)(s32 param1);
    /*6*/ void (*checkpoint)(Vec3f *position, s16 yaw, s32 param3, s32 mapLayer);
    /*7*/ void (*start_loaded_game)(void);
    /*8*/ void (*restart_set)(Vec3f *position, s16 yaw, s32 mapLayer);
    /*9*/ void (*restart_goto)(void);
    /*10*/ void (*restart_clear)(void);
    /*11*/ s32 (*restart_is_set)(void);
    /*12*/ void (*save_game_options)(void);
    /*13*/ u32 (*load_game_options)(void);
    /*14*/ GplayOptions *(*get_game_options)(void);
    /*15*/ u8 (*get_map_setup)(s32 mapID);
    /*16*/ void (*set_map_setup)(s32 mapID, s32 setupID);
    /*17*/ void (*func_1378)(s32 param1, s32 param2);
    /*18*/ u8 (*get_obj_group_status)(s32 mapID, s32 group);
    /*19*/ void (*set_obj_group_status)(s32 mapID, s32 group, s32 status);
    /*20*/ u16 (*get_obj_group_bit_key)(s32 mapID);
    /*21*/ void (*world_load_obj_group_bits)(s32 mapID);
    /*22*/ u32 (*world_get_obj_group_bits)(s32 mapID);
    /*23*/ void (*world_disable_all_obj_groups)(s32 mapID);
    /** Add time save for object UID. */
    /*24*/ void (*add_time)(s32 uid, f32 time);
    /** Whether the time save for the object UID expired. */
    /*25*/ s32 (*did_time_expire)(s32 uid);
    /** Get time left on time save for object UID. */
    /*26*/ f32 (*get_time_remaining)(s32 uid);
    /*27*/ void (*tick)(void);
    /*28*/ u8 (*get_playerno)(void);
    /*29*/ void (*set_playerno)(u8 playerno);
    /*30*/ SavedObject *(*get_saved_objects)(void);
    /*31*/ s16 (*get_num_saved_objects)(void);
    /*32*/ void (*set_num_saved_objects)(s32 num);
    /*33*/ GameState *(*get_state)(void);
    /*34*/ PlayerStats *(*get_player_stats)(void);
    /*35*/ PlayerLocation *(*get_player_saved_location)(void);
    /*36*/ GplayStruct11 *(*func_F30)(void);
    /*37*/ GplayStruct6 *(*func_F60)(void);
    /*38*/ GplayStruct12 *(*func_FA8)(void);
    /*39*/ GplayStruct13 *(*func_FE8)(void);
    /*40*/ FoodbagContents *(*func_1974)(void); //get player foodbag struct
    /*41*/ FoodbagContents *(*func_19B8)(void); //get dino foodbag struct
    /*42*/ u32 (*get_time_played)(void);
    /*43*/ u32 (*is_cheat_unlocked)(u8 cheatIdx);
    /*44*/ void (*unlock_cheat)(u8 cheatIdx);
    /*45*/ s32 (*is_cheat_active)(u8 cheatIdx);
    /*46*/ void (*set_cheat_enabled)(u8 cheatIdx, u8 enabled);
    /*47*/ u32 (*is_cinema_unlocked)(u8 cinemaIdx);
    /*48*/ void (*unlock_cinema)(u8 cinemaIdx);
};

#endif //_DLLS_29_H
