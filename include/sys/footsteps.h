#ifndef _SYS_FOOTSTEPS_H
#define _SYS_FOOTSTEPS_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"
#include "sys/map.h"

// size: 0x90
typedef struct {
/*00*/ Gfx dl[3];
/*18*/ Vtx_t unk18[4];
/*58*/ DLTri unk58[2];
/*78*/ f32 unk78;
/*7C*/ f32 unk7C;
/*80*/ Object *obj;
/*84*/ s32 unk84;
/*88*/ s16 unk88;
} UnkFootstepsStruct;

// size: 0x14
typedef struct {
/*00*/ Vec3f unk0;
/*0C*/ u8 unkC;
/*10*/ Object *unk10;
} UnkFootstepsStruct2;

void footsteps_init(void);
void footsteps_toggle(u32 enabled);
void footsteps_func_8005CA88(Object *obj, Vec3f *a1, u8 a2);
void footsteps_func_8005CC74(Gfx **gdl, Object *arg1);
u16 *footsteps_get_sfx_bank(s32 bank);

#endif
