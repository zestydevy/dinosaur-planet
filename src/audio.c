#include "common.h"

static const char str_80099060[] = "OH DEAR - No audio DMA buffers left\n";
static const char str_80099088[] = "Dma not done\n";

#pragma GLOBAL_ASM("asm/nonmatchings/audio/init_audio.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio/func_80011AFC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio/start_alSyn_thread.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio/stop_alSyn_thread.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio/func_80011C70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio/NOOP_80011ec0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio/al_dame_sub.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio/al_syn_dma.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio/func_800120C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio/func_800121DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio/func_80012224.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio/func_80012230.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio/func_80012348.s")

#pragma GLOBAL_ASM("asm/nonmatchings/audio/func_80012358.s")
