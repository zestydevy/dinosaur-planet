#include "PR/ultratypes.h"
#include "dll.h"
#include "game/objects/object.h"
#include "sys/objhits.h"
#include "sys/main.h"
#include "sys/objects.h"

typedef struct {
    ObjCreateInfo base;
    s8 unk18;
    s8 unk19;
    s16 timer;
} FlameBlastCreateInfo;

typedef struct {
    s16 timer;
    s16 pad;
    u32 sound;
} FlameBlastState;

// offset: 0x0 | ctor
void dll_278_ctor(void* dll){
}

// offset: 0xC | dtor
void dll_278_dtor(void* dll){
}

// offset: 0x18 | func: 0 | export: 0
void flameblast_create(Object* self, FlameBlastCreateInfo* createInfo, s32 arg2) {
    FlameBlastState* state = self->state;

    state->timer = createInfo->timer;
    func_80026128(self, 0x1A, createInfo->unk19 ? 3 : 1, 0);
}

// offset: 0x80 | func: 1 | export: 1
void flameblast_update(Object* self) {
    Object* sidekick;
    FlameBlastState* state;
    SRT sp30;

    sidekick = get_sidekick();
    state = self->state;
    if (!sidekick)
        return;

    if (state->sound == 0)
        gDLL_6_AMSFX->vtbl->play_sound(self, 0x304, 0x7F, &state->sound, 0, 0, 0);

    state->timer -= (s16) delayFloat;
    if (state->timer <= 0) {
        state->timer = 30;

        self->speed.x = 0.0f;
        self->speed.y = 0.0f;
        self->speed.z = -2.0f;

        sp30.transl.x = 0.0f;
        sp30.transl.y = 0.0f;
        sp30.transl.z = 0.0f;
        sp30.scale = 1.0f;
        sp30.roll = sidekick->srt.roll;
        sp30.pitch = sidekick->srt.pitch;
        sp30.yaw = sidekick->srt.yaw;

        rotate_vec3(&sp30, &self->speed);

        self->srt.transl.x = sidekick->srt.transl.x;
        self->srt.transl.y = sidekick->srt.transl.y;
        self->srt.transl.z = sidekick->srt.transl.z;
        self->srt.transl.x += self->speed.x * delayFloat;
        self->srt.transl.y += self->speed.y * delayFloat;
        self->srt.transl.z += self->speed.z * delayFloat;
    }

    self->positionMirror2.x = self->srt.transl.x;
    self->positionMirror2.y = self->srt.transl.y;
    self->positionMirror2.z = self->srt.transl.z;
    self->srt.transl.x += self->speed.x * delayFloat;
    self->srt.transl.y += self->speed.y * delayFloat;
    self->srt.transl.z += self->speed.z * delayFloat;
}

// offset: 0x274 | func: 2 | export: 2
void flameblast_func_274(void){
}

// offset: 0x27C | func: 3 | export: 3
void flameblast_draw(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s32 visibility) {
}

// offset: 0x294 | func: 4 | export: 4
void flameblast_destroy(Object* self, s32 arg1) {
    FlameBlastState* state = self->state;

    gDLL_6_AMSFX->vtbl->func_A1C(state->sound);
}

// offset: 0x2EC | func: 5 | export: 5
u32 flameblast_get_model_flags(Object* self){
    return MODFLAGS_NONE;
}

// offset: 0x2FC | func: 6 | export: 6
u32 flameblast_get_state_size(Object* self, s32 arg1){
    return sizeof(FlameBlastState);
}
