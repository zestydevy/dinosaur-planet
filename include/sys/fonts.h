#ifndef _SYS_FONT_H
#define _SYS_FONT_H

#include "PR/ultratypes.h"
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
/* 0x000 */ s32 unk0;
/* 0x004 */ char name[28];
/* 0x020 */ u16 x;
/* 0x022 */ u16 y;
/* 0x024 */ u16 charWidth;
/* 0x026 */ u16 charHeight;
/* 0x028 */ u8 loadedFonts[24];
/* 0x040 */ s16 textureID[FONTDATA_MAX_TEXTURES];
/* 0x0C0 */ Texture *texturePointers[FONTDATA_MAX_TEXTURES];
/* 0x1C0 */ FontCharData letters[96];
/* 0x4C0 */ u8 _unk4C0[0x8c0 - 0x4C0];
} FontData;

/* Size: 0x28 bytes */
//Dialogue Box text linked list (Including background struct)
typedef struct DialogueBox {
/* 0x00 */ u8 unk00; // Unused
/* 0x01 */ u8 textNum; // A number that is drawn with the text
/* 0x02 */ char *text; // Pointer to the text array
/* 0x06 */ //u8 _unk6[2];
/* 0x08 */ s16 x1; // Left position of the text
/* 0x0A */ s16 y1; // Top portion of the text
/* 0x0C */ s16 x2; // Right portion of the text
/* 0x0E */ s16 y2; // Bottom portion of the text
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
/* 0x1A */ u16 unk1A;
/* 0x1C */ u16 unk1C;
/* 0x1E */ //u8 _unk1E[2];
/* 0x20 */ f32 unk20;
/* 0x24 */ struct DialogueBox *nextBox;
} DialogueBox;


/* Size: 0x28 bytes */
typedef struct DialogueTextElement {
/* 0x00 */ u8 unk0;
/* 0x01 */ u8 number;
/* 0x02 */ //u8 unk2;
/* 0x03 */ //u8 unk3;
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
/* 0x1C */ u16 unk1C;
/* 0x1E */ //u16 _unk1E;
/* 0x20 */ f32 unk20;
/* 0x24 */ DialogueBox *nextBox;
} DialogueTextElement;

/* Size: 0x2C bytes */
// Dialogue Box background
typedef struct DialogueBoxBackground {
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
    f32 unk20;
    s16 textOffsetX;
    s16 textOffsetY;
    DialogueBox *textBox;
} DialogueBoxBackground;

#define DIALOGUEBOXBACKGROUND_COUNT 8
#define DIALOGUETEXTELEMENT_COUNT 64

void init_fonts();
void set_compact_kerning(s32 setting);
void func_8000CB00(s32 fontID);
void func_8000CBC8(s32 fontID);
void func_8000CC8C(s32 dialogueBoxIndex, s32 xPos, s32 yPos, s32 param4);
void func_8000CCFC(Gfx **gdl, s32 dialogueBoxIndex, char *text, u32 alignmentFlags);
void func_8000CD58(Gfx **gdl, s32 dialogueBoxIndex, s32 xPos, s32 yPos, char *text, u32 alignmentFlags);
void caption_func(Gfx **gdl, DialogueBoxBackground *box, char *text, u32 alignmentFlags, f32 scisScale);
s32 func_8000D8C8(s32 dialogueBoxIndex, char *text, s32 x, s32 fontID);
s32 func_8000D920(DialogueBoxBackground *box, char *text, s32 x, s32 fontID);
void func_8000DAA4(s32 dialogueBoxIndex, s32 x1, s32 y1, s32 x2, s32 y2);
void func_8000DB50(s32 dialogueBoxIndex, s32 fontID);
void func_8000DBCC(s32 dialogueBoxIndex, f32 param2);
void func_8000DC0C(s32 dialogueBoxIndex, s32 red, s32 green, s32 blue, s32 alpha);
void func_8000DC54(s32 dialogueBoxIndex, s32 red, s32 green, s32 blue, s32 alpha, s32 opacity);
void func_8000DCA4(s32 dialogueBoxIndex, s32 red, s32 green, s32 blue, s32 alpha);
void func_8000DCEC(s32 dialogueBoxIndex, s32 x, s32 y);
void func_8000DD38(s32 dialogueBoxIndex);
void func_8000DD74(Gfx **gdl, DialogueBoxBackground *box, char *text, f32 scisScale);
DialogueTextElement *func_8000E424(s32 dialogueBoxIndex, char *text, s32 number, s32 flags);
DialogueTextElement *func_8000E49C(s32 dialogueBoxIndex, s32 posX, s32 posY, char *text, s32 number, s32 flags);
void func_8000E758(s32 dialogueBoxIndex, DialogueBox *dialogueBox);
void func_8000E7D8(s32 dialogueBoxIndex);
void func_8000E838(s32 dialogueBoxIndex, DialogueTextElement *box, s32 x, s32 y, s32 flags);
void func_8000E958(s32 dialogueBoxIndex);
void func_8000E998(s32 dialogueBoxIndex);
s32 func_8000E9D8(s32 fontID);
void func_8000EA04(s32 dialogueBoxIndex);
void func_8000EA44(s32 dialogueBoxIndex);
void func_8000EA84(s32 dialogueBoxIndex);
void func_8000EAC4(s32 dialogueBoxIndex);
void func_8000EB04(Gfx **gdl, s32 **mtxs, s32 **vtxs);
void func_8000EBC8(char **outString, s32 number);
void func_8000EE54(Gfx **gdl, s32 ulx, s32 uly, s32 lrx, s32 lry);
void func_8000EF18(Gfx **gdl, s32 **mtxs, s32 **vtxs, s32 dialogueBoxIndex);
void func_8000F2E8(char *input, char *output, s32 number);

#endif
