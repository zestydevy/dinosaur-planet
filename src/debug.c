#include "PR/ultratypes.h"
#include "sys/di_comm.h"
#include "macros.h"

s8 gDebugEnabled = 0;

void debug_toggle(s8 enabled) {
    gDebugEnabled = enabled;

    if (gDebugEnabled) {
        STUBBED_PRINTF("debug:enabled\n");
    } else {
        STUBBED_PRINTF("debug:disabled\n");
    }
}

s8 debug_is_enabled() {
    return gDebugEnabled;
}

s32 debug_tick() {
    if (!gDebugEnabled) {
        return 0;
    } else {
        diCommDebugTick();
        return 1;
    }
}
