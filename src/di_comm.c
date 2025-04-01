#include "common.h"

static s32 D_80093890[6] = {0, -1, -1, -1, -1, -1};
static u8 D_800938A8 = 0;

static const char str_8009b720[] = "COMM: warning, already started (%s)\n";
static const char str_8009b748[] = "COMM: warning, not started (%s)\n";
static const char str_8009b76c[] = "COMM: warning, comms not started\n";
static const char str_8009b790[] = "COMM: warning, comms not started\n";

void di_comm_func_start(const char *str) {
    if (D_800938A8 == 0) {
        D_800938A8 = 1;
    }
}

void di_comm_func_end(const char *str) {
    if (D_800938A8 != 0) {
        D_800938A8 = 0;
    }
}

void di_comm_write_host(void *param1, u32 param2) {
    osWritebackDCache(param1, param2);
    osWriteHost(param1, param2);
}

void di_comm_read_host(void *param1, u32 param2) {
    osInvalDCache(param1, param2);
    osReadHost(param1, param2);
}

void di_comm_write_word(s32 param1) {
    di_comm_write_host((void*)&param1, 4);
}

void di_comm_80063804(s32 param1) {
    s32 i;

    for (i = 5; i > 1; i--) {
        D_80093890[i] = D_80093890[i - 1];
    }

    D_80093890[1] = param1;

    di_comm_write_word(param1);
}

s32 ret0_8006385c() {
    return 0;
}

s32 diCommGameCrashed(s32 param1, s32 param2) {
    s32 var;

    var = 0x2e;
    
    di_comm_func_start("diCommGameCrashed");

    di_comm_write_host(&var, 4);
    di_comm_write_host(&param1, 4);
    di_comm_write_host(&param2, 4);

    di_comm_func_end("diCommGameCrashed");

    return 0;
}

void diCommDebugTick() {
    s32 var1;
    void *var2;
    s32 var3;

    di_comm_func_start("diCommDebugTick");

    di_comm_80063804(0x84);
    di_comm_read_host(&var1, 4);

    switch (var1) {
        case 0:
            break;
        case 1:
            di_comm_read_host(&var2, 4);
            di_comm_read_host(&var3, 4);
            di_comm_write_host(var2, var3);
            break;
        case 2:
            di_comm_read_host(&var2, 4);
            di_comm_read_host(&var3, 4);
            di_comm_read_host(var2, var3);
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            func_80061F30(0);
            break;
    }

    di_comm_func_end("diCommDebugTick");
}
