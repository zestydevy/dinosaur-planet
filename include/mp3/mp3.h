#ifndef _MP3_H
#define _MP3_H

#include "ultra64.h"
#include "libnaudio/n_sndplayer.h"

void mp3Init(ALHeap *heap);
void mp3PlayFile(s32 romAddr, s32 size);
void mp3Stop(void);
s32 mp3IsBusy(void);
void mp3SetVolume(s32 vol, s32);
void mp3SetPan(s32 pan, s32 immediate);
s32 mp3MakeSamples(s32 arg0, Acmd **cmd);

#endif
