#ifndef _DLLS_210_H
#define _DLLS_210_H

#include "PR/ultratypes.h"
#include "dlls/engine/27.h"
#include "dlls/engine/29_gplay.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "sys/rand.h"
#include "sys/main.h"
#include "types.h"

// size: 0x3C
typedef struct Player_Data3B4 {
    s16 unk0;
    s16 unk2;
    u32 unk4;
    s16 unk8;
    s16 padA;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    u32 pad30;
    WeaponDataPtr unk34;
} Player_Data3B4;

// size: 0x48
typedef struct Player_Data490 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    Vec4f unk1C;
    Vec3f unk2C;
    Vec3f unk38;
    u16 pad44;
    s8 unk46;
    u8 unk47;
} Player_Data490;

// TODO: not necessarily in story order
// Which spirit the player is currently holding
typedef enum {
    PLAYER_NO_SPIRIT = 0x0,
    PLAYER_SPIRIT_1 = 0x1,
    PLAYER_SPIRIT_2 = 0x2,
    PLAYER_SPIRIT_3 = 0x4,
    PLAYER_SPIRIT_4 = 0x8,
    PLAYER_SPIRIT_5 = 0x10,
    PLAYER_SPIRIT_6 = 0x20,
    PLAYER_SPIRIT_7 = 0x40,
    PLAYER_SPIRIT_8 = 0x80,
    // Mask to check for any spirit flag
    PLAYER_SPIRIT_ANY = 0xFF
} PlayerSpiritFlags;

// size: 0x58
typedef struct UnkArg4 {
    Vec3f unk0;
    u8 padC[0x1C - 0xC];
    Vec3f unk1C;
    u8 pad28[0x34 - 0x28];
    Object *unk34;
    Object *unk38;
    f32 unk3C;
    f32 unk40;
    u8 pad44;
    s8 unk45;
    s8 unk46;
    u8 pad47;
    f32 unk48;
    f32 unk4C;
    u32 pad50;
    s16 unk54;
} UnkArg4;

// size: 0x60
typedef struct UnkArg3 {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    s8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    u8 pad10[0x18 - 0x10];
    f32 unk18;
    u8 pad1C[0x24 - 0x1C];
    f32 unk24;
    Vec4f unk28;
    Vec3f unk38;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    Vec3f unk54;
} UnkArg3;

// size: 0x30
typedef struct UnkArg2 {
    Vec4f unk0;
    Vec3f unk10;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s32 unk28;
    u8 pad2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F;
} UnkArg2;

// Size: 0x24
typedef struct Unk80032CF8Copy {
    /* 0x00 */ s8 unk0;
    /* 0x01 */ u8 pad1[0x4 - 0x1];
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 x;
    /* 0x0C */ f32 y;
    /* 0x10 */ f32 z;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 pad18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s8 unk1E;
    /* 0x1E */ u8 pad1F[0x24 - 0x1F];
} Unk80032CF8Copy;

typedef struct {
/*000*/ s32 unk0; //bitfield?
/*004*/ DLL27_Data unk4;
/*264*/ s32 unk264;
/*268*/ s32 unk268;
/*26c*/ s16 unk26C;
/*26e*/ s16 unk26E;
/*270*/ s16 unk270;
/*272*/ s8 unk272;
/*273*/ s8 unk273;
/*274*/ Vec3f *unk274;
/*278*/ f32 unk278;
/*27C*/ f32 unk27C;
/*280*/ s32 unk280;
/*284*/ f32 unk284;
/*288*/ f32 unk288;
/*28C*/ f32 unk28C;
/*290*/ f32 unk290;
/*294*/ f32 unk294;
/*298*/ f32 unk298;
/*29C*/ f32 unk29C;
/*2A0*/ s8 unk2A0[0x2B0 - 0x2A0];
/*2b0*/ f32 unk2B0;
/*2b4*/ s8 unk2B4[0x2C8 - 0x2B4];
/*2c8*/ Object *unk2C8;
/*2cc*/ s8 unk2CC[0x2FC - 0x2CC];
/*2fc*/ void* unk2FC; // stores a callback that with prototype: static void (*func)(Object* arg0, UNK_TYPE_32 arg1)
/*300*/ s32 pad300;
/*304*/ s32 unk304;
/*308*/ s32 pad308;
/*30C*/ s32 unk30C;
/*310*/ s32 unk310;
/*314*/ s8 unk314[0x324 - 0x314];
/*324*/ s16 unk324;
/*326*/ u16 pad326;
/*328*/ s16 unk328;
/*32A*/ s16 unk32A;
/*32C*/ s8 unk32C[0x33D - 0x32C];
/*33D*/ u8 unk33D;
/*33E*/ s8 unk33E[0x340 - 0x33E];
/*340*/ s8 unk340;
/*341*/ s8 unk341;
/*342*/ s8 pad342;
/*343*/ s8 unk343;
/*344*/ s8 pad344[0x34C - 0x344];
/*34C*/ PlayerStats* stats; //health, Dusters, etc!
/*350*/ s32 flags;
/*354*/ Unk80032CF8Copy unk354;
/*378*/ Unk80032CF8Copy unk378;
/*39C*/ Vec3f unk39C;
/*3A8*/ u8 pad3A8[0x3B4 - 0x3A8];
/*3B4*/ Player_Data3B4* unk3B4;
/*3B8*/ s16 *unk3B8;
/*3BC*/ void *unk3BC; // callback with prototype (void (*)(Object*, Player_Data*, f32))
/*3C0*/ s16* modAnims; //current modanim index array?
/*3C4*/ f32* unk3C4; //array of floats - seem to be speed thresholds for different walking animations?
/*3C8*/ f32 unk3C8;
/*3CC*/ UnkArg3 unk3CC;
/*42C*/ s32 pad42C;
/*430*/ UnkArg3 unk430;
/*490*/ Player_Data490 unk490;
/*4D8*/ UnkArg2 unk4D8;
/*508*/ u8 pad508[0x680 - 0x508];
/*680*/ UnkArg2 unk680;
/*6B0*/ UnkArg4 unk6B0;
/*708*/ Object *unk708;
/*70C*/ s16 unk70C;
/*70E*/ s8 unk70E[0x728 - 0x70E];
/*728*/ f32 unk728;
/*72C*/ Vec3f unk72C; // This might be the end of an SRT? unk728 would be the scale
/*734*/ u8 pad738[0x754 - 0x738];
/*754*/ s32 unk754;
/*758*/ s32 unk758;
/*75C*/ f32 unk75C;
/*760*/ f32 unk760;
/*764*/ u16 unk764;
/*766*/ u16 unk766;
/*768*/ u16 unk768;
/*76A*/ u16 pad76A;
/*76C*/ s16 *unk76C;
/*770*/ u8 unk770;
/*771*/ s8 unk771[0x774 - 0x771];
/*774*/ Vec3f unk774;
/*780*/ u8 pad780[0x7EC - 0x780];
/*7EC*/ Vec3f unk7EC;
/*7F8*/ s32 pad7F8;
/*7FC*/ f32 unk7FC;
/*800*/ f32 unk800;
/*804*/ f32 unk804;
/*808*/ f32 unk808;
/*80C*/ s32 aimX;
/*810*/ s32 aimY;
/*814*/ s32 aimZ;
/*818*/ f32 unk818;
/*81C*/ f32 unk81C;
/*820*/ s32 pad820;
/*824*/ s32 pad824;
/*828*/ s32 pad828;
/*82C*/ f32 unk82C;
/*830*/ f32 unk830;
/*834*/ f32 unk834;
/*838*/ s32 pad838;
/*83C*/ f32 unk83C;
/*840*/ f32 unk840;
/*844*/ f32 unk844;
/*844*/ u32 unk848;
/*84C*/ f32 unk84C;
/*850*/ Object *unk850;
/*854*/ s32 unk854;
/*858*/ Object *unk858;
/*85C*/ Object *unk85C;
/*860*/ Object *unk860;
/*864*/ s32 unk864;
/*868*/ Object* unk868; //held object (baskets etc.)
/*86C*/ f32 unk86C;
/*870*/ u8 unk870;
/*871*/ u8 unk871;
/*872*/ u16 unk872;
/*874*/ s16 unk874;
/*876*/ u16 pad876;
/*878*/ s16 unk878;
/*87A*/ s16 unk87A;
/*87C*/ s16 unk87C;
/*87E*/ s16 unk87E;
/*880*/ s32 unk880;
/*884*/ s16 unk884;
/*886*/ s16 unk886;
/*888*/ s16 unk888;
/*88A*/ s16 unk88A;
/*88C*/ u16 pad88C;
/*88E*/ u16 unk88E;
/*890*/ UNK_PTR *unk890;
/*894*/ UNK_PTR *unk894;
/*898*/ UNK_PTR *unk898;
/*89C*/ UNK_PTR *unk89C;
/*8A0*/ u8 unk8A0;
/*8A1*/ u8 pad8A1[0x8A4 - 0x8A1];
/*8A4*/ u8 unk8A4;
/*8A5*/ u8 unk8A5;
/*8A6*/ u8 unk8A6;
/*8A7*/ s8 unk8A7;
/*8A8*/ u8 unk8A8;
/*8A9*/ u8 unk8A9;
/*8AA*/ u8 unk8AA;
/*8AB*/ u8 unk8AB;
/*8AC*/ u8 pad8AC;
/*8AD*/ u8 unk8AD;
/*8AE*/ s8 unk8AE[4];
/*8B2*/ u8 unk8B2[0x8B4 - 0x8B2];
/*8B4*/ u8 unk8B4;
/*8B5*/ s8 unk8B5;
/*8B6*/ u8 unk8B6;
/*8B7*/ u8 unk8B7;
/*8B8*/ s8 unk8B8;
/*8B9*/ s8 unk8B9;
/*8BA*/ u8 unk8BA;
/*8BB*/ u8 unk8BB;
/*8BC*/ u8 unk8BC;
/*8BD*/ u8 unk8BD;
/*8BE*/ u8 unk8BE;
/*8BF*/ s8 unk8BF;
/*8C0*/ s8 unk8C0;
} Player_Data;

// AKA. krystal
DLL_INTERFACE(DLL_210_Player) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ Object *(*func7)(Object*);
    /*8*/ UnknownDLLFunc func8;
    /*9*/ UnknownDLLFunc func9;
    /*10*/ UnknownDLLFunc func10;
    /*11*/ UnknownDLLFunc func11;
    /*12*/ UnknownDLLFunc func12;
    /*13*/ UnknownDLLFunc func13;
    /*14*/ void (*func14)(Object* player, s32 magicDifference);
    /*15*/ UnknownDLLFunc func15;
    /*16*/ UnknownDLLFunc func16;
    /*17*/ UnknownDLLFunc func17;
    /*18*/ UnknownDLLFunc func18;
    /*19*/ void (*func19)(Object* player, s32 scarabsDifference);
    /*20*/ UnknownDLLFunc func20;
    /*21*/ UnknownDLLFunc func21;
    /*22*/ UnknownDLLFunc func22;
    /*23*/ UnknownDLLFunc func23;
    /*24*/ void (*func24)(Object* player, s32 healthDifference);
    /*25*/ UnknownDLLFunc func25;
    /*26*/ UnknownDLLFunc func26;
    /*27*/ UnknownDLLFunc func27;
    /*28*/ UnknownDLLFunc func28;
    /*29*/ UnknownDLLFunc func29;
    /*30*/ UnknownDLLFunc func30;
    /*31*/ UnknownDLLFunc func31;
    /*32*/ UnknownDLLFunc func32;
    /*33*/ UnknownDLLFunc func33;
    /*34*/ s32 (*func34)(Object*);
    /*35*/ UnknownDLLFunc func35;
    /*36*/ UnknownDLLFunc func36;
    /*37*/ UnknownDLLFunc func37;
    /*38*/ s32 (*func38)(Object*,s32);
    /*39*/ void (*func39)(Object*,s32,s32);
    /*40*/ UnknownDLLFunc func40;
    /*41*/ UnknownDLLFunc func41;
    /*42*/ UnknownDLLFunc func42;
    /*43*/ s32 (*func43)(Object*);
    /*44*/ UnknownDLLFunc func44;
    /*45*/ UnknownDLLFunc func45;
    /*46*/ UnknownDLLFunc func46;
    /*47*/ UnknownDLLFunc func47;
    /*48*/ s32 (*func48)(Object*);
    /*49*/ UnknownDLLFunc func49;
    /*50*/ s32 (*func50)(Object*);
    /*51*/ UnknownDLLFunc func51;
    /*53*/ f32 *(*func52)(Object*);
    /*52*/ UnknownDLLFunc func53;
    /*55*/ void *(*func54)(Object*);
    /*54*/ UnknownDLLFunc func55;
    /*57*/ f32 (*func56)(Object*);
    /*57*/ UnknownDLLFunc func57;
    /*58*/ UnknownDLLFunc func58;
    /*59*/ UnknownDLLFunc func59;
    /*60*/ UnknownDLLFunc func60;
    /*61*/ UnknownDLLFunc func61;
    /*62*/ UnknownDLLFunc func62;
    /*63*/ UnknownDLLFunc func63;
    /*64*/ UnknownDLLFunc func64;
    /*65*/ void (*func65)(Object*,f32,f32,f32);
    /*66*/ Object *(*func66)(Object*, s32);
    /*67*/ void (*func67)(Object*,s32,f32);
    /*68*/ UnknownDLLFunc func68;
    /*69*/ UnknownDLLFunc func69;
    /*70*/ UnknownDLLFunc func70;
    /*71*/ UnknownDLLFunc func71;
    /*72*/ void (*func72)(Object*,s32,s32);
    /*73*/ UnknownDLLFunc func73;
    /*74*/ UnknownDLLFunc func74;
    /*75*/ UnknownDLLFunc func75;
    /*76*/ UnknownDLLFunc func76;
    /*77*/ UnknownDLLFunc func77;
    /*78*/ UnknownDLLFunc func78;
    /*79*/ UnknownDLLFunc func79;
};

#endif //_DLLS_210_H
