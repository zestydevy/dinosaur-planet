// @DECOMP_OPT_FLAGS=-O1
#include "libultra/rmon/rmonint.h"

OSMesgQueue rmonsio_bss_0000;
OSMesg rmonsio_bss_0018[1];

void __rmonSendFault(OSThread* thread) {
    volatile float var1;
    u8 *threadPtr;
    u32 var3;

    var3 = 0;
    var1 = 0.0f;
    threadPtr = (u8*)thread;

    // bug? 0x1b0 is the size of the original OSThread size. it doesn't include the added data in dino planet
    while (var3 < 0x1b0) {
	    var3 += __osRdbSend(&threadPtr[var3], 0x1b0 - var3, 2);
    }
}

void __rmonIOflush(void) {
    s32 var1;
    u8 var2[4];

    var1 = 0;

    while (var1 < 1) {
	    var1 += __osRdbSend(&var2[0], 1, 0xa);
    }
}

void __rmonIOputw(u32 word) {
    s32 var1;
    u8* var2;

    var1 = 0;
    var2 = (u8*)&word;

    while (var1 < 4) {
	    var1 += __osRdbSend(&var2[var1], 4 - var1, 8);
    }
}

void __rmonIOhandler(void) {
    s32 var1;
    u8 var2[4];

    osCreateMesgQueue(&rmonsio_bss_0000, &rmonsio_bss_0018[0], 1);
    osSetEventMesg(OS_EVENT_RDB_DBG_DONE, &rmonsio_bss_0000, 0);
    
    __osRdb_DbgRead_Buf = rmonRdbReadBuf;
	
    while (1) {
        osRecvMesg(&rmonsio_bss_0000, NULL, OS_MESG_BLOCK);

        __rmonExecute((KKHeader*)rmonRdbReadBuf);
        
        __osRdb_DbgRead_Buf = rmonRdbReadBuf;
        
        var1 = 0;
        while (var1 < 1) {
            var1 += __osRdbSend(&var2[0], 1, 0xb);
        }
    }
}
