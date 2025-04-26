#include "PR/ultratypes.h"
#include "sys/main.h"

void ctor(void *arg) {

}

void dtor(void *arg) {

}

void func_18(void *a0, void *a1, s8 a2, void *a3) {
    if (a2 == 1) {
        set_gplay_bitstring(1003, 1);
    } else if (a2 == -1) {
        set_gplay_bitstring(1003, 0);
    }
}

void func_A4(void *a0, void *a1, s8 a2, void *a3) {
    if (a2 == 1) {
        set_gplay_bitstring(1002, 1);
    } else if (a2 == -1) {
        set_gplay_bitstring(1002, 0);
    }
}
