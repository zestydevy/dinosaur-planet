#include "dlls/engine/27.h"
#include "dlls/engine/4_race.h"
#include "dlls/objects/214_animobj.h"
#include "game/gamebits.h"
#include "game/objects/object_id.h"
#include "sys/dll.h"
#include "sys/gfx/modgfx.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/objtype.h"
#include "sys/objanim.h"
#include "sys/rand.h"
#include "dll.h"
#include "functions.h"
#include "prevent_bss_reordering.h"

typedef struct {
/*00*/ Vec3f unk0;
/*0C*/ Vec3f unkC;
/*18*/ f32 unk18; // also gravity kinda?
/*1C*/ f32 unk1C;
/*20*/ f32 unk20;
/*24*/ f32 _unk24;
/*28*/ f32 unk28; // gravity?
/*2C*/ f32 unk2C;
/*30*/ f32 unk30; // friction?
} IMSnowBike_Data_2AC;

typedef struct {
/*00*/ u8 _unk0[0xE - 0x0];
/*0E*/ s8 turnInput; // for players, this is joystick X
/*0F*/ s8 thrustInput; // for players, this is joystick Y
/*10*/ s32 unk10;
} IMSnowBike_Data_2E0;

typedef struct {
/*000*/ SRT unk0;
/*018*/ RaceStruct unk18; // racePos
/*03C*/ u8 _unk3C[0x48 - 0x3C];
/*048*/ u8 unk48;
/*049*/ u8 unk49;
/*04A*/ u8 _unk4A[0x4C - 0x4A];
/*04C*/ DLL27_Data unk4C;
/*2AC*/ IMSnowBike_Data_2AC unk2AC;
/*2E0*/ IMSnowBike_Data_2E0 unk2E0;
/*2F4*/ DLL_IModgfx *unk2F4;
/*2F8*/ DLL_IModgfx *unk2F8;
/*2FC*/ u8 _unk2FC[0x32C - 0x2FC];
/*32C*/ Vec3f unk32C[5];
/*368*/ u8 _unk368[0x380 - 0x368];
/*380*/ f32 unk380;
/*384*/ f32 unk384;
/*388*/ f32 unk388;
/*38C*/ f32 unk38C;
/*390*/ f32 unk390;
/*394*/ f32 unk394;
/*398*/ f32 unk398;
/*39C*/ f32 unk39C;
/*3A0*/ f32 unk3A0;
/*3A4*/ f32 unk3A4;
/*3A8*/ f32 unk3A8;
/*3AC*/ f32 unk3AC;
/*3B0*/ f32 unk3B0;
/*3B4*/ u32 unk3B4; // sound handle
/*3B8*/ u32 unk3B8;
/*3BC*/ u32 unk3BC;
/*3C0*/ u32 unk3C0;
/*3C4*/ s32 unk3C4;
/*3C8*/ s16 unk3C8;
/*3CA*/ s16 unk3CA;
/*3CC*/ s16 unk3CC;
/*3CE*/ s16 unk3CE;
/*3D0*/ s16 unk3D0;
/*3D2*/ s16 unk3D2; // downward tilt while in air?
/*3D4*/ u8 _unk3D4[0x3D6 - 0x3D4];
/*3D6*/ s16 unk3D6;
/*3D8*/ s16 unk3D8;
/*3DA*/ s8 unk3DA;
/*3DB*/ u8 unk3DB;
/*3DC*/ u8 unk3DC;
/*3DD*/ u8 flags;
/*3DE*/ s8 unk3DE; // state?
/*3DF*/ s8 unk3DF;
/*3E0*/ s8 unk3E0;
/*3E1*/ s8 unk3E1;
} IMSnowBike_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 unk18;
/*19*/ u8 unk19;
/*1A*/ s16 unk1A;
/*1C*/ u8 unk1C;
/*1D*/ u8 unk1D;
/*1E*/ s16 unk1E;
} IMSnowBike_Setup;

enum IMSnowBikeFlags {
    SNOWBIKEFLAG_NONE = 0x0,
    SNOWBIKEFLAG_1 = 0x1,
    SNOWBIKEFLAG_2 = 0x2,
    SNOWBIKEFLAG_GROUNDED = 0x4, // at least one test point touching the ground
    SNOWBIKEFLAG_8 = 0x8,
    SNOWBIKEFLAG_10 = 0x10, // currently in sequence?
    SNOWBIKEFLAG_IS_CPU = 0x20 // is SharpClaw
};

/*0x0*/ static Vec3f _data_0[] = {
    {-6.5f, 0.0f, -7.0f}, 
    { 6.5f, 0.0f, -7.0f}, 
    { 6.5f, 0.0f,  7.0f}, 
    {-6.5f, 0.0f,  7.0f}
};
/*0x30*/ static f32 _data_30[] = {
    0.0f, 0.0f, 0.0f, 0.0f
};
/*0x40*/ static Vec3f _data_40[] = {
    {0.0f, 1.0f, -18.0f}, 
    {-6.5f, 1.0f, -7.0f}, 
    {6.5f, 1.0f, -7.0f}, 
    {-6.0f, 1.0f, 11.0f}, 
    {6.0f, 1.0f, 11.0f}
};
/*0x7C*/ static f32 _data_7C[] = {
    5.0f, 4.0f, 4.0f, 4.5f, 4.5f
};
/*0x90*/ static Vec3f _data_90[] = {
    {14.5f, 0.0f, 9.0f}, 
    {-14.5f, 0.0f, 9.0f}
};
/*0xA8*/ static DLL_IModgfx *_data_A8 = NULL;

/*0x0*/ static Texture *_bss_0;
/*0x4*/ static Texture *_bss_4;
/*0x8*/ static SRT _bss_8;

void dll_711_func_1760(Object *self);
static void dll_711_func_1870(Object *self, IMSnowBike_Data *objdata, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols);
static void dll_711_func_1ED0(Object *self, IMSnowBike_Data_2AC *a1);
static void dll_711_func_1F54(Object *self, IMSnowBike_Data *objdata, IMSnowBike_Data_2AC *arg2, f32 updateRate, s32 arg4);
static void dll_711_func_2BA0(Object *self, IMSnowBike_Data *objdata, IMSnowBike_Data_2AC *arg2, f32 updateRate, s32 arg4);
static void dll_711_func_33B4(Object *self, IMSnowBike_Data_2E0 *arg1, u8 port, s32 buffer);
static void dll_711_func_3430(Object *self, IMSnowBike_Data *objdata, MtxF *a2, s32 a3, s32 a4, s32 a5);
static void dll_711_func_34E4(Object *self, IMSnowBike_Data *objdata);
static int dll_711_func_3598(Object *a0, Object *a1, AnimObj_Data *a2, s8 a3);
static void dll_711_func_3780(Object *self, IMSnowBike_Data *objdata, DLL27_Data *arg2);
static void dll_711_func_3C30(Object *self, IMSnowBike_Data *objdata);
static void dll_711_func_3D4C(Object *self, IMSnowBike_Data *objdata, f32 arg2, s32 arg3, s32 *arg5, u8 arg6);
static s32 dll_711_func_3A34(Object *arg0, void *arg1, IMSnowBike_Data *arg2, IMSnowBike_Data_2E0 *arg3);

// offset: 0x0 | ctor
void dll_711_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_711_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_711_setup(Object *self, IMSnowBike_Setup *setup, s32 arg2) {
    IMSnowBike_Data *objdata = (IMSnowBike_Data*)self->data;
    IMSnowBike_Setup *setup2 = (IMSnowBike_Setup*)self->setup;
    s32 _pad[2];
    u8 sp3C[] = {0x05, 0x05, 0x05, 0x05}; // _data_AC
    s32 var_v0;

    bzero(objdata, sizeof(IMSnowBike_Data));
    objdata->unk3CC = (s16) ((setup->unk18 & 0xFFFF) << 8);
    objdata->unk2F4 = 0;
    objdata->unk2F8 = 0;
    _data_A8 = (DLL_IModgfx*)dll_load_deferred(DLL_ID_106, 1);
    self->srt.yaw = objdata->unk3CC;
    self->animCallback = dll_711_func_3598;
    gDLL_27->vtbl->init(&objdata->unk4C, 
        DLL27FLAG_NONE, 
        DLL27FLAG_40000 | DLL27FLAG_4 | DLL27FLAG_2 | DLL27FLAG_1, 
        1);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->unk4C, 4, _data_0, _data_30, sp3C);
    if (setup->unk19 != 0) {
        objdata->unk3DC = 3;
    } else {
        objdata->unk3DC = 5;
    }
    dll_711_func_3C30(self, objdata);
    dll_711_func_1ED0(self, &objdata->unk2AC);
    func_80023D30(self, 0, 0.0f, 0);
    if (self->shadow != NULL) {
        self->shadow->flags |= (OBJ_SHADOW_FLAG_4000 | OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_DIR);
    }
    if (setup->unk19 != 0) {
        objdata->flags |= SNOWBIKEFLAG_IS_CPU;
    }
    objdata->unk18.unk10 = -1;
    objdata->unk18.unk14 = -1;
    objdata->unk18.unk18 = -1;
    objdata->unk48 = setup->unk1C;
    objdata->unk49 = setup->unk1D;
    objdata->unk0.transl.x = self->srt.transl.x;
    objdata->unk0.transl.y = self->srt.transl.y;
    objdata->unk0.transl.z = self->srt.transl.z;
    obj_add_object_type(self, OBJTYPE_11);
    _bss_0 = queue_load_texture_proxy(0x89);
    _bss_4 = queue_load_texture_proxy(0x3C);
    if (main_get_bits(setup2->unk1E) != 0) {
        var_v0 = SNOWBIKEFLAG_1;
    } else {
        var_v0 = SNOWBIKEFLAG_NONE;
    }
    objdata->flags |= var_v0;
}

// offset: 0x2A0 | func: 1 | export: 1
void dll_711_control(Object *self) {
    IMSnowBike_Data *objdata;
    IMSnowBike_Setup *setup;
    Object *player;
    f32 spB8;
    MtxF sp78;
    f32 sp74;
    f32 sp70;
    f32 sp6C;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 i;
    s32 _pad1;
    s32 _pad2;
    s32 sp54;
    IMSnowBike_Data_2AC *sp44;
    IMSnowBike_Data_2E0 *sp40;
    
    objdata = self->data;
    setup = (IMSnowBike_Setup*)self->setup;
    sp44 = &objdata->unk2AC;
    sp40 = &objdata->unk2E0;
    spB8 = 0.0f;
    if ((objdata->flags & SNOWBIKEFLAG_1)) {
        return;
    }
    if (main_get_bits(setup->unk1E) != 0) {
        objdata->flags |= SNOWBIKEFLAG_1;
        return;
    }
    if (self->unk58 != NULL) {
        if (objdata->unk3DE == 0) {
            if (self->objhitInfo->unk5A != 8) {
                self->objhitInfo->unk9F = 2;
                self->objhitInfo->unk5A = 8;
                func_80026B84(self);
                return;
            }
        } else {
            if (self->objhitInfo->unk5A != 1) {
                self->objhitInfo->unk9E = 1;
                self->objhitInfo->unk5A = 1;
                return;
            }
        }
    }

    player = get_player();
    self->unkAF |= 8;
    objdata->unk39C = self->srt.transl.x;
    objdata->unk3A0 = self->srt.transl.y;
    objdata->unk3A4 = self->srt.transl.z;
    if (objdata->flags & SNOWBIKEFLAG_10) {
        objdata->flags = objdata->flags & ~SNOWBIKEFLAG_10;
        if (!(objdata->flags & SNOWBIKEFLAG_IS_CPU)) {
            sp44->unkC.x = 0.0f;
            sp44->unkC.y = 0.0f;
            sp44->unkC.z = -2.0f;
            objdata->unk3C8 = 0;
            objdata->unk3CA = 0;
            objdata->unk3E0 = 0;
            objdata->unk3D2 = 0;
            objdata->unk3CC = self->srt.yaw;
            objdata->unk3CE = self->srt.pitch;
            objdata->unk3D0 = self->srt.roll;
            dll_711_func_3C30(self, objdata);
        }
    }
    switch (objdata->unk3DE) {
    case 0:
        if (!(objdata->flags & SNOWBIKEFLAG_IS_CPU)) {
            self->objhitInfo->unk5B = 0;
            self->objhitInfo->unk5C = 0;
            objdata->unk3DB = 0;
            if ((setup->unk1A == -1) || (main_get_bits(setup->unk1A) != 0)) {
                self->unkAF &= ~0x10;
            } else {
                self->unkAF |= 0x10;
            }
            if ((player != NULL) && (vec3_distance(&player->positionMirror, &self->positionMirror) < 50.0f)) {
                dll_711_func_3430(self, objdata, &sp78, 0, 1, 1);
                vec3_transform(&sp78, _data_90[0].x, _data_90[0].y, _data_90[0].z, &sp74, &sp70, &sp6C);
                temp_fv0 = player->srt.transl.x - sp74;
                temp_fv1 = player->srt.transl.z - sp6C;
                if ((SQ(temp_fv0) + SQ(temp_fv1)) < 100.0f) {
                    self->unkAF &= ~0x8;
                    objdata->unk3DB = 2;
                } else {
                    vec3_transform(&sp78, _data_90[1].x, _data_90[1].y, _data_90[1].z, &sp74, &sp70, &sp6C);
                    temp_fv0 = player->srt.transl.x - sp74;
                    temp_fv1 = player->srt.transl.z - sp6C;
                    if ((SQ(temp_fv0) + SQ(temp_fv1)) < 100.0f) {
                        self->unkAF &= ~0x8;
                        objdata->unk3DB = 1;
                    }
                }
            }
        }
        if (objdata->unk3B8 != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk3B8);
            objdata->unk3B8 = 0;
        }
        if (objdata->unk3BC != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk3BC);
            objdata->unk3BC = 0;
        }
        if (objdata->unk3C0 != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk3C0);
            objdata->unk3C0 = 0;
        }
        if (objdata->unk2F4 != NULL) {
            dll_unload(objdata->unk2F4);
            objdata->unk2F4 = NULL;
        }
        if (objdata->unk2F8 != NULL) {
            dll_unload(objdata->unk2F8);
            objdata->unk2F8 = NULL;
            return;
        }
        break;
    case 1:
        break;
    case 2:
        if (!(objdata->flags & SNOWBIKEFLAG_8)) {
            objdata->unk18.unk10 = -1;
            objdata->unk18.unk14 = -1;
            objdata->unk18.unk18 = -1;
            objdata->unk18.unk1C = 0;
            if (main_get_bits(BIT_IM_Race_Started) != 0) {
                objdata->flags |= SNOWBIKEFLAG_8;
            } else {
                objdata->flags &= ~SNOWBIKEFLAG_8;
            }
            if (objdata->flags & SNOWBIKEFLAG_8) {
                if (objdata->flags & SNOWBIKEFLAG_IS_CPU) {
                    dll_711_func_1760(self);
                } else {
                    gDLL_4_Race->vtbl->func3(self, &objdata->unk18, 0);
                }
                gDLL_4_Race->vtbl->func9(&objdata->unk18);
            }
        } else if (main_get_bits(BIT_IM_Race_Ended) != 0) {
            objdata->flags &= ~SNOWBIKEFLAG_8;
        }
        dll_711_func_34E4(self, objdata);
        if (objdata->flags & SNOWBIKEFLAG_IS_CPU) {
            if (objdata->flags & SNOWBIKEFLAG_8) {
                if (func_8004454C(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z) >= 0) {
                    if (objdata->flags & SNOWBIKEFLAG_2) {
                        sp54 = gDLL_4_Race->vtbl->func5(&objdata->unk0, &objdata->unk18, 2.8f * gUpdateRateF, 1, 1, 0);
                        gDLL_4_Race->vtbl->func4(self, &objdata->unk18);
                        gDLL_4_Race->vtbl->func10(&objdata->unk18);
                        if (sp54 == 0) {
                            self->srt.yaw = arctan2_f(self->srt.transl.x - objdata->unk0.transl.x, self->srt.transl.z - objdata->unk0.transl.z);
                            self->srt.transl.x = objdata->unk0.transl.x;
                            self->srt.transl.y = objdata->unk0.transl.y;
                            self->srt.transl.z = objdata->unk0.transl.z;
                            sp44->unkC.x = 0.0f;
                            sp44->unkC.y = 0.0f;
                            sp44->unkC.z = -2.0f;
                            objdata->unk3C8 = 0;
                            objdata->unk3CA = 0;
                            objdata->unk3E0 = 0;
                            objdata->unk3D2 = 0;
                            objdata->unk3CC = self->srt.yaw;
                            objdata->unk3CE = self->srt.pitch;
                            objdata->unk3D0 = self->srt.roll;
                            dll_711_func_3C30(self, objdata);
                            func_80058680(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &spB8, 0);
                            self->srt.transl.y -= spB8;
                            objdata->flags &= ~SNOWBIKEFLAG_2;
                        }
                        return;
                    } else if (dll_711_func_3A34(self, objdata, objdata, &objdata->unk2E0) != 0) {
                        return;
                    }
                } else {
                    sp54 = gDLL_4_Race->vtbl->func5(&objdata->unk0, &objdata->unk18, 2.8f * gUpdateRateF, 1, 1, 0);
                    gDLL_4_Race->vtbl->func4(self, &objdata->unk18);
                    gDLL_4_Race->vtbl->func10(&objdata->unk18);
                    if (sp54 == 0) {
                        self->srt.yaw = arctan2_f(self->srt.transl.x - objdata->unk0.transl.x, self->srt.transl.z - objdata->unk0.transl.z);
                        self->srt.transl.x = objdata->unk0.transl.x;
                        self->srt.transl.y = objdata->unk0.transl.y;
                        self->srt.transl.z = objdata->unk0.transl.z;
                        objdata->flags |= SNOWBIKEFLAG_2;
                    }
                    return;
                }
            } else {
                return;
            }
        } else {
            self->objhitInfo->unk5B = 0xA;
            self->objhitInfo->unk5C = 0xA;
        }
        
        if (!(objdata->flags & SNOWBIKEFLAG_IS_CPU)) {
            if (objdata->flags & SNOWBIKEFLAG_8) {
                objdata->unk3C4 = gDLL_4_Race->vtbl->func4(self, &objdata->unk18);
                gDLL_4_Race->vtbl->func10(&objdata->unk18);
                objdata->unk3DF = gDLL_4_Race->vtbl->func12(&objdata->unk18);
            }
            for (i = 0; i < gUpdateRate; i++) {
                dll_711_func_33B4(self, sp40, 0, i);
                dll_711_func_1F54(self, objdata, sp44, gUpdateRateF, (i + 1) == gUpdateRate);
                objdata->unk3C8 += (s16) (((f32) -objdata->unk2E0.turnInput * 60.0f) - (f32) objdata->unk3C8) >> 4;
                objdata->unk3CA += (s16) (((f32) -objdata->unk2E0.turnInput * 105.0f) - (f32) objdata->unk3CA) >> 4;
                self->srt.yaw = objdata->unk3CC + objdata->unk3C8;
                self->srt.roll = objdata->unk3D0 + objdata->unk3CA;
            }
        } else {
            for (i = 0; i < gUpdateRate; i++) {
                dll_711_func_2BA0(self, objdata, sp44, gUpdateRateF, (i + 1) == gUpdateRate);
                objdata->unk3C8 += (s16) (((f32) -objdata->unk2E0.turnInput * 60.0f) - (f32) objdata->unk3C8) >> 4;
                objdata->unk3CA += (s16) (((f32) -objdata->unk2E0.turnInput * 105.0f) - (f32) objdata->unk3CA) >> 4;
                self->srt.yaw = objdata->unk3CC + objdata->unk3C8;
                self->srt.roll = objdata->unk3D0 + objdata->unk3CA;
            }
        }
        if (!(objdata->flags & SNOWBIKEFLAG_IS_CPU)) {
            dll_711_func_3D4C(self, objdata, objdata->unk2AC.unkC.z, sp40->thrustInput, &sp40->unk10, 7);
            return;
        }
        if (objdata->unk3B4 != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk3B4);
            objdata->unk3B4 = 0;
        }
        if (objdata->unk3B8 != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk3B8);
            objdata->unk3B8 = 0;
        }
        if (objdata->unk3BC != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk3BC);
            objdata->unk3BC = 0;
        }
        if (objdata->unk3C0 != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk3C0);
            objdata->unk3C0 = 0;
        }
        
        break;
    }
}

// offset: 0xF34 | func: 2 | export: 2
void dll_711_update(Object *self) {
    IMSnowBike_Data *objdata;
    IMSnowBike_Data_2AC *temp_v0;
    Vec3f spBC;
    MtxF sp7C;
    MtxF sp3C;

    objdata = self->data;
    temp_v0 = &objdata->unk2AC;
    
    if (!(objdata->flags & SNOWBIKEFLAG_IS_CPU)) {
        _bss_8.yaw = -objdata->unk3CC;
        _bss_8.pitch = -objdata->unk3CE;
        _bss_8.roll = -objdata->unk3D0;
        matrix_from_srt_reversed(&sp7C, &_bss_8);
        self->speed.x = (self->srt.transl.x - self->positionMirror2.x) * gUpdateRateInverseF;
        self->speed.y = (self->srt.transl.y - self->positionMirror2.y) * gUpdateRateInverseF;
        self->speed.z = (self->srt.transl.z - self->positionMirror2.z) * gUpdateRateInverseF;
        spBC.f[0] = self->speed.x * 0.93749994f;
        spBC.f[1] = self->speed.y * 0.93749994f;
        spBC.f[2] = self->speed.z * 0.93749994f;
        vec3_transform(&sp7C, spBC.f[0], spBC.f[1], spBC.f[2], &temp_v0->unkC.x, &temp_v0->unkC.y, &temp_v0->unkC.z);
        dll_711_func_3430(self, objdata, &sp3C, 0, 0, 0);
        vec3_transform(&sp3C, 0.0f, 0.0f, -10.0f, &objdata->unk3A8, &objdata->unk3AC, &objdata->unk3B0);
    }
}

// offset: 0x10D4 | func: 3 | export: 3
void dll_711_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    IMSnowBike_Data *objdata = self->data;
    s8 sp3B;
    
    if (!(objdata->flags & SNOWBIKEFLAG_1) && ((main_get_bits(BIT_Played_Seq_0063_IM_Sabre_Intro) == 0) || (main_get_bits(BIT_382) != 0))) {
        if (visibility == -1) {
            sp3B = gDLL_13_Expgfx->vtbl->func10(self);
            if (!(objdata->flags & SNOWBIKEFLAG_IS_CPU)) {
                dll_711_func_1870(self, objdata, gdl, mtxs, vtxs, pols);
            }
            draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
            func_80031F6C(self, 0, &objdata->unk384, &objdata->unk388, &objdata->unk38C, 0);
            if (sp3B != 0) {
                gDLL_13_Expgfx->vtbl->func6(self, gdl, mtxs, NULL, 0, 0, 0);
            }
        } else {
            if (objdata->unk3DE == 2) {
                objdata->unk384 = self->srt.transl.x;
                objdata->unk388 = self->srt.transl.y;
                objdata->unk38C = self->srt.transl.z;
                return;
            }
            if (visibility != 0) {
                draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
                func_80031F6C(self, 0, &objdata->unk384, &objdata->unk388, &objdata->unk38C, 0);
            }
        }
    }
}

// offset: 0x12F4 | func: 4 | export: 4
void dll_711_free(Object *self, s32 a1) {
    IMSnowBike_Data *objdata;

    objdata = self->data;
    if (objdata->unk3B4 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk3B4);
        objdata->unk3B4 = 0;
    }
    if (objdata->unk3B8 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk3B8);
        objdata->unk3B8 = 0;
    }
    if (objdata->unk3BC != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk3BC);
        objdata->unk3BC = 0;
    }
    if (objdata->unk3C0 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk3C0);
        objdata->unk3C0 = 0;
    }
    if (objdata->unk2F4 != NULL) {
        dll_unload(objdata->unk2F4);
    }
    objdata->unk2F4 = 0;
    if (objdata->unk2F8 != NULL) {
        dll_unload(objdata->unk2F8);
    }
    objdata->unk2F8 = 0;
    if (_data_A8 != NULL) {
        dll_unload(_data_A8);
    }
    obj_free_object_type(self, OBJTYPE_11);
    texture_destroy(_bss_0);
    texture_destroy(_bss_4);
}

// offset: 0x14B0 | func: 5 | export: 5
u32 dll_711_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW;
}

// offset: 0x14C0 | func: 6 | export: 6
u32 dll_711_get_data_size(Object *self, u32 a1) {
    return sizeof(IMSnowBike_Data);
}

// offset: 0x14D4 | func: 7 | export: 7
s32 dll_711_func_14D4(Object *self, Object *rider) {
    IMSnowBike_Data *objdata = (IMSnowBike_Data*)self->data;

    if (objdata->flags & SNOWBIKEFLAG_IS_CPU) {
        return 0;
    }
    return objdata->unk3DB;
}

// offset: 0x1500 | func: 8 | export: 8
s32 dll_711_func_1500(Object *self) {
    IMSnowBike_Data *objdata = (IMSnowBike_Data*)self->data;
    return objdata->unk3DB;
}

// offset: 0x1510 | func: 9 | export: 9
void dll_711_func_1510(Object *self, f32 *a1, f32 *a2, f32 *a3) {
    IMSnowBike_Data *objdata = (IMSnowBike_Data*)self->data;
    *a1 = objdata->unk384;
    *a2 = objdata->unk388;
    *a3 = objdata->unk38C;
}

// offset: 0x1534 | func: 10 | export: 10
s32 dll_711_func_1534(Object *self, Object *rider) {
    IMSnowBike_Data *objdata = (IMSnowBike_Data*)self->data;
    if (objdata->flags & SNOWBIKEFLAG_8) {
        return 0;
    }
    if (!(joy_get_pressed(0) & B_BUTTON)) {
        return 0;
    }
    return 2;
}

// offset: 0x15A4 | func: 11 | export: 11
s32 dll_711_func_15A4(Object *self) {
    return 2;
}

// offset: 0x15B4 | func: 12 | export: 12
void dll_711_func_15B4(Object *self, f32 *a1, f32 *a2, f32 *a3) {
    IMSnowBike_Data *objdata = (IMSnowBike_Data*)self->data;
    *a1 = objdata->unk3A8;
    *a2 = objdata->unk3AC;
    *a3 = objdata->unk3B0;
}

// offset: 0x15D8 | func: 13 | export: 13
s32 dll_711_func_15D8(Object *self) {
    IMSnowBike_Data *objdata = (IMSnowBike_Data*)self->data;
    return objdata->unk3DE;
}

// offset: 0x15E8 | func: 14 | export: 14
void dll_711_func_15E8(Object *self, s32 arg1) {
    IMSnowBike_Data *objdata = self->data;

    objdata->unk3DE = arg1;
    if (arg1 == 2) {
        main_set_bits(BIT_42D, 1);
    }
}

// offset: 0x1638 | func: 15 | export: 15
void dll_711_func_1638(Object *self, f32 *arg1, s32 *arg2) {
    IMSnowBike_Data *objdata = self->data;
    
    *arg1 = (f32) objdata->unk3C8 / 2500.0f;
    if (*arg1 > 1.0f) {
        *arg1 = 1.0f;
    } else if (*arg1 < -1.0f) {
        *arg1 = -1.0f;
    }
    *arg2 = objdata->unk3C8 < 0;
}

// offset: 0x16C4 | func: 16 | export: 16
f32 dll_711_func_16C4(Object *self, f32 *arg1) {
    IMSnowBike_Data *objdata;
    f32 magnitude;

    objdata = self->data;
    *arg1 = 5.0f;
    magnitude = VECTOR_MAGNITUDE(objdata->unk2AC.unkC) * 0.2f;
    if (magnitude > 1.0f) {
        magnitude = 1.0f;
    }
    return magnitude;
}

// offset: 0x1750 | func: 17 | export: 17
s32 dll_711_func_1750(Object *self) {
    IMSnowBike_Data *objdata = (IMSnowBike_Data*)self->data;
    return objdata->unk3DF;
}

// offset: 0x1760 | func: 18 | export: 18
void dll_711_func_1760(Object *self) {
    IMSnowBike_Data *objdata;
    RaceCheckpointSetup *checkpoint;
    s32 uid;

    objdata = (IMSnowBike_Data*)self->data;
    if (self->id == OBJ_IMSnowClawBike || self->id != OBJ_IMSnowClawBike2) {
        uid = 0xCA3; // IM checkpoint setup UID
    } else {
        uid = 0xCB3; // IM checkpoint setup UID
    }
    checkpoint = (RaceCheckpointSetup*)func_80044448(uid, NULL, NULL, NULL, NULL);
    if (checkpoint != NULL) {
        self->srt.transl.x = checkpoint->pos.x;
        self->srt.transl.y = checkpoint->pos.y;
        self->srt.transl.z = checkpoint->pos.z;
        self->srt.yaw = checkpoint->unk29 << 8;
        dll_711_func_3C30(self, objdata);
        gDLL_4_Race->vtbl->func3(self, &objdata->unk18, 0);
        objdata->unk0.transl.x = self->srt.transl.x;
        objdata->unk0.transl.y = self->srt.transl.y;
        objdata->unk0.transl.z = self->srt.transl.z;
        objdata->unk0.yaw = self->srt.yaw;
    }
}

// offset: 0x1860 | func: 19 | export: 19
void dll_711_func_1860(Object *self, f32 scale) { }

// offset: 0x1870 | func: 20
static void dll_711_func_1870(Object *self, IMSnowBike_Data *objdata, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols) {
    static u32 soundHandle; // bss+0x20
    Vertex *spA4;
    Gfx *spA0;
    Triangle *sp9C;
    f32 var_fv0;
    s32 pad[5];
    SRT sp6C;
    s32 sp68;
    s32 pad_sp54[3];
    IMSnowBike_Data_2AC *sp3C;
    u32 sp48[] = {0x00000006, 0x00000069, 0x00000069, 0x000000ff};
    s32 volume;

    sp3C = &objdata->unk2AC;
    spA0 = *gdl;
    spA4 = *vtxs;
    sp9C = *pols;
    var_fv0 = 0.0f;
    if (sp3C->unkC.z < 0.0f) {
        var_fv0 = sp3C->unkC.z;
    }
    sp6C.transl.z = var_fv0;
    if (sp3C->unkC.z < 0.0f) {
        var_fv0 = sp3C->unkC.x;
    }
    sp6C.transl.x = var_fv0;
    dl_set_prim_color(&spA0, 0xFF, 0xFF, 0xFF, 0xFF);
    if (sp3C->unkC.z < -0.5f) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_12E, &sp6C, 4, -1, NULL);
    }
    if (sp3C->unkC.z < -1.5f) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_12F, &sp6C, 4, -1, NULL);
    }
    if (sp3C->unkC.z < -2.1f) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_130, &sp6C, 4, -1, NULL);
    }
    sp68 = 0;
    if (objdata->unk2E0.thrustInput > 0) {
        sp68 = 10;
    }
    while (sp68 != 0) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_131, NULL, 4, -1, NULL);
        sp68 -= 1;
    }
    sp6C.yaw = 0;
    sp6C.pitch = 0;
    sp6C.roll = 0;
    sp6C.scale = 1.0f;
    objdata->unk3D8 -= gUpdateRate;
    if (objdata->unk398 < -1.2f) {
        sp48[1] += rand_next(0, 155);
        sp48[2] += rand_next(0, 155);
        volume = (0.0f - objdata->unk398) * 21.0f;
        if ((objdata->unk3E1 & 0xF) && (objdata->unk3D8 <= 0)) {
            sp6C.transl.x = objdata->unk32C[0].x - self->positionMirror.x;
            sp6C.transl.y = objdata->unk32C[0].y - self->positionMirror.y;
            sp6C.transl.z = objdata->unk32C[0].z - self->positionMirror.z;
            _data_A8->vtbl->func0(self, 0, &sp6C, 1, -1, sp48);
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_292, volume, &soundHandle, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->func_954(soundHandle, (volume / 127.0f) + 0.5f);
        } else if ((objdata->unk3E1 & 2) && (objdata->unk3D8 <= 0)) {
            sp6C.transl.x = objdata->unk32C[1].x;
            sp6C.transl.y = objdata->unk32C[1].y;
            sp6C.transl.z = objdata->unk32C[1].z;
            _data_A8->vtbl->func0(self, 0, &sp6C, 1, -1, sp48);
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_292, volume, &soundHandle, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->func_954(soundHandle, (volume / 127.0f) + 0.5f);
        } else if ((objdata->unk3E1 & 4) && (objdata->unk3D8 <= 0)) {
            sp6C.transl.x = objdata->unk32C[2].x;
            sp6C.transl.y = objdata->unk32C[2].y;
            sp6C.transl.z = objdata->unk32C[2].z;
            _data_A8->vtbl->func0(self, 0, &sp6C, 1, -1, sp48);
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_292, volume, &soundHandle, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->func_954(soundHandle, (volume / 127.0f) + 0.5f);
        }
    }
    if (objdata->unk3D8 <= 0) {
        objdata->unk3D8 = 0x1E;
    }
    *gdl = spA0;
    *vtxs = spA4;
    *pols = sp9C;
}



// offset: 0x1ED0 | func: 21
static void dll_711_func_1ED0(Object *self, IMSnowBike_Data_2AC *a1) {
    f32 one;

    one = 1.0f;
    a1->unk1C = 90.0f;
    a1->unk28 = -11.0f;
    a1->unk20 = one / 90.0f;
    a1->unk2C = 0.08f;
    a1->unk30 = 13.0f;
    a1->unk18 = (one / 90.0f) * 0.01666666f;
    _bss_8.transl.x = 0.0f;
    _bss_8.transl.y = 0.0f;
    _bss_8.transl.z = 0.0f;
    _bss_8.scale = 1.0f;
}

// offset: 0x1F54 | func: 22
static void dll_711_func_1F54(Object *self, IMSnowBike_Data *objdata, IMSnowBike_Data_2AC *arg2, f32 updateRate, s32 arg4) {
    MtxF sp140;
    MtxF sp100;
    MtxF spC0;
    f32 temp2;
    DLL27_Data *dll27Data;
    f32 temp3;
    f32 temp4;
    f32 temp1;
    f32 var_fa0;
    f32 spA4;
    f32 var_fv0;
    Vec3f sp94;
    f32 var_fv1;
    s32 var_v0;
    s32 pad2;
    Vec3f sp7C;
    Vec3f sp70;
    s32 temp_a2;
    s32 pad;
    s32 sp64;
    s32 var_a0;
    s32 var_v1;
    f32 temp;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;

    dll27Data = &objdata->unk4C;
    if (objdata->unk3DA != 0) {
        VECTOR_SCALE(arg2->unkC, objdata->unk380);
        objdata->unk3DA--;
        if (objdata->unk3DA < 0) {
            objdata->unk3DA = 0;
        }
    }
    if (arg2->unkC.x > 2.0f) {
        arg2->unkC.x = 2.0f;
    }
    if (arg2->unkC.x < -2.0f) {
        arg2->unkC.x = -2.0f;
    }
    if (arg2->unkC.y > 4.0f) {
        arg2->unkC.y = 4.0f;
    }
    if (arg2->unkC.y < -4.0f) {
        arg2->unkC.y = -4.0f;
    }
    if (arg2->unkC.z > 2.0f) {
        arg2->unkC.z = 2.0f;
    }
    if (arg2->unkC.z < -4.6f) {
        arg2->unkC.z = -4.6f;
    }
    _bss_8.yaw = objdata->unk3CC;
    _bss_8.pitch = objdata->unk3CE;
    _bss_8.roll = objdata->unk3D0;
    matrix_from_srt(&sp140, &_bss_8);
    _bss_8.yaw = -objdata->unk3CC;
    _bss_8.pitch = -objdata->unk3CE;
    _bss_8.roll = -objdata->unk3D0;
    matrix_from_srt_reversed(&sp100, &_bss_8);
    if (!(objdata->flags & SNOWBIKEFLAG_GROUNDED)) {
        var_fv0 = (f32) -objdata->unk2E0.thrustInput / 60.0f;
        if (var_fv0 > 1.0f) {
            var_fv0 = 1.0f;
        } else if (var_fv0 < -1.0f) {
            var_fv0 = -1.0f;
        }
        var_fv0 *= 6144.0f;
        objdata->unk3D2 += ((s32) var_fv0 - objdata->unk3D2) >> 5;
    } else {
        if (objdata->unk3D2 != 0) {
            objdata->unk3D2 = objdata->unk3D2 - (-objdata->unk3D2 >> 5);
        }
    }
    // make down/gravity vec? relative to bike orientation
    vec3_transform(&sp100, 0.0f, arg2->unk28 * arg2->unk1C, 0.0f, &sp94.x, &sp94.y, &sp94.z);
    if (objdata->unk2E0.thrustInput < 0) {
        // joystick held back
        var_fv1 = -(f32) objdata->unk2E0.thrustInput * 4.0f;
    } else {
        // joystick held forward
        var_fv1 = -(f32) objdata->unk2E0.thrustInput * 6.0f;
    }
    // factor in thruster
    var_fv0 = arg2->unk20 * var_fv1;
    if (var_fv0 < 0.0f) {
        arg2->unkC.z += var_fv0 * 0.01666666f;
    } else if (arg2->unkC.z <= 0.0f) {
        arg2->unkC.z += var_fv0 * 0.01666666f;
        if (arg2->unkC.z > 0.0f) {
            arg2->unkC.z = 0.0f;
        }
    }
    arg2->unk0.x = sp94.f[0] * arg2->unk18;
    arg2->unk0.y = sp94.f[1] * arg2->unk18;
    arg2->unk0.z = sp94.f[2] * arg2->unk18;
    arg2->unkC.x = arg2->unk0.x + arg2->unkC.x;
    arg2->unkC.y = arg2->unk0.y + arg2->unkC.y;
    arg2->unkC.z = arg2->unk0.z + arg2->unkC.z;
    if (dll27Data->unk25D != 0) {
        var_fv0 = arg2->unk2C * sp94.y;
        if (arg2->unkC.z < 0.0f) {
            if (var_fv0 < 0.0f) {
                var_fv0 = -var_fv0;
            }
        } else if (var_fv0 > 0.0f) {
            var_fv0 = -var_fv0;
        }
        var_fv0 *= arg2->unk18;
        var_fa0 = arg2->unkC.z + var_fv0;
        if (arg2->unkC.z < 0.0f) {
            if (var_fa0 > 0.0f) {
                arg2->unkC.z = 0.0f;
            } else {
                arg2->unkC.z = var_fa0;
            }
        } else if (var_fa0 < 0.0f) {
            arg2->unkC.z = 0.0f;
        } else {
            arg2->unkC.z = var_fa0;
        }
        if (arg2->unkC.z < 0.0f) {
            var_fa0 = -arg2->unkC.z;
        } else {
            var_fa0 = arg2->unkC.z;
        }
        var_fv0 = arg2->unk2C * sp94.y * (4.0f + SQ(var_fa0));
        if (arg2->unkC.x < 0.0f) {
            if (var_fv0 < 0.0f) {
                var_fv0 = -var_fv0;
            }
        } else if (var_fv0 > 0.0f) {
            var_fv0 = -var_fv0;
        }
        var_fv0 *= arg2->unk18;
        var_fv1 = arg2->unkC.x + var_fv0;
        if (arg2->unkC.x < 0.0f) {
            if (var_fv1 > 0.0f) {
                arg2->unkC.x = 0.0f;
            } else {
                arg2->unkC.x = var_fv1;
            }
        } else if (var_fv1 < 0.0f) {
            arg2->unkC.x = 0.0f;
        } else {
            arg2->unkC.x = var_fv1;
        }
        objdata->unk3E0 = 0;
        objdata->unk3D2 = 0;
    } else {
        objdata->unk3E0 += 1;
        if (objdata->unk3E0 > 0x64) {
            objdata->unk3E0 = 0x64;
        }
    }
    temp = SQ(arg2->unkC.z);
    var_fv0 = arg2->unk30 * temp;
    if (arg2->unkC.z > 0.0f) {
        var_fv0 = -var_fv0;
    }
    var_fv0 *= arg2->unk18;
    var_fa0 = arg2->unkC.z + (var_fv0);
    if (arg2->unkC.z < 0.0f) {
        if (var_fa0 > 0.0f) {
            arg2->unkC.z = 0.0f;
        } else {
            arg2->unkC.z = var_fa0;
        }
    } else if (var_fa0 < 0.0f) {
        arg2->unkC.z = 0.0f;
    } else {
        arg2->unkC.z = var_fa0;
    }
    vec3_transform(&sp140, arg2->unkC.x, arg2->unkC.y, arg2->unkC.z, self->speed.f, &self->speed.y, &self->speed.z);
    VECTOR_SCALE(self->speed, 1.0666667f);
    obj_integrate_speed(self, self->speed.x, self->speed.y, self->speed.z);
    if (arg4 != 0) {
        spA4 = 1.0f / updateRate;
        dll_711_func_3780(self, objdata, dll27Data);
        gDLL_27->vtbl->func_1E8(self, dll27Data, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(self, dll27Data);
        gDLL_27->vtbl->func_624(self, dll27Data, updateRate);
        self->speed.x = (self->srt.transl.x - self->positionMirror2.x) * spA4;
        self->speed.y = (self->srt.transl.y - self->positionMirror2.y) * spA4;
        self->speed.z = (self->srt.transl.z - self->positionMirror2.z) * spA4;
        sp70.f[0] = self->speed.x * 0.93749994f;
        sp70.f[1] = self->speed.y * 0.93749994f;
        sp70.f[2] = self->speed.z * 0.93749994f;
        vec3_transform(&sp100, sp70.f[0], sp70.f[1], sp70.f[2], &arg2->unkC.x, &arg2->unkC.y, &arg2->unkC.z);
        sp7C.f[0] = 0.0f;
        sp7C.f[1] = 1.0f;
        sp7C.f[2] = 0.0f;
        if (dll27Data->unk25C & 0xF) {
            objdata->flags |= SNOWBIKEFLAG_GROUNDED;
        } else {
            objdata->flags &= ~SNOWBIKEFLAG_GROUNDED;
        }
        var_a0 = 0;
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            if (dll27Data->unk25C & (1 << var_v1)) {
                sp7C.x = dll27Data->unk68.unk0[var_v1].x + sp7C.x;
                sp7C.y = dll27Data->unk68.unk0[var_v1].y + sp7C.y;
                sp7C.z = dll27Data->unk68.unk0[var_v1].z + sp7C.z;
                var_a0 += 1;
            }
        }

        VECTOR_SCALE(sp7C, 0.25f);
        if (var_a0 != 0) {
            var_fv0 = 1.0f / var_a0;
            VECTOR_SCALE(sp7C, var_fv0);
        } else {
            sp7C.x = 0.0f;
            sp7C.y = 1.0f;
            sp7C.z = 0.0f;
        }
        _bss_8.yaw = -objdata->unk3CC;
        _bss_8.pitch = 0;
        _bss_8.roll = 0;
        matrix_from_srt_reversed(&spC0, &_bss_8);
        vec3_transform(&spC0, sp7C.x, sp7C.y, sp7C.z, &sp7C.x, &sp7C.y, &sp7C.z);
        sp64 = 0x4000 - arctan2_f(sp7C.y, sp7C.z);
        var_v0 = -(0x4000 - arctan2_f(sp7C.y, sp7C.x));
        sp64 -= (objdata->unk3CE & 0xFFFF);
        CIRCLE_WRAP(sp64);
        objdata->unk3CE += (((sp64 >> 2) / 3) * (s32) updateRate);
        self->srt.pitch = objdata->unk3CE + objdata->unk3D2;
        var_v0 -= (objdata->unk3D0 & 0xFFFF);
        CIRCLE_WRAP(var_v0);
        objdata->unk3D0 += (((var_v0 >> 2) / 3) * (s32) updateRate);
    }
    objdata->unk3CC -= (s16) (objdata->unk2E0.turnInput * (70.0f - ((f32) objdata->unk2E0.thrustInput * 0.05f)) * 0.0666f);
    if (objdata->flags & SNOWBIKEFLAG_8) {
        sp4C = fsin16_precise(objdata->unk3C4);
        sp48 = fcos16_precise(objdata->unk3C4);
        sp54 = fsin16_precise(objdata->unk3CC);
        sp50 = fcos16_precise(objdata->unk3CC);
        if (((sp4C * sp54) + (sp48 * sp50)) > 0.0f) {
            temp1 = -sp48 * sp54;
            temp2 = sp4C * sp50;
            temp3 = sp48 * sp54;
            temp4 = -sp4C * sp50;
            if ((temp3 + temp4) < (temp1 + temp2)) {
                var_v0 = arctan2_f(-sp48, sp4C);
            } else {
                var_v0 = arctan2_f(sp48, -sp4C);
            }
            var_v0 -= objdata->unk3CC & 0xFFFF;
            CIRCLE_WRAP(var_v0);
            objdata->unk3CC += var_v0 >> 3;
        }
    }
}

// offset: 0x2BA0 | func: 23
static void dll_711_func_2BA0(Object *self, IMSnowBike_Data *objdata, IMSnowBike_Data_2AC *arg2, f32 updateRate, s32 arg4) {
    DLL27_Data *dll27Data;
    MtxF sp11C;
    MtxF spDC;
    MtxF sp9C;
    f32 temp_fa0;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 sp8C;
    f32 var_fv1;
    Vec3f sp7C;
    Vec3f sp70;
    Vec3f sp64;
    s32 temp_a1;
    s32 var_a0;
    s32 var_s0;
    s32 var_v1;

    dll27Data = &objdata->unk4C;

    if (objdata->unk3DA != 0) {
        VECTOR_SCALE(arg2->unkC, objdata->unk380);
        objdata->unk3DA--;
        if (objdata->unk3DA < 0) {
            objdata->unk3DA = 0;
        }
    }
    if (arg2->unkC.x > 2.0f) {
        arg2->unkC.x = 2.0f;
    }
    if (arg2->unkC.x < -2.0f) {
        arg2->unkC.x = -2.0f;
    }
    if (arg2->unkC.y > 4.0f) {
        arg2->unkC.y = 4.0f;
    }
    if (arg2->unkC.y < -4.0f) {
        arg2->unkC.y = -4.0f;
    }
    if (arg2->unkC.z > 2.0f) {
        arg2->unkC.z = 2.0f;
    }
    if (arg2->unkC.z < -3.8f) {
        arg2->unkC.z = -3.8f;
    }
    _bss_8.yaw = objdata->unk3CC;
    _bss_8.pitch = objdata->unk3CE;
    _bss_8.roll = objdata->unk3D0;
    matrix_from_srt(&sp11C, &_bss_8);
    _bss_8.yaw = -objdata->unk3CC;
    _bss_8.pitch = -objdata->unk3CE;
    _bss_8.roll = -objdata->unk3D0;
    matrix_from_srt_reversed(&spDC, &_bss_8);
    vec3_transform(&spDC, 0.0f, arg2->unk28 * arg2->unk1C, 0.0f, &sp7C.x, &sp7C.y, &sp7C.z);
    temp_fv0 = 2.0f * -(f32) objdata->unk2E0.thrustInput;
    temp_fv0 *= arg2->unk18;
    arg2->unkC.z += temp_fv0;
    arg2->unk0.x = sp7C.f[0] * arg2->unk18;
    arg2->unk0.y = sp7C.f[1] * arg2->unk18;
    arg2->unk0.z = sp7C.f[2] * arg2->unk18;
    arg2->unkC.x = arg2->unk0.x + arg2->unkC.x;
    arg2->unkC.y = arg2->unk0.y + arg2->unkC.y;
    arg2->unkC.z = arg2->unk0.z + arg2->unkC.z;
    if (dll27Data->unk25D != 0) {
        arg2->unkC.x = 0.0f;
        var_fv1 = arg2->unk2C * sp7C.y;
        if (arg2->unkC.z < 0.0f) {
            if (var_fv1 < 0.0f) {
                var_fv1 = -var_fv1;
            }
        } else if (var_fv1 > 0.0f) {
            var_fv1 = -var_fv1;
        }
        var_fv1 *= arg2->unk18;
        temp_fv0 = arg2->unkC.z + var_fv1;
        if (temp_fv0 > 0.0f) {
            arg2->unkC.z = 0.0f;
        } else {
            arg2->unkC.z = temp_fv0;
        }
    }
    temp_fv0_2 = SQ(arg2->unkC.z);
    var_fv1 = arg2->unk30 * temp_fv0_2;
    var_fv1 *= arg2->unk18;
    temp_fv0 = arg2->unkC.z + (var_fv1);
    if (temp_fv0 > 0.0f) {
        arg2->unkC.z = 0.0f;
    } else {
        arg2->unkC.z = temp_fv0;
    }
    vec3_transform(&sp11C, arg2->unkC.x, arg2->unkC.y, arg2->unkC.z, self->speed.f, &self->speed.y, &self->speed.z);
    VECTOR_SCALE(self->speed, 1.0666667f);
    obj_integrate_speed(self, self->speed.x, self->speed.y, self->speed.z);
    if (arg4 != 0) {
        sp8C = (1.0f / updateRate);
        gDLL_27->vtbl->func_1E8(self, dll27Data, gUpdateRateF);
        gDLL_27->vtbl->func_5A8(self, dll27Data);
        gDLL_27->vtbl->func_624(self, dll27Data, updateRate);
        self->speed.x = (self->srt.transl.x - self->positionMirror2.x) * sp8C;
        self->speed.y = (self->srt.transl.y - self->positionMirror2.y) * sp8C;
        self->speed.z = (self->srt.transl.z - self->positionMirror2.z) * sp8C;
        sp64.f[0] = self->speed.x * 0.93749994f;
        sp64.f[1] = self->speed.y * 0.93749994f;
        sp64.f[2] = self->speed.z * 0.93749994f;
        vec3_transform(&spDC, sp64.f[0], sp64.y, sp64.f[2], &arg2->unkC.x, &arg2->unkC.y, &arg2->unkC.z);
        sp70.f[0] = 0.0f;
        sp70.f[1] = 1.0f;
        sp70.f[2] = 0.0f;
        if (dll27Data->unk25C & 0xF) {
            objdata->flags |= SNOWBIKEFLAG_GROUNDED;
        } else {
            objdata->flags &= ~SNOWBIKEFLAG_GROUNDED;
        }
        var_a0 = 0;
        for (var_v1 = 0; var_v1 < 4; var_v1++) {
            if (dll27Data->unk25C & (1 << var_v1)) {
                sp70.x = dll27Data->unk68.unk0[var_v1].x + sp70.x;
                sp70.y = dll27Data->unk68.unk0[var_v1].y + sp70.y;
                sp70.z = dll27Data->unk68.unk0[var_v1].z + sp70.z;
                var_a0++;
            }
        }
        if (var_a0 != 0) {
            temp_fv0 = 1.0f / var_a0;
            VECTOR_SCALE(sp70, temp_fv0);
        } else {
            sp70.x = 0.0f;
            sp70.y = 1.0f;
            sp70.z = 0.0f;
        }
        _bss_8.yaw = -objdata->unk3CC;
        _bss_8.pitch = 0;
        _bss_8.roll = 0;
        matrix_from_srt_reversed(&sp9C, &_bss_8);
        vec3_transform(&sp9C, sp70.x, sp70.y, sp70.z, &sp70.x, &sp70.y, &sp70.z);
        var_s0 = 0x4000 - arctan2_f(sp70.y, sp70.z);
        temp_a1 = -(0x4000 - arctan2_f(sp70.y, sp70.x));
        var_s0 -= (objdata->unk3CE & 0xFFFF);
        CIRCLE_WRAP(var_s0);
        objdata->unk3CE += (((var_s0 >> 2) / 3) * (s32) updateRate);
        self->srt.pitch = objdata->unk3CE + objdata->unk3D2;
        temp_a1 -= (objdata->unk3D0 & 0xFFFF);
        CIRCLE_WRAP(temp_a1);
        objdata->unk3D0 += (((temp_a1 >> 2) / 3) * (s32) updateRate);
    }
    objdata->unk3CC -= (s16) (objdata->unk2E0.turnInput * (70.0f - (objdata->unk2E0.thrustInput * 0.05f)) * 0.0666f);
}

// offset: 0x33B4 | func: 24
static void dll_711_func_33B4(Object *self, IMSnowBike_Data_2E0 *arg1, u8 port, s32 buffer) {
    arg1->turnInput = joy_get_stick_x_buffered(port, buffer);
    arg1->thrustInput = joy_get_stick_y_buffered(port, buffer);
}

// offset: 0x3430 | func: 25
static void dll_711_func_3430(Object *self, IMSnowBike_Data *objdata, MtxF *a2, s32 a3, s32 a4, s32 a5) {
    SRT sp20;

    sp20.yaw = objdata->unk3CC;
    if (a5 != 0) {
        sp20.pitch = self->srt.pitch;
    } else {
        sp20.pitch = 0;
    }
    if (a4 != 0) {
        sp20.roll = self->srt.roll;
    } else {
        sp20.roll = 0;
    }
    sp20.scale = 1.0f;
    sp20.transl.x = self->srt.transl.x;
    sp20.transl.y = self->srt.transl.y;
    sp20.transl.z = self->srt.transl.z;
    if (a3 != 0) {
        sp20.yaw += objdata->unk3C8;
    }
    matrix_from_srt(a2, &sp20);
}

// offset: 0x34E4 | func: 26
static void dll_711_func_34E4(Object *self, IMSnowBike_Data *objdata) {
    s32 temp_v0;
    s32 sp30;
    s32 sp2C;
    Object *sp28;

    temp_v0 = func_80025F40(self, &sp28, &sp30, &sp2C);
    if ((self->objhitInfo->unk58 & 1) && (temp_v0 != 0)) {
        switch (temp_v0) {
        case 13:
            objdata->unk3DA = 0x14;
            objdata->unk380 = 0.8f;
            return;
        case 15:
            objdata->unk3DA = 0xC;
            objdata->unk380 = 0.5f;
            break;
        }
    }
}

// offset: 0x3598 | func: 27
static int dll_711_func_3598(Object *a0, Object *a1, AnimObj_Data *a2, s8 a3) {
    IMSnowBike_Data* objdata;
    IMSnowBike_Data_2AC* sp44;
    IMSnowBike_Data_2E0* temp_v0;
    f32 spA0[3];
    SRT sp88;
    MtxF sp48;

    objdata = a0->data;
    sp44 = &objdata->unk2AC;
    temp_v0 = &objdata->unk2E0;
    
    if (objdata->unk3DE == 2) {
        spA0[0] = (a0->srt.transl.x - objdata->unk39C) * gUpdateRateInverseF;
        spA0[1] = (a0->srt.transl.y - objdata->unk3A0) * gUpdateRateInverseF;
        spA0[2] = (a0->srt.transl.z - objdata->unk3A4) * gUpdateRateInverseF;
        spA0[0] = spA0[0] * 0.93749994f;
        spA0[1] = spA0[1] * 0.93749994f;
        spA0[2] = spA0[2] * 0.93749994f;
        sp88.transl.x = 0.0f;
        sp88.transl.y = 0.0f;
        sp88.transl.z = 0.0f;
        sp88.scale = 1.0f;
        sp88.yaw = -a0->srt.yaw;
        sp88.pitch = -a0->srt.pitch;
        sp88.roll = -a0->srt.roll;
        matrix_from_srt_reversed(&sp48, &sp88);
        vec3_transform(&sp48, spA0[0], spA0[1], spA0[2], &sp44->unkC.x, &sp44->unkC.y, &sp44->unkC.z);
        temp_v0->thrustInput += (gUpdateRate * 8);
        if (temp_v0->thrustInput > 0x46) {
            temp_v0->thrustInput = 0x46;
        }
        dll_711_func_3D4C(a0, objdata, sp44->unkC.z, temp_v0->thrustInput, &temp_v0->unk10, 4);
    }
    objdata->unk39C = a0->srt.transl.x;
    objdata->unk3A0 = a0->srt.transl.y;
    objdata->unk3A4 = a0->srt.transl.z;
    objdata->flags |= SNOWBIKEFLAG_10;
    objdata->flags &= ~SNOWBIKEFLAG_8;
    return 0;
}

// offset: 0x3780 | func: 28
static void dll_711_func_3780(Object *self, IMSnowBike_Data *objdata, DLL27_Data *arg2) {
    s32 var_s2;
    MtxF spDC;
    Vec3f sp88[7];
    s8 sp87;
    s8 sp86;
    s32 _pad;

    sp86 = (s8) (-0x80 >> (7 - objdata->unk3DC));
    sp86 = ~sp86;
    objdata->unk3E1 = -1;
    objdata->unk398 = objdata->unk2AC.unkC.z;
    if (sp86 != 0) {
        do {
            sp87 = sp86;
            for (var_s2 = 0; var_s2 < objdata->unk3DC; var_s2++) {
                dll_711_func_3430(self, objdata, &spDC, 1, 1, 1);
                vec3_transform(&spDC, 
                               _data_40[var_s2].x, _data_40[var_s2].y, _data_40[var_s2].z, 
                               &sp88[var_s2].x, &sp88[var_s2].y, &sp88[var_s2].z);
                if (func_80059C40(&objdata->unk32C[var_s2], &sp88[var_s2], _data_7C[var_s2], 
                                  0, NULL, self, 8, -1, 0xFF, 0) == 0) {
                    sp87 &= ~(1 << var_s2);
                } else {
                    bcopy(&sp88[var_s2], &objdata->unk32C[var_s2], sizeof(Vec3f));
                    spDC.m[3][0] = sp88[var_s2].x;
                    spDC.m[3][1] = sp88[var_s2].y;
                    spDC.m[3][2] = sp88[var_s2].z;
                    vec3_transform(&spDC, 
                                   -_data_40[var_s2].x, -_data_40[var_s2].y, -_data_40[var_s2].z, 
                                   self->srt.transl.f, &self->srt.transl.y, &self->srt.transl.z);
                }
            }
            if (objdata->unk3E1 == -1) {
                objdata->unk3E1 = sp87;
            }
        } while (sp87 != 0);
    }
    bcopy(&sp88, objdata->unk32C, objdata->unk3DC * sizeof(Vec3f));
}

// offset: 0x3A34 | func: 29
static s32 dll_711_func_3A34(Object *self, void *arg1, IMSnowBike_Data *objdata, IMSnowBike_Data_2E0 *arg3) {
    f32 zDiff;
    f32 var_fv1;
    s32 sp44;
    f32 xDiff;
    s32 var_a0;
    s32 var_v1;

    xDiff = self->srt.transl.x - objdata->unk0.transl.x;
    zDiff = self->srt.transl.z - objdata->unk0.transl.z;
    zDiff = sqrtf(SQ(xDiff) + SQ(zDiff));
    var_fv1 = 100.0f - zDiff;
    if (var_fv1 < 0.0f) {
        var_fv1 = 0.0f;
    }
    sp44 = gDLL_4_Race->vtbl->func5(&objdata->unk0, &objdata->unk18, var_fv1, 1, 0, 0);
    gDLL_4_Race->vtbl->func4(self, &objdata->unk18);
    gDLL_4_Race->vtbl->func10(&objdata->unk18);
    if (sp44 != 0) {
        arg3->turnInput = 0;
        arg3->thrustInput = 0;
        return 1;
    }
    xDiff = self->srt.transl.x - objdata->unk0.transl.x;
    zDiff = self->srt.transl.z - objdata->unk0.transl.z;
    var_v1 = arctan2_f(xDiff, zDiff) - (self->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(var_v1)
    var_a0 = var_v1 >> 5;
    if (var_a0 > 0x41) {
        var_a0 = 0x41;
    } else if (var_a0 < -0x41) {
        var_a0 = -0x41;
    }
    arg3->turnInput = -var_a0;
    if (var_a0 < 0) {
        var_a0 = -var_a0;
    }
    arg3->thrustInput = 20.0f - (var_a0 * 0.2f);
    return 0;
}

// offset: 0x3C30 | func: 30
static void dll_711_func_3C30(Object *self, IMSnowBike_Data *objdata) {
    MtxF sp60;
    s32 i;

    gDLL_27->vtbl->reset(self, &objdata->unk4C);
    dll_711_func_3430(self, objdata, &sp60, 0, 0, 0);
    for (i = 0; i < objdata->unk3DC; i++) {
        vec3_transform(&sp60, 
                       _data_40[i].x, _data_40[i].y, _data_40[i].z, 
                       &objdata->unk32C[i].x, &objdata->unk32C[i].y, &objdata->unk32C[i].z);
    }
}

// offset: 0x3D4C | func: 31
static void dll_711_func_3D4C(Object *self, IMSnowBike_Data *objdata, f32 arg2, s32 arg3, s32 *arg5, u8 arg6) {
    static f32 _bss_24;
    f32 sp54;
    SRT sp3C;
    s32 var_v0;

    sp54 = 1.0f;
    if (arg6 & 1) {
        if (objdata->unk3B4 == 0) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_289_Engine_Loop, MAX_VOLUME, &objdata->unk3B4, NULL, 0, NULL);
        }
        if (objdata->unk3B4 != 0) {
            _bss_24 = arg2 * 11.6f;
            if (_bss_24 < 0.0f) {
                _bss_24 = -_bss_24;
            }
            if (_bss_24 < 40.0f) {
                _bss_24 = 40.0f;
            }
            if (_bss_24 > 200.0f) {
                _bss_24 = 200.0f;
            }
            gDLL_6_AMSFX->vtbl->func_954(objdata->unk3B4, _bss_24 / 70.0f + 0.1f);
            if (objdata->unk3E0 < 0x12) {
                var_v0 = (arg2 * 30.0f);
                if (var_v0 < 0) {
                    var_v0 = -var_v0;
                }
                if (var_v0 > 0x7F) {
                    var_v0 = 0x7F;
                }
                gDLL_6_AMSFX->vtbl->func_860(objdata->unk3B4, var_v0);
            } else {
                gDLL_6_AMSFX->vtbl->func_860(objdata->unk3B4, 0);
            }
        }
    }
    if (arg6 & 2) {
        if (objdata->unk3BC == 0) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_28F, MAX_VOLUME, &objdata->unk3BC, NULL, 0, NULL);
        }
        if (objdata->unk3BC != 0) {
            if (arg2 != 0.0f) {
                _bss_24 = (self->srt.roll * arg2) / 30000.0f;
            } else {
                _bss_24 = 0.0f;
            }
            if (_bss_24 < 0.0f) {
                _bss_24 = -_bss_24;
            } else if (_bss_24 > 1.0f) {
                _bss_24 = 1.0f;
            }
            gDLL_6_AMSFX->vtbl->func_954(objdata->unk3BC, 0.1f + _bss_24);
            if (objdata->unk3E0 < 0x12) {
                _bss_24 *= 127.0f;
                if (_bss_24 > 127.0f) {
                    _bss_24 = 127.0f;
                } else if (_bss_24 < 0.0f) {
                    _bss_24 = 0.0f;
                }
                gDLL_6_AMSFX->vtbl->func_860(objdata->unk3BC, _bss_24);
            } else {
                gDLL_6_AMSFX->vtbl->func_A1C(objdata->unk3BC);
                objdata->unk3BC = 0;
            }
        }
    }
    if (arg6 & 4) {
        if (objdata->unk3D6 <= 0) {
            if (objdata->unk3B8 == 0) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_50D_Jet_Loop, MAX_VOLUME, &objdata->unk3B8, NULL, 0, NULL);
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_28E, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            if (objdata->unk3C0 == 0) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_50C_Low_Rumble_Loop, MAX_VOLUME, &objdata->unk3C0, NULL, 0, NULL);
            }
        }
        if (objdata->unk3B8 != 0) {
            gDLL_6_AMSFX->vtbl->func_954(objdata->unk3B8, (objdata->unk394 * 0.00048828125f) + 0.5f);
            if (arg3 >= 6) {
                objdata->unk394 += gUpdateRateF;
            } else {
                if (objdata->unk394 > 55.0f) {
                    objdata->unk394 -= (0.5f * gUpdateRateF);
                } else {
                    objdata->unk394 += (0.2f * gUpdateRateF);
                }
            }
            if (objdata->unk394 > 90.0f) {
                objdata->unk394 = 90.0f;
            }
            gDLL_6_AMSFX->vtbl->func_860(objdata->unk3B8, objdata->unk394);
        }
        if (objdata->unk3C0 != 0) {
            gDLL_6_AMSFX->vtbl->func_954(objdata->unk3C0, (objdata->unk390 / 75.0f) + 0.2f);
            if (arg3 >= 6) {
                objdata->unk390 = (arg3 * 0.6f) + 15.0f;
            } else {
                if (((0.5f * gUpdateRateF)) < objdata->unk390) {
                    objdata->unk390 -= (0.5f * gUpdateRateF);
                } else {
                    objdata->unk390 = 0.0f;
                }
            }
            if (objdata->unk390 > 127.0f) {
                objdata->unk390 = 127.0f;
            }
            gDLL_6_AMSFX->vtbl->func_860(objdata->unk3C0, objdata->unk390);
        }
    }
    if (objdata->unk2F4 == NULL) {
        objdata->unk2F4 = dll_load_deferred(DLL_ID_141, 1);
    }
    if (objdata->unk2F8 == NULL) {
        objdata->unk2F8 = dll_load_deferred(DLL_ID_142, 1);
    }
    if (objdata->unk3D6 <= 0) {
        sp54 = 0.7f;
        if (arg3 >= 0xB) {
            sp54 = arg3;
            sp54 = (0.01f * sp54) + 0.1f;
            objdata->unk2F4->vtbl->func0(self, 0, NULL, 0x404, -1, &sp54);
        }
        objdata->unk3D6 = 0x1E;
        if ((self->srt.roll >= 0x3E9) && (arg2 < -1.0f)) {
            sp3C.scale = self->srt.roll / 8000.0f;
            sp3C.scale *= (-arg2 / 3.8f);
            objdata->unk2F8->vtbl->func0(self, 0, &sp3C, 0x404, -1, NULL);
        } else if ((self->srt.roll < -0x3E8) && (arg2 < -1.0f)) {
            sp3C.scale = self->srt.roll / -8000.0f;
            sp3C.scale *= (-arg2 / 3.8f);
            objdata->unk2F8->vtbl->func0(self, 1, &sp3C, 0x404, -1, NULL);
        }
    }
    objdata->unk3D6 -= gUpdateRate;
}

// offset: 0x4780 | func: 32 | export: 20
void dll_711_func_4780(UNK_TYPE_32 a0, UNK_TYPE_32 a1, UNK_TYPE_32 a2) { }

/*0x0*/ static const char str_0[] = "Finished Is SEt for Some Reason \n";
