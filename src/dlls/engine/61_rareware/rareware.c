#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "sys/gfx/textable.h"
#include "sys/gfx/texture.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "sys/rcp.h"
#include "dll.h"
#include "dlls/engine/28_screen_fade.h"
#include "types.h"

#define KEYFRAME_40 40      //Logo fade-in start
#define KEYFRAME_50 50      //Logo faded in
#define KEYFRAME_285 285    //Logo glow start
#define KEYFRAME_620 620    //Screen fade to black

#define LOGO_FADE_DURATION 125.0f
#define LOGO_HOLD_DURATION 250.0f

#define GLOW_FADE_DURATION 72.0f

typedef enum {
    Rareware_STATE_Initial = 0,
    Rareware_STATE_Fading_In = 1,
    Rareware_STATE_Visible = 2,
    Rareware_STATE_Glowing = 3
} RarewareStates;

static s32 dFrame = 0;
static s8 dState = Rareware_STATE_Initial;

static s8 sFadeOutStarted;
static s8 sFadeOutTimer;
static s8 sCutToNextScreen;
static f32 sLogoTimer;
static f32 sGlowTimer;
static Texture *sTexRareLogo;
static Texture *sTexRareLogoGlow;
static Texture *sTexRareware;
static Texture *sTexRarewareGlow;

void rareware_ctor(void *dll) {
    u8 dColourUnused[4] = {0xe4, 0x9c, 0x44, 0xff};

    rcp_set_border_color(0, 0, 0);
    func_80010018(0);

    dFrame = 0;
    dState = Rareware_STATE_Initial;
    sFadeOutStarted = FALSE;
    sFadeOutTimer = 0;
    sCutToNextScreen = FALSE;

    sTexRareLogo     = tex_load_deferred(TEXTABLE_2D6_RareLogo);
    sTexRareLogoGlow = tex_load_deferred(TEXTABLE_2D7_RareLogo_Shadow);
    sTexRareware     = tex_load_deferred(TEXTABLE_2D4_RarewareTextLogo);
    sTexRarewareGlow = tex_load_deferred(TEXTABLE_2D5_RarewareTextLogo_Shadow);
}

void rareware_dtor(void *dll) {
    tex_free(sTexRareLogo);
    tex_free(sTexRareLogoGlow);
    tex_free(sTexRareware);
    tex_free(sTexRarewareGlow);
}

s32 rareware_update1(void) {
    s32 delay;

    //Get gUpdateRate, clamped at maximum of 3
    delay = gUpdateRate;
    if (delay > 3) {
        delay = 3;
    }

    //Decrement fadeout timer, if it's started
    if (sFadeOutTimer > 0) {
        sFadeOutTimer -= delay;
    }

    //End of shot
    if (sCutToNextScreen) {
        main_set_bits(BIT_44F, 0);
        //@bug? Skips directly to Game Select screen, likely for developer convenience
        menu_set(MENU_GAME_SELECT);
    }

    dFrame += gUpdateRate;

    //Start fading out
    if (dFrame > KEYFRAME_620) {
        sFadeOutStarted = TRUE;
    }

    if (sFadeOutStarted) {
        gDLL_28_ScreenFade->vtbl->fade(30, SCREEN_FADE_BLACK);
        sFadeOutTimer = 45;
        sCutToNextScreen = TRUE; //@bug: cuts immediately, instead of at end of fade-out
    }

    if (dState >= Rareware_STATE_Fading_In) {
        sLogoTimer -= gUpdateRateF;
    }

    if (dState >= Rareware_STATE_Glowing) {
        sGlowTimer -= gUpdateRateF;
    }

    return 0;
}

void rareware_update2(void) { }

void rareware_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs) {
    f32 A;
    s32 pad;

    //Stop drawing near the end of the screen's fade-out
    if (sCutToNextScreen && sFadeOutTimer < 11) {
        return;
    }

    rcp_clear_screen(gdl, mtxs, 1);
    gDLL_76->vtbl->func2(gdl, mtxs);

    //Handle key frames
    {
        if (dFrame > KEYFRAME_40 && dState == Rareware_STATE_Initial) {
            dState = Rareware_STATE_Fading_In;
            sLogoTimer = 2*LOGO_FADE_DURATION + LOGO_HOLD_DURATION;
        }

        if (dFrame > KEYFRAME_50 && dState == Rareware_STATE_Fading_In) {
            dState = Rareware_STATE_Visible;
        }

        if (dFrame > KEYFRAME_285 && dState == Rareware_STATE_Visible) {
            dState = Rareware_STATE_Glowing;
            sGlowTimer = 2*GLOW_FADE_DURATION + 1;
        }
    }

    //Draw glows around Rare logo and Rareware text (fade in, then out)
    if (dState >= Rareware_STATE_Glowing) {
        //Handle opacity
        if (sGlowTimer <= GLOW_FADE_DURATION) {
            A = 1.0f - ((GLOW_FADE_DURATION - sGlowTimer) / GLOW_FADE_DURATION); //fading out
        } else {
            A = 1.0f - ((sGlowTimer - GLOW_FADE_DURATION) / GLOW_FADE_DURATION); //fading in
        }

        //Clamp opacity coefficient (0-1)
        if (A < 0.0f) {
            A = 0.0f;
        } else if (A > 1.0f) {
            A = 1.0f;
        }

        rcp_screen_full_write(gdl, sTexRareLogoGlow, 165, 148, 0, 0, (s16)(255.0f * A), SCREEN_WRITE_TRANSLUCENT);
        rcp_screen_full_write(gdl, sTexRarewareGlow, 102, 250, 0, 0, (s16)(255.0f * A), SCREEN_WRITE_TRANSLUCENT);
    }

    //Draw Rare logo and Rareware text (fade in, hold for a while, then fade out)
    if (dState >= Rareware_STATE_Fading_In) {
        //Handle opacity
        if (sLogoTimer <= LOGO_FADE_DURATION) {
            A = 1.0f - ((LOGO_FADE_DURATION - sLogoTimer) / LOGO_FADE_DURATION); //out
        } else {
            A = 1.0f - ((sLogoTimer - (LOGO_FADE_DURATION + LOGO_HOLD_DURATION)) / LOGO_FADE_DURATION); //in + hold
        }

        //Clamp opacity coefficient (0-1)
        if (A < 0.0f) {
            A = 0.0f;
        } else if (A > 1.0f) {
            A = 1.0f;
        }

        rcp_screen_full_write(gdl, sTexRareLogo, 165, 148, 0, 0, (u8)(255.0f * A), SCREEN_WRITE_TRANSLUCENT);
        rcp_screen_full_write(gdl, sTexRareware, 102, 250, 0, 0, (u8)(255.0f * A), SCREEN_WRITE_TRANSLUCENT);
    }
}
