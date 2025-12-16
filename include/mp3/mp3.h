#ifndef _MP3_H
#define _MP3_H

#include "ultra64.h"
#include "libnaudio/n_sndplayer.h"

void mp3_init(ALHeap *heap);
void mp3_play_file(s32 romAddr, s32 size);
void mp3_stop(void);
s32 mp3_is_busy(void);
void mp3_set_volume(s32 vol, s32);
void mp3_set_pan(s32 pan, s32 immediate);
s32 mp3_func_800676F0(s32 arg0, Acmd **cmd);

#endif
