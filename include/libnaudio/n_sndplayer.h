#ifndef __N_SNDPLAYER__
#define __N_SNDPLAYER__

#include "libnaudio/n_libaudio.h"
#include "PR/libaudio.h"

struct sndstate;

typedef struct sndstate {
	/*0x00*/ ALLink node;
	/*0x08*/ ALSound *sound;
	/*0x0c*/ N_ALVoice voice;
	/*0x28*/ f32 basepitch;
	/*0x2c*/ f32 pitch;
	/*0x30*/ struct sndstate **unk30;
	/*0x34*/ s32 unk34;
	/*0x38*/ s16 vol;
	/*0x3a*/ s16 envvol;
	/*0x3c*/ ALMicroTime endtime;
	/*0x40*/ u8 priority;
	/*0x41*/ ALPan pan;
	/*0x42*/ u8 fxmix;
	/*0x43*/ u8 fxbus;
	/*0x44*/ u8 flags;
	/*0x45*/ u8 state;
	/*0x46*/ u16 soundnum;
} sndstate;

void audioStop(sndstate *state);
void audioPostEvent(sndstate *state, s16 type, s32 data);
s32 sndGetState(sndstate *state);
void func_80066978(u8 index, u16 volume);
sndstate *some_sound_func(ALBank *bank, s16 soundnum, u16 vol, ALPan pan, f32 pitch, u8 fxmix, u8 fxbus, sndstate **handleptr);

#endif /*  __N_SNDPLAYER__ */
