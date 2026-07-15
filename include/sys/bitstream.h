#ifndef _SYS_BITSTREAM_H
#define _SYS_BITSTREAM_H

#include "PR/ultratypes.h"

typedef struct {
    /*0x0*/  u8 *data;
    /*0x4*/  s32 byteLength;
    /*0x8*/  s32 bitLength;
    /*0xC*/  s32 capacity;
    /*0x10*/ s32 bitPos;
} BitStream;

BitStream *bitstreamInit(BitStream *stream, u8 *data, s32 bitLength, s32 capacity);
u32 bitstreamRead(BitStream *stream, u8 n);
void bitstreamWrite(BitStream *stream, u32 data, u8 n);
void bitstreamAppend(BitStream *stream, u32 data, u8 n);
void bitstreamSetPos(BitStream *stream, s32 bitPos);

#endif
