#include "common.h"

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/segment_78500/func_80077900.s")
#else

f32 func_8007BEB0(f32);                             /* extern */
f32 func_8007C080(f32);                             /* extern */

f32 func_80077900(f32 arg0, f32 arg1) {
    s32 sp34;
    f32 sp30;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;

    sp34 = (s32) arg1;
    if ((arg0 != 0.0f) && (arg1 == 0.0f)) {
        return 1.0f;
    }

    if (arg0 == 0.0f) {
        return 0.0f;
    }

    if (((arg0 == 0.0f) && (arg1 == 0.0f)) || ((arg0 < 0.0f) && ((f32) sp34 != arg1))) {
        return 0.0f;
    }

    if ((f32) sp34 != arg1) {
        arg0 = func_8007BEB0(func_8007C080(arg0) * arg1);
    } else if (arg1 > 0.0f) {
        arg1 -= 1.0f;
        sp30 = arg0;

        var_s0 = 0;
        if (arg1 != 0.0f) {
            var_s0 = 1;
        }
        arg1 -= 1.0f;

        if (var_s0 != 0) {
            do {
                arg0 *= sp30;
                var_s0_2 = 0;
                if (arg1 != 0.0f) {
                    var_s0_2 = 1;
                }
                arg1 -= 1.0f;
            } while (var_s0_2 != 0);
        }
        
    } else {
        sp30 = 1.0f;
        var_s0_3 = 0;
        if (arg1 != 0.0f) {
            var_s0_3 = 1;
        }
        arg1 += 1.0f;
        if (var_s0_3 != 0) {
            do {
                sp30 /= arg0;
                var_s0_4 = 0;
                if (arg1 != 0.0f) {
                    var_s0_4 = 1;
                }
                arg1 += 1.0f;
            } while (var_s0_4 != 0);
        }
        arg0 = sp30;
    }

    return arg0;
}

#endif