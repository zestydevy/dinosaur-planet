#include "sys/fonts.h"
#include "sys/asset_thread.h"
#include "sys/fs.h"
#include "sys/gfx/gx.h"
#include "sys/gfx/map.h"
#include "sys/gfx/texture.h"
#include "sys/memory.h"
#include "functions.h"

static const char str_1[] = "FONTS - unable to load font number %d, out of range\n";
static const char str_2[] = "FONTS - trying to print a string to a no existent window!\n";
static const char str_3[] = "FONTS - trying to print a string to a no existent window!\n";
static const char str_4[] = "FONTS - trying to print a string to a no existent window!\n";
static const char str_5[] = "FONTS - You must specify a font to use before attempting to print anything\n";
static const char str_6[] = "FONTS - Window being set must be in range 1 to %d.\n";
static const char str_7[] = "FONTS - font number %d is not loaded, but in use in window %d.\n";
static const char str_8[] = "FONTS - attempting to use font %d, font out of range.\n";
static const char str_9[] = "FONTS - window %d out of range.\n";
static const char str_10[] = "FONTS - window %d out of range.\n";
static const char str_11[] = "FONTS - Window being set must be in range 1 to %d\n";
static const char str_12[] = "FONTS - Window being set must be in range 1 to %d\n";
static const char str_13[] = "FONTS - Window being set must be in range 1 to %d\n";
static const char str_14[] = "FONTS - Window being set must be in range 1 to %d\n";
static const char str_15[] = "FONTS - Window being set must be in range 1 to %d\n";
static const char str_16[] = "FONTS - cannot word wrap a string using an unspecified font.\n";
static const char str_17[] = "FONTS - can't add a string to window %d, out of range./n";
static const char str_18[] = "FONTS - can't add a string to window %d, out of range./n";
static const char str_19[] = "FONTS - string priority should be greater than -1 and less than 255 (%d)\n";
static const char str_20[] = "FONTS - can't add a string to window %d, out of range./n";
static const char str_21[] = "FONTS - cannot delete entry form window %d,not found.\n";
static const char str_22[] = "FONTS - can't add a string to window %d, out of range./n";
static const char str_23[] = "FONTS - can't scroll a string in window %d, out of range./n";
static const char str_24[] = "FONTS - cannot render a window which is out of range!\n";
static const char str_25[] = "FONTS - cannot render a window which is out of range!\n";
static const char str_26[] = "FONTS - cannot word wrap a window which is out of range!\n";
static const char str_27[] = "FONTS - cannot disable word wrap in a window which is out of range!\n";
static const char str_28[] = "FONTS - cannot render a window which is out of range!\n";
static const char str_29[] = "FONTS - cannot render a window which is out of range!\n";
static const char str_30[] = "FONTS - cannot render a window which is out of range!\n";

extern s32 gNumFonts;
extern FontData *gFile_FONTS_BIN;
extern DialogueBoxBackground *gDialogueBoxBackground;
extern DialogueTextElement *gDialogueText;
extern s32 gCompactKerning;

extern s32 D_8008C7E0[]; // gDescPowsOf10
extern s8 D_8008C804[];

s32 func_8000D920(DialogueBoxBackground *box, char *text, s32 x, s32 fontID);

void init_fonts() {
    s32 i;
    s32 resolution;
    s32 fbWidth;
    s32 fbHeight;

    resolution = get_some_resolution_encoded();
    fbWidth = RESOLUTION_WIDTH(resolution);
    fbHeight = RESOLUTION_HEIGHT(resolution);

    queue_alloc_load_file((void**)&gFile_FONTS_BIN, FONTS_BIN);

    gNumFonts = ((u32*)gFile_FONTS_BIN)[0];
    gFile_FONTS_BIN = (FontData*)((u32*)gFile_FONTS_BIN + 1);

    for (i = 0; i < gNumFonts; i++) {
        gFile_FONTS_BIN[i].loadedFonts[0] = 0;
    }

    gDialogueBoxBackground = (DialogueBoxBackground*)malloc(
        sizeof(DialogueBoxBackground) * DIALOGUEBOXBACKGROUND_COUNT
            + sizeof(DialogueTextElement) * DIALOGUETEXTELEMENT_COUNT, 
        0xFFFF00FF, NULL);
    gDialogueText = (DialogueTextElement*)(gDialogueBoxBackground + DIALOGUEBOXBACKGROUND_COUNT);

    for (i = 0; i < DIALOGUEBOXBACKGROUND_COUNT; i++) {
        gDialogueBoxBackground[i].xpos = 0;
        gDialogueBoxBackground[i].ypos = 0;
        gDialogueBoxBackground[i].x1 = 0;
        gDialogueBoxBackground[i].y1 = 0;
        gDialogueBoxBackground[i].x2 = fbWidth - 1;
        gDialogueBoxBackground[i].y2 = fbHeight - 1;
        gDialogueBoxBackground[i].width = fbWidth;
        gDialogueBoxBackground[i].height = fbHeight;
        gDialogueBoxBackground[i].backgroundColourR = 255;
        gDialogueBoxBackground[i].backgroundColourG = 255;
        gDialogueBoxBackground[i].backgroundColourB = 255;
        gDialogueBoxBackground[i].backgroundColourA = 0;
        gDialogueBoxBackground[i].textColourR = 255;
        gDialogueBoxBackground[i].textColourG = 255;
        gDialogueBoxBackground[i].textColourB = 255;
        gDialogueBoxBackground[i].textColourA = 0;
        gDialogueBoxBackground[i].textBGColourR = 255;
        gDialogueBoxBackground[i].textBGColourG = 255;
        gDialogueBoxBackground[i].textBGColourB = 255;
        gDialogueBoxBackground[i].textBGColourA = 0;
        gDialogueBoxBackground[i].opacity = 255;
        gDialogueBoxBackground[i].font = 0xFF;
        gDialogueBoxBackground[i].flags = (i != 0) ? 0x4000 : 0;
        gDialogueBoxBackground[i].textOffsetX = 0;
        gDialogueBoxBackground[i].textOffsetY = 0;
        gDialogueBoxBackground[i].textBox = NULL;
    }

    for (i = 0; i < DIALOGUETEXTELEMENT_COUNT; i++) {
        gDialogueText[i].number = 255;
        gDialogueText[i].text = NULL;
        gDialogueText[i].textColourR = 255;
        gDialogueText[i].textColourG = 255;
        gDialogueText[i].textColourB = 255;
        gDialogueText[i].textColourA = 0;
        gDialogueText[i].textBGColourR = 255;
        gDialogueText[i].textBGColourG = 255;
        gDialogueText[i].textBGColourB = 255;
        gDialogueText[i].textBGColourA = 0;
        gDialogueText[i].nextBox = NULL;
    }

    gCompactKerning = FALSE;
}

void set_compact_kerning(s32 setting) {
    gCompactKerning = setting;
}

// load_font
void func_8000CB00(s32 fontID) {
    if (fontID < gNumFonts) {
        FontData *fontData = &gFile_FONTS_BIN[fontID];
        fontData->loadedFonts[0]++;

        if (fontData->loadedFonts[0] == 1) {
            s32 i = 0;
            while (i < FONTDATA_MAX_TEXTURES && fontData->textureID[i] != -1) {
                fontData->texturePointers[i] = queue_load_texture_proxy(-fontData->textureID[i]);
                i++;
            }
        }
    }
}

// unload_font
void func_8000CBC8(s32 fontID) {
    if (fontID < gNumFonts) {
        FontData *fontData = &gFile_FONTS_BIN[fontID];

        if (fontData->loadedFonts[0] > 0) {
            fontData->loadedFonts[0]--;

            if (fontData->loadedFonts[0] == 0) {
                s32 i = 0;
                while (i < FONTDATA_MAX_TEXTURES && fontData->textureID[i] != -1) {
                    texture_destroy(fontData->texturePointers[i]);
                    fontData->texturePointers[i] = NULL;
                    i++;
                }
            }
        }
    }
}

// set_dialogue_box_background_pos
void func_8000CC8C(s32 dialogueBoxIndex, s32 xPos, s32 yPos, s32 param4) {
    DialogueBoxBackground *background;
    
    if (dialogueBoxIndex < 0 || dialogueBoxIndex >= DIALOGUEBOXBACKGROUND_COUNT) {
        return;
    }

    background = &gDialogueBoxBackground[dialogueBoxIndex];

    if (xPos == -0x8000) {
        background->xpos = background->width >> 1;
    } else {
        background->xpos = xPos;
    }

    if (yPos == -0x8000) {
        background->ypos = background->height >> 1;
    } else {
        background->ypos = yPos;
    }
}


void func_8000CCFC(Gfx **gdl, s32 dialogueBoxIndex, char *text, u32 alignmentFlags) {
    if (dialogueBoxIndex >= 0 && dialogueBoxIndex < DIALOGUEBOXBACKGROUND_COUNT) {
        DialogueBoxBackground *temp = &gDialogueBoxBackground[dialogueBoxIndex];

        caption_func(gdl, temp, text, alignmentFlags, 1.0f);
    }
}

void func_8000CD58(Gfx **gdl, s32 dialogueBoxIndex, s32 xPos, s32 yPos, char *text, u32 alignmentFlags) {
    if (dialogueBoxIndex >= 0 && dialogueBoxIndex < DIALOGUEBOXBACKGROUND_COUNT) {
        DialogueBoxBackground *background = &gDialogueBoxBackground[dialogueBoxIndex];

        if (xPos == -0x8000) {
            background->xpos = background->width >> 1;
        } else {
            background->xpos = xPos;
        }

        if (yPos == -0x8000) {
            background->ypos = background->height >> 1;
        } else {
            background->ypos = yPos;
        }

        if (alignmentFlags != 0) {
            caption_func(gdl, background, text, alignmentFlags, 1.0f);
        } else {
            func_8000DD74(gdl, background, text, 1.0f);
        }
    }
}

#if NON_MATCHING
void caption_func(Gfx** gdl, DialogueBoxBackground* box, char* text, u32 alignmentFlags, f32 scisScale) {
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
    s32 textureIndex;
    FontData* fontData;
    s16 newData;
    u8 curChar;

    lastTextureIndex = -1;
    xAlignmentDiff = -1;
    if (box->font == 0xFF) {
        return;
    }

    if (text == NULL) {
        return;
    }

    xpos = box->xpos;
    ypos = box->ypos;
    fontData = &gFile_FONTS_BIN[box->font];
   
    gSPLoadGeometryMode(*gdl, G_SHADE | G_SHADING_SMOOTH);
    dl_apply_geometry_mode(gdl);
    
    gDPSetOtherMode(*gdl,
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2);
    dl_apply_other_mode(gdl);
    
    if (box != &gDialogueBoxBackground[0]) {
        scisOffset = ((f32)((box->y2 - box->y1) + 1) / 2) * scisScale;
        scisPos = (box->y1 + box->y2) >> 1;
        gDPSetScissor((*gdl)++, 0, box->x1, scisPos - scisOffset, box->x2, scisPos + scisOffset);
    }
    if (alignmentFlags & 4) {
        xAlignmentDiff = func_8000D920(box, text, xpos, box->font);
        xpos -= xAlignmentDiff >> 1;
    }
    if (alignmentFlags & 2) {
        ypos = (ypos - fontData->y) + 1;
    }
    if (alignmentFlags & 8) {
        ypos -= fontData->y >> 1;
    }

    if (box->textBGColourA != 0) {
        dl_set_env_color(gdl, box->textBGColourR, box->textBGColourG, box->textBGColourB, box->textBGColourA);
        
        if (xAlignmentDiff == -1) {
            xAlignmentDiff = func_8000D920(box, text, xpos, box->font);
        }

        textureLrx = (xpos + xAlignmentDiff) - 1;
        textureLry = (ypos + fontData->y) - 1;
        
        gDPSetCombineLERP((*gdl), 
            0, 0, 0, ENVIRONMENT, 
            0, 0, 0, ENVIRONMENT, 
            0, 0, 0, ENVIRONMENT, 
            0, 0, 0, ENVIRONMENT);
        dl_apply_combine(gdl);

        gDPSetOtherMode((*gdl),
            G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
            G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2);
        dl_apply_other_mode(gdl);

        gDPFillRectangle((*gdl)++, xpos + box->x1, ypos + box->y1, textureLrx + box->x1, textureLry + box->y1);
        
        gDLBuilder->needsPipeSync = 1;
    }
    
    dl_set_prim_color(gdl, 255, 255, 255, box->opacity);
    dl_set_env_color(gdl, box->textColourR, box->textColourG, box->textColourB, box->textColourA);
    
    gDPSetCombineLERP(*gdl, 
        ENVIRONMENT, TEXEL0, ENV_ALPHA, TEXEL0, 
        TEXEL0,      0,      PRIMITIVE, 0, 
        ENVIRONMENT, TEXEL0, ENV_ALPHA, TEXEL0, 
        TEXEL0,      0,      PRIMITIVE, 0);
    dl_apply_combine(gdl);

    gDPSetOtherMode(*gdl, 
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2);
    dl_apply_other_mode(gdl);
    
    charIndex = 0;
    xpos += box->textOffsetX;
    ypos += box->textOffsetY;
    xpos <<= 8;
    
    if (alignmentFlags & 1) {
        while ((text[charIndex] != '\0') && (text[charIndex] != (char)0xFF) && (box->y2 >= ypos)) {
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
                    xpos = box->textOffsetX << 8;
                    ypos += fontData->y;
                    break;
                case '\t':
                    xpos -= (fontData->charHeight - ((xpos - box->textOffsetX) % fontData->charHeight)) << 8;
                    break;
                case '\v': // Vertical tab
                    ypos += fontData->y;
                    break;
                case '\r':
                    xpos = box->textOffsetX << 8;
                    break;
                default:
                    xpos -= fontData->charWidth << 8;
                    break;
                }
            } else {
                curChar -= 0x20;
                textureIndex = fontData->letters[curChar].textureIndex;
                if (0xFF != textureIndex) {
                    if (lastTextureIndex != textureIndex) {
                        lastTextureIndex = textureIndex;
                        tex = fontData->texturePointers[textureIndex];
                        set_textures_on_gdl(gdl, tex, NULL, 0, 0, 0, 0);
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
                textureUlx = (xpos >> 6) + ((box->x1 + textureWidth) * 4);
                textureUly = (box->y1 + ypos + textureHeight) * 4;
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
            xpos -= (s32)(box->unk20 * 255.0f);
        }
    } else {
        for (charIndex = 0; (text[charIndex] != '\0') && (text[charIndex] != (char)0xFF) && (box->y2 >= ypos); charIndex++) {
            curChar = (u8)text[charIndex];
            newData = FALSE;
            charSpace = 0;
            if (curChar <= 0x20) {
                switch (curChar) {
                case ' ':
                    xpos += fontData->charWidth << 8;
                    break;
                case '\n':
                    xpos = box->textOffsetX << 8;
                    ypos += fontData->y;
                    break;
                case '\t':
                    xpos += (fontData->charHeight - ((xpos - box->textOffsetX) % fontData->charHeight)) << 8;
                    break;
                case '\v': // Vertical tab
                    ypos += fontData->y;
                    break;
                case '\r':
                    xpos = box->textOffsetX << 8;
                    break;
                default:
                    xpos += fontData->charWidth << 8;
                    break;
                }
            } else {
                curChar -= 0x20;
                textureIndex = fontData->letters[curChar].textureIndex;
                if (0xFF != textureIndex) {
                    if (lastTextureIndex != textureIndex) {
                        lastTextureIndex = textureIndex;
                        tex = fontData->texturePointers[textureIndex];
                        set_textures_on_gdl(gdl, tex, NULL, 0, 0, 0, 0);
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
                textureUlx = ((box->x1 + textureWidth) * 4) + (xpos >> 6);
                textureUly = ((box->y1 + ypos) + textureHeight) * 4;
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
            
            if ((gCompactKerning) && (charSpace != 0)) {
                charSpace -= 1;
            }
            
            xpos += (charSpace << 8);
            xpos += (s32)(box->unk20 * 255.0f);
        }
    }
    
    box->xpos = (xpos >> 8) - box->textOffsetX;
    box->ypos = ypos - box->textOffsetY;
    
    if (box != gDialogueBoxBackground) {
        func_80002490(gdl);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/fonts/caption_func.s")
#endif

s32 func_8000D8C8(s32 dialogueBoxIndex, char *text, s32 x, s32 fontID) {
    gDialogueBoxBackground[dialogueBoxIndex].unk20 = 0.0f;

    return func_8000D920(&gDialogueBoxBackground[dialogueBoxIndex], text, x, fontID);
}

// get_text_width but a little different
s32 func_8000D920(DialogueBoxBackground *box, char *text, s32 x, s32 fontID) {
    s32 diffX;
    s32 thisDiffX;
    FontData *fontData;
    s32 i;
    u8 c;
    s32 counter;

    counter = 0;

    if (text == NULL) {
        return 0;
    }

    if (fontID < 0) {
        fontID = gDialogueBoxBackground[0].font;
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
            counter++;
            c -= 0x20; // Convert lower case to upper case ASCII

            if (fontData->letters[c].textureIndex != 0xFF) {
                if (fontData->x == 0) {
                    diffX += fontData->letters[c].kerning;
                } else {
                    diffX += fontData->x;
                }
            }
        }

        if (gCompactKerning && diffX != thisDiffX) {
            diffX--;
        }
    }

    if (counter != 0) {
        diffX = (s32)(diffX + box->unk20 * counter);
    }

    return diffX - x;
}

// set_current_dialogue_box_coords
void func_8000DAA4(s32 dialogueBoxIndex, s32 x1, s32 y1, s32 x2, s32 y2) {
    if (dialogueBoxIndex > 0 && dialogueBoxIndex < DIALOGUEBOXBACKGROUND_COUNT) {
        DialogueBoxBackground *box = &gDialogueBoxBackground[dialogueBoxIndex];
        box->xpos = 0;
        box->ypos = 0;
        if (x1 < x2) {
            box->x1 = (s16) x1;
            box->x2 = (s16) x2;
        } else {
            box->x2 = (s16) x1;
            box->x1 = (s16) x2;
        }
        if (y1 < y2) {
            box->y1 = (s16) y1;
            box->y2 = (s16) y2;
        } else {
            box->y2 = (s16) y1;
            box->y1 = (s16) y2;
        }
        box->width = (box->x2 - box->x1) + 1;
        box->height = (box->y2 - box->y1) + 1;
        box->unk20 = 0.0f;
    }
}

void func_8000DB50(s32 dialogueBoxIndex, s32 fontID) {
    if (dialogueBoxIndex >= 0 && dialogueBoxIndex < DIALOGUEBOXBACKGROUND_COUNT) {
        DialogueBoxBackground *box = &gDialogueBoxBackground[dialogueBoxIndex];
        if (fontID < gNumFonts) {
            box->font = fontID;
            if (gFile_FONTS_BIN[fontID].loadedFonts[0] == 0) {
            
            }
        } else {

        }
    } else {
        
    }
}

void func_8000DBCC(s32 dialogueBoxIndex, f32 param2) {
    if (dialogueBoxIndex >= 0 && dialogueBoxIndex < DIALOGUEBOXBACKGROUND_COUNT) {
        DialogueBoxBackground *box = &gDialogueBoxBackground[dialogueBoxIndex];
        box->unk20 = param2;
    }
}

void func_8000DC0C(s32 dialogueBoxIndex, s32 red, s32 green, s32 blue, s32 alpha) {
    if (dialogueBoxIndex > 0 && dialogueBoxIndex < DIALOGUEBOXBACKGROUND_COUNT) {
        DialogueBoxBackground *box = &gDialogueBoxBackground[dialogueBoxIndex];
        box->backgroundColourR = red;
        box->backgroundColourG = green;
        box->backgroundColourB = blue;
        box->backgroundColourA = alpha;
    }
}

void func_8000DC54(s32 dialogueBoxIndex, s32 red, s32 green, s32 blue, s32 alpha, s32 opacity) {
    if (dialogueBoxIndex > 0 && dialogueBoxIndex < DIALOGUEBOXBACKGROUND_COUNT) {
        DialogueBoxBackground *box = &gDialogueBoxBackground[dialogueBoxIndex];
        box->textColourR = red;
        box->textColourG = green;
        box->textColourB = blue;
        box->textColourA = alpha;
        box->opacity = opacity;
    }
}

void func_8000DCA4(s32 dialogueBoxIndex, s32 red, s32 green, s32 blue, s32 alpha) {
    if (dialogueBoxIndex > 0 && dialogueBoxIndex < DIALOGUEBOXBACKGROUND_COUNT) {
        DialogueBoxBackground *box = &gDialogueBoxBackground[dialogueBoxIndex];
        box->textBGColourR = red;
        box->textBGColourG = green;
        box->textBGColourB = blue;
        box->textBGColourA = alpha;
    }
}

void func_8000DCEC(s32 dialogueBoxIndex, s32 x, s32 y) {
    if (dialogueBoxIndex > 0 && dialogueBoxIndex < DIALOGUEBOXBACKGROUND_COUNT) {
        DialogueBoxBackground *box = &gDialogueBoxBackground[dialogueBoxIndex];
        box->textOffsetX += x;
        box->textOffsetY += y;
    }
}

void func_8000DD38(s32 dialogueBoxIndex) {
    if (dialogueBoxIndex > 0 && dialogueBoxIndex < DIALOGUEBOXBACKGROUND_COUNT) {
        DialogueBoxBackground *box = &gDialogueBoxBackground[dialogueBoxIndex];
        box->textOffsetX = 0;
        box->textOffsetY = 0;
    }
}

#if NON_EQUIVALENT
void func_8000DD74(Gfx** gdl, DialogueBoxBackground* box, char* text, f32 scisScale) {
    u8 charBuffer[128];
    u8 lastTextureIndex;
    FontData* fontData;
    s32 ypos;
    s32 xpos;
    s32 scisOffset;
    s32 rectUly;
    s32 scisPos;
    s32 rectUlx;
    s32 yStart;
    s32 wordWidth;
    s32 loopCond;
    s32 xStart;
    s32 bufIdx;
    s32 wordCharCount;
    s32 wordWrapped;
    u8 textureIndex;
    u8 curChar;
    s32 charIndex;
    s32 rectLrx;
    s32 rectLry;
    s32 textureS;
    s32 textureT;

    lastTextureIndex = 0xFF;
    
    if (text == NULL) {
        return;
    }
    if (box->font == 0xFF) {
        return;
    }
    
    xpos = box->xpos;
    ypos = box->ypos;
    fontData = &gFile_FONTS_BIN[box->font];
    
    gSPLoadGeometryMode(*gdl, G_SHADE | G_SHADING_SMOOTH);
    dl_apply_geometry_mode(gdl);
    
    if (box != gDialogueBoxBackground) {
        scisPos = (s32) (box->y2 + box->y1) >> 1;
        scisOffset = (s32) (((f32) ((box->y2 - box->y1) + 1) / 2) * scisScale);
        gDPSetScissor((*gdl)++, 0, box->x1, scisPos - scisOffset, box->x2, scisPos + scisOffset);
    }
    
    dl_set_prim_color(gdl, 255, 255, 255, box->opacity);
    dl_set_env_color(gdl, box->textColourR, box->textColourG, box->textColourB, box->textColourA);
    
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
    
    charIndex = 0;
    loopCond = 0;
    
    while (loopCond >= 0) {
        if (xpos >= box->width) {
            xpos = box->xpos;
            ypos += fontData->y;
            wordWrapped = 1;
        } else {
            wordWrapped = 0;
        }

        if ((box->textOffsetY + ypos) >= box->height) {
            // end if beyond box height
            loopCond = -1;
        } else {
            loopCond = 0;
        }
        
        while (loopCond == 0) {
            curChar = (u8) text[charIndex];
            // if non-printable char
            if ((curChar <= 0x20) || (curChar >= 0x80)) {
                switch (curChar) {
                case '\0':
                    loopCond = -1;
                    break;
                case '\t':
                    xpos = (xpos + fontData->charHeight) - (xpos % fontData->charHeight);
                    break;
                case '\n':
                    wordWrapped = 0;
                    xpos = box->xpos;
                    ypos += fontData->y;
                    break;
                case '\v':
                    wordWrapped = 0;
                    ypos += fontData->y;
                    break;
                case '\r':
                    wordWrapped = 0;
                    xpos = box->xpos;
                    break;
                default:
                    if (wordWrapped == 0) {
                        xpos += fontData->charWidth;
                    }
                    break;
                }
                if (xpos >= box->width) {
                    xpos = box->xpos;
                    ypos += fontData->y;
                    wordWrapped = 1;
                }
                if ((box->textOffsetY + ypos) >= box->height) {
                    loopCond = -1;
                }
                charIndex += 1;
            } else {
                loopCond = 1;
            }
        }
        
        wordWidth = 0;
        if (loopCond > 0) {
            wordCharCount = 0;

            do {
                charBuffer[wordCharCount] = curChar;
                curChar = (curChar - 0x20);
                wordCharCount += 1;
                charIndex += 1;
                if (fontData->letters[curChar].textureIndex != 0xFF) {
                    if (fontData->x != 0) {
                        wordWidth += fontData->x;
                    } else {
                        wordWidth += fontData->letters[curChar].kerning;
                    }
                } else {
                    wordWidth += fontData->charWidth;
                }
                curChar = (u8) text[charIndex];
            } while (wordCharCount < 128 && ((s32) curChar > 0x20) && ((s32) curChar < 0x100));

            if (((xpos + wordWidth) >= box->width) && (xpos != 0)) {
                xpos = box->xpos;
                ypos += fontData->y;
            }
            yStart = box->textOffsetY + ypos;
            if (yStart < box->height) {
                xStart = box->textOffsetX + xpos;
                xpos += wordWidth;
                if ((fontData->y + yStart) > 0) {
                    for (bufIdx = 0; bufIdx < wordCharCount; bufIdx++) {
                        curChar = (charBuffer[bufIdx] - 0x20);
                        textureIndex = fontData->letters[curChar].textureIndex;
                        if (textureIndex != 0xFF) {
                            if (lastTextureIndex != textureIndex) {
                                lastTextureIndex = textureIndex;
                                set_textures_on_gdl(gdl, fontData->texturePointers[textureIndex], NULL, 0, 0, 0, 0);
                            }
                            rectUlx = (fontData->letters[curChar].offsetX + box->x1 + xStart) * 4;
                            rectUly = (fontData->letters[curChar].offsetY + box->y1 + yStart) * 4;
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
    
    box->xpos = xpos;
    box->ypos = ypos;
    if (box != gDialogueBoxBackground) {
        func_80002490(gdl);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/fonts/func_8000DD74.s")
#endif

DialogueTextElement *func_8000E424(s32 dialogueBoxIndex, char *text, s32 number, s32 flags) {
    if (dialogueBoxIndex < 0 || dialogueBoxIndex >= DIALOGUEBOXBACKGROUND_COUNT) {
        return NULL;
    } else {
        return func_8000E49C(dialogueBoxIndex, 
            gDialogueBoxBackground[dialogueBoxIndex].xpos, 
            gDialogueBoxBackground[dialogueBoxIndex].ypos, 
            text, number, flags);
    }
}

DialogueTextElement *func_8000E49C(s32 dialogueBoxIndex, s32 posX, s32 posY, char *text, s32 number, s32 flags) {
    s32 width;
    DialogueBox *textBox;
    char buffer[256];
    DialogueTextElement *ret;
    s32 i;
    DialogueBoxBackground *bg;
    DialogueBox **textBoxPtr;
    FontData *fontData;

    if (text == NULL) {
        return NULL;
    }

    if (dialogueBoxIndex < 0 || dialogueBoxIndex >= DIALOGUEBOXBACKGROUND_COUNT) {
        return NULL;
    }

    if (number < 0 || number >= 255) {
        number = 0;
    }

    for (i = 0, ret = NULL; i < 64 && ret == NULL; i++) {
        if (gDialogueText[i].number == 255) {
            ret = &gDialogueText[i];
        }
    }

    if (ret != NULL) {
        bg = &gDialogueBoxBackground[dialogueBoxIndex];
        if (posX == -0x8000) {
            posX = bg->width >> 1;
        }
        if (posY == -0x8000) {
            posY = bg->height >> 1;
        }
        if (bg->font != 0xFF) {
            fontData = &gFile_FONTS_BIN[bg->font];
            if (flags & 4) {
                func_8000F2E8(text, buffer, number);
                width = func_8000D920(bg, buffer, posX, bg->font);
                posX = posX - (width >> 1);
            }
            if (flags & 2) {
                posY = (posY - fontData->y) + 1;
            }
            if (flags & 8) {
                posY -= fontData->y >> 1;
            }
        }
        if (bg->textBox == NULL) {
            bg->textBox = (DialogueBox *) ret;
            ret->nextBox = NULL;
        } else {
            textBoxPtr = &bg->textBox;
            textBox = *textBoxPtr;
            while (textBox != NULL && number < textBox->textNum) {
                textBoxPtr = &textBox->nextBox;
                textBox = textBox->nextBox;
            }
            *textBoxPtr = (DialogueBox *) ret;
            ret->nextBox = textBox;
        }
        ret->number = number;
        ret->text = text;
        ret->posX = posX;
        ret->posY = posY;
        ret->offsetX = 0;
        ret->offsetY = 0;
        ret->textColourR = bg->textColourR;
        ret->textColourG = bg->textColourG;
        ret->textColourB = bg->textColourB;
        ret->textColourA = bg->textColourA;
        ret->textBGColourR = bg->textBGColourR;
        ret->textBGColourG = bg->textBGColourG;
        ret->textBGColourB = bg->textBGColourB;
        ret->textBGColourA = bg->textBGColourA;
        ret->opacity = bg->opacity;
        ret->font = bg->font;
        ret->flags = bg->flags;
        ret->unk1C = flags;
        ret->unk20 = bg->unk20;
    }

    return ret;
}

void func_8000E758(s32 dialogueBoxIndex, DialogueBox *dialogueBox) {
    DialogueBoxBackground *dialogueBoxTemp;
    DialogueBox *dialogueTextBox;
    DialogueBox **dialogueTextBoxTemp;

    if (dialogueBoxIndex < 0 || dialogueBoxIndex >= DIALOGUEBOXBACKGROUND_COUNT) {
        return;
    }

    dialogueBoxTemp = &gDialogueBoxBackground[dialogueBoxIndex];
    dialogueTextBoxTemp = &dialogueBoxTemp->textBox;
    dialogueTextBox = dialogueBoxTemp->textBox;
    while ((dialogueTextBox != NULL) && (dialogueTextBox != dialogueBox)) {
        dialogueTextBoxTemp = &dialogueTextBox->nextBox;
        dialogueTextBox = dialogueTextBox->nextBox;
    }
    if (dialogueTextBox != NULL) {
        *dialogueTextBoxTemp = dialogueTextBox->nextBox;
        dialogueBox->textNum = 0xFF;
    }
}

void func_8000E7D8(s32 dialogueBoxIndex) {
    DialogueBoxBackground *dialogueBox;
    DialogueBox *dialogueTextBox, *dialogueTextBoxTemp;

    if (dialogueBoxIndex < 0 || dialogueBoxIndex >= DIALOGUEBOXBACKGROUND_COUNT) {
        return;
    }

    dialogueBox = &gDialogueBoxBackground[dialogueBoxIndex];
    dialogueTextBox = dialogueBox->textBox;
    if (dialogueTextBox != NULL) {
        dialogueTextBoxTemp = dialogueTextBox; // This seems redundant.
        while (dialogueTextBoxTemp != NULL) {
            dialogueTextBoxTemp->textNum = 0xFF;
            dialogueTextBoxTemp = dialogueTextBoxTemp->nextBox;
        }
        dialogueBox->textBox = NULL;
    }
}

void func_8000E838(s32 dialogueBoxIndex, DialogueTextElement *box, s32 x, s32 y, s32 flags) {
    FontData *fontData;
    DialogueBoxBackground *dialogueBox;

    if (dialogueBoxIndex < 0 || dialogueBoxIndex >= DIALOGUEBOXBACKGROUND_COUNT) {
        return;
    }

    dialogueBox = &gDialogueBoxBackground[dialogueBoxIndex];

    if (box != NULL) {
        if (box->font != 0xFF) {
            fontData = &gFile_FONTS_BIN[box->font];
            if (flags != 4) {
                switch (flags) {
                    case 1:
                        y *= fontData->y;
                        break;
                    case 2:
                        y *= (dialogueBox->height / fontData->y) * fontData->y;
                        break;
                }
                box->offsetX += x;
                box->offsetY += y;
                return;
            }
            box->offsetX = 0;
            box->offsetY = 0;
            return;
        }
    }
}

void func_8000E958(s32 dialogueBoxIndex) {
    if (dialogueBoxIndex < 0 || dialogueBoxIndex >= DIALOGUEBOXBACKGROUND_COUNT) {
        return;
    }

    gDialogueBoxBackground[dialogueBoxIndex].flags |= 0x8000;
}

void func_8000E998(s32 dialogueBoxIndex) {
    if (dialogueBoxIndex < 0 || dialogueBoxIndex >= DIALOGUEBOXBACKGROUND_COUNT) {
        return;
    }

    gDialogueBoxBackground[dialogueBoxIndex].flags &= ~0x8000;
}

s32 func_8000E9D8(s32 fontID) {
    return gFile_FONTS_BIN[fontID].y;
}

void func_8000EA04(s32 dialogueBoxIndex) {
    if (dialogueBoxIndex < 0 || dialogueBoxIndex >= DIALOGUEBOXBACKGROUND_COUNT) {
        return;
    }

    gDialogueBoxBackground[dialogueBoxIndex].flags |= 1;
}

void func_8000EA44(s32 dialogueBoxIndex) {
    if (dialogueBoxIndex < 0 || dialogueBoxIndex >= DIALOGUEBOXBACKGROUND_COUNT) {
        return;
    }

    gDialogueBoxBackground[dialogueBoxIndex].flags &= ~1;
}

void func_8000EA84(s32 dialogueBoxIndex) {
    if (dialogueBoxIndex < 0 || dialogueBoxIndex >= DIALOGUEBOXBACKGROUND_COUNT) {
        return;
    }

    gDialogueBoxBackground[dialogueBoxIndex].flags |= 0x4000;
}

void func_8000EAC4(s32 dialogueBoxIndex) {
    if (dialogueBoxIndex < 0 || dialogueBoxIndex >= DIALOGUEBOXBACKGROUND_COUNT) {
        return;
    }

    gDialogueBoxBackground[dialogueBoxIndex].flags &= ~0x4000;
}

void func_8000EB04(Gfx **gdl, s32 **mtxs, s32 **vtxs) {
    s32 i;

    for (i = 1; i < DIALOGUEBOXBACKGROUND_COUNT; i++) {
        if (gDialogueBoxBackground[i].flags & 0x8000) {
            if (gDialogueBoxBackground[i].flags & 0x4000) {
                func_8000EF18(gdl, mtxs, vtxs, i);
            } else {
                func_8000EF18(gdl, NULL, NULL, i);
            }
        }
    }
}

void func_8000EBC8(char **outString, s32 number) {
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
        pow = D_8008C7E0[i++];
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

void func_8000EE54(Gfx **gdl, s32 ulx, s32 uly, s32 lrx, s32 lry) {
    u32 resolution = get_some_resolution_encoded();
    u32 width = RESOLUTION_WIDTH(resolution);
    u32 height = RESOLUTION_HEIGHT(resolution);

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


void func_8000EF18(Gfx **gdl, s32 **mtxs, s32 **vtxs, s32 dialogueBoxIndex) {
    DialogueBoxBackground *box;
    DialogueBox *textBox;
    s32 vidMode;
    s32 i;
    s32 x1;
    s32 x2;
    char text[256];
    s32 y1;
    s32 y2;
    s32 height;
    
    if (dialogueBoxIndex < 0 || dialogueBoxIndex >= DIALOGUEBOXBACKGROUND_COUNT) {
        return;
    }

    box = &gDialogueBoxBackground[dialogueBoxIndex];
    if (box->backgroundColourA != 0) {
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

        if (box->x2 - box->x1 < 10 || box->y2 - box->y1 < 10) {
            func_8000EE54(gdl, box->x1 - 2, box->y1 - 2, box->x2 + 2, box->y2 + 2);
        } else {
            func_8000EE54(gdl, box->x1 - 2, box->y1 + 2, box->x1 + 2, box->y2 - 2);
            func_8000EE54(gdl, box->x1 - 2, box->y1 - 2, box->x2 + 2, box->y1 + 2);
            func_8000EE54(gdl, box->x2 - 2, box->y1 + 2, box->x2 + 2, box->y2 - 2);
            func_8000EE54(gdl, box->x1 - 2, box->y2 - 2, box->x2 + 2, box->y2 + 2);
        }

        dl_set_env_color(gdl, box->backgroundColourR, box->backgroundColourG, box->backgroundColourB, box->backgroundColourA);

        i = 0;
        height = box->y2 - box->y1;

        vidMode = get_video_mode();
        if (vidMode == 3) {
            box->x1 += 96;
            box->x2 += 96;
        }

        if (height < 9) {
            i = 45 - (height * 5);
        }

        // Loops through D_8008C804, rendering fillrects until it forms a rounded rectangle.
        // The array determines the width and height of each entry before drawing it.
        for (; D_8008C804[i] >= 0; i += 5) {
            x1 = D_8008C804[i] + box->x1;
            y1 = (D_8008C804[i + 1]) ? D_8008C804[i + 2] + box->y2
                                     : D_8008C804[i + 2] + box->y1;
            x2 = box->x2 - D_8008C804[i];
            y2 = (D_8008C804[i + 3]) ? D_8008C804[i + 4] + box->y2
                                     : D_8008C804[i + 4] + box->y1;
            func_8000EE54(gdl, x1, y1, x2, y2);
        }
    }

    textBox = box->textBox;
    while (textBox != NULL) {
        box->xpos = textBox->x1 + textBox->x2;
        box->ypos = textBox->y1 + textBox->y2;
        box->textColourR = textBox->textColourR;
        box->textColourG = textBox->textColourG;
        box->textColourB = textBox->textColourB;
        box->textColourA = textBox->textColourA;
        box->textBGColourR = textBox->textBGColourR;
        box->textBGColourG = textBox->textBGColourG;
        box->textBGColourB = textBox->textBGColourB;
        box->textBGColourA = textBox->textBGColourA;
        box->opacity = textBox->opacity;
        box->font = textBox->font;
        box->unk20 = textBox->unk20;
        
        func_8000F2E8(textBox->text, text, textBox->textNum);

        if (textBox->unk1A & 1) {
            func_8000DD74(gdl, box, text, 1.0f);
        } else {
            caption_func(gdl, box, text, textBox->unk1C & 1, 1.0f);
        }

        textBox = textBox->nextBox;
    }
}

void func_8000F2E8(char *input, char *output, s32 number) {
    while (*input) {
        if (*input == '~') { // ~ is equivalent to a %d.
            // output the number as part of the string
            func_8000EBC8(&output, number);
            input++;
        } else {
            *output = *input;
            input++;
            output++;
        }
    }
    *output = '\0'; // null terminator
}
