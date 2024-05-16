#include <PR/ultratypes.h>
#include "game/actor/actor.h"

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

void dll_cannon_claw_func_18(TActor *a0, TActor *a1, void *a2) {
    a0->srt.yaw = -32768;
    a0->srt.transl.y = a1->srt.transl.x + 2.0f;  
}

void dll_cannon_claw_func_4C(TActor *s0) {
    s32 temp;
    TActor *func_8002394C_ret;

    func_8002394C_ret = func_8002394C();
    
    if (s0->unk0xdc != 0) {
        return;
    }

    if (s0->curAnimId != 0x208) {
        func_80023D30(s0, 0x208, 0, 0);
    }

    func_80024108(s0, 0.0049999998f, delayFloat, 0);
    temp = func_80025F40(s0, 0, 0, 0);
    
    if (temp != 0) {
        if (s0->linkedActor != 0) {
            ActorUnk0x1a *unk4C = s0->linkedActor->unk0x4c;
            mainSetBits(unk4C->unk0x1a, 1);
            func_8002394C_ret->dll[0]->func[21].withThreeArgs(func_8002394C_ret, 0, 0);
        }
        
        dll_cannon_claw_func_1B4(s0);
    } else {
        if (s0->linkedActor != 0) {
            ActorUnk0x1a *unk4C = s0->linkedActor->unk0x4c;
            if (mainGetBit(unk4C->unk0x1a) != 0) {
                dll_cannon_claw_func_1B4(s0);
            }
        }
    }
}

static void dll_cannon_claw_func_1B4(TActor *a0) {
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
