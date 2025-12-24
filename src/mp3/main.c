// @DECOMP_OPT_FLAGS=-g
// @DECOMP_IDO_VERSION=7.1
#include "mp3/mp3.h"
#include "mp3/mp3_internal.h"
#include "macros.h"

// -------- .data start -------- //
u32 D_80096790 = 0;
// -------- .data end -------- //

// -------- .bss start -------- //
struct asistream g_AsiStream; // 0x800c4e60
// -------- .bss end -------- //

s32 mp3_main_func_80071cf0(struct asistream *stream) {
    s32 sp1C;
    s32 sp18;

    sp1C = 0x1000;
    if ((stream->unk201c + stream->unk3f88) >= 0x1FFC) {
        bcopy(&stream->unk1c[sp1C], stream->unk1c, sp1C);
        stream->unk201c -= sp1C;
        stream->unk2020 -= sp1C * 8;
    }
    sp18 = stream->dmafunc(stream->unk00, &stream->unk1c[stream->unk201c], stream->unk3f88, -1);
    if (sp18 < stream->unk3f88) {
        bzero(&stream->unk1c[sp18], stream->unk3f88 - sp18);
    }
    stream->unk18 += stream->unk3f88;
    stream->unk201c += stream->unk3f88;
    return stream->unk201c - stream->unk3f88;
}

s32 mp3_main_func_80071e18(struct asistream *stream, s32 arg1) {
    s32 sp24;
    s32 sp20;
    s32 sp1C;
    u8 mask;

    if (arg1 != -1) {
        stream->unk18 = arg1;
    }
    sp24 = arg1;
    sp20 = 0;
    mask = 0xFF;

    while (TRUE) {
        sp1C = stream->dmafunc(stream->unk00, &stream->buffer[sp20], 1, sp24);
        if (sp1C <= 0) {
            return FALSE;
        }
        sp24 = -1;
        stream->unk18 += 1;
        if ((stream->buffer[sp20] & mask) != mask) {
            mask = 0xFF;
            sp20 = 0;
            continue;
        }
        sp20 += 1;
        if (mask == 0xF0) {
            break;
        }
        mask = 0xF0;
    }
    
    sp1C = stream->dmafunc(stream->unk00, &stream->buffer[2], 2, -1);
    if (sp1C <= 0) {
        return FALSE;
    }
    stream->unk18 += 2;
    stream->offset = 0xC;
    stream->version = mp3util_get_bits(stream->buffer, &stream->offset, 1);
    stream->layer = mp3util_get_bits(stream->buffer, &stream->offset, 2);
    stream->crctype = mp3util_get_bits(stream->buffer, &stream->offset, 1);
    stream->bitrateindex = mp3util_get_bits(stream->buffer, &stream->offset, 4);
    stream->samplerateindex = mp3util_get_bits(stream->buffer, &stream->offset, 2);
    stream->haspadding = mp3util_get_bits(stream->buffer, &stream->offset, 1);
    stream->privatebit = mp3util_get_bits(stream->buffer, &stream->offset, 1);
    stream->channelmode = mp3util_get_bits(stream->buffer, &stream->offset, 2);
    stream->channelmodeext = mp3util_get_bits(stream->buffer, &stream->offset, 2);
    stream->copyright = mp3util_get_bits(stream->buffer, &stream->offset, 1);
    stream->isoriginal = mp3util_get_bits(stream->buffer, &stream->offset, 1);
    stream->emphasis = mp3util_get_bits(stream->buffer, &stream->offset, 2);
    if ((stream->bitrateindex == 0xF) || (stream->samplerateindex == 3)) { /* bad/reserved values */
        return mp3_main_func_80071e18(stream, -1);
    }
    if (stream->doneinitial == 0) {
        stream->doneinitial = 1;
        stream->initialversion = stream->version;
        stream->initiallayer = stream->layer;
        stream->initialcrctype = stream->crctype;
        stream->initialsamplerateindex = stream->samplerateindex;
        stream->initialchannelmode = stream->channelmode;
        stream->initialcopyright = stream->copyright;
        stream->initialisoriginal = stream->isoriginal;
    } else if ((stream->version != stream->initialversion) || 
               (stream->layer != stream->initiallayer) || 
               (stream->crctype != stream->initialcrctype) || 
               (stream->samplerateindex != stream->initialsamplerateindex) || 
               (stream->channelmode != stream->initialchannelmode) || 
               (stream->isoriginal != stream->initialisoriginal)) {
        return mp3_main_func_80071e18(stream, -1);
    }
    stream->unk2068 = 4;
    if (stream->crctype == CRC_PROTECTED) {
        sp1C = stream->dmafunc(stream->unk00, &stream->buffer[4], 2, -1);
        if (sp1C <= 0) {
            return FALSE;
        }
        stream->unk18 += 2;
        stream->offset += 16;
        stream->unk2068 = 6;
    }
    if (stream->layer == LAYER_3) {
        stream->decoderframefunc = mp3_dec_decode_frame;
        stream->setsideinfofunc = mp3_dec_set_side_info;
    } else if (stream->layer == LAYER_2) {
        return FALSE;
    } else if (stream->layer == LAYER_1) {
        return FALSE;
    }
    if (stream->setsideinfofunc(stream) == 0) {
        return FALSE;
    }
    return TRUE;
}

u32 mp3_main_init(void) {
    if (D_80096790++) {
        return 2;
    }
    mp3_dec_init();
    return 0;
}

struct asistream* mp3_main_func_80072380(s32 arg0, mp3_dmafunc dmafunc, s32 filesize) {
    struct asistream *stream = &g_AsiStream;
    if (stream == NULL) {
        return NULL;
    }
    stream->unk0c = -1;
    stream->unk10 = -1;
    stream->unk14 = -1;
    stream->unk00 = arg0;
    stream->dmafunc = dmafunc;
    stream->filesize = filesize;
    stream->unk201c = 0;
    stream->unk2020 = 0;
    stream->unk3ba0 = 0;
    mp3_main_func_80071e18(stream, 0);
    stream->unk8474 = 0;
    bzero(stream->unk6a64[0], 0x900);
    return stream;
}

s32 mp3_main_func_8007245c(struct asistream *streamptr, u16 **arg1, s32 *numchannels) {
    struct asistream *stream;
    s32 result;

    stream = streamptr;
    stream->unk3ba0 += 1;
    if (stream->unk3ba0 >= (s32)ARRAYCOUNT(stream->unk2070)) {
        stream->unk3ba0 = 0;
    }
    if (mp3_main_func_80071e18(stream, stream->unk8474) == 0) {
        g_Mp3Vars.state = 3;
        return 0;
    }
    stream->unk8474 = -1;
    result = stream->decoderframefunc(stream);
    if (result == 0) {

    } else {
        *arg1 = stream->unk2070[stream->unk3ba0];
        *numchannels = stream->numchannels;
    }
    return result;
}
