#include "common.h"


typedef struct {
/*0x00*/ u8 _unk0 [0x10];
/*0x10*/ s32 unk10;
/*0x14*/ u8 _unk14[0x68 - 0x14];
/*0x68*/ f32 unk68;
/*0x6C*/ u32 _unk6C;
/*0x70*/ f32 unk70;
/*0x74*/ u8 _unk74 [0x108 - 0x74];
/*0x108*/ f32 unk108;
/*0x10C*/ f32 unk10C;
/*0x110*/ f32 unk110;
}WaterLily_Data;


typedef struct {
/*0x00*/ s16 objId;
/*0x02*/ u8 quarterSize; // Bits 0-7: Exclude from map setups 1-8
/*0x03*/ u8 setupExclusions1;
/*0x04*/ u8 loadFlags;
/*0x05*/ union {
u8 byte5; // Bits 7-4 (note the reversal): Exclude from map setups 9-12
u8 setupExclusions2; // Bits 0-3: Fade flags
u8 fadeFlags; };
/*0x06*/ union {
u8 byte6; // If loadFlags & 0x10 IS set, the map object group this object is a part of.
u8 mapObjGroup; // If loadFlags & 0x10 is NOT set, maximum distance object is loaded at (divided by 8).
u8 loadDistance; }; // Max distance object is visible at (divided by 8).
/*0x07*/ u8 fadeDistance;
/*0x08*/ f32 x;
/*0x0c*/ f32 y;
/*0x10*/ f32 z;
/*0x14*/ s32 uID;
/*0x18*/ s8 unk18;
/*0x19*/ s8 unk19;
/*0x1A*/ s16 unk1A;
} WaterLily_Setup;

// offset: 0x0 | ctor
void dll_509_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_509_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_509_setup(Object* self, WaterLily_Setup* setup, s32 arg2) {
    s32 sp34 = 0x19;
    WaterLily_Data* LilyData = self -> data;
  
    if (setup->unk19 == 1) {
        
        gDLL_26_Curves->vtbl->func_4288((UnkCurvesStruct* ) LilyData, self, 1000.0f, &sp34, -1);
        self->srt.transl.f[0] = LilyData->unk68;
        self->srt.transl.f[2] = LilyData->unk70;
        LilyData->unk108 = (setup->unk1A / 10.0f);
    }
    
    self->srt.yaw = setup->unk18 << 8;
    self->srt.transl.f[1] = setup->y - 1.5f;
    LilyData->unk10C = (f32) self->srt.transl.f[1];
}

// offset: 0x110 | func: 1 | export: 1
void dll_509_control(Object* self) {

    WaterLily_Data* WLData = self->data ;
    s32 pad;
    WaterLily_Setup *LilyData = (WaterLily_Setup*)self->setup;
    f32 temp;

    if (LilyData->unk19 == 1) {
   
        if ((func_800053B0((UnkCurvesStruct* ) WLData, WLData->unk108) != 0) || (WLData->unk10 != 0)) {
            gDLL_26_Curves->vtbl->func_4704((UnkCurvesStruct* ) WLData);
        }
        self->srt.transl.f[0] = WLData->unk68;
        self->srt.transl.f[2] = WLData->unk70;
    }
    if (WLData->unk110 > 0.0f) {
        WLData->unk110 = (f32) (WLData->unk110 - gUpdateRateF);
        if (WLData->unk110 <= 0.0f) {
            self->speed.f[1] = -0.25f;
        }
    }

    temp = LilyData->y - self->srt.transl.f[1];
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
    return 0x118U;
}


/*0x0*/ static const char str_0[] = "LILY ERROR: no node found\n";
