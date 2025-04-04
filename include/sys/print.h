#ifndef _SYS_PRINT_H
#define _SYS_PRINT_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "libc/stdarg.h"

typedef struct {
    u8 u, v;
} DiFontCoords;

#define DI_PRINTF_CMD_ARG_BYTE(val) *gDebugPrintBufferEnd = val; gDebugPrintBufferEnd++;
#define DI_PRINTF_CMD_ARG_SHORT(val) DI_PRINTF_CMD_ARG_BYTE(val) DI_PRINTF_CMD_ARG_BYTE(val >> 8)

#define DI_PRINTF_CMD_END DI_PRINTF_CMD_ARG_BYTE(0)

#define DI_PRINTF_CMD_SET_COLOR(red, green, blue, alpha) \
    DI_PRINTF_CMD_ARG_BYTE(0x81)                         \
    DI_PRINTF_CMD_ARG_BYTE(red)                          \
    DI_PRINTF_CMD_ARG_BYTE(green)                        \
    DI_PRINTF_CMD_ARG_BYTE(blue)                         \
    DI_PRINTF_CMD_ARG_BYTE(alpha)                        \
    DI_PRINTF_CMD_END

// This is a bit hacky, but it matches.
#define DI_PRINTF_CMD_SET_POSITION(x, y) \
    u16 tempX, tempY;                    \
    DI_PRINTF_CMD_ARG_BYTE(0x82)         \
    DI_PRINTF_CMD_ARG_BYTE(x & 0xFF)     \
    tempX = x >> 8;                      \
    DI_PRINTF_CMD_ARG_BYTE(tempX)        \
    DI_PRINTF_CMD_ARG_BYTE(y & 0xFF)     \
    tempY = y >> 8;                      \
    DI_PRINTF_CMD_ARG_BYTE(tempY)        \
    DI_PRINTF_CMD_END
    
#define DI_PRINTF_CMD_SET_BACKGROUND_COLOR(red, green, blue, alpha) \
    DI_PRINTF_CMD_ARG_BYTE(0x85)                                    \
    DI_PRINTF_CMD_ARG_BYTE(red)                                     \
    DI_PRINTF_CMD_ARG_BYTE(green)                                   \
    DI_PRINTF_CMD_ARG_BYTE(blue)                                    \
    DI_PRINTF_CMD_ARG_BYTE(alpha)                                   \
    DI_PRINTF_CMD_END

#define DI_PRINTF_CMD_0x87(param1, param2) \
    DI_PRINTF_CMD_ARG_BYTE(0x87)           \
    DI_PRINTF_CMD_ARG_BYTE(param1)         \
    DI_PRINTF_CMD_ARG_BYTE(param2)         \
    DI_PRINTF_CMD_END

#define DI_PRINTF_CMD_0x86(param1)                  \
    DI_PRINTF_CMD_ARG_BYTE(0x86)                    \
    DI_PRINTF_CMD_ARG_BYTE(param1 & 0xFFFF)         \
    DI_PRINTF_CMD_ARG_BYTE((param1 >> 8) & 0xFFFF)  \
    DI_PRINTF_CMD_END
    
void sprintfSetSpacingCodes(s32 setting);
int sprintf(char *str, const char *format, ...);
int vsprintf(char *str, const char *format, va_list args);

void diPrintfInit();
int diPrintf(const char *format, ...);
void diPrintfAll(Gfx **gdl);
void diPrintfClear();
void diPrintfSetCol(u8 red, u8 green, u8 blue, u8 alpha);
void diPrintfSetBG(u8 red, u8 green, u8 blue, u8 alpha);
void diPrintfCmd_0x87(u8, u8);
void diPrintfSetXY(u16 x, u16 y);
void diPrintfCmd_0x86(u16);
s32 diPrintfMeasureString(const char *format, ...);
void diPrintfOrigin();
void diPrintfNewline();

#endif
