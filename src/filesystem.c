#include "common.h"

#define ROM_FILES_START 0xA4AA0

extern u32 *gFST;
extern u32 gLastFSTIndex;

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/filesystem/init_filesystem.s")
#else
extern void * romcopy_dat;
void _init_filesystem(void)
{
    s32 a3;

    osCreateMesgQueue(D_800B2EA8, D_800B2E68, 0x10);
    osCreateMesgQueue(romcopy_mesgq, D_800B2E48, 1);

    osCreatePiManager(0x96, D_800B2EA8, D_800B2E68, 0x10);

    a3 = 0xA4AA0 - 0xA4970;

    romcopy_dat = malloc(a3, 0x7F7F7FFF, NULL);
    possible_romcopy(0xA4970, romcopy_dat, a3);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/filesystem/read_alloc_file.s")
#else
void *_read_alloc_file(u32 id)
{
    u32 *fstEntry;
    u32 size;
    void *data;

    if (id >= gFST[0]) {
        return NULL;
    }

    fstEntry = &gFST[id + 1];
    size = fstEntry[1] - *fstEntry;

    data = malloc(size, 0x7F7F7FFF, 0);
    if (!data) {
        return NULL;
    }

    possible_romcopy(ROM_FILES_START + *fstEntry, data, size);

    return data;
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/filesystem/read_file.s")
#else
s32 _read_file(u32 id, void *dst)
{
    u32 *fstEntry;
    s32 size;

    if (id > gFST[0]) {
        return 0;
    }

    fstEntry = &gFST[id + 1];
    size = fstEntry[1] - *fstEntry;
    possible_romcopy(ROM_FILES_START + *fstEntry, dst, size);

    return size;
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/filesystem/read_file_region.s")
#else
void possible_romcopy(u32 romAddr, u8* dst, s32 size);
s32 _read_file_region(u32 id, void *dst, u32 offset, s32 size)
{
    u32 fileAddr;

    if (size == 0 || id > gFST[0])
    {
        return 0;
    }
    else
    {
        gLastFSTIndex = id + 1;
        fileAddr = ROM_FILES_START + gFST[gLastFSTIndex] + offset;
        possible_romcopy(fileAddr, dst, size);
        return size;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/filesystem/func_800372C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/filesystem/get_file_size.s")

// stack
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/filesystem/possible_romcopy.s")
#else
extern OSIoMesg romcopy_OIMesg;
extern OSMesgQueue romcopy_mesgq;
void _possible_romcopy(u32 romAddr, u8* dst, s32 size)
{
    s32 chunkSize;
    OSMesg mesg;

    osInvalDCache(dst, size);

    chunkSize = 0x5000;
    while (size > 0)
    {
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
#endif
