#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "sys/controller.h"
#include "sys/main.h"
#include "sys/map.h"
#include "functions.h"
#include "types.h"
#include "dll.h"

typedef struct {
    ObjCreateInfo base;
    s8 unk18;
} SHqueenearthwalker_CreateInfo;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} SHqueenearthwalker_State;

static s32 SHqueenearthwalker_func_4F8(Object* a0, Object* a1, AnimObjState* a2, void* a3);

// offset: 0x0 | ctor
void SHqueenearthwalker_ctor(void *dll) { }

// offset: 0xC | dtor
void SHqueenearthwalker_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHqueenearthwalker_create(Object* self, SHqueenearthwalker_CreateInfo* createInfo, s32 arg2) {
    SHqueenearthwalker_State* sp24;

    sp24 = self->state;
    self->srt.yaw = createInfo->unk18 << 8;
    self->unk0xbc = (ObjectCallback)SHqueenearthwalker_func_4F8;
    sp24->unk1 = get_gplay_bitstring(0xC2);
    sp24->unk0 = get_gplay_bitstring(0xB0);
}

// offset: 0xA0 | func: 1 | export: 1
void SHqueenearthwalker_update(Object* self) {
    SHqueenearthwalker_State* state;
    s32 sp28;

    state = self->state;
    sp28 = state->unk0;
    self->unk0xaf &= ~8;
    if (self->curModAnimId != 1) {
        func_80023D30(self, 1, 0.0f, 0);
    }
    func_80024108(self, 0.005f, delayByte, 0);
    switch (state->unk0) {

    case 1:
        state->unk0 = 2;
        break;
    case 2:
        if (self->unk0xaf & 1) {
            set_button_mask(0, 0x8000);
            gDLL_3_Animation->vtbl->func17(1, self, -1);
            set_gplay_bitstring(0xBF, 1);
            state->unk0 = 3;
        }
        break;
    case 3:
        if (self->unk0xaf & 4) {
            if (gDLL_1_UI->vtbl->func7(0x66D) != 0) {
                set_button_mask(0, 0x8000);
                state->unk1 += get_gplay_bitstring(0x66D);
                if (state->unk1 <= 0) {
                    gDLL_3_Animation->vtbl->func17(3, self, -1);
                } else {
                    state->unk0 = 4U;
                    gDLL_30_Task->vtbl->mark_task_completed(0xB);
                    set_gplay_bitstring(0x8D4, 1);
                }
                set_gplay_bitstring(0x66D, 0);
                set_gplay_bitstring(0xC2, state->unk1);
            } else if (self->unk0xaf & 1) {
                set_button_mask(0, 0x8000);
                gDLL_3_Animation->vtbl->func17(4, self, -1);
            }
        }
        break;
    case 4:
        gDLL_3_Animation->vtbl->func17(2, self, -1);
        break;
    case 5:
        gDLL_3_Animation->vtbl->func17(6, self, -1);
        break;
    case 6:
        gDLL_3_Animation->vtbl->func17(7, self, -1);
        state->unk0 = 7;
        break;
    case 7:
        break;
    default:
        state->unk0 = 1;
        break;
    }
    if (sp28 != state->unk0) {
        set_gplay_bitstring(0xB0, state->unk0);
    }
}

// offset: 0x464 | func: 2 | export: 2
void SHqueenearthwalker_func_464(Object *self) { }

// offset: 0x470 | func: 3 | export: 3
void SHqueenearthwalker_draw(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4C4 | func: 4 | export: 4
void SHqueenearthwalker_destroy(Object *self, s32 a1) { }

// offset: 0x4D4 | func: 5 | export: 5
s32 SHqueenearthwalker_get_setup_flags(Object* self) {
    return 0;
}

// offset: 0x4E4 | func: 6 | export: 6
u32 SHqueenearthwalker_get_state_size(Object *self, u32 a1) {
    return sizeof(SHqueenearthwalker_State);
}

// offset: 0x4F8 | func: 7
static s32 SHqueenearthwalker_func_4F8(Object* a0, Object* a1, AnimObjState* a2, void* a3) {
    SHqueenearthwalker_State* state;
    s32 sp40;
    s32 i;

    state = a0->state;
    sp40 = state->unk0;
    a0->unk0xaf |= 8;
    a2->unk62 = 0;

    for (i = 0; i < a2->unk98; i++) {
        switch (a2->unk8E[i]) {
        case 3:
            state->unk0 = 5;
            gDLL_29_Gplay->vtbl->set_map_setup(4, 0xB);
            set_gplay_bitstring(0x486, 1);
            warpPlayer(0x51, 0);
            break;
        case 4:
            state->unk0 = 6;
            gDLL_29_Gplay->vtbl->set_map_setup(4, 0xB);
            set_gplay_bitstring(0x487, 1);
            warpPlayer(0x51, 0);
            break;
        }
    }

    if (sp40 != state->unk0) {
        set_gplay_bitstring(0xB0, state->unk0);
    }
    
    return 0;
}
