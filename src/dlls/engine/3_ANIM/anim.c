//May have been stored as "game/anim.c" (based off leftover string at 0x2F1560 in SFA Kiosk's "default.dol")

#include "common.h"

#include "dlls/objects/214_animobj.h"
#include "prevent_bss_reordering.h"

s32* func_800349B0(void);

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

typedef struct {
Vec3f unk0; 
s8 unkC;
} CameraFunc15Unk_unk74;

typedef struct {
    s8 unk0[0x74 - 0];
    CameraFunc15Unk_unk74* unk74; 
} CameraFunc15Unk;

typedef struct {
    s32 unk0;
    s32 unk4;
} AnimBSS0;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} SequenceBoneStructUnk;

typedef struct {
    Object *unk0;
    s32 unk4;
} ANIMBSSUnk0;

typedef struct{
    Object* object;
    Object* overrideObject;
} ANIMActorOverride;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 totalActors; //maybe?
} ANIMUnk698;

/*0x0*/ static const char str_0[] = "Max activates reached\n";
/*0x18*/ static const char str_18[] = "CODE OVERFLOW\n";
/*0x28*/ static const char str_28[] = "MAX_DECISION reached\n";
/*0x40*/ static const char str_40[] = "st->messages overflow\n";
/*0x58*/ static const char str_58[] = " MODEL NO %i \n";
/*0x68*/ static const char str_68[] = " Could Not FInd Obj %i  over %i \n";
/*0x8C*/ static const char str_8C[] = "****END\n";
/*0x98*/ static const char str_98[] = "endObjSequence: too many obj frees\n";
/*0xBC*/ static const char str_BC[] = "preemptSequenceTime() Overflow!!\n";

/*0x0*/ static u32 _data_0 = 0x00000000;
/*0x4*/ static u32 _data_4[] = {
    0x00000000, 0xff000000
};
/*0xC*/ static f32 _data_C[] = {
    60.0, 0.0
};
/*0x14*/ static s16 _data_14[] = {
    0, 0
};
/*0x18*/ static s16 _data_18[] = {
    0, 0
};
/*0x1C*/ static s8 _data_1C[4] = {0,0,0,0};
/*0x20*/ static s32 _data_20 = -1;
/*0x24*/ static Object *_data_24 = 0;
/*0x28*/ static s8 _data_28[] = {
    0, 0, 0, 0
};
/*0x2C*/ static s32 _data_2C = {
    0
};
/*0x30*/ static u32 _data_30[] = {
    0x00000000
};
/*0x34*/ static u32 _data_34[] = {
    0x00008000, 0x00004000, 0x00000002, 0x00000001, 0x00000004, 0x00000008, 0xffffffff
};
/*0x50*/ static u32 _data_50[] = {
    0x00050001, 0x00050002, 0x00050003, 0x00060001, 0x00060002, 0x000a0001, 0x000a0002, 0x000a0003, 
    0x00000008, 0x00000009, 0x00030002, 0x00030003, 0x000a0004, 0x000a0005, 0x000a0006, 0x000f000b, 
    0x000f000c, 0x000f000d, 0x000f000e, 0x000f000f, 0x000f0010, 0x00130001, 0x00130002
};
/*0xAC*/ static u32 _data_AC[] = {
    0x00000000, 0x00000000, 0x00000202, 0x00000001, 0x01010101, 0x01000000
};
/*0xC4*/ static u32 _data_C4[] = {
    0x00000000
};
/*0xC8*/ static u32 _data_C8[] = {
    0x00000000, 0x00000000
};

/*0x0*/ static ANIMBSSUnk0 _bss_0[4];
/*0x20*/ static s8 _bss_20; //count of items in bss0?
/*0x24*/ static u32 _bss_24;
/*0x28*/ static u8 _bss_28[0x4];
/*0x2C*/ static u8 _bss_2C[0x4];
/*0x30*/ static u8 _bss_30[0x2];
/*0x32*/ static u8 _bss_32[0x1];
/*0x33*/ static u8 _bss_33[0x1];
/*0x34*/ static u8 _bss_34[0x4];
/*0x38*/ static u8 _bss_38[0x8];
/*0x40*/ static u8 _bss_40[0x48];
/*0x88*/ static u8 _bss_88[0x1];
/*0x89*/ static u8 _bss_89[0x1];
/*0x8A*/ static u8 _bss_8A[0x1];
/*0x8B*/ static s8 _bss_8B;
/*0x8C*/ static s32 _bss_8C;
/*0x90*/ static s32 _bss_90;
/*0x94*/ static s32 _bss_94;
/*0x98*/ static s32 _bss_98;
/*0x9C*/ static u8 _bss_9C[0x4];
/*0xA0*/ static f32 _bss_A0;
/*0xA4*/ static s8 _bss_A4[0x4];
/*0xA8*/ static s8 _bss_A8[ANIMCURVES_SCENES_MAX];
/*0xD8*/ static s8 _bss_D8[ANIMCURVES_SCENES_MAX];
/*0x108*/ static s8 _bss_108[ANIMCURVES_SCENES_MAX];
/*0x138*/ static s8 _bss_138[ANIMCURVES_SCENES_MAX];
/*0x168*/ static s8 _bss_168[ANIMCURVES_SCENES_MAX];
/*0x198*/ static s8 _bss_198[ANIMCURVES_SCENES_MAX];
/*0x1C8*/ static s8 _bss_1C8[ANIMCURVES_SCENES_MAX];
/*0x1F8*/ static s16 _bss_1F8[ANIMCURVES_SCENES_MAX];
/*0x258*/ static s16 _bss_258[ANIMCURVES_SCENES_MAX];
/*0x2B8*/ static u8 _bss_2B8[0x8];
/*0x2C0*/ static u8 _bss_2C0[0x58];
/*0x318*/ static u16 _bss_318[0x4];
/*0x320*/ static u8 _bss_320[0x58];
/*0x378*/ static s8 _bss_378[ANIMCURVES_SCENES_MAX];
/*0x3A8*/ static u8 _bss_3A8[0x8];
/*0x3B0*/ static u8 _bss_3B0[0x28];
/*0x3D8*/ static s32 _bss_3D8[ANIMCURVES_SCENES_MAX];
/*0x490*/ static s8 _bss_490[ANIMCURVES_SCENES_MAX];
/*0x4C0*/ static u8 _bss_4C0[0x30];
/*0x4F0*/ static u8 _bss_4F0[0xb4];
/*0x5A4*/ static u8 _bss_5A4[0x4];
/*0x5A8*/ static u8 _bss_5A8[0x4];
/*0x5AC*/ static u8 _bss_5AC[0x4];
/*0x5B0*/ static u8 _bss_5B0[0x8];
/*0x5B8*/ static Vec3f _bss_5B8;
/*0x5C4*/ static u8 _bss_5C4[0x4];
/*0x5C8*/ static u8 _bss_5C8[0x4];
/*0x5CC*/ static u8 _bss_5CC[0x4];
/*0x5D0*/ static u8 _bss_5D0[0x4];
/*0x5D4*/ static u8 _bss_5D4[0x4];
/*0x5D8*/ static void* _bss_5D8; //sequence file buffer
/*0x5DC*/ static u8 _bss_5DC[0x4];
/*0x5E0*/ static u8 _bss_5E0[0x4];
/*0x5E4*/ static u8 _bss_5E4[0x4];
/*0x5E8*/ static u8 _bss_5E8[0x8];
/*0x5F0*/ static u8 _bss_5F0[0xa0];
/*0x690*/ static u8 _bss_690[0x8];
/*0x698*/ static ANIMUnk698 _bss_698[1];
/*0x69E*/ static u8 _bss_69E[0x2];
/*0x6A0*/ static u8 _bss_6A0[0x58];
/*0x6F8*/ static s8 _bss_6F8;
/*0x6FC*/ static s32 _bss_6FC;
/*0x700*/ static s16 _bss_700[2];
/*0x708*/ static ANIMActorOverride _bss_708[44][16];
/*0x1D0C*/ static u8 _bss_1D0C[0x7C];
/*0x1D88*/ static u8 _bss_1D88[8];

void dll_3_func_98(void);
static s32 dll_3_func_4BAC(Object* animObj, Object *parent, f32 x, f32 y, f32 z, f32* yOut, f32 ySetup);
void dll_3_func_7B64(AnimObj_Data*);
s32 dll_3_func_9524(Object * arg0, AnimObj_Data *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

// offset: 0x0 | ctor
void dll_3_ctor(void *dll) {
    _bss_5D8 = mmAlloc(0x10, ALLOC_TAG_ANIMSEQ_COL, 0);
    dll_3_func_98();
}

// offset: 0x58 | dtor
void dll_3_dtor(void *dll) {
    mmFree(_bss_5D8);
}

// offset: 0x98 | func: 0 | export: 0
void dll_3_func_98(void) {
    s32 i;

    for (i = 0; i < ANIMCURVES_SCENES_MAX; i++) {
        _bss_108[i] = 0;
        _bss_138[i] = 0;
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
void dll_3_func_2C0(s32 arg0, s32 arg1, s32 arg2) {   
    if (arg0 >= 0 && arg0 < ANIMCURVES_SCENES_MAX && _bss_6F8 < ANIMCURVES_ACTORS_MAX) {
        (&_bss_698[_bss_6F8])->unk0 = arg0;
        (&_bss_698[_bss_6F8])->totalActors = arg2;
        (&_bss_698[_bss_6F8])->unk2 = arg1; _bss_6F8++; 
    }
}

// offset: 0x324 | func: 2 | export: 2
void dll_3_func_324(s32 arg0, s32 arg1) {
    if (arg0 >= 0 && arg0 < 0x2D) {
        _bss_138[arg0] = arg1;
    }
}

// offset: 0x358 | func: 3 | export: 3
s8 dll_3_func_358(s32 arg0) {
    if (arg0 < 0 || arg0 >= 0x2D) {
        return 0;
    }
    return _bss_138[arg0];
}

// offset: 0x394 | func: 4 | export: 28
void dll_3_func_394(s32 arg0, s32 arg1) {
    _bss_378[arg0] = 1;
    _bss_318[arg0] = arg1;
}

// offset: 0x3D0 | func: 5 | export: 4
s32 dll_3_func_3D0(Object* object, s32 updateRate);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_3D0.s")

// offset: 0x15FC | func: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_15FC.s")

// offset: 0x1A04 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_1A04.s")

// offset: 0x1C04 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_1C04.s")

// offset: 0x2760 | func: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_2760.s")

// offset: 0x2EB4 | func: 10
void dll_3_func_2EB4(s32 arg0, s32 arg1, AnimObj_Data* objData) {
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
        if (event->type == ANIMCURVES_EVENTS_timing) {
            if (objData->animCurvesCurrentFrameA >= event->params) {
                objData->unk74 = event->params;
                objData->unk72++;
            } else {
                finished = TRUE;
            }
        } else {
            if ((event->type == ANIMCURVES_EVENTS_subEvent) && (event->params > 0)) {
                if (objData->animCurvesCurrentFrameA >= objData->unk74) {
                    objData->unk74 += event->delay;
                    objData->unk72 += event->params + 1;
                } else {
                    finished = TRUE;
                }
            } else if (objData->animCurvesCurrentFrameA >= objData->unk74) {
                if (event->type != ANIMCURVES_EVENTS_soundOther) {
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_2FE8.s")

// offset: 0x3170 | func: 12
//TODO: verify argument structs are correct, and investigate lengths of BSS arrays
void dll_3_func_3170(s32 arg0, s32 arg1, AnimObj_Data* arg2) {
    if (arg2->unk9D & 1) {
        _bss_108[arg2->unk63] = 1;
    }
    if (arg2->unk9D & 2) {
        _bss_108[arg2->unk63] = 0;
    }
    if (arg2->unk9D & 4) {
        _bss_138[arg2->unk63] = 1;
    }
    if (arg2->unk9D & 8) {
        _bss_138[arg2->unk63] = 0;
    }
    if (arg2->unk9D & 0x10) {
        _bss_198[arg2->unk63] = 1;
    }
    if (arg2->unk9D & 0x20) {
        _bss_198[arg2->unk63] = 0;
    }
}

// offset: 0x3268 | func: 13
s32 dll_3_func_3268(Object* overrideObject, Object* actor, AnimObj_Data* state) {
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
void dll_3_func_32B0(AnimObj_Data* animObjData, s32 arg1) {
    s32 index;

    for (index = 0; index < 4; index++){
        if (animObjData->unk34[index]) {
            if (gDLL_6_AMSFX->vtbl->func_B48(animObjData->unk34[index]) == 0) {
                gDLL_6_AMSFX->vtbl->func_A1C(animObjData->unk34[index]);
                animObjData->unk34[index] = 0;
                animObjData->unk44[index] = 0;
                if (index != 3) {
                    animObjData->unk8A = index;
                }
            }
            if (gDLL_6_AMSFX->vtbl->func_B48(animObjData->unk34[index]) && (animObjData->unk44[index] <= 0)) {
                gDLL_6_AMSFX->vtbl->func_A1C(animObjData->unk34[index]);
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
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_3414.s")
#else
void dll_3_func_1A04(Object* a0, Object *a1, void* a2); //unsure
void dll_3_func_1C04(Object* actor, Object *a1, void* a2, s32 a3); //unsure
void dll_3_func_2760(Object* actor, Object *a1, void* a2, s32 a3); //unsure
void dll_3_func_4698(Object* actor, Object* override, AnimObj_Data* animObjData, s8 arg3);
void dll_3_func_4924(Object* animObj, Object** actorObject, ModelInstance** actorModelInstance);
void dll_3_func_4B20(Object* animObj, AnimObj_Setup* setup);
void dll_3_func_4FC4(Object* a0, void* a1); //unsure
void dll_3_func_71C0(Object *arg0, Object* arg1, AnimObj_Data* arg2);

void dll_3_func_3414(Object* animObj, Object **arg1, AnimObj_Data* arg2, AnimObj_Setup* arg3, ModelInstance **arg4) {
    Object* temp_s1;

    _bss_90 = 1;
    _bss_98 = 0x5A;
    arg2->animCurvesCurrentFrameA = arg2->unk6A;
    arg2->animCurvesCurrentFrameB = -0x3C;
    dll_3_func_1C04(animObj, *arg1, arg2, 0);
    dll_3_func_2760(animObj, *arg1, arg2, 1);
    dll_3_func_4924(animObj, arg1, arg4);
    dll_3_func_4FC4(animObj, arg2);
    if (arg2->unk86 == 1) {
        dll_3_func_4B20(animObj, arg3);
    }
    
    animObj->srt.yaw += arg2->unk1A;
    temp_s1 = *arg1;
    if ((animObj != temp_s1) && (_bss_5CC[0] == 0)) {
        dll_3_func_4698(temp_s1, animObj, arg2, _bss_A8[arg2->unk63]);
    }
    temp_s1 = *arg1;
    dll_3_func_1A04(animObj, temp_s1, arg2);
    arg2->unk9A = 0;
    arg2->unk9B = 0;
    arg2->unk8B = 1;
    arg2->animCurvesCurrentFrameB = arg2->animCurvesCurrentFrameA;
    if (_bss_A4[0] != 0) {
        dll_3_func_71C0(animObj, *arg1, arg2);
    }
    _bss_258[arg2->unk63] = arg2->animCurvesCurrentFrameA;
}
#endif

// offset: 0x3614 | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_3614.s")

// offset: 0x4158 | func: 17
s8 dll_3_func_4158(AnimObj_Data* animObjData) {
    u32 index;

    index = 0;
    if (animObjData->unk34[animObjData->unk8A] != 0) {

        while (animObjData->unk34[index] && index < 3){ 
            index++;
        }
        
        if (index == 4) {
            gDLL_6_AMSFX->vtbl->func_A1C(animObjData->unk34[animObjData->unk8A]);
            animObjData->unk34[animObjData->unk8A] = 0;
        } else {
            animObjData->unk8A = index - 1;
        }
    }
    return animObjData->unk8A;
}

// offset: 0x422C | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_422C.s")

// offset: 0x4698 | func: 19
void dll_3_func_4698(Object* actor, Object* override, AnimObj_Data* animObjData, s8 arg3) {
    AnimationCallback animCallback;
    s32 callbackResult;
    AnimObj_Setup *setup;

    setup = (AnimObj_Setup*)override->setup;
    actor->positionMirror2.f[0] = actor->srt.transl.f[0];
    actor->positionMirror2.f[1] = actor->srt.transl.f[1];
    actor->positionMirror2.f[2] = actor->srt.transl.f[2];
    actor->positionMirror3.f[0] = actor->positionMirror.f[0];
    actor->positionMirror3.f[1] = actor->positionMirror.f[1];
    actor->positionMirror3.f[2] = actor->positionMirror.f[2];

    if (actor->animCallback != NULL) {
        // a re-cast to use the actual struct instead of a forward declaration is fine here
        animCallback = actor->animCallback;
        callbackResult = animCallback(actor, override, animObjData, arg3);
        if (callbackResult == 4) {
            *_bss_A4 = 1;
        } else if (callbackResult != 0) {
            if (_bss_D8[animObjData->unk63] < 2) {
                _bss_D8[animObjData->unk63] = callbackResult;
            }
        }
        animObjData->unk98 = 0;
        animObjData->unk8D = 0;
    } else {
        if (animObjData->unk87 != 0) {
            animObjData->unk62 = 0;
            return;
        }
        if (animObjData->unk62 >= 4) {
            if (dll_3_func_9524(actor, animObjData, 6, 0x1E, 0x50, -1, -1) != 0) {
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
    get_object_child_position(actor, actor->positionMirror.f, &actor->positionMirror.f[1], &actor->positionMirror.f[2]);
    if (actor->objhitInfo != NULL) {
        actor->objhitInfo->unk48 = 0;
        actor->objhitInfo->unk62 = 0;
    }
    if (actor->unk58 != NULL) {
        actor->unk58->unk10f = 0;
    }
}

// offset: 0x4924 | func: 20
/** get_actor_object_and_model_instance? */
void dll_3_func_4924(Object* animObj, Object** actorObject, ModelInstance** actorModelInstance) {
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_495C.s")

// offset: 0x4A7C | func: 22
s32 dll_3_func_4A7C(AnimObj_Data* objData, s32 arg1) {
    AnimCurvesEvent* event;
    s32 subevent;
    s32 index;
    u32 delay;

    delay = 0;
    for (index = 0; index < objData->animCurvesEventCount; index++, delay += event->delay){
        event = &objData->animCurvesEvents[index];
        if (event->type == ANIMCURVES_EVENTS_timing){
            delay = event->params;
        } else if (event->type == ANIMCURVES_EVENTS_subEvent && event->params > 0){
            subevent = *((s32 *)(event + 1));
            if (((subevent & 0x3F) == 9) && (arg1 == (u16)(subevent >> 0x10))){
                return delay;
            }
            index += event->params;
        }
    }
    
    return -1;
}

// offset: 0x4B20 | func: 23
void dll_3_func_4B20(Object* animObj, AnimObj_Setup* setup) {
    f32 floatVal;

    if (dll_3_func_4BAC(animObj, 
                        animObj->parent, 
                        animObj->positionMirror.x, 
                        animObj->positionMirror.y, 
                        animObj->positionMirror.z,
                        &floatVal,
                        setup->base.y) != 0) {
        animObj->srt.transl.y += floatVal - setup->base.y;
    }
}

// offset: 0x4BAC | func: 24
/** Something to do with snapping characters to mesh height? */
s32 dll_3_func_4BAC(Object* animObj, Object *parent, f32 x, f32 y, f32 z, f32* yOut, f32 ySetup) {
    s32 lowestIndex;
    f32 sampleY;
    Func_80057F1C_Struct** sp5C;
    f32 lowestFound;
    s32 temp_v0;
    s32 index;

    temp_v0 = func_80057F1C(animObj, x, animObj->positionMirror.f[1], z, &sp5C, 0, 1);
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_4FC4.s")

// offset: 0x51E0 | func: 26
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_51E0.s")
#else

typedef struct {
    s32 unk0;
    s32 unk4;
    f32 unk8;
    f32 unkC;
} Func51E0Arg0;

static void dll_3_func_57A4(Func51E0Arg0* arg0, f32 arg1);

s32 dll_3_func_51E0(Func51E0Arg0* arg0, Vec3f* arg1, Vec3f* arg2, s16* arg3, s8 arg4) {
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
    dll_3_func_57A4(arg0, sp80);
    var_s1 = gDLL_26_Curves->vtbl->func_39C(arg0->unk0);
    
    if (var_s1 != NULL) {
        if (arg0->unk4 >= 0) {
            sp84 = gDLL_26_Curves->vtbl->func_39C(arg0->unk4);
            for (var_v1 = 0; var_v1 < 9 && ((f32*)arg0)[var_v1 + 2] <= sp80; var_v1++) {}
            var_v1--;
            sp7C = var_v1;
            sp7C += ((sp80 - ((f32*)arg0)[var_v1 + 2]) / (((f32*)arg0)[var_v1 + 3] - ((f32*)arg0)[var_v1 + 2]));
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
#endif

// offset: 0x5698 | func: 27
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_5698.s")
#else
void dll_3_func_5A48(void* arg0, CurveSetup* a2, CurveSetup* a3, f32 a4, s32 a5);

//NOTE: not sure a0 is AnimObj_Data, could be other struct
void dll_3_func_5698(AnimObj_Data* objData, Object* arg1) {
    CurveSetup* temp_v0;
    s32 pad[2];
    s32 temp_a1;
    s32 sp2C;
    s32 var_a2;
    s32 index;
    s32 var_a0;

    objData->actor = arg1;
    objData->unk4 = -1;
    temp_v0 = gDLL_26_Curves->vtbl->func_39C((s32)arg1);

    for (var_a0 = 1, index = 0, var_a2 = sp2C; index < 4; index++, var_a0 *= 2){
        if ((temp_v0->unk1C[index] >= 0) && !(temp_v0->unk1B & var_a0)) {
            var_a2 = temp_v0->unk1C[index];
            index = 5;
        }
    }
    
    if (index != 6) {
        objData->actor = (Object*)-1;
        return;
    }
    
    objData->unk4 = var_a2;
    dll_3_func_5A48(objData, temp_v0, gDLL_26_Curves->vtbl->func_39C(var_a2), 0, 0);
}
#endif

// offset: 0x57A4 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_57A4.s")

// offset: 0x5A48 | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_5A48.s")

// offset: 0x5D78 | func: 30
/** Called when sequence is waiting for button presses (e.g. menu when talking with Rocky) */
s32 dll_3_func_5D78(Object* override, s32 arg1, AnimObj_Data* objData) {
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_5E50.s")

// offset: 0x60AC | func: 32
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_60AC.s")

// offset: 0x65EC | func: 33 | export: 19
void dll_3_func_65EC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    _bss_8C = arg0;
    _bss_90 = arg1;
    _bss_94 = arg2;
    _bss_98 = arg3;
}

// offset: 0x6620 | func: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_6620.s")

// offset: 0x6EBC | func: 35
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_6EBC.s")
#else

f32 dll_3_func_6F3C(AnimCurvesKeyframe*, s32, s16);

f32 dll_3_func_6EBC(AnimObj_Data* state, s32 channelIndex) {
    f32 result;
    s16 total_keys;
    s16 first_key;

    if (state->animCurvesKeyframes == 0) {
        return 0.0f;
    }

    result = 0.0f;
    total_keys = state->channelTotalKeys[channelIndex];
    if (total_keys) {
        first_key = state->channelFirstKeyIndex[channelIndex];
        result = dll_3_func_6F3C(&state->animCurvesKeyframes[first_key], total_keys & 0xFFF, total_keys);
    }
    return result;
}
#endif

// offset: 0x6F3C | func: 36
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_6F3C.s")

// offset: 0x71C0 | func: 37
void dll_3_func_71C0(s32 arg0, Object* arg1, AnimObj_Data* arg2) {
    s32 i;
    u32 soundHandle;

    if (arg2->unkF4 != NULL) {
        arg2->unkF4(arg2->unk11C, arg0, arg2);
    }

    for (i = 0; i < 4; i++){
        soundHandle = arg2->unk34[i];
        if (soundHandle && (gDLL_6_AMSFX->vtbl->func_B48(soundHandle) == 0)) {
            gDLL_6_AMSFX->vtbl->func_A1C(arg2->unk34[i]);
        }
    }
    
    if (arg2->unk8B != 0) {
        if (arg2->unk87 != 0) {
            arg2->unk87 = 0;
        }
        if (arg2->actor != 0) {
            arg1->unkC0 = 0;
            arg1->unkB0 &= ~0x1000;
            arg2->actor = 0;
        }
    }
    arg2->unk8B = 0;
}

// offset: 0x72E0 | func: 38
void dll_3_func_72E0(Object* arg0) {
    _bss_6FC = arg0;
    _bss_700[0] = gUpdateRate;
}

// offset: 0x730C | func: 39 | export: 5
void dll_3_func_730C(void);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_730C.s")

// offset: 0x7974 | func: 40 | export: 6
void dll_3_func_7974(AnimObj_Data* arg0, AnimObj_Setup* setup) {
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

    arg0->animCurvesEvents = mmAlloc(size, ALLOC_TAG_ANIMSEQ_COL, 0);
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
        _bss_138[arg0->unk63] = 0;
        _bss_198[arg0->unk63] = 0;
    }

    if (setup->unk22 != 0) {
        arg0->unk8B = 2;
    } else {
        arg0->unk8B = 0;
    }

    dll_3_func_7B64(arg0);
}

// offset: 0x7B64 | func: 41 | export: 7
void dll_3_func_7B64(AnimObj_Data* state) {
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
        if ((&state->animCurvesEvents[index])->type == ANIMCURVES_EVENTS_setDuration) {
            state->animCurvesDuration = state->animCurvesEvents[index].params + 1;
        }
    }
}

// offset: 0x7C6C | func: 42 | export: 8
void dll_3_func_7C6C(AnimObj_Data* state) {
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
void dll_3_func_7CF0(void);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_7CF0.s")

// offset: 0x81F8 | func: 44
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_81F8.s")

// offset: 0x8598 | func: 45 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_8598.s")

// offset: 0x8878 | func: 46 | export: 12
s32 dll_3_func_8878(void) {
    return 1;
}

// offset: 0x8884 | func: 47 | export: 13
s32 dll_3_func_8884(void) {
    return _data_2C;
}

// offset: 0x88A0 | func: 48 | export: 14
void dll_3_func_88A0(s32 arg0) {
    _data_2C = arg0;
}

// offset: 0x88BC | func: 49 | export: 11
s32 dll_3_func_88BC(s32 arg0) {
    return 0;
}

// offset: 0x88CC | func: 50 | export: 15
s32 dll_3_func_88CC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return 0;
}

// offset: 0x88E8 | func: 51 | export: 16
void dll_3_func_88E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) { }

// offset: 0x8900 | func: 52 | export: 17
void dll_3_func_8900(s32 objectSeqIndex, Object* object, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_8900.s")

// offset: 0x906C | func: 53 | export: 18
void dll_3_func_906C(s32 arg0);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_906C.s")

// offset: 0x9358 | func: 54 | export: 20
void dll_3_func_9358(Object *arg0, s32 arg1) {
    ANIMBSSUnk0 *temp;
    s8 count;

    count = _bss_20;    
    if (count < 4) {
        temp = &_bss_0[count];
        temp->unk0 = arg0;
        temp->unk4 = arg1;
        _bss_20 = count + 1;
    }
}

// offset: 0x93A0 | func: 55
s32 dll_3_func_93A0(Object* actor) {
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
void dll_3_func_9440(AnimObj_Data* arg0, s32 arg1) {
    arg0->unk80 = arg1;
    arg0->unk9D |= 0x40;
}

// offset: 0x9458 | func: 57 | export: 22
s8 dll_3_func_9458(void) {
    return _data_28[0];
}

// offset: 0x9474 | func: 58 | export: 23
void dll_3_func_9474(s8 arg0) {
    _data_28[0] = arg0;
}

// offset: 0x949C | func: 59 | export: 24
s16 dll_3_func_949C(void) {
    return _data_14[0];
}

// offset: 0x94B8 | func: 60 | export: 25
void dll_3_func_94B8(s16 arg0) {
    _data_14[0] = arg0;
}

// offset: 0x94E0 | func: 61 | export: 26
s16 dll_3_func_94E0(void) {
    return _data_18[0];
}

// offset: 0x94FC | func: 62 | export: 27
void dll_3_func_94FC(s16 arg0) {
    _data_18[0] = arg0;
}

// offset: 0x9524 | func: 63 | export: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_9524.s")

// offset: 0x9B70 | func: 64
void dll_3_func_9B70(Object* arg1, s32 arg2, s32 arg3) {
    s16* temp_v0;

    //NOTE: sequence bone should probably be a struct instead of s16*?
    temp_v0 = func_80034804(arg1, 0);
    if (temp_v0 != NULL) {
        temp_v0[1] = 0;
        temp_v0[0] = 0;
    }
}

// offset: 0x9BC0 | func: 65
void dll_3_func_9BC0(s32 arg0) {
    s32 index;

    for (index = 0; index < ANIMCURVES_ACTORS_MAX; index++) { 
        _bss_708[arg0][index].object = 0; 
    }
}

// offset: 0x9C08 | func: 66
Object* dll_3_func_9C08(s32 animCurvesIndex, Object* searchObject) {
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
void dll_3_func_9C94(s32 index, Object* object, Object* overrideObject) {
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
void dll_3_func_9CE8(s32 arg0) {
    //TODO: figure out what these structs really are
    CameraFunc15Unk* temp_v0;
    CameraFunc15Unk_unk74 sp34;

    if (gDLL_2_Camera->vtbl->func3() == 0x5F) {
        return;
    }
    
    temp_v0 = (CameraFunc15Unk*)gDLL_2_Camera->vtbl->func15();
    if ((temp_v0 != NULL) && (temp_v0->unk74 != NULL)) {
        sp34.unk0.x = temp_v0->unk74->unk0.x;
        sp34.unk0.y = temp_v0->unk74->unk0.y;
        sp34.unk0.z = temp_v0->unk74->unk0.z;
        sp34.unkC = arg0;
        gDLL_2_Camera->vtbl->func6(0x5F, 1, 0, 0x10, &sp34, 0x3C, 0xFF);
    }
}

// offset: 0x9DD4 | func: 69
void dll_3_func_9DD4(void) {
    if (gDLL_2_Camera->vtbl->func3() == 0x5F) {
        gDLL_2_Camera->vtbl->func6(0x54, 0, 3, 0, NULL, 0, 0);
    }
}

// offset: 0x9E58 | func: 70 | export: 30
s32 dll_3_func_9E58(s32 arg0, Object *arg1, s32 arg2) {
    _data_20 = arg0;
    _data_24 = arg1;
    _data_1C[0] = arg2;
    return 1;
}

// offset: 0x9E88 | func: 71 | export: 31
s32 dll_3_func_9E88(f32 arg0, f32 arg1, f32 arg2) {
    _bss_5AC[0] = 1;
    _bss_5B8.x = arg0;
    _bss_5B8.y = arg1;
    _bss_5B8.z = arg2;
    return 1;
}

// offset: 0x9EC8 | func: 72
void dll_3_func_9EC8(Object* arg0, SequenceBoneStructUnk* arg1, s32 arg2) {
    //TODO: figure out what these structs are
    SequenceBoneStructUnk *temp_v0;
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
        temp_v0 = (SequenceBoneStructUnk *) func_80034804(arg0, var_s0[i]);
        if (temp_v0 != NULL){
            temp_v0->unk2 = arg1->unk2;
            temp_v0->unk0 = arg1->unk0;
            temp_v0->unk4 = arg1->unk4;
        }
    }
    
    var_s0 = temp_v1 + 1;
}

// offset: 0x9F90 | func: 73 | export: 32
s32 dll_3_func_9F90(s32 arg0, s32 arg1) {
    switch (arg1) {
        case 0:
            _bss_1D88[0] = (_bss_1D88[0] & 0xFFFF) | 0x80;
            break;
        case 1:
            _bss_1D88[0] =  _bss_1D88[0] & 0xFF7F;
            break;
    }

    return 0;
}
