#include "common.h"
#include "sys/objtype.h"
#include "sys/gfx/modgfx.h"
#include "sys/gfx/projgfx.h"

typedef struct {
    DLL_IProjgfx* unk0;
    DLL_IModgfx* unk4;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 padB;
    u16 unkC;
    s16 unkE;
    f32 unk10;
    s32 unk14;
} Tesla_Data;

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
    u8 damageStrength;
} Tesla_Setup;

static void dll_257_func_1144(Object* self, Tesla_Setup* setup);

// offset: 0x0 | ctor
void dll_257_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_257_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_257_setup(Object* self, Tesla_Setup* setup, s32 arg2) {
    Tesla_Data* objdata;

    objdata = self->data;
    objdata->unk0 = 0;
    objdata->unk4 = 0;
    objdata->unk8 = 2;
    objdata->unk9 = 0;
    objdata->unk10 = 0.0f;
    self->srt.yaw = setup->unk20 << 8;
    self->srt.transl.f[1] = setup->base.y - setup->unk1E;
    obj_add_object_type(self, OBJTYPE_3);
}


// offset: 0xB8 | func: 1 | export: 1
void dll_257_control(Object* self) {
    Tesla_Data* objdata;
    Tesla_Setup* setup;
    f32 temp_fa0;
    f32 temp_fv1;
    Object* curObj;
    f32 sp68;
    f32 sp64;
    Object* player;
    Object* var_s0;
    Object** objects;
    s32 sp54;
    s32 temp_v0_5;
    s32 i;

    objdata = self->data;
    setup = (Tesla_Setup*)self->setup;
    var_s0 = NULL;
    if (objdata->unk0 == NULL) {
        objdata->unk0 = (DLL_IProjgfx*)dll_load_deferred(DLL_ID_205, 1U);
        if (objdata->unk0 == NULL) { return; }
    }
    if (objdata->unk4 == NULL) {
        objdata->unk4 = (DLL_IModgfx*)dll_load_deferred(DLL_ID_157, 1U);
        if (objdata->unk4 == NULL) { return; }
    }

    if (setup) {}
    player = get_player();
    sp68 = vec3_distance_xz(&player->globalPosition, &self->globalPosition);
    switch (objdata->unk8) {
    case 0:
        objdata->unk10 += setup->unk1F * gUpdateRateF * 0.01f;
        self->srt.transl.f[1] += objdata->unk10 * gUpdateRateF;
        dll_257_func_1144(self, setup);
        if (
            setup->unk18 < sp68 || 
            (setup->unk1C != -1 && main_get_bits(setup->unk1C) == 0)
        ) {
            objdata->unk8 = 1U;
            gDLL_6_AMSFX->vtbl->stop(objdata->unk14);
            objdata->unk14 = gDLL_6_AMSFX->vtbl->play(self, SOUND_57E, MAX_VOLUME, NULL, NULL, 0, NULL);
        } else if (setup->base.y == self->srt.transl.f[1]) {
            objdata->unk8 = 3U;
            objdata->unkC = 0U;
            gDLL_6_AMSFX->vtbl->stop(objdata->unk14);
            objdata->unk14 = gDLL_6_AMSFX->vtbl->play(self, SOUND_57C, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        break;
    case 1:
        objdata->unk10 -= setup->unk1F * gUpdateRateF * 0.01f;
        self->srt.transl.f[1] += objdata->unk10 * gUpdateRateF;
        dll_257_func_1144(self, setup);
        if ((sp68 < setup->unk18) && (setup->unk1C == -1 || main_get_bits(setup->unk1C) != 0)) {
            objdata->unk8 = 0U;
            gDLL_6_AMSFX->vtbl->stop(objdata->unk14);
            objdata->unk14 = gDLL_6_AMSFX->vtbl->play(self, SOUND_57A, MAX_VOLUME, NULL, NULL, 0, NULL);
        } else if ((setup->base.y - setup->unk1E) == self->srt.transl.f[1]) {
            objdata->unk8 = 2U;
            gDLL_6_AMSFX->vtbl->stop(objdata->unk14);
        }
        break;
    case 2:
        if ((sp68 < setup->unk18) && (setup->unk1C == -1 || main_get_bits(setup->unk1C) != 0)) {
            objdata->unk8 = 0U;
            objdata->unk14 = gDLL_6_AMSFX->vtbl->play(self, SOUND_57A, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        break;
    case 3:
        objdata->unkC += gUpdateRateF;
        temp_fv1 = setup->unk1B * 2;
        temp_fa0 = (temp_fv1 - objdata->unkC);
        if (temp_fa0 > 120.0f) {
            temp_fa0 = 1.0f - (objdata->unkC / temp_fv1);
            objdata->unk9 += gUpdateRateF;
            if ((60.0f * temp_fa0) <= objdata->unk9) {
                objdata->unk9 = 0U;
                objdata->unk4->vtbl->func0(self, 0, NULL, 0x10001, -1, NULL);
            }
        }
        if (sp68 < setup->unk1A) {
            objdata->unkC = 0U;
            func_8002635C(player, self, 1, 0, 0);
            objdata->unkA = 1U;
            objdata->unk8 = 4U;
            return;
        }
        temp_fv1 = setup->unk1B * 2;
        if (temp_fv1 < objdata->unkC) {
            objdata->unkC -= temp_fv1;
            objdata->unkA = 2;
            gDLL_6_AMSFX->vtbl->play(self, SOUND_57D, MAX_VOLUME, NULL, NULL, 0, NULL);
            sp64 = setup->unk19;
            var_s0 = obj_get_nearest_type_to(OBJTYPE_4, self, &sp64);
            if (var_s0 == NULL || sp68 <= sp64) {
                if (sp68 < setup->unk19) {
                    func_8002635C(player, self, Damage_Type_Sword_Staff_Strike2, setup->damageStrength, 0);
                }
            } else {
                if (sp64 < setup->unk19) {
                    func_8002635C(var_s0, self, Damage_Type_Projectile, setup->damageStrength, 0);
                }
            }
        }
        if (objdata->unkA != 0) {
            if (var_s0 == NULL) {
                sp64 = setup->unk19;
                var_s0 = obj_get_nearest_type_to(OBJTYPE_4, self, &sp64);
            }
            objdata->unkA--;
            if (var_s0 == NULL || sp68 <= sp64) {
                if (sp68 < setup->unk19) {
                    objdata->unk0->vtbl->func0(self, 0, NULL, 1, -1, 0x13, (s32)player);
                    objdata->unk0->vtbl->func0(self, 0, 0, 1, -1, 0x13, (s32)player);
                    goto bail;
                }
            }
            if (var_s0 != NULL) {
                if (sp64 < setup->unk19) {
                    objdata->unk0->vtbl->func0(self, 0, 0, 1, -1, 0x13, (s32)var_s0);
                    objdata->unk0->vtbl->func0(self, 0, 0, 1, -1, 0x13, (s32)var_s0);
                    goto bail;
                }
            }
            objects = obj_get_all_of_type(3, &sp54);
            for (i = 0; i != 2; i++) {
                start:
                temp_v0_5 = rand_next(0, sp54 - 1);\
                if (self->tabIdx != objects[temp_v0_5]->tabIdx) {\
                    curObj = objects[temp_v0_5];
                    if (((DLL_Unknown*)curObj->dll)->vtbl->func[7].withOneArgS32((s32)curObj) == 0) {
                        goto start;
                    }
                }
                objdata->unk0->vtbl->func0(self, 0, 0, 1, -1, 0x13, (s32)objects[temp_v0_5]);
            }
        }
        bail:
        if (setup->unk18 < sp68 || (setup->unk1C != -1 && main_get_bits(setup->unk1C) == 0)) {
            objdata->unk8 = 1U;
            gDLL_6_AMSFX->vtbl->stop(objdata->unk14);
            objdata->unk14 = gDLL_6_AMSFX->vtbl->play(self, SOUND_57E, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        break;
    case 4:
        objdata->unkC += gUpdateRateF;
        if (objdata->unkC > 10.0f) {
            objdata->unkC -= 10.0f;
            func_8002635C(player, self, 1, 0, 0);
            objdata->unk0->vtbl->func0(self, 0, 0, 1, -1, 0x13, (s32)player);
            gDLL_6_AMSFX->vtbl->play(self, SOUND_57D, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        if (setup->unk1A < sp68) {
            objdata->unk8 = 3U;
        }
        break;
    }
}

// offset: 0x1018 | func: 2 | export: 2
void dll_257_update(Object *self) { }

// offset: 0x1024 | func: 3 | export: 3
void dll_257_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Tesla_Data* objdata;

    objdata = self->data;
    if ((visibility != 0) && (objdata->unk8 != 2)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x108C | func: 4 | export: 4
void dll_257_free(Object* self, s32 a1) {
    Tesla_Data* objdata;
    DLL_IModgfx* modgfxDLL;
    DLL_IProjgfx* projgfxDLL;

    objdata = self->data;
    projgfxDLL = objdata->unk0;
    if (projgfxDLL != NULL) {
        dll_unload(projgfxDLL);
    }
    modgfxDLL = objdata->unk4;
    if (modgfxDLL != NULL) {
        dll_unload(modgfxDLL);
    }
    obj_free_object_type(self, OBJTYPE_3);
}

// offset: 0x1120 | func: 5 | export: 5
u32 dll_257_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1130 | func: 6 | export: 6
u32 dll_257_get_data_size(Object *self, u32 a1) {
    return sizeof(Tesla_Data);
}

// offset: 0x1144 | func: 7
static void dll_257_func_1144(Object* self, Tesla_Setup* setup) {
    Tesla_Data* objdata;

    objdata = self->data;
    if (self->srt.transl.y < setup->base.y - setup->unk1E) {
        self->srt.transl.y = setup->base.y - setup->unk1E;
        objdata->unk10 = 0.0f;
    }
    if (setup->base.y < self->srt.transl.y) {
        self->srt.transl.y = setup->base.y;
        objdata->unk10 = 0.0f;
    }
}

// offset: 0x11C8 | func: 8 | export: 7
s32 dll_257_func_11C8(Object* self) {
    Tesla_Data* objdata;
    
    objdata = self->data;
    return objdata->unk8 == 3;
}

/*0x0*/ static const char str_0[] = "Trying to strike baddie, but it doesn't work! Would someone please fix this.\n";
