#ifndef _SYS_RCP_H
#define _SYS_RCP_H

#include "PR/sched.h"
#include "PR/ultratypes.h"
#include "types.h"

s32 gfxtask_run_xbus(Gfx *dlStart, Gfx *dlEnd, s32 param3);
s32 gfxtask_wait(void);
void gfxtask_init(OSSched *sched);

void rsp_init(Gfx **gdl);

void rcp_set_screen_color(u8 red, u8 green, u8 blue);
void rcp_set_border_color(u32 red, u32 green, s32 blue);

void func_80037A14(Gfx**, Mtx **, s32);
// Size: 0xC
typedef struct Func_80037F9C_Struct {
    Texture *unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
} Func_80037F9C_Struct;
void func_80037F9C(Gfx**, Func_80037F9C_Struct*, s32, s32, s32, s32, s32, s32);
/** Draws a 2D texture to screen, scaling it down horizontally (from its left edge) if
  * widescreen is enabled in order to maintain aspect ratio 
  */
void func_8003825C(Gfx **gdl, Texture*, s32, s32, s32, s32, s32, s32);
void func_800382AC(Gfx **gdl, Texture*, s32, s32, s32, s32, s32, s32);
void func_8003833C(Gfx** gdl, Texture* tex, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void draw_pause_screen_freeze_frame(Gfx** gdl);

#endif
