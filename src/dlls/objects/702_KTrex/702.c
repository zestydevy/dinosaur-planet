#include "dlls/engine/33.h"
#include "dlls/objects/214_animobj.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "sys/main.h"
#include "sys/objanim.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "segment_334F0.h"
#include "prevent_bss_reordering.h"

enum KTLogicStates {
    KT_LSTATE_0 = 0,
    KT_LSTATE_1 = 1,
    KT_LSTATE_2 = 2,
    KT_LSTATE_3 = 3,
    KT_LSTATE_4 = 4,
    KT_LSTATE_5 = 5,
    KT_LSTATE_6 = 6,
    KT_LSTATE_7 = 7,
    KT_LSTATE_8 = 8,
    KT_LSTATE_9 = 9,
    KT_LSTATE_10 = 10,
    KT_LSTATE_11 = 11
};

enum KTAnimStates {
    KT_ASTATE_0 = 0,
    KT_ASTATE_1 = 1,
    KT_ASTATE_2 = 2,
    KT_ASTATE_3 = 3,
    KT_ASTATE_4 = 4,
    KT_ASTATE_5 = 5,
    KT_ASTATE_6 = 6,
    KT_ASTATE_7 = 7,
    KT_ASTATE_8 = 8
};

enum KTFlags {
    // Whether the boss is moving clockwise (0) or counter-clockwise (1) around the arena.
    KTFLAG_REVERSED = 0x1,
    // Which arena segment the boss is in.
    // Bits 1-2.
    KTFLAG_SEGMENT = 0x6,
    // Whether the boss was just damaged by the player.
    KTFLAG_DAMAGED = 0x8,
    // Whether the boss can be damaged.
    KTFLAG_VULNERABLE = 0x10,
    // Whether the boss is currently charging forward. Not used for the full lap he does after getting zapped.
    KTFLAG_CHARGING = 0x20
};

#define KTFLAG_GET_SEGMENT(ktflags) ((ktflags >> 1) & 3)
#define KTFLAG_SET_SEGMENT(segment) (segment << 1)

// Note: This is not the only object data. DLL 33 data is prepended to this in memory.
typedef struct {
/*000*/ GenericStack *stateStack;
/*004*/ f32 timer;
        // Where in the current segment the boss in, ranging from 0-1.
        // When reversed (moving counter-clockwise), this value progresses from 1 to 0 instead of 0 to 1.
/*008*/ f32 segmentPos;
/*00C*/ s32 standingUpSegment; // The last segment the boss stood up in after being knocked down.
        // The following fields are curve XYZ coords. These determine the positions used
        // for near and far sides of each segment, for both clockwise and counter-clockwise movement.
/*010*/ f32 unk10[4];
/*020*/ f32 unk20[4];
/*030*/ f32 unk30[4];
/*040*/ f32 unk40[4];
/*050*/ f32 unk50[4];
/*060*/ f32 unk60[4];
/*070*/ f32 unk70[4]; // Reversed positions start here (see above comments)
/*080*/ f32 unk80[4];
/*090*/ f32 unk90[4];
/*0A0*/ f32 unkA0[4];
/*0B0*/ f32 unkB0[4];
/*0C0*/ f32 unkC0[4];
        // The current near and far positions of each segment.
        // These are flipped when the boss is reversed (moving counter-clockwise).
/*0D0*/ f32 *segNearX;
/*0D4*/ f32 *segNearY;
/*0D8*/ f32 *segNearZ;
/*0DC*/ f32 *segFarX;
/*0E0*/ f32 *segFarY;
/*0E4*/ f32 *segFarZ;
        // World position that the boss should be in.
        // Moves to this position on the next tick.
        // Calculated by combining the current segment pos with the target curve pos.
/*0E8*/ Vec3f pos;
        // Where in the boss's current segment the player is, ranging from 0-1.
        // Note: This is *not* the pos of the player in their current segment. If the player is not in
        // the same segment as the boss, this value is not well defined.
/*0F4*/ f32 playerSegmentPos;
/*0F8*/ s16 turnStartYaw; // The yaw of the boss at the start of a turn.
/*0FA*/ u16 flags;
        // = 0 when walking around normally.
        // = 1 when charging at sabre.
        // = 2 when charging around the arena after getting zapped.
/*0FC*/ u8 anger;
/*0FD*/ u8 playerSpotted; // Whether the player was seen.
        // Which arena segment the boss is in (bitfield).
        // This is used to check if the boss is in the same segment as the player.
/*0FE*/ u8 selfSegmentBitfield;
        // Which arena segment the player is in (bitfield).
        // Determined by triggers that set/unset gamebits as the player walks around the arena.
/*0FF*/ u8 playerSegmentBitfield;
/*100*/ u8 laserWallBitfield; // Bitfield of which laser walls are active.
/*101*/ u8 fightProgress; // Increases as the player progresses the fight by damaging the boss.
/*102*/ u8 health;
/*103*/ s8 chargeCounter; // How many segments to charge down. Decrements after each turn while charging.
/*104*/ s32 fxFlags;
/*108*/ u8 unk108;
/*10C*/ SRT unk10C;
/*124*/ SRT unk124;
/*13C*/ SRT unk13C;
/*154*/ SRT unk154;
/*16C*/ Vec3f unk16C;
} KTrex_Data;

typedef struct {
/*00*/ DLL33_ObjSetup base;
/*38*/ f32 speeds[3]; // Movement speed, per "anger" level.
/*44*/ u16 chargePrepTime[3]; // Delay before charging.
/*4A*/ u16 vulnerableTime[4]; // How long the boss is vulnerable for.
/*52*/ u8 reverseChance[4]; // Chance to reverse direction.
/*56*/ u8 chargeChance[4]; // Chance to charge without seeing Sabre.
} KTrex_ObjSetup;

/*0x0*/ static u32 _data_0[] = {
    0x00000002, 0x00000002
};
/*0x8*/ static s16 _data_8[] = { // mod anim indices
    0x0009, 0x0012, 0x0012, 0x0000
};
/*0x10*/ static s16 _data_10[] = { // mod anim indices
    0x0001, 0x0002, 0x0003, 0x0000
};
/*0x18*/ static s16 _data_18[] = { // mod anim indices
    0x0004, 0x0006, 0x0006, 0x0000
};
/*0x20*/ static s16 _data_20[3][2] = { // mod anim indices
    { 0x0008, 0x000e }, 
    { 0x0010, 0x0011 }, 
    { 0x0010, 0x0011 }
};
/*0x2C*/ static u16 _data_2C[] = {
    0x0001, 0x0004, 0x0010, 0x0000
};
/*0x34*/ static u16 _data_34[] = {
    0x0002, 0x0008, 0x0020, 0x0000
};
/*0x3C*/ static u16 _data_3C[] = {
    0x4000, 0x4000, 0x4000, 0x0000
};
/*0x44*/ static u16 _data_44[] = {
    0x8000, 0x8000, 0x8000, 0x0000
};
/*0x4C*/ static u16 _data_4C[] = {
    0x0040, 0x0080, 0x0100, 0x0000
};
/*0x54*/ static f32 _data_54[] = {
    0.006, 0.003, 0.003
};
/*0x60*/ static f32 _data_60[] = {
    0.0055, 0.012, 0.012
};
/*0x6C*/ static f32 _data_6C[] = { // Segment progress to turn at when near the start of a segment.
    0, 0.025, 0.025
};
/*0x78*/ static f32 _data_78[] = { // Segment progress to turn at when near the end of a segment.
    1, 0.975, 0.975
};
/*0x84*/ static u16 _data_84[2] = {SOUND_6FA, SOUND_6FB};
/*0x88*/ static u16 _data_88[2] = {SOUND_691_KT_Rex_Roar, SOUND_6FD_KT_Rex_Roar};
/*0x8C*/ static u16 _data_8C[2] = {SOUND_6FE_KT_Rex_Roar, 0};
/*0x90*/ static u16 _data_90[2] = {SOUND_25B_Magic_Attack_Deflected, SOUND_25C_Melee_Attack_Deflected};
/*0x94*/ static u32 _data_94[] = { // curve uIDs
    0x32136, 0x3213d, 0x32128, 0x3212f
};
/*0xA4*/ static u32 _data_A4[] = { // curve uIDs
    0x32130, 0x32137, 0x32123, 0x32129
};
/*0xB4*/ static u32 _data_B4[] = { // curve uIDs
    0x32367, 0x32369, 0x3236a, 0x32365
};
/*0xC4*/ static u32 _data_C4[] = { // curve uIDs
    0x32132, 0x32139, 0x32124, 0x3212b
};
/*0xD4*/ static s16 _data_D4[] = { // game bits
    BIT_566, BIT_567, BIT_568, BIT_569
};
/*0xDC*/ static s16 _data_DC[] = { // game bits
    BIT_560, BIT_561, BIT_562, BIT_563
};
/*0xE4*/ static DLL_IModgfx *_data_E4 = NULL;

/*0x0*/ static ObjFSA_StateCallback sAnimStateCallbacks[9];
/*0x28*/ static ObjFSA_StateCallback sLogicStateCallbacks[12];
/*0x58*/ static DLL33_Data* sDLL33Data;
/*0x5C*/ static KTrex_Data *sKTData;

static s32 dll_702_anim_state_0(Object* arg1, ObjFSA_Data* arg2, f32 arg3);
static s32 dll_702_anim_state_1(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_anim_state_2(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_anim_state_3(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_anim_state_4(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_anim_state_5(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_anim_state_6(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_anim_state_7(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_anim_state_8(Object* arg1, ObjFSA_Data* arg2, f32 arg3);

static s32 dll_702_logic_state_0(Object *self, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_logic_state_1(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_logic_state_2(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_logic_state_3(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_logic_state_4(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_logic_state_5(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_logic_state_6(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_logic_state_7(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_logic_state_8(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_logic_state_9(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_logic_state_10(Object* arg0, ObjFSA_Data* arg1, f32 arg2);
static s32 dll_702_logic_state_11(Object* arg0, ObjFSA_Data* arg1, f32 arg2);

static s32 dll_702_func_B98(void);
static u8 dll_702_func_C2C(u16 arg0);
static f32 dll_702_func_C74(Object* arg0, KTrex_Data* arg1);
static s32 dll_702_func_D5C(u8 arg0);
/*static*/ int dll_702_func_119C(Object* a0, Object* a1, AnimObj_Data* a2, s8 a3);
static void dll_702_func_12DC(Object* arg0);
static void dll_702_func_1EF0(Object* arg0, DLL33_Data* arg1);

// offset: 0x0 | func: 0
static void dll_702_func_0(void) {
    sAnimStateCallbacks[0] = dll_702_anim_state_0;
    sAnimStateCallbacks[1] = dll_702_anim_state_1;
    sAnimStateCallbacks[2] = dll_702_anim_state_2;
    sAnimStateCallbacks[3] = dll_702_anim_state_3;
    sAnimStateCallbacks[4] = dll_702_anim_state_4;
    sAnimStateCallbacks[5] = dll_702_anim_state_5;
    sAnimStateCallbacks[6] = dll_702_anim_state_6;
    sAnimStateCallbacks[7] = dll_702_anim_state_7;
    sAnimStateCallbacks[8] = dll_702_anim_state_8;
    
    sLogicStateCallbacks[0] = dll_702_logic_state_0;
    sLogicStateCallbacks[1] = dll_702_logic_state_1;
    sLogicStateCallbacks[2] = dll_702_logic_state_2;
    sLogicStateCallbacks[3] = dll_702_logic_state_3;
    sLogicStateCallbacks[4] = dll_702_logic_state_4;
    sLogicStateCallbacks[5] = dll_702_logic_state_5;
    sLogicStateCallbacks[6] = dll_702_logic_state_6;
    sLogicStateCallbacks[7] = dll_702_logic_state_7;
    sLogicStateCallbacks[8] = dll_702_logic_state_8;
    sLogicStateCallbacks[9] = dll_702_logic_state_9;
    sLogicStateCallbacks[10] = dll_702_logic_state_10;
    sLogicStateCallbacks[11] = dll_702_logic_state_11;
}

// offset: 0x120 | ctor
void dll_702_ctor(void *dll) {
    dll_702_func_0();
}

// offset: 0x160 | dtor
void dll_702_dtor(void *dll) { }

// offset: 0x16C | func: 1 | export: 0
#ifndef NON_MATCHING
void dll_702_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_setup.s")
#else
// missing static refs
void dll_702_setup(Object* self, DLL33_ObjSetup* setup, s32 arg2) {
    CurveSetup* curve;
    s32 i;
    s32 var_v0;
    DLL33_Data* objdata;
    KTrex_Data* ktdata;

    objdata = (DLL33_Data*)self->data;
    var_v0 = 0x10;
    if (arg2 != 0) {
        var_v0 = 0x11;
    }
    gDLL_33->vtbl->func21(self, setup, objdata, 9, 0xC, 0x100, (u8) var_v0, 20.0f);
    self->unk0xbc = (ObjectCallback)dll_702_func_119C;
    gDLL_18_objfsa->vtbl->set_anim_state(self, &objdata->fsa, KT_ASTATE_0);
    objdata->fsa.logicState = KT_LSTATE_2; // set initial state
    objdata->fsa.target = NULL;
    objdata->fsa.unk4.mode = 0;
    objdata->fsa.unk33D = 0;
    self->unk0xaf |= 0x88;
    func_8002674C(self);
    if (self->ptr0x64 != NULL) {
        self->ptr0x64->flags |= 0x810;
    }
    ktdata = objdata->unk3F4;
    ktdata->stateStack = generic_stack_new(4, sizeof(s32));

    for (i = 0; i < 4; i++) {
        curve = gDLL_26_Curves->vtbl->curves_func_39c(_data_A4[i]);
        if (curve != NULL) {
            ktdata->unk10[i] = curve->base.x;
            ktdata->unk20[i] = curve->base.y;
            ktdata->unk30[i] = curve->base.z;
            curve = gDLL_26_Curves->vtbl->curves_func_39c(_data_94[i]);
            ktdata->unk40[i] = curve->base.x;
            ktdata->unk50[i] = curve->base.y;
            ktdata->unk60[i] = curve->base.z;
            curve = gDLL_26_Curves->vtbl->curves_func_39c(_data_C4[i]);
            ktdata->unk70[i] = curve->base.x;
            ktdata->unk80[i] = curve->base.y;
            ktdata->unk90[i] = curve->base.z;
            curve = gDLL_26_Curves->vtbl->curves_func_39c(_data_B4[i]);
            ktdata->unkA0[i] = curve->base.x;
            ktdata->unkB0[i] = curve->base.y;
            ktdata->unkC0[i] = curve->base.z;
        }
    }
    
    ktdata->segNearX = ktdata->unk10;
    ktdata->segNearY = ktdata->unk20;
    ktdata->segNearZ = ktdata->unk30;
    ktdata->segFarX = ktdata->unk40;
    ktdata->segFarY = ktdata->unk50;
    ktdata->segFarZ = ktdata->unk60;
    ktdata->health = 4;
    _data_E4 = (DLL_IModgfx*)dll_load_deferred(DLL_ID_106, 1);
}
#endif

// offset: 0x448 | func: 2 | export: 1
void dll_702_control(Object* self) {
    DLL33_Data* temp_s1;
    f32 sp38[3];

    if (self->unk0xdc == 0) {
        temp_s1 = (DLL33_Data*)self->data;
        sDLL33Data = temp_s1;
        sKTData = sDLL33Data->unk3F4;
        func_80028D2C(self);
        temp_s1->fsa.target = get_player();
        if (temp_s1->fsa.target != NULL) {
            sp38[0] = temp_s1->fsa.target->positionMirror.x - self->positionMirror.x;
            sp38[1] = temp_s1->fsa.target->positionMirror.y - self->positionMirror.y;
            sp38[2] = temp_s1->fsa.target->positionMirror.z - self->positionMirror.z;
            temp_s1->fsa.targetDist = sqrtf(SQ(sp38[0]) + SQ(sp38[1]) + SQ(sp38[2]));
        }
        func_80032A08(self, &sDLL33Data->unk3BC);
        sKTData->playerSegmentBitfield = dll_702_func_B98();
        sKTData->playerSegmentPos = dll_702_func_C74(temp_s1->fsa.target, sKTData);
        sKTData->selfSegmentBitfield = dll_702_func_C2C(sKTData->flags);
        sKTData->laserWallBitfield = dll_702_func_D5C(sKTData->selfSegmentBitfield);
        gDLL_33->vtbl->func20(self, &temp_s1->fsa, &sDLL33Data->unk34C, sDLL33Data->unk39E, &sDLL33Data->unk3B4, 2, 2, 0);
        if (sKTData->flags & KTFLAG_VULNERABLE) {
            temp_s1->fsa.unk348 = 2;
        } else {
            temp_s1->fsa.unk348 = 0;
        }
        dll_702_func_1EF0(self, temp_s1);
        dll_702_func_12DC(self);
        gDLL_33->vtbl->func10(self, temp_s1, 0.0f, 0);
        func_80026128(self, 0x17, 1, -1);
        gDLL_18_objfsa->vtbl->tick(self, &temp_s1->fsa, delayFloat, delayFloat, sAnimStateCallbacks, sLogicStateCallbacks);
        self->srt.transl.y = sKTData->pos.y;
    }
}

// offset: 0x72C | func: 3 | export: 2
void dll_702_update(Object *self) { }

// offset: 0x738 | func: 4 | export: 3
void dll_702_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    MtxF sp48;
    s32 _pad;
 
    sDLL33Data = (DLL33_Data* ) self->data;
    if ((visibility != 0) && (self->unk0xdc == 0)) {
        if (sDLL33Data->unk3E8 != 0.0f) {
            func_80036FBC(0xC8, 0, 0, sDLL33Data->unk3E8);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_80031F6C(self, 1, &sKTData->unk124.transl.x, &sKTData->unk124.transl.y, &sKTData->unk124.transl.z, 0);
        func_80031F6C(self, 2, &sKTData->unk13C.transl.x, &sKTData->unk13C.transl.y, &sKTData->unk13C.transl.z, 0);
        func_80031F6C(self, 3, &sKTData->unk154.transl.x, &sKTData->unk154.transl.y, &sKTData->unk154.transl.z, 0);
        func_80031F6C(self, 0, &sKTData->unk10C.transl.x, &sKTData->unk10C.transl.y, &sKTData->unk10C.transl.z, 0);
        memcpy(&sp48, func_80032170(self, 4), 0x40);
        sp48.m[3][1] = 0.0f;
        sp48.m[3][0] = 0.0f;
        sp48.m[3][2] = 0.0f;
        sKTData->unk16C.x = (f32) ((f32) rand_next(-0x32, 0x32) * 0.1f);
        sKTData->unk16C.y = (f32) ((f32) rand_next(0x3C, 0x78) * 0.1f);
        sKTData->unk16C.z = (f32) ((f32) rand_next(0x64, 0x96) * -0.25f);
        vec3_transform(&sp48, sKTData->unk16C.x, sKTData->unk16C.y, sKTData->unk16C.z, &sKTData->unk16C.x, &sKTData->unk16C.y, &sKTData->unk16C.z);
        sKTData->fxFlags |= 0x100000;
    }
}

// offset: 0xA78 | func: 5 | export: 4
void dll_702_free(Object* self, s32 a1) {
    sDLL33Data = (DLL33_Data* ) self->data;
    sKTData = sDLL33Data->unk3F4;
    obj_free_object_type(self, 4);
    gDLL_33->vtbl->func15(self, sDLL33Data, 0);
    generic_stack_free(sKTData->stateStack);
    if (_data_E4 != NULL) {
        dll_unload(_data_E4);
    }
    _data_E4 = NULL;
}

// offset: 0xB3C | func: 6 | export: 5
u32 dll_702_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_8 | MODFLAGS_EVENTS;
}

// offset: 0xB4C | func: 7 | export: 6
u32 dll_702_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL33_Data) + sizeof(KTrex_Data);
}

// offset: 0xB60 | func: 8 | export: 7
s16 dll_702_get_fsa_anim_state(Object* self) {
    DLL33_Data *dll33data;
    
    dll33data = (sDLL33Data = (DLL33_Data*)self->data);
    return dll33data->fsa.animState;
}

// offset: 0xB88 | func: 9 | export: 8
void dll_702_func_B88(Object *self, s32 arg1) { 

}

// offset: 0xB98 | func: 10
static s32 dll_702_func_B98(void) {
    s32 i;
    u8 var_s2;

    var_s2 = 0;
    for (i = 0;  i < 4; i++) {
        if (main_get_bits(_data_D4[i]) != 0) {
            var_s2 |= (1 << i);
        }
    }
    return var_s2;
}

// offset: 0xC2C | func: 11
static u8 dll_702_func_C2C(u16 ktflags) {
  s8 data_E8_local[4] = {0x02, 0x08, 0x01, 0x04};

  return data_E8_local[KTFLAG_GET_SEGMENT(ktflags)];
}

// offset: 0xC74 | func: 12
static f32 dll_702_func_C74(Object* obj, KTrex_Data* ktdata) {
    s32 _pad;
    s32 segment;
    f32 segmentLength;
    f32 zDiff;
    f32 xDiff;

    segment = KTFLAG_GET_SEGMENT(ktdata->flags);
    xDiff = ktdata->segFarX[segment] - ktdata->segNearX[segment];
    zDiff = ktdata->segFarZ[segment] - ktdata->segNearZ[segment];
    segmentLength = sqrtf(SQ(xDiff) + SQ(zDiff));
    xDiff = obj->srt.transl.x - ktdata->segNearX[segment];
    zDiff = obj->srt.transl.z - ktdata->segNearZ[segment];
    return sqrtf(SQ(xDiff) + SQ(zDiff)) / segmentLength;
}

// offset: 0xD5C | func: 13
static s32 dll_702_func_D5C(u8 segmentBitfield) {
    s32 mask;
    s32 i;
    u8 var_s2;

    var_s2 = 0;
    
    for (i = 0; i < 4; i++) {
        mask = 1 << i;
        if ((segmentBitfield & mask) && (main_get_bits(_data_DC[i]) != 0)) {
            var_s2 |= mask;
        }
    }
    return var_s2;
}

// offset: 0xE24 | func: 14
static void dll_702_push_state(s32 state) {
    if (!generic_stack_is_full(sKTData->stateStack)) {
        generic_stack_push(sKTData->stateStack, &state);
    }
}

// offset: 0xE88 | func: 15
static s32 dll_702_pop_state(void) {
    s32 state;

    state = 0;
    if (!generic_stack_is_empty(sKTData->stateStack)) {
        generic_stack_pop(sKTData->stateStack, &state);
    }
    return state;
}

// offset: 0xEF0 | func: 16
static s32 dll_702_func_EF0(Object* arg0) {
    f32 temp_fv1;
    f32 temp_fv1_2;

    if (sKTData->laserWallBitfield == 0) {
        return FALSE;
    }
    switch (sKTData->laserWallBitfield) {
    case 1:
    case 2:
        temp_fv1 = arg0->srt.transl.z - 150.0f;
        temp_fv1_2 = arg0->srt.transl.z + 150.0f;
        if ((temp_fv1 > -8960.0f) || (temp_fv1_2 < -8960.0f)) {
            return FALSE;
        }
        return TRUE;
    case 4:
    case 8:
        temp_fv1 = arg0->srt.transl.x - 150.0f;
        temp_fv1_2 = arg0->srt.transl.x + 150.0f;
        if ((temp_fv1 > -13470.0f) || (temp_fv1_2 < -13470.0f)) {
            return FALSE;
        }
        return TRUE;
    default:
        return FALSE;
    }
}

// offset: 0xFE4 | func: 17
static s32 dll_702_func_FE4(ObjFSA_Data* fsa, KTrex_Data* ktdata) {
    f32 posDelta;
    s32 reversed;
    s32 segment;
    s32 turn;

    turn = FALSE;
    reversed = ktdata->flags & KTFLAG_REVERSED;
    segment = KTFLAG_GET_SEGMENT(ktdata->flags);
    if (reversed) {
        posDelta = -fsa->speed;
    } else {
        posDelta = fsa->speed;
    }

    ktdata->segmentPos += posDelta * delayFloat;

    if ((_data_78[ktdata->anger] < ktdata->segmentPos) || (ktdata->segmentPos < _data_6C[ktdata->anger])) {
        // Reached end of segment, turn
        turn = TRUE;
        if (reversed) {
            segment -= 1;
            if (segment < 0) {
                segment = 3;
            }
        } else {
            segment += 1;
            if (segment > 3) {
                segment = 0;
            }
        }
        ktdata->flags &= ~KTFLAG_SEGMENT;
        ktdata->flags |= KTFLAG_SET_SEGMENT(segment);
        if (ktdata->segmentPos > _data_78[ktdata->anger]) {
            ktdata->segmentPos = _data_78[ktdata->anger];
        } else if (ktdata->segmentPos < _data_6C[ktdata->anger]) {
            ktdata->segmentPos = _data_6C[ktdata->anger];
        }
        if ((ktdata && ktdata) && ktdata){} // @fake
    }
    
    ktdata->pos.x = ktdata->segNearX[segment] + ((ktdata->segFarX[segment] - ktdata->segNearX[segment]) * ktdata->segmentPos);
    ktdata->pos.y = ktdata->segNearY[segment] + ((ktdata->segFarY[segment] - ktdata->segNearY[segment]) * ktdata->segmentPos);
    ktdata->pos.z = ktdata->segNearZ[segment] + ((ktdata->segFarZ[segment] - ktdata->segNearZ[segment]) * ktdata->segmentPos);
    return turn;
}

// offset: 0x119C | func: 18
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/702_KTrex/dll_702_func_119C.s")
#else
/*static*/ int dll_702_func_119C(Object* a0, Object* a1, AnimObj_Data* a2, s8 a3) {
    s32 i;

    a2->unk62 = 0;

    for (i = 0; i < a2->unk98; i++) {
        switch (a2->unk8E[i]) {
        case 1:
            sKTData->fxFlags |= 4;
            break;
        case 2:
            sKTData->fxFlags |= 8;
            break;
        case 3:
            sKTData->fxFlags |= 0x800;
            break;
        case 4:
            sKTData->fxFlags |= 0x1000;
            break;
        case 5:
            sKTData->fxFlags |= 0x20000;
            break;
        default:
            break;
        }
    }
    dll_702_func_12DC(a0);
    return 0;
}
#endif

// offset: 0x12DC | func: 19
static void dll_702_func_12DC(Object* arg0) {
    s32 i;
    f32 sp48;

    sp48 = 1.0f - (sDLL33Data->fsa.targetDist / 2000.0f);
    if (sp48 < 0.0f) {
        sp48 = 0.0f;
    } else if (sp48 > 1.0f) {
        sp48 = 1.0f;
    }
    if (sKTData->fxFlags & 0x40) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_68E_KT_Rex_Noise, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & 0x80) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_68F_KT_Rex_Roar, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & 0x100) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_690_KT_Rex_Roar, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & 0x200) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_68D_KT_Rex_Roar_Kinda, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & 0x10000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_6FC_KT_Rex_Slam, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & 0x40000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, _data_84[rand_next(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & 0x80000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, *_data_8C, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & 0x2000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, _data_88[rand_next(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & 0x1000) {
        sKTData->fxFlags = sKTData->fxFlags & ~0x1800;
    }
    if (sKTData->fxFlags & 0x20000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_6F9_Explosion, MAX_VOLUME, NULL, NULL, 0, NULL);
        func_800013BC();
        func_80003B70(2.0f * sp48);
    }
    if (sKTData->fxFlags & 0x4000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_68B_KT_Rex_Groan, MAX_VOLUME, NULL, NULL, 0, NULL);
        sKTData->unk108 ^= 1;
    }
    if (sKTData->fxFlags & 0x8000) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_68C_KT_Rex_Groan, MAX_VOLUME, NULL, NULL, 0, NULL);
        sKTData->unk108 ^= 1;
    }
    if (sKTData->fxFlags & 3) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_688_KT_Rex_Stomp, MAX_VOLUME, NULL, NULL, 0, NULL);
        if (sp48 > 0.1f) {
            func_800013BC();
            func_80003B70(sp48);
            main_set_bits(BIT_554, 1);
        }
    }
    if (sKTData->fxFlags & 0xC) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_689_KT_Rex_Stomp, MAX_VOLUME, NULL, NULL, 0, NULL);
        if (sp48 > 0.1f) {
            func_800013BC();
            func_80003B70(2.0f * sp48);
            main_set_bits(BIT_554, 1);
        }
    }
    if (sKTData->fxFlags & 0x30) {
        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_68A_KT_Rex_Stomp, MAX_VOLUME, NULL, NULL, 0, NULL);
        if (sp48 > 0.1f) {
            func_800013BC();
            func_80003B70(3.0f * sp48);
            main_set_bits(BIT_554, 1);
        }
    }
    if (!(sKTData->fxFlags & 0x100000)) {
        sKTData->fxFlags = sKTData->fxFlags & 0x1800;
        return;
    }
    if (sKTData->fxFlags & 1) {
        sKTData->unk124.scale = 1.0f;
        for (i = 0; i != 0xA; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &sKTData->unk124, 0x200001, -1, NULL);
        }
    }
    if (sKTData->fxFlags & 2) {
        sKTData->unk13C.scale = 1.0f;
        for (i = 0; i != 0xA; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &sKTData->unk13C, 0x200001, -1, NULL);
        }
    }
    if (sKTData->fxFlags & 4) {
        sKTData->unk124.scale = 1.5f;
        for (i = 0; i != 0xD; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &sKTData->unk124, 0x200001, -1, NULL);
        }
    }
    if (sKTData->fxFlags & 8) {
        sKTData->unk13C.scale = 1.5f;
        for (i = 0; i != 0xD; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &sKTData->unk13C, 0x200001, -1, NULL);
        }
    }
    if (sKTData->fxFlags & 0x10) {
        sKTData->unk124.scale = 2.0f;
        for (i = 0; i != 0x10; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk124, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &sKTData->unk124, 0x200001, -1, NULL);
        }
    }
    if (sKTData->fxFlags & 0x20) {
        sKTData->unk13C.scale = 2.0f;
        for (i = 0; i != 0x10; i++) {
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x483, &sKTData->unk13C, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x484, &sKTData->unk13C, 0x200001, -1, NULL);
        } 
    }
    if (sKTData->fxFlags & 0x800) {
        gDLL_17->vtbl->func1(arg0, 0x487, &sKTData->unk10C, 0x200001, -1, (void*)&sKTData->unk16C);
    }
    sKTData->fxFlags = sKTData->fxFlags & 0x1800;
}

// offset: 0x1E9C | func: 20
static void dll_702_func_1E9C(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = 1 << arg0;
    if (sDLL33Data->fsa.unk308 & temp_v0) {
        sDLL33Data->fsa.unk308 &= ~temp_v0;
        sKTData->fxFlags |= arg1;
    }
}

// offset: 0x1EF0 | func: 21
static void dll_702_func_1EF0(Object* arg0, DLL33_Data* arg1) {
    static SRT _bss_60;
    DLL33_Data* temp_v0 = (DLL33_Data*)arg0->data;
    s32 sp60;
    s32 sp5C;
    s32 sp58;
    Object* sp54;
    MtxF* temp_v1; // maybe not a matrix?
    ModelInstance* modelInst;
    u32 sp3C[] = {0x00000006, 0x00000069, 0x00000069, 0x000000ff};

    if (temp_v0->unk3E8 > 0.0f) {
        temp_v0->unk3E8 += (delayFloat * temp_v0->unk3EC);
        if (temp_v0->unk3E8 < 0.0f) {
            temp_v0->unk3E8 = 0.0f;
        } else if (temp_v0->unk3E8 > 120.0f) {
            temp_v0->unk3E8 = (f32) (120.0f - (temp_v0->unk3E8 - 120.0f));
            temp_v0->unk3EC = (f32) -temp_v0->unk3EC;
        }
    }
    sp60 = func_80025F40(arg0, &sp54, &sp5C, &sp58);
    if (sp60 != 0) {
        modelInst = arg0->modelInsts[arg0->modelInstIdx];
        temp_v1 = modelInst->unk_0x24;
        if ((arg1->fsa.unk348 != 0) && ((sp5C == 3) || (sp5C == 2))) {
            _bss_60.transl.x = temp_v1->m[sp5C][1] + gWorldX;
            _bss_60.transl.y = temp_v1->m[sp5C][2];
            _bss_60.transl.z = temp_v1->m[sp5C][3] + gWorldZ;
            gDLL_6_AMSFX->vtbl->play_sound(arg0, _data_8C[0], MAX_VOLUME, NULL, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_693_Explosion, MAX_VOLUME, NULL, NULL, 0, NULL);
            gDLL_2_Camera->vtbl->func8(2, 0);
            gDLL_17->vtbl->func1(arg0, 0x4B2, &_bss_60, 0x200001, -1, NULL);
            gDLL_17->vtbl->func1(arg0, 0x4B3, &_bss_60, 0x200001, -1, NULL);
            sKTData->flags &= ~KTFLAG_VULNERABLE;
            sKTData->flags |= KTFLAG_DAMAGED;
            arg1->fsa.unk343 = (s8) sp60;
            arg1->fsa.unk348 -= 1;
        } else {
            gDLL_6_AMSFX->vtbl->play_sound(arg0, _data_90[rand_next(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
            modelInst = arg0->modelInsts[arg0->modelInstIdx];
            temp_v1 = modelInst->unk_0x24;
            _bss_60.transl.x = temp_v1->m[sp5C][1] + gWorldX;
            _bss_60.transl.y = temp_v1->m[sp5C][2];
            _bss_60.transl.z = temp_v1->m[sp5C][3] + gWorldZ;
            gDLL_17->vtbl->func1(arg0, 0x328, &_bss_60, 0x200001, -1, NULL);
            _bss_60.transl.x -= arg0->positionMirror.x;
            _bss_60.transl.y -= arg0->positionMirror.y;
            _bss_60.transl.z -= arg0->positionMirror.z;
            _bss_60.scale = 1.0f;
            _bss_60.yaw = 0;
            _bss_60.pitch = 0;
            _bss_60.roll = 0;
            sp3C[1] += rand_next(0, 0x9B);
            sp3C[2] += rand_next(0, 0x9B);
            _data_E4->vtbl->func0(arg0, 0, &_bss_60, 1, -1, &sp3C);
        }
        if (arg1->fsa.unk348 <= 0) {
            arg1->fsa.unk348 = 0;
        }
        obj_send_mesg(sp54, 0xE0001, arg0, NULL);
    }
}

// offset: 0x23EC | func: 22
static s32 dll_702_anim_state_0(Object* self, ObjFSA_Data* fsa, f32 arg3) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0, 0.0f, 0);
    }
    fsa->unk298 = 0.01f;
    return 0;
}

// offset: 0x2454 | func: 23
/** In this state during: 
  * intro cutscene, 
  * walking straight down hall, 
  * charging at sabre (after spotted) 
  // TODO: 
  * after getting back up, charge around the whole arena?
  */
static s32 dll_702_anim_state_1(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    f32 temp_fa1;
    f32 temp_fv0;
    s32 var_a1;

    if (fsa->enteredAnimState) {
        func_80023D30(self, (s32) _data_10[sKTData->anger], 0.0f, 0);
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
    }
    dll_702_func_1E9C(2, (s32) _data_2C[sKTData->anger]);
    dll_702_func_1E9C(1, (s32) _data_34[sKTData->anger]);
    if (sKTData->unk108 != 0) {
        var_a1 = _data_3C[sKTData->anger];
    } else {
        var_a1 = _data_44[sKTData->anger];
    }
    dll_702_func_1E9C(0, var_a1);
    temp_fv0 = (sKTData->pos.x - self->srt.transl.x) * inverseDelay;
    temp_fa1 = (sKTData->pos.z - self->srt.transl.z) * inverseDelay;
    func_8002493C(self, sqrtf(SQ(temp_fv0) + SQ(temp_fa1)), &fsa->unk298);
    self->srt.transl.x = sKTData->pos.x;
    self->srt.transl.z = sKTData->pos.z;
    return 0;
}

// offset: 0x2644 | func: 24
/** In this state during: turning corner */
static s32 dll_702_anim_state_2(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    MtxF tempMtx;
    f32 tempY;
    SRT tempSRT;
    u16 reversed;

    reversed = sKTData->flags & KTFLAG_REVERSED;
    if (fsa->enteredAnimState) {
        func_80023D30(self, _data_20[sKTData->anger & 0xFFFF][reversed], 0.0f, 0);
        fsa->unk298 = _data_60[sKTData->anger];
        sKTData->turnStartYaw = self->srt.yaw;
    }
    dll_702_func_1E9C(2, 1);
    dll_702_func_1E9C(1, 2);
    dll_702_func_1E9C(0, 0x40);
    dll_702_func_1E9C(7, 0x10000);
    fsa->unk340 |= 1;
    gDLL_18_objfsa->vtbl->func7(self, fsa, delayFloat, 3);
    tempSRT.yaw = sKTData->turnStartYaw;
    tempSRT.pitch = 0;
    tempSRT.roll = 0;
    tempSRT.transl.x = 0.0f;
    tempSRT.transl.y = 0.0f;
    tempSRT.transl.z = 0.0f;
    tempSRT.scale = 1.0f;
    matrix_from_srt(&tempMtx, &tempSRT);
    vec3_transform(&tempMtx, fsa->unk27C, 0.0f, -fsa->unk278, &self->speed.x, &tempY, &self->speed.z);
    if (reversed) {
        self->srt.yaw = (s16) (s32) ((f32) sKTData->turnStartYaw + (16384.0f * self->animProgress));
    } else {
        self->srt.yaw = (s16) (s32) ((f32) sKTData->turnStartYaw - (16384.0f * self->animProgress));
    }
    return 0;
}

// offset: 0x28BC | func: 25
static s32 dll_702_anim_state_3(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    u16 reversed;

    reversed = sKTData->flags & KTFLAG_REVERSED;
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0xF, 0.0f, 0);
        fsa->unk298 = 0.005f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
        sKTData->turnStartYaw = self->srt.yaw;
    }
    if (reversed) {
        self->srt.yaw = (s16) (s32) ((f32) sKTData->turnStartYaw + (32768.0f * self->animProgress));
    } else {
        self->srt.yaw = (s16) (s32) ((f32) sKTData->turnStartYaw - (32768.0f * self->animProgress));
    }
    return 0;
}

// offset: 0x29D0 | func: 26
/** In this state during: roaring at sabre (when spotted) */
static s32 dll_702_anim_state_4(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, _data_18[sKTData->playerSpotted], 0.0f, 0);
        fsa->unk298 = _data_54[sKTData->playerSpotted];
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
    }
    dll_702_func_1E9C(0, _data_4C[sKTData->playerSpotted]);
    dll_702_func_1E9C(9, 0x800);
    dll_702_func_1E9C(0xA, 0x1000);
    return 0;
}
// offset: 0x2AF0 | func: 27
/** In this state during: roaring (at end of charge) */
static s32 dll_702_anim_state_5(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, _data_8[sKTData->anger], 0.0f, 0);
        fsa->unk298 = 0.005f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
    }
    dll_702_func_1E9C(0, 0x200);
    return 0;
}

// offset: 0x2BA4 | func: 28
/** In this state during: zapped */
static s32 dll_702_anim_state_6(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0xB, 0.0f, 0);
        fsa->unk298 = 0.006f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
    }
    dll_702_func_1E9C(0, 0x80000);
    dll_702_func_1E9C(7, 0x20000);
    return 0;
}

// offset: 0x2C54 | func: 29
/** In this state during: on ground (after zapped) */
static s32 dll_702_anim_state_7(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0xC, 0.0f, 0);
        fsa->unk298 = 0.01f;
    }
    dll_702_func_1E9C(0, 0x2000);
    dll_702_func_1E9C(7, 0x40000);
    return 0;
}

// offset: 0x2CF8 | func: 30
/** In this state during: getting back up (after zapped, hit or not) */
static s32 dll_702_anim_state_8(Object* self, ObjFSA_Data* fsa, f32 arg3) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, 0xD, 0.0f, 0);
        fsa->unk298 = 0.0017f;
    }
    dll_702_func_1E9C(0, 0x2000);
    return 0;
}

// offset: 0x2D80 | func: 31
static s32 dll_702_logic_state_0(Object *self, ObjFSA_Data* fsa, f32 arg2) {
    return 0;
}

// offset: 0x2D98 | func: 32
static s32 dll_702_logic_state_1(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    if (fsa->enteredLogicState) {
        gDLL_28_ScreenFade->vtbl->fade(30, SCREEN_FADE_BLACK);
        func_80013FB4();
        self->unk0xaf |= 8;
        fsa->unk33D = 0;
        fsa->unk4.mode = 0;
        obj_send_mesg(get_player(), 0xE0000, self, NULL);
    }
    return 0;
}

// offset: 0x2E64 | func: 33
/** In this state during: intro cutscene, walking straight down hall */
static s32 dll_702_logic_state_2(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    KTrex_ObjSetup* objsetup;
    s32 sp30;
    s32 reversed;

    objsetup = (KTrex_ObjSetup*)self->setup;
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_1);
        sKTData->anger = 0;
        sKTData->flags &= ~KTFLAG_CHARGING;
        fsa->speed = objsetup->speeds[sKTData->anger] / 1000.0f;
    }
    if (dll_702_func_FE4(fsa, sKTData) != 0) {
        dll_702_push_state(KT_LSTATE_2);
        return KT_LSTATE_3 + 1;
    }

    reversed = sKTData->flags & KTFLAG_REVERSED;
    if (sKTData->anger == 0) {
        if ((sKTData->fightProgress >= 2) && !(sKTData->flags & KTFLAG_CHARGING) && 
                ((!reversed && sKTData->segmentPos >= 0.7f) || (reversed && sKTData->segmentPos <= 0.3f))) {
            sp30 = (s32) sKTData->fightProgress >> 1;
            if (rand_next(0, 100) <= (s32) objsetup->chargeChance[sp30]) {
                sKTData->chargeCounter = 2;
                dll_702_push_state(KT_LSTATE_5);
                sKTData->playerSpotted = TRUE;
                return KT_LSTATE_4 + 1;
            }
            if (rand_next(0, 100) <= (s32) objsetup->reverseChance[sp30]) {
                sKTData->playerSpotted = FALSE;
                dll_702_push_state(KT_LSTATE_11);
                return KT_LSTATE_4 + 1;
            }
            sKTData->flags |= KTFLAG_CHARGING;
        }
    }
    if ((sKTData->selfSegmentBitfield & sKTData->playerSegmentBitfield) && 
            ((!reversed && sKTData->segmentPos <= sKTData->playerSegmentPos) || (reversed && sKTData->playerSegmentPos <= sKTData->segmentPos))) {
        // Player is ahead of the boss, in the same segment. Charge!
        sKTData->chargeCounter = 1;
        dll_702_push_state(KT_LSTATE_5);
        sKTData->playerSpotted = TRUE;
        return KT_LSTATE_4 + 1;
    }
    return 0;
}

// offset: 0x3160 | func: 34
/** In this state during: turning corner */
static s32 dll_702_logic_state_3(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_2);
    } else if (fsa->unk33A != 0) {
        sKTData->segmentPos = dll_702_func_C74(self, sKTData);
        return dll_702_pop_state() + 1;
    }

    return 0;
}

// offset: 0x3208 | func: 35
// TODO: its not specific to roaring necessarily
/** In this state during: roaring at sabre (when spotted) */
static s32 dll_702_logic_state_4(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    KTrex_ObjSetup* objsetup;
    f32 var_ft1;
    u16 temp_t3;

    objsetup = (KTrex_ObjSetup*)self->setup;
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_4);
        sKTData->timer = (f32) objsetup->chargePrepTime[sKTData->playerSpotted];
    } else {
        sKTData->timer -= delayFloat;
        if (sKTData->timer < 0.0f) {
            sKTData->timer = 0.0f;
        }
        if ((fsa->unk33A != 0) && (sKTData->timer <= 0.0f)) {
            return dll_702_pop_state() + 1;
        }
    }
    return 0;
}

// offset: 0x3330 | func: 36
/** In this state during: charging at sabre (after spotted) */
static s32 dll_702_logic_state_5(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    KTrex_ObjSetup* objsetup;

    objsetup = (KTrex_ObjSetup*)self->setup;
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_1);
        sKTData->anger = 1;
        fsa->speed = objsetup->speeds[sKTData->anger] / 1000.0f;
    }
    if (dll_702_func_FE4(fsa, sKTData) != 0) {
        sKTData->chargeCounter -= 1;
    }
    if (sKTData->chargeCounter <= 0) {
        dll_702_push_state(KT_LSTATE_2);
        dll_702_push_state(KT_LSTATE_6);
        return KT_LSTATE_3 + 1;
    }
    if (dll_702_func_EF0(self) != 0) {
        return KT_LSTATE_7 + 1;
    }
    return 0;
}

// offset: 0x3490 | func: 37
/** In this state during: roaring (at end of charge) */
static s32 dll_702_logic_state_6(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_5);
    } else if (fsa->unk33A != 0) {
        return dll_702_pop_state() + 1;
    }

    return 0;
}

// offset: 0x3518 | func: 38
/** In this state during: zapped */
static s32 dll_702_logic_state_7(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_6);
        self->unk0xaf &= ~0x8;
    } else if (fsa->unk33A != 0) {
        return KT_LSTATE_8 + 1;
    }

    return 0;
}

// offset: 0x35A0 | func: 39
/** In this state during: on ground (after zapped) */
static s32 dll_702_logic_state_8(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    KTrex_ObjSetup* objsetup;

    objsetup = (KTrex_ObjSetup*)self->setup;
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_7);
        sKTData->timer = (f32) objsetup->vulnerableTime[sKTData->fightProgress >> 1];
        sKTData->flags |= KTFLAG_VULNERABLE;
        sKTData->flags &= ~KTFLAG_DAMAGED;
        self->unk0xaf &= ~8;
    } else {
        if ((sKTData->flags & KTFLAG_DAMAGED) || (sKTData->timer -= delayFloat) <= 0.0f) {
            if (sKTData->flags & KTFLAG_DAMAGED) {
                sKTData->health -= 1;
            }
            if (sKTData->health <= 0) {
                return KT_LSTATE_1 + 1;
            }
            self->unk0xaf |= 8;
            return KT_LSTATE_9 + 1;
        }
    }

    return 0;
}

// offset: 0x3720 | func: 40
/** In this state during: getting back up (after zapped, hit or not) */
static s32 dll_702_logic_state_9(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    u16 ktflags;
    
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_8);
    } else if (fsa->unk33A != 0) {
        if (sKTData->flags & KTFLAG_DAMAGED) {
            sKTData->fightProgress += 1;
            main_set_bits(BIT_572, sKTData->fightProgress);
        }
        ktflags = sKTData->flags;
        sKTData->standingUpSegment = KTFLAG_GET_SEGMENT(ktflags);
        sKTData->timer = 300.0f;
        gDLL_2_Camera->vtbl->func8(2, 0);
        return KT_LSTATE_10 + 1;
    }
    return 0;
}

// offset: 0x3828 | func: 41
// TODO: 
/** In this state during: after getting back up, charge around the whole arena? */
static s32 dll_702_logic_state_10(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    KTrex_ObjSetup* objsetup;
    s32 segment;
    s32 reversed;

    objsetup = (KTrex_ObjSetup*)self->setup;
    segment = KTFLAG_GET_SEGMENT(sKTData->flags);
    reversed = sKTData->flags & KTFLAG_REVERSED;
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_1);
        sKTData->anger = 2;
        fsa->speed = objsetup->speeds[sKTData->anger] / 1000.0f;
    }
    if (dll_702_func_FE4(fsa, sKTData) != 0) {
        dll_702_push_state(KT_LSTATE_10);
        return KT_LSTATE_3 + 1;
    }
    sKTData->timer -= delayFloat;
    if (sKTData->timer <= 0.0f) {
        sKTData->timer = 0.0f;
    }
    if ((sKTData->timer <= 0.0f) && (segment == sKTData->standingUpSegment) && 
            ((!reversed && sKTData->segmentPos >= 0.75f) || (reversed && sKTData->segmentPos <= 0.25f))) {
        if (sKTData->flags & KTFLAG_DAMAGED) {
            sKTData->fightProgress += 1;
            sKTData->playerSpotted = FALSE;
            dll_702_push_state(KT_LSTATE_11);
            dll_702_push_state(KT_LSTATE_4);
        } else {
            dll_702_push_state(KT_LSTATE_2);
        }
        gDLL_2_Camera->vtbl->func8(3, 0);
        main_set_bits(BIT_572, sKTData->fightProgress);
        return KT_LSTATE_6 + 1;
    }
    return 0;
}

// offset: 0x3AA0 | func: 42
static s32 dll_702_logic_state_11(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_3);
    } else if (fsa->unk33A != 0) {
        sKTData->flags ^= KTFLAG_REVERSED;
        if (sKTData->flags & KTFLAG_REVERSED) {
            sKTData->segNearX = sKTData->unk70;
            sKTData->segNearY = sKTData->unk80;
            sKTData->segNearZ = sKTData->unk90;
            sKTData->segFarX = sKTData->unkA0;
            sKTData->segFarY = sKTData->unkB0;
            sKTData->segFarZ = sKTData->unkC0;
        } else {
            sKTData->segNearX = sKTData->unk10;
            sKTData->segNearY = sKTData->unk20;
            sKTData->segNearZ = sKTData->unk30;
            sKTData->segFarX = sKTData->unk40;
            sKTData->segFarY = sKTData->unk50;
            sKTData->segFarZ = sKTData->unk60;
        }
        sKTData->segmentPos = dll_702_func_C74(self, sKTData);
        return KT_LSTATE_2 + 1;
    }

    return 0;
}

/*0x0*/ static const char str_0[] = "TREX: state stack overflow\n";
/*0x1C*/ static const char str_1C[] = "State Error!!!\n";
