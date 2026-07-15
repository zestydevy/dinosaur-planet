// @DECOMP_OPT_FLAGS=-O1 -g2
// @DECOMP_IDO_VERSION=7.1
#include "PR/ultratypes.h"
#include "sys/math.h"

// official name: acosf
f32 acosf(f32 value) {
    s16 var1;
    u16 var2;

    if (value >= 1.0f) {
        var1 = 0x7FFF;
    } else if (value <= -1.0f) {
        var1 = -0x7FFF;
    } else {
        var1 = (s16)(value * 0x7FFF);
    }

    var2 = acoss(var1);

    return (var2 * 3.1415927f) / 0xFFFF;
}

f32 asinf(f32 value) {
    s16 var1;
    s16 var2;

    if (value >= 1.0f) {
        var1 = 0x7FFF;
    } else if (value <= -1.0f) {
        var1 = -0x7FFF;
    } else {
        var1 = (s16)(value * 0x7FFF);
    }

    var2 = asins(var1);

    return (var2 * 3.1415927f) / 0xFFFF;
}
