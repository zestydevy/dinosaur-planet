#define EXPANSION_RAM_START 0x80400000
#define EXPANSION_SIZE 0x00800000

#define HEAP_AREA_00 0x8042C000
#define HEAP_AREA_01 0x80245000

//Allocations are given a color tag.
//names extracted from default.dol
enum AllocTag {
    ALLOC_TAG_0,
    ALLOC_TAG_LISTS_COL,
    ALLOC_TAG_SCREEN_COL,
    ALLOC_TAG_CODE_COL,
    ALLOC_TAG_DLL_COL,
    ALLOC_TAG_TRACK_COL,
    ALLOC_TAG_TEX_COL,
    ALLOC_TAG_TRACKTEX_COL,
    ALLOC_TAG_SPRITETEX_COL,
    ALLOC_TAG_MODELS_COL,
    ALLOC_TAG_ANIMS_COL,
    ALLOC_TAG_AUDIO_COL,
    ALLOC_TAG_SEQ_COL,
    ALLOC_TAG_SFX_COL,
    ALLOC_TAG_OBJECTS_COL,
    ALLOC_TAG_CAM_COL,
    ALLOC_TAG_VOX_COL,
    ALLOC_TAG_ANIMSEQ_COL,
    ALLOC_TAG_LFX_COL,
    ALLOC_TAG_GFX_COL,
    ALLOC_TAG_EXPGFX_COL,
    ALLOC_TAG_MODGFX_COL,
    ALLOC_TAG_PROJGFX_COL,
    ALLOC_TAG_SKY_COL,
    ALLOC_TAG_SHAD_COL,
    ALLOC_TAG_GAME_COL,
    ALLOC_TAG_TEST_COL,
    ALLOC_TAG_BLACK,
    ALLOC_TAG_RED,
    ALLOC_TAG_GREEN,
    ALLOC_TAG_BLUE,
    ALLOC_TAG_CYAN,
    ALLOC_TAG_MAGENTA,
    ALLOC_TAG_YELLOW,
    ALLOC_TAG_WHITE,
    ALLOC_TAG_GREY,
    ALLOC_TAG_ORANGE
};

struct HeapBlock {
    s32 maxItems;
    s32 itemCount;
    struct HeapBlock *ptr;
    s32 memAllocated;
    // s16 index;
    s32 memUsed;
};

extern u32 * bss_end;
extern u8 gHeapBlkListSize;
extern struct HeapBlock gHeapBlkList[];
extern s32 memMonVal0;
extern s32 memMonVal1;
extern s32 memMonVal2;

void init_memory(void);
void free(void*);
void *malloc(s32 size, s32 tag, const char *name);
