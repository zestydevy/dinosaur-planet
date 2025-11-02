#ifndef _SYS_FONT_H
#define _SYS_FONT_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "types.h"
#include "sys/gfx/texture.h"

/* Size: 8 bytes */
typedef struct FontCharData {
    /*0x0*/ u8 textureIndex;
    /*0x1*/ u8 kerning;
    /*0x2*/ s8 offsetX;
    /*0x3*/ s8 offsetY;
    /*0x4*/ u8 textureU;
    /*0x5*/ u8 textureV;
    /*0x6*/ u8 width;
    /*0x7*/ u8 height;
} FontCharData;

#define FONTDATA_MAX_TEXTURES 64

/* Size: 0x8C0 bytes */
typedef struct FontData {
/* 0x000 */ char name[32];
/* 0x020 */ u16 x;
/* 0x022 */ u16 y;
/* 0x024 */ u16 charWidth;
/* 0x026 */ u16 charHeight;
/* 0x028 */ u8 referenceCount;
/* 0x029 */ u8 _unk29[23];
/* 0x040 */ s16 textureID[FONTDATA_MAX_TEXTURES];
/* 0x0C0 */ Texture *texturePointers[FONTDATA_MAX_TEXTURES];
/* 0x1C0 */ FontCharData letters[224];
} FontData;

/* Size: 0x28 bytes */
typedef struct FontString {
/* 0x00 */ u8 unk0;
/* 0x01 */ u8 priority;
/* 0x04 */ char *text; // Pointer to the text array
/* 0x08 */ s16 posX;
/* 0x0A */ s16 posY;
/* 0x0C */ s16 offsetX;
/* 0x0E */ s16 offsetY;
/* 0x10 */ u8 textColourR;
/* 0x11 */ u8 textColourG;
/* 0x12 */ u8 textColourB;
/* 0x13 */ u8 textColourA;
/* 0x14 */ u8 textBGColourR;
/* 0x15 */ u8 textBGColourG;
/* 0x16 */ u8 textBGColourB;
/* 0x17 */ u8 textBGColourA;
/* 0x18 */ u8 opacity;
/* 0x19 */ u8 font;
/* 0x1A */ u16 flags;
/* 0x1C */ u16 alignmentFlags;
/* 0x20 */ f32 extraCharacterSpacing;
/* 0x24 */ struct FontString *next; // Next string in linked list
} FontString;

/* Size: 0x2C bytes */
typedef struct FontWindow {
    s16 xpos;
    s16 ypos;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    s16 width;
    s16 height;
    u8 backgroundColourR;
    u8 backgroundColourG;
    u8 backgroundColourB;
    u8 backgroundColourA;
    u8 textColourR;
    u8 textColourG;
    u8 textColourB;
    u8 textColourA;
    u8 textBGColourR;
    u8 textBGColourG;
    u8 textBGColourB;
    u8 textBGColourA;
    u8 opacity;
    u8 font;
    u16 flags;
    f32 extraCharacterSpacing;
    s16 textOffsetX;
    s16 textOffsetY;
    FontString *strings; // Linked list of strings ordered by priority
} FontWindow;

#define FONT_WINDOW_COUNT 8
#define FONT_STRING_COUNT 64

#define TEXT_POS_CENTER (-0x8000)

typedef enum HorizontalAlignmentFlags {
    HORZ_ALIGN_LEFT   = 0,
    HORZ_ALIGN_RIGHT  = 1,
    HORZ_ALIGN_CENTER = 4
} HorizontalAlignmentFlags;

typedef enum VerticalAlignmentFlags {
    VERT_ALIGN_TOP    = 0,
    VERT_ALIGN_BOTTOM = 2,
    VERT_ALIGN_MIDDLE = 8
} VerticalAlignmentFlags;

typedef enum AlignmentFlags {
    ALIGN_TOP_LEFT      = VERT_ALIGN_TOP    | HORZ_ALIGN_LEFT,
    ALIGN_TOP_CENTER    = VERT_ALIGN_TOP    | HORZ_ALIGN_CENTER,
    ALIGN_TOP_RIGHT     = VERT_ALIGN_TOP    | HORZ_ALIGN_RIGHT,
    ALIGN_MIDDLE_LEFT   = VERT_ALIGN_MIDDLE | HORZ_ALIGN_LEFT,
    ALIGN_MIDDLE_CENTER = VERT_ALIGN_MIDDLE | HORZ_ALIGN_CENTER,
    ALIGN_MIDDLE_RIGHT  = VERT_ALIGN_MIDDLE | HORZ_ALIGN_RIGHT,
    ALIGN_BOTTOM_LEFT   = VERT_ALIGN_BOTTOM | HORZ_ALIGN_LEFT,
    ALIGN_BOTTOM_CENTER = VERT_ALIGN_BOTTOM | HORZ_ALIGN_CENTER,
    ALIGN_BOTTOM_RIGHT  = VERT_ALIGN_BOTTOM | HORZ_ALIGN_RIGHT
} AlignmentFlags;

typedef enum FontWindowFlags {
    FONT_WINDOW_WORDWRAP = 1,
    FONT_WINDOW_VERTS = 0x4000,
    FONT_WINDOW_ENABLED = 0x8000
} FontStringFlags;

typedef enum FontID {
    FONT_FUN_FONT = 0,
    FONT_DINO_SUBTITLE_FONT_1 = 1,
    FONT_DINO_SUBTITLE_FONT_2 = 2,
    FONT_SUBTITLE_FONT = 3,
    FONT_DINO_MEDIUM_FONT_IN = 4,
    FONT_DINO_MEDIUM_FONT_OUT = 5
} FontID;

void fonts_init();
void fonts_set_squash(s32 enabled);
void font_load(s32 fontID);
void font_unload(s32 fontID);
void font_window_set_text_coords(s32 windowID, s32 xPos, s32 yPos, s32 param4);
void font_print_window(Gfx **gdl, s32 windowID, char *text, AlignmentFlags alignmentFlags);
void font_print_window_xy(Gfx **gdl, s32 windowID, s32 xPos, s32 yPos, char *text, AlignmentFlags alignmentFlags);
void font_render_text(Gfx **gdl, FontWindow *window, char *text, AlignmentFlags alignmentFlags, f32 scisScale);
s32 font_get_text_width(s32 windowID, const char *text, s32 x, s32 fontID);
s32 font_get_text_width_internal(FontWindow *window, const char *text, s32 x, s32 fontID);
void font_window_set_coords(s32 windowID, s32 x1, s32 y1, s32 x2, s32 y2);
void font_window_use_font(s32 windowID, s32 fontID);
void font_window_set_extra_char_spacing(s32 windowID, f32 spacing);
void font_window_set_bg_colour(s32 windowID, s32 red, s32 green, s32 blue, s32 alpha);
void font_window_set_text_colour(s32 windowID, s32 red, s32 green, s32 blue, s32 alpha, s32 opacity);
void font_window_set_text_bg_colour(s32 windowID, s32 red, s32 green, s32 blue, s32 alpha);
void font_window_add_text_offset(s32 windowID, s32 x, s32 y);
void font_window_reset_text_offset(s32 windowID);
void font_render_text_wordwrap(Gfx **gdl, FontWindow *window, char *text, f32 scisScale);
FontString *font_window_add_string(s32 windowID, char *text, s32 priority, AlignmentFlags alignmentFlags);
FontString *font_window_add_string_xy(s32 windowID, s32 posX, s32 posY, char *text, s32 priority, AlignmentFlags alignmentFlags);
void font_window_add_font_string(s32 windowID, FontString *string);
void font_window_flush_strings(s32 windowID);
void font_window_scroll_string(s32 windowID, FontString *string, s32 x, s32 y, s32 flags);
void font_window_enable(s32 windowID);
void font_window_disable(s32 windowID);
s32 font_get_y_spacing(s32 fontID);
void font_window_enable_wordwrap(s32 windowID);
void font_window_disable_wordwrap(s32 windowID);
void font_window_enable_verts(s32 windowID);
void font_window_disable_verts(s32 windowID);
void font_windows_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs);
void font_number_to_string(char **outString, s32 number);
void font_render_fill_rect(Gfx **gdl, s32 ulx, s32 uly, s32 lrx, s32 lry);
void font_window_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, s32 windowID);
void font_string_format(char *input, char *output, s32 number);

#endif
