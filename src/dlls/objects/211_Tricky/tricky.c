#include "common.h"

#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/280_sidekicktoy.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/setups/118_FXEmit.h"
#include "game/objects/setups/51_Generic.h"
#include "game/objects/setups/78_GroundAnimator.h"
#include "sys/objexpr.h"
#include "sys/objtype.h"
#include "sys/objlib.h"
#include "sys/route.h"

#include "prevent_bss_reordering.h"

typedef s32 (*DLL210_Unk5FC_Callback)(Object* , s32, void *, Object* );

// size: 0x610
typedef struct {
    /*000*/ void *unk0; // holds a pointer to a (loaded) DLL
    /*004*/ SidekickStats *stats;
    /*008*/ Object* player;
    /*00C*/ s32 unkC;
    /*010*/ s32 unk10;
    /*014*/ s32 unk14;
    /*018*/ u8 state; // TrickyState
    /*019*/ u8 unk19;
    /*01A*/ u8 unk1A; // some sort of state?
    /*01B*/ u8 unk1B; //bitfield of available sidekick commands
    /*01C*/ u8 unk1C;
    /*01D*/ u8 unk1D;
    /*01E*/ u16 pad1E;
    /*020*/ f32 unk20;
    /*024*/ f32 unk24;
    /*028*/ Object* unk28;
    /*02C*/ Vec3f *unk2C;
    /*030*/ f32 unk30[2];
    /*038*/ f32 unk38;
    /*03C*/ f32 unk3C;
    /*040*/ f32 unk40;
    /*044*/ f32 unk44;
    /*048*/ f32 unk48;
    /*04C*/ s32 flags; // TrickyFlags
    /*050*/ f32 unk50;
    /*054*/ f32 unk54;
    /*058*/ f32 unk58;
    /*05C*/ f32 unk5C;
    /*060*/ f32 unk60;
    /*064*/ s16 unk64;
    /*066*/ s16 unk66;
    /*068*/ Vec3f *unk68;
    /*06C*/ s16 unk6C[4];
    /*074*/ Vec3f unk74[4];
    /*0A4*/ u16 unkA4;
    /*0A6*/ s16 unkA6;
    /*0A8*/ Vec3f unkA8;
    /*0B4*/ Vec3f unkB4;
    /*0C0*/ Vec3f unkC0;
    /*0CC*/ DLL27_Data unkCC;
    /*32C*/ Object* unk32C;
    /*330*/ f32 unk330;
    /*334*/ s32 hitByDamageType;
    /*338*/ Object* hitBy;
    /*33C*/ HeadAnimation unk33C;
    /*360*/ HeadAnimation unk360;
    /*384*/ Vec3f unk384[4];
    /*3B4*/ Vec3f unk3B4;
    /*3C0*/ CurveSetup *unk3C0;
    /*3C4*/ u8 unk3C4;
    /*3C8*/ UnkCurvesStruct unk3C8;
    /*4D0*/ CurveSetup *unk4D0;
    /*4D4*/ CurveSetup *unk4D4;
    /*4D8*/ u16 unk4D8;
    /*4DA*/ u8 unk4DA;
    /*4DB*/ u8 pad4DB;
    /*4DC*/ Route unk4DC[4];
    /*59C*/ Route unk59C;
    /*5CC*/ CurveSetup *unk5CC;
    /*5D0*/ void *unk5D0;
    /*5D4*/ Vec3f *unk5D4;
    /*5D8*/ Vec3f unk5D8;

    // unk5E4 .. unk600 are dynamic based on something?
    /*5E4*/ Object* unk5E4[3]; // objects or floats or curve setups or object setups?
    /*5F0*/ union {
        // sometimes float or u32 as well
        Object* unk5F0;
        f32 unk5F0_f32;
        u32 unk5F0_u32;
    };
    /*5F4*/ union {
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
    /*600*/ f32 unk600;
    /*604*/ Object* unk604;
    /*608*/ s32 unk608;
    /*60C*/ u8 unk60C;
} Tricky_Data;

typedef void (*TrickyStateCallback)(Object* self, Tricky_Data* objdata);

enum TrickyState {
    TRICKY_STATE_0 = 0,
    TRICKY_STATE_1 = 1,
    TRICKY_STATE_2 = 2,
    TRICKY_STATE_3 = 3,
    TRICKY_STATE_4 = 4,
    TRICKY_STATE_5 = 5,
    TRICKY_STATE_6 = 6,
    TRICKY_STATE_7 = 7,
    TRICKY_STATE_8 = 8,
    TRICKY_STATE_9 = 9,
    TRICKY_STATE_10 = 10,
    TRICKY_STATE_11 = 11,
    TRICKY_STATE_12 = 12,
    TRICKY_STATE_13 = 13,
    TRICKY_STATE_14 = 14,
    TRICKY_STATE_15 = 15,
    TRICKY_STATE_16 = 16,
    TRICKY_STATE_17 = 17
};

enum TrickyFlags {
    TRICKY_FLAG_UNK1 = 0x1,
    TRICKY_FLAG_UNK2 = 0x2,
    TRICKY_FLAG_UNK4 = 0x4,
    TRICKY_FLAG_UNK8 = 0x8,
    TRICKY_FLAG_UNK10 = 0x10,
    TRICKY_FLAG_UNK20 = 0x20,
    TRICKY_FLAG_UNK40 = 0x40,
    TRICKY_FLAG_UNK80 = 0x80,
    TRICKY_FLAG_UNK100 = 0x100,
    TRICKY_FLAG_UNK200 = 0x200,
    TRICKY_FLAG_UNK400 = 0x400,
    TRICKY_FLAG_EmittingFlames = 0x800,
    TRICKY_FLAG_UNK1000 = 0x1000,
    TRICKY_FLAG_UNK2000 = 0x2000,
    TRICKY_FLAG_UNK4000 = 0x4000,
};

static void tricky_func_1408(Object* self, Object* arg1);
static int tricky_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3);
static void tricky_state_0(Object* self, Tricky_Data* objData);
static void tricky_state_1(Object* self, Tricky_Data* objData);
static void tricky_state_2(Object* self, Tricky_Data* objData);
static void tricky_state_3(Object* self, Tricky_Data* objData);
static void tricky_state_4(Object* self, Tricky_Data* objData);
static void tricky_state_5(Object* self, Tricky_Data* objData);
static void tricky_state_6(Object* self, Tricky_Data* objData);
static void tricky_state_7(Object* self, Tricky_Data* objData);
static void tricky_state_8(Object* self, Tricky_Data* objData);
static void tricky_state_9(Object* self, Tricky_Data* objData);
static void tricky_state_10(Object* self, Tricky_Data* objData);
static void tricky_state_11(Object* self, Tricky_Data* objData);
static void tricky_state_12(Object* self, Tricky_Data* objData);
static void tricky_state_13(Object* self, Tricky_Data* objData);
static void tricky_state_14(Object* self, Tricky_Data* objData);
static void tricky_state_15(Object* self, Tricky_Data* objData);
static void tricky_state_16(Object* self, Tricky_Data* objData);
static void tricky_state_17(Object* self, Tricky_Data* objData);
static s32 tricky_func_4F3C(Object* self, f32 arg1, Tricky_Data* arg2, Vec3f* arg3);
static void tricky_func_514C(Object* self, Tricky_Data* objData);
static void tricky_func_52B8(Object* self, Tricky_Data* objData);
static s32 tricky_func_53E4(Object* self, f32 arg1, Tricky_Data* arg2);
static void tricky_func_7188(Object* self);
static void tricky_func_726C(Object* self);
static void tricky_func_74C4(Object* self, Gfx** arg1, Mtx** arg2, Vtx** arg3, Triangle** pols);
static void tricky_func_7794(Object* self, u8 *arg1, s16 arg2, CurveSetup** arg3);
static CurveSetup* tricky_func_7A7C(Tricky_Data* objData, CurveSetup* arg1, s32 arg2, u8 arg3);
static s8 tricky_func_7BAC(Tricky_Data* objData, CurveSetup** arg1, u8 *arg2, void* arg3);
static CurveSetup* tricky_func_7DB8(Tricky_Data* objData, CurveSetup* arg1, void* arg2);
static CurveSetup* tricky_func_7EFC(Tricky_Data* objData, CurveSetup* arg1, s32 arg2, s32 arg3);
static CurveSetup* tricky_func_8114(CurveSetup* setup);
static void tricky_func_81A8(Object* self);
static f32 tricky_func_81D8(Object* self);
static void tricky_func_82B8(Tricky_Data* objData);
static void tricky_func_8308(Object* self, s32 modAnim);
static void tricky_func_83BC(Object* self, Tricky_Data* objData);
static void tricky_func_8470(Object* self, Vec3f* arg1);
static void tricky_func_871C(Object* self, s8* arg1);
static void tricky_func_87E4(Object* self);
static void tricky_func_885C(Object* self);
static void tricky_func_88F4(Object* self, s16 arg1);
static void tricky_func_8974(Object* self, UnkCurvesStruct* arg1, f32 arg2);
static void tricky_func_8A94(Object* self, UnkCurvesStruct* arg1);
static void tricky_func_8B5C(Object* self, s32 arg1);
static void tricky_func_8BEC(Object* self);
static void tricky_func_8ED0(f32 arg0, f32 arg1, f32* arg2);
static void tricky_func_8F18(Tricky_Data* objData);
static void tricky_func_8F84(Object* self, Vec3f* arg1, f32* arg2);
static void tricky_func_9024(Tricky_Data* objData, Vec3f *arg1);
static void tricky_func_9050(Object* self, Tricky_Data* objData);
static void tricky_func_9200(Object* self, Tricky_Data* objData);
static void tricky_func_940C(Object* self, Tricky_Data* objData);
static Object* tricky_func_94BC(Object* self, f32 arg1);
static void tricky_func_95E0(Object* self, Tricky_Data* objData, s16* arg2);
static s32 tricky_func_9668(DLL27_Data* objData);

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
/*0x94*/ static Vec3f _data_94[] = { VEC3F(0.0f, 0.0f, 0.0f) };
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

/*0x0*/ static u8 _bss_0[0x8];
/*0x8*/ static TrickyStateCallback sStateFuncs[18];

// offset: 0x0 | ctor
void tricky_ctor(void* dll) {
    sStateFuncs[TRICKY_STATE_0] = tricky_state_0; // talk?
    sStateFuncs[TRICKY_STATE_1] = tricky_state_1; // eat?
    sStateFuncs[TRICKY_STATE_2] = tricky_state_2; // dig?
    sStateFuncs[TRICKY_STATE_3] = tricky_state_3; // dig + curves?
    sStateFuncs[TRICKY_STATE_4] = tricky_state_4; // empty
    sStateFuncs[TRICKY_STATE_5] = tricky_state_5; // ??
    sStateFuncs[TRICKY_STATE_6] = tricky_state_6; // something obj seq
    sStateFuncs[TRICKY_STATE_7] = tricky_state_7; // ??
    sStateFuncs[TRICKY_STATE_8] = tricky_state_8; // ??
    sStateFuncs[TRICKY_STATE_9] = tricky_state_9; // empty
    sStateFuncs[TRICKY_STATE_10] = tricky_state_10; // ??
    sStateFuncs[TRICKY_STATE_11] = tricky_state_11; // ??
    sStateFuncs[TRICKY_STATE_12] = tricky_state_12; // something related to water?
    sStateFuncs[TRICKY_STATE_13] = tricky_state_13; // something related to water?
    sStateFuncs[TRICKY_STATE_14] = tricky_state_14; // ??
    sStateFuncs[TRICKY_STATE_15] = tricky_state_15; // ??
    sStateFuncs[TRICKY_STATE_16] = tricky_state_16; // ??
    sStateFuncs[TRICKY_STATE_17] = tricky_state_17; // ??
}

// offset: 0xF8 | dtor
void tricky_dtor(void *dll) { }

// offset: 0x104 | func: 0 | export: 0
void tricky_obj_Setup(Object* self, ObjSetup* setup, s32 arg2) {
    f32 var_ft5;
    f32 var_fv0;
    f32 var_fv1;
    Tricky_Data* objData = self->data;
    s32 pad;
    s32 pad2;
    s32 i;
    u8 pad3;
    u8 sp50[] = {1, 2};
    s32 pad4;
    s32 temp_v1;

    mainSetBits(BIT_4E3, 0xFF);
    if (mainGetBits(BIT_SW_Tricky_Ball_Collected) != 0) {
        mainSetBits(BIT_Tricky_Ball_Unlocked, 1);
    }

    self->animCallback = tricky_animCallback;
    objAddObjectType(self, OBJTYPE_Sidekick);
    routeInit(&objData->unk4DC[0]);
    routeInit(&objData->unk4DC[1]);
    routeInit(&objData->unk4DC[2]);
    routeInit(&objData->unk4DC[3]);
    routeInit(&objData->unk59C);
    objData->stats = gDLL_29_Gplay->vtbl->get_sidekick_stats();
    objData->player = objGetPlayer();
    temp_v1 = _data_EC[map_func_80045D58()];
    objData->state = TRICKY_STATE_0;
    objData->unk1B = 0;
    objData->unk5D4 = 0;
    objData->unkA4 = 0;
    objData->unk14 = temp_v1;
    objData->unk10 = temp_v1;
    objData->unkC = temp_v1;
    gDLL_27->vtbl->init(&objData->unkCC, 
        DLL27FLAG_2000000, 
        DLL27FLAG_80 | DLL27FLAG_20 | DLL27FLAG_4 | DLL27FLAG_2 | DLL27FLAG_1, 
        DLL27MODE_1);
    gDLL_27->vtbl->setup_hits_collider(&objData->unkCC, 1, _data_94, &_data_A0, 2);
    gDLL_27->vtbl->setup_terrain_collider(&objData->unkCC, 2, _data_74, _data_8C, sp50);
    gDLL_25->vtbl->LoadWalkGroups();
    for (i = 0, var_ft5 = 0/*.0f*/; var_ft5 < 1.8f; i++, var_ft5 += 0.1f) {
        for (var_fv0 = var_ft5, var_fv1 = 0.0f; var_fv0 > 0.0f; var_fv0 -= 0.09f) {
            var_fv1 += var_fv0 * 3.0f;
        }
        _data_A4[i] = var_fv1 * 1.75f;
    }
}

// offset: 0x3FC | func: 1 | export: 1
void tricky_obj_Control(Object* self) {
    s32 usedSideCmd;
    s32 sp90;
    ObjSetup* temp_v0_2;
    Tricky_Data* objData = self->data;
    f32 sp84;
    UnkFunc_80024108Struct sp68;
    s32 cmds[] = { 
        Sidekick_Command_INDEX_0_Heel,
        Sidekick_Command_INDEX_1_Find,
        Sidekick_Command_INDEX_2_Distract,
        Sidekick_Command_INDEX_3_Guard,
        Sidekick_Command_INDEX_4_Flame,
        Sidekick_Command_INDEX_5_Play
    };
    Object* sp4C;
    Object* temp_v0_3;

    // FAKE
    if ((s32)self){}
    sp90 = _data_EC[map_func_80045D58()];
    objData->unk14 = _data_EC[mapWorldXZToMapID(objData->player->globalPosition.x, objData->player->globalPosition.z)];
    if ((sp90 != objData->unkC) && (sp90 != objData->unk10)) {
        if (objData->unkC == objData->unk14) {
            objData->unk10 = sp90;
        } else {
            objData->unkC = sp90;
        }
        gDLL_25->vtbl->LoadWalkGroups();
    }
    if (objData->flags & TRICKY_FLAG_UNK200) {
        if (!(objData->flags & TRICKY_FLAG_UNK4000)) {
            gDLL_27->vtbl->reset(self, &objData->unkCC);
            tricky_func_82B8(objData);
            objData->unk19 = 0;
            objData->unk20 = 0.0f;
        }
        objData->flags &= ~(TRICKY_FLAG_UNK4000 | TRICKY_FLAG_UNK200);
    }
    if (objData->unk28 != NULL && (objData->unk28->stateFlags & OBJSTATE_DESTROYED)) {
        tricky_func_82B8(objData);
        objData->unk28 = NULL;
    }
    usedSideCmd = gDLL_1_cmdmenu->vtbl->was_used_item_in_gamebit_array(cmds, ARRAYCOUNT(cmds));
    if ((objData->state == TRICKY_STATE_11) && (usedSideCmd == Sidekick_Command_INDEX_4_Flame)) {
        objData->unk60C ^= 1;
    } else if ((objData->state == TRICKY_STATE_16) && (usedSideCmd == Sidekick_Command_INDEX_4_Flame)) {
        objData->unk5F8_s32 = 1;
    } else if ((objData->state == TRICKY_STATE_17) && (usedSideCmd == Sidekick_Command_INDEX_4_Flame)) {
        objData->unk5F8_s32 = 1;
    } else if (!(objData->flags & TRICKY_FLAG_UNK10)) {
        switch (usedSideCmd) {
        case Sidekick_Command_INDEX_0_Heel:
            tricky_func_82B8(objData);
            objData->unk5F4_vec.x = 600.0f;
            break;
        case Sidekick_Command_INDEX_1_Find:
            objData->unk1D = 1;
            tricky_func_8F18(objData);
            dll_amSfx->Play(self, mathRnd(50, 51), MAX_VOLUME, NULL, NULL, 0, NULL);
            switch (objData->unk28->id) {
            case OBJ_GroundAnimator:
                objData->state = TRICKY_STATE_2;
                temp_v0_2 = objData->unk28->setup;
                objData->unk5F8_s32 = ((GroundAnimator_Setup*)temp_v0_2)->sidekickExperience;
                objData->unk5FC_s32 = ((GroundAnimator_Setup*)temp_v0_2)->sidekickCost;
                break;
            case OBJ_WallAnimator:
                objData->state = TRICKY_STATE_3;
                break;
            case OBJ_TrickyWarp:
                objData->state = TRICKY_STATE_8;
                break;
            case OBJ_SHbluemushroom:
            case OBJ_Tumbleweed2:
                objData->state = TRICKY_STATE_12;
                break;
            case OBJ_DBDustGeezer:
                objData->state = TRICKY_STATE_2;
                objData->unk5F8_s32 = 0;
                objData->unk5FC_s32 = 1;
                break;
            case OBJ_SandGrabber:
                objData->state = TRICKY_STATE_17;
                break;
            default:
                tricky_func_82B8(objData);
                break;
            }
            break;
        case Sidekick_Command_INDEX_2_Distract:
            objData->unk1D = 2;
            sp84 = M_INFINITY_F;
            sp4C = objGetNearestTypeTo(OBJTYPE_Baddie, self, &sp84);
            temp_v0_3 = objGetNearestTypeTo(OBJTYPE_40, self, &sp84);
            if (temp_v0_3 != NULL) {
                tricky_func_1408(self, temp_v0_3);
            } else {
                tricky_func_1408(self, sp4C);
            }
            break;
        case Sidekick_Command_INDEX_3_Guard:
            objData->unk1D = 3;
            tricky_func_8F18(objData);
            switch (objData->unk28->id) {
            case OBJ_DR_PressurePad:
                objData->state = TRICKY_STATE_13;
                break;
            case OBJ_VFP_PuzzlePad:
                objData->state = TRICKY_STATE_13;
                break;
            default:
                objData->state = TRICKY_STATE_11;
            }
            break;
        case Sidekick_Command_INDEX_4_Flame:
            objData->unk1D = 4;
            tricky_func_8F18(objData);
            objData->state = TRICKY_STATE_10;
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
            case OBJ_DR_Chimmey:
                objData->unk5FC = (DLL210_Unk5FC_Callback) ((DLL_Unknown*)objData->unk28->dll)->vtbl->func[7].withFourArgsS32;
                break;
            case OBJ_SHvines:
            case OBJ_DR_Vines:
            case OBJ_DB_FlameSwitch:
                objData->unk5FC = 0;
                break;
            default:
                tricky_func_82B8(objData);
                break;
            }
            break;
        case Sidekick_Command_INDEX_5_Play:
            objData->unk1D = 5;
            temp_v0_2 = objAllocSetup(sizeof(ObjSetup), OBJ_SidekickBall);
            temp_v0_2->fadeDistance = 0x96;
            temp_v0_2->loadFlags = 2;
            temp_v0_2->x = self->globalPosition.x;
            temp_v0_2->y = self->globalPosition.y;
            temp_v0_2->z = self->globalPosition.z;
            objData->unk28 = objSetupObject(temp_v0_2, OBJINIT_STANDALONE | OBJINIT_FLAG4, -1, -1, self->parent);
            tricky_func_9024(objData, &objData->unk28->globalPosition);
            objData->unk1A = 0;
            objData->state = TRICKY_STATE_14;
            break;
        default:
            if ((objData->state == TRICKY_STATE_1) && (objData->unk5F4_vec.x <= 0.0f)) {
                sp84 = 150.0f;
                if (tricky_func_94BC(objData->player, 150.0f) != NULL) {
                    objData->state = TRICKY_STATE_16;
                    objData->unk1A = 0;
                }
            }
            break;
        }
    } else if ((gDLL_1_cmdmenu->vtbl->was_this_item_used(Sidekick_Command_INDEX_0_Heel)) 
            && (objData->state == TRICKY_STATE_7)) {
        ((s16*)objData->unk5E4)[0] = 1;
    }
    self->unkAF |= 8;
    objData->unkCC.mode = 1;
    objData->unk24 += gUpdateRateF;
    sStateFuncs[objData->state](self, objData);
    objAnimAdvance(self, objData->unk38, gUpdateRateF, &sp68);
    if (objData->flags & TRICKY_FLAG_UNK40) {
        self->srt.transl.x += objData->unk30[0] * -sp68.unk0[2] * objData->unk40;
        self->srt.transl.z += objData->unk30[1] * -sp68.unk0[2] * objData->unk40;
    }
    if (objData->flags & TRICKY_FLAG_UNK80) {
        self->srt.transl.y += sp68.unk0[1] * objData->unk44;
    }
    if (objData->flags & TRICKY_FLAG_UNK20) {
        self->srt.transl.x += objData->unk30[1] * sp68.unk0[0] * objData->unk3C;
        self->srt.transl.z += objData->unk30[0] * -sp68.unk0[0] * objData->unk3C;
    }
    if (objData->flags & TRICKY_FLAG_UNK100) {
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
    objExpr_func_800328F0(self, &objData->unk33C, 0.0f);
    objExprEyeIdle(self, &objData->unk33C);
    objExpr_func_80034BC0(self, &objData->unk360);
    tricky_func_81A8(self);
    tricky_func_8BEC(self);
}

// offset: 0xD1C | func: 2 | export: 2
void tricky_obj_Update(Object* self) { }

// offset: 0xD28 | func: 3 | export: 3
void tricky_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Tricky_Data* objData;

    if (visibility == 0) {
        return;
    }

    objData = self->data;
    objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    tricky_func_7188(self);
    if (objData->flags & TRICKY_FLAG_UNK10) {
        switch (objData->state) {
        case TRICKY_STATE_2:
            tricky_func_726C(self);
            break;
        case TRICKY_STATE_3:
            if (objData->unk1A == 4) {
                tricky_func_726C(self);
            }
            break;
        case TRICKY_STATE_8:
            if ((objData->unk1A == 3) || (objData->unk1A == 2)) {
                tricky_func_74C4(self, gdl, mtxs, (Vtx** ) vtxs, pols);
                break;
            }
            if (objData->unk1A != 4) {
                break;
            }
            return;
        }

        if (objData->state == TRICKY_STATE_14) {
            if (objData->unk1A >= 3) {
                if (objData->unk1A != 3) {
                    objData->unk5E4[0]->srt.transl.x = objData->unk3B4.x;
                    objData->unk5E4[0]->srt.transl.y = objData->unk3B4.y;
                    objData->unk5E4[0]->srt.transl.z = objData->unk3B4.z;
                }
                objprintDrawModel(objData->unk5E4[0], gdl, mtxs, vtxs, pols, 1.0f);
            }
        }
    }

    tricky_func_9200(self, objData);
}

// offset: 0xF20 | func: 4 | export: 4
void tricky_obj_Free(Object* self, s32 a1) {
    Tricky_Data* temp_s0;

    temp_s0 = self->data;
    routeFree(&temp_s0->unk4DC[0]);
    routeFree(&temp_s0->unk4DC[1]);
    routeFree(&temp_s0->unk4DC[2]);
    routeFree(&temp_s0->unk4DC[3]);
    routeFree(&temp_s0->unk59C);
    objFreeObjectType(self, 1);
    gDLL_13_Expgfx->vtbl->func4(self);
    dll_modgfx->Func4(self);
    if (a1 == 0) {
        tricky_func_940C(self, temp_s0);
    }
}

// offset: 0x1040 | func: 5 | export: 5
u32 tricky_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1050 | func: 6 | export: 6
u32 tricky_obj_GetDataSize(Object* self, u32 a1) {
    return sizeof(Tricky_Data);
}

// offset: 0x1064 | func: 7 | export: 7
s32 tricky_sidekick_Func7(s32 arg0) {
    return 0;
}

// offset: 0x1074 | func: 8 | export: 8
void tricky_sidekick_Func8(Object* self, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2, UNK_TYPE_32 arg3) {
}

// offset: 0x108C | func: 9 | export: 9
void tricky_sidekick_Func9(Object* self, UNK_TYPE_32 arg1) {
}

// offset: 0x109C | func: 10 | export: 10
void tricky_sidekick_Func10(Object* self, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2, UNK_TYPE_32 arg3) {
}

// offset: 0x10B4 | func: 11 | export: 11
f32 tricky_sidekick_Func11(Object* self) {
    Tricky_Data* objData = self->data;
    return objData->unk20;
}

// offset: 0x10C4 | func: 12 | export: 12
DLL27_Data* tricky_sidekick_Func12(s32 arg0) {
    return NULL;
}

// offset: 0x10D4 | func: 13 | export: 13
s32 tricky_sidekick_GetAvailableCommands(Object* self) {
    Tricky_Data* objData;
    s32 sideCmdBits;

    objData = self->data;

    if (mainGetBits(BIT_Tricky_Unlocked_Sidekick_Commands)) {
        sideCmdBits = objData->unk1B | Sidekick_Command_FLAG_01_Heel;

        //Check if Flame command should be shown
        if ((objData->state == TRICKY_STATE_11) || 
            ((objData->state == TRICKY_STATE_16) && (objData->unk1A == 1)) || 
            ((objData->state == TRICKY_STATE_17) && (objData->unk1A == 1))
        ) {
            sideCmdBits |= Sidekick_Command_FLAG_10_Flame;
        }

        //Check if Tricky's ball is unlocked
        if (mainGetBits(BIT_Tricky_Ball_Unlocked)) {
            sideCmdBits |= Sidekick_Command_FLAG_20_Play;
        }

        //Check if Guard isn't unlocked
        if (mainGetBits(BIT_Tricky_Learned_Guard) == FALSE) {
            sideCmdBits &= ~Sidekick_Command_FLAG_08_Guard;
        }

        //Check if Distract isn't unlocked
        if (mainGetBits(BIT_Tricky_Learned_Distract) == FALSE) {
            sideCmdBits &= ~Sidekick_Command_FLAG_04_Distract;
        }

        //Check if Flame isn't unlocked
        if (mainGetBits(BIT_Tricky_Learned_Flame) == FALSE) {
            sideCmdBits &= ~Sidekick_Command_FLAG_10_Flame;
        }
        
        objData->unk1B = 0;
        
        return sideCmdBits;
    }
    
    return NO_SIDEKICK_COMMAND;
}

// offset: 0x1230 | func: 14 | export: 26
int tricky_sidekick_Func26(Object* self, s32* arg1) {
    Tricky_Data* objData = self->data;
    *arg1 = objData->unk1D;
    return TRUE;
}

/*0x0*/ static const char str_0[] = "%d %d %d %d\n";
/*0x10*/ static const char str_10[] = "hits: %d %d %d %d %d %d %d %d";
/*0x30*/ static const char str_30[] = "\nEnergy: %d, Skill %d\n";
/*0x48*/ static const char str_48[] = "find command used on the wrong object\n";
/*0x70*/ static const char str_70[] = "find command used on the wrong object\n";
/*0x98*/ static const char str_98[] = "sideCommandEnable error, command number too high\n";

/**
 * Enables a sidekick command (e.g. allowing Flame when near vines).
 */
// offset: 0x1248 | func: 15 | export: 14
void tricky_sidekick_EnableCommand(Object* self, s32 commandIndex) {
    Tricky_Data* objData = self->data;
    
    if (commandIndex < TOTAL_SIDEKICK_COMMANDS) {
        objData->unk1B |= (1 << commandIndex);
    }
}

// offset: 0x1270 | func: 16 | export: 15
u8 tricky_sidekick_GetBlueFoodCount(Object* self) {
    Tricky_Data* objData;

    objData = self->data;
    return objData->stats->blueFood;
}

// offset: 0x1284 | func: 17 | export: 16
u8 tricky_sidekick_GetRedFoodCount(Object* self) {
    Tricky_Data* objData;

    objData = self->data;
    return objData->stats->redFood;
}

// offset: 0x1298 | func: 18 | export: 17
s32 tricky_sidekick_GetBlueAndRedFoodCount(Object* self) {
    Tricky_Data* objData;

    objData = self->data;
    return objData->stats->blueFood + objData->stats->redFood;
}

// offset: 0x12B4 | func: 19 | export: 18
void tricky_sidekick_SetRedFoodCount(Object* self, s32 count) {
    Tricky_Data* objData = self->data;
    objData->stats->redFood = count;
}

// offset: 0x12C8 | func: 20 | export: 19
s32 tricky_sidekick_Func19(Object* self) {
    return 1;
}

// offset: 0x12D8 | func: 21 | export: 20
void tricky_sidekick_Func20(Object* self, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2, UNK_TYPE_32 arg3) {
}

// offset: 0x12F0 | func: 22 | export: 21
void tricky_sidekick_Func21(Object* self, s32 arg1, Object* arg2) {
    Tricky_Data* objData;

    objData = self->data;
    if (arg1 != 0) {
        if (!(objData->flags & TRICKY_FLAG_UNK10)) {
            tricky_func_1408(self, arg2);
        }
    } else {
        if (objData->state >= TRICKY_STATE_5 && objData->state <= TRICKY_STATE_7) {
            if (objData->flags & TRICKY_FLAG_UNK10) {
                ((s16*)objData->unk5E4)[0] = 1;
                return;
            }
            tricky_func_82B8(objData);
        }
    }
}

// offset: 0x1398 | func: 23 | export: 22
void tricky_sidekick_Func22(Object* self, Object* arg1) {
    Tricky_Data* objData;

    objData = self->data;
    STUBBED_PRINTF("sideFind 1\n");
    if (!(objData->flags & TRICKY_FLAG_UNK10)) {
        STUBBED_PRINTF("sideFind 2\n");
        objData->unk28 = arg1;
        tricky_func_9024(objData, &arg1->globalPosition);
        objData->unk1A = 0;
        objData->state = TRICKY_STATE_13;
    }
}

// offset: 0x1408 | func: 24
static void tricky_func_1408(Object* self, Object* arg1) {
    Tricky_Data* objData;

    objData = self->data;
    objData->unk5E4[1] = gDLL_25->vtbl->FindNearestEnabledTrickyCurve(&arg1->srt.transl, -1, 3);
    if (objData->unk5E4[1] == NULL) {
        return;
    }

    ((s16*)objData->unk5E4)[0] = 0;
    if (((u8*)objData->unk5E4[1])[3] != 0) {
        if (((s32*)objData->unk5E4[1])[7] == -1) {
            objData->state = TRICKY_STATE_6;
        } else {
            objData->state = TRICKY_STATE_7;
        }
    } else {
        objData->state = TRICKY_STATE_5;
    }
    objData->unk28 = arg1;
    tricky_func_9024(objData, (Vec3f* ) &objData->unk5E4[1]->srt.scale);
    objData->unk1A = 0;
}

// offset: 0x14E8 | func: 25 | export: 24
s32 tricky_sidekick_Func24(Object* self) {
    Tricky_Data* objData = self->data;
    
    switch (objData->state) {
    case TRICKY_STATE_5:
    case TRICKY_STATE_6:
        if (objData->flags & TRICKY_FLAG_UNK200) {
            return 1;
        }
        break;
    case TRICKY_STATE_7:
        if (objData->flags & TRICKY_FLAG_UNK10) {
            return 1;
        }
        break;
    }

    return 0;
}

// offset: 0x1550 | func: 26 | export: 25
s32 tricky_sidekick_Func25(Object* self) {
    Tricky_Data* objData = self->data;

    if (objData->state == TRICKY_STATE_11) {
        return 1;
    }

    return 0;
}

// offset: 0x1578 | func: 27 | export: 23
void tricky_sidekick_Func23(Object* self) {
    Tricky_Data* objData;

    objData = self->data;
    if (!(objData->flags & TRICKY_FLAG_UNK10)) {
        tricky_func_82B8(objData);
        objData->unk5F4_vec.x = 600.0f;
    } else if (objData->state >= TRICKY_STATE_5 && objData->state <= TRICKY_STATE_7) {
        if (mainGetBits(BIT_Tricky_Unlocked_Sidekick_Commands) != 0) {
            ((s16*)objData->unk5E4)[0] = 1;
        }
    }
}

// offset: 0x1624 | func: 28
static int tricky_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    Tricky_Data* objData;
    s32 i;

    objData = self->data;
    if (!(objData->flags & TRICKY_FLAG_UNK200)) {
        if (objData->flags & TRICKY_FLAG_EmittingFlames) {
            tricky_func_940C(self, objData);
        }
        objData->flags |= TRICKY_FLAG_UNK200;
        if (!(animObjData->unk7A & 3)) {
            objData->flags |= TRICKY_FLAG_UNK4000;
        }
    }

    if ((objData->flags & TRICKY_FLAG_UNK4000) && (objData->state == TRICKY_STATE_5 || objData->state == TRICKY_STATE_6)) {
        if (gDLL_1_cmdmenu->vtbl->was_this_item_used(Sidekick_Command_INDEX_0_Heel)) {
            (((s16*)objData->unk5E4))[0] = 1;
        }
        if (((((s16*)objData->unk5E4))[0] != 0) && (self->curModAnimId == 0)) {
            gDLL_3_Animation->vtbl->end_obj_sequence((s32) animObjData->seqSlot);
        }
    }

    for (i = 0; i < animObjData->messageCount; i++) {
        switch (animObjData->messages[i]) {
            case 1:
                if (objData->flags & TRICKY_FLAG_EmittingFlames) {
                    tricky_func_940C(self, objData);
                } else {
                    tricky_func_9050(self, objData);
                }
                break;
            default:
                break;
        }
    }

    if (objData->flags & TRICKY_FLAG_UNK1) {
        return 0;
    }

    return gDLL_3_Animation->vtbl->func29(self, animObjData, 2, 0, 0, 1, 1);
}

// offset: 0x183C | func: 29 | state: 0
static void tricky_state_0(Object* self, Tricky_Data* objData) {
    s32 walkGroup;

    walkGroup = gDLL_25->vtbl->FindWalkGroup(&self->srt.transl, NULL);
    if (walkGroup != 0) {
        mainSetBits(BIT_Tricky_Walk_Group, walkGroup);
        tricky_func_82B8(objData);
    }
}

// offset: 0x18C4 | func: 30 | state: 1
static void tricky_state_1(Object* self, Tricky_Data* objData) {
    u8 var_v0;
    u8 numPlayerBlueFood;
    s16 pad;
    s16 blueFoodBit;
    s16 cmdmenuCategory;
    Object* temp_v0_2;

    switch (objData->unk1A) {
    case 0:
        objData->unk28 = objData->player;
        tricky_func_9024(objData, &objData->unk28->globalPosition);
        objData->unk5F4_vec.x -= gUpdateRateF;
        if (objData->unk5F4_vec.x < 0.0f) {
            var_v0 = tricky_func_53E4(self, 50.0f, objData);
            objData->unk5F4_vec.x = 0.0f;
        } else {
            var_v0 = tricky_func_53E4(self, 25.0f, objData);
        }
        if (var_v0 != 1) {
            tricky_func_83BC(self, objData);
            if ((objData->hitByDamageType != 0) && (objData->hitBy->id == OBJ_sword)) {
                // slapped by sabre >:(
                objData->unk5F4_vec.y = 300.0f;
                tricky_func_95E0(self, objData, _data_0[mathRnd(10, 12)]);
                objData->unk1A = 1;
                objData->flags |= TRICKY_FLAG_UNK10;
                objAnimSet(self, 0x106, 0.0f, 0);
                break;
            }

            if (objData->stats->blueFood == 0) {
                objData->unk600 -= gUpdateRateF;
                if (objData->unk600 < 0.0f) {
                    tricky_func_95E0(self, objData, _data_0[mathRnd(13, 17)]);
                    objData->unk600 = 600.0f;
                }
            }

            temp_v0_2 = (Object*) ((DLL_210_Player*)objData->player->dll)->vtbl->func66(objData->player, 16);
            if (((DLL_Unknown *)temp_v0_2->dll)->vtbl->func[7].withOneArgS32((s32)temp_v0_2) != 0) {
                blueFoodBit = BIT_Dino_Bag_Blue_Mushrooms;
                cmdmenuCategory = CMDMENU_CATEGORY_6_Food;
            } else {
                blueFoodBit = BIT_Inventory_Blue_Mushrooms;
                cmdmenuCategory = CMDMENU_CATEGORY_3_Items;
            }

            numPlayerBlueFood = mainGetBits(blueFoodBit);
            if (numPlayerBlueFood && gDLL_1_cmdmenu->vtbl->get_page_category() == cmdmenuCategory) {
                if (self->unkAF & ARROW_FLAG_1_Interacted) {
                    if (gDLL_1_cmdmenu->vtbl->was_this_item_used(blueFoodBit) != 0) {
                        mainSetBits(BIT_Inventory_Blue_Mushrooms, numPlayerBlueFood - 1);
                        objData->stats->blueFood += 2;
                        tricky_func_8B5C(self, 2);
                        objData->flags |= TRICKY_FLAG_UNK1;
                    }
                } else {
                    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                }
            } else {
                var_v0 = mainGetBits(BIT_4E3);
                if (var_v0 != 0xFF) {
                    if (self->unkAF & ARROW_FLAG_1_Interacted) {
                        mainSetBits(BIT_4E3, 0xFF);
                        tricky_func_8B5C(self, var_v0);
                    } else {
                        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
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
            objData->flags &= ~TRICKY_FLAG_UNK10;
            tricky_func_83BC(self, objData);
        } else {
            tricky_func_885C(self);
            if (mathRnd(0, 10) == 0) {
                tricky_func_95E0(self, objData, _data_0[mathRnd(10, 12)]);
            }
            if (objData->hitByDamageType != 0 && objData->hitBy->id == OBJ_sword) {
                objData->unk5F4_vec.y = 300.0f;
            }
            if (objData->unk5F4_vec.z > 3000.0f) {
                objData->unk1A = 2;
                objAnimSet(self, 0x1A, 0.0f, 0);
                objData->unk38 = 0.004f;
            }
        }
        break;
    case 2:
        if ((self->animProgress > 0.25f) && !(objData->flags & TRICKY_FLAG_EmittingFlames)) {
            tricky_func_9050(self, objData);
        } else if (self->animProgress > 0.99f) {
            tricky_func_940C(self, objData);
            objData->unk5F4_vec.y = 300.0f;
            objData->unk5F4_vec.z = 0.0f;
            tricky_func_95E0(self, objData, _data_0[mathRnd(10, 12)]);
            objData->unk1A = 1;
            objData->flags |= TRICKY_FLAG_UNK10;
            objAnimSet(self, 0x106, 0.0f, 0);
        }
        break;
    }
}

// offset: 0x1F3C | func: 31 | state: 2
static void tricky_state_2(Object* self, Tricky_Data* objData) {
    f32 temp_fv0;
    CurveSetup* temp_v1_2;
    Object* sp34;
    f32 *temp;

    sp34 = objData->unk28;
    switch (objData->unk1A) {
    case 0:
        objData->unk5F4 = gDLL_25->vtbl->FindNearestEnabledTrickyCurve(&objData->unk28->srt.transl, -1, 2);
        objData->unk600 = 1.0f;
        if ((objData->unk5F4 != NULL) && (vec3DistanceXZSquared(&objData->unk28->globalPosition, &objData->unk5F4->pos) > 10000.0f)) {
            objData->unk5F4 = NULL;
            objData->unk600 = 2.0f;
        }
        temp_v1_2 = objData->unk5F4;
        if (temp_v1_2 != 0) {
            tricky_func_9024(objData, &temp_v1_2->pos);
        }
        objData->unk1A = 1;
        /* fallthrough */
    case 1:
        if (tricky_func_53E4(self, 5.0f, objData) == 0) {
            objData->flags |= TRICKY_FLAG_UNK10;
            objData->unk1A = 2;
            ((f32*)objData->unk5E4)[0] = 0.0f;
            ((u32*)objData->unk5E4)[1] = dll_amSfx->Play(self, SOUND_48_Dig_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
            objAnimSet(self, 14, 0.0f, 0);
            temp_v1_2 = objData->unk5F4;
            objData->unk38 = 0.033f;
            if (temp_v1_2 != 0) {
                tricky_func_8F84(objData->unk28, &temp_v1_2->pos, objData->unk30);
            }
        }
        return;
    case 2:
        ((f32*)objData->unk5E4)[0] += gUpdateRateF;
        tricky_func_87E4(self);
        if (((f32*)objData->unk5E4)[0] >= 60.0f) {
            if (objData->stats->unk2 < ((s32)objData->unk5F8)) {
                dll_amSfx->Stop((u32) objData->unk5E4[1]);
                tricky_func_95E0(self, objData, _data_0[1]);
                gDLL_22_Subtitles->vtbl->func_368(0xBD);
                tricky_func_82B8(objData);
            } else if (objData->stats->blueFood < ((s32)objData->unk5FC)) {
                dll_amSfx->Stop((u32) objData->unk5E4[1]);
                tricky_func_95E0(self, objData, _data_0[0]);
                gDLL_22_Subtitles->vtbl->func_368(0xBC);
                tricky_func_82B8(objData);
            } else {
                objData->unk1A = 3;
                temp_v1_2 = objData->unk5F4;
                ((f32*)objData->unk5E4)[2] = self->srt.transl.x;
                objData->unk5F0_f32 = self->srt.transl.z;
                if (temp_v1_2 != 0) {
                    tricky_func_8F84(objData->unk28, &temp_v1_2->pos, objData->unk30);
                }
            }
        }
        break;
    case 3:
        temp = objData->unk30;
        temp_fv0 = ((DLL_Unknown*)sp34->dll)->vtbl->func[7].withTwoArgsF32((s32)sp34, (s32)self);
        self->srt.transl.x = ((f32*)objData->unk5E4)[2] - (temp[0] * (temp_fv0 / objData->unk600));
        self->srt.transl.z = objData->unk5F0_f32 - (temp[1] * (temp_fv0 / objData->unk600));
        tricky_func_87E4(self);
        if (((DLL_Unknown*)sp34->dll)->vtbl->func[8].withOneArgS32((s32)sp34) != 0) {
            objData->stats->blueFood -= (s32)objData->unk5FC;
            objData->stats->unk2++;
            dll_amSfx->Stop((u32)objData->unk5E4[1]);
            tricky_func_82B8(objData);
        }
        break;
    }
}

// offset: 0x23B8 | func: 32 | state: 3
static void tricky_state_3(Object* self, Tricky_Data* objData) {
    Object* temp_a0;
    CurveSetup* sp38;
    CurveSetup* temp_t0;
    s32 sp30;
    CurveSetup* var_a1;
    f32 temp_fv0;
    s32 i;

    switch (objData->unk1A) {
    case 0:
        sp38 = gDLL_25->vtbl->FindNearestTrickyCurve(objData->unk2C, -1, 2);
        objData->unk5E4[2] = (Object*) gDLL_26_Curves->vtbl->FindByUID(sp38->links[0]);
        objData->unk5E4[0] = (Object*) sp38;
        objData->unk5E4[1] = (Object*) gDLL_26_Curves->vtbl->FindByUID(sp38->links[1]);
        if (((CurveSetup*)objData->unk5E4[1])->unk3 != 0) {
            objData->unk5E4[1] = (Object*)((s32)objData->unk5E4[2] ^ (s32)objData->unk5E4[1]);\
            objData->unk5E4[2] = (Object*)((s32)objData->unk5E4[1] ^ (s32)objData->unk5E4[2]);\
            objData->unk5E4[1] = (Object*)((s32)objData->unk5E4[2] ^ (s32)objData->unk5E4[1]);\
        }
        tricky_func_9024(objData, &((CurveSetup*)objData->unk5E4[2])->pos);
        objData->unk1A = 1;
        /* fallthrough */
    case 1:
        tricky_func_53E4(self, 5.0f, objData);
        if (gDLL_25->vtbl->FindWalkGroup(&self->srt.transl, NULL) == ((u8*)objData->unk5E4[2])[3]) {
            objData->unk19 = 1;
            objData->unk1A = 2;
            return;
        }
    default:
        return;
    case 2:
        if (tricky_func_4F3C(self, 2.5f, objData, (Vec3f* ) &objData->unk5E4[0]->srt.scale) == 0) {
            objData->unk20 = vec3DistanceXZ(&self->globalPosition, (Vec3f* ) &objData->unk5E4[0]->srt.scale) / gUpdateRateF;
            tricky_func_8470(self, (Vec3f* ) &objData->unk5E4[0]->srt.scale);
            objData->flags |= TRICKY_FLAG_UNK10;
            objData->unk1A = 3;
        } else if (gDLL_25->vtbl->FindWalkGroup(&self->srt.transl, NULL) == 0) {
            objData->flags |= (TRICKY_FLAG_UNK2000 | TRICKY_FLAG_UNK10);
        }
        return;
    case 3:
        objAnimSet(self, 14, 0.0f, 0);
        temp_t0 = (CurveSetup *) objData->unk5E4[1];
        objData->unk38 = 0.033f;
        objData->unk30[0] = temp_t0->pos.x - objData->unk5E4[0]->srt.scale;
        objData->unk30[1] = temp_t0->pos.z - objData->unk5E4[0]->srt.transl.y;
        objData->unk5F0 = (Object* ) dll_amSfx->Play(self, SOUND_48_Dig_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
        objData->unk1A = 4;
        /* fallthrough */
    case 4:
        temp_a0 = objData->unk28;
        temp_fv0 = ((DLL_Unknown*)temp_a0->dll)->vtbl->func[7].withTwoArgsF32((s32)temp_a0, (s32)self);
        self->srt.transl.x = objData->unk5E4[0]->srt.scale + (objData->unk30[0] * temp_fv0);
        self->srt.transl.z = objData->unk5E4[0]->srt.transl.y + (objData->unk30[1] * temp_fv0);
        tricky_func_87E4(self);
        temp_a0 = objData->unk28;
        if (((DLL_Unknown*)temp_a0->dll)->vtbl->func[8].withOneArgS32((s32)temp_a0) != 0) {
            temp_t0 = (CurveSetup *) objData->unk5E4[1];
            for (i = 0; i < 4; i++) {
                if ((temp_t0->links[i] >= 0) && (((CurveSetup*)objData->unk5E4[0])->uID != temp_t0->links[i])) {
                    objData->unk5E4[0] = (Object* ) temp_t0;
                    objData->unk5E4[1] = (Object* ) gDLL_26_Curves->vtbl->FindByUID(temp_t0->links[i]);
                    break;
                }
            }
            dll_amSfx->Stop((u32)objData->unk5F0);
            objData->unk1A = 5;
        }
        return;
    case 5:
        if (tricky_func_4F3C(self, 2.5f, objData, (Vec3f* ) &objData->unk5E4[1]->srt.scale) == 0) {
            temp_t0 = (CurveSetup *) objData->unk5E4[1];
            for (i = 0; i < 4; i++) {
                if ((temp_t0->links[i] >= 0) && (((CurveSetup*)objData->unk5E4[0])->uID != temp_t0->links[i])) {
                    objData->unk5E4[0] = (Object* ) temp_t0;
                    objData->unk5E4[1] = (Object* ) gDLL_26_Curves->vtbl->FindByUID(temp_t0->links[i]);
                    break;
                }
            }
            objData->unk1A = 6;
        }
        return;
    case 6:
        if (tricky_func_4F3C(self, 2.5f, objData, (Vec3f* ) &objData->unk5E4[1]->srt.scale) == 0) {
            tricky_func_83BC(self, objData);
            objData->flags &= ~(TRICKY_FLAG_UNK2000 | TRICKY_FLAG_UNK10);
            objData->unk1A = 7;
        }
        return;
    case 7:
        sp30 = gDLL_25->vtbl->FindWalkGroup(&self->srt.transl, NULL);
        if (gDLL_25->vtbl->FindWalkGroup(&objData->player->srt.transl, NULL) == sp30) {
            tricky_func_82B8(objData);
        }
        return;
    }
}

// offset: 0x2928 | func: 33 | state: 4
static void tricky_state_4(Object* self, Tricky_Data* objData) {
}

// offset: 0x2938 | func: 34 | state: 5
static void tricky_state_5(Object* self, Tricky_Data* objData) {
    CurveSetup* temp_v0;

    switch (objData->unk1A) {
    case 0:
        temp_v0 = gDLL_26_Curves->vtbl->FindByUID(((s32*)objData->unk5E4[1])[7 /* unk1C */]);
        objData->unk5E4[2] = temp_v0;
        tricky_func_9024(objData, &temp_v0->pos);
        objData->unk1A = 1;
        /* fallthrough */
    case 1:
        if (gDLL_25->vtbl->FindWalkGroup(&self->srt.transl, NULL) == ((u8*)objData->unk5E4[2])[3]) {
            objData->unk1A = 2;
        } else {
            tricky_func_53E4(self, 5.0f, objData);
            return;
        }
        /* fallthrough */
    case 2:
        if (tricky_func_4F3C(self, 2.5f, objData, (Vec3f* ) &objData->unk5E4[1]->srt.scale) == 0) {
            objData->flags |= TRICKY_FLAG_UNK10;
            objData->unk1A = 3;
            ((f32*)&objData->unk5F0)[0] = 0.0f;
        } else {
            if (gDLL_25->vtbl->FindWalkGroup(&self->srt.transl, NULL) == 0) {
                objData->flags |= TRICKY_FLAG_UNK10;
            }
            return;
        }
        /* fallthrough */
    case 3:
        objData->flags |= (TRICKY_FLAG_UNK4000 | TRICKY_FLAG_UNK10);
        objData->unk1A = 4;
        return;
    default:
        return;
    case 4:
        tricky_func_4F3C(self, 2.5f, objData, (Vec3f* ) &objData->unk5E4[2]->srt.scale);
        if (gDLL_25->vtbl->FindWalkGroup(&self->srt.transl, NULL) != 0) {
            tricky_func_82B8(objData);
        }
        return;
    }
}

// offset: 0x2B50 | func: 35 | state: 6
static void tricky_state_6(Object* self, Tricky_Data* objData) {
    switch (objData->unk1A) {
    case 0:
        tricky_func_9024(objData, (Vec3f* ) &objData->unk5E4[1]->srt.scale);
        objData->unk1A = 1;
        /* fallthrough */
    case 1:
        if (tricky_func_53E4(self, 5.0f, objData) == 0) {
            objData->unk1A = 3;
            ((f32*)&objData->unk5F0)[0] = NULL;
        } else {
            return;
        }
        /* fallthrough */
    case 3:
        objData->flags |= (TRICKY_FLAG_UNK4000 | TRICKY_FLAG_UNK10);
        gDLL_3_Animation->vtbl->start_obj_sequence(4, self, -1);
        objData->unk1A = 4;
        return;
    case 4:
        tricky_func_82B8(objData);
        break;
    }
}

// offset: 0x2C64 | func: 36 | state: 7
static void tricky_state_7(Object* self, Tricky_Data* objData) {
    CurveSetup* sp44;
    CurveSetup* sp40;
    CurveSetup* var_a3;
    CurveSetup* sp38;
    CurveSetup* sp34;
    s32 pad;

    if (objData->flags & TRICKY_FLAG_UNK10) {
        if (objData->unk3C8.unk0.unk80 == 0) {
            if (objData->unk3C8.unk0.unk10 != 0) {
                pad = gDLL_26_Curves->vtbl->func_438(objData->unk3C8.unkA4, 0);
                gDLL_25->vtbl->func_21F4(&objData->unk3C8, gDLL_26_Curves->vtbl->FindByUID(pad));
            }
        } else if (objData->unk3C8.unk0.unk10 == 0) {
            pad = gDLL_26_Curves->vtbl->func_590(objData->unk3C8.unkA4, 0);
            gDLL_25->vtbl->func_21F4(&objData->unk3C8, gDLL_26_Curves->vtbl->FindByUID(pad));
        }
        tricky_func_8ED0(objData->unk20, 0.8f, &objData->unk20);
        tricky_func_8974(self, &objData->unk3C8, objData->unk20);
        tricky_func_8470(self, &objData->unk3C8.unk0.unk68);
        if ((((s16*)objData->unk5E4)[0] != 0) && (gDLL_25->vtbl->FindWalkGroup(&self->srt.transl, NULL) != 0)) {
            tricky_func_82B8(objData);
        }
        ((f32*)&objData->unk5F0)[0] -= gUpdateRateF;
        if (((f32*)&objData->unk5F0)[0] < 0.0f) {
            ((f32*)&objData->unk5F0)[0] = mathRnd(200, 600);
            tricky_func_95E0(self, objData, _data_0[mathRnd(3, 5)]);
        }
    } else {
        tricky_func_53E4(self, 5.0f, objData);
        // TODO: something very wrong with the 2nd FindWalkGroup call
        if (gDLL_25->vtbl->FindWalkGroup(&self->srt.transl, NULL) == gDLL_25->vtbl->FindWalkGroup((Vec3f* ) &objData->unk5E4[1]->srt.scale, NULL)) {
            sp44 = (CurveSetup* ) objData->unk5E4[1];
            objData->flags |= TRICKY_FLAG_UNK10;
            sp38 = gDLL_26_Curves->vtbl->FindByUID(gDLL_26_Curves->vtbl->func_438(sp44, 0));
            sp34 = gDLL_26_Curves->vtbl->FindByUID(gDLL_26_Curves->vtbl->func_590(sp44, 0));
            if (vec3DistanceXZSquared(&self->globalPosition, &sp38->pos) > vec3DistanceXZSquared(&self->globalPosition, &sp34->pos)) {
                sp40 = sp38;
                var_a3 = gDLL_26_Curves->vtbl->FindByUID(gDLL_26_Curves->vtbl->func_438(sp38, 0));
                objData->unk3C8.unk0.unk80 = 0;
            } else {
                sp40 = sp34;
                var_a3 = gDLL_26_Curves->vtbl->FindByUID(gDLL_26_Curves->vtbl->func_590(sp34, 0));
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

// offset: 0x3188 | func: 37 | state: 8
static void tricky_state_8(Object* self, Tricky_Data* objData) {
    Object* temp_v0_3;
    ObjSetup* sp38;
    Object* var_v1;

    if (objData->flags & TRICKY_FLAG_UNK10) {
        switch (objData->unk1A) {
        case 3:
            self->shadow->flags |= TRICKY_FLAG_UNK1000;
            ((f32*)objData->unk5E4)[0] += 0.1f * gUpdateRateF;
            if (((f32*)objData->unk5E4)[0] > 20.0f) {
                ((f32*)objData->unk5E4)[0] = 20.0f;
                objData->unk1A = 4;
                ((f32*)&objData->unk5F0)[0] = 30.0f;
                dll_amSfx->Stop(objData->unk5F4_amsFxID);
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
                tricky_func_82B8(objData);
                dll_amSfx->Stop(objData->unk5F4_amsFxID);
                self->shadow->flags &= ~TRICKY_FLAG_UNK1000;
            }
            break;
        case 4:
            ((f32*)&objData->unk5F0)[0] -= gUpdateRateF;
            if (((f32*)&objData->unk5F0)[0] <= 0.0f) {
                objData->unk1A = 2;
                objData->unk5F4_amsFxID = dll_amSfx->Play(self, SOUND_4BB, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            break;
        }
    } else if (tricky_func_53E4(self, 5.0f, objData) == 0) {
        // FAKE
        if (1) {
            var_v1 = objData->unk28;
        }
        sp38 = var_v1->setup;
        ((f32*)objData->unk5E4)[0] = -5.0f;
        ((f32*)objData->unk5E4)[2] = 0.0001f;
        tricky_func_83BC(self, objData);
        if (objData->unk1A == 0) {
            if (vec3DistanceXZSquared(&self->globalPosition, &objData->unk28->globalPosition) > 400.0f) {
                sp38 = mapFindObjSetup(((s32*)sp38)[7 /* unk1C */], NULL, NULL, NULL, NULL);
                tricky_func_9024(objData, (Vec3f*)&sp38->x);
                objData->unk1A = 1;
            } else {
                objData->unk5E4[1] = mapFindObjSetup(((s32*)sp38)[7 /* unk1C */], NULL, NULL, NULL, NULL);
                objData->flags |= TRICKY_FLAG_UNK10;
                objData->unk1A = 3;
                objData->unk5F4_amsFxID = dll_amSfx->Play(self, SOUND_4B9, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
        } else {
            objData->unk5E4[1] = (Object* ) sp38;
            objData->flags |= TRICKY_FLAG_UNK10;
            objData->unk1A = 3;
            objData->unk5F4_amsFxID = dll_amSfx->Play(self, SOUND_4B9, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    } else {
        return;
    }
    objData->unk38 = 0.005f;
}

// offset: 0x3560 | func: 38 | state: 9
static void tricky_state_9(Object* self, Tricky_Data* objData) {
}

// offset: 0x3570 | func: 39
static s32 tricky_func_3570(Object* self, Tricky_Data* objData) {
    if (self->animProgress > 0.25f) {
        if (!(objData->flags & TRICKY_FLAG_EmittingFlames)) {
            tricky_func_9050(self, objData);
        } else if ((objData->unk5FC == 0 || objData->unk5FC(objData->unk28, 1, objData, self) != 0) && self->animProgress > 0.8f) {
            tricky_func_940C(self, objData);
            return 0;
        }
    }
    return 1;
}

// offset: 0x3664 | func: 40 | state: 10
static void tricky_state_10(Object* self, Tricky_Data* objData) {
    switch (objData->unk1A) {
    case 0:
        objData->unk5F4 = gDLL_25->vtbl->FindNearestTrickyCurve(&objData->player->srt.transl, -1, 4);
        if (objData->unk5F4->unk3 != 0) {
            tricky_func_9024(objData, &objData->unk5F4->pos);
            objData->unk1A = 1;
        } else {
            objData->unk5F8 = gDLL_26_Curves->vtbl->FindByUID(objData->unk5F4->links[0]);
            tricky_func_9024(objData, &objData->unk5F8->pos);
            objData->unk1A = 2;
        }
        tricky_func_53E4(self, 5.0f, objData);
        return;
    default:
        return;
    case 2:
        tricky_func_53E4(self, 5.0f, objData);
        if (gDLL_25->vtbl->FindWalkGroup(&self->srt.transl, NULL) == objData->unk5F8->unk3) {
            objData->unk19 = 1;
            objData->unk1A = 3;
        }
        return;
    case 3:
        tricky_func_4F3C(self, 2.5f, objData, &objData->unk5F4->pos);
        if (gDLL_25->vtbl->FindWalkGroup(&self->srt.transl, NULL) == 0) {
            objData->flags |= TRICKY_FLAG_UNK10;
            objData->unk1A = 4;
        }
        return;
    case 4:
        if (tricky_func_4F3C(self, 2.5f, objData, &objData->unk5F4->pos) == 0) {
            objAnimSet(self, 0x1A, 0.0f, 0);
            objData->unk38 = 0.004f;
            objData->unk1A = 6;
        } else {
            return;
        }
        /* fallthrough */
    case 6:
        tricky_func_871C(self, (s8* ) objData->unk5F4);
        if (tricky_func_3570(self, objData) == 0) {
            objData->unk1A = 7;
            objData->unk600 = 60.0f;
        }
        return;
    case 1:
        if (tricky_func_53E4(self, 5.0f, objData) == 0) {
            objAnimSet(self, 0x1A, 0.0f, 0);
            objData->unk38 = 0.004f;
            objData->flags |= TRICKY_FLAG_UNK10;
            objData->unk1A = 5;
            objData->unk30[0] = objData->unk28->srt.transl.x - self->srt.transl.x;
            objData->unk30[1] = objData->unk28->srt.transl.z - self->srt.transl.z;
        }
        return;
    case 5:
        tricky_func_87E4(self);
        if (tricky_func_3570(self, objData) == 0) {
            tricky_func_82B8(objData);
        }
        return;
    case 7:
        objData->unk600 -= gUpdateRateF;
        if (objData->unk600 <= 0.0f) {
            tricky_func_4F3C(self, 2.5f, objData, &objData->unk5F8->pos);
            if (gDLL_25->vtbl->FindWalkGroup(&self->srt.transl, NULL) != 0) {
                tricky_func_82B8(objData);
            }
        }
        return;
    }
}

// offset: 0x3A78 | func: 41
static s32 tricky_func_3A78(Tricky_Data* objData) {
    s32 count;
    Object* var_s2;
    Object** temp_s5;
    f32 sp58;
    f32 temp_fs0;
    s16 i;

    var_s2 = NULL;
    temp_s5 = objGetAllOfType(OBJTYPE_Baddie, &count);
    for (i = 0; i < count; i++) {
        temp_fs0 = vec3DistanceXZSquared(&temp_s5[i]->globalPosition, &objData->unk5F4_vec);
        if (var_s2 == NULL) {
            if (gDLL_25->vtbl->FindWalkGroup(&temp_s5[i]->globalPosition, NULL) == objData->unk608) {
                sp58 = temp_fs0;
                var_s2 = temp_s5[i];
            }
        } else if ((temp_fs0 < sp58) && (gDLL_25->vtbl->FindWalkGroup(&temp_s5[i]->globalPosition, NULL) == objData->unk608)) {
            sp58 = temp_fs0;
            var_s2 = temp_s5[i];
        }
    }
    if (var_s2 != NULL) {
        objData->unk604 = var_s2;
        tricky_func_9024(objData, &var_s2->globalPosition);
        objData->unk1A = 4;
        return 1;
    }

    return 0;
}

// offset: 0x3C1C | func: 42
static s32 tricky_func_3C1C(Object* self) {
    s32 count;
    Object** objects;
    s16 i;

    objects = objGetAllOfType(OBJTYPE_Baddie, &count);
    for (i = 0; i < count; i++) {
        if (self == objects[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

// offset: 0x3CAC | func: 43 | state: 11
static void tricky_state_11(Object* self, Tricky_Data* objData) {
    f32 temp_ft0;

    switch (objData->unk1A) {
    case 0:
        objData->unk608 = gDLL_25->vtbl->FindWalkGroup(objData->unk2C, NULL);
        objData->unk5F4_vec.x = objData->unk28->globalPosition.x - (mathSinfInterp(objData->unk28->srt.yaw) * 15.0f);
        objData->unk5F4_vec.y = objData->unk28->globalPosition.y;
        temp_ft0 = mathCosfInterp(objData->unk28->srt.yaw) * 15.0f;
        objData->unk5F4_vec.z = objData->unk28->globalPosition.z - temp_ft0;
        objData->unk60C = 0;
        objData->unk1A = 1;
        return;
    case 1:
        tricky_func_53E4(self, 5.0f, objData);
        if (gDLL_25->vtbl->FindWalkGroup(&self->srt.transl, NULL) == objData->unk608) {
            objData->unk1A = 2;
        }
        return;
    default:
        return;
    case 2:
        if (tricky_func_53E4(self, 5.0f, objData) == 0) {
            tricky_func_9024(objData, &objData->unk5F4_vec);
            objData->unk1A = 3;
        } else {
            tricky_func_3A78(objData);
            return;
        }
        /* fallthrough */
    case 3:
        if (tricky_func_53E4(self, 5.0f, objData) == 0) {
            tricky_func_83BC(self, objData);
        }

        tricky_func_3A78(objData);
        return;
    case 4:
        if (tricky_func_53E4(self, 15.0f, objData) == 0) {
            objData->flags |= TRICKY_FLAG_UNK10;
            if ((objData->stats->redFood != 0) && (objData->unk60C != 0)) {
                tricky_func_9050(self, objData);
                objData->stats->redFood--;
                objAnimSet(self, 0x205, 0.0f, 0);
                objData->unk38 = 0.005f;
                objData->unk1A = 5;
            } else {
                objAnimSet(self, 0x105, 0.0f, 0);
                objData->unk38 = 0.01f;
                objData->unk1A = 6;
            }
        } else {
            if (gDLL_25->vtbl->FindWalkGroup(objData->unk2C, NULL) != objData->unk608) {
                tricky_func_9024(objData, &objData->unk28->globalPosition);
                objData->unk1A = 2;
            }
            return;
        }
        /* fallthrough */
    case 5:
        if (self->animProgress >= 0.95f) {
            tricky_func_940C(self, objData);
            objData->flags &= ~TRICKY_FLAG_UNK10;
            if (tricky_func_3A78(objData) == 0) {
                tricky_func_9024(objData, &objData->unk28->globalPosition);
                objData->unk1A = 2;
            }
        } else if (tricky_func_3C1C(objData->unk604) != 0) {
            tricky_func_885C(self);
        }
        return;
    case 6:
        if (self->animProgress >= 0.95f) {
            objAnimSet(self, 0x106, 0.0f, 0);
            objData->unk38 = 0.005f;
            objData->unk600 = 0.0f;
            tricky_func_95E0(self, objData, _data_0[mathRnd(10, 12)]);
            objData->unk1A = 7;
        } else if (tricky_func_3C1C(objData->unk604) != 0) {
            tricky_func_885C(self);
        }
        return;
    case 7:
        if (mathRnd(0, 10) == 0) {
            tricky_func_95E0(self, objData, _data_0[mathRnd(10, 12)]);
        }
        objData->unk600 += gUpdateRateF;
        if (((objData->unk600 >= 150.0f) && (vec3DistanceXZSquared(objData->unk2C, &self->srt.transl) >= 2500.0f)) || (tricky_func_3C1C(objData->unk604) == 0)) {
            objAnimSet(self, 0x105, 1.0f, 0);
            objData->unk38 = -0.01f;
            objData->unk1A = 8;
        } else {
            tricky_func_885C(self);
        }
        return;
    case 8:
        if (self->animProgress <= 0.05f) {
            objData->flags &= ~TRICKY_FLAG_UNK10;
            if (tricky_func_3A78(objData) == 0) {
                tricky_func_9024(objData, &objData->unk28->globalPosition);
                objData->unk1A = 2;
            }
        }
        return;
    }
}

// offset: 0x4340 | func: 44 | state: 12
static void tricky_state_12(Object* self, Tricky_Data* objData) {
    if (tricky_func_53E4(self, 15.0f, objData) == 0) {
        tricky_func_83BC(self, objData);
    }
}

// offset: 0x43AC | func: 45 | state: 13
static void tricky_state_13(Object* self, Tricky_Data* objData) {
    if (tricky_func_53E4(self, 5.0f, objData) == 0) {
        tricky_func_83BC(self, objData);
    }
}

// offset: 0x4418 | func: 46 | state: 14
static void tricky_state_14(Object* self, Tricky_Data* objData) {
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
            if (tricky_func_53E4(self, 13.0f, objData) == 0) {
                objAnimSet(self, 17, 0.0f, 0);
                objData->unk38 = 0.03f;
                objData->flags |= TRICKY_FLAG_UNK10;
                objData->unk1A = 3;
                ((DLL_Unknown*)objData->unk5E4[0]->dll)->vtbl->func[8].withTwoArgs(objData->unk5E4[0], self);
            }
            return;
        }
        if (tricky_func_53E4(self, 25.0f, objData) == 0) {
            if (((f32*)objData->unk5E4)[1] > 0.0f) {
                tricky_func_83BC(self, objData);
                objData->unk38 = 0.005f;
                ((f32*)objData->unk5E4)[1] -= gUpdateRateF;
                if (((f32*)objData->unk5E4)[1] <= 0.0f) {
                    ((f32*)objData->unk5E4)[2] = 60.0f;
                }
            } else {
                tricky_func_8308(self, 0x10);
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
            sp30 = mathSinfInterp(self->srt.yaw);
            ((DLL_Unknown*)objData->unk5E4[0]->dll)->vtbl->func[7].withFiveArgsCustom4(objData->unk5E4[0], self, -sp30, 1.0f, -mathCosfInterp(self->srt.yaw));
            objData->unk1A = 2;
        }
        return;
    case 2:
        if (self->animProgress >= 0.95f) {
            tricky_func_82B8(objData);
        }
        return;
    case 3:
        if (self->animProgress >= 0.5f) {
            objData->unk1A = 4;
        }
        return;
    case 4:
        if (self->animProgress >= 0.95f) {
            objData->unk28 = objData->player;
            tricky_func_9024(objData, &objData->player->globalPosition);
            objData->unk1A = 5;
            /* fallthrough */
        } else {
            return;
        }
    case 5:
        if (tricky_func_53E4(self, 50.0f, objData) == 0) {
            objAnimSet(self, 0x13, 0.0f, 0);
            objData->unk38 = 0.03f;
            objData->unk1A = 6;
        }
        return;
    }
}

// offset: 0x47E8 | func: 47 | state: 15
static void tricky_state_15(Object* self, Tricky_Data* objData) {
    s32 sp24;
    s32 var_v0;
    s32 var_v1_2;

    sp24 = Arctanf(objData->unk28->srt.transl.x - self->srt.transl.x, objData->unk28->srt.transl.z - self->srt.transl.z);
    if (objData->unk1A == 0) {
        ((s32*)objData->unk5E4)[0] = mathRnd(0, 1);
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
    ((f32*)objData->unk5E4)[2] = objData->unk28->srt.transl.x - (Sinf(((s32*)objData->unk5E4)[1]) * 50.0f);
    ((f32*)objData->unk5E4)[3] = objData->unk28->srt.transl.y;
    objData->unk5F4_vec.x = objData->unk28->srt.transl.z - (Cosf(((s32*)objData->unk5E4)[1]) * 50.0f);
    tricky_func_53E4(self, 5.0f, objData);
}

// offset: 0x4974 | func: 48 | state: 16
static void tricky_state_16(Object* self, Tricky_Data* objData) {
    objData->unk28 = tricky_func_94BC(objData->player, 150.0f);
    if (objData->unk28 != NULL) {
        tricky_func_9024(objData, &objData->unk28->srt.transl);
    } else if (!(objData->flags & TRICKY_FLAG_UNK10)) {
        tricky_func_82B8(objData);
    }

    switch (objData->unk1A) {
    case 0:
        if (tricky_func_53E4(self, 50.0f, objData) == 0) {
            objData->unk1A = 1;
            objData->unk5F8 = 0;
            objData->unk5F4_vec.x = 0.0f;
            tricky_func_83BC(self, objData);
        }
        return;
    case 1:
        tricky_func_885C(self);
        if (vec3DistanceXZSquared(&self->srt.transl, objData->unk2C) > 5625.0f) {
            objData->unk1A = 0;
            return;
        }
        if (objData->stats->redFood != 0 && objData->unk5F8 != 0) {
            objData->unk1A = 2;
            return;
        }
        objData->unk5F4_vec.x -= gUpdateRateF;
        if (objData->unk5F4_vec.x < 0.0f) {
            objData->unk5F4_vec.x = mathRnd(200, 600) * 0.5f;
            tricky_func_95E0(self, objData, _data_0[6]);
        }
        break;
    case 2:
        if (tricky_func_53E4(self, 25.0f, objData) == 0) {
            tricky_func_9050(self, objData);
            objData->stats->redFood--;
            objAnimSet(self, 0x205, 0.0f, 0);
            objData->unk38 = 0.005f;
            objData->flags |= TRICKY_FLAG_UNK10;
            objData->unk1A = 3;
            objData->unk5F8 = 0;
            return;
        }
        break;
    case 3:
        if (self->animProgress >= 0.95f) {
            tricky_func_940C(self, objData);
            objData->flags &= ~TRICKY_FLAG_UNK10;
            objData->unk1A = 0;
            return;
        }
        if (objData->unk28 != NULL) {
            tricky_func_885C(self);
        }
        break;
    }
}

// offset: 0x4C94 | func: 49 | state: 17
static void tricky_state_17(Object* self, Tricky_Data* objData) {
    switch (objData->unk1A) {
    case 0:
        if (tricky_func_53E4(self, 50.0f, objData) == 0) {
            tricky_func_95E0(self, objData, _data_0[mathRnd(10, 12)]);
            objData->unk1A = 1;
            objAnimSet(self, 0x106, 0.0f, 0);
            objData->unk5F8 = 0;
            return;
        }
        return;
    case 1:
        if ((objData->stats->redFood != 0) && (objData->unk5F8 != 0)) {
            objData->unk1A = 2;
            return;
        }
        tricky_func_885C(self);
        if (mathRnd(0, 10) == 0) {
            tricky_func_95E0(self, objData, _data_0[mathRnd(10, 12)]);
        }
        break;
    case 2:
        if (tricky_func_53E4(self, 25.0f, objData) == 0) {
            tricky_func_9050(self, objData);
            objData->stats->redFood--;
            objAnimSet(self, 0x205, 0.0f, 0);
            objData->unk38 = 0.005f;
            objData->flags |= TRICKY_FLAG_UNK10;
            objData->unk1A = 3;
            objData->unk5F8 = 0;
            return;
        }
        break;
    case 3:
        if (self->animProgress >= 0.95f) {
            tricky_func_940C(self, objData);
            tricky_func_82B8(objData);
        } else {
            tricky_func_885C(self);
        }
        break;
    }
}

// offset: 0x4F3C | func: 50
static s32 tricky_func_4F3C(Object* self, f32 arg1, Tricky_Data* arg2, Vec3f* arg3) {
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
        tricky_func_8ED0(arg2->unk20, 1.5f, &arg2->unk20);
    } else {
        arg2->unk20 += -0.03f * gUpdateRateF;
        if (arg2->unk20 < 0.0f) {
            arg2->unk20 = 0.0f;
            return 0;
        }
    }

    tricky_func_8470(self, arg3);
    return 1;
}

// offset: 0x507C | func: 51
static void tricky_func_507C(Object* self, Tricky_Data* objData) {
    tricky_func_8F84(self, &objData->unk3C8.unkA0->pos, objData->unk30);
    objData->unk20 = 1.5f;
    objAnimSet(self, 0x15, 0.0f, 0);
    objData->unk19 = 9;
    tricky_func_95E0(self, objData, _data_0[mathRnd(7, 9)]);
}


// offset: 0x514C | func: 52
static void tricky_func_514C(Object* self, Tricky_Data* objData) {
    tricky_func_8F84(self, &objData->unk3C8.unkA0->pos, objData->unk30);
    objData->flags |= (TRICKY_FLAG_UNK80 | TRICKY_FLAG_UNK40);
    objData->unk44 = (objData->unk3C8.unkA0->pos.y - self->srt.transl.y) / 32.865f;
    objData->unk40 = 1.0f;
    if (objData->unk20 == 1.5f) {
        objAnimSet(self, 0x17, 0.0f, 0);
        objData->unk38 = 0.0135f;
    } else {
        objAnimSet(self, 0x18, 0.0f, 0);
        objData->unk38 = 0.00975f;
    }
    objData->unk19 = 12;
    tricky_func_8A94(self, &objData->unk3C8);
    tricky_func_95E0(self, objData, _data_0[mathRnd(7, 9)]);
}

// offset: 0x52B8 | func: 53
static void tricky_func_52B8(Object* self, Tricky_Data* objData) {
    tricky_func_8F84(self, &objData->unk3C8.unkA0->pos, objData->unk30);
    objData->flags |= (TRICKY_FLAG_UNK80 | TRICKY_FLAG_UNK40);
    objData->unk44 = (self->srt.transl.y - objData->unk3C8.unkA0->pos.y) / 33.114f;
    objData->unk40 = 1.0f;
    objAnimSet(self, 0x19, 0.0f, 0);
    objData->unk38 = 0.0125f;
    objData->unk19 = 14;
    tricky_func_8A94(self, &objData->unk3C8);
    tricky_func_95E0(self, objData, _data_0[mathRnd(7, 9)]);
}

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

typedef struct {
    u8 pad0;
    u8 unk1;
    u16 unk2[4];
    u16 padA;
    f32 unkC;
    f32 unk10;
} UnkSp44; // TODO: this isn't right

// offset: 0x53E4 | func: 54
static s32 tricky_func_53E4(Object* self, f32 arg1, Tricky_Data* objData) {
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
    spC0 = gDLL_25->vtbl->FindWalkGroup(&self->srt.transl, NULL);
    if (spC0 != 0 && spC0 != objData->unkA4) {
        objData->unkA4 = spC0;
        objData->flags &= ~TRICKY_FLAG_UNK400;
        for (var_a0 = 0; var_a0 < 4; var_a0++) { objData->unk6C[var_a0] = 0; }
    }
    spBC = gDLL_25->vtbl->FindWalkGroup(temp_s1, &sp44);
    spB8 = mainGetBits(BIT_Tricky_Walk_Group);
    if (spBC != 0 && spBC != spB8) {
        spB8 = spBC;
        mainSetBits(BIT_Tricky_Walk_Group, spBC);
    }
    sp84 = objData->unk20;
    objData->unk20 += -0.03f * gUpdateRateF;
    if (objData->unk20 < 0.0f) {
        objData->unk20 = 0.0f;
    }
    temp_fv0 = vec3DistanceXZSquared(temp_s1, &self->srt.transl);
    var_a0 = objData->unk20 * 10.0f;
    if (var_a0 >= 0x12) {
        var_fv1 = 100.0f;
    } else {
        var_fv1 = _data_A4[var_a0] + arg1;
        var_fv1 *= var_fv1;
    }
    if (var_fv1 < temp_fv0) {
        tricky_func_8ED0(sp84, 1.5f, &objData->unk20);
    } else if (temp_fv0 > 400.0f) {
        temp_fv0 = tricky_func_81D8(self);
        if (objData->unk20 < temp_fv0 && temp_fv0 < sp84) {
            objData->unk20 = temp_fv0;
        }
    }
    if (spBC == objData->unkA4) {
        objData->flags |= TRICKY_FLAG_UNK400;
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
    gDLL_25->vtbl->IsPointWithinAnyGroup(temp_s1);
    gDLL_25->vtbl->GetPatchGroupContainingPoint(temp_s1, objData->unkA4);
    if (objData->flags & TRICKY_FLAG_UNK400) {
        // @the fakest of fakes (this is an empty loop in default.dol)
        for (var_a0 = 0; var_a0 < 4; var_a0++) {
            // FAKE
            do { } while (0);
            if (sp44.unk1 & (1 << var_a0)) {
            }
        }
    }
    var_a3 = gDLL_25->vtbl->GetPatchGroupContainingPoint(temp_s1, objData->unkA4);
    if (objData->unk19 != 0xF && 
            ((spBC != 0 && spC0 == spBC) || (var_a3 != 0 && gDLL_25->vtbl->IsPointWithinPatchGroup(&self->srt.transl, objData->unkA4, var_a3) != 0))) {
        objData->unk19 = 1;
    } else if (objData->unk19 < 5) {
        var_a3 = gDLL_25->vtbl->GetPatchGroupContainingPoint(temp_s1, objData->unkA4);\
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
                if (spC0 == 0 && (var_a3 = gDLL_25->vtbl->GetPatchGroupContainingPoint(&self->srt.transl, objData->unkA4), (var_a3 != 0))) {
                    if (var_a3 == objData->unkA6) {
                        objData->unk19 = 3;
                    } else {
                        gDLL_25->vtbl->GetClosestExitOfPatch(&objData->player->srt.transl, &objData->unkC0, var_a3);
                        objData->unk19 = 4;
                    }
                } else {
                    objData->unk19 = 0;
                    gDLL_25->vtbl->IsPointWithinPatchGroup(&self->srt.transl, objData->unkA4, var_a3);
                }
            }
        } else if (spBC == 0) {
            objData->unk19 = 0;
        } else if (spC0 != 0) {
            var_a3 = spBC * spC0;
            if (gDLL_25->vtbl->IsPointWithinPatchGroup(&self->srt.transl, objData->unkA4, var_a3) != 0) {
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
            var_a3 = gDLL_25->vtbl->GetPatchGroupContainingPoint(&self->srt.transl, objData->unkA4);\
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
                        gDLL_25->vtbl->GetClosestExitOfPatch(&objData->player->srt.transl, &objData->unkC0, var_a3);
                        objData->unk19 = 4;
                    }
                } else if (var_a3 == objData->unkA6) {
                    objData->unk19 = 3;
                } else {
                    gDLL_25->vtbl->GetClosestExitOfPatch(&objData->player->srt.transl, &objData->unkC0, var_a3);
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
        tricky_func_8470(self, temp_s1);
        break;
    default:
        break;
    case 1:
        if (sp84 != 0.0f) {
            tricky_func_8470(self, temp_s1);
            break;
        }
        /* fallthrough */
    case 2:
        if (sp84 != 0.0f) {
            tricky_func_8470(self, &objData->unk74[sp5B]);
            break;
        }
        /* fallthrough */
    case 4:
        if (sp84 != 0.0f) {
            tricky_func_8470(self, &objData->unkC0);
        }
        /* fallthrough */
        break;
    case 3:
        if (sp84 != 0.0f) {
            tricky_func_8470(self, &objData->unkA8);
            break;
        }

        if (tricky_func_9668(&objData->unkCC) == 0) {
            sp68[0] = objData->unk2C->x - self->srt.transl.x;
            sp68[1] = objData->unk2C->z - self->srt.transl.z;
            var_s1 = (u16)mathAtan2f(-sp68[0], -sp68[1]);
            var_s1 = self->srt.yaw - (var_s1 & 0xFFFF);
            CIRCLE_WRAP(var_s1);
            var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
            if (var_v1 >= 0x1A17) {
                var_s1 = (var_s1 * 5) / 4;
                if (var_s1 > 0) {
                    if (var_s1 >= 0x342F) {
                        objAnimSet(self, 12, 0.0f, 0);
                        var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
                        objData->unk48 = var_v1 / 24490.0f;
                        objData->unk38 = 0.01f;
                    } else {
                        objAnimSet(self, 10, 0.0f, 0);
                        var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
                        objData->unk48 = var_v1 / 6679.0f;
                        objData->unk38 = 0.06f;
                    }
                } else if (var_s1 >= 0x342F) {
                    objAnimSet(self, 11, 0.0f, 0);
                    var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
                    objData->unk48 = var_v1 / 24490.0f;
                    objData->unk38 = 0.01f;
                } else {
                    objAnimSet(self, 9, 0.0f, 0);
                    var_v1 = (var_s1 >= 0 ? var_s1 : -var_s1);
                    objData->unk48 = var_v1 / 6679.0f;
                    objData->unk38 = 0.06f;
                }
                objData->unk19 = 0xF;
                objData->unk38 /= objData->unk48;
                objData->unk3C = 1.0f;
                objData->unk40 = 1.0f;
                objData->flags |= TRICKY_FLAG_UNK20;
                objData->flags |= TRICKY_FLAG_UNK40;
                objData->flags |= TRICKY_FLAG_UNK100;
            }
        }
        break;
    case 6:
        vec3DistanceXZSquared(&objData->unk3C0->pos, &self->srt.transl);
        if (vec3DistanceXZSquared(&objData->unk3C0->pos, &self->srt.transl) < 60.0f) {
            setup = objData->unk3C0;
            objData->unk3C8.unk0.unk80 = objData->unk3C4;
            spA0 = tricky_func_7A7C(objData, setup, spB8, objData->unk3C4);
            gDLL_25->vtbl->func_1D30(&objData->unk3C8, setup, spA0, tricky_func_7A7C(objData, spA0, spB8, objData->unk3C4));
            curves_func_800053B0(&objData->unk3C8.unk0, 0.1f);
            tricky_func_8974(self, &objData->unk3C8, objData->unk20);
            tricky_func_8470(self, &objData->unk3C8.unk0.unk68);
            if (setup != NULL && spA0 != NULL /*&& ? != NULL*/) {
                // TODO: the third param should be using the result of the 2nd tricky_func_7A7C call, but i can't get it to match 
                STUBBED_PRINTF("walk types are %d %d %d\n", setup->unk1A, spA0->unk1A, 0/*?->unk1A*/);
            }
            switch (setup->unk1A) {
            case 1:
                tricky_func_507C(self, objData);
                break;
            case 5:
                tricky_func_514C(self, objData);
                break;
            case 6:
                tricky_func_52B8(self, objData);
                break;
            case 2:
            case 7:
                objData->flags |= TRICKY_FLAG_UNK2000;
                /* fallthrough */
            default:
                objData->unk19 = 7;
                break;
            }
        } else {
            tricky_func_8470(self, &objData->unk3C0->pos);
        }
        break;
    case 5:
        tricky_func_7794(self, spA8, spC0, sp88);
        
        // NOTE: the return value here is var_a0 (the iterator var) in default.dol
        var_a0 = tricky_func_7BAC(objData, sp88, spA8, spB8);
        if (var_a0 == -1) {
            objData->unk20 = sp84;
            return 2;
        }
        objData->unk3C4 = spA8[var_a0];
        objData->unk3C0 = sp88[var_a0];

        // FAKE
        if (1) {
            tricky_func_8470(self, &objData->unk3C0->pos);
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
        mathRotateRPY((SRT*)&sp60, sp74.f);
        if (sp74.z > 0.0f && objData->unk20 != 0.0f) {
            if (!(objData->flags & TRICKY_FLAG_UNK2) || (objData->unk64 != spB8) || (objData->unk68 != objData->unk2C) || (objData->unk66 != objData->unk3C8.unk0.unk80)) {
                objData->flags |= TRICKY_FLAG_UNK2;
                objData->flags &= ~TRICKY_FLAG_UNK4;
                objData->unk64 = spB8;
                objData->unk68 = objData->unk2C;
                objData->unk66 = objData->unk3C8.unk0.unk80;
                routeSetup(objData->unk4DC, objData->unk3C8.unkA0, objData->unk2C, (void* ) spB8, objData->unk3C8.unk0.unk80 ^ 1);
                routeSetup(&objData->unk4DC[1], objData->unk3C8.unkA0, objData->unk2C, (void* ) spB8, objData->unk3C8.unk0.unk80);
            }
            if (!(objData->flags & TRICKY_FLAG_UNK4)) {
                temp_v1_4 = routeFind(&objData->unk4DC[1], 10);
                switch (temp_v1_4) {
                case 1:
                    objData->flags &= ~TRICKY_FLAG_UNK2;
                    break;
                case -1:
                    objData->flags |= TRICKY_FLAG_UNK4;
                    break;
                default:
                    temp_v1_4 = routeFind(objData->unk4DC, 10);
                    if (temp_v1_4 != -1) {
                        if (temp_v1_4 == 1) {
                            objData->flags |= TRICKY_FLAG_UNK4;
                        }
                    } else {
                        objData->flags &= ~TRICKY_FLAG_UNK2;
                    }
                    break;
                }
            }
            objData->unk50 -= gUpdateRateF;
            if (objData->unk50 <= 0.0f && objData->flags & TRICKY_FLAG_UNK4) {
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
            objData->flags &= ~TRICKY_FLAG_UNK2;
        }
        if ((objData->unk3C8.unk0.unk80 == 0 && objData->unk3C8.unk0.unk10 != 0) || (objData->unk3C8.unk0.unk80 != 0 && objData->unk3C8.unk0.unk10 == 0)) {
            setup2 = tricky_func_7A7C(objData, objData->unk3C8.unkA4, spB8, objData->unk3C8.unk0.unk80);
            if (setup2 != NULL) {
                dummy:
                gDLL_25->vtbl->func_21F4(&objData->unk3C8, setup2);
                if ((objData->unk3C8.unk9C->unk1A == 2) || (objData->unk3C8.unk9C->unk1A == 7)) {
                    if (objData->flags & TRICKY_FLAG_UNK2000) {
                        objData->flags &= ~TRICKY_FLAG_UNK2000;
                    } else {
                        objData->flags |= TRICKY_FLAG_UNK2000;
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
            setup2 = tricky_func_7A7C(objData, objData->unk3C8.unkA0, spB8, objData->unk3C8.unk0.unk80);
            if (setup2 != objData->unk3C8.unkA4) {
                dummy2:
                gDLL_25->vtbl->func_27A8(&objData->unk3C8, setup2);
            }
        }
        tricky_func_8974(self, &objData->unk3C8, objData->unk20);
        tricky_func_8470(self, &objData->unk3C8.unk0.unk68);
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
        tricky_func_8ED0(sp84, 1.5f, &objData->unk20);
        tricky_func_8974(self, &objData->unk3C8, objData->unk20);
        tricky_func_8470(self, &objData->unk3C8.unk0.unk68);
        if ((objData->unk3C8.unk0.unk80 == 0 && objData->unk3C8.unk0.unk10 != 0) || (objData->unk3C8.unk0.unk80 != 0 && objData->unk3C8.unk0.unk10 == 0)) {
            gDLL_25->vtbl->func_21F4(&objData->unk3C8, tricky_func_7A7C(objData, objData->unk3C8.unkA4, spB8, objData->unk3C8.unk0.unk80));
            tricky_func_507C(self, objData);
        }
        break;
    case 9:
        tricky_func_8ED0(sp84, 2.3f, &objData->unk20);
        tricky_func_87E4(self);
        if (self->animProgress < 0.5f) {
            objGetAnimChange(self, objData->unk20, &sp44.unk10);
            self->srt.transl.x += objData->unk30[0] * objData->unk20 * gUpdateRateF;
            self->srt.transl.z += objData->unk30[1] * objData->unk20 * gUpdateRateF;
        } else {
            objGetAnimChange(self, objData->unk20 * 0.25f, &sp44.unk10);
            self->srt.transl.x += objData->unk30[0] * (objData->unk20 * 0.25f) * gUpdateRateF;
            // FAKE
            if (var_v1) {}
            self->srt.transl.z += objData->unk30[1] * (objData->unk20 * 0.25f) * gUpdateRateF;
        }
        if (objAnimAdvance(self, sp44.unk10, gUpdateRateF, NULL) != 0) {
            setup = objData->unk3C8.unkA0;
            temp_fv0 = setup->pos.x - self->srt.transl.x;
            var_fv1 = setup->pos.z - self->srt.transl.z;
            temp_fv0 = sqrtf(SQ(temp_fv0) + SQ(var_fv1));
            objData->unk54 = temp_fv0 / 2.3f;
            objData->unk58 = 0.0f;
            objData->unk60 = self->srt.transl.y;
            objData->unk5C = (((setup->pos.y - self->srt.transl.y) - (-0.017f * objData->unk54 * objData->unk54)) / objData->unk54);
            objAnimSet(self, 0x16, 0.0f, 0);
            objAnimSetProgress(self, objData->unk58 / objData->unk54);
            objData->unk20 = 2.3f;
            objData->unk19 = 10;
            tricky_func_8A94(self, &objData->unk3C8);
        }
        objData->unk38 = 0.0f;
        break;
    case 10:
        objData->unk58 += gUpdateRateF;
        if (objData->unk54 <= objData->unk58) {
            self->srt.transl.y = objData->unk3C8.unkA0->pos.y;
            objAnimSetProgress(self, 1.0f);
            objData->unk19 = 7;
            objData->unk38 = 0.0f;
        } else {
            self->srt.transl.y = objData->unk60 + (objData->unk5C * objData->unk58) + (-0.017f * objData->unk58 * objData->unk58);
            var_fv1 = objData->unk54;
            if (var_fv1 <= 24.0f) {
                objAnimSetProgress(self, objData->unk58 / var_fv1);
            } else {
                sp44.unkC = 1.0f;
                if (objData->unk58 <= 6.0f) {
                    objAnimSetProgress(self, objData->unk58 / 24.0f);
                } else if ((var_fv1 - 6.0f) <= objData->unk58) {
                    objAnimSetProgress(self, ((24.0f - var_fv1) + objData->unk58) / 24.0f);
                    sp44.unkC = (((objData->unk54 - objData->unk58) / 6.0f) * 0.75f) + 0.25f;
                } else {
                    objAnimSetProgress(self, (((objData->unk58 - 6.0f) / (var_fv1 - 12.0f)) * 0.5f) + 0.25f);
                }
            }
            objData->unkCC.mode = 0;
            objData->unk38 = 0.0f;
            self->srt.transl.x += objData->unk30[0] * 2.3f * gUpdateRateF * sp44.unkC;
            self->srt.transl.z += objData->unk30[1] * 2.3f * gUpdateRateF * sp44.unkC;
        }
        break;
    case 11:
        //STUBBED_PRINTF("i before %d\n", objData->unk3C8.unk0.unk10);
        tricky_func_8ED0(sp84, 1.5f, &objData->unk20);
        tricky_func_8974(self, &objData->unk3C8, objData->unk20);
        tricky_func_8470(self, &objData->unk3C8.unk0.unk68);
        //STUBBED_PRINTF("i now %d\n", objData->unk3C8.unk0.unk10);
        if ((objData->unk3C8.unk0.unk80 == 0 && objData->unk3C8.unk0.unk10 != 0) || (objData->unk3C8.unk0.unk80 != 0 && objData->unk3C8.unk0.unk10 == 0)) {
            gDLL_25->vtbl->func_21F4(&objData->unk3C8, tricky_func_7A7C(objData, objData->unk3C8.unkA4, spB8, objData->unk3C8.unk0.unk80));
            tricky_func_514C(self, objData);
        }
        break;
    case 12:
    case 14:
        objData->unkCC.mode = 0;
        tricky_func_8974(self, &objData->unk3C8, objData->unk20);
        tricky_func_87E4(self);
        if (self->animProgress >= 0.95f) {
            objData->unk20 = 0.375f;
            tricky_func_8470(self, &objData->unk3C8.unk0.unk68);
            objData->unk19 = 7;
        }
        break;
    case 13:
        tricky_func_8ED0(sp84, 1.5f, &objData->unk20);
        tricky_func_8974(self, &objData->unk3C8, objData->unk20);
        tricky_func_8470(self, &objData->unk3C8.unk0.unk68);
        if ((objData->unk3C8.unk0.unk80 == 0 && objData->unk3C8.unk0.unk10 != 0) || (objData->unk3C8.unk0.unk80 != 0 && objData->unk3C8.unk0.unk10 == 0)) {
            gDLL_25->vtbl->func_21F4(&objData->unk3C8, tricky_func_7A7C(objData, objData->unk3C8.unkA4, spB8, objData->unk3C8.unk0.unk80));
            tricky_func_52B8(self, objData);
        }
        break;
    case 15:
        if (self->animProgress >= 0.95f) {
            objData->flags &= ~TRICKY_FLAG_UNK20;
            objData->flags &= ~TRICKY_FLAG_UNK40;
            objData->flags &= ~TRICKY_FLAG_UNK100;
            objData->unk20 = 0.0f;
            tricky_func_8F84(self, temp_s1, objData->unk30);
            objData->unk19 = 1;
            return 1;
        }
        break;
    }

    if (objData->unk19 < 5 && (gDLL_25->vtbl->IsPointWithinAnyGroup(&self->srt.transl) == FALSE)) {
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
static void tricky_func_7188(Object* self) {
    s32 i;
    Tricky_Data* objData;

    objData = self->data;
    for (i = 0; i < 4; i++) {
        objGetAttachPointWorldSpace(self, i + 1, &objData->unk384[i].x, &objData->unk384[i].y, &objData->unk384[i].z, 0);
    }

    objGetAttachPointWorldSpace(self, 5, &objData->unk3B4.x, &objData->unk3B4.y, &objData->unk3B4.z, 0);
}

// offset: 0x726C | func: 56
static void tricky_func_726C(Object* self) {
    Tricky_Data* objData; // sp4C
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
    if (mathRnd(0, 4) == 0) {
        gDLL_17_partfx->vtbl->spawn(self, 0xCA, &sp30, 1, -1, NULL);
    }
    if (mathRnd(0, 4) == 0) {
        gDLL_17_partfx->vtbl->spawn(self, 0xCB, &sp30, 1, -1, NULL);
    }
    sp30.transl.x = objData->unk384[1].x;
    sp30.transl.y = objData->unk384[1].y;
    sp30.transl.z = objData->unk384[1].z;
    sp30.yaw = self->srt.yaw;
    if (mathRnd(0, 4) == 0) {
        gDLL_17_partfx->vtbl->spawn(self, 0xCA, &sp30, 1, -1, NULL);
    }
    if (mathRnd(0, 4) == 0) {
        gDLL_17_partfx->vtbl->spawn(self, 0xCB, &sp30, 1, -1, NULL);
    }
}

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
static void tricky_func_74C4(Object* self, Gfx** arg1, Mtx** arg2, Vtx** arg3, Triangle** pols) {
    Vtx* sp6C;
    Tricky_Data* objData;
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
    dlSetPrimColor(arg1, 0xFF, 0xFF, 0xFF, 0x80);
    gSPGeometryMode(*arg1, 0xFFFFFF, 0x210405);
    dlApplyGeometryMode(arg1);
    gDPSetCombineLERP((*arg1), TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED);
    dlApplyCombine(arg1);
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
    dlApplyOtherMode(arg1);
    sp48.transl.x = self->srt.transl.x;
    sp48.transl.y = self->srt.transl.y;
    sp48.transl.z = self->srt.transl.z;
    sp48.yaw = self->srt.yaw;
    sp48.pitch = 0;
    sp48.roll = 0;
    sp48.scale = 0.05f;
    camSetupObjectSRTMatrix(arg1, arg2, &sp48, 1.0f, 0.0f, NULL);
    gSPVertex((*arg1)++, OS_PHYSICAL_TO_K0(sp6C), 8, 0);
    dlTriangles(arg1, (DLTri*)_rodata_778, 12);
    *arg3 = var_s0;
}

// offset: 0x7794 | func: 58
static void tricky_func_7794(Object* self, u8 *arg1, s16 arg2, CurveSetup** arg3) {
    s32 i;
    f32 sp7C[4];
    CurveNode* temp_a1;
    s32 sp74;
    CurveSetup* temp_s1;
    f32 temp_fs0;
    u8 var_a1;
    u8 sp6A;
    u8 var_a2;
    Tricky_Data* objData;

    objData = self->data;
    temp_a1 = gDLL_26_Curves->vtbl->GetCurves(&sp74);
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
                (temp_s1->type24.activeBit == -1 || mainGetBits(temp_s1->type24.activeBit) != 0) &&
                (temp_s1->type24.usedBit == -1 || mainGetBits(temp_s1->type24.usedBit) == 0)
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
static CurveSetup* tricky_func_7A7C(Tricky_Data* objData, CurveSetup* arg1, s32 arg2, u8 arg3) {
    CurveSetup* var_v1;

    var_v1 = NULL;
    if (objData->unk4D0 == arg1 && objData->unk4D8 == arg2 && objData->unk4DA == arg3) {
        var_v1 = tricky_func_8114(objData->unk4D4);
    }
    if (var_v1 == NULL) {
        var_v1 = tricky_func_7EFC(objData, arg1, arg2, arg3);
        if (var_v1 == NULL) {
            var_v1 = tricky_func_7DB8(objData, arg1, (void* ) arg2);
            if (var_v1 == NULL) {
                var_v1 = tricky_func_7EFC(objData, arg1, 0, arg3);
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
static s8 tricky_func_7BAC(Tricky_Data* objData, CurveSetup** arg1, u8 *arg2, void* arg3) {
    s8 sp5C[4];
    s8 sp5B;
    s8 temp_v0;
    s8 var_s0;
    s8 var_s2;

    for (sp5B = 0; sp5B < 4; sp5B++) {
        if (arg1[sp5B] != NULL) {
            routeSetup(&objData->unk4DC[sp5B], arg1[sp5B], objData->unk2C, arg3, arg2[sp5B]);
        }
    }

    for (sp5B = 0; sp5B < 0x64; sp5B++) {
        var_s2 = 0;
        for (var_s0 = 0; var_s0 < 4; var_s0++) {
            if (arg1[var_s0] != NULL) {
                sp5C[var_s0] = routeFind(&objData->unk4DC[var_s0], 5);
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
                    sp5C[0] = routeFind(objData->unk4DC, 500);
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
static CurveSetup* tricky_func_7DB8(Tricky_Data* objData, CurveSetup* arg1, void* arg2) {
    if (arg2 == NULL) {
        return NULL;
    }

    if (arg2 == objData->unk5D0 && arg1 == objData->unk5CC) {
        objData->unk5CC = routeNext(&objData->unk59C);
        if (objData->unk5CC == NULL) {
            return NULL;
        }

        objData->unk5CC = tricky_func_8114(objData->unk5CC);
        if (objData->unk5CC != NULL) {
            return objData->unk5CC;
        }
    }

    routeSetup(&objData->unk59C, arg1, objData->unk2C, arg2, objData->unk3C8.unk0.unk80);
    if (routeFind(&objData->unk59C, 500) != 1) {
        return NULL;
    }

    routeReconstruct(&objData->unk59C);
    objData->unk5CC = routeNext(&objData->unk59C);
    objData->unk5D0 = arg2;
    return objData->unk5CC;
}

// offset: 0x7EFC | func: 62
static CurveSetup* tricky_func_7EFC(Tricky_Data* objData, CurveSetup* arg1, s32 arg2, s32 arg3) {
    CurveSetup *sp68[4];
    f32 temp_fv0;
    f32 var_fs0;
    u16 var_s0;
    u16 var_s1;
    u16 var_s2;
    u16 var_s3;

    for (var_s0 = 0, var_s2 = 0, var_s3 = 1; var_s0 < 4; var_s0++, var_s3 <<= 1, arg3 <<= 1) {
        if (arg1->links[var_s0] >= 0 && ((arg3^0) != (arg1->unk1B & var_s3)) == 0) {
            sp68[var_s2] = gDLL_26_Curves->vtbl->FindByUID(arg1->links[var_s0]);
            // FAKE
            if (1) { }
            if (sp68[var_s2] != NULL) {
                if ((arg2 == 0 || arg2 == (arg1->unk4_u8[var_s2]))) {
                    if (
                        (sp68[var_s2]->type24.activeBit == -1 || mainGetBits(sp68[var_s2]->type24.activeBit) != 0) &&
                        (sp68[var_s2]->type24.usedBit == -1 || mainGetBits(sp68[var_s2]->type24.usedBit) == 0)
                    ) {
                        var_s2++;
                    }
                }
            }
        }
    }

    if (var_s2) {
        var_s0 = 0;
        var_fs0 = vec3DistanceXZSquared(&objData->player->globalPosition, &sp68[var_s0]->pos);
        var_s1 = var_s0;
        for (var_s0 = 1; var_s0 < var_s2; var_s0++) {
            temp_fv0 = vec3DistanceXZSquared(&objData->player->globalPosition, &sp68[var_s0]->pos);
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
static CurveSetup* tricky_func_8114(CurveSetup* setup) {
    if ((setup->type24.activeBit == -1 || mainGetBits(setup->type24.activeBit) != 0) 
        && (setup->type24.usedBit == -1 || mainGetBits(setup->type24.usedBit) == 0)) {
        return setup;
    }
    return NULL;
}

// offset: 0x81A8 | func: 64
static void tricky_func_81A8(Object* self) {
    Tricky_Data* temp_v0;
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
static f32 tricky_func_81D8(Object* self) {
    Tricky_Data* objData;
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
static void tricky_func_82B8(Tricky_Data* objData) {
    objData->state = TRICKY_STATE_1;
    objData->unk1A = 0;
    objData->unk5F4_vec.x = 0.0f;
    objData->unk5F4_vec.y = 0.0f;
    objData->unk5F4_vec.z = 0.0f;
    objData->unk600 = 600.0f;
    objData->flags &= ~TRICKY_FLAG_UNK10;
    objData->unk1D = 0;
}

// offset: 0x8308 | func: 67
static void tricky_func_8308(Object* self, s32 modAnim) {
    Tricky_Data* objData = self->data;
    
    if (modAnim == self->curModAnimId) {
        return;
    }

    if (self->curModAnimId > 0 && self->curModAnimId < 6 && modAnim > 0 && modAnim < 6) {
        if (objData->unk24 > 20.0f) {
            objData->unk24 = 0.0f;
            objAnimSet(self, modAnim, self->animProgress, 0);
        }
    } else {
        objData->unk24 = 0.0f;
        objAnimSet(self, modAnim, self->animProgress, 0);
    }
}

// offset: 0x83BC | func: 68
static void tricky_func_83BC(Object* self, Tricky_Data* objData) {
    if (tricky_func_9668(&objData->unkCC) != 0) {
        tricky_func_8308(self, 8);
        self->srt.transl.y = objData->unkCC.waterY;
        objData->unk38 = 0.02f;
    } else {
        tricky_func_8308(self, 0);
        objData->unk38 = 0.005f;
    }
}

// offset: 0x8470 | func: 69
static void tricky_func_8470(Object* self, Vec3f* arg1) {
    s32 temp;
    f32 var_f0;
    f32* temp_a2;
    Tricky_Data* objData;
    s16 sp2E;
    DLL27_Data* sp24;

    objData = self->data;
    sp24 = &objData->unkCC;
    var_f0=  objData->unk20;
    temp_a2 = objData->unk30;
    tricky_func_8F84(self, arg1, objData->unk30);
    temp = sp2E - (self->srt.yaw & 0xFFFF);
    if (var_f0 > 0.0f) {
        sp2E = self->srt.yaw;
        tricky_func_87E4(self);

        // FAKE code that probably got optimized out?
        temp = sp2E - (self->srt.yaw & 0xFFFF);
        // FAKE
        if (1)
        CIRCLE_WRAP(temp);
        if (tricky_func_9668(&objData->unkCC) != 0) {
            tricky_func_8308(self, 7);
            self->srt.transl.y = sp24->waterY;
        } else if (var_f0 > 1.0f) {
            tricky_func_8308(self, 5);
        } else if (var_f0 > 0.66f) {
            tricky_func_8308(self, 4);
        } else if (var_f0 > 0.33f) {
            tricky_func_8308(self, 2);
        } else {
            tricky_func_8308(self, 1);
        }
        self->srt.transl.x += temp_a2[0] * var_f0 * gUpdateRateF;
        self->srt.transl.z += temp_a2[1] * var_f0 * gUpdateRateF;
        objGetAnimChange(self, var_f0, &objData->unk38);
        return;
    }
    if (tricky_func_9668(&objData->unkCC) != 0) {
        tricky_func_8308(self, 7);
        self->srt.transl.y = sp24->waterY;
    } else {
        tricky_func_8308(self, 0);
        objData->unk38 = 0.005f;
    }
}

// offset: 0x871C | func: 70
static void tricky_func_871C(Object* self, s8* arg1) {
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
    tricky_func_88F4(self, var_a1);
}

// offset: 0x87E4 | func: 71
static void tricky_func_87E4(Object* self) {
    f32* objData;
    f32 x, z;

    objData = self->data;
    objData += 12;
    z = -objData[1];
    x = -objData[0];
    tricky_func_88F4(self, mathAtan2f(x, z));
}

// offset: 0x885C | func: 72
static void tricky_func_885C(Object* self) {
    Tricky_Data* objData;
    f32 sp24[2];

    objData = self->data;
    sp24[0] = objData->unk2C->x - self->srt.transl.x;
    sp24[1] = objData->unk2C->z - self->srt.transl.z;
    tricky_func_88F4(self, mathAtan2f(-sp24[0], -sp24[1]));
}

// offset: 0x88F4 | func: 73
static void tricky_func_88F4(Object* self, s16 arg1) {
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
static void tricky_func_8974(Object* self, UnkCurvesStruct* arg1, f32 arg2) {
    f32 square;
    f32 var_fs0;
    f32 distanceSquared;
    s32 i;
    
    distanceSquared = (arg2 * gUpdateRateF) * 1.5f;
    square = distanceSquared * distanceSquared;
    distanceSquared = vec3DistanceXZSquared((Vec3f *) (&arg1->unk0.unk68), &self->srt.transl);
    
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
        distanceSquared = vec3DistanceXZSquared((Vec3f *) (&arg1->unk0.unk68), &self->srt.transl);
    }
}

// offset: 0x8A94 | func: 75
static void tricky_func_8A94(Object* self, UnkCurvesStruct* arg1) {
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
static void tricky_func_8B5C(Object* self, s32 arg1) {
    Tricky_Data* objData;

    objData = self->data;
    objData->flags &= ~TRICKY_FLAG_UNK1;
    objData->flags |= TRICKY_FLAG_UNK4000;
    tricky_func_83BC(self, objData);
    gDLL_3_Animation->vtbl->start_obj_sequence(arg1, self, -1);
}

// offset: 0x8BEC | func: 77
static void tricky_func_8BEC(Object* self) {
    Tricky_Data* objData;
    f32 sp38;
    Object* sp34;

    objData = self->data;
    if (mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z) == -1) {
        objData->unkCC.mode = 0;
        if (objData->unk28 != NULL) {
            self->srt.transl.y = objData->unk28->srt.transl.y;
        }
    }
    if (objData->flags & TRICKY_FLAG_UNK2000) {
        trackGetHeightNearest(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &sp38, 0);
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
    if (objData->flags & TRICKY_FLAG_UNK8) {
        objData->unk330 += gUpdateRateF;
        if (objData->unk330 >= 40.0f) {
            objData->unk330 -= 40.0f;
            func_8002674C(self);
            objData->flags &= ~TRICKY_FLAG_UNK8;
        }
    } else {
        if ((objData->unk32C != NULL) && (sp34 == objData->unk32C)) {
            objData->unk330 += gUpdateRateF;
            if (objData->unk330 >= 20.0f) {
                objData->unk330 -= 20.0f;
                objData->flags |= TRICKY_FLAG_UNK8;
                func_800267A4(self);
            }
        } else {
            objData->unk330 = 0.0f;
        }
    }
    objData->unk32C = sp34;
    objData->hitByDamageType = func_80025F40(self, &objData->hitBy, NULL, NULL);
    gDLL_27->vtbl->func_1E8(self, &objData->unkCC, gUpdateRateF);
    gDLL_27->vtbl->func_5A8(self, &objData->unkCC);
    gDLL_27->vtbl->func_624(self, &objData->unkCC, gUpdateRateF);
    self->srt.pitch = objData->unkCC.relativeFloorPitchSmooth;
    self->srt.roll = objData->unkCC.relativeFloorRollSmooth;
}


// offset: 0x8ED0 | func: 78
static void tricky_func_8ED0(f32 arg0, f32 arg1, f32* arg2) {
    *arg2 = (0.02f * gUpdateRateF) + arg0;
    if (arg1 < *arg2) {
        *arg2 = arg1;
    }
}

// offset: 0x8F18 | func: 79
static void tricky_func_8F18(Tricky_Data* objData) {
    Object* temp_v0;

    temp_v0 = objGetNearestTypeTo(OBJTYPE_TrickyTarget, objData->player, NULL);
    objData->unk28 = temp_v0;
    tricky_func_9024(objData, &temp_v0->globalPosition);
    objData->unk1A = 0;
}

// offset: 0x8F84 | func: 80
// arg2 might be a Vec3f as well
static void tricky_func_8F84(Object* self, Vec3f* arg1, f32* arg2) {
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
static void tricky_func_9024(Tricky_Data* objData, Vec3f *arg1) {
    if (arg1 != objData->unk2C) {
        objData->unk2C = arg1;
        objData->unkA6 = 0;
        objData->flags &= ~TRICKY_FLAG_UNK400;
    }
}

// offset: 0x9050 | func: 82
static void tricky_func_9050(Object* self, Tricky_Data* objData) {
    FlameBlast_Setup* flame;
    FXEmit_Setup* fxemit;
    s32 i;

    objData->flags |= TRICKY_FLAG_EmittingFlames;
    objData->unk0 = dllLoad(DLL_ID_178, 1);
    for (i = 0; i < 3; i++) {
        flame = objAllocSetup(sizeof(FlameBlast_Setup), OBJ_flameblast);
        flame->base.x = self->srt.transl.x;
        flame->base.y = self->srt.transl.y;
        flame->base.z = self->srt.transl.z;
        flame->base.loadFlags = 2;
        flame->base.byte5 = 1;
        flame->timer = i * 10;
        objData->unk5E4[i] = objSetupObject(&flame->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, self->parent);
    }
    fxemit = objAllocSetup(sizeof(FXEmit_Setup), OBJ_FXEmit);
    fxemit->base.loadFlags = 2;
    fxemit->base.byte5 = 1;
    fxemit->base.x = self->srt.transl.x;
    fxemit->base.y = self->srt.transl.y;
    fxemit->base.z = self->srt.transl.z;
    fxemit->toggleGamebit = -1;
    fxemit->disableGamebit = -1;
    fxemit->yaw = 0;
    fxemit->flagConfig = 1;
    fxemit->pitch = 0;
    fxemit->roll = 0;
    fxemit->rollSpeed = 0;
    fxemit->pitchSpeed = 0;
    fxemit->yawSpeed = 0;
    fxemit->activationRange = 0;
    fxemit->bank = 1;
    fxemit->id = 0x4A;
    fxemit->fxRate = -0x1E;
    objData->unk5F0 = objSetupObject(&fxemit->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, self->parent);
}

// offset: 0x9200 | func: 83
static void tricky_func_9200(Object* self, Tricky_Data* objData) {
    SRT sp40;
    u8 temp_v0;

    if (objData->flags & (TRICKY_FLAG_UNK1000 | TRICKY_FLAG_EmittingFlames)) {
        sp40.transl.x = objData->unk3B4.x - self->srt.transl.x;
        sp40.transl.y = objData->unk3B4.y - self->srt.transl.y;
        sp40.transl.z = objData->unk3B4.z - self->srt.transl.z;
        sp40.scale = 1.0f;
        sp40.yaw = self->srt.yaw;
        sp40.pitch = self->srt.pitch;
        sp40.roll = self->srt.roll;
        if (objData->flags & TRICKY_FLAG_EmittingFlames) {
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
        tricky_func_95E0(self, objData, _data_0[2]);
        temp_v0 = 20;
        // FAKE
        do {} while (0);
        while (temp_v0--) {
            gDLL_17_partfx->vtbl->spawn(self, 0x533, &sp40, 2, -1, NULL);
        }
        objData->flags &= ~TRICKY_FLAG_UNK1000;
    }
}

// offset: 0x940C | func: 84
static void tricky_func_940C(Object* self, Tricky_Data* objData) {
    s32 i;

    objData->flags &= ~TRICKY_FLAG_EmittingFlames;
    objData->flags |= TRICKY_FLAG_UNK1000;
    dllFree(objData->unk0);
    objFreeObject(objData->unk5F0);
    for (i = 0; i < 3; i++) { objFreeObject(objData->unk5E4[i]); }
}

// offset: 0x94BC | func: 85
static Object* tricky_func_94BC(Object* self, f32 range) {
    Object* baddie;
    Object** baddies;
    f32 dist;
    s32 numBaddies;
    s32 i;

    baddie = NULL;
    baddies = objGetAllOfType(OBJTYPE_Baddie, &numBaddies);
    range = SQ(range);
    for (i = 0; i < numBaddies; i++) {
        if ((objIsObjectType(baddies[i], OBJTYPE_TrickyTarget) == 0) && (gDLL_33_BaddieControl->vtbl->get_health_ratio(baddies[i]) > 0.0f)) {
            dist = vec3DistanceSquared(&self->globalPosition, &baddies[i]->globalPosition);
            if (dist < range) {
                range = dist;
                baddie = baddies[i];
            }
        }
    }
    return baddie;
}

// offset: 0x95E0 | func: 86
static void tricky_func_95E0(Object* self, Tricky_Data* objData, s16* arg2) {
    if (arg2[2] != 0) {
        objExpr_func_800349C0(self, &objData->unk360, arg2[0], arg2[1], arg2[2], 1);
    } else {
        objExpr_func_80034B54(self, &objData->unk360, arg2, 1);
    }
}

// offset: 0x9668 | func: 87
static s32 tricky_func_9668(DLL27_Data* objData) {
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
