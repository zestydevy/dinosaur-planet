#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"
#include "types.h"

#include "PR/ultratypes.h"
#include "dll.h"
#include "game/objects/object.h"
#include "sys/dll.h"

typedef struct{
/*00*/ s32 unk0;
/*04*/ s32 unk4;
/*08*/ u32 soundHandle;
/*0C*/ s32 unkC;
/*10*/ s32 unk10;
/*14*/ s16 shakeSoundTimer;
/*16*/ s8 unk16[0x28 - 0x16];
} SmallBasketState;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw;
/*19*/ s8 storedItemID;
/*1A*/ s8 unk1A;
/*1B*/ s8 storedItemQuantity;
/*1C*/ s8 unk1C;
/*1D*/ s8 unk1D;
/*1E*/ s8 unk1E;
/*1F*/ s8 unk1F;
/*20*/ s16 autoThrowRadius;
} SmallBasket_Setup;

/*0x0*/ static s32 _data_0[] = {
    0, 0, 0, 0, 0, 0, 0, 0
};

/*0x0*/ static u8 _bss_0[0x10];

// offset: 0x0 | ctor
void dll_295_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_295_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_295_create(Object *self, ObjSetup* setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/295_smallbasket/dll_295_create.s")

// offset: 0x1B8 | func: 1 | export: 1
void dll_295_update(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/295_smallbasket/dll_295_update.s")

// offset: 0xC0C | func: 2 | export: 2
void dll_295_func_C0C(Object *self) { }

// offset: 0xC18 | func: 3 | export: 3
void dll_295_draw(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/295_smallbasket/dll_295_draw.s")

// offset: 0xD04 | func: 4 | export: 4
void dll_295_destroy(Object* self, s32 arg1) {
    SmallBasketState* state = self->state;

    gDLL_14_Modgfx->vtbl->func5(self);
    dll_unload((void*)_data_0[0]);
    if (state->soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(state->soundHandle);
        state->soundHandle = 0;
    }
}

// offset: 0xD9C | func: 5 | export: 5
u32 dll_295_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0xDAC | func: 6 | export: 6
u32 dll_295_get_state_size(Object *self, u32 a1) {
    return sizeof(SmallBasketState);
}

// offset: 0xDC0 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/295_smallbasket/dll_295_func_DC0.s")

// offset: 0xE78 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/295_smallbasket/dll_295_func_E78.s")

// offset: 0x1104 | func: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/295_smallbasket/dll_295_func_1104.s")

// offset: 0x2024 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/295_smallbasket/dll_295_func_2024.s")
