#include "common.h"
#include "libc/stdarg.h"
#include "sys/string.h"

/* -------- .bss start 800be1d0 -------- */
char gDebugPrintBufferStart[0x900];
Texture *gDiTextures[3];
u16 D_800BEADC;
u16 D_800BEADE;
u16 D_800BEAE0;
u16 D_800BEAE2;
s32 D_800BEAE4;
s32 D_800BEAE8;
s32 D_800BEAEC;
s32 D_800BEAF0;
s32 D_800BEAF4;
s32 D_800BEAF8;
s32 D_800BEAFC;
u16 D_800BEB00; // gDebugScreenWidth?
u16 D_800BEB02; // gDebugScreenHeight?
u8 D_800BEB04;
u8 D_800BEB05;
u8 D_800BEB06;
u8 D_800BEB07;
/* -------- .bss end 800beb10 -------- */

// .data
s32 gSprintfSpacingCodes = 0;
u16 D_800930E4 = 32;
char *gDebugPrintBufferEnd = &gDebugPrintBufferStart[0];
// Char width is (v - u) + 1
DiFontCoords gDiFontCoords[3][32] = {
    // ASCII symbols and numbers
    // (This is out of order since they subtract 0x21 instead of 0x20)
    // !"#$%&'()*+,-./0123456789:;<=>?{SPACE}
    {
        { 0x02, 0x04 }, // !
        { 0x06, 0x08 }, // "
        { 0x0A, 0x0F }, // #
        { 0x11, 0x15 }, // $
        { 0x17, 0x1F }, // %
        { 0x21, 0x27 }, // &
        { 0x29, 0x2B }, // '
        { 0x2D, 0x2F }, // (
        { 0x31, 0x33 }, // )
        { 0x35, 0x38 }, // *
        { 0x3A, 0x3F }, // +
        { 0x41, 0x43 }, // ,
        { 0x45, 0x48 }, // -
        { 0x4A, 0x4B }, // .
        { 0x4D, 0x50 }, // /
        { 0x52, 0x56 }, // 0
        { 0x58, 0x5B }, // 1
        { 0x5D, 0x62 }, // 2
        { 0x64, 0x68 }, // 3
        { 0x6A, 0x6F }, // 4
        { 0x71, 0x76 }, // 5
        { 0x78, 0x7D }, // 6
        { 0x7F, 0x84 }, // 7
        { 0x86, 0x8B }, // 8
        { 0x8D, 0x92 }, // 9
        { 0x94, 0x96 }, // :
        { 0x98, 0x9A }, // ;
        { 0x9D, 0xA2 }, // <
        { 0xA5, 0xA9 }, // =
        { 0xAB, 0xB0 }, // >
        { 0xB3, 0xB8 }, // ?
        { 0x00, 0x01 }, // {SPACE}
    },
    // ASCII Upper Case
    // @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_
    {
        { 0x00, 0x09 }, // @
        { 0x0B, 0x11 }, // A
        { 0x13, 0x19 }, // B
        { 0x1B, 0x21 }, // C
        { 0x23, 0x29 }, // D
        { 0x2B, 0x31 }, // E
        { 0x33, 0x38 }, // F
        { 0x3A, 0x41 }, // G
        { 0x43, 0x49 }, // H
        { 0x4B, 0x4C }, // I
        { 0x4E, 0x53 }, // J
        { 0x55, 0x5B }, // K
        { 0x5D, 0x62 }, // L
        { 0x64, 0x6B }, // M
        { 0x6D, 0x73 }, // N
        { 0x75, 0x7B }, // O
        { 0x7D, 0x83 }, // P
        { 0x85, 0x8B }, // Q
        { 0x8D, 0x93 }, // R
        { 0x95, 0x9B }, // S
        { 0x9D, 0xA3 }, // T
        { 0xA5, 0xAA }, // U
        { 0xAC, 0xB2 }, // V
        { 0xB4, 0xBC }, // W
        { 0xBE, 0xC4 }, // X
        { 0xC6, 0xCC }, // Y
        { 0xCE, 0xD3 }, // Z
        { 0xD5, 0xD7 }, // [
        { 0xD9, 0xDC }, // {backslash}
        { 0xDE, 0xE0 }, // ]
        { 0xE2, 0xE7 }, // ^
        { 0xE9, 0xEF }, // _
    },
    // ASCII Lower Case
    // `abcdefghijklmnopqrstuvwxyz{|}~
    {
        { 0x00, 0x01 }, // `
        { 0x03, 0x08 }, // a
        { 0x09, 0x0F }, // b
        { 0x11, 0x16 }, // c
        { 0x18, 0x1D }, // d
        { 0x1F, 0x24 }, // e
        { 0x26, 0x28 }, // f
        { 0x2A, 0x2F }, // g
        { 0x31, 0x36 }, // h
        { 0x38, 0x39 }, // i
        { 0x3B, 0x3D }, // j
        { 0x3F, 0x43 }, // k
        { 0x45, 0x46 }, // l
        { 0x48, 0x4F }, // m
        { 0x51, 0x56 }, // n
        { 0x58, 0x5D }, // o
        { 0x5F, 0x64 }, // p
        { 0x66, 0x6B }, // q
        { 0x6C, 0x70 }, // r
        { 0x72, 0x77 }, // s
        { 0x79, 0x7C }, // t
        { 0x7E, 0x82 }, // u
        { 0x84, 0x89 }, // v
        { 0x8B, 0x92 }, // w
        { 0x94, 0x99 }, // x
        { 0x9B, 0xA0 }, // y
        { 0xA2, 0xA6 }, // z
        { 0xA8, 0xAB }, // {
        { 0xAD, 0xAE }, // |
        { 0xB0, 0xB3 }, // }
        { 0xB5, 0xB9 }, // ~
        { 0xB5, 0xB9 }  // ASCII DEL, so it just reuses the tilde above.
    }
};
u8 D_800931AC = 0;
u8 D_800931B0 = 0;
u8 D_800931B4 = 0;
u8 D_800931B8 = 0;
Gfx D_800931C0[6] = { // dDebugFontSettings?
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_PRIMITIVE, G_CC_PRIMITIVE),
    gsDPSetOtherMode(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | 
        G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | 
        G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2),
    gsDPSetEnvColor(255, 255, 255, 255),
    gsDPSetPrimColor(0, 0, 0, 0, 0, 0),
    gsDPSetBlendColor(0, 0, 0, 255)
};

// .rodata
const char gLowercaseAlphabet[] = "0123456789abcdefghijklmnopqrstuvwxyz";
const char gUppercaseAlphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char D_8009AE40[] = "";
const char D_8009AE44[] = "(null)";
const char D_8009AE4C[] = "(nil)";
const char D_8009AE54[] = "*** diPrintf Error *** ---> Out of string space. (Print less text!)\n";

void diPrintfUpdateBounds();
void diPrintfOrigin();
s32 diPrintf_func_80061210(Gfx **gdl, char *buffer);
void diPrintfRenderBackground(Gfx **gdl, u32 ulx, u32 uly, u32 lrx, u32 lry);

char *strcpy(char *dest, char *src) {
    char *start = dest;

    while ((*(dest++) = *(src++)) != '\0') { }

    return start;
}

char *strcat(char *dest, const char *src) {
    char *start = dest;

    while (*dest != '\0') {
        dest++;
    }

    while ((*(dest++) = *(src++)) != '\0') { }

    return start;
}

/**
 * Case insensitive strcmp.
 */
int strcasecmp(const char *lhs, const char *rhs) {
    char c1, c2;
    
    while (*lhs != '\0' || *rhs != '\0') {
        // To uppercase
        c1 = *lhs;
        if (c1 >= 'a' && c1 <= 'z') {
            c1 -= 32;
        }

        c2 = *rhs;
        if (c2 >= 'a' && c2 <= 'z') {
            c2 -= 32;
        }

        if (c1 < c2) {
            return -1;
        }
        if (c1 > c2) {
            return 1;
        }

        lhs++;
        rhs++;
    }

    return 0;
}

/**
 * Case insensitive strncmp.
 */
int strncasecmp(const char *lhs, const char *rhs, size_t count) {
    char c1, c2;
    
    while ((*lhs != '\0' || *rhs != '\0') && count != 0) {
        // To uppercase
        c1 = *lhs;
        if (c1 >= 'a' && c1 <= 'z') {
            c1 -= 32;
        }

        c2 = *rhs;
        if (c2 >= 'a' && c2 <= 'z') {
            c2 -= 32;
        }

        if (c1 < c2) {
            return -1;
        }
        if (c1 > c2) {
            return 1;
        }

        lhs++;
        rhs++;
        count--;
    }

    return 0;
}

void *memset(void *dest, int ch, size_t count) {
    unsigned char *ptr = dest;

    while (count-- > 0) {
        *(ptr++) = (unsigned char)ch;
    }

    return dest;
}

char *_itoa(u64 n, char *outBuffer, u32 radix, s32 bUppercase) {
    char *chars;
    char *buffer;

    chars = bUppercase ? (char*)gUppercaseAlphabet : (char*)gLowercaseAlphabet;

    for (buffer = outBuffer; n > 0; n /= radix) {
        *(--buffer) = chars[n % radix];
    }

    return buffer;
}

void sprintfSetSpacingCodes(s32 setting) {
    gSprintfSpacingCodes = setting;
}

int sprintf(char *str, const char *fmt, ...) {
    va_list ap;
    int ret;

    va_start(ap, fmt);
    
    ret = vsprintf(str, fmt, ap);

    va_end(ap);

    return ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/print/vsprintf.s")

void diPrintfInit() {
    u32 fbRes;

    fbRes = vi_get_current_size();
    if (GET_VIDEO_WIDTH(fbRes) > 320) {
        D_800931AC = 1;
    }
    if (GET_VIDEO_HEIGHT(fbRes) > 240) {
        D_800931B0 = 1;
    }

    D_800931B4 = 0;
    D_800931B8 = 0;

    gDiTextures[0] = tex_load_deferred(TEXTABLE_0_DiFontAtlas1);
    gDiTextures[1] = tex_load_deferred(TEXTABLE_1_DiFontAtlas2);
    gDiTextures[2] = tex_load_deferred(TEXTABLE_2_DiFontAtlas3);

    gDebugPrintBufferEnd = &gDebugPrintBufferStart[0];
}

int diPrintf(const char *format, ...) {
    return 0;
}

// guessed parameters
void unused_printf_80060b40(const char *param1, ...) {
    
}

// guessed parameters
void unused_printf_80060B5C(const char *param1, ...) {

}

// guessed parameters
void unused_printf_80060B78(const char *param1, ...) {

}

void diPrintfAll(Gfx **gdl) {
    char *buffer;
    u32 fbRes;

    fbRes = vi_get_current_size();
    D_800BEB02 = GET_VIDEO_HEIGHT(fbRes);
    D_800BEB00 = GET_VIDEO_WIDTH(fbRes);

    gDPSetScissor((*gdl)++, G_SC_NON_INTERLACE, 0, 0, D_800BEB00, D_800BEB02);

    diPrintfUpdateBounds();

    bcopy(D_800931C0, *gdl, sizeof(D_800931C0));
    *gdl += ARRAYCOUNT(D_800931C0);

    dl_set_all_dirty();

    tex_render_reset();

    buffer = gDebugPrintBufferStart;
    diPrintfOrigin();

    D_800BEAFC = -1;
    D_800BEAE4 = 0;
    D_800BEAE0 = D_800BEADC;
    D_800BEAE2 = D_800BEADE;

    while (buffer != gDebugPrintBufferEnd) {
        D_800BEAE8 = 0;
        buffer += diPrintf_func_80061210(gdl, buffer);
    }

    buffer = gDebugPrintBufferStart;

    diPrintfRenderBackground(gdl, D_800BEAE0, D_800BEAE2, D_800BEADC, D_800BEADE + 10);
    diPrintfOrigin();

    D_800BEAFC = -1;
    D_800BEAE4 = 0;

    while (buffer != gDebugPrintBufferEnd) {
        D_800BEAE8 = 1;
        buffer += diPrintf_func_80061210(gdl, buffer);
    }

    gDebugPrintBufferEnd = gDebugPrintBufferStart;

    dl_set_all_dirty();
    tex_render_reset();
}

// guessed name
void diPrintfClear() {
    gDebugPrintBufferEnd = gDebugPrintBufferStart;
    diPrintfOrigin();
}

void diPrintfSetCol(u8 red, u8 green, u8 blue, u8 alpha) {
    DI_PRINTF_CMD_SET_COLOR(red, green, blue, alpha);
}

void diPrintfSetBG(u8 red, u8 green, u8 blue, u8 alpha) {
    DI_PRINTF_CMD_SET_BACKGROUND_COLOR(red, green, blue, alpha);
}

void diPrintfCmd_0x87(u8 param1, u8 param2) {
    DI_PRINTF_CMD_0x87(param1, param2);
}

void diPrintfSetXY(u16 x, u16 y) {
    DI_PRINTF_CMD_SET_POSITION(x, y);
}

void diPrintfCmd_0x86(u16 param1) {
    DI_PRINTF_CMD_0x86(param1);
}

// guessed name
s32 diPrintfMeasureString(const char *format, ...) {
    s32 pad;
    s32 fontCharU;
    s32 stringLength;
    char s[255];
    u8 *ch;
    va_list args;
    va_start(args, format);

    stringLength = 0;
    sprintfSetSpacingCodes(TRUE);
    vsprintf(s, format, args);
    sprintfSetSpacingCodes(FALSE);
    for (ch = (u8 *) &s[0]; *ch != '\0'; ch++) {
        pad = *ch;
        if (*ch != (0, '\n')) {
            if (pad == ' ') {
                stringLength += 6;
                if (1) {}
            } else {
                if (*ch < '@') {
                    // Character is a symbol or number and not a letter
                    D_800BEAFC = 0;
                    *ch -= '!';
                } else if (*ch < '`') {
                    // Character is a upper case letter
                    D_800BEAFC = 1;
                    *ch -= '@';
                } else if (*ch <= 0x7F) {
                    // Character is a lower case letter
                    D_800BEAFC = 2;
                    *ch -= '`';
                }
                fontCharU = gDiFontCoords[D_800BEAFC][*ch].u;
                stringLength = ((stringLength + gDiFontCoords[D_800BEAFC][*ch].v) - fontCharU) + (pad = 1);
            }
        }
    }
    va_end(args);
    return stringLength;
}

// see https://github.com/DavidSM64/Diddy-Kong-Racing/blob/224d0f62f5342c85400d939dc10537a080bb3499/src/printf.c#L356
#pragma GLOBAL_ASM("asm/nonmatchings/print/diPrintf_func_80061210.s")

// guessed name
#pragma GLOBAL_ASM("asm/nonmatchings/print/diPrintfRenderBackground.s")

// guessed name
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/print/diPrintfRenderChar.s")
#else
// gfx commands arent quite correct
s32 diPrintfRenderChar(Gfx **gdl, s32 asciiVal) {
    s32 fontCharWidth;
    s32 fontCharU;
    s32 var1;
    s32 var2;

    if (asciiVal < '@') {
        // Character is a symbol or number and not a letter
        if (D_800BEAFC != 0) {
            if (D_800BEAE8) {
                gDPLoadTextureBlock((*gdl)++, OS_PHYSICAL_TO_K0(gDiTextures[0] + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 192, 11,
                                    0, 2, 2, 0, 0, 0, 0);
            }
            D_800BEAFC = 0;
        }
        asciiVal -= '!';
    } else if (asciiVal < '`') {
        // Character is a upper case letter
        if (D_800BEAFC != 1) {
            if (D_800BEAE8) {
                gDPLoadTextureBlock((*gdl)++, OS_PHYSICAL_TO_K0(gDiTextures[1] + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 248, 11,
                                    0, 2, 2, 0, 0, 0, 0);
            }
            D_800BEAFC = 1;
        }
        asciiVal -= '@';
    } else if (asciiVal <= 0x7F) {
        // Character is a lower case letter
        if (D_800BEAFC != 2) {
            if (D_800BEAE8) {
                gDPLoadTextureBlock((*gdl)++, OS_PHYSICAL_TO_K0(gDiTextures[2] + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 192, 11,
                                    0, 2, 2, 0, 0, 0, 0);
            }
            D_800BEAFC = 2;
        }
        asciiVal -= '`';
    }
    fontCharU = gDiFontCoords[D_800BEAFC][asciiVal].u;
    fontCharWidth = (gDiFontCoords[D_800BEAFC][asciiVal].v - fontCharU) + 1;
    if (D_800BEAE8) {
        var1 = (D_800BEADC << (D_800931AC + D_800931B4));
        var2 = (D_800BEADE << (D_800931B0 + D_800931B8));

        gDPSetCombineLERP((*gdl), 
            0, 0, ENVIRONMENT, 0, 0, 0, TEXEL0, 0, 
            0, 0, ENVIRONMENT, 0, 0, 0, TEXEL0, 0);
        dl_apply_combine(gdl);
        gSPTextureRectangle((*gdl)++, 
            /*xl*/(var1 << 2), 
            /*yl*/(var2 << 2), 
            /*xh*/((var1 + (fontCharWidth << (D_800931AC + D_800931B4))) << 2),
            /*yh*/((var2 + (10 << (D_800931B0 + D_800931B8))) << 2), 
            /*tile*/0, 
            /*s*/(fontCharU << 5), 
            /*t*/0, 
            /*dsdx*/1 << (10 - D_800931AC - D_800931B4), 
            /*dtdy*/1 << (10 - D_800931B0 - D_800931B8));
        gDLBuilder->needsPipeSync = TRUE;
    }
    return fontCharWidth;
}
#endif

// guessed name
void diPrintfUpdateBounds() {
    if (D_800BEB00 <= 320) {
        D_800BEAEC = 16;
        D_800BEAF0 = D_800BEB00 - 16;
    } else {
        D_800BEAEC = 32;
        D_800BEAF0 = D_800BEB00 - 32;
    }
    if (D_800BEB02 <= 240) {
        D_800BEAF4 = 16;
        D_800BEAF8 = D_800BEB02 - 16;
    } else {
        D_800BEAF4 = 32;
        D_800BEAF8 = D_800BEB02 - 32;
    }
}

// guessed name
void diPrintfOrigin() {
    D_800BEADC = D_800BEAEC;
    D_800BEADE = D_800BEAF4;
}

// guessed name
void diPrintfNewline() {
    D_800BEADC = D_800BEAEC;
    D_800BEADE += 11;
}
