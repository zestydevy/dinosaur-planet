#define EXPANSION_RAM_START 0x80400000
#define EXPANSION_SIZE 0x00800000

#define HEAP_AREA_00 0x8042C000
#define HEAP_AREA_01 0x80245000

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
