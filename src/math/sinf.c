// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include <PR/ultratypes.h>

f32 sinf(f32 arg0) {
    f32 sp1C;
    f32 sp18;
    s32 sp14;
    register f64 var_f15;
    register f32 var_f16;
    
    if (arg0 >= 0.0f) {
        var_f15 = 0.5;
    } else {
        var_f15 = -0.5;
    }
    
    sp14 = (s32) ((f64) var_f15 + (arg0 / 3.1415927f));
    arg0 = arg0 - ((f32) sp14 * 3.1415927f);

    var_f16 = (arg0 > 0.0f) ? arg0 : (f32) (-arg0 > 1.1920929e-7f);
    
    if (var_f16 != 0.0f) {
        sp1C = arg0 * arg0;
        sp18 = ((((((((((((((sp1C * 2.8114574E-15f) + -7.6471636E-13f) * sp1C) + 1.6059044E-10f) * sp1C) + -2.5052108E-8f) * sp1C) + 0.0000027557319f) * sp1C) + -0.0001984127f) * sp1C) + 0.0083333338f) * sp1C) + -0.16666667f) * sp1C;
        arg0 = arg0 + arg0 * sp18;
    }
    
    if (sp14 & 1) {
        return -arg0;
    } else {
        return arg0;
    }
}
