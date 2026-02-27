#include "common.h"
#include "sys/objtype.h"
#include "sys/gfx/modgfx.h"

typedef struct {
   s8 unk0;
   s8 unk1;
}DLL320_Data;
typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    s16 unk1C;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
}DLL320_Setup;
/*0x0*/ static DLL_IModgfx* _data_0 = NULL;

// offset: 0x0 | ctor
void dll_320_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_320_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_320_setup(Object* self, DLL320_Setup* setup, s32 arg2) {
    DLL320_Data* objdata;
    f32 sp20;
    ObjectHitInfo* temp_v1_3;
    
    objdata = self->data;
    self->srt.yaw = setup->unk18 << 8;
    self->srt.pitch = setup->unk19 << 8;
    self->srt.roll = setup->unk1A << 8;
    self->modelInstIdx = (s8) setup->unk1B;
    self->unkB0 |= 0x2000;
    sp20 = setup->unk1E * 0.015625f;
    if (sp20 < 0.05f) {
        sp20 = 0.05f;
    }
    _data_0 = dll_load_deferred(0x1003U, 1U);
    if (self->shadow != NULL) {
        self->shadow->flags |= 0x810;
        self->shadow->scale = self->def->shadowScale * sp20; 
        self->shadow->maxDistScale = self->shadow->scale * 0.5f;
    }
    objdata->unk1 = 2;
    objdata->unk0 = main_get_bits((s32) setup->unk1C);
    temp_v1_3 = self->objhitInfo;
    self->srt.scale = self->def->scale * sp20;
    temp_v1_3->unk52 = (s16) (s32) ((f32) temp_v1_3->unk52 * sp20);
}


// offset: 0x1BC | func: 1 | export: 1
void dll_320_control(Object* self) {
    ObjectHitInfo* temp_v0;
    DLL320_Data* objdata;
    void* sp48;
    DLL320_Setup* sp40;
    f32 sp3C;
    f32 temp_fv0;
    Object* temp_v0_2;

    objdata = self->data;
    if (objdata->unk0 != 0) {
        self->unkAF |= 8;
        return;
    }
    sp48 = objdata;
    if (func_80025F40(self, NULL, NULL, NULL) != 0) {
        objdata->unk1 = (s8) (objdata->unk1 - 1);
        if (objdata->unk1 > 0) {
            sp48 = objdata;
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_372_Crate_Struck, MAX_VOLUME, NULL, NULL, 0, NULL);
            func_80023A18(self, 2 - objdata->unk1);
        }
    }
    if (objdata->unk1 <= 0) {
        sp40 = (DLL320_Setup*)self->setup;
        objdata->unk0 = 1;
        temp_v0 = self->objhitInfo;
        temp_v0->unk58 &= 0xFFFE;
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_371_Crate_Smash, MAX_VOLUME, NULL, NULL, 0, NULL);
        if (sp40->unk1C != -1) {
            main_set_bits((s32) sp40->unk1C, 1U);
        }
        sp3C = 50.0f;
        temp_v0_2 = obj_get_nearest_type_to(5, self, &sp3C);
        if (temp_v0_2 != NULL) {
            temp_fv0 = self->srt.transl.f[0];
            temp_v0_2->positionMirror.f[0] = temp_fv0;
            temp_v0_2->srt.transl.f[0] = temp_fv0;
            temp_fv0 = self->srt.transl.f[1] + 15.0f;
            temp_v0_2->positionMirror.f[1] = temp_fv0;
            temp_v0_2->srt.transl.f[1] = temp_fv0;
            temp_fv0 = self->srt.transl.f[2];
            temp_v0_2->positionMirror.f[2] = temp_fv0;
            temp_v0_2->srt.transl.f[2] = temp_fv0;
            temp_v0_2->srt.yaw = self->srt.yaw;
        }
        (_data_0)->vtbl->func0(self, 1, 0, 2, -1, 0);
    }
}


// offset: 0x3D4 | func: 2 | export: 2
void dll_320_update(Object *self) { }

// offset: 0x3E0 | func: 3 | export: 3

void dll_320_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DLL320_Setup* objsetup;
    DLL320_Data* objdata = self->data;

    if (objdata->unk0 != 0) {
        return;
    }

    if (visibility != 0) {
        objsetup = (DLL320_Setup*)self->setup; 
        if (objsetup->unk1F & 1) {
            func_80036F6C((s16) objsetup->unk20, (s16) objsetup->unk21, (s16) objsetup->unk22);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x498 | func: 4 | export: 4
void dll_320_free(Object* self, s32 a1) {
    dll_unload( _data_0);
}

// offset: 0x4DC | func: 5 | export: 5
u32 dll_320_get_model_flags(Object* self) {
    return 2U;
}

// offset: 0x4EC | func: 6 | export: 6
u32 dll_320_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL320_Data);
}
