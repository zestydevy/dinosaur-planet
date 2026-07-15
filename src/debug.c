#include "PR/ultratypes.h"
#include "sys/debug.h"
#include "sys/di_comm.h"
#include "macros.h"

s8 gDebugEnabled = 0;

void debugToggle(s8 enabled) {
    gDebugEnabled = enabled;

    if (gDebugEnabled) {
        STUBBED_PRINTF("debug:enabled\n");
    } else {
        STUBBED_PRINTF("debug:disabled\n");
    }
}

s8 debugIsEnabled(void) {
    return gDebugEnabled;
}

s32 debugTick(void) {
    if (!gDebugEnabled) {
        return 0;
    } else {
        diCommDebugTick();
        return 1;
    }
}
