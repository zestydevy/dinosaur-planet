#ifndef _SYS_OLDSHADOWS_H
#define _SYS_OLDSHADOWS_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"
#include "sys/gfx/map.h"

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
} UnkOldShadowStruct;

// size: 0x14
typedef struct {
/*00*/ Vec3f unk0;
/*0C*/ u8 unkC;
/*10*/ Object *unk10;
} UnkOldShadowStruct2;

void oldshadow_init(void);
void oldshadow_toggle(u32 enabled);
void oldshadow_func_8005CA88(Object *obj, Vec3f *a1, u8 a2);
void oldshadow_func_8005CC74(Gfx **gdl, Object *arg1);

#endif
