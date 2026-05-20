
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/53_movelib.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/713_DRearthwalk.h"
#include "game/gamebits.h"
#include "game/gametexts.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object_id.h"
#include "sys/dll.h"
#include "sys/gfx/modgfx.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/map_enums.h"
#include "sys/objprint.h"
#include "sys/print.h"
#include "sys/rand.h"
#include "sys/segment_1050.h"
#include "sys/objtype.h"
#include "sys/objlib.h"
#include "dll.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
} DRearthwalk_Setup;

typedef struct {
    ObjFSA_Data unk0;
    u8 _unk34C[0x370 - 0x34C];
    HeadAnimation unk370;
    HeadAnimation unk394;
    MoveLibData unk3B8;
    UnkCurvesStruct unk870;
    Vec3f unk978[4];
    u8 _unk9A8[0x9B4 - 0x9A8];
    f32 unk9B4;
    f32 unk9B8;
    f32 unk9BC;
    u8 _unk9C0[0xA52 - 0x9C0];
    s16 unkA52;
    u8 _unkA54[0xA56 - 0xA54];
    s16 unkA56;
    u16 unkA58;
    u8 unkA5A;
    u8 _unkA5B;
    u8 unkA5C;
    u8 _unkA5D;
    u8 unkA5E;
    u8 unkA5F;
    u8 unkA60_0 : 1;
    u8 unkA60_1 : 1;
    u8 unkA60_2 : 1;
    u8 unkA60_3 : 1;
    u8 unkA60_4 : 1;
    u8 unkA60_5 : 1;
    s8 unkA61;
    s8 unkA62;
    s8 unkA63;
} DRearthwalk_Data;

/*0x0*/ static u16 _data_0[] = {0x0854, 0x081a};
/*0x4*/ static u16 _data_4[] = {0x0853, 0x0852};
/*0x8*/ static s16 _data_8[][2] = {
    {0x0817, 0x0800}, 
    {0x0818, 0x0800}, 
    {0x0819, 0x1000}, 
    {0x081a, 0x1000}
};
/*0x18*/ static s16 _data_18[][2] = {
    {0x07bf, 0x1000}, 
    {0x0816, 0x1000}, 
    {0x081c, 0x1000}
};
/*0x24*/ static s16 _data_24[][2] = {
    {SOUND_8ED, 0x1000}
};
/*0x24*/ static s16 _data_28[] = {
    SOUND_855, 
    SOUND_856, 
    SOUND_857, 
    SOUND_858
};
/*0x30*/ static Vec3f _data_30[] = {
    VEC3F(-9.0f, 0.0f, -12.0f), 
    VEC3F(9.0f, 0.0f, -12.0f), 
    VEC3F(9.0f, 0.0f, 12.0f), 
    VEC3F(-9.0f, 0.0f, 12.0f)
};
/*0x60*/ static f32 _data_60[] = {
    0.0f, 0.0f, 0.0f, 0.0f
};
/*0x70*/ static Vec3f _data_70[] = {
    VEC3F(0.0f, 0.0f, 35.0f), 
    VEC3F(0.0f, 0.0f, -35.0f)
};
/*0x88*/ static f32 _data_88[] = {
    25.0f, 25.0f
};
/*0x90*/ static u32 _data_90 = 0x037b037b;
/*0x94*/ static u32 _data_94[] = {
    0x00000008, 0x00000007, 0x00000009, 0x0000000a, 0x01c80000
};
/*0xA8*/ static s16 _data_A8[] = {0x0018, 0x0000};
/*0xAC*/ static f32 _data_AC[] = {
    0.0031f, /*0x00030003*/2.7551e-40f // hmm
};
/*0xB4*/ static f32 _data_B4[][2] = {
    {0.0f, 0.03f}, 
    {0.05f, 0.7f}, 
    {0.6f, 1.2f}, 
    {0.8f, 1.93f}, 
    {1.72f, 2.7f}
};
/*0xDC*/ static s16 _data_DC[] = {
    0x0002, 0x0003, 0x0017, 0x0016, 0x0004, 0x0000
};

/*0x0*/ static ObjFSA_StateCallback _bss_0[8];
/*0x20*/ static ObjFSA_StateCallback _bss_20[1];
/*0x28*/ static MtxF _bss_28;

static void dll_713_func_ABC(Object* self);
static void dll_713_func_B54(Object* self, s32 arg1, s32 arg2);
static void dll_713_func_1684(Object* self, DRearthwalk_Data* objdata, ObjFSA_Data* fsa);
static int dll_713_func_1EBC(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3);
static void dll_713_func_3210(Object* self, s16 arg1, s16 arg2);
static s32 dll_713_func_32EC(Object* self, u8 arg1);

/*static*/ s32 dll_713_func_21A8(Object* self, ObjFSA_Data* fsa, f32 updateRate);
/*static*/ s32 dll_713_func_21DC(Object* self, ObjFSA_Data* fsa, f32 updateRate);
/*static*/ s32 dll_713_func_2454(Object* self, ObjFSA_Data* fsa, f32 updateRate);
/*static*/ s32 dll_713_func_2704(Object* self, ObjFSA_Data* fsa, f32 updateRate);
/*static*/ s32 dll_713_func_2860(Object* self, ObjFSA_Data* fsa, f32 updateRate);
/*static*/ s32 dll_713_func_2D80(Object* self, ObjFSA_Data* fsa, f32 updateRate);
/*static*/ s32 dll_713_func_2F90(Object* self, ObjFSA_Data* fsa, f32 updateRate);
/*static*/ s32 dll_713_func_311C(Object* self, ObjFSA_Data* fsa, f32 updateRate);

/*static*/ s32 dll_713_func_31F8(Object *self, ObjFSA_Data *fsa, f32 updateRate);

// offset: 0x0 | func: 0
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_0.s")
#else
void dll_713_func_0(void) {
    _bss_0[0] = dll_713_func_21A8;
    _bss_0[1] = dll_713_func_21DC;
    _bss_0[2] = dll_713_func_2454;
    _bss_0[3] = dll_713_func_2704;
    _bss_0[4] = dll_713_func_2860;
    _bss_0[5] = dll_713_func_2D80;
    _bss_0[6] = dll_713_func_2F90;
    _bss_0[7] = dll_713_func_311C;

    _bss_20[0] = dll_713_func_31F8;
}
#endif

// offset: 0x8C | ctor
#ifndef NON_MATCHING
void dll_713_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_ctor.s")
#else
void dll_713_ctor(void* dll) {
    dll_713_func_0();
}
#endif

// offset: 0xCC | dtor
void dll_713_dtor(void *dll) { }

// offset: 0xD8 | func: 1 | export: 0
void dll_713_setup(Object* self, DRearthwalk_Setup* setup, s32 arg2) {
    DRearthwalk_Data* objdata;
    s32 _pad[2];
    u8 sp50[] = {1, 1, 1, 1};
    s16 sp44[] = {0x000a, 0x000a, 0x0000, 0x0000, 0x0000};
    s16 sp38[] = {0x0014, 0x0014, 0x0000, 0x0000, 0x0000};

    self->srt.yaw = setup->unk18 << 8;
    self->animCallback = dll_713_func_1EBC;
    obj_add_object_type(self, OBJTYPE_11);
    objdata = self->data;
    objdata->unkA5C = setup->unk19;
    objdata->unkA52 = 5;
    objdata->unkA62 = -1;
    if (self->shadow != NULL) {
        self->shadow->flags |= (OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_DIR);
        self->shadow->maxDistScale = self->shadow->scale * 0.4f;
    }
    gDLL_18_objfsa->vtbl->func0(self, &objdata->unk0, 8, 1);
    objdata->unk0.unk29C = 0.17f;
    objdata->unk0.unk4.mode = 0;
    gDLL_27->vtbl->init(&objdata->unk0.unk4, DLL27FLAG_4000000 | DLL27FLAG_2000000 | DLL27FLAG_4, DLL27FLAG_NONE, DLL27MODE_DISABLED);
    gDLL_27->vtbl->setup_hits_collider(&objdata->unk0.unk4, 2, _data_70, _data_88, 8);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->unk0.unk4, 4, _data_30, _data_60, sp50);
    gDLL_27->vtbl->reset(self, &objdata->unk0.unk4);
    func_8002674C(self);
    create_temp_dll(53);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func2(self, &objdata->unk3B8, -4551, 5461, 2);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func5(&objdata->unk3B8, 300, 120);
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func6(&objdata->unk3B8, sp38, sp44, 2);
    if (setup->unk19 == 1) {
        objdata->unkA56 = 1;
        objdata->unkA58 |= 0x100;
        objdata->unkA60_1 = 1;
    } else {
        objdata->unkA56 = 5;
        objdata->unkA60_4 = 1;
    }
    objdata->unkA61 = 0x1E;
}

// offset: 0x408 | func: 2 | export: 1
void dll_713_control(Object* self) {
    DRearthwalk_Data* temp_v0;
    ObjFSA_Data* sp50;
    s32 sp4C;
    Object* sp48;
    s32 var_a2;
    s32 sp40;
    s16 var_v0;

    temp_v0 = self->data;
    sp50 = self->data;
    sp4C = 1;
    sp48 = get_player();
    temp_v0->unkA52 = 5;
    diPrintf(" EARTHWALK ENERGY : %i ", temp_v0->unkA56);
    self->unkAF &= ~8;
    if (temp_v0->unkA5A == 2) {
        self->unkAF |= 8;
        sp50->unk4.mode = 1;
        self->objhitInfo->unk5B = 0xF4;
        self->objhitInfo->unk5C = 0xF4;
        var_a2 = 0;
        while (var_a2 < gUpdateRate) {
            dll_713_func_B54(self, 1, var_a2);
            var_a2 += 1;
        }
    } else {
        if (!(temp_v0->unkA58 & 0x20) && (main_get_bits(0x628) == 0)) {
            self->unkAF |= 8;
            obj_get_all_of_type(4, &sp40);
            diPrintf(" num %i ", sp40);
            if ((sp40 == 0) && (main_get_bits(0x789) == 3)) {
                main_set_bits(0x628, 1);
                temp_v0->unkA62 = 9;
            }
        }
        sp50->unk4.mode = 1;
        gDLL_27->vtbl->reset(self, &sp50->unk4);
        self->objhitInfo->unk5B = 0;
        self->objhitInfo->unk5C = 0;
        dll_713_func_B54(self, gUpdateRate, -1);
    }
    func_80028D2C(self);
    func_80032A08(self, &temp_v0->unk370);
    func_80034BC0(self, &temp_v0->unk394);
    if (!(temp_v0->unkA58 & 0x20) && (main_get_bits(0x5FE) != 0) && (temp_v0->unkA56 > 0)) {
        main_set_bits(0x654, 1);
        temp_v0->unkA62 = 2;
        temp_v0->unkA60_1 = 0;
    }
    if (!(temp_v0->unkA58 & 0x20) && (main_get_bits(0x655) != 0)) {
        temp_v0->unkA58 |= 0x40;
        temp_v0->unkA62 = 2;
        temp_v0->unkA58 |= 0x20;
        temp_v0->unk3B8.unk4A9 |= 2;
        gDLL_26_Curves->vtbl->func_4288(&temp_v0->unk870, self, 1000.0f, &sp4C, -1);
    }
    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func0(self, &temp_v0->unk3B8);
    temp_v0->unkA60_3 = 0;
    if (self->unkAF & 1) {
        temp_v0->unkA60_3 = 1;
        if ((temp_v0->unkA60_3) && (temp_v0->unkA60_4)) {
            joy_disable_buttons(0, 0x8000);
            var_v0 = self->srt.yaw - (sp48->srt.yaw & 0xFFFF);
            CIRCLE_WRAP(var_v0);
            if (var_v0 > 0) {
                temp_v0->unkA5F = 0;
                gDLL_3_Animation->vtbl->start_obj_sequence(5, self, -1);
            } else {
                temp_v0->unkA5F = 1;
                gDLL_3_Animation->vtbl->start_obj_sequence(6, self, -1);
            }
            gDLL_5_AMSEQ2->vtbl->set(self, 0xF9, 0, 0, 0);
            temp_v0->unkA58 |= 4;
            temp_v0->unk3B8.unk4A9 |= 1;
        }
        if (gDLL_1_cmdmenu->vtbl->was_this_item_used(0xC1) != 0) {
            if (sp50->animState == 2) {
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                joy_disable_buttons(0, 0x8000);
            } else {
                gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
                joy_disable_buttons(0, 0x8000);
            }
            temp_v0->unkA56 += 0xC;
            main_set_bits(0xC1, main_get_bits(0xC1) - 1);
        } else if ((temp_v0->unkA62 != -1) && (gDLL_1_cmdmenu->vtbl->was_any_item_used() == 0)) {
            if (!temp_v0->unkA60_4) {
                gDLL_3_Animation->vtbl->start_obj_sequence(temp_v0->unkA62, self, -1);
                joy_disable_buttons(0, 0x8000);
            } else {
                temp_v0->unkA60_3 = 1;
            }
        }
    }
    if ((temp_v0->unkA61 == 0) && (main_get_bits(0x7B6) == 0)) {
        dll_713_func_ABC(self);
    }
    if (temp_v0->unkA61 != 0) {
        temp_v0->unkA61 -= gUpdateRate;
        if (temp_v0->unkA61 < 0) {
            temp_v0->unkA61 = 0;
        }
    }
}

// offset: 0xABC | func: 3
static void dll_713_func_ABC(Object* self) {
    if (vec3_distance_xz(&get_player()->globalPosition, &self->globalPosition) > 600.0f) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_DRAGON_ROCK_BOTTOM, 2, 0);
    }
}

// offset: 0xB54 | func: 4
static void dll_713_func_B54(Object* self, s32 arg1, s32 arg2) {
    DRearthwalk_Data* objdata;
    s32 _pad3;
    Camera* sp54;
    s32 sp50;
    s32 sp4C;
    s32 _pad;
    f32 temp_fa1;
    f32 temp_fv0;
    UnkCurvesStruct* sp28;
    s16 sp3A;
    f32 dist;
    f32 sp30;
    Object* temp_v1;

    sp4C = 1;
    sp30 = 12.0f;
    temp_v1 = get_player();
    if (arg2 != -1) {
        sp50 = (arg2 + 1) == gUpdateRate;
    } else {
        sp50 = 1;
    }
    sp54 = get_main_camera();
    objdata = self->data;
    objdata->unk0.hitpoints = 0;
    objdata->unk0.flags &= ~0x8000;
    if (objdata->unkA5A == 2) {
        objdata->unk0.xAnalogInput = (f32) joy_get_stick_x_buffered(0, arg2);
        objdata->unk0.yAnalogInput = (f32) joy_get_stick_y_buffered(0, arg2);
        objdata->unk0.unk310 = joy_get_pressed_buffered(0, arg2);
        objdata->unk0.unk30C = joy_get_buttons_buffered(0, arg2);
        objdata->unk0.unk324 = sp54->srt.yaw;
    } else if (objdata->unkA58 & 0x40) {
        objdata->unk3B8.unk4A9 |= 2;
        objdata->unkA60_4 = 0;
        sp28 = &objdata->unk870;
        temp_fv0 = sp28->unk68.x - self->srt.transl.x;
        temp_fa1 = sp28->unk68.z - self->srt.transl.z;
        sqrtf(SQ(temp_fv0) + SQ(temp_fa1));
        if ((objdata->unkA58 & 0x80) && ((gDLL_11_Newlfx->vtbl->func4.withTwoArgsF32Custom(&self->srt.transl, 128.0f) < 0.0f) || (temp_v1->stateFlags & 0x1000))) {
            sp4C = 0;
        }
        temp_fv0 = sp28->unk68.x - temp_v1->srt.transl.x;
        temp_fa1 = sp28->unk68.z - temp_v1->srt.transl.z;
        temp_fa1 = SQ(temp_fv0) + SQ(temp_fa1);
        if (sqrtf(temp_fa1) < 22.0f) {
            sp4C = 0;
        }
        temp_fv0 = sp28->unk68.x - self->srt.transl.x;
        temp_fa1 = sp28->unk68.z - self->srt.transl.z;
        temp_fa1 = SQ(temp_fv0) + SQ(temp_fa1);
        dist = sqrtf(temp_fa1);
        if ((dist < 50.0f) && (sp4C != 0)) {
            if ((func_800053B0(sp28, 0.9f) != 0) || (sp28->unk10 != 0)) {
                if (dll_713_func_32EC(self, (u8) sp28->unkA0->unk18) != 0) {
                    if (gDLL_26_Curves->vtbl->func_4704(sp28) != 0) {
                        objdata->unkA58 &= ~0x140;
                        objdata->unk3B8.unk4A9 &= ~0x2;
                        gDLL_18_objfsa->vtbl->set_anim_state(self, &objdata->unk0, 7);
                    }
                } else {
                    sp30 = 0.0f;
                }
            }
        } else if (dist < 50.0f) {
            sp30 = 0.0f;
        } else {
            sp30 = 11.0f;
        }
        sp3A = arctan2_f(self->srt.transl.x - sp28->unk68.x, self->srt.transl.z - sp28->unk68.z) & 0xFFFF;
        objdata->unk0.xAnalogInput = -fsin16_precise(-sp3A) * sp30;
        objdata->unk0.yAnalogInput = -fcos16_precise(-sp3A) * sp30;
        objdata->unk0.unk324 = 0;
        objdata->unk0.unk310 = 0;
        objdata->unk0.unk30C = 0;
    } else {
        objdata->unk0.unk310 = 0;
        objdata->unk0.unk30C = 0;
        objdata->unk0.unk324 = 0;
        objdata->unk0.xAnalogInput = 0.0f;
        objdata->unk0.yAnalogInput = 0.0f;
    }
    objdata->unk0.flags |= 0x400000;
    if (sp50 != 0) {
        objdata->unk0.flags &= ~0x400000;
    }
    gDLL_18_objfsa->vtbl->tick(self, &objdata->unk0, (f32) arg1, gUpdateRateF, _bss_0, _bss_20);
    dll_713_func_1684(self, objdata, &objdata->unk0);
}

// offset: 0x107C | func: 5 | export: 2
void dll_713_update(Object* self) {
    DRearthwalk_Data* objdata;
    ObjFSA_Data* fsa;
    Object* hitBy;
    DLL27_Data* temp_v1;
    s16 angle;
    s32 dmgType;

    objdata = self->data;
    fsa = self->data;
    if (self->stateFlags & OBJSTATE_IN_SEQ) {
        return;
    }
    
    if (fsa->animState != 6) {
        dmgType = func_80025F40(self, &hitBy, NULL, NULL);
        if (dmgType != 0) {
            if ((dmgType == Damage_Type_Flame_Command) || (get_player() == hitBy) || (hitBy->id == OBJ_sword)) {
                return;
            }
            func_80034B54(self, &objdata->unk394, _data_18[rand_next(0, 2)], 1);
            angle = self->srt.yaw - (hitBy->srt.yaw & 0xFFFF);
            CIRCLE_WRAP(angle);
            if ((angle > 0x4000) || (angle < -0x4000)) {
                objdata->unkA60_0 = 0;
            } else {
                objdata->unkA60_0 = 1;
            }
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 6);
        }
    }

    if (fsa->unk4.mode != 0) {
        temp_v1 = &fsa->unk4;
        if (temp_v1->hitsTouchBits != 0) {
            fsa->speed *= 0.5f;
            fsa->unk2B0 *= 1.5f;
        } else {
            if (fsa->unk2B0 != 18.0f) {
                fsa->unk2B0 *= 0.8f;
                if (fsa->unk2B0 <= 18.0f) {
                    fsa->unk2B0 = 18.0f;
                }
            }
        }
        if (temp_v1->unk140.unk0 != NULL) {
            hitBy = temp_v1->unk140.unk0;
            if ((hitBy->id == OBJ_DR_PushCart) && (objdata->unkA60_5) && (self->curModAnimId == 0x14)) {
                gDLL_6_AMSFX->vtbl->play(self, _data_28[1], MAX_VOLUME, NULL, NULL, 0, NULL);
                ((DLL_Unknown*)hitBy->dll)->vtbl->func[7].withThreeArgsCustom(hitBy, self, 190.f);
                objdata->unkA60_5 = 0;
            }
            temp_v1->unk140.unk0 = NULL;
        }
    }
}

// offset: 0x138C | func: 6 | export: 3
void dll_713_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DRearthwalk_Data* objdata = self->data;
    s32 numObjs;
    Object** objs;
    s32 i;

    if (visibility == -1) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_80031F6C(self, 2, &objdata->unk9B4, &objdata->unk9B8, &objdata->unk9BC, 0);
        func_80032238(self, 3, 4, objdata->unk978);
    }
    if ((objdata->unkA5A != 2) && (visibility != 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_80031F6C(self, 2, &objdata->unk9B4, &objdata->unk9B8, &objdata->unk9BC, 0);
        func_80032238(self, 3, 4, objdata->unk978);
        ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func3(self, &objdata->unk3B8, 0);
        if (objdata->unkA60_1) {
            objs = obj_get_all_of_type(OBJTYPE_57, &numObjs);
            for (i = 0; i < numObjs; i++) {
                ((DLL_Unknown*)objs[i]->dll)->vtbl->func[7].objtype57Func7(objs[i], self, 
                    _data_94[((DLL_Unknown*)objs[i]->dll)->vtbl->func[8].withOneVoidArgS32(objs[i])], 
                    gdl, mtxs, vtxs, pols);
            }
        }
    }
}

// offset: 0x160C | func: 7 | export: 4
void dll_713_free(Object* self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_11);
    remove_temp_dll(53);
}

// offset: 0x1660 | func: 8 | export: 5
u32 dll_713_get_model_flags(Object *self) {
    return MODFLAGS_EVENTS | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0x1670 | func: 9 | export: 6
u32 dll_713_get_data_size(Object *self, u32 a1) {
    return sizeof(DRearthwalk_Data);
}

// offset: 0x1684 | func: 10
static void dll_713_func_1684(Object* self, DRearthwalk_Data* objdata, ObjFSA_Data* fsa) {
    u8 var_s7;
    u8 var_s0_2;
    u8 var_s5;
    u8 var_s0;
    SRT sp7C;
    f32 var_fv0;

    var_s7 = 0;
    var_s0 = 0;
    if (fsa->unk308 & 2) {
        var_s0 = 1;
    }
    if (fsa->unk308 & 4) {
        var_s0 |= 2;
    }
    if (fsa->unk308 & 8) {
        var_s0 |= 4;
    }
    if (fsa->unk308 & 0x10) {
        var_s0 |= 8;
    }
    if (fsa->unk308 & 0x81) {
        if (fsa->unk308 & 0x80) {
            var_s7 = 1;
        }
        gDLL_6_AMSFX->vtbl->play(self, _data_0[var_s7], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    var_s5 = 0;
    while (var_s0) {
        if (var_s0 & 1) {
            if (fsa->speed > 1.0f) {
                var_s7 = 1;
            }
            gDLL_6_AMSFX->vtbl->play(self, _data_4[var_s7], MAX_VOLUME, NULL, NULL, 0, NULL);
            sp7C.transl.x = objdata->unk978[var_s5].x;
            sp7C.transl.y = objdata->unk978[var_s5].y;
            sp7C.transl.z = objdata->unk978[var_s5].z;
            sp7C.scale = fsa->speed * 0.0005f;
            if (sp7C.scale < 0.0004f) {
                sp7C.scale = 0.0004f;
            } else {
                if (sp7C.scale > 0.004f) {
                    var_fv0 = 0.004f;
                } else {
                    var_fv0 = sp7C.scale;
                }
                sp7C.scale = var_fv0;
            }
            var_s0_2 = rand_next(2, 6);
            while (var_s0_2 != 0) {
                gDLL_17_partfx->vtbl->spawn(self, rand_next(0, 0) + PARTICLE_676, &sp7C, PARTFXFLAG_200000 | PARTFXFLAG_10000 | PARTFXFLAG_1, -1, NULL);
                var_s0_2--;
            }
        }
        var_s0 >>= 1;
        var_s5++;
    }
}

// offset: 0x197C | func: 11 | export: 7
s32 dll_713_func_197C(Object* self, Object* rider) {
    return 0;
}

// offset: 0x1990 | func: 12 | export: 8
s32 dll_713_func_1990(Object* self) {
    DRearthwalk_Data* objdata = self->data;

    return objdata->unkA5F != 0 ? 1 : 2;
}

// offset: 0x19B8 | func: 13 | export: 9
void dll_713_func_19B8(Object* self, f32* a1, f32* a2, f32* a3) {
    DRearthwalk_Data* objdata = self->data;
    *a1 = objdata->unk9B4;
    *a2 = objdata->unk9B8;
    *a3 = objdata->unk9BC;
}

// offset: 0x19DC | func: 14 | export: 10
s32 dll_713_func_19DC(Object* self, Object* rider) {
    DRearthwalk_Data* objdata = self->data;
    Vec3f sp48;
    
    if ((objdata->unkA56 != 0) && (joy_get_pressed(0) & 0x4000)) {
        gDLL_5_AMSEQ2->vtbl->set(self, 0xF6, 0, 0, 0);
        sp48.x = self->srt.transl.x;
        sp48.y = self->srt.transl.y;
        sp48.z = self->srt.transl.z;
        sp48.x += fsin16_precise(self->srt.yaw - 0x4000) * 100.0f;
        sp48.z += fcos16_precise(self->srt.yaw - 0x4000) * 100.0f;
        if (func_80059C40(&self->srt.transl, &sp48, 10.0f, 1, NULL, self, 8, -1, 0xFF, 0) != 0) {
            objdata->unkA5E = 1;
        } else {
            objdata->unkA5E = 0;
        }
        gDLL_3_Animation->vtbl->start_obj_sequence(objdata->unkA5E + 7, self, -1);
    }
    if (!objdata->unkA56) {
        gDLL_5_AMSEQ2->vtbl->set(self, 0xF6, 0, 0, 0);
    }
    return objdata->unkA56 == 0;
}

// offset: 0x1BB8 | func: 15 | export: 11
s32 dll_713_func_1BB8(Object* self) {
    DRearthwalk_Data* objdata = self->data;

    return objdata->unkA5E != 0 ? 2 : 1;
}

// offset: 0x1BE0 | func: 16 | export: 12
void dll_713_func_1BE0(Object* self, f32* a1, f32* a2, f32* a3) {
    MtxF mtx;
    SRT srt;

    srt.transl.x = self->srt.transl.x;
    srt.transl.y = self->srt.transl.y;
    srt.transl.z = self->srt.transl.z;
    srt.yaw = self->srt.yaw;
    srt.pitch = self->srt.pitch;
    srt.roll = self->srt.roll;
    srt.scale = 1.0f;
    matrix_from_srt(&mtx, &srt);
    vec3_transform(&mtx, 0.0f, 50.0f, -35.0f, a1, a2, a3);
}

// offset: 0x1CA4 | func: 17 | export: 13
s32 dll_713_func_1CA4(Object* self) {
    return 0;
}

// offset: 0x1CB4 | func: 18 | export: 14
void dll_713_func_1CB4(Object* self, s32 arg1) {
    DRearthwalk_Data* objdata = self->data;
    objdata->unkA5A = arg1;
    if (arg1 == 0) {
        main_set_bits(BIT_7BC, 0);
        main_set_bits(BIT_7D4, 1);
        objdata->unk3B8.unk4A9 &= ~1;
    } else {
        main_set_bits(BIT_7BC, 1);
        main_set_bits(BIT_7D4, 0);
    }
}

// offset: 0x1D5C | func: 19 | export: 15
void dll_713_func_1D5C(Object* self, f32* arg1, s32* arg2) {
    *arg1 = 0.0f;
    *arg2 = 0;
}

// offset: 0x1D84 | func: 20 | export: 16
f32 dll_713_func_1D84(Object* self, f32* arg1) {
    *arg1 = 5.0f;
    return 0.0f;
}

// offset: 0x1DAC | func: 21 | export: 17
s32 dll_713_func_1DAC(Object* self) {
    return 0;
}

// offset: 0x1DBC | func: 22 | export: 18
void dll_713_func_1DBC(Object* self) { }

// offset: 0x1DC8 | func: 23 | export: 19
void dll_713_func_1DC8(Object* self, f32 scale) {
    MtxF* sp4C;
    f32 atX;
    f32 atY;
    f32 atZ;
    SRT srt;

    sp4C = func_80032170(self, 2);
    func_800321E4(self, 2, &atX, &atY, &atZ);
    srt.yaw = 0;
    srt.pitch = 0;
    srt.roll = 0;
    srt.transl.x = atX;
    srt.transl.y = atY;
    srt.transl.z = atZ;
    srt.scale = scale / self->def->scale;
    matrix_from_srt(&_bss_28, &srt);
    matrix_concat_4x3(&_bss_28, sp4C, &_bss_28);
    func_80034FF0(&_bss_28);
}

// offset: 0x1EBC | func: 24
static int dll_713_func_1EBC(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3) {
    DRearthwalk_Data* objdata;
    Object** objList;
    s32 i;
    s32 numObjs;
    s32 partIdx;
    DLL_IModgfx* modgfx;

    objdata = actor->data;
    actor->unkAF |= ARROW_FLAG_8_No_Targetting;
    if (((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func4(actor, animObjData, &objdata->unk3B8, 3, 3) != 0) {
        return 1;
    }
    if (objdata->unkA60_1 && !objdata->unkA60_2) {
        objList = obj_get_all_of_type(OBJTYPE_25, &numObjs);
        for (i = 0; i < numObjs; i++) {
            ((DLL_Unknown*)objList[i]->dll)->vtbl->func[15].withTwoArgsCustom2(objList[i], actor->srt.transl.y + 4.0f);
        }
        objdata->unkA60_2 = 1;
    }
    for (i = 0; i < animObjData->messageCount; i++) {
        switch (animObjData->messages[i]) {
        case 10:
            break;
        case 14:
            gDLL_6_AMSFX->vtbl->play(actor, SOUND_57, MAX_VOLUME, NULL, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->play(actor, SOUND_9E, MAX_VOLUME, NULL, NULL, 0, NULL);
            for (partIdx = 0; partIdx != 10; partIdx++) {
                gDLL_17_partfx->vtbl->spawn(actor, PARTICLE_3C, NULL, 2, -1, NULL);
            }
            modgfx = dll_load_deferred(DLL_ID_109, 1);
            modgfx->vtbl->func0(actor, 0, 0, 2, -1, 0);
            dll_unload(modgfx);
            break;
        }
    }
    return 0;
}

// offset: 0x2174 | func: 25
static void dll_713_func_2174(Object* self, ObjFSA_Data* fsa) {
    fsa->speed = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->unk278 = 0.0f;
    self->velocity.x = 0.0f;
    self->velocity.y = 0.0f;
    self->velocity.z = 0.0f;
}

// offset: 0x21A8 | func: 26
s32 dll_713_func_21A8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DRearthwalk_Data* objdata = self->data;
    if (objdata->unkA5C != 1) {
        objdata->unkA58 |= 0x20;
    }
    return 2;
}

// offset: 0x21DC | func: 27
s32 dll_713_func_21DC(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DRearthwalk_Data* objdata = self->data;
    s16 var_a1;
    s32 temp_v0;
    
    dll_713_func_2174(self, fsa);
    if (fsa->unk33A != 0) {
        if (rand_next(0, 5) == 0) {
            temp_v0 = rand_next(0, 1);
            fsa->animTickDelta = _data_AC[temp_v0];
            var_a1 = _data_A8[temp_v0];
        } else {
            var_a1 = _data_DC[0];
        }
        fsa->animTickDelta = 0.01f;
        func_80023D30(self, var_a1, 0.0f, 0);
        func_80024D74(self, 0x14);
    }
    if (fsa->analogInputPower < 0.1f) {
        fsa->unk328 = 0;
        fsa->unk32A = 0;
        fsa->analogInputPower = 0.0f;
    }
    if (objdata->unkA56 <= 0) {
        return 3;
    }
    if (fsa->enteredAnimState != 0) {
        fsa->animStateTime = 0;
        fsa->animTickDelta = 0.01f;
        fsa->unk2B0 = 18.0f;
        if (self->curModAnimId != _data_DC[0]) {
            func_80023D30(self, _data_DC[0], 0.0f, 0);
            func_80024D74(self, 0x14);
        }
    }
    if ((fsa->prevAnalogInputPower > 0.0f) && (fsa->analogInputPower > 0.0f)) {
        return 5;
    }
    if (fsa->unk310 & 0x8000) {
        return 6;
    }
    if (rand_next(0, 60) == 0) {
        func_80034B54(self, &objdata->unk394, _data_8[rand_next(0, 2)], 0);
    }
    return 0;
}

// offset: 0x2454 | func: 28
s32 dll_713_func_2454(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DRearthwalk_Data* objdata = self->data;

    fsa->flags |= 0x200000;
    objdata->unkA62 = 3;
    switch (self->curModAnimId) {
    case 10:
        self->unkAF |= 8;
        if (fsa->unk33A != 0) {
            func_80023D30(self, 0xE, 0.0f, 0);
            fsa->animTickDelta = 0.01f;
        }
        if (objdata->unkA56 > 0) {
            if ((fsa->animTickDelta > 0.0f) && ((fsa->unk310 != 0) || (fsa->xAnalogInput != 0.0f) || (fsa->yAnalogInput != 0.0f))) {
                fsa->animTickDelta = -fsa->animTickDelta;
            }
        }
        break;
    case 12:
    case 14:
    case 16:
    case 17:
    case 18:
    case 19:
        fsa->animTickDelta = 0.005f;
        if ((objdata->unkA56 != 0) && (objdata->unkA56 >= 5)) {
            func_80023D30(self, 0xB, 0.0f, 0U);
            fsa->animTickDelta = 0.01f;
        }
        break;
    case 11:
        if (fsa->unk33A != 0) {
            objdata->unk3B8.unk4A9 &= ~0x2;
            if (objdata->unkA58 & 0x20) {
                objdata->unkA60_4 = 1;
            } else {
                objdata->unkA62 = 9;
            }
            objdata->unk3B8.unk4A9 &= ~1;
            return 2;
        }
        break;
    default:
        objdata->unkA60_4 = 0;
        func_80023D30(self, 0xA, 0.0f, 0);
        objdata->unk3B8.unk4A9 |= 2;
        dll_713_func_2174(self, fsa);
        fsa->animTickDelta = 0.01f;
        break;
    }

    if (rand_next(0, 30) == 0) {
        func_80034B54(self, &objdata->unk394, _data_8[rand_next(0, 4)], 0);
    }
    return 0;
}

// offset: 0x2704 | func: 29
s32 dll_713_func_2704(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DRearthwalk_Data* objdata = self->data;
    s16 temp_ft1;
    
    fsa->flags |= OBJFSA_FLAG_200000;
    if ((fsa->unk328 < (objdata->unkA52 / 3)) || (fsa->analogInputPower == 0.0f)) {
        return 5;
    }
    if (fsa->unk32A < -0xAF) {
        fsa->unk32A = -fsa->unk32A;
    }
    if (self->curModAnimId != 5) {
        func_80023D30(self, 5, 0.0f, 0U);
    }
    temp_ft1 = (f32) fsa->unk32A * updateRate * 182.04f;
    self->srt.yaw += temp_ft1 >> 4;
    dll_713_func_3210(self, temp_ft1, 0);
    fsa->animTickDelta = fsa->analogInputPower * 0.08f;
    if (fsa->unk310 & 0x8000) {
        return 6;
    }
    return 0;
}

// offset: 0x2860 | func: 30
#ifndef NON_MATCHING
s32 dll_713_func_2860(Object *self, ObjFSA_Data *fsa, f32 updateRate);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/713_DR_EarthWarrior/dll_713_func_2860.s")
#else
s32 dll_713_func_2860(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    u32 var_v1_3;
    s16 temp2;
    u32 var_t0;
    f32 var_fa0;
    f32 temp_fv1;
    s32 _pad2;
    DRearthwalk_Data* sp2C;
    s32 _pad3;
    s32 _pad4;
    s32 sp20;
    f32 var_fv0;
    s32 temp;

    sp2C = self->data;
    sp20 = 1;
    fsa->flags |= 0x200000;
    if (fsa->enteredAnimState != 0) {
        fsa->unk32A = 0;
        fsa->unk328 = 0;
    }
    if (fsa->analogInputPower < 0.08f) {
        fsa->unk328 = 0;
        fsa->unk32A = 0;
        fsa->analogInputPower = 0.0f;
    }
    fsa->unk32A = CLAMP_EXPR(fsa->unk32A, -0x1000, 0x1000);
    if (fsa->unk32A >= 0) {
        temp = fsa->unk32A;
    } else {
        temp = -fsa->unk32A;
    }
    if (sp2C->unkA52 < temp) {
        temp2 = (f32) fsa->unk32A * updateRate * 182.04f;
        self->srt.yaw += temp2 >> 5;
    } else {
        self->srt.yaw += ((((f32) fsa->unk32A * updateRate) / 36.0f) * 182.0f);
    }
    dll_713_func_3210(self, (s16) ((f32) fsa->unk32A * updateRate * 182.04f), 0);
    var_fv0 = fsa->analogInputPower;
    if (var_fv0 < 0.0f) {
        var_fv0 = 0.0f;
    }
    if (var_fv0 > 1.0f) {
        var_fv0 = 1.0f;
    }
    var_fa0 = var_fv0 * 2.7f;
    if (var_fa0 < 0) {
        var_fa0 = 0;
    }
    fsa->speed += (((var_fa0 - fsa->speed) / fsa->unk2B0) * updateRate);
    if (self->srt.pitch > 0) {
        var_fa0 -= (fsin16_precise(self->srt.pitch) * 0.3f);
    } else {
        var_fa0 -= (fsin16_precise(self->srt.pitch) * 0.15f);
    }
    fsa->unk278 += (((var_fa0 - fsa->unk278) / fsa->unk2B0) * updateRate);
    var_t0 = 0;
    temp_fv1 = self->animProgress;
    var_v1_3 = 0;
    while (self->curModAnimId != _data_DC[var_v1_3] && var_v1_3 < 5) {
        var_v1_3++;
    }
    if (var_v1_3 >= 5) {
        var_v1_3 = 0;
    }
    if (rand_next(0, 0x46 - (s32) ((fsa->speed / 2.7f) * 50.0f)) == 0) {
        func_80034B54(self, &sp2C->unk394, _data_8[rand_next(0, 3)], 0);
    }

    while (sp20) {
        if (fsa->speed < _data_B4[var_v1_3][0]) {
            if (var_v1_3 == 1) {
                return 2;
            }
            var_v1_3 -= 1;
            var_t0 = 1;
        } else if (_data_B4[var_v1_3][1] <= fsa->speed) {
            if (var_v1_3 == 0) {
                temp_fv1 = 0.0f;
            }
            var_v1_3 += 1;
            var_t0 = 1;
        } else {
            sp20 = 0;
        }
    }
    if (var_t0 != 0) {
        func_80023D30(self, _data_DC[var_v1_3], temp_fv1, 0);
        func_80024D74(self, 0xA);
    }
    func_8002493C(self, fsa->unk278, &fsa->animTickDelta);
    if (fsa->unk310 & 0x8000) {
        joy_disable_buttons(0, 0x8000);
        return 6;
    }
    return 0;
}
#endif

// offset: 0x2D80 | func: 31
s32 dll_713_func_2D80(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    ObjectHitInfo* hit;
    DRearthwalk_Data* objdata;

    hit = self->objhitInfo;
    objdata = self->data;
    fsa->flags |= OBJFSA_FLAG_200000;
    if (fsa->enteredAnimState != 0) {
        objdata->unkA58 &= ~0x8;
        hit->unk58 |= 0x200;
        if (fsa->unk278 > 0.45f) {
            func_80023D30(self, 0x14, 0.0f, 0);
            fsa->animTickDelta = 0.021f;
            hit->unk60 = fsa->unk278 > 1.93f ? 3 : 2;
        } else {
            func_80023D30(self, 0x15, 0.0f, 0);
            fsa->animTickDelta = 0.011f;
            hit->unk60 = 1;
        }
    }
    if (self->animProgress < 0.4f) {
        if ((self->animProgress + (fsa->animTickDelta * (f32) gUpdateRate)) > 0.4f) {
            objdata->unkA60_5 = 1;
        }
    }
    hit->unk5F = 5;
    if (fsa->unk33A != 0) {
        objdata->unkA60_5 = 0;
        return 2;
    }
    if (self->curModAnimId == 0x14) {
        gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    }
    return 0;
}

// offset: 0x2F90 | func: 32
s32 dll_713_func_2F90(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DRearthwalk_Data* objdata = self->data;

    dll_713_func_2174(self, fsa);
    if (fsa->enteredAnimState != 0) {
        if (objdata->unkA56 > 0) {
            if (objdata->unkA60_0 != 0) {
                func_80023D30(self, 7, 0.0f, 0);
            } else {
                func_80023D30(self, 8, 0.0f, 0);
            }
        } else if (objdata->unkA60_0 != 0) {
            func_80023D30(self, 0x12, 0.0f, 0);
        } else {
            func_80023D30(self, 0x13, 0.0f, 0);
        }
        fsa->animTickDelta = 0.02f;
    }
    if (self->animProgress > 0.9f) {
        objdata->unkA56--;
        if (objdata->unkA56 <= 0) {
            if (objdata->unkA56 < -20) {
                main_set_bits(BIT_5DE, 1);
            }
            return 3;
        }
        return 2;
    }
    return 0;
}

// offset: 0x311C | func: 33
s32 dll_713_func_311C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    DRearthwalk_Data* objdata = self->data;
    
    if (fsa->enteredAnimState != 0) {
        dll_713_func_2174(self, fsa);
        func_80023D30(self, *_data_DC, 0.0f, 0);
    }
    if ((main_get_bits(BIT_5CF) == 0) && (main_get_bits(BIT_5CD) != 0)) {
        main_set_bits(BIT_656, 1);
        objdata->unkA60_4 = 1;
        return 2;
    }
    return 0;
}

// offset: 0x31F8 | func: 34
s32 dll_713_func_31F8(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    return 0;
}

// offset: 0x3210 | func: 35
static void dll_713_func_3210(Object* self, s16 arg1, s16 arg2) {
    s16* temp_v0;

    temp_v0 = func_80034804(self, 9);
    if (temp_v0 != NULL) {
        temp_v0[1] += ((arg1 - temp_v0[1]) >> 5);
        temp_v0[0] += ((arg2 - temp_v0[0]) >> 5);
        
        // @bug? should be clamping [0]?
        temp_v0[1] = CLAMP_EXPR(temp_v0[1], -0x1555, 0x1555);
        temp_v0[1] = CLAMP_EXPR(temp_v0[1], -0x1555, 0x1555);
    }
}

// offset: 0x32EC | func: 36
static s32 dll_713_func_32EC(Object* self, u8 arg1) {
    DRearthwalk_Data* objdata;

    objdata = self->data;
    switch (arg1) {
        case 5:
            objdata->unkA58 |= 0x80;
            objdata->unkA62 = 0xA;
            //Set Mind Read text
            if (!gDLL_22_Subtitles->vtbl->func_21C0(self->id, GAMETEXT_0CF_DR_Mind_Read_messages_4)) {
                break;
            }
        // fallthrough
        case 1:
            objdata->unkA62 = 2;
            func_80000450(self, self, 0x22C, 0, 0, 0);
            break;
        case 2:
            func_80000450(self, self, 0x22E, 0, 0, 0);
            objdata->unkA58 &= ~0x80;
            //Set Mind Read text
            gDLL_22_Subtitles->vtbl->func_21C0(self->id, GAMETEXT_0D0_DR_Mind_Read_messages_5);
    }

    return 1;
}

// offset: 0x3420 | func: 37 | export: 20
void dll_713_func_3420(Object* self, s32 arg1, s32 arg2) {
    DRearthwalk_Data* objdata = self->data;
    if (arg1 == 1) {
        objdata->unkA56 += 12;
        func_80034B54(self, &objdata->unk394, _data_24[0], 1);
    }
}
