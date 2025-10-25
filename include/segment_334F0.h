#ifndef _SEGMENT_334F0
#define _SEGMENT_334F0

#include "PR/ultratypes.h"
#include "game/objects/object.h"

// Size: 0x24
typedef struct Unk80032CF8 {
    /* 0x00 */ s8 unk0;
    /* 0x01 */ u8 pad1[0x4 - 0x1];
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 z;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s8 unk1E;
    /* 0x1F */ s8 unk1F;
    /* 0x20 */ s8 unk20;
    /* 0x21 */ s8 unk21;
    /* 0x22 */ s8 unk22;
} Unk80032CF8;

void func_80032A08(Object* obj, Unk80032CF8* arg1);
void func_800328F0(Object* obj, Unk80032CF8* arg1, f32 arg2);
void func_80033B68(Object*, Unk80032CF8*, f32);
void func_80034678(Object*, Unk80032CF8*, f32);
void* func_800348A0(Object*, s32, s32);
void func_800339E0(Object*, s32, s32, f32);
void func_80033AA0(Object*, s32, s32, f32);
s16* func_80034804(Object *obj, s32 sequenceBoneID);
s32 func_80034250(Unk80032CF8*, s16*);
s32 func_80034518(Unk80032CF8*, s16*, f32, f32);
void func_80033C54(Object*, Unk80032CF8*, f32, s16*);
void func_80033FD8(Object*, Unk80032CF8*, f32, s16*);
s32 func_800343B8(Unk80032CF8* arg0, s16* arg1, f32 arg2, f32 arg3);

#endif //_SEGMENT_334F0
