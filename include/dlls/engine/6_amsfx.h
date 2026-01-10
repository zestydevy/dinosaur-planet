#ifndef _DLLS_6_H
#define _DLLS_6_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
    u16 bankAndClipID; //bankID (1 bit), clipID (15 bit)
    u8 volume;
    u8 unk3;
    u8 pitch; //100 for base pitch (default: 0 interpretted as 100)
    u8 pan;
    u8 unk6; //reverb strength?
    u8 volumeFalloff; //flags? Enables volume falloff
    u16 pad8;
    u16 falloffNear; //max volume used when within this radius
    u16 falloffFar; //0 volume used when outside this radius (or linear fade in when between near/far radii)
} SoundDef;

enum SoundID {
    SOUND_15_Heavy_Stone_Moving = 0x15, // used by door to Randorn's hall

    SOUND_Sabre_Heel = 0x20,            //"*whistle* Here boy!"
    SOUND_Sabre_Find = 0x21,            //"Seek it out!"
    SOUND_Sabre_Flame = 0x22,           //"Chase!"
    SOUND_Sabre_Distract = 0x23,        //"Distract 'em!"
    SOUND_Sabre_Guard = 0x24,           //"Look after it!"
    SOUND_Sabre_Fetch = 0x25,           //"Fetch!" (play command) 

    SOUND_40 = 0x40,
    SOUND_41 = 0x41,
    SOUND_42 = 0x42,
    SOUND_43 = 0x43,
    SOUND_44 = 0x44,
    SOUND_45 = 0x45,

    SOUND_61 = 0x61,
    SOUND_62 = 0x62,
    SOUND_63 = 0x63,
    SOUND_64 = 0x64,
    SOUND_65 = 0x65,
    SOUND_66 = 0x66,
    SOUND_67 = 0x67,
    SOUND_68 = 0x68,

    SOUND_6D = 0x6D,
    SOUND_6E = 0x6E,
    SOUND_6F = 0x6F,
    SOUND_70 = 0x70,

    SOUND_73_Thunder = 0x73,

    SOUND_86 = 0x86,
    SOUND_87 = 0x87,
    SOUND_88 = 0x88,
    SOUND_89 = 0x89,
    SOUND_8A = 0x8A,
    SOUND_8B = 0x8B,

    SOUND_8E_Magic_Chime = 0x8E,  // used by collectable DLL
    SOUND_8F_Water_Paddle = 0x8F, // used by DFlog

    SOUND_96_Cannon = 0x96, //SB_ShipGun

    SOUND_9B_Transporter_Hiss = 0x9B,

    SOUND_Krystal_Hurt_Agh = 0xD9,      //player hurt sfx
    SOUND_Krystal_Hurt_Ough = 0xDA,     //player hurt sfx

    SOUND_Krystal_Heel = 0xE9,          //"*whistle* Come here!"
    SOUND_Krystal_Find = 0xEA,          //"Seek it out!"
    SOUND_Krystal_Flame = 0xEB,         //"Fight!"
    SOUND_Krystal_Distract = 0xEC,      //"Distract them!"
    SOUND_Krystal_Guard = 0xED,         //"Look after it."
    SOUND_Krystal_Fetch = 0xEE,         //"Go play!"

    SOUND_10A_Galleon_Roar = 0x10A,

    SOUND_129_SnowHorn_Yawn_1 = 0x129,
    SOUND_12A_SnowHorn_SnoreHorn = 0x12A,
    SOUND_12B_SnowHorn_Yawn_2 = 0x12B,

    SOUND_140_Galleon_Propeller_Loop = 0x140,

    SOUND_174_Machinery_Move_A = 0x174, //SB_ShipGun
    SOUND_175_Machinery_Move_B = 0x175, //SB_ShipGun
    SOUND_176_Explosion_A = 0x176, //SB_ShipGun
    SOUND_177_Explosion_B = 0x177, //SB_ShipGun

    SOUND_179_Galleon_Roar = 0x179,
    SOUND_17A_Galleon_Roar = 0x17A,
    SOUND_17B_Galleon_Rumble_Loop = 0x17B,

    SOUND_Sabre_Freezing_A = 0x183,     //"I'm freezing!"
    SOUND_Sabre_Exit_Icy_Water = 0x184, //"That's much better!"

    SOUND_Sabre_Freezing_B = 0x184,     //"That's too cold for me!"

    SOUND_1D2_Roar = 0x1D2, // Deleted genprops object

    SOUND_1e1_Stone_Moving_Loop = 0x1e1,

    SOUND_1E2_Garunda_Te_Will_somebody_get_me_out_of_here = 0x1E2,
    SOUND_1E3_SharpClaw_Ah_Shuddup = 0x1E3, // used in DLL 33

    SOUND_1EB_Wooden_Partition_Sliding = 0x1EB, // used when WM transporter room door moves

    SOUND_222_NW_Tricky_Sharpclaw_Help = 0x222,

    SOUND_226_Fall_Impact = 0x226,

    SOUND_242_Failure_Glissando = 0x242,

    SOUND_25B_Magic_Attack_Deflected = 0x25B,
    SOUND_25C_Melee_Attack_Deflected = 0x25C,

    SOUND_25F_SB_CloudRunner_battle_tips_1 = 0x25F,
    SOUND_260_SB_CloudRunner_battle_tips_2 = 0x260,
    SOUND_261_SB_CloudRunner_battle_tips_3 = 0x261,

    SOUND_277_Galleon_Creaking = 0x277,

    SOUND_27A_Wood_Creak_A = 0x27A,
    SOUND_27B_Wood_Creak_B = 0x27B,
    SOUND_27C = 0x27C,
    SOUND_27D = 0x27D,
    SOUND_27E = 0x27E,
    SOUND_27F = 0x27F,

    SOUND_280 = 0x280,
    SOUND_281 = 0x281,
    SOUND_282 = 0x282,
    SOUND_283 = 0x283,

    SOUND_284_SB_Kyte_Heeelp = 0x284,
    SOUND_285_SB_Kyte_Find_the_wizard_and_get_me_out_of_here = 0x285,
    SOUND_286 = 0x286, //Krazoa text
    SOUND_287_SB_Kyte_Empty = 0x287,

    SOUND_289_Engine_Loop = 0x289,
    
    SOUND_28E = 0x28E,
    SOUND_28F = 0x28F,
    SOUND_290_Propeller_Loop = 0x290,

    SOUND_292 = 0x292, // used by IMSnowBike

    SOUND_2B8_Spell_Fired = 0x2B8, //used by Projectile Spell and Grenade Spell (and by Ice Beam?)

    SOUND_2F8_Door_Opening = 0x2f8, // used by doors in Krazoa Shrines
    SOUND_2F9_Door_Closing = 0x2f9, // used by doors in Krazoa Shrines

    SOUND_304_Burning_Loop = 0x304, // flame blast

    SOUND_33E = 0x33E,

    SOUND_342_Low_Whoosh = 0x342, // used in DLL 466

    SOUND_344_Chime = 0x344,

    SOUND_35A_Low_Whoosh = 0x35A, // WMrock
    SOUND_35B_Rock_Slide = 0x35B, // WMrock

    SOUND_368 = 0x368,
    SOUND_369 = 0x369,
    SOUND_36A = 0x36A,
    SOUND_36B = 0x36B,

    SOUND_36E_Lever_Clunk = 0x36e, // used by many levers (e.g. ones leading to Cape Claw courtyard)

    SOUND_371_Crate_Smash = 0x371,  // used by MediumMetalCrat
    SOUND_372_Crate_Struck = 0x372, // used by MediumMetalCrat

    SOUND_377_Metal_Smack = 0x377,

    SOUND_380_Scorching_Impact = 0x380, //DRbullet

    SOUND_386_Squelched_Impact = 0x386, //DRbullet

    SOUND_3D8_Water_Splash = 0x3D8, // used in pushpull (DLL 274)

    SOUND_3EC_Water_Wade_Slow_A = 0x3EC,
    SOUND_3ED_Water_Wade_Slow_B = 0x3ED,

    SOUND_402 = 0x402,
    SOUND_403 = 0x403,
    SOUND_404 = 0x404,

    SOUND_406 = 0x406,
    SOUND_407 = 0x407,

    SOUND_43D_Transporter_Fire = 0x43D,

    SOUND_WM_Sun_Whoosh = 0x487, //used by WMsun

    SOUND_4BC_Tricky_Dig_EMPTY = 0x4BC, //NWtricky (empty)

    SOUND_43E_Stone_Block_Moving = 0x43e,

    SOUND_LightFoot_Shout = 0x4B7,

    SOUND_506_Chomping_Food = 0x506, //collectable

    SOUND_509_Engine_Boom = 0x509, //IMspacecraft
    SOUND_50a_Fire_Burning_Low_Loop = 0x50a,
    SOUND_50b_Fire_Burning_High_Loop = 0x50b,

    SOUND_50C_Low_Rumble_Loop = 0x50C, //IMSnowBike
    SOUND_50D_Jet_Loop = 0x50D,        //IMSnowBike

    SOUND_5EB_Magic_Refill_Chime = 0x5EB, // used in DLL 210

    SOUND_5EE_Eating_Food = 0x5EE, //used by DLL 314 foodbag

    SOUND_PICMENU_SELECT = 0x5D3,
    SOUND_PICMENU_BACK = 0x5D4, // Used in DLL 66
    SOUND_PICMENU_MOVE = 0x5D5, // Used in DLL 66
    SOUND_RECAP_MENU_SELECT = 0x5D6, // Gong when starting game

    SOUND_5F6_Tumbleweed_Roll = 0x5F6, // used in DLL 227
    SOUND_5F7_Tumbleweed_Disintegrate = 0x5F7, // used in DLL 227

    SOUND_614_Tumbleweed_Squeak_1 = 0x614, // used in DLL 227
    SOUND_615_Tumbleweed_Squeak_2 = 0x615, // used in DLL 227

    SOUND_633 = 0x633, // empty? used by player DLL

    SOUND_64C_Wicker_Rattle = 0x64C, // used by MediumCrate
    SOUND_64D_Wooden_Rattle = 0x64D, // used by MediumCrate

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

    SOUND_6AB_Electric_Arcing_Loop = 0x6AB, //played at tiny volume in player DLL?
    SOUND_6AC_Electric_Zap = 0x6AC,
    SOUND_6AD_Electric_Crackle = 0x6AD,

    SOUND_6B4_Illusion_Spell = 0x6B4, 
    SOUND_6B5_Crate_Struck = 0x6B5, // used by MediumCrate
    SOUND_6B6_Crate_Smash = 0x6B6,  // used by MediumCrate
    SOUND_6B7_Crate_Hiss = 0x6B7,   // used by MediumCrate

    SOUND_6BC_Creature_Cry = 0x6BC, // Duster

    SOUND_6CA_Chime = 0x6CA, // Duster

    SOUND_6E2 = 0x6E2, // used by KT_RexFloorSwitch

    SOUND_6EC_Mechanical_Hum_Loop = 0x6EC, // used in DLL 641

    SOUND_6F9_Explosion = 0x6F9, // used by KT_Rex
    SOUND_6FA = 0x6FA, // used by KT_Rex
    SOUND_6FB = 0x6FB, // used by KT_Rex
    SOUND_6FC_KT_Rex_Slam = 0x6FC,
    SOUND_6FD_KT_Rex_Roar = 0x6FD,
    SOUND_6FE_KT_Rex_Roar = 0x6FE,

    SOUND_700_Krystal_Ugh = 0x700,      //player hurt sfx
    SOUND_701_Sabre_Ugh_EMPTY = 0x701,  //NOTE: seems to be empty, but intended as Sabre's counterpart to 0x700

    SOUND_72F_Harsh_Magical_Thrum_Loop = 0x72F, // used in DLL 475 GPSH_flybaddie
    SOUND_730_Electrified_Blast = 0x730, // used in DLL 475 GPSH_flybaddie

    SOUND_74B_Garunda_Te_That_tastes_great_Hurry_up_boy = 0x74B,

    SOUND_768_Rope_Climb = 0x768, // used by player DLL
    SOUND_769_Whoosh_Ladder_Slide = 0x769, // used by player DLL

    SOUND_76D_Log_Bump = 0x76D, // used by DFlog, BWlog

    SOUND_775_Timer_Countdown = 0x775,

    SOUND_798_Puzzle_Solved = 0x798, // louder than B89, used when placing MoonSeeds

    SOUND_7B0 = 0x7B0,
    SOUND_7B1 = 0x7B1,

    SOUND_80B_Crackling_Freezing = 0x80B,
    SOUND_80C_Steam_Hissing = 0x80C,

    SOUND_851_Laser_Blast = 0x851, // DRbullet

    SOUND_87B_Ting = 0x87B, // Duster

    SOUND_87E_Duster_Cry = 0x87E, // Duster "Hey look at this!!"

    SOUND_883_Footstep_Mud_1 = 0x883, //player 
    SOUND_884_Footstep_Mud_2 = 0x884, //player 

    SOUND_88D = 0x88D,

    SOUND_8A2_Spore_Disintegrate = 0x8A2, // used in SHspore

    SOUND_8FC_Egg_Rattle = 0x8FC, //used by meatPickup (Dino Eggs)

    SOUND_912_Object_Refused = 0x912,

    SOUND_927_Harsh_Electric_Loop = 0x927, // DRbullet

    SOUND_95A_Frigid_Air_Loop = 0x95A, //used by Ice Blast Spell

    SOUND_986_Waterfall_Low_Loop = 0x986,   // used in DLL 6
    SOUND_987_Waterfall_High_Loop = 0x987,  // used in DLL 6

    SOUND_98F = 0x98F,
    SOUND_990 = 0x990,

    SOUND_99a_Mechanical_Ratcheting = 0x99a,

    SOUND_9A0 = 0x9A0,
    SOUND_9A1 = 0x9A1,

    SOUND_9A3_Magic_Reverse_Cymbal = 0x9A3,

    SOUND_9AA = 0x9AA,
    SOUND_9AB = 0x9AB,
    SOUND_9AC = 0x9AC,
    SOUND_9AD = 0x9AD,
    SOUND_9AE = 0x9AE,

    SOUND_9B1_Flames_Venting = 0x9B1,   // kamerian related
    SOUND_9B2_Fire_Burning = 0x9B2,     // kamerian related

    SOUND_9B4_Flamethrower = 0x9B4,     // kamerian related
    SOUND_9B5_Explosion = 0x9B5,        // kamerian related

    SOUND_9B9_Block_Shifting = 0x9B9,       //WCpushblock
    SOUND_9BA_Thud = 0x9BA,                 //WCpushblock
    SOUND_9BB_Magic_Reverse_Cymbal = 0x9BB, //WCpushblock

    SOUND_9C5_Vanish = 0x9C5,               //WCpushblock
    SOUND_9C6_Appear = 0x9C6,               //WCpushblock

    SOUND_A71_Rattling_1 = 0xA71, // used by MoonSeeds
    SOUND_A72_Rattling_2 = 0xA72, // used by MoonSeeds

    SOUND_A74 = 0xA74, // used by BWlog
    SOUND_A75 = 0xA75, // used by BWlog
    SOUND_A77 = 0xA77, // used by BWlog
    SOUND_A78_Water_Paddle = 0xA78,

    SOUND_A83_Lunaimar_Grunt = 0xA83,  //Lunaimar

    SOUND_AA4_Hock_Up = 0xAA4,         //Lunaimar
    SOUND_AA5_Heavy_Thwack = 0xAA5,    //Lunaimar

    SOUND_AAE = 0xAAE, // VFP_lavastar sound (Empty)

    SOUND_AB4_Magic_Barrier_Deflect = 0xAB4, //Lunaimar

    SOUND_B1C_Ladder_Climb_A = 0xB1C,
    SOUND_B1D_Ladder_Climb_B = 0xB1D,

    SOUND_B1E_Climb_Sound_B = 0xB1E,
    SOUND_B1F_Slow_Magic_Chimes = 0xB1F,    //Lunaimar, Balloon Baddie (respawn?)
    SOUND_B20_Low_Grunt = 0xB20,            //Lunaimar, Balloon Baddie (respawn?)
    SOUND_B21_Dissipating_Hiss = 0xB21,     //Lunaimar, Balloon Baddie 
    SOUND_B22_Climb_Sound_A = 0xB22,
    SOUND_B23_Teeth_Chattering = 0xB23, //rarely heard when leaving icy water?

    SOUND_B31_Item_Collection_Chime = 0xB31,

    SOUND_B5C_Machinery_Clunk = 0xB5C,

    SOUND_B72_Game_Saved = 0xB72,
    SOUND_B73_Option_Selected = 0xB73,
    SOUND_B74_Gentle_Magic_Loop = 0xB74,
    SOUND_B75_Water_Splash_Big = 0xB75, //Used by Balloon Baddie??

    SOUND_B89_Puzzle_Solved = 0xB89, // "puzzle complete" jingle

    SOUND_B95_Gasvent_Uncovered = 0xB95,
    SOUND_B96_Gasvent_Covered = 0xB96,

    SOUND_BA3_Burning_Loop = 0xBA3,
    SOUND_BA4_Spell_Aim_Hum_Loop = 0xBA4,

    SOUND_B3C_Machinery_Charge_Up = 0xB3C,

    NO_SOUND = -1
};

#define MAX_VOLUME 0x7F
#define MAX_VOLUME_F 127.0f
#define HALF_VOLUME 0x3F

#define PAN_CENTRE 0x40

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
/*11*/ s32 (*func_DE8)(u16 arg0, SoundDef* arg1);
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
