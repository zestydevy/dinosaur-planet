#include "common.h"
#include "sys/objanim.h"

typedef struct {
    HeadAnimation unk0;
    u8 unk24; 
    u8 unk25;
    u8 unk26; 
    s8 _unk27;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    //[0x34]; 
} DLL455_Data;

typedef struct{
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
}DLL455_Setup;

static int dll_455_func_6BC(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3);

// offset: 0x0 | ctor
void dll_455_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_455_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_455_setup(Object* self, DLL455_Setup* setup, s32 arg2) {
    DLL455_Data* hightopData;

    hightopData = self->data;
    self->srt.yaw = setup->unk1A << 8;
    self->srt.pitch = setup->unk19 << 8;
    self->srt.roll = setup->unk18 << 8;
    self->animCallback = dll_455_func_6BC; 
    hightopData->unk24 = 0;
    hightopData->unk25 = 0;
}

// offset: 0x6C | func: 1 | export: 1
void dll_455_control(Object* self) {
    DLL455_Data* HighTopData;
    Object* temp_v0;
    TextureAnimator* sp3C;
    TextureAnimator* sp38;

    HighTopData = self->data;
    sp3C = func_800348A0(self, 5, 0);
    sp38 = func_800348A0(self, 4, 0);
    switch (HighTopData->unk24) {                             
    case 0:                                        
        switch (HighTopData->unk25) {                          
        case 0:                                    
            sp3C->frame = 0x200;
            sp38->frame = 0x200;
            HighTopData->unk0.aimIsActive = 0;
            func_80023D30(self, 0x12, 0.0f, 0);
            HighTopData->unk2C = 0.001f;
            HighTopData->unk25 = 1;
            HighTopData->unk26 = 0;
            HighTopData->unk30 = 0.0f;
            break;
        case 1:                                     
            if (self->unkAF & 4) {
                sp3C->frame = 0;
                sp38->frame = 0;
                HighTopData->unk28 = 0.0f;
                func_80023D30(self, 0x10, 0.0f, 0);
                HighTopData->unk2C = 0.005f;
                HighTopData->unk25 = 2;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_841, MAX_VOLUME, NULL, NULL, 0, NULL);
                /* fallthrough */
            } else {
                HighTopData->unk30 -= gUpdateRateF;
                if (HighTopData->unk30 <= 0.0f) {
                    if (HighTopData->unk26 == 0) {
                        HighTopData->unk26 = 1U;
                        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_83F, MAX_VOLUME, NULL, NULL, 0, NULL);
                    } else {
                        HighTopData->unk26 = 0U;
                        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_840, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                    HighTopData->unk30 += rand_next(0x12C, 0x190);
                }
                break;
            }
        case 2:                                 
            HighTopData->unk2C += (gUpdateRateF * 0.0002f);
            if (HighTopData->unk2C > 0.005f) {
                HighTopData->unk2C = 0.005f;
            }
            if (self->animProgress > 0.95f) {
                HighTopData->unk0.aimIsActive = 1;
                func_80023D30(self, 0x11, 0.0f, 0);
                HighTopData->unk2C = 0.005f;
                HighTopData->unk25 = 3U;
                HighTopData->unk30 = 0.0f;
                /* fallthrough */
            } else {
                break;
            }
        case 3:                             
            if (self->unkAF & 4) {
                HighTopData->unk28 = 0.0f;
            } else {
                HighTopData->unk28 += gUpdateRateF;
            }
            if (HighTopData->unk28 >= 300.0f) {
                HighTopData->unk0.aimIsActive = 0;
                func_80023D30(self, 0x10, 1.0f, 0);
                HighTopData->unk2C = -0.004f;
                HighTopData->unk25 = 4;
                /* fallthrough */
            } else {
                if (HighTopData->unk0.aimIsActive != 0) {
                    temp_v0 = get_player();
                    HighTopData->unk0.headAimX = temp_v0->srt.transl.x;
                    HighTopData->unk0.headAimY = temp_v0->srt.transl.y;
                    HighTopData->unk0.headAimZ = temp_v0->srt.transl.z;
                }
                func_800328F0(self, &HighTopData->unk0, 0.0f);
                func_80032A08(self, &HighTopData->unk0);
                HighTopData->unk30 -= gUpdateRateF;
                if (HighTopData->unk30 <= 0.0f) {
                    HighTopData->unk30 += 150.0f;
                    gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_842, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
                break;
            }
        case 4:                         
            if (self->unkAF & 4) {
                sp3C->frame = 0;
                sp38->frame = 0;
                HighTopData->unk25 = 2;
                HighTopData->unk28 = 0.0f;
            } else if (self->animProgress < 0.05f) {
                sp3C->frame = 0x200;
                sp38->frame = 0x200;
                HighTopData->unk25 = 1;
                func_80023D30(self, 0x12, 0.0f, 0);
                HighTopData->unk2C = 0.001f;
                HighTopData->unk26 = 0;
                HighTopData->unk30 = 0.0f;
            }
            func_800328F0(self, &HighTopData->unk0, 0.0f);
            func_80032A08(self, &HighTopData->unk0);
            break;
        }
        if (self->unkAF & 1) {
            gDLL_3_Animation->vtbl->func17(0, self, -1);
        }
        func_80024108(self, HighTopData->unk2C, gUpdateRateF, NULL);
        /* fallthrough */
    case 1:                                         
        return;
    }
}

// offset: 0x628 | func: 2 | export: 2
void dll_455_update(Object *self) { }

// offset: 0x634 | func: 3 | export: 3
void dll_455_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x688 | func: 4 | export: 4
void dll_455_free(Object *self, s32 a1) { }

// offset: 0x698 | func: 5 | export: 5
u32 dll_455_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x6A8 | func: 6 | export: 6
u32 dll_455_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL455_Data);
}

// offset: 0x6BC | func: 7
static int dll_455_func_6BC(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3) {
    dll_455_control(arg0);
    return 0;
}

/*0x0*/ static const char str_0[] = "trying to yawn\n";
