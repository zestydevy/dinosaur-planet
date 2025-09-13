#include "common.h"

s32 D_8008C500 = 0;
u8 D_8008C504 = 0;
s32 D_8008C508 = 0;

s32 func_800007D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return 0;
}

s32 func_800007EC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return -1;
}

s32 func_80000808(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return 0;
}

s16 func_80000824(s32 arg0) {
    if (arg0 != -1) {
        D_8008C508 = arg0;
        return arg0;
    }
    return D_8008C508;
}
