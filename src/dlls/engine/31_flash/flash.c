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

/*0x0*/ static const char str_0[] = "Savegame size in pages: %d %d\n";
/*0x20*/ static const char str_20[] = "flashLoadGame: dest=%x slotno=%d size=%d\n";
/*0x4C*/ static const char str_4C[] = "Error reading from flash\n";
/*0x68*/ static const char str_68[] = "Flash checksum passed:\n";
/*0x80*/ static const char str_80[] = "\tloaded:     XOR %x %x ";
/*0x98*/ static const char str_98[] = "SUM %x %x\n";
/*0xA4*/ static const char str_A4[] = "\tcalculated: XOR %x %x ";
/*0xBC*/ static const char str_BC[] = "SUM %x %x\n";
/*0xC8*/ static const char str_C8[] = "Flash checksum failed:\n";
/*0xE0*/ static const char str_E0[] = "\tloaded:     XOR %x %x ";
/*0xF8*/ static const char str_F8[] = "SUM %x %x\n";
/*0x104*/ static const char str_104[] = "\tcalculated: XOR %x %x ";
/*0x11C*/ static const char str_11C[] = "SUM %x %x\n";
/*0x128*/ static const char str_128[] = "Error in loading game number:%d\n";
/*0x14C*/ static const char str_14C[] = "flashSaveGame: slotno %d\n";
/*0x168*/ static const char str_168[] = "Error erasing flash sector\n";
/*0x184*/ static const char str_184[] = "Error writing to flash write buffer\n";
/*0x1AC*/ static const char str_1AC[] = "Error writing to flash\n";
/*0x1C4*/ static const char str_1C4[] = "Flash saved worked: %d\n";
/*0x1DC*/ static const char str_1DC[] = "Checksum: XOR %x %x, ";
/*0x1F4*/ static const char str_1F4[] = "SUM %x %x\n";
/*0x200*/ static const char str_200[] = "\n";
/*0x204*/ static const char str_204[] = "Flash checksum was zero!, it is possible (about a million to one), but it's more likely that your flash is bust\n";
/*0x278*/ static const char str_278[] = "";
/*0x27C*/ static const char str_27C[] = "";

static OSMesg sFlashDmaMqBuffer[1];
static OSMesgQueue sFlashDmaMq;
static OSIoMesg sFlashIoMesg;
static FlashId *sFlashId;
static OSIoMesg sFlashReadIoMesg;
static OSMesgQueue sFlashReadIdQueue;
static OSPiHandle sFlashEPiHandle;
static OSMesg sFlashReadIdQueueBuffer[1];
static u32 sFlashVersion;

static void flash_compute_checksum(FlashStruct *param);
static OSPiHandle *osFlashInit();
static void osFlashReadId(u32 *flash_type, u32 *flash_maker);
static s32 osFlashSectorErase(u32 page_num);
static s32 osFlashWriteBuffer(OSIoMesg *mb, s32 priority, void *dramAddr, OSMesgQueue *mq);
static s32 osFlashWriteArray(u32 page_num);
static s32 osFlashReadArray(OSIoMesg *mb, s32 priority, u32 page_num, void *dramAddr, 
    u32 n_pages, OSMesgQueue *mq);

// offset: 0x0 | ctor
void flash_ctor(void *dll) {
    osFlashInit();
    osCreateMesgQueue(&sFlashDmaMq, &sFlashDmaMqBuffer[0], 1);
}

// offset: 0x60 | dtor
void flash_dtor(void *dll)  {
}

// offset: 0x6C | func: 0 | export: 0
s16 flash_load_game(void *dest, u8 slotno, s32 size, s32 validateChecksum) {
    s16 i;
    s16 result;
    u64 xor;
    u64 sum;

    for (i = 0; i < 3; i++) {
        osInvalDCache(dest, size);

        result = osFlashReadArray(&sFlashIoMesg, OS_MESG_PRI_NORMAL, 
            slotno * 128, dest, size / 128, &sFlashDmaMq);
        
        if (result == -1) {
            return FALSE;
        }

        osRecvMesg(&sFlashDmaMq, NULL, OS_MESG_BLOCK);

        if (validateChecksum) {
            FlashStruct *flashStruct = (FlashStruct*)dest;
            xor = flashStruct->xor;
            sum = flashStruct->sum;
    
            flash_compute_checksum(flashStruct);
    
            if (xor == flashStruct->xor && sum == flashStruct->sum) {
                return TRUE;
            }
        } else {
            return TRUE;
        }
    }

    return FALSE;
}

// offset: 0x224 | func: 1 | export: 1
s16 flash_save_game(void *src, u8 slotno, s32 size, s32 computeChecksum) {
    s16 i;
    s16 page;
    s16 pad;
    u8 *data;
    s16 pad3;
    s16 ret;

    ret = 1;

    if (computeChecksum) {
        flash_compute_checksum((FlashStruct*)src);
    }

    data = src;

    for (i = 0; i < 3; i++) {
        if (osFlashSectorErase(slotno * 128) == -1) {
            ret = 0;
            break;
        }

        for (page = 0; page < (size / 128); page++) {
            osWritebackDCache(data + (page * 128), 128);

            if ((s16)osFlashWriteBuffer(&sFlashIoMesg, OS_MESG_PRI_NORMAL, data + (page * 128), &sFlashDmaMq) == -1) {
                ret = 0;
                break;
            }

            osRecvMesg(&sFlashDmaMq, NULL, OS_MESG_BLOCK);

            if ((s16)osFlashWriteArray(slotno * 128 + page) == -1) {
                ret = 0;
                break;
            }
        }
    }
    
    if (computeChecksum){}

    return ret;
}

// offset: 0x404 | func: 2
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

// offset: 0x4DC | func: 3
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
        sFlashVersion = 0x40;
    } else {
        sFlashVersion = 0x80;
    }

    return &sFlashEPiHandle;
}

// offset: 0x638 | func: 4
static void flash_func_638() { }

// offset: 0x640 | func: 5
static void osFlashReadStatus(u8 *flash_status) {
    u32 data;

    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress | 0x10000, 0xd2000000);
    osEPiReadIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress, &data);

    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress | 0x10000, 0xd2000000);
    osEPiReadIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress, &data);

    *flash_status = data & 0xFF;
}

// offset: 0x70C | func: 6
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

// offset: 0x820 | func: 7
static void osFlashClearStatus() {
    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress | 0x10000, 0xd2000000);
    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress, 0);
}

// offset: 0x88C | func: 8
static s32 osFlashSectorErase(u32 page_num) {
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

// offset: 0x9A4 | func: 9
static s32 osFlashWriteBuffer(OSIoMesg *mb, s32 priority, void *dramAddr, OSMesgQueue *mq) {
    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress | 0x10000, 0xb4000000);

    mb->hdr.pri = priority;
    mb->hdr.retQueue = mq;
    mb->dramAddr = dramAddr;
    mb->devAddr = 0;
    mb->size = 0x80;

    return osEPiStartDma(&sFlashEPiHandle, mb, OS_WRITE);
}

// offset: 0xA50 | func: 10
static s32 osFlashWriteArray(u32 page_num) {
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

// offset: 0xB48 | func: 11
static s32 osFlashReadArray(OSIoMesg *mb, s32 priority, u32 page_num, void *dramAddr, 
        u32 n_pages, OSMesgQueue *mq) {
    u32 unused;
    u32 data;

    osEPiWriteIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress | 0x10000, 0xf0000000);
    osEPiReadIo(&sFlashEPiHandle, sFlashEPiHandle.baseAddress, &data);

    mb->hdr.pri = priority;
    mb->hdr.retQueue = mq;
    mb->dramAddr = dramAddr;
    mb->devAddr = page_num * sFlashVersion;
    mb->size = n_pages * 128;

    return osEPiStartDma(&sFlashEPiHandle, mb, OS_READ);
}
