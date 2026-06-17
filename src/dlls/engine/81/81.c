#include "common.h"
#include "sys/segment_1460.h"

// offset: 0x0 | ctor
void dll_81_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_81_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_81_func_18(Object* objA, Object* objB, s8 arg2, s32 arg3) {
    if (arg2 == 1) {
        main_set_bits(BIT_351, TRUE);
    } else if (arg2 == -1) {
        main_set_bits(BIT_351, FALSE);
    }
}
// offset: 0xA4 | func: 1 | export: 1
void dll_81_func_A4(Object* objA, Object* objB, s8 arg2, s32 arg3) {
    f32 time;
    s32 isNighttime;

    isNighttime = gDLL_7_Newday->vtbl->func8(&time);

    if (arg2 == 1) {
        func_80000860(objA, objB, 0x142, arg3);
        gDLL_5_AMSEQ2->vtbl->set(NULL, 5, 0, 0, 0);
        main_set_bits(BIT_Played_Seq_01FD_Rocky_Teaches_Distract, TRUE);
    } else if (arg2 == -1) {
        func_80000860(objA, objB, 0x60, arg3);
        if (isNighttime != 0) {
            gDLL_5_AMSEQ2->vtbl->set(NULL, 4, 0, 0, 0);
        } else {
            gDLL_5_AMSEQ2->vtbl->set(NULL, 1, 0, 0, 0);
        }
        main_set_bits(BIT_Played_Seq_01FD_Rocky_Teaches_Distract, FALSE);
    }
}

// offset: 0x224 | func: 2 | export: 2
void dll_81_func_224(s32 arg0, Object* arg1, s8 arg2, s32 arg3) {
    f32 time;
    s32 isNighttime;

    isNighttime = gDLL_7_Newday->vtbl->func8(&time);
    
    if (arg2 == 1) {
        gDLL_5_AMSEQ2->vtbl->set(NULL, 0xC, 0, 0, 0);
        main_set_bits(BIT_177, TRUE);
    } else if (arg2 == -1) {
        if (isNighttime) {
            gDLL_5_AMSEQ2->vtbl->set(NULL, 4, 0, 0, 0);
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x16, 0, 0, 0);
        } else {
            gDLL_5_AMSEQ2->vtbl->set(NULL, 1, 0, 0, 0);
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x15, 0, 0, 0);
        }
        main_set_bits(BIT_177, FALSE);
    }
}

// offset: 0x3C8 | func: 3 | export: 3
void dll_81_func_3C8(Object* objA, Object* objB, s8 arg2, s32 arg3) {
    f32 time;
    s32 isNighttime;

    isNighttime = gDLL_7_Newday->vtbl->func8(&time);
    
    if (arg2 == 1) {
        gDLL_5_AMSEQ2->vtbl->set(NULL, 9, 0, 0, 0);
    } else if (arg2 == -1) {
        if (isNighttime) {
            gDLL_5_AMSEQ2->vtbl->set(NULL, 4, 0, 0, 0);
        } else {
            gDLL_5_AMSEQ2->vtbl->set(NULL, 1, 0, 0, 0);
        }
    }
}

// offset: 0x4DC | func: 4 | export: 4
void dll_81_func_4DC(Object* objA, Object* objB, s8 arg2, s32 arg3) {

}
