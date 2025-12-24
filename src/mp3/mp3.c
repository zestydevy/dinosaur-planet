// @DECOMP_OPT_FLAGS=-g
// @DECOMP_IDO_VERSION=7.1
#include "libnaudio/n_abi.h"
#include "libnaudio/n_synthInternals.h"
#include "mp3/mp3.h"
#include "mp3/mp3_internal.h"
#include "sys/audio/speaker.h"

#define N_EQPOWER_LENGTH 128
extern s16 n_eqpower[N_EQPOWER_LENGTH];
extern s16 _getRate(f32 vol, f32 tgt, s32 count, u16 *ratel);
extern s16 _getVol(s16 ivol, s32 samples, s16 ratem, u16 ratel);

/**
 * These two ABI commands are used in this file, but the format of the data
 * doesn't match the format used by n_audio's ABI.
 */
#define	aMp3ExecDma(pkt, a, b)                               \
{                                                            \
	Acmd *_a = (Acmd *)pkt;                                  \
	_a->words.w0 = _SHIFTL(0x07, 24, 8) | _SHIFTL(a, 0, 16); \
	_a->words.w1 = b;                                        \
}

#define	aMp3SetAddr(pkt, a)              \
{                                        \
	Acmd *_a = (Acmd *)pkt;              \
	_a->words.w0 = _SHIFTL(0x08, 24, 8); \
	_a->words.w1 = a;                    \
}

/* -------- .bss start 800bff00 -------- */
struct mp3vars g_Mp3Vars;
/* -------- .bss end 800bff70 -------- */

void mp3_update_vars(struct mp3vars*);
s32 mp3_handle_dma(s32 arg0, u8 *dst, s32 len, s32 dmaoffset);
void mp3_set_dma_func(mp3_dmafunc fn);
void mp3_dma(void);

void mp3_init(ALHeap *heap) {
    bzero(&g_Mp3Vars, sizeof(g_Mp3Vars));
    
    D_800C01C0 = alHeapDBAlloc(NULL, 0, heap, 0xA410, 1);
    D_800C01C4 = alHeapDBAlloc(NULL, 0, heap, 0x8000, 1);
    
    mp3_main_init();
    
    g_Mp3Vars.em_state = alHeapDBAlloc(NULL, 0, heap, 1, ALIGN(sizeof(ENVMIX_STATE), 16));
    g_Mp3Vars.var8009c3d4[0] = alHeapDBAlloc(NULL, 0, heap, 1, 0x440);
    g_Mp3Vars.state = MP3STATE_IDLE;
    g_Mp3Vars.currentvol = AL_VOL_FULL;
    g_Mp3Vars.currentpan = g_Mp3Vars.targetpan = AL_PAN_CENTER;
    g_Mp3Vars.em_volume = AL_VOL_FULL;
    g_Mp3Vars.em_pan = AL_PAN_CENTER;
    g_Mp3Vars.em_cvolL = (n_eqpower[g_Mp3Vars.em_pan] * g_Mp3Vars.em_volume) >> 15;
    g_Mp3Vars.em_cvolR = (n_eqpower[N_EQPOWER_LENGTH - (g_Mp3Vars.em_pan) - 1] * g_Mp3Vars.em_volume) >> 15;
    g_Mp3Vars.em_first = 1;
    g_Mp3Vars.em_dryamt = 0x7FFC;
    g_Mp3Vars.em_wetamt = 0;
    
    mp3_set_dma_func(mp3_handle_dma);
}

void mp3_play_file(s32 romAddr, s32 size) {
    if (g_Mp3Vars.dmafunc == NULL) {
        return;
    }
    g_Mp3Vars.romaddr = romAddr;
    g_Mp3Vars.filesize = size;
    g_Mp3Vars.dmaoffset = 0;
    g_Mp3Vars.var8009c3e8 = 0;
    g_Mp3Vars.currentvol = AL_VOL_FULL;
    g_Mp3Vars.statetimer = 5;
    
    mp3_dma();
    
    g_Mp3Vars.state = MP3STATE_LOADING;
}

void mp3_stop(void) {
    g_Mp3Vars.state = MP3STATE_STOPPED;
}

void mp3_pause(void) {
    g_Mp3Vars.state = MP3STATE_PAUSED;
}

void mp3_unpause(void) {
    g_Mp3Vars.statetimer = 5;
    g_Mp3Vars.state = MP3STATE_UNPAUSING;
}

s32 mp3_is_busy(void) {
    if (g_Mp3Vars.state == MP3STATE_PLAYING || 
        g_Mp3Vars.state == MP3STATE_LOADING || 
        g_Mp3Vars.state == MP3STATE_UNPAUSING || 
        g_Mp3Vars.state == MP3STATE_PAUSED) {
        return (s32) g_Mp3Vars.state;
    } else {
        return 0;
    }
}

void mp3_set_volume(s32 vol, s32 arg1) {
    if (vol < 0) {
        g_Mp3Vars.currentvol = 0;
    } else if (vol > AL_VOL_FULL) {
        g_Mp3Vars.currentvol = AL_VOL_FULL;
    } else {
        g_Mp3Vars.currentvol = vol;
    }
    g_Mp3Vars.var8009c3e8 = (u32) arg1;
}

void mp3_set_pan(s32 pan, s32 immediate) {
    if (pan > 127) {
        pan = 127;
    }
    if (pan < 0) {
        pan = 0;
    }
    g_Mp3Vars.targetpan = (s16) pan;
    if (immediate != 0) {
        g_Mp3Vars.currentpan = g_Mp3Vars.targetpan;
    }
}

s32 mp3_make_samples(s32 arg0, Acmd** cmd) {
    s32 i;
    s32 diff;
    s32 numchannels;
    u16* sp58;
    u16* sp54;
    s32 sp50;
    s32 sp4C;
    s32 sp48;

    numchannels = 0;
    sp54 = NULL;
    sp4C = N_AL_MAIN_L_OUT;
    sp48 = N_AL_MAIN_R_OUT;
    if (g_Mp3Vars.currentpan != g_Mp3Vars.targetpan) {
        diff = g_Mp3Vars.targetpan - g_Mp3Vars.currentpan;
        if ((diff >= -2) && (diff < 3)) {
            g_Mp3Vars.currentpan = g_Mp3Vars.currentpan + diff;
        } else if (diff >= 3) {
            g_Mp3Vars.currentpan = g_Mp3Vars.currentpan + 2;
        } else {
            g_Mp3Vars.currentpan = g_Mp3Vars.currentpan - 2;
        }
    }
    if (g_Mp3Vars.state == MP3STATE_LOADING) {
        mp3_dma();
        if (g_Mp3Vars.statetimer == 0) {
            g_Mp3Vars.stream = mp3_main_func_80072380(0, g_Mp3Vars.dmafunc, g_Mp3Vars.filesize);
            if (g_Mp3Vars.stream == NULL) {
                g_Mp3Vars.state = MP3STATE_IDLE;
                return FALSE;
            } else {
                g_Mp3Vars.state = MP3STATE_PLAYING;
                g_Mp3Vars.em_first = 1;
                g_Mp3Vars.var8009c3c8 = NULL;
                g_Mp3Vars.var8009c3cc = 0;
                g_Mp3Vars.var8009c3d0 = 0;
                g_Mp3Vars.var8009c3d8 = 0;
                bzero(g_Mp3Vars.var8009c3d4[0], 0x440);
            }
        } else {
            g_Mp3Vars.statetimer--;
        }
    }
    if (g_Mp3Vars.state == MP3STATE_UNPAUSING) {
        mp3_dma();
        if (g_Mp3Vars.statetimer == 0) {
            g_Mp3Vars.state = MP3STATE_PLAYING;
        } else {
            g_Mp3Vars.statetimer--;
        }
    }
    if (g_Mp3Vars.state == MP3STATE_PLAYING) {
        if ((arg0 + g_Mp3Vars.var8009c3d0) > g_Mp3Vars.var8009c3cc) {
            g_Mp3Vars.var8009c3cc -= g_Mp3Vars.var8009c3d0;
            if ((g_Mp3Vars.var8009c3cc != 0) && (g_Mp3Vars.var8009c3c8 != NULL)) {
                sp54 = &g_Mp3Vars.var8009c3c8[g_Mp3Vars.var8009c3d0];
                sp50 = g_Mp3Vars.var8009c3cc;
            }
            g_Mp3Vars.var8009c3d0 = 0;
            g_Mp3Vars.var8009c3cc = 0;
            if (mp3_main_func_8007245c(g_Mp3Vars.stream, &sp58, &numchannels) != 0) {
                g_Mp3Vars.var8009c3c8 = sp58;
                
                for (i = 0; i < numchannels; i++) {
                    aMp3SetAddr((*cmd)++, osVirtualToPhysical(g_Mp3Vars.var8009c3d4[i]));
					aMp3ExecDma((*cmd)++, g_Mp3Vars.var8009c3d8, osVirtualToPhysical(sp58));

                    sp58 += 580;
                }
                g_Mp3Vars.var8009c3d8 = (g_Mp3Vars.var8009c3d8 - 0x24) & 0x1E;
                g_Mp3Vars.var8009c3cc = 0x240;
                g_Mp3Vars.dualchannel = numchannels == 2;
            } else {
                
            }
        }
        mp3_dma();
        if (g_Mp3Vars.dualchannel == 0) {
            sp4C = 0;
        }
        if (sp54 != NULL) {
            n_aLoadBuffer((*cmd)++, sp50 + sp50, sp4C, osVirtualToPhysical(sp54));
            
            if (g_Mp3Vars.dualchannel != 0) {
                sp54 += 580;
                n_aLoadBuffer((*cmd)++, sp50 + sp50, sp48, osVirtualToPhysical(sp54));
            }
            sp4C += sp50 + sp50;
            sp48 += sp50 + sp50;
            arg0 -= sp50;
        }
        if ((arg0 > 0) && (g_Mp3Vars.var8009c3c8 != NULL)) {
            sp54 = &g_Mp3Vars.var8009c3c8[g_Mp3Vars.var8009c3d0];
            n_aLoadBuffer((*cmd)++, arg0 + arg0, sp4C, osVirtualToPhysical(sp54));
            if (g_Mp3Vars.dualchannel != 0) {
                sp54 += 580;
                n_aLoadBuffer((*cmd)++, arg0 + arg0, sp48, osVirtualToPhysical(sp54));
            }
            g_Mp3Vars.var8009c3d0 += arg0;
        }
        mp3_update_vars(&g_Mp3Vars);
        if (g_Mp3Vars.dualchannel == 0) {
            aClearBuffer((*cmd)++, N_AL_MAIN_L_OUT, N_AL_TEMP_2);
            
            if (g_Mp3Vars.em_first != 0) {
                g_Mp3Vars.em_first = 0;

                g_Mp3Vars.em_ltgt = (g_Mp3Vars.em_volume * n_eqpower[g_Mp3Vars.em_pan]) >> 15;
				g_Mp3Vars.em_lratm = _getRate(g_Mp3Vars.em_cvolL, g_Mp3Vars.em_ltgt, g_Mp3Vars.em_segEnd, &g_Mp3Vars.em_lratl);

				g_Mp3Vars.em_rtgt = (g_Mp3Vars.em_volume * n_eqpower[N_EQPOWER_LENGTH - (g_Mp3Vars.em_pan) - 1]) >> 15;
				g_Mp3Vars.em_rratm = _getRate(g_Mp3Vars.em_cvolR, g_Mp3Vars.em_rtgt, g_Mp3Vars.em_segEnd, &g_Mp3Vars.em_rratl);

                n_aSetVolume((*cmd)++, A_VOL | A_LEFT, g_Mp3Vars.em_cvolL, g_Mp3Vars.em_dryamt, g_Mp3Vars.em_wetamt);
				n_aSetVolume((*cmd)++, A_VOL | A_RIGHT, g_Mp3Vars.em_rtgt, g_Mp3Vars.em_rratm, g_Mp3Vars.em_rratl);
				n_aSetVolume((*cmd)++, A_RATE, g_Mp3Vars.em_ltgt, g_Mp3Vars.em_lratm, g_Mp3Vars.em_lratl);
				n_aEnvMixer((*cmd)++, A_INIT, g_Mp3Vars.em_cvolR, osVirtualToPhysical(g_Mp3Vars.em_state));

            } else {
                n_aEnvMixer((*cmd)++, A_CONTINUE, 0, osVirtualToPhysical(g_Mp3Vars.em_state));
            }
            g_Mp3Vars.em_delta += SAMPLES;
            if (g_Mp3Vars.em_delta > g_Mp3Vars.em_segEnd) {
                g_Mp3Vars.em_delta = g_Mp3Vars.em_segEnd;
            }
        }
    } else {
        return FALSE;
    }
    return TRUE;
}

void mp3_update_vars(struct mp3vars* vars) {
    if ((vars->em_volume != vars->currentvol) || (vars->em_pan != vars->currentpan)) {
        if (vars->em_delta >= vars->em_segEnd) {
            vars->em_ltgt = (n_eqpower[vars->em_pan] * vars->em_volume >> 15);
			vars->em_rtgt = (n_eqpower[N_EQPOWER_LENGTH - (vars->em_pan) - 1] * vars->em_volume >> 15);
            vars->em_delta = vars->em_segEnd;
            vars->em_cvolL = vars->em_ltgt;
            vars->em_cvolR = vars->em_rtgt;
        } else {
            vars->em_cvolL = _getVol(vars->em_cvolL, vars->em_delta, vars->em_lratm, vars->em_lratl);
            vars->em_cvolR = _getVol(vars->em_cvolR, vars->em_delta, vars->em_rratm, vars->em_rratl);
        }
        if (vars->em_cvolL == 0) {
            vars->em_cvolL = 1;
        }
        if (vars->em_cvolR == 0) {
            vars->em_cvolR = 1;
        }
        vars->em_volume = (s16) vars->currentvol;
        if (vars->em_pan != vars->currentpan) {
            if (D_800BFE82.headphone != 0) {
                vars->em_pan = ((s16) vars->currentpan >> 1) + 32;
            } else if (D_800BFE82.mono != 0) {
                vars->em_pan = AL_PAN_CENTER;
            } else {
                vars->em_pan = vars->currentpan;
            }
        }
        vars->em_delta = 0;
        vars->em_segEnd = SAMPLE184(vars->var8009c3e8);
        vars->em_first = 1;
    }
}

void mp3_set_dma_func(mp3_dmafunc fn) {
    g_Mp3Vars.dmafunc = fn;
}

s32 mp3_handle_dma(s32 arg0, u8 *dst, s32 len, s32 dmaoffset) {
    u8 *bufptr;
    ALDMAproc proc;

    if (dmaoffset != -1) {
        g_Mp3Vars.dmaoffset = dmaoffset;
    }
    if ((g_Mp3Vars.dmaoffset + len) > g_Mp3Vars.filesize) {
        len = g_Mp3Vars.filesize - g_Mp3Vars.dmaoffset;
    }
    proc = n_syn->dma(&bufptr);
    bufptr = (u8 *) OS_K0_TO_PHYSICAL(proc(g_Mp3Vars.romaddr + g_Mp3Vars.dmaoffset, len, NULL));
    bcopy(bufptr, dst, len);
    g_Mp3Vars.dmaoffset += len;
    return len;
}

void mp3_dma(void) {
    u8 *bufptr;
    ALDMAproc proc;

    proc = n_syn->dma(&bufptr);
    proc(g_Mp3Vars.romaddr + g_Mp3Vars.dmaoffset, 0x400, NULL);
}
