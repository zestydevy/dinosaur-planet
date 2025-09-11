/** Memory management
 */
#ifndef _SYS_MEMORY_H
#define _SYS_MEMORY_H

#include "PR/ultratypes.h"

#define EXPANSION_RAM_START 0x80400000
#define EXPANSION_SIZE 0x00800000
#define RAM_END (0x80000000 + EXPANSION_SIZE)

#define MEM_POOL_AREA_00 0x8042C000
#define MEM_POOL_AREA_01 0x80245000
#define MEM_POOL_AREA_02 0x80119000
#define MEM_POOL_AREA_NO_EXPANSION 0x802D4000

typedef enum MempoolFlags {
    SLOT_FREE = 0,             // The slot is free.
    SLOT_USED = (1 << 0),      // The slot is used.
    SLOT_LOCKED = (1 << 1),    // The slot is used, and cannot be freed by normal means. (unused in dino planet)
    SLOT_SAFEGUARD = (1 << 2)  // The slot is used, and marks the stopping point of a global pool clear.
} MempoolFlags;

#define MEMSLOT_NONE -1

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

// Color tag defines below are taken from DKR, might not be 100% accurate

// Model data. Mesh, collision, animation.
#define COLOUR_TAG_RED 0xFF0000FF
// Model headers
#define COLOUR_TAG_GREEN 0x00FF00FF
// Objects
#define COLOUR_TAG_BLUE 0x0000FFFF
// Tracks, Fonts, GPlay, Flash
#define COLOUR_TAG_YELLOW 0xFFFF00FF
// Textures(?)
#define COLOUR_TAG_MAGENTA 0xFF00FFFF
#define COLOUR_TAG_LIME 0x00FF0163
// Audio, DLDebug(?)
#define COLOUR_TAG_CYAN 0x00FFFFFF
// Buffers and heaps
#define COLOUR_TAG_WHITE 0xFFFFFFFF
// Assets
#define COLOUR_TAG_GREY 0x7F7F7FFF
// Particles(?)
#define COLOUR_TAG_SEMITRANS_GREY 0x80808080
// Model normals(?)
#define COLOUR_TAG_ORANGE 0xFF7F7FFF
// RareZip (Controller Pak in DKR)
#define COLOUR_TAG_BLACK 0x000000FF
// Weather(?)
#define COLOUR_TAG_LIGHT_ORANGE 0xFFAA55FF

typedef struct {
/*0000*/    u8 *data;
/*0004*/    s32 size;
/*0008*/    s16 flags;
/*000A*/    s16 prevIndex;
/*000C*/    s16 nextIndex;
/*000E*/    s16 index;
/*0010*/    s32 tag;
} MemoryPoolSlot;

// Doubly linked list of MemoryPoolSlots
typedef struct {
/*0000*/    s32 maxSlots;
/*0004*/    s32 numSlots;
/*0008*/    MemoryPoolSlot *slots;
/*000C*/    s32 memAllocated;
/*0010*/    s32 memUsed;
} MemoryPool;

typedef struct {
    void *address;
    u8 ticksLeft;
} MemoryFreeQueueElement;

extern void *bss_end;

extern u8 gNumMemoryPools;
extern MemoryPool gMemoryPools[];
extern s32 memMonVal0;
extern s32 memMonVal1;
extern s32 memMonVal2;

extern MemoryFreeQueueElement gMemoryFreeQueue[];
extern s16 gMemoryFreeQueueLength;
extern s32 gMemoryFreeDelay;

void mmInit(void);
MemoryPoolSlot *mmInitPool(MemoryPoolSlot *slots, s32 size, s32 maxSlots);
void *mmAlloc(s32 size, s32 tag, const char *name);
void *mmRealloc(void *address, s32 newSize, const char *name);
s32 mmGetSlotSize(void *address);
void *mmAllocR(s32 poolIndex, s32 size, s32 tag, const char *name);
void *mmAllocAtAddr(s32 size, void *address, s32 tag, const char *name);
void mmSetDelay(s32 delay);
void mmFree(void *address);
void mmFreeTick();
void mmFreeNow(void *address);
void mmFreeEnqueue(void *address);
s32 mmFindPool(void *address);
void mmFreeSlot(s32 poolIndex, s32 slotIndex);
MemoryPoolSlot *mmGetSlotPtr(s32 poolIndex, s32 a1);
s32 mmAllocSlot2(s32 poolIndex, s32 slotIndex, s32 size, s32 flags, s32 flags2, s32 tag, const char* name);
s32 mmAllocSlot(s32 poolIndex, s32 slotIndex, s32 size, s32 flags, s32 flags2, s32 tag);
u32 mmAlign16(u32 a);
u32 mmAlign8(u32 a);
u32 mmAlign4(u32 a);
u32 mmAlign2(u32 a);
s32 mmSlotPrint(s32 arg0);

#endif //_SYS_MEMORY_H
