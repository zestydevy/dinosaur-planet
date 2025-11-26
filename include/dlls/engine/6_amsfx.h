#ifndef _DLLS_6_H
#define _DLLS_6_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct UnkDE8 {
    u16 unk0;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u16 pad8;
    u16 unkA;
    u16 unkC;
} UnkDE8;

enum SoundID {
    SOUND_15_Heavy_Stone_Moving = 0x15, // used by door to Randorn's hall

    SOUND_73 = 0x73,

    SOUND_8F = 0x8F, // used by DFlog

    SOUND_9B = 0x9B,

    SFX_RECAP_MENU_BACK = 0x5D4,
    SFX_RECAP_MENU_MOVE = 0x5D5,
    SFX_RECAP_MENU_SELECT = 0x5D6,

    SOUND_10A = 0x10A,

    SFX_129_SnowHorn_Yawn_1 = 0x129,
    SFX_12A_SnowHorn_SnoreHorn = 0x12A,
    SFX_12B_SnowHorn_Yawn_2 = 0x12B,

    SOUND_140 = 0x140,

    SOUND_177 = 0x177,

    SOUND_179 = 0x179,
    SOUND_17A = 0x17A,
    SOUND_17B = 0x17B,

    SOUND_1e1_Stone_Moving = 0x1e1,

    SOUND_1E2_Garunda_Te_Will_somebody_get_me_out_of_here = 0x1E2,
    SOUND_1E3_SharpClaw_Ah_Shuddup = 0x1E3, // used in DLL 33

    SOUND_1EB_Wooden_Partition_Sliding = 0x1EB, // used when WM transporter room door moves

    SOUND_222_NW_Tricky_Sharpclaw_Help = 0x222,

    SOUND_242_Failure_Glissando = 0x242,

    SOUND_25B_Magic_Attack_Deflected = 0x25B,
    SOUND_25C_Melee_Attack_Deflected = 0x25C,

    SOUND_25F_SB_CloudRunner_battle_tips_1 = 0x25F,
    SOUND_260_SB_CloudRunner_battle_tips_2 = 0x260,
    SOUND_261_SB_CloudRunner_battle_tips_3 = 0x261,

    SOUND_277 = 0x277,

    SOUND_27A = 0x27A,
    SOUND_27B = 0x27B,

    SOUND_284_SB_Kyte_Heeelp = 0x284,
    SOUND_285_SB_Kyte_Find_the_wizard_and_get_me_out_of_here = 0x285,

    SOUND_287_SB_Kyte_Empty = 0x287,

    SOUND_289 = 0x289,
    
    SOUND_28E = 0x28E,
    SOUND_28F = 0x28F,
    SOUND_290 = 0x290,

    SOUND_292 = 0x292, // used by IMSnowBike

    SOUND_2F8_Door_Opening = 0x2f8, // used by doors in Krazoa Shrines
    SOUND_2F9_Door_Closing = 0x2f9, // used by doors in Krazoa Shrines

    SOUND_33E = 0x33E,

    SOUND_342 = 0x342, // used in DLL 466

    SOUND_344 = 0x344,

    SOUND_36E_Lever_Clunk = 0x36e, // used by many levers (e.g. ones leading to Cape Claw courtyard)

    SOUND_371 = 0x371, // used by MediumMetalCrat
    SOUND_372 = 0x372, // used by MediumMetalCrat

    SOUND_377_Metal_Smack = 0x377,

    SOUND_380 = 0x380,
    SOUND_386 = 0x386,

    SOUND_3D8_Water_Splash = 0x3D8, // used in pushpull (DLL 274)

    SOUND_43D = 0x43D,

    SOUND_WM_Sun_Whoosh = 0x487, //used by WMsun

    SOUND_4BC = 0x4BC,

    SOUND_43E_Stone_Block_Moving = 0x43e,

    SOUND_LightFoot_Shout = 0x4B7,

    SOUND_50a_Fire_Burning_Low_Loop = 0x50a,
    SOUND_50b_Fire_Burning_High_Loop = 0x50b,

    SOUND_50C = 0x50C,
    SOUND_50D = 0x50D,

    SOUND_5EB_Magic_Refill_Chime = 0x5EB, // used in DLL 210

    SFX_PAUSE_MENU_BACK = 0x5D4, // Used in DLL 66
    SFX_PAUSE_MENU_MOVE = 0x5D5, // Used in DLL 66

    SOUND_5F6_Tumbleweed_Roll = 0x5F6, // used in DLL 227
    SOUND_5F7_Tumbleweed_Disintegrate = 0x5F7, // used in DLL 227

    SOUND_614_Tumbleweed_Squeak_1 = 0x614, // used in DLL 227
    SOUND_615_Tumbleweed_Squeak_2 = 0x615, // used in DLL 227

    SOUND_64C = 0x64C, // used by MediumCrate
    SOUND_64D = 0x64D, // used by MediumCrate

    SOUND_677_Metal_Clang = 0x677, // snowhorn related

    SOUND_688_KT_Rex_Stomp = 0x688,
    SOUND_689_KT_Rex_Stomp = 0x689,
    SOUND_68A_KT_Rex_Stomp = 0x68A,
    
    SOUND_68B_KT_Rex_Breathing1 = 0x68B,
    SOUND_68C_KT_Rex_Breathing2 = 0x68C,
    SOUND_68D_KT_Rex_Roar_Kinda = 0x68D,
    SOUND_68E_KT_Rex_Noise = 0x68E, // when he notices sabre?
    SOUND_68F_KT_Rex_Roar = 0x68F,
    SOUND_690_KT_Rex_Roar = 0x690,
    SOUND_691_KT_Rex_Roar = 0x691,

    SOUND_693_Explosion = 0x693, // used by KT_Rex
    SOUND_694_KT_Laserwall_PowerUp = 0x694, // used by KT_Lazerwall
    SOUND_695_KT_Laserwall_Zap = 0x695, // used by KT_Lazerwall
    SOUND_696_KT_Laserwall_PowerDown = 0x696, // used by KT_Lazerwall

    SOUND_699_KT_RaisingFloorSwitches = 0x699,

    SOUND_6B5 = 0x6B5, // used by MediumCrate
    SOUND_6B6 = 0x6B6, // used by MediumCrate
    SOUND_6B7 = 0x6B7, // used by MediumCrate

    SOUND_6E2 = 0x6E2, // used by KT_RexFloorSwitch

    SOUND_6EC_Mechanical_Hum_Loop = 0x6EC, // used in DLL 641

    SOUND_6F9_Explosion = 0x6F9, // used by KT_Rex
    SOUND_6FA = 0x6FA, // used by KT_Rex
    SOUND_6FB = 0x6FB, // used by KT_Rex
    SOUND_6FC_KT_Rex_Slam = 0x6FC,
    SOUND_6FD_KT_Rex_Roar = 0x6FD,
    SOUND_6FE_KT_Rex_Roar = 0x6FE,

    SOUND_72F_Harsh_Magical_Thrum_Loop = 0x72F, // used in DLL 475 GPSH_flybaddie
    SOUND_730_Electrified_Blast = 0x730, // used in DLL 475 GPSH_flybaddie

    SOUND_74B_Garunda_Te_That_tastes_great_Hurry_up_boy = 0x74B,

    SOUND_76D = 0x76D, // used by DFlog, BWlog

    SOUND_775_Timer_Countdown = 0x775,

    SOUND_798_Puzzle_Solved = 0x798, // louder than B89, used when placing MoonSeeds

    SOUND_80B_Crackling_Freezing = 0x80B,
    SOUND_80C_Steam_Hissing = 0x80C,

    SOUND_8A2 = 0x8A2, // used in DLL 505

    SOUND_912_Object_Refused = 0x912,

    SOUND_986 = 0x986, // used in DLL 6
    SOUND_987 = 0x987, // used in DLL 6

    SOUND_99a_Mechanical_Ratcheting = 0x99a,

    SOUND_9A3 = 0x9A3,

    SOUND_9B1_Flames_Venting = 0x9B1, // kamerian related
    SOUND_9B2_Fire_Burning = 0x9B2, // kamerian related

    SOUND_9B4_Flamethrower = 0x9B4, // kamerian related
    SOUND_9B5_Explosion = 0x9B5, // kamerian related

    SOUND_9B9 = 0x9B9,
    SOUND_9BA = 0x9BA,
    SOUND_9BB = 0x9BB,

    SOUND_9C5 = 0x9C5,
    SOUND_9C6 = 0x9C6,

    SOUND_A71_Rattling_1 = 0xA71, // used by MoonSeeds
    SOUND_A72_Rattling_2 = 0xA72, // used by MoonSeeds

    SOUND_A74 = 0xA74, // used by BWlog
    SOUND_A75 = 0xA75, // used by BWlog
    SOUND_A77 = 0xA77, // used by BWlog

    SOUND_A83 = 0xA83,

    SOUND_AA4 = 0xAA4,
    SOUND_AA5 = 0xAA5,

    SOUND_AAE = 0xAAE, // VFP_lavastar sound

    SOUND_AB4 = 0xAB4,

    SOUND_B1F = 0xB1F,
    SOUND_B20 = 0xB20,
    SOUND_B21 = 0xB21,

    SOUND_B31_Item_Collection_Chime = 0xB31,

    SOUND_B5C_Machinery_Clunk = 0xB5C,

    SOUND_B72_Game_Saved = 0xB72,
    SOUND_B73_Option_Selected = 0xB73,
    SOUND_B74 = 0xB74,
    SOUND_B75 = 0xB75,

    SOUND_B89_Puzzle_Solved = 0xB89, // "puzzle complete" jingle

    SOUND_B95_Gasvent_Uncovered = 0xB95,
    SOUND_B96_Gasvent_Covered = 0xB96,

    SOUND_BA3 = 0xBA3,

    SOUND_B3C = 0xB3C,

    NO_SOUND = -1
};

#define MAX_VOLUME 0x7F

DLL_INTERFACE(DLL_6_AMSFX) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func_338)(void);
/*1*/ void (*func_480)(Object *obj);
// Play sound using source object for panning and distance falloff
/*2*/ u32 (*play_sound)(Object* obj, u16 soundID, u8 volume, u32* soundHandle, char *arg4, s32 arg5, char *arg6);
/*3*/ void (*func_7E4)(u8 arg0);
/*4*/ void (*func_860)(u32 arg0, u8 arg1);
/*5*/ void (*func_954)(u32 arg0, f32 pitch);
/*6*/ void (*func_A1C)(u32 arg0);
/*7*/ void (*func_A6C)(Object *obj);
/*8*/ s32 (*func_B48)(u32 arg0);
/*9*/ s32 (*func_BBC)(Object* arg0, s32 arg1, s32 arg2, Object **arg3, s32 arg4);
/*10*/ s32 (*func_DCC)(s32 arg0, UNK_TYPE_32 arg1, s32* arg2, s32* arg3);
/*11*/ s32 (*func_DE8)(u16 arg0, UnkDE8* arg1);
/*12*/ s32 (*func_EE4)(s32 arg0, UNK_TYPE_32 arg1);
/*13*/ void (*func_F2C)(Object* arg0, u16 arg1, u32* arg2, char *arg3, s32 arg4);
/*14*/ void (*func_10D0)(Object* obj, u16 soundID, u32* arg2);
/*15*/ void (*func_1174)(Object* obj, u16 soundID, u8 volume, u32* arg3);
/*16*/ void (*func_1218)(Object *obj);
/*17*/ void (*func_1504)(s32 arg0, Object* arg1, Object* arg2, f32 arg3);
/*18*/ void (*func_16E0)(void);
/*19*/ void (*water_falls_set_flags)(u8 arg0);
/*20*/ void (*func_1320)(Object* arg0, u16 arg1, f32 arg2, f32 arg3, f32 arg4, u32* arg5);
};

#endif //_DLLS_6_H
