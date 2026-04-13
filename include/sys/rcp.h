#ifndef _SYS_RCP_H
#define _SYS_RCP_H

#include "PR/sched.h"
#include "PR/ultratypes.h"
#include "sys/gfx/texture.h"
#include "types.h"

// Size: 0xC
typedef struct TextureTile {
/*0*/ Texture *tex;
/*4*/ s32 animProgress;
/*8*/ s16 x;
/*A*/ s16 y;
} TextureTile;

enum RCPScreenWriteFlags {
    SCREEN_WRITE_TRANSLUCENT = 0,
    SCREEN_WRITE_OPAQUE = 1,
    SCREEN_WRITE_CYC_COPY = 2
};

enum RCPTileWriteFlags {
    TILE_WRITE_POINT_FILT = 0x2,
    TILE_WRITE_FLIP_X = 0x1000,
    TILE_WRITE_FLIP_Y = 0x2000,
    TILE_WRITE_TRANSLUCENT = 0x4000
};

enum RCPClearScreenFlags {
    // Note: If letterboxing is active, the letterboxed area will still be cleared
    CLEAR_NONE = 0x0,
    CLEAR_COLOR = 0x1,
    CLEAR_ZBUFFER = 0x2
};

s32 gfxtask_run_xbus(Gfx *dlStart, Gfx *dlEnd, s32 param3);
s32 gfxtask_wait(void);
void gfxtask_init(OSSched *sched);

void rsp_init(Gfx **gdl);

void rcp_set_screen_color(u8 red, u8 green, u8 blue);
void rcp_set_border_color(u32 red, u32 green, s32 blue);

/** flags = RCPClearScreenFlags */
void rcp_clear_screen(Gfx **gdl, Mtx **mtx, s32 flags);
void rcp_tile_write(Gfx** gdl, TextureTile* tiles, s32 x, s32 y, u8 r, u8 g, u8 b, u8 a);
/** Draws a 2D texture to screen, scaling it down horizontally (from its left edge) if
  * widescreen is enabled in order to maintain aspect ratio.
  *
  * flags = RCPScreenWriteFlags
  */
void rcp_screen_full_write(Gfx** gdl, Texture* tex, s32 x, s32 y, s32 arg4, s32 frameno, s32 alpha, s32 flags);
/** flags = RCPScreenWriteFlags */
void rcp_screen_scroll_write(Gfx** gdl, Texture* tex, s32 dstX, s32 dstY, s32 top, s32 bottom, s32 alpha, s32 flags);
/** flags = RCPScreenWriteFlags */
void rcp_screen_write(Gfx** gdl, Texture* tex, s32 dstX, s32 dstY, s32 srcTop, s32 srcBottom, s32 frameno, s32 alpha, s32 flags);
void draw_pause_screen_freeze_frame(Gfx** gdl);
/** flags = RCPTileWriteFlags */
void rcp_tile_write_x(Gfx** gdl, TextureTile* tiles, f32 x, f32 y, f32 width, f32 height, s32 s, s32 t, f32 scaleX, f32 scaleY, u32 color, s32 flags);

#endif
