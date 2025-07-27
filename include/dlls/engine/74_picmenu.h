#ifndef _DLLS_74_H
#define _DLLS_74_H

#include "PR/ultratypes.h"
#include "sys/gfx/texture.h"
#include "dll_def.h"
#include "types.h"

typedef struct
{
    Texture *texture;
    s16 textureID;
    u8 width;
} PicMenuTexture;

typedef enum {
    PICMENU_ALIGN_TEXT_CENTER = 0x2,
    PICMENU_HAS_BACKGROUND = 0x4,
    // Align horizontally after the leftLink item
    PICMENU_INLINE = 0x8,
    // Automatically determine width based on text
    PICMENU_AUTO_WIDTH = 0x10,
    PICMENU_DISABLED = 0x20,
    PICMENU_FLAG_INVISIBLE = 0x40,
    // Use base and highlight text colors defined on the item instead of colors from the font
    PICMENU_USE_TEXT_COLOR = 0x80,
    // Draws the text with a raised "3D" effect
    PICMENU_RAISED_EFFECT = 0x100,
    PICMENU_ALWAYS_REDRAW = 0x200,
    PICMENU_ALIGN_ITEM_CENTER = 0x400,
    // Draw item at half transparency
    PICMENU_TRANSPARENT = 0x800,
    // Item cannot be navigated to and is implicitly invisible
    PICMENU_INTANGIBLE = 0x1000
} PicMenuItemFlags;

typedef enum {
    PICMENU_ACTION_NONE = -1,
    PICMENU_ACTION_BACK = 0,
    PICMENU_ACTION_SELECT = 1
} PicMenuAction;

// size: 0x3C
typedef struct
{
    /*00*/ char *text;
    /*04*/ s16 textX;
    /*06*/ s16 textY;
    /*08*/ u16 innerWidth;
    /*0A*/ s16 itemX;
    /*0C*/ s16 itemY;
    /*10*/ union
    {
        Texture *asPtr;
        s32 asID;
    } texture;
    /*14*/ u16 outerWidth;
    /*16*/ u16 flags;
    /*18*/ u8 fontID;
    /*19*/ u8 highlightFontID;
    /*1A*/ s8 upLink;
    /*1C*/ s8 downLink;
    /*1C*/ s8 leftLink;
    /*1D*/ s8 rightLink;
    /*1E*/ s8 overrideWith;         // While not -1, this item becomes the item at the set index instead
    /*1F*/ s8 bgTextureIndices[25]; // Texture indices making up the item's background
    /*38*/ s8 redrawFrames;         // Frames remaining where this item will be redrawn
} PicMenuItem;

#define NEW_PICMENU_ITEM(textX, textY, innerWidth, itemX, itemY, textureID, outerWidth, flags, fontID, highlightFontID, upLink, downLink, leftLink, rightLink, overrideWith) \
    {NULL, textX, textY, innerWidth, itemX, itemY, ((void *)textureID), outerWidth, flags, fontID, highlightFontID, upLink, downLink, leftLink, rightLink, overrideWith, {0}, 0}

typedef struct
{
    s32 moveSoundID;
    s32 selectSoundID;
    s32 backSoundID;
} PicMenuSounds;

DLL_INTERFACE_BEGIN(74_picmenu)
/*0*/ void (*set_items)(PicMenuItem *items, s32 count,
                        s32 defaultItem, PicMenuSounds *sounds, s32 param5, s32 param6,
                        s32 textColorR, s32 textColorG, s32 textColorB,
                        s32 textHighlightR, s32 textHighlightG, s32 textHighlightB);
/*1*/ void (*clear_items)();
/*2*/ PicMenuAction (*update)();
/*3*/ void (*draw)(Gfx **gdl);
/*4*/ s32 (*get_selected_item)();
/*5*/ void (*set_selected_item)(s32 index);
/*6*/ s32 (*get_item_override)(s32 index);
/*7*/ void (*set_item_override)(s32 index, s32 overrideWith);
/*8*/ void (*update_text)(PicMenuItem *items);
/*9*/ u8 (*get_highlight_alpha)();
/*10*/ void (*update_flags)(PicMenuItem *items);
/*11*/ void (*set_opacity)(u8 opacity);
/*12*/ void (*redraw_all)();
DLL_INTERFACE_END()

#endif //_DLLS_74_H
