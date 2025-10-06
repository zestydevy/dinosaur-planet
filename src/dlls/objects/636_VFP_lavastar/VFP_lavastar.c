#include "PR/ultratypes.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/modgfx/182.h"
#include "sys/dll.h"
#include "sys/main.h"
#include "sys/rand.h"
#include "dll.h"

typedef struct {
    ObjCreateInfo base;
    u8 _unk18[2];
    s16 unk1A;
    u8 _unk1C[2];
    s16 unk1E;
} VFP_lavastar_CreateInfo;

typedef struct {
    f32 speed;
    s16 unk4;
    u32 soundHandle;
} VFP_lavastar_State;

/*0x0*/ static DLL_182 *sDLL_182 = NULL;

// offset: 0x0 | ctor
void VFP_lavastar_ctor(void *dll) { }

// offset: 0xC | dtor
void VFP_lavastar_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void VFP_lavastar_create(Object* self, VFP_lavastar_CreateInfo* createInfo, s32 arg2) {
    VFP_lavastar_State* state;

    state = self->state;
    state->unk4 = createInfo->unk1E;
    state->speed = rand_next(5, 20) * 0.1f;
    self->srt.transl.y = createInfo->unk1A + createInfo->base.y;
    self->unk0xb0 |= 0x2000;
    sDLL_182 = dll_load_deferred(DLL_ID_182, 1);
    gDLL_17->vtbl->func1(self, 0x3A3, NULL, 0x802, -1, NULL);
    gDLL_17->vtbl->func1(self, 0x3A4, NULL, 0x802, -1, NULL);
}

// offset: 0x158 | func: 1 | export: 1
void VFP_lavastar_update(Object* self) {
    VFP_lavastar_CreateInfo* createInfo;
    VFP_lavastar_State* state;

    state = (VFP_lavastar_State*)self->state;
    createInfo = (VFP_lavastar_CreateInfo*)self->createInfo;
    self->srt.transl.y += delayFloat * state->speed;
    if ((createInfo->base.y + 1200.0f) < self->srt.transl.y) {
        state->speed = rand_next(5, 20) * 0.1f;
        self->srt.transl.y = createInfo->base.y;
    }
    if (rand_next(0, 3) == 0) {
        sDLL_182->vtbl->func0(self, 0, NULL, 4, -1, NULL);
    }
    if (state->soundHandle == 0) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_AAE, MAX_VOLUME, &state->soundHandle, NULL, 0, NULL);
    }
}

// offset: 0x2BC | func: 2 | export: 2
void VFP_lavastar_func_2BC(Object *self) { }

// offset: 0x2C8 | func: 3 | export: 3
void VFP_lavastar_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x2E0 | func: 4 | export: 4
void VFP_lavastar_destroy(Object* self, s32 arg1) {
    VFP_lavastar_State* state;

    state = (VFP_lavastar_State*)self->state;
    gDLL_13_Expgfx->vtbl->func5(self);
    gDLL_14_Modgfx->vtbl->func4(self);
    dll_unload(sDLL_182);
    gDLL_6_AMSFX->vtbl->func_A1C(state->soundHandle);
    state->soundHandle = 0;
}


// offset: 0x390 | func: 5 | export: 5
u32 VFP_lavastar_func_390(Object *self) {
    return 0;
}

// offset: 0x3A0 | func: 6 | export: 6
u32 VFP_lavastar_get_state_size(Object *self, u32 a1) {
    return sizeof(VFP_lavastar_State);
}
