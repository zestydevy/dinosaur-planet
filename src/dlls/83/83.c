#include <PR/ultratypes.h>

void mainSetBits(s32, s32);

void ctor(void *arg) {

}

void dtor(void *arg) {

}

void func_18(void *a0, void *a1, s8 a2, void *a3) {
    if (a2 == 1) {
        mainSetBits(0x3eb, 1);
    } else if (a2 == -1) {
        mainSetBits(0x3eb, 0);
    }
}

void func_A4(void *a0, void *a1, s8 a2, void *a3) {
    if (a2 == 1) {
        mainSetBits(0x3ea, 1);
    } else if (a2 == -1) {
        mainSetBits(0x3ea, 0);
    }
}
