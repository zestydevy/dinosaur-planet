#include "common.h"

s32 func_800007D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return 0;
}

s32 func_800007EC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return -1;
}

s32 func_80000808(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return 0;
}

extern s32 D_8008C508;
extern s16 D_8008C50A;
s16 func_80000824(s32 arg0);
s16 func_80000824(s32 arg0) {
    if (arg0 != -1) {
        D_8008C508 = arg0;
        return arg0;
    }
    return D_8008C50A;
}