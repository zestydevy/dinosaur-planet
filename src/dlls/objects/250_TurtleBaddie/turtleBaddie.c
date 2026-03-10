#include "common.h"
#include "sys/objanim.h"
#include "sys/objtype.h"

typedef struct {
/*0x00*/UnkCurvesStruct* unk0;
/*0x04*/Object* player;
/*0x08*/f32 unk8;
/*0x0C*/f32 unkC;
/*0x10*/f32 unk10;
/*0x14*/f32 unk14;
/*0x18*/f32 unk18;
/*0x1C*/f32 unk1C;
/*0x20*/f32 unk20;
/*0x24*/s32 unk24;
/*0x28*/u16 unk28;
/*0x2A*/u16 unk2A;
/*0x2C*/u16 unk2C;
/*0x2E*/ u8 flags;
}DLL250_Data;

typedef struct {
 ObjSetup base;
       s8 unk18;
       s8 unk19;
      s16 unk1A;
      s16 unk1C;
      s16 _unk1E; 
       u8 unk20;
}DLL250_Setup;

/*0x0*/ static s32 _data_0[] = {
    0x00000002, 0x00000003, 0x00000000, 0x00000000
};

/*0x0*/ static s32 _bss_0;

static void dll_250_func_A5C(Object* self, DLL250_Data* objdata);

// offset: 0x0 | ctor
void dll_250_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_250_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_250_setup(Object* self, DLL250_Setup* setup, s32 arg2) {
    UnkCurvesStruct* temp_v0;
    DLL250_Data* objdata;

    objdata = self->data;
    objdata->unk8 = (f32) ((f32) setup->unk1A / 100.0f);
    objdata->unkC = 0.005f;
    objdata->unk20 = (f32) ((f32) setup->unk19 * 4.0f);
    self->unkAF &= ~8;
    if (arg2 == 0) {
        objdata->unk0 = mmAlloc(0x108, 0x1A, NULL);
        if (objdata->unk0 != NULL) {
            bzero(objdata->unk0, 0x108);
        }
        if (gDLL_26_Curves->vtbl->func_4288(objdata->unk0, self, objdata->unk20, _data_0, -1) == 0) {
            objdata->flags = (u8) (objdata->flags | 1);
        }
        objdata->unk24 = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B74_Gentle_Magic_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    self->unkB0 |= 0x2000;
}

// offset: 0x178 | func: 1 | export: 1
void dll_250_control(Object* self) {
    DLL250_Data* objdata;
    UnkCurvesStruct* sp68;
    DLL250_Setup* setup;
    Object* sp60;
    Vec3f sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    s32 _pad;
    s32 sp40;
    s32 sp3C;

    objdata = self->data;
    sp68 = objdata->unk0;
    setup = (DLL250_Setup*)self->setup;
    if (self->unkDC != 0) {
        if (gDLL_29_Gplay->vtbl->did_time_expire(setup->base.uID) != 0) {
            self->unkDC = 0;
            self->opacity = 0xFF;
            self->unkAF &= 0xFFF7;
            objdata->flags = (u8) (objdata->flags | 8);
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B20_Low_Grunt, MAX_VOLUME, NULL, NULL, 0, NULL);
            objdata->unk24 = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B74_Gentle_Magic_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    } else {
        if (objdata->unk18 > 0.0f) {
            objdata->unk18 = (f32) (objdata->unk18 + (gUpdateRateF * 0.01f));
            if (objdata->unk18 > 3.0f) {
                self->unkDC = 1;
                objdata->unk18 = 0.0f;
            } else if (objdata->unk18 > 2.0f) {
                objdata->flags = (u8) (objdata->flags & 0xFFCF);
            } else {
                self->opacity = (u8) (u32) ((2.0f - objdata->unk18) * 255.0f);
            }
            func_800267A4(self);
        } else {
            if (func_8002601C(self, &sp60, &sp40, &sp3C, &sp50, &sp4C, &sp48) != 0) {
                if (objdata->unk24 != 0) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk24);
                    objdata->unk24 = 0U;
                }
                objdata->flags = (u8) (objdata->flags | 0x30);
                objdata->unk18 = 1.0f;
                self->unkAF |= 8;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B21_Dissipating_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B1F_Slow_Magic_Chimes, MAX_VOLUME, NULL, NULL, 0, NULL);
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B75_Water_Splash_Big, MAX_VOLUME, NULL, NULL, 0, NULL);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_331, NULL, 2, -1, NULL);
                gDLL_33_BaddieControl->vtbl->func18(self, (s32) setup->unk18, -1, 0U);
                gDLL_29_Gplay->vtbl->add_time(setup->base.uID, (f32) (setup->unk1C * 0x3C));
            }
            func_80026128(self, 0xA, 1, 0);
            func_8002674C(self);
            objdata->unk1C = (f32) (objdata->unk1C - gUpdateRateF);
            if (objdata->unk1C < 0.0f) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_Green_Slime_Drop, NULL, 2, -1, NULL);
                objdata->unk1C = (f32) rand_next(0x1E, 0x78);
            }
        }
        objdata->player = get_player();
        if (objdata->player != NULL) {
            sp54.f[0] = objdata->player->positionMirror.f[0] - self->positionMirror.f[0];
            sp54.f[1] = objdata->player->positionMirror.f[1] - self->positionMirror.f[1];
            sp54.f[2] = objdata->player->positionMirror.f[2] - self->positionMirror.f[2];
            objdata->unk10 = sqrtf(SQ(sp54.f[0]) + SQ(sp54.f[1]) + SQ(sp54.f[2]));
        }
        if (sp68 != NULL) {
            sp54.f[0] = sp68->unk68.f[0] - self->positionMirror.f[0];
            sp54.f[1] = sp68->unk68.f[1] - self->positionMirror.f[1];
            sp54.f[2] = sp68->unk68.f[2] - self->positionMirror.f[2];
            objdata->unk14 = sqrtf(SQ(sp54.f[0]) + SQ(sp54.f[1]) + SQ(sp54.f[2]));
        }
        if (objdata->flags & 2) {
            if (objdata->unk14 > 250.0f) {
                objdata->flags &= ~0x2;
                objdata->flags |= 4;
            }
        }
        if (objdata->flags & 4) {
            if (objdata->unk14 < 30.0f) {
                objdata->flags &= ~0x4;
            }
        }
        if (!(objdata->flags & 6) && (objdata->player != NULL) && (objdata->unk10 < objdata->unk20)) {
            objdata->flags |= 2;
        }
        dll_250_func_A5C(self, (DLL250_Data* ) objdata);
    }
}

// offset: 0x808 | func: 2 | export: 2
void dll_250_update(Object *self) { }

// offset: 0x814 | func: 3 | export: 3
void dll_250_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DLL250_Data* objdata;

    objdata = self->data;
    if ((visibility != 0) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        if (objdata->flags & 0x30) {
            if (objdata->flags & 0x10) {
                gDLL_32->vtbl->func2(self, 0x330, &objdata->unk18);
                gDLL_32->vtbl->func2(self, 0x330, &objdata->unk18);
                objdata->flags = (u8) (objdata->flags & 0xFFEF);
            }
            gDLL_32->vtbl->func2(self, 0x32F, &objdata->unk18);
        }
        if (objdata->flags & 8) {
            gDLL_32->vtbl->func2(self, 0x333, &objdata->unk18);
            gDLL_32->vtbl->func2(self, 0x334, &objdata->unk18);
            objdata->flags = (u8) (objdata->flags & 0xFFF7);
        }
    }
}

// offset: 0x99C | func: 4 | export: 4
void dll_250_free(Object* self, s32 arg1) {
    DLL250_Data* objdata;
    void* temp_a0;

    objdata = self->data;
    obj_free_object_type(self, 4);
    temp_a0 = objdata->unk0;
    if (temp_a0 != NULL) {
        mmFree(temp_a0);
        objdata->unk0 = NULL;
    }
    if (objdata->unk24 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk24);
    }
}


// offset: 0xA38 | func: 5 | export: 5
u32 dll_250_get_model_flags(Object* self) {
    return 9U;
}

// offset: 0xA48 | func: 6 | export: 6
u32 dll_250_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL250_Data);
}

// offset: 0xA5C | func: 7
static void dll_250_func_A5C(Object* self, DLL250_Data* objdata) {
    f32 sp3C;
    UnkFunc_80024108Struct sp50;
    f32 sp38;
    UnkCurvesStruct* temp_s2;
    s32 var_v1;
    s32* pad;

    temp_s2 = objdata->unk0;
    if (((func_800053B0(temp_s2, objdata->unk8) != 0) || (_bss_0 != temp_s2->unk10)) && 
        (gDLL_26_Curves->vtbl->func_4704(temp_s2) != 0) && 
        (gDLL_26_Curves->vtbl->func_4288(objdata->unk0, self, 400.0f, (s32* ) _data_0, -1) != 0)) {
        objdata->flags &= ~8;
    }
    _bss_0 = temp_s2->unk10;
    objdata->unk28 += (u16)(128.0f * gUpdateRateF);
    objdata->unk2A += (u16) (256.0f * gUpdateRateF);
    objdata->unk2C += (u16) (512.0f * gUpdateRateF);
    self->srt.roll = (s16) (s32) ((fsin16_precise((s16) objdata->unk28) + fsin16_precise((s16) objdata->unk2A)) * 1000.0f);
    self->srt.pitch = (s16) (s32) ((fsin16_precise((s16) objdata->unk28) + fsin16_precise((s16) objdata->unk2C)) * 1000.0f);
    if (objdata->flags & 2) {
        self->speed.f[0] += (objdata->player->srt.transl.f[0] - self->srt.transl.f[0]) * 0.001f;
        self->speed.f[1] += ((objdata->player->srt.transl.f[1] + 60.0f) - self->srt.transl.f[1]) * 0.001f;
        self->speed.f[2] += (objdata->player->srt.transl.f[2] - self->srt.transl.f[2]) * 0.001f;
    } else if (objdata->flags & 4) {
        self->speed.f[0] += (temp_s2->unk68.f[0] - self->srt.transl.f[0]) * 0.001f;
        self->speed.f[1] += (temp_s2->unk68.f[1] - self->srt.transl.f[1]) * 0.001f;
        self->speed.f[2] += (temp_s2->unk68.f[2] - self->srt.transl.f[2]) * 0.001f;
    } else {
        self->speed.f[0] += (temp_s2->unk68.f[0] - self->srt.transl.f[0]) * 0.001f;
        self->speed.f[1] += ((temp_s2->unk68.f[1] + ((fsin16_precise((s16) objdata->unk28) + fsin16_precise((s16) objdata->unk2A)) * 10.0f)) - self->srt.transl.f[1]) * 0.001f;
        self->speed.f[2] += (temp_s2->unk68.f[2] - self->srt.transl.f[2]) * 0.001f;
    }
    self->speed.f[0] *= 0.9f;
    self->speed.f[1] *= 0.9f;
    self->speed.f[2] *= 0.9f;
    if (self->speed.f[0] > 0.5f) {
        self->speed.f[0] = 0.5f;
    }
    if (self->speed.f[1] > 0.5f) {
        self->speed.f[1] = 0.5f;
    }
    if (self->speed.f[2] > 0.5f) {
        self->speed.f[2] = 0.5f;
    }
    if (self->speed.f[0] < -0.5f) {
        self->speed.f[0] = -0.5f;
    }
    if (self->speed.f[1] < -0.5f) {
        self->speed.f[1] = -0.5f;
    }
    if (self->speed.f[2] < -0.5f) {
        self->speed.f[2] = -0.5f;
    }
    obj_integrate_speed(self, self->speed.f[0] * gUpdateRateF, self->speed.f[1] * gUpdateRateF, self->speed.f[2] * gUpdateRateF);
    func_80024108(self, objdata->unkC, gUpdateRateF, &sp50);
    var_v1 = arctan2_f(self->positionMirror.f[0] - objdata->player->positionMirror.f[0], self->positionMirror.f[2] - objdata->player->positionMirror.f[2]) - (self->srt.yaw & 0xFFFF);
    if (var_v1 >= 0x8001) {
        var_v1 += 0xFFFF0001;
    }
    if (var_v1 < -0x8000) {
        var_v1 += 0xFFFF;
    }
    self->srt.yaw = self->srt.yaw + (s32) (((f32) var_v1 * gUpdateRateF) / 12.0f);
}


