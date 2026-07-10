#include "PR/os.h"
#include "sys/debug.h"
#include "sys/di_comm.h"

// not sure which file this should be in, it's unused
static s32 D_80093860[] = {
    -1, 0, -1, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
};
static s32 D_80093890[6] = {0, -1, -1, -1, -1, -1};
static u8 D_800938A8 = 0;

static const char str_8009b720[] = "COMM: warning, already started (%s)\n";
static const char str_8009b748[] = "COMM: warning, not started (%s)\n";
static const char str_8009b76c[] = "COMM: warning, comms not started\n";
static const char str_8009b790[] = "COMM: warning, comms not started\n";

void diCommFuncStart(const char *str) {
    if (D_800938A8 == 0) {
        D_800938A8 = 1;
    }
}

void diCommFuncEnd(const char *str) {
    if (D_800938A8 != 0) {
        D_800938A8 = 0;
    }
}

void diCommWriteHost(void *param1, u32 param2) {
    osWritebackDCache(param1, param2);
    osWriteHost(param1, param2);
}

void diCommReadHost(void *param1, u32 param2) {
    osInvalDCache(param1, param2);
    osReadHost(param1, param2);
}

void diCommWriteWord(s32 param1) {
    diCommWriteHost((void*)&param1, 4);
}

void diCommFunc80063804(s32 param1) {
    s32 i;

    for (i = 5; i > 1; i--) {
        D_80093890[i] = D_80093890[i - 1];
    }

    D_80093890[1] = param1;

    diCommWriteWord(param1);
}

s32 diCommFunc8006385C(void) {
    return 0;
}

s32 diCommGameCrashed(s32 param1, s32 param2) {
    s32 var;

    var = 0x2e;
    
    diCommFuncStart("diCommGameCrashed");

    diCommWriteHost(&var, 4);
    diCommWriteHost(&param1, 4);
    diCommWriteHost(&param2, 4);

    diCommFuncEnd("diCommGameCrashed");

    return 0;
}

void diCommDebugTick(void) {
    s32 var1;
    void *var2;
    s32 var3;

    diCommFuncStart("diCommDebugTick");

    diCommFunc80063804(0x84);
    diCommReadHost(&var1, 4);

    switch (var1) {
        case 0:
            break;
        case 1:
            diCommReadHost(&var2, 4);
            diCommReadHost(&var3, 4);
            diCommWriteHost(var2, var3);
            break;
        case 2:
            diCommReadHost(&var2, 4);
            diCommReadHost(&var3, 4);
            diCommReadHost(var2, var3);
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            debugToggle(0);
            break;
    }

    diCommFuncEnd("diCommDebugTick");
}
