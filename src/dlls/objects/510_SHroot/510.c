#include "common.h"

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    s32 _unkC;
    s16 unk10;
    u8 unk12;
    u8 unk13; 
    s16 unk14;
//0x18
}DLL510_Data;
typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 _unk19;
    s16 unk1A;
    u16 _unk1C;
    s16 unk1E;
}DLL510_Setup;

// offset: 0x0 | ctor
void dll_510_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_510_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_510_setup(Object* self, DLL510_Setup* setup, s32 arg2) {

    DLL510_Data* temp_v1;
    temp_v1 = self->data;

    temp_v1->unk0 = (setup->unk1A / 255.0f);
    self->srt.scale = self->def->scale - (temp_v1->unk0 * 0.5f);
    self->srt.yaw = setup->unk18 << 8;
    self->speed.f[1] = 0.0f;
    if (main_get_bits((s32) setup->unk1E) != 0) {
        temp_v1->unk12 = 4;
    }
}

// offset: 0xC8 | func: 1 | export: 1

void dll_510_control(Object* self) {
    s32 pad;
    DLL510_Data* SHrootData;
    DLL510_Setup* sp54;
    s32 temp_v0; 
    s32 i;
    Func_80057F1C_Struct** sp48;

    SHrootData = self->data;
    sp54 = (DLL510_Setup*)self->setup; 
    switch (SHrootData->unk12) {
    case 0:
        if (func_80025F40(self, NULL, NULL, NULL) == 0xF) {
            SHrootData->unk10 = rand_next(0x320, 0x4B0);
            SHrootData->unk12 = 3U;
            gDLL_6_AMSFX->vtbl->play_sound(self, 0x3D6U, 0x7FU, NULL, NULL, 0, NULL);
            return;
        }
    break;
        
    case 3:
        self->srt.pitch = SHrootData->unk10;
        SHrootData->unk10 = (s16) (s32) ((f32) SHrootData->unk10 * 0.333f);
        if (self->srt.pitch < 0xA) {
            self->srt.pitch = 0;
            SHrootData->unk12 = 1U;
            SHrootData->unk14 = 0x1E;
            return;
        }
        break;
    case 1:
        if (!(SHrootData->unk13 & 2)) {
            temp_v0 = func_80057F1C(self, self->srt.transl.f[0], self->srt.transl.f[1], self->srt.transl.f[2], &sp48, 0, 0);
            SHrootData->unk4 = -100000.0f;
            for (i = 0; i < temp_v0; i++) {
            
                if (sp48[i]->unk0[0] < self->srt.transl.f[1]) {
                    
                    if (!(SHrootData->unk13 & 4) && (sp48[i]->unk14 == 0xE)) {
                        SHrootData->unk8 = sp48[i]->unk0[0];
                        SHrootData->unk13 |= 4;
                        
                    } else if (!(SHrootData->unk13 & 2)) {
                        SHrootData->unk4 = sp48[i]->unk0[0];
                        SHrootData->unk13 |= 2;
                    }
                }
            }
            SHrootData->unk13 = (u8) (SHrootData->unk13 & 0xFFFE);
        }
        if (SHrootData->unk14 > 0) {
            SHrootData->unk14 = (s16) (SHrootData->unk14 - gUpdateRate);
            if (SHrootData->unk14 <= 0) {
                gDLL_6_AMSFX->vtbl->play_sound(self, 0x3D7U, 0x7FU, NULL, NULL, 0, NULL);
            }
        }
        self->speed.f[1] -= 0.1f * gUpdateRateF;
        if (self->speed.f[1] < -10.0f) {
            self->speed.f[1] = -10.0f;
        }
        self->srt.transl.f[1] += self->speed.f[1] * gUpdateRateF;
        if (!(SHrootData->unk13 & 1) && (SHrootData->unk13 & 4)) {
            if (self->srt.transl.f[1] < (SHrootData->unk8 + 10.0f)) {
                gDLL_24_Waterfx->vtbl->func_174C(self->srt.transl.f[0], SHrootData->unk8, self->srt.transl.f[2], 10.0f);
                gDLL_24_Waterfx->vtbl->func_1CC8(self->srt.transl.f[0], SHrootData->unk8, self->srt.transl.f[2], 0, 0.0f, 2);
                gDLL_6_AMSFX->vtbl->play_sound(self, 0x3D8U, 0x7FU, NULL, NULL, 0, NULL);
                SHrootData->unk13 |= 1 ;
               // SHrootData->unk13 = SHrootData->unk13 ;
            }
        }
        if ((SHrootData->unk13 & 2) && (self->srt.transl.f[1] < (SHrootData->unk4 + 40.0f))) {
            main_set_bits((s32) sp54->unk1E, 1U);
            SHrootData->unk12 = 2;
            return;
        }
        break;
    case 4:
        if (!(SHrootData->unk13 & 2)) {
            temp_v0 = func_80057F1C(self, self->srt.transl.f[0], self->srt.transl.f[1], self->srt.transl.f[2], &sp48, 0, 0);   
            SHrootData->unk4 = -100000.0f;
            for (i = 0; i < temp_v0; i++) {
                
                
                if (sp48 [i]->unk0[0] < self->srt.transl.f[1]) {
                        
                    if (!(SHrootData->unk13 & 2)) {
                        SHrootData->unk4 = sp48 [i]->unk0[0];
                        SHrootData->unk13 = (u8) (SHrootData->unk13 | 2);
                    }
                }
            }
        }
        if (SHrootData->unk13 & 2) {
            SHrootData->unk12 = 2;
            self->srt.transl.f[1] = SHrootData->unk4 + 40.0f;
        }
        break;
            case 2:
        break;
    }
}

// offset: 0x5E0 | func: 2 | export: 2
void dll_510_update(Object *self) { }

// offset: 0x5EC | func: 3 | export: 3
void dll_510_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}
// offset: 0x640 | func: 4 | export: 4
void dll_510_free(Object *self, s32 a1) { }

// offset: 0x650 | func: 5 | export: 5
u32 dll_510_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x660 | func: 6 | export: 6
u32 dll_510_get_data_size(Object* self, u32 a1) {
    return 0x18U;
}

