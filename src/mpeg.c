#include "PR/ultratypes.h"
#include "mp3/mp3.h"
#include "sys/asset_thread.h"
#include "sys/fs.h"
#include "sys/mpeg.h"

s32 *gFile_MPEG_TAB = NULL;

void mpeg_init(ALHeap *heap) {
	queue_alloc_load_file((void**) (&gFile_MPEG_TAB), MPEG_TAB);
	mp3_init(heap);
}

void mpeg_play(s32 id) {
    s32 size;
    s32 address;

    address = file_get_romaddr(MPEG_BIN, gFile_MPEG_TAB[id]);
    size = gFile_MPEG_TAB[id + 1] - gFile_MPEG_TAB[id];
    if ((address != 0) && (size != 0)) {
        mp3_play_file(address, size);
    }
}
