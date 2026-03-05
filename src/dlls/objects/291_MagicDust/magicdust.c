#include "common.h"
#include "sys/objmsg.h"

typedef struct {
    DLL27_Data unk0;
    f32 unk260;
    f32 unk264;
    s16 unk268;
    s16 unk26A;
    s16 unk26C;
    s16 unk26E;
    u16 _unk270;
    u8 unk272;
} DLL291_Data;

typedef struct {
    ObjSetup base;
    u16 unk18;
    u8 unk1A;
    u8 unk1B; 
    s16 unk1C;
    u16 _unk1E; 
    s32 _unk20; 
    s16 unk24;
    u8 unk26;
    u8 _unk27;
    s32 _unk28;
    s16 unk2C;
    s16 unk2E;
}MagicDustSetup;

/*0x0*/ static Vec3f _data_0 = { 0.0f, 0.0f, 0.0f };

// offset: 0x0 | ctor
void dll_291_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_291_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_291_setup(Object* self, MagicDustSetup* setup, s32 arg2) {
    s32 sp54;
    f32 sp50;
    DLL291_Data* objdata;
    f32 temp;
    u8 sp47;
    Object* temp_v0;

    objdata = self->data;
    sp47 = 3;
    sp54 = rand_next(0, 0xFFFF);
    sp50 = (rand_next(0x27, 0x2C) / 100.0f);
    self->speed.f[0] = fsin16_precise(sp54) * sp50;
    self->speed.f[2] = fcos16_precise(sp54) * sp50;
    self->speed.f[1] = rand_next(0x28, 0x32) / 50.0f;
    if (setup->unk2E == 1) {
        objdata->unk272 |= 1;
        self->opacity = 1;
    } else if (setup->unk2E == 2) {
        objdata->unk272 |= 1;
        self->opacity = 1;
        func_800267A4(self);
        temp = 120.0f;
        if (temp != 0.0f) {
            temp_v0 = get_player();
            self->speed.f[0] = (temp_v0->srt.transl.f[0] - self->srt.transl.f[0]) / temp;
            self->speed.f[1] = (temp_v0->srt.transl.f[1] - self->srt.transl.f[1]) / temp;
            self->speed.f[2] = (temp_v0->srt.transl.f[2] - self->srt.transl.f[2]) / temp;
        }
    }
    self->modelInstIdx = (s8) setup->unk26;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
    if (self->shadow != NULL) {
        self->shadow->distFadeMaxOpacity = 0x64;
        self->shadow->distFadeMinOpacity = 0x96;
        self->shadow->maxDistScale *= 2.0f;
    }
    switch (self->id) {                            /* irregular */
    case OBJ_MagicDustSmall:
        objdata->unk26A = 0x54D;
        objdata->unk268 = 0x549;
        objdata->unk26C = 0xB2A;
        objdata->unk26E = 0x5B0;
        break;
    case OBJ_MagicDustMid:
        objdata->unk26A = 0x54E;
        objdata->unk268 = 0x54A;
        objdata->unk26C = 0xB6B;
        objdata->unk26E = 0x5B1;
        break;
    case OBJ_MagicDustLarge:
        objdata->unk26A = 0x54F;
        objdata->unk268 = 0x54B;
        objdata->unk26C = 0xB6C;
        objdata->unk26E = 0x5B2;
        break;
    default:
    case OBJ_MagicDustHuge:
        objdata->unk26A = 0x550;
        objdata->unk268 = 0x54C;
        objdata->unk26C = 0xB6D;
        objdata->unk26E = 0x5B3;
        break;
    }
    objdata->unk260 = (f32) self->objhitInfo->unk52;
    if (self->srt.flags & 0x2000) {
        gDLL_27->vtbl->init(&objdata->unk0, 
    DLL27FLAG_NONE, 
    DLL27FLAG_1 | DLL27FLAG_2 | DLL27FLAG_4 | DLL27FLAG_40000, 
    DLL27MODE_DISABLED);
        gDLL_27->vtbl->setup_terrain_collider(&objdata->unk0, 1, &_data_0, &objdata->unk260, &sp47);
        gDLL_27->vtbl->reset(self, &objdata->unk0);
    }
    self->unkB0 |= 0x2000;
    if (objdata->unk272 & 1) {
        objdata->unk264 = 60.0f;
        return;
    }
    objdata->unk272 |= 4;
    objdata->unk264 = 1800.0f;
}

// offset: 0x3F0 | func: 1 | export: 1
void dll_291_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/291_MagicDust/dll_291_control.s")

// offset: 0xEFC | func: 2 | export: 2
void dll_291_update(Object *self) { }

// offset: 0xF08 | func: 3 | export: 3
void dll_291_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if ((visibility != 0) && (self->unkC4 == NULL)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xF68 | func: 4 | export: 4
void dll_291_free(Object *self, s32 a1) { }

// offset: 0xF78 | func: 5 | export: 5
u32 dll_291_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xF88 | func: 6 | export: 6
u32 dll_291_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL291_Data);
}

// offset: 0xF9C | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/291_MagicDust/dll_291_func_F9C.s")
