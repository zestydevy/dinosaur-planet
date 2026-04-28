#include "PR/os.h"
#include "sys/acache.h"
#include "sys/asset_thread.h"
#include "sys/memory.h"

static const char str_80098210[] = "acacheInit %d,%d,%d %dbytes\n";
static const char str_80098230[] = "\nfetch %d\n";
static const char str_8009823c[] = "in cache %d\n";
static const char str_8009824c[] = "flush\n";
static const char str_80098254[] = "in load %d\n";

ACache* acache_init(u8 fileID, u8 recordsPerLoad, u8 recordSize, u8 slotCount, ACacheCallback loadCallback) {
    u8* slotBuffer;
    ACache* cache;
    s32 layoutOffsets[5];
    s32 slotIndex;

    layoutOffsets[0] = mmAlign16(sizeof(ACache));
    layoutOffsets[1] = layoutOffsets[0] + (recordsPerLoad * recordSize);
    layoutOffsets[2] = layoutOffsets[1] + (slotCount * recordSize);
    layoutOffsets[3] = layoutOffsets[2] + (slotCount * 2);
    slotIndex = layoutOffsets[3] + (slotCount * 4);
    layoutOffsets[4] = slotIndex;
    
    cache = mmAlloc(layoutOffsets[4], COLOUR_TAG_GREY, NULL);
    if (cache != NULL) {
        if ((!cache->slotStorage) && (!cache->slotStorage)) {}
        cache->fileID = fileID;
        cache->recordsPerLoad = recordsPerLoad;
        cache->recordSize = recordSize;
        cache->slotCount = slotCount;
        cache->loadCallback = loadCallback;
        cache->loadedRecords = (u8* ) (layoutOffsets[0] + (u32)cache);
        cache->slotStorage = (u8* ) (layoutOffsets[1] + (u32)cache);
        cache->slotRecordIds = (s16* ) (layoutOffsets[2] + (u32)cache);
        cache->slotBuffers = (u8** ) (layoutOffsets[3] + (u32)cache);
        cache->loadedStartIndex = -recordSize - 1;
        slotBuffer = cache->slotStorage;
        cache->nextSlot = 0;
        for (slotIndex = 0; slotIndex < slotCount; slotIndex++) {
            cache->slotRecordIds[slotIndex] = -1;
            cache->slotBuffers[slotIndex] = slotBuffer;
            slotBuffer += recordSize;
        }
    }
    
    return cache;
}

void acache_free(ACache *cache) {
    mmFree(cache);
}

void acache_flush(ACache *cache) {
    s32 slotIndex;

    cache->loadedStartIndex = -cache->recordSize - 1;
    cache->nextSlot = 0;

    for (slotIndex = 0; slotIndex < cache->slotCount; ++slotIndex) {
       cache->slotRecordIds[slotIndex] = -1;
    }
}

void *acache_get(ACache *cache, s32 recordIndex) {
    u8* slotBuffer;
    s32 slotIndex;
    s32 cacheIndex;
    s32 nextSlot;
    void *loadedRecord;

    slotBuffer = NULL;
    cache->nextSlot--;
    if (cache->nextSlot < 0) {
        cache->nextSlot = cache->slotCount - 1;
    }
    for (cacheIndex = 0; cacheIndex < cache->slotCount; cacheIndex++) {
        if (recordIndex == cache->slotRecordIds[cacheIndex]) {
            slotBuffer = cache->slotBuffers[cacheIndex];
            break;
        }
    }
    if (slotBuffer != NULL) {
        slotIndex = cacheIndex;
        nextSlot = cacheIndex + 1;
        while (slotIndex != cache->nextSlot) {
            if (nextSlot == cache->slotCount) {
                nextSlot = 0;
            }
            cache->slotRecordIds[slotIndex] = cache->slotRecordIds[nextSlot];
            cache->slotBuffers[slotIndex] = cache->slotBuffers[nextSlot];
            slotIndex += 1;
            nextSlot += 1;
            if (slotIndex == cache->slotCount) {
                slotIndex = 0;
            }
        }
    } else {
        slotBuffer = cache->slotBuffers[cache->nextSlot];
        cacheIndex = recordIndex - cache->loadedStartIndex;
        if ((cacheIndex < 0) || (cacheIndex >= cache->recordsPerLoad)) {
            // Requested item is outside of the currently cached region
            cacheIndex = recordIndex - (cache->recordsPerLoad >> 1);
            if (cacheIndex < 0) {
                cacheIndex = 0;
            }
            cache->loadedStartIndex = cacheIndex;
            if (cache->loadCallback != NULL) {
                // Load from callback
                for (slotIndex = 0; slotIndex < (s32) cache->recordsPerLoad; slotIndex += 1) {
                    // Get thing from callback
                    loadedRecord = cache->loadCallback(cacheIndex);
                    // Copy it into cache
                    bcopy(loadedRecord, &cache->loadedRecords[slotIndex * cache->recordSize], cache->recordSize);
                    mmFree(loadedRecord);
                    cacheIndex += 1;
                }
            } else {
                // Load from file
                queue_load_file_region_to_ptr(
                    /*dest*/   (void** ) cache->loadedRecords,
                    /*fileID*/ (s32) cache->fileID, 
                    /*offset*/ cache->recordSize * cacheIndex,
                    /*length*/ cache->recordsPerLoad * cache->recordSize);
            }
            cacheIndex = recordIndex - cache->loadedStartIndex;
        }
        bcopy(&cache->loadedRecords[cacheIndex * cache->recordSize], slotBuffer, cache->recordSize);
    }
    cache->slotRecordIds[cache->nextSlot] = recordIndex;
    cache->slotBuffers[cache->nextSlot] = slotBuffer;
    return slotBuffer;
}
