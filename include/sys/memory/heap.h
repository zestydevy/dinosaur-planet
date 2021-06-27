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
