#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "sys/objlib.h"
#include "dlls/objects/210_player.h"

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s32 _unk14;
    s32 _unk18;
    s32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    u32 unk34;
    s16 unk38;
    u8 unk3A;
    u8 unk3B; 
    s8 _unk3C;
    s8 unk3D;
    s16 _unk3E;
} DLL503_Data;
typedef struct {
    ObjSetup base;
    u16 unk18;
    u16 unk1A;
    s16 unk1C;
    u8 unk1E;
    u8 unk1F;
}DLL503_Setup;

static void dll_503_func_DE4(Object* arg0, DLL503_Data* arg1, s32 arg2);


/*0x0*/ static s16 _data_0[] = {
    0, 0, 4, 1, 2, 3, 5, 6, 6, 9, 0, 0
};
/*0x18*/ static f32 _data_18[] = {
    0.0f, 0.0f, 0.008f, 0.025f, 0.018f, 0.015f, 0.006f, 0.008f, 
    0.005f, 0.012f, 0.005f, 0.0f, 0.0f, 0.0f
};

// offset: 0x0 | ctor
void dll_503_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_503_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_503_setup(Object* self, DLL503_Setup* setup, s32 arg2) {
    ObjectShadow* temp_v0;
    DLL503_Data* objdata;

    objdata = self->data;
    objdata->unk0 = 0;
    objdata->unk2C = 0; 
    objdata->unkC = self->srt.scale;
    objdata->unk38 =  setup->unk1A;
    if (objdata->unk38 < 0x708) {
        objdata->unk38 = 0x708U;
    }
    self->srt.transl.f[1] = setup->base.y - 2.0f;
    temp_v0 = self->shadow;
    if (temp_v0 != NULL) {
        temp_v0->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR;
    }
    if (arg2 == 0) {
        dll_503_func_DE4(self, objdata, 0);
    }
}

// offset: 0xC4 | func: 1 | export: 1
void dll_503_control(Object* self) {
    DLL503_Data* objdata;
    Object* player;
    Object* sp7C;
    s32 sp78;
    s32 sp74;
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 var_v0;
    s32 pad;
    u16 temp;
    u8 sp5D;
    DLL503_Setup* sp58;
    SRT sp40;

    objdata = self->data; 
    player = get_player();
    sp58 = (DLL503_Setup*)self->setup;
    func_80026160(self);
    self->unkAF |= 8;
    objdata->unk3B |= 4;

    switch (objdata->unk3A) {
    case 6:
        if (objdata->unk34 == 0) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_53B, MAX_VOLUME, &objdata->unk34, NULL, 0, NULL);
        }
        objdata->unk3B &= ~4;
        objdata->unk2C = (f32) (objdata->unk2C + (2.5f * gUpdateRateF));
        if (objdata->unk2C > 40.0f) {
            objdata->unk2C = 40.0f;
        }
        if (!(objdata->unk3B & 1) && (vec3_distance(&self->globalPosition, &player->globalPosition) <= objdata->unk2C) && 
            (((DLL_210_Player*)player->dll)->vtbl->func42(player) == 0) && 
            (((DLL_210_Player*)player->dll)->vtbl->func43(player) == 0)) {
            func_8002635C(player, self, 0x15, 1, 0);
            objdata->unk3B |= 1;
        }
        if (objdata->unk3B & 2) {
            objdata->unk3A = 2U;
            objdata->unk0 = 0.0f;
            if (objdata->unk34 != 0) {
                gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk34);
                objdata->unk34 = 0U;
            }
        }
        sp40.transl.f[0] = objdata->unk20;
        sp40.transl.f[1] = objdata->unk24;
        sp40.transl.f[2] = objdata->unk28;
        for (sp5D = 5; sp5D != 0; sp5D--) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3EB, &sp40, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
        }
        break;
    case 2:
        objdata->unk3B &= ~4;
        if (objdata->unk3B & 2) {
            var_v0 = self->opacity - (gUpdateRate * 4);
            if (var_v0 < 0) {
                var_v0 = 0;
            }
            self->opacity = var_v0;
            objdata->unk0 = (f32) (objdata->unk0 + gUpdateRateF);
            if ((f32) objdata->unk38 < objdata->unk0) {
                dll_503_func_DE4(self, (DLL503_Data* ) objdata, 1);
                objdata->unk3A = 1U;
            }
        }
        break;
    case 3:
        self->unkAF &= ~8;
        if (objdata->unk3B & 2) {
            objdata->unk3A = 4U;
        }
        break;
    case 4:
        if (objdata->unk34 == 0) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_53B, MAX_VOLUME, &objdata->unk34, NULL, 0, NULL);
        }
        self->unkAF &= ~8;
        objdata->unk2C = (f32) (objdata->unk2C + (2.5f * gUpdateRateF));
        if (!(objdata->unk3B & 1) && (vec3_distance(&self->globalPosition, &player->globalPosition) <= objdata->unk2C) && 
            (((DLL_210_Player*)player->dll)->vtbl->func42(player) == 0) && 
            (((DLL_210_Player*)player->dll)->vtbl->func43(player) == 0)) {
            func_8002635C(player, self, 0x15, 1, 0);
            objdata->unk3B |= 1;
        }
        if (objdata->unk2C > 40.0f) {
            objdata->unk2C = 40.0f;
        }
        objdata->unk0 = (f32) (objdata->unk0 + gUpdateRateF);
        if (objdata->unk0 > 120.0f) {
            objdata->unk3A = 5U;
            objdata->unk0 = 0.0f;
            if (objdata->unk34 != 0) {
                gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk34);
                objdata->unk34 = 0U;
            }
        }
        sp40.transl.f[0] = objdata->unk20;
        sp40.transl.f[1] = objdata->unk24;
        sp40.transl.f[2] = objdata->unk28;
        for (sp5D = 5; sp5D != 0; sp5D--) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3EB, &sp40, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
        }
        break;
    case 5:
        self->unkAF &= ~8;
        objdata->unk0 += gUpdateRateF;
        if (sp58->unk18 < objdata->unk0) {
            if (objdata->unk3B & 2) {
                objdata->unk3A = 0U;
                objdata->unk3B &= ~1;
                objdata->unk2C = 0.0f;
            }
        }
        break;
    case 1:
        objdata->unk3B &= ~4;
        if (objdata->unk4 < self->srt.scale) {
            objdata->unk10 = (f32) (objdata->unk10 / 1.1f);
        }
        if (objdata->unk10 < 0.00001f) {
            objdata->unk10 = 0.0f;
        }
        objdata->unk0 = (f32) (objdata->unk0 + gUpdateRateF);
        self->srt.scale += objdata->unk10 * gUpdateRateF;
        if (objdata->unk8 < objdata->unk0) {
            objdata->unk3A = 0U;
        }
        break;
    case 9:
        if (objdata->unk0 <= 0.0f) {
            func_80023BF8(self, 0x19, 0, 0, 0U, 6U);
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_745, MAX_VOLUME, &objdata->unk34, NULL, 0, NULL);
            objdata->unk0 = (f32) rand_next(0xF0, 0x12C);
        }
        objdata->unk0 = (f32) (objdata->unk0 - gUpdateRateF);
        if (objdata->unk0 <= 0.0f) {
            gDLL_13_Expgfx->vtbl->func4(self);
            objdata->unk3A = 0U;
            func_80023C6C(self);
            if (objdata->unk34 != 0) {
                gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk34);
                objdata->unk34 = 0U;
            }
        } else {
            objdata->unk30 -= gUpdateRateF;
            if (objdata->unk30 <= 0.0f) {
                sp40.transl.f[0] = 14.0f;
                sp40.transl.f[1] = 25.0f;
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_51D, &sp40, PARTFXFLAG_2, -1, NULL);
                objdata->unk30 = 20.0f;
            }
            if (self->unkAF & 1) {
                obj_send_mesg(player, 0x7000AU, self, (void* )0x119);
                objdata->unk3A = 0xAU;
                if (objdata->unk34 != 0) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk34);
                    objdata->unk34 = 0U;
                }
            }
            self->unkAF &= ~8;
        }
        break;
    case 10:
        func_800267A4(self);
        objdata->unk0 = (f32) (objdata->unk0 + gUpdateRateF);
        if ((f32) objdata->unk38 < objdata->unk0) {
            dll_503_func_DE4(self, (DLL503_Data* ) objdata, 1);
            objdata->unk3A = 1U;
            func_80023C6C(self);
        }
        break;
    case 0:
    default:
        self->unkAF &= ~8;
        temp_fv0 = player->srt.transl.f[0] - self->srt.transl.f[0];
        temp_fv1 = player->srt.transl.f[1] - self->srt.transl.f[1];
        temp_fa1 = player->srt.transl.f[2] - self->srt.transl.f[2];
        // temp var required?
        temp = sqrtf(SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa1));
        if (temp < sp58->unk1E && 
            ((((DLL_210_Player*)player->dll)->vtbl->func56(player)) >= 0.54f)) {
            objdata->unk3B &= ~1;
            objdata->unk3A = 3U;
            objdata->unk0 = 0.0f;
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_53A, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        break;
    }
    if ((func_80025F40(self, &sp7C, &sp78, &sp74) != 0) && (objdata->unk3B & 4)) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_744, MAX_VOLUME, NULL, NULL, 0, NULL);
        objdata->unk3B &= ~1;
        if (sp58->unk1C != -1) {
            main_set_bits((s32) sp58->unk1C, 1U);
        }
        objdata->unk3A = 9U;
        objdata->unk0 = 0.0f;
        if (objdata->unk34 != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk34);
            objdata->unk34 = 0U;
        }
    }
    if (self->curModAnimId != _data_0[objdata->unk3A]) {
        func_80023D30(self, _data_0[objdata->unk3A], 0.0f, 0U);
    }
    if (func_80024108(self, _data_18[objdata->unk3A], gUpdateRateF, NULL) != 0) {
        objdata->unk3B |= 2;
        return;
    }
    objdata->unk3B &= ~2;
}

// offset: 0xCDC | func: 2 | export: 2
void dll_503_update(Object *self) { }

// offset: 0xCE8 | func: 3 | export: 3
void dll_503_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DLL503_Data* objdata;

    objdata = self->data;
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_80031F6C(self, 0, &objdata->unk20, &objdata->unk24, &objdata->unk28, 0);
    }
}

// offset: 0xD70 | func: 4 | export: 4
void dll_503_free(Object* self, s32 a1) {
    gDLL_13_Expgfx->vtbl->func4(self);
}

// offset: 0xDB8 | func: 5 | export: 5
u32 dll_503_get_model_flags(Object* self) {
    DLL503_Setup* setup;

    setup = (DLL503_Setup*)self->setup;
    return MODFLAGS_MODEL_INDEX(setup->unk1F) | MODFLAGS_LOAD_SINGLE_MODEL;
}
// offset: 0xDD0 | func: 6 | export: 6
u32 dll_503_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL503_Data);
}

// offset: 0xDE4 | func: 7
static void dll_503_func_DE4(Object* arg0, DLL503_Data* arg1, s32 arg2) {
    ObjSetup* setup;
    f32 temp_fv0;

    setup = arg0->setup;
    arg0->srt.roll = rand_next(-0x5DC, 0x5DC);
    arg0->srt.pitch = rand_next(-0x5DC, 0x5DC);
    arg0->srt.yaw = rand_next(-0x5DC, 0x5DC);
    arg0->opacity = 0xFF;
    arg0->srt.flags &= 0xBFFF;
    arg0->srt.transl.f[0] = setup->x;
    arg0->srt.transl.f[1] = setup->y;
    arg0->srt.transl.f[2] = setup->z;
    if (arg2 != 0) {
        arg0->srt.scale = 0.00001f;
        arg1->unk0 = 0.0f;
        arg1->unk8 = (rand_next(0, 0x64) + 200.0f);
        temp_fv0 = (rand_next(-0x64, 0x64) * 0.001f) + arg1->unkC;
        arg1->unk4 = temp_fv0;
        arg1->unk10 =(temp_fv0 / arg1->unk8);
    }
    func_8002674C(arg0);
    func_800264D0(arg0);
}
