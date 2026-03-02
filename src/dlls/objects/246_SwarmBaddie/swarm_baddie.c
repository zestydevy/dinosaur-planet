#include "common.h"
#include "sys/objtype.h"

typedef struct {
    UnkCurvesStruct* unk0;
    Object* player;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 unk24;
    s8 _unk25;
    s16 unk26;
    s16 unk28;
    s16 _unk2A;
   // [0x2C]; 
} DLL246_Data;
typedef struct {
    ObjSetup base;
    u8 _unk18;
    s8 unk19;
    s16 unk1A;
} DLL426_Setup;

/*0x0*/ static s32 _data_0[] = {
    0x00000002, 0x00000003, 0x00000000, 0x00000000
};

typedef struct {
    s32 unk0;
    u8 pad4[0x10-0x4];
} UnkBss0;
/*0x0*/ static s32 _bss_0;

static void dll_246_func_6C8(Object* self, DLL246_Data* objdata);

// offset: 0x0 | ctor
void dll_246_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_246_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_246_setup(Object* self, DLL426_Setup* setup, s32 arg2) {
    DLL246_Data* objdata;

    objdata = self->data;
    objdata->unk8 = (f32) ((f32) setup->unk1A / 50.0f);
    objdata->unk14 = (f32) ((f32) setup->unk19 * 4.0f);
    objdata->unk18 = 1.0f; 
    if (arg2 == 0) {
        objdata->unk0 = mmAlloc(0x108, 0x1A, NULL);
        if (objdata->unk0 != NULL) {
            bzero(objdata->unk0, 0x108);
        }
        if (gDLL_26_Curves->vtbl->func_4288(objdata->unk0, self, objdata->unk14, (s32* ) _data_0, -1) == 0) {
            objdata->unk24 = (u8) (objdata->unk24 | 1);
        }
        objdata->unk1C = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B83, MAX_VOLUME, NULL, NULL, 0, NULL);
        objdata->unk20 = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B83, HALF_VOLUME+1, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->func_954(objdata->unk20, 0.7f);
    }
    self->unkB0 |= 0x2000;
}

// offset: 0x1CC | func: 1 | export: 1
void dll_246_control(Object* self) {
    DLL246_Data* objdata;
    UnkCurvesStruct* sp68;
    Object* sp64;
    Vec3f sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    s32 pad_sp48;
    s32 sp44;
    s32 sp40;

    objdata = self->data;
    sp68 = objdata->unk0;
    if (func_8002601C(self, &sp64, &sp44, &sp40, &sp54, &sp50, &sp4C) != 0) {
        objdata->unk18 = 2.0f;
    }
    func_80026128(self, 0xA, 1, 0);
    func_8002674C(self);
    if (objdata->unk18 > 1.0f) {
        objdata->unk18 = (f32) (objdata->unk18 - 0.005f);
    }
    gDLL_6_AMSFX->vtbl->func_954(objdata->unk1C, (fsin16_precise((s16) (objdata->unk26 + objdata->unk28)) * 0.05f) + objdata->unk18);
    gDLL_6_AMSFX->vtbl->func_860(objdata->unk1C, (u32) (objdata->unk18 * 63.0f) );
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_336, NULL, 2, -1, &objdata->unk18);
    objdata->player = get_player();
    if (objdata->player != NULL) {
        sp58.f[0] = objdata->player->positionMirror.x - self->positionMirror.x;
        sp58.f[1] = objdata->player->positionMirror.y - self->positionMirror.y;
        sp58.f[2] = objdata->player->positionMirror.z - self->positionMirror.z;
        objdata->unkC = VECTOR_MAGNITUDE(sp58);
    }
    if (sp68 != NULL) {
        sp58.f[0] = sp68->unk68.x - self->positionMirror.x;
        sp58.f[1] = sp68->unk68.y - self->positionMirror.y;
        sp58.f[2] = sp68->unk68.z - self->positionMirror.z;
        objdata->unk10 = VECTOR_MAGNITUDE(sp58);
    }
    if (objdata->unk24 & 2) {
        if (objdata->unk10 > 250.0f) {
            objdata->unk24 &= ~2; 
            objdata->unk24 |= 4;
        }
    }
    if (objdata->unk24 & 4) {
        if (objdata->unk10 < 60.0f) {
            objdata->unk24 &= ~4; 
        }
    }
    if (!(objdata->unk24 & 6) && (objdata->player != NULL) && (objdata->unkC < objdata->unk14)) {
        objdata->unk24 |= 2;
    }
    dll_246_func_6C8(self, objdata);
}

// offset: 0x5B0 | func: 2 | export: 2
void dll_246_update(Object *self) { }

// offset: 0x5BC | func: 3 | export: 3
void dll_246_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x5D4 | func: 4 | export: 4
void dll_246_free(Object* self, s32 arg1) {
    DLL246_Data* objdata;
    void* temp_a0;

    objdata = self->data;
    obj_free_object_type(self, 4);
    temp_a0 = objdata->unk0;
    if (temp_a0 != NULL) {
        mmFree(temp_a0);
        objdata->unk0 = 0;
    }
    if (objdata->unk1C != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk1C);
    }
    if (objdata->unk20 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk20);
    }
}

// offset: 0x6A4 | func: 5 | export: 5
s32 dll_246_get_model_flags(s32 arg0) {
    return 9;
}

// offset: 0x6B4 | func: 6 | export: 6
u32 dll_246_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL246_Data);
}

// offset: 0x6C8 | func: 7
static void dll_246_func_6C8(Object* self, DLL246_Data* objdata) {
    UnkCurvesStruct* temp_s1;

    temp_s1 = objdata->unk0;
    if (
        ((func_800053B0(temp_s1, objdata->unk8) != 0) || (_bss_0 != temp_s1->unk10)) && 
        (gDLL_26_Curves->vtbl->func_4704(temp_s1) != 0) && 
        (gDLL_26_Curves->vtbl->func_4288(objdata->unk0, self, 400.0f, _data_0, -1) != 0)
   ) {
        objdata->unk24 &= ~1;
    }
    _bss_0 = temp_s1->unk10;
    if (objdata->unk24 & 2) {
        self->speed.f[0] += (objdata->player->srt.transl.f[0] - self->srt.transl.f[0]) * 0.003f;
        self->speed.f[1] += ((objdata->player->srt.transl.f[1] + 30.0f) - self->srt.transl.f[1]) * 0.003f;
        self->speed.f[2] += ((objdata->player->srt.transl.f[2] - self->srt.transl.f[2]) * 0.003f);
    } else {
        self->speed.f[0] += (temp_s1->unk68.f[0] - self->srt.transl.f[0]) * 0.003f;
        self->speed.f[1] += (temp_s1->unk68.f[1] - self->srt.transl.f[1]) * 0.003f;
        self->speed.f[2] += ((temp_s1->unk68.f[2] - self->srt.transl.f[2]) * 0.003f);
    }
    self->speed.f[0] *= 0.9f;
    self->speed.f[1] *= 0.9f;
    self->speed.f[2] *= 0.9f;
    if (self->speed.f[0] > 0.8f) {
        self->speed.f[0] = 0.8f;
    }
    if (self->speed.f[1] > 0.8f) {
        self->speed.f[1] = 0.8f;
    }
    if (self->speed.f[2] > 0.8f) {
        self->speed.f[2] = 0.8f;
    }
    if (self->speed.f[0] < -0.8f) {
        self->speed.f[0] = -0.8f;
    }
    if (self->speed.f[1] < -0.8f) {
        self->speed.f[1] = -0.8f;
    }
    if (self->speed.f[2] < -0.8f) {
        self->speed.f[2] = -0.8f;
    }
    obj_integrate_speed(self, self->speed.f[0] * gUpdateRateF, self->speed.f[1] * gUpdateRateF, self->speed.f[2] * gUpdateRateF);
    objdata->unk26 += (s16) (32.0f * gUpdateRateF);
    objdata->unk28 += (s16) (23.0f * gUpdateRateF);
    self->srt.yaw += (s16) (fsin16_precise(objdata->unk26) * 182.0f * 4.0f);
    self->srt.roll += (s16) (fsin16_precise(objdata->unk28) * 182.0f * 4.0f);
    
    
}

