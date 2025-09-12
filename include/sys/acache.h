#ifndef _SYS_ACACHE_H
#define _SYS_ACACHE_H

#include "PR/ultratypes.h"

typedef void *(*ACacheCallback)(u16 arg0);

// size: 0x1C
typedef struct {
/*0x00*/ u8 fileID;
/*0x01*/ u8 unk1;
/*0x02*/ u8 unk2;
/*0x03*/ u8 unk3;
/*0x04*/ s8 unk4;
/*0x06*/ s16 unk6;
/*0x08*/ u8 *unk8;
/*0x0C*/ u8 *unkC;
/*0x10*/ s16 *unk10;
/*0x14*/ u8 **unk14;
/*0x18*/ ACacheCallback unk18;
} ACache;

ACache* acache_init(u8 fileID, u8 arg1, u8 arg2, u8 arg3, ACacheCallback arg4);
void acache_free(ACache *cache);
void acache_func_80000D50(ACache *cache);
void *acache_get(ACache *cache, s32 arg1);

#endif //_SYS_ACACHE_H
