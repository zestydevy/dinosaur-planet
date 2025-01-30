#include <PR/ultratypes.h>
#include "variables.h"
#include "functions.h"
#include "prevent_bss_reordering.h"

static u8 bss_0x0[5];
static u8 bss_0x5;
static s8 bss_0x6;

void dll_30_ctor(void *self) {

}

void dll_30_dtor(void *self) {

}

void dll_30_func_18() {
    s32 i;
    u8 val;

    for (i = 0; i != 5; i++) {
        val = get_gplay_bitstring(311 + i);
        bss_0x0[i] = val;

        if (val != 0) {
            bss_0x6 = i;
        }
    }

    val = get_gplay_bitstring(316);
    bss_0x5 = val;
    if (val == 0) {
        bss_0x5 = 1;
        bss_0x6 = -1;
    }
}

// tricky int casting
#if 1
void dll_30_func_DC(u8 param1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/30_tasktext/dll_30_func_DC.s")
#else
void dll_30_func_DC(u8 param1) {
    s16 i;
    s16 bs_entry;
    s32 curr_bs_value;
    s32 bit_to_set;
    s32 bit_idx;
    u32 new_bs_value;
    s16 bs_entry2;
    s32 tmp;

    // bail if already in array
    for (i = 0; i < 5; i++) {
        if (param1 == bss_0x0[i]) {
            return;
        }
    }

    if (bss_0x6 != 4) {
        // append if there's room
        bss_0x6 = bss_0x6 + 1;
        bss_0x0[bss_0x6] = param1;

        set_gplay_bitstring(311 + bss_0x6, param1);
    } else {
        // otherwise, shift everything down and add to the end
        for (i = 0; i < 4; i++) {
            bss_0x0[i] = bss_0x0[i + 1];
        }

        bss_0x0[4] = param1;

        for (i = 0; i < 5; i++) {
            set_gplay_bitstring(311 + i, bss_0x0[i]);
        }
    }

    tmp = param1;
    if (tmp < 0) {
        tmp = tmp + 31;
    }

    bs_entry = 303 + (tmp >> 5);

    curr_bs_value = get_gplay_bitstring((s16)bs_entry);

    bit_to_set = param1 & 0x1f;
    if (param1 < 0 && (param1 & 0x1f) != 0) {
        bit_to_set = bit_to_set - 32;
    }

    new_bs_value = (1 << (s16)bit_to_set) | curr_bs_value;

    set_gplay_bitstring((s16)bs_entry, new_bs_value);
    

    if (param1 == bss_0x5) {
        do {
            tmp = bss_0x5 + 1;
            bss_0x5 = tmp;

            if (bss_0x5 < 0) {
                tmp = tmp + 31;
            }

            bs_entry2 = ((tmp >> 5) + 303) >> 16;

            if (bs_entry2 != bs_entry) {
                bs_entry = bs_entry2;

                new_bs_value = get_gplay_bitstring(bs_entry2);
            }

            bit_idx = bss_0x5 & 0x1f;
            if (bss_0x5 < 0 && bit_idx != 0) {
                bit_idx = bit_idx - 32;
            }

        } while ((new_bs_value >> (s16)bit_idx) & 1);

        set_gplay_bitstring(316, bss_0x5);
    }

    if (!param1) {
        gDLL_29_gplay->exports->func_958(NULL, 0, 1, func_80048498());
    }
}
#endif

u8 dll_30_func_368() {
    return bss_0x6 + 1;
}

void dll_30_func_390(u8 param1) {
    gDLL_text->exports->func[5].withTwoArgs((u16)(bss_0x0[param1] + 244), 0);
}

void dll_30_func_3F8() {
    gDLL_text->exports->func[5].withTwoArgs((u16)(bss_0x5 + 244), 1);
}

s16 dll_30_func_454() {
    f32 tmp = bss_0x5 / 206.0f;
    return tmp * 100.0f;
}
