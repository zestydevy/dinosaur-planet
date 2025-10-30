#ifndef _DLL_17_H
#define _DLL_17_H

#include "dll_def.h"
#include "game/objects/object.h"
#include "types.h"

enum ParticleID {
    PARTICLE_5A = 0x5A,
    PARTICLE_5B = 0x5B,
    PARTICLE_85 = 0x85,
    PARTICLE_Explosion = 0xAB,
    PARTICLE_C9 = 0xC9,
    PARTICLE_136 = 0x136,
    PARTICLE_202 = 0x202,
    PARTICLE_286 = 0x286,
    PARTICLE_320 = 0x320,
    PARTICLE_321 = 0x321,
    PARTICLE_322 = 0x322,
    PARTICLE_323 = 0x323, // used by objhits.c
    PARTICLE_324 = 0x324, // used by objhits.c
    PARTICLE_325 = 0x325, // used by objhits.c
    PARTICLE_328 = 0x328,
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
    PARTICLE_351 = 0x351,
    PARTICLE_Gasvent_Gas = 0x3DF,
    PARTICLE_3A3 = 0x3A3,
    PARTICLE_3A4 = 0x3A4,
    PARTICLE_3EC = 0x3EC,
    PARTICLE_3ED = 0x3ED,
    PARTICLE_3F1 = 0x3F1,
    PARTICLE_3F3 = 0x3F3,
    PARTICLE_425 = 0x425,
    PARTICLE_426 = 0x426,
    PARTICLE_427 = 0x427,
    PARTICLE_47E = 0x47E,
    PARTICLE_483 = 0x483,
    PARTICLE_484 = 0x484,
    PARTICLE_486 = 0x486,
    PARTICLE_487 = 0x487,
    PARTICLE_488 = 0x488,
    PARTICLE_48C = 0x48C,
    PARTICLE_4B2 = 0x4B2,
    PARTICLE_4B3 = 0x4B3,
    PARTICLE_4BB = 0x4BB,
    PARTICLE_4B5 = 0x4B5,
    PARTICLE_4BA = 0x4BA,
    PARTICLE_4BC = 0x4BC,
    PARTICLE_4C6 = 0x4C6,
    PARTICLE_4C7 = 0x4C7,
    PARTICLE_4C8 = 0x4C8,
    PARTICLE_51A = 0x51A,
    PARTICLE_51F = 0x51F,
    PARTICLE_521 = 0x521,
    PARTICLE_525 = 0x525,
    PARTICLE_526 = 0x526,
    PARTICLE_529 = 0x529,
    PARTICLE_680 = 0x680,
    PARTICLE_683 = 0x683,
    PARTICLE_684 = 0x684,
    PARTICLE_70D = 0x70D,
    PARTICLE_70E = 0x70E,
    PARTICLE_70F = 0x70F,
    PARTICLE_76C = 0x76C
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
/*2*/ void (*func2)(void);
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
