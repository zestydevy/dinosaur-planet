#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/rand.h"


typedef struct {
/*00*/ ObjCreateInfo base;
/*18*/ s16 unk18;
/*1A*/ s16 unk1A;
/*1C*/ s16 unk1C;
} SHSporeCreateInfo;

typedef struct {
/*000*/ s8 unk0[0x25C - 0];
/*25C*/ s8 unk25C;
/*25D*/ s8 unk25D;
/*25E*/ s8 unk25E;
/*25F*/ s8 unk25F;
/*260*/ f32 unk260;
/*264*/ f32 unk264;
/*268*/ f32 unk268;
/*26C*/ f32 unk26C;
/*270*/ f32 unk270;
/*274*/ f32 unk274;
/*278*/ f32 unk278;
/*27C*/ f32 unk27C;
/*280*/ f32 unk280;
/*284*/ f32 unk284;
/*288*/ f32 unk288;
/*28C*/ f32 unk28C;
/*290*/ f32 unk290;
/*294*/ s16 unk294;
/*296*/ s16 unk296;
/*298*/ s16 unk298;
/*29A*/ s16 unk29A;
} SHSporeState;

static void SHspore_func_750(Object* self, SHSporeState* state);
static void SHspore_func_A00(Object* arg0, SHSporeState* state);

/*0x0*/ static u32 _data_0[] = { 0, 0, 0 };
/*0xC*/ static f32 _data_C = 5.0f;

// offset: 0x0 | ctor
void SHspore_ctor(void *dll) {
}

// offset: 0xC | dtor
void SHspore_dtor(void *dll) {
}

// offset: 0x18 | func: 0 | export: 0
void SHspore_create(Object* self, s32 arg1, s32 arg2) {
    SHSporeState* state;
    s8 pad[4];
    s8 sp37;

    state = self->state;
    sp37 = 5;
    state->unk260 = 1500.0f;
    self->unk0xb0 |= 0x6000;
    self->speed.y = 3.0f;
    
    func_800267A4(self);
    state->unk298 = rand_next(0, 0xFFFF);
    state->unk26C = rand_next(0, 1000) / 1000.0f;
    
    gDLL_27_HeadTurn->vtbl->head_turn_func_18((void*)state, 0, 0x40002, 1);
    gDLL_27_HeadTurn->vtbl->head_turn_func_c0((void*)state, 1, (s32)&_data_0, (s32)&_data_C, (s32)&sp37);
    gDLL_17->vtbl->func1(self, 0x3F1, NULL, 4, -1, NULL);
}

// offset: 0x18C | func: 1 | export: 1
void SHspore_update(Object* self) {
    SHSporeState* state;
    s8 pad[10];
    s32 particleCount;
    f32 temp;
    f32 temp_fv0;
    s32 var_s0;
    Object* collidedObject;
    s32 var_s0_2;

    state = (SHSporeState*)self->state;
    if (state->unk290 != 0.0f) {
        self->srt.yaw += delayByte << 6;
        state->unk290 -= delayFloat;
        if (state->unk290 <= 0.0f) {
            obj_destroy_object(self);
        }
    } else {
        //Update motion
        state->unk270 -= delayFloat;
        if (state->unk270 < 0.0f) {
            state->unk270 = 0.0f;
        }
        state->unk28C -= delayFloat;
        if (state->unk28C < 0.0f) {
            state->unk28C = 0.0f;
        }
        self->speed.y += -0.009f * delayFloat;
        if (self->speed.y < -0.2f) {
            self->speed.y = -0.2f;
        }
        if (self->speed.y > 0) {
            self->speed.y *= 0.97f;
        }
        if (self->speed.y < 0.0f) {
            func_8002674C(self);
        }
        SHspore_func_750(self, state);
        if ((rand_next(0, 0x64) < 5) && (state->unk270 <= 0.0f)) {
            SHspore_func_A00(self, state);
        }
        state->unk284 -= delayFloat;
        if (state->unk284 <= 0.0f) {
            state->unk27C *= 0.97f;
            state->unk280 *= 0.97f;
            state->unk284 = 0.0f;
        } else {
            temp_fv0 = state->unk288 - state->unk268;
            state->unk268 += temp_fv0 * 0.01f * delayFloat;
        }
        self->speed.x = state->unk274 + (state->unk27C * state->unk268);
        self->speed.z = state->unk278 + (state->unk280 * state->unk268);
        obj_integrate_speed(self, self->speed.x * delayFloat, self->speed.y * delayFloat, self->speed.z * delayFloat);
        gDLL_27_HeadTurn->vtbl->head_turn_func_1e8(self, (void*)state, delayFloat);
        gDLL_27_HeadTurn->vtbl->head_turn_func_5a8(self, (void*)state);
        gDLL_27_HeadTurn->vtbl->head_turn_func_624(self, (void*)state, delayFloat);
        func_80026128(self, 0xA, 0, 0);

        //Handle collisions
        collidedObject = (Object*)self->objhitInfo->unk_0x48;
        if (collidedObject) {
            particleCount = 20;
            if (get_player() == collidedObject) {
                //Player collecting purple mushroom
                increment_gplay_bitstring(0x66C);
                particleCount = 0;
            }
            if (collidedObject->id != OBJ_SHrocketmushroo) {
                //Other objects (ignoring SHrocketmushroom since the spores emerge out of it)
                gDLL_6_AMSFX->vtbl->func2(self, 0xB31, 0x7F, NULL, 0, 0, 0);
                gDLL_13_Expgfx->vtbl->func4.withOneArg((s32)self);
                
                //Create collision particles
                for (var_s0 = 0; var_s0 < particleCount; var_s0++){
                    gDLL_17->vtbl->func1(self, 0x3F3, NULL, 4, -1, NULL);
                }
                
                state->unk290 = 200.0f;
                self->srt.flags |= 0x4000;
                func_800267A4(self);
            }
        } else {
            state->unk260 -= delayFloat;
            if (state->unk260 <= 0.0f || state->unk25C & 0x11) {
                gDLL_6_AMSFX->vtbl->func2(self, 0x8A2, 0x7F, NULL, 0, 0, 0);
                gDLL_13_Expgfx->vtbl->func4.withOneArg((s32)self);
                
                //Create collision particles
                for (var_s0 = 0; var_s0 < 0x14; var_s0++){
                    gDLL_17->vtbl->func1(self, 0x3F3, NULL, 4, -1, NULL);
                }
                
                state->unk290 = 200.0f;
                self->srt.flags |= 0x4000;
                func_800267A4(self);
            }
        }
    }
}

// offset: 0x750 | func: 2
static void SHspore_func_750(Object* self, SHSporeState* state) {
    s8 pad[4];
    SHSporeCreateInfo* createInfo;
    f32 temp_fv1;
    s32 sp20;
    s32 var_v0;

    createInfo = (SHSporeCreateInfo*)self->createInfo;
    sp20 = createInfo->unk1C;
    
    if (rand_next(0, 0x64) < 0xA && state->unk28C <= 0.0f) {
        state->unk298 = rand_next(0x7D0, 0xFA0);
        if (rand_next(0, 1) != 0) {
            state->unk298 = -state->unk298;
        }
        state->unk298 += state->unk294;
        var_v0 = state->unk298 - (sp20 & 0xFFFF);
        if (var_v0 > 0x8000) {
            var_v0 += 0xFFFF0001;
        }
        if (var_v0 < -0x8000) {
            var_v0 += 0xFFFF;
        }

        if (createInfo->unk1A < var_v0) {
            state->unk298 = createInfo->unk1A + sp20;
        }
        if (var_v0 < -createInfo->unk1A) {
            state->unk298 = sp20 - createInfo->unk1A;
        }
        state->unk28C = 150.0f;
    }
    
    if (rand_next(0, 0x64) < 0xA && state->unk28C <= 0.0f) {
        state->unk26C = (rand_next(-0xC8, 0xC8) / 1000.0f) + state->unk264;
        if (state->unk26C < 0.5f) {
            state->unk26C = 0.5f;
        } else if (state->unk26C > 1.0f) {
            state->unk26C = 1.0f;
        }
    }
    
    var_v0 = state->unk298 - (state->unk294 & 0xFFFF);
    if (var_v0 >= 0x8001) {
        var_v0 += 0xFFFF0001;
    }
    if (var_v0 < -0x8000) {
        var_v0 += 0xFFFF;
    }

    state->unk294 += (var_v0 * delayByte) >> 4;
    
    temp_fv1 = state->unk26C - state->unk264;
    state->unk264 += temp_fv1 * 0.006f * delayFloat;
    state->unk274 = fsin16_precise(state->unk294) * state->unk264;
    state->unk278 = fcos16_precise(state->unk294) * state->unk264;
}

// offset: 0xA00 | func: 3
static void SHspore_func_A00(Object* arg0, SHSporeState* state) {
    SHSporeCreateInfo* createInfo;
    s32 sp20;
    s32 var_v1;

    createInfo = (SHSporeCreateInfo*)arg0->createInfo;
    sp20 = createInfo->unk1C;
    state->unk284 = rand_next(0x1E, 0x2D);
    state->unk270 = rand_next(0x78, 0xB4) + state->unk284;
    state->unk296 = rand_next(-0x7D0, 0x7D0) + state->unk294;
    
    var_v1 = state->unk296 - (sp20 & 0xFFFF);
    if (var_v1 >= 0x8001) {
        var_v1 += 0xFFFF0001;
    }
    if (var_v1 < -0x8000) {
        var_v1 += 0xFFFF;
    }
    
    if (createInfo->unk1A < var_v1) {
        state->unk296 = createInfo->unk1A + sp20;
    }
    if (var_v1 < -createInfo->unk1A) {
        state->unk296 = sp20 - createInfo->unk1A;
    }
    
    state->unk288 = rand_next(0x384, 0x514) / 1000.0f;
    state->unk268 = 0.0f;
    state->unk27C = fsin16_precise(state->unk296);
    state->unk280 = fcos16_precise(state->unk296);
}

// offset: 0xB88 | func: 4 | export: 2
void SHspore_func_B88(Object *self) {
}

// offset: 0xB94 | func: 5 | export: 3
void SHspore_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
}

// offset: 0xBAC | func: 6 | export: 4
void SHspore_destroy(Object* arg0, s32 arg1) {
    gDLL_13_Expgfx->vtbl->func4.withOneArg((s32)arg0);
}

// offset: 0xBF4 | func: 7 | export: 5
s32 SHspore_func_BF4(Object *self) {
    return 0;
}

// offset: 0xC04 | func: 8 | export: 6
s32 SHspore_get_state_size(Object *self, s32 arg1) {
    return 0x29C;
}
