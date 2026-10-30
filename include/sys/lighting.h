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
    s8 unk6;
    s8 unk7;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
    Vec3f pos;
} ModelLight;

extern u8 gLastModLight;
extern s32 gDisableSkyLightUpdates;
extern f32 gSkyLightPower;
extern u8 gNumModLights;
extern u8 D_800B1847;
extern ModelLight *gModLights;
extern Object *gModLightParents[30];

void lightInit(void);
void light_func_8001CDE4(u8 arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4);
void lightSetModLightPalette1(u8 r, u8 g, u8 b, s32 slot);
void lightSetModLightPalette2(u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2, s32 slot, u8 baseSkylightAmt);
void lightDynamicModelLighting(Object* obj, ModelInstance* modelInst);
void light_func_8001E818(Object*, Model*, ModelInstance*);
void lightDimSkyLight(u8 r, u8 g, u8 b, f32 t);
void lightClearEmitters(void);
void lightSetInside(s32 inside);
s32 lightGetInside(void);
void lightUpdateSkyLight(f32 dirX, f32 dirY, f32 dirZ, u8 arg3, u8 arg4, s8 arg5, s8 arg6, u8 r, u8 g, u8 b);
void lightModelSphereMapping(ModelInstance*);
void lightBlockSphereMapping(Block* block, s32 blockX, s32 blockZ);
void lightGetSkyLight(u8 *r, u8 *g, u8 *b);
void lightSkyLightDL(Gfx **gdl);

#endif
