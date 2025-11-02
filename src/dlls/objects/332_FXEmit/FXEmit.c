#include "common.h"
#include "sys/gfx/modgfx.h"
#include "sys/gfx/projgfx.h"

typedef struct {
    f32 unk0;
    f32 unk4;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    u8 _unk10[0x12 - 0x10];
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    u8 unk1C;
} FXEmit_Data;

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s8 unk22;
    s8 unk23;
    s8 unk24;
    s8 unk25;
    s8 unk26;
    s8 unk27;
    u8 unk28;
    u8 unk29;
    s16 unk2A;
} FXEmit_Setup;

static int dll_332_func_6C0(Object *, Object *, AnimObj_Data *, s8);
static void dll_332_func_8E8(Object *self);

// offset: 0x0 | ctor
void dll_332_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_332_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_332_setup(Object *self, FXEmit_Setup *setup, s32 arg2) {
    FXEmit_Data *objdata = (FXEmit_Data*)self->data;

    self->srt.yaw = 0;
    self->animCallback = dll_332_func_6C0;
    objdata->unk0 = (f32) (setup->unk18 * 4);
    objdata->unk8 = (s16) setup->unk19;
    objdata->unkA = (s16) setup->unk1A;
    objdata->unkE = (s16) setup->unk1C;
    self->srt.scale = 0.1f;
    objdata->unk14 = (s16) setup->unk1E;
    objdata->unk16 = (s16) setup->unk20;
    objdata->unk18 = 0;
    if (objdata->unkE <= 0) {
        self->unkDC = (s32) objdata->unkE;
    } else {
        self->unkDC = 0;
    }
    if (objdata->unk16 != -1) {
        if (main_get_bits(objdata->unk16) != 0) {
            objdata->unk18 = 1;
        }
    }
    self->srt.yaw = setup->unk24 << 8;
    self->srt.pitch = setup->unk23 << 8;
    self->srt.roll = setup->unk22 << 8;
    objdata->unk1A = (s16) (setup->unk29 * 0x64);
    objdata->unk4 = (f32) self->srt.transl.x;
    objdata->unk12 = rand_next(0, 0xA);
    objdata->unkC = 0;
}

// offset: 0x174 | func: 1 | export: 1
void dll_332_control(Object* self) {
    FXEmit_Data* objdata;
    f32 sp40[3];
    s32 _pad;
    FXEmit_Setup* setup;
    Object* player;

    objdata = (FXEmit_Data*)self->data;
    setup = (FXEmit_Setup*)self->setup;
    if (objdata->unk12 != 0) {
        objdata->unk12 -= (s16)gUpdateRateF;
        if (objdata->unk12 < 0) {
            objdata->unk12 = 0;
        }
    } else {
        self->srt.transl.x += self->speed.x * gUpdateRateF;
        self->srt.transl.y += self->speed.y * gUpdateRateF;
        self->srt.transl.z += self->speed.z * gUpdateRateF;
        self->positionMirror.x = self->srt.transl.x;
        self->positionMirror.y = self->srt.transl.y;
        self->positionMirror.z = self->srt.transl.z;
        player = get_player();
        if ((player != NULL) && (setup != NULL)) {
            if ((setup->unk29 != 0) && (setup->unk29 != 0xFF)) {
                if (objdata->unk1A <= 0) {
                    objdata->unk18 = 0;
                    objdata->unk1A = (s16) (setup->unk29 * 0x64);
                    if (setup->unk2A != 0) {
                        gDLL_6_AMSFX->vtbl->play_sound(self, setup->unk2A, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                } else {
                    objdata->unk18 = 1;
                }
                objdata->unk1A = (s16) (objdata->unk1A - gUpdateRate);
            }
            if (setup->unk27 == 0x7F) {
                self->srt.yaw += gUpdateRate * 0xA;
            } else {
                self->srt.yaw += setup->unk27 * gUpdateRate * 0x64;
            }
            if (setup->unk26 == 0x7F) {
                self->srt.pitch += gUpdateRate * 0xA;
            } else {
                self->srt.pitch += setup->unk26 * gUpdateRate * 0x64;
            }
            if (setup->unk25 == 0x7F) {
                self->srt.roll += gUpdateRate * 0xA;
            } else {
                self->srt.roll += setup->unk25 * gUpdateRate * 0x64;
            }
            if (((objdata->unk14 == -1) || (main_get_bits(objdata->unk14) != 0)) && (objdata->unk18 == 0)) {
                if (objdata->unk16 != -1) {
                    if (main_get_bits(objdata->unk16) != 0) {
                        objdata->unk18 = 1;
                    }
                }
                if (setup->unk29 == 0xFF) {
                    objdata->unk18 = 1;
                }
                if ((objdata->unkE >= 0) || ((objdata->unkE < 0) && (self->unkDC <= 0))) {
                    sp40[0] = self->positionMirror.x - player->positionMirror.x;
                    sp40[1] = self->positionMirror.y - player->positionMirror.y;
                    sp40[2] = self->positionMirror.z - player->positionMirror.z;
                    if (objdata->unkE == 0) {
                        objdata->unk18 = 1;
                    }
                    if ((sqrtf(SQ(sp40[0]) + SQ(sp40[1]) + SQ(sp40[2])) <= objdata->unk0) || (objdata->unk0 == 0.0f)) {
                        dll_332_func_8E8(self);
                    }
                    self->unkDC = (s32) -objdata->unkE;
                    return;
                }
                if (objdata->unkE < 0) {
                    if (self->unkDC > 0) {
                        self->unkDC -= gUpdateRate;
                    }
                }
            }
        }
    }
}

// offset: 0x608 | func: 2 | export: 2
void dll_332_update(Object *self) { }

// offset: 0x614 | func: 3 | export: 3
void dll_332_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x62C | func: 4 | export: 4
void dll_332_free(Object *self, s32 a1) {
    gDLL_13_Expgfx->vtbl->func5(self);
    gDLL_14_Modgfx->vtbl->func4(self);
}

// offset: 0x69C | func: 5 | export: 5
u32 dll_332_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x6AC | func: 6 | export: 6
u32 dll_332_get_data_size(Object *self, u32 a1) {
    return sizeof(FXEmit_Data);
}

// offset: 0x6C0 | func: 7
static int dll_332_func_6C0(Object *arg0, Object *arg1, AnimObj_Data *arg2, s8 arg3) {
    FXEmit_Data *objdata;
    FXEmit_Setup *setup;
    s32 i;

    objdata = (FXEmit_Data*)arg0->data;
    setup = (FXEmit_Setup*)arg0->setup;
    
    for (i = 0; i < arg2->unk98; i++) {
        if (arg2->unk8E[i] == 1) {
            dll_332_func_8E8(arg0);
        }
        if (arg2->unk8E[i] == 2) {
            objdata->unk1C = (u8) (1 - objdata->unk1C);
        }
        arg2->unk8E[i] = 0;
    }
    if (objdata->unk1C != 0) {
        if (setup->unk27 == 0x7F) {
            arg0->srt.yaw += gUpdateRate * 0xA;
        } else {
            arg0->srt.yaw += setup->unk27 * gUpdateRate * 0x64;
        }
        if (setup->unk26 == 0x7F) {
            arg0->srt.pitch += gUpdateRate * 0xA;
        } else {
            arg0->srt.pitch += setup->unk26 * gUpdateRate * 0x64;
        }
        if (setup->unk25 == 0x7F) {
            arg0->srt.roll += gUpdateRate * 0xA;
        } else {
            arg0->srt.roll += setup->unk25 * gUpdateRate * 0x64;
        }
        dll_332_func_8E8(arg0);
    }
    return 0;
}

// offset: 0x8E8 | func: 8
static void dll_332_func_8E8(Object *self) {
    FXEmit_Data *objdata;
    FXEmit_Setup *setup;
    s32 var_s3;
    DLL_IModgfx *modfxDLL;
    DLL_IProjgfx *projfxDLL;
    s16 i;
    SRT srt;

    objdata = self->data;
    setup = (FXEmit_Setup*)self->setup;
    var_s3 = 0;
    switch (setup->unk28) {
    case 3:
        break;
    default:
        var_s3 = 2;
        break;
    case 0:
        if (objdata->unk8 == 0) {
            var_s3 = 2;
        }
        if (objdata->unk8 == 1) {
            var_s3 = 2;
        }
        if (objdata->unk8 == 2) {
            var_s3 = 2;
        }
        break;
    case 1:
        if (objdata->unk8 == 0) {
            var_s3 = 4;
        }
        if (objdata->unk8 == 1) {
            var_s3 = 4;
        }
        if (objdata->unk8 == 2) {
            var_s3 = 4;
        }
        break;
    case 2:
        if (objdata->unk8 == 0) {
            var_s3 = 0x200001;
        }
        if (objdata->unk8 == 1) {
            var_s3 = 1;
        }
        if (objdata->unk8 == 2) {
            var_s3 = 1;
        }
        break;
    }
    if (var_s3 & 1) {
        srt.transl.x = self->srt.transl.x;
        srt.transl.y = self->srt.transl.y;
        srt.transl.z = self->srt.transl.z;
        srt.yaw = self->srt.yaw;
        srt.roll = self->srt.roll;
        srt.pitch = self->srt.pitch;
        srt.scale = 1.0f;
        if (objdata->unkE > 0) {
            for (i = 0; i < objdata->unkE; i++) {
                gDLL_17_partfx->vtbl->spawn(self, objdata->unkA, &srt, var_s3, -1, NULL);
            }
        } else {
            gDLL_17_partfx->vtbl->spawn(self, objdata->unkC, &srt, var_s3, -1, NULL);
        }
    } else {
        if (objdata->unk8 == 0) {
            if (objdata->unkE > 0) {
                for (i = 0; i < objdata->unkE; i++) {
                    gDLL_17_partfx->vtbl->spawn(self, objdata->unkA, NULL, var_s3, -1, NULL);
                }
            } else {
                gDLL_17_partfx->vtbl->spawn(self, objdata->unkA, NULL, var_s3, -1, NULL);
            }
        } else if (objdata->unk8 == 1) {
            modfxDLL = dll_load_deferred((objdata->unkA + 0x1000), 1);
            if (objdata->unkE > 0) {
                for (i = 0; i < objdata->unkE; i++) {
                    modfxDLL->vtbl->func0(self, 0, 0, var_s3, -1, 0);
                }
            } else {
                modfxDLL->vtbl->func0(self, 0, 0, var_s3, -1, 0);
            }
            dll_unload(modfxDLL);
        } else if (objdata->unk8 == 2) {
            projfxDLL = dll_load_deferred((objdata->unkA + 0x2000), 1);
            if (objdata->unkE > 0) {
                for (i = 0; i < objdata->unkE; i++) {
                    projfxDLL->vtbl->func0(self, 0, 0, var_s3, -1, objdata->unkA, 0);
                }
            } else {
                projfxDLL->vtbl->func0(self, 0, 0, var_s3, -1, objdata->unkA, 0);
            }
            dll_unload(projfxDLL);
        }
    }
}
