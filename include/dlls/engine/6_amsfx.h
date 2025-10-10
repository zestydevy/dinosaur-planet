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
    SFX_RECAP_MENU_BACK = 0x5D4,
    SFX_RECAP_MENU_MOVE = 0x5D5,
    SFX_RECAP_MENU_SELECT = 0x5D6,

    SFX_SNOWHORN_YAWN1 = 0x129,
    SFX_SNOWHORN_SNOREHORN = 0x12A,
    SFX_SNOWHORN_YAWN2 = 0x12B,

    SOUND_1e1_Stone_Moving = 0x1e1,

    SOUND_1E2 = 0x1E2, // snowhorn related

    SOUND_242 = 0x242, // menu related

    SOUND_342 = 0x342, // used in DLL 466

    SOUND_377 = 0x377, // snowhorn related

    SOUND_3D8 = 0x3D8, // used in pushpull (DLL 274)

    SOUND_50a_Fire_Burning_Low_Loop = 0x50a,
    SOUND_50b_Fire_Burning_High_Loop = 0x50b,

    SOUND_5EB = 0x5EB, // used in DLL 210

    SFX_PAUSE_MENU_BACK = 0x5D4, // Used in DLL 66
    SFX_PAUSE_MENU_MOVE = 0x5D5, // Used in DLL 66

    SOUND_5F6 = 0x5F6, // used in DLL 227
    SOUND_5F7 = 0x5F7, // used in DLL 227

    SOUND_614 = 0x614, // used in DLL 227
    SOUND_615 = 0x615, // used in DLL 227

    SOUND_677 = 0x677, // snowhorn related

    SOUND_6EC = 0x6EC, // used in DLL 641

    SOUND_74B = 0x74B, // snowhorn related

    SOUND_775 = 0x775, // menu related

    SOUND_8A2 = 0x8A2, // used in DLL 505

    SOUND_986 = 0x986, // used in DLL 6
    SOUND_987 = 0x987, // used in DLL 6

    SOUND_99a_Mechanical_Ratcheting = 0x99a,

    SOUND_9B1 = 0x9B1, // kamerian related
    SOUND_9B2 = 0x9B2, // kamerian related

    SOUND_9B4 = 0x9B4, // kamerian related
    SOUND_9B5 = 0x9B5, // kamerian related

    SOUND_AAE = 0xAAE, // VFP_lavastar sound

    SOUND_B31 = 0xB31, // Used in DLL 505

    SOUND_B72 = 0xB72, // pause menu item selected
    SOUND_B73 = 0xB73, // pause menu no item selected

    SOUND_B89_Puzzle_Solved = 0xB89, // "puzzle complete" jingle

    NO_SOUND = -1
};

#define MAX_VOLUME 0x7F

DLL_INTERFACE(DLL_6_AMSFX) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ void (*func_338)(void);
    /*1*/ void (*func_480)(Object *obj);
    // Play sound using source object for panning and distance falloff
    /*2*/ u32 (*play_sound)(Object* obj, u16 soundID, u8 volume, u32* arg3, char *arg4, s32 arg5, char *arg6);
    /*3*/ void (*func_7E4)(u8 arg0);
    /*4*/ void (*func_860)(u32 arg0, u8 arg1);
    /*5*/ void (*func_954)(u32 arg0, f32 arg1);
    /*6*/ void (*func_A1C)(u32 arg0); //stop_sound?
    /*7*/ void (*func_A6C)(Object *obj);
    /*8*/ s32 (*func_B48)(u32 arg0);
    /*9*/ s32 (*func_BBC)(Object* arg0, s32 arg1, s32 arg2, Object **arg3, s32 arg4);
    /*10*/ s32 (*func_DCC)(s32 arg0, UNK_TYPE_32 arg1, s32* arg2, s32* arg3);
    /*11*/ void (*func_DE8)(u16 arg0, UnkDE8* arg1);
    /*12*/ s32 (*func_EE4)(s32 arg0, UNK_TYPE_32 arg1);
    /*13*/ void (*func_F2C)(Object* arg0, u16 arg1, u32* arg2, char *arg3, s32 arg4);
    /*14*/ void (*func_10D0)(Object* obj, u16 soundID, u32*);
    /*15*/ void (*func_1174)(Object* arg0, u16 arg1, u8 arg2, u32* arg3);
    /*16*/ void (*func_1218)(Object *obj);
    /*17*/ void (*func_1504)(s32 arg0, Object* arg1, Object* arg2, f32 arg3);
    /*18*/ void (*func_16E0)(void);
    /*19*/ void (*water_falls_set_flags)(u8 arg0);
    // arg2 might be passed in as a f32
    /*20*/ void (*func_1320)(Object* arg0, u16 arg1, s32 arg2, f32 arg3, f32 arg4, u32* arg5) ;
};

#endif //_DLLS_6_H
