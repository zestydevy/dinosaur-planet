#include "common.h"
#include "game/gametexts.h"
#include "macros.h"
#include "sys/fonts.h"
#include "sys/gfx/textable.h"

#define MAX_OPACITY 0xFF

#define BASE_X_LEFT 19
#define BASE_X_RIGHT 299
#define BASE_Y 53

typedef struct {
    u16 frameIn;        //Line starts fading in
    u16 frameHold;      //Line finished fading in, holds at max opacity
    u16 frameOut;       //Line starts fading out
    u16 frameFinished;  //Line finished fading out, holds at zero opacity
    s8 textID;          //String number in the credits' text file (`gametext1FD`)
    s8 lineIndex;       //Decides y position for string
    s8 alignment;       //Screen left/right (`CreditsAlignments`), also decides text colour (left for headings)
    u8 opacity;         //Opacity of the line's text
    f32 spacing;        //Font tracking/character spacing (expands when group line's group finished)
} CreditsLine;

typedef struct {
    CreditsLine lines[9];   //Section heading and developer names
    u16 frameExpand;        //Group lines' horizontal text spacing animates outwards once this frame is reached
    u16 frameFinished;      //Advances to the next group once this frame is reached
    u8 lineCount;           //Count of nonzero items in lines
} CreditsGroup;

typedef enum {
    CREDITS_L,
    CREDITS_R
} CreditsAlignments;

#define EMPTY {0, 0, 0, 0, 0, 0, 0, 0, 0}

/*0x0*/ static CreditsGroup dCredits[] = {
    {
        {
            {128, 216, 536, 560, 255, 0, CREDITS_L, 0, 0}, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY
        }, 
        536, 560, 1
    },

    {
        {
            {824, 864, 1052, 1112, 0, 8, CREDITS_L, 0, 0}, 
            {824, 864, 1052, 1112, 1, 9, CREDITS_R, 0, 0}, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY
        }, 
        1052, 1112, 2
    },

    {
        {
            {1200, 1240, 1884, 1944, 2, 1, CREDITS_L, 0, 0}, 
            {1200, 1240, 1468, 1524, 3, 2, CREDITS_R, 0, 0}, 
            {1240, 1280, 1526, 1584, 4, 3, CREDITS_R, 0, 0}, 
            {1280, 1320, 1586, 1644, 5, 4, CREDITS_R, 0, 0}, 
            {1320, 1360, 1644, 1704, 6, 5, CREDITS_R, 0, 0}, 
            {1360, 1400, 1704, 1764, 7, 6, CREDITS_R, 0, 0}, 
            {1400, 1440, 1762, 1824, 8, 7, CREDITS_R, 0, 0}, 
            {1440, 1482, 1824, 1884, 9, 8, CREDITS_R, 0, 0}, 
            {1482, 1526, 1884, 1944, 10, 9, CREDITS_R, 0, 0}
        }, 
        1884, 1944, 9
    },

    {
        {
            {2030, 2070, 2528, 2588, 11, 6, CREDITS_L, 0, 0}, 
            {2030, 2070, 2414, 2468, 12, 7, CREDITS_R, 0, 0}, 
            {2070, 2110, 2470, 2528, 13, 8, CREDITS_R, 0, 0}, 
            {2110, 2150, 2528, 2588, 14, 9, CREDITS_R, 0, 0}, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY
        }, 
        2528, 2588, 4
    }, 

    {
        {
            {2982, 3020, 3572, 3632, 15, 1, CREDITS_L, 0, 0}, 
            {2982, 3020, 3334, 3392, 16, 2, CREDITS_R, 0, 0}, 
            {3022, 3062, 3390, 3452, 17, 3, CREDITS_R, 0, 0}, 
            {3062, 3102, 3452, 3512, 18, 4, CREDITS_R, 0, 0}, 
            {3102, 3142, 3512, 3572, 19, 5, CREDITS_R, 0, 0}, 
            {3142, 3184, 3572, 3632, 20, 6, CREDITS_R, 0, 0}, 
            EMPTY, 
            EMPTY, 
            EMPTY
        }, 
        3572, 3632, 6
    },

    {
        {
            {3784, 3824, 4378, 4438, 21, 3, CREDITS_L, 0, 0}, 
            {3784, 3824, 4188, 4258, 22, 4, CREDITS_R, 0, 0}, 
            {3824, 3864, 4254, 4318, 23, 5, CREDITS_R, 0, 0}, 
            {3864, 3904, 4316, 4378, 24, 6, CREDITS_R, 0, 0}, 
            {3904, 3946, 4378, 4438, 25, 7, CREDITS_R, 0, 0}, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY
        }, 
        4378, 4438, 5
    },

    {
        {
            {4540, 4580, 4946, 5006, 26, 7, CREDITS_L, 0, 0}, 
            {4540, 4580, 4880, 4946, 27, 8, CREDITS_R, 0, 0}, 
            {4580, 4622, 4946, 5006, 28, 9, CREDITS_R, 0, 0}, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY
        }, 
        4946, 5006, 3
    },

    {
        {
            {5094, 5134, 5316, 5376, 29, 1, CREDITS_L, 0, 0}, 
            {5094, 5134, 5316, 5376, 30, 2, CREDITS_R, 0, 0}, 
            {5284, 5324, 5516, 5576, 31, 3, CREDITS_L, 0, 0}, 
            {5284, 5324, 5516, 5576, 32, 4, CREDITS_R, 0, 0}, 
            {5486, 5526, 5688, 5746, 33, 5, CREDITS_L, 0, 0}, 
            {5486, 5526, 5688, 5746, 34, 6, CREDITS_R, 0, 0}, 
            EMPTY, 
            EMPTY, 
            EMPTY
        }, 
        5688, 5746, 6
    }, 

    {
        {
            {6030, 6070, 6468, 6528, 35, 6, CREDITS_L, 0, 0}, 
            {6030, 6070, 6340, 6408, 36, 7, CREDITS_R, 0, 0}, 
            {6070, 6110, 6406, 6468, 37, 8, CREDITS_R, 0, 0}, 
            {6110, 6150, 6468, 6528, 38, 9, CREDITS_R, 0, 0}, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY
        }, 
        6468, 6528, 4
    },

    {
        {
            {6766, 6806, 7290, 7350, 39, 8, CREDITS_L, 0, 0}, 
            {6766, 6806, 7290, 7350, 40, 9, CREDITS_R, 0, 0}, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY, 
            EMPTY
        }, 
     7290, 7350, 2
    }
};

/*0x0*/ static Texture* sDinosaurPlanetLogoTex; //Shown at the beginning of the credits
/*0x4*/ static u8 sGroupIdx;                    //The index of the current group of names
/*0x8*/ static GameTextChunk* sText;            //Gametext file 0x1FD
/*0xC*/ static f32 sTime;                       //The current frame of the credits

// offset: 0x0 | ctor
void credits_ctor(void *dll) {
    sDinosaurPlanetLogoTex = tex_load_deferred(TEXTABLE_C5_DinosaurPlanetLogo);
    sText = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_1FD_Credits);
    sGroupIdx = 0;
    sTime = 0.0f;
}

// offset: 0x80 | dtor
void credits_dtor(void *dll) {
    tex_free(sDinosaurPlanetLogoTex);
    mmFree(sText);
}

// offset: 0xD4 | func: 0 | export: 0
s32 credits_update1(void) {
    CreditsLine* line;
    s32 i;
    f32 tValue;
    u8 opacity;
    
    if (sGroupIdx < (s32)ARRAYCOUNT(dCredits)) {
        //Advance credits' time
        sTime += gUpdateRateF;
        
        //Advance to the next group of names
        if (sTime >= dCredits[sGroupIdx].frameFinished) {
            sGroupIdx++;
        }
        
        //Update the current text group's opacity and spacing animation
        if (sGroupIdx < (s32)ARRAYCOUNT(dCredits)) {
            //Iterate over the group's lines
            for (i = 0; i < dCredits[sGroupIdx].lineCount; i++) {
                line = &dCredits[sGroupIdx].lines[i];

                //Line hasn't shown up yet, not visible
                if (sTime < line->frameIn) {
                    opacity = 0;

                //Line fading in
                } else if (sTime < line->frameHold) {
                    //Get opacity tValue for fade-in
                    tValue = (sTime - line->frameIn) / (line->frameHold - line->frameIn);
                    if (tValue < 0.0f) {
                        tValue = 0.0f;
                    } else if (tValue > 1.0f) {
                        tValue = 1.0f;
                    }
                    
                    opacity = (u8)(tValue * MAX_OPACITY);

                //Line holding at max opacity
                } else if (sTime < line->frameOut) {
                    opacity = MAX_OPACITY;

                //Line fading out
                } else if (sTime < line->frameFinished) {
                    tValue = (sTime - line->frameOut) / (line->frameFinished - line->frameOut);
                    if (tValue < 0.0f) {
                        tValue = 0.0f;
                    } else if (tValue > 1.0f) {
                        tValue = 1.0f;
                    }
                    
                    opacity = MAX_OPACITY - (u8)(tValue * MAX_OPACITY);

                //Line finished fading out
                } else {
                    opacity = 0;
                }
                
                line->opacity = opacity;
                
                //Animate group's text spacing expanding outwards before it disappears
                if ((sTime >= line->frameIn) && 
                    (sTime <= line->frameFinished) && 
                    (sTime >= dCredits[sGroupIdx].frameExpand)
                ) {
                    line->spacing += (gUpdateRateF / 60.0f) * 8.0f;
                }
            }
        }
    }
    
    return 0;
}

// offset: 0x568 | func: 1 | export: 1
void credits_update2(void) {
}

// offset: 0x570 | func: 2 | export: 2
void credits_draw(Gfx** gdl, Mtx** mtx, Vertex** vtx) {
    CreditsLine* line;
    s32 align;
    s32 i;
    s32 x;
    s32 y;

    if (sGroupIdx == ARRAYCOUNT(dCredits)) {
        return;
    }
    
    //Set up text window and font
    font_window_set_coords(
        1, 0, 0, 
        GET_VIDEO_WIDTH(vi_get_current_size()), 
        GET_VIDEO_HEIGHT(vi_get_current_size())
    );
    font_window_flush_strings(1);
    font_window_use_font(1, FONT_DINO_SUBTITLE_FONT_1);
        
    if (sGroupIdx == 0) {
        //Draw the Dinosaur Planet logo
        line = &dCredits[sGroupIdx].lines[0];
        rcp_screen_full_write(gdl, sDinosaurPlanetLogoTex, 45, 76, 0, 0, line->opacity, 0);
    } else {
        //Draw the developer credits
        for (i = 0; i < dCredits[sGroupIdx].lineCount; i++) {
            line = &dCredits[sGroupIdx].lines[i];
            
            if (line->opacity == 0) {
                continue;
            }
            
            //Set text colour, alignment, and screen x
            if (line->alignment == CREDITS_L) {
                //Section headings
                font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0xFF, line->opacity);
                align = ALIGN_TOP_LEFT;
                x = BASE_X_LEFT;
            } else {
                //Developer names
                font_window_set_text_colour(1, 0x98, 0x9F, 0xBA, 0xFF, line->opacity);
                align = ALIGN_TOP_RIGHT;
                x = BASE_X_RIGHT;
            }
            
            y = BASE_Y + ((line->lineIndex - 1) << 4);
            
            //Text
            font_window_set_extra_char_spacing(1, line->spacing);
            font_window_add_string_xy(1, x, y, sText->strings[line->textID], 1, align);
            
            //Text drop-shadow
            font_window_set_text_colour(1, 0, 0, 0, 0xFF, line->opacity);
            font_window_add_string_xy(1, x - 2, y - 2, sText->strings[line->textID], 2, align);
        }
    }
    
    font_window_draw(gdl, 0, 0, 1);
}
