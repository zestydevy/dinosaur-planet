#include "dlls/engine/29_gplay.h"
#include "sys/lfx.h"
#include "sys/lighting.h"
#include "dll.h"

/**
 * @file "Sky Lights"
 * Custom sky light gradients that models can opt into.
 * Does not manage the base sky light set by newday.
 *
 * Skylights:
 * [0] - pallete 14
 * [1] - pallete 13 (used by player, sidekicks, and more)
 * [2] - pallete 12
 * [3] - pallete 11
 */

#define NUM_LIGHTS 4

/*0x0*/ static s32 sTransitionTimer[NUM_LIGHTS] = {0};
/*0x10*/ static s32 sMode[NUM_LIGHTS] = {0};

// size: 0x18
typedef struct {
    u8 _unk0[0x8 - 0x0];
    f32 tValue;
    f32 rate;
    u8 r1;
    u8 g1;
    u8 b1;
    u8 r2;
    u8 g2;
    u8 b2;
    u8 _unk16[0x18 - 0x16];
} SkyLight;

/*0x0*/ static SkyLight sLights[NUM_LIGHTS * 2];

// offset: 0x0 | ctor
void skylight_ctor(void* dll) { }

// offset: 0xC | dtor
void skylight_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void skylight_Init(void) {
    sTransitionTimer[0] = 0;
    sTransitionTimer[1] = 0;
    sTransitionTimer[2] = 0;
    sTransitionTimer[3] = 0;
    sMode[0] = 0;
    sMode[1] = 0;
    sMode[2] = 0;
    sMode[3] = 0;
    sLights[0].r1 = 100;
    sLights[0].g1 = 100;
    sLights[0].b1 = 100;
    sLights[0].r2 = 0;
    sLights[0].g2 = 0;
    sLights[0].b2 = 0;
    sLights[1].r1 = 100;
    sLights[1].g1 = 100;
    sLights[1].b1 = 100;
    sLights[1].r2 = 220;
    sLights[1].g2 = 220;
    sLights[1].b2 = 250;
    sLights[2].r1 = 0;
    sLights[2].g1 = 0;
    sLights[2].b1 = 0;
    sLights[2].r2 = 0;
    sLights[2].g2 = 0;
    sLights[2].b2 = 0;
    sLights[3].r1 = 100;
    sLights[3].g1 = 100;
    sLights[3].b1 = 100;
    sLights[3].r2 = 250;
    sLights[3].g2 = 220;
    sLights[3].b2 = 220;
    sLights[4].r1 = 0;
    sLights[4].g1 = 0;
    sLights[4].b1 = 0;
    sLights[4].r2 = 0;
    sLights[4].g2 = 0;
    sLights[4].b2 = 0;
    sLights[5].r1 = 100;
    sLights[5].g1 = 100;
    sLights[5].b1 = 100;
    sLights[5].r2 = 250;
    sLights[5].g2 = 220;
    sLights[5].b2 = 220;
    sLights[6].r1 = 0;
    sLights[6].g1 = 0;
    sLights[6].b1 = 0;
    sLights[6].r2 = 0;
    sLights[6].g2 = 0;
    sLights[6].b2 = 0;
    sLights[7].r1 = 100;
    sLights[7].g1 = 100;
    sLights[7].b1 = 100;
    sLights[7].r2 = 250;
    sLights[7].g2 = 220;
    sLights[7].b2 = 220;
    lightSetModLightPalette2(/*c1*/sLights[1].r1, sLights[1].g1, sLights[1].b1, /*c2*/sLights[1].r2, sLights[1].g2, sLights[1].b2, 14, 0);
    lightSetModLightPalette2(/*c1*/sLights[3].r1, sLights[3].g1, sLights[3].b1, /*c2*/sLights[3].r2, sLights[3].g2, sLights[3].b2, 13, 0);
    lightSetModLightPalette2(/*c1*/sLights[5].r1, sLights[5].g1, sLights[5].b1, /*c2*/sLights[5].r2, sLights[5].g2, sLights[5].b2, 12, 0);
    lightSetModLightPalette2(/*c1*/sLights[7].r1, sLights[7].g1, sLights[7].b1, /*c2*/sLights[7].r2, sLights[7].g2, sLights[7].b2, 11, 0);
    lightSetModLightPalette2(/*c1*/0, 0, 50,                                    /*c2*/240, 240, 255,                               10, 0);
}

// offset: 0x264 | func: 1 | export: 1
void skylight_DoAction(LightAction* action) {
    PlayerLightActions* lactions;
    SkyLight* light;
    s32 idx;

    lactions = gDLL_29_Gplay->vtbl->get_current_player_lactions();
    // index 0
    idx = action->slot * 2;
    light = &sLights[idx];
    if (light->tValue != 1.0f) {
        // We're interupting a transition, set the start color to what that transition is currently at
        light->r1 = (u8) (((f32) ((light + 1)->r1 - light->r1) * light->tValue) + light->r1);
        light->g1 = (u8) (((f32) ((light + 1)->g1 - light->g1) * light->tValue) + light->g1);
        light->b1 = (u8) (((f32) ((light + 1)->b1 - light->b1) * light->tValue) + light->b1);
        light->r2 = (u8) (((f32) ((light + 1)->r2 - light->r2) * light->tValue) + light->r2);
        light->g2 = (u8) (((f32) ((light + 1)->g2 - light->g2) * light->tValue) + light->g2);
        light->b2 = (u8) (((f32) ((light + 1)->b2 - light->b2) * light->tValue) + light->b2);
    }
    // index 1
    // Set target color
    idx = action->slot * 2 + 1;
    light = &sLights[idx];
    light->r1 = action->r1;
    light->g1 = action->g1;
    light->b1 = action->b1;
    light->r2 = action->r2;
    light->g2 = action->g2;
    light->b2 = action->b2;
    sTransitionTimer[action->slot] = action->timer;
    if (sTransitionTimer[action->slot] <= 0) {
        sTransitionTimer[action->slot] = 1;
    }
    if (action->flags3 & 1) {
        sMode[action->slot] = 1;
    } else {
        sMode[action->slot] = 0;
    }
    idx = action->slot * 2;
    light = &sLights[idx];
    light->tValue = 0.0f;
    light->rate = (f32) (1.0f / (f32) sTransitionTimer[action->slot]);
    lactions->skylights[action->slot] = (s16) action->id - 1;
}

// offset: 0x8A4 | func: 2 | export: 2
void skylight_Tick(void) {
    struct {
        u8 c1[3];
        u8 c2[3];
    } c;
    s32 i;

    for (i = 0; i < NUM_LIGHTS; i++){
        if (sMode[i] != 0) {
            if (sTransitionTimer[i] > 0) {
                // Lerp i -> i + 1
                c.c1[0] = sLights[i * 2].r1 + (sLights[i * 2].tValue * (sLights[i * 2 + 1].r1 - sLights[i * 2].r1));
                c.c1[1] = sLights[i * 2].g1 + (sLights[i * 2].tValue * (sLights[i * 2 + 1].g1 - sLights[i * 2].g1));
                c.c1[2] = sLights[i * 2].b1 + (sLights[i * 2].tValue * (sLights[i * 2 + 1].b1 - sLights[i * 2].b1));
                c.c2[0] = sLights[i * 2].r2 + (sLights[i * 2].tValue * (sLights[i * 2 + 1].r2 - sLights[i * 2].r2));
                c.c2[1] = sLights[i * 2].g2 + (sLights[i * 2].tValue * (sLights[i * 2 + 1].g2 - sLights[i * 2].g2));
                c.c2[2] = sLights[i * 2].b2 + (sLights[i * 2].tValue * (sLights[i * 2 + 1].b2 - sLights[i * 2].b2));
                sLights[i * 2].tValue += sLights[i * 2].rate;
                sTransitionTimer[i]--;
                if (sTransitionTimer[i] <= 0) {
                    sLights[i * 2].r1 = c.c1[0];
                    sLights[i * 2].g1 = c.c1[1];
                    sLights[i * 2].b1 = c.c1[2];
                    sLights[i * 2].r2 = c.c2[0];
                    sLights[i * 2].g2 = c.c2[1];
                    sLights[i * 2].b2 = c.c2[2];
                    sTransitionTimer[i] = 0;
                    lightSetModLightPalette2(
                        sLights[i * 2].r1, sLights[i * 2].g1, sLights[i * 2].b1, 
                        sLights[i * 2].r2, sLights[i * 2].g2, sLights[i * 2].b2, 
                        14 - i, 
                        255);
                } else {
                    lightSetModLightPalette2(
                        c.c1[0], c.c1[1], c.c1[2], 
                        c.c2[0], c.c2[1], c.c2[2], 
                        14 - i, 
                        sLights[i * 2].tValue * 255.0f);
                }
            }
        } else {
            if (sTransitionTimer[i] > 0) {
                // Lerp i -> i + 1
                c.c1[0] = sLights[i * 2].r1 + (sLights[i * 2].tValue * (sLights[i * 2 + 1].r1 - sLights[i * 2].r1));
                c.c1[1] = sLights[i * 2].g1 + (sLights[i * 2].tValue * (sLights[i * 2 + 1].g1 - sLights[i * 2].g1));
                c.c1[2] = sLights[i * 2].b1 + (sLights[i * 2].tValue * (sLights[i * 2 + 1].b1 - sLights[i * 2].b1));
                c.c2[0] = sLights[i * 2].r2 + (sLights[i * 2].tValue * (sLights[i * 2 + 1].r2 - sLights[i * 2].r2));
                c.c2[1] = sLights[i * 2].g2 + (sLights[i * 2].tValue * (sLights[i * 2 + 1].g2 - sLights[i * 2].g2));
                c.c2[2] = sLights[i * 2].b2 + (sLights[i * 2].tValue * (sLights[i * 2 + 1].b2 - sLights[i * 2].b2));
                sLights[i * 2].tValue += sLights[i * 2].rate;
                sTransitionTimer[i]--;
                if (sTransitionTimer[i] <= 0) {
                    sLights[i * 2].r1 = c.c1[0];
                    sLights[i * 2].g1 = c.c1[1];
                    sLights[i * 2].b1 = c.c1[2];
                    sLights[i * 2].r2 = c.c2[0];
                    sLights[i * 2].g2 = c.c2[1];
                    sLights[i * 2].b2 = c.c2[2];
                    sTransitionTimer[i] = 0;
                }
                lightSetModLightPalette2(
                    c.c1[0], c.c1[1], c.c1[2], 
                    c.c2[0], c.c2[1], c.c2[2], 
                    14 - i, 
                    0);
            }
        }
    }
}

// offset: 0x14D4 | func: 3 | export: 3
void skylight_Func3(s32 arg0) {

}

// offset: 0x14E0 | func: 4 | export: 4
void skylight_Func4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {

}
