#include "PR/ultratypes.h"
#include "dlls/objects/210_player.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/gfx/animseq.h"
#include "sys/asset_thread.h"
#include "sys/curves.h"
#include "sys/dll.h"
#include "sys/fs.h"
#include "sys/joypad.h"
#include "sys/memory.h"
#include "sys/menu.h"
#include "sys/objlib.h"
#include "sys/objmsg.h"
#include "sys/segment_13D0.h"
#include "sys/segment_1460.h"
#include "sys/framebuffer_fx.h"
#include "sys/gfx/projgfx.h"
#include "macros.h"
#include "dll.h"
#include "dongle.h"

// official filename: game/anim.c (default.dol)

// Maximum number of active object sequences
#define MAX_SEQSLOTS 45
// Maximum number of actors in an object sequence
#define MAX_ACTORS 16
#define MAX_ACTIVATES 16
#define ANIMCURVES_IS_OBJSEQ2CURVE_INDEX 0x8000

// Some names inferred from default.dol
enum AnimEventType {
    ANIM_EVT_SETDURATION = -1,
    ANIM_EVT_SETTIME = 0,
    ANIM_EVT_MOVEMODE = 1,
    ANIM_EVT_ANIM = 2,
    ANIM_EVT_OVERRIDE = 3,
    ANIM_EVT_VTXANIM = 4,
    ANIM_EVT_SOFTWARE = 5,
    ANIM_EVT_SFX = 6,
    ANIM_EVT_GROUND_MODE = 7,
    ANIM_EVT_TUNE = 8,
    ANIM_EVT_ANGLE_MODE = 9,
    ANIM_EVT_LOOK_AT = 10,
    ANIM_EVT_CODE = 11,
    ANIM_EVT_SPEECH = 12,
    ANIM_EVT_ENVFX = 13, // TODO: "ENVFX" is presumably the official name but maybe we can do better?
    ANIM_EVT_STORYBOARD = 14,
    ANIM_EVT_SFX_WITH_DURATION = 15
};

// Subtypes for ANIM_EVT_ENVFX
enum AnimEnvFxEventType {
    ANIM_EVT_ENVFX_SET_MUSIC = 0,
    ANIM_EVT_ENVFX_APPLY = 2,
    ANIM_EVT_ENVFX_PARTFX = 3,
    ANIM_EVT_ENVFX_4 = 4, // noop
    ANIM_EVT_ENVFX_PROJGFX = 5,
    ANIM_EVT_ENVFX_WARP = 6,
    ANIM_EVT_ENVFX_SFX = 7,
    ANIM_EVT_ENVFX_BLINK = 8,
    ANIM_EVT_ENVFX_SCREEN_FX = 9,
    ANIM_EVT_ENVFX_SUBTITLES = 10,
    ANIM_EVT_ENVFX_SET_BIT = 11,
    ANIM_EVT_ENVFX_CLEAR_BIT = 12,
    ANIM_EVT_ENVFX_CMDMENU_BUTTON_OVERRIDE = 13,
    ANIM_EVT_ENVFX_EYELID_R = 14,
    ANIM_EVT_ENVFX_EYELID_L = 15
};

// Subtypes for ANIM_EVT_ENVFX_SCREEN_FX
enum AnimScreenFxType {
    ANIM_SCREEN_FX_FBFX_NONE = FBFX_NONE,
    ANIM_SCREEN_FX_FBFX_SINE_WAVES = FBFX_SINE_WAVES,
    ANIM_SCREEN_FX_FBFX_FADE_OUT_FADE_IN = FBFX_FADE_OUT_FADE_IN,
    ANIM_SCREEN_FX_FBFX_LERP = FBFX_LERP,
    ANIM_SCREEN_FX_FBFX_SLIDE = FBFX_SLIDE,
    ANIM_SCREEN_FX_FBFX_NOOP = FBFX_NOOP,

    ANIM_SCREEN_FX_FADE_WHITE_RADIAL = 6,
    ANIM_SCREEN_FX_FADE_WHITE_RADIAL_REVERSED = 7,
    ANIM_SCREEN_FX_FADE_WHITE = 8,
    ANIM_SCREEN_FX_FADE_WHITE_REVERSED = 9,

    ANIM_SCREEN_FX_FBFX_MOTION_BLUR = FBFX_MOTION_BLUR,

    ANIM_SCREEN_FX_FADE_RED = 11,
    ANIM_SCREEN_FX_RED_OVERLAY = 12,

    ANIM_SCREEN_FX_FBFX_FADE_OUT_DOWN_FADE_IN = FBFX_FADE_OUT_DOWN_FADE_IN,
    ANIM_SCREEN_FX_FBFX_FADE_OUT_UP_FADE_IN = FBFX_FADE_OUT_UP_FADE_IN,
    ANIM_SCREEN_FX_FBFX_FBFX_FADE_OUT = FBFX_FADE_OUT

};

// Subtypes for ANIM_EVT_CODE
enum AnimCodeEventType {
    ANIM_CODE_EVT_JUMPTOTIME = 1,
    ANIM_CODE_EVT_SET = 2,
    ANIM_CODE_EVT_COUNTER_ADD = 3,
    ANIM_CODE_EVT_PAUSE = 4,
    ANIM_CODE_EVT_CONTINUE = 5,
    ANIM_CODE_EVT_6 = 6,
    ANIM_CODE_EVT_MESSAGE = 7,
    ANIM_CODE_EVT_DECISION = 8,
    ANIM_CODE_EVT_JUMPTARGET = 9,
    ANIM_CODE_EVT_JUMPTOLABEL = 10
};

// Subtypes for ANIM_CODE_EVT_SET
enum AnimSetCodeEventType {
    ANIM_CODE_EVT_SET_MESSAGE = 0,
    ANIM_CODE_EVT_SET_COUNTER = 1,
    ANIM_CODE_EVT_SET_ANIMCOUNT1 = 3,
    ANIM_CODE_EVT_SET_ANIMCOUNT2 = 4,
    ANIM_CODE_EVT_SET_FLAGS = 5,
    ANIM_CODE_EVT_SET_BIT = 6
};

// Subtypes for ANIM_CODE_EVT_COUNTER_ADD
enum AnimCounterAddCodeEventType {
    ANIM_CODE_EVT_COUNTER_ADD_ENABLED = 0,
    ANIM_CODE_EVT_COUNTER_ADD_DISABLED = 1
};

// Subtypes for ANIM_CODE_EVT_6
enum Anim6CodeEventType {
    ANIM_CODE_EVT_6_END = 0, // signal end of seq for the current object
    ANIM_CODE_EVT_6_2 = 2, // curve related
    ANIM_CODE_EVT_6_5 = 5, // sfx related, noop
    ANIM_CODE_EVT_6_6 = 6, // sfx related, noop
    ANIM_CODE_EVT_6_CAMERA_SHAKE = 7,
    ANIM_CODE_EVT_6_9 = 9,
    ANIM_CODE_EVT_6_COUNTUP_TIMER = 10,
    ANIM_CODE_EVT_6_COUNTDOWN_TIMER = 11,
    ANIM_CODE_EVT_6_COUNTDOWN_TIMER_SFX = 12,
    ANIM_CODE_EVT_6_SFX_STOP = 13,
    ANIM_CODE_EVT_6_14 = 14,
    ANIM_CODE_EVT_6_15 = 15,
    ANIM_CODE_EVT_6_16 = 16,
    ANIM_CODE_EVT_6_TOGGLE_LETTERBOX = 18,
    ANIM_CODE_EVT_6_ENABLE_LETTERBOX = 19,
    ANIM_CODE_EVT_6_PATH_CAMERA = 20,
    ANIM_CODE_EVT_6_SET_MODEL = 23,
    ANIM_CODE_EVT_6_24 = 24,
    ANIM_CODE_EVT_6_25 = 25,
    ANIM_CODE_EVT_6_NORMAL_CAMERA = 26,
    ANIM_CODE_EVT_6_ENABLE_OBJ_GROUP = 27,
    ANIM_CODE_EVT_6_DISABLE_OBJ_GROUP = 28,
    ANIM_CODE_EVT_6_SET_ACT = 29,
    ANIM_CODE_EVT_6_DISABLE_LETTERBOX = 30,
    ANIM_CODE_EVT_6_RESTART_CLEAR = 31,
    ANIM_CODE_EVT_6_RESTART_GOTO = 32,
    ANIM_CODE_EVT_6_33 = 33,
    ANIM_CODE_EVT_6_34 = 34,
    ANIM_CODE_EVT_6_SAVEPOINT = 35,
    ANIM_CODE_EVT_6_SAVEPOINT_NO_LOCATION = 36,
    ANIM_CODE_EVT_6_TOGGLE_PLAYER_CONTROL = 37
};

enum AnimCurvesKeyframeChannels {
/*00*/ CHANNEL_headRotateZ = 0,
/*01*/ CHANNEL_headRotateX = 1,
/*02*/ CHANNEL_headRotateY = 2,
/*03*/ CHANNEL_opacity = 3,
/*04*/ CHANNEL_dayTime = 4,
/*05*/ CHANNEL_scale = 5,
/*06*/ CHANNEL_rotateZ = 6,
/*07*/ CHANNEL_rotateY = 7,
/*08*/ CHANNEL_rotateX = 8,
/*09*/ CHANNEL_animSpeed = 9,
/*0A*/ CHANNEL_animBlendSpeed = 10,
/*0B*/ CHANNEL_translateZ = 11,
/*0C*/ CHANNEL_translateY = 12,
/*0D*/ CHANNEL_translateX = 13,
/*0E*/ CHANNEL_fieldOfView = 14,
/*0F*/ CHANNEL_eyeX = 15,
/*10*/ CHANNEL_eyeY = 16,
/*11*/ CHANNEL_jaw = 17,
/*12*/ CHANNEL_soundVolume = 18
};

enum KeyframeInterpolationType {
    KF_INTERP_Hermite = 0,
    KF_INTERP_Linear = 1,
    KF_INTERP_Stepped = 2
};

typedef union {
    struct {
        f32 start;
        f32 end;
        f32 outTangentStart;
        f32 inTangentEnd;
    };
    f32 v[4];
} SplinePoints;

#define KEY_TYPE(keyframe) (keyframe.interpolation & 3)
#define KEY_EASE(keyframe) (keyframe.interpolation >> 2)

enum AnimEventConditionType {
    ANIM_EVTCOND_COUNTER_LTE_ZERO = 1,
    ANIM_EVTCOND_COUNTER_GT_ZERO = 2,
    ANIM_EVTCOND_DAYTIME = 3,
    ANIM_EVTCOND_NIGHTTIME = 4,
    ANIM_EVTCOND_EVENTFLAG_FALSE = 5,
    ANIM_EVTCOND_EVENTFLAG_TRUE = 6,
    ANIM_EVTCOND_7 = 7,
    ANIM_EVTCOND_8 = 8,
    ANIM_EVTCOND_ANIMCOUNTER1_LTE_ZERO = 9,
    ANIM_EVTCOND_ANIMCOUNTER1_GT_ZERO = 10,
    ANIM_EVTCOND_ANIMCOUNTER2_LTE_ZERO = 11,
    ANIM_EVTCOND_ANIMCOUNTER2_GT_ZERO = 12,
    ANIM_EVTCOND_13 = 13, // countdown timer related
    ANIM_EVTCOND_14 = 14, // countdown timer related
    ANIM_EVTCOND_16 = 16,
    ANIM_EVTCOND_17 = 17
};

enum ActorSettings {
    ACTORSETTING_UNK4000 = 0x4000, // look for existing obj to override
    ACTORSETTING_UNK8000 = 0x8000
};

// TODO: better name
enum ActorUpperSettings {
    ACTORUSETTING_DONT_OVERRIDE_POS = 0x1,
    ACTORUSETTING_DONT_OVERRIDE_ROT = 0x2,
    ACTORUSETTING_ZERO_YAW = 0x4,
    ACTORUSETTING_SKIPPABLE = 0x8, // whether the seq can be skipped with L trigger
    ACTORUSETTING_NO_LETTERBOX = 0x10,
    ACTORUSETTING_UNK20 = 0x20
};

// size:0x8
typedef struct {
    u32 uid;
    u16 settings;
    u16 objID;
} Actor;

typedef struct {
    Vec3f coord; 
    s8 unkC;
} CameraFunc15Unk_unk74; //Related to CameraAction and Unk_DLL2_Func888? TO-DO: figure out

typedef struct {
    Object *obj;
    s32 preemptTime;
} PreemptTime;

typedef struct{
    Object* object;
    Object* overrideObject;
} ANIMActorOverride;

typedef struct {
    s16 seqSlot;
    s16 startTime;
    u16 numActors;
} Activate;

typedef struct {
    Object* actor;
    s16 value;
    s8 type;
} QueuedEnvFx;

typedef struct {
    s32* events; // pointer to list of code events
    s16 numEvents;
    s16 time; // timestamp of code event
} CodeEventList;

/*0x0*/ static s16 _data_0 = 0;
/*0x4*/ static s16 _data_4 = 0;
/*0x8*/ static s16 _data_8 = 0xff00;
/*0xC*/ static f32 _data_C = 60.0;
/*0x10*/ static f32 _data_10 = 0.0f;
/*0x14*/ static s16 sAnimCounter1 = 0;
/*0x18*/ static s16 sAnimCounter2 = 0;
/*0x1C*/ static u8 _data_1C = 0;
/*0x20*/ static s32 sVariableObjID = -1; // object ID of the thing the player should hold when playing the first time pickup sequence
/*0x24*/ static Object* _data_24 = 0;
/*0x28*/ static s8 _data_28 = 0;
/*0x2C*/ static s32 _data_2C = 0;
/*0x30*/ static s8 _data_30 = 0;
/*0x34*/ static s32 sButtonMasks[] = {
    A_BUTTON, 
    B_BUTTON, 
    L_CBUTTONS, 
    R_CBUTTONS, 
    D_CBUTTONS, 
    U_CBUTTONS, 
    -1
};
/*0x50*/ static u32 sObjMesgIDs[] = {
    0x00050001, 0x00050002, 0x00050003, 0x00060001, 0x00060002, 0x000a0001, 0x000a0002, 0x000a0003, 
    0x00000008, 0x00000009, 0x00030002, 0x00030003, 0x000a0004, 0x000a0005, 0x000a0006, 0x000f000b, 
    0x000f000c, 0x000f000d, 0x000f000e, 0x000f000f, 0x000f0010, 0x00130001, 0x00130002
};
/*0xAC*/ static s8 _data_AC[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x01, 
    0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00
};
/*0xC4*/ static s16 _data_C4 = 0;

/*0x0*/ static PreemptTime sPreemptTimeList[4];
/*0x20*/ static s8 sPreemptTimeListCount;
/*0x24*/ static f32 _bss_24;
/*0x28*/ static f32 _bss_28;
/*0x2C*/ static f32 _bss_2C;
/*0x30*/ static s16 _bss_30;
/*0x32*/ static s8 _bss_32;
/*0x33*/ static s8 _bss_33;
/*0x38*/ static QueuedEnvFx sEnvFxQueue[10];
/*0x88*/ static s8 sEnvFxQueueCount;
/*0x89*/ static s8 _bss_89;
/*0x8A*/ static s8 _bss_8A;
/*0x8B*/ static s8 _bss_8B;
/*0x8C*/ static s32 sCameraModule;
/*0x90*/ static s32 _bss_90;
/*0x94*/ static s32 _bss_94;
/*0x98*/ static s32 _bss_98;
/*0x9C*/ static s16 sPendingWarpID;
/*0xA0*/ static f32 _bss_A0;
/*0xA4*/ static s8 sSeqEnded; // when true, the seq for the current object ended
/*0xA8*/ static s8 _bss_A8[MAX_SEQSLOTS];
/*0xD8*/ static s8 _bss_D8[MAX_SEQSLOTS];
/*0x108*/ static s8 _bss_108[MAX_SEQSLOTS];
/*0x138*/ static s8 sEventFlags[MAX_SEQSLOTS];
/*0x168*/ static s8 sSlotInUse[MAX_SEQSLOTS];
/*0x198*/ static s8 _bss_198[MAX_SEQSLOTS];
/*0x1C8*/ static s8 _bss_1C8[MAX_SEQSLOTS];
/*0x1F8*/ static s16 _bss_1F8[MAX_SEQSLOTS];
/*0x258*/ static s16 _bss_258[MAX_SEQSLOTS];
/*0x2B8*/ static s16 _bss_2B8[MAX_SEQSLOTS];
/*0x318*/ static s16 _bss_318[MAX_SEQSLOTS];
/*0x378*/ static u8 _bss_378[MAX_SEQSLOTS];
/*0x3A8*/ static u8 _bss_3A8[MAX_SEQSLOTS];
/*0x3D8*/ static s32 sSlotObjID[MAX_SEQSLOTS]; // TODO: also ends up being the UID of the first actor?
/*0x490*/ static u8 _bss_490[MAX_SEQSLOTS];
/*0x4C0*/ static u8 _bss_4C0[MAX_SEQSLOTS];
/*0x4F0*/ static f32 _bss_4F0[MAX_SEQSLOTS];
/*0x5A4*/ static f32 _bss_5A4;
/*0x5A8*/ static f32 _bss_5A8;
/*0x5AC*/ static u8 _bss_5AC;
/*0x5B0*/ static f32 _bss_5B0;
/*0x5B8*/ static Vec3f _bss_5B8;
/*0x5C4*/ static f32 _bss_5C4;
/*0x5C8*/ static s32 _bss_5C8;
/*0x5CC*/ static s8 sProcessedAnimCallback;
/*0x5D0*/ static s32 _bss_5D0;
/*0x5D4*/ static s32 _bss_5D4;
/*0x5D8*/ static void* sTempBuffer; //sequence file buffer
/*0x5DC*/ static f32 _bss_5DC;
/*0x5E0*/ static f32 _bss_5E0;
/*0x5E4*/ static f32 _bss_5E4;
/*0x5E8*/ static f32 _bss_5E8;
/*0x5F0*/ static CodeEventList sCodeEvtQueue[20];
/*0x690*/ static s32 sCodeEvtQueueCount;
/*0x694*/ static u8 _bss_694[0x4]; // unused gap
/*0x698*/ static Activate sActivates[MAX_ACTIVATES];
/*0x6F8*/ static s8 sActivatesCount;
/*0x6FC*/ static Object *_bss_6FC; // camera animobj (AnimCamera)?
/*0x700*/ static s16 _bss_700;
/*0x708*/ static ANIMActorOverride sOverrides[MAX_SEQSLOTS][16];
typedef struct {
    u32 unk0_8: 1;
    u32 unk0_1: 31; // unused
} UnkBss1D88;
/*0x1D88*/ static UnkBss1D88 _bss_1D88;

void anim_init(void);
static s32 anim_find_obj_ground_y(Object* animObj, Object *parent, f32 x, f32 y, f32 z, f32* yOut, f32 ySetup);
void anim_init_curve_keyframes(AnimObj_Data* st);
s32 anim_func_9524(Object* actor, AnimObj_Data* st, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6);
static void anim_func_9DD4(void);
static void anim_func_9B70(Object* arg1, Object* animObj, AnimObj_Data* st);
static void anim_override_list_clear(s32 slot);
void anim_end_obj_sequence(s32 slot);
static s32 anim_get_preempt_time(Object* obj);
static void anim_func_9CE8(s32 arg0);
static Object* anim_override_list_get(s32 seqSlot, Object* searchObject);
static Object* anim_find_animobj_target_in_world(Object* animObj);
s32 anim_func_8878(void);
static f32 anim_calc_channel_value_at_time(AnimCurvesKeyframe* keyframes, s32 count, s32 time);
static void anim_func_5A48(UnkAnimStruct* arg0, CurveSetup* a2, CurveSetup* a3, f32 a4, s8 a5);
static s32 anim_do_code_event_6(Object *animObj, Object *actor, AnimObj_Data *st, s32 arg3, s8 arg4);
static void anim_func_57A4(UnkAnimStruct* arg0, f32 arg1);
static s32 anim_func_51E0(UnkAnimStruct* arg0, Vec3f* arg1, Vec3f* arg2, s16* arg3, s8 arg4);
static s32 anim_check_condition(s32 cond, AnimObj_Data* st, AnimObj_Setup* setup);
s16 anim_find_override_target(Object* animObj);
static void anim_override_list_add(s32 seqSlot, Object* actor, Object* animObj);
static void anim_func_4FC4(Object* animObj, AnimObj_Data* st);
static void anim_snap_obj_to_ground(Object* animObj, AnimObj_Setup* setup);
static f32 anim_channel_value(AnimObj_Data* st, s32 channel, s32 time);
static s8 anim_get_free_sfx_slot(AnimObj_Data* st);
static Object* anim_toggle_override(Object* animObj, AnimObj_Data* st, AnimObj_Setup* setup);
static s32 anim_process_event(Object* animObj, ModelInstance* animObjModelInst, AnimCurvesEvent** events, s8 arg3, s32* arg4);
static void anim_get_actor_and_model_inst(Object* animObj, Object** actorObject, ModelInstance** actorModelInstance);
static s32 anim_do_code_event(Object* animObj, Object* actor, AnimObj_Data* st, s32* codeEvents, s16 codeEventTime, s16 numCodeEvents, s8 arg6, s8 arg7);
static void anim_func_9EC8(Object* actor, s16* arg1, s32 arg2);
static void anim_func_72E0(Object* animObj);
static void anim_do_obj_anim_callback(Object* actor, Object* animObj, AnimObj_Data* st, s8 arg3);
static void anim_handle_seq_end(Object* animObj, Object* actor, AnimObj_Data* st);
static void anim_process_envfx_queue(AnimObj_Data* st, Object* actor, u8 skipping);
static void anim_process_remaining_events_immediate(Object* animObj, Object* _actor, AnimObj_Data* st, AnimObj_Setup* setup);
static void anim_apply_channel_values(Object* animObj, Object* actor, AnimObj_Data* st, s32 time);
static void anim_update_actor_transform(Object* animObj, Object* actor, AnimObj_Data* st);
static void anim_func_2EB4(Object* animObj, Object* actor, AnimObj_Data* st);
static void anim_tick_seq_sfx(AnimObj_Data* st, s32 updateRate);
static void anim_time_skip(Object* animObj, Object* actor, AnimObj_Data* st, s32 arg3);
static void anim_func_3170(Object* animObj, Object* actor, AnimObj_Data* st);
static void anim_activate_obj(Object* animObj, Object **actorPtr, AnimObj_Data* st, AnimObj_Setup* setup, ModelInstance **modelInstPtr);
static s32 anim_func_495C(AnimObj_Data* st, Object* animObj);
static s32 anim_check_decision(Object* animObj, s32 cond, AnimObj_Data* st);
static s32 anim_func_3268(Object* animObj, Object* actor, AnimObj_Data* st);

// offset: 0x0 | ctor
void anim_ctor(void *dll) {
    sTempBuffer = mmAlloc(0x10, ALLOC_TAG_ANIMSEQ_COL, ALLOC_NAME("anim:acbuff"));
    anim_init();
}

// offset: 0x58 | dtor
void anim_dtor(void *dll) {
    mmFree(sTempBuffer);
}

// offset: 0x98 | func: 0 | export: 0
void anim_init(void) {
    s32 i;

    for (i = 0; i < MAX_SEQSLOTS; i++) {
        _bss_108[i] = 0;
        sEventFlags[i] = 0;
        sSlotInUse[i] = 0;
        _bss_A8[i] = 0;
        _bss_D8[i] = 0;
        _bss_198[i] = 0;
        _bss_1C8[i] = 0;
        _bss_1F8[i] = 0;
        _bss_258[i] = -1;
        _bss_378[i] = 0;
        sSlotObjID[i] = 0;
        _bss_490[i] = 0;
    }

    sPreemptTimeListCount = 0;
    sCameraModule = 0;
    _bss_8B = 0;
    _bss_A0 = 0.0f;
    _bss_6FC = 0;
    sActivatesCount = 0;
}

// offset: 0x2C0 | func: 1 | export: 1
void anim_queue_activate(s32 seqSlot, s32 startTime, s32 numActors) {   
    if (seqSlot >= 0 && seqSlot < MAX_SEQSLOTS) {
        if (sActivatesCount < MAX_ACTIVATES) {
            sActivates[sActivatesCount].seqSlot = seqSlot;
            sActivates[sActivatesCount].numActors = numActors;
            sActivates[sActivatesCount++].startTime = startTime; 
        } else {
            STUBBED_PRINTF("Max activates reached\n");
        }
    }
}

// offset: 0x324 | func: 2 | export: 2
void anim_set_flag(s32 seqSlot, s32 value) {
    if (seqSlot >= 0 && seqSlot < MAX_SEQSLOTS) {
        sEventFlags[seqSlot] = value;
    }
}

// offset: 0x358 | func: 3 | export: 3
s8 anim_get_flag(s32 seqSlot) {
    if (seqSlot < 0 || seqSlot >= MAX_SEQSLOTS) {
        return 0;
    }
    return sEventFlags[seqSlot];
}

// offset: 0x394 | func: 4 | export: 28
void anim_func_394(s32 arg0, s32 arg1) {
    _bss_378[arg0] = 1;
    _bss_318[arg0] = arg1;
}

// offset: 0x3D0 | func: 5 | export: 4
s32 anim_tick_obj(Object* animObj, s32 updateRate) {
    AnimObj_Data* st;
    Object* actor;
    AnimObj_Setup* setup;
    f32 newZ;
    f32 newX;
    f32 xDiff;
    ModelInstance* actorModelInst;
    AnimCurvesEvent* evt;
    f32 modAnimSpeed;
    s32 i;
    f32 var_fv1;
    s32 temp_v0_6;
    s32 var_s1;
    s32 currTime;
    f32 prevX;
    f32 prevZ;
    s32 sp94;

    sp94 = 0;
    setup = (AnimObj_Setup*)animObj->setup;
    if (setup == NULL) {
        return 1;
    }
    st = animObj->data;
    actor = st->actor;
    sSeqEnded = FALSE;
    _bss_32 = 0;
    _bss_89 = 0;
    _bss_8A = 0;
    if ((_bss_3A8[st->seqSlot] & ACTORUSETTING_SKIPPABLE) && ((_bss_4C0[st->seqSlot] != 0) || (joy_get_pressed(0) & L_TRIG))) {
        if (st->state == ANIMOBJ_STATE_Completed) {
            return 1;
        }
        st->unk9D |= 0x80;
        actor = animObj;
        if (st->actor != NULL) {
            actor = st->actor;
            actor->animObj = animObj;
            actor->stateFlags |= OBJSTATE_IN_SEQ;
        }
        if (_bss_4C0[st->seqSlot] != 0) {
            if ((st->actor != NULL) && (st->actor->seqSlot != SEQSLOT_NONE)) {
                _bss_4F0[st->seqSlot] += gUpdateRateF;
                if (_bss_4F0[st->seqSlot] > 60.0f) {
                    if (sPendingWarpID != -1) {
                        warpPlayer(sPendingWarpID, FALSE);
                    } else {
                        gDLL_28_ScreenFade->vtbl->fade_reversed(30, SCREEN_FADE_BLACK);
                    }
                    anim_handle_seq_end(animObj, actor, st);
                    return 1;
                }
                if (_bss_4F0[st->seqSlot] > 40.0f) {
                    anim_process_remaining_events_immediate(animObj, actor, st, setup);
                    gDLL_22_Subtitles->vtbl->func_448();
                }
            } else {
                if (_bss_4F0[st->seqSlot] > 50.0f) {
                    anim_handle_seq_end(animObj, actor, st);
                    return 1;
                }
                if (_bss_4F0[st->seqSlot] > 40.0f) {
                    anim_process_remaining_events_immediate(animObj, actor, st, setup);
                }
            }
        } else if ((st->actor != NULL) && (st->actor->seqSlot != SEQSLOT_NONE)) {
            gDLL_28_ScreenFade->vtbl->fade(30, SCREEN_FADE_BLACK);
            _bss_4F0[st->seqSlot] = 0;
            _bss_4C0[st->seqSlot] = 1;
            sPendingWarpID = -1;
        }

        anim_apply_channel_values(animObj, actor, st, st->time);
        if (actor != animObj) {
            anim_do_obj_anim_callback(actor, animObj, st, 0);
        }
        anim_func_4FC4(animObj, st);
        if (st->groundMode == 1) {
            anim_snap_obj_to_ground(animObj, setup);
        }
        animObj->srt.yaw += st->seqYaw;
        anim_update_actor_transform(animObj, actor, st);
        if ((st->actor != NULL) && (st->actor->seqSlot != SEQSLOT_NONE) && !(_bss_3A8[st->seqSlot] & ACTORUSETTING_NO_LETTERBOX)) {
            gDLL_2_Camera->vtbl->set_letterbox_goal(30, TRUE);
        }
        return 0;
    }
    if (st->state == ANIMOBJ_STATE_WaitingForActors) {
        if (st->actor != NULL) {
            actor->animObj = animObj;
            actor->stateFlags |= OBJSTATE_IN_SEQ;
        }
        return 0;
    }
    if (_bss_490[st->seqSlot] == 1) {
        st->time = _bss_2B8[st->seqSlot];
        st->prevTime = st->time;
        anim_func_2EB4(animObj, actor, st);
    } else {
        st->time = _bss_1F8[st->seqSlot];
    }
    anim_tick_seq_sfx(st, updateRate);
    _bss_33 = 0;
    do {
        sEnvFxQueueCount = 0;
        if (st->state == ANIMOBJ_STATE_Completed) {
            animObj->opacity = 0;
            return 1;
        }
        actor = animObj;
        if (st->actor != NULL) {
            actor = st->actor;
            actor->animObj = animObj;
            actor->stateFlags |= OBJSTATE_IN_SEQ;
        } else if ((st->unk87 == 0) && (st->unk62 < 4)) {
            st->unk62 = -1;
        }
        if (_bss_A8[st->seqSlot] != 0) {
            if (_bss_1C8[st->seqSlot] != 0) {
                st->time -= _bss_1C8[st->seqSlot];
                if (st->time < 0) {
                    st->time = 0;
                }
                st->prevTime = (s16) (st->time - 1);
                anim_time_skip(animObj, actor, st, 1);
            }
        }
        sProcessedAnimCallback = FALSE;
        if (actor != animObj) {
            anim_do_obj_anim_callback(actor, animObj, st, _bss_A8[st->seqSlot]);
            sProcessedAnimCallback = TRUE;
        }
        anim_func_3170(animObj, actor, st);
        if (st->state == ANIMOBJ_STATE_Ready) {
            anim_activate_obj(animObj, &actor, st, setup, &actorModelInst);
            return 0;
        }
        if (_bss_A8[st->seqSlot] == 1) {
            updateRate = 0;
        } else if (_bss_A8[st->seqSlot] == 2) {
            st->time = st->duration;
            _bss_89 = 1;
        } else if (_bss_A8[st->seqSlot] == 3) {
            temp_v0_6 = anim_func_495C(st, animObj);
            if (temp_v0_6 >= 0) {
                _bss_33 = 1;
                st->time = (s16) temp_v0_6;
                st->prevTime = st->time;
            }
        }
        if ((st->actor != NULL) && (st->actor->seqSlot != SEQSLOT_NONE) && !(_bss_3A8[st->seqSlot] & ACTORUSETTING_NO_LETTERBOX)) {
            gDLL_2_Camera->vtbl->set_letterbox_goal(30, TRUE);
        }
        if (_bss_378[st->seqSlot] != 0) {
            st->seqYaw = _bss_318[st->seqSlot];
        }
        if (st->pauseCond != 0) {
            if (anim_check_condition(st->pauseCond - 1, st, setup) == 0) {
                // No longer paused
                st->pauseCond = 0;
            } else {
                _bss_258[st->seqSlot] = st->time;
                return 0;
            }
        }
        // Progress sequence
        st->time += updateRate;
        if (st->duration < st->time) {
            st->time = st->duration;
        }
        currTime = st->time;
        anim_apply_channel_values(animObj, actor, st, currTime);
        animObj->srt.transl.x += st->unk4;
        animObj->srt.transl.y += st->unk8;
        animObj->srt.transl.z += st->unkC;
        animObj->srt.roll += st->unk18;
        animObj->srt.pitch += st->unk16;
        animObj->srt.yaw += st->unk14;
        actorModelInst = actor->modelInsts[actor->modelInstIdx];
        sCodeEvtQueueCount = 0;
        if (actorModelInst != NULL) {
            prevX = anim_channel_value(st, CHANNEL_translateX, st->prevTime) + setup->base.x;
            prevZ = anim_channel_value(st, CHANNEL_translateZ, st->prevTime) + setup->base.z;
        }
        // Process tick in constant time
        st->time = st->prevTime;
        while (st->time < currTime) {
            st->time += 1;
            newX = anim_channel_value(st, CHANNEL_translateX, st->time) + setup->base.x;
            newZ = anim_channel_value(st, CHANNEL_translateZ, st->time) + setup->base.z;
            if ((st->time > 0) && (st->unk7A & ANIM7AFLAG_OVERRIDE_MODEL)) {
                if ((st->unk84 == 1) && (st->unk87 == 0) && (actorModelInst != NULL)) {
                    xDiff = newX - prevX;
                    var_fv1 = newZ - prevZ;
                    if (func_8002493C(actor, sqrtf(SQ(xDiff) + SQ(var_fv1)), &modAnimSpeed) == 0) {
                        modAnimSpeed = anim_channel_value(st, CHANNEL_animSpeed, st->time - 1) * 0.0004f;
                    }
                } else {
                    modAnimSpeed = anim_channel_value(st, CHANNEL_animSpeed, st->time - 1) * 0.0004f;
                }
                if (actorModelInst != NULL) {
                    func_80024108(actor, modAnimSpeed, 1.0f, &st->unkFC);
                    func_80025780(actor, 1.0f, &st->unkFC, 0);
                    if ((st->unk30 != 0) && (actorModelInst->model->unk71 & 1)) {
                        func_80032B44(actor, st->unk30);
                    }
                    func_8001B084(actorModelInst, 1.0f);
                    if (st->unk20 > 0.0f) {
                        if (st->channelTotalKeys[CHANNEL_animBlendSpeed] != 0) {
                            var_fv1 = anim_channel_value(st, CHANNEL_animBlendSpeed, st->time - 1);
                        } else {
                            var_fv1 = 8.0f;
                        }
                        if (var_fv1 < 1.0f) {
                            var_fv1 = 1.0f;
                        }
                        var_fv1 = 1.0f / var_fv1;
                        st->unk20 -= var_fv1;
                        if (st->unk20 < 0.0f) {
                            st->unk20 = 0.0f;
                        }
                    }
                    dummy_label1: ; // @fake
                } else {
                    actor->animProgress += modAnimSpeed;
                    while (actor->animProgress > 1.0f) {
                        actor->animProgress -= 1.0f;
                    }
                    while (actor->animProgress < 0.0f) {
                        actor->animProgress += 1.0f;
                    }
                }
            }
            prevX = newX;
            prevZ = newZ;
            var_s1 = 0;
            while ((var_s1 == 0) && (st->eventIdx < st->animCurvesEventCount)) {
                evt = &st->animCurvesEvents[st->eventIdx];
                if (evt->type == ANIM_EVT_SETTIME) {
                    if (st->time >= evt->params) {
                        st->eventTime = evt->params;
                        st->eventIdx += 1;
                    } else {
                        var_s1 = 1;
                    }
                } else if (st->time >= st->eventTime) {
                    if (evt->type != ANIM_EVT_SFX_WITH_DURATION) {
                        st->eventTime += evt->delay;
                    }
                    st->eventIdx += 1;
                    if (anim_process_event(animObj, actorModelInst, &evt, 0, NULL) != 0) {
                        currTime = st->time;
                    }
                    anim_get_actor_and_model_inst(animObj, &actor, &actorModelInst);
                } else {
                    var_s1 = 1;
                }
            }
        }
        // Process decisions
        for (i = 0; i < MAX_DECISION; i++) {
            if (st->decisionConditions[i] && (anim_check_decision(animObj, st->decisionConditions[i], st) != 0)) {
                _bss_33 = 1;
                st->time = st->decisionTimes[i];
                st->prevTime = st->time;
                i = 0;
                while (i < MAX_DECISION) {
                    st->decisionConditions[i++] = 0;
                }
                break;
            }
        }
        // Do late anim callback if it wasn't handled earlier
        if ((sProcessedAnimCallback == FALSE) && (actor != animObj)) {
            anim_do_obj_anim_callback(actor, animObj, st, _bss_A8[st->seqSlot]);
        }
        if (st->unk9D != 0) {
            _bss_33 = anim_func_3268(animObj, actor, st);
        }
        st->messageCount = 0;
        st->lastMessage = 0;
        if ((actorModelInst != NULL) && (st->unk7A & ANIM7AFLAG_OVERRIDE_MODEL)) {
            actorModelInst->animState0->unk58[0] = (s16) (st->unk20 * 1023.0f);
        }
        anim_func_4FC4(animObj, st);
        if (st->groundMode == 1) {
            anim_snap_obj_to_ground(animObj, setup);
        }
        animObj->srt.yaw += st->seqYaw;
        anim_update_actor_transform(animObj, actor, st);
        anim_process_envfx_queue(st, actor, FALSE);
        gDLL_15_Projgfx->vtbl->func2(st->time - st->prevTime, actor);
        // Process pending code events
        for (i = 0; i < sCodeEvtQueueCount; i++) {
            if (anim_do_code_event(animObj, actor, st, 
                                sCodeEvtQueue[i].events, 
                                sCodeEvtQueue[i].time, 
                                sCodeEvtQueue[i].numEvents, 0, 0) != 0) {
                i = sCodeEvtQueueCount;
            }
            anim_get_actor_and_model_inst(animObj, &actor, &actorModelInst);
        }
        // Save previous timestamp
        st->prevTime = st->time;
        if (sSeqEnded) {
            anim_get_actor_and_model_inst(animObj, &actor, &actorModelInst);
            anim_handle_seq_end(animObj, actor, st);
        } else {
            if (_bss_33 != 0) {
                _bss_2B8[st->seqSlot] = st->time;
                _bss_490[st->seqSlot] = 2;
                _bss_258[st->seqSlot] = st->time;
            }
            if (_bss_258[st->seqSlot] == -1) {
                _bss_258[st->seqSlot] = _bss_1F8[st->seqSlot] + updateRate;
            }
        }
    } while (!sSeqEnded && (st->time < st->duration) && sp94--);
    
    return 0;
}

// offset: 0x15FC | func: 6
static void anim_process_remaining_events_immediate(Object* animObj, Object* _actor, AnimObj_Data* st, AnimObj_Setup* setup) {
    AnimCurvesEvent* evt;
    s32 _pad;
    s32 _pad2;
    CodeEventList* evtList;
    s32 _pad3;
    Object* actor;
    s32 i;
    s32 sp60;
    ModelInstance* animObjModelInst;

    if (_actor){} // @fake

    actor = animObj;
    animObjModelInst = animObj->modelInsts[animObj->modelInstIdx];
    if (st->actor != NULL) {
        actor = st->actor;
        actor->animObj = animObj;
        actor->stateFlags |= OBJSTATE_IN_SEQ;
    }
    if (st->animCurvesEvents != NULL) {
        st->modAnimIdx = -1;
        sEnvFxQueueCount = 0;
        sCodeEvtQueueCount = sEnvFxQueueCount;
        while (st->eventIdx < st->animCurvesEventCount) {
            evt = &st->animCurvesEvents[st->eventIdx];
            if (evt->type == ANIM_EVT_SETTIME) {
                st->eventTime = evt->params;
            } else if ((evt->type == ANIM_EVT_CODE) && (evt->params > 0)) {
                st->eventTime += evt->delay;
                anim_process_event(animObj, animObjModelInst, &evt, 0, &sp60);
            } else {
                if (evt->type != ANIM_EVT_SFX_WITH_DURATION) {
                    st->eventTime += evt->delay;
                }
                if (evt->type == ANIM_EVT_ENVFX) {
                    if (((evt->params >> 0xC) & 0xF) == ANIM_EVT_ENVFX_WARP) {
                        sPendingWarpID = evt->params & 0xFFF;
                    }
                }
                if (evt->type == ANIM_EVT_ANIM) {
                    st->modAnimIdx = evt->params & 0xFFF;
                } else {
                    anim_process_event(animObj, animObjModelInst, &evt, 8, &sp60);
                }
            }
            st->eventIdx += 1;
            if (sCodeEvtQueueCount >= 20) {
                for (i = 0; i < sCodeEvtQueueCount; i++) {
                    evtList = &sCodeEvtQueue[i];
                    if (anim_do_code_event(animObj, actor, st, 
                                        evtList->events, 
                                        evtList->time, 
                                        evtList->numEvents, 0, 1) != 0) {
                        i = sCodeEvtQueueCount;
                    }
                }
                sCodeEvtQueueCount = 0;
            }
            if (sEnvFxQueueCount >= 10) {
                anim_process_envfx_queue(st, actor, TRUE);
                sEnvFxQueueCount = 0;
            }
            if ((st->messageCount >= 10) && (actor != animObj)) {
                anim_do_obj_anim_callback(actor, animObj, st, 0);
            }
        }
        for (i = 0; i < sCodeEvtQueueCount; i++) {
            evtList = &sCodeEvtQueue[i];
            if (anim_do_code_event(animObj, actor, st, 
                                evtList->events, 
                                evtList->time, 
                                evtList->numEvents, 0, 1) != 0) {
                i = sCodeEvtQueueCount;
            }
        }
        sCodeEvtQueueCount = 0;
        if (sEnvFxQueueCount != 0) {
            anim_process_envfx_queue(st, actor, TRUE);
            sEnvFxQueueCount = 0;
        }
        if (animObjModelInst != NULL) {
            if (st->modAnimIdx != -1) {
                func_80023D30(actor, st->modAnimIdx, 0.0f, 0);
                animObjModelInst->animState0->unk58[0] = 0;
            }
        }
    }
    st->time = st->duration - 1;
}

// offset: 0x1A04 | func: 7
static void anim_update_actor_transform(Object* animObj, Object* actor, AnimObj_Data* st) {
    f32 var_fa0;
    f32 var_fv0;
    f32 var_fv1;
    s16 temp_a1;
    s16 temp_a3;
    s16 var_v1;

    if ((actor->parent == animObj->parent) || (_bss_32 == 0)) {
        var_fv0 = animObj->srt.transl.x;
        var_fv1 = animObj->srt.transl.y;
        var_fa0 = animObj->srt.transl.z;
        var_v1 = animObj->srt.yaw;
    } else {
        var_fv0 = _bss_24;
        var_v1 = _bss_30;
        var_fv1 = _bss_28;
        var_fa0 = _bss_2C;
    }
    temp_a1 = animObj->srt.pitch;
    temp_a3 = animObj->srt.roll;
    if (actor != animObj) {
        if (st->unk7A & ANIM7AFLAG_OVERRIDE_POS) {
            if (st->unk62 == 2) {
                actor->srt.transl.x = (st->unk4C.x * st->unk58) + var_fv0;
                actor->srt.transl.y = (st->unk4C.y * st->unk58) + var_fv1;
                actor->srt.transl.z = (st->unk4C.z * st->unk58) + var_fa0;
            } else {
                actor->srt.transl.x = var_fv0;
                actor->srt.transl.y = var_fv1;
                actor->srt.transl.z = var_fa0;
            }
        }
        if (st->unk7A & ANIM7AFLAG_OVERRIDE_ROT) {
            if (st->unk62 == 2) {
                actor->srt.yaw = var_v1 + (s16) ((f32) st->yawDiff * st->unk58);
                actor->srt.pitch = temp_a1 + (s16) ((f32) st->pitchDiff * st->unk58);
                actor->srt.roll = temp_a3 + (s16) ((f32) st->rollDiff * st->unk58);
            } else {
                actor->srt.yaw = var_v1;
                actor->srt.pitch = temp_a1;
                actor->srt.roll = temp_a3;
            }
        }
    }
    if ((st->unk87 != 0) && (st->unk84 != 0)) {
        anim_func_72E0(animObj);
    }
    get_object_child_position(actor, &actor->globalPosition.x, &actor->globalPosition.y, &actor->globalPosition.z);
}

// offset: 0x1C04 | func: 8
static void anim_apply_channel_values(Object* animObj, Object* actor, AnimObj_Data* st, s32 time) {
    f32 var_fv1;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    s32 _pad;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    ObjSetup* setup;
    Object* player;
    s32 _pad2;
    s16* temp_v0_3;

    setup = animObj->setup;
    animObj->srt.transl.x = setup->x;
    animObj->srt.transl.y = setup->y;
    animObj->srt.transl.z = setup->z;
    animObj->srt.pitch = 0;
    animObj->srt.yaw = 0;
    animObj->srt.roll = 0;
    if (st->unk7A & ANIM7AFLAG_OVERRIDE_OPACITY) {
        actor->opacity = 0xFF;
    }
    _bss_5E0 = 0.0f;\
    _bss_5E4 = 0.0f;\
    _bss_5E8 = 0.0f;
    if (st->animCurvesKeyframes != NULL) {
        var_fv1 = anim_channel_value(st, CHANNEL_soundVolume, time);
        if (var_fv1 > 0.0f) {
            if (st->sfxHandles[3] != 0) {
                gDLL_6_AMSFX->vtbl->set_vol(st->sfxHandles[3], var_fv1);
            }
        }
        if (!(st->unk8C & 2)) {
            animObj->srt.yaw = (s16) (anim_channel_value(st, CHANNEL_rotateY, time) * 182.044f);
            animObj->srt.pitch = (s16) (anim_channel_value(st, CHANNEL_rotateX, time) * 182.044f);
            animObj->srt.roll = (s16) (anim_channel_value(st, CHANNEL_rotateZ, time) * 182.044f);
            _bss_5E0 = anim_channel_value(st, CHANNEL_translateX, time);
            _bss_5E4 = anim_channel_value(st, CHANNEL_translateY, time);
            _bss_5E8 = anim_channel_value(st, CHANNEL_translateZ, time);
            _bss_24 = _bss_5E0;
            _bss_28 = _bss_5E4;
            _bss_2C = _bss_5E8;
            _bss_30 = animObj->srt.yaw;
            _bss_32 = 1;
            animObj->srt.transl.x = setup->x + _bss_5E0;
            animObj->srt.transl.y = setup->y + _bss_5E4;
            animObj->srt.transl.z = setup->z + _bss_5E8;
        } else {
            _bss_5E4 = anim_channel_value(st, CHANNEL_translateY, time);
            _bss_5E8 = anim_channel_value(st, CHANNEL_translateZ, time);
            animObj->srt.yaw = (s16) (anim_channel_value(st, CHANNEL_rotateY, time) * 182.044f);
            animObj->srt.pitch = (s16) (anim_channel_value(st, CHANNEL_rotateX, time) * 182.044f);
            sp58 = fsin16_precise(animObj->srt.yaw - 0x4000);
            sp50 = fcos16_precise(animObj->srt.yaw - 0x4000);
            sp54 = fcos16_precise(animObj->srt.pitch);
            sp44 = fsin16_precise(animObj->srt.pitch);
            sp44 = _bss_5E8 * sp44;
            sp40 = _bss_5E8 * sp54;
            sp48 = sp40 * sp50;
            sp40 = sp40 * sp58;
            player = get_player();
            animObj->srt.transl.x = player->srt.transl.x + sp48;
            animObj->srt.transl.y = player->srt.transl.y + _bss_5E4 + sp44;
            animObj->srt.transl.z = player->srt.transl.z + sp40;
            animObj->srt.yaw = 0x8000 - animObj->srt.yaw;
            animObj->srt.pitch = -animObj->srt.pitch;
            _bss_24 = animObj->srt.transl.x - setup->x;
            _bss_28 = animObj->srt.transl.y - setup->y;
            _bss_2C = animObj->srt.transl.z - setup->z;
            _bss_30 = animObj->srt.yaw;
            _bss_32 = 1;
        }
        if (st->channelTotalKeys[CHANNEL_fieldOfView] != 0) {
            var_fv1 = anim_channel_value(st, CHANNEL_fieldOfView, time);
            if (st->unk87 != 0) {
                if (var_fv1 < 35.0f) {
                    var_fv1 = 35.0f;
                }
                if (var_fv1 > 120.0f) {
                    var_fv1 = 125.0f;
                }
                _data_30 = 1;
                _bss_5DC = var_fv1;
            } else {
                st->unk10 = var_fv1;
            }
        }
        if ((st->unk7A & ANIM7AFLAG_OVERRIDE_OPACITY) && (st->channelTotalKeys[CHANNEL_opacity] != 0)) {
            var_fv1 = anim_channel_value(st, CHANNEL_opacity, time);
            if (var_fv1 < 0.0f) {
                var_fv1 = 0.0f;
            }
            if (var_fv1 > 255.0f) {
                var_fv1 = 255.0f;
            }
            actor->opacity = (u8) (u32) var_fv1;
        }
        if (st->channelTotalKeys[CHANNEL_dayTime] != 0) {
            gDLL_7_Newday->vtbl->func9(anim_channel_value(st, CHANNEL_dayTime, time) * 60.0f);
        }
        if ((st->unk7A & ANIM7AFLAG_OVERRIDE_SCALE) && (st->channelTotalKeys[CHANNEL_scale] != 0)) {
            var_fv1 = anim_channel_value(st, CHANNEL_scale, time);
            actor->srt.scale = actor->def->scale * var_fv1;
        }
        if (st->unk7A & ANIM7AFLAG_OVERRIDE_HEAD) {
            temp_v0_3 = func_80034804(actor, 0);
            if (temp_v0_3 != NULL) {
                if (st->channelTotalKeys[CHANNEL_headRotateX] != 0) {
                    var_fv1 = anim_channel_value(st, CHANNEL_headRotateX, time);
                } else {
                    var_fv1 = 0.0f;
                }
                temp_v0_3[0] = st->unk122 + (s16) (var_fv1 * 182.044f);
                if (st->channelTotalKeys[CHANNEL_headRotateY] != 0) {
                    var_fv1 = anim_channel_value(st, CHANNEL_headRotateY, time);
                } else {
                    var_fv1 = 0.0f;
                }
                temp_v0_3[1] = st->unk120 + (s16) (var_fv1 * 182.044f);
                if (st->channelTotalKeys[CHANNEL_headRotateZ] != 0) {
                    var_fv1 = anim_channel_value(st, CHANNEL_headRotateZ, time);
                } else {
                    var_fv1 = 0.0f;
                }
                temp_v0_3[2] = (s16) (var_fv1 * 182.044f);
                if (st->unk7A & ANIM7AFLAG_UNK400) {
                    anim_func_9EC8(actor, temp_v0_3, st->unk142_4);
                }
            }
            if (1){} // @fake
        }
        if (st->unk7A & ANIM7AFLAG_OVERRIDE_JAW) {
            temp_v0_3 = func_80034804(actor, 1);
            if (temp_v0_3 != NULL) {
                if (st->channelTotalKeys[CHANNEL_jaw] != 0) {
                    var_fv1 = anim_channel_value(st, CHANNEL_jaw, time);
                } else {
                    var_fv1 = 0.0f;
                }
                temp_v0_3[0] = (s16) (var_fv1 * 182.044f);
            }
            if (1){} // @fake
        }
        if (st->unk7A & ANIM7AFLAG_OVERRIDE_EYES) {
            TextureAnimator* temp_v0_6;
            TextureAnimator* var_v1;
            TextureAnimator* temp_s0;
            temp_s0 = func_800348A0(actor, HEAD_ANIMATION_TAG_Pupil_R, 0);
            var_v1 = func_800348A0(actor, HEAD_ANIMATION_TAG_Pupil_L, 0);
            if ((temp_s0 != NULL) || (var_v1 != NULL)) {
                if (st->channelTotalKeys[CHANNEL_eyeX] != 0) {
                    var_fv1 = anim_channel_value(st, CHANNEL_eyeX, time);
                } else {
                    var_fv1 = 0.0f;
                }
                if (temp_s0 != NULL) {
                    temp_s0->positionU = (s16) var_fv1;
                }
                if (var_v1 != NULL) {
                    var_v1->positionU = -(s16) var_fv1;
                }
                if (st->channelTotalKeys[CHANNEL_eyeY] != 0) {
                    var_fv1 = anim_channel_value(st, CHANNEL_eyeY, time);
                } else {
                    var_fv1 = 0.0f;
                }
                if (temp_s0 != NULL) {
                    temp_s0->positionV = -(s16) var_fv1;
                }
                if (var_v1 != NULL) {
                    var_v1->positionV = -(s16) var_fv1;
                }
            }
            temp_s0 = func_800348A0(actor, HEAD_ANIMATION_TAG_Eyelid_R, 0);
            temp_v0_6 = func_800348A0(actor, HEAD_ANIMATION_TAG_Eyelid_L, 0);
            if (temp_s0 != NULL) {
                temp_s0->frame = st->blinkFrameR << 8;
            }
            if (temp_v0_6 != NULL) {
                temp_v0_6->frame = st->blinkFrameL << 8;
            }
        }
    } else {
        _bss_24 = _bss_5E0;
        _bss_28 = _bss_5E4;
        _bss_2C = _bss_5E8;
        _bss_30 = 0;
        _bss_32 = 1;
    }
}

// offset: 0x2760 | func: 9
/** 
 * Processes all curve events from the start of the seq up to the current time.
 * If an object was added to a sequence late, this can sync its time up with the other actors.
 * Only supports skipping from the start of the sequence.
 */
static void anim_time_skip(Object* animObj, Object* actor, AnimObj_Data* st, s32 arg3) {
    AnimCurvesEvent* evt;
    AnimObj_Setup* setup;
    ModelInstance* modelInst;
    s32 _pad;
    f32 temp_fv0;
    s32 currTime;
    f32 temp_fv1;
    f32 spA0;
    f32 var_fv1;
    s32 _pad2;
    s32 var_s6;
    f32 sp90;
    f32 sp8C;
    s8 sp8B;
    f32 sp7C[3];
    s32 i;
    s32 var_s0_2;
    s32 sp70;

    if (st->animCurvesEvents == NULL) {
        return;
    }

    sp8B = 1;
    if (arg3 != 0) {
        sp8B |= 2;
    }
    setup = (AnimObj_Setup*)animObj->setup;
    currTime = (s32) st->time;
    _data_C4 = currTime;
    st->eventIdx = 0;
    st->eventTime = -50;
    st->unk84 = 0;
    st->groundMode = 0;
    st->overrideMode = 0;
    st->actor = NULL;
    st->unk87 = 0;
    st->unk20 = 0.0f;
    st->time = -1;
    actor = animObj;
    var_s6 = -1;
    for (i = 0; i < st->animCurvesEventCount && currTime >= st->time; i++) {
        evt = &st->animCurvesEvents[i];
        switch (evt->type) {
        case ANIM_EVT_OVERRIDE:
            sp8B |= 4;
            actor = anim_toggle_override(animObj, st, setup);
            actor->curModAnimIdLayered = -1;
            break;
        case ANIM_EVT_SETTIME:
            st->time = evt->params;
            break;
        case ANIM_EVT_ANGLE_MODE:
            var_s6 = st->time;
            break;
        case ANIM_EVT_CODE:
            if (evt->params > 0) {
                i += evt->params;
            }
            break;
        default:
            if (evt->type != ANIM_EVT_SFX_WITH_DURATION) {
                st->time += evt->delay;
            }
            break;
        }
    }
    st->time = var_s6;
    modelInst = actor->modelInsts[actor->modelInstIdx];
    if (modelInst != NULL) {
        
    }
    if (modelInst != NULL) {
        sp90 = anim_channel_value(st, CHANNEL_translateX, -1) + setup->base.x;
        sp8C = anim_channel_value(st, CHANNEL_translateZ, -1) + setup->base.z;
    }
    while (st->time < currTime) {
        st->time += 1;
        sp7C[0] = anim_channel_value(st, CHANNEL_translateX, st->time) + setup->base.x;
        sp7C[1] = anim_channel_value(st, CHANNEL_translateY, st->time) + setup->base.y;
        sp7C[2] = anim_channel_value(st, CHANNEL_translateZ, st->time) + setup->base.z;
        if (st->time > 0 && arg3 != 0) {
            if ((st->unk84 == 1) && (st->unk87 == 0) && (modelInst != NULL)) {
                temp_fv0 = sp7C[0] - sp90;
                var_fv1 = sp7C[2] - sp8C;
                if (func_8002493C(actor, sqrtf(SQ(temp_fv0) + SQ(var_fv1)), &spA0) == 0) {
                    spA0 = anim_channel_value(st, CHANNEL_animSpeed, st->time - 1) * 0.0004f;
                }
            } else {
                spA0 = anim_channel_value(st, CHANNEL_animSpeed, st->time - 1) * 0.0004f;
            }
            if (modelInst != NULL) {
                func_80024108(actor, spA0, 1.0f, &st->unkFC);
                func_80025780(actor, 1.0f, &st->unkFC, 0);
                if ((arg3 != 0) && (st->unk20 > 0.0f)) {
                    if (st->channelTotalKeys[CHANNEL_animBlendSpeed] != 0) {
                        var_fv1 = anim_channel_value(st, CHANNEL_animBlendSpeed, st->time - 1);
                    } else {
                        var_fv1 = 8.0f;
                    }
                    if (var_fv1 < 1.0f) {
                        var_fv1 = 1.0f;
                    }
                    var_fv1 = 1.0f / var_fv1;
                    st->unk20 -= var_fv1;
                    if (st->unk20 < 0.0f) {
                        st->unk20 = 0.0f;
                    }
                }
                dummy_label_46300: ; // @fake
            } else {
                actor->animProgress += spA0;
                while (actor->animProgress > 1.0f) {
                    actor->animProgress -= 1.0f;
                }
                while (actor->animProgress < 0.0f) {
                    actor->animProgress += 1.0f;
                }
            }
        }
        sp90 = sp7C[0];
        sp8C = sp7C[2];
        sCodeEvtQueueCount = 0;
        var_s0_2 = 0;
        while (var_s0_2 == 0 && st->eventIdx < st->animCurvesEventCount) {
            evt = &st->animCurvesEvents[st->eventIdx];
            if (evt->type == ANIM_EVT_SETTIME) {
                if (st->time >= evt->params) {
                    st->eventTime = evt->params;
                    st->eventIdx += 1;
                } else {
                    var_s0_2 = 1;
                }
            } else {
                if (st->time >= st->eventTime) {
                    if (evt->type != ANIM_EVT_SFX_WITH_DURATION) {
                        st->eventTime = st->eventTime + evt->delay;
                    }
                    st->eventIdx += 1;
                    if (anim_process_event(animObj, modelInst, &evt, sp8B, &sp70) != 0) {
                        return;
                    }
                    anim_get_actor_and_model_inst(animObj, &actor, &modelInst);
                } else {
                    var_s0_2 = 1;
                }
            }
        }
        for (i = 0; i < sCodeEvtQueueCount; i++) {
            if (anim_do_code_event(animObj, actor, st, 
                    sCodeEvtQueue[i].events, 
                    sCodeEvtQueue[i].time, 
                    sCodeEvtQueue[i].numEvents, 1, 0) != 0) {
                i = sCodeEvtQueueCount;
            }
            anim_get_actor_and_model_inst(animObj, &actor, &modelInst);
        }
        sCodeEvtQueueCount = 0;
    }
}

// offset: 0x2EB4 | func: 10
static void anim_func_2EB4(Object* animObj, Object* actor, AnimObj_Data* st) {
    s16 frame;
    s16 temp_t0;
    s32 finished;
    s8 type;
    AnimCurvesEvent* event;

    if (st->animCurvesEvents == NULL) {
        return;
    }
    
    st->eventIdx = 0;
    st->eventTime = -1;
    st->unk20 = 0.0f;

    finished = FALSE;
    while (!finished && (st->eventIdx < st->animCurvesEventCount)){
        event = &st->animCurvesEvents[st->eventIdx];
        if (event->type == ANIM_EVT_SETTIME) {
            if (st->time >= event->params) {
                st->eventTime = event->params;
                st->eventIdx++;
            } else {
                finished = TRUE;
            }
        } else {
            if ((event->type == ANIM_EVT_CODE) && (event->params > 0)) {
                if (st->time >= st->eventTime) {
                    st->eventTime += event->delay;
                    st->eventIdx += event->params + 1;
                } else {
                    finished = TRUE;
                }
            } else if (st->time >= st->eventTime) {
                if (event->type != ANIM_EVT_SFX_WITH_DURATION) {
                    st->eventTime += event->delay;
                }
                st->eventIdx++;
            } else {
                finished = TRUE;
            }
        }
    }
}

// offset: 0x2FE8 | func: 11
static Object* anim_toggle_override(Object* animObj, AnimObj_Data* st, AnimObj_Setup* setup) {
    Object* actor;

    actor = animObj;
    st->overrideMode ^= 1;
    if (st->overrideMode != 0) {
        anim_find_override_target(animObj);
        if (st->actor != NULL) {
            actor = st->actor;
            actor->animObj = animObj;
            actor->stateFlags |= OBJSTATE_IN_SEQ;
            st->overrideTarget = actor;
            anim_override_list_add(st->seqSlot, st->actor, animObj);
        }
    } else {
        if (st->actor != NULL) {
            if (st->unk7A & ANIM7AFLAG_OVERRIDE_POS) {
                actor->srt.transl.x = animObj->srt.transl.x;
                actor->srt.transl.y = animObj->srt.transl.y;
                actor->srt.transl.z = animObj->srt.transl.z;
                actor->srt.roll = animObj->srt.roll;
                anim_func_4FC4(actor, st);
            }
            if (st->groundMode == 1) {
                anim_snap_obj_to_ground(actor, setup);
            }
            if (st->unk7A & ANIM7AFLAG_OVERRIDE_ROT) {
                actor->srt.yaw += st->seqYaw;
            }
            actor->animObj = NULL;
            actor->stateFlags &= ~OBJSTATE_IN_SEQ;
            st->actor = NULL;
            actor = animObj;
        }
    }
    return actor;
}

// offset: 0x3170 | func: 12
static void anim_func_3170(Object* animObj, Object* actor, AnimObj_Data* st) {
    if (st->unk9D & 1) {
        _bss_108[st->seqSlot] = 1;
    }
    if (st->unk9D & 2) {
        _bss_108[st->seqSlot] = 0;
    }
    if (st->unk9D & 4) {
        sEventFlags[st->seqSlot] = 1;
    }
    if (st->unk9D & 8) {
        sEventFlags[st->seqSlot] = 0;
    }
    if (st->unk9D & 0x10) {
        _bss_198[st->seqSlot] = 1;
    }
    if (st->unk9D & 0x20) {
        _bss_198[st->seqSlot] = 0;
    }
}

// offset: 0x3268 | func: 13
static s32 anim_func_3268(Object* animObj, Object* actor, AnimObj_Data* st) {
    s32 returnVal;

    returnVal = 0;
    if (st->unk9D & 0x40) {
        returnVal = 1;
        st->unk9D &= ~0x40;
        st->time = st->unk80;
        st->prevTime = st->time;
    }
    st->unk9D = 0;
    return returnVal;
}

// offset: 0x32B0 | func: 14
static void anim_tick_seq_sfx(AnimObj_Data* st, s32 updateRate) {
    s32 i;

    for (i = 0; i < 4; i++){
        if (st->sfxHandles[i]) {
            if (gDLL_6_AMSFX->vtbl->is_playing(st->sfxHandles[i]) == FALSE) {
                gDLL_6_AMSFX->vtbl->stop(st->sfxHandles[i]);
                st->sfxHandles[i] = 0;
                st->sfxTimer[i] = 0;
                if (i != 3) {
                    st->sfxNextSlot = i;
                }
            }
            if (gDLL_6_AMSFX->vtbl->is_playing(st->sfxHandles[i]) && (st->sfxTimer[i] <= 0)) {
                gDLL_6_AMSFX->vtbl->stop(st->sfxHandles[i]);
                st->sfxHandles[i] = 0;
                st->sfxTimer[i] = 0;
                if (i != 3) {
                    st->sfxNextSlot = i;
                }
            }
            if ((st->sfxTimer[i] > 0) && (st->sfxTimer[i] != 32000)) {
                st->sfxTimer[i] -= updateRate;
            }
        }
    }
}

// offset: 0x3414 | func: 15
static void anim_activate_obj(Object* animObj, Object **actorPtr, AnimObj_Data* st, AnimObj_Setup* setup, ModelInstance **modelInstPtr) {
    Object* actor;

    _bss_90 = 1;
    _bss_98 = 0x5A;
    st->time = st->startTime;
    st->prevTime = -60;
    anim_apply_channel_values(animObj, *actorPtr, st, 0);
    anim_time_skip(animObj, *actorPtr, st, 1);
    anim_get_actor_and_model_inst(animObj, actorPtr, modelInstPtr);
    anim_func_4FC4(animObj, st);
    if (st->groundMode == 1) {
        anim_snap_obj_to_ground(animObj, setup);
    }
    
    animObj->srt.yaw += st->seqYaw;
    actor = *actorPtr;
    if ((animObj != actor) && (sProcessedAnimCallback == FALSE)) {
        anim_do_obj_anim_callback(actor, animObj, st, _bss_A8[st->seqSlot]);
    }
    actor = *actorPtr;
    anim_update_actor_transform(animObj, actor, st);
    st->blinkFrameR = 0;
    st->blinkFrameL = 0;
    st->state = ANIMOBJ_STATE_Playing;
    st->prevTime = st->time;
    if (sSeqEnded) {
        anim_handle_seq_end(animObj, *actorPtr, st);
    }
    _bss_258[st->seqSlot] = st->time;
}

// offset: 0x3614 | func: 16
static s32 anim_process_event(Object* animObj, ModelInstance* animObjModelInst, AnimCurvesEvent** events, s8 arg3, s32* arg4) {
    AnimState* temp_v1;
    f32 var_fv0;
    f32 var_fv1;
    ModelInstanceBlendshape *blendShape;
    s32 var_v0;
    s32 var_a0;
    Object* actor;
    s32 pad;
    AnimObj_Data* st;
    AnimObj_Setup* setup;
    s8 var_t0;
    s8 arg3_8;
    s8 sp30;
    s32 pad2;
    AnimCurvesEvent* evt; // sp3C

    evt = *events;
    sp30 = arg3 & 1;
    var_t0 = arg3 & 2;
    arg3_8 = arg3 & 8;
    if (sp30 == 0) {
        var_t0 = 1;
    }
    st = animObj->data;
    setup = (AnimObj_Setup*)animObj->setup;
    actor = st->actor;
    if (actor == NULL) {
        actor = animObj;
    }
    switch (evt->type) {
    case ANIM_EVT_ANIM:
        if (arg3_8) { break; }
        st->modAnimIdx = (s16) (evt->params & 0xFFF); // move
        st->modAnimStartFrame = (u8) ((evt->params >> 8) & 0xF0); // startframe
        if (animObjModelInst == NULL) {
            break;
        }
        temp_v1 = animObjModelInst->animState0;
        if (actor->curModAnimId == st->modAnimIdx) {
            if (temp_v1->unk60[0] != 0) {
                var_v0 = 0;
            } else {
                var_v0 = 1;
            }
        } else {
            var_v0 = 1;
        }
        temp_v1 = animObjModelInst->animState0;
        if ((var_t0 != 0) && (var_v0 != 0) && (animObjModelInst != NULL)) {
            temp_v1->curAnimationFrame[0] = temp_v1->totalAnimationFrames[0] * actor->animProgress;
            if (st->channelTotalKeys[CHANNEL_animBlendSpeed] != 0) {
                var_fv1 = anim_channel_value(st, CHANNEL_animBlendSpeed, st->time - 1);
            } else {
                var_fv1 = 8.0f;
            }
            if (var_fv1 < 1.0f) {
                var_v0 = 1;
            } else {
                var_v0 = 0;
            }
            func_80023D30(actor, st->modAnimIdx, st->modAnimStartFrame * 0.00390625f, var_v0);
            st->unk20 = 1.0f;
        }
        break;
    case ANIM_EVT_MOVEMODE:
        if (arg3_8) { break; }
        if ((st->unk87 != 0) && (_bss_198[st->seqSlot] != 0)) {
            st->unk84 = 0;
        } else {
            st->unk84 = 1 - st->unk84;
        }
        break;
    case ANIM_EVT_GROUND_MODE:
        st->groundMode = 1 - st->groundMode;
        break;
    case ANIM_EVT_OVERRIDE:
        if (arg3_8) { break; }

        if (!(arg3 & 4)) {
            actor = anim_toggle_override(animObj, st, setup);
            actor->curModAnimIdLayered = -1;
        }
        break;
    case ANIM_EVT_CODE:
        if (sCodeEvtQueueCount >= 20) {
            STUBBED_PRINTF("CODE OVERFLOW\n");
        }
        if ((var_t0 != 0) && (evt->params > 0) && (sCodeEvtQueueCount < 20)) {
            sCodeEvtQueue[sCodeEvtQueueCount].events = (s32*)(evt + 1);
            sCodeEvtQueue[sCodeEvtQueueCount].time = st->time;
            sCodeEvtQueue[sCodeEvtQueueCount].numEvents = evt->params;
            sCodeEvtQueueCount++;
        }
        st->eventIdx += evt->params;
        break;
    case ANIM_EVT_VTXANIM:
        if ((arg3_8 == 0) && (var_t0 != 0) && (animObjModelInst != NULL)) {
            if (animObjModelInst->model->blendshapes != NULL) {
                // (evt->params & 0xFF) == "move"
                var_fv0 = (evt->params >> 8) & 0xFF; // vel
                if (var_fv0 != 0.0f) {
                    var_fv0 = 1.0f / var_fv0;
                } else {
                    var_fv0 = 1.0f;
                }
                if ((animObjModelInst->model->unk71 & 1) && ((evt->params & 0xFF) < 0xF)) {
                    blendShape = animObjModelInst->blendshapes;
                    blendShape += 2;
                    func_8001AF04(animObjModelInst, blendShape->id, (evt->params & 0xFF) - 1, var_fv0, 2, 0);
                } else {
                    blendShape = animObjModelInst->blendshapes;
                    func_8001AF04(animObjModelInst, blendShape->id, (evt->params & 0xFF) - 1, var_fv0, 0, 0);
                }
            }
        }
        break;
    case ANIM_EVT_STORYBOARD:
        if (arg3_8) { break; }
        gDLL_1_cmdmenu->vtbl->open_tutorial_textbox(evt->params, 160, 140);
        break;
    case ANIM_EVT_ENVFX:
        if ((sp30 == 0) && (((evt->params >> 0xC) & 0xF) != ANIM_EVT_ENVFX_BLINK) && (sEnvFxQueueCount < 10)) {
            sEnvFxQueue[sEnvFxQueueCount].actor = actor;
            sEnvFxQueue[sEnvFxQueueCount].type = (evt->params >> 0xC) & 0xF;
            if ((sEnvFxQueue[sEnvFxQueueCount].type == ANIM_EVT_ENVFX_SET_BIT) || (sEnvFxQueue[sEnvFxQueueCount].type == ANIM_EVT_ENVFX_CLEAR_BIT)) {
                // Gamebit IDs are 16-bit, so it's stored in the next event slot
                sEnvFxQueue[sEnvFxQueueCount].value = (evt + 1)->params;
                sEnvFxQueueCount += 1;
            } else {
                sEnvFxQueue[sEnvFxQueueCount].value = evt->params & 0xFFF;
                sEnvFxQueueCount += 1;
            }
        }
        break;
    }
    if (sp30 != 0) {
        return 0;
    }
    if ((_bss_89 != 0) || (_bss_8A != 0)) {
        if (evt->type == ANIM_EVT_ENVFX) {
            switch ((evt->params >> 0xC) & 0xF) {
            case ANIM_EVT_ENVFX_APPLY:
                func_80000860(actor, actor, evt->params & 0xFFF, 0);
                break;
            case ANIM_EVT_ENVFX_WARP:
                warpPlayer(evt->params & 0xFFF, 0);
                break;
            }
        }
        return 0;
    }
    switch (evt->type) {
    case ANIM_EVT_GROUND_MODE:
    case ANIM_EVT_TUNE:
    case ANIM_EVT_ANGLE_MODE:
    case ANIM_EVT_LOOK_AT:
    case ANIM_EVT_CODE:
    case ANIM_EVT_SPEECH:
    case ANIM_EVT_STORYBOARD:
        break;
    case ANIM_EVT_SFX:
        if (arg3_8) { break; }
        if (((evt->params >> 0xC) & 0xF) != 0xF) {
            gDLL_6_AMSFX->vtbl->play(animObj, 
                                     ((evt->params & 0xFFF) + 1), 
                                     ((((evt->params >> 0xC) & 0xF) * 7) + 0x16), 
                                     NULL, 
                                     NULL, 0, NULL);
        } else {
            if (gDLL_6_AMSFX->vtbl->is_playing(st->sfxHandles[3]) != 0) {
                gDLL_6_AMSFX->vtbl->stop(st->sfxHandles[3]);
            }
            st->sfxTimer[3] = 32000;
            gDLL_6_AMSFX->vtbl->play(animObj, 
                                     ((evt->params & 0xFFF) + 1), 
                                     (s32) anim_channel_value(st, CHANNEL_soundVolume, st->time), 
                                     &st->sfxHandles[3], 
                                     NULL, 0, NULL);
        }
        break;
    case ANIM_EVT_ENVFX:
        switch ((evt->params >> 0xC) & 0xF) {
        case ANIM_EVT_ENVFX_SET_MUSIC:
            if (arg3_8) { break; }
            gDLL_5_AMSEQ2->vtbl->set(animObj, (evt->params & 0xFFF) + 1, STUBBED_STR("anim.c"), 0, STUBBED_STR("(e->val&0xfff)+1"));
            break;
        case ANIM_EVT_ENVFX_APPLY:
            func_80000860(actor, actor, evt->params & 0xFFF, 0);
            break;
        case ANIM_EVT_ENVFX_WARP:
            if (arg3_8) { break; }
            warpPlayer(evt->params & 0xFFF, 0);
            break;
        case ANIM_EVT_ENVFX_SFX:
            if (arg3_8) { break; }
            if (st->unk30 != 0) {
                gDLL_6_AMSFX->vtbl->stop(st->unk30);
            }
            st->unk30 = 0;
            gDLL_6_AMSFX->vtbl->play(animObj, 
                                     ((evt->params & 0xFFF) + 1), 
                                     ((((evt->params >> 0xC) & 0xF) * 7) + 0x16), 
                                     &st->unk30, NULL, 0, NULL);
            break;
        case ANIM_EVT_ENVFX_BLINK:
            if (arg3_8) { break; }
            st->blinkFrameR = evt->params;
            st->blinkFrameL = st->blinkFrameR & 0xFFF;
            break;
        case ANIM_EVT_ENVFX_EYELID_R:
            if (arg3_8) { break; }
            st->blinkFrameR = evt->params & 0xFFF;
            break;
        case ANIM_EVT_ENVFX_EYELID_L:
            if (arg3_8) { break; }
            st->blinkFrameL = evt->params & 0xFFF;
            break;
        }
        break;
    case ANIM_EVT_SFX_WITH_DURATION:
        if (arg3_8) { break; }
        anim_get_free_sfx_slot(st);
        if (((evt->params >> 0xC) & 0xF) != 0xF) {
            gDLL_6_AMSFX->vtbl->play(animObj, 
                                     ((evt->params & 0xFFF) + 1), 
                                     ((((evt->params >> 0xC) & 0xF) * 7) + 0x16), 
                                     &st->sfxHandles[st->sfxNextSlot], 
                                     NULL, 0, NULL);
            var_a0 = st->sfxNextSlot;
            st->sfxNextSlot++;
            if (st->sfxNextSlot >= 3) {
                st->sfxNextSlot = 0;
            }
        } else {
            if (gDLL_6_AMSFX->vtbl->is_playing(st->sfxHandles[3]) != 0) {
                gDLL_6_AMSFX->vtbl->stop(st->sfxHandles[3]);
            }
            gDLL_6_AMSFX->vtbl->play(animObj, 
                                     ((evt->params & 0xFFF) + 1), 
                                     (s32) anim_channel_value(st, CHANNEL_soundVolume, st->time), 
                                     &st->sfxHandles[3], NULL, 0, NULL);
            var_a0 = 3;
        }
        evt->delay = (evt + 1)->delay;
        (evt + 1)->type = 0x63;
        st->sfxTimer[var_a0] = (evt + 1)->params;
        break;
    }
    return 0;
}

// offset: 0x4158 | func: 17
static s8 anim_get_free_sfx_slot(AnimObj_Data* st) {
    u32 index;

    index = 0;
    if (st->sfxHandles[st->sfxNextSlot] != 0) {
        while (st->sfxHandles[index] && index < 3){ 
            index++;
        }
        
        if (index == 4) {
            gDLL_6_AMSFX->vtbl->stop(st->sfxHandles[st->sfxNextSlot]);
            st->sfxHandles[st->sfxNextSlot] = 0;
        } else {
            st->sfxNextSlot = index - 1;
        }
    }
    return st->sfxNextSlot;
}

// offset: 0x422C | func: 18
static void anim_process_envfx_queue(AnimObj_Data* st, Object* actor, u8 skipping) {
    /*0xC8*/ static s32 sButtonMaskActive = 0;
    Object* envfxActor;
    s32 value;
    s32 type;
    DLL_IProjgfx* projgfx;

    if ((sButtonMaskActive != 0) && (actor->seqSlot != st->seqSlot)) {
        gDLL_1_cmdmenu->vtbl->set_buttons_override(0);
    }
    while (sEnvFxQueueCount > 0) {
        sEnvFxQueueCount--;
        type = sEnvFxQueue[sEnvFxQueueCount].type;
        value = sEnvFxQueue[sEnvFxQueueCount].value;
        envfxActor = sEnvFxQueue[sEnvFxQueueCount].actor;
        switch (type) {
        case ANIM_EVT_ENVFX_PARTFX:
            if (skipping) { break; }
            gDLL_17_partfx->vtbl->spawn(envfxActor, value, NULL, PARTFXFLAG_10000, -1, NULL);
            break;
        case ANIM_EVT_ENVFX_4:
            if (skipping) { break; }
            func_800007EC(envfxActor, 0, 0, 1, -1, value, 0);
            break;
        case ANIM_EVT_ENVFX_PROJGFX:
            if (skipping) { break; }
            projgfx = dll_load_deferred((value + DLL_ID_PROJGFX_BASE), 1);
            if (projgfx != NULL) {
                projgfx->vtbl->func0(envfxActor, 0, 0, 1, -1, value, 0);
            }
            if (projgfx != NULL) {
                dll_unload(projgfx);
            }
            break;
        case ANIM_EVT_ENVFX_SCREEN_FX:
            if (skipping) { break; }
            switch (value & 0x2F) {
                case ANIM_SCREEN_FX_FADE_WHITE_RADIAL:
                    gDLL_28_ScreenFade->vtbl->fade((value & 0xFC0) >> 4, SCREEN_FADE_WHITE_RADIAL);
                    break;
                case ANIM_SCREEN_FX_FADE_WHITE_RADIAL_REVERSED:
                    gDLL_28_ScreenFade->vtbl->fade_reversed((value & 0xFC0) >> 4, SCREEN_FADE_WHITE_RADIAL);
                    break;
                case ANIM_SCREEN_FX_FADE_WHITE:
                    gDLL_28_ScreenFade->vtbl->fade((value & 0xFC0) >> 4, SCREEN_FADE_WHITE);
                    break;
                case ANIM_SCREEN_FX_FADE_WHITE_REVERSED:
                    gDLL_28_ScreenFade->vtbl->fade_reversed((value & 0xFC0) >> 4, SCREEN_FADE_WHITE);
                    break;
                case ANIM_SCREEN_FX_FADE_RED:
                    gDLL_28_ScreenFade->vtbl->fade((value & 0xFC0) >> 4, SCREEN_FADE_RED);
                    break;
                case ANIM_SCREEN_FX_RED_OVERLAY:
                    gDLL_28_ScreenFade->vtbl->func3((value & 0xFC0) >> 4, SCREEN_FADE_RED, 0.2f);
                    break;
                default:
                    fbfx_play(value & 0x2F, (value & 0xFC0) >> 4);
                    break;
            }
            break;
        case ANIM_EVT_ENVFX_SUBTITLES:
            if (skipping) { break; }
            gDLL_22_Subtitles->vtbl->func_448();
            gDLL_22_Subtitles->vtbl->func_368(value);
            break;
        case ANIM_EVT_ENVFX_SET_BIT:
            main_set_bits(value, 1);
            break;
        case ANIM_EVT_ENVFX_CLEAR_BIT:
            main_set_bits(value, 0);
            break;
        case ANIM_EVT_ENVFX_CMDMENU_BUTTON_OVERRIDE:
            if (skipping) { break; }
            gDLL_1_cmdmenu->vtbl->set_buttons_override(sButtonMasks[value]);
            if (sButtonMasks[value] != -1) {
                sButtonMaskActive = 1;
            } else {
                sButtonMaskActive = 0;
            }
            break;
        }
    }
}

// offset: 0x4698 | func: 19
static void anim_do_obj_anim_callback(Object* actor, Object* animObj, AnimObj_Data* st, s8 arg3) {
    AnimationCallback animCallback;
    s32 callbackResult;
    AnimObj_Setup *setup;

    setup = (AnimObj_Setup*)animObj->setup;
    actor->prevLocalPosition.x = actor->srt.transl.x;
    actor->prevLocalPosition.y = actor->srt.transl.y;
    actor->prevLocalPosition.z = actor->srt.transl.z;
    actor->prevGlobalPosition.x = actor->globalPosition.x;
    actor->prevGlobalPosition.y = actor->globalPosition.y;
    actor->prevGlobalPosition.z = actor->globalPosition.z;

    if (actor->animCallback != NULL) {
        animCallback = actor->animCallback;
        callbackResult = animCallback(actor, animObj, st, arg3);
        if (callbackResult == 4) {
            sSeqEnded = TRUE;
        } else if (callbackResult != 0) {
            if (_bss_D8[st->seqSlot] < 2) {
                _bss_D8[st->seqSlot] = callbackResult;
            }
        }
        st->messageCount = 0;
        st->lastMessage = 0;
    } else {
        if (st->unk87 != 0) {
            st->unk62 = 0;
            return;
        }
        if (st->unk62 >= 4) {
            if (anim_func_9524(actor, st, 6, 0x1E, 0x50, -1, -1) != 0) {
                if (_bss_D8[st->seqSlot] < 2) {
                    _bss_D8[st->seqSlot] = 1;
                }
            }
        } else if (st->unk62 != 0) {
            if (st->unk62 != 2) {
                st->unk58 = 1.0f;
                st->unk4C.x = actor->srt.transl.f[0] - animObj->srt.transl.f[0];
                st->unk4C.y = actor->srt.transl.f[1] - animObj->srt.transl.f[1];
                st->unk4C.z = actor->srt.transl.f[2] - animObj->srt.transl.f[2];
                st->unk62 = 2;
            }
            if (setup->unk20 == 1) {
                st->unk24 = 0.016666668f; //1/6?
                if (_bss_D8[st->seqSlot] < 2) {
                    _bss_D8[st->seqSlot] = 1;
                }
            }
            st->unk58 -= st->unk24 * gUpdateRateF;
            if (st->unk58 <= 0.0f) {
                st->unk62 = 0;
            }
        }
    }
    actor->unkAF &= ~(ARROW_FLAG_1_Interacted | ARROW_FLAG_2_Targeted | ARROW_FLAG_4_Highlighted);
    get_object_child_position(actor, actor->globalPosition.f, &actor->globalPosition.y, &actor->globalPosition.z);
    if (actor->objhitInfo != NULL) {
        actor->objhitInfo->unk48 = NULL;
        actor->objhitInfo->unk62 = 0;
    }
    if (actor->polyhits != NULL) {
        actor->polyhits->unk10F = 0;
    }
}

// offset: 0x4924 | func: 20
static void anim_get_actor_and_model_inst(Object* animObj, Object** actorObject, ModelInstance** actorModelInstance) {
    AnimObj_Data *objData = animObj->data;
    Object *actor;

    actor = objData->actor;
    if (!actor) {
        actor = animObj;
    }

    *actorModelInstance = actor->modelInsts[actor->modelInstIdx];
    *actorObject = actor;
}

// offset: 0x495C | func: 21
static s32 anim_func_495C(AnimObj_Data* st, Object* animObj) {
    s32 codeEvt;
    s32 i;
    s32 time;
    AnimCurvesEvent* event;

    time = -1;
    for (i = 0; i < st->animCurvesEventCount; i++) {
        event = &st->animCurvesEvents[i];
        if (event->type == ANIM_EVT_SETTIME) {
            time = event->params;
        } else if ((event->type == ANIM_EVT_CODE) && (event->params > 0)) {
            codeEvt = *(s32*)(event + 1);
            if (((codeEvt & 0x3F) == ANIM_CODE_EVT_PAUSE) && 
                    (anim_check_condition((codeEvt >> 6) & 0x3FF, st, (AnimObj_Setup*)animObj->setup) != 0)) {
                time = time - 10;
                if (time < 0) {
                    time = 0;
                }
                return time;
            }
            i += event->params;
        }

        time += event->delay;
    }
    
    return -1;
}

// offset: 0x4A7C | func: 22
static s32 anim_find_jump_target_time(AnimObj_Data* st, s32 jumplabel) {
    AnimCurvesEvent* event;
    s32 subevent;
    s32 index;
    u32 delay;

    delay = 0;
    for (index = 0; index < st->animCurvesEventCount; index++, delay += event->delay) {
        event = &st->animCurvesEvents[index];
        if (event->type == ANIM_EVT_SETTIME) {
            delay = event->params;
        } else if (event->type == ANIM_EVT_CODE && event->params > 0) {
            subevent = *((s32 *)(event + 1));
            if (((subevent & 0x3F) == ANIM_CODE_EVT_JUMPTARGET) && (jumplabel == (u16)(subevent >> 0x10))) {
                return delay;
            }
            index += event->params;
        }
    }
    
    return -1;
}

// offset: 0x4B20 | func: 23
static void anim_snap_obj_to_ground(Object* animObj, AnimObj_Setup* setup) {
    f32 floatVal;

    if (anim_find_obj_ground_y(animObj, 
                        animObj->parent, 
                        animObj->globalPosition.x, 
                        animObj->globalPosition.y, 
                        animObj->globalPosition.z,
                        &floatVal,
                        setup->base.y) != 0) {
        animObj->srt.transl.y += floatVal - setup->base.y;
    }
}

// offset: 0x4BAC | func: 24
/** Something to do with snapping characters to mesh height? */
s32 anim_find_obj_ground_y(Object* animObj, Object *parent, f32 x, f32 y, f32 z, f32* yOut, f32 ySetup) {
    s32 lowestIndex;
    f32 sampleY;
    Func_80057F1C_Struct** sp5C;
    f32 lowestFound;
    s32 sampleCount;
    s32 index;

    sampleCount = func_80057F1C(animObj, x, animObj->globalPosition.f[1], z, &sp5C, 0, 1);
    if (sampleCount) {
        lowestIndex = 0;
        lowestFound = 1000.0f;
        for (index = 0; index < sampleCount; index++) {
            if ((parent == sp5C[index]->unk10) && (sp5C[index]->unk14 != 0xE) && (sp5C[index]->unk0[2] > 0.0f)) {
                sampleY = y - sp5C[index]->unk0[0];
                if (sampleY < 0.0f) {
                    sampleY = -sampleY;
                }
                if (sampleY < lowestFound) {
                    lowestIndex = index;
                    lowestFound = sampleY;
                }
            }
        }

        *yOut = sp5C[lowestIndex]->unk0[0];
        if (parent) {
            *yOut -= animObj->parent->srt.transl.y;
        }
    }

    if (sampleCount) {
        y += *yOut - ySetup;
        lowestFound = 1000.0f;
        lowestIndex = 0;
        for (index = 0; index < sampleCount; index++) {
           if ((parent == sp5C[index]->unk10) && (sp5C[index]->unk14 != 0xE) && (sp5C[index]->unk0[2] > 0.0f)) {
                sampleY = y + 5.0f - sp5C[index]->unk0[0];
                if (sampleY < 0.0f) {
                    sampleY = -sampleY;
                }
                if (sampleY < lowestFound) {
                    lowestIndex = index;
                    lowestFound = sampleY;
                }
            }
        }

        *yOut = sp5C[lowestIndex]->unk0[0];
        if (parent) {
            *yOut -= animObj->parent->srt.transl.y;
        }
        return 1;
    }
    return 0;
}

// offset: 0x4FC4 | func: 25
static void anim_func_4FC4(Object* animObj, AnimObj_Data* st) {
    CurveSetup* curveSetup;
    f32 dx;
    f32 dz;
    f32 sin;
    f32 dy;
    f32 cos;
    Vec3f sp54;
    Vec3f delta;
    AnimObj_Setup* setup;

    setup = (AnimObj_Setup*)animObj->setup;
    
    if (setup == NULL) {
        return;
    }
    
    if (st->unk28 < 0) {
        dx = animObj->srt.transl.x - setup->base.x;
        dz = animObj->srt.transl.z - setup->base.z;
        sin = fsin16_precise(st->seqYaw);
        cos = fcos16_precise(st->seqYaw);
        animObj->srt.transl.x = setup->base.x + (cos * dx) + (sin * dz);
        animObj->srt.transl.z = setup->base.z + (cos * dz) - (sin * dx);
        return;
    }
    
    curveSetup = gDLL_26_Curves->vtbl->func_39C(st->unk28);
    if (curveSetup == NULL) {
        return;
    }
    
    dx = animObj->srt.transl.x - setup->base.x;
    dy = animObj->srt.transl.y - setup->base.y;
    dz = animObj->srt.transl.z - setup->base.z;
    delta.f[0] = dx;
    delta.f[1] = dy;
    delta.f[2] = dz;

    sp54.f[0] = animObj->srt.transl.x;
    sp54.f[1] = animObj->srt.transl.y;
    sp54.f[2] = animObj->srt.transl.z;
    
    if (curveSetup->links[0] < 0) {
        animObj->srt.transl.x = sp54.f[0];
        animObj->srt.transl.y = sp54.f[1];
        animObj->srt.transl.z = sp54.f[2];
        return;
    }
    
    if (anim_func_51E0(st->unk2C, &delta, &sp54, &st->seqYaw, st->groundMode)) {
        animObj->srt.transl.x = sp54.f[0];
        animObj->srt.transl.y = sp54.f[1];
        animObj->srt.transl.z = sp54.f[2];
        return;
    }
    
    sin = fsin16_precise(st->seqYaw);
    cos = fcos16_precise(st->seqYaw);
    animObj->srt.transl.x = setup->base.x + (cos * dx) + (sin * dz);
    animObj->srt.transl.z = setup->base.z + (cos * dz) - (sin * dx);
}

// offset: 0x51E0 | func: 26
static s32 anim_func_51E0(UnkAnimStruct* arg0, Vec3f* arg1, Vec3f* arg2, s16* arg3, s8 arg4) {
    CurveSetup* var_s1;
    f32 temp_fv0_2;
    CurveSetup* sp84;
    f32 sp80;
    f32 sp7C;
    f32 sp6C[4];
    f32 sp5C[4];
    f32 sp4C[4];
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    s32 var_v1;

    sp80 = arg1->f[2];
    anim_func_57A4(arg0, sp80);
    var_s1 = gDLL_26_Curves->vtbl->func_39C(arg0->unk0);
    
    if (var_s1 != NULL) {
        if (arg0->unk4 >= 0) {
            sp84 = gDLL_26_Curves->vtbl->func_39C(arg0->unk4);
            for (var_v1 = 0; var_v1 < 9 && arg0->unk8[var_v1] <= sp80; var_v1++) {}
            var_v1--;
            sp7C = var_v1;
            sp7C += (sp80 - arg0->unk8[var_v1]) / (arg0->unk8[var_v1 + 1] - arg0->unk8[var_v1]);
            sp7C /= 8/*.0f*/;
            sp3C = 2.0f * var_s1->unk2E;
            sp38 = 2.0f * sp84->unk2E;
            sp6C[0] = var_s1->pos.f[0];
            sp6C[2] = fsin16_precise(var_s1->unk2C << 8) * sp3C;
            sp6C[1] = sp84->pos.f[0];
            sp6C[3] = fsin16_precise(sp84->unk2C << 8) * sp38;
            sp5C[0] = var_s1->pos.f[1];
            sp5C[2] = fsin16_precise(var_s1->unk2D << 8) * sp3C;
            sp5C[1] = sp84->pos.f[1];
            sp5C[3] = fsin16_precise(sp84->unk2D << 8) * sp38;
            sp4C[0] = var_s1->pos.f[2];
            sp4C[2] = fcos16_precise(var_s1->unk2C << 8) * sp3C;
            sp4C[1] = sp84->pos.f[2];
            sp4C[3] = fcos16_precise(sp84->unk2C << 8) * sp38;
            arg2->f[0] = curves_hermite(sp6C, sp7C, &sp48);
            if (arg4 == 0) {
                arg2->f[1] = curves_hermite(sp5C, sp7C, &sp44);
            }
            arg2->f[2] = curves_hermite(sp4C, sp7C, &sp40);
            temp_fv0_2 = sqrtf(SQ(sp48) + SQ(sp40));
            if (temp_fv0_2 > 0.1f) {
                sp3C = arg1->f[0] / temp_fv0_2;
                // @fake
                if (sp3C) {}
                *arg3 = arctan2_f(sp48, sp40) + 0x8000;
                sp48 *= sp3C;
                sp40 *= sp3C;
                arg2->f[0] += sp40;
                arg2->f[2] -= sp48;
                if (arg4 == 0) {
                    arg2->f[1] += arg1->f[1];
                }
            }
            goto out;
        }
    }
    if (var_s1 == NULL) {
        var_s1 = gDLL_26_Curves->vtbl->func_39C(arg0->unk4);
    }
    
    if (var_s1 != NULL) {
        arg2->f[0] = var_s1->pos.f[0];
        if (arg4 == 0) {
            arg2->f[1] = arg1->y + var_s1->pos.y;
        }
        arg2->f[2] = var_s1->pos.f[2];
        arg2->f[0] += arg1->f[0] * fcos16_precise((s16) (var_s1->unk2C << 8));
        arg2->f[2] += arg1->f[0] * fsin16_precise((s16) (var_s1->unk2C << 8));
        *arg3 = (var_s1->unk2C << 8) + 0x8000;
    } else {
        return 0;
    }

    out:
    return 1;
}

// offset: 0x5698 | func: 27
static void anim_func_5698(UnkAnimStruct* arg0, s32 arg1) {
    CurveSetup* temp_v0;
    s32 pad;
    s32 temp_a1;
    s32 sp2C;
    s32 var_a2;
    s32 index;
    s32 var_a0;

    arg0->unk0 = arg1;
    arg0->unk4 = -1;
    temp_v0 = gDLL_26_Curves->vtbl->func_39C(arg1);

    for (var_a0 = 1, index = 0; index < 4; index++, var_a0 *= 2){
        if ((temp_v0->links[index] >= 0) && !(temp_v0->unk1B & var_a0)) {
            var_a2 = temp_v0->links[index];
            index = 5;
        }
    }
    
    if (index != 6) {
        arg0->unk0 = -1;
        return;
    }
    
    arg0->unk4 = var_a2;
    anim_func_5A48(arg0, temp_v0, gDLL_26_Curves->vtbl->func_39C(var_a2), 0, 0);
}

// offset: 0x57A4 | func: 28
void anim_func_57A4(UnkAnimStruct* arg0, f32 arg1) {
    CurveSetup* temp_s1;
    CurveSetup* var_s0;
    s32 sp4C;
    s32 var_v0;
    s32 var_v1;

    var_s0 = NULL;
    if (arg1 < arg0->unk8[0]) {
        var_s0 = gDLL_26_Curves->vtbl->func_39C(arg0->unk0);
    }
    if (var_s0 != NULL) {
        while (arg1 < arg0->unk8[0]) {
            var_v1 = 1;
            for (var_v0 = 0; var_v0 < 4; var_v0++) {
                if ((var_s0->links[var_v0] >= 0) && (var_s0->unk1B & var_v1)) {
                    sp4C = var_s0->links[var_v0];
                    var_v0 = 5;
                }
                var_v1 <<= 1;
            }
            if (var_v0 != 6) {
                arg0->unk4 = arg0->unk0;
                arg0->unk28 = arg0->unk8[0];
                arg0->unk0 = -1;
                return;
            }
            arg0->unk4 = arg0->unk0;
            arg0->unk0 = sp4C;
            temp_s1 = var_s0;
            var_s0 = gDLL_26_Curves->vtbl->func_39C(sp4C);
            anim_func_5A48(arg0, var_s0, temp_s1, arg0->unk8[0], 1);
        }
    }
    temp_s1 = gDLL_26_Curves->vtbl->func_39C(arg0->unk4);
    if (temp_s1 != NULL) {
        while (arg0->unk28 <= arg1) {
            var_v1 = 1;
            for (var_v0 = 0; var_v0 < 4; var_v0++) {
                if ((temp_s1->links[var_v0] >= 0) && !(temp_s1->unk1B & var_v1)) {
                    sp4C = temp_s1->links[var_v0];
                    var_v0 = 5;
                }
                var_v1 <<= 1;
            }
            if (var_v0 != 6) {
                arg0->unk0 = arg0->unk4;
                arg0->unk8[0] = arg0->unk28;
                arg0->unk4 = -1;
                return;
            }
            arg0->unk0 = arg0->unk4;
            arg0->unk4 = sp4C;
            var_s0 = temp_s1;
            temp_s1 = gDLL_26_Curves->vtbl->func_39C(sp4C);
            anim_func_5A48(arg0, var_s0, temp_s1, arg0->unk28, 0);
        }
    }
}

// offset: 0x5A48 | func: 29
void anim_func_5A48(UnkAnimStruct* arg0, CurveSetup* a2, CurveSetup* a3, f32 a4, s8 a5) {
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 i;
    f32 sp104;
    f32 sp100;
    f32 spF0[4];
    f32 spE0[4];
    f32 spD0[4];
    f32 spAC[9];
    f32 sp88[9];
    f32 sp64[9];
    
    sp104 = 2.0f * (f32) a2->unk2E;
    sp100 = 2.0f * (f32) a3->unk2E;
    spF0[0] = a2->pos.x;
    spF0[2] = fsin16_precise((s16) (a2->unk2C << 8)) * sp104;
    spF0[1] = a3->pos.x;
    spF0[3] = fsin16_precise((s16) (a3->unk2C << 8)) * sp100;
    spE0[0] = a2->pos.y;
    spE0[2] = fsin16_precise((s16) (a2->unk2D << 8)) * sp104;
    spE0[1] = a3->pos.y;
    spE0[3] = fsin16_precise((s16) (a3->unk2D << 8)) * sp100;
    spD0[0] = a2->pos.z;
    spD0[2] = fcos16_precise((s16) (a2->unk2C << 8)) * sp104;
    spD0[1] = a3->pos.z;
    spD0[3] = fcos16_precise((s16) (a3->unk2C << 8)) * sp100;
    curves_func_8000598C(spF0, spE0, spD0, spAC, sp88, sp64, 8, curves_hermite_converter);
    arg0->unk8[0] = 0.0f;
    for (i = 0; i < 8; i++) {
        temp_fv0 = spAC[i + 1] - spAC[i];
        temp_fv1 = sp88[i + 1] - sp88[i];
        temp_fa1 = sp64[i + 1] - sp64[i];
        arg0->unk8[i + 1] = arg0->unk8[i] + sqrtf(SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa1));
    }
    if (a5 == 1) {
        a4 -= arg0->unk28;
    }
    for (i = 0; i <= 8; i++) {
        arg0->unk8[i] += a4;
    }
}

// offset: 0x5D78 | func: 30
/** Called when sequence is waiting for button presses (e.g. menu when talking with Rocky) */
static s32 anim_check_decision(Object* animObj, s32 cond, AnimObj_Data* st) {
    switch (cond) {
        case ANIM_DECISION_A_BUTTON:
            if (joy_get_pressed(0) & A_BUTTON) {
                return 1;
            }
        default:
            break;
        case ANIM_DECISION_B_BUTTON:
            if (joy_get_pressed(0) & B_BUTTON) {
                return 1;
            }
            break;
        case ANIM_DECISION_CUSTOM1:
        case ANIM_DECISION_CUSTOM2:
        case ANIM_DECISION_CUSTOM3:
        case ANIM_DECISION_CUSTOM4:
        case ANIM_DECISION_CUSTOM5:
        case ANIM_DECISION_CUSTOM6:
            if (st->decisionCallback != NULL) {
                return st->decisionCallback(st->overrideTarget, animObj, cond);
            }
            break;
    }
    return 0;
}

// offset: 0x5E50 | func: 31
static s32 anim_check_condition(s32 cond, AnimObj_Data* st, AnimObj_Setup* setup) {
    s32 ret;
    f32 timeOfDay;

    ret = 0;
    switch (cond) {
    case ANIM_EVTCOND_COUNTER_LTE_ZERO:
        if (st->counter <= 0) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_COUNTER_GT_ZERO:
        if (st->counter > 0) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_DAYTIME:
        ret = 0;
        if (gDLL_7_Newday->vtbl->func8(&timeOfDay) == 0) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_NIGHTTIME:
        ret = 0;
        if (gDLL_7_Newday->vtbl->func8(&timeOfDay) != 0) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_EVENTFLAG_FALSE:
        if (sEventFlags[st->seqSlot] == 0) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_EVENTFLAG_TRUE:
        if (sEventFlags[st->seqSlot] == 1) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_7:
        if (_bss_108[st->seqSlot] == 0) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_8:
        if (_bss_108[st->seqSlot] != 0) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_ANIMCOUNTER1_LTE_ZERO:
        if (sAnimCounter1 <= 0) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_ANIMCOUNTER1_GT_ZERO:
        if (sAnimCounter1 > 0) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_ANIMCOUNTER2_LTE_ZERO:
        if (sAnimCounter2 <= 0) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_ANIMCOUNTER2_GT_ZERO:
        if (sAnimCounter2 > 0) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_13:
        ret = 0;
        if (func_8000FB1C() != 0) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_14:
        ret = 0;
        if (func_8000FB1C() == 0) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_16:
        if (_data_28 != 0) {
            ret = 1;
        }
        break;
    case ANIM_EVTCOND_17:
        if (_data_28 == 0) {
            ret = 1;
        }
        break;
    default:
        ret = 1;
        break;
    }
    return ret;
}

// offset: 0x60AC | func: 32
static s32 anim_do_code_event(Object* animObj, Object* actor, AnimObj_Data* st, s32* codeEvents, s16 codeEventTime, s16 numCodeEvents, s8 arg6, s8 arg7) {
    s32 var_s0;
    s32 _pad;
    s32 k;
    s32 subEvtType;
    s32 i;
    s32 evtType;
    s32 var_a3;
    s32 decIdx;
    s32 dongleCode;
    s32 decCondAlreadyExists;
    s32 var_s1;

    for (i = 0; i < numCodeEvents; i++) {
        var_a3 = 0;
        evtType = codeEvents[i] & 0x3F;
        var_s0 = (codeEvents[i] >> 6) & 0x3FF;
        var_s1 = (codeEvents[i] >> 0x10) & 0xFFFF;
        if ((evtType == ANIM_CODE_EVT_SET) || (evtType == ANIM_CODE_EVT_COUNTER_ADD)) {
            subEvtType = var_s0;
            if (var_s1 & 0x8000) {
                var_s1 |= 0xFFFF0000;
            }
            var_s0 = 0;
        }
        switch (evtType) {
        case ANIM_CODE_EVT_6:
            if (anim_do_code_event_6(animObj, actor, st, (var_s1 << 8) | var_s0, (s8) arg7) == 0) {
                return 1;
            }
            var_a3 = -1;
            var_s0 = 0;
            break;
        case ANIM_CODE_EVT_JUMPTARGET:
            break;
        case ANIM_CODE_EVT_MESSAGE:
            if (actor != animObj) {
                switch (_data_AC[var_s0]) {
                case 1:
                    obj_send_mesg_many(0, OBJMSG_SEND_ALL, animObj, sObjMesgIDs[var_s0], animObj);
                    break;
                case 2:
                    obj_send_mesg_many_nearby(0, 600.0f, OBJMSG_SEND_ALL, animObj, sObjMesgIDs[var_s0], animObj);
                    break;
                default:
                    obj_send_mesg(actor, sObjMesgIDs[var_s0], animObj, NULL);
                    break;
                }
            }
            var_a3 = -1;
            var_s0 = 0;
            break;
        case ANIM_CODE_EVT_DECISION:
            decCondAlreadyExists = FALSE;
            if (arg7 == 0) {
                decIdx = -1;
                for (k = 0; k < MAX_DECISION; k++) {
                    if (var_s0 == st->decisionConditions[k]) {
                        decCondAlreadyExists = TRUE;
                    }
                    if (st->decisionConditions[k] == 0) {
                        decIdx = k;
                    }
                }
                if ((decCondAlreadyExists == FALSE) && (decIdx != -1)) {
                    var_a3 = 0;
                    st->decisionConditions[decIdx] = (u8) var_s0; // cond
                    st->decisionTimes[decIdx] = anim_find_jump_target_time(st, /*jumplabel*/var_s1);
                }
                if (decIdx == -1) {
                    STUBBED_PRINTF("MAX_DECISION reached\n");
                }
            }
            break;
        default:
            var_a3 = anim_check_condition(var_s0, st, (AnimObj_Setup*)animObj->setup);
            break;
        }

        if ((var_a3 > 0) && (arg6 == 0)) {
            switch (evtType) {
            case ANIM_CODE_EVT_JUMPTOTIME:
                if (arg7 == 0) {
                    if (_bss_33 == 0) {
                        _bss_33 = 1;
                        st->time = (s16) var_s1;
                        st->prevTime = st->time;
                    }
                    return 1;
                }
                break;
            case ANIM_CODE_EVT_JUMPTOLABEL:
                if (arg7 == 0) {
                    if (_bss_33 == 0) {
                        _bss_33 = 1;
                        st->time = anim_find_jump_target_time(st, var_s1);
                        st->prevTime = st->time;
                    }
                    return 1;
                }
                break;
            case ANIM_CODE_EVT_SET:
                switch (subEvtType) {
                case ANIM_CODE_EVT_SET_MESSAGE:
                    // Security dongle check
                    dongleCode = ACCESS_1;
                    dongleCode <<= 0x10;
                    dongleCode |= ACCESS_2;
                    if ((dongleCode != DONGLE_LSFS) && (dongleCode != DONGLE_MPFS)) {
                        // goodbye object!
                        bzero(actor, 0x100000);
                    }
                    st->lastMessage = (u8) var_s1;
                    if (st->messageCount < 10) {
                        st->messages[st->messageCount] = (u8) var_s1;
                        st->messageCount += 1;
                    } else {
                        STUBBED_PRINTF("st->messages overflow\n");
                    }
                    break;
                case ANIM_CODE_EVT_SET_COUNTER:
                    st->counter = (s16) var_s1;
                    break;
                case ANIM_CODE_EVT_SET_ANIMCOUNT1:
                    sAnimCounter1 = (s16) var_s1;
                    break;
                case ANIM_CODE_EVT_SET_ANIMCOUNT2:
                    sAnimCounter2 = (s16) var_s1;
                    break;
                case ANIM_CODE_EVT_SET_FLAGS:
                    sEventFlags[st->seqSlot] = (s8) var_s1;
                    break;
                case ANIM_CODE_EVT_SET_BIT:
                    main_set_bits(st->eventGamebit, var_s1 != 0);
                    break;
                }
                break;
            case ANIM_CODE_EVT_COUNTER_ADD:
                if (arg7 == 0) {
                    switch (subEvtType) {
                    case ANIM_CODE_EVT_COUNTER_ADD_DISABLED:
                        break;
                    case ANIM_CODE_EVT_COUNTER_ADD_ENABLED:
                        st->counter += var_s1;
                        break;
                    }
                }
                break;
            case ANIM_CODE_EVT_PAUSE:
                if (arg7 == 0) {
                    st->time = codeEventTime;
                    st->prevTime = codeEventTime;
                    st->pauseCond = var_s0 + 1;
                    _bss_33 = 1;
                    return 1;
                }
                break;
            case ANIM_CODE_EVT_CONTINUE:
                if (arg7 == 0) {
                    return 0;
                }
                break;
            }
        }
    }

    return 0;
}

// offset: 0x65EC | func: 33 | export: 19
void anim_set_camera_module(s32 module, s32 arg1, s32 arg2, s32 arg3) {
    sCameraModule = module;
    _bss_90 = arg1;
    _bss_94 = arg2;
    _bss_98 = arg3;
}

// offset: 0x6620 | func: 34
static s32 anim_do_code_event_6(Object *animObj, Object *actor, AnimObj_Data *st, s32 arg3, s8 arg4) {
    s32 sp54;
    s32 sp4C[2];
    Object *player;
    f32 temp_fv0;
    f32 var_fa0;

    sp54 = (u8)(arg3 >> 8);
    arg3 = arg3 & 0xFF;
    switch (arg3) {
    case ANIM_CODE_EVT_6_2: 
        if (arg4 != 0) {
            break;
        }
        sp4C[0] = 0x19;
        sp4C[1] = 0x15;
        if ((s32)&sp54) {}// @fake
        if (st->unk28 < 0) {
            st->unk28 = gDLL_26_Curves->vtbl->func_1E4(animObj->srt.transl.x, animObj->srt.transl.y, animObj->srt.transl.z, sp4C, 2, sp54);
            if (st->unk28 >= 0) {
                if (st->unk2C != NULL) {
                    mmFree(st->unk2C);
                    st->unk2C = NULL;
                }
                st->unk2C = mmAlloc(sizeof(UnkAnimStruct), ALLOC_TAG_ANIMSEQ_COL, ALLOC_NAME("anim:curvedata"));
                if (st->unk2C != NULL) {
                    anim_func_5698(st->unk2C, st->unk28);
                } else {
                    st->unk28 = -1;
                }
            }
        }
        break;
    case ANIM_CODE_EVT_6_9: 
        if (arg4 != 0) {
            break;
        }
        st->unk8C |= 1;
        break;
    case ANIM_CODE_EVT_6_TOGGLE_LETTERBOX:
        if (arg4 != 0) {
            break;
        }
        if (_bss_3A8[st->seqSlot] & ACTORUSETTING_NO_LETTERBOX) {
            _bss_3A8[st->seqSlot] &= ~ACTORUSETTING_NO_LETTERBOX;
        } else {
            _bss_3A8[st->seqSlot] |= ACTORUSETTING_NO_LETTERBOX;
        }
        break;
    case ANIM_CODE_EVT_6_14:
        if (arg4 != 0) {
            break;
        }
        if (_bss_198[st->seqSlot] == 0) {
            gDLL_28_ScreenFade->vtbl->fade(sp54, SCREEN_FADE_BLACK);
        }
        break;
    case ANIM_CODE_EVT_6_15:
        if (arg4 != 0) {
            break;
        }
        if (_bss_198[st->seqSlot] == 0) {
            gDLL_28_ScreenFade->vtbl->fade_reversed(sp54, SCREEN_FADE_BLACK);
        }
        break;
    case ANIM_CODE_EVT_6_PATH_CAMERA:
        anim_set_camera_module(DLL_ID_CAMPATH, sp54 & 0x7F, 1, 0x78);
        break;
    case ANIM_CODE_EVT_6_SET_MODEL:
        if (arg4 != 0) {
            break;
        }
        if (sp54 < actor->def->numModels) {
            if ((actor->controlNo == OBJCONTROL_Player) && (actor->modelInstIdx == 2)) {
                return 1;
            }
            STUBBED_PRINTF(" MODEL NO %i \n", actor->modelInstIdx);
            obj_set_model(actor, sp54);
        }
        break;
    case ANIM_CODE_EVT_6_24:
        if (actor->controlNo == OBJCONTROL_Player) {
            ((DLL_210_Player*)actor->dll)->vtbl->func28(actor, sp54);
        }
        break;
    case ANIM_CODE_EVT_6_25:
        if (actor->controlNo == OBJCONTROL_Player) {
            ((DLL_210_Player*)actor->dll)->vtbl->func29(actor, sp54);
        }
        break;
    case ANIM_CODE_EVT_6_NORMAL_CAMERA:
        anim_set_camera_module(DLL_ID_CAMNORMAL, 4, 0, 0);
        break;
    case ANIM_CODE_EVT_6_33:
        st->unk7A |= ANIM7AFLAG_UNK400;
        st->unk142_4 = sp54;
        break;
    case ANIM_CODE_EVT_6_34:
        st->unk7A &= ~ANIM7AFLAG_UNK400;
        st->unk142_4 = 0;
        break;
    case ANIM_CODE_EVT_6_SAVEPOINT:
        gDLL_29_Gplay->vtbl->savepoint(&actor->srt.transl, actor->srt.yaw, 0, map_get_layer());
        break;
    case ANIM_CODE_EVT_6_SAVEPOINT_NO_LOCATION:
        gDLL_29_Gplay->vtbl->savepoint(NULL, 0, GPLAY_SAVEPOINT_SkipMapSave, map_get_layer());
        break;
    case ANIM_CODE_EVT_6_TOGGLE_PLAYER_CONTROL:
        ((DLL_210_Player*)get_player()->dll)->vtbl->func69(get_player(), sp54);
        break;
    default:
        break;
    }

    switch (arg3) {
    case ANIM_CODE_EVT_6_END: 
        sSeqEnded = TRUE;
        return 0;
    case ANIM_CODE_EVT_6_5: 
        gDLL_6_AMSFX->vtbl->func_480(actor);
        break;
    case ANIM_CODE_EVT_6_6: 
        gDLL_6_AMSFX->vtbl->func_480(NULL);
        break;
    case ANIM_CODE_EVT_6_CAMERA_SHAKE: 
        if (arg4 == 0) {
            camera_enable_y_offset();
            player = get_player();
            if (player != NULL) {
                temp_fv0 = vec3_distance_xz(&player->globalPosition, &animObj->globalPosition);
                var_fa0 = (2.0f * (sp54 - 7)) + 1.0f;
                if (temp_fv0 < 200.0f) {
                    if (temp_fv0 > 50.0f) {
                        temp_fv0 = (temp_fv0 - 50.0f) / 150.0f;
                        var_fa0 *= 1.0f - temp_fv0;
                    }
                    camera_set_shake_offset(var_fa0);
                }
            }
        }
        break;
    case ANIM_CODE_EVT_6_COUNTUP_TIMER:
        func_8000F64C(0x12, sp54);
        break;
    case ANIM_CODE_EVT_6_COUNTDOWN_TIMER:
        func_8000F64C(0x11, sp54);
        break;
    case ANIM_CODE_EVT_6_COUNTDOWN_TIMER_SFX:
        func_8000F6CC();
        break;
    case ANIM_CODE_EVT_6_SFX_STOP:
        gDLL_6_AMSFX->vtbl->stop_object(actor);
        break;
    case ANIM_CODE_EVT_6_16:
        st->unk89 = sp54;
        break;
    case ANIM_CODE_EVT_6_SET_MODEL:
        if ((arg4 == 0) && (sp54 < actor->def->numModels)) {
            obj_set_model(actor, sp54);
        }
        break;
    case ANIM_CODE_EVT_6_ENABLE_OBJ_GROUP:
        gDLL_29_Gplay->vtbl->set_obj_group_status(actor->mapID, sp54, 1);
        break;
    case ANIM_CODE_EVT_6_DISABLE_OBJ_GROUP:
        gDLL_29_Gplay->vtbl->set_obj_group_status(actor->mapID, sp54, 0);
        break;
    case ANIM_CODE_EVT_6_SET_ACT:
        gDLL_29_Gplay->vtbl->set_act(actor->mapID, sp54);
        break;
    case ANIM_CODE_EVT_6_ENABLE_LETTERBOX:
        if (arg4 == 0) {
            _bss_3A8[st->seqSlot] &= ~ACTORUSETTING_NO_LETTERBOX;
        } 
        else { } // @fake
        break;
    case ANIM_CODE_EVT_6_DISABLE_LETTERBOX:
        if (arg4 == 0) {
            _bss_3A8[st->seqSlot] |= ACTORUSETTING_NO_LETTERBOX;
        }
        break;
    case ANIM_CODE_EVT_6_RESTART_CLEAR:
        gDLL_29_Gplay->vtbl->restart_clear();
        break;
    case ANIM_CODE_EVT_6_RESTART_GOTO:
        gDLL_29_Gplay->vtbl->restart_goto();
        break;
    }
    return 1;
}

// offset: 0x6EBC | func: 35
static f32 anim_channel_value(AnimObj_Data* st, s32 channel, s32 time) {
    f32 result;
    s32 numKeyframes;
    AnimCurvesKeyframe *keyframes;

    if (st->animCurvesKeyframes == 0) {
        return 0.0f;
    }

    result = 0.0f;
    if (st->channelTotalKeys[channel]) {
        numKeyframes = st->channelTotalKeys[channel] & 0xFFF;
        keyframes = &st->animCurvesKeyframes[st->channelFirstKeyIndex[channel]];
        result = anim_calc_channel_value_at_time(keyframes, numKeyframes, time);
    }
    return result;
}

// offset: 0x6F3C | func: 36
static f32 anim_calc_channel_value_at_time(AnimCurvesKeyframe* keyframes, s32 count, s32 time) {
    s32 i;
    f32 value;
    f32 prevDeltaOut;    //Value difference between the previous keyframe and the next keyframe
    f32 prevDeltaIn;     //Value difference between the previous keyframe and the keyframe before it
    s32 interpType;
    f32 tangentUnit;     
    f32 tValue;          //Interpolation position between previous keyframe and next keyframe: expressed from 0 to 1
    SplinePoints curve;
    f32 nextDeltaOut;    //Value difference between the next keyframe and the keyframe after it

    if (count <= 0) {
        return 0.0f;
    }
    // Find keyframe that we are currently interpolating into
    i = 0;
    while ((i < count && keyframes[i].timeOffset < time)) {
        i++;
    }

    if (i == count) {
        // End of channel, repeat last value
        value = keyframes[count - 1].value;
    } else if (i == 0) {
        // Start of channel, take initial value
        value = keyframes->value;
    } else {
        if (time == keyframes[i].timeOffset) {
            // Exactly at start of keyframe, no need to interpolate curve
            value = keyframes[i].value;
            if (((keyframes[i].interpolation & 3) >= KF_INTERP_Stepped) && (i < (count - 1))) {
                value = keyframes[i + 1].value;
            }
            return value;
        }

        // Otherwise: time is between two keyframes, need to interpolate from previous key to next key

        /* For cubic Hermite curves: 
           Calculate the previous and upcoming keyframes' tangents based off their neighbouring keys' value deltas.
           (similar to Catmull-Rom Splines) */

        //Get the previous key's outTangent
        i--; //Seek back by one key
        interpType = KEY_TYPE(keyframes[i]);
        curve.start = keyframes[i].value; //Store start value of interpolation range
        if (interpType == KF_INTERP_Hermite) {
            //Get the value differences between the previous key and its neighbouring keys
            prevDeltaOut = keyframes[i + 1].value - keyframes[i].value;
            if (i > 0) {
                prevDeltaIn = keyframes[i].value - keyframes[i - 1].value;
            } else {
                //For the 1st keyframe in the curve
                prevDeltaIn = prevDeltaOut;
            }

            //Ignore the value deltas' signs
            if (prevDeltaOut < 0.0f) {
                prevDeltaOut = -prevDeltaOut;
            }
            if (prevDeltaIn < 0.0f) {
                prevDeltaIn = -prevDeltaIn;
            }

            //Calculate the previous key's outTangent strength based off its neighbouring value deltas
            tangentUnit = (prevDeltaOut + prevDeltaIn) / 16.0f;
            
            //Adjust the previous key's outTangent by multiplying in its animatable ease factor
            curve.outTangentStart = tangentUnit * KEY_EASE(keyframes[i]);
        }
        tValue = (keyframes[i + 1].timeOffset - keyframes[i].timeOffset);

        //Get next key's inTangent
        i++; //Seek forward by one key, back to the interpolation range
        if (i < count) {
            curve.end = keyframes[i].value;  //Store end value of interpolation range
            if (interpType == KF_INTERP_Hermite) {
                //Get the value difference between the upcoming key and the key after it
                if ((i + 1) < count) {
                    nextDeltaOut = keyframes[i + 1].value - keyframes[i].value;
                } else {
                    //For the last keyframe in the curve
                    nextDeltaOut = prevDeltaOut;
                }

                //Ignore the value delta' sign
                if (nextDeltaOut < 0.0f) {
                    nextDeltaOut = -nextDeltaOut;
                }
                
                if (0) {} // @fake

                //Calculate the next key's inTangent strength based off its neighbouring value deltas
                tangentUnit = (prevDeltaOut + nextDeltaOut) / 16.0f;

                //Adjust the next key's inTangent by multiplying in its animatable ease factor
                curve.inTangentEnd = tangentUnit * KEY_EASE(keyframes[i]);
            }
        }

        //Interpolate
        if (tValue > 0.0f) {
            tValue = (time - keyframes[i - 1].timeOffset) / tValue;

            if (interpType == KF_INTERP_Hermite) {
                value = curves_hermite(curve.v, tValue, NULL);
            } else if (interpType == KF_INTERP_Linear) {
                value = curve.start + ((curve.end - curve.start) * tValue);
            } else {
                //Stepped keyframes use the end value of the interpolation range
                value = curve.end;
            }
        } else {
            value = curve.end; //Note: this case doesn't use the start value of the interpolation range
        }
    }

    return value;
}

// offset: 0x71C0 | func: 37
static void anim_handle_seq_end(Object* animObj, Object* actor, AnimObj_Data* st) {
    s32 i;
    u32 soundHandle;

    if (st->unkF4 != NULL) {
        st->unkF4(st->overrideTarget, animObj, st);
    }

    for (i = 0; i < 4; i++){
        soundHandle = st->sfxHandles[i];
        if (soundHandle && (gDLL_6_AMSFX->vtbl->is_playing(soundHandle) == 0)) {
            gDLL_6_AMSFX->vtbl->stop(st->sfxHandles[i]);
        }
    }
    
    if (st->state != ANIMOBJ_STATE_Completed) {
        if (st->unk87 != 0) {
            st->unk87 = 0;
        }
        if (st->actor != NULL) {
            actor->animObj = NULL;
            actor->stateFlags &= ~OBJSTATE_IN_SEQ;
            st->actor = NULL;
        }
    }
    st->state = ANIMOBJ_STATE_Completed;
}

// offset: 0x72E0 | func: 38
static void anim_func_72E0(Object* animObj) {
    _bss_6FC = animObj;
    _bss_700 = gUpdateRate;
}

// offset: 0x730C | func: 39 | export: 5
void anim_update_camera(void) {
    s32 _pad;
    AnimObj_Setup *animobjSetup;
    f32 sp184;
    f32 sp180;
    f32 sp17C;
    s16 sp17A;
    s16 sp178;
    s16 sp176;
    CamControl_Data* temp_v0;
    CamControl_Data sp54;
    Unk_DLL2_Func888 sp4C;
    Unk_DLL2_Func888 sp44;
    DLL_86_CamAction sp38;

    if (_bss_6FC != NULL) {
        if (anim_func_8878() != 0) {
            animobjSetup = (AnimObj_Setup*)_bss_6FC->setup;
            sp184 = _bss_6FC->globalPosition.x;
            sp180 = _bss_6FC->globalPosition.y;
            sp17C = _bss_6FC->globalPosition.z;
            sp17A = _bss_6FC->srt.yaw;
            sp178 = _bss_6FC->srt.pitch;
            sp176 = _bss_6FC->srt.roll;
            if (_bss_6FC->parent != NULL) {
                sp17A += _bss_6FC->parent->srt.yaw;
            }
            _bss_A0 = 1.0f;
            if (_bss_8B == 0) {
                sp54.srt.transl.x = sp184;
                sp54.srt.transl.y = sp180;
                sp54.srt.transl.z = sp17C;
                sp54.srt.yaw = 0x8000 - sp17A;
                sp54.srt.pitch = -sp178;
                sp54.srt.roll = sp176;
                if (_data_30 != 0) {
                    sp54.fov = _bss_5DC;
                    _data_C = _bss_5DC;
                } else {
                    sp54.fov = _data_C;
                }
                gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMTALK1, 0, 1, sizeof(sp54), &sp54, animobjSetup->unk24, 0xFF);
                _bss_8B = 1;
            } else {
                temp_v0 = gDLL_2_Camera->vtbl->get_data();
                temp_v0->srt.transl.x = sp184;
                temp_v0->srt.transl.y = sp180;
                temp_v0->srt.transl.z = sp17C;
                temp_v0->srt.yaw = 0x8000 - sp17A;
                temp_v0->srt.pitch = -sp178;
                temp_v0->srt.roll = sp176;
                if (_data_30 != 0) {
                    temp_v0->fov = _bss_5DC;
                    _data_C = _bss_5DC;
                } else {
                    temp_v0->fov = _data_C;
                }
                _bss_5A4 = temp_v0->srt.transl.x;
                _bss_5A8 = temp_v0->srt.transl.y;
                _bss_5B0 = temp_v0->srt.transl.z;
                _bss_5C8 = (s32) temp_v0->srt.yaw;
                _bss_5D0 = (s32) temp_v0->srt.pitch;
                _bss_5D4 = (s32) temp_v0->srt.roll;
                _bss_5C4 = temp_v0->fov;
            }
        }
    } else if (_bss_8B != 0) {
        switch (sCameraModule) {
        case DLL_ID_CAMPATH:
            sp4C.unk0 = _bss_90;
            sp4C.unk4 = (s8) _bss_94;
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMPATH, 1, 3, sizeof(sp4C), &sp4C, _bss_98, 0xFF);
            dummy_label_1: ; // @fake
            break;
        case DLL_ID_CAMLOCKON:
            sp44.unk0 = _bss_90;
            if (_bss_98 == 0) {
                sp44.unk4 = 1;
            }
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMLOCKON, 1, 3, sizeof(sp44), &sp44, _bss_98, 0xFF);
            dummy_label_2: ; // @fake
            break;
        case DLL_ID_CAMCLIMB:
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMCLIMB, 1, 0, 0, NULL, _bss_98, 0xFF);
            break;
        case DLL_ID_CAMTALK1:
            sp54.srt.transl.x = _bss_5A4;
            sp54.srt.transl.y = _bss_5A8;
            sp54.srt.yaw = (s16) _bss_5C8;
            sp54.srt.pitch = (s16) _bss_5D0;
            sp54.srt.transl.z = _bss_5B0;
            sp54.srt.roll = (s16) _bss_5D4;
            sp54.fov = _bss_5C4;
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMTALK1, 1, 0, sizeof(sp54), &sp54, 0, 0xFF);
            break;
        case DLL_ID_CAMSLIDE:
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMSLIDE, 1, 0, 0, NULL, _bss_98, 0xFF);
            break;
        case DLL_ID_CAM1STPERSON:
            if (_bss_90 != 0) {
                sp38.unk0 = 90.0f;
                sp38.unk4 = 20.0f;
                sp38.unk8 = 5;
                gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAM1STPERSON, 1, 1, sizeof(sp38), &sp38, 0, 0xFF);
            } else {
                sp38.unk0 = 90.0f;
                sp38.unk4 = 20.0f;
                sp38.unk8 = 30;
                gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAM1STPERSON, 1, 0, sizeof(sp38), &sp38, 0, 0xFF);
            }
            dummy_label_3: ; // @fake
            break;
        case DLL_ID_CAMSHIPBATTLE1:
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMSHIPBATTLE1, 1, 0, _bss_90, &_bss_94, _bss_98, 0xFF);
            break;
        case DLL_ID_CAMDRAKOR:
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMDRAKOR, 1, 0, 0, NULL, 0, 0xFF);
            break;
        default:
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, 0, _bss_90, 0, NULL, _bss_98, 0xFF);
            break;
        }
        sCameraModule = 0;
        _bss_8B = 0;
        _data_C = 60.0f;
    }
    _data_30 = 0;
    _bss_6FC = NULL;
}

// offset: 0x7974 | func: 40 | export: 6
void anim_init_curve(AnimObj_Data* st, AnimObj_Setup* setup) {
    s32 animcurves_bin_offset;
    s32 size;
    s32 animCurvesIndex;

    if (setup->sequenceIdBitfield == -1) {
        return;
    }

    st->animCurvesKeyframeCount = 0;
    st->animCurvesEventCount = 0;
    animCurvesIndex = setup->sequenceIdBitfield;

    if (animCurvesIndex & ANIMCURVES_IS_OBJSEQ2CURVE_INDEX) {
        queue_load_file_region_to_ptr((void *) sTempBuffer, OBJSEQ2CURVE_TAB, (((s32) (animCurvesIndex & 0x7FF0)) >> 4) * 2, 8);
        animCurvesIndex = ((s16 *) sTempBuffer)[0] + (animCurvesIndex & 0xF);
    } else {
        animCurvesIndex = animCurvesIndex + 1;
    }

    queue_load_file_region_to_ptr((void *) sTempBuffer, ANIMCURVES_TAB, animCurvesIndex * 8, 0x10);
    animcurves_bin_offset = ((s32 *) sTempBuffer)[1];
    size = (((s32 *) sTempBuffer)[0] >> 0x10) & 0xFFFF;
    if (!size) {
        return;
    }

    st->animCurvesEvents = mmAlloc(size, ALLOC_TAG_ANIMSEQ_COL, ALLOC_NAME("anim:events"));
    if (st->animCurvesEvents == NULL) {
        return;
    }

    queue_load_file_region_to_ptr((void *) st->animCurvesEvents, ANIMCURVES_BIN, animcurves_bin_offset, size);
    st->animCurvesEventCount = ((s32 *) sTempBuffer)[0] & 0xFFFF;
    st->animCurvesKeyframeCount = ((size >> 2) - st->animCurvesEventCount) >> 1;
    st->animCurvesKeyframes = (AnimCurvesKeyframe *) (&st->animCurvesEvents[st->animCurvesEventCount]);
    st->seqSlot = setup->seqSlot;

    if (st->seqSlot >= 0) {
        _bss_108[st->seqSlot] = 0;
        sEventFlags[st->seqSlot] = 0;
        _bss_198[st->seqSlot] = 0;
    }

    if (setup->activate != 0) {
        st->state = ANIMOBJ_STATE_Ready;
    } else {
        st->state = ANIMOBJ_STATE_Completed;
    }

    anim_init_curve_keyframes(st);
}

// offset: 0x7B64 | func: 41 | export: 7
void anim_init_curve_keyframes(AnimObj_Data* st) {
    s32 channelKeyIndex;
    s32 index;
    s32 channelIndex;
    AnimCurvesKeyframe* kf;

    //Initialising keyframes per channel to 0
    for (index = 0; index < ANIMCURVES_KEYFRAME_CHANNELS; index++) { st->channelTotalKeys[index] = 0; }

    //Counting keyframes per channel
    for (channelIndex = 0, index = 0; index < st->animCurvesKeyframeCount; channelIndex++) {      
        channelKeyIndex = 0;
        if (index < st->animCurvesKeyframeCount) {
            kf = &st->animCurvesKeyframes[index];

            while (channelIndex == (kf[channelKeyIndex].channel & 0x1F)) {
                channelKeyIndex++;
                if (index + channelKeyIndex >= st->animCurvesKeyframeCount) {
                    break;
                }
            }            
        }

        st->channelTotalKeys[channelIndex] = channelKeyIndex;
        st->channelFirstKeyIndex[channelIndex] = index;
        index += channelKeyIndex;
    }

    //Find the length of the animation timeline (from event type 0xFF, which must be somewhere in the first two slots)
    st->duration = 1000;
    for (index = 0; index < 2 && index < st->animCurvesEventCount; index++) {
        if ((&st->animCurvesEvents[index])->type == ANIM_EVT_SETDURATION) {
            st->duration = st->animCurvesEvents[index].params + 1;
        }
    }
}

// offset: 0x7C6C | func: 42 | export: 8
void anim_free_curve(AnimObj_Data* st) {
    if (st->animCurvesEvents) {
        mmFree(st->animCurvesEvents);
        st->animCurvesEvents = 0;
        st->animCurvesKeyframes = 0;
    }

    if (st->unk2C) {
        mmFree(st->unk2C);
        st->unk2C = 0;
    }
}

// offset: 0x7CF0 | func: 43 | export: 9
void anim_tick(void) {
    s32 i;
    s32 k;
    s32 numAnimObjs;
    s32 allActorsPresent;
    AnimObj_Setup* setup;
    AnimObj_Data* st;
    Object* animObjs[20];
    s32 startTime;
    s32 slot;
    Object* obj;
    Activate *resume;
    s32 numUnready;
    Activate unreadyResumes[MAX_ACTORS];
    Object** objList;
    s32 numObjs;
    s32 start;

    objList = get_world_objects(&start, &numObjs);
    if (_data_0 != _data_4) {
        _data_4 = _data_0;
    }
    for (i = 0; i < MAX_SEQSLOTS; i++) {
        _bss_1C8[i] = 0;
        if ((_bss_D8[i] != 0) && (_bss_A8[i] == 0)) {
            _bss_1C8[i] = gUpdateRate;
        }
        _bss_A8[i] = _bss_D8[i];
        _bss_D8[i] = 0;
        _bss_1F8[i] = _bss_258[i];
        _bss_258[i] = -1;
        if (_bss_490[i] == 2) {
            _bss_490[i] = 1;
        } else {
            _bss_490[i] = 0;
        }
    }
    // Try to activate anim objects
    k = sActivatesCount;
    numUnready = 0;
    while (k > 0) {
        k--;
        resume = &sActivates[k];
        slot = resume->seqSlot;
        startTime = resume->startTime;
        sEventFlags[slot] = 0;
        _bss_108[slot] = 0;
        _bss_198[slot] = 0;
        numAnimObjs = 0;
        allActorsPresent = TRUE;
        for (i = 0; i < numObjs; i++) {
            obj = objList[i];
            if (obj->controlNo == OBJCONTROL_AnimObj) {
                setup = (AnimObj_Setup*)obj->setup;
                st = obj->data;
                if ((setup != NULL) && (slot == setup->seqSlot)) {
                    if ((setup->target >= 4) && (anim_find_animobj_target_in_world(obj) == NULL)) {
                        allActorsPresent = FALSE;
                        STUBBED_PRINTF(" Could Not FInd Obj %i  over %i \n", obj->id, setup->target);
                    } else {
                        st->actor = NULL;
                    }
                    /* default.dol
                    if (numAnimObjs >= 20) {
                        STUBBED_PRINTF("ANIM: noanims overflow\n");
                    }
                    */
                    animObjs[numAnimObjs] = obj;
                    numAnimObjs += 1;
                }
            }
        }
        for (i = 0; i < numAnimObjs; i++) {
            obj = animObjs[i];
            setup = (AnimObj_Setup*)obj->setup;
            if ((setup != NULL) && (slot == setup->seqSlot)) {
                st = obj->data;
                if (allActorsPresent) {
                    // seq is ready, all actors/targets are present in the world
                    st->state = ANIMOBJ_STATE_Ready;
                    st->startTime = startTime;
                    anim_tick_obj(obj, 1);
                    get_object_child_position(obj, 
                        &obj->globalPosition.x, &obj->globalPosition.y, &obj->globalPosition.z);
                } else {
                    st->state = ANIMOBJ_STATE_WaitingForActors;
                }
            }
        }
        if (!allActorsPresent) {
            unreadyResumes[numUnready].seqSlot = slot;
            unreadyResumes[numUnready].startTime = startTime;
            numUnready += 1;
        }
    }
    for (i = 0; i < numUnready; i++) {
        sActivates[i].seqSlot = unreadyResumes[i].seqSlot;
        sActivates[i].startTime = unreadyResumes[i].startTime;
    }
    sActivatesCount = (s8) numUnready;
}

// offset: 0x81F8 | func: 44
static Object* anim_find_animobj_target_in_world(Object* animObj) {
    AnimObj_Data *st;
    s32 numObjs;
    s32 start;
    Object** objList;
    AnimObj_Setup *objsetup;
    s32 targetObjID;
    s32 i;
    Object *obj;
    f32 xDist;
    f32 yDist;
    f32 zDist;
    f32 dist;
    Object* closestObj;
    f32 closestDist;

    st = animObj->data;
    
    if (st->actorUID != 0) {
        return func_800211B4(st->actorUID);
    }
    
    objList = get_world_objects(&start, &numObjs);
    
    objsetup = (AnimObj_Setup*)animObj->setup;
    targetObjID = objsetup->target - 4;
    
    if ((targetObjID == OBJ_Krystal) || (targetObjID == OBJ_Sabre)) {
        return get_player();
    }
    if ((targetObjID == OBJ_Tricky) || (targetObjID == OBJ_Kyte)) {
        return get_sidekick();
    }
    
    closestObj = NULL;
    closestDist = -1.0f;
    for (i = 0; i < numObjs; i++) {
        obj = objList[i];

        if (targetObjID == obj->id) {
            xDist = animObj->srt.transl.x - obj->srt.transl.x;
            yDist = animObj->srt.transl.y - obj->srt.transl.y;
            zDist = animObj->srt.transl.z - obj->srt.transl.z;
            dist = SQ(xDist) + SQ(yDist) + SQ(zDist);

            if ((closestDist < 0.0f) || (dist < closestDist)) {
                closestObj = obj;
                closestDist = dist;
            }
        }
    }
    
    return closestObj;
}

// offset: 0x8598 | func: 45 | export: 10
s16 anim_find_override_target(Object* animObj) {
    AnimObj_Data* st;
    AnimObj_Setup* setup;
    Object* overrideObj;
    Object* obj;
    s32 targetObjID;
    s32 i;
    s32 _pad;
    f32 closestDist;
    Object** objList;
    f32 distZ;
    f32 distX;
    f32 distY;
    s32 numObjs;
    s32 start;

    objList = get_world_objects(&start, &numObjs);
    st = animObj->data;
    setup = (AnimObj_Setup*)animObj->setup;
    if (animObj->controlNo == OBJCONTROL_Unk17) {
        st->actor = NULL;
        return -1;
    }
    switch (setup->target) {
    case 0:
        st->actor = NULL;
        break;
    case 1:
        st->actor = get_player();
        break;
    case 2:
        st->actor = get_sidekick();
        break;
    case 3:
        st->actor = NULL;
        st->unk87 = (s8) (setup->target - 2);
        if (!(_bss_3A8[st->seqSlot] & ACTORUSETTING_NO_LETTERBOX)) {
            gDLL_2_Camera->vtbl->set_letterbox_goal(30, TRUE);
        }
        break;
    default:
        st->actor = NULL;
        targetObjID = setup->target - 4;
        if ((targetObjID == OBJ_Krystal) || (targetObjID == OBJ_Sabre)) {
            st->actor = get_player();
        } else if (st->actorUID != 0) {
            st->actor = func_800211B4(st->actorUID);
        } else {
            closestDist = -1.0f;
            for (i = 0; i < numObjs; i++) {
                obj = objList[i];
                overrideObj = anim_override_list_get(st->seqSlot, obj);
                if (overrideObj == animObj) {
                    st->actor = obj;
                    break;
                }

                if ((overrideObj == NULL) && (targetObjID == obj->id)) {
                    distX = animObj->srt.transl.x - obj->srt.transl.x;
                    distY = animObj->srt.transl.y - obj->srt.transl.y;
                    distZ = animObj->srt.transl.z - obj->srt.transl.z;
                    
                    if ((closestDist < 0.0f) || ((SQ(distX) + SQ(distY) + SQ(distZ)) < closestDist)) {
                        st->actor = obj;
                        closestDist = SQ(distX) + SQ(distY) + SQ(distZ);
                    }
                }
            }
        }
        break;
    }
    if (st->actor != NULL) {
        if (st->seqSlot < 25) {
            if (st->actor->seqSlot != SEQSLOT_NONE) {
                STUBBED_PRINTF("****END\n");
                anim_end_obj_sequence(st->actor->seqSlot);
            }
        }
        return st->actor->tabIdx;
    }
    return -1;
}

// offset: 0x8878 | func: 46 | export: 12
s32 anim_func_8878(void) {
    return 1;
}

// offset: 0x8884 | func: 47 | export: 13
s32 anim_func_8884(void) {
    return _data_2C;
}

// offset: 0x88A0 | func: 48 | export: 14
void anim_func_88A0(s32 arg0) {
    _data_2C = arg0;
}

// offset: 0x88BC | func: 49 | export: 11
s32 anim_func_88BC(s32 arg0) {
    return 0;
}

// offset: 0x88CC | func: 50 | export: 15
s32 anim_func_88CC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return 0;
}

// offset: 0x88E8 | func: 51 | export: 16
void anim_func_88E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) { }

// offset: 0x8900 | func: 52 | export: 17
// official name: startObjSequence
s32 anim_start_obj_sequence(s32 seqno, Object* object, s32 enabledActors) {
    AnimObj_Setup* actorSetup;
    Object* actorObj;
    f32 temp_fv1;
    s32 numActors;
    s32 actorObjID;
    s16* tabEntry;
    s32 slot;
    s32 i;
    s32 temp_v1_4;
    Actor* actors;
    s32 preemptTime;
    Object* actorParent;
    s32 j;
    AnimObj_Data* actorObjData;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    s16 yaw;
    s32 sp4C;
    s32 sp48;

    sp48 = 0;
    if (seqno == -1) {
        return -1;
    }
    for (i = 25; i < MAX_SEQSLOTS; i++) {
        if (sSlotInUse[i] == 0) {
            slot = i;
            sSlotInUse[i] = 1;
            anim_override_list_clear(i);
            i = MAX_SEQSLOTS + 1; // break
        }
    }
    if (i == MAX_SEQSLOTS) {
        // STUBBED_PRINTF("game/anim.c: startObjSequence() couldn't find seqno free (ABORTED)!!\n"); // default.dol
        return -1;
    }
    if ((seqno < 0) || (seqno >= object->def->numSequences)) {
        // Note: default.dol also moves this check to be right before the above loop
        // STUBBED_PRINTF("game/anim.c: startObjSequence() seqno out of range [%d][%d]\n", object->id, objectSeqIndex); // default.dol
        return -1;
    }
    if (object->def->pSeq != NULL) {
        seqno = object->def->pSeq[seqno];
    }
    if ((object->seqSlot != SEQSLOT_NONE) && (_data_24 == NULL)) {
        anim_end_obj_sequence(object->seqSlot);
    }
    actors = mmAlloc(sizeof(Actor) * MAX_ACTORS, ALLOC_TAG_ANIMSEQ_COL, ALLOC_NAME("anim:table"));
    tabEntry = (s16*)actors;
    queue_load_file_region_to_ptr((void*)actors, OBJSEQ_TAB, seqno * sizeof(s16), 8);
    numActors = tabEntry[1] - tabEntry[0];
    queue_load_file_region_to_ptr((void*)actors, OBJSEQ_BIN, ((s16*)tabEntry)[0] * sizeof(Actor), numActors * sizeof(Actor));
    if (_data_24 != NULL) {
        object = _data_24;
    }
    object->seqSlot = slot;
    actorParent = object->parent;
    sp5C = object->srt.transl.x;\
    sp58 = object->srt.transl.y;\
    sp54 = object->srt.transl.z;\
    if (_bss_1D88.unk0_8) {
        actorParent = NULL;
        sp54 = object->globalPosition.z;
        sp58 = object->globalPosition.y;
        sp5C = object->globalPosition.x;
    }
    yaw = object->srt.yaw;
    if (_data_1C != 0) {
        sp5C -= (fsin16_precise(object->srt.yaw) * object->visRadius);
        sp54 -= (fcos16_precise(object->srt.yaw) * object->visRadius);
    }
    _bss_3A8[object->seqSlot] = 0;
    _bss_4C0[object->seqSlot] = 0;
    sSlotObjID[object->seqSlot] = object->id;
    for (i = 0; i < numActors; i++) {
        if ((1 << i) & enabledActors) {
            actorSetup = obj_alloc_setup(sizeof(AnimObj_Setup), OBJ_Override);
            actorObjID = actors[i].objID;
            if (actorObjID == 0xFFFF) {
                actorSetup->base.objId = OBJ_Override;
                actorSetup->target = object->id + 4;
                if ((object->id == OBJ_VariableObject) && (sVariableObjID != -1)) {
                    actorSetup->target = sVariableObjID + 4;
                }
                actors[i].settings |= ACTORSETTING_UNK8000;
            } else if (actorObjID == 0xFFFE) {
                actorSetup->base.objId = OBJ_AnimCamera;
                actorSetup->target = 3;
            } else if (actors[i].settings & ACTORSETTING_UNK4000) {
                actorSetup->base.objId = OBJ_Override;
                actorSetup->target = actorObjID + 4;
            } else {
                actorSetup->base.objId = actorObjID;
                actorSetup->target = 0;
            }
            if (actors[i].settings & ACTORSETTING_UNK8000) {
                actorSetup->unk20 = 0;
                actorSetup->unk21 = 0;
            } else {
                actorSetup->unk20 = 1;
                actorSetup->unk21 = 1;
            }
            actorSetup->sequenceIdBitfield = ((seqno & 0x7FF) * 0x10) | 0x8000 | (i & 0xF);
            actorSetup->unk1A = -1;
            if (i != 0) {
                if ((_bss_5AC != 0) && (actorSetup->base.objId == OBJ_AnimCamera)) {
                    actorSetup->base.x = sp5C + _bss_5B8.x;
                    actorSetup->base.y = sp58 + _bss_5B8.y;
                    actorSetup->base.z = sp54 + _bss_5B8.z;
                    _bss_5AC = 0;
                } else {
                    actorSetup->base.x = sp5C;
                    actorSetup->base.y = sp58;
                    actorSetup->base.z = sp54;
                }
            } else {
                actorSetup->base.x = object->srt.transl.x;
                actorSetup->base.y = object->srt.transl.y;
                actorSetup->base.z = object->srt.transl.z;
            }
            actorSetup->seqSlot = slot;
            actorSetup->activate = 1;
            actorSetup->unk24 = actors[i].settings & 0x7F;
            actorSetup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
            actorSetup->base.fadeFlags = OBJSETUP_FADE_MANUAL;
            if (actorSetup->base.objId == OBJ_AnimCamera) {
                actorSetup->base.loadFlags = OBJSETUP_LOAD_LEVEL;
            }
            if ((actorSetup->base.objId == OBJ_VariableObject) && (sVariableObjID != -1)) {
                actorSetup->base.objId = sVariableObjID;
            }
            actorObj = obj_create(&actorSetup->base, OBJINIT_FLAG4 | OBJINIT_STANDALONE, -1, -1, actorParent);
            actorObj->seqSlot = SEQSLOT_ANIMOBJ;
            actorObjData = actorObj->data;
            actorObjData->seqYaw = yaw;
            actorObjData->unk7A = -1;
            actorObjData->unk7A &= ~ANIM7AFLAG_UNK400;
            for (j = 0; j < 4; j++) { // @bug? max decisions is 10 not 4
                actorObjData->decisionConditions[j] = 0;
            }
            if (actors[i].settings & 0x80) {
                actorObjData->unk62 = 4;
                if (actors[i].settings & 0x7F) {
                    sp4C = actors[i].settings & 0x7F;
                } else {
                    sp4C = 0;
                }
                sp48 = 1;
            } else {
                actorObjData->unk62 = -1;
            }
            actorObjData->actorUID = actors[i].uid;
            temp_v1_4 = (actors[i].settings >> 8) & 0x3F;
            if (temp_v1_4 & ACTORUSETTING_DONT_OVERRIDE_POS) {
                actorObjData->unk7A &= ~ANIM7AFLAG_OVERRIDE_POS;
            }
            if (temp_v1_4 & ACTORUSETTING_DONT_OVERRIDE_ROT) {
                actorObjData->unk7A &= ~ANIM7AFLAG_OVERRIDE_ROT;
            }
            if (temp_v1_4 & ACTORUSETTING_ZERO_YAW) {
                actorObjData->seqYaw = 0;
            }
            if (temp_v1_4 & ACTORUSETTING_SKIPPABLE) {
                actorObjData->unk7A &= ~ANIM7AFLAG_UNK100;
            }
            if (temp_v1_4 & ACTORUSETTING_UNK20) {
                actorObjData->unk8C |= 2;
            }
            actorObjData->unk7C = actorObjData->unk7A;
            if (i == 0) {
                _bss_3A8[object->seqSlot] = temp_v1_4;
                sSlotObjID[object->seqSlot] = actorObj->setup->uID;
                if ((object->def->flags & OBJDEF_IS_MOBILE_MAP) && !(object->def->flags & OBJDEF_MOBILE_MAP_NEVER_PLAYER_PARENT)) {
                    actorParent = object;
                    sp5C = 0.0f;
                    sp58 = 0.0f;
                    sp54 = 0.0f;
                    yaw = 0;
                }
            }
        }
    }
    _bss_318[object->seqSlot] = yaw;
    _bss_378[object->seqSlot] = 0;
    _bss_490[object->seqSlot] = 0;
    preemptTime = anim_get_preempt_time(object);
    if (preemptTime != 0) {
        _bss_3A8[object->seqSlot] |= ACTORUSETTING_NO_LETTERBOX;
    }
    anim_queue_activate(slot, preemptTime, numActors);
    if (sp48 != 0) {
        anim_func_9CE8(sp4C);
    }
    mmFree(actors);
    _data_1C = 0;
    _bss_1D88.unk0_8 = 0;
    return slot;
}

// offset: 0x906C | func: 53 | export: 18
// official name: endObjSequence
void anim_end_obj_sequence(s32 slot) {
    s32 i;
    Object* obj;
    AnimObj_Data *animObjData;
    s32 _pad;
    Object *freeList[12];
    s32 numObjs;
    s32 sp40;
    s32 freeListLen;
    Object** objList;
    
    objList = get_world_objects(&sp40, &numObjs);
    freeListLen = 0;

    for (i = 0; i < numObjs; i++) {
        obj = objList[i];
        if (slot == obj->seqSlot) {
            obj->seqSlot = SEQSLOT_NONE;
        }
        if (obj->controlNo == OBJCONTROL_AnimObj) {
            animObjData = (AnimObj_Data*)obj->data;
            if (slot == animObjData->seqSlot) {
                if (obj == _bss_6FC) {
                    _bss_6FC = NULL;
                }
                freeList[freeListLen] = obj;
                freeListLen += 1;
                if (freeListLen == 12) {
                    STUBBED_PRINTF("endObjSequence: too many obj frees\n");
                    freeListLen = 11;
                }
            }
        }
    }
    for (i = 0; i < freeListLen; i++) {
        obj_destroy_object(freeList[i]);
    }
    anim_func_9DD4();
    _data_24 = NULL;
    sSlotInUse[slot] = 0;
}

// offset: 0x9358 | func: 54 | export: 20
// official name: preemptSequenceTime
void anim_preempt_sequence_time(Object *obj, s32 preemptTime) {
    PreemptTime *t;
    s8 count;

    count = sPreemptTimeListCount;    
    if (count < 4) {
        t = &sPreemptTimeList[count];
        t->obj = obj;
        t->preemptTime = preemptTime;
        sPreemptTimeListCount = count + 1;
    } else {
        STUBBED_PRINTF("preemptSequenceTime() Overflow!!\n");
    }
}

// offset: 0x93A0 | func: 55
static s32 anim_get_preempt_time(Object* obj) {
    s32 preemptTime;
    s32 i;

    for (i = 0; i < sPreemptTimeListCount; i++) {
        if (obj == (&sPreemptTimeList[i])->obj) {
            sPreemptTimeListCount -= 1;
            preemptTime = (&sPreemptTimeList[i])->preemptTime;

            //Remove item from array and shift subsequent items down in array
            while (i < sPreemptTimeListCount) {
                (&sPreemptTimeList[i])->obj = (&sPreemptTimeList[i + 1])->obj;
                (&sPreemptTimeList[i])->preemptTime = (&sPreemptTimeList[i + 1])->preemptTime;
                i++;
            }

            return preemptTime;
        }
    }
    
    return 0;    
}

// offset: 0x9440 | func: 56 | export: 21
void anim_func_9440(AnimObj_Data* st, s32 arg1) {
    st->unk80 = arg1;
    st->unk9D |= 0x40;
}

// offset: 0x9458 | func: 57 | export: 22
s8 anim_func_9458(void) {
    return _data_28;
}

// offset: 0x9474 | func: 58 | export: 23
void anim_func_9474(s8 arg0) {
    _data_28 = arg0;
}

// offset: 0x949C | func: 59 | export: 24
s16 anim_get_anim_counter1(void) {
    return sAnimCounter1;
}

// offset: 0x94B8 | func: 60 | export: 25
void anim_set_anim_counter1(s16 value) {
    sAnimCounter1 = value;
}

// offset: 0x94E0 | func: 61 | export: 26
s16 anim_get_anim_counter2(void) {
    return sAnimCounter2;
}

// offset: 0x94FC | func: 62 | export: 27
void anim_set_anim_counter2(s16 value) {
    sAnimCounter2 = value;
}

// offset: 0x9524 | func: 63 | export: 29
s32 anim_func_9524(Object* actor, AnimObj_Data* st, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6) {
    s16 sp56;
    s16 temp_v0;
    s16* sp50;
    f32 sp4C;
    f32 var_fv0;
    f32 var_fv1;
    s32 var_a0;
    f32 sp34[3];
    Object* sp30;

    sp30 = get_player();
    arg3 *= 182.04f;
    arg4 *= 182.04f;
    arg2 *= 182.04f;
    if (st->unk62 == 4) {
        st->unk7A &= ~ANIM7AFLAG_OVERRIDE_ROT;
        if (func_80034804(actor, 0) != NULL) {
            st->unk7A &= ~ANIM7AFLAG_OVERRIDE_HEAD;
        }
        st->unkF4 = anim_func_9B70;
        st->unk4C.f[0] = 0.0f;
        st->unk4C.f[1] = 0.0f;
        st->unk4C.f[2] = 0.0f;
        temp_v0 = func_80031DD8(actor, sp30, NULL);
        if (temp_v0 >= 0) {
            var_a0 = temp_v0;
        } else {
            var_a0 = -temp_v0;
        }
        if (var_a0 < arg3) {
            sp56 = 0;
        } else if (temp_v0 > 0) {
            sp56 = temp_v0 - arg3;
        } else {
            sp56 = temp_v0 + arg3;
        }
        st->yawDiff = sp56;
        sp34[0] = sp30->srt.transl.f[0] - actor->unk74->drawPoint.f[0];
        sp34[1] = sp30->srt.transl.f[1] - actor->unk74->drawPoint.f[1];
        sp34[2] = sp30->srt.transl.f[2] - actor->unk74->drawPoint.f[2];
        sp34[1] += 30.0f;
        st->pitchDiff = arctan2_f(sp34[1], sqrtf(SQ(sp34[2]) + SQ(sp34[0])));
        st->rollDiff = 0;
        st->unk62 = 5;
        st->unk58 = 0.0f;
        if (sp56 != 0) {
            var_fv0 = (f32) arg2 / (f32) sp56;
            if (var_fv0 >= 0) {
                st->unk24 = var_fv0;
            } else {
                st->unk24 = -var_fv0;
            }
        } else {
            st->unk24 = 1.0f;
        }
        if (st->unk24 < 0.0f) {
            st->unk24 = 0.0f;
        } else {
            if (st->unk24 > 0.25f) {
                var_fv1 = 0.25f;
            } else {
                var_fv1 = st->unk24;
            }
            st->unk24 = var_fv1;
        }
        if ((arg5 != -1) && (arg6 != -1)) {
            st->unk7A &= ~ANIM7AFLAG_OVERRIDE_MODEL;
            if (st->yawDiff < 0) {
                if (arg6 != -1) {
                    func_80023D30(actor, arg6, 0.0f, 0);
                }
            } else if (arg5 != -1) {
                func_80023D30(actor, arg5, 0.0f, 0);
            }
        }
        st->unkF4 = anim_func_9B70;
        return 1;
    }
    if (st->unk62 == 5) {
        st->unk58 += st->unk24;
        if (st->unk58 > 1.0f) {
            st->unk58 = 1.0001f;
        }
        actor->srt.yaw += (s16) (st->unk24 * st->yawDiff);
        sp50 = func_80034804(actor, 0);
        if (sp50 != NULL) {
            st->unk7A &= ~ANIM7AFLAG_OVERRIDE_HEAD;
            var_fv0 = (func_80031DD8(actor, sp30, NULL) * st->unk58) + (sp50[1] * (1.0f - st->unk58)) ;
            if (var_fv0 < -arg4) {
                var_fv0 = -arg4;
            } else {
                var_fv1 = MIN(arg4, var_fv0);
                var_fv0 = var_fv1;
            }
            sp50[1] = var_fv0;
            var_fv0 = st->pitchDiff * st->unk58;
            sp50[0] = var_fv0;

            if (var_fv0 < (f32) -(arg4 >> 1)) {
                var_fv0 = (f32) -(arg4 >> 1);
            } else if (var_fv0 > (f32) (arg4 >> 1)) {
                var_fv0 = (f32) (arg4 >> 1);
                if (var_fv0){} // @fake
            }
        }
        if ((arg5 != -1) && (arg6 != -1)) {
            if (st->yawDiff >= 0) {
                var_fv0 = st->yawDiff;
            } else {
                var_fv0 = -st->yawDiff;
            }
            var_fv0 = (var_fv0 * 3.142f) / 325767.0f;
            func_8002493C(actor, var_fv0, &sp4C);
            func_80024108(actor, sp4C, gUpdateRate, NULL);
        }
        if (st->unk58 > 1.0f) {
            st->unk62 = 0;
            st->unk7A |= ANIM7AFLAG_OVERRIDE_HEAD;
            sp50 = func_80034804(actor, 0);
            st->unk120 = sp50[1];
            st->unk122 = sp50[0];
            if (st->unk58 > 1.0f) {
                st->unk7A |= ANIM7AFLAG_OVERRIDE_MODEL;
            }
        }
        return 1;
    }
    return 0;
}

// offset: 0x9B70 | func: 64
static void anim_func_9B70(Object* arg1, Object* animObj, AnimObj_Data* st) {
    s16* temp_v0;

    //NOTE: sequence bone should probably be a struct instead of s16*?
    temp_v0 = func_80034804(arg1, 0);
    if (temp_v0 != NULL) {
        temp_v0[1] = 0;
        temp_v0[0] = 0;
    }
}

// offset: 0x9BC0 | func: 65
static void anim_override_list_clear(s32 slot) {
    s32 index;

    for (index = 0; index < MAX_ACTORS; index++) { 
        sOverrides[slot][index].object = 0; 
    }
}

// offset: 0x9C08 | func: 66
static Object* anim_override_list_get(s32 seqSlot, Object* searchObject) {
    s32 i;
    ANIMActorOverride* actors;

    actors = sOverrides[seqSlot];

    for (i = 0; i < MAX_ACTORS; i++) {
        if (searchObject == actors[i].object) {
            return actors[i].overrideObject;
        }
    }
    return NULL;
}

// offset: 0x9C94 | func: 67
static void anim_override_list_add(s32 seqSlot, Object* actor, Object* animObj) {
    s32 offset;
    u8 *actors;
    ANIMActorOverride *over;
    Object *actorObject;

    actors = (u8*)&sOverrides[seqSlot][0];
    actorObject = NULL;

    for (offset = 0; offset < 0x80; offset += 8) {
        over = (ANIMActorOverride *) (actors + offset);
        actorObject = over->object;
        if (!actorObject || actor == actorObject) {
            break;
        }
    }

    over = (ANIMActorOverride *) (actors + offset);
    over->object = actor;
    over->overrideObject = animObj;
}

// offset: 0x9CE8 | func: 68
static void anim_func_9CE8(s32 arg0) {
    Object* hlObject;
    CameraFunc15Unk_unk74 action;

    if (gDLL_2_Camera->vtbl->get_dll_ID() == DLL_ID_CAM95) {
        return;
    }
    
    hlObject = gDLL_2_Camera->vtbl->get_highlighted_object();
    if ((hlObject != NULL) && (hlObject->unk74 != NULL)) {
        action.coord.x = hlObject->unk74->drawPoint.x;
        action.coord.y = hlObject->unk74->drawPoint.y;
        action.coord.z = hlObject->unk74->drawPoint.z;
        action.unkC = arg0;
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAM95, 1, 0, sizeof(CameraFunc15Unk_unk74), &action, 60, 0xFF);
    }
}

// offset: 0x9DD4 | func: 69
void anim_func_9DD4(void) {
    if (gDLL_2_Camera->vtbl->get_dll_ID() == DLL_ID_CAM95) {
        gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, 0, 3, 0, NULL, 0, 0);
    }
}

// offset: 0x9E58 | func: 70 | export: 30
s32 anim_set_variable_obj(s32 objID, Object *arg1, s32 arg2) {
    sVariableObjID = objID;
    _data_24 = arg1;
    _data_1C = arg2;
    return 1;
}

// offset: 0x9E88 | func: 71 | export: 31
s32 anim_func_9E88(f32 arg0, f32 arg1, f32 arg2) {
    _bss_5AC = 1;
    _bss_5B8.x = arg0;
    _bss_5B8.y = arg1;
    _bss_5B8.z = arg2;
    return 1;
}

// offset: 0x9EC8 | func: 72
static void anim_func_9EC8(Object* actor, s16* arg1, s32 arg2) {
    s16 *temp_v0;
    s32 *temp_v1;
    s32 i;
    s32 *var_s0;
    
    var_s0 = func_800349B0();
    temp_v1 = var_s0;
    if (arg2 == 0){
        arg2 = 9;
    }
    
    if (arg1 == NULL){
        return;
    }
    
    for (i = 1; i < arg2; i++){
        temp_v0 = func_80034804(actor, var_s0[i]);
        if (temp_v0 != NULL){
            temp_v0[1] = arg1[1];
            temp_v0[0] = arg1[0];
            temp_v0[2] = arg1[2];
        }
    }
    
    var_s0 = temp_v1 + 1;
}

// offset: 0x9F90 | func: 73 | export: 32
s32 anim_func_9F90(s32 arg0, s32 arg1) {
    switch (arg1) {
        case 0:
            _bss_1D88.unk0_8 = 1;
            break;
        case 1:
            _bss_1D88.unk0_8 = 0;
            break;
    }

    return 0;
}
