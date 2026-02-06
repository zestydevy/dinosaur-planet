#include "sys/fonts.h"
#include "sys/asset_thread.h"
#include "sys/fs.h"
#include "sys/gfx/gx.h"
#include "sys/gfx/map.h"
#include "sys/gfx/texture.h"
#include "sys/memory.h"
#include "functions.h"

/* -------- .bss start 800a7d30 -------- */
s32 gNumFonts;
FontData *gFile_FONTS_BIN;
FontWindow *gFontWindows;
FontString *gFontStrings;
s32 gFontSquash;
/* -------- .bss end 800a7d50 -------- */

// Descending powers of 10
s32 gDescPowsOf10[] = {
    1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10,
};

// The font window will draw in pieces, using properties from each line. It starts with a box
// and goes inwards or outwards depending on the direction, set by the define.

#define INWARDS 0
#define OUTWARDS 1

s8 gFontWindowDimensions[] = {
    /*X Offset*/ 0,  INWARDS,  /*Y Start*/  4,  OUTWARDS, /*Y end*/ -4,
    /*X Offset*/ 1,  INWARDS,  /*Y Start*/  3,  INWARDS,  /*Y end*/  4,
    /*X Offset*/ 1,  OUTWARDS, /*Y Start*/ -4,  OUTWARDS, /*Y end*/ -3,
    /*X Offset*/ 1,  INWARDS,  /*Y Start*/  2,  INWARDS,  /*Y end*/  3,
    /*X Offset*/ 1,  OUTWARDS, /*Y Start*/ -3,  OUTWARDS, /*Y end*/ -2,
    /*X Offset*/ 2,  INWARDS,  /*Y Start*/  1,  INWARDS,  /*Y end*/  2,
    /*X Offset*/ 2,  OUTWARDS, /*Y Start*/ -2,  OUTWARDS, /*Y end*/ -1,
    /*X Offset*/ 4,  INWARDS,  /*Y Start*/  0,  INWARDS,  /*Y end*/  1,
    /*X Offset*/ 4,  OUTWARDS, /*Y Start*/ -1,  OUTWARDS, /*Y end*/  0,
    /*X Offset*/ -1, // End of Data
};

#undef INWARDS
#undef OUTWARDS

void fonts_init() {
    s32 i;
    s32 resolution;
    s32 fbWidth;
    s32 fbHeight;

    resolution = vi_get_current_size();
    fbWidth = GET_VIDEO_WIDTH(resolution);
    fbHeight = GET_VIDEO_HEIGHT(resolution);

    queue_alloc_load_file((void**)&gFile_FONTS_BIN, FONTS_BIN);

    gNumFonts = ((u32*)gFile_FONTS_BIN)[0];
    gFile_FONTS_BIN = (FontData*)((u32*)gFile_FONTS_BIN + 1);

    for (i = 0; i < gNumFonts; i++) {
        gFile_FONTS_BIN[i].referenceCount = 0;
    }

    gFontWindows = (FontWindow*)mmAlloc(
        sizeof(FontWindow) * FONT_WINDOW_COUNT
            + sizeof(FontString) * FONT_STRING_COUNT, 
        COLOUR_TAG_YELLOW, ALLOC_NAME("font:window"));
    gFontStrings = (FontString*)(gFontWindows + FONT_WINDOW_COUNT);

    for (i = 0; i < FONT_WINDOW_COUNT; i++) {
        gFontWindows[i].xpos = 0;
        gFontWindows[i].ypos = 0;
        gFontWindows[i].x1 = 0;
        gFontWindows[i].y1 = 0;
        gFontWindows[i].x2 = fbWidth - 1;
        gFontWindows[i].y2 = fbHeight - 1;
        gFontWindows[i].width = fbWidth;
        gFontWindows[i].height = fbHeight;
        gFontWindows[i].backgroundColourR = 255;
        gFontWindows[i].backgroundColourG = 255;
        gFontWindows[i].backgroundColourB = 255;
        gFontWindows[i].backgroundColourA = 0;
        gFontWindows[i].textColourR = 255;
        gFontWindows[i].textColourG = 255;
        gFontWindows[i].textColourB = 255;
        gFontWindows[i].textColourA = 0;
        gFontWindows[i].textBGColourR = 255;
        gFontWindows[i].textBGColourG = 255;
        gFontWindows[i].textBGColourB = 255;
        gFontWindows[i].textBGColourA = 0;
        gFontWindows[i].opacity = 255;
        gFontWindows[i].font = 0xFF;
        gFontWindows[i].flags = (i != 0) ? FONT_WINDOW_VERTS : 0;
        gFontWindows[i].textOffsetX = 0;
        gFontWindows[i].textOffsetY = 0;
        gFontWindows[i].strings = NULL;
    }

    for (i = 0; i < FONT_STRING_COUNT; i++) {
        gFontStrings[i].priority = 255;
        gFontStrings[i].text = NULL;
        gFontStrings[i].textColourR = 255;
        gFontStrings[i].textColourG = 255;
        gFontStrings[i].textColourB = 255;
        gFontStrings[i].textColourA = 0;
        gFontStrings[i].textBGColourR = 255;
        gFontStrings[i].textBGColourG = 255;
        gFontStrings[i].textBGColourB = 255;
        gFontStrings[i].textBGColourA = 0;
        gFontStrings[i].next = NULL;
    }

    gFontSquash = FALSE;
}

void fonts_set_squash(s32 enabled) {
    gFontSquash = enabled;
}

static const char str_1[] = "FONTS - unable to load font number %d, out of range\n";
void font_load(s32 fontID) {
    if (fontID < gNumFonts) {
        FontData *fontData = &gFile_FONTS_BIN[fontID];
        fontData->referenceCount++;

        if (fontData->referenceCount == 1) {
            s32 i = 0;
            while (i < FONTDATA_MAX_TEXTURES && fontData->textureID[i] != -1) {
                fontData->texturePointers[i] = tex_load_deferred(-fontData->textureID[i]);
                i++;
            }
        }
    }
}

void font_unload(s32 fontID) {
    if (fontID < gNumFonts) {
        FontData *fontData = &gFile_FONTS_BIN[fontID];

        if (fontData->referenceCount > 0) {
            fontData->referenceCount--;

            if (fontData->referenceCount == 0) {
                s32 i = 0;
                while (i < FONTDATA_MAX_TEXTURES && fontData->textureID[i] != -1) {
                    tex_free(fontData->texturePointers[i]);
                    fontData->texturePointers[i] = NULL;
                    i++;
                }
            }
        }
    }
}

static const char str_2[] = "FONTS - trying to print a string to a no existent window!\n";
void font_window_set_text_coords(s32 windowID, s32 xPos, s32 yPos, s32 param4) {
    if (windowID >= 0 && windowID < FONT_WINDOW_COUNT) {
        FontWindow *window = &gFontWindows[windowID];

        if (xPos == TEXT_POS_CENTER) {
            window->xpos = window->width >> 1;
        } else {
            window->xpos = xPos;
        }

        if (yPos == TEXT_POS_CENTER) {
            window->ypos = window->height >> 1;
        } else {
            window->ypos = yPos;
        }
    }
}


static const char str_3[] = "FONTS - trying to print a string to a no existent window!\n";
void font_print_window(Gfx **gdl, s32 windowID, char *text, AlignmentFlags alignmentFlags) {
    if (windowID >= 0 && windowID < FONT_WINDOW_COUNT) {
        FontWindow *window = &gFontWindows[windowID];

        font_render_text(gdl, window, text, alignmentFlags, 1.0f);
    }
}

static const char str_4[] = "FONTS - trying to print a string to a no existent window!\n";
void font_print_window_xy(Gfx **gdl, s32 windowID, s32 xPos, s32 yPos, char *text, AlignmentFlags alignmentFlags) {
    if (windowID >= 0 && windowID < FONT_WINDOW_COUNT) {
        FontWindow *window = &gFontWindows[windowID];

        if (xPos == TEXT_POS_CENTER) {
            window->xpos = window->width >> 1;
        } else {
            window->xpos = xPos;
        }

        if (yPos == TEXT_POS_CENTER) {
            window->ypos = window->height >> 1;
        } else {
            window->ypos = yPos;
        }

        if (alignmentFlags != 0) {
            font_render_text(gdl, window, text, alignmentFlags, 1.0f);
        } else {
            font_render_text_wordwrap(gdl, window, text, 1.0f);
        }
    }
}

static const char str_5[] = "FONTS - You must specify a font to use before attempting to print anything\n";
void font_render_text(Gfx** gdl, FontWindow* window, char* text, AlignmentFlags alignmentFlags, f32 scisScale) {
    s32 charIndex; // spDC
    s32 charSpace;
    s32 scisOffset;
    s32 scisPos;
    s32 ypos;
    s32 xpos;
    s32 textureLry;
    s32 textureS;
    s32 textureT;
    Texture *tex;
    s32 textureLrx;
    s32 textureWidth;
    s32 textureHeight;
    s32 xAlignmentDiff;
    s32 yAlignmentDiff;
    s32 textureUly;
    s32 textureUlx;
    s32 lastTextureIndex;
    s32 temp;
    FontData* fontData;
    s16 newData;
    u8 curChar;

    xAlignmentDiff = -1;
    lastTextureIndex = -1;
    if (window->font == 0xFF) {
        return;
    }

    if (text == NULL) {
        return;
    }

    xpos = window->xpos;
    ypos = window->ypos;
    fontData = &gFile_FONTS_BIN[window->font];
   
    gSPLoadGeometryMode(*gdl, G_SHADE | G_SHADING_SMOOTH);
    dl_apply_geometry_mode(gdl);
    
    gDPSetOtherMode(*gdl,
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2);
    dl_apply_other_mode(gdl);
    
    if (window != &gFontWindows[0]) {
        scisOffset = ((f32)((window->y2 - window->y1) + 1) / 2) * scisScale;
        scisPos = (window->y1 + window->y2) >> 1;
        gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, window->x1, scisPos - scisOffset, window->x2, scisPos + scisOffset);
    }
    if (alignmentFlags & HORZ_ALIGN_CENTER) {
        xAlignmentDiff = font_get_text_width_internal(window, text, xpos, window->font);
        xpos -= xAlignmentDiff >> 1;
    }
    if (alignmentFlags & VERT_ALIGN_BOTTOM) {
        ypos = (ypos - fontData->y) + 1;
    }
    if (alignmentFlags & VERT_ALIGN_MIDDLE) {
        ypos -= fontData->y >> 1;
    }

    if (window->textBGColourA != 0) {
        dl_set_env_color(gdl, window->textBGColourR, window->textBGColourG, window->textBGColourB, window->textBGColourA);
        
        if (xAlignmentDiff == -1) {
            xAlignmentDiff = font_get_text_width_internal(window, text, xpos, window->font);
        }

        textureLrx = (xpos + xAlignmentDiff) - 1;
        textureLry = (ypos + fontData->y) - 1;
        
        gDPSetCombineLERP((*gdl), 0, 0, 0, ENVIRONMENT, 0, 0, 0, ENVIRONMENT, 0, 0, 0, ENVIRONMENT, 0, 0, 0, ENVIRONMENT);
        dl_apply_combine(gdl);

        gDPSetOtherMode((*gdl),
            G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
            G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2);
        dl_apply_other_mode(gdl);

        gDPFillRectangle((*gdl)++, xpos + window->x1, ypos + window->y1, textureLrx + window->x1, textureLry + window->y1);
        
        gDLBuilder->needsPipeSync = 1;
    }
    
    dl_set_prim_color(gdl, 255, 255, 255, window->opacity);
    dl_set_env_color(gdl, window->textColourR, window->textColourG, window->textColourB, window->textColourA);
    
    gDPSetCombineLERP(*gdl, ENVIRONMENT, TEXEL0, ENV_ALPHA, TEXEL0, TEXEL0, 0, PRIMITIVE, 0, ENVIRONMENT, TEXEL0, ENV_ALPHA, TEXEL0, TEXEL0, 0, PRIMITIVE, 0);
    dl_apply_combine(gdl);

    gDPSetOtherMode(*gdl, 
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2);
    dl_apply_other_mode(gdl);
    
    charIndex = 0;
    xpos += window->textOffsetX;
    ypos += window->textOffsetY;
    xpos <<= 8;
    
    if (alignmentFlags & HORZ_ALIGN_RIGHT) {
        while ((text[charIndex] != '\0') && (text[charIndex] != (char)0xFF) && (window->y2 >= ypos)) {
            charIndex++;
        }
        charIndex -= 1;
        
        while (charIndex >= 0) {
            curChar = (u8)text[charIndex];
            newData = FALSE;
            charSpace = 0;
            if (curChar <= 0x20) {
                switch (curChar) {
                case ' ':
                    xpos -= fontData->charWidth << 8;
                    break;
                case '\n':
                    xpos = window->textOffsetX << 8;
                    ypos += fontData->y;
                    break;
                case '\t':
                    xpos -= (fontData->charHeight - ((xpos - window->textOffsetX) % fontData->charHeight)) << 8;
                    break;
                case '\v': // Vertical tab
                    ypos += fontData->y;
                    break;
                case '\r':
                    xpos = window->textOffsetX << 8;
                    break;
                default:
                    xpos -= fontData->charWidth << 8;
                    break;
                }
            } else {
                curChar -= 0x20;
                if (0xFF != fontData->letters[curChar].textureIndex) {
                    if (lastTextureIndex != fontData->letters[curChar].textureIndex) {
                        lastTextureIndex = fontData->letters[curChar].textureIndex;
                        tex = fontData->texturePointers[lastTextureIndex];
                        tex_gdl_set_textures(gdl, tex, NULL, 0, 0, 0, 0);
                    }
                    newData = TRUE;
                    textureWidth = fontData->letters[curChar].offsetX;
                    textureHeight = fontData->letters[curChar].offsetY;
                    textureS = fontData->letters[curChar].textureU;
                    textureT = fontData->letters[curChar].textureV;
                    xAlignmentDiff = fontData->letters[curChar].width;
                    yAlignmentDiff = fontData->letters[curChar].height;
                    charSpace = (fontData->x == 0) ? fontData->letters[curChar].kerning : fontData->x;
                }
            }
            xpos -= (charSpace << 8);
            if (newData) {
                textureUlx = (xpos >> 6) + ((window->x1 + textureWidth) * 4);
                textureUly = (window->y1 + ypos + textureHeight) * 4;
                textureLrx = (xAlignmentDiff * 4) + textureUlx;
                textureLry = (yAlignmentDiff * 4) + textureUly;
                textureS *= 32;
                textureT *= 32;
                if ((textureUlx < 0) && (textureLrx > 0)) {
                    textureS += -textureUlx * 8;
                    textureUlx = 0;
                }
                if ((textureUly < 0) && (textureLry > 0)) {
                    textureT += -textureUly * 8;
                    textureUly = 0;
                }

                gSPTextureRectangle((*gdl)++, textureUlx, textureUly, textureLrx, textureLry, 0, textureS, textureT,
                    1 << 10, 1 << 10);
                
                gDLBuilder->needsPipeSync = 1;
            }
            charIndex -= 1;
            temp = (window->extraCharacterSpacing * 255.0f);
            xpos -= temp;
        }
    } else {
        charIndex = 0;
        // @fake
        dummy_label_315584: ;
        while ((text[charIndex] != '\0') && (text[charIndex] != (char)0xFF) && (window->y2 >= ypos)) {
            curChar = text[charIndex];
            newData = FALSE;
            charSpace = 0;
            if (curChar <= 0x20) {
                switch (curChar) {
                case ' ':
                    xpos += fontData->charWidth << 8;
                    break;
                case '\n':
                    xpos = window->textOffsetX << 8;
                    ypos += fontData->y;
                    break;
                case '\t':
                    xpos += (fontData->charHeight - ((xpos - window->textOffsetX) % fontData->charHeight)) << 8;
                    break;
                case '\v': // Vertical tab
                    ypos += fontData->y;
                    break;
                case '\r':
                    xpos = window->textOffsetX << 8;
                    break;
                default:
                    xpos += fontData->charWidth << 8;
                    break;
                }
            } else {
                curChar -= 0x20;
                if (0xFF != fontData->letters[curChar].textureIndex) {
                    if (lastTextureIndex != fontData->letters[curChar].textureIndex) {
                        lastTextureIndex = fontData->letters[curChar].textureIndex;
                        tex = fontData->texturePointers[lastTextureIndex];
                        tex_gdl_set_textures(gdl, tex, NULL, 0, 0, 0, 0);
                    }
                    newData = TRUE;
                    textureWidth = fontData->letters[curChar].offsetX;
                    textureHeight = fontData->letters[curChar].offsetY;
                    textureS = fontData->letters[curChar].textureU;
                    textureT = fontData->letters[curChar].textureV;
                    xAlignmentDiff = fontData->letters[curChar].width;
                    yAlignmentDiff = fontData->letters[curChar].height;
                    charSpace = (fontData->x == 0) ? fontData->letters[curChar].kerning : fontData->x;
                }
            }

            if (newData) {
                textureUlx = (xpos >> 6) + ((window->x1 + textureWidth) * 4);
                textureUly = ((window->y1 + ypos) + textureHeight) * 4;
                textureLrx = (xAlignmentDiff * 4) + textureUlx;
                textureLry = (yAlignmentDiff * 4) + textureUly;
                textureS *= 32;
                textureT *= 32;
                if ((textureUlx < 0) && (textureLrx > 0)) {
                    textureS += -textureUlx * 8;
                    textureUlx = 0;
                }
                if ((textureUly < 0) && (textureLry > 0)) {
                    textureT += -textureUly * 8;
                    textureUly = 0;
                }

                gSPTextureRectangle((*gdl)++, textureUlx, textureUly, textureLrx, textureLry, 0, textureS, textureT,
                    1 << 10, 1 << 10);
                
                gDLBuilder->needsPipeSync = 1;
            }
            
            if ((gFontSquash) && (charSpace != 0)) {
                charSpace -= 1;
            }
            
            xpos += (charSpace << 8);
            temp = (window->extraCharacterSpacing * 255.0f);
            xpos += temp;
            charIndex++;
        }
    }
    
    window->xpos = (xpos >> 8) - window->textOffsetX;
    window->ypos = ypos - window->textOffsetY;
    
    if (window != gFontWindows) {
        func_80002490(gdl);
    }
}

s32 font_get_text_width(s32 windowID, const char *text, s32 x, s32 fontID) {
    gFontWindows[windowID].extraCharacterSpacing = 0.0f;

    return font_get_text_width_internal(&gFontWindows[windowID], text, x, fontID);
}

s32 font_get_text_width_internal(FontWindow *window, const char *text, s32 x, s32 fontID) {
    s32 diffX;
    s32 thisDiffX;
    FontData *fontData;
    s32 i;
    u8 c;
    s32 numPrintableChars;

    numPrintableChars = 0;

    if (text == NULL) {
        return 0;
    }

    if (fontID < 0) {
        fontID = gFontWindows[0].font;
    }

    fontData = &gFile_FONTS_BIN[fontID];
    diffX = x;

    for (i = 0; text[i] != '\0' && text[i] != (char)0xFF; i++) {
        thisDiffX = diffX;
        c = text[i];

        if (c <= 0x20) {
            if (c == '\t') {
                diffX += fontData->charHeight - (diffX % fontData->charHeight);
            } else {
                diffX += fontData->charWidth;
            }
        } else {
            numPrintableChars++;
            c -= 0x20;

            if (fontData->letters[c].textureIndex != 0xFF) {
                if (fontData->x == 0) {
                    diffX += fontData->letters[c].kerning;
                } else {
                    diffX += fontData->x;
                }
            }
        }

        if (gFontSquash && diffX != thisDiffX) {
            diffX--;
        }
    }

    if (numPrintableChars != 0) {
        diffX += (window->extraCharacterSpacing * numPrintableChars);
    }

    return diffX - x;
}

static const char str_6[] = "FONTS - Window being set must be in range 1 to %d.\n";
void font_window_set_coords(s32 windowID, s32 x1, s32 y1, s32 x2, s32 y2) {
    if (windowID > 0 && windowID < FONT_WINDOW_COUNT) {
        FontWindow *window = &gFontWindows[windowID];
        window->xpos = 0;
        window->ypos = 0;
        if (x1 < x2) {
            window->x1 = (s16) x1;
            window->x2 = (s16) x2;
        } else {
            window->x2 = (s16) x1;
            window->x1 = (s16) x2;
        }
        if (y1 < y2) {
            window->y1 = (s16) y1;
            window->y2 = (s16) y2;
        } else {
            window->y2 = (s16) y1;
            window->y1 = (s16) y2;
        }
        window->width = (window->x2 - window->x1) + 1;
        window->height = (window->y2 - window->y1) + 1;
        window->extraCharacterSpacing = 0.0f;
    }
}

static const char str_7[] = "FONTS - font number %d is not loaded, but in use in window %d.\n";
static const char str_8[] = "FONTS - attempting to use font %d, font out of range.\n";
static const char str_9[] = "FONTS - window %d out of range.\n";
void font_window_use_font(s32 windowID, s32 fontID) {
    if (windowID >= 0 && windowID < FONT_WINDOW_COUNT) {
        FontWindow *window = &gFontWindows[windowID];
        if (fontID < gNumFonts) {
            window->font = fontID;
            if (gFile_FONTS_BIN[fontID].referenceCount == 0) {
            
            }
        } else {

        }
    } else {
        
    }
}

static const char str_10[] = "FONTS - window %d out of range.\n";
void font_window_set_extra_char_spacing(s32 windowID, f32 spacing) {
    if (windowID >= 0 && windowID < FONT_WINDOW_COUNT) {
        FontWindow *window = &gFontWindows[windowID];
        window->extraCharacterSpacing = spacing;
    }
}

static const char str_11[] = "FONTS - Window being set must be in range 1 to %d\n";
void font_window_set_bg_colour(s32 windowID, s32 red, s32 green, s32 blue, s32 alpha) {
    if (windowID > 0 && windowID < FONT_WINDOW_COUNT) {
        FontWindow *window = &gFontWindows[windowID];
        window->backgroundColourR = red;
        window->backgroundColourG = green;
        window->backgroundColourB = blue;
        window->backgroundColourA = alpha;
    }
}

static const char str_12[] = "FONTS - Window being set must be in range 1 to %d\n";
void font_window_set_text_colour(s32 windowID, s32 red, s32 green, s32 blue, s32 alpha, s32 opacity) {
    if (windowID > 0 && windowID < FONT_WINDOW_COUNT) {
        FontWindow *window = &gFontWindows[windowID];
        window->textColourR = red;
        window->textColourG = green;
        window->textColourB = blue;
        window->textColourA = alpha;
        window->opacity = opacity;
    }
}

static const char str_13[] = "FONTS - Window being set must be in range 1 to %d\n";
void font_window_set_text_bg_colour(s32 windowID, s32 red, s32 green, s32 blue, s32 alpha) {
    if (windowID > 0 && windowID < FONT_WINDOW_COUNT) {
        FontWindow *window = &gFontWindows[windowID];
        window->textBGColourR = red;
        window->textBGColourG = green;
        window->textBGColourB = blue;
        window->textBGColourA = alpha;
    }
}

static const char str_14[] = "FONTS - Window being set must be in range 1 to %d\n";
void font_window_add_text_offset(s32 windowID, s32 x, s32 y) {
    if (windowID > 0 && windowID < FONT_WINDOW_COUNT) {
        FontWindow *window = &gFontWindows[windowID];
        window->textOffsetX += x;
        window->textOffsetY += y;
    }
}

static const char str_15[] = "FONTS - Window being set must be in range 1 to %d\n";
void font_window_reset_text_offset(s32 windowID) {
    if (windowID > 0 && windowID < FONT_WINDOW_COUNT) {
        FontWindow *window = &gFontWindows[windowID];
        window->textOffsetX = 0;
        window->textOffsetY = 0;
    }
}

static const char str_16[] = "FONTS - cannot word wrap a string using an unspecified font.\n";
void font_render_text_wordwrap(Gfx** gdl, FontWindow* window, char* text, f32 scisScale) {
    s32 loopCond;
    FontData* fontData;
    s32 ypos;
    s32 xpos;
    s32 scisOffset;
    s32 rectUly;
    s32 scisPos;
    s32 rectUlx;
    s32 wordWidth;
    s32 xStart;
    s32 bufIdx;
    s32 wordCharCount;
    s32 yStart;
    s32 wordWrapped;
    s32 rectLrx;
    s32 rectLry;
    Texture *tex;
    u8 charBuffer[128];
    u8 curChar;
    u8 lastTextureIndex; // sp8A
    s32 textureS;
    s32 textureT;

    lastTextureIndex = 0xFF;
    
    if (text == NULL) {
        return;
    }
    if (window->font == 0xFF) {
        return;
    }
    
    xpos = window->xpos;
    ypos = window->ypos;
    fontData = &gFile_FONTS_BIN[window->font];
    
    gSPLoadGeometryMode(*gdl, G_SHADE | G_SHADING_SMOOTH);
    dl_apply_geometry_mode(gdl);
    
    // @fake
    if (window) {}
    if (window != gFontWindows) {
        scisOffset = (s32) (((f32) ((window->y2 - window->y1) + 1) / 2) * scisScale);
        scisPos = (s32) (window->y1 + window->y2) >> 1;
        gDPSetScissor((*gdl)++, 0, window->x1,  scisPos - scisOffset, window->x2,  scisPos + scisOffset);
    }
    
    dl_set_prim_color(gdl, 255, 255, 255, window->opacity);
    dl_set_env_color(gdl, window->textColourR, window->textColourG, window->textColourB, window->textColourA);
    
    gDPSetCombineLERP(*gdl, 
        ENVIRONMENT, TEXEL0, ENV_ALPHA, TEXEL0, 
        TEXEL0, 0, PRIMITIVE, 0, 
        ENVIRONMENT, TEXEL0, ENV_ALPHA, TEXEL0, 
        TEXEL0, 0, PRIMITIVE, 0);
    dl_apply_combine(gdl);
    
    gDPSetOtherMode(*gdl,
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2);
    dl_apply_other_mode(gdl);
    
    loopCond = 0;
    
    while (loopCond >= 0) {
        if (xpos >= window->width) {
            xpos = window->xpos;
            ypos += fontData->y;
            wordWrapped = 1;
        } else {
            wordWrapped = 0;
        }

        if ((window->textOffsetY + ypos) >= window->height) {
            // end if beyond window height
            loopCond = -1;
        } else {
            loopCond = 0;
        }
        
        while (loopCond == 0) {
            curChar = *text;
            // if non-printable char
            if ((curChar <= 0x20) || (curChar >= 0x80)) {
                switch (curChar) {
                case '\0':
                    loopCond = -1;
                    break;
                case '\t':
                    xpos += fontData->charHeight - (xpos % fontData->charHeight);
                    break;
                case '\n':
                    wordWrapped = 0;
                    xpos = window->xpos;
                    ypos += fontData->y;
                    break;
                case '\v':
                    wordWrapped = 0;
                    ypos += fontData->y;
                    break;
                case '\r':
                    wordWrapped = 0;
                    xpos = window->xpos;
                    break;
                default:
                    if (wordWrapped == 0) {
                        xpos += fontData->charWidth;
                    }
                    break;
                }
                if (xpos >= window->width) {
                    xpos = window->xpos;
                    ypos += fontData->y;
                    wordWrapped = 1;
                }
                if ((window->textOffsetY + ypos) >= window->height) {
                    loopCond = -1;
                }
                text++;
            } else {
                loopCond = 1;
            }
        }
        
        if (loopCond > 0) {
            wordCharCount = 0;
            wordWidth = 0;

            do {
                charBuffer[wordCharCount] = curChar;
                curChar = (curChar - 0x20);
                wordCharCount += 1;
                if (fontData->letters[curChar].textureIndex != 0xFF) {
                    if (fontData->x != 0) {
                        wordWidth += fontData->x;
                    } else {
                        wordWidth += fontData->letters[curChar].kerning;
                    }
                } else {
                    wordWidth += fontData->charWidth;
                }
                text++;
                curChar = *text;
            } while (wordCharCount < 128 && ((s32) curChar > 0x20) && ((s32) curChar < 0x100));

            if (((xpos + wordWidth) >= window->width) && (xpos != 0)) {
                xpos = window->xpos;
                ypos += fontData->y;
            }
            yStart = window->textOffsetY + ypos;
            if (yStart < window->height) {
                xStart = window->textOffsetX + xpos;
                xpos += wordWidth;
                if ((fontData->y + yStart) > 0) {
                    for (bufIdx = 0; bufIdx < wordCharCount; bufIdx++) {
                        curChar = (charBuffer[bufIdx] - 0x20);
                        if (fontData->letters[curChar].textureIndex != 0xFF) {
                            if (lastTextureIndex != fontData->letters[curChar].textureIndex) {
                                lastTextureIndex = fontData->letters[curChar].textureIndex;
                                tex = fontData->texturePointers[lastTextureIndex];
                                tex_gdl_set_textures(gdl, tex, NULL, 0, 0, 0, 0);
                            }
                            rectUlx = (fontData->letters[curChar].offsetX + window->x1 + xStart) * 4;
                            rectUly = (fontData->letters[curChar].offsetY + window->y1 + yStart) * 4;
                            rectLrx = (fontData->letters[curChar].width * 4) + rectUlx;
                            rectLry = (fontData->letters[curChar].height * 4) + rectUly;
                            textureS = fontData->letters[curChar].textureU * 32;
                            textureT = fontData->letters[curChar].textureV * 32;

                            gSPTextureRectangle((*gdl)++, rectUlx, rectUly, rectLrx, rectLry, 0, textureS, textureT,
                                1 << 10, 1 << 10);
                            
                            gDLBuilder->needsPipeSync = 1;
                            
                            if (fontData->x == 0) {
                                xStart += fontData->letters[curChar].kerning;
                            } else {
                                xStart += fontData->x;
                            }
                        } else {
                            xStart += fontData->charWidth;
                        }
                    }
                }
            } else {
                loopCond = -1;
            }
        }
    }
    
    window->xpos = xpos;
    window->ypos = ypos;
    if (window != gFontWindows) {
        func_80002490(gdl);
    }
}

static const char str_17[] = "FONTS - can't add a string to window %d, out of range./n";
FontString *font_window_add_string(s32 windowID, char *text, s32 priority, AlignmentFlags alignmentFlags) {
    if (windowID < 0 || windowID >= FONT_WINDOW_COUNT) {
        return NULL;
    } else {
        return font_window_add_string_xy(windowID, 
            gFontWindows[windowID].xpos, 
            gFontWindows[windowID].ypos, 
            text, priority, alignmentFlags);
    }
}

static const char str_18[] = "FONTS - can't add a string to window %d, out of range./n";
static const char str_19[] = "FONTS - string priority should be greater than -1 and less than 255 (%d)\n";
FontString *font_window_add_string_xy(s32 windowID, s32 posX, s32 posY, char *text, s32 priority, AlignmentFlags alignmentFlags) {
    s32 width;
    FontString *stringTemp;
    char buffer[256];
    FontString *string;
    s32 i;
    FontWindow *window;
    FontString **nextFieldPtr;
    FontData *fontData;

    if (text == NULL) {
        return NULL;
    }

    if (windowID < 0 || windowID >= FONT_WINDOW_COUNT) {
        return NULL;
    }

    if (priority < 0 || priority >= 255) {
        priority = 0;
    }

    for (i = 0, string = NULL; i < FONT_STRING_COUNT && string == NULL; i++) {
        if (gFontStrings[i].priority == 255) {
            string = &gFontStrings[i];
        }
    }

    if (string != NULL) {
        window = &gFontWindows[windowID];
        if (posX == TEXT_POS_CENTER) {
            posX = window->width >> 1;
        }
        if (posY == TEXT_POS_CENTER) {
            posY = window->height >> 1;
        }
        if (window->font != 0xFF) {
            fontData = &gFile_FONTS_BIN[window->font];
            if (alignmentFlags & HORZ_ALIGN_CENTER) {
                font_string_format(text, buffer, priority);
                width = font_get_text_width_internal(window, buffer, posX, window->font);
                posX = posX - (width >> 1);
            }
            if (alignmentFlags & VERT_ALIGN_BOTTOM) {
                posY = (posY - fontData->y) + 1;
            }
            if (alignmentFlags & VERT_ALIGN_MIDDLE) {
                posY -= fontData->y >> 1;
            }
        }
        if (window->strings == NULL) {
            window->strings = string;
            string->next = NULL;
        } else {
            nextFieldPtr = &window->strings;
            stringTemp = *nextFieldPtr;
            while (stringTemp != NULL && priority < stringTemp->priority) {
                nextFieldPtr = &stringTemp->next;
                stringTemp = stringTemp->next;
            }
            *nextFieldPtr = string;
            string->next = stringTemp;
        }
        string->priority = priority;
        string->text = text;
        string->posX = posX;
        string->posY = posY;
        string->offsetX = 0;
        string->offsetY = 0;
        string->textColourR = window->textColourR;
        string->textColourG = window->textColourG;
        string->textColourB = window->textColourB;
        string->textColourA = window->textColourA;
        string->textBGColourR = window->textBGColourR;
        string->textBGColourG = window->textBGColourG;
        string->textBGColourB = window->textBGColourB;
        string->textBGColourA = window->textBGColourA;
        string->opacity = window->opacity;
        string->font = window->font;
        string->flags = window->flags;
        string->alignmentFlags = alignmentFlags;
        string->extraCharacterSpacing = window->extraCharacterSpacing;
    }

    return string;
}

static const char str_20[] = "FONTS - can't add a string to window %d, out of range./n";
void font_window_add_font_string(s32 windowID, FontString *string) {
    FontWindow *window;
    FontString *curString;
    FontString **nextFieldPtr;

    if (windowID < 0 || windowID >= FONT_WINDOW_COUNT) {
        return;
    }

    window = &gFontWindows[windowID];
    nextFieldPtr = &window->strings;
    curString = window->strings;
    while ((curString != NULL) && (curString != string)) {
        nextFieldPtr = &curString->next;
        curString = curString->next;
    }
    if (curString != NULL) {
        *nextFieldPtr = curString->next;
        string->priority = 0xFF;
    }
}

static const char str_21[] = "FONTS - cannot delete entry form window %d,not found.\n";
void font_window_flush_strings(s32 windowID) {
    FontWindow *window;
    FontString *string;

    if (windowID < 0 || windowID >= FONT_WINDOW_COUNT) {
        return;
    }

    window = &gFontWindows[windowID];
    if (window->strings != NULL) {
        string = window->strings;
        while (string != NULL) {
            string->priority = 0xFF;
            string = string->next;
        }
        window->strings = NULL;
    }
}

static const char str_22[] = "FONTS - can't add a string to window %d, out of range./n";
static const char str_23[] = "FONTS - can't scroll a string in window %d, out of range./n";
void font_window_scroll_string(s32 windowID, FontString *string, s32 x, s32 y, s32 flags) {
    FontData *fontData;
    FontWindow *window;

    if (windowID < 0 || windowID >= FONT_WINDOW_COUNT) {
        return;
    }

    window = &gFontWindows[windowID];

    if (string != NULL) {
        if (string->font != 0xFF) {
            fontData = &gFile_FONTS_BIN[string->font];
            if (flags != 4) {
                switch (flags) {
                    case 1:
                        y *= fontData->y;
                        break;
                    case 2:
                        y *= (window->height / fontData->y) * fontData->y;
                        break;
                }
                string->offsetX += x;
                string->offsetY += y;
                return;
            }
            string->offsetX = 0;
            string->offsetY = 0;
            return;
        }
    }
}

static const char str_24[] = "FONTS - cannot render a window which is out of range!\n";
void font_window_enable(s32 windowID) {
    if (windowID < 0 || windowID >= FONT_WINDOW_COUNT) {
        return;
    }

    gFontWindows[windowID].flags |= FONT_WINDOW_ENABLED;
}

static const char str_25[] = "FONTS - cannot render a window which is out of range!\n";
void font_window_disable(s32 windowID) {
    if (windowID < 0 || windowID >= FONT_WINDOW_COUNT) {
        return;
    }

    gFontWindows[windowID].flags &= ~FONT_WINDOW_ENABLED;
}

s32 font_get_y_spacing(s32 fontID) {
    return gFile_FONTS_BIN[fontID].y;
}

static const char str_26[] = "FONTS - cannot word wrap a window which is out of range!\n";
void font_window_enable_wordwrap(s32 windowID) {
    if (windowID < 0 || windowID >= FONT_WINDOW_COUNT) {
        return;
    }

    gFontWindows[windowID].flags |= FONT_WINDOW_WORDWRAP;
}

static const char str_27[] = "FONTS - cannot disable word wrap in a window which is out of range!\n";
void font_window_disable_wordwrap(s32 windowID) {
    if (windowID < 0 || windowID >= FONT_WINDOW_COUNT) {
        return;
    }

    gFontWindows[windowID].flags &= ~FONT_WINDOW_WORDWRAP;
}

void font_window_enable_verts(s32 windowID) {
    if (windowID < 0 || windowID >= FONT_WINDOW_COUNT) {
        return;
    }

    gFontWindows[windowID].flags |= FONT_WINDOW_VERTS;
}

void font_window_disable_verts(s32 windowID) {
    if (windowID < 0 || windowID >= FONT_WINDOW_COUNT) {
        return;
    }

    gFontWindows[windowID].flags &= ~FONT_WINDOW_VERTS;
}

void font_windows_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs) {
    s32 i;

    for (i = 1; i < FONT_WINDOW_COUNT; i++) {
        if (gFontWindows[i].flags & FONT_WINDOW_ENABLED) {
            if (gFontWindows[i].flags & FONT_WINDOW_VERTS) {
                font_window_draw(gdl, mtxs, vtxs, i);
            } else {
                font_window_draw(gdl, NULL, NULL, i);
            }
        }
    }
}

void font_number_to_string(char **outString, s32 number) {
    u8 digit;
    s32 i;
    s32 hasDigit; // boolean
    s32 div;
    s32 pow;
    char *ret = *outString;

    // Check for a negative number
    if (number < 0) {
        *ret = '-';
        ret++;
        number = -number;
    }

    // Loop through digit places.
    hasDigit = FALSE;
    i = 0;
    while (i < 9) {
        pow = gDescPowsOf10[i++];
        digit = '0';
        if (number >= pow) {
            div = number / pow;
            number -= div * pow;
            digit += div;
            hasDigit = TRUE;
        }
        if (hasDigit) {
            *ret = digit;
            ret++;
        }
    }

    // One's digit place
    *ret = '0' + number;
    ret++;
    *outString = ret;
}

void font_render_fill_rect(Gfx **gdl, s32 ulx, s32 uly, s32 lrx, s32 lry) {
    u32 resolution = vi_get_current_size();
    u32 width = GET_VIDEO_WIDTH(resolution);
    u32 height = GET_VIDEO_HEIGHT(resolution);

    if (lrx >= 0 && (u32) ulx < width && lry >= 0 && (u32) uly < height) {
        if (ulx < 0) {
            ulx = 0;
        }
        if (uly < 0) {
            uly = 0;
        }

        gDPFillRectangle((*gdl)++, ulx, uly, lrx, lry);
        gDLBuilder->needsPipeSync = TRUE;
    }
}

static const char str_28[] = "FONTS - cannot render a window which is out of range!\n";
static const char str_29[] = "FONTS - cannot render a window which is out of range!\n";
static const char str_30[] = "FONTS - cannot render a window which is out of range!\n";
void font_window_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, s32 windowID) {
    FontWindow *window;
    FontString *string;
    s32 vidMode;
    s32 i;
    s32 x1;
    s32 x2;
    char text[256];
    s32 y1;
    s32 y2;
    s32 height;
    
    if (windowID < 0 || windowID >= FONT_WINDOW_COUNT) {
        return;
    }

    window = &gFontWindows[windowID];
    if (window->backgroundColourA != 0) {
        gSPLoadGeometryMode(*gdl, G_SHADE | G_SHADING_SMOOTH);
        dl_apply_geometry_mode(gdl);

        gDPSetCombineLERP(*gdl, 
            0, 0, 0, ENVIRONMENT, 
            0, 0, 0, ENVIRONMENT, 
            0, 0, 0, ENVIRONMENT, 
            0, 0, 0, ENVIRONMENT);
        dl_apply_combine(gdl);

        gDPSetOtherMode(*gdl,
            G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
            G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2);
        dl_apply_other_mode(gdl);

        dl_set_env_color(gdl, 0, 0, 0, 0);

        if (window->x2 - window->x1 < 10 || window->y2 - window->y1 < 10) {
            font_render_fill_rect(gdl, window->x1 - 2, window->y1 - 2, window->x2 + 2, window->y2 + 2);
        } else {
            font_render_fill_rect(gdl, window->x1 - 2, window->y1 + 2, window->x1 + 2, window->y2 - 2);
            font_render_fill_rect(gdl, window->x1 - 2, window->y1 - 2, window->x2 + 2, window->y1 + 2);
            font_render_fill_rect(gdl, window->x2 - 2, window->y1 + 2, window->x2 + 2, window->y2 - 2);
            font_render_fill_rect(gdl, window->x1 - 2, window->y2 - 2, window->x2 + 2, window->y2 + 2);
        }

        dl_set_env_color(gdl, window->backgroundColourR, window->backgroundColourG, window->backgroundColourB, window->backgroundColourA);

        i = 0;
        height = window->y2 - window->y1;

        vidMode = vi_get_mode();
        if (vidMode == 3) {
            window->x1 += 96;
            window->x2 += 96;
        }

        if (height < 9) {
            i = 45 - (height * 5);
        }

        // Loops through gFontWindowDimensions, rendering fillrects until it forms a rounded rectangle.
        // The array determines the width and height of each entry before drawing it.
        while (gFontWindowDimensions[i] >= 0) {
            x1 = gFontWindowDimensions[i] + window->x1;
            y1 = (gFontWindowDimensions[i + 1]) ? gFontWindowDimensions[i + 2] + window->y2
                                                : gFontWindowDimensions[i + 2] + window->y1;
            x2 = window->x2 - gFontWindowDimensions[i];
            y2 = (gFontWindowDimensions[i + 3]) ? gFontWindowDimensions[i + 4] + window->y2
                                                : gFontWindowDimensions[i + 4] + window->y1;
            font_render_fill_rect(gdl, x1, y1, x2, y2);

            i += 5;
        }
    }

    string = window->strings;
    while (string != NULL) {
        window->xpos = string->posX + string->offsetX;
        window->ypos = string->posY + string->offsetY;
        window->textColourR = string->textColourR;
        window->textColourG = string->textColourG;
        window->textColourB = string->textColourB;
        window->textColourA = string->textColourA;
        window->textBGColourR = string->textBGColourR;
        window->textBGColourG = string->textBGColourG;
        window->textBGColourB = string->textBGColourB;
        window->textBGColourA = string->textBGColourA;
        window->opacity = string->opacity;
        window->font = string->font;
        window->extraCharacterSpacing = string->extraCharacterSpacing;
        
        font_string_format(string->text, text, string->priority);

        if (string->flags & FONT_WINDOW_WORDWRAP) {
            font_render_text_wordwrap(gdl, window, text, 1.0f);
        } else {
            font_render_text(gdl, window, text, string->alignmentFlags & HORZ_ALIGN_RIGHT, 1.0f);
        }

        string = string->next;
    }
}

void font_string_format(char *input, char *output, s32 number) {
    while (*input) {
        if (*input == '~') { // ~ is equivalent to a %d.
            // output the number as part of the string
            font_number_to_string(&output, number);
            input++;
        } else {
            *output = *input;
            input++;
            output++;
        }
    }
    *output = '\0'; // null terminator
}
