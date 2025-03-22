#ifndef _SYS_GAME_ACTOR_HITBOX_H
#define _SYS_GAME_ACTOR_HITBOX_H

#include "sys/math.h"

/** Actor hit detection system
 */

typedef struct {
/*0000*/    u8 unk_0x0[0x10 - 0x0];
/*0010*/    Vec3f unk_0x10;
/*001C*/    u8 unk_0x1c[0x20 - 0x1c];
/*0020*/    Vec3f unk_0x20;
/*002C*/    u8 unk_0x2c[0x30 - 0x2c];
/*0030*/    u8 unk_0x30[0x40 - 0x30];
/*0040*/    s32 unk_0x40;
/*0044*/    s32 unk_0x44;
/*0048*/    s32 unk_0x48;
/*004C*/    s32 unk_0x4c;
/*0050*/    u8 unk_0x50[0x52 - 0x50];
/*0052*/    s16 unk_0x52;
/*0054*/    s16 unk_0x54;
/*0056*/    s16 unk_0x56;
/*0058*/    s16 unk_0x58;
/*005A*/    u8 unk_0x5a;
/*005B*/    u8 unk_0x5b[0x5f - 0x5b];
/*005F*/    u8 unk_0x5f;
/*0060*/    u8 unk_0x60;
/*0061*/    u8 unk_0x61;
/*0062*/    u8 unk_0x62;
/*0063*/    u8 unk_0x63[0x9e - 0x63];
/*009E*/    s8 unk_0x9e;
/*009F*/    s8 unk_0x9f;
} ActorObjhitInfo;

#endif //_SYS_GAME_ACTOR_HITBOX_H
