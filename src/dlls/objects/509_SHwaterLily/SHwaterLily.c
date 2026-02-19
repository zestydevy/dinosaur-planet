#include "sys/curves.h"
#include "sys/main.h"
#include "dll.h"
#include "functions.h"

// size:0x118
typedef struct {
/*000*/ UnkCurvesStruct unk0;
/*108*/ f32 unk108;
/*10C*/ f32 unk10C;
/*110*/ f32 unk110;
/*114*/ u8 _unk114[0x118 - 0x114];
} WaterLily_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 unk18;
/*19*/ s8 unk19;
/*1A*/ s16 unk1A;
} WaterLily_Setup;

// offset: 0x0 | ctor
void dll_509_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_509_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_509_setup(Object* self, WaterLily_Setup* setup, s32 arg2) {
    s32 sp34 = 0x19;
    WaterLily_Data* LilyData = self->data;
  
    if (setup->unk19 == 1) {
        gDLL_26_Curves->vtbl->func_4288(&LilyData->unk0, self, 1000.0f, &sp34, -1);
        self->srt.transl.f[0] = LilyData->unk0.unk68.f[0];
        self->srt.transl.f[2] = LilyData->unk0.unk68.f[2];
        LilyData->unk108 = (setup->unk1A / 10.0f);
    }
    
    self->srt.yaw = setup->unk18 << 8;
    self->srt.transl.f[1] = setup->base.y - 1.5f;
    LilyData->unk10C = self->srt.transl.f[1];
}

// offset: 0x110 | func: 1 | export: 1
void dll_509_control(Object* self) {
    WaterLily_Data* WLData = self->data ;
    s32 pad;
    WaterLily_Setup *LilyData = (WaterLily_Setup*)self->setup;
    f32 temp;

    if (LilyData->unk19 == 1) {
        if ((func_800053B0(&WLData->unk0, WLData->unk108) != 0) || (WLData->unk0.unk10 != 0)) {
            gDLL_26_Curves->vtbl->func_4704(&WLData->unk0);
        }
        self->srt.transl.f[0] = WLData->unk0.unk68.f[0];
        self->srt.transl.f[2] = WLData->unk0.unk68.f[2];
    }
    if (WLData->unk110 > 0.0f) {
        WLData->unk110 = (f32) (WLData->unk110 - gUpdateRateF);
        if (WLData->unk110 <= 0.0f) {
            self->speed.f[1] = -0.25f;
        }
    }

    temp = LilyData->base.y - self->srt.transl.f[1];
    self->speed.f[1] += temp * 0.25f * 0.08f * gUpdateRateF;
    self->speed.f[1] -= 0.05f * gUpdateRateF;
    self->speed.f[1] *= 0.9f;
    if (self->speed.f[1] > 1.4f) {
        self->speed.f[1] = 1.4f;
    }
    self->srt.transl.f[1] += self->speed.f[1] * gUpdateRateF;
}

// offset: 0x2C8 | func: 2 | export: 2
void dll_509_update(Object *self) { }

// offset: 0x2D4 | func: 3 | export: 3
void dll_509_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x328 | func: 4 | export: 4
void dll_509_free(Object *self, s32 a1) { }

// offset: 0x338 | func: 5 | export: 5
u32 dll_509_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x348 | func: 6 | export: 6
u32 dll_509_get_data_size(Object* self, u32 a1) {
    return sizeof(WaterLily_Data);
}

/*0x0*/ static const char str_0[] = "LILY ERROR: no node found\n";
