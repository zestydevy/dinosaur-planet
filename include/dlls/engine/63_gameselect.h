#ifndef _DLLS_63_H
#define _DLLS_63_H

#include "PR/ultratypes.h"
#include "dlls/engine/74_picmenu.h"
#include "sys/menu.h"
#include "dll_def.h"

typedef struct {
    /*0*/ u8 unk0;
    /*1*/ u8 character;
    /*2*/ u8 unk2;
    /*3*/ u8 unk3;
    /*4*/ s16 unk4;
    /*6*/ s16 unk6;
    /*8*/ s16 unk8;
    /*A*/ u16 unkA;
    /*C*/ char unkC[6];
} DLL63Struct;

typedef struct {
    PicMenuItem *menuItems;
    s8 *textIDs;
    u8 count;
    u8 unk9;
    u8 unkA;
    u8 unkB;
} DLL63Struct2;

typedef DLLInst_Menu DLLInst_63_gameselect;

#endif //_DLLS_63_H
