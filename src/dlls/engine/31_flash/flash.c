#include "PR/ultratypes.h"
#include "PR/os.h"
#include "dlls/engine/31_flash.h"
#include "sys/dll.h"
#include "sys/memory.h"

typedef struct {
    u32 flashType;
    u32 flashMaker;
    u32 unk[2];
} FlashId;

const char str1[] = "Savegame size in pages: %d %d\n";
const char str2[] = "flashLoadGame: dest=%x slotno=%d size=%d\n";
const char str3[] = "Error reading from flash\n";
const char str4[] = "Flash checksum passed:\n";
const char str5[] = "\tloaded:     XOR %x %x ";
const char str6[] = "SUM %x %x\n";
const char str7[] = "\tcalculated: XOR %x %x ";
const char str8[] = "SUM %x %x\n";
const char str9[] = "Flash checksum failed:\n";
const char str10[] = "\tloaded:     XOR %x %x ";
const char str11[] = "SUM %x %x\n";
const char str12[] = "\tcalculated: XOR %x %x ";
const char str13[] = "SUM %x %x\n";
const char str14[] = "Error in loading game number:%d\n";
const char str15[] = "flashSaveGame: slotno %d\n";
const char str16[] = "Error erasing flash sector\n";
const char str17[] = "Error writing to flash write buffer\n";
const char str18[] = "Error writing to flash\n";
const char str19[] = "Flash saved worked: %d\n";
const char str20[] = "Checksum: XOR %x %x, ";
const char str21[] = "SUM %x %x\n";
const char str22[] = "\n";
const char str23[] = "Flash checksum was zero!, it is possible (about a million to one), but it's more likely that your flash is bust\n";

static OSMesg sFlashDmaMqBuffer[1];
static OSMesgQueue sFlashDmaMq;
static OSIoMesg sFlashIoMesg;
static FlashId *sFlashId;
static OSIoMesg sFlashReadIoMesg;
static OSMesgQueue sFlashReadIdQueue;
static OSPiHandle sFlashEPiHandle;
static OSMesg sFlashReadIdQueueBuffer[1];
static u32 DAT_81084030;

static void flash_compute_checksum(FlashStruct *param);
static OSPiHandle *osFlashInit();
static void osFlashReadId(u32 *flash_type, u32 *flash_maker);
/*static*/ s32 osFlashSectorErase(u32 page_num);
/*static*/ s32 osFlashWriteBuffer(OSIoMesg *mb, s32 priority, void *dramAddr, OSMesgQueue *mq);
/*static*/ s32 osFlashWriteArray(u32 page_num);
static s32 osFlashReadArray(OSIoMesg *mb, s32 priority, u32 page_num, void *dramAddr, 
    u32 n_pages, OSMesgQueue *mq);

/*export*/ void flash_ctor(DLLFile *self) {
    osFlashInit();
    osCreateMesgQueue(&sFlashDmaMq, &sFlashDmaMqBuffer[0], 1);
}

/*export*/ void flash_dtor(DLLFile *self)  {

}

/*export*/ s32 flash_load_game(FlashStruct *param1, u8 param2, s32 param3, s32 param4) {
    s16 i;
    s16 result;
    u64 xor;
    u64 sum;

    for (i = 0; i < 3; i++) {
        osInvalDCache((void*)param1, param3);

        result = osFlashReadArray(&sFlashIoMesg, OS_MESG_PRI_NORMAL, 
            param2 * 128, (void*)param1, param3 / 128, &sFlashDmaMq);
        
        if (result == -1) {
            return FALSE;
        }

        osRecvMesg(&sFlashDmaMq, NULL, OS_MESG_BLOCK);

        if (param4 != 0) {
            xor = param1->xor;
            sum = param1->sum;
    
            flash_compute_checksum(param1);
    
            if (xor == param1->xor && sum == param1->sum) {
                return TRUE;
            }
        } else {
            return TRUE;
        }
    }

    return FALSE;
}

#if 1
s16 flash_save_game(FlashStruct *param1, u8 param2, s32 param3, s32 param4);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/31_flash/flash_save_game.s")
#else
/*export*/ s16 flash_save_game(FlashStruct *param1, u8 param2, s32 param3, s32 param4) {
    u32 *buf;
    s16 i;
    s16 page;
    s16 status;
    s16 ret;

    ret = 1;
    buf = ((u32*)param1);

    if (param4 != 0) {
        flash_compute_checksum(param1);
    }

    for (i = 0; i < 3; i++) {
        if (osFlashSectorErase(param2 * 128) == -1) {
            ret = 0;
            break;
        }

        for (page = 0; page < (param3 / 128); page++) {
            osWritebackDCache(&buf[page * 32], 128);

            if ((s16)osFlashWriteBuffer(&sFlashIoMesg, OS_MESG_PRI_NORMAL, 
                    &buf[page * 32], &sFlashDmaMq) == -1) {
                ret = 0;
                break;
            }

            osRecvMesg(&sFlashDmaMq, NULL, OS_MESG_BLOCK);

            if ((s16)osFlashWriteArray(param2 * 128 + page) == -1) {
                ret = 0;
                break;
            }
        }
    }

    return ret;
}
#endif

static void flash_compute_checksum(FlashStruct *param) {
    u16 i;
    u64 *valPtr;

    param->xor = 0;
    param->sum = 0;

    for (i = 0; i < (0x5EAu / 2); i++) {
        valPtr = &((u64*)param)[i];
        
        param->xor ^= *valPtr;
        param->sum += *valPtr;
    }

    param->xor ^= param->sum + 3;
    param->sum = param->xor;

    if (param->xor == 0 && param->sum == 0) {
        
    }
}

static OSPiHandle *osFlashInit() {
    u32 flashType;
    u32 flashMaker;

    osCreateMesgQueue(&sFlashReadIdQueue, &sFlashReadIdQueueBuffer[0], 1);

    sFlashId = (FlashId*)mmAlloc(sizeof(FlashId), COLOUR_TAG_YELLOW, NULL);

    if (sFlashEPiHandle.baseAddress == 0xa8000000) {
        return &sFlashEPiHandle;
    }

    sFlashEPiHandle.type = 8;
    sFlashEPiHandle.baseAddress = 0xa8000000;
    sFlashEPiHandle.latency = 5;
    sFlashEPiHandle.pulse = 12;
    sFlashEPiHandle.pageSize = 15;
    sFlashEPiHandle.relDuration = 2;
    sFlashEPiHandle.domain = PI_DOMAIN2;
    sFlashEPiHandle.speed = 0;
    bzero(&sFlashEPiHandle.transferInfo, sizeof(__OSTranxInfo));

    osEPiLinkHandle(&sFlashEPiHandle);

    osFlashReadId(&flashType, &flashMaker);

    if (flashMaker == 0xc2001e || flashMaker == 0xc20001 || flashMaker == 0xc20000) {
        DAT_81084030 = 0x40;
    } else {
        DAT_81084030 = 0x80;
    }

    return &sFlashEPiHandle;
}

static void flash_func_638() { }

static void osFlashReadStatus(u8 *flash_status) {
    u32 data;

    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress | 0x10000, 0xd2000000);
    osEPiReadIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress, &data);

    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress | 0x10000, 0xd2000000);
    osEPiReadIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress, &data);

    *flash_status = data & 0xFF;
}

static void osFlashReadId(u32 *flash_type, u32 *flash_maker) {
    u8 flash_status;

    osFlashReadStatus(&flash_status);

    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress | 0x10000, 0xe1000000);

    sFlashReadIoMesg.hdr.pri = OS_MESG_PRI_NORMAL;
    sFlashReadIoMesg.hdr.retQueue = &sFlashReadIdQueue;
    sFlashReadIoMesg.dramAddr = sFlashId;
    sFlashReadIoMesg.devAddr = 0;
    sFlashReadIoMesg.size = 8;

    osWritebackDCache(sFlashId, sizeof(FlashId));

    osEPiStartDma(&sFlashEPiHandle, &sFlashReadIoMesg, OS_READ);
    osRecvMesg(&sFlashReadIdQueue, NULL, OS_MESG_BLOCK);

    *flash_type = sFlashId->flashType;
    *flash_maker = sFlashId->flashMaker;
}

static void osFlashClearStatus() {
    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress | 0x10000, 0xd2000000);
    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress, 0);
}

/*static*/ s32 osFlashSectorErase(u32 page_num) {
    u32 data;

    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress | 0x10000, page_num | 0x4b000000);
    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress | 0x10000, 0x78000000);

    do {
        osEPiReadIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress, &data);
    } while ((data & 2) == 2);

    osEPiReadIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress, &data);

    osFlashClearStatus();

    if ((data & 0xff) == 8 || (data & 0xff) == 0x48 || (data & 0x8) == 8) {
        return 0;
    } else {
        return -1;
    }
}

/*static*/ s32 osFlashWriteBuffer(OSIoMesg *mb, s32 priority, void *dramAddr, OSMesgQueue *mq) {
    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress | 0x10000, 0xb4000000);

    mb->hdr.pri = priority;
    mb->hdr.retQueue = mq;
    mb->dramAddr = dramAddr;
    mb->devAddr = 0;
    mb->size = 0x80;

    return osEPiStartDma(&sFlashEPiHandle, mb, OS_WRITE);
}

/*static*/ s32 osFlashWriteArray(u32 page_num) {
    u32 data;

    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress | 0x10000, page_num | 0xa5000000);

    do {
        osEPiReadIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress, &data);
    } while ((data & 1) == 1);

    osEPiReadIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress, &data);

    osFlashClearStatus();

    if ((data & 0xff) == 4 || (data & 0xff) == 0x44 || (data & 0x4) == 4) {
        return 0;
    } else {
        return -1;
    }
}

static s32 osFlashReadArray(OSIoMesg *mb, s32 priority, u32 page_num, void *dramAddr, 
        u32 n_pages, OSMesgQueue *mq) {
    u32 unused;
    u32 data;

    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress | 0x10000, 0xf0000000);
    osEPiReadIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress, &data);

    mb->hdr.pri = priority;
    mb->hdr.retQueue = mq;
    mb->dramAddr = dramAddr;
    mb->devAddr = page_num * DAT_81084030;
    mb->size = n_pages * 128;

    return osEPiStartDma(&sFlashEPiHandle, mb, OS_READ);
}
