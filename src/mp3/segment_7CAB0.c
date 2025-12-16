// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include "PR/ultratypes.h"
#include "intrinsics.h"
#include "constants.h"

f32 mp3_func_8007BEB0(f32 arg0) {
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    s32 sp8;
    s32 sp4;

    spC = fabsf(arg0);
    
    if (spC < 1.1920929e-7f) {
        return 1.0f;
    }
    
    sp4 = (s32) (spC / 0.69314718f);
    
    if (sp4 > 1024) {
        if (arg0 >= 0.0f) {
            return F32_MAX;
        } else {
            return 0.0f;
        }
    }
    
    sp10 = spC - ((f32) sp4 * 0.69314718f);
    sp8 = 1;
    sp1C = 1.0f;
    sp18 = 1.0f;
    
    do {
        sp14 = sp1C;
        sp18 *= sp10 / (f32) sp8;
        sp1C += sp18;
        sp8 += 1;
    } while (sp1C != sp14);
    
    while (sp4 != 0) {
        sp1C *= 2.0f;
        sp4 -= 1;
    }
    
    if (arg0 >= 0.0f) {
        return sp1C;
    } else {
        return 1.0f / sp1C;
    }
}
