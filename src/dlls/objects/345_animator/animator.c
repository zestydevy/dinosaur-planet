#include "PR/ultratypes.h"
#include "sys/objects.h"
#include "sys/main.h"
#include "sys/rand.h"
#include "dll.h"
#include "game/objects/object.h"

typedef struct {
/*00*/ ObjCreateInfo base;
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
} AnimatorCreateInfo;

// offset: 0x0 | ctor
void animator_ctor(void *dll) {
}

// offset: 0xC | dtor
void animator_dtor(void *dll) {
}

// offset: 0x18 | func: 0 | export: 0
void animator_create(Object *self, AnimatorCreateInfo *createInfo, s32 arg2) {
    self->srt.roll = createInfo->roll << 8;
    self->srt.pitch = createInfo->pitch << 8;
    self->srt.yaw = createInfo->yaw << 8;
    self->unk0xdc = 0;
    self->unk0xb0 |= 0x2000;
}

// offset: 0x58 | func: 1 | export: 1
void animator_update(Object *self) {
    Object *player;
    f32 dz;
    f32 dx;
    f32 dy;
    s16 i;
    AnimatorCreateInfo *createInfo;
    SRT srt;

    createInfo = (AnimatorCreateInfo *)self->createInfo;
    player = get_player();
    if (!player)
        return;

    if (self->unk0xdc <= 0) {
        dx = self->positionMirror.x - player->positionMirror.x;
        dy = self->positionMirror.y - player->positionMirror.y;
        dz = self->positionMirror.z - player->positionMirror.z;

        if (sqrtf(dx*dx + dy*dy + dz*dz) <= createInfo->distance * 0x10 || createInfo->distance == 0) {

            for (i = 0; i < createInfo->iterations; i++){
                srt.transl.x = rand_next(-createInfo->amplitudeX, createInfo->amplitudeX);
                srt.transl.y = rand_next(-createInfo->amplitudeY, createInfo->amplitudeY);
                srt.transl.z = rand_next(-createInfo->amplitudeZ, createInfo->amplitudeZ);

                if (createInfo->unk23 & 1) {
                    gDLL_17->vtbl->func1(self, 0x320, &srt, 4, -1, 0);
                }
                if (createInfo->unk23 & 2) {
                    gDLL_17->vtbl->func1(self, 0x321, &srt, 4, -1, 0);
                }
                if (createInfo->unk23 & 4) {
                    gDLL_17->vtbl->func1(self, 0x322, &srt, 4, -1, 0);
                }
                if (createInfo->unk23 & 8) {
                    gDLL_17->vtbl->func1(self, 0x351, &srt, 4, -1, 0);
                }
            }

        }
        self->unk0xdc = -createInfo->iterations;
    } else if (self->unk0xdc > 0) {
        self->unk0xdc -= delayByte;
    }
}

// offset: 0x300 | func: 2 | export: 2
void animator_func_300(Object *self) {
}

// offset: 0x30C | func: 3 | export: 3
void animator_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
}

// offset: 0x324 | func: 4 | export: 4
void animator_destroy(Object *self, s32 arg1) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x36C | func: 5 | export: 5
u32 animator_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x37C | func: 6 | export: 6
u32 animator_get_state_size(Object *self, s32 arg1) {
    return 0;
}
