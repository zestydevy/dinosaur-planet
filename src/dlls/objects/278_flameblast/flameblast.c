#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "game/objects/object.h"
#include "sys/math.h"
#include "sys/gfx/model.h"
#include "sys/objhits.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 timer;
} FlameBlast_Setup;

typedef struct {
    s16 timer;
    s16 pad;
    u32 sound;
} FlameBlast_Data;

// offset: 0x0 | ctor
void dll_278_ctor(void* dll){ }

// offset: 0xC | dtor
void dll_278_dtor(void* dll){ }

// offset: 0x18 | func: 0 | export: 0
void flameblast_setup(Object* self, FlameBlast_Setup* setup, s32 arg2) {
    FlameBlast_Data* objdata = self->data;

    objdata->timer = setup->timer;
    func_80026128(self, 0x1A, setup->unk19 ? 3 : 1, 0);
}

// offset: 0x80 | func: 1 | export: 1
void flameblast_control(Object* self) {
    Object* sidekick;
    FlameBlast_Data* objdata;
    SRT sp30;

    sidekick = get_sidekick();
    objdata = self->data;
    if (!sidekick)
        return;

    if (objdata->sound == 0)
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_304_Burning_Loop, 0x7F, &objdata->sound, 0, 0, 0);

    objdata->timer -= (s16) gUpdateRateF;
    if (objdata->timer <= 0) {
        objdata->timer = 30;

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

        rotate_vec3(&sp30, self->speed.f);

        self->srt.transl.x = sidekick->srt.transl.x;
        self->srt.transl.y = sidekick->srt.transl.y;
        self->srt.transl.z = sidekick->srt.transl.z;
        self->srt.transl.x += self->speed.x * gUpdateRateF;
        self->srt.transl.y += self->speed.y * gUpdateRateF;
        self->srt.transl.z += self->speed.z * gUpdateRateF;
    }

    self->positionMirror2.x = self->srt.transl.x;
    self->positionMirror2.y = self->srt.transl.y;
    self->positionMirror2.z = self->srt.transl.z;
    self->srt.transl.x += self->speed.x * gUpdateRateF;
    self->srt.transl.y += self->speed.y * gUpdateRateF;
    self->srt.transl.z += self->speed.z * gUpdateRateF;
}

// offset: 0x274 | func: 2 | export: 2
void flameblast_update(void){ }

// offset: 0x27C | func: 3 | export: 3
void flameblast_print(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s32 visibility) { }

// offset: 0x294 | func: 4 | export: 4
void flameblast_free(Object* self, s32 arg1) {
    FlameBlast_Data* objdata = self->data;

    gDLL_6_AMSFX->vtbl->func_A1C(objdata->sound);
}

// offset: 0x2EC | func: 5 | export: 5
u32 flameblast_get_model_flags(Object* self){
    return MODFLAGS_NONE;
}

// offset: 0x2FC | func: 6 | export: 6
u32 flameblast_get_data_size(Object* self, s32 arg1){
    return sizeof(FlameBlast_Data);
}
