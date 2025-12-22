#include "PR/libaudio.h"
#include "PR/os.h"
#include "libnaudio/n_libaudio.h"
#include "libnaudio/n_unkfuncs.h"
#include "sys/asset_thread.h"
#include "sys/audio/speaker.h"
#include "sys/audio.h"
#include "sys/fs.h"
#include "sys/gfx/gx.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "dll.h"
#include "macros.h"

/* -------- .data start -------- */
s32 gCurAcmdList = 0;
s32 gAudioInfoIdx = 1;
s32 gLastAudioInfoIdx = 0;
u32 audFrameCt = 0;
u32 nextDMA = 0;
u8 D_8008C8D4 = 1;
static u8 _unk[1] = {0};
u8 D_8008C8DC[4] = {0, 0, 0, 0};
/* -------- .data end -------- */

/* -------- .bss start 800a8690 -------- */
OSSched* gAudioSched;
OSScClient gAudioScClient;
OSThread gAudioThread;
OSScTask gAudioTask;
u64 gAudioThreadStack[AUDIO_THREAD_STACK_SIZE];
N_ALGlobals __am_g;
Acmd *__am_ACMDList[NUM_ACMD_LISTS];
void *__am_audioInfo[NUM_OUTPUT_BUFFERS];
short gFrameSamplesList[NUM_OUTPUT_BUFFERS];
OSMesgQueue __am_audioFrameMsgQ;
OSMesgQueue __am_audioReplyMsgQ;
OSMesg __am_audioFrameMsgBuf[MAX_MESGS];
OSMesg __am_audioReplyMsgBuf[MAX_MESGS];
AMDMAState dmaState;
AMDMABuffer gAudDmaBuffs[NUM_DMA_BUFFERS];
s32 D_800AB078; // gMinFrameSize?
s32 D_800AB07C; // gFrameSize?
s32 D_800AB080; // gMaxFrameSize?
s32 D_800AB084; // gAudioLagFrames?
OSIoMesg audDMAIOMesgBuf[NUM_DMA_MESSAGES];
OSMesgQueue audDMAMessageQ;
OSMesg audDMAMessageBuf[NUM_DMA_MESSAGES];
u32 D_800AB960;
/* -------- .bss end 800ab970 -------- */

// @bug: This file uses the wrong signature for mpeg_init
#ifdef AVOID_UB
#include "sys/mpeg.h" 
#else
extern void mpeg_init(void);
#endif

void __amMain(void *arg);
void __amHandleFrameMsg(void);
void __amHandleDoneMsg(void);
void __clearAudioDMA(void);
ALDMAproc __amDmaNew(AMDMAState **state);

// Official name: amCreateAudioMgr ?
void init_audio(OSSched* sched, OSPri threadPriority) {
    ALSynConfig c;
    f32 fsize;
    s32 frameSize;
    u32* audioTab;
    s32 offset;
    s32 size;
    s32 i;

    audioTab = NULL;
    
#ifdef AVOID_UB
    mpeg_init(NULL);
#else
    mpeg_init();
#endif
    
    c.maxVVoices = c.maxPVoices = 72;
    c.maxUpdates = 96;
    c.maxFXbusses = 2;
    c.outputRate = osAiSetFrequency(OUTPUT_RATE);
    c.dmaproc = &__amDmaNew;
    c.heap = NULL;
    
    queue_alloc_load_file((void**)&audioTab, AUDIO_TAB);
    
    offset = audioTab[24];
    size = audioTab[25] - offset;
    queue_load_file_region_to_ptr((void** ) &D_800AB960, AUDIO_BIN, offset, size);
    
    offset = audioTab[D_800AB960 + 3];
    size = audioTab[D_800AB960 + 4] - offset;
    
    c.fxTypes[0] = AL_FX_CUSTOM;
    c.params[0] = mmAlloc(size, 0xB, NULL);
    queue_load_file_region_to_ptr((void**)c.params[0], AUDIO_BIN, offset, size);
    
    c.fxTypes[1] = AL_FX_CUSTOM;
    c.params[1] = mmAlloc(size, 0xB, NULL);
    queue_load_file_region_to_ptr((void**)c.params[1], AUDIO_BIN, offset, size);
    
    n_alInit(&__am_g, &c);
    
    mmFree(c.params[0]);
    mmFree(c.params[1]);
    mmFree(audioTab);
    
    /*
     * Calculate the frame sample parameters from the
     * video field rate and the output rate
     */
    fsize = ((f32) c.outputRate * 2) / (f32) gDisplayHertz;
    D_800AB078 = (s32)fsize;
    if (D_800AB078 < fsize) {
        D_800AB078++;
    }
    D_800AB078 = ((D_800AB078 / 184) * 184) + 184;
    D_800AB07C = D_800AB078 - 184;
    D_800AB080 = D_800AB078 + 80;

    D_800AB084 = 0;

    for (i = 0; i < NUM_ACMD_LISTS; i++) {
        __am_ACMDList[i] = (Acmd*)alHeapAlloc(c.heap, 1, 0x4000);
    }
    for (i = 0; i < NUM_OUTPUT_BUFFERS; i++) {
        __am_audioInfo[i] = alHeapAlloc(c.heap, 1, D_800AB080 * 4);
        gFrameSamplesList[i] = 0;
    }
    
    gAudDmaBuffs->node.prev = NULL;
    gAudDmaBuffs->node.next = NULL;
    for (i = 0; i < (NUM_DMA_BUFFERS - 1); i++) {
        alLink(&gAudDmaBuffs[i + 1].node, &gAudDmaBuffs[i].node);
        gAudDmaBuffs[i].ptr = alHeapAlloc(c.heap, 1, 0x800);
    }
    /* last buffer already linked, but still needs buffer */
    gAudDmaBuffs[i].ptr = alHeapAlloc(c.heap, 1, 0x800);
    
    gAudioSched = sched;
    osCreateMesgQueue(&__am_audioReplyMsgQ, __am_audioReplyMsgBuf, MAX_MESGS);
    osCreateMesgQueue(&__am_audioFrameMsgQ, __am_audioFrameMsgBuf, MAX_MESGS);
    osCreateMesgQueue(&audDMAMessageQ, audDMAMessageBuf, NUM_DMA_MESSAGES);
    osCreateThread(&gAudioThread, 4, __amMain, NULL, 
        &gAudioThreadStack[AUDIO_THREAD_STACK_SIZE], threadPriority);
    
    speaker_set_mode(4);
    speaker_func_80063bb4(0, 4);
    speaker_func_80063bb4(1, 4);
}

void __amMain(void *arg) {
    AudioMsg *msg = NULL;
    OSMesg *lastInfo = 0;
    s32 done = 0;

    osScAddClient(gAudioSched, &gAudioScClient, &__am_audioFrameMsgQ, OS_SC_ID_AUDIO);

    while (!done) {
        (void) osRecvMesg(&__am_audioFrameMsgQ, (OSMesg *) &msg, OS_MESG_BLOCK);
        switch (msg->gen.type) {
            case OS_SC_RETRACE_MSG:
                __amHandleFrameMsg();
                /* wait for done message */
                osRecvMesg(&__am_audioReplyMsgQ, (OSMesg *) &lastInfo, OS_MESG_BLOCK);
                __amHandleDoneMsg();
                break;
            case OS_SC_PRE_NMI_MSG:
                /* what should we really do here? quit? ramp down volume? */
                break;
            case QUIT_MSG:
                done = 1;
                break;
            default:
                break;
        }
    }

    n_alClose(&__am_g);
}

// Official Name: amGo
void start_audio_thread(void) {
    if (alSynFlag) {
        osStartThread(&gAudioThread);
    }
}

// Official Name: amGoStop
void stop_audio_thread(void) {
    osStopThread(&gAudioThread);
}

void __amHandleFrameMsg(void) {
    s32 _pad;
    Acmd* cmp;
    s32 audioCmdLen;
    s32 samplesLeft;
    s16 *audioPtr;

    __clearAudioDMA();
    
    audioPtr = (s16*)osVirtualToPhysical(__am_audioInfo[gAudioInfoIdx]);
    samplesLeft = IO_READ(AI_LEN_REG) >> 2;
    
    osAiSetNextBuffer(__am_audioInfo[gLastAudioInfoIdx], gFrameSamplesList[gLastAudioInfoIdx] * 4);
    
    if ((samplesLeft >= 249) && (D_800AB084 == 0)) {
        gFrameSamplesList[gAudioInfoIdx] = D_800AB07C;
        D_800AB084 = 2;
    } else {
        gFrameSamplesList[gAudioInfoIdx] = D_800AB078;
        if (D_800AB084 != 0) {
            D_800AB084 -= 1;
        }
    }
    
    cmp = n_alAudioFrame(__am_ACMDList[gCurAcmdList], &audioCmdLen, audioPtr, gFrameSamplesList[gAudioInfoIdx]);
    
    gAudioTask.taskType = 1;
    gAudioTask.flags = 2;
    gAudioTask.next = NULL;
    gAudioTask.msgQ = &__am_audioReplyMsgQ;
    gAudioTask.msg = NULL;
    gAudioTask.unk58 = -1;
    gAudioTask.unk60 = 0xFF;
    gAudioTask.unk5C = 0;
    gAudioTask.unk64 = 0;
    
    gAudioTask.list.t.type = 2;
    gAudioTask.list.t.flags = 2;
    gAudioTask.list.t.ucode_boot = (u64* ) rspbootTextStart;
    gAudioTask.list.t.ucode_boot_size = (s32)gspF3DEX2_xbusTextStart - (s32)rspbootTextStart;
    gAudioTask.list.t.ucode = (u64* ) aspMainTextStart;
    gAudioTask.list.t.ucode_data = (u64* ) aspMainDataStart;
    gAudioTask.list.t.ucode_data_size = 0x800;
    gAudioTask.list.t.data_ptr = (u64* ) __am_ACMDList[gCurAcmdList];
    gAudioTask.list.t.data_size = (cmp - __am_ACMDList[gCurAcmdList]) * sizeof(Acmd);
    gAudioTask.list.t.yield_data_ptr = NULL;
    gAudioTask.list.t.yield_data_size = 0x400;
    
    osWritebackDCacheAll();
    osSendMesg(osScGetCmdQ(gAudioSched), &gAudioTask, 0);
    
    gCurAcmdList ^= 1;
    
    gLastAudioInfoIdx = gAudioInfoIdx;
    gAudioInfoIdx++;
    gAudioInfoIdx = gAudioInfoIdx % 3;
}

void __amHandleDoneMsg(void) { }

/*static*/ s32 __amDMA(s32 addr, s32 len, void *state) {
    AMDMABuffer *dmaPtr, *lastDmaPtr;
    void *foundBuffer;
    s32 delta, addrEnd, buffEnd;
    s32 pad;

    lastDmaPtr = NULL;
    delta = addr & 1;
    dmaPtr = dmaState.firstUsed;
    addrEnd = addr + len;

    /* first check to see if a currently existing buffer contains the
       sample that you need.  */

    while (dmaPtr) {
        buffEnd = dmaPtr->startAddr + DMA_BUFFER_LENGTH;
        if (dmaPtr->startAddr > (u32) addr) { /* since buffers are ordered */
            break;                            /* abort if past possible */
        } else if (addrEnd <= buffEnd) {      /* yes, found a buffer with samples */
            dmaPtr->lastFrame = audFrameCt;   /* mark it used */
            foundBuffer = dmaPtr->ptr + addr - dmaPtr->startAddr;
            return (int) osVirtualToPhysical(foundBuffer);
        }
        lastDmaPtr = dmaPtr;
        dmaPtr = (AMDMABuffer *) dmaPtr->node.next;
    }

    /* get here, and you didn't find a buffer, so dma a new one */

    /* get a buffer from the free list */
    dmaPtr = dmaState.firstFree;

    if (!dmaPtr && !lastDmaPtr) {
        lastDmaPtr = dmaState.firstUsed;
    }

    /*
     * if you get here and dmaPtr is null, send back a bogus
     * pointer, it's better than nothing
     */
    if (!dmaPtr) {
        STUBBED_PRINTF("OH DEAR - No audio DMA buffers left\n");
        return (int) osVirtualToPhysical(lastDmaPtr->ptr) + delta;
    }

    dmaState.firstFree = (AMDMABuffer *) dmaPtr->node.next;
    alUnlink((ALLink *) dmaPtr);

    /* add it to the used list */
    if (lastDmaPtr) { /* if you have other dmabuffers used, add this one */
                      /* to the list, after the last one checked above */
        alLink((ALLink *) dmaPtr, (ALLink *) lastDmaPtr);
    } else if (dmaState.firstUsed) { /* if this buffer is before any others */
                                     /* jam at begining of list */
        lastDmaPtr = dmaState.firstUsed;
        dmaState.firstUsed = dmaPtr;
        dmaPtr->node.next = (ALLink *) lastDmaPtr;
        dmaPtr->node.prev = 0;
        lastDmaPtr->node.prev = (ALLink *) dmaPtr;
    } else { /* no buffers in list, this is the first one */
        dmaState.firstUsed = dmaPtr;
        dmaPtr->node.next = 0;
        dmaPtr->node.prev = 0;
    }

    foundBuffer = dmaPtr->ptr;
    addr -= delta;
    dmaPtr->startAddr = addr;
    dmaPtr->lastFrame = audFrameCt; /* mark it */

    osPiStartDma(&audDMAIOMesgBuf[nextDMA++], OS_MESG_PRI_HIGH, OS_READ, addr, foundBuffer, DMA_BUFFER_LENGTH,
                 &audDMAMessageQ);

    return (int) osVirtualToPhysical(foundBuffer) + delta;
}

ALDMAproc __amDmaNew(AMDMAState **state) {
    if (!dmaState.initialized) { /* only do this once */
        dmaState.firstUsed = 0;
        dmaState.firstFree = &gAudDmaBuffs[0];
        dmaState.initialized = 1;
    }

    *state = &dmaState; /* state is never used in this case */

    return __amDMA;
}

void __clearAudioDMA(void) {
    u32 i;
    OSIoMesg *iomsg = 0;
    AMDMABuffer *dmaPtr, *nextPtr;

    /*
     * Don't block here. If dma's aren't complete, you've had an audio
     * overrun. (Bad news, but go for it anyway, and try and recover.)
     */
    for (i = 0; i < nextDMA; i++) {
        if (osRecvMesg(&audDMAMessageQ, (OSMesg *) &iomsg, OS_MESG_NOBLOCK) == -1) {
            STUBBED_PRINTF("Dma not done\n");
        }
    }

    dmaPtr = dmaState.firstUsed;
    while (dmaPtr) {
        nextPtr = (AMDMABuffer *) dmaPtr->node.next;

        /* remove old dma's from list */
        /* Can change FRAME_LAG value.  Should be at least one.  */
        /* Larger values mean more buffers needed, but fewer DMA's */
        if (dmaPtr->lastFrame + FRAME_LAG < audFrameCt) {
            if (dmaState.firstUsed == dmaPtr) {
                dmaState.firstUsed = (AMDMABuffer *) dmaPtr->node.next;
            }
            alUnlink((ALLink *) dmaPtr);
            if (dmaState.firstFree) {
                alLink((ALLink *) dmaPtr, (ALLink *) dmaState.firstFree);
            } else {
                dmaState.firstFree = dmaPtr;
                dmaPtr->node.next = 0;
                dmaPtr->node.prev = 0;
            }
        }
        dmaPtr = nextPtr;
    }

    nextDMA = 0; /* reset */
    audFrameCt++;
}

void audio_func_800121DC(void) {
    gDLL_5_AMSEQ2->vtbl->func18();
    gDLL_6_AMSFX->vtbl->func_338();
}

void audio_func_80012224(s32 a0) { }

void func_80012230(u8 a0) {
    u8 i;
    
    if (a0 == D_8008C8D4) {
        return;
    }

    D_8008C8D4 = a0;

    if (D_8008C8D4 != 0) {
        for (i = 0; i < 4; i++) {
            if (D_8008C8DC[i]) {
                gDLL_5_AMSEQ->vtbl->func4(i, D_8008C8DC[i]);
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            D_8008C8DC[i] = gDLL_5_AMSEQ->vtbl->func7(i);
            gDLL_5_AMSEQ->vtbl->func6(i);
        }

        func_80066854();
    }
}

u8 audio_func_80012348(void) {
    return D_8008C8D4;
}

void func_80012358(void) {
    alBnkfNew(NULL,NULL);
    alSeqFileNew(NULL,NULL);
    n_alCSPDelete(NULL);
    n_alCSPPlay(NULL);
    n_alCSPStop(NULL);
    n_alCSPSetBank(NULL,NULL);
    n_alCSPSetSeq(NULL,NULL);
    n_alCSPSetVol(NULL,0);
    n_alCSPGetTempo(NULL);
    n_alCSPSetTempo(NULL,0);
    n_alCSPGetState(NULL);
    n_alCSPGetChlPan(NULL,0);
    n_alCSPSetChlPan(NULL,0,0);
    n_alCSPGetChlVol(NULL,0);
    n_alCSPSetChlVol(NULL,0,0);
    n_alCSPGetChlFXMix(NULL,0);
}
