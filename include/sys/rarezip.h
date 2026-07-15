#ifndef _SYS_RAREZIP_H
#define _SYS_RAREZIP_H

#include "PR/ultratypes.h"

void rarezipInit(void);
s32 rarezipUncompressSize(u8 *b);
s32 rarezipUncompressSizeROM(s32 fileId, s32 offset, s32 directLoad);
u8 *rarezipUncompress(u8 *compressedInput, u8 *decompressedOutput, s32 outputSize);

#endif
