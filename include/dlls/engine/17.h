#ifndef _DLL_17_H
#define _DLL_17_H

#include "dll_def.h"
#include "game/objects/object.h"
#include "types.h"

enum ParticleID {
    PARTICLE_85 = 0x85,
    PARTICLE_Explosion = 0xAB,
    PARTICLE_32F = 0x32F,
    PARTICLE_330 = 0x330,
    PARTICLE_331 = 0x331,
    PARTICLE_333 = 0x333,
    PARTICLE_334 = 0x334,
    PARTICLE_Green_Slime_Drop = 0x335,
    PARTICLE_340 = 0x340,
    PARTICLE_4BB = 0x4BB,
    PARTICLE_Gasvent_Gas = 0x3DF,
    PARTICLE_4B5 = 0x4B5,
    PARTICLE_4BA = 0x4BA,
    PARTICLE_4BC = 0x4BC,
    PARTICLE_4C6 = 0x4C6,
    PARTICLE_4C7 = 0x4C7,
    PARTICLE_4C8 = 0x4C8
};

DLL_INTERFACE(DLL_17) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(void);
/*1*/ s32 (*func1)(Object *obj, s32, SRT *, s32, s32, void *); //creates billboard particles
/*2*/ UnknownDLLFunc func2;
};

#endif // _DLL_17_H
