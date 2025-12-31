#ifndef _SYS_AUDIO_H
#define _SYS_AUDIO_H

#include "PR/libaudio.h"
#include "PR/sched.h"
#include "PR/ultratypes.h"

#define NUM_OUTPUT_BUFFERS      3      /* Need three of these */

#define OUTPUT_RATE             22020
#define MAX_MESGS               8
#define QUIT_MSG                10

#define DMA_BUFFER_LENGTH       0x800  /* Larger buffers result in fewer DMA' but more  */
                                       /* memory being used.  */

#define NUM_ACMD_LISTS          2      /* two lists used by this example                */

#define NUM_DMA_BUFFERS         80     /* max number of dma buffers needed.             */
                                       /* Mainly dependent on sequences and sfx's       */
#define NUM_DMA_MESSAGES        80     /* The maximum number of DMAs any one frame can  */
                                       /* have.                                         */

#define FRAME_LAG               1      /* The number of frames to keep a dma buffer.    */
                                       /* Increasing this number causes buffers to not  */
                                       /* be deleted as quickly. This results in fewer  */
                                       /* DMA's but you need more buffers.              */

#define AUDIO_THREAD_STACK_SIZE 0x2000

typedef union {

    struct {
        short type;
    } gen;

    struct {
        short type;
        struct AudioInfo_s *info;
    } done;

    OSScMsg app;

} AudioMsg;

typedef struct {
    ALLink node;
    u32 startAddr;
    u32 lastFrame;
    char *ptr;
} AMDMABuffer;

typedef struct {
    u8 initialized;
    AMDMABuffer *firstUsed;
    AMDMABuffer *firstFree;
} AMDMAState;

void init_audio(OSSched* sched, OSPri threadPriority);
void start_audio_thread(void);
void stop_audio_thread(void);
void audio_func_800121DC(void);
void audio_func_80012224(s32 a0);
u8 audio_func_80012348(void);

#endif //_SYS_AUDIO_H
