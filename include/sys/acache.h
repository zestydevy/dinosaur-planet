#ifndef _SYS_ACACHE_H
#define _SYS_ACACHE_H

#include "PR/ultratypes.h"

typedef void *(*ACacheCallback)(u16 recordIndex);

// size: 0x1C
typedef struct {
/*0x00*/ u8 fileID;
/*0x01*/ u8 recordsPerLoad;
/*0x02*/ u8 recordSize;
/*0x03*/ u8 slotCount;
/*0x04*/ s8 nextSlot;
/*0x06*/ s16 loadedStartIndex;
/*0x08*/ u8 *loadedRecords;
/*0x0C*/ u8 *slotStorage;
/*0x10*/ s16 *slotRecordIds;
/*0x14*/ u8 **slotBuffers;
/*0x18*/ ACacheCallback loadCallback;
} ACache;

ACache* acache_init(u8 fileID, u8 recordsPerLoad, u8 recordSize, u8 slotCount, ACacheCallback loadCallback);
void acache_free(ACache *cache);
void acache_flush(ACache *cache);
void *acache_get(ACache *cache, s32 recordIndex);

#endif //_SYS_ACACHE_H
