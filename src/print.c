#include "PR/gbi.h"
#include "PR/mbi.h"
#include "libc/stdarg.h"
#include "libc/string.h"
#include "sys/print.h"
#include "sys/gfx/textable.h"
#include "sys/gfx/texture.h"
#include "sys/map.h"
#include "sys/string.h"
#include "sys/vi.h"
#include "macros.h"

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
u32 D_800BEAFC;
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
s32 diPrintfRenderChar(Gfx**gdl, s32 asciiVal);

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

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/print/vsprintf.s")
#else
// https://decomp.me/scratch/fqmpD
// DKRs (very similar) vsprintf function: https://decomp.me/scratch/ivqHu

#define outchar(x) do { \
    done++;          \
    (*s++) = x;      \
} while(0)

#define PAD(x) \
    while(width-- > 0) { \
        outchar(x); \
    }

#define HAVE_LONGLONG 1

#define PTR void*

/* Cast the next arg, of type ARGTYPE, into CASTTYPE, and put it in VAR.  */
#define    castarg(var, argtype, casttype) \
 var = (casttype) va_arg(args, argtype)

/* Get the next arg, of type TYPE, and put it in VAR.  */
#define    nextarg(var, type)    castarg(var, type, type)

#define BUFSIZ 100

#define isdigit(c) ((c >= '0') && (c <= '9'))

// Returns the total number of characters written.
int vsprintf(char* s, const char* format, va_list args) {
    /* Pointer into the format string.  */
    const char *f;

    /* The string describing the size of groups of digits.  */
    UNUSED char *grouping;

    /* Number of characters written.  */
    int done = 0;

    f = format;
    while (*f != '\0') {
          /* Type modifiers.  */
          char is_short, is_long, is_long_double;

#ifdef    HAVE_LONGLONG
        /* We use the `L' modifier for `long long int'.  */
#define    is_longlong    is_long_double
#else
#define    is_longlong    0
#endif

        /* Format spec modifiers.  */
        char space, showsign, left, alt;

        /* Padding character: ' ' or '0'.  */
        char pad;

        /* Width of a field.  */
        int width;

        /* Precision of a field.  */
        int prec;

        /* Decimal integer is negative.  */
        char is_neg;

        /* Current character of the format.  */
        char fc;

        /* Base of a number to be written.  */
        int base;

        /* Integral values to be written.  */
        u64 num;
        s64 signed_num;

        /* String to be written.  */
        char *str;

        char work[BUFSIZ]; // sp17B

        s32 a1;
        s32 i;
        s32 unused2;
        s32 v1;
        s32 a0;
        s32 digit;

        if (*f != '%') {
            /*   This isn't a format spec, so write everything out until the
                 next one.  To properly handle multibyte characters, we cannot
                 just search for a '%'.  Since multibyte characters are hairy
                 (and dealt with above), if we hit any byte above 127 (only
                 those can start a multibyte character) we just punt back to
                 that code.  */
            while (*f != '%' && *f != '\0') {
                outchar(*f++);
            }
            continue;
        }

        ++f;

        /* Check for "%%".  Note that although the ANSI standard lists
            '%' as a conversion specifier, it says "The complete format
            specification shall be `%%'," so we can avoid all the width
            and precision processing.  */
        if (*f == '%') {
            ++f;
            outchar('%');
            continue;
        }

        /* Check for spec modifiers.  */
        space = showsign = left = alt = 0;
        pad = ' ';
        while (*f == ' ' || *f == '+' || *f == '-' || *f == '#' || *f == '0') {
            switch (*f++) {
                case ' ':
                    /* Output a space in place of a sign, when there is no sign.  */
                    space = 1;
                    break;
                case '+':
                    /* Always output + or - for numbers.  */
                    showsign = 1;
                    break;
                case '-':
                    /* Left-justify things.  */
                    left = 1;
                    break;
                case '#':
                    /* Use the "alternate form":
                    Hex has 0x or 0X, FP always has a decimal point.  */
                    alt = 1;
                    break;
                case '0':
                    /* Pad with 0s.  */
                    pad = '0';
                    break;
            }
        }
        // end of while loop

        if (left) {
            pad = ' ';
        }

        /* Get the field width.  */
        width = 0;
        if (*f == '*') {
            /* The field width is given in an argument.
               A negative field width indicates left justification.  */
            nextarg(width, int);
            if (width < 0) {
                width = -width;
                left = 1;
            }
            ++f;
        } else {
            while (isdigit(*f)) {
                width *= 10;
                width += *f++ - '0';
            }
        }

        /* Get the precision.  */
        /* -1 means none given; 0 means explicit 0.  */
        prec = -1;
        if (*f == '.') {
            ++f;
            if (*f == '*') {
                /* The precision is given in an argument.  */
                nextarg(prec, int);
                /* Avoid idiocy.  */
                if (prec < 0) {
                    prec = -1;
                }
                ++f;
            } else if (isdigit (*f)) {
                prec = 0;
                while (isdigit(*f)) {
                    prec *= 10;
                    prec += *f++ - '0';
                }
            }
        }

        /* Check for type modifiers.  */
        is_short = is_long = is_long_double = 0;
        while (*f == 'h' || *f == 'l' || *f == 'L' || *f == 'Z' || *f == 'q') {
            switch (*f++) {
                case 'h':
                    /* int's are short int's.  */
                    is_short = 1;
                    break;
                case 'l':
#ifdef    HAVE_LONGLONG
                    if (is_long) {
                        /* A double `l' is equivalent to an `L'.  */
                        is_longlong = 1;
                    } else {
#endif
                    /* int's are long int's.  */
                        is_long = 1;
                    }
                    break;
                case 'L':
                    /* double's are long double's, and int's are long long int's.  */
                    is_long_double = 1;
                    break;
                case 'Z':
                    /* int's are size_t's.  */
#ifdef    HAVE_LONGLONG
                    //assert (sizeof(size_t) <= sizeof(unsigned long long int));
                    //is_longlong = sizeof(size_t) > sizeof(unsigned long int);
#endif
                    is_long = TRUE; //sizeof(size_t) > sizeof(unsigned int);
                    break;
                case 'q':
                    is_longlong = 1;
                    break;
            }
        }

        /* Format specification.  */
        fc = *f++;
        switch (fc) {
            case 'i':
            case 'd':
                /* Decimal integer.  */
                base = 10;
                if (is_longlong) {
                    nextarg(signed_num, s64);
                } else if (is_long) {
                    nextarg(signed_num, long int);
                } else if (!is_short) {
                    castarg(signed_num, int, long int);
                } else {
                    castarg(signed_num, int, short int);
                }

                is_neg = signed_num < 0;
                num = is_neg ? (- signed_num) : signed_num;
                goto number;
            case 'u':
                /* Decimal unsigned integer.  */
                base = 10;
                goto unsigned_number;
            case 'o':
                /* Octal unsigned integer.  */
                base = 8;
                goto unsigned_number;
            case 'X':
                /* Hexadecimal unsigned integer.  */
                base = 16;
                goto unsigned_number;
            case 'x':
                /* Hex with lower-case digits.  */
                base = 16;

            unsigned_number:
                /* Unsigned number of base BASE.  */
                if (is_longlong) {
                    castarg(num, s64, u64);
                } else if (is_long) {
                    castarg(num, long int, unsigned long int);
                } else if (!is_short) {
                    castarg(num, int, unsigned int);
                } else {
                    castarg(num, int, unsigned short int);
                }
                /* ANSI only specifies the `+' and
                   ` ' flags for signed conversions.  */
                is_neg = showsign = space = 0;

            number:
                /* Number of base BASE.  */
                {
                    char *w;
                    char *workend = &work[sizeof(work) - 1]; // spF8

                    if (gSprintfSpacingCodes) {
                        outchar(0x84);
                    }
                    if (prec >= 0) {
                        pad = ' ';
                    }
                      /* Supply a default precision if none was given.  */
                    if (prec == -1) {
                        prec = 1;
                    }

                    /* Put the number in WORK.  */
                    w = _itoa(num, workend + 1, base, fc == 'X') - 1;
                    width -= workend - w;
                    prec -= workend - w;

                    if (alt && base == 8 && prec <= 0) {
                        *w-- = '0';
                        --width;
                    }

                    if (prec > 0) {
                        width -= prec;
                        while (prec-- > 0) {
                            *w-- = '0';
                        }
                    }

                    if (alt && base == 16) {
                        width -= 2;
                    }

                    if (is_neg || showsign || space) {
                        --width;
                    }

                    if (!left && pad == ' ') {
                        PAD(' ');
                    }

                    if (is_neg) {
                        outchar('-');
                    } else if (showsign) {
                        outchar('+');
                    } else if (space) {
                        outchar(' ');
                    }

                    if (alt && base == 16) {
                        outchar('0');
                        outchar(fc);
                    }

                    if (!left && pad == '0') {
                        PAD('0');
                    }

                    /* Write the number.  */
                    while (++w <= workend) {
                        outchar(*w);
                    }

                    if (left) {
                        PAD(' ');
                    }
                }
                break;

            case 'e':
            case 'E':
            {
                s32 dash; // a3 in this scope
                f32 f2;
                f32 f16;
                s32 unused;
                f32 spBC;
                f32 f0;
                s32 exponent; // spB4 / s3
                s32 unused2;

                if (gSprintfSpacingCodes) {
                    done++;
                    (*s++) = 0x84;
                    // outchar(0x84);
                }

                if (prec < 0) {
                    prec = 6;
                }

                if (is_short) {
                    nextarg(spBC, f32);
                } else {
                    nextarg(spBC, f32);
                }

                dash = FALSE;
                if (*((s8*)&spBC) < 0) {
                    dash = TRUE;
                    spBC = -spBC;
                }

                if (spBC == 0.0f) {
                    exponent = 0;
                    f16 = 1.0f;
                } else if (spBC < 1.0f) {
                    exponent = 0;
                    f16 = 1.0f;
                    while (spBC < f16) {
                        f16 /= 10.0f;
                        exponent--;
                    }
                }

                if (spBC >= 1.0f) {
                    exponent = 0;
                    f16 = 1.0f;
                    f0 = 10.0f;
                    while (f0 <= spBC) {
                        f16 = f0;
                        f0 *= 10.0f;
                        exponent++;
                    }
                }

                f2 = f16 * 0.5f;
                for (digit = prec; digit > 0; digit--) {
                    f2 /= 10.0f;
                }
                spBC += f2;

                f2 = f16 * 10.0f;
                if (spBC >= f2) {
                    f16 = f2;
                    exponent++;
                }

                a0 = (dash || showsign || space) /* a1 */ + prec /* t2 */+ (prec > 0 || alt) /* v1 */ + (exponent >= 100) /* a2 */ + 5;

                if (!left && pad == ' ') {
                    while (width-- > a0) {
                        outchar(pad);
                    }
                }

                if (dash) {
                    outchar('-');
                } else if (showsign) {
                    outchar('+');
                } else if (space) {
                    outchar(' ');
                }

                if (!left && pad == '0') {
                    while (width-- > a0) {
                        outchar(pad);
                    }
                }

                digit = '0'; // v0
                while (spBC >= f16) {
                    spBC -= f16;
                    digit++;
                }
                outchar(digit);
                f16 /= 10.0f;

                if (prec > 0 || alt /* s8 */) {
                    outchar('.');
                }

                while (prec > 0) {
                    digit = '0';
                    while (spBC >= f16) {
                        spBC -= f16;
                        digit++;
                    }
                    outchar(digit);
                    f16 /= 10.0f;
                    prec--;
                }

                outchar(fc);

                if (exponent < 0) {
                    exponent = -exponent;
                    outchar('-');
                } else {
                    outchar('+');
                }

                if (exponent >= 100) {
                    outchar('0' + (exponent / 100));
                }

                outchar('0' + ((exponent / 10) % 10));
                outchar('0' + (exponent % 10));

                if (left) {
                    while (width-- > a0) {
                        outchar(' ');
                    }
                }
                break;
            }
            case 'G':
            case 'g':
                break;
            case 'f':
            {
                f32 f12 = 1.0f;
                f32 f14;
                f32 f2;
                f32 *temp; // spE4
                s32 dash; // a3 in this scope, maybe shared with 'e' case?
                s32 length;
                s32 v0;
                f32 spBC;

                dash = FALSE;

                if (gSprintfSpacingCodes) {
                     outchar(0x84);
                }

                if (prec < 0) {
                    prec = 3;
                }

                for (digit = 0; digit < prec; digit++) {
                    f12 /= 10.0f;
                }

                if (is_short) {
                    nextarg(temp, f32 *);
                    spBC = *temp;
                } else {
                    nextarg(temp, f32 *);
                    if ((u32)temp < 0x80000000 || (u32)temp >= 0x80800001) {
                        outchar('*');
                        outchar('E');
                        outchar('*');
                        spBC = 0.0f;
                    } else {
                        spBC = *temp;
                    }
                }

                if (spBC < 0.0f) {
                    dash = TRUE;
                    spBC = -spBC;
                }

                spBC += f12 * 0.5f;

                digit = 1;
                f2 = 1.0f;
                f14 = 10.0f;
                while (spBC >= f14) {
                    f2 = f14;
                    f14 *= 10.0f;
                    digit++;
                }

                length = (dash || showsign || space) + (prec > 0 || alt) + digit + prec;

                if (!left && pad == ' ') {
                    while (width-- > length) {
                        outchar(pad);
                    }
                }

                if (dash) {
                    outchar('-');
                } else if (showsign) {
                    outchar('+');
                } else if (space) {
                    outchar(' ');
                }

                if (!left && pad == '0') {
                    while (width-- > length) {
                        outchar(pad);
                    }
                }

                do {
                    digit = '0';
                    while (spBC >= f2) {
                        spBC -= f2;
                        digit++;
                    }
                    f2 /= 10.0f;
                    outchar(digit);
                } while (f2 >= 1.0f);

                if (prec > 0 || alt) {
                    outchar('.');
                }

                while (prec > 0) {
                    digit = '0';
                    while (spBC >= f2) {
                        spBC -= f2;
                        digit++;
                    }
                    outchar(digit);
                    f2 /= 10.0f;
                    prec--;
                }

                if (left) {
                    while (width-- > length) {
                        outchar(' ');
                    }
                }
                break;
            }
            case 'c':
                /* Character.  */
                nextarg(num, int);
                if (!left) {
                    while (--width > 0) {
                        outchar(pad);
                    }
                }
                outchar((unsigned char) num);
                if (left) {
                    while (--width > 0) {
                        outchar(' ');
                    }
                }
                break;

            case 's':
            {
                static char null[] = "(null)";
                u32 len; // a0

                nextarg(str, char *);

                // move of a1 into a0 should appear as soon as str is stored into a1
                if (str == NULL) {
                    /* Write "(null)" if there's space.  */
                    if (prec == -1 || prec >= (int) sizeof(null) - 1) {
                        str = null;
                        len = sizeof(null) - 1;
                    } else {
                        str = "";
                        len = 0;
                    }
                } else {
                    len = strlen(str);
                }

                if (prec != -1 && len > prec) {
                    len = prec;
                }
                width -= len;

                if (!left) {
                    PAD(' ');
                }

                while (len-- > 0) {
                    outchar(*str++);
                }

                if (left) {
                    PAD(' ');
                }
            }
            break;

            case 'p':
                /* Generic pointer.  */
            {
                PTR ptr;
                nextarg(ptr, PTR);
                if (ptr != NULL) {
                    /* If the pointer is not NULL, write it as a %#x spec.  */
                    base = 16;
                    fc = 'x';
                    alt = 1;
                    num = (u64) (unsigned long int) ptr;
                    is_neg = 0;
                    goto number;
                } else {
                    /* Write "(nil)" for a nil pointer.  */
                    static char nil[] = "(nil)";
                    char *p;

                    width -= sizeof (nil) - 1;
                    if (!left) {
                        PAD(' ');
                    }

                    grouping = nil;
                    while (*grouping != '\0') {
                        outchar(*grouping++);
                    }

                    if (left) {
                        PAD(' ');
                    }
                }
            }
            break;

            case 'n':
                /* Answer the count of characters written.  */
                if (is_longlong) {
                    s64 *p;
                    nextarg(p, s64 *);
                    *p = done;
                } else if (is_long) {
                    long int *p;
                    nextarg(p, long int *);
                    *p = done;
                } else if (!is_short) {
                    int *p;
                    nextarg(p, int *);
                    *p = done;
                } else {
                    short int *p;
                    nextarg(p, short int *);
                    *p = done;
                }
                break;
            default:
                /* Unrecognized format specifier.  */
                break;
        }

        if (gSprintfSpacingCodes) {
            outchar(0x83);
        }
    }
    *s = '\0';
    return done;
}
#endif

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

s32 diPrintf_func_80061210(Gfx** gdl, char* buffer) {
    char* bufferCopy;
    s32 temp_hi;
    s32 xOffset;
    s32 v0;
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
    u8 bufferValue;

    bufferCopy = buffer;
    while ((bufferValue = *buffer++) != '\0') {
        xOffset = 0;
        switch (bufferValue) {
        case 0x83: // Leave fixed-width mode
            D_800BEAE4 = 0;
            break;
        case 0x84: // Enter fixed-width mode
            D_800BEAE4 = 1;
            break;
        case 0x81: // Set the text color from the next 4 bytes
            red = buffer[0];
            green = buffer[1];
            blue = buffer[2];
            alpha = buffer[3];
            buffer += 4;
            if (D_800BEAE8) {
                dl_set_env_color(gdl, red, green, blue, alpha);
            }
            break;
        case 0x87:
            D_800931B4 = buffer[0];
            D_800931B8 = buffer[1];
            buffer += 2;
            if (D_800931B4 || D_800931B8) {
                gDPSetOtherMode(
                    *gdl,
                    G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE | 0x1,
                    G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2
                );
                dl_apply_other_mode(gdl);
            } else {
                gDPSetOtherMode(
                    *gdl,
                    G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2
                );
                dl_apply_other_mode(gdl);
            }
            break;
        case 0x85: // Set the background color from the next 4 bytes
            red = buffer[0];
            green = buffer[1];
            blue = buffer[2];
            alpha = buffer[3];
            buffer += 4;
            if (!D_800BEAE8) {
                D_800BEB04 = red;
                D_800BEB05 = green;
                D_800BEB06 = blue;
                D_800BEB07 = alpha;
                dl_set_prim_color(gdl, red, green, blue, alpha);
            }
            break;
        case 0x82: // Set debug text position from the next 4 bytes
            if (!D_800BEAE8) {
                diPrintfRenderBackground(gdl, D_800BEAE0, D_800BEAE2, D_800BEADC, D_800BEADE + 10);
            }
            D_800BEADC = buffer[0];
            D_800BEADC |= buffer[1] << 8;
            D_800BEADE = buffer[2];
            D_800BEADE |= buffer[3] << 8;
            D_800BEAE0 = D_800BEADC;
            D_800BEAE2 = D_800BEADE;
            buffer += 4;
            break;
        case 0x86:
            D_800930E4 = buffer[0];
            D_800930E4 |= buffer[1] << 8;
            buffer += 2;
            break;
        case ' ': // Space
            xOffset = 6;
            break;
        case '\n': // Line Feed
            if (!D_800BEAE8) {
                diPrintfRenderBackground(gdl, D_800BEAE0, D_800BEAE2, D_800BEADC, D_800BEADE + 10);
            }
            diPrintfNewline();
            D_800BEAE0 = D_800BEADC;
            D_800BEAE2 = D_800BEADE;
            break;
        case '\t': // HT - Horizontal Tab
            temp_hi = D_800BEADC % D_800930E4;
            if (temp_hi == 0) {
                xOffset = D_800930E4;
            } else {
                xOffset = D_800930E4 - temp_hi;
            }
            break;
        default:
            xOffset = diPrintfRenderChar(gdl, bufferValue);
        }

        if (D_800BEAE4 != 0 && bufferValue >= 0x20 && bufferValue < 0x80) {
            xOffset = 7;
        }

        D_800BEADC += xOffset;
        if ((D_800BEB00 - 16) < D_800BEADC) {
            if (!D_800BEAE8) {
                diPrintfRenderBackground(gdl, D_800BEAE0, D_800BEAE2, D_800BEADC, D_800BEADE + 10);
            }
            diPrintfNewline();
            D_800BEAE0 = D_800BEADC;
            D_800BEAE2 = D_800BEADE;
        }
    }

    return buffer - bufferCopy;
}

void diPrintfRenderBackground(Gfx** gdl, u32 ulx, u32 uly, u32 lrx, u32 lry) {
    s32 temp_v0;
    s32 temp_v1;

    if (((ulx == lrx) | (uly == lry)) == 0) {
        if (ulx >= 2U) {
            ulx -= 2;
        }
        lrx += 2;
        temp_v0 = D_800931AC + D_800931B4;
        temp_v1 = D_800931B0 + D_800931B8;
        ulx <<= temp_v0;
        lrx <<= temp_v0;
        uly <<= temp_v1;
        lry <<= temp_v1;
        gDPSetCombineMode(*gdl, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
        dl_apply_combine(gdl);
        if ((D_800931B4 != 0) || (D_800931B8 != 0)) {
            dl_set_prim_color(gdl, 0U, 0U, 0U, 0xFFU);
            gDPFillRectangle((*gdl)++, ulx - 4, uly - 4, lrx + 4, lry + 4);
            gDLBuilder->needsPipeSync = 1;
            dl_set_prim_color(gdl, D_800BEB04, D_800BEB05, D_800BEB06, D_800BEB07);
        }
        gDPFillRectangle((*gdl)++, ulx, uly, lrx, lry);
        gDLBuilder->needsPipeSync = 1;
    }
}

s32 diPrintfRenderChar(Gfx** gdl, s32 asciiVal) {
    s32 fontCharU;
    s32 sp28;
    s32 sp24;
    s32 fontCharWidth;

    if (asciiVal < 0x40) {
        // Character is a symbol or number and not a letter
        if (D_800BEAFC != 0) {
            if (D_800BEAE8) {
                gDPLoadTextureBlockS((*gdl)++, OS_K0_TO_PHYSICAL(gDiTextures[0] + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 192, 11,
                                    0, 2, 2, 0, 0, 0, 0);
            }
            D_800BEAFC = 0;
        }
        asciiVal -= 0x21;
    } else if (asciiVal < 0x60) {
        // Character is a upper case letter
        if (D_800BEAFC != 1) {
            if (D_800BEAE8) {
                gDPLoadTextureBlockS((*gdl)++, OS_K0_TO_PHYSICAL(gDiTextures[1] + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 248, 11,
                                    0, 2, 2, 0, 0, 0, 0);
            }
            D_800BEAFC = 1;
        }
        asciiVal -= 0x40;
    } else if (asciiVal < 0x80) {
        // Character is a lower case letter
        if (D_800BEAFC != 2) {
            if (D_800BEAE8) {
                gDPLoadTextureBlockS((*gdl)++, OS_K0_TO_PHYSICAL(gDiTextures[2] + 1), G_IM_FMT_IA, G_IM_SIZ_8b, 192, 11,
                                    0, 2, 2, 0, 0, 0, 0);
            }
            D_800BEAFC = 2;
        }
        asciiVal -= 0x60;
    }
    fontCharU = gDiFontCoords[D_800BEAFC][asciiVal].u;
    fontCharWidth = (gDiFontCoords[D_800BEAFC][asciiVal].v - fontCharU) + 1;
    if (D_800BEAE8 != 0) {
        sp28 = D_800BEADC << (D_800931AC + D_800931B4);
        sp24 = D_800BEADE << (D_800931B0 + D_800931B8);
        gDPSetCombineLERP(*gdl, 0, 0, 0, ENVIRONMENT, 0, 0, 0, TEXEL0, 0, 0, 0, ENVIRONMENT, 0, 0, 0, TEXEL0);
        dl_apply_combine(gdl);
        gSPTextureRectangle((*gdl)++,
            /*xl*/(sp28 << 2),
            /*yl*/(sp24 << 2),
            /*xh*/((sp28 + (fontCharWidth << (D_800931AC + D_800931B4))) << 2),
            /*yh*/((sp24 + (10 << (D_800931B0 + D_800931B8))) << 2),
            /*tile*/0,
            /*s*/(fontCharU << 5),
            /*t*/0,
            /*dsdx*/1 << (10 - D_800931AC - D_800931B4),
            /*dtdy*/1 << (10 - D_800931B0 - D_800931B8));
        gDLBuilder->needsPipeSync = TRUE;
    }
    return fontCharWidth;
}


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
