#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "functions.h"
#include "game/objects/object.h"
#include "sys/camera.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "types.h"

/*0x0*/ static s32 impactSoundID;
/*0x4*/ static u32 _bss_4;
/*0x8*/ static u8 _bss_8[0x8];

// offset: 0x0 | ctor
void kamerian_flame_ctor(void* dll){ }

// offset: 0xC | dtor
void kamerian_flame_dtor(void* dll){ }

// offset: 0x18 | func: 0 | export: 0
void kamerian_flame_setup(Object* self, s32 arg1, s32 arg2) {
    ObjectStruct64* temp_v0;
    ObjectStruct64* temp_v0_2;

    self->unk36 = 0xFF;
    func_800267A4(self);
    temp_v0 = self->ptr0x64;
    if (self->ptr0x64) {
        temp_v0->flags |= 0x810;
        temp_v0_2 = self->ptr0x64;
        temp_v0_2->unk2c = temp_v0_2->unk0 * 2.5f;
    }

    gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9B1_Flames_Venting, MAX_VOLUME, NULL, 0, 0, 0);
    if (_bss_4 == 0) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_9B2_Fire_Burning, MAX_VOLUME, (u32*)&_bss_4, 0, 0, 0);
    }
}

// offset: 0x118 | func: 1
static void kamerian_flame_create_flame_billboards(Object* self) {
    Vec3f delta;
    SRT transform;

    transform.transl.x = 0.0f;
    transform.transl.y = 0.0f;
    transform.transl.z = 0.0f;
    transform.roll = 0;
    transform.pitch = 0;
    transform.yaw = 0;
    transform.scale = 0.003f;

    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_680, &transform, PARTFXFLAG_1, -1, NULL);

    delta.x = self->srt.transl.x - self->positionMirror2.x;
    delta.y = self->srt.transl.y - self->positionMirror2.y;
    delta.z = self->srt.transl.z - self->positionMirror2.z;

    transform.transl.x = delta.x / 3.0f;
    transform.transl.y = delta.y / 3.0f;
    transform.transl.z = delta.z / 3.0f;

    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_680, &transform, PARTFXFLAG_1, -1, NULL);

    transform.transl.x *= 4.0f;
    transform.transl.y *= 4.0f;
    transform.transl.z *= 4.0f;

    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_680, &transform, PARTFXFLAG_1, -1, NULL);
}

// offset: 0x280 | func: 2 | export: 1
void kamerian_flame_control(Object* self) {
    s32 dOpacity;
    s32 particleIndex;
    u8 temp_v0;

    dOpacity = gUpdateRate * 3;
    if (dOpacity >= self->unk36) {
        //Destroy if faded out
        self->unk36 = 0;
        obj_destroy_object(self);
        return;
    }

    //Fade out if not fully opaque
    if (self->unk36 != 255) {
        self->unk36 -= dOpacity;
        return;
    }

    self->speed.y += -0.046f * gUpdateRateF;
    self->srt.yaw = arctan2_f(self->speed.x, self->speed.z);
    self->srt.pitch = arctan2_f(sqrtf((self->speed.x * self->speed.x) + (self->speed.z * self->speed.z)), self->speed.y) - 0x4000;
    obj_integrate_speed(self, self->speed.x * gUpdateRateF, self->speed.y * gUpdateRateF, self->speed.z * gUpdateRateF);
    func_80026128(self, 0xA, 1, 0);
    func_80026940(self, 0x10);
    func_8002674C(self);

    //Handle object collisions
    if (self->objhitInfo->unk48 && 
        (((s32)get_player()) == self->objhitInfo->unk48 || ((s32)get_sidekick()) == self->objhitInfo->unk48)) {

        func_800013BC();
        func_80003B70(1.0f);

        //Create particles (@bug: particles don't seem to show up?)
        impactSoundID = SOUND_9B5_Explosion;
        particleIndex = 25;
        while (particleIndex--) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_343, NULL, PARTFXFLAG_1, -1, NULL);
        }

        //Hide fireball
        self->unk36 = 0;
        func_800267A4(self);
    } else {
        kamerian_flame_create_flame_billboards(self);
    }
}

// offset: 0x518 | func: 3 | export: 2
void kamerian_flame_update(Object* self) {
    ObjectHitInfo* objHitInfo;

    objHitInfo = self->objhitInfo;
    if (objHitInfo->unk9D != 0) {
        self->srt.transl.x = objHitInfo->unk34;
        self->srt.transl.y = objHitInfo->unk38;
        self->srt.transl.z = objHitInfo->unk3C;
        self->speed.x = 0.0f;
        self->speed.y = 0.0f;
        self->speed.z = 0.0f;
        self->unk36 = 0xFE;
        func_800267A4(self);
        impactSoundID = SOUND_9B4_Flamethrower;
    }
}

// offset: 0x59C | func: 4 | export: 3
void kamerian_flame_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility){
    //no 3D model since it uses billboard particles
}

// offset: 0x5B4 | func: 5 | export: 4
void kamerian_flame_free(Object* self, s32 arg1) {
    if (_bss_4 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(_bss_4);
        _bss_4 = 0;
    }
    gDLL_6_AMSFX->vtbl->play_sound(self, impactSoundID, MAX_VOLUME, NULL, 0, 0, 0);
    func_800013D0();
}

// offset: 0x668 | func: 6 | export: 5
u32 kamerian_flame_get_model_flags(Object* self){
    return MODFLAGS_NONE;
}

// offset: 0x678 | func: 7 | export: 6
u32 kamerian_flame_get_data_size(Object* self, s32 arg1){
    return 0;
}
