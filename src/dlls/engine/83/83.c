#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "sys/main.h"

void ctor(void *arg) {

}

void dtor(void *arg) {

}

void func_18(void *a0, void *a1, s8 a2, void *a3) {
    if (a2 == 1) {
        set_gplay_bitstring(BIT_Play_Seq_0107_Rocky_Intro_Unused, 1);
    } else if (a2 == -1) {
        set_gplay_bitstring(BIT_Play_Seq_0107_Rocky_Intro_Unused, 0);
    }
}

void func_A4(void *a0, void *a1, s8 a2, void *a3) {
    if (a2 == 1) {
        set_gplay_bitstring(BIT_Play_Seq_00D7_Swapped_to_Krystal, 1);
    } else if (a2 == -1) {
        set_gplay_bitstring(BIT_Play_Seq_00D7_Swapped_to_Krystal, 0);
    }
}
