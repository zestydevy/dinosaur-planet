#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include "ultra64.h"

#define HW_REG2(reg, type) *(volatile type *)(u32)(reg | 0xa0000000)
#define ACCESS_1 *(s16 *)(u32)(0x1C000000 | 0xa0000000)
#define ACCESS_2 *(s16 *)(u32)(0x1C000002 | 0xa0000000)
#define EXPANSION_RAM_START 0x80400000
#define EXPANSION_SIZE 0x00800000
#define RAM_END (0x80000000 + EXPANSION_SIZE)

#define HEAP_AREA_00 0x8042C000
#define HEAP_AREA_01 0x80245000
#define HEAP_AREA_02 0x80119000
#define HEAP_AREA_NO_EXPANSION 0x802D4000

#define IDLE_THREAD_SIZE (0x800 / sizeof(u64))
#define IDLE_THREAD_ID 1
#define IDLE_THREAD_PRIORITY OS_PRIORITY_IDLE

#define MAIN_THREAD_SIZE 1024
#define MAIN_THREAD_ID 3
#define MAIN_THREAD_PRIORITY 10

#define PLAYER_POSBUF_SIZE 60

#define DEBUG_LOOP1 do {/* omitted debug code */} while(0)

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

#endif
