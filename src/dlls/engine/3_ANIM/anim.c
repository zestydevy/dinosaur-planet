#include "PR/ultratypes.h"
#include "dlls/objects/210_player.h"
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

// official filename: game/anim.c (default.dol)

//Terminology mightn't be correct
#define ANIMCURVES_SCENES_MAX 45
#define ANIMCURVES_ACTORS_MAX 16
#define ANIMCURVES_IS_OBJSEQ2CURVE_INDEX 0x8000

// Names inferred from default.dol
enum AnimEventType {
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
    ANIM_EVT_ENVFX = 13,
    ANIM_EVT_STORYBOARD = 14,
    ANIM_EVT_SFX_WITH_DURATION = 15
};

enum AnimEnvFxEventType {
    ANIM_EVT_ENVFX_SET_MUSIC = 0,
    ANIM_EVT_ENVFX_APPLY = 2,
    ANIM_EVT_ENVFX_WARP = 6,
    ANIM_EVT_ENVFX_SFX = 7,
    ANIM_EVT_ENVFX_8 = 8,
    ANIM_EVT_ENVFX_14 = 14,
    ANIM_EVT_ENVFX_15 = 15
};

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

typedef enum {
/*00*/    ANIMCURVES_CHANNEL_headRotateZ = 0,
/*01*/    ANIMCURVES_CHANNEL_headRotateX = 1,
/*02*/    ANIMCURVES_CHANNEL_headRotateY = 2,
/*03*/    ANIMCURVES_CHANNEL_opacity = 3,
/*04*/    ANIMCURVES_CHANNEL_dayTime = 4,
/*05*/    ANIMCURVES_CHANNEL_scale = 5,
/*06*/    ANIMCURVES_CHANNEL_rotateZ = 6,
/*07*/    ANIMCURVES_CHANNEL_rotateY = 7,
/*08*/    ANIMCURVES_CHANNEL_rotateX = 8,
/*09*/    ANIMCURVES_CHANNEL_animSpeed = 9,
/*0A*/    ANIMCURVES_CHANNEL_animBlendSpeed = 10,
/*0B*/    ANIMCURVES_CHANNEL_translateZ = 11,
/*0C*/    ANIMCURVES_CHANNEL_translateY = 12,
/*0D*/    ANIMCURVES_CHANNEL_translateX = 13,
/*0E*/    ANIMCURVES_CHANNEL_fieldOfView = 14,
/*0F*/    ANIMCURVES_CHANNEL_eyeX = 15,
/*10*/    ANIMCURVES_CHANNEL_eyeY = 16,
/*11*/    ANIMCURVES_CHANNEL_jaw = 17,
/*12*/    ANIMCURVES_CHANNEL_soundVolume = 18
} AnimCurvesKeyframeChannels;

typedef enum {
/*FF*/    ANIMCURVES_EVENTS_setDuration = -1,
/*00*/    ANIMCURVES_EVENTS_timing = 0,
/*01*/    ANIMCURVES_EVENTS_unk1 = 1,
/*02*/    ANIMCURVES_EVENTS_playAnimation = 2,
/*03*/    ANIMCURVES_EVENTS_setObj = 3,
/*04*/    ANIMCURVES_EVENTS_blendShape = 4,
/*05*/    ANIMCURVES_EVENTS_unk5 = 5,
/*06*/    ANIMCURVES_EVENTS_sound = 6,
/*07*/    ANIMCURVES_EVENTS_unk7 = 7,
/*08*/    ANIMCURVES_EVENTS_unk8 = 8,
/*09*/    ANIMCURVES_EVENTS_unk9 = 9,
/*0A*/    ANIMCURVES_EVENTS_unkA = 10,
/*0B*/    ANIMCURVES_EVENTS_subEvent = 11,
/*0C*/    ANIMCURVES_EVENTS_unkC = 12,
/*0D*/    ANIMCURVES_EVENTS_setObjParam = 13,
/*0E*/    ANIMCURVES_EVENTS_setLoop = 14,
/*0F*/    ANIMCURVES_EVENTS_soundOther = 15,
/*7F*/    ANIMCURVES_EVENTS_stopSoundOther = 127
} AnimCurvesEvents;

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
    s32 unk0;
    s32 unk4;
} AnimBSS0;

typedef struct {
    Object *unk0;
    s32 unk4;
} ANIMBSSUnk0;

typedef struct{
    Object* object;
    Object* overrideObject;
} ANIMActorOverride;

typedef struct {
    s16 unk0;
    s16 unk2;
    u16 unk4;
} ANIMUnk698;

typedef struct {
    Object* unk0;
    s16 unk4;
    s8 unk6;
} Bss38Thing;

typedef struct {
    s32* unk0;
    s16 unk4;
    s16 unk6;
} Bss5F0Thing;

/*0x0*/ static s16 _data_0 = 0;
/*0x4*/ static s16 _data_4 = 0;
/*0x8*/ static s16 _data_8 = 0xff00;
/*0xC*/ static f32 _data_C = 60.0;
/*0x10*/ static f32 _data_10 = 0.0f;
/*0x14*/ static s16 sAnimCounter1 = 0;
/*0x18*/ static s16 sAnimCounter2 = 0;
/*0x1C*/ static u8 _data_1C = 0;
/*0x20*/ static s32 _data_20 = -1; // object ID of the thing the player should hold when playing the first time pickup sequence
/*0x24*/ static Object* _data_24 = 0;
/*0x28*/ static s8 _data_28 = 0;
/*0x2C*/ static s32 _data_2C = 0;
/*0x30*/ static s8 _data_30 = 0;
/*0x34*/ static s32 _data_34[] = {
    0x00008000, 0x00004000, 0x00000002, 0x00000001, 0x00000004, 0x00000008, 0xffffffff
};
/*0x50*/ static u32 _data_50[] = {
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

/*0x0*/ static ANIMBSSUnk0 _bss_0[4];
/*0x20*/ static s8 _bss_20; //count of items in bss0?
/*0x24*/ static f32 _bss_24;
/*0x28*/ static f32 _bss_28;
/*0x2C*/ static f32 _bss_2C;
/*0x30*/ static s16 _bss_30;
/*0x32*/ static s8 _bss_32;
/*0x33*/ static s8 _bss_33;
/*0x38*/ static Bss38Thing _bss_38[10];
/*0x88*/ static s8 _bss_88;
/*0x89*/ static s8 _bss_89;
/*0x8A*/ static s8 _bss_8A;
/*0x8B*/ static s8 _bss_8B;
/*0x8C*/ static s32 _bss_8C;
/*0x90*/ static s32 _bss_90;
/*0x94*/ static s32 _bss_94;
/*0x98*/ static s32 _bss_98;
/*0x9C*/ static s16 _bss_9C;
/*0xA0*/ static f32 _bss_A0;
/*0xA4*/ static s8 _bss_A4;
/*0xA8*/ static s8 _bss_A8[ANIMCURVES_SCENES_MAX];
/*0xD8*/ static s8 _bss_D8[ANIMCURVES_SCENES_MAX];
/*0x108*/ static s8 _bss_108[ANIMCURVES_SCENES_MAX];
/*0x138*/ static s8 sEventFlags[ANIMCURVES_SCENES_MAX];
/*0x168*/ static s8 _bss_168[ANIMCURVES_SCENES_MAX];
/*0x198*/ static s8 _bss_198[ANIMCURVES_SCENES_MAX];
/*0x1C8*/ static s8 _bss_1C8[ANIMCURVES_SCENES_MAX];
/*0x1F8*/ static s16 _bss_1F8[ANIMCURVES_SCENES_MAX];
/*0x258*/ static s16 _bss_258[ANIMCURVES_SCENES_MAX];
/*0x2B8*/ static s16 _bss_2B8[ANIMCURVES_SCENES_MAX];
/*0x318*/ static s16 _bss_318[ANIMCURVES_SCENES_MAX];
/*0x378*/ static u8 _bss_378[ANIMCURVES_SCENES_MAX];
/*0x3A8*/ static u8 _bss_3A8[ANIMCURVES_SCENES_MAX];
/*0x3D8*/ static s32 _bss_3D8[ANIMCURVES_SCENES_MAX];
/*0x490*/ static u8 _bss_490[ANIMCURVES_SCENES_MAX];
/*0x4C0*/ static u8 _bss_4C0[ANIMCURVES_SCENES_MAX];
/*0x4F0*/ static f32 _bss_4F0[ANIMCURVES_SCENES_MAX];
/*0x5A4*/ static f32 _bss_5A4;
/*0x5A8*/ static f32 _bss_5A8;
/*0x5AC*/ static u8 _bss_5AC;
/*0x5B0*/ static f32 _bss_5B0;
/*0x5B8*/ static Vec3f _bss_5B8;
/*0x5C4*/ static f32 _bss_5C4;
/*0x5C8*/ static s32 _bss_5C8;
/*0x5CC*/ static s8 _bss_5CC;
/*0x5D0*/ static s32 _bss_5D0;
/*0x5D4*/ static s32 _bss_5D4;
/*0x5D8*/ static void* _bss_5D8; //sequence file buffer
/*0x5DC*/ static f32 _bss_5DC;
/*0x5E0*/ static f32 _bss_5E0;
/*0x5E4*/ static f32 _bss_5E4;
/*0x5E8*/ static f32 _bss_5E8;
/*0x5F0*/ static Bss5F0Thing _bss_5F0[20];
/*0x690*/ static s32 _bss_690;
/*0x694*/ static u8 _bss_694[0x4]; // unused gap
/*0x698*/ static ANIMUnk698 _bss_698[16];
/*0x6F8*/ static s8 _bss_6F8;
/*0x6FC*/ static Object *_bss_6FC;
/*0x700*/ static s16 _bss_700;
/*0x708*/ static ANIMActorOverride _bss_708[45][16];
typedef struct {
    u32 unk0_8: 1;
    u32 unk0_1: 31;
} UnkBss1D88;
/*0x1D88*/ static UnkBss1D88 _bss_1D88;

void anim_func_98(void);
static s32 anim_func_4BAC(Object* animObj, Object *parent, f32 x, f32 y, f32 z, f32* yOut, f32 ySetup);
void anim_func_7B64(AnimObj_Data*);
s32 anim_func_9524(Object* arg0, AnimObj_Data* arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6);
static void anim_func_9DD4(void);
static void anim_func_9B70(Object* arg1, Object* arg2, AnimObj_Data* arg3);
static void anim_func_9BC0(s32 arg0);
void anim_end_obj_sequence(s32 arg0);
static s32 anim_func_93A0(Object* actor);
static void anim_func_9CE8(s32 arg0);
static Object* anim_func_9C08(s32 animCurvesIndex, Object* searchObject);
static Object* anim_func_81F8(Object* animObj);
s32 anim_func_8878(void);
static f32 anim_func_6F3C(AnimCurvesKeyframe*, s32, s32);
static void anim_func_5A48(UnkAnimStruct* arg0, CurveSetup* a2, CurveSetup* a3, f32 a4, s8 a5);
static s32 anim_func_6620(Object *arg0, Object *arg1, AnimObj_Data *arg2, s32 arg3, s8 arg4);
static void anim_func_57A4(UnkAnimStruct* arg0, f32 arg1);
static s32 anim_func_51E0(UnkAnimStruct* arg0, Vec3f* arg1, Vec3f* arg2, s16* arg3, s8 arg4);
static s32 anim_func_5E50(s32 arg0, AnimObj_Data* arg1, AnimObj_Setup* arg2);
s16 anim_func_8598(Object* animObj);
static void anim_func_9C94(s32 index, Object* object, Object* overrideObject);
static void anim_func_4FC4(Object* animObj, AnimObj_Data* arg1);
static void anim_func_4B20(Object* animObj, AnimObj_Setup* setup);
static f32 anim_func_6EBC(AnimObj_Data* state, s32 channelIndex, s32 arg2);
static s8 anim_func_4158(AnimObj_Data* animObjData);
static Object* anim_func_2FE8(Object* arg0, AnimObj_Data* arg1, AnimObj_Setup* arg2);
static s32 anim_func_3614(Object* arg0, ModelInstance* arg1, AnimCurvesEvent** arg2, s8 arg3, s32* arg4);
static void anim_func_4924(Object* animObj, Object** actorObject, ModelInstance** actorModelInstance);
static s32 anim_func_60AC(Object* arg0, Object* arg1, AnimObj_Data* arg2, s32* arg3, s16 arg4, s16 arg5, s8 arg6, s8 arg7);
static void anim_func_9EC8(Object* arg0, s16* arg1, s32 arg2);
static void anim_func_72E0(Object* arg0);
static void anim_func_4698(Object* actor, Object* override, AnimObj_Data* animObjData, s8 arg3);
static void anim_func_71C0(Object* arg0, Object* arg1, AnimObj_Data* arg2);
static void anim_func_422C(AnimObj_Data* arg0, Object* arg1, u8 arg2);
static void anim_func_15FC(Object* arg0, Object* arg1, AnimObj_Data* arg2, AnimObj_Setup* arg3);
static void anim_func_1C04(Object* arg0, Object* arg1, AnimObj_Data* arg2, s32 arg3);
static void anim_func_1A04(Object* arg0, Object* arg1, AnimObj_Data* arg2);
static void anim_func_2EB4(Object* arg0, Object* arg1, AnimObj_Data* objData);
static void anim_func_32B0(AnimObj_Data* animObjData, s32 arg1);
static void anim_func_2760(Object* arg0, Object* arg1, AnimObj_Data* arg2, s32 arg3);
static void anim_func_3170(Object* arg0, Object* arg1, AnimObj_Data* arg2);
static void anim_func_3414(Object* animObj, Object **arg1, AnimObj_Data* arg2, AnimObj_Setup* arg3, ModelInstance **arg4);
static s32 anim_func_495C(AnimObj_Data* arg0, Object* arg1);
static s32 anim_func_5D78(Object* override, s32 arg1, AnimObj_Data* objData);
static s32 anim_func_3268(Object* overrideObject, Object* actor, AnimObj_Data* state);

// offset: 0x0 | ctor
void anim_ctor(void *dll) {
    _bss_5D8 = mmAlloc(0x10, ALLOC_TAG_ANIMSEQ_COL, ALLOC_NAME("anim:acbuff"));
    anim_func_98();
}

// offset: 0x58 | dtor
void anim_dtor(void *dll) {
    mmFree(_bss_5D8);
}

// offset: 0x98 | func: 0 | export: 0
void anim_func_98(void) {
    s32 i;

    for (i = 0; i < ANIMCURVES_SCENES_MAX; i++) {
        _bss_108[i] = 0;
        sEventFlags[i] = 0;
        _bss_168[i] = 0;
        _bss_A8[i] = 0;
        _bss_D8[i] = 0;
        _bss_198[i] = 0;
        _bss_1C8[i] = 0;
        _bss_1F8[i] = 0;
        _bss_258[i] = -1;
        _bss_378[i] = 0;
        _bss_3D8[i] = 0;
        _bss_490[i] = 0;
    }

    _bss_20 = 0;
    _bss_8C = 0;
    _bss_8B = 0;
    _bss_A0 = 0.0f;
    _bss_6FC = 0;
    _bss_6F8 = 0;
}

// offset: 0x2C0 | func: 1 | export: 1
void anim_func_2C0(s32 arg0, s32 arg1, s32 arg2) {   
    if (arg0 >= 0 && arg0 < ANIMCURVES_SCENES_MAX) {
        if (_bss_6F8 < ANIMCURVES_ACTORS_MAX) {
            _bss_698[_bss_6F8].unk0 = arg0;
            _bss_698[_bss_6F8].unk4 = arg2;
            _bss_698[_bss_6F8++].unk2 = arg1; 
        } else {
            STUBBED_PRINTF("Max activates reached\n");
        }
    }
}

// offset: 0x324 | func: 2 | export: 2
void anim_func_324(s32 arg0, s32 arg1) {
    if (arg0 >= 0 && arg0 < ANIMCURVES_SCENES_MAX) {
        sEventFlags[arg0] = arg1;
    }
}

// offset: 0x358 | func: 3 | export: 3
s8 anim_func_358(s32 arg0) {
    if (arg0 < 0 || arg0 >= ANIMCURVES_SCENES_MAX) {
        return 0;
    }
    return sEventFlags[arg0];
}

// offset: 0x394 | func: 4 | export: 28
void anim_func_394(s32 arg0, s32 arg1) {
    _bss_378[arg0] = 1;
    _bss_318[arg0] = arg1;
}

// offset: 0x3D0 | func: 5 | export: 4
s32 anim_func_3D0(Object* object, s32 updateRate) {
    AnimObj_Data* temp_s0;
    Object* spD0;
    AnimObj_Setup* spCC;
    f32 temp_fs2;
    f32 temp_fs3;
    f32 temp_fv0_3;
    ModelInstance* spBC;
    AnimCurvesEvent* spB8;
    f32 spB4;
    s32 var_s4;
    f32 var_fv1;
    s32 temp_v0_6;
    s32 var_s1;
    s32 var_s3;
    f32 sp9C;
    f32 sp98;
    s32 sp94;

    sp94 = 0;
    spCC = (AnimObj_Setup*)object->setup;
    if (spCC == NULL) {
        return 1;
    }
    temp_s0 = object->data;
    spD0 = temp_s0->actor;
    _bss_A4 = 0;
    _bss_32 = 0;
    _bss_89 = 0;
    _bss_8A = 0;
    if ((_bss_3A8[temp_s0->unk63] & 8) && ((_bss_4C0[temp_s0->unk63] != 0) || (joy_get_pressed(0) & 0x20))) {
        if (temp_s0->unk8B == 0) {
            return 1;
        }
        temp_s0->unk9D |= 0x80;
        spD0 = object;
        if (temp_s0->actor != NULL) {
            spD0 = temp_s0->actor;
            spD0->unkC0 = object;
            spD0->stateFlags |= 0x1000;
        }
        if (_bss_4C0[temp_s0->unk63] != 0) {
            if ((temp_s0->actor != NULL) && (temp_s0->actor->unkB4 != -1)) {
                _bss_4F0[temp_s0->unk63] += gUpdateRateF;
                if (_bss_4F0[temp_s0->unk63] > 60.0f) {
                    if (_bss_9C != -1) {
                        warpPlayer(_bss_9C, 0);
                    } else {
                        gDLL_28_ScreenFade->vtbl->fade_reversed(30, 1);
                    }
                    anim_func_71C0(object, spD0, temp_s0);
                    return 1;
                }
                if (_bss_4F0[temp_s0->unk63] > 40.0f) {
                    anim_func_15FC(object, spD0, temp_s0, spCC);
                    gDLL_22_Subtitles->vtbl->func_448();
                }
            } else {
                if (_bss_4F0[temp_s0->unk63] > 50.0f) {
                    anim_func_71C0(object, spD0, temp_s0);
                    return 1;
                }
                if (_bss_4F0[temp_s0->unk63] > 40.0f) {
                    anim_func_15FC(object, spD0, temp_s0, spCC);
                }
            }
        } else if ((temp_s0->actor != NULL) && (temp_s0->actor->unkB4 != -1)) {
            gDLL_28_ScreenFade->vtbl->fade(30, 1);
            _bss_4F0[temp_s0->unk63] = 0;
            _bss_4C0[temp_s0->unk63] = 1;
            _bss_9C = -1;
        }

        anim_func_1C04(object, spD0, temp_s0, temp_s0->animCurvesCurrentFrameA);
        if (spD0 != object) {
            anim_func_4698(spD0, object, temp_s0, 0);
        }
        anim_func_4FC4(object, temp_s0);
        if (temp_s0->unk86 == 1) {
            anim_func_4B20(object, spCC);
        }
        object->srt.yaw += temp_s0->unk1A;
        anim_func_1A04(object, spD0, temp_s0);
        if ((temp_s0->actor != NULL) && (temp_s0->actor->unkB4 != -1) && !(_bss_3A8[temp_s0->unk63] & 0x10)) {
            gDLL_2_Camera->vtbl->set_letterbox_goal(30, 1);
        }
        return 0;
    }
    if (temp_s0->unk8B == 3) {
        if (temp_s0->actor != NULL) {
            spD0->unkC0 = object;
            spD0->stateFlags |= 0x1000;
        }
        return 0;
    }
    if (_bss_490[temp_s0->unk63] == 1) {
        temp_s0->animCurvesCurrentFrameA = _bss_2B8[temp_s0->unk63];
        temp_s0->animCurvesCurrentFrameB = temp_s0->animCurvesCurrentFrameA;
        anim_func_2EB4(object, spD0, temp_s0);
    } else {
        temp_s0->animCurvesCurrentFrameA = _bss_1F8[temp_s0->unk63];
    }
    anim_func_32B0(temp_s0, updateRate);
    _bss_33 = 0;
    do {
        _bss_88 = 0;
        if (temp_s0->unk8B == 0) {
            object->opacity = 0;
            return 1;
        }
        spD0 = object;
        if (temp_s0->actor != NULL) {
            spD0 = temp_s0->actor;
            spD0->unkC0 = object;
            spD0->stateFlags |= 0x1000;
        } else if ((temp_s0->unk87 == 0) && (temp_s0->unk62 < 4)) {
            temp_s0->unk62 = -1;
        }
        if (_bss_A8[temp_s0->unk63] != 0) {
            if (_bss_1C8[temp_s0->unk63] != 0) {
                temp_s0->animCurvesCurrentFrameA -= _bss_1C8[temp_s0->unk63];
                if (temp_s0->animCurvesCurrentFrameA < 0) {
                    temp_s0->animCurvesCurrentFrameA = 0;
                }
                temp_s0->animCurvesCurrentFrameB = (s16) (temp_s0->animCurvesCurrentFrameA - 1);
                anim_func_2760(object, spD0, temp_s0, 1);
            }
        }
        _bss_5CC = 0;
        if (spD0 != object) {
            anim_func_4698(spD0, object, temp_s0, _bss_A8[temp_s0->unk63]);
            _bss_5CC = 1;
        }
        anim_func_3170(object, spD0, temp_s0);
        if (temp_s0->unk8B == 2) {
            anim_func_3414(object, &spD0, temp_s0, spCC, &spBC);
            return 0;
        }
        if (_bss_A8[temp_s0->unk63] == 1) {
            updateRate = 0;
        } else if (_bss_A8[temp_s0->unk63] == 2) {
            temp_s0->animCurvesCurrentFrameA = temp_s0->animCurvesDuration;
            _bss_89 = 1;
        } else if (_bss_A8[temp_s0->unk63] == 3) {
            temp_v0_6 = anim_func_495C(temp_s0, object);
            if (temp_v0_6 >= 0) {
                _bss_33 = 1;
                temp_s0->animCurvesCurrentFrameA = (s16) temp_v0_6;
                temp_s0->animCurvesCurrentFrameB = temp_s0->animCurvesCurrentFrameA;
            }
        }
        if ((temp_s0->actor != NULL) && (temp_s0->actor->unkB4 != -1) && !(_bss_3A8[temp_s0->unk63] & 0x10)) {
            gDLL_2_Camera->vtbl->set_letterbox_goal(30, 1);
        }
        if (_bss_378[temp_s0->unk63] != 0) {
            temp_s0->unk1A = _bss_318[temp_s0->unk63];
        }
        if (temp_s0->unk88 != 0) {
            if (anim_func_5E50(temp_s0->unk88 - 1, temp_s0, spCC) == 0) {
                temp_s0->unk88 = 0;
            } else {
                _bss_258[temp_s0->unk63] = temp_s0->animCurvesCurrentFrameA;
                return 0;
            }
        }
        temp_s0->animCurvesCurrentFrameA += updateRate;
        if (temp_s0->animCurvesDuration < temp_s0->animCurvesCurrentFrameA) {
            temp_s0->animCurvesCurrentFrameA = temp_s0->animCurvesDuration;
        }
        var_s3 = (s32) temp_s0->animCurvesCurrentFrameA;
        anim_func_1C04(object, spD0, temp_s0, var_s3);
        object->srt.transl.x += temp_s0->unk4;
        object->srt.transl.y += temp_s0->unk8;
        object->srt.transl.z += temp_s0->unkC;
        object->srt.roll += temp_s0->unk18;
        object->srt.pitch += temp_s0->unk16;
        object->srt.yaw += temp_s0->unk14;
        spBC = spD0->modelInsts[spD0->modelInstIdx];
        _bss_690 = 0;
        if (spBC != NULL) {
            sp9C = anim_func_6EBC(temp_s0, 0xD, temp_s0->animCurvesCurrentFrameB) + spCC->base.x;
            sp98 = anim_func_6EBC(temp_s0, 0xB, temp_s0->animCurvesCurrentFrameB) + spCC->base.z;
        }
        temp_s0->animCurvesCurrentFrameA = temp_s0->animCurvesCurrentFrameB;
        while (temp_s0->animCurvesCurrentFrameA < var_s3) {
            temp_s0->animCurvesCurrentFrameA += 1;
            temp_fs3 = anim_func_6EBC(temp_s0, 0xD, temp_s0->animCurvesCurrentFrameA) + spCC->base.x;
            temp_fs2 = anim_func_6EBC(temp_s0, 0xB, temp_s0->animCurvesCurrentFrameA) + spCC->base.z;
            if ((temp_s0->animCurvesCurrentFrameA > 0) && (temp_s0->unk7A & 4)) {
                if ((temp_s0->unk84 == 1) && (temp_s0->unk87 == 0) && (spBC != NULL)) {
                    temp_fv0_3 = temp_fs3 - sp9C;
                    var_fv1 = temp_fs2 - sp98;
                    if (func_8002493C(spD0, sqrtf(SQ(temp_fv0_3) + SQ(var_fv1)), &spB4) == 0) {
                        spB4 = anim_func_6EBC(temp_s0, 9, temp_s0->animCurvesCurrentFrameA - 1) * 0.0004f;
                    }
                } else {
                    spB4 = anim_func_6EBC(temp_s0, 9, temp_s0->animCurvesCurrentFrameA - 1) * 0.0004f;
                }
                if (spBC != NULL) {
                    func_80024108(spD0, spB4, 1.0f, &temp_s0->unkFC);
                    func_80025780(spD0, 1.0f, &temp_s0->unkFC, 0);
                    if ((temp_s0->unk30 != 0) && (spBC->model->unk71 & 1)) {
                        func_80032B44(spD0, temp_s0->unk30);
                    }
                    func_8001B084(spBC, 1.0f);
                    if (temp_s0->unk20 > 0.0f) {
                        if (temp_s0->channelTotalKeys[10] != 0) {
                            var_fv1 = anim_func_6EBC(temp_s0, 10, temp_s0->animCurvesCurrentFrameA - 1);
                        } else {
                            var_fv1 = 8.0f;
                        }
                        if (var_fv1 < 1.0f) {
                            var_fv1 = 1.0f;
                        }
                        var_fv1 = 1.0f / var_fv1;
                        temp_s0->unk20 -= var_fv1;
                        if (temp_s0->unk20 < 0.0f) {
                            temp_s0->unk20 = 0.0f;
                        }
                    }
                    dummy_label1: ; // @fake
                } else {
                    spD0->animProgress += spB4;
                    while (spD0->animProgress > 1.0f) {
                        spD0->animProgress -= 1.0f;
                    }
                    while (spD0->animProgress < 0.0f) {
                        spD0->animProgress += 1.0f;
                    }
                }
            }
            sp9C = temp_fs3;
            sp98 = temp_fs2;
            var_s1 = 0;
            while ((var_s1 == 0) && (temp_s0->unk72 < temp_s0->animCurvesEventCount)) {
                spB8 = temp_s0->animCurvesEvents + temp_s0->unk72;
                // TODO: check enum usage
                if (spB8->type == ANIM_EVT_SETTIME) {
                    if (temp_s0->animCurvesCurrentFrameA >= spB8->params) {
                        temp_s0->unk74 = spB8->params;
                        temp_s0->unk72 += 1;
                    } else {
                        var_s1 = 1;
                    }
                } else {
                    if (temp_s0->animCurvesCurrentFrameA >= temp_s0->unk74) {
                        if (spB8->type != ANIM_EVT_SFX_WITH_DURATION) {
                            temp_s0->unk74 += spB8->delay;
                        }
                        temp_s0->unk72 += 1;
                        if (anim_func_3614(object, spBC, &spB8, 0, NULL) != 0) {
                            var_s3 = temp_s0->animCurvesCurrentFrameA;
                        }
                        anim_func_4924(object, &spD0, &spBC);
                    } else {
                        var_s1 = 1;
                    }
                }
            }
        }
        for (var_s4 = 0; var_s4 < 10; var_s4++) {
            if (temp_s0->unk138[var_s4] && (anim_func_5D78(object, temp_s0->unk138[var_s4], temp_s0) != 0)) {
                _bss_33 = 1;
                temp_s0->animCurvesCurrentFrameA = temp_s0->unk124[var_s4];
                temp_s0->animCurvesCurrentFrameB = temp_s0->animCurvesCurrentFrameA;
                var_s4 = 0;
                while (var_s4 < 10) {
                    temp_s0->unk138[var_s4++] = 0;
                }
                break;
            }
        }
        if ((_bss_5CC == 0) && (spD0 != object)) {
            anim_func_4698(spD0, object, temp_s0, _bss_A8[temp_s0->unk63]);
        }
        if (temp_s0->unk9D != 0) {
            _bss_33 = anim_func_3268(object, spD0, temp_s0);
        }
        temp_s0->messageCount = 0;
        temp_s0->lastMessage = 0;
        if ((spBC != NULL) && (temp_s0->unk7A & 4)) {
            spBC->animState0->unk58[0] = (s16) (temp_s0->unk20 * 1023.0f);
        }
        anim_func_4FC4(object, temp_s0);
        if (temp_s0->unk86 == 1) {
            anim_func_4B20(object, spCC);
        }
        object->srt.yaw += temp_s0->unk1A;
        anim_func_1A04(object, spD0, temp_s0);
        anim_func_422C(temp_s0, spD0, 0);
        gDLL_15_Projgfx->vtbl->func2(temp_s0->animCurvesCurrentFrameA - temp_s0->animCurvesCurrentFrameB, spD0);
        for (var_s4 = 0; var_s4 < _bss_690; var_s4++) {
            if (anim_func_60AC(object, spD0, temp_s0, 
                                _bss_5F0[var_s4].unk0, 
                                _bss_5F0[var_s4].unk6, 
                                _bss_5F0[var_s4].unk4, 0, 0) != 0) {
                var_s4 = _bss_690;
            }
            anim_func_4924(object, &spD0, &spBC);
        }
        temp_s0->animCurvesCurrentFrameB = temp_s0->animCurvesCurrentFrameA;
        if (_bss_A4 != 0) {
            anim_func_4924(object, &spD0, &spBC);
            anim_func_71C0(object, spD0, temp_s0);
        } else {
            if (_bss_33 != 0) {
                _bss_2B8[temp_s0->unk63] = temp_s0->animCurvesCurrentFrameA;
                _bss_490[temp_s0->unk63] = 2;
                _bss_258[temp_s0->unk63] = temp_s0->animCurvesCurrentFrameA;
            }
            if (_bss_258[temp_s0->unk63] == -1) {
                _bss_258[temp_s0->unk63] = _bss_1F8[temp_s0->unk63] + updateRate;
            }
        }
    } while ((_bss_A4 == 0) && (temp_s0->animCurvesCurrentFrameA < temp_s0->animCurvesDuration) && sp94--);
    
    return 0;
}

// offset: 0x15FC | func: 6
static void anim_func_15FC(Object* arg0, Object* arg1, AnimObj_Data* arg2, AnimObj_Setup* arg3) {
    AnimCurvesEvent* sp7C;
    s32 _pad;
    s32 _pad2;
    Bss5F0Thing* temp_v0_2;
    s32 _pad3;
    Object* var_s4;
    s32 var_s0;
    s32 sp60;
    ModelInstance* sp5C;

    if (arg1){} // @fake

    var_s4 = arg0;
    sp5C = arg0->modelInsts[arg0->modelInstIdx];
    if (arg2->actor != NULL) {
        var_s4 = arg2->actor;
        var_s4->unkC0 = arg0;
        var_s4->stateFlags |= 0x1000;
    }
    if (arg2->animCurvesEvents != NULL) {
        arg2->unk78 = -1;
        _bss_88 = 0;
        _bss_690 = _bss_88;
        while (arg2->unk72 < arg2->animCurvesEventCount) {
            sp7C = &arg2->animCurvesEvents[arg2->unk72];
            // TODO: check enum usage
            if (sp7C->type == ANIM_EVT_SETTIME) {
                arg2->unk74 = sp7C->params;
            } else if ((sp7C->type == ANIM_EVT_CODE) && (sp7C->params > 0)) {
                arg2->unk74 += sp7C->delay;
                anim_func_3614(arg0, sp5C, &sp7C, 0, &sp60);
            } else {
                if (sp7C->type != ANIM_EVT_SFX_WITH_DURATION) {
                    arg2->unk74 += sp7C->delay;
                }
                if (sp7C->type == ANIM_EVT_ENVFX) {
                    if (((sp7C->params >> 0xC) & 0xF) == 6) {
                        _bss_9C = sp7C->params & 0xFFF;
                    }
                }
                if (sp7C->type == ANIM_EVT_ANIM) {
                    arg2->unk78 = sp7C->params & 0xFFF;
                } else {
                    anim_func_3614(arg0, sp5C, &sp7C, 8, &sp60);
                }
            }
            arg2->unk72 += 1;
            if (_bss_690 >= 20) {
                for (var_s0 = 0; var_s0 < _bss_690; var_s0++) {
                    temp_v0_2 = &_bss_5F0[var_s0];
                    if (anim_func_60AC(arg0, var_s4, arg2, 
                                        temp_v0_2->unk0, 
                                        temp_v0_2->unk6, 
                                        temp_v0_2->unk4, 0, 1) != 0) {
                        var_s0 = _bss_690;
                    }
                }
                _bss_690 = 0;
            }
            if (_bss_88 >= 10) {
                anim_func_422C(arg2, var_s4, 1);
                _bss_88 = 0;
            }
            if ((arg2->messageCount >= 10) && (var_s4 != arg0)) {
                anim_func_4698(var_s4, arg0, arg2, 0);
            }
        }
        for (var_s0 = 0; var_s0 < _bss_690; var_s0++) {
            temp_v0_2 = &_bss_5F0[var_s0];
            if (anim_func_60AC(arg0, var_s4, arg2, 
                                temp_v0_2->unk0, 
                                temp_v0_2->unk6, 
                                temp_v0_2->unk4, 0, 1) != 0) {
                var_s0 = _bss_690;
            }
        }
        _bss_690 = 0;
        if (_bss_88 != 0) {
            anim_func_422C(arg2, var_s4, 1);
            _bss_88 = 0;
        }
        if (sp5C != NULL) {
            if (arg2->unk78 != -1) {
                func_80023D30(var_s4, arg2->unk78, 0.0f, 0);
                sp5C->animState0->unk58[0] = 0;
            }
        }
    }
    arg2->animCurvesCurrentFrameA = arg2->animCurvesDuration - 1;
}

// offset: 0x1A04 | func: 7
static void anim_func_1A04(Object* arg0, Object* arg1, AnimObj_Data* arg2) {
    f32 var_fa0;
    f32 var_fv0;
    f32 var_fv1;
    s16 temp_a1;
    s16 temp_a3;
    s16 var_v1;

    if ((arg1->parent == arg0->parent) || (_bss_32 == 0)) {
        var_fv0 = arg0->srt.transl.x;
        var_fv1 = arg0->srt.transl.y;
        var_fa0 = arg0->srt.transl.z;
        var_v1 = arg0->srt.yaw;
    } else {
        var_fv0 = _bss_24;
        var_v1 = _bss_30;
        var_fv1 = _bss_28;
        var_fa0 = _bss_2C;
    }
    temp_a1 = arg0->srt.pitch;
    temp_a3 = arg0->srt.roll;
    if (arg1 != arg0) {
        if (arg2->unk7A & 1) {
            if (arg2->unk62 == 2) {
                arg1->srt.transl.x = (arg2->unk4C.x * arg2->unk58) + var_fv0;
                arg1->srt.transl.y = (arg2->unk4C.y * arg2->unk58) + var_fv1;
                arg1->srt.transl.z = (arg2->unk4C.z * arg2->unk58) + var_fa0;
            } else {
                arg1->srt.transl.x = var_fv0;
                arg1->srt.transl.y = var_fv1;
                arg1->srt.transl.z = var_fa0;
            }
        }
        if (arg2->unk7A & 2) {
            if (arg2->unk62 == 2) {
                arg1->srt.yaw = var_v1 + (s16) ((f32) arg2->yawDiff * arg2->unk58);
                arg1->srt.pitch = temp_a1 + (s16) ((f32) arg2->pitchDiff * arg2->unk58);
                arg1->srt.roll = temp_a3 + (s16) ((f32) arg2->rollDiff * arg2->unk58);
            } else {
                arg1->srt.yaw = var_v1;
                arg1->srt.pitch = temp_a1;
                arg1->srt.roll = temp_a3;
            }
        }
    }
    if ((arg2->unk87 != 0) && (arg2->unk84 != 0)) {
        anim_func_72E0(arg0);
    }
    get_object_child_position(arg1, &arg1->globalPosition.x, &arg1->globalPosition.y, &arg1->globalPosition.z);
}

// offset: 0x1C04 | func: 8
static void anim_func_1C04(Object* arg0, Object* arg1, AnimObj_Data* arg2, s32 arg3) {
    f32 var_fv1;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    s32 _pad;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    ObjSetup* sp3C;
    Object* temp_v0_2;
    s32 _pad2;
    s16* temp_v0_3;

    sp3C = arg0->setup;
    arg0->srt.transl.x = sp3C->x;
    arg0->srt.transl.y = sp3C->y;
    arg0->srt.transl.z = sp3C->z;
    arg0->srt.pitch = 0;
    arg0->srt.yaw = 0;
    arg0->srt.roll = 0;
    if (arg2->unk7A & 0x20) {
        arg1->opacity = 0xFF;
    }
    _bss_5E0 = 0.0f;\
    _bss_5E4 = 0.0f;\
    _bss_5E8 = 0.0f;
    if (arg2->animCurvesKeyframes != NULL) {
        var_fv1 = anim_func_6EBC(arg2, 0x12, arg3);
        if (var_fv1 > 0.0f) {
            if (arg2->unk34[3] != 0) {
                gDLL_6_AMSFX->vtbl->set_vol(arg2->unk34[3], var_fv1);
            }
        }
        if (!(arg2->unk8C & 2)) {
            arg0->srt.yaw = (s16) (anim_func_6EBC(arg2, 7, arg3) * 182.044f);
            arg0->srt.pitch = (s16) (anim_func_6EBC(arg2, 8, arg3) * 182.044f);
            arg0->srt.roll = (s16) (anim_func_6EBC(arg2, 6, arg3) * 182.044f);
            _bss_5E0 = anim_func_6EBC(arg2, 0xD, arg3);
            _bss_5E4 = anim_func_6EBC(arg2, 0xC, arg3);
            _bss_5E8 = anim_func_6EBC(arg2, 0xB, arg3);
            _bss_24 = _bss_5E0;
            _bss_28 = _bss_5E4;
            _bss_2C = _bss_5E8;
            _bss_30 = arg0->srt.yaw;
            _bss_32 = 1;
            arg0->srt.transl.x = sp3C->x + _bss_5E0;
            arg0->srt.transl.y = sp3C->y + _bss_5E4;
            arg0->srt.transl.z = sp3C->z + _bss_5E8;
        } else {
            _bss_5E4 = anim_func_6EBC(arg2, 0xC, arg3);
            _bss_5E8 = anim_func_6EBC(arg2, 0xB, arg3);
            arg0->srt.yaw = (s16) (anim_func_6EBC(arg2, 7, arg3) * 182.044f);
            arg0->srt.pitch = (s16) (anim_func_6EBC(arg2, 8, arg3) * 182.044f);
            sp58 = fsin16_precise(arg0->srt.yaw - 0x4000);
            sp50 = fcos16_precise(arg0->srt.yaw - 0x4000);
            sp54 = fcos16_precise(arg0->srt.pitch);
            sp44 = fsin16_precise(arg0->srt.pitch);
            sp44 = _bss_5E8 * sp44;
            sp40 = _bss_5E8 * sp54;
            sp48 = sp40 * sp50;
            sp40 = sp40 * sp58;
            temp_v0_2 = get_player();
            arg0->srt.transl.x = temp_v0_2->srt.transl.x + sp48;
            arg0->srt.transl.y = temp_v0_2->srt.transl.y + _bss_5E4 + sp44;
            arg0->srt.transl.z = temp_v0_2->srt.transl.z + sp40;
            arg0->srt.yaw = 0x8000 - arg0->srt.yaw;
            arg0->srt.pitch = -arg0->srt.pitch;
            _bss_24 = arg0->srt.transl.x - sp3C->x;
            _bss_28 = arg0->srt.transl.y - sp3C->y;
            _bss_2C = arg0->srt.transl.z - sp3C->z;
            _bss_30 = arg0->srt.yaw;
            _bss_32 = 1;
        }
        if (arg2->channelTotalKeys[0xE] != 0) {
            var_fv1 = anim_func_6EBC(arg2, 0xE, arg3);
            if (arg2->unk87 != 0) {
                if (var_fv1 < 35.0f) {
                    var_fv1 = 35.0f;
                }
                if (var_fv1 > 120.0f) {
                    var_fv1 = 125.0f;
                }
                _data_30 = 1;
                _bss_5DC = var_fv1;
            } else {
                arg2->unk10 = var_fv1;
            }
        }
        if ((arg2->unk7A & 0x20) && (arg2->channelTotalKeys[3] != 0)) {
            var_fv1 = anim_func_6EBC(arg2, 3, arg3);
            if (var_fv1 < 0.0f) {
                var_fv1 = 0.0f;
            }
            if (var_fv1 > 255.0f) {
                var_fv1 = 255.0f;
            }
            arg1->opacity = (u8) (u32) var_fv1;
        }
        if (arg2->channelTotalKeys[4] != 0) {
            gDLL_7_Newday->vtbl->func9(anim_func_6EBC(arg2, 4, arg3) * 60.0f);
        }
        if ((arg2->unk7A & 0x10) && (arg2->channelTotalKeys[5] != 0)) {
            var_fv1 = anim_func_6EBC(arg2, 5, arg3);
            arg1->srt.scale = arg1->def->scale * var_fv1;
        }
        if (arg2->unk7A & 8) {
            temp_v0_3 = func_80034804(arg1, 0);
            if (temp_v0_3 != NULL) {
                if (arg2->channelTotalKeys[1] != 0) {
                    var_fv1 = anim_func_6EBC(arg2, 1, arg3);
                } else {
                    var_fv1 = 0.0f;
                }
                temp_v0_3[0] = arg2->unk122 + (s16) (var_fv1 * 182.044f);
                if (arg2->channelTotalKeys[2] != 0) {
                    var_fv1 = anim_func_6EBC(arg2, 2, arg3);
                } else {
                    var_fv1 = 0.0f;
                }
                temp_v0_3[1] = arg2->unk120 + (s16) (var_fv1 * 182.044f);
                if (arg2->channelTotalKeys[0] != 0) {
                    var_fv1 = anim_func_6EBC(arg2, 0, arg3);
                } else {
                    var_fv1 = 0.0f;
                }
                temp_v0_3[2] = (s16) (var_fv1 * 182.044f);
                if (arg2->unk7A & 0x400) {
                    anim_func_9EC8(arg1, temp_v0_3, arg2->unk142_4);
                }
            }
            if (1){} // @fake
        }
        if (arg2->unk7A & 0x200) {
            temp_v0_3 = func_80034804(arg1, 1);
            if (temp_v0_3 != NULL) {
                if (arg2->channelTotalKeys[0x11] != 0) {
                    var_fv1 = anim_func_6EBC(arg2, 0x11, arg3);
                } else {
                    var_fv1 = 0.0f;
                }
                temp_v0_3[0] = (s16) (var_fv1 * 182.044f);
            }
            if (1){} // @fake
        }
        if (arg2->unk7A & 0x40) {
            TextureAnimator* temp_v0_6;
            TextureAnimator* var_v1;
            TextureAnimator* temp_s0;
            temp_s0 = func_800348A0(arg1, 1, 0);
            var_v1 = func_800348A0(arg1, 0, 0);
            if ((temp_s0 != NULL) || (var_v1 != NULL)) {
                if (arg2->channelTotalKeys[0xF] != 0) {
                    var_fv1 = anim_func_6EBC(arg2, 0xF, arg3);
                } else {
                    var_fv1 = 0.0f;
                }
                if (temp_s0 != NULL) {
                    temp_s0->positionU = (s16) var_fv1;
                }
                if (var_v1 != NULL) {
                    var_v1->positionU = -(s16) var_fv1;
                }
                if (arg2->channelTotalKeys[0x10] != 0) {
                    var_fv1 = anim_func_6EBC(arg2, 0x10, arg3);
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
            temp_s0 = func_800348A0(arg1, 5, 0);
            temp_v0_6 = func_800348A0(arg1, 4, 0);
            if (temp_s0 != NULL) {
                temp_s0->frame = arg2->unk9A << 8;
            }
            if (temp_v0_6 != NULL) {
                temp_v0_6->frame = arg2->unk9B << 8;
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
static void anim_func_2760(Object* arg0, Object* arg1, AnimObj_Data* arg2, s32 arg3) {
    AnimCurvesEvent* spBC;
    AnimObj_Setup* spB8;
    ModelInstance* spB4;
    s32 _pad;
    f32 temp_fv0;
    s32 spA8;
    f32 temp_fv1;
    f32 spA0;
    f32 var_fv1;
    s32 _pad2;
    s32 var_s6;
    f32 sp90;
    f32 sp8C;
    s8 sp8B;
    f32 sp7C[3];
    s32 var_s0;
    s32 var_s0_2;
    s32 sp70;

    if (arg2->animCurvesEvents == NULL) {
        return;
    }

    sp8B = 1;
    if (arg3 != 0) {
        sp8B |= 2;
    }
    spB8 = (AnimObj_Setup*)arg0->setup;
    spA8 = (s32) arg2->animCurvesCurrentFrameA;
    _data_C4 = spA8;
    arg2->unk72 = 0;
    arg2->unk74 = -0x32;
    arg2->unk84 = 0;
    arg2->unk86 = 0;
    arg2->unk85 = 0;
    arg2->actor = NULL;
    arg2->unk87 = 0;
    arg2->unk20 = 0.0f;
    arg2->animCurvesCurrentFrameA = -1;
    arg1 = arg0;
    var_s6 = -1;
    for (var_s0 = 0; var_s0 < arg2->animCurvesEventCount && spA8 >= arg2->animCurvesCurrentFrameA; var_s0++) {
        spBC = &arg2->animCurvesEvents[var_s0];
        // TODO: are these the correct enums?
        switch (spBC->type) {
        case ANIM_EVT_OVERRIDE:
            sp8B |= 4;
            arg1 = anim_func_2FE8(arg0, arg2, spB8);
            arg1->curModAnimIdLayered = -1;
            break;
        case ANIM_EVT_SETTIME:
            arg2->animCurvesCurrentFrameA = spBC->params;
            break;
        case ANIM_EVT_ANGLE_MODE:
            var_s6 = arg2->animCurvesCurrentFrameA;
            break;
        case ANIM_EVT_CODE:
            if (spBC->params > 0) {
                var_s0 += spBC->params;
            }
            break;
        default:
            if (spBC->type != ANIM_EVT_SFX_WITH_DURATION) {
                arg2->animCurvesCurrentFrameA += spBC->delay;
            }
            break;
        }
    }
    arg2->animCurvesCurrentFrameA = var_s6;
    spB4 = arg1->modelInsts[arg1->modelInstIdx];
    if (spB4 != NULL) {
        
    }
    if (spB4 != NULL) {
        sp90 = anim_func_6EBC(arg2, 0xD, -1) + spB8->base.x;
        sp8C = anim_func_6EBC(arg2, 0xB, -1) + spB8->base.z;
    }
    while (arg2->animCurvesCurrentFrameA < spA8) {
        arg2->animCurvesCurrentFrameA += 1;
        sp7C[0] = anim_func_6EBC(arg2, 0xD, arg2->animCurvesCurrentFrameA) + spB8->base.x;
        sp7C[1] = anim_func_6EBC(arg2, 0xC, arg2->animCurvesCurrentFrameA) + spB8->base.y;
        sp7C[2] = anim_func_6EBC(arg2, 0xB, arg2->animCurvesCurrentFrameA) + spB8->base.z;
        if (arg2->animCurvesCurrentFrameA > 0 && arg3 != 0) {
            if ((arg2->unk84 == 1) && (arg2->unk87 == 0) && (spB4 != NULL)) {
                temp_fv0 = sp7C[0] - sp90;
                var_fv1 = sp7C[2] - sp8C;
                if (func_8002493C(arg1, sqrtf(SQ(temp_fv0) + SQ(var_fv1)), &spA0) == 0) {
                    spA0 = anim_func_6EBC(arg2, 9, arg2->animCurvesCurrentFrameA - 1) * 0.0004f;
                }
            } else {
                spA0 = anim_func_6EBC(arg2, 9, arg2->animCurvesCurrentFrameA - 1) * 0.0004f;
            }
            if (spB4 != NULL) {
                func_80024108(arg1, spA0, 1.0f, &arg2->unkFC);
                func_80025780(arg1, 1.0f, &arg2->unkFC, 0);
                if ((arg3 != 0) && (arg2->unk20 > 0.0f)) {
                    if (arg2->channelTotalKeys[10] != 0) {
                        var_fv1 = anim_func_6EBC(arg2, 10, arg2->animCurvesCurrentFrameA - 1);
                    } else {
                        var_fv1 = 8.0f;
                    }
                    if (var_fv1 < 1.0f) {
                        var_fv1 = 1.0f;
                    }
                    var_fv1 = 1.0f / var_fv1;
                    arg2->unk20 -= var_fv1;
                    if (arg2->unk20 < 0.0f) {
                        arg2->unk20 = 0.0f;
                    }
                }
                dummy_label_46300: ; // @fake
            } else {
                arg1->animProgress += spA0;
                while (arg1->animProgress > 1.0f) {
                    arg1->animProgress -= 1.0f;
                }
                while (arg1->animProgress < 0.0f) {
                    arg1->animProgress += 1.0f;
                }
            }
        }
        sp90 = sp7C[0];
        sp8C = sp7C[2];
        _bss_690 = 0;
        var_s0_2 = 0;
        while (var_s0_2 == 0 && arg2->unk72 < arg2->animCurvesEventCount) {
            spBC = &arg2->animCurvesEvents[arg2->unk72];
            if (spBC->type == ANIM_EVT_SETTIME) {
                if (arg2->animCurvesCurrentFrameA >= spBC->params) {
                    arg2->unk74 = spBC->params;
                    arg2->unk72 += 1;
                } else {
                    var_s0_2 = 1;
                }
            } else {
                if (arg2->animCurvesCurrentFrameA >= arg2->unk74) {
                    if (spBC->type != ANIM_EVT_SFX_WITH_DURATION) {
                        arg2->unk74 = arg2->unk74 + spBC->delay;
                    }
                    arg2->unk72 += 1;
                    if (anim_func_3614(arg0, spB4, &spBC, sp8B, &sp70) != 0) {
                        return;
                    }
                    anim_func_4924(arg0, &arg1, &spB4);
                } else {
                    var_s0_2 = 1;
                }
            }
        }
        for (var_s0 = 0; var_s0 < _bss_690; var_s0++) {
            if (anim_func_60AC(arg0, arg1, arg2, _bss_5F0[var_s0].unk0, _bss_5F0[var_s0].unk6, _bss_5F0[var_s0].unk4, 1, 0) != 0) {
                var_s0 = _bss_690;
            }
            anim_func_4924(arg0, &arg1, &spB4);
        }
        _bss_690 = 0;
    }
}

// offset: 0x2EB4 | func: 10
static void anim_func_2EB4(Object* arg0, Object* arg1, AnimObj_Data* objData) {
    s16 frame;
    s16 temp_t0;
    s32 finished;
    s8 type;
    AnimCurvesEvent* event;

    if (objData->animCurvesEvents == NULL) {
        return;
    }
    
    objData->unk72 = 0;
    objData->unk74 = -1;
    objData->unk20 = 0.0f;

    finished = FALSE;
    while (!finished && (objData->unk72 < objData->animCurvesEventCount)){
        event = &objData->animCurvesEvents[objData->unk72];
        if (event->type == ANIM_EVT_SETTIME) {
            if (objData->animCurvesCurrentFrameA >= event->params) {
                objData->unk74 = event->params;
                objData->unk72++;
            } else {
                finished = TRUE;
            }
        } else {
            if ((event->type == ANIM_EVT_CODE) && (event->params > 0)) {
                if (objData->animCurvesCurrentFrameA >= objData->unk74) {
                    objData->unk74 += event->delay;
                    objData->unk72 += event->params + 1;
                } else {
                    finished = TRUE;
                }
            } else if (objData->animCurvesCurrentFrameA >= objData->unk74) {
                if (event->type != ANIM_EVT_SFX_WITH_DURATION) {
                    objData->unk74 += event->delay;
                }
                objData->unk72++;
            } else {
                finished = TRUE;
            }
        }
    }
}

// offset: 0x2FE8 | func: 11
static Object* anim_func_2FE8(Object* arg0, AnimObj_Data* arg1, AnimObj_Setup* arg2) {
    Object* var_v1;

    var_v1 = arg0;
    arg1->unk85 ^= 1;
    if (arg1->unk85 != 0) {
        anim_func_8598(arg0);
        if (arg1->actor != NULL) {
            var_v1 = arg1->actor;
            var_v1->unkC0 = arg0;
            var_v1->stateFlags |= OBJSTATE_IN_SEQ;
            arg1->unk11C = var_v1;
            anim_func_9C94(arg1->unk63, arg1->actor, arg0);
        }
    } else if (arg1->actor != NULL) {
        if (arg1->unk7A & 1) {
            var_v1->srt.transl.x = arg0->srt.transl.x;
            var_v1->srt.transl.y = arg0->srt.transl.y;
            var_v1->srt.transl.z = arg0->srt.transl.z;
            var_v1->srt.roll = arg0->srt.roll;
            anim_func_4FC4(var_v1, arg1);
        }
        if (arg1->unk86 == 1) {
            anim_func_4B20(var_v1, arg2);
        }
        if (arg1->unk7A & 2) {
            var_v1->srt.yaw += arg1->unk1A;
        }
        var_v1->unkC0 = NULL;
        var_v1->stateFlags &= ~OBJSTATE_IN_SEQ;
        arg1->actor = NULL;
        var_v1 = arg0;
    }
    return var_v1;
}

// offset: 0x3170 | func: 12
static void anim_func_3170(Object* arg0, Object* arg1, AnimObj_Data* arg2) {
    if (arg2->unk9D & 1) {
        _bss_108[arg2->unk63] = 1;
    }
    if (arg2->unk9D & 2) {
        _bss_108[arg2->unk63] = 0;
    }
    if (arg2->unk9D & 4) {
        sEventFlags[arg2->unk63] = 1;
    }
    if (arg2->unk9D & 8) {
        sEventFlags[arg2->unk63] = 0;
    }
    if (arg2->unk9D & 0x10) {
        _bss_198[arg2->unk63] = 1;
    }
    if (arg2->unk9D & 0x20) {
        _bss_198[arg2->unk63] = 0;
    }
}

// offset: 0x3268 | func: 13
static s32 anim_func_3268(Object* overrideObject, Object* actor, AnimObj_Data* state) {
    s32 returnVal;

    returnVal = 0;
    if (state->unk9D & 0x40) {
        returnVal = 1;
        state->unk9D &= 0xFFBF;
        state->animCurvesCurrentFrameA = state->unk80;
        state->animCurvesCurrentFrameB = state->animCurvesCurrentFrameA;
    }
    state->unk9D = 0;
    return returnVal;
}

// offset: 0x32B0 | func: 14
static void anim_func_32B0(AnimObj_Data* animObjData, s32 arg1) {
    s32 index;

    for (index = 0; index < 4; index++){
        if (animObjData->unk34[index]) {
            if (gDLL_6_AMSFX->vtbl->is_playing(animObjData->unk34[index]) == 0) {
                gDLL_6_AMSFX->vtbl->stop(animObjData->unk34[index]);
                animObjData->unk34[index] = 0;
                animObjData->unk44[index] = 0;
                if (index != 3) {
                    animObjData->unk8A = index;
                }
            }
            if (gDLL_6_AMSFX->vtbl->is_playing(animObjData->unk34[index]) && (animObjData->unk44[index] <= 0)) {
                gDLL_6_AMSFX->vtbl->stop(animObjData->unk34[index]);
                animObjData->unk34[index] = 0;
                animObjData->unk44[index] = 0;
                if (index != 3) {
                    animObjData->unk8A = index;
                }
            }
            if ((animObjData->unk44[index] > 0) && (animObjData->unk44[index] != 32000)) {
                animObjData->unk44[index] = animObjData->unk44[index] - arg1;
            }
        }
    }
}

// offset: 0x3414 | func: 15
static void anim_func_3414(Object* animObj, Object **arg1, AnimObj_Data* arg2, AnimObj_Setup* arg3, ModelInstance **arg4) {
    Object* temp_s1;

    _bss_90 = 1;
    _bss_98 = 0x5A;
    arg2->animCurvesCurrentFrameA = arg2->unk6A;
    arg2->animCurvesCurrentFrameB = -0x3C;
    anim_func_1C04(animObj, *arg1, arg2, 0);
    anim_func_2760(animObj, *arg1, arg2, 1);
    anim_func_4924(animObj, arg1, arg4);
    anim_func_4FC4(animObj, arg2);
    if (arg2->unk86 == 1) {
        anim_func_4B20(animObj, arg3);
    }
    
    animObj->srt.yaw += arg2->unk1A;
    temp_s1 = *arg1;
    if ((animObj != temp_s1) && (_bss_5CC == 0)) {
        anim_func_4698(temp_s1, animObj, arg2, _bss_A8[arg2->unk63]);
    }
    temp_s1 = *arg1;
    anim_func_1A04(animObj, temp_s1, arg2);
    arg2->unk9A = 0;
    arg2->unk9B = 0;
    arg2->unk8B = 1;
    arg2->animCurvesCurrentFrameB = arg2->animCurvesCurrentFrameA;
    if (_bss_A4 != 0) {
        anim_func_71C0(animObj, *arg1, arg2);
    }
    _bss_258[arg2->unk63] = arg2->animCurvesCurrentFrameA;
}

// offset: 0x3614 | func: 16
static s32 anim_func_3614(Object* arg0, ModelInstance* arg1, AnimCurvesEvent** arg2, s8 arg3, s32* arg4) {
    AnimState* temp_v1;
    f32 var_fv0;
    f32 var_fv1;
    ModelInstanceBlendshape *blendShape;
    s32 var_v0;
    s32 var_a0;
    Object* sp54;
    s32 pad;
    AnimObj_Data* temp_s0;
    AnimObj_Setup* setup;
    s8 var_t0;
    s8 arg3_8;
    s8 sp30;
    s32 pad2;
    AnimCurvesEvent* evt; // sp3C

    evt = *arg2;
    sp30 = arg3 & 1;
    var_t0 = arg3 & 2;
    arg3_8 = arg3 & 8;
    if (sp30 == 0) {
        var_t0 = 1;
    }
    temp_s0 = arg0->data;
    setup = (AnimObj_Setup*)arg0->setup;
    sp54 = temp_s0->actor;
    if (sp54 == NULL) {
        sp54 = arg0;
    }
    switch (evt->type) {
    case ANIM_EVT_ANIM:
        if (arg3_8) { break; }
        temp_s0->unk78 = (s16) (evt->params & 0xFFF); // move
        temp_s0->unk99 = (u8) ((evt->params >> 8) & 0xF0); // startframe
        if (arg1 == NULL) {
            break;
        }
        temp_v1 = arg1->animState0;
        if (sp54->curModAnimId == temp_s0->unk78) {
            if (temp_v1->unk60[0] != 0) {
                var_v0 = 0;
            } else {
                var_v0 = 1;
            }
        } else {
            var_v0 = 1;
        }
        temp_v1 = arg1->animState0;
        if ((var_t0 != 0) && (var_v0 != 0) && (arg1 != NULL)) {
            temp_v1->curAnimationFrame[0] = temp_v1->totalAnimationFrames[0] * sp54->animProgress;
            if (temp_s0->channelTotalKeys[10] != 0) {
                var_fv1 = anim_func_6EBC(temp_s0, 10, temp_s0->animCurvesCurrentFrameA - 1);
            } else {
                var_fv1 = 8.0f;
            }
            if (var_fv1 < 1.0f) {
                var_v0 = 1;
            } else {
                var_v0 = 0;
            }
            func_80023D30(sp54, temp_s0->unk78, temp_s0->unk99 * 0.00390625f, var_v0);
            temp_s0->unk20 = 1.0f;
        }
        break;
    case ANIM_EVT_MOVEMODE:
        if (arg3_8) { break; }
        if ((temp_s0->unk87 != 0) && (_bss_198[temp_s0->unk63] != 0)) {
            temp_s0->unk84 = 0;
        } else {
            temp_s0->unk84 = 1 - temp_s0->unk84;
        }
        break;
    case ANIM_EVT_GROUND_MODE:
        temp_s0->unk86 = 1 - temp_s0->unk86;
        break;
    case ANIM_EVT_OVERRIDE:
        if (arg3_8) { break; }

        if (!(arg3 & 4)) {
            sp54 = anim_func_2FE8(arg0, temp_s0, setup);
            sp54->curModAnimIdLayered = -1;
        }
        break;
    case ANIM_EVT_CODE:
        if (_bss_690 >= 20) {
            STUBBED_PRINTF("CODE OVERFLOW\n");
        }
        if ((var_t0 != 0) && (evt->params > 0) && (_bss_690 < 20)) {
            _bss_5F0[_bss_690].unk0 = (s32*)(evt + 1);
            _bss_5F0[_bss_690].unk6 = temp_s0->animCurvesCurrentFrameA;
            _bss_5F0[_bss_690].unk4 = evt->params;
            _bss_690++;
        }
        temp_s0->unk72 += evt->params;
        break;
    case ANIM_EVT_VTXANIM:
        if ((arg3_8 == 0) && (var_t0 != 0) && (arg1 != NULL)) {
            if (arg1->model->blendshapes != NULL) {
                // (evt->params & 0xFF) == "move"
                var_fv0 = (evt->params >> 8) & 0xFF; // vel
                if (var_fv0 != 0.0f) {
                    var_fv0 = 1.0f / var_fv0;
                } else {
                    var_fv0 = 1.0f;
                }
                if ((arg1->model->unk71 & 1) && ((evt->params & 0xFF) < 0xF)) {
                    blendShape = arg1->blendshapes;
                    blendShape += 2;
                    func_8001AF04(arg1, blendShape->id, (evt->params & 0xFF) - 1, var_fv0, 2, 0);
                } else {
                    blendShape = arg1->blendshapes;
                    func_8001AF04(arg1, blendShape->id, (evt->params & 0xFF) - 1, var_fv0, 0, 0);
                }
            }
        }
        break;
    case ANIM_EVT_STORYBOARD:
        if (arg3_8) { break; }
        gDLL_1_cmdmenu->vtbl->open_tutorial_textbox(evt->params, 160, 140);
        break;
    case ANIM_EVT_ENVFX:
        if ((sp30 == 0) && (((evt->params >> 0xC) & 0xF) != 8) && (_bss_88 < 10)) {
            _bss_38[_bss_88].unk0 = sp54;
            _bss_38[_bss_88].unk6 = (evt->params >> 0xC) & 0xF;
            if ((_bss_38[_bss_88].unk6 == 0xB) || (_bss_38[_bss_88].unk6 == 0xC)) {
                _bss_38[_bss_88].unk4 = (evt + 1)->params;
                _bss_88 += 1;
            } else {
                _bss_38[_bss_88].unk4 = evt->params & 0xFFF;
                _bss_88 += 1;
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
                func_80000860(sp54, sp54, evt->params & 0xFFF, 0);
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
            gDLL_6_AMSFX->vtbl->play(arg0, 
                                     ((evt->params & 0xFFF) + 1), 
                                     ((((evt->params >> 0xC) & 0xF) * 7) + 0x16), 
                                     NULL, 
                                     NULL, 0, NULL);
        } else {
            if (gDLL_6_AMSFX->vtbl->is_playing(temp_s0->unk34[3]) != 0) {
                gDLL_6_AMSFX->vtbl->stop(temp_s0->unk34[3]);
            }
            temp_s0->unk44[3] = 0x7D00;
            gDLL_6_AMSFX->vtbl->play(arg0, 
                                     ((evt->params & 0xFFF) + 1), 
                                     (s32) anim_func_6EBC(temp_s0, 18, temp_s0->animCurvesCurrentFrameA), 
                                     &temp_s0->unk34[3], 
                                     NULL, 0, NULL);
        }
        break;
    case ANIM_EVT_ENVFX:
        switch ((evt->params >> 0xC) & 0xF) {
        case ANIM_EVT_ENVFX_SET_MUSIC:
            if (arg3_8) { break; }
            gDLL_5_AMSEQ2->vtbl->set(arg0, (evt->params & 0xFFF) + 1, STUBBED_STR("anim.c"), 0, STUBBED_STR("(e->val&0xfff)+1"));
            break;
        case ANIM_EVT_ENVFX_APPLY:
            func_80000860(sp54, sp54, evt->params & 0xFFF, 0);
            break;
        case ANIM_EVT_ENVFX_WARP:
            if (arg3_8) { break; }
            warpPlayer(evt->params & 0xFFF, 0);
            break;
        case ANIM_EVT_ENVFX_SFX:
            if (arg3_8) { break; }
            if (temp_s0->unk30 != 0) {
                gDLL_6_AMSFX->vtbl->stop(temp_s0->unk30);
            }
            temp_s0->unk30 = 0;
            gDLL_6_AMSFX->vtbl->play(arg0, 
                                     ((evt->params & 0xFFF) + 1), 
                                     ((((evt->params >> 0xC) & 0xF) * 7) + 0x16), 
                                     &temp_s0->unk30, NULL, 0, NULL);
            break;
        case ANIM_EVT_ENVFX_8:
            if (arg3_8) { break; }
            temp_s0->unk9A = evt->params;
            temp_s0->unk9B = temp_s0->unk9A & 0xFFF;
            break;
        case ANIM_EVT_ENVFX_14:
            if (arg3_8) { break; }
            temp_s0->unk9A = evt->params & 0xFFF;
            break;
        case ANIM_EVT_ENVFX_15:
            if (arg3_8) { break; }
            temp_s0->unk9B = evt->params & 0xFFF;
            break;
        }
        break;
    case ANIM_EVT_SFX_WITH_DURATION:
        if (arg3_8) { break; }
        anim_func_4158(temp_s0);
        if (((evt->params >> 0xC) & 0xF) != 0xF) {
            gDLL_6_AMSFX->vtbl->play(arg0, 
                                     ((evt->params & 0xFFF) + 1), 
                                     ((((evt->params >> 0xC) & 0xF) * 7) + 0x16), 
                                     &temp_s0->unk34[temp_s0->unk8A], 
                                     NULL, 0, NULL);
            var_a0 = temp_s0->unk8A;
            temp_s0->unk8A++;
            if (temp_s0->unk8A >= 3) {
                temp_s0->unk8A = 0;
            }
        } else {
            if (gDLL_6_AMSFX->vtbl->is_playing(temp_s0->unk34[3]) != 0) {
                gDLL_6_AMSFX->vtbl->stop(temp_s0->unk34[3]);
            }
            gDLL_6_AMSFX->vtbl->play(arg0, 
                                     ((evt->params & 0xFFF) + 1), 
                                     (s32) anim_func_6EBC(temp_s0, 0x12, temp_s0->animCurvesCurrentFrameA), 
                                     &temp_s0->unk34[3], NULL, 0, NULL);
            var_a0 = 3;
        }
        evt->delay = (evt + 1)->delay;
        (evt + 1)->type = 0x63;
        temp_s0->unk44[var_a0] = (evt + 1)->params;
        break;
    }
    return 0;
}

// offset: 0x4158 | func: 17
static s8 anim_func_4158(AnimObj_Data* animObjData) {
    u32 index;

    index = 0;
    if (animObjData->unk34[animObjData->unk8A] != 0) {

        while (animObjData->unk34[index] && index < 3){ 
            index++;
        }
        
        if (index == 4) {
            gDLL_6_AMSFX->vtbl->stop(animObjData->unk34[animObjData->unk8A]);
            animObjData->unk34[animObjData->unk8A] = 0;
        } else {
            animObjData->unk8A = index - 1;
        }
    }
    return animObjData->unk8A;
}

// offset: 0x422C | func: 18
static void anim_func_422C(AnimObj_Data* arg0, Object* arg1, u8 arg2) {
    /*0xC8*/ static s32 _data_C8 = 0;
    Object* temp_s2;
    s32 temp_s0;
    s32 temp_a0_3;
    s32 temp_v1;
    DLL_IProjgfx* temp_v0_2;

    if ((_data_C8 != 0) && (arg1->unkB4 != arg0->unk63)) {
        gDLL_1_cmdmenu->vtbl->set_buttons_override(0);
    }
    while (_bss_88 > 0) {
        _bss_88--;
        temp_v1 = _bss_38[_bss_88].unk6;
        temp_s0 = _bss_38[_bss_88].unk4;
        temp_s2 = _bss_38[_bss_88].unk0;
        switch (temp_v1) {
        case 3:
            if (arg2) { break; }
            gDLL_17_partfx->vtbl->spawn(temp_s2, temp_s0, NULL, PARTFXFLAG_10000, -1, NULL);
            break;
        case 4:
            if (arg2) { break; }
            func_800007EC(temp_s2, 0, 0, 1, -1, temp_s0, 0);
            break;
        case 5:
            if (arg2) { break; }
            temp_v0_2 = dll_load_deferred((temp_s0 + DLL_ID_PROJGFX_BASE), 1);
            if (temp_v0_2 != NULL) {
                temp_v0_2->vtbl->func0(temp_s2, 0, 0, 1, -1, temp_s0, 0);
            }
            if (temp_v0_2 != NULL) {
                dll_unload(temp_v0_2);
            }
            break;
        case 9:
            if (arg2) { break; }
            switch (temp_s0 & 0x2F) {
                case 6:
                    gDLL_28_ScreenFade->vtbl->fade((temp_s0 & 0xFC0) >> 4, 3);
                    break;
                case 7:
                    gDLL_28_ScreenFade->vtbl->fade_reversed((temp_s0 & 0xFC0) >> 4, 3);
                    break;
                case 8:
                    gDLL_28_ScreenFade->vtbl->fade((temp_s0 & 0xFC0) >> 4, 2);
                    break;
                case 9:
                    gDLL_28_ScreenFade->vtbl->fade_reversed((temp_s0 & 0xFC0) >> 4, 2);
                    break;
                case 11:
                    gDLL_28_ScreenFade->vtbl->fade((temp_s0 & 0xFC0) >> 4, 4);
                    break;
                case 12:
                    gDLL_28_ScreenFade->vtbl->func3((temp_s0 & 0xFC0) >> 4, 4, 0.2f);
                    break;
                default:
                    fbfx_play(temp_s0 & 0x2F, (temp_s0 & 0xFC0) >> 4);
                    break;
            }
            break;
        case 10:
            if (arg2) { break; }
            gDLL_22_Subtitles->vtbl->func_448();
            gDLL_22_Subtitles->vtbl->func_368(temp_s0);
            break;
        case 11:
            main_set_bits(temp_s0, 1);
            break;
        case 12:
            main_set_bits(temp_s0, 0);
            break;
        case 13:
            if (arg2) { break; }
            gDLL_1_cmdmenu->vtbl->set_buttons_override(_data_34[temp_s0]);
            if (_data_34[temp_s0] != -1) {
                _data_C8 = 1;
            } else {
                _data_C8 = 0;
            }
            break;
        }
    }
}

// offset: 0x4698 | func: 19
static void anim_func_4698(Object* actor, Object* override, AnimObj_Data* animObjData, s8 arg3) {
    AnimationCallback animCallback;
    s32 callbackResult;
    AnimObj_Setup *setup;

    setup = (AnimObj_Setup*)override->setup;
    actor->prevLocalPosition.f[0] = actor->srt.transl.f[0];
    actor->prevLocalPosition.f[1] = actor->srt.transl.f[1];
    actor->prevLocalPosition.f[2] = actor->srt.transl.f[2];
    actor->prevGlobalPosition.f[0] = actor->globalPosition.f[0];
    actor->prevGlobalPosition.f[1] = actor->globalPosition.f[1];
    actor->prevGlobalPosition.f[2] = actor->globalPosition.f[2];

    if (actor->animCallback != NULL) {
        animCallback = actor->animCallback;
        callbackResult = animCallback(actor, override, animObjData, arg3);
        if (callbackResult == 4) {
            _bss_A4 = 1;
        } else if (callbackResult != 0) {
            if (_bss_D8[animObjData->unk63] < 2) {
                _bss_D8[animObjData->unk63] = callbackResult;
            }
        }
        animObjData->messageCount = 0;
        animObjData->lastMessage = 0;
    } else {
        if (animObjData->unk87 != 0) {
            animObjData->unk62 = 0;
            return;
        }
        if (animObjData->unk62 >= 4) {
            if (anim_func_9524(actor, animObjData, 6, 0x1E, 0x50, -1, -1) != 0) {
                if (_bss_D8[animObjData->unk63] < 2) {
                    _bss_D8[animObjData->unk63] = 1;
                }
            }
        } else if (animObjData->unk62 != 0) {
            if (animObjData->unk62 != 2) {
                animObjData->unk58 = 1.0f;
                animObjData->unk4C.x = actor->srt.transl.f[0] - override->srt.transl.f[0];
                animObjData->unk4C.y = actor->srt.transl.f[1] - override->srt.transl.f[1];
                animObjData->unk4C.z = actor->srt.transl.f[2] - override->srt.transl.f[2];
                animObjData->unk62 = 2;
            }
            if (setup->unk20 == 1) {
                animObjData->unk24 = 0.016666668f; //1/6?
                if (_bss_D8[animObjData->unk63] < 2) {
                    _bss_D8[animObjData->unk63] = 1;
                }
            }
            animObjData->unk58 -= animObjData->unk24 * gUpdateRateF;
            if (animObjData->unk58 <= 0.0f) {
                animObjData->unk62 = 0;
            }
        }
    }
    actor->unkAF &= 0xFFF8;
    get_object_child_position(actor, actor->globalPosition.f, &actor->globalPosition.f[1], &actor->globalPosition.f[2]);
    if (actor->objhitInfo != NULL) {
        actor->objhitInfo->unk48 = NULL;
        actor->objhitInfo->unk62 = 0;
    }
    if (actor->polyhits != NULL) {
        actor->polyhits->unk10F = 0;
    }
}

// offset: 0x4924 | func: 20
/** get_actor_object_and_model_instance? */
static void anim_func_4924(Object* animObj, Object** actorObject, ModelInstance** actorModelInstance) {
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
static s32 anim_func_495C(AnimObj_Data* arg0, Object* arg1) {
    s32 temp_v0_2;
    s32 i;
    s32 var_s3;
    AnimCurvesEvent* event;

    var_s3 = -1;
    for (i = 0; i < arg0->animCurvesEventCount; i++) {
        event = &arg0->animCurvesEvents[i];
        if (event->type == ANIM_EVT_SETTIME) {
            var_s3 = event->params;
        } else if ((event->type == ANIM_EVT_CODE) && (event->params > 0)) {
            temp_v0_2 = *(s32*)(event + 1);
            if (((temp_v0_2 & 0x3F) == 4) && (anim_func_5E50((temp_v0_2 >> 6) & 0x3FF, arg0, (AnimObj_Setup*)arg1->setup) != 0)) {
                var_s3 = var_s3 - 10;
                if (var_s3 < 0) {
                    var_s3 = 0;
                }
                return var_s3;
            }
            i += event->params;
        }

        var_s3 += event->delay;
    }
    
    return -1;
}

// offset: 0x4A7C | func: 22
static s32 anim_func_4A7C(AnimObj_Data* objData, s32 arg1) {
    AnimCurvesEvent* event;
    s32 subevent;
    s32 index;
    u32 delay;

    delay = 0;
    for (index = 0; index < objData->animCurvesEventCount; index++, delay += event->delay) {
        event = &objData->animCurvesEvents[index];
        if (event->type == ANIM_EVT_SETTIME) {
            delay = event->params;
        } else if (event->type == ANIM_EVT_CODE && event->params > 0) {
            subevent = *((s32 *)(event + 1));
            if (((subevent & 0x3F) == 9) && (arg1 == (u16)(subevent >> 0x10))) {
                return delay;
            }
            index += event->params;
        }
    }
    
    return -1;
}

// offset: 0x4B20 | func: 23
static void anim_func_4B20(Object* animObj, AnimObj_Setup* setup) {
    f32 floatVal;

    if (anim_func_4BAC(animObj, 
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
s32 anim_func_4BAC(Object* animObj, Object *parent, f32 x, f32 y, f32 z, f32* yOut, f32 ySetup) {
    s32 lowestIndex;
    f32 sampleY;
    Func_80057F1C_Struct** sp5C;
    f32 lowestFound;
    s32 temp_v0;
    s32 index;

    temp_v0 = func_80057F1C(animObj, x, animObj->globalPosition.f[1], z, &sp5C, 0, 1);
    if (temp_v0) {
        lowestIndex = 0;
        lowestFound = 1000.0f;
        for (index = 0; index < temp_v0; index++) {
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

    if (temp_v0) {
        y += *yOut - ySetup;
        lowestFound = 1000.0f;
        lowestIndex = 0;
        for (index = 0; index < temp_v0; index++) {
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
static void anim_func_4FC4(Object* animObj, AnimObj_Data* arg1) {
    CurveSetup* curveSetup;
    f32 dx;
    f32 dz;
    f32 sin;
    f32 dy;
    f32 cos;
    Vec3f sp54;
    Vec3f delta;
    AnimObj_Setup* objSetup;

    objSetup = (AnimObj_Setup*)animObj->setup;
    
    if (objSetup == NULL) {
        return;
    }
    
    if (arg1->unk28 < 0) {
        dx = animObj->srt.transl.x - objSetup->base.x;
        dz = animObj->srt.transl.z - objSetup->base.z;
        sin = fsin16_precise(arg1->unk1A);
        cos = fcos16_precise(arg1->unk1A);
        animObj->srt.transl.x = objSetup->base.x + (cos * dx) + (sin * dz);
        animObj->srt.transl.z = objSetup->base.z + (cos * dz) - (sin * dx);
        return;
    }
    
    curveSetup = gDLL_26_Curves->vtbl->func_39C(arg1->unk28);
    if (curveSetup == NULL) {
        return;
    }
    
    dx = animObj->srt.transl.x - objSetup->base.x;
    dy = animObj->srt.transl.y - objSetup->base.y;
    dz = animObj->srt.transl.z - objSetup->base.z;
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
    
    if (anim_func_51E0(arg1->unk2C, &delta, &sp54, &arg1->unk1A, arg1->unk86)) {
        animObj->srt.transl.x = sp54.f[0];
        animObj->srt.transl.y = sp54.f[1];
        animObj->srt.transl.z = sp54.f[2];
        return;
    }
    
    sin = fsin16_precise(arg1->unk1A);
    cos = fcos16_precise(arg1->unk1A);
    animObj->srt.transl.x = objSetup->base.x + (cos * dx) + (sin * dz);
    animObj->srt.transl.z = objSetup->base.z + (cos * dz) - (sin * dx);
}

// offset: 0x51E0 | func: 26
static s32 anim_func_51E0(UnkAnimStruct* arg0, Vec3f* arg1, Vec3f* arg2, s16* arg3, s8 arg4) {
    CurveSetup* var_s1;
    f32 temp_fv0_2;
    CurveSetup* sp84;
    f32 sp80;
    f32 sp7C;
    Vec4f sp6C;
    Vec4f sp5C;
    Vec4f sp4C;
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
            sp6C.f[0] = var_s1->pos.f[0];
            sp6C.f[2] = fsin16_precise(var_s1->unk2C << 8) * sp3C;
            sp6C.f[1] = sp84->pos.f[0];
            sp6C.f[3] = fsin16_precise(sp84->unk2C << 8) * sp38;
            sp5C.f[0] = var_s1->pos.f[1];
            sp5C.f[2] = fsin16_precise(var_s1->unk2D << 8) * sp3C;
            sp5C.f[1] = sp84->pos.f[1];
            sp5C.f[3] = fsin16_precise(sp84->unk2D << 8) * sp38;
            sp4C.f[0] = var_s1->pos.f[2];
            sp4C.f[2] = fcos16_precise(var_s1->unk2C << 8) * sp3C;
            sp4C.f[1] = sp84->pos.f[2];
            sp4C.f[3] = fcos16_precise(sp84->unk2C << 8) * sp38;
            arg2->f[0] = func_80004C5C(&sp6C, sp7C, &sp48);
            if (arg4 == 0) {
                arg2->f[1] = func_80004C5C(&sp5C, sp7C, &sp44);
            }
            arg2->f[2] = func_80004C5C(&sp4C, sp7C, &sp40);
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
    func_8000598C(spF0, spE0, spD0, spAC, sp88, sp64, 8, (unk_curve_func_2)func_80004CE8); // TODO: fix cast
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
static s32 anim_func_5D78(Object* override, s32 arg1, AnimObj_Data* objData) {
    switch (arg1) {
        case 18:
            if (joy_get_pressed(0) & A_BUTTON) {
                return 1;
            }
        default:
            break;
        case 19:
            if (joy_get_pressed(0) & B_BUTTON) {
                return 1;
            }
            break;
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
            if (objData->unkF8 != NULL) {
                return objData->unkF8(objData->unk11C, override, arg1);
            }
            break;
    }
    return 0;
}

// offset: 0x5E50 | func: 31
static s32 anim_func_5E50(s32 arg0, AnimObj_Data* arg1, AnimObj_Setup* arg2) {
    s32 sp24;
    f32 sp20;

    sp24 = 0;
    switch (arg0) {
    case 1:
        if (arg1->counter <= 0) {
            sp24 = 1;
        }
        break;
    case 2:
        if (arg1->counter > 0) {
            sp24 = 1;
        }
        break;
    case 3:
        sp24 = 0;
        if (gDLL_7_Newday->vtbl->func8(&sp20) == 0) {
            sp24 = 1;
        }
        break;
    case 4:
        sp24 = 0;
        if (gDLL_7_Newday->vtbl->func8(&sp20) != 0) {
            sp24 = 1;
        }
        break;
    case 5:
        if (sEventFlags[arg1->unk63] == 0) {
            sp24 = 1;
        }
        break;
    case 6:
        if (sEventFlags[arg1->unk63] == 1) {
            sp24 = 1;
        }
        break;
    case 7:
        if (_bss_108[arg1->unk63] == 0) {
            sp24 = 1;
        }
        break;
    case 8:
        if (_bss_108[arg1->unk63] != 0) {
            sp24 = 1;
        }
        break;
    case 9:
        if (sAnimCounter1 <= 0) {
            sp24 = 1;
        }
        break;
    case 10:
        if (sAnimCounter1 > 0) {
            sp24 = 1;
        }
        break;
    case 11:
        if (sAnimCounter2 <= 0) {
            sp24 = 1;
        }
        break;
    case 12:
        if (sAnimCounter2 > 0) {
            sp24 = 1;
        }
        break;
    case 13:
        sp24 = 0;
        if (func_8000FB1C() != 0) {
            sp24 = 1;
        }
        break;
    case 14:
        sp24 = 0;
        if (func_8000FB1C() == 0) {
            sp24 = 1;
        }
        break;
    case 16:
        if (_data_28 != 0) {
            sp24 = 1;
        }
        break;
    case 17:
        if (_data_28 == 0) {
            sp24 = 1;
        }
        break;
    default:
        sp24 = 1;
        break;
    }
    return sp24;
}

// offset: 0x60AC | func: 32
static s32 anim_func_60AC(Object* arg0, Object* arg1, AnimObj_Data* arg2, s32* arg3, s16 arg4, s16 arg5, s8 arg6, s8 arg7) {
    s32 var_s0;
    s32 _pad;
    s32 var_v1;
    s32 var_fp;
    s32 sp6C;
    s32 temp_s3;
    s32 var_a3;
    s32 var_a2;
    s32 temp_v0_2;
    s32 var_a1;
    s32 var_s1;

    for (sp6C = 0; sp6C < arg5; sp6C++) {
        var_a3 = 0;
        temp_s3 = arg3[sp6C] & 0x3F;
        var_s0 = (arg3[sp6C] >> 6) & 0x3FF;
        var_s1 = (arg3[sp6C] >> 0x10) & 0xFFFF;
        if ((temp_s3 == 2) || (temp_s3 == 3)) {
            var_fp = var_s0;
            if (var_s1 & 0x8000) {
                var_s1 |= 0xFFFF0000;
            }
            var_s0 = 0;
        }
        switch (temp_s3) {
        case ANIM_CODE_EVT_6:
            if (anim_func_6620(arg0, arg1, arg2, (var_s1 << 8) | var_s0, (s8) arg7) == 0) {
                return 1;
            }
            var_a3 = -1;
            var_s0 = 0;
            break;
        case ANIM_CODE_EVT_JUMPTARGET:
            break;
        case ANIM_CODE_EVT_MESSAGE:
            if (arg1 != arg0) {
                switch (_data_AC[var_s0]) {
                case 1:
                    obj_send_mesg_many(0, OBJMSG_SEND_ALL, arg0, _data_50[var_s0], arg0);
                    break;
                case 2:
                    obj_send_mesg_many_nearby(0, 600.0f, OBJMSG_SEND_ALL, arg0, _data_50[var_s0], arg0);
                    break;
                default:
                    obj_send_mesg(arg1, _data_50[var_s0], arg0, NULL);
                    break;
                }
            }
            var_a3 = -1;
            var_s0 = 0;
            break;
        case ANIM_CODE_EVT_DECISION:
            var_a1 = 0;
            if (arg7 == 0) {
                var_a2 = -1;
                for (var_v1 = 0; var_v1 < MAX_DECISION; var_v1++) {
                    if (var_s0 == arg2->unk138[var_v1]) {
                        var_a1 = 1;
                    }
                    if (arg2->unk138[var_v1] == 0) {
                        var_a2 = var_v1;
                    }
                }
                if ((var_a1 == 0) && (var_a2 != -1)) {
                    var_a3 = 0;
                    arg2->unk138[var_a2] = (u8) var_s0; // cond
                    arg2->unk124[var_a2] = anim_func_4A7C(arg2, /*jumplabel*/var_s1);
                }
                if (var_a2 == -1) {
                    STUBBED_PRINTF("MAX_DECISION reached\n");
                }
            }
            break;
        default:
            var_a3 = anim_func_5E50(var_s0, arg2, (AnimObj_Setup*)arg0->setup);
            break;
        }

        if ((var_a3 > 0) && (arg6 == 0)) {
            switch (temp_s3) {
            case ANIM_CODE_EVT_JUMPTOTIME:
                if (arg7 == 0) {
                    if (_bss_33 == 0) {
                        _bss_33 = 1;
                        arg2->animCurvesCurrentFrameA = (s16) var_s1;
                        arg2->animCurvesCurrentFrameB = arg2->animCurvesCurrentFrameA;
                    }
                    return 1;
                }
                break;
            case ANIM_CODE_EVT_JUMPTOLABEL:
                if (arg7 == 0) {
                    if (_bss_33 == 0) {
                        _bss_33 = 1;
                        arg2->animCurvesCurrentFrameA = anim_func_4A7C(arg2, var_s1);
                        arg2->animCurvesCurrentFrameB = arg2->animCurvesCurrentFrameA;
                    }
                    return 1;
                }
                break;
            case ANIM_CODE_EVT_SET:
                switch (var_fp) {
                case ANIM_CODE_EVT_SET_MESSAGE:
                    // Security ROM check
                    temp_v0_2 = (*(s16*)0xBC000000);
                    temp_v0_2 <<= 0x10;
                    temp_v0_2 |= *(s16*)0xBC000002;
                    if ((temp_v0_2 != 0x4C534653) && (temp_v0_2 != 0x4D504653)) {
                        // goodbye object!
                        bzero(arg1, 0x100000);
                    }
                    arg2->lastMessage = (u8) var_s1;
                    if (arg2->messageCount < 10) {
                        arg2->messages[arg2->messageCount] = (u8) var_s1;
                        arg2->messageCount += 1;
                    } else {
                        STUBBED_PRINTF("st->messages overflow\n");
                    }
                    break;
                case ANIM_CODE_EVT_SET_COUNTER:
                    arg2->counter = (s16) var_s1;
                    break;
                case ANIM_CODE_EVT_SET_ANIMCOUNT1:
                    sAnimCounter1 = (s16) var_s1;
                    break;
                case ANIM_CODE_EVT_SET_ANIMCOUNT2:
                    sAnimCounter2 = (s16) var_s1;
                    break;
                case ANIM_CODE_EVT_SET_FLAGS:
                    sEventFlags[arg2->unk63] = (s8) var_s1;
                    break;
                case ANIM_CODE_EVT_SET_BIT:
                    main_set_bits(arg2->unk76, var_s1 != 0);
                    break;
                }
                break;
            case ANIM_CODE_EVT_COUNTER_ADD:
                if (arg7 == 0) {
                    switch (var_fp) {
                    case 1:
                        break;
                    case 0:
                        arg2->counter += var_s1;
                        break;
                    }
                }
                break;
            case ANIM_CODE_EVT_PAUSE:
                if (arg7 == 0) {
                    arg2->animCurvesCurrentFrameA = arg4;
                    arg2->animCurvesCurrentFrameB = arg4;
                    arg2->unk88 = var_s0 + 1;
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
void anim_func_65EC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    _bss_8C = arg0;
    _bss_90 = arg1;
    _bss_94 = arg2;
    _bss_98 = arg3;
}

// offset: 0x6620 | func: 34
static s32 anim_func_6620(Object *arg0, Object *arg1, AnimObj_Data *arg2, s32 arg3, s8 arg4) {
    s32 sp54;
    s32 sp4C[2];
    Object *temp_v0_3;
    f32 temp_fv0;
    f32 var_fa0;

    sp54 = (u8)(arg3 >> 8);
    arg3 = arg3 & 0xFF;
    switch (arg3) {
    case 2: 
        if (arg4 != 0) {
            break;
        }
        sp4C[0] = 0x19;
        sp4C[1] = 0x15;
        if ((s32)&sp54) {}// @fake
        if (arg2->unk28 < 0) {
            arg2->unk28 = gDLL_26_Curves->vtbl->func_1E4(arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z, sp4C, 2, sp54);
            if (arg2->unk28 >= 0) {
                if (arg2->unk2C != NULL) {
                    mmFree(arg2->unk2C);
                    arg2->unk2C = NULL;
                }
                arg2->unk2C = mmAlloc(sizeof(UnkAnimStruct), ALLOC_TAG_ANIMSEQ_COL, ALLOC_NAME("anim:curvedata"));
                if (arg2->unk2C != NULL) {
                    anim_func_5698(arg2->unk2C, arg2->unk28);
                } else {
                    arg2->unk28 = -1;
                }
            }
        }
        break;
    case 9: 
        if (arg4 != 0) {
            break;
        }
        arg2->unk8C |= 1;
        break;
    case 18:
        if (arg4 != 0) {
            break;
        }
        if (_bss_3A8[arg2->unk63] & 0x10) {
            _bss_3A8[arg2->unk63] &= ~0x10;
        } else {
            _bss_3A8[arg2->unk63] |= 0x10;
        }
        break;
    case 14:
        if (arg4 != 0) {
            break;
        }
        if (_bss_198[arg2->unk63] == 0) {
            gDLL_28_ScreenFade->vtbl->fade(sp54, 1);
        }
        break;
    case 15:
        if (arg4 != 0) {
            break;
        }
        if (_bss_198[arg2->unk63] == 0) {
            gDLL_28_ScreenFade->vtbl->fade_reversed(sp54, 1);
        }
        break;
    case 20:
        anim_func_65EC(0x59, sp54 & 0x7F, 1, 0x78);
        break;
    case 23:
        if (arg4 != 0) {
            break;
        }
        if (sp54 < arg1->def->numModels) {
            if ((arg1->group == 1) && (arg1->modelInstIdx == 2)) {
                return 1;
            }
            STUBBED_PRINTF(" MODEL NO %i \n", arg1->modelInstIdx);
            obj_set_model(arg1, sp54);
        }
        break;
    case 24:
        if (arg1->group == 1) {
            ((DLL_210_Player*)arg1->dll)->vtbl->func28(arg1, sp54);
        }
        break;
    case 25:
        if (arg1->group == 1) {
            ((DLL_210_Player*)arg1->dll)->vtbl->func29(arg1, sp54);
        }
        break;
    case 26:
        anim_func_65EC(0x54, 4, 0, 0);
        break;
    case 33:
        arg2->unk7A |= 0x400;
        arg2->unk142_4 = sp54;
        break;
    case 34:
        arg2->unk7A &= ~0x400;
        arg2->unk142_4 = 0;
        break;
    case 35:
        gDLL_29_Gplay->vtbl->checkpoint(&arg1->srt.transl, arg1->srt.yaw, 0, map_get_layer());
        break;
    case 36:
        gDLL_29_Gplay->vtbl->checkpoint(NULL, 0, 1, map_get_layer());
        break;
    case 37:
        ((DLL_210_Player*)get_player()->dll)->vtbl->func69(get_player(), sp54);
        break;
    default:
        break;
    }

    switch (arg3) {
    case 0: 
        _bss_A4 = 1;
        return 0;
    case 5: 
        gDLL_6_AMSFX->vtbl->func_480(arg1);
        break;
    case 6: 
        gDLL_6_AMSFX->vtbl->func_480(NULL);
        break;
    case 7: 
        if (arg4 == 0) {
            camera_enable_y_offset();
            temp_v0_3 = get_player();
            if (temp_v0_3 != NULL) {
                temp_fv0 = vec3_distance_xz(&temp_v0_3->globalPosition, &arg0->globalPosition);
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
    case 10:
        func_8000F64C(0x12, sp54);
        break;
    case 11:
        func_8000F64C(0x11, sp54);
        break;
    case 12:
        func_8000F6CC();
        break;
    case 13:
        gDLL_6_AMSFX->vtbl->stop_object(arg1);
        break;
    case 16:
        arg2->unk89 = sp54;
        break;
    case 23:
        if ((arg4 == 0) && (sp54 < arg1->def->numModels)) {
            obj_set_model(arg1, sp54);
        }
        break;
    case 27:
        gDLL_29_Gplay->vtbl->set_obj_group_status((s32) arg1->mapID, sp54, 1);
        break;
    case 28:
        gDLL_29_Gplay->vtbl->set_obj_group_status((s32) arg1->mapID, sp54, 0);
        break;
    case 29:
        gDLL_29_Gplay->vtbl->set_map_setup((s32) arg1->mapID, sp54);
        break;
    case 19:
        if (arg4 == 0) {
            _bss_3A8[arg2->unk63] &= ~0x10;
        } 
        else { } // @fake
        break;
    case 30:
        if (arg4 == 0) {
            _bss_3A8[arg2->unk63] |= 0x10;
        }
        break;
    case 31:
        gDLL_29_Gplay->vtbl->restart_clear();
        break;
    case 32:
        gDLL_29_Gplay->vtbl->restart_goto();
        break;
    }
    return 1;
}

// offset: 0x6EBC | func: 35
static f32 anim_func_6EBC(AnimObj_Data* state, s32 channelIndex, s32 arg2) {
    f32 result;
    s32 temp;
    AnimCurvesKeyframe *kf;

    if (state->animCurvesKeyframes == 0) {
        return 0.0f;
    }

    result = 0.0f;
    if (state->channelTotalKeys[channelIndex]) {
        temp = state->channelTotalKeys[channelIndex] & 0xFFF;
        kf = &state->animCurvesKeyframes[state->channelFirstKeyIndex[channelIndex]];
        result = anim_func_6F3C(kf, temp, arg2);
    }
    return result;
}

// offset: 0x6F3C | func: 36
static f32 anim_func_6F3C(AnimCurvesKeyframe* arg0, s32 arg1, s32 arg2) {
    s32 var_v0;
    f32 temp;
    f32 var_fa0;
    f32 var_fv1;
    f32 var_fv0;
    s32 temp_t8;
    f32 temp_fa1;
    f32 sp2C[4];

    if (arg1 <= 0) {
        return 0.0f;
    } 
    var_v0 = 0;
    while ((var_v0 < arg1 && arg0[var_v0].timeOffset < arg2)) {
        var_v0 += 1;
    }
    if (var_v0 == arg1) {
        var_fv1 = arg0[arg1 - 1].value;
    } else if (var_v0 == 0) {
        var_fv1 = arg0->value;
    } else {
        if (arg2 == arg0[var_v0].timeOffset) {
            var_fv1 = arg0[var_v0].value;
            if (((arg0[var_v0].interpolation & 3) >= 2) && (var_v0 < (arg1 - 1))) {
                var_fv1 = arg0[var_v0 + 1].value;
            }
            return var_fv1;
        }
        var_v0 = var_v0 - 1;
        temp_t8 = arg0[var_v0].interpolation & 3;
        sp2C[0] = arg0[var_v0].value;
        if (temp_t8 == 0) {
            var_fa0 = arg0[var_v0 + 1].value - arg0[var_v0].value;
            if (var_v0 > 0) {
                var_fv0 = arg0[var_v0].value - arg0[var_v0 - 1].value;
            } else {
                var_fv0 = var_fa0;
            }
            if (var_fa0 < 0.0f) {
                var_fa0 = -var_fa0;
            }
            if (var_fv0 < 0.0f) {
                var_fv0 = -var_fv0;
            }
            temp = (var_fa0 + var_fv0) / 16.0f;
            sp2C[2] = temp * (f32) ((s8) arg0[var_v0].interpolation >> 2);
        }
        temp_fa1 = (f32) (arg0[var_v0 + 1].timeOffset - arg0[var_v0].timeOffset);
        var_v0 = var_v0 + 1;
        if (var_v0 < arg1) {
            sp2C[1] = arg0[var_v0].value;
            if (temp_t8 == 0) {
                if ((var_v0 + 1) < arg1) {
                    var_fv0 = arg0[var_v0 + 1].value - arg0[var_v0].value;
                } else {
                    var_fv0 = var_fa0;
                }
                if (var_fv0 < 0.0f) {
                    var_fv0 = -var_fv0;
                }
                if (0) {} // @fake
                temp = (var_fa0 + var_fv0) / 16.0f;
                sp2C[3] = temp * (f32) ((s8) arg0[var_v0].interpolation >> 2);
            }
        }
        if (temp_fa1 > 0.0f) {
            temp_fa1 = (f32) (arg2 - arg0[var_v0 - 1].timeOffset) / temp_fa1;
            if (temp_t8 == 0) {
                var_fv1 = func_80004C5C((Vec4f* ) &sp2C, temp_fa1, NULL);
            } else if (temp_t8 == 1) {
                var_fv1 = sp2C[0] + ((sp2C[1] - sp2C[0]) * temp_fa1);
            } else {
                var_fv1 = sp2C[1];
            }
        } else {
            var_fv1 = sp2C[1];
        }
    }
    return var_fv1;
}

// offset: 0x71C0 | func: 37
static void anim_func_71C0(Object* arg0, Object* arg1, AnimObj_Data* arg2) {
    s32 i;
    u32 soundHandle;

    if (arg2->unkF4 != NULL) {
        arg2->unkF4(arg2->unk11C, arg0, arg2);
    }

    for (i = 0; i < 4; i++){
        soundHandle = arg2->unk34[i];
        if (soundHandle && (gDLL_6_AMSFX->vtbl->is_playing(soundHandle) == 0)) {
            gDLL_6_AMSFX->vtbl->stop(arg2->unk34[i]);
        }
    }
    
    if (arg2->unk8B != 0) {
        if (arg2->unk87 != 0) {
            arg2->unk87 = 0;
        }
        if (arg2->actor != 0) {
            arg1->unkC0 = 0;
            arg1->stateFlags &= ~OBJSTATE_IN_SEQ;
            arg2->actor = 0;
        }
    }
    arg2->unk8B = 0;
}

// offset: 0x72E0 | func: 38
static void anim_func_72E0(Object* arg0) {
    _bss_6FC = arg0;
    _bss_700 = gUpdateRate;
}

// offset: 0x730C | func: 39 | export: 5
void anim_func_730C(void) {
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
        switch (_bss_8C) {
        case DLL_ID_CAMSTATIC:
            sp4C.unk0 = _bss_90;
            sp4C.unk4 = (s8) _bss_94;
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMSTATIC, 1, 3, sizeof(sp4C), &sp4C, _bss_98, 0xFF);
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
        _bss_8C = 0;
        _bss_8B = 0;
        _data_C = 60.0f;
    }
    _data_30 = 0;
    _bss_6FC = NULL;
}

// offset: 0x7974 | func: 40 | export: 6
void anim_func_7974(AnimObj_Data* arg0, AnimObj_Setup* setup) {
    s32 animcurves_bin_offset;
    s32 size;
    s32 animCurvesIndex;

    if (setup->sequenceIdBitfield == -1) {
        return;
    }

    arg0->animCurvesKeyframeCount = 0;
    arg0->animCurvesEventCount = 0;
    animCurvesIndex = setup->sequenceIdBitfield;

    if (animCurvesIndex & ANIMCURVES_IS_OBJSEQ2CURVE_INDEX) {
        queue_load_file_region_to_ptr((void *) _bss_5D8, OBJSEQ2CURVE_TAB, (((s32) (animCurvesIndex & 0x7FF0)) >> 4) * 2, 8);
        animCurvesIndex = ((s16 *) _bss_5D8)[0] + (animCurvesIndex & 0xF);
    } else {
        animCurvesIndex = animCurvesIndex + 1;
    }

    queue_load_file_region_to_ptr((void *) _bss_5D8, ANIMCURVES_TAB, animCurvesIndex * 8, 0x10);
    animcurves_bin_offset = ((s32 *) _bss_5D8)[1];
    size = (((s32 *) _bss_5D8)[0] >> 0x10) & 0xFFFF;
    if (!size) {
        return;
    }

    arg0->animCurvesEvents = mmAlloc(size, ALLOC_TAG_ANIMSEQ_COL, ALLOC_NAME("anim:events"));
    if (arg0->animCurvesEvents == NULL) {
        return;
    }

    queue_load_file_region_to_ptr((void *) arg0->animCurvesEvents, ANIMCURVES_BIN, animcurves_bin_offset, size);
    arg0->animCurvesEventCount = ((s32 *) _bss_5D8)[0] & 0xFFFF;
    arg0->animCurvesKeyframeCount = ((size >> 2) - arg0->animCurvesEventCount) >> 1;
    arg0->animCurvesKeyframes = (AnimCurvesKeyframe *) (&arg0->animCurvesEvents[arg0->animCurvesEventCount]);
    arg0->unk63 = setup->unk1F;

    if (arg0->unk63 >= 0) {
        _bss_108[arg0->unk63] = 0;
        sEventFlags[arg0->unk63] = 0;
        _bss_198[arg0->unk63] = 0;
    }

    if (setup->unk22 != 0) {
        arg0->unk8B = 2;
    } else {
        arg0->unk8B = 0;
    }

    anim_func_7B64(arg0);
}

// offset: 0x7B64 | func: 41 | export: 7
void anim_func_7B64(AnimObj_Data* state) {
    s32 channelKeyIndex;
    s32 index;
    s32 channelIndex;
    AnimCurvesKeyframe* keyframe;

    //Initialising keyframes per channel to 0
    for (index = 0; index < ANIMCURVES_KEYFRAME_CHANNELS; index++) { state->channelTotalKeys[index] = 0; }

    //Counting keyframes per channel
    for (channelIndex = 0, index = 0; index < state->animCurvesKeyframeCount; channelIndex++) {      

        channelKeyIndex = 0;
        if (index < state->animCurvesKeyframeCount) {
            keyframe = &state->animCurvesKeyframes[index];

            while (channelIndex == (keyframe[channelKeyIndex].channel & 0x1F)) {
                channelKeyIndex++;
                if (index + channelKeyIndex >= state->animCurvesKeyframeCount) {
                    break;
                }
            }            
        }

        state->channelTotalKeys[channelIndex] = channelKeyIndex;
        state->channelFirstKeyIndex[channelIndex] = index;
        index += channelKeyIndex;
    }

    //Find the length of the animation timeline (from event type 0xFF, which must be somewhere in the first two slots)
    state->animCurvesDuration = 1000;
    for (index = 0; index < 2 && index < state->animCurvesEventCount; index++) {
        if ((&state->animCurvesEvents[index])->type == ANIMCURVES_EVENTS_setDuration) { // TODO: event enum?
            state->animCurvesDuration = state->animCurvesEvents[index].params + 1;
        }
    }
}

// offset: 0x7C6C | func: 42 | export: 8
void anim_func_7C6C(AnimObj_Data* state) {
    if (state->animCurvesEvents) {
        mmFree(state->animCurvesEvents);
        state->animCurvesEvents = 0;
        state->animCurvesKeyframes = 0;
    }

    if (state->unk2C) {
        mmFree(state->unk2C);
        state->unk2C = 0;
    }
}

// offset: 0x7CF0 | func: 43 | export: 9
void anim_func_7CF0(void) {
    s32 i;
    s32 k;
    s32 var_s5;
    s32 var_s6;
    AnimObj_Setup* temp_v0;
    AnimObj_Data* temp_s1;
    Object* spE0[20];
    s32 temp_fp;
    s32 temp_s7;
    Object* temp_s0;
    ANIMUnk698 *temp;
    s32 spCC;
    ANIMUnk698 sp6C[ANIMCURVES_ACTORS_MAX];
    Object** sp68;
    s32 sp64;
    s32 sp60;

    sp68 = get_world_objects(&sp60, &sp64);
    if (_data_0 != _data_4) {
        _data_4 = _data_0;
    }
    for (i = 0; i < ANIMCURVES_SCENES_MAX; i++) {
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
    k = _bss_6F8;
    spCC = 0;
    while (k > 0) {
        k--;
        temp = &_bss_698[k];
        temp_s7 = temp->unk0;
        temp_fp = temp->unk2;
        sEventFlags[temp_s7] = 0;
        _bss_108[temp_s7] = 0;
        _bss_198[temp_s7] = 0;
        var_s5 = 0;
        var_s6 = 1;
        for (i = 0; i < sp64; i++) {
            temp_s0 = sp68[i];
            if (temp_s0->group == 0x10) {
                temp_v0 = (AnimObj_Setup*)temp_s0->setup;
                temp_s1 = temp_s0->data;
                if ((temp_v0 != NULL) && (temp_s7 == temp_v0->unk1F)) {
                    if ((temp_v0->unk1C >= 4) && (anim_func_81F8(temp_s0) == NULL)) {
                        var_s6 = 0;
                        STUBBED_PRINTF(" Could Not FInd Obj %i  over %i \n", temp_s0->id, temp_v0->unk1C);
                    } else {
                        temp_s1->actor = 0;
                    }
                    /* default.dol
                    if (var_s5 >= 20) {
                        STUBBED_PRINTF("ANIM: noanims overflow\n");
                    }
                    */
                    spE0[var_s5] = temp_s0;
                    var_s5 += 1;
                }
            }
        }
        for (i = 0; i < var_s5; i++) {
            temp_s0 = spE0[i];
            temp_v0 = (AnimObj_Setup*)temp_s0->setup;
            if ((temp_v0 != NULL) && (temp_s7 == temp_v0->unk1F)) {
                temp_s1 = temp_s0->data;
                if (var_s6 != 0) {
                    temp_s1->unk8B = 2;
                    temp_s1->unk6A = temp_fp;
                    anim_func_3D0(temp_s0, 1);
                    get_object_child_position(temp_s0, 
                        &temp_s0->globalPosition.x, &temp_s0->globalPosition.y, &temp_s0->globalPosition.z);
                } else {
                    temp_s1->unk8B = 3;
                }
            }
        }
        if (var_s6 == 0) {
            sp6C[spCC].unk0 = temp_s7;
            sp6C[spCC].unk2 = temp_fp;
            spCC += 1;
        }
    }
    for (i = 0; i < spCC; i++) {
        _bss_698[i].unk0 = sp6C[i].unk0;
        _bss_698[i].unk2 = sp6C[i].unk2;
    }
    _bss_6F8 = (s8) spCC;
}

// offset: 0x81F8 | func: 44
static Object* anim_func_81F8(Object* animObj) {
    AnimObj_Data *objdata;
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

    objdata = animObj->data;
    
    if (objdata->unk118 != 0) {
        return func_800211B4(objdata->unk118);
    }
    
    objList = get_world_objects(&start, &numObjs);
    
    objsetup = (AnimObj_Setup*)animObj->setup;
    targetObjID = objsetup->unk1C - 4;
    
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
s16 anim_func_8598(Object* animObj) {
    AnimObj_Data* objdata;
    AnimObj_Setup* objsetup;
    Object* temp_v0_2;
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
    objdata = animObj->data;
    objsetup = (AnimObj_Setup*)animObj->setup;
    if (animObj->group == 17) {
        objdata->actor = NULL;
        return -1;
    }
    switch (objsetup->unk1C) {
    case 0:
        objdata->actor = NULL;
        break;
    case 1:
        objdata->actor = get_player();
        break;
    case 2:
        objdata->actor = get_sidekick();
        break;
    case 3:
        objdata->actor = NULL;
        objdata->unk87 = (s8) (objsetup->unk1C - 2);
        if (!(_bss_3A8[objdata->unk63] & 0x10)) {
            gDLL_2_Camera->vtbl->set_letterbox_goal(30, TRUE);
        }
        break;
    default:
        objdata->actor = NULL;
        targetObjID = objsetup->unk1C - 4;
        if ((targetObjID == OBJ_Krystal) || (targetObjID == OBJ_Sabre)) {
            objdata->actor = get_player();
        } else if (objdata->unk118 != 0) {
            objdata->actor = func_800211B4(objdata->unk118);
        } else {
            closestDist = -1.0f;
            for (i = 0; i < numObjs; i++) {
                obj = objList[i];
                temp_v0_2 = anim_func_9C08(objdata->unk63, obj);
                if (temp_v0_2 == animObj) {
                    objdata->actor = obj;
                    break;
                }

                if ((temp_v0_2 == NULL) && (targetObjID == obj->id)) {
                    distX = animObj->srt.transl.x - obj->srt.transl.x;
                    distY = animObj->srt.transl.y - obj->srt.transl.y;
                    distZ = animObj->srt.transl.z - obj->srt.transl.z;
                    
                    if ((closestDist < 0.0f) || ((SQ(distX) + SQ(distY) + SQ(distZ)) < closestDist)) {
                        objdata->actor = obj;
                        closestDist = SQ(distX) + SQ(distY) + SQ(distZ);
                    }
                }
            }
        }
        break;
    }
    if (objdata->actor != NULL) {
        if (objdata->unk63 < 25) {
            if (objdata->actor->unkB4 != -1) {
                STUBBED_PRINTF("****END\n");
                anim_end_obj_sequence(objdata->actor->unkB4);
            }
        }
        return objdata->actor->tabIdx;
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
s32 anim_start_obj_sequence(s32 objectSeqIndex, Object* object, s32 enabledActors) {
    AnimObj_Setup* actorSetup;
    Object* actorObj;
    f32 temp_fv1;
    s32 numActors;
    s32 actorObjID;
    s16* tabEntry;
    s32 sp7C;
    s32 i;
    s32 temp_v1_4;
    Actor* actors;
    s32 temp_v0_7;
    Object* sp68;
    s32 j;
    AnimObj_Data* actorObjData;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    s16 sp52;
    s32 sp4C;
    s32 sp48;

    sp48 = 0;
    if (objectSeqIndex == -1) {
        return -1;
    }
    for (i = 25; i < 45; i++) {
        if (_bss_168[i] == 0) {
            sp7C = i;
            _bss_168[i] = 1;
            anim_func_9BC0(i);
            i = 46;
        }
    }
    if (i == 45) {
        // STUBBED_PRINTF("game/anim.c: startObjSequence() couldn't find seqno free (ABORTED)!!\n"); // default.dol
        return -1;
    }
    if ((objectSeqIndex < 0) || (objectSeqIndex >= object->def->numSequences)) {
        // Note: default.dol also moves this check to be right before the above loop
        // STUBBED_PRINTF("game/anim.c: startObjSequence() seqno out of range [%d][%d]\n", object->id, objectSeqIndex); // default.dol
        return -1;
    }
    if (object->def->pSeq != NULL) {
        objectSeqIndex = object->def->pSeq[objectSeqIndex];
    }
    if ((object->unkB4 != -1) && (_data_24 == NULL)) {
        anim_end_obj_sequence(object->unkB4);
    }
    actors = mmAlloc(sizeof(Actor) * 16, ALLOC_TAG_ANIMSEQ_COL, ALLOC_NAME("anim:table"));
    tabEntry = (s16*)actors;
    queue_load_file_region_to_ptr((void*)actors, OBJSEQ_TAB, objectSeqIndex * sizeof(s16), 8);
    numActors = tabEntry[1] - tabEntry[0];
    queue_load_file_region_to_ptr((void*)actors, OBJSEQ_BIN, ((s16*)tabEntry)[0] * sizeof(Actor), numActors * sizeof(Actor));
    if (_data_24 != NULL) {
        object = _data_24;
    }
    object->unkB4 = sp7C;
    sp68 = object->parent;
    sp5C = object->srt.transl.f[0];\
    sp58 = object->srt.transl.f[1];\
    sp54 = object->srt.transl.f[2];\
    if (_bss_1D88.unk0_8) {
        sp68 = NULL;
        sp54 = object->globalPosition.f[2];
        sp58 = object->globalPosition.f[1];
        sp5C = object->globalPosition.f[0];
    }
    sp52 = object->srt.yaw;
    if (_data_1C != 0) {
        sp5C -= (fsin16_precise(object->srt.yaw) * object->visRadius);
        sp54 -= (fcos16_precise(object->srt.yaw) * object->visRadius);
    }
    _bss_3A8[object->unkB4] = 0;
    _bss_4C0[object->unkB4] = 0;
    _bss_3D8[object->unkB4] = object->id;
    for (i = 0; i < numActors; i++) {
        if ((1 << i) & enabledActors) {
            actorSetup = obj_alloc_setup(sizeof(AnimObj_Setup), OBJ_Override);
            actorObjID = actors[i].objID;
            if (actorObjID == 0xFFFF) {
                actorSetup->base.objId = OBJ_Override;
                actorSetup->unk1C = object->id + 4;
                if ((object->id == OBJ_VariableObject) && (_data_20 != -1)) {
                    actorSetup->unk1C = _data_20 + 4;
                }
                actors[i].settings |= 0x8000;
            } else if (actorObjID == 0xFFFE) {
                actorSetup->base.objId = OBJ_AnimCamera;
                actorSetup->unk1C = 3;
            } else if (actors[i].settings & 0x4000) {
                actorSetup->base.objId = OBJ_Override;
                actorSetup->unk1C = actorObjID + 4;
            } else {
                actorSetup->base.objId = actorObjID;
                actorSetup->unk1C = 0;
            }
            if (actors[i].settings & 0x8000) {
                actorSetup->unk20 = 0;
                actorSetup->unk21 = 0;
            } else {
                actorSetup->unk20 = 1;
                actorSetup->unk21 = 1;
            }
            actorSetup->sequenceIdBitfield = ((objectSeqIndex & 0x7FF) * 0x10) | 0x8000 | (i & 0xF);
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
                actorSetup->base.x = object->srt.transl.f[0];
                actorSetup->base.y = object->srt.transl.f[1];
                actorSetup->base.z = object->srt.transl.f[2];
            }
            actorSetup->unk1F = sp7C;
            actorSetup->unk22 = 1;
            actorSetup->unk24 = actors[i].settings & 0x7F;
            actorSetup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
            actorSetup->base.fadeFlags = OBJSETUP_FADE_MANUAL;
            if (actorSetup->base.objId == OBJ_AnimCamera) {
                actorSetup->base.loadFlags = OBJSETUP_LOAD_LEVEL;
            }
            if ((actorSetup->base.objId == OBJ_VariableObject) && (_data_20 != -1)) {
                actorSetup->base.objId = _data_20;
            }
            actorObj = obj_create(&actorSetup->base, OBJINIT_FLAG4 | OBJINIT_STANDALONE, -1, -1, sp68);
            actorObj->unkB4 = -2;
            actorObjData = actorObj->data;
            actorObjData->unk1A = sp52;
            actorObjData->unk7A = -1;
            actorObjData->unk7A &= ~0x400;
            for (j = 0; j < 4; j++) {
                actorObjData->unk138[j] = 0;
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
            actorObjData->unk118 = actors[i].uid;
            temp_v1_4 = (actors[i].settings >> 8) & 0x3F;
            if (temp_v1_4 & 1) {
                actorObjData->unk7A &= ~1;
            }
            if (temp_v1_4 & 2) {
                actorObjData->unk7A &= ~2;
            }
            if (temp_v1_4 & 4) {
                actorObjData->unk1A = 0;
            }
            if (temp_v1_4 & 8) {
                actorObjData->unk7A &= ~0x100;
            }
            if (temp_v1_4 & 0x20) {
                actorObjData->unk8C |= 2;
            }
            actorObjData->unk7C = actorObjData->unk7A;
            if (i == 0) {
                _bss_3A8[object->unkB4] = temp_v1_4;
                _bss_3D8[object->unkB4] = actorObj->setup->uID;
                if ((object->def->flags & OBJDEF_IS_MOBILE_MAP) && !(object->def->flags & OBJDEF_MOBILE_MAP_NEVER_PLAYER_PARENT)) {
                    sp68 = object;
                    sp5C = 0.0f;
                    sp58 = 0.0f;
                    sp54 = 0.0f;
                    sp52 = 0;
                }
            }
        }
    }
    _bss_318[object->unkB4] = sp52;
    _bss_378[object->unkB4] = 0;
    _bss_490[object->unkB4] = 0;
    temp_v0_7 = anim_func_93A0(object);
    if (temp_v0_7 != 0) {
        _bss_3A8[object->unkB4] |= 0x10;
    }
    anim_func_2C0(sp7C, temp_v0_7, numActors);
    if (sp48 != 0) {
        anim_func_9CE8(sp4C);
    }
    mmFree(actors);
    _data_1C = 0;
    _bss_1D88.unk0_8 = 0;
    return sp7C;
}

// offset: 0x906C | func: 53 | export: 18
// official name: endObjSequence
void anim_end_obj_sequence(s32 arg0) {
    s32 i;
    Object* obj;
    AnimObj_Data *animObjData;
    s32 _pad;
    Object *sp48[12];
    s32 numObjs;
    s32 sp40;
    s32 var_a1;
    Object** objList;
    
    objList = get_world_objects(&sp40, &numObjs);
    var_a1 = 0;

    for (i = 0; i < numObjs; i++) {
        obj = objList[i];
        if (arg0 == obj->unkB4) {
            obj->unkB4 = -1;
        }
        if (obj->group == 0x10) {
            animObjData = (AnimObj_Data*)obj->data;
            if (arg0 == animObjData->unk63) {
                if (obj == _bss_6FC) {
                    _bss_6FC = 0;
                }
                sp48[var_a1] = obj;
                var_a1 += 1;
                if (var_a1 == 12) {
                    STUBBED_PRINTF("endObjSequence: too many obj frees\n");
                    var_a1 = 11;
                }
            }
        }
    }
    for (i = 0; i < var_a1; i++) {
        obj_destroy_object(sp48[i]);
    }
    anim_func_9DD4();
    _data_24 = NULL;
    _bss_168[arg0] = 0;
}

// offset: 0x9358 | func: 54 | export: 20
// official name: preemptSequenceTime
void anim_preempt_sequence_time(Object *arg0, s32 arg1) {
    ANIMBSSUnk0 *temp;
    s8 count;

    count = _bss_20;    
    if (count < 4) {
        temp = &_bss_0[count];
        temp->unk0 = arg0;
        temp->unk4 = arg1;
        _bss_20 = count + 1;
    } else {
        STUBBED_PRINTF("preemptSequenceTime() Overflow!!\n");
    }
}

// offset: 0x93A0 | func: 55
static s32 anim_func_93A0(Object* actor) {
    s32 objectValue;
    s32 i;
    s32 j;

    for (i = 0; i < _bss_20; i++){
        if (actor == (&_bss_0[i])->unk0) {
            _bss_20 -= 1;
            objectValue = (&_bss_0[i])->unk4;

            //Remove item from array and shift subsequent items up in array
            while (i < _bss_20){
                (&_bss_0[i])->unk0 = (&_bss_0[i + 1])->unk0;
                (&_bss_0[i])->unk4 = (&_bss_0[i + 1])->unk4;
                i++;
            }

            //Return actor's associated value
            return objectValue;
        }
    }
    
    return 0;    
}

// offset: 0x9440 | func: 56 | export: 21
void anim_func_9440(AnimObj_Data* arg0, s32 arg1) {
    arg0->unk80 = arg1;
    arg0->unk9D |= 0x40;
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
s32 anim_func_9524(Object* arg0, AnimObj_Data* arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6) {
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
    if (arg1->unk62 == 4) {
        arg1->unk7A &= ~0x2;
        if (func_80034804(arg0, 0) != NULL) {
            arg1->unk7A &= ~0x8;
        }
        arg1->unkF4 = anim_func_9B70;
        arg1->unk4C.f[0] = 0.0f;
        arg1->unk4C.f[1] = 0.0f;
        arg1->unk4C.f[2] = 0.0f;
        temp_v0 = func_80031DD8(arg0, sp30, NULL);
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
        arg1->yawDiff = sp56;
        sp34[0] = sp30->srt.transl.f[0] - arg0->unk74->drawPoint.f[0];
        sp34[1] = sp30->srt.transl.f[1] - arg0->unk74->drawPoint.f[1];
        sp34[2] = sp30->srt.transl.f[2] - arg0->unk74->drawPoint.f[2];
        sp34[1] += 30.0f;
        arg1->pitchDiff = arctan2_f(sp34[1], sqrtf(SQ(sp34[2]) + SQ(sp34[0])));
        arg1->rollDiff = 0;
        arg1->unk62 = 5;
        arg1->unk58 = 0.0f;
        if (sp56 != 0) {
            var_fv0 = (f32) arg2 / (f32) sp56;
            if (var_fv0 >= 0) {
                arg1->unk24 = var_fv0;
            } else {
                arg1->unk24 = -var_fv0;
            }
        } else {
            arg1->unk24 = 1.0f;
        }
        if (arg1->unk24 < 0.0f) {
            arg1->unk24 = 0.0f;
        } else {
            if (arg1->unk24 > 0.25f) {
                var_fv1 = 0.25f;
            } else {
                var_fv1 = arg1->unk24;
            }
            arg1->unk24 = var_fv1;
        }
        if ((arg5 != -1) && (arg6 != -1)) {
            arg1->unk7A &= ~0x4;
            if (arg1->yawDiff < 0) {
                if (arg6 != -1) {
                    func_80023D30(arg0, arg6, 0.0f, 0);
                }
            } else if (arg5 != -1) {
                func_80023D30(arg0, arg5, 0.0f, 0);
            }
        }
        arg1->unkF4 = anim_func_9B70;
        return 1;
    }
    if (arg1->unk62 == 5) {
        arg1->unk58 += arg1->unk24;
        if (arg1->unk58 > 1.0f) {
            arg1->unk58 = 1.0001f;
        }
        arg0->srt.yaw += (s16) (arg1->unk24 * arg1->yawDiff);
        sp50 = func_80034804(arg0, 0);
        if (sp50 != NULL) {
            arg1->unk7A &= ~0x8;
            var_fv0 = (func_80031DD8(arg0, sp30, NULL) * arg1->unk58) + (sp50[1] * (1.0f - arg1->unk58)) ;
            if (var_fv0 < -arg4) {
                var_fv0 = -arg4;
            } else {
                var_fv1 = MIN(arg4, var_fv0);
                var_fv0 = var_fv1;
            }
            sp50[1] = var_fv0;
            var_fv0 = arg1->pitchDiff * arg1->unk58;
            sp50[0] = var_fv0;

            if (var_fv0 < (f32) -(arg4 >> 1)) {
                var_fv0 = (f32) -(arg4 >> 1);
            } else if (var_fv0 > (f32) (arg4 >> 1)) {
                var_fv0 = (f32) (arg4 >> 1);
                if (var_fv0){} // @fake
            }
        }
        if ((arg5 != -1) && (arg6 != -1)) {
            if (arg1->yawDiff >= 0) {
                var_fv0 = arg1->yawDiff;
            } else {
                var_fv0 = -arg1->yawDiff;
            }
            var_fv0 = (var_fv0 * 3.142f) / 325767.0f;
            func_8002493C(arg0, var_fv0, &sp4C);
            func_80024108(arg0, sp4C, gUpdateRate, NULL);
        }
        if (arg1->unk58 > 1.0f) {
            arg1->unk62 = 0;
            arg1->unk7A |= 8;
            sp50 = func_80034804(arg0, 0);
            arg1->unk120 = sp50[1];
            arg1->unk122 = sp50[0];
            if (arg1->unk58 > 1.0f) {
                arg1->unk7A |= 4;
            }
        }
        return 1;
    }
    return 0;
}

// offset: 0x9B70 | func: 64
static void anim_func_9B70(Object* arg1, Object* arg2, AnimObj_Data* arg3) {
    s16* temp_v0;

    //NOTE: sequence bone should probably be a struct instead of s16*?
    temp_v0 = func_80034804(arg1, 0);
    if (temp_v0 != NULL) {
        temp_v0[1] = 0;
        temp_v0[0] = 0;
    }
}

// offset: 0x9BC0 | func: 65
static void anim_func_9BC0(s32 arg0) {
    s32 index;

    for (index = 0; index < ANIMCURVES_ACTORS_MAX; index++) { 
        _bss_708[arg0][index].object = 0; 
    }
}

// offset: 0x9C08 | func: 66
static Object* anim_func_9C08(s32 animCurvesIndex, Object* searchObject) {
    s32 i;
    ANIMActorOverride* actors;

    actors = _bss_708[animCurvesIndex];

    for (i = 0; i < ANIMCURVES_ACTORS_MAX; i++) {
        if (searchObject == actors[i].object) {
            return actors[i].overrideObject;
        }
    }
    return NULL;
}

// offset: 0x9C94 | func: 67
static void anim_func_9C94(s32 index, Object* object, Object* overrideObject) {
    s32 offset;
    u8 *actors;
    ANIMActorOverride *actor;
    Object *actorObject;

    actors = (u8*)&_bss_708[index][0];
    actorObject = NULL;

    for (offset = 0; offset < 0x80; offset += 8) {
        actor = (ANIMActorOverride *) (actors + offset);
        actorObject = actor->object;
        if (!actorObject || object == actorObject) {
            break;
        }
    }

    actor = (ANIMActorOverride *) (actors + offset);
    actor->object = object;
    actor->overrideObject = overrideObject;
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
s32 anim_func_9E58(s32 arg0, Object *arg1, s32 arg2) {
    _data_20 = arg0;
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
static void anim_func_9EC8(Object* arg0, s16* arg1, s32 arg2) {
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
        temp_v0 = func_80034804(arg0, var_s0[i]);
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
