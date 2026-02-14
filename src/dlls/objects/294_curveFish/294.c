#include "common.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/314_foodbag.h"
#include "sys/objanim.h"

typedef struct {
    f32 unk0;
    UnkCurvesStruct unk4;
    u8 unk10C;
    u8 unk10D;
    s16 unk10E;
    f32 unk110;
    f32 unk114;
    f32 unk118;
    f32 unk11C;
    f32 unk120;
    f32 unk124;
    //[0x128];
} DLL294_Data;
typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B; 
    u16 unk1C;
    u16 unk1E;
    u16 unk20;
    u8 unk22;
    u8 unk23; 
} curveFishSetup;

/*0x0*/ static s32 _data_0 = 0x00000023;

static void dll_294_func_968(Object* self);
static void dll_294_func_6C(Object* self);

// offset: 0x0 | ctor
void dll_294_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_294_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_294_setup(Object* self, ObjSetup* setup, s32 arg2) {
    DLL294_Data* curveFishData = self->data;
    
    self->unkB0 |= 0x6000;
    curveFishData->unk10D = 0;
    dll_294_func_6C(self);
}

// offset: 0x6C | func: 1
static void dll_294_func_6C(Object* self) {
    DLL294_Data* curvefishdata;
    curveFishSetup* sp30;
    s32 sp2C;
    ObjDef* temp_a2;

    curvefishdata = self->data;
    sp30 = (curveFishSetup*)self->setup;
    sp2C = _data_0;
    if (func_80058B1C(self, self->srt.transl.f[0], self->srt.transl.f[1], self->srt.transl.f[2], &curvefishdata->unk0, 0U) != 0) {
 
        curvefishdata->unk0 = (self->srt.transl.f[1] + curvefishdata->unk0) - sp30->unk23;
        gDLL_26_Curves->vtbl->func_4288((&curvefishdata->unk4), self, 100.0f, &sp2C, -1);
        curvefishdata->unk10C = 1;
        curvefishdata->unk10D = 1;
        curvefishdata->unk10E = (s16) self->objhitInfo->unk58;
        curvefishdata->unk110 = (sp30->unk19 / 100.0f);
        curvefishdata->unk114 = (sp30->unk1A / 1000.0f);
        curvefishdata->unk118 = (sp30->unk1B / 1000.0f);
        curvefishdata->unk11C = 0.0f;
        temp_a2 = self->def;
 
        self->srt.scale = temp_a2->scale * ( sp30->unk18 / 100.0f);
        func_80026940(self, (s16) ((s32) (temp_a2->hitbox_flagsB6 * sp30->unk18) / 100));
    }
}

// offset: 0x280 | func: 2 | export: 1
void dll_294_control(Object* self) {
    DLL294_Data* curvefishdata;
    curveFishSetup* sp40;
    Object* sp3C;
    s32 temp_v0_2;
    f32 var_fa1;

    curvefishdata = self->data;
    sp40 = (curveFishSetup*)self->setup;
    sp3C = get_player();
    if (curvefishdata->unk10D == 0) {
        dll_294_func_6C(self);
    } 
    if (curvefishdata->unk10D != 0) {
        if (sp3C->id == OBJ_Krystal) {
            if (main_get_bits(BIT_2EA) == 0) {
                self->unkAF |= 8;
            } else {
                self->unkAF &= ~0x8;
            }
        } else if (main_get_bits(BIT_3E1) == 0) {
            self->unkAF |= 8;
        } else {
            self->unkAF &= ~0x8;
        }
        temp_v0_2 = func_80025F40(self, NULL, NULL, NULL);
        if ((temp_v0_2 != 0) && (temp_v0_2 == Collision_Type_Fishingnet)) {
            dll_294_func_968(self);
        } else {
            curvefishdata->unk124 += gUpdateRateF;
            curvefishdata->unk11C += (((curvefishdata->unk110 * (f32) (s8)rand_next(-0x32, 0x64)) / 1000.0f) * gUpdateRateF);
            if (curvefishdata->unk11C < 0) {
                curvefishdata->unk11C = 0.0f;
            } else if (curvefishdata->unk110 < curvefishdata->unk11C) {
                curvefishdata->unk11C = curvefishdata->unk110;
            }
            if ((curvefishdata->unk110 * 0.5f) < curvefishdata->unk11C) {
                var_fa1 = curvefishdata->unk110 - curvefishdata->unk11C;
            } else {
                var_fa1 = curvefishdata->unk11C;
            }
            curvefishdata->unk120 = (var_fa1 * 0.06f) / (curvefishdata->unk110 * 0.5f);
            func_800053B0(&curvefishdata->unk4, curvefishdata->unk11C);
            if ((curvefishdata->unk4.unk10 != 0) && (gDLL_26_Curves->vtbl->func_4704(&curvefishdata->unk4) != 0)) {
                curvefishdata->unk10C = 0;
                curvefishdata->unk124 = 0.0f;
                self->objhitInfo->unk58 = 0x100;
                self->opacity = 0;
            }
            self->srt.transl.f[0] = curvefishdata->unk4.unk68.x;
            self->srt.transl.f[2] = curvefishdata->unk4.unk68.z;
            self->srt.yaw = arctan2_f(curvefishdata->unk4.unk74, curvefishdata->unk4.unk7C) + 0x4000;
            switch (curvefishdata->unk10C) {
            case 0:
                self->unkAF |= 8;

                if ((f32) sp40->unk20 <= curvefishdata->unk124) {
                    curvefishdata->unk10C = 1U;
                    curvefishdata->unk124 = (f32) (curvefishdata->unk124 - (f32) sp40->unk20);
                }
                break;
            case 1:
                if (curvefishdata->unk124 <= 60.0f) {
                    self->unkAF |= 8;
                    self->opacity = (u8) (s32) ((curvefishdata->unk124 / 60.0f) * 255.0f);
                } else {
                    self->objhitInfo->unk58 = curvefishdata->unk10E;
                    self->opacity = 0xFF;
                    curvefishdata->unk10C = 2U;
                    curvefishdata->unk124 = (f32) (curvefishdata->unk124 - 60.0f);
                }
                break;
            case 2:
                self->srt.transl.f[1] = curvefishdata->unk4.unk68.y + (f32) sp40->unk22;
                if ((f32) sp40->unk1C < curvefishdata->unk124) {
                    curvefishdata->unk10C = 3U;
                    func_80023D30(self, 2, 0.0f, 0U);
                    func_80024D74(self, 0x1E);
                }
                break;
            case 3:
                self->srt.transl.f[1] += curvefishdata->unk114 * gUpdateRateF;
                if (curvefishdata->unk0 < self->srt.transl.f[1]) {
                    self->srt.transl.f[1] = curvefishdata->unk0;
                    curvefishdata->unk10C = 4U;
                    curvefishdata->unk124 = 0.0f;
                    func_80023D30(self, 0, 0.0f, 0U);
                    func_80024D74(self, 0x1E);
                }
                break;
            case 4:
                if ((f32) sp40->unk1E < curvefishdata->unk124) {
                    curvefishdata->unk10C = 5U;
                    func_80023D30(self, 3, 0.0f, 0U);
                    func_80024D74(self, 0x1E);
                }
                break;
            case 5:
                self->srt.transl.f[1] -= curvefishdata->unk118 * gUpdateRateF;
                if (self->srt.transl.f[1] < curvefishdata->unk4.unk68.y + (f32) sp40->unk22) {
                    self->srt.transl.f[1] = curvefishdata->unk4.unk68.y + (f32) sp40->unk22;
                    curvefishdata->unk10C = 2U;
                    curvefishdata->unk124 = 0.0f;
                    func_80023D30(self, 0, 0.0f, 0U);
                    func_80024D74(self, 0x1E);
                }
                break;
            }
            func_80024108(self, curvefishdata->unk120, gUpdateRateF, NULL);
        }
    }
}


// offset: 0x8C8 | func: 3 | export: 2
void dll_294_update(Object *self) { }

// offset: 0x8D4 | func: 4 | export: 3
void dll_294_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DLL294_Data* curvefishdata = self->data;
    
    if ((curvefishdata->unk10C != 0) && (visibility != 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}
// offset: 0x934 | func: 5 | export: 4
void dll_294_free(Object *self, s32 a1) { }

// offset: 0x944 | func: 6 | export: 5
u32 dll_294_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x954 | func: 7 | export: 6
u32 dll_294_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL294_Data);
}

// offset: 0x968 | func: 8
static void dll_294_func_968(Object* self) {
    DLL294_Data* curvefishdata;
    Object* player;
    Object* foodbag;

    curvefishdata = self->data;
    if (self->id == OBJ_DF_Lantern) {
        main_set_bits(BIT_Gold_Nugget_LFV, 1);
    } else {
        player = get_player();
        foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0xF);
        ((DLL_314_Foodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Fish);
    }
    curvefishdata->unk124 = 0.0f;
    curvefishdata->unk10C = 0;
    self->objhitInfo->unk58 = 0x100;
    self->opacity = 0;
}


/*0x0*/ static const char str_0[] = "fish has reached the end of it life\n";
