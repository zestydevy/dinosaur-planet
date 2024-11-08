#ifndef _SYS_GAME_ACTOR_HITBOX_H
#define _SYS_GAME_ACTOR_HITBOX_H

#include "sys/math.h"

/** Actor hit detection system
 */

typedef struct {
/*0000*/    u8 unk_0x0[0x10 - 0x0];
/*0010*/    Vec3f unk_0x10;
/*001c*/    u8 unk_0x1c[0x20 - 0x1c];
/*0020*/    Vec3f unk_0x20;
/*002c*/    u8 unk_0x2c[0x30 - 0x2c];
/*0030*/    u8 unk_0x30[0x5a - 0x30];
/*005A*/    u8 unk_0x5a;
/*005B*/    u8 unk_0x5b[0x9f - 0x5b];
/*009F*/    s8 unk_0x9f;
} ActorObjhitInfo;

#endif //_SYS_GAME_ACTOR_HITBOX_H
