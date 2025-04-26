#ifndef _DLLS_21_H
#define _DLLS_21_H

#include "PR/ultratypes.h"
#include "dll_def.h"

typedef struct {
    // Num strings
    u8 count;
    void *unk4;
    char **strings;
} GameTextChunk;

DLL_INTERFACE_BEGIN(21_gametext)
    /*0*/ u16 (*bank_count)();
    /*1*/ u16 (*chunk_count)();
    /*2*/ void (*set_bank)(s8 bank);
    /*3*/ s8 (*curr_bank)();
    /*4*/ GameTextChunk *(*get_chunk)(u16 chunk);
    /*5*/ char *(*get_text)(u16 chunk, u16 strIndex);
DLL_INTERFACE_END()

#endif //_DLLS_21_H
