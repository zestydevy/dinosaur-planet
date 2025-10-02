#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "dll.h"
#include "functions.h"

typedef struct {
/*0x00*/ ObjCreateInfo base;
/*0x18*/ s8 unk18;
/*0x1A*/ s16 unk1A;
/*0x1C*/ s16 unk1C;
/*0x1E*/ s16 unk1E;
/*0x20*/ s16 unk20;
} DFPLiftCreateInfo;

typedef struct {
/*0x00*/ f32 unk0;
/*0x04*/ u32 unk4;
/*0x08*/ s16 state;
/*0x0A*/ s16 unkA;
/*0x0C*/ s16 unkC;
/*0x0E*/ s16 unkE;
/*0x10*/ u8 unk10;
/*0x11*/ u8 unk11;
} DFPLiftState;

static s32 DFPLift_func_91C(Object *, Object *, void *, void *);

// offset: 0x0 | ctor
void DFPLift_ctor(void *dll) { }

// offset: 0xC | dtor
void DFPLift_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFPLift_create(Object *self, DFPLiftCreateInfo *createInfo, s32 a2) {
    DFPLiftState *state;

    state = (DFPLiftState*)self->state;

    self->unk0xbc = DFPLift_func_91C;
    self->srt.yaw = createInfo->unk18 << 8;

    state->state = 0;
    state->unkA = createInfo->unk20;
    state->unkC = createInfo->unk1E;
    state->unk0 = createInfo->unk1A;
    state->unk10 = createInfo->unk1C;

    self->srt.transl.y -= 1228.0f;
    self->unk0xb0 |= 0x2000;
}

// offset: 0x9C | func: 1 | export: 1
void DFPLift_update(Object* self) {
    DFPLiftCreateInfo* createInfo;
    DFPLiftState* state;
    Object* player;

    createInfo = (DFPLiftCreateInfo*)self->createInfo;
    state = (DFPLiftState*)self->state;
    
    player = get_player();
    if (player == NULL) {
        return;
    }
    
    switch (state->state) {
    case 0:
        if ((get_gplay_bitstring(state->unkA) != 0) && (state->unk10 != 1) && 
                (vec3_distance_xz(&self->positionMirror, &player->positionMirror) < 230.0f)) {
            if (self->srt.transl.y < (createInfo->base.y + 60.0f)) {
                if (state->unk4 == 0) {
                    gDLL_6_AMSFX->vtbl->play_sound(self, 0x6EC, 0x75, &state->unk4, NULL, 0, NULL);
                    state->unk11 = 1;
                }
                self->srt.transl.y += delayFloat;
                if ((createInfo->base.y + 60.0f) <= self->srt.transl.y) {
                    self->srt.transl.y = (createInfo->base.y + 60.0f);
                    state->state = 1;
                    if (state->unk4 != 0) {
                        gDLL_6_AMSFX->vtbl->func_A1C(state->unk4);
                        state->unk4 = 0;
                    }
                }
                return;
            }
        } else if (state->unk10 == 1) {
            if (vec3_distance_xz(&self->positionMirror, &player->positionMirror) < 230.0f) {
                if (self->srt.transl.y < (createInfo->base.y + 60.0f)) {
                    self->srt.transl.y = self->srt.transl.y + delayFloat;
                    if ((createInfo->base.y + 60.0f) <= self->srt.transl.y) {
                        self->srt.transl.y = (createInfo->base.y + 60.0f);
                        state->state = 1;
                    }
                }
            }
        }
        break;
    case 1:
        state->state = 2;
        state->unkE = 0x64;
        break;
    case 2:
        if (state->unkE != 0) {
            state->unkE -= (s16)delayFloat;
            if (state->unkE <= 0) {
                state->unkE = 0;
            }
        } else {
            if (vec3_distance_xz(&self->positionMirror, &player->positionMirror) < 50.0f) {
                if (self->srt.transl.y == (createInfo->base.y + 60.0f)) {
                    state->state = 3;
                    if (state->unk4 == 0) {
                        gDLL_6_AMSFX->vtbl->play_sound(self, 0x6EC, 0x6B, &state->unk4, NULL, 0, NULL);
                        state->unk11 = 1;
                    }
                } else if (self->srt.transl.y == (createInfo->base.y - 1228.0f)) {
                    state->state = 4;
                    if (state->unk4 == 0) {
                        gDLL_6_AMSFX->vtbl->play_sound(self, 0x6EC, 0x43, &state->unk4, NULL, 0, NULL);
                        state->unk11 = 1;
                    }
                }
            } else {
                if (player->srt.transl.y < createInfo->base.y) {
                    state->state = 3;
                    if (state->unk11 == 1) {
                        state->unk11 = 0;
                    }
                } else if (createInfo->base.y < player->srt.transl.y) {
                    state->state = 4;
                    if (state->unk11 == 1) {
                        state->unk11 = 0;
                    }
                }
            }
        }
        break;
    case 3:
        if ((createInfo->base.y - 1228.0f) < self->srt.transl.y) {
            self->srt.transl.y = self->srt.transl.y - delayFloat;
            if (self->srt.transl.y <= (createInfo->base.y - 1228.0f)) {
                self->srt.transl.y = (createInfo->base.y - 1228.0f);
                state->state = 2;
                if (state->unk4 != 0) {
                    gDLL_6_AMSFX->vtbl->func_A1C(state->unk4);
                    state->unk4 = 0;
                }
                state->unkE = 0x64;
            }
            if ((vec3_distance_xz(&self->positionMirror, &player->positionMirror) < 50.0f) && (state->unk11 == 1)) {
                
            }
        } else {
            if (state->unk4 != 0) {
                gDLL_6_AMSFX->vtbl->func_A1C(state->unk4);
                state->unk4 = 0;
            }
            vec3_distance_xz(&self->positionMirror, &player->positionMirror);
            state->state = 2;
            state->unkE = 0x64;
        }
        break;
    case 4:
        if (self->srt.transl.y < (createInfo->base.y + 60.0f)) {
            self->srt.transl.y = self->srt.transl.y + delayFloat;
            if ((createInfo->base.y + 60.0f) <= self->srt.transl.y) {
                self->srt.transl.y = (createInfo->base.y + 60.0f);
                state->state = 2;
                state->unkE = 0x64;
                if (state->unk4 != 0) {
                    gDLL_6_AMSFX->vtbl->func_A1C(state->unk4);
                    state->unk4 = 0;
                }
            }
            if ((vec3_distance_xz(&self->positionMirror, &player->positionMirror) < 50.0f) && (state->unk11 == 1)) {
                
            }
        } else {
            state->state = 2;
            state->unkE = 0x64;
            if (state->unk4 != 0) {
                gDLL_6_AMSFX->vtbl->func_A1C(state->unk4);
                state->unk4 = 0;
            }
            if ((vec3_distance_xz(&self->positionMirror, &player->positionMirror) < 50.0f) && (state->unk11 == 1)) {

            }
        }
        break;
    default:
        break;
    }
}

// offset: 0x7EC | func: 2 | export: 2
void DFPLift_func_7EC(Object *self) { }

// offset: 0x7F8 | func: 3 | export: 3
void DFPLift_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x84C | func: 4 | export: 4
void DFPLift_destroy(Object *self, s32 a1) {
    DFPLiftState *state;

    state = (DFPLiftState*)self->state;

    gDLL_13_Expgfx->vtbl->func5(self);

    if (state->unk4 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(state->unk4);
        state->unk4 = 0;
    }

    func_80000450(self, self, 0, 0, 0, 0);
}

// offset: 0x8F8 | func: 5 | export: 5
u32 DFPLift_func_8F8(Object *self) {
    return 0;
}

// offset: 0x908 | func: 6 | export: 6
u32 DFPLift_get_state_size(Object *self, u32 a1) {
    return sizeof(DFPLiftState);
}

// offset: 0x91C | func: 7
static s32 DFPLift_func_91C(Object *a0, Object *a1, void *a2, void *a3) {
    return 0;
}
