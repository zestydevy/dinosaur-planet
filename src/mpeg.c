#include "PR/ultratypes.h"
#include "mp3/mp3.h"
#include "sys/asset.h"
#include "sys/pi.h"
#include "sys/mpeg.h"

s32 *gFile_MPEG_TAB = NULL;

void mpegInit(ALHeap *heap) {
	assetRomLoad((void**) (&gFile_MPEG_TAB), MPEG_TAB);
	mp3Init(heap);
}

void mpegPlay(s32 id) {
    s32 size;
    s32 address;

    address = piRomGetSectionPtr(MPEG_BIN, gFile_MPEG_TAB[id]);
    size = gFile_MPEG_TAB[id + 1] - gFile_MPEG_TAB[id];
    if ((address != 0) && (size != 0)) {
        mp3PlayFile(address, size);
    }
}
