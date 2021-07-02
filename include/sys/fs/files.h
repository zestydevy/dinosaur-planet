#include "PR/ultratypes.h"

enum FILE_ID {
    FILE_TABLES_BIN   = 0x16,
    FILE_TABLES_TAB   = 0x17,
    FILE_BITTABLE     = 0x37,
    FILE_OBJECTS_TAB  = 0x41,
    FILE_OBJINDEX_BIN = 0x43
};

extern void *gFile_TABLES_BIN;
extern s32  *gFile_TABLES_TAB;
extern s32  *gFile_OBJECTS_TAB;
extern s16  *gFile_OBJINDEX;

int get_file_size(int file);
void queue_alloc_load_file(void **dest, s32 fileId);
void queue_load_file_to_ptr(void **dest, s32 fileId);
void queue_load_file_region_to_ptr(void **dest, s32 arg1, s32 arg2, s32 arg3);
