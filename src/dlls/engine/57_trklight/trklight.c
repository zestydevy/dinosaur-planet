#include "PR/ultratypes.h"
#include "dlls/engine/29_gplay.h"
#include "sys/lfx.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/rand.h"
#include "dll.h"

/** 
 * @file "Track Light" DLL. 
 * Manages dynamic global lights that track geometry (block shapes) can opt into. 
 */

// size: 0x28
typedef struct {
    u8 _unk0[0x4 - 0x0];
    s32 timer;
    u8 _unk8[0xC - 0x8];
    f32 time;
    f32 rate;
    s16 type; // LightActionType
    s16 min;
    s16 max;
    u8 flags;
    u8 r1;
    u8 g1;
    u8 b1;
    u8 r2;
    u8 g2;
    u8 b2;
    u8 unk21;
    u8 unk22;
    u8 speed;
    u8 inUse;
    u8 slot;
    u8 _unk26[0x28 - 0x26];
} TrackLight;

typedef struct {
    s32 r;
    s32 g;
    s32 b;
} Rgb32;

/*0x0*/ static Rgb32 sCurrColor[] = {
    {255, 255, 255}, 
    {255, 255, 255}
};
/*0x18*/ static f32 sTValue[] = {
    1.0f, 1.0f
};

/*0x0*/ static Rgb32 sPrevColor[2];
/*0x18*/ static Rgb32 sTargColor[2];
/*0x30*/ static TrackLight sLights[2];

static void trklight_initSlot(LightAction* action);
static void trklight_updateSlot(LightAction* action);

// offset: 0x0 | ctor
void trklight_ctor(void* dll) { }

// offset: 0xC | dtor
void trklight_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void trklight_Init(void) {
    sCurrColor[0].r = 0;
    sCurrColor[0].g = 0;
    sCurrColor[0].b = 0;
    sCurrColor[1].r = 0;
    sCurrColor[1].g = 0;
    sCurrColor[1].b = 0;
    sLights[0].inUse = FALSE;
    sLights[1].inUse = FALSE;
    sTValue[0] = 1.0f;
    sTValue[1] = 1.0f;
}

// offset: 0x74 | func: 1 | export: 1
void trklight_DoAction(LightAction* laction) {
    PlayerLightActions* lactions;

    lactions = gDLL_29_Gplay->vtbl->get_current_player_lactions();
    if (laction->slot >= 2) {
        // @bug: stores the action index out of bounds
        lactions->unk8[laction->slot] = laction->id - 1;
        laction->slot = 1;
    }
    if (laction->flags & 1) {
        // Set light
        if (sLights[laction->slot].inUse == FALSE) {
            trklight_initSlot(laction);
        } else {
            trklight_updateSlot(laction);
        }
        // @bug? active lights aren't stored in the gplay lactions?
    } else if (laction->flags & 2) {
        // Free light (doesn't reset the color?)
        sLights[laction->slot].inUse = FALSE;
        lactions->unk8[laction->slot] = laction->id - 1;
    }
}

// offset: 0x190 | func: 2 | export: 2
void trklight_GetLights(s32* r1, s32* g1, s32* b1, s32* r2, s32* g2, s32* b2) {
    *r1 = sCurrColor[0].r;
    *g1 = sCurrColor[0].g;
    *b1 = sCurrColor[0].b;
    *r2 = sCurrColor[1].r;
    *g2 = sCurrColor[1].g;
    *b2 = sCurrColor[1].b;
}

// offset: 0x1E4 | func: 3 | export: 3
void trklight_Tick(void) {
    TrackLight* light;
    f32 t;
    f32 temp_fv1;
    f32 var_fa0;
    s32 _tempR;
    f32 multiplier = 1.0f;
    s16 _tempRgb[3];
    s16 var_ft1;
    s16 i;
    s16 counter;
    s16 r;
    s16 g;
    s16 b;
    s32 _tempG;
    s32 _tempB;

    counter = 2;
    i = 0;
    do {
        light = &sLights[i];
        if (light->flags & 8) {
            // Animated light
            light->time += 180.0f * light->rate * (f32) gUpdateRate;
            if (light->time > 180.0f) {
                light->time = 0.0f;
                if (light->unk21 == 0) {
                    light->unk22 = (u8) ((f32) (light->max - mathRnd(light->min, light->max)) * 0.5f);
                } else {
                    light->unk22 = (u8) (mathRnd(light->min, light->unk21) + light->min);
                }
            }
            var_fa0 = mathCosInterp(light->time * 182.04166f) / M_360_DEGREES_F;
            if (var_fa0 < 0.0f) {
                var_fa0 *= -1.0f;
            }
            var_ft1 = light->max - (light->unk22 * (1.0f - var_fa0));
            if (sTValue[i] < 1.0f) {
                _tempR = sPrevColor[i].r;
                _tempG = sPrevColor[i].g;
                _tempB = sPrevColor[i].b;
                // Lerp sPrevColor[i] -> sTargColor[i] via sTValue[i]
                r = sPrevColor[i].r + (sTValue[i] * ((sTargColor[i].r - sPrevColor[i].r)));
                g = sPrevColor[i].g + (sTValue[i] * ((sTargColor[i].g - sPrevColor[i].g)));
                b = sPrevColor[i].b + (sTValue[i] * ((sTargColor[i].b - sPrevColor[i].b)));
                _tempRgb[0] = sCurrColor[i].r = r;
                _tempRgb[1] = sCurrColor[i].g = g;
                _tempRgb[2] = sCurrColor[i].b = b;
                sTValue[i] += 0.02f; // @bug: framerate-dependent
                if (sTValue[i] > 1.0f) {
                    sTValue[i] = 1.0f;
                }
            } else {
                temp_fv1 = (light->unk22 + light->min) / 255.0f;
                t = 1.0f - ((((var_ft1 / 255.0f) - temp_fv1) * (0, var_fa0)) + temp_fv1);
                // Lerp color 2 -> color 1
                sCurrColor[i].r = ((light->r1 - light->r2) * t) + light->r2;
                sCurrColor[i].g = ((light->g1 - light->g2) * t) + light->g2;
                sCurrColor[i].b = ((light->b1 - light->b2) * t) + light->b2;
            }
            if (multiplier != 1.0f) {
                sCurrColor[i].r *= multiplier;
                sCurrColor[i].g *= multiplier;
                sCurrColor[i].b *= multiplier;
            }
        } else {
            // Stable light
            if (sTValue[i] < 1.0f) {
                _tempR = sPrevColor[i].r;
                _tempG = sPrevColor[i].g;
                _tempB = sPrevColor[i].b;
                // Lerp sPrevColor[i] -> sTargColor[i] via sTValue[i]
                r = sPrevColor[i].r + (sTValue[i] * ((sTargColor[i].r - sPrevColor[i].r)));
                g = sPrevColor[i].g + (sTValue[i] * ((sTargColor[i].g - sPrevColor[i].g)));
                b = sPrevColor[i].b + (sTValue[i] * ((sTargColor[i].b - sPrevColor[i].b)));
                _tempRgb[0] = sCurrColor[i].r = r;
                _tempRgb[1] = sCurrColor[i].g = g;
                _tempRgb[2] = sCurrColor[i].b = b;
                sTValue[i] += 0.02f; // @bug: framerate-dependent
                if (sTValue[i] > 1.0f) {
                    sTValue[i] = 1.0f;
                }
            } else {
                sCurrColor[i].r = sTargColor[i].r;
                sCurrColor[i].g = sTargColor[i].g;
                sCurrColor[i].b = sTargColor[i].b;
            }
            if (multiplier != 1.0f) {
                sCurrColor[i].r *= multiplier;
                sCurrColor[i].g *= multiplier;
                sCurrColor[i].b *= multiplier;
            }
        }

        if (light){} // @fake

        // Clamp final color
        if (sCurrColor[i].r > 255) {
            sCurrColor[i].r = 255;
        } else if (sCurrColor[i].r < 0) {
            sCurrColor[i].r = 0;
        }
        if (sCurrColor[i].g > 255) {
            sCurrColor[i].g = 255;
        } else if (sCurrColor[i].g < 0) {
            sCurrColor[i].g = 0;
        }
        if (sCurrColor[i].b > 255) {
            sCurrColor[i].b = 255;
        } else if (sCurrColor[i].b < 0) {
            sCurrColor[i].b = 0;
        }

        i += 1;
        counter -= 1;
    } while (counter != 0);
}

// offset: 0x9A0 | func: 4
static void trklight_initSlot(LightAction* action) {
    s16 slot = (s16) action->slot;
    sLights[slot].slot = (u8) slot;
    sLights[slot].type = action->type;
    sLights[slot].flags = action->flags;
    sLights[slot].timer = action->timer;
    sLights[slot].r1 = action->r1;
    sLights[slot].g1 = action->g1;
    sLights[slot].b1 = action->b1;
    sLights[slot].r2 = action->r2;
    sLights[slot].g2 = action->g2;
    sLights[slot].b2 = action->b2;
    sLights[slot].time = (f32) mathRnd(0, 180);
    sLights[slot].unk22 = mathRnd(action->min, action->max);
    if (action->speed == 10) {
        sLights[slot].rate = 0.16666667f;
    } else if (action->speed) {
        sLights[slot].rate = 1.0f / ((f32) action->speed * 60.0f);
    } else {
        sLights[slot].rate = 1.0f / ((f32) mathRnd(1, 10) * 60.0f);
    }
    sLights[slot].speed = action->speed;
    sLights[slot].max = action->max;
    sLights[slot].min = action->min;
    sLights[slot].unk21 = action->unk23;
    sTValue[slot] = 0.0f;
    sTargColor[slot].r = action->r2;
    sTargColor[slot].g = action->g2;
    sTargColor[slot].b = action->b2;
    sLights[slot].inUse = TRUE;
}

// offset: 0xBB0 | func: 5
static void trklight_updateSlot(LightAction* action) {
    s16 i = (s16) action->slot;
    sPrevColor[i].r = sTargColor[i].r;
    sPrevColor[i].g = sTargColor[i].g;
    sPrevColor[i].b = sTargColor[i].b;
    sTValue[i] = 0.0f;
    sTargColor[i].r = action->r2;
    sTargColor[i].g = action->g2;
    sTargColor[i].b = action->b2;
    sLights[i].r1 = action->r1;
    sLights[i].g1 = action->g1;
    sLights[i].b1 = action->b1;
    sLights[i].r2 = action->r2;
    sLights[i].g2 = action->g2;
    sLights[i].b2 = action->b2;
    sLights[i].max = action->max;
    sLights[i].min = action->min;
    sLights[i].unk22 = mathRnd(action->min, action->max);
    if (action->speed == 0xA) {
        sLights[i].rate = 0.16666667f;
    } else if (action->speed) {
        sLights[i].rate = 1.0f / ((f32) action->speed * 60.0f);
    } else {
        sLights[i].rate = 1.0f / ((f32) mathRnd(1, 10) * 60.0f);
    }
    sLights[i].speed = action->speed;
    sLights[i].max = action->max;
    sLights[i].min = action->min;
    sLights[i].unk21 = action->unk23;
}
