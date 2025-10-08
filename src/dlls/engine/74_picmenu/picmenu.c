#include "dlls/engine/74_picmenu.h"

#include "PR/os.h"
#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "sys/rand.h"
#include "sys/controller.h"
#include "sys/fonts.h"
#include "sys/gfx/texture.h"
#include "dll.h"
#include "functions.h"
#include "macros.h"

static const char str_1[] = "PICMENU: maxitems exceeded\n";
static const char str_2[] = "UPLINK overflow=%d\n";
static const char str_3[] = "DOWNLINK overflow=%d\n";
static const char str_4[] = "LEFTLINK overflow=%d\n";
static const char str_5[] = "RIGHTLINK overflow=%d\n";
static const char str_6[] = "PICMENU: tex overflow\n";

/*0*/ static PicMenuSounds sDefaultSounds = {
    /*move*/    0x5D5, 
    /*select*/  0x5D3, 
    /*back*/    0x5D4
};

/*C*/ static PicMenuTexture sTextures[] = {
    //     ptr   ID     width
    /*0*/{ NULL, 0x314, 40 },
    /*1*/{ NULL, 0x315, 40 },
    /*2*/{ NULL, 0x317, 80 },
    /*3*/{ NULL, 0x319, 80 },
    /*4*/{ NULL, 0x318, 40 },
    /*5*/{ NULL, 0x31A, 20 }
};

/*0*/ static PicMenuItem sItems[40];
/*960*/ static s8 sAllowButtons;
/*961*/ static s8 sSelectedItem; // index of selected item
/*962*/ static s8 sItemCount;
/*963*/ static s8 sHighlightReverse;
/*964*/ static s16 sHighlightAlpha;
/*966*/ static s16 sOpacity;
/*968*/ static PicMenuSounds *sSounds;
/*96C*/ static s16 sTextColorR;
/*96E*/ static s16 sTextColorG;
/*970*/ static s16 sTextColorB;
/*972*/ static s16 sTextHighlightR;
/*974*/ static s16 sTextHighlightG;
/*976*/ static s16 sTextHighlightB;

static u16 picmenu_calculate_auto_width(const char *text, s32 fontID);
static void picmenu_generate_item_background(PicMenuItem *item);
void picmenu_calculate_redraw_area();
void picmenu_calculate_items_to_redraw();

void picmenu_ctor(void *self) {
    s32 i;

    for (i = 0; i < (s32)ARRAYCOUNT(sTextures); i++) {
        sTextures[i].texture = queue_load_texture_proxy(sTextures[i].textureID);
    }

    sOpacity = 255;
}

void picmenu_dtor(void *self) {
    s32 i;

    for (i = 0; i < (s32)ARRAYCOUNT(sTextures); i++) {
        texture_destroy(sTextures[i].texture);
    }
}

// export 0
void picmenu_set_items(PicMenuItem *items, s32 count, 
        s32 defaultItem, PicMenuSounds *sounds, s32 param5, s32 param6, 
        s32 textColorR, s32 textColorG, s32 textColorB, 
        s32 textHighlightR, s32 textHighlightG, s32 textHighlightB) {
    s32 i;
    s32 width;
    PicMenuItem *item;
    PicMenuItem *leftItem;

    if (count > (s32)ARRAYCOUNT(sItems)) {
        return;
    }

    sItemCount = count;
    sHighlightAlpha = 255;
    sSelectedItem = defaultItem;
    sHighlightReverse = FALSE;
    sAllowButtons = FALSE;

    bcopy(items, sItems, count * sizeof(PicMenuItem));

    for (i = 0; i < count; i++) {
        item = &sItems[i];

        if (items[i].texture.asID != -1) {
            item->texture.asPtr = queue_load_texture_proxy(items[i].texture.asID);
        } else {
            item->texture.asPtr = NULL;
        }

        if (item->flags & PICMENU_AUTO_WIDTH) {
            if (((!count) && (!count)) && (!count)){} // fakematch

            width = picmenu_calculate_auto_width(item->text, item->fontID);
            item->outerWidth = width;
            item->innerWidth = width;
        }

        if (item->flags & PICMENU_HAS_BACKGROUND) {
            picmenu_generate_item_background(item);
        }

        if (item->leftLink != -1 && item->flags & PICMENU_INLINE) {
            leftItem = &sItems[item->leftLink];
            item->itemX = leftItem->itemX + leftItem->outerWidth;
            item->textX = leftItem->textX + leftItem->outerWidth;
        }

        if (item->flags & PICMENU_ALIGN_ITEM_CENTER) {
            if (1) { } if (1) { } if (1) { } if (1) { } if (1) { } if (1) { } // fakematch

            item->itemX -= (item->outerWidth >> 1);
            item->textX = item->itemX;
        }

        if (item->flags & PICMENU_ALIGN_TEXT_CENTER) {
            item->textX = item->itemX + (item->innerWidth >> 1);
        }

        item->redrawFrames = 4;
    }

    sTextColorR = textColorR;
    sTextColorG = textColorG;
    sTextColorB = textColorB;
    sTextHighlightR = textHighlightR;
    sTextHighlightG = textHighlightG;
    sTextHighlightB = textHighlightB;
    sSounds = sounds == NULL ? &sDefaultSounds : sounds;
}

// export 1
void picmenu_clear_items() {
    s32 i;

    for (i = 0; i < sItemCount; i++) {
        if (sItems[i].texture.asPtr != NULL) {
            texture_destroy(sItems[i].texture.asPtr);
        }
    }

    sItemCount = 0;
}

// just needs picmenu_calculate_items_to_redraw and picmenu_calculate_redraw_area to be static
#ifdef NON_MATCHING
// export 2
PicMenuAction picmenu_update() {
    s32 buttons;
    s32 ret;
    s8 joyXSign;
    s8 joyYSign;
    PicMenuItem *item;
    
    item = &sItems[sSelectedItem];

    if (sItemCount == 0) {
        return PICMENU_ACTION_NONE;
    }

    ret = PICMENU_ACTION_NONE;

    get_joystick_menu_xy_sign(0, &joyXSign, &joyYSign);
    if (joyYSign != 0) {
        joyXSign = 0;
    }

    if (joyXSign != 0 || joyYSign != 0) {
        if (joyYSign < 0 && item->downLink != -1 && !(sItems[item->downLink].flags & PICMENU_INTANGIBLE)) {
            sSelectedItem = item->downLink;
            sHighlightAlpha = 255;

            if (sSounds->moveSoundID > -1) {
                gDLL_6_AMSFX->vtbl->play_sound(0, sSounds->moveSoundID, MAX_VOLUME, 0, 0, 0, 0);
            }
        } else if (joyYSign > 0 && item->upLink != -1 && !(sItems[item->upLink].flags & PICMENU_INTANGIBLE)) {
            sSelectedItem = item->upLink;
            sHighlightAlpha = 255;

            if (sSounds->moveSoundID > -1) {
                gDLL_6_AMSFX->vtbl->play_sound(0, sSounds->moveSoundID, MAX_VOLUME, 0, 0, 0, 0);
            }
        }
        
        if (item->overrideWith != -1) {
            item = &sItems[item->overrideWith];
            if (joyXSign < 0 && item->leftLink != -1) {
                sItems[sSelectedItem].overrideWith = item->leftLink;
                sHighlightAlpha = 255;

                if (sSounds->moveSoundID > -1) {
                    gDLL_6_AMSFX->vtbl->play_sound(0, sSounds->moveSoundID, MAX_VOLUME, 0, 0, 0, 0);
                }
            } else if (joyXSign > 0 && item->rightLink != -1) {
                sItems[sSelectedItem].overrideWith = item->rightLink;
                sHighlightAlpha = 255;

                if (sSounds->moveSoundID > -1) {
                    gDLL_6_AMSFX->vtbl->play_sound(0, sSounds->moveSoundID, MAX_VOLUME, 0, 0, 0, 0);
                }
            }
        } else {
            if (joyXSign < 0 && item->leftLink != -1 && !(sItems[item->leftLink].flags & PICMENU_INTANGIBLE)) {
                sSelectedItem = item->leftLink;
                sHighlightAlpha = 255;

                if (sSounds->moveSoundID > -1) {
                    gDLL_6_AMSFX->vtbl->play_sound(0, sSounds->moveSoundID, MAX_VOLUME, 0, 0, 0, 0);
                }
                
            } else if (joyXSign > 0 && item->rightLink != -1 && !(sItems[item->rightLink].flags & PICMENU_INTANGIBLE)) {
                sSelectedItem = item->rightLink;
                sHighlightAlpha = 255;

                if (sSounds->moveSoundID > -1) {
                    gDLL_6_AMSFX->vtbl->play_sound(0, sSounds->moveSoundID, MAX_VOLUME, 0, 0, 0, 0);
                }
            }
        }

        if (sSelectedItem < 0) {
            sSelectedItem = sItemCount - 1;
        }
        if (sItemCount <= sSelectedItem) {
            sSelectedItem = 0;
        }
    }

    if (sAllowButtons) {
        buttons = get_masked_button_presses(0);
        if (buttons & (A_BUTTON | START_BUTTON)) {
            if (!(sItems[sSelectedItem].flags & PICMENU_DISABLED) && get_gplay_bitstring(BIT_44F) == 0) {
                set_button_mask(0, A_BUTTON | START_BUTTON);

                if (sSounds->selectSoundID > -1) {
                    gDLL_6_AMSFX->vtbl->play_sound(0, sSounds->selectSoundID, MAX_VOLUME, 0, 0, 0, 0);
                }

                ret = PICMENU_ACTION_SELECT;
            }            
        } else if (buttons & B_BUTTON) {
            if (sSounds->backSoundID > -1) {
                gDLL_6_AMSFX->vtbl->play_sound(0, sSounds->backSoundID, MAX_VOLUME, 0, 0, 0, 0);
            }

            set_button_mask(0, B_BUTTON);

            ret = PICMENU_ACTION_BACK;
        }
    }

    if (sHighlightReverse) {
        sHighlightAlpha += delayByte * 5;
    } else {
        sHighlightAlpha -= delayByte * 5;
    }

    if (sHighlightAlpha >= 256) {
        sHighlightAlpha = 510 - sHighlightAlpha;
        sHighlightReverse ^= 1;
    } else if (sHighlightAlpha < 0) {
        sHighlightAlpha = -sHighlightAlpha;
        sHighlightReverse ^= 1;
    }

    sAllowButtons = TRUE;

    picmenu_calculate_items_to_redraw();
    picmenu_calculate_redraw_area();

    return ret;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/74_picmenu/picmenu_update.s")
#endif

// export 3
void picmenu_draw(Gfx **gdl) {
    s32 i;
    PicMenuItem *item;
    s32 x;
    s32 y;
    s32 k;
    s32 opacity;
    s16 r,g,b;

    for (i = 0; i < sItemCount; i++) {
        item = &sItems[i];

        if (item->flags & (PICMENU_INTANGIBLE | PICMENU_FLAG_INVISIBLE)) {
            item->redrawFrames -= 1;
            if (item->redrawFrames < 0) {
                item->redrawFrames = 0;
            }
            continue;
        }
        
        if (item->redrawFrames != 0 || item->flags & PICMENU_ALWAYS_REDRAW) {
            if (item->overrideWith != -1) {
                item = &sItems[item->overrideWith];
            }

            if (item->flags & PICMENU_HAS_BACKGROUND) {
                k = 0;
                
                x = item->itemX;
                y = item->itemY;

                if (item->flags & PICMENU_TRANSPARENT) {
                    opacity = (sOpacity << 7) >> 8;
                } else {
                    opacity = (sOpacity << 8) >> 8;
                }

                while (item->bgTextureIndices[k] != -1 && k < (s32)ARRAYCOUNT(item->bgTextureIndices)) {
                    func_8003825C(gdl, sTextures[item->bgTextureIndices[k]].texture, 
                        x, y, 0, 0, opacity, 0);
                    x += sTextures[item->bgTextureIndices[k]].width;
                    k++;
                }
            }

            if (item->flags & PICMENU_TRANSPARENT) {
                opacity = (sOpacity << 7) >> 8;
            } else {
                opacity = sOpacity;
            }

            if (item->flags & PICMENU_USE_TEXT_COLOR) {
                if (i == sSelectedItem) {
                    r = sTextColorR + (((sTextHighlightR - sTextColorR) * sHighlightAlpha) >> 8);
                    g = sTextColorG + (((sTextHighlightG - sTextColorG) * sHighlightAlpha) >> 8);
                    b = sTextColorB + (((sTextHighlightB - sTextColorB) * sHighlightAlpha) >> 8);

                    if (item->flags & PICMENU_TRANSPARENT) {
                        opacity = (sOpacity << 7) >> 8;
                    } else {
                        opacity = (sOpacity << 8) >> 8;
                    }

                    font_window_set_text_colour(1, 
                        r,
                        g,
                        b, 
                        255, 
                        opacity);
                } else {
                    font_window_set_text_colour(1, 
                        sTextColorR, 
                        sTextColorG, 
                        sTextColorB,
                        255, 
                        opacity);
                }
            } else {
                font_window_set_text_colour(1, 255, 255, 255, 0, opacity);
            }

            font_window_use_font(1, item->fontID);
            font_window_add_string_xy(1, item->textX, item->textY, item->text, 2, 
                item->flags & PICMENU_ALIGN_TEXT_CENTER ? ALIGN_TOP_CENTER : ALIGN_TOP_LEFT);

            if (!(item->flags & PICMENU_USE_TEXT_COLOR) && i == sSelectedItem) {
                font_window_set_text_colour(1, 255, 255, 255, 0, ((sHighlightAlpha + 1) * sOpacity) >> 8);
                font_window_use_font(1, item->highlightFontID);
                font_window_add_string_xy(1, item->textX, item->textY, item->text, 1, 
                    item->flags & PICMENU_ALIGN_TEXT_CENTER ? ALIGN_TOP_CENTER : ALIGN_TOP_LEFT);
            }

            if (item->flags & PICMENU_RAISED_EFFECT) {
                font_window_set_text_colour(1, 0, 0, 0, 255, (sOpacity * 150) >> 8);
                font_window_add_string_xy(1, item->textX - 1, item->textY - 1, item->text, 3, 
                    item->flags & PICMENU_ALIGN_TEXT_CENTER ? ALIGN_TOP_CENTER : ALIGN_TOP_LEFT);
            }

            if (item->texture.asPtr != NULL) {
                if (item->flags & PICMENU_HAS_BACKGROUND) {
                    func_8003825C(gdl, item->texture.asPtr,
                        item->itemX + 11, item->itemY, 0, 0, 
                        item->flags & PICMENU_TRANSPARENT ? 128 : 255, 0);
                } else {
                    func_8003825C(gdl, item->texture.asPtr,
                        item->itemX, item->itemY, 0, 0, 
                        item->flags & PICMENU_TRANSPARENT ? 128 : 255, 0);
                }
            }
        }

        item->redrawFrames -= 1;
        if (item->redrawFrames < 0) {
            item->redrawFrames = 0;
        }
    }
}

// export 4
s32 picmenu_get_selected_item() {
    return sSelectedItem;
}

// export 5
void picmenu_set_selected_item(s32 index) {
    sSelectedItem = index;
}

// export 11
void picmenu_set_opacity(u8 opacity) {
    sOpacity = opacity;
}

// export 6
s32 picmenu_get_item_override(s32 index) {
    return sItems[index].overrideWith;
}

// export 7
void picmenu_set_item_override(s32 index, s32 overrideWith) {
    sItems[index].overrideWith = overrideWith;
}

// export 8
void picmenu_update_text(PicMenuItem *items) {
    s32 i;

    for (i = 0; i < sItemCount; i++) {
        sItems[i].text = items[i].text;
        sItems[i].redrawFrames = 2;
    }
}

// export 9
u8 picmenu_get_highlight_alpha() {
    return sHighlightAlpha;
}

// export 10
void picmenu_update_flags(PicMenuItem *items) {
    s32 i;

    for (i = 0; i < sItemCount; i++) {
        sItems[i].flags = items[i].flags;
    }
}

// export 12
void picmenu_redraw_all() {
    s32 i;

    for (i = 0; i < sItemCount; i++) {
        sItems[i].redrawFrames = 4;
    }
}

void picmenu_calculate_redraw_area() {
    s32 minY;
    s32 maxY;
    s32 i;
    PicMenuItem *item;
    Texture *tex;
    s32 y;
    s32 height;

    minY = 480;
    maxY = 0;

    for (i = 0; i < sItemCount; i++) {
        item = &sItems[i];

        if (item->redrawFrames != 0 || item->flags & PICMENU_ALWAYS_REDRAW) {
            if (item->flags & PICMENU_HAS_BACKGROUND && item->bgTextureIndices[0] != -1) {
                tex = sTextures[item->bgTextureIndices[0]].texture;
            } else {
                tex = item->texture.asPtr;
            }

            if (tex != NULL) {
                height = tex->height | ((tex->unk_0x1b & 0xF) << 8);
                y = item->itemY;
            } else {
                height = font_get_y_spacing(item->fontID) + 2;
                y = item->textY - 2;
            }

            if (y < minY) {
                minY = y;
            }

            if (maxY < (y + height)) {
                maxY = (y + height);
            }
        }
    }

    func_800100D4(0, 640, minY, maxY);
}

// this is just missing some optimized-out control flow i think
#ifdef NON_MATCHING
void picmenu_calculate_items_to_redraw() {
    Texture *tex;
    s32 selectedItemHeight;
    s32 selectedItemY;
    s32 i;
    Texture *tex2;
    s32 otherItemHeight;
    s32 otherItemY;
    PicMenuItem *item1;
    PicMenuItem *item2;

    sItems[sSelectedItem].redrawFrames = 4;

    item1 = &sItems[sSelectedItem];

    if (item1->flags & PICMENU_HAS_BACKGROUND && item1->bgTextureIndices[0] != -1) {
        tex = sTextures[item1->bgTextureIndices[0]].texture;
    } else {
        tex = item1->texture.asPtr;
    }

    if (tex != NULL) {
        selectedItemHeight = tex->height | ((tex->unk_0x1b & 0xF) << 8);
        selectedItemY = item1->itemY;
    } else {
        selectedItemHeight = font_get_y_spacing(item1->fontID) + 2;
        selectedItemY = item1->textY - 2;
    }

    for (i = 0; i < sItemCount; i++) {
        item2 = &sItems[i];

        if (i != sSelectedItem) {
            if (item2->flags & PICMENU_HAS_BACKGROUND && item2->bgTextureIndices[0] != -1) {
                tex2 = sTextures[item2->bgTextureIndices[0]].texture;
            } else {
                tex2 = item2->texture.asPtr;
            }

            if (tex2 != NULL) {
                otherItemHeight = tex2->height | ((tex2->unk_0x1b & 0xF) << 8);
                otherItemY = item2->itemY;
            } else {
                otherItemHeight = font_get_y_spacing(item2->fontID) + 2;
                otherItemY = item2->textY - 2;
            }

            if (otherItemY < (selectedItemY + selectedItemHeight) && 
                    (otherItemY + otherItemHeight) > selectedItemY) {
                item2->redrawFrames = 4;
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/74_picmenu/picmenu_calculate_items_to_redraw.s")
#endif

static u16 picmenu_calculate_auto_width(const char *text, s32 fontID) {
    u16 width;
    u16 var;

    width = font_get_text_width(1, text, 0, fontID) + 40;
    var = (width % 20);
    if (var != 0) {
        width = (width - var) + 20;
    }

    return width;
}

static void picmenu_generate_item_background(PicMenuItem *item) {
    s32 i;
    s32 pxRemaining;
    
    pxRemaining = item->outerWidth;

    i = 0;
    while (i < (s32)ARRAYCOUNT(item->bgTextureIndices)) {
        item->bgTextureIndices[i] = -1;
        i++;
    }

    i = 1;

    item->bgTextureIndices[0] = 0;

    pxRemaining = (pxRemaining - sTextures[0].width) - sTextures[1].width;
    while (pxRemaining != 0) {
        if (pxRemaining >= 80) {
            item->bgTextureIndices[i] = rand_next(2, 5);
        } else if (pxRemaining >= 40) {
            item->bgTextureIndices[i] = rand_next(4, 5);
        } else {
            item->bgTextureIndices[i] = 5;
        }

        pxRemaining -= sTextures[item->bgTextureIndices[i]].width;
        i++;
    }

    item->bgTextureIndices[i] = 1;
}