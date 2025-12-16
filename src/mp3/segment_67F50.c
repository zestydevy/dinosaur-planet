// @DECOMP_OPT_FLAGS=-g
// @DECOMP_IDO_VERSION=7.1
#include "common.h"
#include "mp3/mp3.h"
#include "mp3/mp3_internal.h"

#define N_EQPOWER_LENGTH 128
extern s16 n_eqpower[N_EQPOWER_LENGTH];

// .bss
struct mp3vars g_Mp3Vars; // 0x800bff00

s32 mp3_func_80068278(s32 arg0, u8 *dst, s32 len, s32 dmaoffset);
void mp3_func_80068260(void *fn);

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
    
    mp3_func_80068260(mp3_func_80068278);
}

#pragma GLOBAL_ASM("asm/nonmatchings/mp3/segment_67F50/mp3_play_file.s")

#pragma GLOBAL_ASM("asm/nonmatchings/mp3/segment_67F50/mp3_func_8006758C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/mp3/segment_67F50/mp3_func_800675A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/mp3/segment_67F50/mp3_func_800675C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/mp3/segment_67F50/mp3_func_800675EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/mp3/segment_67F50/mp3_func_80067650.s")

#pragma GLOBAL_ASM("asm/nonmatchings/mp3/segment_67F50/mp3_func_800676A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/mp3/segment_67F50/mp3_func_800676F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/mp3/segment_67F50/mp3_func_8006804C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/mp3/segment_67F50/mp3_func_80068260.s")

#pragma GLOBAL_ASM("asm/nonmatchings/mp3/segment_67F50/mp3_func_80068278.s")

#pragma GLOBAL_ASM("asm/nonmatchings/mp3/segment_67F50/mp3_dma.s")
