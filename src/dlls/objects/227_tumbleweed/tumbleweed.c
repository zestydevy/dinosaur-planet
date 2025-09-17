#include "PR/ultratypes.h"
#include "functions.h"
#include "game/objects/object.h"

void func_8002674C(Object* obj);

typedef struct {
/*000*/ s8 unk0[0x270 - 0];
/*270*/ u8 unk270;
/*271*/ s8 unk271[0x280 - 0x271];
/*280*/ s32 unk280;
/*284*/ s32 unk284;
/*288*/ f32 unk288;
/*28C*/ f32 unk28C;
/*290*/ Vec3f* unk290;
} TumbleweedState;

/*0x0*/ static u32 _data_0[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0xC*/ static u32 _data_C[] = {
    0x41c80000
};

// offset: 0x0 | ctor
void dll_227_ctor(void* dll){
}

// offset: 0xC | dtor
void dll_227_dtor(void* dll){
}

// offset: 0x18 | func: 0 | export: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/227_tumbleweed/dll_227_func_18.s")

// offset: 0x354 | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/227_tumbleweed/dll_227_func_354.s")

// offset: 0x420 | func: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/227_tumbleweed/dll_227_func_420.s")

// offset: 0x658 | func: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/227_tumbleweed/dll_227_func_658.s")

// offset: 0xC04 | func: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/227_tumbleweed/dll_227_func_C04.s")

// offset: 0x1518 | func: 5 | export: 2
void dll_227_func_1518(Object* self){
}

// offset: 0x1524 | func: 6 | export: 3
void dll_227_func_1524(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility > 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1578 | func: 7 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/227_tumbleweed/dll_227_func_1578.s")

// offset: 0x16F8 | func: 8 | export: 5
s32 dll_227_func_16F8(Object* self) {
    return 0;
}

// offset: 0x1708 | func: 9 | export: 6
s32 dll_227_func_1708(Object* self, s32 arg1) {
    return 0x29C;
}

// offset: 0x171C | func: 10 | export: 7
u8 dll_227_func_171C(Object* self) {
    TumbleweedState* state = self->state;
    return state->unk270;
}

// offset: 0x172C | func: 11 | export: 8
void dll_227_func_172C(Object* self, f32 arg1, f32 arg2) {
    TumbleweedState *state = self->state;
    state->unk288 = arg1;
    state->unk28C = arg2;
}

// offset: 0x174C | func: 12 | export: 9
void dll_227_func_174C(Object* self) {
    TumbleweedState *state = self->state;
    if (state->unk270 == 1) {
        func_8002674C(self);
        state->unk270 = 2;
        self->unk0xaf &= ~8;
    }
}

// offset: 0x17C0 | func: 13 | export: 10
//When being eaten
void dll_227_func_17C0(Object* self, Vec3f* arg1) {
    TumbleweedState *state = self->state;
    f32 dx;
    f32 dy;
    f32 dz;
    
    dx = arg1->x - self->srt.transl.x;
    dy = arg1->y - self->srt.transl.y;
    dz = arg1->z - self->srt.transl.z;
    
    //@bug: not using delayByte (delta-time)?
    self->speed.x = dx * 0.02f;
    self->speed.y = dy * 0.02f;
    self->speed.z = dz * 0.02f;
    
    state->unk270 = 5;
    state->unk290 = arg1;
}

// offset: 0x1828 | func: 14 | export: 11
s32 dll_227_func_1828(Object* self) {
    TumbleweedState *state = self->state;
    return state->unk270 == 5;
}

// offset: 0x1840 | func: 15 | export: 12
void dll_227_func_1840(Object* self, s32 arg1) {
    TumbleweedState *state = self->state;
    state->unk280 = arg1;
}

// offset: 0x1850 | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/227_tumbleweed/dll_227_func_1850.s")

// offset: 0x1D64 | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/227_tumbleweed/dll_227_func_1D64.s")

// offset: 0x1FA0 | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/227_tumbleweed/dll_227_func_1FA0.s")

// offset: 0x2038 | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/227_tumbleweed/dll_227_func_2038.s")
