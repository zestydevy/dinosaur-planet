#include "PR/ultratypes.h"
#include "sys/bitstream.h"
#include "macros.h"

BitStream *bitstream_init(BitStream *stream, u8 *data, s32 bitLength, s32 capacity) {
    stream->byteLength = bitLength >> 3;
    if (bitLength & 7) {
        // need one extra byte to hold bits
        stream->byteLength += 1;
    }

    stream->bitLength = bitLength;
    stream->capacity = capacity;
    stream->data = data;
    stream->bitPos = 0;

    return stream;
}

u32 bitstream_read(BitStream *stream, u8 n) {
    u32 data;
    s32 i;
    s32 byteIdx;
    s32 bitIdx;
    u8 bit;

    data = 0;
    i = 0;

    if (stream->bitLength <= stream->bitPos) {
        STUBBED_PRINTF("BITSTREAM: failed read\n");
    }

    while (n != 0 && stream->bitPos < stream->bitLength) {
        byteIdx = stream->bitPos >> 3;
        bitIdx = stream->bitPos & 7;

        bit = (stream->data[byteIdx] >> bitIdx) & 1;
        data |= bit << i;

        i++;
        stream->bitPos++;
        n--;
    }

    return data;
}

static const char str_800986e8[] = "BIT STREAM: write overflow\n";

void bitstream_write(BitStream *stream, u32 data, u8 n) {
    s32 i;
    s32 byteIdx;
    s32 bitIdx;
    s32 bitPos;
    s32 capacity;
    u8 bit;

    if (stream->capacity) {}
    if (stream->bitPos) {}

    i = 0;

    while (n != 0 && stream->bitPos < stream->capacity) {
        byteIdx = stream->bitPos >> 3;
        bitIdx = stream->bitPos & 7;

        bit = (data >> i) & 1;
        stream->data[byteIdx] |= bit << bitIdx;

        i++;
        
        stream->bitPos++;
        if (stream->bitLength < stream->bitPos) {
            stream->bitLength += 1;
        }

        n--;
    }
}

static const char str_80098704[] = "BIT STREAM: append overflow\n";

void bitstream_append(BitStream *stream, u32 data, u8 n) {
    s32 len;

    len = stream->bitLength;

    stream->bitPos = len;

    bitstream_write(stream, data, n);
}

void bitstream_set_pos(BitStream *stream, s32 bitPos) {
    stream->bitPos = bitPos;
}
