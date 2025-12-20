#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "sys/gfx/model.h"
#include "sys/objects.h"
#include "sys/main.h"
#include "sys/rand.h"
#include "dll.h"
#include "game/objects/object.h"
#include "types.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 unk18;
/*1A*/ s8 roll;
/*1B*/ s8 pitch;
/*1C*/ s8 yaw;
/*1D*/ u8 amplitudeX;
/*1E*/ u8 amplitudeZ;
/*1F*/ u8 amplitudeY;
/*20*/ u8 distance;
/*21*/ u8 unk21;
/*22*/ s8 unk22;
/*23*/ u8 unk23;
/*24*/ u8 iterations;
} WaterFallSpray_Setup;

// offset: 0x0 | ctor
void WaterFallSpray_ctor(void *dll) { }

// offset: 0xC | dtor
void WaterFallSpray_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WaterFallSpray_setup(Object *self, WaterFallSpray_Setup *setup, s32 arg2) {
    self->srt.roll = setup->roll << 8;
    self->srt.pitch = setup->pitch << 8;
    self->srt.yaw = setup->yaw << 8;
    self->unkDC = 0;
    self->unkB0 |= 0x2000;
}

// offset: 0x58 | func: 1 | export: 1
void WaterFallSpray_control(Object *self) {
    Object *player;
    f32 dz;
    f32 dx;
    f32 dy;
    s16 i;
    WaterFallSpray_Setup *setup;
    SRT srt;

    setup = (WaterFallSpray_Setup *)self->setup;
    player = get_player();
    if (!player)
        return;

    if (self->unkDC <= 0) {
        dx = self->positionMirror.x - player->positionMirror.x;
        dy = self->positionMirror.y - player->positionMirror.y;
        dz = self->positionMirror.z - player->positionMirror.z;

        if (sqrtf(dx*dx + dy*dy + dz*dz) <= setup->distance * 0x10 || setup->distance == 0) {

            for (i = 0; i < setup->iterations; i++){
                srt.transl.x = rand_next(-setup->amplitudeX, setup->amplitudeX);
                srt.transl.y = rand_next(-setup->amplitudeY, setup->amplitudeY);
                srt.transl.z = rand_next(-setup->amplitudeZ, setup->amplitudeZ);

                if (setup->unk23 & 1) {
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_320, &srt, PARTFXFLAG_4, -1, NULL);
                }
                if (setup->unk23 & 2) {
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_321, &srt, PARTFXFLAG_4, -1, NULL);
                }
                if (setup->unk23 & 4) {
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_322, &srt, PARTFXFLAG_4, -1, NULL);
                }
                if (setup->unk23 & 8) {
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_351, &srt, PARTFXFLAG_4, -1, NULL);
                }
            }

        }
        self->unkDC = -setup->iterations;
    } else if (self->unkDC > 0) {
        self->unkDC -= gUpdateRate;
    }
}

// offset: 0x300 | func: 2 | export: 2
void WaterFallSpray_update(Object *self) { }

// offset: 0x30C | func: 3 | export: 3
void WaterFallSpray_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x324 | func: 4 | export: 4
void WaterFallSpray_free(Object *self, s32 arg1) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x36C | func: 5 | export: 5
u32 WaterFallSpray_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x37C | func: 6 | export: 6
u32 WaterFallSpray_get_data_size(Object *self, s32 arg1) {
    return 0;
}
