#include "common.h"

#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/278_flameblast.h"
#include "dlls/objects/280_sidekicktoy.h"
#include "dlls/objects/332_FXEmit.h"
#include "sys/objexpr.h"
#include "sys/objtype.h"
#include "sys/objlib.h"
#include "sys/route.h"

#include "prevent_bss_reordering.h"

typedef s32 (*DLL210_Unk5FC_Callback)(Object* , s32, void *, Object* );

// size: 0x610
typedef struct {
    void *unk0; // holds a pointer to a (loaded) DLL
    SidekickStats *unk4;
    Object* unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A; // some sort of state?
    u8 unk1B; //bitfield of available sidekick commands
    u8 unk1C;
    u8 unk1D;
    u16 pad1E;
    f32 unk20;
    f32 unk24;
    Object* unk28;
    Vec3f *unk2C;
    f32 unk30[2];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    s32 unk4C; // some sort of flag
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    s16 unk64;
    s16 unk66;
    Vec3f *unk68;
    s16 unk6C[4];
    Vec3f unk74[4];
    u16 unkA4;
    s16 unkA6;
    Vec3f unkA8;
    Vec3f unkB4;
    Vec3f unkC0;
    DLL27_Data unkCC;
    Object* unk32C;
    f32 unk330;
    s32 unk334;
    Object* unk338;
    HeadAnimation unk33C;
    HeadAnimation unk360;
    Vec3f unk384[4];
    Vec3f unk3B4;
    CurveSetup *unk3C0;
    u8 unk3C4;
    UnkCurvesStruct unk3C8;
    CurveSetup *unk4D0;
    CurveSetup *unk4D4;
    u16 unk4D8;
    u8 unk4DA;
    u8 pad4DB;
    Route unk4DC[4];
    Route unk59C;
    CurveSetup *unk5CC;
    void *unk5D0;
    Vec3f *unk5D4;
    Vec3f unk5D8;

    // unk5E4 .. unk600 are dynamic based on something?
    Object* unk5E4[3]; // objects or floats or curve setups or object setups?
    union {
        // sometimes float or u32 as well
        Object* unk5F0;
        f32 unk5F0_f32;
        u32 unk5F0_u32;
    };
    union {
        Vec3f unk5F4_vec;
        struct {
            union {
                CurveSetup *unk5F4;
                u32 unk5F4_amsFxID;
            };
            union {
                CurveSetup *unk5F8;
                s32 unk5F8_s32;
            };
            union {
                DLL210_Unk5FC_Callback unk5FC;
                s32 unk5FC_s32;
            };
        };
    };
    f32 unk600;
    Object* unk604;
    s32 unk608;
    u8 unk60C;
} DLL211_Data;

static void dll_211_func_1408(Object* self, Object* arg1);
static s32 dll_211_func_1624(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3);
static void dll_211_func_183C(Object* self, DLL211_Data* objData);
static void dll_211_func_18C4(Object* self, DLL211_Data* objData);
static void dll_211_func_1F3C(Object* self, DLL211_Data* objData);
static void dll_211_func_23B8(Object* self, DLL211_Data* objData);
static void dll_211_func_2928(Object* self, DLL211_Data* objData);
static void dll_211_func_2938(Object* self, DLL211_Data* objData);
static void dll_211_func_2B50(Object* self, DLL211_Data* objData);
static void dll_211_func_2C64(Object* self, DLL211_Data* objData);
static void dll_211_func_3188(Object* self, DLL211_Data* objData);
static void dll_211_func_3560(Object* self, DLL211_Data* objData);
static void dll_211_func_3664(Object* self, DLL211_Data* objData);
static void dll_211_func_3CAC(Object* self, DLL211_Data* objData);
static void dll_211_func_4340(Object* self, DLL211_Data* objData);
static void dll_211_func_43AC(Object* self, DLL211_Data* objData);
static void dll_211_func_4418(Object* self, DLL211_Data* objData);
static void dll_211_func_47E8(Object* self, DLL211_Data* objData);
static void dll_211_func_4974(Object* self, DLL211_Data* objData);
static void dll_211_func_4C94(Object* self, DLL211_Data* objData);
static s32 dll_211_func_4F3C(Object* self, f32 arg1, DLL211_Data* arg2, Vec3f* arg3);
static void dll_211_func_514C(Object* self, DLL211_Data* objData);
static void dll_211_func_52B8(Object* self, DLL211_Data* objData);
static s32 dll_211_func_53E4(Object* self, f32 arg1, DLL211_Data* arg2);
static void dll_211_func_7188(Object* self);
static void dll_211_func_726C(Object* self);
static void dll_211_func_74C4(Object* self, Gfx** arg1, Mtx** arg2, Vtx** arg3, Triangle** pols);
static void dll_211_func_7794(Object* self, u8 *arg1, s16 arg2, CurveSetup** arg3);
static CurveSetup* dll_211_func_7A7C(DLL211_Data* objData, CurveSetup* arg1, s32 arg2, u8 arg3);
static s8 dll_211_func_7BAC(DLL211_Data* objData, CurveSetup** arg1, u8 *arg2, s32 arg3);
static CurveSetup* dll_211_func_7DB8(DLL211_Data* objData, CurveSetup* arg1, void* arg2);
static CurveSetup* dll_211_func_7EFC(DLL211_Data* objData, CurveSetup* arg1, s32 arg2, s32 arg3);
static CurveSetup* dll_211_func_8114(CurveSetup* setup);
static void dll_211_func_81A8(Object* self);
static f32 dll_211_func_81D8(Object* self);
static void dll_211_func_82B8(DLL211_Data* objData);
static void dll_211_func_8308(Object* self, s32 arg1);
static void dll_211_func_83BC(Object* self, DLL211_Data* objData);
static void dll_211_func_8470(Object* self, Vec3f* arg1);
static void dll_211_func_871C(Object* self, s8* arg1);
static void dll_211_func_87E4(Object* self);
static void dll_211_func_885C(Object* self);
static void dll_211_func_88F4(Object* self, s16 arg1);
static void dll_211_func_8974(Object* self, UnkCurvesStruct* arg1, f32 arg2);
static void dll_211_func_8A94(Object* self, UnkCurvesStruct* arg1);
static void dll_211_func_8B5C(Object* self, s32 arg1);
static void dll_211_func_8BEC(Object* self);
static void dll_211_func_8ED0(f32 arg0, f32 arg1, f32* arg2);
static void dll_211_func_8F18(DLL211_Data* objData);
static void dll_211_func_8F84(Object* self, Vec3f* arg1, f32* arg2);
static void dll_211_func_9024(DLL211_Data* objData, Vec3f *arg1);
static void dll_211_func_9050(Object* self, DLL211_Data* objData);
static void dll_211_func_9200(Object* self, DLL211_Data* objData);
static void dll_211_func_940C(Object* self, DLL211_Data* objData);
static Object* dll_211_func_94BC(Object* self, f32 arg1);
static void dll_211_func_95E0(Object* self, DLL211_Data* objData, s16* arg2);
static s32 dll_211_func_9668(DLL27_Data* objData);

/*0x0*/ static s16 _data_0[][3] = {
    { 0x06d7, 0x0500, 0x0000 },
    { 0x01dc, 0x0500, 0x0000 },
    { 0x095b, 0x0100, 0x0000 },
    { 0x0037, 0x1000, 0x0000 },
    { 0x0038, 0x1000, 0x0000 },
    { 0x0039, 0x1000, 0x0000 },
    { 0x04b8, 0x1500, 0x0014 },
    { 0x002a, 0x0100, 0x0000 },
    { 0x002b, 0x0100, 0x0000 },
    { 0x002c, 0x0100, 0x0000 },
    { 0x0034, 0x0100, 0x0000 },
    { 0x0035, 0x0100, 0x0000 },
    { 0x0036, 0x0100, 0x0000 },
    { 0x003a, 0x0100, 0x0000 },
    { 0x003b, 0x0100, 0x0000 },
    { 0x003c, 0x0100, 0x0000 },
    { 0x003d, 0x0100, 0x0000 },
    { 0x003e, 0x0100, 0x0000 },
    { 0x003f, 0x1500, 0x0000 },
};
/*0x74*/ static Vec3f _data_74[] = {
    VEC3F(0.0f, 0.0f, 0.0f),
    VEC3F(0.0f, 17.0f, 0.0f)
};
/*0x8C*/ static f32 _data_8C[] = { 0.0f, 8.5f };
/*0x94*/ static Vec3f _data_94[] = VEC3F(0.0f, 0.0f, 0.0f);
/*0xA0*/ static f32 _data_A0 = 8.0f;
/*0xA4*/ static f32 _data_A4[] = {
    0.0f, 0.33f, 0.99f, 1.98f, 3.3f, 4.95f,
    6.93f, 9.24f, 11.88f, 14.85f, 18.18f, 21.84f,
    25.83f, 30.15f, 34.8f, 39.78f, 45.09f, 50.73f
};
/*0xEC*/ static u8 _data_EC[] = {
    0, 0, 2, 0, 4, 0, 0, 7, 7, 0, 10, 0, 0, 13, 0, 0, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 26, 28, 28, 0, 0, 0,
    0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 19, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
/*0x164*/ static u16 _data_164 = 0x0102;

/*0x0*/ static u8 _bss_0[0x8];
typedef void (*Bss8)(Object* , DLL211_Data* );
/*0x8*/ static Bss8 _bss_8[18];

// offset: 0x0 | ctor
void dll_211_ctor(void* dll) {
    _bss_8[0] = (Bss8) dll_211_func_183C; // talk?
    _bss_8[1] = (Bss8) dll_211_func_18C4; // eat?
    _bss_8[2] = (Bss8) dll_211_func_1F3C; // dig?
    _bss_8[3] = (Bss8) dll_211_func_23B8; // dig + curves?
    _bss_8[4] = (Bss8) dll_211_func_2928; // empty
    _bss_8[5] = (Bss8) dll_211_func_2938; // ??
    _bss_8[6] = (Bss8) dll_211_func_2B50; // something obj seq
    _bss_8[7] = (Bss8) dll_211_func_2C64; // ??
    _bss_8[8] = (Bss8) dll_211_func_3188; // ??
    _bss_8[9] = (Bss8) dll_211_func_3560; // empty
    _bss_8[10] = (Bss8) dll_211_func_3664; // ??
    _bss_8[11] = (Bss8) dll_211_func_3CAC; // ??
    _bss_8[12] = (Bss8) dll_211_func_4340; // something related to water?
    _bss_8[13] = (Bss8) dll_211_func_43AC; // something related to water?
    _bss_8[14] = (Bss8) dll_211_func_4418; // ??
    _bss_8[15] = (Bss8) dll_211_func_47E8; // ??
    _bss_8[16] = (Bss8) dll_211_func_4974; // ??
    _bss_8[17] = (Bss8) dll_211_func_4C94; // ??
}

// offset: 0xF8 | dtor
void dll_211_dtor(void *dll) { }

// offset: 0x104 | func: 0 | export: 0
void dll_211_setup(Object* self, ObjSetup* setup, s32 arg2) {
    f32 var_ft5;
    f32 var_fv0;
    f32 var_fv1;
    DLL211_Data* objData;
    s32 pad;
    s32 pad2;
    s32 i;
    u8 pad3;
    u16 sp50;
    s32 pad4;
    s32 temp_v1;

    objData = self->data;
    sp50 = _data_164;
    main_set_bits(BIT_4E3, 0xFF);
    if (main_get_bits(BIT_SW_Tricky_Ball_Collected) != 0) {
        main_set_bits(BIT_Tricky_Ball_Unlocked, 1);
    }

    self->animCallback = (AnimationCallback ) dll_211_func_1624;
    obj_add_object_type(self, 1);
    route_init(&objData->unk4DC[0]);
    route_init(&objData->unk4DC[1]);
    route_init(&objData->unk4DC[2]);
    route_init(&objData->unk4DC[3]);
    route_init(&objData->unk59C);
    objData->unk4 = gDLL_29_Gplay->vtbl->get_sidekick_stats();
    objData->unk8 = get_player();
    temp_v1 = _data_EC[func_80045D58()];
    objData->unk18 = 0;
    objData->unk1B = 0;
    objData->unk5D4 = 0;
    objData->unkA4 = 0;
    objData->unk14 = temp_v1;
    objData->unk10 = temp_v1;
    objData->unkC = temp_v1;
    gDLL_27->vtbl->init(&objData->unkCC, 0x02000000, 0xA7, 1);
    gDLL_27->vtbl->setup_hits_collider(&objData->unkCC, 1, _data_94, &_data_A0, 2);
    gDLL_27->vtbl->setup_terrain_collider(&objData->unkCC, 2, _data_74, _data_8C, (u8* ) &sp50);
    gDLL_25->vtbl->func_D8();
    for (i = 0, var_ft5 = 0/*.0f*/; var_ft5 < 1.8f; i++, var_ft5 += 0.1f) {
        for (var_fv0 = var_ft5, var_fv1 = 0.0f; var_fv0 > 0.0f; var_fv0 -= 0.09f) {
            var_fv1 += var_fv0 * 3.0f;
        }
        _data_A4[i] = var_fv1 * 1.75f;
    }
}

// offset: 0x3FC | func: 1 | export: 1
void dll_211_control(Object* self) {
    s32 temp_v0;
    s32 sp90;
    ObjSetup* temp_v0_2;
    DLL211_Data* objData = self->data;
    f32 sp84;
    UnkFunc_80024108Struct sp68;
    s32 sp50[6] = { 0, 1, 2, 3, 4, 5 };
    Object* sp4C;
    Object* temp_v0_3;

    // FAKE
    if ((s32)self);
    sp90 = _data_EC[func_80045D58()];
    objData->unk14 = _data_EC[map_world_xz_to_map_id(objData->unk8->globalPosition.x, objData->unk8->globalPosition.z)];
    if ((sp90 != objData->unkC) && (sp90 != objData->unk10)) {
        if (objData->unkC == objData->unk14) {
            objData->unk10 = sp90;
        } else {
            objData->unkC = sp90;
        }
        gDLL_25->vtbl->func_D8();
    }
    if (objData->unk4C & 0x200) {
        if (!(objData->unk4C & 0x4000)) {
            gDLL_27->vtbl->reset(self, &objData->unkCC);
            dll_211_func_82B8(objData);
            objData->unk19 = 0;
            objData->unk20 = 0.0f;
        }
        objData->unk4C &= ~0x4200;
    }
    if (objData->unk28 != NULL && (objData->unk28->stateFlags & 0x40)) {
        dll_211_func_82B8(objData);
        objData->unk28 = NULL;
    }
    temp_v0 = gDLL_1_cmdmenu->vtbl->was_used_item_in_gamebit_array(sp50, 6);
    if ((objData->unk18 == 0xB) && (temp_v0 == 4)) {
        objData->unk60C ^= 1;
    } else if ((objData->unk18 == 0x10) && (temp_v0 == 4)) {
        objData->unk5F8_s32 = 1;
    } else if ((objData->unk18 == 17) && (temp_v0 == 4)) {
        objData->unk5F8_s32 = 1;
    } else if (!(objData->unk4C & 0x10)) {
        switch (temp_v0) {
        case 0:
            dll_211_func_82B8(objData);
            objData->unk5F4_vec.x = 600.0f;
            break;
        case 1:
            objData->unk1D = 1;
            dll_211_func_8F18(objData);
            gDLL_6_AMSFX->vtbl->play(self, rand_next(50, 51), 0x7F, NULL, NULL, 0, NULL);
            switch (objData->unk28->id) {
            case 0x1CA:
                objData->unk18 = 2;
                temp_v0_2 = objData->unk28->setup;
                objData->unk5F8_s32 = ((s16*)temp_v0_2)[13];
                objData->unk5FC_s32 = ((s16*)temp_v0_2)[14];
                break;
            case 0x160:
                objData->unk18 = 3;
                break;
            case 0x31C:
                objData->unk18 = 8;
                break;
            case 0x6A:
            case 0x3FB:
                objData->unk18 = 12;
                break;
            case 0x352:
                objData->unk18 = 2;
                objData->unk5F8_s32 = 0;
                objData->unk5FC_s32 = 1;
                break;
            case 0x358:
                objData->unk18 = 17;
                break;
            default:
                dll_211_func_82B8(objData);
                break;
            }
            break;
        case 2:
            objData->unk1D = 2;
            sp84 = M_INFINITY_F;
            sp4C = obj_get_nearest_type_to(OBJTYPE_Baddie, self, &sp84);
            temp_v0_3 = obj_get_nearest_type_to(OBJTYPE_40, self, &sp84);
            if (temp_v0_3 != NULL) {
                dll_211_func_1408(self, temp_v0_3);
            } else {
                dll_211_func_1408(self, sp4C);
            }
            break;
        case 3:
            objData->unk1D = 3;
            dll_211_func_8F18(objData);
            switch (objData->unk28->id) {
            case OBJ_DR_PressurePad:
                objData->unk18 = 13;
                break;
            case OBJ_VFP_PuzzlePad:
                objData->unk18 = 13;
                break;
            default:
                objData->unk18 = 11;
            }
            break;
        case 4:
            objData->unk1D = 4;
            dll_211_func_8F18(objData);
            objData->unk18 = 10;
            switch (objData->unk28->id) {
            case OBJ_DIMIceWall:
                objData->unk5FC = (DLL210_Unk5FC_Callback) ((DLL_Unknown*)objData->unk28->dll)->vtbl->func[7].withFourArgsS32;
                break;
            case OBJ_VFP_flamepoint:
                objData->unk5FC = (DLL210_Unk5FC_Callback) ((DLL_Unknown*)objData->unk28->dll)->vtbl->func[7].withFourArgsS32;
                break;
            case OBJ_DIMLogFire:
                objData->unk5FC = (DLL210_Unk5FC_Callback) ((DLL_Unknown*)objData->unk28->dll)->vtbl->func[7].withFourArgsS32;
                break;
            case 0x470:
                objData->unk5FC = (DLL210_Unk5FC_Callback) ((DLL_Unknown*)objData->unk28->dll)->vtbl->func[7].withFourArgsS32;
                break;
            case OBJ_SHvines:
            case OBJ_DR_Vines:
            case OBJ_DB_FlameSwitch:
                objData->unk5FC = 0;
                break;
            default:
                dll_211_func_82B8(objData);
                break;
            }
            break;
        case 5:
            objData->unk1D = 5;
            temp_v0_2 = obj_alloc_setup(sizeof(SidekickToy_Setup), OBJ_SidekickBall);
            temp_v0_2->fadeDistance = 0x96;
            temp_v0_2->loadFlags = 2;
            temp_v0_2->x = self->globalPosition.x;
            temp_v0_2->y = self->globalPosition.y;
            temp_v0_2->z = self->globalPosition.z;
            objData->unk28 = obj_create(temp_v0_2, 5, -1, -1, self->parent);
            dll_211_func_9024(objData, &objData->unk28->globalPosition);
            objData->unk1A = 0;
            objData->unk18 = 14;
            break;
        default:
            if ((objData->unk18 == 1) && (objData->unk5F4_vec.x <= 0.0f)) {
                sp84 = 150.0f;
                if (dll_211_func_94BC(objData->unk8, 150.0f) != NULL) {
                    objData->unk18 = 0x10;
                    objData->unk1A = 0;
                }
            }
            break;
        }
    } else if ((gDLL_1_cmdmenu->vtbl->was_this_item_used(0) != 0) && (objData->unk18 == 7)) {
        ((s16*)objData->unk5E4)[0] = 1;
    }
    self->unkAF |= 8;
    objData->unkCC.mode = 1;
    objData->unk24 += gUpdateRateF;
    _bss_8[objData->unk18](self, objData);
    func_80024108(self, objData->unk38, gUpdateRateF, &sp68);
    if (objData->unk4C & 0x40) {
        self->srt.transl.x += objData->unk30[0] * -sp68.unk0[2] * objData->unk40;
        self->srt.transl.z += objData->unk30[1] * -sp68.unk0[2] * objData->unk40;
    }
    if (objData->unk4C & 0x80) {
        self->srt.transl.y += sp68.unk0[1] * objData->unk44;
    }
    if (objData->unk4C & 0x20) {
        self->srt.transl.x += objData->unk30[1] * sp68.unk0[0] * objData->unk3C;
        self->srt.transl.z += objData->unk30[0] * -sp68.unk0[0] * objData->unk3C;
    }
    if (objData->unk4C & 0x100) {
        self->srt.yaw += sp68.unkC[1] * objData->unk48;
    }
    if (objData->unk28 != NULL) {
        objData->unk33C.aimIsActive = 1;
        objData->unk33C.headAimX = objData->unk28->srt.transl.x;
        objData->unk33C.headAimY = objData->unk28->srt.transl.y;
        objData->unk33C.headAimZ = objData->unk28->srt.transl.z;
    } else {
        objData->unk33C.aimIsActive = 0;
    }
    func_800328F0(self, &objData->unk33C, 0.0f);
    func_80032A08(self, &objData->unk33C);
    func_80034BC0(self, &objData->unk360);
    dll_211_func_81A8(self);
    dll_211_func_8BEC(self);
}

// offset: 0xD1C | func: 2 | export: 2
void dll_211_update(Object* self) { }

// offset: 0xD28 | func: 3 | export: 3
void dll_211_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DLL211_Data* objData;

    if (visibility == 0) {
        return;
    }

    objData = self->data;
    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    dll_211_func_7188(self);
    if (objData->unk4C & 0x10) {
        switch (objData->unk18) {
        case 2:
            dll_211_func_726C(self);
            break;
        case 3:
            if (objData->unk1A == 4) {
                dll_211_func_726C(self);
            }
            break;
        case 8:
            if ((objData->unk1A == 3) || (objData->unk1A == 2)) {
                dll_211_func_74C4(self, gdl, mtxs, (Vtx** ) vtxs, pols);
                break;
            }
            if (objData->unk1A != 4) {
                break;
            }
            return;
        }

        if (objData->unk18 == 14) {
            if (objData->unk1A >= 3) {
                if (objData->unk1A != 3) {
                    objData->unk5E4[0]->srt.transl.x = objData->unk3B4.x;
                    objData->unk5E4[0]->srt.transl.y = objData->unk3B4.y;
                    objData->unk5E4[0]->srt.transl.z = objData->unk3B4.z;
                }
                draw_object(objData->unk5E4[0], gdl, mtxs, vtxs, pols, 1.0f);
            }
        }
    }

    dll_211_func_9200(self, objData);
}

// offset: 0xF20 | func: 4 | export: 4
void dll_211_free(Object* self, s32 a1) {
    DLL211_Data* temp_s0;

    temp_s0 = self->data;
    route_free(&temp_s0->unk4DC[0]);
    route_free(&temp_s0->unk4DC[1]);
    route_free(&temp_s0->unk4DC[2]);
    route_free(&temp_s0->unk4DC[3]);
    route_free(&temp_s0->unk59C);
    obj_free_object_type(self, 1);
    gDLL_13_Expgfx->vtbl->func4(self);
    gDLL_14_Modgfx->vtbl->func4(self);
    if (a1 == 0) {
        dll_211_func_940C(self, temp_s0);
    }
}

// offset: 0x1040 | func: 5 | export: 5
u32 dll_211_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1050 | func: 6 | export: 6
u32 dll_211_get_data_size(Object* self, u32 a1) {
    return sizeof(DLL211_Data);
}

// offset: 0x1064 | func: 7 | export: 7
s32 dll_211_func_1064(s32 arg0) {
    return 0;
}

// offset: 0x1074 | func: 8 | export: 8
void dll_211_func_1074(Object* self, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2, UNK_TYPE_32 arg3) {
}

// offset: 0x108C | func: 9 | export: 9
void dll_211_func_108C(Object* self, UNK_TYPE_32 arg1) {
}

// offset: 0x109C | func: 10 | export: 10
void dll_211_func_109C(Object* self, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2, UNK_TYPE_32 arg3) {
}

// offset: 0x10B4 | func: 11 | export: 11
f32 dll_211_func_10B4(Object* self) {
    DLL211_Data* objData;

    objData = self->data;
    return objData->unk20;
}

// offset: 0x10C4 | func: 12 | export: 12
DLL27_Data* dll_211_func_10C4(s32 arg0) {
    return NULL;
}

// offset: 0x10D4 | func: 13 | export: 13
s32 dll_211_get_available_commands(Object* self) {
    DLL211_Data* objData;
    s32 sideCmdBits;

    objData = self->data;

    if (main_get_bits(BIT_Tricky_Unlocked_Sidekick_Commands)) {
        sideCmdBits = objData->unk1B | Sidekick_Command_FLAG_01_Heel;

        //Check if Flame command should be shown
        if ((objData->unk18 == 0xB) || 
            ((objData->unk18 == 0x10) && (objData->unk1A == 1)) || 
            ((objData->unk18 == 17) && (objData->unk1A == 1))
        ) {
            sideCmdBits |= Sidekick_Command_FLAG_10_Flame;
        }

        //Check if Tricky's ball is unlocked
        if (main_get_bits(BIT_Tricky_Ball_Unlocked)) {
            sideCmdBits |= Sidekick_Command_FLAG_20_Play;
        }

        //Check if Guard isn't unlocked
        if (main_get_bits(BIT_Tricky_Learned_Guard) == FALSE) {
            sideCmdBits &= ~Sidekick_Command_FLAG_08_Guard;
        }

        //Check if Distract isn't unlocked
        if (main_get_bits(BIT_Tricky_Learned_Distract) == FALSE) {
            sideCmdBits &= ~Sidekick_Command_FLAG_04_Distract;
        }

        //Check if Flame isn't unlocked
        if (main_get_bits(BIT_Tricky_Learned_Flame) == FALSE) {
            sideCmdBits &= ~Sidekick_Command_FLAG_10_Flame;
        }
        
        objData->unk1B = 0;
        
        return sideCmdBits;
    }
    
    return NO_SIDEKICK_COMMAND;
}

// offset: 0x1230 | func: 14 | export: 26
s32 dll_211_func_1230(Object* self, s32* arg1) {
    DLL211_Data* objData = self->data;
    *arg1 = objData->unk1D;
    return TRUE;
}

/**
 * Enables a sidekick command (e.g. allowing Flame when near vines).
 */
// offset: 0x1248 | func: 15 | export: 14
void dll_211_enable_command(Object* self, s32 commandIndex) {
    DLL211_Data* objData = self->data;
    
    if (commandIndex < TOTAL_SIDEKICK_COMMANDS) {
        objData->unk1B |= (1 << commandIndex);
    }
}

// offset: 0x1270 | func: 16 | export: 15
u8 dll_211_get_blue_food_count(Object* self) {
    DLL211_Data* objData;

    objData = self->data;
    return objData->unk4->blueFood;
}

// offset: 0x1284 | func: 17 | export: 16
u8 dll_211_get_red_food_count(Object* self) {
    DLL211_Data* objData;

    objData = self->data;
    return objData->unk4->redFood;
}

// offset: 0x1298 | func: 18 | export: 17
s32 dll_211_get_blue_and_red_food_count(Object* self) {
    DLL211_Data* objData;

    objData = self->data;
    return objData->unk4->blueFood + objData->unk4->redFood;
}

// offset: 0x12B4 | func: 19 | export: 18
void dll_211_set_red_food_count(Object* self, s32 count) {
    DLL211_Data* objData = self->data;
    objData->unk4->redFood = count;
}

// offset: 0x12C8 | func: 20 | export: 19
s32 dll_211_func_12C8(s32 arg0) {
    return 1;
}

// offset: 0x12D8 | func: 21 | export: 20
void dll_211_func_12D8(Object* self, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2, UNK_TYPE_32 arg3) {
}

// offset: 0x12F0 | func: 22 | export: 21
void dll_211_func_12F0(Object* self, s32 arg1, Object* arg2) {
    DLL211_Data* objData;

    objData = self->data;
    if (arg1 != 0) {
        if (!(objData->unk4C & 0x10)) {
            dll_211_func_1408(self, arg2);
        }
    } else {
        if (objData->unk18 >= 5 && objData->unk18 < 8) {
            if (objData->unk4C & 0x10) {
                ((s16*)objData->unk5E4)[0] = 1;
                return;
            }
            dll_211_func_82B8(objData);
        }
    }
}

// offset: 0x1398 | func: 23 | export: 22
void dll_211_func_1398(Object* self, Object* arg1) {
    DLL211_Data* objData;

    objData = self->data;
    if (!(objData->unk4C & 0x10)) {
        objData->unk28 = arg1;
        dll_211_func_9024(objData, &arg1->globalPosition);
        objData->unk1A = 0;
        objData->unk18 = 13;
    }
}

// offset: 0x1408 | func: 24
static void dll_211_func_1408(Object* self, Object* arg1) {
    DLL211_Data* objData;

    objData = self->data;
    objData->unk5E4[1] = gDLL_25->vtbl->func_1864(&arg1->srt.transl, -1, 3);
    if (objData->unk5E4[1] == NULL) {
        return;
    }

    ((s16*)objData->unk5E4)[0] = 0;
    if (((u8*)objData->unk5E4[1])[3] != 0) {
        if (((s32*)objData->unk5E4[1])[7] == -1) {
            objData->unk18 = 6;
        } else {
            objData->unk18 = 7;
        }
    } else {
        objData->unk18 = 5;
    }
    objData->unk28 = arg1;
    dll_211_func_9024(objData, (Vec3f* ) &objData->unk5E4[1]->srt.scale);
    objData->unk1A = 0;
}

// offset: 0x14E8 | func: 25 | export: 24
s32 dll_211_func_14E8(Object* self) {
    DLL211_Data* objData;

    objData = self->data;
    switch (objData->unk18) {
    case 5:
    case 6:
        if (objData->unk4C & 0x200) {
            return 1;
        }
        break;
    case 7:
        if (objData->unk4C & 0x10) {
            return 1;
        }
        break;
    }

    return 0;
}

// offset: 0x1550 | func: 26 | export: 25
s32 dll_211_func_1550(Object* self) {
    DLL211_Data* objData = self->data;

    if (objData->unk18 == 0xB) {
        return 1;
    }

    return 0;
}

// offset: 0x1578 | func: 27 | export: 23
void dll_211_func_1578(Object* self) {
    DLL211_Data* objData;

    objData = self->data;
    if (!(objData->unk4C & 0x10)) {
        dll_211_func_82B8(objData);
        objData->unk5F4_vec.x = 600.0f;
    } else if (objData->unk18 >= 5 && objData->unk18 < 8) {
        if (main_get_bits(BIT_Tricky_Unlocked_Sidekick_Commands) != 0) {
            ((s16*)objData->unk5E4)[0] = 1;
        }
    }
}

// offset: 0x1624 | func: 28
static s32 dll_211_func_1624(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    DLL211_Data* objData;
    s32 i;

    objData = self->data;
    if (!(objData->unk4C & 0x200)) {
        if (objData->unk4C & 0x800) {
            dll_211_func_940C(self, objData);
        }
        objData->unk4C |= 0x200;
        if (!(animObjData->unk7A & 3)) {
            objData->unk4C |= 0x4000;
        }
    }

    if ((objData->unk4C & 0x4000) && (objData->unk18 == 5 || objData->unk18 == 6)) {
        if (gDLL_1_cmdmenu->vtbl->was_this_item_used(0) != 0) {
            (((s16*)objData->unk5E4))[0] = 1;
        }
        if (((((s16*)objData->unk5E4))[0] != 0) && (self->curModAnimId == 0)) {
            gDLL_3_Animation->vtbl->end_obj_sequence((s32) animObjData->seqSlot);
        }
    }

    for (i = 0; i < animObjData->messageCount; i++) {
        switch (animObjData->messages[i]) {
            case 1:
                if (objData->unk4C & 0x800) {
                    dll_211_func_940C(self, objData);
                } else {
                    dll_211_func_9050(self, objData);
                }
                break;
            default:
                break;
        }
    }

    if (objData->unk4C & 1) {
        return 0;
    }

    return gDLL_3_Animation->vtbl->func29(self, animObjData, 2, 0, 0, 1, 1);
}

// offset: 0x183C | func: 29
static void dll_211_func_183C(Object* self, DLL211_Data* objData) {
    s32 temp_v0;

    temp_v0 = gDLL_25->vtbl->func_1158(&self->srt.transl, NULL);
    if (temp_v0 != 0) {
        main_set_bits(BIT_Tricky_Talk_Sequence, temp_v0);
        dll_211_func_82B8(objData);
    }
}

// offset: 0x18C4 | func: 30
static void dll_211_func_18C4(Object* self, DLL211_Data* objData) {
    u8 var_v0;
    u8 sp2E;
    s16 pad;
    s16 sp2A;
    s16 sp28;
    Object* temp_v0_2;

    switch (objData->unk1A) {
    case 0:
        objData->unk28 = objData->unk8;
        dll_211_func_9024(objData, &objData->unk28->globalPosition);
        objData->unk5F4_vec.x -= gUpdateRateF;
        if (objData->unk5F4_vec.x < 0.0f) {
            var_v0 = dll_211_func_53E4(self, 50.0f, objData);
            objData->unk5F4_vec.x = 0.0f;
        } else {
            var_v0 = dll_211_func_53E4(self, 25.0f, objData);
        }
        if (var_v0 != 1) {
            dll_211_func_83BC(self, objData);
            if ((objData->unk334 != 0) && (objData->unk338->id == 0x23)) {
                objData->unk5F4_vec.y = 300.0f;
                dll_211_func_95E0(self, objData, _data_0[rand_next(10, 12)]);
                objData->unk1A = 1;
                objData->unk4C |= 0x10;
                func_80023D30(self, 0x106, 0.0f, 0);
                break;
            }

            if (objData->unk4->blueFood == 0) {
                objData->unk600 -= gUpdateRateF;
                if (objData->unk600 < 0.0f) {
                    dll_211_func_95E0(self, objData, _data_0[rand_next(13, 17)]);
                    objData->unk600 = 600.0f;
                }
            }

            temp_v0_2 = (Object*) ((DLL_210_Player*)objData->unk8->dll)->vtbl->func66(objData->unk8, 16);
            if (((DLL_Unknown *)temp_v0_2->dll)->vtbl->func[7].withOneArgS32((s32)temp_v0_2) != 0) {
                sp2A = BIT_Dino_Bag_Blue_Mushrooms;
                sp28 = 6;
            } else {
                sp2A = BIT_Inventory_Blue_Mushrooms;
                sp28 = 3;
            }

            sp2E = main_get_bits(sp2A);
            if (sp2E && gDLL_1_cmdmenu->vtbl->get_page_category() == sp28) {
                if (self->unkAF & 1) {
                    if (gDLL_1_cmdmenu->vtbl->was_this_item_used(sp2A) != 0) {
                        main_set_bits(BIT_Inventory_Blue_Mushrooms, sp2E - 1);
                        objData->unk4->blueFood += 2;
                        dll_211_func_8B5C(self, 2);
                        objData->unk4C |= 1;
                    }
                } else {
                    self->unkAF &= ~8;
                }
            } else {
                var_v0 = main_get_bits(BIT_4E3);
                if (var_v0 != 0xFF) {
                    if (self->unkAF & 1) {
                        main_set_bits(BIT_4E3, 0xFF);
                        dll_211_func_8B5C(self, var_v0);
                    } else {
                        self->unkAF &= ~8;
                    }
                }
            }
        }
        break;
    case 1:
        objData->unk5F4_vec.y -= gUpdateRateF;
        objData->unk5F4_vec.z += gUpdateRateF;
        if (objData->unk5F4_vec.y < 0.0f) {
            objData->unk1A = 0;
            objData->unk4C &= ~0x10;
            dll_211_func_83BC(self, objData);
        } else {
            dll_211_func_885C(self);
            if (rand_next(0, 10) == 0) {
                dll_211_func_95E0(self, objData, _data_0[rand_next(10, 12)]);
            }
            if (objData->unk334 != 0 && objData->unk338->id == OBJ_sword) {
                objData->unk5F4_vec.y = 300.0f;
            }
            if (objData->unk5F4_vec.z > 3000.0f) {
                objData->unk1A = 2;
                func_80023D30(self, 0x1A, 0.0f, 0);
                objData->unk38 = 0.004f;
            }
        }
        break;
    case 2:
        if ((self->animProgress > 0.25f) && !(objData->unk4C & 0x800)) {
            dll_211_func_9050(self, objData);
        } else if (self->animProgress > 0.99f) {
            dll_211_func_940C(self, objData);
            objData->unk5F4_vec.y = 300.0f;
            objData->unk5F4_vec.z = 0.0f;
            dll_211_func_95E0(self, objData, _data_0[rand_next(10, 12)]);
            objData->unk1A = 1;
            objData->unk4C |= 0x10;
            func_80023D30(self, 0x106, 0.0f, 0);
        }
        break;
    }
}

// offset: 0x1F3C | func: 31
static void dll_211_func_1F3C(Object* self, DLL211_Data* objData) {
    f32 temp_fv0;
    CurveSetup* temp_v1_2;
    Object* sp34;
    f32 *temp;

    sp34 = objData->unk28;
    switch (objData->unk1A) {
    case 0:
        objData->unk5F4 = gDLL_25->vtbl->func_1864(&objData->unk28->srt.transl, -1, 2);
        objData->unk600 = 1.0f;
        if ((objData->unk5F4 != NULL) && (vec3_distance_xz_squared(&objData->unk28->globalPosition, &objData->unk5F4->pos) > 10000.0f)) {
            objData->unk5F4 = NULL;
            objData->unk600 = 2.0f;
        }
        temp_v1_2 = objData->unk5F4;
        if (temp_v1_2 != 0) {
            dll_211_func_9024(objData, &temp_v1_2->pos);
        }
        objData->unk1A = 1;
        /* fallthrough */
    case 1:
        if (dll_211_func_53E4(self, 5.0f, objData) == 0) {
            objData->unk4C |= 0x10;
            objData->unk1A = 2;
            ((f32*)objData->unk5E4)[0] = 0.0f;
            ((u32*)objData->unk5E4)[1] = gDLL_6_AMSFX->vtbl->play(self, SOUND_48_Dig_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
            func_80023D30(self, 14, 0.0f, 0);
            temp_v1_2 = objData->unk5F4;
            objData->unk38 = 0.033f;
            if (temp_v1_2 != 0) {
                dll_211_func_8F84(objData->unk28, &temp_v1_2->pos, objData->unk30);
            }
        }
        return;
    case 2:
        ((f32*)objData->unk5E4)[0] += gUpdateRateF;
        dll_211_func_87E4(self);
        if (((f32*)objData->unk5E4)[0] >= 60.0f) {
            if (objData->unk4->_unk2[0] < ((s32)objData->unk5F8)) {
                gDLL_6_AMSFX->vtbl->stop((u32) objData->unk5E4[1]);
                dll_211_func_95E0(self, objData, _data_0[1]);
                gDLL_22_Subtitles->vtbl->func_368(0xBD);
                dll_211_func_82B8(objData);
            } else if (objData->unk4->blueFood < ((s32)objData->unk5FC)) {
                gDLL_6_AMSFX->vtbl->stop((u32) objData->unk5E4[1]);
                dll_211_func_95E0(self, objData, _data_0[0]);
                gDLL_22_Subtitles->vtbl->func_368(0xBC);
                dll_211_func_82B8(objData);
            } else {
                objData->unk1A = 3;
                temp_v1_2 = objData->unk5F4;
                ((f32*)objData->unk5E4)[2] = self->srt.transl.x;
                objData->unk5F0_f32 = self->srt.transl.z;
                if (temp_v1_2 != 0) {
                    dll_211_func_8F84(objData->unk28, &temp_v1_2->pos, objData->unk30);
                }
            }
        }
        break;
    case 3:
        temp = objData->unk30;
        temp_fv0 = ((DLL_Unknown*)sp34->dll)->vtbl->func[7].withTwoArgsF32((s32)sp34, (s32)self);
        self->srt.transl.x = ((f32*)objData->unk5E4)[2] - (temp[0] * (temp_fv0 / objData->unk600));
        self->srt.transl.z = objData->unk5F0_f32 - (temp[1] * (temp_fv0 / objData->unk600));
        dll_211_func_87E4(self);
        if (((DLL_Unknown*)sp34->dll)->vtbl->func[8].withOneArgS32((s32)sp34) != 0) {
            objData->unk4->blueFood -= (s32)objData->unk5FC;
            objData->unk4->_unk2[0]++;
            gDLL_6_AMSFX->vtbl->stop((u32)objData->unk5E4[1]);
            dll_211_func_82B8(objData);
        }
        break;
    }
}

// offset: 0x23B8 | func: 32
static void dll_211_func_23B8(Object* self, DLL211_Data* objData) {
    Object* temp_a0;
    CurveSetup* sp38;
    CurveSetup* temp_t0;
    s32 sp30;
    CurveSetup* var_a1;
    f32 temp_fv0;
    s32 i;

    switch (objData->unk1A) {
    case 0:
        sp38 = gDLL_25->vtbl->func_1A2C(objData->unk2C, -1, 2);
        objData->unk5E4[2] = (Object*) gDLL_26_Curves->vtbl->func_39C(sp38->links[0]);
        objData->unk5E4[0] = (Object*) sp38;
        objData->unk5E4[1] = (Object*) gDLL_26_Curves->vtbl->func_39C(sp38->links[1]);
        if (((CurveSetup*)objData->unk5E4[1])->unk3 != 0) {
            objData->unk5E4[1] = (Object*)((s32)objData->unk5E4[2] ^ (s32)objData->unk5E4[1]);\
            objData->unk5E4[2] = (Object*)((s32)objData->unk5E4[1] ^ (s32)objData->unk5E4[2]);\
            objData->unk5E4[1] = (Object*)((s32)objData->unk5E4[2] ^ (s32)objData->unk5E4[1]);\
        }
        dll_211_func_9024(objData, &((CurveSetup*)objData->unk5E4[2])->pos);
        objData->unk1A = 1;
        /* fallthrough */
    case 1:
        dll_211_func_53E4(self, 5.0f, objData);
        if (gDLL_25->vtbl->func_1158(&self->srt.transl, NULL) == ((u8*)objData->unk5E4[2])[3]) {
            objData->unk19 = 1;
            objData->unk1A = 2;
            return;
        }
    default:
        return;
    case 2:
        if (dll_211_func_4F3C(self, 2.5f, objData, (Vec3f* ) &objData->unk5E4[0]->srt.scale) == 0) {
            objData->unk20 = vec3_distance_xz(&self->globalPosition, (Vec3f* ) &objData->unk5E4[0]->srt.scale) / gUpdateRateF;
            dll_211_func_8470(self, (Vec3f* ) &objData->unk5E4[0]->srt.scale);
            objData->unk4C |= 0x10;
            objData->unk1A = 3;
        } else if (gDLL_25->vtbl->func_1158(&self->srt.transl, NULL) == 0) {
            objData->unk4C |= 0x2010;
        }
        return;
    case 3:
        func_80023D30(self, 14, 0.0f, 0);
        temp_t0 = (CurveSetup *) objData->unk5E4[1];
        objData->unk38 = 0.033f;
        objData->unk30[0] = temp_t0->pos.x - objData->unk5E4[0]->srt.scale;
        objData->unk30[1] = temp_t0->pos.z - objData->unk5E4[0]->srt.transl.y;
        objData->unk5F0 = (Object* ) gDLL_6_AMSFX->vtbl->play(self, SOUND_48_Dig_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
        objData->unk1A = 4;
        /* fallthrough */
    case 4:
        temp_a0 = objData->unk28;
        temp_fv0 = ((DLL_Unknown*)temp_a0->dll)->vtbl->func[7].withTwoArgsF32((s32)temp_a0, (s32)self);
        self->srt.transl.x = objData->unk5E4[0]->srt.scale + (objData->unk30[0] * temp_fv0);
        self->srt.transl.z = objData->unk5E4[0]->srt.transl.y + (objData->unk30[1] * temp_fv0);
        dll_211_func_87E4(self);
        temp_a0 = objData->unk28;
        if (((DLL_Unknown*)temp_a0->dll)->vtbl->func[8].withOneArgS32((s32)temp_a0) != 0) {
            temp_t0 = (CurveSetup *) objData->unk5E4[1];
            for (i = 0; i < 4; i++) {
                if ((temp_t0->links[i] >= 0) && (((CurveSetup*)objData->unk5E4[0])->uID != temp_t0->links[i])) {
                    objData->unk5E4[0] = (Object* ) temp_t0;
                    objData->unk5E4[1] = (Object* ) gDLL_26_Curves->vtbl->func_39C(temp_t0->links[i]);
                    break;
                }
            }
            gDLL_6_AMSFX->vtbl->stop((u32)objData->unk5F0);
            objData->unk1A = 5;
        }
        return;
    case 5:
        if (dll_211_func_4F3C(self, 2.5f, objData, (Vec3f* ) &objData->unk5E4[1]->srt.scale) == 0) {
            temp_t0 = (CurveSetup *) objData->unk5E4[1];
            for (i = 0; i < 4; i++) {
                if ((temp_t0->links[i] >= 0) && (((CurveSetup*)objData->unk5E4[0])->uID != temp_t0->links[i])) {
                    objData->unk5E4[0] = (Object* ) temp_t0;
                    objData->unk5E4[1] = (Object* ) gDLL_26_Curves->vtbl->func_39C(temp_t0->links[i]);
                    break;
                }
            }
            objData->unk1A = 6;
        }
        return;
    case 6:
        if (dll_211_func_4F3C(self, 2.5f, objData, (Vec3f* ) &objData->unk5E4[1]->srt.scale) == 0) {
            dll_211_func_83BC(self, objData);
            objData->unk4C &= ~0x2010;
            objData->unk1A = 7;
        }
        return;
    case 7:
        sp30 = gDLL_25->vtbl->func_1158(&self->srt.transl, NULL);
        if (gDLL_25->vtbl->func_1158(&objData->unk8->srt.transl, NULL) == sp30) {
            dll_211_func_82B8(objData);
        }
        return;
    }
}

// offset: 0x2928 | func: 33
static void dll_211_func_2928(Object* self, DLL211_Data* objData) {
}

// offset: 0x2938 | func: 34
static void dll_211_func_2938(Object* self, DLL211_Data* objData) {
    CurveSetup* temp_v0;

    switch (objData->unk1A) {
    case 0:
        temp_v0 = gDLL_26_Curves->vtbl->func_39C(((s32*)objData->unk5E4[1])[7 /* unk1C */]);
        objData->unk5E4[2] = temp_v0;
        dll_211_func_9024(objData, &temp_v0->pos);
        objData->unk1A = 1;
        /* fallthrough */
    case 1:
        if (gDLL_25->vtbl->func_1158(&self->srt.transl, NULL) == ((u8*)objData->unk5E4[2])[3]) {
            objData->unk1A = 2;
        } else {
            dll_211_func_53E4(self, 5.0f, objData);
            return;
        }
        /* fallthrough */
    case 2:
        if (dll_211_func_4F3C(self, 2.5f, objData, (Vec3f* ) &objData->unk5E4[1]->srt.scale) == 0) {
            objData->unk4C |= 0x10;
            objData->unk1A = 3;
            ((f32*)&objData->unk5F0)[0] = 0.0f;
        } else {
            if (gDLL_25->vtbl->func_1158(&self->srt.transl, NULL) == 0) {
                objData->unk4C |= 0x10;
            }
            return;
        }
        /* fallthrough */
    case 3:
        objData->unk4C |= 0x4010;
        objData->unk1A = 4;
        return;
    default:
        return;
    case 4:
        dll_211_func_4F3C(self, 2.5f, objData, (Vec3f* ) &objData->unk5E4[2]->srt.scale);
        if (gDLL_25->vtbl->func_1158(&self->srt.transl, NULL) != 0) {
            dll_211_func_82B8(objData);
        }
        return;
    }
}

// offset: 0x2B50 | func: 35
static void dll_211_func_2B50(Object* self, DLL211_Data* objData) {
    switch (objData->unk1A) {
    case 0:
        dll_211_func_9024(objData, (Vec3f* ) &objData->unk5E4[1]->srt.scale);
        objData->unk1A = 1;
        /* fallthrough */
    case 1:
        if (dll_211_func_53E4(self, 5.0f, objData) == 0) {
            objData->unk1A = 3;
            ((f32*)&objData->unk5F0)[0] = NULL;
        } else {
            return;
        }
        /* fallthrough */
    case 3:
        objData->unk4C |= 0x4010;
        gDLL_3_Animation->vtbl->start_obj_sequence(4, self, -1);
        objData->unk1A = 4;
        return;
    case 4:
        dll_211_func_82B8(objData);
        break;
    }
}

// offset: 0x2C64 | func: 36
static void dll_211_func_2C64(Object* self, DLL211_Data* objData) {
    CurveSetup* sp44;
    CurveSetup* sp40;
    CurveSetup* var_a3;
    CurveSetup* sp38;
    CurveSetup* sp34;
    s32 pad;

    if (objData->unk4C & 0x10) {
        if (objData->unk3C8.unk0.unk80 == 0) {
            if (objData->unk3C8.unk0.unk10 != 0) {
                pad = gDLL_26_Curves->vtbl->func_438(objData->unk3C8.unkA4, 0);
                gDLL_25->vtbl->func_21F4(&objData->unk3C8, gDLL_26_Curves->vtbl->func_39C(pad));
            }
        } else if (objData->unk3C8.unk0.unk10 == 0) {
            pad = gDLL_26_Curves->vtbl->func_590(objData->unk3C8.unkA4, 0);
            gDLL_25->vtbl->func_21F4(&objData->unk3C8, gDLL_26_Curves->vtbl->func_39C(pad));
        }
        dll_211_func_8ED0(objData->unk20, 0.8f, &objData->unk20);
        dll_211_func_8974(self, &objData->unk3C8, objData->unk20);
        dll_211_func_8470(self, &objData->unk3C8.unk0.unk68);
        if ((((s16*)objData->unk5E4)[0] != 0) && (gDLL_25->vtbl->func_1158(&self->srt.transl, NULL) != 0)) {
            dll_211_func_82B8(objData);
        }
        ((f32*)&objData->unk5F0)[0] -= gUpdateRateF;
        if (((f32*)&objData->unk5F0)[0] < 0.0f) {
            ((f32*)&objData->unk5F0)[0] = rand_next(200, 600);
            dll_211_func_95E0(self, objData, _data_0[rand_next(3, 5)]);
        }
    } else {
        dll_211_func_53E4(self, 5.0f, objData);
        if (gDLL_25->vtbl->func_1158(&self->srt.transl, NULL) == gDLL_25->vtbl->func_1158((Vec3f* ) &objData->unk5E4[1]->srt.scale, NULL)) {
            sp44 = (CurveSetup* ) objData->unk5E4[1];
            objData->unk4C |= 0x10;
            sp38 = gDLL_26_Curves->vtbl->func_39C(gDLL_26_Curves->vtbl->func_438(sp44, 0));
            sp34 = gDLL_26_Curves->vtbl->func_39C(gDLL_26_Curves->vtbl->func_590(sp44, 0));
            if (vec3_distance_xz_squared(&self->globalPosition, &sp38->pos) > vec3_distance_xz_squared(&self->globalPosition, &sp34->pos)) {
                sp40 = sp38;
                var_a3 = gDLL_26_Curves->vtbl->func_39C(gDLL_26_Curves->vtbl->func_438(sp38, 0));
                objData->unk3C8.unk0.unk80 = 0;
            } else {
                sp40 = sp34;
                var_a3 = gDLL_26_Curves->vtbl->func_39C(gDLL_26_Curves->vtbl->func_590(sp34, 0));
                objData->unk3C8.unk0.unk80 = 1;
            }
            gDLL_25->vtbl->func_1D30(&objData->unk3C8, sp44, sp40, var_a3);
            if (objData->unk3C8.unk0.unk80 != 0) {
                curves_func_800053B0(&objData->unk3C8.unk0, -10.0f);
            } else {
                curves_func_800053B0(&objData->unk3C8.unk0, -10.0f);
            }
            ((f32*)&objData->unk5F0)[0] = 0.0f;
        }
    }
}

// offset: 0x3188 | func: 37
static void dll_211_func_3188(Object* self, DLL211_Data* objData) {
    Object* temp_v0_3;
    ObjSetup* sp38;
    Object* var_v1;

    if (objData->unk4C & 0x10) {
        switch (objData->unk1A) {
        case 3:
            self->shadow->flags |= 0x1000;
            ((f32*)objData->unk5E4)[0] += 0.1f * gUpdateRateF;
            if (((f32*)objData->unk5E4)[0] > 20.0f) {
                ((f32*)objData->unk5E4)[0] = 20.0f;
                objData->unk1A = 4;
                ((f32*)&objData->unk5F0)[0] = 30.0f;
                gDLL_6_AMSFX->vtbl->stop(objData->unk5F4_amsFxID);
                temp_v0_3 = objData->unk5E4[1];
                self->srt.transl.x = temp_v0_3->srt.scale;
                self->srt.transl.y = temp_v0_3->srt.transl.x;
                self->srt.transl.z = temp_v0_3->srt.transl.y;
                objData->unkCC.mode = 0;
            }
            break;
        case 2:
            ((f32*)objData->unk5E4)[0] -= 0.1f * gUpdateRateF;
            if (((f32*)objData->unk5E4)[0] < -5.0f) {
                dll_211_func_82B8(objData);
                gDLL_6_AMSFX->vtbl->stop(objData->unk5F4_amsFxID);
                self->shadow->flags &= ~0x1000;
            }
            break;
        case 4:
            ((f32*)&objData->unk5F0)[0] -= gUpdateRateF;
            if (((f32*)&objData->unk5F0)[0] <= 0.0f) {
                objData->unk1A = 2;
                objData->unk5F4_amsFxID = gDLL_6_AMSFX->vtbl->play(self, SOUND_4BB, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            break;
        }
    } else if (dll_211_func_53E4(self, 5.0f, objData) == 0) {
        // FAKE
        if (1) {
            var_v1 = objData->unk28;
        }
        sp38 = var_v1->setup;
        ((f32*)objData->unk5E4)[0] = -5.0f;
        ((f32*)objData->unk5E4)[2] = 0.0001f;
        dll_211_func_83BC(self, objData);
        if (objData->unk1A == 0) {
            if (vec3_distance_xz_squared(&self->globalPosition, &objData->unk28->globalPosition) > 400.0f) {
                sp38 = map_find_obj_setup(((s32*)sp38)[7 /* unk1C */], NULL, NULL, NULL, NULL);
                dll_211_func_9024(objData, &sp38->x);
                objData->unk1A = 1;
            } else {
                objData->unk5E4[1] = map_find_obj_setup(((s32*)sp38)[7 /* unk1C */], NULL, NULL, NULL, NULL);
                objData->unk4C |= 0x10;
                objData->unk1A = 3;
                objData->unk5F4_amsFxID = gDLL_6_AMSFX->vtbl->play(self, SOUND_4B9, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
        } else {
            objData->unk5E4[1] = (Object* ) sp38;
            objData->unk4C |= 0x10;
            objData->unk1A = 3;
            objData->unk5F4_amsFxID = gDLL_6_AMSFX->vtbl->play(self, SOUND_4B9, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    } else {
        return;
    }
    objData->unk38 = 0.005f;
}

// offset: 0x3560 | func: 38
static void dll_211_func_3560(Object* self, DLL211_Data* objData) {
}

// offset: 0x3570 | func: 39
static s32 dll_211_func_3570(Object* self, DLL211_Data* objData) {
    if (self->animProgress > 0.25f) {
        if (!(objData->unk4C & 0x800)) {
            dll_211_func_9050(self, objData);
        } else if ((objData->unk5FC == 0 || objData->unk5FC(objData->unk28, 1, objData, self) != 0) && self->animProgress > 0.8f) {
            dll_211_func_940C(self, objData);
            return 0;
        }
    }
    return 1;
}

// offset: 0x3664 | func: 40
static void dll_211_func_3664(Object* self, DLL211_Data* objData) {
    switch (objData->unk1A) {
    case 0:
        objData->unk5F4 = gDLL_25->vtbl->func_1A2C(&objData->unk8->srt.transl, -1, 4);
        if (objData->unk5F4->unk3 != 0) {
            dll_211_func_9024(objData, &objData->unk5F4->pos);
            objData->unk1A = 1;
        } else {
            objData->unk5F8 = gDLL_26_Curves->vtbl->func_39C(objData->unk5F4->links[0]);
            dll_211_func_9024(objData, &objData->unk5F8->pos);
            objData->unk1A = 2;
        }
        dll_211_func_53E4(self, 5.0f, objData);
        return;
    default:
        return;
    case 2:
        dll_211_func_53E4(self, 5.0f, objData);
        if (gDLL_25->vtbl->func_1158(&self->srt.transl, NULL) == objData->unk5F8->unk3) {
            objData->unk19 = 1;
            objData->unk1A = 3;
        }
        return;
    case 3:
        dll_211_func_4F3C(self, 2.5f, objData, &objData->unk5F4->pos);
        if (gDLL_25->vtbl->func_1158(&self->srt.transl, NULL) == 0) {
            objData->unk4C |= 0x10;
            objData->unk1A = 4;
        }
        return;
    case 4:
        if (dll_211_func_4F3C(self, 2.5f, objData, &objData->unk5F4->pos) == 0) {
            func_80023D30(self, 0x1A, 0.0f, 0);
            objData->unk38 = 0.004f;
            objData->unk1A = 6;
        } else {
            return;
        }
        /* fallthrough */
    case 6:
        dll_211_func_871C(self, (s8* ) objData->unk5F4);
        if (dll_211_func_3570(self, objData) == 0) {
            objData->unk1A = 7;
            objData->unk600 = 60.0f;
        }
        return;
    case 1:
        if (dll_211_func_53E4(self, 5.0f, objData) == 0) {
            func_80023D30(self, 0x1A, 0.0f, 0);
            objData->unk38 = 0.004f;
            objData->unk4C |= 0x10;
            objData->unk1A = 5;
            objData->unk30[0] = objData->unk28->srt.transl.x - self->srt.transl.x;
            objData->unk30[1] = objData->unk28->srt.transl.z - self->srt.transl.z;
        }
        return;
    case 5:
        dll_211_func_87E4(self);
        if (dll_211_func_3570(self, objData) == 0) {
            dll_211_func_82B8(objData);
        }
        return;
    case 7:
        objData->unk600 -= gUpdateRateF;
        if (objData->unk600 <= 0.0f) {
            dll_211_func_4F3C(self, 2.5f, objData, &objData->unk5F8->pos);
            if (gDLL_25->vtbl->func_1158(&self->srt.transl, NULL) != 0) {
                dll_211_func_82B8(objData);
            }
        }
        return;
    }
}

// offset: 0x3A78 | func: 41
static s32 dll_211_func_3A78(DLL211_Data* objData) {
    s32 count;
    Object* var_s2;
    Object** temp_s5;
    f32 sp58;
    f32 temp_fs0;
    s16 i;

    var_s2 = NULL;
    temp_s5 = obj_get_all_of_type(OBJTYPE_Baddie, &count);
    for (i = 0; i < count; i++) {
        temp_fs0 = vec3_distance_xz_squared(&temp_s5[i]->globalPosition, &objData->unk5F4_vec);
        if (var_s2 == NULL) {
            if (gDLL_25->vtbl->func_1158(&temp_s5[i]->globalPosition, NULL) == objData->unk608) {
                sp58 = temp_fs0;
                var_s2 = temp_s5[i];
            }
        } else if ((temp_fs0 < sp58) && (gDLL_25->vtbl->func_1158(&temp_s5[i]->globalPosition, NULL) == objData->unk608)) {
            sp58 = temp_fs0;
            var_s2 = temp_s5[i];
        }
    }
    if (var_s2 != NULL) {
        objData->unk604 = var_s2;
        dll_211_func_9024(objData, &var_s2->globalPosition);
        objData->unk1A = 4;
        return 1;
    }

    return 0;
}

// offset: 0x3C1C | func: 42
static s32 dll_211_func_3C1C(Object* self) {
    s32 count;
    Object** objects;
    s16 i;

    objects = obj_get_all_of_type(OBJTYPE_Baddie, &count);
    for (i = 0; i < count; i++) {
        if (self == objects[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

// offset: 0x3CAC | func: 43
static void dll_211_func_3CAC(Object* self, DLL211_Data* objData) {
    f32 temp_ft0;

    switch (objData->unk1A) {
    case 0:
        objData->unk608 = gDLL_25->vtbl->func_1158(objData->unk2C, NULL);
        objData->unk5F4_vec.x = objData->unk28->globalPosition.x - (fsin16_precise(objData->unk28->srt.yaw) * 15.0f);
        objData->unk5F4_vec.y = objData->unk28->globalPosition.y;
        temp_ft0 = fcos16_precise(objData->unk28->srt.yaw) * 15.0f;
        objData->unk5F4_vec.z = objData->unk28->globalPosition.z - temp_ft0;
        objData->unk60C = 0;
        objData->unk1A = 1;
        return;
    case 1:
        dll_211_func_53E4(self, 5.0f, objData);
        if (gDLL_25->vtbl->func_1158(&self->srt.transl, NULL) == objData->unk608) {
            objData->unk1A = 2;
        }
        return;
    default:
        return;
    case 2:
        if (dll_211_func_53E4(self, 5.0f, objData) == 0) {
            dll_211_func_9024(objData, &objData->unk5F4_vec);
            objData->unk1A = 3;
        } else {
            dll_211_func_3A78(objData);
            return;
        }
        /* fallthrough */
    case 3:
        if (dll_211_func_53E4(self, 5.0f, objData) == 0) {
            dll_211_func_83BC(self, objData);
        }

        dll_211_func_3A78(objData);
        return;
    case 4:
        if (dll_211_func_53E4(self, 15.0f, objData) == 0) {
            objData->unk4C |= 0x10;
            if ((objData->unk4->redFood != 0) && (objData->unk60C != 0)) {
                dll_211_func_9050(self, objData);
                objData->unk4->redFood--;
                func_80023D30(self, 0x205, 0.0f, 0);
                objData->unk38 = 0.005f;
                objData->unk1A = 5;
            } else {
                func_80023D30(self, 0x105, 0.0f, 0);
                objData->unk38 = 0.01f;
                objData->unk1A = 6;
            }
        } else {
            if (gDLL_25->vtbl->func_1158(objData->unk2C, NULL) != objData->unk608) {
                dll_211_func_9024(objData, &objData->unk28->globalPosition);
                objData->unk1A = 2;
            }
            return;
        }
        /* fallthrough */
    case 5:
        if (self->animProgress >= 0.95f) {
            dll_211_func_940C(self, objData);
            objData->unk4C &= ~0x10;
            if (dll_211_func_3A78(objData) == 0) {
                dll_211_func_9024(objData, &objData->unk28->globalPosition);
                objData->unk1A = 2;
            }
        } else if (dll_211_func_3C1C(objData->unk604) != 0) {
            dll_211_func_885C(self);
        }
        return;
    case 6:
        if (self->animProgress >= 0.95f) {
            func_80023D30(self, 0x106, 0.0f, 0);
            objData->unk38 = 0.005f;
            objData->unk600 = 0.0f;
            dll_211_func_95E0(self, objData, _data_0[rand_next(10, 12)]);
            objData->unk1A = 7;
        } else if (dll_211_func_3C1C(objData->unk604) != 0) {
            dll_211_func_885C(self);
        }
        return;
    case 7:
        if (rand_next(0, 10) == 0) {
            dll_211_func_95E0(self, objData, _data_0[rand_next(10, 12)]);
        }
        objData->unk600 += gUpdateRateF;
        if (((objData->unk600 >= 150.0f) && (vec3_distance_xz_squared(objData->unk2C, &self->srt.transl) >= 2500.0f)) || (dll_211_func_3C1C(objData->unk604) == 0)) {
            func_80023D30(self, 0x105, 1.0f, 0);
            objData->unk38 = -0.01f;
            objData->unk1A = 8;
        } else {
            dll_211_func_885C(self);
        }
        return;
    case 8:
        if (self->animProgress <= 0.05f) {
            objData->unk4C &= ~0x10;
            if (dll_211_func_3A78(objData) == 0) {
                dll_211_func_9024(objData, &objData->unk28->globalPosition);
                objData->unk1A = 2;
            }
        }
        return;
    }
}

// offset: 0x4340 | func: 44
static void dll_211_func_4340(Object* self, DLL211_Data* objData) {
    if (dll_211_func_53E4(self, 15.0f, objData) == 0) {
        dll_211_func_83BC(self, objData);
    }
}

// offset: 0x43AC | func: 45
static void dll_211_func_43AC(Object* self, DLL211_Data* objData) {
    if (dll_211_func_53E4(self, 5.0f, objData) == 0) {
        dll_211_func_83BC(self, objData);
    }
}

// offset: 0x4418 | func: 46
static void dll_211_func_4418(Object* self, DLL211_Data* objData) {
    s32 pad;
    f32 sp30;

    switch (objData->unk1A) {
    case 0:
        objData->unk5E4[0] = objData->unk28;
        objData->unk1A = 1;
        ((f32*)objData->unk5E4)[1] = 180.0f;
        /* fallthrough */
    case 1:
        if (((DLL_Unknown*)objData->unk5E4[0]->dll)->vtbl->func[9].withOneArgS32((s32)objData->unk5E4[0]) != 0) {
            if (dll_211_func_53E4(self, 13.0f, objData) == 0) {
                func_80023D30(self, 17, 0.0f, 0);
                objData->unk38 = 0.03f;
                objData->unk4C |= 0x10;
                objData->unk1A = 3;
                ((DLL_Unknown*)objData->unk5E4[0]->dll)->vtbl->func[8].withTwoArgs(objData->unk5E4[0], self);
            }
            return;
        }
        if (dll_211_func_53E4(self, 25.0f, objData) == 0) {
            if (((f32*)objData->unk5E4)[1] > 0.0f) {
                dll_211_func_83BC(self, objData);
                objData->unk38 = 0.005f;
                ((f32*)objData->unk5E4)[1] -= gUpdateRateF;
                if (((f32*)objData->unk5E4)[1] <= 0.0f) {
                    ((f32*)objData->unk5E4)[2] = 60.0f;
                }
            } else {
                dll_211_func_8308(self, 0x10);
                objData->unk38 = 0.02f;
                ((f32*)objData->unk5E4)[2] -= gUpdateRateF;
                if (((f32*)objData->unk5E4)[2] <= 0.0f) {
                    ((f32*)objData->unk5E4)[1] = 180.0f;
                }
            }
        }
        return;
    default:
        return;
    case 6:
        if (self->animProgress >= 0.65f) {
            objData->unk5E4[0]->srt.transl.y += 5.0f;
            sp30 = fsin16_precise(self->srt.yaw);
            ((DLL_Unknown*)objData->unk5E4[0]->dll)->vtbl->func[7].withFiveArgsCustom4(objData->unk5E4[0], self, -sp30, 1.0f, -fcos16_precise(self->srt.yaw));
            objData->unk1A = 2;
        }
        return;
    case 2:
        if (self->animProgress >= 0.95f) {
            dll_211_func_82B8(objData);
        }
        return;
    case 3:
        if (self->animProgress >= 0.5f) {
            objData->unk1A = 4;
        }
        return;
    case 4:
        if (self->animProgress >= 0.95f) {
            objData->unk28 = objData->unk8;
            dll_211_func_9024(objData, &objData->unk8->globalPosition);
            objData->unk1A = 5;
            /* fallthrough */
        } else {
            return;
        }
    case 5:
        if (dll_211_func_53E4(self, 50.0f, objData) == 0) {
            func_80023D30(self, 0x13, 0.0f, 0);
            objData->unk38 = 0.03f;
            objData->unk1A = 6;
        }
        return;
    }
}

// offset: 0x47E8 | func: 47
static void dll_211_func_47E8(Object* self, DLL211_Data* objData) {
    s32 sp24;
    s32 var_v0;
    s32 var_v1_2;

    sp24 = atan2f_to_s(objData->unk28->srt.transl.x - self->srt.transl.x, objData->unk28->srt.transl.z - self->srt.transl.z);
    if (objData->unk1A == 0) {
        ((s32*)objData->unk5E4)[0] = rand_next(0, 1);
        if (((s32*)objData->unk5E4)[0] == 0) {
            ((s32*)objData->unk5E4)[0] = -1;
        }
        ((s32*)objData->unk5E4)[1] = sp24;
        objData->unk1A = 1;
    }
    var_v0 = sp24 - (((s32*)objData->unk5E4)[1] & 0xFFFF);
    CIRCLE_WRAP(var_v0);
    if (var_v0 >= 0) {
        var_v1_2 = var_v0;
    } else {
        var_v1_2 = -var_v0;
    }
    if (var_v1_2 < 0x2000) {
        ((s32*)objData->unk5E4)[1] += (((s32*)objData->unk5E4)[0] << 0xB);
    }
    ((f32*)objData->unk5E4)[2] = objData->unk28->srt.transl.x - (fsin16(((s32*)objData->unk5E4)[1]) * 50.0f);
    ((f32*)objData->unk5E4)[3] = objData->unk28->srt.transl.y;
    objData->unk5F4_vec.x = objData->unk28->srt.transl.z - (fcos16(((s32*)objData->unk5E4)[1]) * 50.0f);
    dll_211_func_53E4(self, 5.0f, objData);
}

// offset: 0x4974 | func: 48
static void dll_211_func_4974(Object* self, DLL211_Data* objData) {
    objData->unk28 = dll_211_func_94BC(objData->unk8, 150.0f);
    if (objData->unk28 != NULL) {
        dll_211_func_9024(objData, &objData->unk28->srt.transl);
    } else if (!(objData->unk4C & 0x10)) {
        dll_211_func_82B8(objData);
    }

    switch (objData->unk1A) {
    case 0:
        if (dll_211_func_53E4(self, 50.0f, objData) == 0) {
            objData->unk1A = 1;
            objData->unk5F8 = 0;
            objData->unk5F4_vec.x = 0.0f;
            dll_211_func_83BC(self, objData);
        }
        return;
    case 1:
        dll_211_func_885C(self);
        if (vec3_distance_xz_squared(&self->srt.transl, objData->unk2C) > 5625.0f) {
            objData->unk1A = 0;
            return;
        }
        if (objData->unk4->redFood != 0 && objData->unk5F8 != 0) {
            objData->unk1A = 2;
            return;
        }
        objData->unk5F4_vec.x -= gUpdateRateF;
        if (objData->unk5F4_vec.x < 0.0f) {
            objData->unk5F4_vec.x = rand_next(200, 600) * 0.5f;
            dll_211_func_95E0(self, objData, _data_0[6]);
        }
        break;
    case 2:
        if (dll_211_func_53E4(self, 25.0f, objData) == 0) {
            dll_211_func_9050(self, objData);
            objData->unk4->redFood--;
            func_80023D30(self, 0x205, 0.0f, 0);
            objData->unk38 = 0.005f;
            objData->unk4C |= 0x10;
            objData->unk1A = 3;
            objData->unk5F8 = 0;
            return;
        }
        break;
    case 3:
        if (self->animProgress >= 0.95f) {
            dll_211_func_940C(self, objData);
            objData->unk4C &= ~0x10;
            objData->unk1A = 0;
            return;
        }
        if (objData->unk28 != NULL) {
            dll_211_func_885C(self);
        }
        break;
    }
}

// offset: 0x4C94 | func: 49
static void dll_211_func_4C94(Object* self, DLL211_Data* objData) {
    switch (objData->unk1A) {
    case 0:
        if (dll_211_func_53E4(self, 50.0f, objData) == 0) {
            dll_211_func_95E0(self, objData, _data_0[rand_next(10, 12)]);
            objData->unk1A = 1;
            func_80023D30(self, 0x106, 0.0f, 0);
            objData->unk5F8 = 0;
            return;
        }
        return;
    case 1:
        if ((objData->unk4->redFood != 0) && (objData->unk5F8 != 0)) {
            objData->unk1A = 2;
            return;
        }
        dll_211_func_885C(self);
        if (rand_next(0, 10) == 0) {
            dll_211_func_95E0(self, objData, _data_0[rand_next(10, 12)]);
        }
        break;
    case 2:
        if (dll_211_func_53E4(self, 25.0f, objData) == 0) {
            dll_211_func_9050(self, objData);
            objData->unk4->redFood--;
            func_80023D30(self, 0x205, 0.0f, 0);
            objData->unk38 = 0.005f;
            objData->unk4C |= 0x10;
            objData->unk1A = 3;
            objData->unk5F8 = 0;
            return;
        }
        break;
    case 3:
        if (self->animProgress >= 0.95f) {
            dll_211_func_940C(self, objData);
            dll_211_func_82B8(objData);
        } else {
            dll_211_func_885C(self);
        }
        break;
    }
}

// offset: 0x4F3C | func: 50
static s32 dll_211_func_4F3C(Object* self, f32 arg1, DLL211_Data* arg2, Vec3f* arg3) {
    f32 temp2;
    f32 temp_fa0;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 temp;

    temp_fv1 = arg3->x - self->srt.transl.x;
    temp_fa0 = arg3->z - self->srt.transl.z;
    temp = arg2->unk20 * 10.0f;
    temp_fv0 = _data_A4[temp] + arg1;
    temp_fv0 *= temp_fv0;
    temp2 = SQ(temp_fv1) + SQ(temp_fa0);
    if (temp_fv0 < temp2) {
        dll_211_func_8ED0(arg2->unk20, 1.5f, &arg2->unk20);
    } else {
        arg2->unk20 += -0.03f * gUpdateRateF;
        if (arg2->unk20 < 0.0f) {
            arg2->unk20 = 0.0f;
            return 0;
        }
    }

    dll_211_func_8470(self, arg3);
    return 1;
}

// offset: 0x507C | func: 51
static void dll_211_func_507C(Object* self, DLL211_Data* objData) {
    dll_211_func_8F84(self, &objData->unk3C8.unkA0->pos, objData->unk30);
    objData->unk20 = 1.5f;
    func_80023D30(self, 0x15, 0.0f, 0);
    objData->unk19 = 9;
    dll_211_func_95E0(self, objData, _data_0[rand_next(7, 9)]);
}


// offset: 0x514C | func: 52
static void dll_211_func_514C(Object* self, DLL211_Data* objData) {
    dll_211_func_8F84(self, &objData->unk3C8.unkA0->pos, objData->unk30);
    objData->unk4C |= 0xC0;
    objData->unk44 = (objData->unk3C8.unkA0->pos.y - self->srt.transl.y) / 32.865f;
    objData->unk40 = 1.0f;
    if (objData->unk20 == 1.5f) {
        func_80023D30(self, 0x17, 0.0f, 0);
        objData->unk38 = 0.0135f;
    } else {
        func_80023D30(self, 0x18, 0.0f, 0);
        objData->unk38 = 0.00975f;
    }
    objData->unk19 = 12;
    dll_211_func_8A94(self, &objData->unk3C8);
    dll_211_func_95E0(self, objData, _data_0[rand_next(7, 9)]);
}

// offset: 0x52B8 | func: 53
static void dll_211_func_52B8(Object* self, DLL211_Data* objData) {
    dll_211_func_8F84(self, &objData->unk3C8.unkA0->pos, objData->unk30);
    objData->unk4C |= 0xC0;
    objData->unk44 = (self->srt.transl.y - objData->unk3C8.unkA0->pos.y) / 33.114f;
    objData->unk40 = 1.0f;
    func_80023D30(self, 0x19, 0.0f, 0);
    objData->unk38 = 0.0125f;
    objData->unk19 = 14;
    dll_211_func_8A94(self, &objData->unk3C8);
    dll_211_func_95E0(self, objData, _data_0[rand_next(7, 9)]);
}

// offset: 0x53E4 | func: 54
typedef struct {
    u8 pad0;
    u8 unk1;
    u16 unk2[4];
    u16 padA;
    f32 unkC;
    f32 unk10;
} UnkSp44;

static s32 dll_211_func_53E4(Object* self, f32 arg1, DLL211_Data* objData) {
    s32 pad;
    s32 spC0;
    s32 spBC;
    u32 spB8;
    Vec3f* temp_s1;
    f32 temp_fv0;
    f32 var_fv1;
    u8 spA8[4];
    s16 var_a3;
    s16 var_s1;
    CurveSetup* spA0;
    s32 var_v1;
    s32 var_a0;
    CurveSetup* sp88[4];
    f32 sp84; // fv0?
    CurveSetup* setup;
    Vec3f sp74;
    f32 sp68[2];
    s32 pad2;
    s16 sp60[3];
    CurveSetup* setup2;
    u8 sp5B;
    u8 temp_s1_4;
    u8 temp_v1_3;
    u8 temp_v1_4;
    UnkSp44 sp44;

    temp_s1 = objData->unk2C;
    spC0 = gDLL_25->vtbl->func_1158(&self->srt.transl, NULL);
    if (spC0 != 0 && spC0 != objData->unkA4) {
        objData->unkA4 = spC0;
        objData->unk4C &= ~0x400;
        for (var_a0 = 0; var_a0 < 4; var_a0++) { objData->unk6C[var_a0] = 0; }
    }
    spBC = gDLL_25->vtbl->func_1158(temp_s1, &sp44);
    spB8 = main_get_bits(BIT_Tricky_Talk_Sequence);
    if (spBC != 0 && spBC != spB8) {
        spB8 = spBC;
        main_set_bits(BIT_Tricky_Talk_Sequence, spBC);
    }
    sp84 = objData->unk20;
    objData->unk20 += -0.03f * gUpdateRateF;
    if (objData->unk20 < 0.0f) {
        objData->unk20 = 0.0f;
    }
    temp_fv0 = vec3_distance_xz_squared(temp_s1, &self->srt.transl);
    var_a0 = objData->unk20 * 10.0f;
    if (var_a0 >= 0x12) {
        var_fv1 = 100.0f;
    } else {
        var_fv1 = _data_A4[var_a0] + arg1;
        var_fv1 *= var_fv1;
    }
    if (var_fv1 < temp_fv0) {
        dll_211_func_8ED0(sp84, 1.5f, &objData->unk20);
    } else if (temp_fv0 > 400.0f) {
        temp_fv0 = dll_211_func_81D8(self);
        if (objData->unk20 < temp_fv0 && temp_fv0 < sp84) {
            objData->unk20 = temp_fv0;
        }
    }
    if (spBC == objData->unkA4) {
        objData->unk4C |= 0x400;
        for (var_a0 = 0, temp_v1_3 = 1; var_a0 < 4; var_a0++, temp_v1_3 <<= 1) {
            if (sp44.unk1 & temp_v1_3) {
                objData->unk6C[var_a0] = sp44.unk2[var_a0];
                objData->unk74[var_a0].x = temp_s1->x;
                objData->unk74[var_a0].y = temp_s1->y;
                objData->unk74[var_a0].z = temp_s1->z;
            }
        }
    }
    if (spBC != 0 && spBC == objData->unkA4) {
        objData->unkA6 = 0;
    } else {
        var_a3 = objData->unkA4 * spBC;
        if (var_a3 != 0) {
            for (var_a0 = 0; var_a0 < 4; var_a0++) {
                if ((var_a3 == sp44.unk2[var_a0]) && (sp44.unk1 & (1 << var_a0))) {
                    objData->unkA6 = var_a3;
                    objData->unkA8.x = temp_s1->x;
                    objData->unkA8.y = temp_s1->y;
                    objData->unkA8.z = temp_s1->z;
                }
            }
        }
    }
    gDLL_25->vtbl->func_12FC(temp_s1);
    gDLL_25->vtbl->func_1474(temp_s1, objData->unkA4);
    if (objData->unk4C & 0x400) {
        // @the fakest of fakes (this is an empty loop in default.dol)
        for (var_a0 = 0; var_a0 < 4; var_a0++) {
            // FAKE
            do { } while (0);
            if (sp44.unk1 & (1 << var_a0)) {
            }
        }
    }
    var_a3 = gDLL_25->vtbl->func_1474(temp_s1, objData->unkA4);
    if (objData->unk19 != 0xF && ((spBC != 0 && spC0 == spBC) || (var_a3 != 0 && gDLL_25->vtbl->func_15B0(&self->srt.transl, objData->unkA4, var_a3) != 0))) {
        objData->unk19 = 1;
    } else if (objData->unk19 < 5) {
        var_a3 = gDLL_25->vtbl->func_1474(temp_s1, objData->unkA4);\
        if (var_a3 != 0) {
            if (spBC == 0) {
                for (var_a0 = 0; var_a0 < 4; var_a0++) {
                    if (var_a3 == objData->unk6C[var_a0]) {
                        sp5B = var_a0;
                        objData->unk19 = 2;
                        break;
                    }
                }
                if (var_a0 == 4) {
                    objData->unk19 = 0;
                }
            } else if (spC0 != 0) {
                for (var_a0 = 0; var_a0 < 4; var_a0++) {
                    if (var_a3 == objData->unk6C[var_a0]) {
                        sp5B = var_a0;
                        objData->unk19 = 2;
                        break;
                    }
                }
                if (var_a0 == 4) {
                    objData->unk19 = 5;
                }
            } else {
                if (spC0 == 0 && (var_a3 = gDLL_25->vtbl->func_1474(&self->srt.transl, objData->unkA4), (var_a3 != 0))) {
                    if (var_a3 == objData->unkA6) {
                        objData->unk19 = 3;
                    } else {
                        gDLL_25->vtbl->func_16D4(&objData->unk8->srt.transl, &objData->unkC0, var_a3);
                        objData->unk19 = 4;
                    }
                } else {
                    objData->unk19 = 0;
                    gDLL_25->vtbl->func_15B0(&self->srt.transl, objData->unkA4, var_a3);
                }
            }
        } else if (spBC == 0) {
            objData->unk19 = 0;
        } else if (spC0 != 0) {
            var_a3 = spBC * spC0;
            if (gDLL_25->vtbl->func_15B0(&self->srt.transl, objData->unkA4, var_a3) != 0) {
                if (var_a3 == objData->unkA6) {
                    objData->unk19 = 3;
                } else {
                    objData->unk19 = 5;
                }
            } else {
                for (var_a0 = 0; var_a0 < 4; var_a0++) {
                    if (var_a3 == objData->unk6C[var_a0]) {
                        sp5B = var_a0;
                        objData->unk19 = 2;
                        break;
                    }
                }
                if (var_a0 == 4 || var_a3 != objData->unkA6) {
                    objData->unk19 = 5;
                }
            }
        } else {
            var_a3 = gDLL_25->vtbl->func_1474(&self->srt.transl, objData->unkA4);\
            if (var_a3 != 0) {
                if (spBC == objData->unkA4) {
                    for (var_a0 = 0; var_a0 < 4; var_a0++) {
                        if (var_a3 == objData->unk6C[var_a0]) {
                            sp5B = var_a0;
                            objData->unk19 = 2;
                            break;
                        }
                    }
                    if (var_a0 == 4) {
                        gDLL_25->vtbl->func_16D4(&objData->unk8->srt.transl, &objData->unkC0, var_a3);
                        objData->unk19 = 4;
                    }
                } else if (var_a3 == objData->unkA6) {
                    objData->unk19 = 3;
                } else {
                    gDLL_25->vtbl->func_16D4(&objData->unk8->srt.transl, &objData->unkC0, var_a3);
                    objData->unk19 = 4;
                }
            } else {
                // objData->unk19 = 0; // default.dol
            }
        }
    }
    switch (objData->unk19) {
    case 0:
        objData->unk20 = (-0.03f * gUpdateRateF) + sp84;
        if (objData->unk20 < 0.0f) {
            objData->unk20 = 0.0f;
        }
        dll_211_func_8470(self, temp_s1);
        break;
    default:
        break;
    case 1:
        if (sp84 != 0.0f) {
            dll_211_func_8470(self, temp_s1);
            break;
        }
        /* fallthrough */
    case 2:
        if (sp84 != 0.0f) {
            dll_211_func_8470(self, &objData->unk74[sp5B]);
            break;
        }
        /* fallthrough */
    case 4:
        if (sp84 != 0.0f) {
            dll_211_func_8470(self, &objData->unkC0);
        }
        /* fallthrough */
        break;
    case 3:
        if (sp84 != 0.0f) {
            dll_211_func_8470(self, &objData->unkA8);
            break;
        }

        if (dll_211_func_9668(&objData->unkCC) == 0) {
            sp68[0] = objData->unk2C->x - self->srt.transl.x;
            sp68[1] = objData->unk2C->z - self->srt.transl.z;
            var_s1 = (u16)arctan2_f(-sp68[0], -sp68[1]);
            var_s1 = self->srt.yaw - (var_s1 & 0xFFFF);
            CIRCLE_WRAP(var_s1);
            var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
            if (var_v1 >= 0x1A17) {
                var_s1 = (var_s1 * 5) / 4;
                if (var_s1 > 0) {
                    if (var_s1 >= 0x342F) {
                        func_80023D30(self, 12, 0.0f, 0);
                        var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
                        objData->unk48 = var_v1 / 24490.0f;
                        objData->unk38 = 0.01f;
                    } else {
                        func_80023D30(self, 10, 0.0f, 0);
                        var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
                        objData->unk48 = var_v1 / 6679.0f;
                        objData->unk38 = 0.06f;
                    }
                } else if (var_s1 >= 0x342F) {
                    func_80023D30(self, 11, 0.0f, 0);
                    var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
                    objData->unk48 = var_v1 / 24490.0f;
                    objData->unk38 = 0.01f;
                } else {
                    func_80023D30(self, 9, 0.0f, 0);
                    var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
                    objData->unk48 = var_v1 / 6679.0f;
                    objData->unk38 = 0.06f;
                }
                objData->unk19 = 0xF;
                objData->unk38 /= objData->unk48;
                objData->unk3C = 1.0f;
                objData->unk40 = 1.0f;
                objData->unk4C |= 0x20;
                objData->unk4C |= 0x40;
                objData->unk4C |= 0x100;
            }
        }
        break;
    case 6:
        vec3_distance_xz_squared(&objData->unk3C0->pos, &self->srt.transl);
        if (vec3_distance_xz_squared(&objData->unk3C0->pos, &self->srt.transl) < 60.0f) {
            setup = objData->unk3C0;
            objData->unk3C8.unk0.unk80 = objData->unk3C4;
            spA0 = dll_211_func_7A7C(objData, setup, spB8, objData->unk3C4);
            gDLL_25->vtbl->func_1D30(&objData->unk3C8, setup, spA0, dll_211_func_7A7C(objData, spA0, spB8, objData->unk3C4));
            curves_func_800053B0(&objData->unk3C8.unk0, 0.1f);
            dll_211_func_8974(self, &objData->unk3C8, objData->unk20);
            dll_211_func_8470(self, &objData->unk3C8.unk0.unk68);
            switch (setup->unk1A) {
            case 1:
                dll_211_func_507C(self, objData);
                break;
            case 5:
                dll_211_func_514C(self, objData);
                break;
            case 6:
                dll_211_func_52B8(self, objData);
                break;
            case 2:
            case 7:
                objData->unk4C |= 0x2000;
                /* fallthrough */
            default:
                objData->unk19 = 7;
                break;
            }
        } else {
            dll_211_func_8470(self, &objData->unk3C0->pos);
        }
        break;
    case 5:
        dll_211_func_7794(self, spA8, spC0, sp88);
        
        // NOTE: the return value here is var_a0 (the iterator var) in default.dol
        var_a0 = dll_211_func_7BAC(objData, sp88, spA8, spB8);
        if (var_a0 == -1) {
            objData->unk20 = sp84;
            return 2;
        }
        objData->unk3C4 = spA8[var_a0];
        objData->unk3C0 = sp88[var_a0];

        // FAKE
        if (1) {
            dll_211_func_8470(self, &objData->unk3C0->pos);
        }
        objData->unk19 = 6;
        break;
    case 7:
        sp74.x = objData->unk2C->x - self->srt.transl.x;
        sp74.y = objData->unk2C->y - self->srt.transl.y;
        sp74.z = objData->unk2C->z - self->srt.transl.z;
        sp60[0] = -self->srt.yaw;
        sp60[1] = 0;
        sp60[2] = 0;
        rotate_vec3(&sp60, sp74.f);
        if (sp74.z > 0.0f && objData->unk20 != 0.0f) {
            if (!(objData->unk4C & 2) || (objData->unk64 != spB8) || (objData->unk68 != objData->unk2C) || (objData->unk66 != objData->unk3C8.unk0.unk80)) {
                objData->unk4C |= 2;
                objData->unk4C &= ~4;
                objData->unk64 = spB8;
                objData->unk68 = objData->unk2C;
                objData->unk66 = objData->unk3C8.unk0.unk80;
                route_setup(objData->unk4DC, objData->unk3C8.unkA0, objData->unk2C, (void* ) spB8, objData->unk3C8.unk0.unk80 ^ 1);
                route_setup(&objData->unk4DC[1], objData->unk3C8.unkA0, objData->unk2C, (void* ) spB8, objData->unk3C8.unk0.unk80);
            }
            if (!(objData->unk4C & 4)) {
                temp_v1_4 = route_find(&objData->unk4DC[1], 10);
                switch (temp_v1_4) {
                case 1:
                    objData->unk4C &= ~2;
                    break;
                case -1:
                    objData->unk4C |= 4;
                    break;
                default:
                    temp_v1_4 = route_find(objData->unk4DC, 10);
                    if (temp_v1_4 != -1) {
                        if (temp_v1_4 == 1) {
                            objData->unk4C |= 4;
                        }
                    } else {
                        objData->unk4C &= ~2;
                    }
                    break;
                }
            }
            objData->unk50 -= gUpdateRateF;
            if (objData->unk50 <= 0.0f && objData->unk4C & 4) {
                temp_s1_4 = objData->unk3C8.unk0.unk80 ^ 1;
                objData->unk50 = 120.0f;
                if (!temp_s1_4) {
                    curves_func_800053B0(&objData->unk3C8.unk0, 2.0f);
                } else {
                    curves_func_800053B0(&objData->unk3C8.unk0, -2.0f);
                }
                objData->unk3C8.unk0.unk80 = temp_s1_4;
                gDLL_25->vtbl->func_29FC(&objData->unk3C8);
            }
        } else {
            objData->unk50 = 120.0f;
            objData->unk4C &= ~2;
        }
        if ((objData->unk3C8.unk0.unk80 == 0 && objData->unk3C8.unk0.unk10 != 0) || (objData->unk3C8.unk0.unk80 != 0 && objData->unk3C8.unk0.unk10 == 0)) {
            setup2 = dll_211_func_7A7C(objData, objData->unk3C8.unkA4, spB8, objData->unk3C8.unk0.unk80);
            if (setup2 != NULL) {
                dummy:
                gDLL_25->vtbl->func_21F4(&objData->unk3C8, setup2);
                if ((objData->unk3C8.unk9C->unk1A == 2) || (objData->unk3C8.unk9C->unk1A == 7)) {
                    if (objData->unk4C & 0x2000) {
                        objData->unk4C &= ~0x2000;
                    } else {
                        objData->unk4C |= 0x2000;
                    }
                }
            } else {
                if (objData->unk3C8.unk0.unk80 != 0) {
                    curves_func_800053B0(&objData->unk3C8.unk0, 2.0f);
                } else {
                    curves_func_800053B0(&objData->unk3C8.unk0, -2.0f);
                }
                objData->unk3C8.unk9C = (CurveSetup *)((s32)objData->unk3C8.unk9C ^ (s32)objData->unk3C8.unkA4);\
                objData->unk3C8.unkA4 = (CurveSetup *)((s32)objData->unk3C8.unkA4 ^ (s32)objData->unk3C8.unk9C);\
                objData->unk3C8.unk9C = (CurveSetup *)((s32)objData->unk3C8.unk9C ^ (s32)objData->unk3C8.unkA4);
            }
        } else {
            setup2 = dll_211_func_7A7C(objData, objData->unk3C8.unkA0, spB8, objData->unk3C8.unk0.unk80);
            if (setup2 != objData->unk3C8.unkA4) {
                dummy2:
                gDLL_25->vtbl->func_27A8(&objData->unk3C8, setup2);
            }
        }
        dll_211_func_8974(self, &objData->unk3C8, objData->unk20);
        dll_211_func_8470(self, &objData->unk3C8.unk0.unk68);
        switch (var_v1 = objData->unk3C8.unkA0->unk1A) {
        case 1:
            objData->unk19 = 8;
            break;
        case 5:
            objData->unk19 = 11;
            break;
        case 6:
            objData->unk19 = 13;
            break;
        }
        break;
    case 8:
        dll_211_func_8ED0(sp84, 1.5f, &objData->unk20);
        dll_211_func_8974(self, &objData->unk3C8, objData->unk20);
        dll_211_func_8470(self, &objData->unk3C8.unk0.unk68);
        if ((objData->unk3C8.unk0.unk80 == 0 && objData->unk3C8.unk0.unk10 != 0) || (objData->unk3C8.unk0.unk80 != 0 && objData->unk3C8.unk0.unk10 == 0)) {
            gDLL_25->vtbl->func_21F4(&objData->unk3C8, dll_211_func_7A7C(objData, objData->unk3C8.unkA4, spB8, objData->unk3C8.unk0.unk80));
            dll_211_func_507C(self, objData);
        }
        break;
    case 9:
        dll_211_func_8ED0(sp84, 2.3f, &objData->unk20);
        dll_211_func_87E4(self);
        if (self->animProgress < 0.5f) {
            func_8002493C(self, objData->unk20, &sp44.unk10);
            self->srt.transl.x += objData->unk30[0] * objData->unk20 * gUpdateRateF;
            self->srt.transl.z += objData->unk30[1] * objData->unk20 * gUpdateRateF;
        } else {
            func_8002493C(self, objData->unk20 * 0.25f, &sp44.unk10);
            self->srt.transl.x += objData->unk30[0] * (objData->unk20 * 0.25f) * gUpdateRateF;
            // FAKE
            if (var_v1);
            self->srt.transl.z += objData->unk30[1] * (objData->unk20 * 0.25f) * gUpdateRateF;
        }
        if (func_80024108(self, sp44.unk10, gUpdateRateF, NULL) != 0) {
            setup = objData->unk3C8.unkA0;
            temp_fv0 = setup->pos.x - self->srt.transl.x;
            var_fv1 = setup->pos.z - self->srt.transl.z;
            temp_fv0 = sqrtf(SQ(temp_fv0) + SQ(var_fv1));
            objData->unk54 = temp_fv0 / 2.3f;
            objData->unk58 = 0.0f;
            objData->unk60 = self->srt.transl.y;
            objData->unk5C = (((setup->pos.y - self->srt.transl.y) - (-0.017f * objData->unk54 * objData->unk54)) / objData->unk54);
            func_80023D30(self, 0x16, 0.0f, 0);
            func_800240BC(self, objData->unk58 / objData->unk54);
            objData->unk20 = 2.3f;
            objData->unk19 = 10;
            dll_211_func_8A94(self, &objData->unk3C8);
        }
        objData->unk38 = 0.0f;
        break;
    case 10:
        objData->unk58 += gUpdateRateF;
        if (objData->unk54 <= objData->unk58) {
            self->srt.transl.y = objData->unk3C8.unkA0->pos.y;
            func_800240BC(self, 1.0f);
            objData->unk19 = 7;
            objData->unk38 = 0.0f;
        } else {
            self->srt.transl.y = objData->unk60 + (objData->unk5C * objData->unk58) + (-0.017f * objData->unk58 * objData->unk58);
            var_fv1 = objData->unk54;
            if (var_fv1 <= 24.0f) {
                func_800240BC(self, objData->unk58 / var_fv1);
            } else {
                sp44.unkC = 1.0f;
                if (objData->unk58 <= 6.0f) {
                    func_800240BC(self, objData->unk58 / 24.0f);
                } else if ((var_fv1 - 6.0f) <= objData->unk58) {
                    func_800240BC(self, ((24.0f - var_fv1) + objData->unk58) / 24.0f);
                    sp44.unkC = (((objData->unk54 - objData->unk58) / 6.0f) * 0.75f) + 0.25f;
                } else {
                    func_800240BC(self, (((objData->unk58 - 6.0f) / (var_fv1 - 12.0f)) * 0.5f) + 0.25f);
                }
            }
            objData->unkCC.mode = 0;
            objData->unk38 = 0.0f;
            self->srt.transl.x += objData->unk30[0] * 2.3f * gUpdateRateF * sp44.unkC;
            self->srt.transl.z += objData->unk30[1] * 2.3f * gUpdateRateF * sp44.unkC;
        }
        break;
    case 11:
        dll_211_func_8ED0(sp84, 1.5f, &objData->unk20);
        dll_211_func_8974(self, &objData->unk3C8, objData->unk20);
        dll_211_func_8470(self, &objData->unk3C8.unk0.unk68);
        if ((objData->unk3C8.unk0.unk80 == 0 && objData->unk3C8.unk0.unk10 != 0) || (objData->unk3C8.unk0.unk80 != 0 && objData->unk3C8.unk0.unk10 == 0)) {
            gDLL_25->vtbl->func_21F4(&objData->unk3C8, dll_211_func_7A7C(objData, objData->unk3C8.unkA4, spB8, objData->unk3C8.unk0.unk80));
            dll_211_func_514C(self, objData);
        }
        break;
    case 12:
    case 14:
        objData->unkCC.mode = 0;
        dll_211_func_8974(self, &objData->unk3C8, objData->unk20);
        dll_211_func_87E4(self);
        if (self->animProgress >= 0.95f) {
            objData->unk20 = 0.375f;
            dll_211_func_8470(self, &objData->unk3C8.unk0.unk68);
            objData->unk19 = 7;
        }
        break;
    case 13:
        dll_211_func_8ED0(sp84, 1.5f, &objData->unk20);
        dll_211_func_8974(self, &objData->unk3C8, objData->unk20);
        dll_211_func_8470(self, &objData->unk3C8.unk0.unk68);
        if ((objData->unk3C8.unk0.unk80 == 0 && objData->unk3C8.unk0.unk10 != 0) || (objData->unk3C8.unk0.unk80 != 0 && objData->unk3C8.unk0.unk10 == 0)) {
            gDLL_25->vtbl->func_21F4(&objData->unk3C8, dll_211_func_7A7C(objData, objData->unk3C8.unkA4, spB8, objData->unk3C8.unk0.unk80));
            dll_211_func_52B8(self, objData);
        }
        break;
    case 15:
        if (self->animProgress >= 0.95f) {
            objData->unk4C &= ~0x20;
            objData->unk4C &= ~0x40;
            objData->unk4C &= ~0x100;
            objData->unk20 = 0.0f;
            dll_211_func_8F84(self, temp_s1, objData->unk30);
            objData->unk19 = 1;
            return 1;
        }
        break;
    }

    if (objData->unk19 < 5 && (gDLL_25->vtbl->func_12FC(&self->srt.transl) == 0)) {
        self->srt.transl.x = objData->unkB4.x;
        self->srt.transl.y = objData->unkB4.y;
        self->srt.transl.z = objData->unkB4.z;
    }
    objData->unkB4.x = self->srt.transl.x;
    objData->unkB4.y = self->srt.transl.y;
    objData->unkB4.z = self->srt.transl.z;
    if (objData->unk19 == 0) {
        return 2;
    }
    if (objData->unk20 == 0.0f && objData->unk19 < 8) {
        return 0;
    }
    return 1;
}


// offset: 0x7188 | func: 55
static void dll_211_func_7188(Object* self) {
    s32 i;
    DLL211_Data* objData;

    objData = self->data;
    for (i = 0; i < 4; i++) {
        func_80031F6C(self, i + 1, &objData->unk384[i].x, &objData->unk384[i].y, &objData->unk384[i].z, 0);
    }

    func_80031F6C(self, 5, &objData->unk3B4.x, &objData->unk3B4.y, &objData->unk3B4.z, 0);
}

// offset: 0x726C | func: 56
static void dll_211_func_726C(Object* self) {
    DLL211_Data* objData; // sp4C
    Object* temp_a1;
    SRT sp30;

    objData = self->data;
    temp_a1 = objData->unk28;
    sp30.transl.x = objData->unk384[0].x;
    sp30.transl.y = objData->unk384[0].y;
    sp30.transl.z = objData->unk384[0].z;
    sp30.yaw = self->srt.yaw;
    if (temp_a1->id == OBJ_GroundAnimator) {
        sp30.roll = ((DLL_Unknown*)temp_a1->dll)->vtbl->func[9].withOneArgS32((s32)temp_a1);
    } else if (temp_a1->id == OBJ_WallAnimator) {
        sp30.roll = ((DLL_Unknown*)temp_a1->dll)->vtbl->func[9].withOneArgS32((s32)temp_a1);
    } else {
        sp30.roll = 0;
    }
    if (rand_next(0, 4) == 0) {
        gDLL_17_partfx->vtbl->spawn(self, 0xCA, &sp30, 1, -1, NULL);
    }
    if (rand_next(0, 4) == 0) {
        gDLL_17_partfx->vtbl->spawn(self, 0xCB, &sp30, 1, -1, NULL);
    }
    sp30.transl.x = objData->unk384[1].x;
    sp30.transl.y = objData->unk384[1].y;
    sp30.transl.z = objData->unk384[1].z;
    sp30.yaw = self->srt.yaw;
    if (rand_next(0, 4) == 0) {
        gDLL_17_partfx->vtbl->spawn(self, 0xCA, &sp30, 1, -1, NULL);
    }
    if (rand_next(0, 4) == 0) {
        gDLL_17_partfx->vtbl->spawn(self, 0xCB, &sp30, 1, -1, NULL);
    }
}

/*0x0*/ static const char str_0[] = "%d %d %d %d\n";
/*0x10*/ static const char str_10[] = "hits: %d %d %d %d %d %d %d %d";
/*0x30*/ static const char str_30[] = "\nEnergy: %d, Skill %d\n";
/*0x48*/ static const char str_48[] = "find command used on the wrong object\n";
/*0x70*/ static const char str_70[] = "find command used on the wrong object\n";
/*0x98*/ static const char str_98[] = "sideCommandEnable error, command number too high\n";
/*0xCC*/ static const char str_CC[] = "sideFind 1\n";
/*0xD8*/ static const char str_D8[] = "sideFind 2\n";
/*0xE4*/ static const char str_E4[] = "Couldn't find a distract node\n";
/*0x104*/ static const char str_104[] = "DISTRACT_SPOTFINDING (1)\n";
/*0x120*/ static const char str_120[] = "DISTRACT_SPOTTOSPOT (1)\n";
/*0x13C*/ static const char str_13C[] = "DISTRACT_SPOTDISTRACTING (1)\n";
/*0x15C*/ static const char str_15C[] = "DISTRACT_SPOTRETURING (1)\n";
/*0x178*/ static const char str_178[] = "DISTRACT_SPOTFINDING (2)\n";
/*0x194*/ static const char str_194[] = "DISTRACT_SPOTDISTRACTING (2)\n";
/*0x1B4*/ static const char str_1B4[] = "FLAME_NONE\n";
/*0x1C0*/ static const char str_1C0[] = "FLAME_FINDING_OUT\n";
/*0x1D4*/ static const char str_1D4[] = "FLAME_GOINGTOEDGE\n";
/*0x1E8*/ static const char str_1E8[] = "FLAME_TOSTART\n";
/*0x1F8*/ static const char str_1F8[] = "FLAME_OUT\n";
/*0x204*/ static const char str_204[] = "FLAME_FINDING_IN\n";
/*0x218*/ static const char str_218[] = "FLAME_IN\n";
/*0x224*/ static const char str_224[] = "FLAME_TOEND\n";
/*0x234*/ static const char str_234[] = "GUARD_INIT\n";
/*0x240*/ static const char str_240[] = "GUARD_FINDING\n";
/*0x250*/ static const char str_250[] = "GUARD_TOSPOT\n";
/*0x260*/ static const char str_260[] = "GUARD_TOFRONT\n";
/*0x270*/ static const char str_270[] = "GUARD_TOBADDIE\n";
/*0x280*/ static const char str_280[] = "GUARD_FLAME\n";
/*0x290*/ static const char str_290[] = "GUARD_DOWNTOGROWL\n";
/*0x2A4*/ static const char str_2A4[] = "GUARD_GROWL\n";
/*0x2B4*/ static const char str_2B4[] = "GUARD_UPFROMGROWL\n";
/*0x2C8*/ static const char str_2C8[] = "error tricky should never stop when circling\n";
/*0x2F8*/ static const char str_2F8[] = "BADDIEALERT_GOTO\n";
/*0x30C*/ static const char str_30C[] = "BADDIEALERT_BARK %d %d\n";
/*0x324*/ static const char str_324[] = "BADDIEALLERT_GOTOFLAME\n";
/*0x33C*/ static const char str_33C[] = "BADDIEALLERT_FLAME\n";
/*0x350*/ static const char str_350[] = "GROWLAT_GOTO\n";
/*0x360*/ static const char str_360[] = "GROWLAT_GROWLING\n";
/*0x374*/ static const char str_374[] = "GROWLAT_GOTOFLAME\n";
/*0x388*/ static const char str_388[] = "GROWLAT_FLAME\n";
/*0x398*/ static const char str_398[] = "tricky wg %d->%d target wg %d, dest wg %d\n";
/*0x3C4*/ static const char str_3C4[] = "tricky last walk group is zero. Has he been loaded within a walk group? %f %f %f\n";
/*0x418*/ static const char str_418[] = "target is within a walkGroup or its patch\n";
/*0x444*/ static const char str_444[] = "target is not within a walkGroup or any patches\n";
/*0x478*/ static const char str_478[] = "target is within patch group %d\n";
/*0x49C*/ static const char str_49C[] = "Patch %d: Last xyz %f %f %f\n";
/*0x4BC*/ static const char str_4BC[] = "Last Patch Point %f %f %f\n";
/*0x4D8*/ static const char str_4D8[] = "tricky error, target patch %d, targetWalkGroup %d, trickyWalkGroup %d, tricky last walkGroup %d, tricky in patch %d\n";
/*0x550*/ static const char str_550[] = "tricky error 2!!!!!\n";
/*0x568*/ static const char str_568[] = "walk wait\n";
/*0x574*/ static const char str_574[] = "walk free\n";
/*0x580*/ static const char str_580[] = "walk start patch\n";
/*0x594*/ static const char str_594[] = "walk patch exit\n";
/*0x5A8*/ static const char str_5A8[] = "walk end patch\n";
/*0x5B8*/ static const char str_5B8[] = "about to turn\n";
/*0x5C8*/ static const char str_5C8[] = "large turn\n";
/*0x5D4*/ static const char str_5D4[] = "large turn\n";
/*0x5E0*/ static const char str_5E0[] = "standing\n";
/*0x5EC*/ static const char str_5EC[] = "walk to node %d %d\n";
/*0x600*/ static const char str_600[] = "walk types are %d %d %d\n";
/*0x61C*/ static const char str_61C[] = "curve setup\n";
/*0x62C*/ static const char str_62C[] = "walk nodes\n";
/*0x638*/ static const char str_638[] = "No route exists in the forward direction\n";
/*0x664*/ static const char str_664[] = "Reverse direction is quicker\n";
/*0x684*/ static const char str_684[] = "Reversing direction\n";
/*0x69C*/ static const char str_69C[] = "Jump run up\n";
/*0x6AC*/ static const char str_6AC[] = "Jump prep\n";
/*0x6B8*/ static const char str_6B8[] = "Jumping\n";
/*0x6C4*/ static const char str_6C4[] = "Jump up run up\n";
/*0x6D4*/ static const char str_6D4[] = "i before %d\n";
/*0x6E4*/ static const char str_6E4[] = "i now %d\n";
/*0x6F0*/ static const char str_6F0[] = "JUMPDOWN or JUMPUP\n";
/*0x704*/ static const char str_704[] = "JUMPDOWN_RUNUP\n";
/*0x714*/ static const char str_714[] = "TURNING\n";
/*0x720*/ static const char str_720[] = "entered a non valid movementstate\n";
/*0x744*/ static const char str_744[] = "tricky left a valid area, pushing him back...\n";
/*0x774*/ static const char str_774[] = "";
/*0x778*/ static const DLTri _rodata_778[] = {
    MASK_TRI(0x40, 2, 1, 0),
    MASK_TRI(0x40, 3, 1, 2),
    MASK_TRI(0x40, 4, 5, 6),
    MASK_TRI(0x40, 6, 5, 7),
    MASK_TRI(0x40, 6, 3, 2),
    MASK_TRI(0x40, 3, 6, 7),
    MASK_TRI(0x40, 3, 7, 1),
    MASK_TRI(0x40, 7, 5, 1),
    MASK_TRI(0x40, 4, 0, 1),
    MASK_TRI(0x40, 4, 1, 5),
    MASK_TRI(0x40, 0, 4, 2),
    MASK_TRI(0x40, 2, 4, 6)
};
/*0x838*/ static const Vec3f _rodata_838[] = {
    VEC3F(-12.0f, 20.0f, -20.0f),
    VEC3F(12.0f, 20.0f, -20.0f),
    VEC3F(-12.0f, 20.0f, 20.0f),
    VEC3F(12.0f, 20.0f, 20.0f),
    VEC3F(-12.0f, -7.0f, -20.0f),
    VEC3F(12.0f, -7.0f, -20.0f),
    VEC3F(-12.0f, -7.0f, 20.0f),
    VEC3F(12.0f, -7.0f, 20.0f)
};
/*0x898*/ static const char str_898[] = "Reached end of network, curve %d at (%f,%f)\n";
/*0x8C8*/ static const char str_8C8[] = "in water\n";
/*0x8D4*/ static const char str_8D4[] = "out of water\n";
/*0x8E4*/ static const char str_8E4[] = "Vel %f\n";
/*0x8EC*/ static const char str_8EC[] = "in water\n";
/*0x8F8*/ static const char str_8F8[] = "out of water\n";
/*0x908*/ static const char str_908[] = "in water\n";
/*0x914*/ static const char str_914[] = "out of water\n";

// offset: 0x74C4 | func: 57
static void dll_211_func_74C4(Object* self, Gfx** arg1, Mtx** arg2, Vtx** arg3, Triangle** pols) {
    Vtx* sp6C;
    DLL211_Data* objData;
    s32 i;
    f32 sp60;
    SRT sp48;
    Vtx* var_s0;

    objData = self->data;
    // unk5E4 is an array of Object pointers? Unless the given objects objData is not Tricky?
    sp60 = ((f32*)objData->unk5E4)[0];
    sp6C = *arg3;
    sp48.transl.x = self->srt.transl.x;
    var_s0 = sp6C;
    sp48.transl.y = self->srt.transl.y + sp60;
    if (objData->unk1A != 2) {
        sp48.transl.y += 5.0f;
    }
    sp48.transl.z = self->srt.transl.z;
    sp48.scale = 0.001f;
    gDLL_17_partfx->vtbl->spawn(self, 0x1B6, &sp48, 0x200001, -1, NULL);
    dl_set_prim_color(arg1, 0xFF, 0xFF, 0xFF, 0x80);
    gSPGeometryMode(*arg1, 0xFFFFFF, 0x210405);
    dl_apply_geometry_mode(arg1);
    gDPSetCombineLERP((*arg1), TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED);
    dl_apply_combine(arg1);
    for (i = 0; i < 8; i++) {
        var_s0->v.ob[0] = _rodata_838[i].x * 20.0f;
        if (i < 4) {
            var_s0->v.ob[1] = sp60 * 20.0f;
        } else {
            var_s0->v.ob[1] = -0x8C;
        }
        var_s0->v.ob[2] = _rodata_838[i].z * 20.0f;
        var_s0->v.cn[0] = 0xFF;
        var_s0->v.cn[1] = 0;
        var_s0->v.cn[2] = 0;
        var_s0->v.cn[3] = 0x40;
        var_s0++;
    }
    gDPSetOtherMode(
        (*arg1),
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
        G_AC_NONE | G_ZS_PIXEL | Z_CMP | Z_UPD | IM_RD | CVG_DST_SAVE | ZMODE_XLU | FORCE_BL | G_RM_FOG_SHADE_A | GBL_c2(G_BL_CLR_IN, G_BL_0, G_BL_CLR_MEM, G_BL_1MA)
    );
    dl_apply_other_mode(arg1);
    sp48.transl.x = self->srt.transl.x;
    sp48.transl.y = self->srt.transl.y;
    sp48.transl.z = self->srt.transl.z;
    sp48.yaw = self->srt.yaw;
    sp48.pitch = 0;
    sp48.roll = 0;
    sp48.scale = 0.05f;
    camSetupObjectSRTMatrix(arg1, arg2, &sp48, 1.0f, 0.0f, NULL);
    gSPVertex((*arg1)++, OS_PHYSICAL_TO_K0(sp6C), 8, 0);
    dl_triangles(arg1, _rodata_778, 12);
    *arg3 = var_s0;
}

// offset: 0x7794 | func: 58
static void dll_211_func_7794(Object* self, u8 *arg1, s16 arg2, CurveSetup** arg3) {
    s32 i;
    f32 sp7C[4];
    CurveNode* temp_a1;
    s32 sp74;
    CurveSetup* temp_s1;
    f32 temp_fs0;
    u8 var_a1;
    u8 sp6A;
    u8 var_a2;
    DLL211_Data* objData;

    objData = self->data;
    temp_a1 = gDLL_26_Curves->vtbl->func_1BC(&sp74);
    for (i = 0; i < 4; i++) {
        sp7C[i] = M_INFINITY_F;
        arg3[i] = 0;   
    }

    if (arg2 == 0) {
        return;
    }

    for (i = 0; i < sp74; i++) {
        temp_s1 = temp_a1[i].setup;
        if (temp_s1->curveType == 0x24 && temp_s1->unk3 == 0) {
            if (
                (temp_s1->type22.unk30 == -1 || main_get_bits(temp_s1->type22.unk30) != 0) &&
                (temp_s1->type22.usedBit == -1 || main_get_bits(temp_s1->type22.usedBit) == 0)
            ) {
                temp_fs0 = 
                    SQ(objData->unk2C->z - temp_s1->pos.z) + (
                        (SQ(self->globalPosition.x - temp_s1->pos.x) + SQ(self->globalPosition.z - temp_s1->pos.z)) +
                        SQ(objData->unk2C->x - temp_s1->pos.x)
                    );
                if (temp_fs0 < sp7C[3]) {
                    for (var_a1 = 0; var_a1 < 4; var_a1++) {
                        if ((temp_s1->links[var_a1] >= 0) && (arg2 == temp_s1->unk4_u8[var_a1])) {
                            sp6A = temp_s1->unk1B >> var_a1;
                            break;
                        }
                    }

                    if (var_a1 != 4) {
                        for (var_a1 = 0; var_a1 < 4; var_a1++) {
                            if (temp_fs0 < sp7C[var_a1]) {
                                for (var_a2 = 3; var_a2 > var_a1; var_a2--) {
                                    arg1[var_a2] = arg1[var_a2 - 1];
                                    arg3[var_a2] = arg3[var_a2 - 1];
                                    sp7C[var_a2] = sp7C[var_a2 - 1];
                                }
                                arg1[var_a1] = (sp6A & 1) ^ 1;
                                arg3[var_a1] = temp_s1;
                                sp7C[var_a1] = temp_fs0;
                                break;
                            }    
                        }
                    }
                }
            }
        }
    }
}

// offset: 0x7A7C | func: 59
static CurveSetup* dll_211_func_7A7C(DLL211_Data* objData, CurveSetup* arg1, s32 arg2, u8 arg3) {
    CurveSetup* var_v1;

    var_v1 = NULL;
    if (objData->unk4D0 == arg1 && objData->unk4D8 == arg2 && objData->unk4DA == arg3) {
        var_v1 = dll_211_func_8114(objData->unk4D4);
    }
    if (var_v1 == NULL) {
        var_v1 = dll_211_func_7EFC(objData, arg1, arg2, arg3);
        if (var_v1 == NULL) {
            var_v1 = dll_211_func_7DB8(objData, arg1, (void* ) arg2);
            if (var_v1 == NULL) {
                var_v1 = dll_211_func_7EFC(objData, arg1, 0, arg3);
                if (var_v1 == NULL) {
                    return NULL;
                }
            }
        }
    }
    objData->unk4D0 = arg1;
    objData->unk4D4 = var_v1;
    objData->unk4D8 = arg2;
    objData->unk4DA = arg3;
    return var_v1;
}

// offset: 0x7BAC | func: 60
static s8 dll_211_func_7BAC(DLL211_Data* objData, CurveSetup** arg1, u8 *arg2, s32 arg3) {
    s8 sp5C[4];
    s8 sp5B;
    s8 temp_v0;
    s8 var_s0;
    s8 var_s2;

    for (sp5B = 0; sp5B < 4; sp5B++) {
        if (arg1[sp5B] != NULL) {
            route_setup(&objData->unk4DC[sp5B], arg1[sp5B], objData->unk2C, arg3, arg2[sp5B]);
        }
    }

    for (sp5B = 0; sp5B < 0x64; sp5B++) {
        var_s2 = 0;
        for (var_s0 = 0; var_s0 < 4; var_s0++) {
            if (arg1[var_s0] != NULL) {
                sp5C[var_s0] = route_find(&objData->unk4DC[var_s0], 5);
            } else {
                sp5C[var_s0] = -1;
            }
            temp_v0 = sp5C[var_s0];
            if (temp_v0 != -1) {
                if (temp_v0 == 1) {
                    return var_s0;
                }
            } else {
                arg1[var_s0] = NULL;
                var_s2++;
            } 
        }

        switch (var_s2) {
            case 3:
                if (*arg1 != NULL) {
                    sp5C[0] = route_find(objData->unk4DC, 500);
                }
            
                if (sp5C[0] == 1) {
                    return 0;
                }
            
                return -1;
            case 4:
                return -1;
            default:
                break;
        }    
    }

    return -1;
}

// offset: 0x7DB8 | func: 61
static CurveSetup* dll_211_func_7DB8(DLL211_Data* objData, CurveSetup* arg1, void* arg2) {
    if (arg2 == NULL) {
        return NULL;
    }

    if (arg2 == objData->unk5D0 && arg1 == objData->unk5CC) {
        objData->unk5CC = route_next(&objData->unk59C);
        if (objData->unk5CC == NULL) {
            return NULL;
        }

        objData->unk5CC = dll_211_func_8114(objData->unk5CC);
        if (objData->unk5CC != NULL) {
            return objData->unk5CC;
        }
    }

    route_setup(&objData->unk59C, arg1, objData->unk2C, arg2, objData->unk3C8.unk0.unk80);
    if (route_find(&objData->unk59C, 500) != 1) {
        return NULL;
    }

    route_reconstruct(&objData->unk59C);
    objData->unk5CC = route_next(&objData->unk59C);
    objData->unk5D0 = arg2;
    return objData->unk5CC;
}

// offset: 0x7EFC | func: 62
static CurveSetup* dll_211_func_7EFC(DLL211_Data* objData, CurveSetup* arg1, s32 arg2, s32 arg3) {
    CurveSetup *sp68[4];
    f32 temp_fv0;
    f32 var_fs0;
    u16 var_s0;
    u16 var_s1;
    u16 var_s2;
    u16 var_s3;

    for (var_s0 = 0, var_s2 = 0, var_s3 = 1; var_s0 < 4; var_s0++, var_s3 <<= 1, arg3 <<= 1) {
        if (arg1->links[var_s0] >= 0 && ((arg3^0) != (arg1->unk1B & var_s3)) == 0) {
            sp68[var_s2] = gDLL_26_Curves->vtbl->func_39C(arg1->links[var_s0]);
            // FAKE
            if (1) { }
            if (sp68[var_s2] != NULL) {
                if ((arg2 == 0 || arg2 == (arg1->unk4_u8[var_s2]))) {
                    if (
                        (sp68[var_s2]->type22.unk30 == -1 || main_get_bits(sp68[var_s2]->type22.unk30) != 0) &&
                        (sp68[var_s2]->type22.usedBit == -1 || main_get_bits(sp68[var_s2]->type22.usedBit) == 0)
                    ) {
                        var_s2++;
                    }
                }
            }
        }
    }

    if (var_s2) {
        var_s0 = 0;
        var_fs0 = vec3_distance_xz_squared(&objData->unk8->globalPosition, &sp68[var_s0]->pos);
        var_s1 = var_s0;
        for (var_s0 = 1; var_s0 < var_s2; var_s0++) {
            temp_fv0 = vec3_distance_xz_squared(&objData->unk8->globalPosition, &sp68[var_s0]->pos);
            if (temp_fv0 < var_fs0) {
                var_fs0 = temp_fv0;
                var_s1 = var_s0;
            }
        }
        return sp68[var_s1];
    }

    return NULL;
}

// offset: 0x8114 | func: 63
static CurveSetup* dll_211_func_8114(CurveSetup* setup) {
    if ((setup->type22.unk30 == -1 || main_get_bits(setup->type22.unk30) != 0) && (setup->type22.usedBit == -1 || main_get_bits(setup->type22.usedBit) == 0)) {
        return setup;
    }
    return NULL;
}

// offset: 0x81A8 | func: 64
static void dll_211_func_81A8(Object* self) {
    DLL211_Data* temp_v0;
    Vec3f* temp_v1;

    temp_v0 = self->data;
    temp_v1 = temp_v0->unk2C;
    temp_v0->unk5D4 = temp_v1;
    if (temp_v1 != NULL) {
        temp_v0->unk5D8.x = temp_v1->x;
        temp_v0->unk5D8.y = temp_v1->y;
        temp_v0->unk5D8.z = temp_v1->z;
    }
}

// offset: 0x81D8 | func: 65
static f32 dll_211_func_81D8(Object* self) {
    DLL211_Data* objData;
    Vec3f* sp28;
    f32 sp24;
    f32 temp_fa1;
    f32 temp_fv1;

    objData = self->data;
    sp28 = objData->unk2C;
    if (sp28 == objData->unk5D4) {
        temp_fv1 = objData->unk5D8.x - self->srt.transl.x;
        temp_fa1 = objData->unk5D8.z - self->srt.transl.z;
        sp24 = sqrtf(SQ(temp_fv1) + SQ(temp_fa1)) / gUpdateRateF;
        temp_fv1 = sp28->x - self->srt.transl.x;
        temp_fa1 = sp28->z - self->srt.transl.z;
        temp_fv1 = sqrtf(SQ(temp_fv1) + SQ(temp_fa1)) / gUpdateRateF;
        return temp_fv1 - sp24;
    }

    return 0.0f;
}

// offset: 0x82B8 | func: 66
static void dll_211_func_82B8(DLL211_Data* objData) {
    objData->unk18 = 1;
    objData->unk1A = 0;
    objData->unk5F4_vec.x = 0.0f;
    objData->unk5F4_vec.y = 0.0f;
    objData->unk5F4_vec.z = 0.0f;
    objData->unk600 = 600.0f;
    objData->unk4C &= ~0x10;
    objData->unk1D = 0;
}

// offset: 0x8308 | func: 67
static void dll_211_func_8308(Object* self, s32 arg1) {
    DLL211_Data* objData;

    objData = self->data;
    if (arg1 == self->curModAnimId) {
        return;
    }

    if (self->curModAnimId > 0 && self->curModAnimId < 6 && arg1 > 0 && arg1 < 6) {
        if (objData->unk24 > 20.0f) {
            objData->unk24 = 0.0f;
            func_80023D30(self, arg1, self->animProgress, 0);
        }
    } else {
        objData->unk24 = 0.0f;
        func_80023D30(self, arg1, self->animProgress, 0);
    }
}

// offset: 0x83BC | func: 68
static void dll_211_func_83BC(Object* self, DLL211_Data* objData) {
    if (dll_211_func_9668(&objData->unkCC) != 0) {
        dll_211_func_8308(self, 8);
        self->srt.transl.y = objData->unkCC.waterY;
        objData->unk38 = 0.02f;
    } else {
        dll_211_func_8308(self, 0);
        objData->unk38 = 0.005f;
    }
}

// offset: 0x8470 | func: 69
static void dll_211_func_8470(Object* self, Vec3f* arg1) {
    s32 temp;
    f32 var_f0;
    f32* temp_a2;
    DLL211_Data* objData;
    s16 sp2E;
    DLL27_Data* sp24;

    objData = self->data;
    sp24 = &objData->unkCC;
    var_f0=  objData->unk20;
    temp_a2 = objData->unk30;
    dll_211_func_8F84(self, arg1, objData->unk30);
    temp = sp2E - (self->srt.yaw & 0xFFFF);
    if (var_f0 > 0.0f) {
        sp2E = self->srt.yaw;
        dll_211_func_87E4(self);

        // FAKE code that probably got optimized out?
        temp = sp2E - (self->srt.yaw & 0xFFFF);
        // FAKE
        if (1)
        CIRCLE_WRAP(temp);
        if (dll_211_func_9668(&objData->unkCC) != 0) {
            dll_211_func_8308(self, 7);
            self->srt.transl.y = sp24->waterY;
        } else if (var_f0 > 1.0f) {
            dll_211_func_8308(self, 5);
        } else if (var_f0 > 0.66f) {
            dll_211_func_8308(self, 4);
        } else if (var_f0 > 0.33f) {
            dll_211_func_8308(self, 2);
        } else {
            dll_211_func_8308(self, 1);
        }
        self->srt.transl.x += temp_a2[0] * var_f0 * gUpdateRateF;
        self->srt.transl.z += temp_a2[1] * var_f0 * gUpdateRateF;
        func_8002493C(self, var_f0, &objData->unk38);
        return;
    }
    if (dll_211_func_9668(&objData->unkCC) != 0) {
        dll_211_func_8308(self, 7);
        self->srt.transl.y = sp24->waterY;
    } else {
        dll_211_func_8308(self, 0);
        objData->unk38 = 0.005f;
    }
}

// offset: 0x871C | func: 70
static void dll_211_func_871C(Object* self, s8* arg1) {
    s16 temp_a2;
    s16 var_a1;
    s16 var_v0;
    s32 var_v1;

    temp_a2 = arg1[0x2C] << 8;
    var_v0 = temp_a2 - (self->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(var_v0);
    var_a1 = temp_a2;
    if (var_v0 >= 0) {
        var_v1 = var_v0;
    } else {
        var_v1 = -var_v0;
    }
    if (var_v1 >= 0x4000) {
        var_a1 = temp_a2 + 0x8000;
    }
    dll_211_func_88F4(self, var_a1);
}

// offset: 0x87E4 | func: 71
static void dll_211_func_87E4(Object* self) {
    f32* objData;
    f32 x, z;

    objData = self->data;
    objData += 12;
    z = -objData[1];
    x = -objData[0];
    dll_211_func_88F4(self, arctan2_f(x, z));
}

// offset: 0x885C | func: 72
static void dll_211_func_885C(Object* self) {
    DLL211_Data* objData;
    f32 sp24[2];

    objData = self->data;
    sp24[0] = objData->unk2C->x - self->srt.transl.x;
    sp24[1] = objData->unk2C->z - self->srt.transl.z;
    dll_211_func_88F4(self, arctan2_f(-sp24[0], -sp24[1]));
}

// offset: 0x88F4 | func: 73
static void dll_211_func_88F4(Object* self, s16 arg1) {
    s32 temp_a2;
    s32 var_v1;

    temp_a2 = self->srt.yaw;
    var_v1 = self->srt.yaw - (arg1 & 0xFFFF);
    // can't use the CIRCLE_WRAP macro here as we need the repeated calc of var_v1
    if ((self->srt.yaw - (arg1 & 0xFFFF)) > 0x8000) {
        var_v1 += 0xFFFF0001;
    }
    if (var_v1 < -0x8000) {
        var_v1 += 0xFFFF;
    }
    temp_a2 = var_v1 >> 8;
    if (temp_a2 > 0x10) {
        self->srt.yaw -= 0x1000;
        return;
    }
    if (temp_a2 < -0x10) {
        self->srt.yaw += 0x1000;
        return;
    }
    self->srt.yaw = arg1;
}

// offset: 0x8974 | func: 74
static void dll_211_func_8974(Object* self, UnkCurvesStruct* arg1, f32 arg2) {
    f32 square;
    f32 var_fs0;
    f32 distanceSquared;
    s32 i;
    
    distanceSquared = (arg2 * gUpdateRateF) * 1.5f;
    square = distanceSquared * distanceSquared;
    distanceSquared = vec3_distance_xz_squared((Vec3f *) (&arg1->unk0.unk68), &self->srt.transl);
    
    if (arg1->unk0.unk80 != 0){
        var_fs0 = -2.0f;
    } else {
        var_fs0 = 2.0f;
    }
    
    for (i = 0; i < 5; i++){
        if (square < distanceSquared){
            break;
        }
        curves_func_800053B0(&arg1->unk0, var_fs0);
        distanceSquared = vec3_distance_xz_squared((Vec3f *) (&arg1->unk0.unk68), &self->srt.transl);
    }
}

// offset: 0x8A94 | func: 75
static void dll_211_func_8A94(Object* self, UnkCurvesStruct* arg1) {
    if (arg1->unk0.unk80 != 0) {
        while (arg1->unk0.unk10 != 0) {
            curves_func_800053B0(&arg1->unk0, -2.0f);
        }
    } else{
        while (arg1->unk0.unk10 == 0) {
            curves_func_800053B0(&arg1->unk0, 2.0f);
        }
    }
}

// offset: 0x8B5C | func: 76
static void dll_211_func_8B5C(Object* self, s32 arg1) {
    DLL211_Data* objData;

    objData = self->data;
    objData->unk4C &= ~1;
    objData->unk4C |= 0x4000;
    dll_211_func_83BC(self, objData);
    gDLL_3_Animation->vtbl->start_obj_sequence(arg1, self, -1);
}

// offset: 0x8BEC | func: 77
static void dll_211_func_8BEC(Object* self) {
    DLL211_Data* objData;
    f32 sp38;
    Object* sp34;

    objData = self->data;
    if (map_world_coords_to_block_index(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z) == -1) {
        objData->unkCC.mode = 0;
        if (objData->unk28 != NULL) {
            self->srt.transl.y = objData->unk28->srt.transl.y;
        }
    }
    if (objData->unk4C & 0x2000) {
        func_80058680(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &sp38, 0);
        self->srt.transl.y -= sp38;
        objData->unkCC.mode = 0;
    }
    if (objData->unkCC.mode != 0) {
        self->velocity.y += -0.17f * gUpdateRateF;
        self->srt.transl.y += self->velocity.y * gUpdateRateF;
    } else {
        self->velocity.y = 0.0f;
    }
    if (self->objhitInfo->unk58 & 8) {
        sp34 = self->objhitInfo->unk0;
    } else {
        sp34 = NULL;
    }
    if (objData->unk4C & 8) {
        objData->unk330 += gUpdateRateF;
        if (objData->unk330 >= 40.0f) {
            objData->unk330 -= 40.0f;
            func_8002674C(self);
            objData->unk4C &= ~8;
        }
    } else {
        if ((objData->unk32C != NULL) && (sp34 == objData->unk32C)) {
            objData->unk330 += gUpdateRateF;
            if (objData->unk330 >= 20.0f) {
                objData->unk330 -= 20.0f;
                objData->unk4C |= 8;
                func_800267A4(self);
            }
        } else {
            objData->unk330 = 0.0f;
        }
    }
    objData->unk32C = sp34;
    objData->unk334 = func_80025F40(self, &objData->unk338, NULL, NULL);
    gDLL_27->vtbl->func_1E8(self, &objData->unkCC, gUpdateRateF);
    gDLL_27->vtbl->func_5A8(self, &objData->unkCC);
    gDLL_27->vtbl->func_624(self, &objData->unkCC, gUpdateRateF);
    self->srt.pitch = objData->unkCC.relativeFloorPitchSmooth;
    self->srt.roll = objData->unkCC.relativeFloorRollSmooth;
}


// offset: 0x8ED0 | func: 78
static void dll_211_func_8ED0(f32 arg0, f32 arg1, f32* arg2) {
    *arg2 = (0.02f * gUpdateRateF) + arg0;
    if (arg1 < *arg2) {
        *arg2 = arg1;
    }
}

// offset: 0x8F18 | func: 79
static void dll_211_func_8F18(DLL211_Data* objData) {
    Object* temp_v0;

    temp_v0 = obj_get_nearest_type_to(OBJTYPE_TrickyTarget, objData->unk8, NULL);
    objData->unk28 = temp_v0;
    dll_211_func_9024(objData, &temp_v0->globalPosition);
    objData->unk1A = 0;
}

// offset: 0x8F84 | func: 80
// arg2 might be a Vec3f as well
static void dll_211_func_8F84(Object* self, Vec3f* arg1, f32* arg2) {
    f32 temp_fv0;

    arg2[0] = arg1->x - self->srt.transl.x;
    arg2[1] = arg1->z - self->srt.transl.z;
    temp_fv0 = sqrtf(SQ(arg2[0]) + SQ(arg2[1]));
    if (temp_fv0 != 0.0f) {
        arg2[0] /= temp_fv0;
        arg2[1] /= temp_fv0;
    }
}

// offset: 0x9024 | func: 81
static void dll_211_func_9024(DLL211_Data* objData, Vec3f *arg1) {
    if (arg1 != objData->unk2C) {
        objData->unk2C = arg1;
        objData->unkA6 = 0;
        objData->unk4C &= ~0x400;
    }
}

// offset: 0x9050 | func: 82
static void dll_211_func_9050(Object* self, DLL211_Data* objData) {
    FlameBlast_Setup* temp_v0;
    FXEmit_Setup* temp_v0_2;
    s32 i;

    objData->unk4C |= 0x800;
    objData->unk0 = dllLoadDeferred(DLL_ID_178, 1);
    for (i = 0; i < 3; i++) {
        temp_v0 = obj_alloc_setup(sizeof(FlameBlast_Setup), OBJ_flameblast);
        temp_v0->base.x = self->srt.transl.x;
        temp_v0->base.y = self->srt.transl.y;
        temp_v0->base.z = self->srt.transl.z;
        temp_v0->base.loadFlags = 2;
        temp_v0->base.byte5 = 1;
        temp_v0->timer = i * 10;
        objData->unk5E4[i] = obj_create(&temp_v0->base, 5, self->mapID, -1, self->parent);
    }
    temp_v0_2 = obj_alloc_setup(sizeof(FXEmit_Setup), OBJ_FXEmit);
    temp_v0_2->base.loadFlags = 2;
    temp_v0_2->base.byte5 = 1;
    temp_v0_2->base.x = self->srt.transl.x;
    temp_v0_2->base.y = self->srt.transl.y;
    temp_v0_2->base.z = self->srt.transl.z;
    temp_v0_2->toggleGamebit = -1;
    temp_v0_2->disableGamebit = -1;
    temp_v0_2->yaw = 0;
    temp_v0_2->flagConfig = 1;
    temp_v0_2->pitch = 0;
    temp_v0_2->roll = 0;
    temp_v0_2->rollSpeed = 0;
    temp_v0_2->pitchSpeed = 0;
    temp_v0_2->yawSpeed = 0;
    temp_v0_2->activationRange = 0;
    temp_v0_2->bank = 1;
    temp_v0_2->indexInBank = 0x4A;
    temp_v0_2->fxRate = -0x1E;
    objData->unk5F0 = obj_create(&temp_v0_2->base, 5, self->mapID, -1, self->parent);
}

// offset: 0x9200 | func: 83
static void dll_211_func_9200(Object* self, DLL211_Data* objData) {
    SRT sp40;
    u8 temp_v0;

    if (objData->unk4C & 0x1800) {
        sp40.transl.x = objData->unk3B4.x - self->srt.transl.x;
        sp40.transl.y = objData->unk3B4.y - self->srt.transl.y;
        sp40.transl.z = objData->unk3B4.z - self->srt.transl.z;
        sp40.scale = 1.0f;
        sp40.yaw = self->srt.yaw;
        sp40.pitch = self->srt.pitch;
        sp40.roll = self->srt.roll;
        if (objData->unk4C & 0x800) {
            objData->unk5F0->srt.transl.x = objData->unk3B4.x;
            objData->unk5F0->srt.transl.y = objData->unk3B4.y;
            objData->unk5F0->srt.transl.z = objData->unk3B4.z;
            objData->unk5F0->srt.roll = self->srt.roll;
            objData->unk5F0->srt.pitch = self->srt.pitch;
            objData->unk5F0->srt.yaw = self->srt.yaw;
            gDLL_17_partfx->vtbl->spawn(self, 0x535, &sp40, 2, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, 0x532, &sp40, 2, -1, NULL);
            return;
        }
        dll_211_func_95E0(self, objData, _data_0[2]);
        temp_v0 = 20;
        // FAKE
        do {} while (0);
        while (temp_v0--) {
            gDLL_17_partfx->vtbl->spawn(self, 0x533, &sp40, 2, -1, NULL);
        }
        objData->unk4C &= ~0x1000;
    }
}

// offset: 0x940C | func: 84
static void dll_211_func_940C(Object* self, DLL211_Data* objData) {
    s32 i;

    objData->unk4C &= ~0x800;
    objData->unk4C |= 0x1000;
    dllFree(objData->unk0);
    obj_destroy_object(objData->unk5F0);
    for (i = 0; i < 3; i++) { obj_destroy_object(objData->unk5E4[i]); }
}

// offset: 0x94BC | func: 85
static Object* dll_211_func_94BC(Object* self, f32 arg1) {
    Object* var_s3;
    Object** temp_v0;
    f32 temp_fv0;
    s32 sp48;
    s32 i;

    var_s3 = NULL;
    temp_v0 = obj_get_all_of_type(OBJTYPE_Baddie, &sp48);
    arg1 = SQ(arg1);
    for (i = 0; i < sp48; i++) {
        if ((obj_is_object_type(temp_v0[i], OBJTYPE_TrickyTarget) == 0) && (gDLL_33_BaddieControl->vtbl->get_health_ratio(temp_v0[i]) > 0.0f)) {
            temp_fv0 = vec3_distance_squared(&self->globalPosition, &temp_v0[i]->globalPosition);
            if (temp_fv0 < arg1) {
                arg1 = temp_fv0;
                var_s3 = temp_v0[i];
            }
        }
    }
    return var_s3;
}

// offset: 0x95E0 | func: 86
static void dll_211_func_95E0(Object* self, DLL211_Data* objData, s16* arg2) {
    if (arg2[2] != 0) {
        func_800349C0(self, &objData->unk360, arg2[0], arg2[1], arg2[2], 1);
    } else {
        func_80034B54(self, &objData->unk360, arg2, 1);
    }
}

// offset: 0x9668 | func: 87
static s32 dll_211_func_9668(DLL27_Data* objData) {
    if (objData->underwaterDist == 0.0f) {
        return 0;
    }

    if (objData->floorY == -100000.0f) {
        return 1;
    }

    if ((objData->underwaterDist - objData->floorY) > 5.0f) {
        return 1;
    }

    return 0;
}
