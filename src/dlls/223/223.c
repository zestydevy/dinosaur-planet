#include <PR/ultratypes.h>
#include "game/actor/actor.h"

typedef struct {
    /* 0x0 */  s16 unk0x0;
    /* 0x4 */  s32 unk0x4;
    /* 0x8 */  s32 unk0x8;
    /* 0xC */  f32 unk0xC;
    /* 0x10 */ f32 unk0x10;
} UnkDll223Struct1;

typedef union {
    void (*func_58)(TActor*,s32,s32);
} UnkDllFuncs1;

typedef struct {
    UnkDllFuncs1 *func;
} UnkDllInst;

typedef void (*self_func_1B4)(TActor *a0);

extern f32 delayFloat;

extern void func_800267A4();
extern void draw_actor(TActor*,s32,s32,s32,s32,float);
extern TActor *func_8002394C();
extern s32 func_80023D30(TActor*,s32,f32,s32);
extern s32 func_80024108(TActor*,f32,f32,s32);
extern s32 func_80025F40(TActor*,s32,s32,s32);
s32 mainGetBit(s32);
void mainSetBits(s32, s32);

void dll_cannon_claw_func_1B4(TActor *a0);

void dll_cannon_claw_ctor(void *arg) {

}

void dll_cannon_claw_dtor(void *arg) {
    
}

void dll_cannon_claw_func_18(UnkDll223Struct1 *a0, UnkDll223Struct1 *a1, void *a2) {
    a0->unk0x0 = -0x8000;
    a0->unk0x10 = a1->unk0xC + 2.0f;  
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/223/dll_cannon_claw_func_4C.s")
#else
// Regalloc
void dll_cannon_claw_func_4C(TActor *s0) {
    UnkDllInst *dll_inst;
    TActor *func_8002394C_ret = func_8002394C();
    
    if (s0->unk0xdc != 0) {
        return;
    }

    if (s0->curAnimId != 0x208) {
        func_80023D30(s0, 0x208, 0, 0);
    }

    func_80024108(s0, 0.0049999998f, delayFloat, 0);
    
    if (func_80025F40(s0, 0, 0, 0) != 0) {
        if (s0->linkedActor != 0) {
            mainSetBits(s0->linkedActor->unk0x4c->unk0x1a, 1);
            dll_inst = ((UnkDllInst*)func_8002394C_ret->dll);
            dll_inst->func[0x16].func_58(func_8002394C_ret, 0, 0);
        }
        
        // TODO: this matches but is awful
        ((self_func_1B4)(((int)&dll_cannon_claw_ctor) + 0x1b4))(s0);
    } else if (s0->linkedActor != 0 && mainGetBit(s0->linkedActor->unk0x4c->unk0x1a) != 0) {
        ((self_func_1B4)(((int)&dll_cannon_claw_ctor) + 0x1b4))(s0);
    }
}
#endif

void dll_cannon_claw_func_1B4(TActor *a0) {
    a0->unk0xdc = 1;
    a0->unk0xaf = a0->unk0xaf | 8;    
    func_800267A4();   
}

void dll_cannon_claw_func_200(void *a0) {

}

void dll_cannon_claw_func_20C(TActor *a0, s32 a1, s32 a2, s32 a3, s32 a4, s8 a5) {
    if (a5 != 0 && a0->unk0xdc == 0) {
		draw_actor(a0, a1, a2, a3, a4, 1.0f);
	}
}

void dll_cannon_claw_func_26C(void *a0, void *a1) {

}

s32 dll_cannon_claw_func_27C(void *a0) {
    return 0;
}

s32 dll_cannon_claw_func_28C(void *a0, void *a1) {
    return 0;
}
