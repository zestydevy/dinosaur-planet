#include "dlls/objects/214_animobj.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "functions.h"
#include "dll.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    s16 unk22;
} CFMainSlideDoor_Setup;

typedef struct {
    u8 unk0;
} CFMainSlideDoor_State;

static int CFMainSlideDoor_func_268(Object* a0, Object* a1, AnimObjState* a2, void* a3);

// offset: 0x0 | ctor
void CFMainSlideDoor_ctor(void *dll) { }

// offset: 0xC | dtor
void CFMainSlideDoor_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFMainSlideDoor_create(Object* self, CFMainSlideDoor_Setup* setup, s32 arg2) {
    CFMainSlideDoor_State* state;

    self->unk0xdc = 0;
    self->srt.yaw = setup->unk1F << 8;
    self->unk0xbc = (ObjectCallback)CFMainSlideDoor_func_268;
    self->srt.scale = setup->unk21 * 0.015625f;
    self->srt.scale *= self->def->scale;
    state = (CFMainSlideDoor_State*)self->state;
    state->unk0 = vec3_distance_xz(&self->positionMirror, &get_player()->positionMirror) < 130.0f;
}

// offset: 0x108 | func: 1 | export: 1
void CFMainSlideDoor_update(Object* self) {
    CFMainSlideDoor_State* state;
    CFMainSlideDoor_Setup* setup;

    if (self->unk0xdc == 0) {
        setup = (CFMainSlideDoor_Setup*)self->setup;
        state = (CFMainSlideDoor_State*)self->state;
        if ((setup->unk1C != 0) && (state->unk0 != 0)) {
            gDLL_3_Animation->vtbl->func20(self, setup->unk1C);
        }
        if (setup->unk1E != -1) {
            gDLL_3_Animation->vtbl->func17(setup->unk1E, self, -1);
        }
        self->unk0xdc = 1;
    }
}

// offset: 0x1D4 | func: 2 | export: 2
void CFMainSlideDoor_func_1D4(Object *self) { }

// offset: 0x1E0 | func: 3 | export: 3
void CFMainSlideDoor_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x234 | func: 4 | export: 4
void CFMainSlideDoor_destroy(Object *self, s32 a1) { }

// offset: 0x244 | func: 5 | export: 5
u32 CFMainSlideDoor_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x254 | func: 6 | export: 6
u32 CFMainSlideDoor_get_state_size(Object *self, u32 a1) {
    return sizeof(CFMainSlideDoor_State);
}

// offset: 0x268 | func: 7
int CFMainSlideDoor_func_268(Object* a0, Object* a1, AnimObjState* a2, void* a3) {
    CFMainSlideDoor_State* state;
    CFMainSlideDoor_Setup* setup;
    Object* player;
    Object* sidekick;
    s32 var_t6;
    s32 var_t7;

    player = get_player();
    sidekick = get_sidekick();
    if (player != NULL) {
        var_t6 = vec3_distance_xz(&a0->positionMirror, &player->positionMirror) < 130.0f;
    } else {
        var_t6 = 0;
    }
    if (sidekick != NULL) {
        var_t7 = vec3_distance_xz(&a0->positionMirror, &sidekick->positionMirror) < 130.0f;
    } else {
        var_t7 = 0;
    }
    state = (CFMainSlideDoor_State*)a0->state;
    setup = (CFMainSlideDoor_Setup*)a0->setup;
    if (state->unk0 == 0) {
        if (main_get_bits(setup->unk18) != 0) {
            if (setup->unk22 == -1 || main_get_bits(setup->unk22) != 0) {
                main_set_bits(setup->unk1A, 1);
                if (var_t6 != 0 || var_t7 != 0) {
                    state->unk0 = 2;
                }
            }
        }
    } else if (state->unk0 == 1) {
        if (main_get_bits(setup->unk18) != 0 || (setup->unk22 != -1 && main_get_bits(setup->unk22) != 0)) {
            if ((var_t6 == 0) && (var_t7 == 0)) {
                state->unk0 = 3;
            }
        }
    }
    if (state->unk0 == 2) {
        if (a2->unk8D == 2) {
            state->unk0 = 1;
        }
    } else if (state->unk0 == 3) {
        if (a2->unk8D == 1) {
            state->unk0 = 0;
        }
    }
    
    return !(state->unk0 == 2) && !(state->unk0 == 3);
}
