#include "sys/main.h"
#include "game/gamebits.h"
#include "unktypes.h"

// offset: 0x0 | ctor
void dll_83_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_83_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_83_func_18(UNK_PTR *a0, UNK_PTR *a1, s8 a2, UNK_PTR *a3) {
    if (a2 == 1) {
        main_set_bits(BIT_Play_Seq_0107_Rocky_Intro_Unused, 1);
    } else if (a2 == -1) {
        main_set_bits(BIT_Play_Seq_0107_Rocky_Intro_Unused, 0);
    }

}

// offset: 0xA4 | func: 1 | export: 1
void dll_83_func_A4(UNK_PTR *a0, UNK_PTR *a1, s8 a2, UNK_PTR *a3) {
    if (a2 == 1) {
        main_set_bits(BIT_Play_Seq_00D7_Swapped_to_Krystal, 1);
    } else if (a2 == -1) {
        main_set_bits(BIT_Play_Seq_00D7_Swapped_to_Krystal, 0);
    }
}