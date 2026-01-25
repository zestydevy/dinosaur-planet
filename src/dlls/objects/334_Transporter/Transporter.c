#include "dll.h"
#include "sys/dll.h"
#include "sys/main.h"
#include "sys/rand.h"
#include "sys/joypad.h"
#include "functions.h"
#include "game/gamebits.h"
#include "dlls/engine/17_partfx.h"
#include "dlls/modgfx/129.h"
#include "dlls/modgfx/130.h"
#include "dlls/modgfx/140.h"

typedef struct {
/*00*/ void *dll129;
/*04*/ void *dll130;
/*08*/ void *dll140;
/*0C*/ f32 unkC;
/*10*/ s16 unk10;
/*12*/ s16 unk12;
/*14*/ s16 unk14;
/*16*/ s16 unk16;
/*18*/ s16 unk18;
/*1A*/ s16 unk1A;
/*1C*/ s16 unk1C;
/*1E*/ s16 unk1E;
/*20*/ s16 unk20;
/*22*/ s16 unk22;
/*24*/ u8 unk24;
/*25*/ u8 unk25;
/*26*/ u8 unk26;
} Transporter_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 unk19;
/*1A*/ s8 warpID;
/*1B*/ u8 unk1B[0x20-0x1B];
/*20*/ s16 gamebit;
} Transporter_Setup;

static int Transporter_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);

// offset: 0x0 | ctor
void Transporter_ctor(void *dll) { }

// offset: 0xC | dtor
void Transporter_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void Transporter_setup(Object *self, Transporter_Setup *setup, s32 arg2) {
    Transporter_Data *objdata;

    objdata = self->data;
    self->srt.yaw = 0;
    objdata->dll129 = NULL;
    objdata->dll130 = NULL;
    objdata->dll140 = NULL;
    objdata->unk10 = 600;
    objdata->unk12 = 600;
    objdata->unk14 = 10;
    objdata->unk16 = 0;
    objdata->unk18 = 0;
    objdata->unk1A = 10;
    objdata->unk1C = 400;
    objdata->unk20 = 0;
    objdata->unk1E = 2;
    objdata->unkC = 0.01f;
    self->srt.yaw = (s16)(s32)(f32)(setup->yaw << 8); // ?
    self->unkDC = 0;
    self->unkE0 = 150;
    self->animCallback = Transporter_anim_callback;
}

// offset: 0xBC | func: 1 | export: 1
void Transporter_control(Object *self) {
    Camera *camera;
    s16 i;
    Object *player;
    Vec3f delta;
    f32 distToPlayer;
    f32 mag1;
    SRT transform;
    Transporter_Setup *setup;
    Transporter_Data *objdata;

    objdata = self->data;
    setup = (Transporter_Setup*)self->setup;
    if ((self->unkAF & 4) && !main_get_bits(BIT_Shown_ZLock_Interact_Message)) {
        gDLL_3_Animation->vtbl->func17(2, self, -1);
        main_set_bits(BIT_Shown_ZLock_Interact_Message, 1);
        return;
    }
    func_800013BC();
    player = get_player();
    if (player) {
        distToPlayer = vec3_distance_xz(&player->positionMirror, &self->positionMirror);
        if ((objdata->unk25 == 0) && (objdata->unk24 == 0) && (distToPlayer < 40.0f)) {
            if (!objdata->dll140) {
                objdata->dll140 = dll_load_deferred(DLL_ID_140, 1);
            }
            if (D_800B4A5E >= 0) {
                objdata->unk25 = 0;
                player->opacity = 0;
                gDLL_3_Animation->vtbl->func17(1, self, -1);
                self->unkDC = objdata->unk1C;
                if (!objdata->dll129) {
                    objdata->dll129 = dll_load_deferred(DLL_ID_129, 1);
                }
                if (!objdata->dll130) {
                    objdata->dll130 = dll_load_deferred(DLL_ID_130, 1);
                }
                objdata->unk24 = 1;
                D_80092A78 = 2;
                objdata->unk26 = 0;
            } else {
                if (((setup->gamebit == -1) || (main_get_bits(setup->gamebit))) && (self->unkAF & 1)) {
                    joy_set_button_mask(0, A_BUTTON);
                    func_80000860(self, self, 187, 0);
                    gDLL_3_Animation->vtbl->func17(0, self, -1);
                    objdata->unk25 = 1;
                    self->unkDC = objdata->unk1C;
                    if (!objdata->dll129) {
                        objdata->dll129 = dll_load_deferred(DLL_ID_129, 1);
                    }
                    if (!objdata->dll130) {
                        objdata->dll130 = dll_load_deferred(DLL_ID_130, 1);
                    }
                    objdata->unk24 = 1;
                    objdata->unk26 = 1;
                }
            }
            if (((setup->gamebit == -1) || (main_get_bits(setup->gamebit))) && (rand_next(0, 40) == 0)) {
                ((DLL_140*)objdata->dll140)->vtbl->base.func0(self, rand_next(0, 5), NULL, 1, -1, NULL);
            }
        }
        if ((objdata->unk24 != 0) && (self->unkDC > 0)) {
            objdata->unk16 -= gUpdateRate;
            objdata->unk18 -= gUpdateRate;
            if (objdata->unk16 <= 0) {
                ((DLL_129*)objdata->dll129)->vtbl->base.func0(self, 0, NULL, 1, -1, NULL);
                objdata->unk16 = objdata->unk10;
            }
            if ((objdata->unk25 != 0) && (objdata->unk18 <= 0)) {
                ((DLL_130*)objdata->dll130)->vtbl->base.func0(self, 0, NULL, 1, -1, NULL);
                objdata->unk18 = objdata->unk12;
            }
            camera = get_camera();
            delta.f[0] = self->positionMirror.x - camera->tx;
            delta.f[1] = self->positionMirror.y - camera->ty;
            delta.f[2] = self->positionMirror.z - camera->tz;
            mag1 = VECTOR_MAGNITUDE(delta);
            if (mag1 != 0.0f) {
                delta.f[0] /= mag1; delta.f[0] *= 20.0f;
                delta.f[1] /= mag1; delta.f[1] *= 20.0f;
                delta.f[2] /= mag1; delta.f[2] *= 20.0f;
            }
            transform.transl.x = player->positionMirror.x;
            transform.transl.y = player->positionMirror.y;
            transform.transl.z = player->positionMirror.z;
            transform.yaw = 0;
            transform.roll = 0;
            transform.pitch = 0;
            transform.scale = 1.0f;
            if (self->unkE0 <= 0) {
                if (objdata->unk25 != 0) {
                    for (i = 0; i < objdata->unk14; i++) {
                        gDLL_17_partfx->vtbl->spawn(player, PARTICLE_79, NULL, PARTFXFLAG_4, -1, NULL);
                    }
                    func_80003B70(rand_next(0, 10) * 0.1f);
                } else if ((objdata->unk25 == 0) && (self->unkDC < 200)) {
                    func_80003B70(rand_next(0, 10) * 0.05f);
                }
                transform.transl.x = delta.f[0] + self->positionMirror.x;
                transform.transl.y = delta.f[1] + self->positionMirror.y;
                transform.transl.z = delta.f[2] + self->positionMirror.z;
                if ((self->unkDC % 10) < (s16)(s32)gUpdateRateF) {
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_78, &transform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                }
            } else {
                self->unkE0 -= gUpdateRate;
                if ((self->unkE0 <= 0) && (objdata->unk25 == 0)) {
                    transform.transl.x = player->positionMirror.x;
                    transform.transl.y = player->positionMirror.y;
                    transform.transl.z = player->positionMirror.z;
                    transform.scale = 1.0f;
                    transform.yaw = 0;
                    transform.roll = 0;
                    transform.pitch = 0;
                    func_80003B70(8.0f);
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_43D_Transporter_Fire, MAX_VOLUME, NULL, NULL, 0, NULL);
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_75, NULL, PARTFXFLAG_2, -1, NULL);
                }
            }
            self->unkDC -= gUpdateRate;
        } else if ((objdata->unk24 != 0) && (self->unkDC <= 0)) {
            transform.transl.x = player->positionMirror.x;
            transform.transl.y = player->positionMirror.y;
            transform.transl.z = player->positionMirror.z;
            transform.yaw = 0;
            transform.roll = 0;
            transform.pitch = 0;
            transform.scale = 1.0f;
            if (objdata->unk25 != 0) {
                func_80003B70(8.0f);
                gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_43D_Transporter_Fire, MAX_VOLUME, NULL, NULL, 0, NULL);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_75, NULL, PARTFXFLAG_2, -1, NULL);
                for (i = 0; i < 80; i++) {
                    gDLL_17_partfx->vtbl->spawn(player, PARTICLE_77, NULL, PARTFXFLAG_NONE, -1, NULL);
                }
            } else {
                gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_9B_Transporter_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
                for (i = 0; i < 100; i++) {
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_7A, NULL, PARTFXFLAG_NONE, -1, NULL);
                }
            }
            dll_unload(objdata->dll129);
            objdata->dll129 = NULL;
            dll_unload(objdata->dll130);
            objdata->dll130 = NULL;
            self->unkDC = 0;
            self->unkE0 = 150;
            objdata->unk16 = 0;
            objdata->unk18 = 0;
            objdata->unk1A = 10;
            objdata->unk1E = 2;
            objdata->unkC = 0.01f;
            objdata->unk24 = 0;
            objdata->unk20 = 550;
            objdata->unk22 = 550;
        }
        if ((objdata->unk22 != 0) && (objdata->unk26 != 0)) {
            objdata->unk22 -= gUpdateRate;
            if (objdata->unk22 <= 0) {
                func_80000860(self, self, D_80092A7C[0], 0);
                objdata->unk22 = 0;
            }
        }
        if ((objdata->unk20 != 0) && (objdata->unk26 != 0)) {
            objdata->unk20 -= gUpdateRate;
            if (objdata->unk20 <= 0) {
                warpPlayer(setup->warpID, FALSE);
                objdata->unk26 = 0;
                objdata->unk20 = 0;
            }
        }
        if (player && player->linkedObject && player->opacity < OBJECT_OPACITY_MAX) {
            player->linkedObject->opacity = 0;
        } else if (player && player->linkedObject) {
            player->linkedObject->opacity = OBJECT_OPACITY_MAX;
        }
        self->srt.yaw += 0x3E8;
    }
}

// offset: 0xC00 | func: 2 | export: 2
void Transporter_update(Object *self) {
    Transporter_Setup *setup;

    setup = (Transporter_Setup*)self->setup;
    if (setup->gamebit != -1 && !main_get_bits(setup->gamebit)) {
        self->unkAF |= 8;
    } else {
        self->unkAF &= ~8;
    }
    if (self->unk74) {
        func_80036438(self);
    }
}

// offset: 0xCA0 | func: 3 | export: 3
void Transporter_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0xCB8 | func: 4 | export: 4
void Transporter_free(Object *self, s32 a1) {
    Transporter_Data *objdata;

    objdata = self->data;
    gDLL_13_Expgfx->vtbl->func5(self);
    if (objdata->dll129) {
        dll_unload(objdata->dll129);
    }
    if (objdata->dll130) {
        dll_unload(objdata->dll130);
    }
    if (objdata->dll140) {
        dll_unload(objdata->dll140);
    }
}

// offset: 0xD68 | func: 5 | export: 5
u32 Transporter_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xD78 | func: 6 | export: 6
u32 Transporter_get_data_size(Object *self, u32 a1) {
    return sizeof(Transporter_Data);
}

// offset: 0xD8C | func: 7
int Transporter_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    return 0;
}
