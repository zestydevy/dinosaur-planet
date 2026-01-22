#ifndef _SYS_NEWSHADOWS_H
#define _SYS_NEWSHADOWS_H

#include "PR/ultratypes.h"
#include "ultra64.h"
#include "PR/gbi.h"

#include "types.h"
#include "game/objects/object.h"
#include "sys/map.h"

// Might be incorrect definition but size should be correct
typedef struct Unk8004FA58 {
    Vec4f pos;
    u8 unk10;
} Unk8004FA58;

// Size: 0x2C
typedef struct Unk80092C48 {
    f32 unk0[8];
    u8 pad20[0xC];
} Unk80092C48;

typedef struct UnkFunc80052300Arg3 {
    Vec3f unk0[3];
} UnkFunc80052300Arg3;

typedef struct Unk8004EEC0 {
    u8 pad0[0x10];
    Gfx *gdl;
    u8 pad14[0x30 - 0x14];
    s32 unk30;
    u8 pad34[0x41 - 0x34];
    s8 unk41;
} Unk8004EEC0;

typedef struct Unk8004E540 {
    u8 pad0[0x18];
    f32 x;
    f32 y;
    f32 z;
    u8 pad24[0x37 - 0x24];
    u8 unk37;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 unk3B;
} Unk8004E540;

// xyz pair
extern f32 D_80092BD0;
extern f32 D_80092BD4;
extern f32 D_80092BD8;
extern UnkFunc80051D68Arg3 *D_80092BDC;
extern Vec3s32 *D_80092BE0;
extern f32 D_80092BE4;
extern s8 D_80092BE8;
extern s8 D_80092BF4;
extern s8 D_80092BF8;
extern s8 D_80092BFC;

// Some sort of toggle list
extern s8 D_80092C00;
extern s8 D_80092C04;
extern s8 D_80092C08;
extern s8 D_80092C0C;
extern s8 D_80092C10;
extern s8 D_80092C14;
extern s16 D_80092C18;
extern s16 D_80092C1C;
extern s16 D_80092C20;
extern s16 D_80092C24;
extern s16 D_80092C28;
extern s16 D_80092C2C;
extern s16 D_80092C30;
extern s16 D_80092C34;
extern s16 D_80092C38;
extern f32 D_80092C3C;
extern s32 D_80092C40;
extern f32 D_80092C44;
extern Unk80092C48 D_80092C48;
extern Unk80092C48 D_80092C74;
extern s32 D_80092CA0;
extern s32 D_80092CA4;

void shadows_init(void);
s32 shadows_update_obj_geom(Object *obj, s32 arg1, s32 arg2, s32 updateRate);
void shadows_update_obj_box(Object* arg0);
s32 shadows_func_80051D68(Object* arg0, s16 arg1, s16 arg2, UnkFunc80051D68Arg3* arg3, s32 arg4, Vec4f* arg5);
void shadows_set_custom_obj_pos(Object *obj, f32 x, f32 y, f32 z);
void shadows_clear_custom_obj_pos(Object *arg0);
void shadows_update_dynamic_tex(Object *obj, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols);
u32 shadows_init_obj_shadow(Object *obj, u32 addr, s32);
f32 shadows_func_800528AC(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec4f* arg3);
s32 shadows_calc_opacity(Object *obj, ObjectShadow *shadow);
void shadows_func_8004D974(s32 arg0);

#endif
