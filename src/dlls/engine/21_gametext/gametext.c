#include "PR/ultratypes.h"
#include "libc/string.h"
#include "sys/fs.h"
#include "sys/memory.h"
#include "sys/asset_thread.h"
#include "dlls/engine/21_gametext.h"
#include "variables.h"
#include "functions.h"
#include "prevent_bss_reordering.h"

static s8 sCurrentBankIndex = -1;

/*0x0*/  static u16 sBankCount;
/*0x2*/  static u16 sBankEntryCount;
/*0x4*/  static u16 sBankSize;
/*0x8*/  static u8 *sCurrentBank;
/*0xc*/  static s32 sCurrentBank_GlobalOffset;
// Number of strings in each chunk.
/*0x10*/ static u8 *sCurrentBank_StrCounts;
// Byte size of each chunk.
/*0x14*/ static u16 *sCurrentBank_Sizes;
// Byte offset to each chunk.
/*0x18*/ static u16 *sCurrentBank_Offsets;

void gametext_set_bank(s8 bank);

void gametext_ctor(void *self) {
    u16 *header;

    header = (u16*)malloc(4, 0x19, NULL);
    queue_load_file_region_to_ptr((void**)header, GAMETEXT_TAB, 0, 4);

    sBankCount = header[0];
    sBankEntryCount = header[1];

    free(header);

    sBankSize = sBankEntryCount * 5 + 4;
    sCurrentBank = malloc(sBankSize, 0x19, NULL);
    sCurrentBank_StrCounts = sCurrentBank + 4;
    sCurrentBank_Sizes = (u16*)(sCurrentBank + sBankEntryCount + 4);
    sCurrentBank_Offsets = (u16*)(sCurrentBank + (sBankEntryCount * 3) + 4);

    gametext_set_bank(0);
}

void gametext_dtor(void *self) {
    free(sCurrentBank);
}

u16 gametext_bank_count() {
    return sBankCount;
}

u16 gametext_chunk_count() {
    return sBankEntryCount;
}

void gametext_set_bank(s8 bank) {
    u32 offset;

    offset = (sBankSize * bank) + 4; 

    if (sCurrentBankIndex == bank) {
        return;
    }

    sCurrentBankIndex = bank;

    queue_load_file_region_to_ptr(
        (void**)sCurrentBank, 
        GAMETEXT_TAB, 
        offset, 
        sBankSize);
    
    sCurrentBank_GlobalOffset = *((u32*)sCurrentBank);
}

s8 gametext_curr_bank() {
    return sCurrentBankIndex;
}

GameTextChunk *gametext_get_chunk(u16 chunk) {
    GameTextChunk *chunkPtr;
    s32 headerSize;
    s32 alignmentPad;
    s32 i;
    s32 k;

    // mem map
    // 0x0  GameTextChunk
    // 0xC  string pointer array
    //      alignment padding
    //      string chunk

    // Size for struct and string pointers
    headerSize = sCurrentBank_StrCounts[chunk] * 4 + sizeof(GameTextChunk);
    // Room for alignment
    alignmentPad = headerSize % 8;

    headerSize += alignmentPad + sCurrentBank_Sizes[chunk];

    chunkPtr = (GameTextChunk*)malloc(headerSize, 0x19, NULL);

    chunkPtr->strings = (char**)((u32)chunkPtr + sizeof(GameTextChunk));

    chunkPtr->count = sCurrentBank_StrCounts[chunk];

    // Where to load actual chunk contents to (also contains data for some chunks)
    chunkPtr->unk4 = (void*)((u32)chunkPtr->strings + sCurrentBank_StrCounts[chunk] * 4 + alignmentPad);
    
    // Set up first string pointer
    chunkPtr->strings[0] = (char*)((u32)chunkPtr->unk4 + sCurrentBank_StrCounts[chunk] * 2);

    queue_load_file_region_to_ptr(
        (void**)chunkPtr->unk4,
        GAMETEXT_BIN,
        sCurrentBank_Offsets[chunk] * 2 + sCurrentBank_GlobalOffset,
        sCurrentBank_Sizes[chunk]);
    
    // Set up remaining pointers to each string
    for (i = 1; i < sCurrentBank_StrCounts[chunk]; i++) {
        k = 0;

        while (chunkPtr->strings[i - 1][k++] != '\0') {}

        chunkPtr->strings[i] = chunkPtr->strings[i - 1] + k;
    }

    return chunkPtr;
}

char *gametext_get_text(u16 chunk, u16 strIndex) {
    u8 *text;
    char *str;
    char *copy;
    s32 len;
    s32 i;
    s32 k;

    text = malloc(sCurrentBank_Sizes[chunk], 0x19, NULL);

    queue_load_file_region_to_ptr(
        (void**)text, 
        GAMETEXT_BIN, 
        sCurrentBank_Offsets[chunk] * 2, 
        sCurrentBank_Sizes[chunk]);

    str = (char*)(text + sCurrentBank_StrCounts[chunk] * 2);

    for (i = 0; i != strIndex; i++) {
        k = 0;

        while (str[k++] != '\0') {}

        str += k;
    }

    len = strlen(str) + 1;

    copy = malloc(len, 0x19, NULL);
    bcopy(str, copy, len);

    free(text);

    return copy;
}
