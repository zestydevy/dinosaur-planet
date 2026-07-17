#include "PR/os.h"
#include "sys/pi.h"
#include "sys/memory.h"
#include "macros.h"

extern u8 __fstAddress[];
extern u8 __file1Address[];

typedef struct { 
    u32 fileCount;
    u32 offsets[NUM_FILES];
} Fs;

void romCopy(u32 romAddr, u8* dst, s32 size);

// -------- .bss start 800b2e30 -------- //
OSIoMesg romcopy_OIMesg;
OSMesg D_800B2E48[1];
OSMesgQueue romcopy_mesgq;
OSMesg D_800B2E68[16];
OSMesgQueue D_800B2EA8;
Fs *gFST;
u32 gLastFSTIndex;
// -------- .bss end 800b2ed0 -------- //

// official name: piInit
void piInit(void) {
    s32 size;

    osCreateMesgQueue(&D_800B2EA8, D_800B2E68, ARRAYCOUNT(D_800B2E68));
    osCreateMesgQueue(&romcopy_mesgq, D_800B2E48, ARRAYCOUNT(D_800B2E48));

    osCreatePiManager(OS_PRIORITY_PIMGR, &D_800B2EA8, D_800B2E68, ARRAYCOUNT(D_800B2E68));

    // A4AA0 - A4970
    size = __file1Address - __fstAddress;

    gFST = (Fs *)mmAlloc(size, COLOUR_TAG_GREY, NULL);
    romCopy((u32)__fstAddress, (u8 *)gFST, size);
}

// official name: piRomLoad
void *piRomLoad(u32 id, u32 a1) {
    u32 * fstEntry;
    void * data;
    s32 size;
    u32 offset;

    if (id > gFST->fileCount)
        return NULL;

    ++id;

    fstEntry = id + gFST->offsets - 1;
    offset = fstEntry[0];
    size = fstEntry[1] - offset;

    data = mmAlloc(size, COLOUR_TAG_GREY, NULL);
    if (data == NULL) {
        return NULL;
    }

    romCopy((u32)__file1Address + offset, data, size);

    return data;
}

s32 piRomLoadToDest(u32 id, void *dest) {
    u32 * fstEntry;
    s32 size;
    u32 offset;

    if (id > gFST->fileCount) {
        return NULL;
    }

    ++id;

    fstEntry = id + gFST->offsets - 1;
    offset = fstEntry[0];
    size = fstEntry[1] - offset;

    romCopy((u32)__file1Address + offset, dest, size);

    return size;
}

// official name: piRomLoadSection
s32 piRomLoadSection(u32 id, void *dst, u32 offset, s32 size) {
  s32 fileAddr;
  u32 * tmp;

  if (size == 0 || id > gFST->fileCount)
    return 0;

  tmp      = ++id + gFST->offsets - 1;
  fileAddr = *tmp + offset;

  gLastFSTIndex = id;

  romCopy(fileAddr + (u32)__file1Address, dst, size);

  return size;
}

// official name: piRomGetSectionPtr
u32 piRomGetSectionPtr(u32 id, s32 offset) {
    u32 * fstEntry;
    s32 offs;

    if (gFST->fileCount < id) {
        return 0;
    }
    
    ++id;

    fstEntry = id + gFST->offsets - 1;
    offs = *fstEntry + offset;

    return (offs + (u32)__file1Address);
}

// official name: piRomGetFileSize
s32 piRomGetFileSize(u32 id) {
    u32 * fstEntry;
    s32 size;

    if (id > gFST->fileCount) {
        return NULL;
    }

    ++id;

    fstEntry = id + gFST->offsets - 1;
    size = fstEntry[1] - fstEntry[0];

    return size;
}

// official name: romCopy
void romCopy(u32 romAddr, u8* dst, s32 size) {
    OSMesg mesg;
    OSMesg mesg2;
    s32 chunkSize;

    osInvalDCache(dst, size);

    chunkSize = 0x5000;
    while (size > 0) {
        if (size < chunkSize) {
            chunkSize = size;
        }

        osPiStartDma(&romcopy_OIMesg, OS_MESG_PRI_NORMAL, OS_READ, romAddr, dst, chunkSize, &romcopy_mesgq);
        osRecvMesg(&romcopy_mesgq, &mesg, OS_MESG_BLOCK);

        size -= chunkSize;
        romAddr += chunkSize;
        dst += chunkSize;
    }
}
