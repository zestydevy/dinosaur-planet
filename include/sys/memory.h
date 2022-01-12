/** Memory management
 */
#ifndef _SYS_MEMORY_H
#define _SYS_MEMORY_H

#include "PR/ultratypes.h"
#define EXPANSION_RAM_START 0x80400000
#define EXPANSION_SIZE 0x00800000
#define RAM_END (0x80000000 + EXPANSION_SIZE)

#define HEAP_AREA_00 0x8042C000
#define HEAP_AREA_01 0x80245000
#define HEAP_AREA_02 0x80119000
#define HEAP_AREA_NO_EXPANSION 0x802D4000

//Allocations are given a color tag.
//names extracted from default.dol
enum AllocTag {
    /* 0x00 */ ALLOC_TAG_0,
    /* 0x01 */ ALLOC_TAG_LISTS_COL,
    /* 0x02 */ ALLOC_TAG_SCREEN_COL,
    /* 0x03 */ ALLOC_TAG_CODE_COL,
    /* 0x04 */ ALLOC_TAG_DLL_COL,
    /* 0x05 */ ALLOC_TAG_TRACK_COL,
    /* 0x06 */ ALLOC_TAG_TEX_COL,
    /* 0x07 */ ALLOC_TAG_TRACKTEX_COL,
    /* 0x08 */ ALLOC_TAG_SPRITETEX_COL,
    /* 0x09 */ ALLOC_TAG_MODELS_COL,
    /* 0x0A */ ALLOC_TAG_ANIMS_COL,
    /* 0x0B */ ALLOC_TAG_AUDIO_COL,
    /* 0x0C */ ALLOC_TAG_SEQ_COL,
    /* 0x0D */ ALLOC_TAG_SFX_COL,
    /* 0x0E */ ALLOC_TAG_OBJECTS_COL,
    /* 0x0F */ ALLOC_TAG_CAM_COL,
    /* 0x10 */ ALLOC_TAG_VOX_COL,
    /* 0x11 */ ALLOC_TAG_ANIMSEQ_COL,
    /* 0x12 */ ALLOC_TAG_LFX_COL,
    /* 0x13 */ ALLOC_TAG_GFX_COL,
    /* 0x14 */ ALLOC_TAG_EXPGFX_COL,
    /* 0x15 */ ALLOC_TAG_MODGFX_COL,
    /* 0x16 */ ALLOC_TAG_PROJGFX_COL,
    /* 0x17 */ ALLOC_TAG_SKY_COL,
    /* 0x18 */ ALLOC_TAG_SHAD_COL,
    /* 0x19 */ ALLOC_TAG_GAME_COL,
    /* 0x1A */ ALLOC_TAG_TEST_COL,
    /* 0x1B */ ALLOC_TAG_BLACK,
    /* 0x1C */ ALLOC_TAG_RED,
    /* 0x1D */ ALLOC_TAG_GREEN,
    /* 0x1E */ ALLOC_TAG_BLUE,
    /* 0x1F */ ALLOC_TAG_CYAN,
    /* 0x20 */ ALLOC_TAG_MAGENTA,
    /* 0x21 */ ALLOC_TAG_YELLOW,
    /* 0x22 */ ALLOC_TAG_WHITE,
    /* 0x23 */ ALLOC_TAG_GREY,
    /* 0x24 */ ALLOC_TAG_ORANGE,
    /* 0x25 */ NUM_ALLOC_TAGS
};
//default.dol contains a table mapping each tag to a color.
//The game also sometimes passes the color directly.
//That table is used to look up the color to render it.
//The table and renderer are not in retail SFA, and probably also not this ROM.
//XXX add defines for those colors if this ROM is passing them as tags.

typedef struct {
/*0000*/    u8 * data;
/*0004*/    s32 maxSize;
/*0008*/    s16 flags;
/*000A*/    s16 prevIdx;
/*000C*/    s16 nextIdx;
/*000E*/    s16 index;
/*0010*/    s32 tag;
} HeapBlock;

typedef struct {
/*0000*/    s32 maxBlocks;
/*0004*/    s32 blockCount;
/*0008*/    HeapBlock * blocks;
/*000C*/    s32 memAllocated;
/*0010*/    s32 memUsed;
} Heap;

extern u32 * bss_end;
extern u8 gHeapListSize;
extern Heap gHeapList[];
extern char * D_800991E0;
extern s32 memMonVal0;
extern s32 memMonVal1;
extern s32 memMonVal2;

void init_memory(void);
HeapBlock * set_heap_block(HeapBlock * blocks, s32 size, s32 maxBlocks);
void *malloc(s32 size, s32 tag, const char *name);
void free(void* p);
void _bcopy(const void *src,void *dst,int length);

#endif //_SYS_MEMORY_H
