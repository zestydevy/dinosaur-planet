#ifndef _SYS_LIGHTING_H
#define _SYS_LIGHTING_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"
#include "sys/map.h"
#include "sys/gfx/model.h"

// size: 0x1C
typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u8 pad6[0x10 - 0x6];
    Vec3f pos;
} Unk800B1858;

// size: 0x1C?
typedef struct {
    s16 yaw;
    u8 pad6[0xC - 0x2];
    Vec3f pos;
    u32 pad1C;
} Unk800B1860;

extern u8 D_80090CA0;
extern s32 D_80090CA8;
extern f32 D_80090CAC;
extern u8 D_800B1845;
extern u8 D_800B1847;
extern Unk800B1858 *D_800B1858;
extern Unk800B1860 *D_800B1860[30];

void lightInit(void);
void light_func_8001CDE4(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
void light_func_8001D548(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7);
void lightDynamicModelLighting(Object* obj, ModelInstance* modelInst);
void light_func_8001E818(Object*, Model*, ModelInstance*);
void lightDimAmbient(u8 r, u8 g, u8 b, f32 t);
void lightClearEmitters(void);
void lightSetInside(s32 inside);
u8 lightGetInside(void);
void lightUpdateSkyLight(f32 dirX, f32 dirY, f32 dirZ, u8 arg3, u8 arg4, s8 arg5, s8 arg6, u8 r, u8 g, u8 b);
void lightModelSphereMapping(ModelInstance*);
void lightBlockSphereMapping(Block* block, s32 blockX, s32 blockZ);
void lightGetAmbient(u8 *r, u8 *g, u8 *b);
void lightAmbientDL(Gfx **gdl);

#endif
