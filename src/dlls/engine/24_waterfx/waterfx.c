#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "PR/os.h"
#include "dlls/engine/24_waterfx.h"

#include "gbi_extra.h"
#include "macros.h"
#include "sys/camera.h"
#include "sys/gfx/textable.h"
#include "sys/gfx/texture.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "sys/rand.h"
#include "sys/main.h"
#include "sys/map.h"

#include "prevent_bss_reordering.h"

#define MAX_CIRCULAR_RIPPLES 30
#define MAX_MOVEMENT_RIPPLES 30
#define MAX_SPLASHES 10
#define MAX_SPLASH_PARTICLES 30

// size: 0x1C
typedef struct {
/*00*/ f32 x;
/*04*/ f32 y;
/*08*/ f32 z;
/*0C*/ f32 unkC;
/*10*/ f32 scale;
/*14*/ s16 yaw;
/*16*/ s16 alpha;
/*18*/ s16 decayRate;
} CircularWaterRipple;

// size: 0x5C
typedef struct {
/*00*/ f32 x;
/*04*/ f32 y;
/*08*/ f32 z;
/*0C*/ f32 unkC[6];
/*24*/ f32 unk24[6];
/*3C*/ f32 unk3C[6];
/*54*/ s16 alpha;
/*56*/ s16 unk56;
/*58*/ u8 particleCount;
} WaterSplash;

// size: 0x1C
typedef struct {
/*00*/ f32 x;
/*04*/ f32 y;
/*08*/ f32 z;
/*0C*/ f32 unkC;
/*10*/ f32 scale;
/*14*/ s16 alpha;
/*16*/ s16 yaw;
/*18*/ u8 hide;
} MovementWaterRipple;

// size: 0x14
typedef struct {
/*00*/ f32 xVel;
/*04*/ f32 zVel;
/*08*/ f32 speed; // lateral only
/*0C*/ f32 yVel;
/*10*/ s16 unk10;
/*12*/ s8 splashIdx; // index of the linked water splash instance
} WaterSplashParticle;

void waterfx_init(void);
void waterfx_spawn_splash(f32 x, f32 y, f32 z, f32 size);
void waterfx_spawn_movement_ripple(f32 x, f32 y, f32 z, s16 yaw, f32 arg4);
void waterfx_spawn_circular_ripple(f32 x, f32 y, f32 z, s16 yaw, f32 arg4, s32 decayRate);

static Vtx *sCircularRippleVerts;
static DLTri *sCircularRippleTris;

static Vtx *sWaterSplashVerts;
static DLTri *sWaterSplashTris;

static Vtx *sWaterSplashPartVerts;
static DLTri *sWaterSplashPartTris;

static Vtx *sMovementRippleVerts;
static DLTri *sMovementRippleTris;

static s32 sNumCircularRipples;
static CircularWaterRipple *sCircularRipples;

static s32 sNumWaterSplashes;
static WaterSplash *sWaterSplashes;

static s32 sNumMovementRipples;
static MovementWaterRipple *sMovementRipples;

static s32 sNumWaterSplashParticles;
static WaterSplashParticle *sWaterSplashParticles;

static Texture *sCircularWaterRippleTex;
static Texture *sWaterSplashTex;
static Texture *sWaterSplashParticleTex;
static Texture *sMovementWaterRippleTex;

static f32 sCircularRippleScale;

// offset: 0x0 | ctor
void waterfx_ctor(void *dll) {
    u8 *mem;

    const u32 circRipTrisOffset = 0;
    const u32 splashTrisOffset = circRipTrisOffset + (sizeof(DLTri) * MAX_CIRCULAR_RIPPLES * 2);
    const u32 splashPartTrisOffset = splashTrisOffset + (sizeof(DLTri) * MAX_SPLASHES * 12);
    const u32 movRipTrisOffset = splashPartTrisOffset + (sizeof(DLTri) * MAX_SPLASH_PARTICLES * 2);

    const u32 circRipVtxsOffset = movRipTrisOffset + (sizeof(DLTri) * MAX_MOVEMENT_RIPPLES * 2);
    const u32 splashVtxsOffset = circRipVtxsOffset + (sizeof(Vtx) * MAX_CIRCULAR_RIPPLES * 4);
    const u32 splashPartVtxsOffset = splashVtxsOffset + (sizeof(Vtx) * MAX_SPLASHES * 14);
    const u32 movRipVtxsOffset = splashPartVtxsOffset + (sizeof(Vtx) * MAX_SPLASH_PARTICLES * 4);

    const u32 movRipVtxsSize = (sizeof(Vtx) * MAX_MOVEMENT_RIPPLES * 4);
    const u32 circRipsSize = (sizeof(CircularWaterRipple) * MAX_CIRCULAR_RIPPLES);
    const u32 splashesSize = (sizeof(WaterSplash) * MAX_SPLASHES);
    const u32 splashPartsSize = (sizeof(WaterSplashParticle) * MAX_SPLASH_PARTICLES);
    const u32 movRipsSize = (sizeof(MovementWaterRipple) * MAX_MOVEMENT_RIPPLES);

    const u32 allocSize = movRipVtxsOffset + movRipVtxsSize + circRipsSize + splashesSize + splashPartsSize + movRipsSize;

    mem = mmAlloc(allocSize, ALLOC_TAG_GFX_COL, ALLOC_NAME("waterfx:mem"));
    if (mem == NULL) {
        STUBBED_PRINTF("Could not allocate memory for waterfx dll\n");
        return;
    }

    sCircularRippleTris = (DLTri*) (mem + circRipTrisOffset);
    sWaterSplashTris = (DLTri*) (mem + splashTrisOffset);
    sWaterSplashPartTris = (DLTri*) (mem + splashPartTrisOffset);
    sMovementRippleTris = (DLTri*) (mem + movRipTrisOffset);
    
    sCircularRippleVerts = (Vtx*) (mem + circRipVtxsOffset);
    sWaterSplashVerts = (Vtx*) (mem + splashVtxsOffset);
    sWaterSplashPartVerts = (Vtx*) (mem + splashPartVtxsOffset);
    sMovementRippleVerts = (Vtx*) (mem + movRipVtxsOffset);
    
    mem += movRipVtxsOffset + movRipVtxsSize;
    sCircularRipples = (CircularWaterRipple*) mem;
    mem += circRipsSize;
    sWaterSplashes = (WaterSplash*) mem;
    mem += splashesSize;
    sWaterSplashParticles = (WaterSplashParticle*) mem;
    mem += splashPartsSize;
    sMovementRipples = (MovementWaterRipple*) mem;
    
    sNumCircularRipples = 0;
    sNumWaterSplashes = 0;
    sNumWaterSplashParticles = 0;
    sNumMovementRipples = 0;
    
    sCircularWaterRippleTex = tex_load_deferred(TEXTABLE_56_CircularWaterRipple);
    sWaterSplashTex = tex_load_deferred(TEXTABLE_59_WaterSplash);
    sWaterSplashParticleTex = tex_load_deferred(TEXTABLE_22_WaterSplashParticle);
    sMovementWaterRippleTex = tex_load_deferred(TEXTABLE_57_MovementWaterRipple);
    
    waterfx_init();
}

// offset: 0x160 | dtor
void waterfx_dtor(void *dll) {
    if (sCircularRippleTris != NULL) { // this frees all of the waterfx memory
        mmFree(sCircularRippleTris);
    }
    if (sCircularWaterRippleTex != NULL) {
        tex_free(sCircularWaterRippleTex);
        sCircularWaterRippleTex = NULL;
    }
    if (sWaterSplashTex != NULL) {
        tex_free(sWaterSplashTex);
        sWaterSplashTex = NULL;
    }
    if (sWaterSplashParticleTex != NULL) {
        tex_free(sWaterSplashParticleTex);
        sWaterSplashParticleTex = NULL;
    }
    if (sMovementWaterRippleTex != NULL) {
        tex_free(sMovementWaterRippleTex);
        sMovementWaterRippleTex = NULL;
    }
}

// offset: 0x24C | func: 0 | export: 6
void waterfx_init(void) {
    DLTri *tempTri;
    WaterSplashParticle *splashPart;
    DLTri *tri;
    s32 var_a2;
    CircularWaterRipple *circRipple;
    MovementWaterRipple *movRipple;
    WaterSplash *splash;
    s32 i;
    u32 targetU;
    s32 temp_var;

    tri = sCircularRippleTris;
    for (i = 0; i < MAX_CIRCULAR_RIPPLES; i++) {
        tri->v0 = 3;
        tri->v1 = 1;
        tri->v2 = 0;
        tri++;
        tri->v0 = 3;
        tri->v1 = 2;
        tri->v2 = 1;
        tri++;
        circRipple = &sCircularRipples[i];
        circRipple->x = 0.0f;
        circRipple->y = 0.0f;
        circRipple->z = 0.0f;
        circRipple->unkC = 0.0f;
        circRipple->scale = 0.01f;
        circRipple->alpha = 0;
    }
    for (i = 0; i < MAX_SPLASHES; i++) {
        var_a2 = 0;

        tempTri = &sWaterSplashTris[i * 12];
        tri = (DLTri *) tempTri;
        targetU = (u32)&sWaterSplashTris[i * 12 + 12];
        while ((u32)tri < targetU) {
            temp_var = var_a2 + 1;
            tri->v0 = var_a2;
            tri->v2 = var_a2 + 2;
            // @fake
            if (1) { }
            tri->v1 = temp_var;
            tri++;
            tri->v1 = var_a2 + 3;
            tri->v2 = var_a2 + 2;
            tri->v0 = temp_var;
            tri++;
            var_a2 += 2;
        }

        splash = &sWaterSplashes[i];
        splash->x = 0.0f;
        splash->y = 0.0f;
        splash->z = 0.0f;
        splash->alpha = 0;
        splash->particleCount = 0;
    }

    tri = sMovementRippleTris;
    for (i = 0; i < MAX_MOVEMENT_RIPPLES; i++) {
        tri->v0 = 3;
        tri->v1 = 1;
        tri->v2 = 0;
        tri++;
        tri->v0 = 3;
        tri->v1 = 2;
        tri->v2 = 1;
        tri++;
        movRipple = &sMovementRipples[i];
        movRipple->x = 0.0f;
        movRipple->y = 0.0f;
        movRipple->z = 0.0f;
        movRipple->unkC = 0.0f;
        movRipple->scale = 0.01f;
        movRipple->alpha = 0;
        movRipple->yaw = 0;
    }

    tri = sWaterSplashPartTris;
    for (i = 0; i < MAX_SPLASH_PARTICLES; i++) {
        tri->v0 = 0;
        tri->v1 = 1;
        tri->v2 = 3;
        tri++;
        tri->v0 = 0;
        tri->v1 = 3;
        tri->v2 = 2;
        tri++;

        splashPart = &sWaterSplashParticles[i];
        splashPart->splashIdx = -1;
        splashPart->xVel = 0.0f;
        splashPart->zVel = 0.0f;
        splashPart->unk10 = 0;
        splashPart->speed = 0.0f;
        splashPart->yVel = 0.0f;
    }
}

// offset: 0x564 | func: 1 | export: 1
void waterfx_func_564(SRT *transform, u16 arg1, Vec3f *arg2, DLL27_Data *arg3, f32 speed) {
    f32 temp_fs1;
    f32 temp_fs2;
    s32 i;

    i = 0;
    while (arg1) {
        if (arg1 & 1) {
            temp_fs1 = arg2[i].x;
            temp_fs2 = arg2[i].z;
            if (arg3->underwaterDist < 10.0f) {
                waterfx_spawn_splash(temp_fs1, transform->transl.y + arg3->underwaterDist, temp_fs2, 0.0f);
            }
            waterfx_spawn_circular_ripple(temp_fs1, transform->transl.y + arg3->underwaterDist, temp_fs2, transform->yaw, 0.0f, 3);
        }
        arg1 >>= 1;
        i += 1;
    }
    if (speed > 0.01f) {
        waterfx_spawn_movement_ripple(transform->transl.x, transform->transl.y + arg3->underwaterDist, transform->transl.z, transform->yaw, 0.0f);
    }
}

// offset: 0x6E8 | func: 2 | export: 0
void waterfx_tick(void) {
    s32 i;
    f32 temp_fv1;
    f32 transparency;
    s16 yMove;
    s16 alpha;
    s16 xMove;
    s16 zMove;
    s32 j;
    Vtx* vtx;
    Vtx* temp_a1_2;
    WaterSplash* splash;
    CircularWaterRipple* circRipple;
    MovementWaterRipple* movRipple;
    WaterSplashParticle* part;
    WaterSplash *temp2;
    f32 temp;

    for (i = 0; i < MAX_CIRCULAR_RIPPLES; i++) {
        circRipple = &sCircularRipples[i];
        if (circRipple->alpha != 0) {
            circRipple->scale += 0.0007f * gUpdateRateF;
            circRipple->alpha -= (gUpdateRate * circRipple->decayRate);
            if (circRipple->alpha < 0) {
                circRipple->alpha = 0;
                sNumCircularRipples -= 1;
            }
        }
    }

    for (i = 0; i < MAX_SPLASHES; i++) {
        splash = &sWaterSplashes[i];
        if (splash->alpha != 0) {
            vtx = &sWaterSplashVerts[i * 14];
            transparency = vtx[1].v.ob[1] / 400.0f;
            if (transparency < 0.0f) {
                transparency = 0.0f;
            } else if (transparency > 1.0f) {
                transparency = 1.0f;
            } 
            j = 0;
            while (j < 14) {
                if (j & 1) {
                    vtx[j].v.ob[0] += (s16) (splash->unkC[(j % 12) >> 1] * gUpdateRateF * 100.0f);
                    vtx[j].v.ob[1] += (s16) (splash->unk24[(j % 12) >> 1] * gUpdateRateF * 100.0f);
                    vtx[j].v.ob[2] += (s16) (splash->unk3C[(j % 12) >> 1] * gUpdateRateF * 100.0f);
                }
                alpha = 255.0f - (transparency * 255.0f);
                vtx[j].v.cn[3] = alpha;
                j++;
            }
            if (vtx[1].v.ob[1] > 400) {
                splash->alpha = 0;
                sNumWaterSplashes -= 1;
            }
        }
    }

    for (i = 0; i < MAX_MOVEMENT_RIPPLES; i++) {
        movRipple = &sMovementRipples[i];
        if (movRipple->alpha != 0) {
            movRipple->scale += 0.004f * gUpdateRateF;
            movRipple->alpha -= gUpdateRate * 5;
            if (movRipple->alpha < 0) {
                movRipple->alpha = 0;
                sNumMovementRipples -= 1;
            }
            if (movRipple->hide == FALSE) {
                movRipple->hide = TRUE;
            }
        }
    }

    for (i = 0; i < MAX_SPLASH_PARTICLES; i++) {
        part = &sWaterSplashParticles[i];
        if (part->splashIdx != -1) {
            splash = &sWaterSplashes[part->splashIdx];
            vtx = &sWaterSplashPartVerts[i * 4];

            temp = 100.0f * gUpdateRateF;
            temp_fv1 = part->speed * temp;
            xMove = (part->xVel * temp_fv1);
            yMove = (part->yVel * temp);
            zMove = (part->zVel * temp_fv1);

            vtx->v.ob[0] += xMove;
            vtx->v.ob[1] += yMove;
            vtx->v.ob[2] += zMove;
            vtx += 3;
            vtx[-2].v.ob[0] += xMove;
            vtx[-2].v.ob[1] += yMove;
            vtx[-2].v.ob[2] += zMove;
            vtx[-1].v.ob[0] += xMove;
            vtx[-1].v.ob[1] += yMove;
            vtx[-1].v.ob[2] += zMove;
            vtx->v.ob[0] += xMove;
            vtx->v.ob[1] += yMove;
            vtx->v.ob[2] += zMove;

            part->yVel += -0.025f * gUpdateRateF;
            if (splash->alpha == 0) {
                if (vtx->v.ob[1] < 0) {
                    part->splashIdx = -1;
                    sNumWaterSplashParticles -= 1;
                    splash->particleCount--;
                    waterfx_spawn_circular_ripple((vtx->v.ob[0] / 100.0f) + splash->x, splash->y, (vtx->v.ob[2] / 100.0f) + splash->z, 0, 0.0f, 4);
                }
            } else {
                temp_a1_2 = &sWaterSplashVerts[part->splashIdx * 14];
                part->unk10 = (0xFF - temp_a1_2[1].v.cn[3]);
            }
        }
    }
}


// offset: 0xC7C | func: 3 | export: 2
void waterfx_print(Gfx** gdl, Mtx** mtxs) {
    s32 i;
    SRT srt;
    CircularWaterRipple* circRipple;
    WaterSplash* splash;
    MovementWaterRipple *movRipple;
    WaterSplashParticle* splashPart;

    if ((sNumCircularRipples != 0) || (sNumMovementRipples != 0) || (sNumWaterSplashes != 0) || (sNumWaterSplashParticles != 0)) {
        gSPLoadGeometryMode(*gdl, G_SHADE | G_ZBUFFER | G_SHADING_SMOOTH);
        dl_apply_geometry_mode(gdl);
        gDPSetCombineLERP(*gdl, 0, 0, 0, TEXEL0, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED, COMBINED, 0, SHADE, 0);
        dl_apply_combine(gdl);
        gDPSetOtherMode(*gdl, 
            G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE, 
            G_AC_NONE | G_ZS_PIXEL | G_RM_NOOP | G_RM_ZB_CLD_SURF2);
        dl_apply_other_mode(gdl);

        // Draw circular water ripples
        if (sNumCircularRipples != 0) {
            gSPDisplayList((*gdl)++, OS_PHYSICAL_TO_K0(sCircularWaterRippleTex->gdl));
        }
        for (i = 0; i < MAX_CIRCULAR_RIPPLES; i++) {
            if (sCircularRipples[i].alpha != 0) {
                circRipple = &sCircularRipples[i];
                dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, circRipple->alpha);
                srt.transl.x = circRipple->x;
                srt.transl.y = circRipple->y;
                srt.transl.z = circRipple->z;
                srt.scale = circRipple->scale;
                srt.yaw = circRipple->yaw;
                srt.roll = 0;
                srt.pitch = 0;
                camera_setup_object_srt_matrix(gdl, mtxs, &srt, 1.0f, 0.0f, NULL);
                gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(&sCircularRippleVerts[i << 2]), 4, 0);
                dl_triangles(gdl, &sCircularRippleTris[i << 1], 2);
            }
        }

        // Draw 3D water splashes
        if (sNumWaterSplashes != 0) {
            gSPDisplayList((*gdl)++, OS_PHYSICAL_TO_K0(sWaterSplashTex->gdl));
        }
        for (i = 0; i < MAX_SPLASHES; i++) {
            if (sWaterSplashes[i].alpha != 0) {
                splash = &sWaterSplashes[i];
                dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, splash->alpha);
                srt.transl.x = splash->x;
                srt.transl.y = splash->y;
                srt.transl.z = splash->z;
                srt.scale = 0.01f;
                srt.yaw = 0;
                srt.roll = 0;
                srt.pitch = 0;
                camera_setup_object_srt_matrix(gdl, mtxs, &srt, 1.0f, 0.0f, NULL);
                gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(&sWaterSplashVerts[i * 14]), 14, 0);
                dl_triangles(gdl, &sWaterSplashTris[i * 12], 12);
            }
        }

        // Draw water splash particles
        if (sNumWaterSplashParticles != 0) {
            gSPDisplayList((*gdl)++, OS_PHYSICAL_TO_K0(sWaterSplashParticleTex->gdl));
            dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, 0xB4);
        }
        for (i = 0; i < MAX_SPLASH_PARTICLES; i++) {
            if (sWaterSplashParticles[i].splashIdx != -1 && sWaterSplashParticles[i].unk10 != 0) {
                splashPart = &sWaterSplashParticles[i];
                srt.transl.x = sWaterSplashes[splashPart->splashIdx].x;
                srt.transl.y = sWaterSplashes[splashPart->splashIdx].y;
                srt.transl.z = sWaterSplashes[splashPart->splashIdx].z;
                srt.scale = 0.01f;
                srt.yaw = 0;
                srt.roll = 0;
                srt.pitch = 0;
                camera_setup_object_srt_matrix(gdl, mtxs, &srt, 1.0f, 0.0f, NULL);
                gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(&sWaterSplashPartVerts[i << 2]), 4, 0);
                dl_triangles(gdl, &sWaterSplashPartTris[i << 1], 2);
            }
        }

        // Draw movement water ripples
        if (sNumMovementRipples != 0) {
            gSPDisplayList((*gdl)++, OS_PHYSICAL_TO_K0(sMovementWaterRippleTex->gdl));
        }
        for (i = 0; i < MAX_MOVEMENT_RIPPLES; i++) {
            if (sMovementRipples[i].alpha != 0 && sMovementRipples[i].hide == FALSE) {
                movRipple = &sMovementRipples[i];
                dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, movRipple->alpha);
                srt.transl.x = movRipple->x;
                srt.transl.y = movRipple->y;
                srt.transl.z = movRipple->z;
                srt.scale = movRipple->scale;
                srt.yaw = movRipple->yaw;
                srt.roll = 0;
                srt.pitch = 0;
                camera_setup_object_srt_matrix(gdl, mtxs, &srt, 1.0f, 0.0f, NULL);
                gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(&sMovementRippleVerts[i << 2]), 4, 0);
                dl_triangles(gdl, &sMovementRippleTris[i << 1], 2);
            }
        }
        tex_render_reset();
    }
}

// offset: 0x135C | func: 4
static s32 waterfx_make_splash_particles(WaterSplash *splash, s32 splashIdx) {
    WaterSplashParticle *part;
    s32 _pad;
    s32 count;
    f32 temp_fs0;
    f32 temp_fs0_2;
    f32 temp_fs1;
    f32 lateralMag;
    f32 temp_fv0;
    s32 i;
    s32 idx;
    Vtx *vtx;
    s32 sectorSize[1]; // this needs to be an array for some reason
    s16 angle;
    s16 angleDeviation;

    count = rand_next(2, 4);
    if ((count + sNumWaterSplashParticles) > MAX_SPLASH_PARTICLES) {
        count = MAX_SPLASH_PARTICLES - sNumWaterSplashParticles;
    }
    angle = 0;
    if (count != 0) {
        for (i = 0; i < count; i++) {
            for (idx = 0; idx < MAX_SPLASH_PARTICLES && sWaterSplashParticles[idx].splashIdx != -1; idx++) {}

            if (idx < MAX_SPLASH_PARTICLES) {
                part = &sWaterSplashParticles[idx];
                sectorSize[0] = (s16)(0xFFFF / count);
                angleDeviation = rand_next(-0x7D0, 0x7D0);
                temp_fs0 = fsin16_precise(angle + angleDeviation);
                temp_fv0 = fcos16_precise(angle + angleDeviation);
                part->xVel = 4.0f * temp_fv0;
                part->zVel = 4.0f * temp_fs0;
                lateralMag = SQ(part->xVel) + SQ(part->zVel);
                if (lateralMag > 0.0f) {
                    lateralMag = 1.0f / lateralMag;
                    part->xVel *= lateralMag;
                    part->zVel *= lateralMag;
                }
                part->speed = rand_next(600, 800) * 0.001f;
                part->yVel = rand_next(500, 600) * 0.001f;
                part->unk10 = 0;
                part->splashIdx = splashIdx;

                temp_fs0 *= 100.0f;
                temp_fv0 *= 100.0f;

                vtx = &sWaterSplashPartVerts[idx * 4];
                vtx->v.ob[1] = -300;
                vtx->v.cn[3] = 0xFF;
                vtx->v.tc[0] = qu105(31);
                vtx->v.tc[1] = qu105(31);
                vtx->v.ob[0] = -2.0f * temp_fv0;
                vtx->v.ob[2] = -2.0f * temp_fs0;

                vtx[1].v.ob[1] = 300;
                vtx[1].v.cn[3] = 0xFF;
                vtx[1].v.tc[0] = qu105(31);
                vtx[1].v.tc[1] = qu105(0);
                vtx[1].v.ob[0] = vtx->v.ob[0];
                vtx[1].v.ob[2] = vtx->v.ob[2];
                vtx += 3;

                vtx[-1].v.ob[1] = -300;
                vtx[-1].v.cn[3] = 0xFF;
                vtx[-1].v.tc[0] = qu105(0);
                vtx[-1].v.tc[1] = qu105(31);
                vtx[-1].v.ob[0] = 4 * temp_fv0;
                vtx[-1].v.ob[2] = 4 * temp_fs0;

                vtx[0].v.ob[1] = 300;
                vtx[0].v.tc[0] = qu105(0);
                vtx[0].v.tc[1] = qu105(0);
                vtx[0].v.cn[3] = 0xFF;
                vtx[0].v.ob[0] = vtx[-1].v.ob[0];
                vtx[0].v.ob[2] = vtx[-1].v.ob[2];
                sNumWaterSplashParticles += 1;
                angle += sectorSize[0];
            }
        }
    }
    return count;
}

// offset: 0x174C | func: 5 | export: 3
void waterfx_spawn_splash(f32 x, f32 y, f32 z, f32 size) {
    WaterSplash* splash;
    f32 temp_fs1;
    f32 temp_ft4;
    f32 temp_fv0;
    f32 temp_fv0_2;
    s16 s;
    s16 angle;
    s32 idx;
    Vtx* vtx;
    s32 i;

    if (size == 0.0f) {
        size = 4.0f;
    }
    for (idx = 0; idx < MAX_SPLASHES && sWaterSplashes[idx].particleCount != 0; idx++) {}
    if (idx >= MAX_SPLASHES) {
        return;
    }

    splash = &sWaterSplashes[idx];
    vtx = &sWaterSplashVerts[idx * 14];
    temp_fs1 = size * 100.0f;
    vtx->v.ob[0] = temp_fs1;
    vtx->v.ob[1] = 0;
    vtx->v.ob[2] = 0;
    vtx->v.cn[0] = 0xFF;
    vtx->v.cn[1] = 0;
    vtx->v.cn[2] = 0;
    vtx->v.cn[3] = 0xFF;
    vtx->v.tc[0] = qu105(0);
    vtx->v.tc[1] = qu105(0);
    vtx++;
    vtx->v.ob[0] = temp_fs1;
    vtx->v.ob[1] = 0;
    vtx->v.ob[2] = 0;
    vtx->v.cn[0] = 0xFF;
    vtx->v.cn[1] = 0;
    vtx->v.cn[2] = 0;
    vtx->v.cn[3] = 0xFF;
    vtx->v.tc[0] = qu105(0);
    vtx->v.tc[1] = qu105(29);
    vtx++;
    splash->unkC[0] = 4.0f;
    splash->unk24[0] = 4.0f;
    splash->unk3C[0] = 0.0f;
    temp_ft4 = 2.0f * (splash->unkC[0] * splash->unk24[0]);
    angle = 0x2AAA;
    if (temp_ft4 > 0.0f) {
        temp_ft4 = 1.0f / temp_ft4;
        splash->unkC[0] *= temp_ft4;
        splash->unk24[0] *= temp_ft4;
    }
    s = qu105(0);
    i = 1;
    do {
        temp_fv0 = fcos16_precise(angle);
        temp_fv0_2 = fsin16_precise(angle);
        splash->unkC[i] = 4.0f * temp_fv0;
        splash->unk24[i] = 4.0f;
        splash->unk3C[i] = 4.0f * temp_fv0_2;
        temp_ft4 = SQ(splash->unkC[i]) + SQ(splash->unk24[i]) + SQ(splash->unk3C[i]);
        if (temp_ft4 > 0.0f) {
            temp_ft4 = 1.0f / temp_ft4;
            splash->unkC[i] *= temp_ft4;
            splash->unk24[i] *= temp_ft4;
            splash->unk3C[i] *= temp_ft4;
        }
        s += qu105(96);
        vtx->v.ob[1] = 0;
        vtx->v.cn[3] = 0xFF;
        vtx->v.tc[0] = s;
        vtx->v.tc[1] = qu105(0);
        vtx[1].v.ob[1] = 0;
        vtx[1].v.cn[3] = 0xFF;
        vtx[1].v.tc[0] = s;
        if (0) { } // @fake
        vtx[1].v.tc[1] = qu105(29);
        vtx->v.ob[0] = temp_fs1 * temp_fv0;
        vtx->v.ob[2] = temp_fs1 * temp_fv0_2;
        vtx[1].v.ob[0] = vtx->v.ob[0];
        vtx[1].v.ob[2] = vtx->v.ob[2];
        angle += 0x2AAA;
        vtx += 2;
        i++;
    } while (i != 6);
    s += qu105(96);
    vtx->v.ob[0] = temp_fs1;
    vtx->v.ob[2] = 0;
    vtx->v.ob[1] = 0;
    vtx->v.cn[3] = 0xFF;
    vtx->v.cn[2] = 0;
    vtx->v.cn[1] = 0;
    vtx->v.cn[0] = 0xFF;
    vtx->v.tc[0] = s;
    vtx->v.tc[1] = qu105(0);
    vtx++;
    vtx->v.ob[0] = temp_fs1;
    vtx->v.ob[2] = 0;
    vtx->v.ob[1] = 0;
    vtx->v.cn[3] = 0xFF;
    vtx->v.cn[2] = 0;
    vtx->v.cn[1] = 0;
    vtx->v.cn[0] = 0xFF;
    vtx->v.tc[0] = s;
    vtx->v.tc[1] = qu105(29);
    splash->alpha = 0xFF;
    splash->x = x;
    splash->y = y;
    splash->z = z;
    sNumWaterSplashes += 1;
    splash->particleCount = waterfx_make_splash_particles(&sWaterSplashes[idx], idx);
}

// offset: 0x1B28 | func: 6 | export: 5
void waterfx_spawn_movement_ripple(f32 x, f32 y, f32 z, s16 yaw, f32 arg4) {
    MovementWaterRipple *ripple;
    s32 idx;
    Vtx *vtx;

    for (idx = 0; idx < MAX_MOVEMENT_RIPPLES && sMovementRipples[idx].alpha != 0; idx++) {}
    if (idx >= MAX_MOVEMENT_RIPPLES) {
        return;
    }

    vtx = &sMovementRippleVerts[idx * 4];
    vtx->v.ob[0] = -200;
    vtx->v.ob[1] = 0;
    vtx->v.ob[2] = 400;
    vtx->v.cn[3] = -1;
    vtx->v.tc[0] = qu105(0);
    vtx->v.tc[1] = qu105(0);
    vtx++;
    vtx->v.ob[0] = -200;
    vtx->v.ob[1] = 0;
    vtx->v.ob[2] = -200;
    vtx->v.cn[3] = -1;
    vtx->v.tc[0] = qu105(0);
    vtx->v.tc[1] = qu105(31);
    vtx++;
    vtx->v.ob[0] = 200;
    vtx->v.ob[1] = 0;
    vtx->v.ob[2] = -200;
    vtx->v.cn[3] = -1;
    vtx->v.tc[0] = qu105(63);
    vtx->v.tc[1] = qu105(31);
    vtx++;
    vtx->v.ob[0] = 200;
    vtx->v.ob[1] = 0;
    vtx->v.ob[2] = 400;
    vtx->v.cn[3] = -1;
    vtx->v.tc[0] = qu105(63);
    vtx->v.tc[1] = qu105(0);
    ripple = &sMovementRipples[idx];
    ripple->x = x;
    ripple->y = y;
    ripple->z = z;
    ripple->unkC = arg4;
    ripple->scale = 0.01f;
    ripple->alpha = 128;
    ripple->yaw = yaw;
    ripple->hide = FALSE;
    sNumMovementRipples += 1;
}

// offset: 0x1C88 | func: 7 | export: 7
void waterfx_set_circular_ripple_scale(s32 useDefault, f32 scale) {
    if (useDefault != 0) {
        sCircularRippleScale = 0.01f;
    } else {
        sCircularRippleScale = scale;
    }
}

// offset: 0x1CC8 | func: 8 | export: 4
void waterfx_spawn_circular_ripple(f32 x, f32 y, f32 z, s16 yaw, f32 arg4, s32 decayRate) {
    s32 idx;
    s32 vtxIdx;
    Vtx* vtx;

    for (idx = 0; idx < MAX_CIRCULAR_RIPPLES && sCircularRipples[idx].alpha != 0; idx++) {}
    if (idx >= MAX_CIRCULAR_RIPPLES) {
        return;
    }

    vtxIdx = idx * 4;
    vtx = &sCircularRippleVerts[vtxIdx];
    vtx->v.ob[0] = -200;
    vtx->v.ob[1] = 0;
    vtx->v.ob[2] = 200;
    vtx->v.cn[3] = 0xFF;
    vtx->v.tc[0] = qu105(0);
    vtx->v.tc[1] = qu105(0);
    vtx = &sCircularRippleVerts[vtxIdx + 1];
    vtx->v.ob[0] = -200;
    vtx->v.ob[1] = 0;
    vtx->v.ob[2] = -200;
    vtx->v.cn[3] = 0xFF;
    vtx->v.tc[0] = qu105(0);
    vtx->v.tc[1] = qu105(63);
    vtx = &sCircularRippleVerts[vtxIdx + 2];
    vtx->v.ob[0] = 200;
    vtx->v.ob[1] = 0;
    vtx->v.ob[2] = -200;
    vtx->v.cn[3] = 0xFF;
    vtx->v.tc[0] = qu105(63);
    vtx->v.tc[1] = qu105(63);
    vtx = &sCircularRippleVerts[vtxIdx + 3];
    vtx->v.ob[0] = 200;
    vtx->v.ob[1] = 0;
    vtx->v.ob[2] = 200;
    vtx->v.cn[3] = 0xFF;
    vtx->v.tc[0] = qu105(63);
    vtx->v.tc[1] = qu105(0);
    sCircularRipples[idx].unkC = arg4;
    sCircularRipples[idx].alpha = 0xFF;
    sCircularRipples[idx].x = x;
    sCircularRipples[idx].y = y;
    sCircularRipples[idx].z = z;
    sCircularRipples[idx].yaw = yaw;
    sCircularRipples[idx].scale = sCircularRippleScale;
    sCircularRipples[idx].decayRate = decayRate;
    sNumCircularRipples += 1;
}
