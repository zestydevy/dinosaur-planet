#ifndef _LIBNAUDIO_MP3_H
#define _LIBNAUDIO_MP3_H

#include "ultra64.h"
#include "libnaudio/n_sndplayer.h"

void mp3_init(s32 arg0);
void mp3_play_file(s32 romAddr, s32 size);
void func_8006758C(sndstate *state);
s32 func_800675EC(sndstate*);
void func_80067650(s32, s32);
void func_800676A4(s8, s32);
s32 func_800676F0(s32 arg0, Acmd **cmd);

#endif
