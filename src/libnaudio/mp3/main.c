// @DECOMP_OPT_FLAGS=-g
// @DECOMP_IDO_VERSION=7.1
#include "common.h"
#include "libnaudio/mp3/decoder.h"
#include "libnaudio/mp3.h"

s32 mp3_dec_set_side_info(DecoderStream* stream);
s32 mp3_util_func_80077CEC(u8 *buffer, s32 *offset, s32 numbits);
s32 mp3_dec_decode_frame(struct asistream *stream);

extern struct mp3vars g_Mp3Vars; // 0x800bff00

// .data
u32 D_80096790 = 0;

// .bss
extern struct asistream g_AsiStream; // 0x800c4e60

s32 mp3_main_func_80071cf0(struct asistream* arg0) {
    s32 sp1C;
    s32 sp18;

    sp1C = 0x1000;
    if ((arg0->unk201c + arg0->unk3f88) >= 0x1FFC) {
        bcopy(&arg0->unk1c[sp1C], arg0->unk1c, sp1C);
        arg0->unk201c -= sp1C;
        arg0->unk2020 -= sp1C * 8;
    }
    sp18 = arg0->unk04(arg0->unk00, &arg0->unk1c[arg0->unk201c], arg0->unk3f88, -1);
    if (sp18 < arg0->unk3f88) {
        bzero(&arg0->unk1c[sp18], arg0->unk3f88 - sp18);
    }
    arg0->unk18 += arg0->unk3f88;
    arg0->unk201c += arg0->unk3f88;
    return arg0->unk201c - arg0->unk3f88;
}

s32 mp3_main_func_80071e18(struct asistream* stream, s32 arg1) {
    s32 sp24;
    s32 sp20;
    s32 sp1C;
    u8 sp1B;

    if (arg1 != -1) {
        stream->unk18 = arg1;
    }
    sp24 = arg1;
    sp20 = 0;
    sp1B = 0xFF;

    while (TRUE) {
        sp1C = stream->unk04(stream->unk00, &stream->buffer[sp20], 1, sp24);
        if (sp1C <= 0) {
            return 0;
        }
        sp24 = -1;
        stream->unk18 += 1;
        if ((stream->buffer[sp20] & sp1B) != sp1B) {
            sp1B = 0xFF;
            sp20 = 0;
            continue;
        }
        sp20 += 1;
        if (sp1B == 0xF0) {
            break;
        }
        sp1B = 0xF0;
    }
    
    sp1C = stream->unk04(stream->unk00, &stream->buffer[2], 2, -1);
    if (sp1C <= 0) {
        return 0;
    }
    stream->unk18 += 2;
    stream->count = 0xC;
    stream->version = mp3_util_func_80077CEC(stream->buffer, &stream->count, 1);
    stream->layer = mp3_util_func_80077CEC(stream->buffer, &stream->count, 2);
    stream->crctype = mp3_util_func_80077CEC(stream->buffer, &stream->count, 1);
    stream->bitrateindex = mp3_util_func_80077CEC(stream->buffer, &stream->count, 4);
    stream->samplerateindex = mp3_util_func_80077CEC(stream->buffer, &stream->count, 2);
    stream->haspadding = mp3_util_func_80077CEC(stream->buffer, &stream->count, 1);
    stream->privatebit = mp3_util_func_80077CEC(stream->buffer, &stream->count, 1);
    stream->channelmode = mp3_util_func_80077CEC(stream->buffer, &stream->count, 2);
    stream->unk3bc4 = mp3_util_func_80077CEC(stream->buffer, &stream->count, 2);
    stream->unk3bc8 = mp3_util_func_80077CEC(stream->buffer, &stream->count, 1);
    stream->unk3bcc = mp3_util_func_80077CEC(stream->buffer, &stream->count, 1);
    stream->unk3bd0 = mp3_util_func_80077CEC(stream->buffer, &stream->count, 2);
    if ((stream->bitrateindex == 0xF) || (stream->samplerateindex == 3)) {
        return mp3_main_func_80071e18(stream, -1);
    }
    if (stream->doneinitial == 0) {
        stream->doneinitial = 1;
        stream->initialversion = stream->version;
        stream->initiallayer = stream->layer;
        stream->initialcrctype = stream->crctype;
        stream->initialsamplerateindex = stream->samplerateindex;
        stream->initialchannelmode = stream->channelmode;
        stream->unk3bec = stream->unk3bc8;
        stream->unk3bf0 = stream->unk3bcc;
    } else if ((stream->version != stream->initialversion) || 
               (stream->layer != stream->initiallayer) || 
               (stream->crctype != stream->initialcrctype) || 
               (stream->samplerateindex != stream->initialsamplerateindex) || 
               (stream->channelmode != stream->initialchannelmode) || 
               (stream->unk3bcc != stream->unk3bf0)) {
        return mp3_main_func_80071e18(stream, -1);
    }
    stream->unk2068 = 4;
    if (stream->crctype == 0) {
        sp1C = stream->unk04(stream->unk00, &stream->buffer[4], 2, -1);
        if (sp1C <= 0) {
            return 0;
        }
        stream->unk18 += 2;
        stream->count += 0x10;
        stream->unk2068 = 6;
    }
    if (stream->layer == 1) {
        stream->unk8478 = mp3_dec_decode_frame;
        stream->unk847c = mp3_dec_set_side_info;
    } else if (stream->layer == 2) {
        return 0;
    } else if (stream->layer == 3) {
        return 0;
    }
    if (stream->unk847c(stream) == 0) {
        return 0;
    }
    return 1;
}

u32 mp3_main_init(void) {
    if (D_80096790++) {
        return 2;
    }
    mp3_dec_init();
    return 0;
}

struct asistream* mp3_main_func_80072380(s32 arg0, s32 (*arg1)(s32, void*, s32, s32), s32 arg2) {
    struct asistream* sp1C;

    sp1C = &g_AsiStream;
    if (sp1C == NULL) {
        return NULL;
    }
    sp1C->unk0c = -1;
    sp1C->unk10 = -1;
    sp1C->unk14 = -1;
    sp1C->unk00 = arg0;
    sp1C->unk04 = arg1;
    sp1C->unk08 = arg2;
    sp1C->unk201c = 0;
    sp1C->unk2020 = 0;
    sp1C->unk3ba0 = 0;
    mp3_main_func_80071e18(sp1C, 0);
    sp1C->unk8474 = 0;
    bzero(sp1C->unk6a64[0], 0x900);
    return sp1C;
}

s32 mp3_main_func_8007245c(struct asistream* arg0, struct mp3thing** arg1, s32* arg2) {
    struct asistream* sp1C;
    s32 sp18;

    sp1C = arg0;
    sp1C->unk3ba0 += 1;
    if (sp1C->unk3ba0 >= 6) {
        sp1C->unk3ba0 = 0;
    }
    if (mp3_main_func_80071e18(sp1C, sp1C->unk8474) == 0) {
        g_Mp3Vars.state = 3;
        return 0;
    }
    sp1C->unk8474 = -1;
    sp18 = sp1C->unk8478(sp1C);
    if (sp18 == 0) {

    } else {
        *arg1 = &sp1C->unk2070[sp1C->unk3ba0];
        *arg2 = sp1C->numchannels;
    }
    return sp18;
}
