#ifndef _SYS_GAME_OBJECTS_HITBOX_H
#define _SYS_GAME_OBJECTS_HITBOX_H

#include "sys/math.h"

/** Object hit detection system
 */

typedef struct {
    /*0x00*/ struct Object *unk0;
    /*0x04*/ s16 unk4;
    /*0x06*/ s16 unk6;
    /*0x08*/ u32 *unk8;
    /*0x0c*/ f32 unkC;
    /*0x10*/ Vec3f unk10;
    /*0x1c*/ f32 unk1C;
    /*0x20*/ Vec3f unk20;
    /*0x2c*/ f32 unk2C;
    /*0x30*/ f32 unk30;
    /*0x34*/ f32 unk34;
    /*0x38*/ f32 unk38;
    /*0x3C*/ f32 unk3C;
    /*0x40*/ u32 unk40;
    /*0x44*/ u32 unk44;
    /*0x48*/ s32 unk48;
    /*0x4c*/ s32 unk4C;
    /*0x50*/ s16 unk50;
    /*0x52*/ s16 unk52;
    /*0x54*/ s16 unk54;
    /*0x56*/ s16 unk56;
    /*0x58*/ s16 unk58;
    /*0x5a*/ u8 unk5A;
    /*0x5b*/ u8 unk5B;
    /*0x5c*/ u8 unk5C;
    /*0x5d*/ s8 unk5D;
    /*0x5e*/ s8 unk5E;
    /*0x5f*/ u8 unk5F;
    /*0x60*/ u8 unk60;
    /*0x61*/ s8 unk61;
    /*0x62*/ s8 unk62;
    /*0x63*/ s8 unk63[3];
    /*0x66*/ s8 unk66[3];
    /*0x69*/ u8 unk69[3];
    /*0x6c*/ struct Object *unk6C[3];
    /*0x78*/ f32 unk78[3];
    /*0x84*/ f32 unk84[3];
    /*0x90*/ f32 unk90[3];
    /*0x9c*/ s8 unk9C;
    /*0x9d*/ s8 unk9D;
    /*0x9e*/ u8 unk9E;
    /*0x9f*/ s8 unk9F;
    /*0xa0*/ u8 unkA0;
    /*0xa1*/ u8 unkA1;
} ObjectHitInfo;

#endif
