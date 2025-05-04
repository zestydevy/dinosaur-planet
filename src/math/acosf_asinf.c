// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include "PR/ultratypes.h"
#include "libnaudio/n_unkfuncs.h"
#include "sys/math.h"

f32 acosf(f32 value) {
    s16 var1;
    u16 var2;

    if (value >= 1.0f) {
        var1 = 32767;
    } else if (value <= -1.0f) {
        var1 = 32769;
    } else {
        var1 = (s16)(value * 32767.0f);
    }

    var2 = acos(var1);

    return (var2 * 3.1415927f) / 65535.0f;
}

f32 asinf(f32 value) {
    s16 var1;
    s16 var2;

    if (value >= 1.0f) {
        var1 = 32767;
    } else if (value <= -1.0f) {
        var1 = 32769;
    } else {
        var1 = (s16)(value * 32767.0f);
    }

    var2 = asin(var1);

    return (var2 * 3.1415927f) / 65535.0f;
}
