// @DECOMP_OPT_FLAGS=-g
// @DECOMP_IDO_VERSION=7.1
#include "PR/ultratypes.h"
#include "intrinsics.h"

f32 mp3_func_8007C080(f32 param1) {
    s32 var2;
    f32 var3;
    f32 var4;
    f32 var1;
    f32 tmp;

    if (param1 < 0.0f) {
        return 0.0f;
    }
    if (param1 == 0.0f) {
        return 0.0f;
    }

    var1 = 0.0f;

    while (2.0f <= param1) {
        param1 = param1 / 2.0f;
        var1 += 0.69314718f;
    }

    while (param1 < 1.0f) {
        param1 = param1 * 2.0f;
        var1 -= 0.69314718f;
    }

    var2 = 1;
    var3 = (param1 - 1.0f) / (param1 + 1.0f);
    var4 = 2.0f * var3;
    var3 = var3 * var3;

    do {
        tmp = var1;
        var1 += var4 / (f32)var2;
        var4 = var4 * var3;
        var2 = var2 + 2;
    } while (var1 != tmp);

    return var1;
}
