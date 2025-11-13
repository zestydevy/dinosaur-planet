#ifndef _DLL_17_H
#define _DLL_17_H

#include "dll_def.h"
#include "game/objects/object.h"
#include "types.h"

enum ParticleID {
    PARTICLE_5A = 0x5A,
    PARTICLE_5B = 0x5B,
    PARTICLE_84 = 0x84, // implemented by DLL 40
    PARTICLE_85 = 0x85, // implemented by DLL 40
    /* --- DLL 40 START --- */
    PARTICLE_8A = 0x8A,
    PARTICLE_Explosion = 0xAB,
    PARTICLE_C7 = 0xC7,
    /* --- DLL 40 END --- */
    /* --- DLL 38 START --- */
    PARTICLE_C8 = 0xC8,
    PARTICLE_C9 = 0xC9,
    PARTICLE_12B = 0x12B,
    /* --- DLL 38 END --- */
    /* --- DLL 44 START --- */
    PARTICLE_12C = 0x12C,
    PARTICLE_12E = 0x12E,
    PARTICLE_12F = 0x12F,
    PARTICLE_130 = 0x130,
    PARTICLE_131 = 0x131,
    PARTICLE_136 = 0x136,
    PARTICLE_18F = 0x18F,
    /* --- DLL 44 END --- */
    /* --- DLL 37 START --- */
    PARTICLE_190 = 0x190,
    PARTICLE_1F3 = 0x1F3,
    /* --- DLL 37 END --- */
    /* --- DLL 36 START --- */
    PARTICLE_1F4 = 0x1F4,
    PARTICLE_202 = 0x202,
    PARTICLE_257 = 0x257,
    /* --- DLL 36 END --- */
    /* --- DLL 35 START --- */
    PARTICLE_258 = 0x258,
    PARTICLE_286 = 0x286,
    PARTICLE_2BB = 0x2BB,
    /* --- DLL 35 END --- */
    PARTICLE_320 = 0x320,
    PARTICLE_321 = 0x321,
    PARTICLE_322 = 0x322,
    PARTICLE_323 = 0x323, // used by objhits.c
    PARTICLE_324 = 0x324, // used by objhits.c
    PARTICLE_325 = 0x325, // used by objhits.c
    PARTICLE_328 = 0x328,
    /* --- DLL 43 START --- */
    PARTICLE_32A = 0x32A,
    PARTICLE_32D = 0x32D,
    PARTICLE_32E = 0x32E,
    PARTICLE_32F = 0x32F,
    PARTICLE_330 = 0x330,
    PARTICLE_331 = 0x331,
    PARTICLE_333 = 0x333,
    PARTICLE_334 = 0x334,
    PARTICLE_Green_Slime_Drop = 0x335,
    PARTICLE_340 = 0x340,
    PARTICLE_343 = 0x343,
    PARTICLE_34C = 0x34C,
    PARTICLE_34D = 0x34D,
    PARTICLE_350 = 0x350,
    /* --- DLL 43 END --- */
    PARTICLE_351 = 0x351,
    /* --- DLL 41 START --- */
    PARTICLE_352 = 0x352,
    PARTICLE_383 = 0x383,
    /* --- DLL 41 END --- */
    /* --- DLL 34 START --- */
    PARTICLE_384 = 0x384,
    PARTICLE_3A3 = 0x3A3,
    PARTICLE_3A4 = 0x3A4,
    PARTICLE_3B4 = 0x3B4,
    /* --- DLL 34 END --- */
    PARTICLE_3B5 = 0x3B5,
    /* --- DLL 42 START --- */
    PARTICLE_3B6 = 0x3B6,
    PARTICLE_3C4 = 0x3C4,
    PARTICLE_3C5 = 0x3C5,
    PARTICLE_3DD = 0x3DD,
    /* --- DLL 42 END --- */
    PARTICLE_Gasvent_Gas = 0x3DF,
    /* --- DLL 48 START --- */
    PARTICLE_3E8 = 0x3E8,
    PARTICLE_3EC = 0x3EC,
    PARTICLE_3ED = 0x3ED,
    PARTICLE_3F1 = 0x3F1,
    PARTICLE_3F3 = 0x3F3,
    PARTICLE_419 = 0x419,
    /* --- DLL 48 END --- */
    /* --- DLL 39 START --- */
    PARTICLE_41A = 0x41A,
    PARTICLE_425 = 0x425,
    PARTICLE_426 = 0x426,
    PARTICLE_427 = 0x427,
    PARTICLE_44B = 0x44B,
    /* --- DLL 39 END --- */
    /* --- DLL 46 START --- */
    PARTICLE_44C = 0x44C,
    PARTICLE_47D = 0x47D,
    /* --- DLL 46 END --- */
    /* --- DLL 45 START --- */
    PARTICLE_47E = 0x47E,
    PARTICLE_483 = 0x483,
    PARTICLE_484 = 0x484,
    PARTICLE_486 = 0x486,
    PARTICLE_487 = 0x487,
    PARTICLE_488 = 0x488,
    PARTICLE_48C = 0x48C,
    PARTICLE_4AF = 0x4AF,
    /* --- DLL 45 END --- */
    /* --- DLL 47 START --- */
    PARTICLE_4B0 = 0x4B0,
    PARTICLE_4B2 = 0x4B2,
    PARTICLE_4B3 = 0x4B3,
    PARTICLE_4BB = 0x4BB,
    PARTICLE_4B5 = 0x4B5,
    PARTICLE_4BA = 0x4BA,
    PARTICLE_4BC = 0x4BC,
    PARTICLE_4C6 = 0x4C6,
    PARTICLE_4C7 = 0x4C7,
    PARTICLE_4C8 = 0x4C8,
    PARTICLE_4E1 = 0x4E1,
    /* --- DLL 47 END --- */
    PARTICLE_51A = 0x51A,
    PARTICLE_51F = 0x51F,
    PARTICLE_521 = 0x521,
    PARTICLE_525 = 0x525,
    PARTICLE_526 = 0x526,
    PARTICLE_529 = 0x529,
    /* --- DLL 34 START --- */
    PARTICLE_5DD = 0x5DD,
    PARTICLE_640 = 0x640,
    /* --- DLL 34 END --- */
    PARTICLE_680 = 0x680,
    PARTICLE_683 = 0x683,
    PARTICLE_684 = 0x684,
    /* --- DLL 49 START --- */
    PARTICLE_6D7 = 0x6D7,
    PARTICLE_707 = 0x707,
    /* --- DLL 49 END --- */
    /* --- DLL 51 START --- */
    PARTICLE_708 = 0x708,
    PARTICLE_70D = 0x70D,
    PARTICLE_70E = 0x70E,
    PARTICLE_70F = 0x70F,
    PARTICLE_710 = 0x710,
    PARTICLE_711 = 0x711,
    PARTICLE_713 = 0x713,
    PARTICLE_717 = 0x717,
    PARTICLE_718 = 0x718,
    PARTICLE_739 = 0x739,
    /* --- DLL 51 END --- */
    /* --- DLL 50 START --- */
    PARTICLE_73A = 0x73A,
    PARTICLE_73B = 0x73B,
    PARTICLE_76B = 0x76B,
    /* --- DLL 50 END --- */
    /* --- DLL 52 START --- */
    PARTICLE_76C = 0x76C,
    PARTICLE_79D = 0x79D
    /* --- DLL 52 END --- */
};

enum PartfxFlags {
    PARTFXFLAG_NONE = 0x0,
    PARTFXFLAG_1 = 0x1,
    PARTFXFLAG_2 = 0x2,
    PARTFXFLAG_4 = 0x4,
    PARTFXFLAG_800 = 0x800,
    PARTFXFLAG_200000 = 0x200000
};

// Interface for DLLs 34-52, which implement many of the particle routines that DLL 17 does not handle.
DLL_INTERFACE(DLL_IPartfx) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(void);
/*1*/ s32 (*spawn)(Object *obj, s32 id, SRT *transform, s32 flags, s8 arg4, void *data);
/*2*/ void (*func2)(s32 arg0);
};

DLL_INTERFACE(DLL_17) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(void);
      // Spawn one instance of a particle.
      // The last argument varies in datatype depending on the type of particle being spawned.
/*1*/ s32 (*spawn)(Object *obj, s32 id, SRT *transform, s32 flags, s8 arg4, void *data);
/*2*/ s32 (*func2)(s32 arg0);
};

#endif // _DLL_17_H
