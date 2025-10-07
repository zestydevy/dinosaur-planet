#include "PR/ultratypes.h"
#include "dlls/objects/common/sidekick.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "dll.h"

typedef struct {
    u8 unk0;
} SHtricky_State;

// offset: 0x0 | ctor
void SHtricky_ctor(void *dll) { }

// offset: 0xC | dtor
void SHtricky_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHtricky_create(Object* self, ObjCreateInfo* createInfo, s32 arg2) {
    SHtricky_State *state;

    state = (SHtricky_State*)self->state;
    
    if (get_gplay_bitstring(0xB0) != 0) {
        state->unk0 = 3;
    } else {
        set_gplay_bitstring(0xC3, 0);
        state->unk0 = 0;
    }

    self->unk0xb0 |= 0x2000;
}

// offset: 0xA8 | func: 1 | export: 1
void SHtricky_update(Object *self) {
    SHtricky_State *state;
    Object *sidekick;

    state = (SHtricky_State*)self->state;
    sidekick = get_sidekick();

    switch (state->unk0) {
    case 0:
        if (get_gplay_bitstring(0xC3) != 0) {
            gDLL_3_Animation->vtbl->func17(3, sidekick, -1);
            state->unk0 = 1;
        } 
        break;
    case 1:
        state->unk0 = 2;
        break;
    case 2:
        ((DLL_ISidekick*)sidekick->dll)->vtbl->func22(sidekick, self);
        state->unk0 = 3;
        break;
    case 3:
        break;
    }
}

// offset: 0x1A8 | func: 2 | export: 2)
void SHtricky_func_1A8(Object *self) { }

// offset: 0x1B4 | func: 3 | export: 3
void SHtricky_draw(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x1CC | func: 4 | export: 4
void SHtricky_destroy(Object *self, s32 a1) { }

// offset: 0x1DC | func: 5 | export: 5
s32 SHtricky_func_1DC(Object* self) {
    return 0;
}

// offset: 0x1EC | func: 6 | export: 6
u32 SHtricky_get_state_size(Object *self, u32 a1) {
    return sizeof(SHtricky_State);
}
