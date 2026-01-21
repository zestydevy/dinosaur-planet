#include "libc/string.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/33.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/214_animobj.h"
#include "game/gamebits.h"
#include "sys/dll.h"
#include "sys/generic_stack.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "sys/main.h"
#include "sys/objanim.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "segment_334F0.h"
#include "sys/rand.h"
#include "dll.h"
#include "functions.h"
#include "prevent_bss_reordering.h"

enum KTLogicStates {
    KT_LSTATE_0 = 0,
    KT_LSTATE_1_DEFEATED = 1,
    KT_LSTATE_2_WALK = 2,
    KT_LSTATE_3_TURN_CORNER = 3,
    KT_LSTATE_4_ROAR = 4,
    KT_LSTATE_5_CHARGE = 5,
    KT_LSTATE_6_CHARGE_END = 6,
    KT_LSTATE_7_ZAPPED = 7,
    KT_LSTATE_8_ON_GROUND = 8,
    KT_LSTATE_9_STAND_UP = 9,
    KT_LSTATE_10_FULL_CHARGE = 10,
    KT_LSTATE_11_REVERSE = 11
};

enum KTAnimStates {
    KT_ASTATE_0 = 0,
    KT_ASTATE_1_MOVING_STRAIGHT = 1,
    KT_ASTATE_2_TURN_90 = 2,
    KT_ASTATE_3_TURN_180 = 3,
    KT_ASTATE_4_ROAR = 4,
    KT_ASTATE_5_CHARGE_END = 5,
    KT_ASTATE_6_ZAPPED = 6,
    KT_ASTATE_7_ON_GROUND = 7,
    KT_ASTATE_8_STAND_UP = 8
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
    // Whether the random chance reverse/charge was already rolled for the current logic state.
    KTFLAG_ROLLED_CHANCE = 0x20
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
        // for the A and B sides of each arena segment, for both clockwise and counter-clockwise movement.
/*010*/ f32 segStartCW_X[4];
/*020*/ f32 segStartCW_Y[4];
/*030*/ f32 segStartCW_Z[4];
/*040*/ f32 segEndCW_X[4];
/*050*/ f32 segEndCW_Y[4];
/*060*/ f32 segEndCW_Z[4];
/*070*/ f32 segEndCCW_X[4]; // Reversed positions start here (see above comments)
/*080*/ f32 segEndCCW_Y[4];
/*090*/ f32 segEndCCW_Z[4];
/*0A0*/ f32 segStartCCW_X[4];
/*0B0*/ f32 segStartCCW_Y[4];
/*0C0*/ f32 segStartCCW_Z[4];
        // The current A and B positions of each segment.
        // These are flipped when the boss is reversed (moving counter-clockwise).
/*0D0*/ f32 *segA_X;
/*0D4*/ f32 *segA_Y;
/*0D8*/ f32 *segA_Z;
/*0DC*/ f32 *segB_X;
/*0E0*/ f32 *segB_Y;
/*0E4*/ f32 *segB_Z;
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
/*0FD*/ u8 roarType;
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
/*108*/ u8 breathingSfxIndex;
/*10C*/ SRT unk10C;
/*124*/ SRT unk124;
/*13C*/ SRT unk13C;
/*154*/ SRT unk154;
/*16C*/ Vec3f unk16C;
} KTrex_Data;

typedef struct {
/*00*/ DLL33_ObjSetup base;
/*38*/ f32 speeds[3]; // Movement speed, per "anger" level.
/*44*/ u16 roarTime[3]; // How long a roar lasts.
/*4A*/ u16 vulnerableTime[4]; // How long the boss is vulnerable for.
/*52*/ u8 reverseChance[4]; // Chance to reverse direction.
/*56*/ u8 chargeChance[4]; // Chance to charge without seeing Sabre.
} KTrex_ObjSetup;

enum KTModAnims {
    KTANIM_Idle = 0,
    KTANIM_Walk1 = 1, // anger level 0
    KTANIM_Walk2 = 2, // anger level 1
    KTANIM_Walk3 = 3, // anger level 2
    KTANIM_Roar_Small = 4,
    KTANIM_Roar_VeryBig = 5, // unused
    KTANIM_Roar_Big = 6,
    KTANIM_Walk_Slow = 7, // creepy, unused
    KTANIM_Turn_90_CW_Normal = 8,
    KTANIM_Roar_ChargeEnd1 = 9, // referenced but unused due to logic
    KTANIM_Roar_10 = 10, // unused
    KTANIM_FallDown = 11,
    KTANIM_FlailOnGround = 12,
    KTANIM_GetBackUp = 13,
    KTANIM_Turn_90_CCW_Normal = 14,
    KTANIM_Turn_180 = 15,
    KTANIM_Turn_90_CW_Fast = 16,
    KTANIM_Turn_90_CCW_Fast = 17,
    KTANIM_Roar_ChargeEnd2 = 18
};

enum KTModAnimEvents {
    KTANIM_EVT_0_Speak = 0,
    KTANIM_EVT_1_Footfall_Left = 1,
    KTANIM_EVT_2_Footfall_Right = 2,
    KTANIM_EVT_7 = 7,
    KTANIM_EVT_9_Spit_Partfx_Enable = 9,
    KTANIM_EVT_10_Spit_Partfx_Disable = 10
};

enum KTFxFlags {
    KTFX_Footfall_Right1 = 0x1,
    KTFX_Footfall_Left1 = 0x2,
    KTFX_Footfall_Right2 = 0x4,
    KTFX_Footfall_Left2 = 0x8,
    KTFX_Footfall_Right3 = 0x10,
    KTFX_Footfall_Left3 = 0x20,
    KTFX_Sound_68E = 0x40,
    KTFX_Sound_BigRoar = 0x80,
    KTFX_Sound_IntroRoar = 0x100,
    KTFX_Sound_ChargeEndRoar = 0x200,
    KTFX_Spit_Partfx = 0x800,
    KTFX_Spit_Partfx_Disable = 0x1000,
    KTFX_Sound_FlailRoar = 0x2000,
    KTFX_Sound_Breathing1 = 0x4000,
    KTFX_Sound_Breathing2 = 0x8000,
    KTFX_Sound_WallSlam = 0x10000,
    KTFX_Sound_Explosion = 0x20000,
    KTFX_Sound_GroundScrape = 0x40000,
    KTFX_Sound_PainRoar = 0x80000,
    KTFX_EnablePartFx = 0x100000
};

/*0x0*/ static u32 _data_0[] = {
    0x00000002, 0x00000002
};
/*0x8*/ static s16 sChargeEndModAnims[] = { // mod anim indices (charge end)
    KTANIM_Roar_ChargeEnd1, KTANIM_Roar_ChargeEnd2, KTANIM_Roar_ChargeEnd2, 0
};
/*0x10*/ static s16 sWalkModAnims[] = { // mod anim indices (walking)
    KTANIM_Walk1, KTANIM_Walk2, KTANIM_Walk3, 0
};
/*0x18*/ static s16 sRoarModAnims[] = { // mod anim indices (roars)
    KTANIM_Roar_Small, KTANIM_Roar_Big, KTANIM_Roar_Big, 0
};
/*0x20*/ static s16 sTurn90ModAnims[3][2] = { // mod anim indices (90 degree turns (CW, CCW))
    { KTANIM_Turn_90_CW_Normal, KTANIM_Turn_90_CCW_Normal }, 
    { KTANIM_Turn_90_CW_Fast, KTANIM_Turn_90_CCW_Fast }, 
    { KTANIM_Turn_90_CW_Fast, KTANIM_Turn_90_CCW_Fast }
};
/*0x2C*/ static u16 sFootfallRightFxFlags[] = {
    KTFX_Footfall_Right1, KTFX_Footfall_Right2, KTFX_Footfall_Right3, 0x0000
};
/*0x34*/ static u16 sFootfallLeftFxFlags[] = {
    KTFX_Footfall_Left1, KTFX_Footfall_Left2, KTFX_Footfall_Left3, 0x0000
};
/*0x3C*/ static u16 sBreathingFxFlags1[] = {
    KTFX_Sound_Breathing1, KTFX_Sound_Breathing1, KTFX_Sound_Breathing1, 0x0000
};
/*0x44*/ static u16 sBreathingFxFlags2[] = {
    KTFX_Sound_Breathing2, KTFX_Sound_Breathing2, KTFX_Sound_Breathing2, 0x0000
};
/*0x4C*/ static u16 sRoarFxFlags[] = {
    KTFX_Sound_68E, KTFX_Sound_BigRoar, KTFX_Sound_IntroRoar, 0x0000
};
/*0x54*/ static f32 sRoarAnimTickDeltas[] = {
    0.006, 0.003, 0.003
};
/*0x60*/ static f32 sTurn90AnimTickDeltas[] = {
    0.0055, 0.012, 0.012
};
/*0x6C*/ static f32 _data_6C[] = { // Segment progress to turn at when near the start of a segment.
    0, 0.025, 0.025
};
/*0x78*/ static f32 _data_78[] = { // Segment progress to turn at when near the end of a segment.
    1, 0.975, 0.975
};
/*0x84*/ static u16 sSndScrapeGround[2] = {SOUND_6FA, SOUND_6FB};
/*0x88*/ static u16 sSndRoars[3] = {SOUND_691_KT_Rex_Roar, SOUND_6FD_KT_Rex_Roar, SOUND_6FE_KT_Rex_Roar};
/*0x90*/ static u16 sSndDeflectAttack[2] = {SOUND_25B_Magic_Attack_Deflected, SOUND_25C_Melee_Attack_Deflected};
/*0x94*/ static u32 sSegEndCurvesCW[] = { // curve uIDs
    0x32136, // end of segment 0 (cw), more in
    0x3213d, // end of segment 1 (cw), more in
    0x32128, // end of segment 2 (cw), more in
    0x3212f  // end of segment 3 (cw), more in
};
/*0xA4*/ static u32 sSegStartCurvesCW[] = { // curve uIDs
    0x32130, // start of segment 0 (cw), more out
    0x32137, // start of segment 1 (cw), more out
    0x32123, // start of segment 2 (cw), more out 
    0x32129  // start of segment 3 (cw), more out
};
/*0xB4*/ static u32 sSegStartCurvesCCW[] = { // curve uIDs
    0x32367, // end of segment 0 (cw), more out
    0x32369, // end of segment 1 (cw), more out
    0x3236a, // end of segment 2 (cw), more out
    0x32365  // end of segment 3 (cw), more out
};
/*0xC4*/ static u32 sSegEndCurvesCCW[] = { // curve uIDs
    0x32132, // start of segment 0 (cw), more in
    0x32139, // start of segment 1 (cw), more in 
    0x32124, // start of segment 2 (cw), more in 
    0x3212b  // start of segment 3 (cw), more in
};
/*0xD4*/ static s16 sPlayerSegmentGamebits[] = {
    BIT_KT_Player_In_Segment_2, BIT_KT_Player_In_Segment_0, BIT_KT_Player_In_Segment_3, BIT_KT_Player_In_Segment_1
};
/*0xDC*/ static s16 sLaserWallGamebits[] = {
    BIT_KT_Lazerwall_1, BIT_KT_Lazerwall_2, BIT_KT_Lazerwall_3, BIT_KT_Lazerwall_4
};
/*0xE4*/ static DLL_IModgfx *_data_E4 = NULL;

/*0x0*/ static ObjFSA_StateCallback sAnimStateCallbacks[9];
/*0x28*/ static ObjFSA_StateCallback sLogicStateCallbacks[12];
/*0x58*/ static DLL33_Data* sDLL33Data;
/*0x5C*/ static KTrex_Data *sKTData;

static s32 dll_702_anim_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_anim_state_1(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_anim_state_2(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_anim_state_3(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_anim_state_4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_anim_state_5(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_anim_state_6(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_anim_state_7(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_anim_state_8(Object* self, ObjFSA_Data* fsa, f32 updateRate);

static s32 dll_702_logic_state_0(Object *self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_logic_state_1(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_logic_state_2(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_logic_state_3(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_logic_state_4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_logic_state_5(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_logic_state_6(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_logic_state_7(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_logic_state_8(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_logic_state_9(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_logic_state_10(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 dll_702_logic_state_11(Object* self, ObjFSA_Data* fsa, f32 updateRate);

static s32 dll_702_get_player_segment_bitfield(void);
static u8 dll_702_get_self_segment_bitfield(u16 ktflags);
static f32 dll_702_get_obj_segment_pos(Object* obj, KTrex_Data* ktdata);
static s32 dll_702_get_laser_wall_bitfield(u8 segmentBitfield);
/*static*/ int dll_702_func_119C(Object* a0, Object* a1, AnimObj_Data* a2, s8 a3);
static void dll_702_fx_tick(Object* self);
static void dll_702_func_1EF0(Object* self, ObjFSA_Data* fsa);

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
    self->animCallback = dll_702_func_119C;
    gDLL_18_objfsa->vtbl->set_anim_state(self, &objdata->fsa, KT_ASTATE_0);
    objdata->fsa.logicState = KT_LSTATE_2_WALK; // set initial state
    objdata->fsa.target = NULL;
    objdata->fsa.unk4.mode = 0;
    objdata->fsa.unk33D = 0;
    self->unkAF |= 0x88;
    func_8002674C(self);
    if (self->shadow != NULL) {
        self->shadow->flags |= (OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR);
    }
    ktdata = (KTrex_Data*)objdata->unk3F4;
    ktdata->stateStack = generic_stack_new(4, sizeof(s32));

    for (i = 0; i < 4; i++) {
        curve = gDLL_26_Curves->vtbl->func_39C(sSegStartCurvesCW[i]);
        if (curve != NULL) {
            ktdata->segStartCW_X[i] = curve->pos.x;
            ktdata->segStartCW_Y[i] = curve->pos.y;
            ktdata->segStartCW_Z[i] = curve->pos.z;
            curve = gDLL_26_Curves->vtbl->func_39C(sSegEndCurvesCW[i]);
            ktdata->segEndCW_X[i] = curve->pos.x;
            ktdata->segEndCW_Y[i] = curve->pos.y;
            ktdata->segEndCW_Z[i] = curve->pos.z;
            curve = gDLL_26_Curves->vtbl->func_39C(sSegEndCurvesCCW[i]);
            ktdata->segEndCCW_X[i] = curve->pos.x;
            ktdata->segEndCCW_Y[i] = curve->pos.y;
            ktdata->segEndCCW_Z[i] = curve->pos.z;
            curve = gDLL_26_Curves->vtbl->func_39C(sSegStartCurvesCCW[i]);
            ktdata->segStartCCW_X[i] = curve->pos.x;
            ktdata->segStartCCW_Y[i] = curve->pos.y;
            ktdata->segStartCCW_Z[i] = curve->pos.z;
        }
    }
    
    ktdata->segA_X = ktdata->segStartCW_X;
    ktdata->segA_Y = ktdata->segStartCW_Y;
    ktdata->segA_Z = ktdata->segStartCW_Z;
    ktdata->segB_X = ktdata->segEndCW_X;
    ktdata->segB_Y = ktdata->segEndCW_Y;
    ktdata->segB_Z = ktdata->segEndCW_Z;
    ktdata->health = 4;
    _data_E4 = (DLL_IModgfx*)dll_load_deferred(DLL_ID_106, 1);
}
#endif

// offset: 0x448 | func: 2 | export: 1
void dll_702_control(Object* self) {
    DLL33_Data* dll33Data;
    f32 vec[3];

    if (self->unkDC == 0) {
        dll33Data = (DLL33_Data*)self->data;
        sDLL33Data = dll33Data;
        sKTData = sDLL33Data->unk3F4;
        func_80028D2C(self);
        dll33Data->fsa.target = get_player();
        if (dll33Data->fsa.target != NULL) {
            vec[0] = dll33Data->fsa.target->positionMirror.x - self->positionMirror.x;
            vec[1] = dll33Data->fsa.target->positionMirror.y - self->positionMirror.y;
            vec[2] = dll33Data->fsa.target->positionMirror.z - self->positionMirror.z;
            dll33Data->fsa.targetDist = sqrtf(SQ(vec[0]) + SQ(vec[1]) + SQ(vec[2]));
        }
        func_80032A08(self, &sDLL33Data->unk3BC);
        sKTData->playerSegmentBitfield = dll_702_get_player_segment_bitfield();
        sKTData->playerSegmentPos = dll_702_get_obj_segment_pos(dll33Data->fsa.target, sKTData);
        sKTData->selfSegmentBitfield = dll_702_get_self_segment_bitfield(sKTData->flags);
        sKTData->laserWallBitfield = dll_702_get_laser_wall_bitfield(sKTData->selfSegmentBitfield);
        gDLL_33->vtbl->func20(self, &dll33Data->fsa, &sDLL33Data->unk34C, sDLL33Data->unk39E, &sDLL33Data->unk3B4, 2, 2, 0);
        if (sKTData->flags & KTFLAG_VULNERABLE) {
            dll33Data->fsa.unk348 = 2;
        } else {
            dll33Data->fsa.unk348 = 0;
        }
        dll_702_func_1EF0(self, &dll33Data->fsa);
        dll_702_fx_tick(self);
        gDLL_33->vtbl->func10(self, &dll33Data->fsa, 0.0f, 0);
        func_80026128(self, 0x17, 1, -1);
        gDLL_18_objfsa->vtbl->tick(self, &dll33Data->fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
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
    if ((visibility != 0) && (self->unkDC == 0)) {
        if (sDLL33Data->unk3E8 != 0.0f) {
            func_80036FBC(0xC8, 0, 0, sDLL33Data->unk3E8);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        func_80031F6C(self, 1, &sKTData->unk124.transl.x, &sKTData->unk124.transl.y, &sKTData->unk124.transl.z, 0);
        func_80031F6C(self, 2, &sKTData->unk13C.transl.x, &sKTData->unk13C.transl.y, &sKTData->unk13C.transl.z, 0);
        func_80031F6C(self, 3, &sKTData->unk154.transl.x, &sKTData->unk154.transl.y, &sKTData->unk154.transl.z, 0);
        func_80031F6C(self, 0, &sKTData->unk10C.transl.x, &sKTData->unk10C.transl.y, &sKTData->unk10C.transl.z, 0);
        memcpy(&sp48, func_80032170(self, 4), sizeof(MtxF));
        sp48.m[3][1] = 0.0f;
        sp48.m[3][0] = 0.0f;
        sp48.m[3][2] = 0.0f;
        sKTData->unk16C.x = (f32) ((f32) rand_next(-0x32, 0x32) * 0.1f);
        sKTData->unk16C.y = (f32) ((f32) rand_next(0x3C, 0x78) * 0.1f);
        sKTData->unk16C.z = (f32) ((f32) rand_next(0x64, 0x96) * -0.25f);
        vec3_transform(&sp48, sKTData->unk16C.x, sKTData->unk16C.y, sKTData->unk16C.z, &sKTData->unk16C.x, &sKTData->unk16C.y, &sKTData->unk16C.z);
        sKTData->fxFlags |= KTFX_EnablePartFx;
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
static s32 dll_702_get_player_segment_bitfield(void) {
    s32 i;
    u8 var_s2;

    var_s2 = 0;
    for (i = 0;  i < 4; i++) {
        if (main_get_bits(sPlayerSegmentGamebits[i]) != 0) {
            var_s2 |= (1 << i);
        }
    }
    return var_s2;
}

// offset: 0xC2C | func: 11
static u8 dll_702_get_self_segment_bitfield(u16 ktflags) {
  s8 segmentToBitfield[4] = {0x02, 0x08, 0x01, 0x04};

  // segment 0 -> player bit 1
  // segment 1 -> player bit 3
  // segment 2 -> player bit 0
  // segment 3 -> player bit 2

  return segmentToBitfield[KTFLAG_GET_SEGMENT(ktflags)];
}

// offset: 0xC74 | func: 12
static f32 dll_702_get_obj_segment_pos(Object* obj, KTrex_Data* ktdata) {
    s32 _pad;
    s32 segment;
    f32 segmentLength;
    f32 zDiff;
    f32 xDiff;

    segment = KTFLAG_GET_SEGMENT(ktdata->flags);
    xDiff = ktdata->segB_X[segment] - ktdata->segA_X[segment];
    zDiff = ktdata->segB_Z[segment] - ktdata->segA_Z[segment];
    segmentLength = sqrtf(SQ(xDiff) + SQ(zDiff));
    xDiff = obj->srt.transl.x - ktdata->segA_X[segment];
    zDiff = obj->srt.transl.z - ktdata->segA_Z[segment];
    return sqrtf(SQ(xDiff) + SQ(zDiff)) / segmentLength;
}

// offset: 0xD5C | func: 13
static s32 dll_702_get_laser_wall_bitfield(u8 segmentBitfield) {
    s32 mask;
    s32 i;
    u8 var_s2;

    var_s2 = 0;
    
    for (i = 0; i < 4; i++) {
        mask = 1 << i;
        if ((segmentBitfield & mask) && (main_get_bits(sLaserWallGamebits[i]) != 0)) {
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
static s32 dll_702_is_in_active_laser_wall(Object* self) {
    f32 temp_fv1;
    f32 temp_fv1_2;

    if (sKTData->laserWallBitfield == 0) {
        return FALSE;
    }
    switch (sKTData->laserWallBitfield) {
    case 1:
    case 2:
        temp_fv1 = self->srt.transl.z - 150.0f;
        temp_fv1_2 = self->srt.transl.z + 150.0f;
        if ((temp_fv1 > -8960.0f) || (temp_fv1_2 < -8960.0f)) {
            return FALSE;
        }
        return TRUE;
    case 4:
    case 8:
        temp_fv1 = self->srt.transl.x - 150.0f;
        temp_fv1_2 = self->srt.transl.x + 150.0f;
        if ((temp_fv1 > -13470.0f) || (temp_fv1_2 < -13470.0f)) {
            return FALSE;
        }
        return TRUE;
    default:
        return FALSE;
    }
}

// offset: 0xFE4 | func: 17
/** 
 * Returns true if turned at the end of a segment. 
 * Note: This function has the side effect of changing the active segment if a turn is detected.
 *       The caller is expected to handle the rest of the turn (i.e. state transitions).
 */
static s32 dll_702_move_and_check_turn(ObjFSA_Data* fsa, KTrex_Data* ktdata) {
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

    ktdata->segmentPos += posDelta * gUpdateRateF;

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
    
    ktdata->pos.x = ktdata->segA_X[segment] + ((ktdata->segB_X[segment] - ktdata->segA_X[segment]) * ktdata->segmentPos);
    ktdata->pos.y = ktdata->segA_Y[segment] + ((ktdata->segB_Y[segment] - ktdata->segA_Y[segment]) * ktdata->segmentPos);
    ktdata->pos.z = ktdata->segA_Z[segment] + ((ktdata->segB_Z[segment] - ktdata->segA_Z[segment]) * ktdata->segmentPos);
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
            sKTData->fxFlags |= KTFX_Footfall_Right2;
            break;
        case 2:
            sKTData->fxFlags |= KTFX_Footfall_Left2;
            break;
        case 3:
            sKTData->fxFlags |= KTFX_Spit_Partfx;
            break;
        case 4:
            sKTData->fxFlags |= KTFX_Spit_Partfx_Disable;
            break;
        case 5:
            sKTData->fxFlags |= KTFX_Sound_Explosion;
            break;
        default:
            break;
        }
    }
    dll_702_fx_tick(a0);
    return 0;
}
#endif

// offset: 0x12DC | func: 19
static void dll_702_fx_tick(Object *self) {
    s32 i;
    f32 sp48; // camera shake amount?

    sp48 = 1.0f - (sDLL33Data->fsa.targetDist / 2000.0f);
    if (sp48 < 0.0f) {
        sp48 = 0.0f;
    } else if (sp48 > 1.0f) {
        sp48 = 1.0f;
    }
    if (sKTData->fxFlags & KTFX_Sound_68E) {
        // the little sound he makes turning a corner
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_68E_KT_Rex_Noise, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & KTFX_Sound_BigRoar) {
        // the big roar
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_68F_KT_Rex_Roar, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & KTFX_Sound_IntroRoar) {
        // intro cutscene roar
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_690_KT_Rex_Roar, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & KTFX_Sound_ChargeEndRoar) {
        // unused roar on "charge end" anim
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_68D_KT_Rex_Roar_Kinda, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & KTFX_Sound_WallSlam) {
        // wall slam on charge turn
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_6FC_KT_Rex_Slam, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & KTFX_Sound_GroundScrape) {
        // scraping ground, while knocked over
        gDLL_6_AMSFX->vtbl->play_sound(self, sSndScrapeGround[rand_next(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & KTFX_Sound_PainRoar) {
        // roar from getting zapped
        gDLL_6_AMSFX->vtbl->play_sound(self, sSndRoars[2], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & KTFX_Sound_FlailRoar) {
        // roar, flailing on ground
        gDLL_6_AMSFX->vtbl->play_sound(self, sSndRoars[rand_next(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    if (sKTData->fxFlags & KTFX_Spit_Partfx_Disable) {
        sKTData->fxFlags = sKTData->fxFlags & ~(KTFX_Spit_Partfx_Disable | KTFX_Spit_Partfx);
    }
    if (sKTData->fxFlags & KTFX_Sound_Explosion) {
        // explosion sound, when he falls on the ground. also during intro
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_6F9_Explosion, MAX_VOLUME, NULL, NULL, 0, NULL);
        func_800013BC();
        func_80003B70(2.0f * sp48);
    }
    if (sKTData->fxFlags & KTFX_Sound_Breathing1) {
        // breathing
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_68B_KT_Rex_Breathing1, MAX_VOLUME, NULL, NULL, 0, NULL);
        sKTData->breathingSfxIndex ^= 1;
    }
    if (sKTData->fxFlags & KTFX_Sound_Breathing2) {
        // more breathing
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_68C_KT_Rex_Breathing2, MAX_VOLUME, NULL, NULL, 0, NULL);
        sKTData->breathingSfxIndex ^= 1;
    }
    if (sKTData->fxFlags & (KTFX_Footfall_Right1 | KTFX_Footfall_Left1)) {
        // footfall
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_688_KT_Rex_Stomp, MAX_VOLUME, NULL, NULL, 0, NULL);
        if (sp48 > 0.1f) {
            func_800013BC();
            func_80003B70(sp48);
            main_set_bits(BIT_554, 1);
        }
    }
    if (sKTData->fxFlags & (KTFX_Footfall_Right2 | KTFX_Footfall_Left2)) {
        // footfall
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_689_KT_Rex_Stomp, MAX_VOLUME, NULL, NULL, 0, NULL);
        if (sp48 > 0.1f) {
            func_800013BC();
            func_80003B70(2.0f * sp48);
            main_set_bits(BIT_554, 1);
        }
    }
    if (sKTData->fxFlags & (KTFX_Footfall_Right3 | KTFX_Footfall_Left3)) {
        // footfall
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_68A_KT_Rex_Stomp, MAX_VOLUME, NULL, NULL, 0, NULL);
        if (sp48 > 0.1f) {
            func_800013BC();
            func_80003B70(3.0f * sp48);
            main_set_bits(BIT_554, 1);
        }
    }
    if (!(sKTData->fxFlags & KTFX_EnablePartFx)) {
        sKTData->fxFlags &= (KTFX_Spit_Partfx_Disable | KTFX_Spit_Partfx);
        return;
    }
    if (sKTData->fxFlags & KTFX_Footfall_Right1) {
        sKTData->unk124.scale = 1.0f;
        for (i = 0; i != 0xA; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_483, &sKTData->unk124, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_483, &sKTData->unk124, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_484, &sKTData->unk124, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
        }
    }
    if (sKTData->fxFlags & KTFX_Footfall_Left1) {
        sKTData->unk13C.scale = 1.0f;
        for (i = 0; i != 0xA; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_483, &sKTData->unk13C, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_483, &sKTData->unk13C, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_484, &sKTData->unk13C, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
        }
    }
    if (sKTData->fxFlags & KTFX_Footfall_Right2) {
        sKTData->unk124.scale = 1.5f;
        for (i = 0; i != 0xD; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_483, &sKTData->unk124, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_483, &sKTData->unk124, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_484, &sKTData->unk124, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
        }
    }
    if (sKTData->fxFlags & KTFX_Footfall_Left2) {
        sKTData->unk13C.scale = 1.5f;
        for (i = 0; i != 0xD; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_483, &sKTData->unk13C, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_483, &sKTData->unk13C, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_484, &sKTData->unk13C, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
        }
    }
    if (sKTData->fxFlags & KTFX_Footfall_Right3) {
        sKTData->unk124.scale = 2.0f;
        for (i = 0; i != 0x10; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_483, &sKTData->unk124, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_483, &sKTData->unk124, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_484, &sKTData->unk124, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
        }
    }
    if (sKTData->fxFlags & KTFX_Footfall_Left3) {
        sKTData->unk13C.scale = 2.0f;
        for (i = 0; i != 0x10; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_483, &sKTData->unk13C, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_483, &sKTData->unk13C, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_484, &sKTData->unk13C, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
        } 
    }
    if (sKTData->fxFlags & KTFX_Spit_Partfx) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_487, &sKTData->unk10C, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, &sKTData->unk16C);
    }
    sKTData->fxFlags &= (KTFX_Spit_Partfx_Disable | KTFX_Spit_Partfx);
}

// offset: 0x1E9C | func: 20
static void dll_702_anim_event_to_fx(s32 modAnimEvent, s32 fxFlags) {
    s32 mask;

    mask = 1 << modAnimEvent;
    if (sDLL33Data->fsa.unk308 & mask) {
        sDLL33Data->fsa.unk308 &= ~mask;
        sKTData->fxFlags |= fxFlags;
    }
}

// offset: 0x1EF0 | func: 21
static void dll_702_func_1EF0(Object* self, ObjFSA_Data* fsa) {
    static SRT _bss_60;
    DLL33_Data* dll33Data = (DLL33_Data*)self->data;
    s32 sp60;
    s32 sp5C;
    s32 sp58;
    Object* sp54;
    MtxF* temp_v1; // TODO: maybe not a matrix?
    ModelInstance* modelInst;
    u32 sp3C[] = {0x00000006, 0x00000069, 0x00000069, 0x000000ff};

    if (dll33Data->unk3E8 > 0.0f) {
        dll33Data->unk3E8 += (gUpdateRateF * dll33Data->unk3EC);
        if (dll33Data->unk3E8 < 0.0f) {
            dll33Data->unk3E8 = 0.0f;
        } else if (dll33Data->unk3E8 > 120.0f) {
            dll33Data->unk3E8 = 120.0f - (dll33Data->unk3E8 - 120.0f);
            dll33Data->unk3EC = -dll33Data->unk3EC;
        }
    }
    sp60 = func_80025F40(self, &sp54, &sp5C, &sp58);
    if (sp60 != 0) {
        modelInst = self->modelInsts[self->modelInstIdx];
        temp_v1 = modelInst->unk24;
        if ((fsa->unk348 != 0) && ((sp5C == 3) || (sp5C == 2))) {
            _bss_60.transl.x = temp_v1->m[sp5C][1] + gWorldX;
            _bss_60.transl.y = temp_v1->m[sp5C][2];
            _bss_60.transl.z = temp_v1->m[sp5C][3] + gWorldZ;
            gDLL_6_AMSFX->vtbl->play_sound(self, sSndRoars[2], MAX_VOLUME, NULL, NULL, 0, NULL);
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_693_Explosion, MAX_VOLUME, NULL, NULL, 0, NULL);
            gDLL_2_Camera->vtbl->func8(2, 0);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_4B2, &_bss_60, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_4B3, &_bss_60, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            sKTData->flags &= ~KTFLAG_VULNERABLE;
            sKTData->flags |= KTFLAG_DAMAGED;
            fsa->unk343 = (s8) sp60;
            fsa->unk348 -= 1;
        } else {
            gDLL_6_AMSFX->vtbl->play_sound(self, sSndDeflectAttack[rand_next(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
            modelInst = self->modelInsts[self->modelInstIdx];
            temp_v1 = modelInst->unk24;
            _bss_60.transl.x = temp_v1->m[sp5C][1] + gWorldX;
            _bss_60.transl.y = temp_v1->m[sp5C][2];
            _bss_60.transl.z = temp_v1->m[sp5C][3] + gWorldZ;
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_328, &_bss_60, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
            _bss_60.transl.x -= self->positionMirror.x;
            _bss_60.transl.y -= self->positionMirror.y;
            _bss_60.transl.z -= self->positionMirror.z;
            _bss_60.scale = 1.0f;
            _bss_60.yaw = 0;
            _bss_60.pitch = 0;
            _bss_60.roll = 0;
            sp3C[1] += rand_next(0, 0x9B);
            sp3C[2] += rand_next(0, 0x9B);
            _data_E4->vtbl->func0(self, 0, &_bss_60, 1, -1, &sp3C);
        }
        if (fsa->unk348 <= 0) {
            fsa->unk348 = 0;
        }
        obj_send_mesg(sp54, 0xE0001, self, NULL);
    }
}

// offset: 0x23EC | func: 22
static s32 dll_702_anim_state_0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, KTANIM_Idle, 0.0f, 0);
    }
    fsa->animTickDelta = 0.01f;
    return 0;
}

// offset: 0x2454 | func: 23
/** Walking straight through arena segment. */
static s32 dll_702_anim_state_1(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    f32 temp_fa1;
    f32 temp_fv0;
    s32 speakFxFlags;

    if (fsa->enteredAnimState) {
        func_80023D30(self, sWalkModAnims[sKTData->anger], 0.0f, 0);
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
    }
    dll_702_anim_event_to_fx(KTANIM_EVT_2_Footfall_Right, sFootfallRightFxFlags[sKTData->anger]); // on event 2 -> right footfall
    dll_702_anim_event_to_fx(KTANIM_EVT_1_Footfall_Left, sFootfallLeftFxFlags[sKTData->anger]); // on event 1 -> left footfall
    if (sKTData->breathingSfxIndex != 0) {
        speakFxFlags = sBreathingFxFlags1[sKTData->anger];
    } else {
        speakFxFlags = sBreathingFxFlags2[sKTData->anger];
    }
    dll_702_anim_event_to_fx(KTANIM_EVT_0_Speak, speakFxFlags); // on event 0 -> breathing sfx
    temp_fv0 = (sKTData->pos.x - self->srt.transl.x) * gUpdateRateInverseF;
    temp_fa1 = (sKTData->pos.z - self->srt.transl.z) * gUpdateRateInverseF;
    func_8002493C(self, sqrtf(SQ(temp_fv0) + SQ(temp_fa1)), &fsa->animTickDelta);
    self->srt.transl.x = sKTData->pos.x;
    self->srt.transl.z = sKTData->pos.z;
    return 0;
}

// offset: 0x2644 | func: 24
/** 90 degree turn. */
static s32 dll_702_anim_state_2(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    MtxF tempMtx;
    f32 tempY;
    SRT tempSRT;
    u16 reversed;

    reversed = sKTData->flags & KTFLAG_REVERSED;
    if (fsa->enteredAnimState) {
        func_80023D30(self, sTurn90ModAnims[sKTData->anger & 0xFFFF][reversed], 0.0f, 0);
        fsa->animTickDelta = sTurn90AnimTickDeltas[sKTData->anger];
        sKTData->turnStartYaw = self->srt.yaw;
    }
    dll_702_anim_event_to_fx(KTANIM_EVT_2_Footfall_Right, KTFX_Footfall_Right1); // on event 2 -> right footfall
    dll_702_anim_event_to_fx(KTANIM_EVT_1_Footfall_Left, KTFX_Footfall_Left1); // on event 1 -> left footfall
    dll_702_anim_event_to_fx(KTANIM_EVT_0_Speak, KTFX_Sound_68E); // on event 1 -> little noise he makes on the corner
    dll_702_anim_event_to_fx(KTANIM_EVT_7, KTFX_Sound_WallSlam); // on event 7 -> wall slam fx
    fsa->unk340 |= 1;
    gDLL_18_objfsa->vtbl->func7(self, fsa, gUpdateRateF, 3);
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
        self->srt.yaw = (f32) sKTData->turnStartYaw + (16384.0f * self->animProgress);
    } else {
        self->srt.yaw = (f32) sKTData->turnStartYaw - (16384.0f * self->animProgress);
    }
    return 0;
}

// offset: 0x28BC | func: 25
/** 180 degree turn. */
static s32 dll_702_anim_state_3(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    u16 reversed;

    reversed = sKTData->flags & KTFLAG_REVERSED;
    if (fsa->enteredAnimState) {
        func_80023D30(self, KTANIM_Turn_180, 0.0f, 0);
        fsa->animTickDelta = 0.005f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
        sKTData->turnStartYaw = self->srt.yaw;
    }
    if (reversed) {
        self->srt.yaw = (f32) sKTData->turnStartYaw + (32768.0f * self->animProgress);
    } else {
        self->srt.yaw = (f32) sKTData->turnStartYaw - (32768.0f * self->animProgress);
    }
    return 0;
}

// offset: 0x29D0 | func: 26
/** Roar, standing in place. */
static s32 dll_702_anim_state_4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, sRoarModAnims[sKTData->roarType], 0.0f, 0);
        fsa->animTickDelta = sRoarAnimTickDeltas[sKTData->roarType];
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
    }
    dll_702_anim_event_to_fx(KTANIM_EVT_0_Speak, sRoarFxFlags[sKTData->roarType]);
    dll_702_anim_event_to_fx(KTANIM_EVT_9_Spit_Partfx_Enable, KTFX_Spit_Partfx); // on event 9 -> enable spit partfx
    dll_702_anim_event_to_fx(KTANIM_EVT_10_Spit_Partfx_Disable, KTFX_Spit_Partfx_Disable); // on event 10 -> disable spit partfx
    return 0;
}

// offset: 0x2AF0 | func: 27
/** Roar, after completing a charge. */
static s32 dll_702_anim_state_5(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, sChargeEndModAnims[sKTData->anger], 0.0f, 0);
        fsa->animTickDelta = 0.005f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
    }
    // @bug: this event never plays with the above mod anim. the boss ends up doing a roar anim
    //       but with no sound here. the sound is a shorter quieter roar.
    dll_702_anim_event_to_fx(KTANIM_EVT_0_Speak, KTFX_Sound_ChargeEndRoar);
    return 0;
}

// offset: 0x2BA4 | func: 28
/** Zapped, fall down. */
static s32 dll_702_anim_state_6(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, KTANIM_FallDown, 0.0f, 0);
        fsa->animTickDelta = 0.006f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
    }
    dll_702_anim_event_to_fx(KTANIM_EVT_0_Speak, KTFX_Sound_PainRoar);
    dll_702_anim_event_to_fx(KTANIM_EVT_7, KTFX_Sound_Explosion);
    return 0;
}

// offset: 0x2C54 | func: 29
/** Knocked over on the ground. */
static s32 dll_702_anim_state_7(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, KTANIM_FlailOnGround, 0.0f, 0);
        fsa->animTickDelta = 0.01f;
    }
    dll_702_anim_event_to_fx(KTANIM_EVT_0_Speak, KTFX_Sound_FlailRoar);
    dll_702_anim_event_to_fx(KTANIM_EVT_7, KTFX_Sound_GroundScrape);
    return 0;
}

// offset: 0x2CF8 | func: 30
/** Standing back up, after being zapped and knocked down. */
static s32 dll_702_anim_state_8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        func_80023D30(self, KTANIM_GetBackUp, 0.0f, 0);
        fsa->animTickDelta = 0.0017f;
    }
    dll_702_anim_event_to_fx(KTANIM_EVT_0_Speak, KTFX_Sound_FlailRoar);
    return 0;
}

// offset: 0x2D80 | func: 31
static s32 dll_702_logic_state_0(Object *self, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
}

// offset: 0x2D98 | func: 32
/** Boss defeated. */
static s32 dll_702_logic_state_1(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_28_ScreenFade->vtbl->fade(30, SCREEN_FADE_BLACK);
        func_80013FB4();
        self->unkAF |= 8;
        fsa->unk33D = 0;
        fsa->unk4.mode = 0;
        obj_send_mesg(get_player(), 0xE0000, self, NULL);
    }
    return 0;
}

// offset: 0x2E64 | func: 33
/** Walk straight down arena segment. */
static s32 dll_702_logic_state_2(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    KTrex_ObjSetup* objsetup;
    s32 chanceIdx;
    s32 reversed;

    objsetup = (KTrex_ObjSetup*)self->setup;
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_1_MOVING_STRAIGHT);
        sKTData->anger = 0;
        sKTData->flags &= ~KTFLAG_ROLLED_CHANCE;
        fsa->speed = objsetup->speeds[sKTData->anger] / 1000.0f;
    }
    if (dll_702_move_and_check_turn(fsa, sKTData) != 0) {
        dll_702_push_state(KT_LSTATE_2_WALK);
        return KT_LSTATE_3_TURN_CORNER + 1;
    }

    reversed = sKTData->flags & KTFLAG_REVERSED;
    if (sKTData->anger == 0) {
        if ((sKTData->fightProgress >= 2) && !(sKTData->flags & KTFLAG_ROLLED_CHANCE) && 
                ((!reversed && sKTData->segmentPos >= 0.7f) || (reversed && sKTData->segmentPos <= 0.3f))) {
            chanceIdx = sKTData->fightProgress >> 1;
            if (rand_next(0, 100) <= objsetup->chargeChance[chanceIdx]) {
                sKTData->chargeCounter = 2;
                dll_702_push_state(KT_LSTATE_5_CHARGE);
                sKTData->roarType = 1;
                return KT_LSTATE_4_ROAR + 1;
            }
            if (rand_next(0, 100) <= objsetup->reverseChance[chanceIdx]) {
                sKTData->roarType = 0;
                dll_702_push_state(KT_LSTATE_11_REVERSE);
                return KT_LSTATE_4_ROAR + 1;
            }
            sKTData->flags |= KTFLAG_ROLLED_CHANCE;
        }
    }
    if ((sKTData->selfSegmentBitfield & sKTData->playerSegmentBitfield) && 
            ((!reversed && sKTData->segmentPos <= sKTData->playerSegmentPos) || (reversed && sKTData->playerSegmentPos <= sKTData->segmentPos))) {
        // Player is ahead of the boss, in the same segment. Charge!
        sKTData->chargeCounter = 1;
        dll_702_push_state(KT_LSTATE_5_CHARGE);
        sKTData->roarType = 1;
        return KT_LSTATE_4_ROAR + 1;
    }
    return 0;
}

// offset: 0x3160 | func: 34
/** Turn 90 degrees around a corner. */
static s32 dll_702_logic_state_3(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_2_TURN_90);
    } else if (fsa->unk33A != 0) {
        sKTData->segmentPos = dll_702_get_obj_segment_pos(self, sKTData);
        return dll_702_pop_state() + 1;
    }

    return 0;
}

// offset: 0x3208 | func: 35
/** Stop moving and do a roar. */
static s32 dll_702_logic_state_4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    KTrex_ObjSetup* objsetup;
    f32 var_ft1;
    u16 temp_t3;

    objsetup = (KTrex_ObjSetup*)self->setup;
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_4_ROAR);
        sKTData->timer = (f32) objsetup->roarTime[sKTData->roarType];
    } else {
        sKTData->timer -= gUpdateRateF;
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
/** Charging straight down an arena segment. */
static s32 dll_702_logic_state_5(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    KTrex_ObjSetup* objsetup;

    objsetup = (KTrex_ObjSetup*)self->setup;
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_1_MOVING_STRAIGHT);
        sKTData->anger = 1;
        fsa->speed = objsetup->speeds[sKTData->anger] / 1000.0f;
    }
    if (dll_702_move_and_check_turn(fsa, sKTData) != 0) {
        sKTData->chargeCounter -= 1;
        // @bug: If chargeCounter started at > 1, this turn will not be properly handled (state 3 won't be entered).
    }
    if (sKTData->chargeCounter <= 0) {
        dll_702_push_state(KT_LSTATE_2_WALK);
        dll_702_push_state(KT_LSTATE_6_CHARGE_END);
        return KT_LSTATE_3_TURN_CORNER + 1;
    }
    if (dll_702_is_in_active_laser_wall(self) != 0) {
        return KT_LSTATE_7_ZAPPED + 1;
    }
    return 0;
}

// offset: 0x3490 | func: 37
/** Handle end of a charge. */
static s32 dll_702_logic_state_6(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_5_CHARGE_END);
    } else if (fsa->unk33A != 0) {
        return dll_702_pop_state() + 1;
    }

    return 0;
}

// offset: 0x3518 | func: 38
/** Got zapped, fall down. */
static s32 dll_702_logic_state_7(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_6_ZAPPED);
        self->unkAF &= ~0x8;
    } else if (fsa->unk33A != 0) {
        return KT_LSTATE_8_ON_GROUND + 1;
    }

    return 0;
}

// offset: 0x35A0 | func: 39
/** On ground, vulnerable after getting zapped. */
static s32 dll_702_logic_state_8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    KTrex_ObjSetup* objsetup;

    objsetup = (KTrex_ObjSetup*)self->setup;
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_7_ON_GROUND);
        sKTData->timer = (f32) objsetup->vulnerableTime[sKTData->fightProgress >> 1];
        sKTData->flags |= KTFLAG_VULNERABLE;
        sKTData->flags &= ~KTFLAG_DAMAGED;
        self->unkAF &= ~8;
    } else {
        if ((sKTData->flags & KTFLAG_DAMAGED) || (sKTData->timer -= gUpdateRateF) <= 0.0f) {
            if (sKTData->flags & KTFLAG_DAMAGED) {
                sKTData->health -= 1;
            }
            if (sKTData->health <= 0) {
                return KT_LSTATE_1_DEFEATED + 1;
            }
            self->unkAF |= 8;
            return KT_LSTATE_9_STAND_UP + 1;
        }
    }

    return 0;
}

// offset: 0x3720 | func: 40
/** Get back up after being on the ground. */
static s32 dll_702_logic_state_9(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    u16 ktflags;
    
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_8_STAND_UP);
    } else if (fsa->unk33A != 0) {
        if (sKTData->flags & KTFLAG_DAMAGED) {
            sKTData->fightProgress += 1;
            main_set_bits(BIT_572_KT_FightProgress, sKTData->fightProgress);
        }
        ktflags = sKTData->flags;
        sKTData->standingUpSegment = KTFLAG_GET_SEGMENT(ktflags);
        sKTData->timer = 300.0f;
        gDLL_2_Camera->vtbl->func8(2, 0);
        return KT_LSTATE_10_FULL_CHARGE + 1;
    }
    return 0;
}

// offset: 0x3828 | func: 41
/** Full charge around the entire arena. */
static s32 dll_702_logic_state_10(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    KTrex_ObjSetup* objsetup;
    s32 segment;
    s32 reversed;

    objsetup = (KTrex_ObjSetup*)self->setup;
    segment = KTFLAG_GET_SEGMENT(sKTData->flags);
    reversed = sKTData->flags & KTFLAG_REVERSED;
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_1_MOVING_STRAIGHT);
        sKTData->anger = 2;
        fsa->speed = objsetup->speeds[sKTData->anger] / 1000.0f;
    }
    if (dll_702_move_and_check_turn(fsa, sKTData) != 0) {
        dll_702_push_state(KT_LSTATE_10_FULL_CHARGE);
        return KT_LSTATE_3_TURN_CORNER + 1;
    }
    sKTData->timer -= gUpdateRateF;
    if (sKTData->timer <= 0.0f) {
        sKTData->timer = 0.0f;
    }
    if ((sKTData->timer <= 0.0f) && (segment == sKTData->standingUpSegment) && 
            ((!reversed && sKTData->segmentPos >= 0.75f) || (reversed && sKTData->segmentPos <= 0.25f))) {
        if (sKTData->flags & KTFLAG_DAMAGED) {
            sKTData->fightProgress += 1;
            sKTData->roarType = 0;
            dll_702_push_state(KT_LSTATE_11_REVERSE);
            dll_702_push_state(KT_LSTATE_4_ROAR);
        } else {
            dll_702_push_state(KT_LSTATE_2_WALK);
        }
        gDLL_2_Camera->vtbl->func8(3, 0);
        main_set_bits(BIT_572_KT_FightProgress, sKTData->fightProgress);
        return KT_LSTATE_6_CHARGE_END + 1;
    }
    return 0;
}

// offset: 0x3AA0 | func: 42
/** Turn 180 to reverse direction to move around the arena. */
static s32 dll_702_logic_state_11(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, KT_ASTATE_3_TURN_180);
    } else if (fsa->unk33A != 0) {
        sKTData->flags ^= KTFLAG_REVERSED;
        if (sKTData->flags & KTFLAG_REVERSED) {
            sKTData->segA_X = sKTData->segEndCCW_X;
            sKTData->segA_Y = sKTData->segEndCCW_Y;
            sKTData->segA_Z = sKTData->segEndCCW_Z;
            sKTData->segB_X = sKTData->segStartCCW_X;
            sKTData->segB_Y = sKTData->segStartCCW_Y;
            sKTData->segB_Z = sKTData->segStartCCW_Z;
        } else {
            sKTData->segA_X = sKTData->segStartCW_X;
            sKTData->segA_Y = sKTData->segStartCW_Y;
            sKTData->segA_Z = sKTData->segStartCW_Z;
            sKTData->segB_X = sKTData->segEndCW_X;
            sKTData->segB_Y = sKTData->segEndCW_Y;
            sKTData->segB_Z = sKTData->segEndCW_Z;
        }
        sKTData->segmentPos = dll_702_get_obj_segment_pos(self, sKTData);
        return KT_LSTATE_2_WALK + 1;
    }

    return 0;
}

/*0x0*/ static const char str_0[] = "TREX: state stack overflow\n";
/*0x1C*/ static const char str_1C[] = "State Error!!!\n";
