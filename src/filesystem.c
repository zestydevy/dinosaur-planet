#include "common.h"

extern Fs * gFST;
extern u32 gLastFSTIndex;
extern s32 __fstAddress;
extern s32 __file1Address;

void possible_romcopy(u32 romAddr, u8* dst, s32 size);


extern void * romcopy_dat;
extern OSMesgQueue D_800B2EA8;
extern OSMesg D_800B2E68;
extern OSMesg D_800B2E48;
extern OSIoMesg romcopy_OIMesg;
extern OSMesgQueue romcopy_mesgq;

void init_filesystem(void)
{
    s32 size;

    osCreateMesgQueue(&D_800B2EA8, &D_800B2E68, 0x10);
    osCreateMesgQueue(&romcopy_mesgq, &D_800B2E48, 1);

    osCreatePiManager(0x96, &D_800B2EA8, &D_800B2E68, 0x10);

    // A4AA0 - A4970
    size = (s32)&__file1Address - (s32)&__fstAddress;

    gFST = (Fs *)malloc(size, 0x7F7F7FFF, NULL);
    possible_romcopy(&__fstAddress, (u8 *)gFST, size);
}

void * read_alloc_file(u32 id, u32 a1)
{
    s32 * fstEntry;
    void * data;
    s32 size;
    u32 offset;

    if (id > gFST->fileCount)
        return NULL;

    ++id;

    fstEntry = id + gFST->offsets - 1;
    offset = fstEntry[0];
    size = fstEntry[1] - offset;

    data = malloc(size, ALLOC_TAG_FS_COL, NULL);
    if (data == NULL)
        return NULL;

    possible_romcopy((u32)&__file1Address + offset, data, size);

    return data;
}

s32 read_file(u32 id, void * dest)
{
    s32 * fstEntry;
    s32 size;
    u32 offset;

    if (id > gFST->fileCount)
        return NULL;

    ++id;

    fstEntry = id + gFST->offsets - 1;
    offset = fstEntry[0];
    size = fstEntry[1] - offset;

    possible_romcopy((u32)&__file1Address + offset, dest, size);

    return size;
}

s32 read_file_region(u32 id, void * dst, u32 offset, s32 size)
{
  s32 fileAddr;
  s32 * tmp;

  if (size == 0 || id > gFST->fileCount)
    return 0;

  tmp      = ++id + gFST->offsets - 1;
  fileAddr = *tmp + offset;

  gLastFSTIndex = id;

  possible_romcopy(fileAddr + (s32)&__file1Address, dst, size);

  return size;
}

s32 func_800372C8(u32 id, s32 offset)
{
    s32 * fstEntry;
    s32 offs;

    if (gFST->fileCount < id) {
        return 0;
    }
    
    ++id;

    fstEntry = id + gFST->offsets - 1;
    offs = *fstEntry + offset;

    return (offs + (u32)&__file1Address);
}


s32 get_file_size(u32 id)
{
    s32 * fstEntry;
    s32 size;

    if (id > gFST->fileCount)
        return NULL;

    ++id;

    fstEntry = id + gFST->offsets - 1;
    size = fstEntry[1] - fstEntry[0];

    return size;
}

void possible_romcopy(u32 romAddr, u8* dst, s32 size)
{
    OSMesg mesg;
    OSMesg mesg2;
    s32 chunkSize;

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
