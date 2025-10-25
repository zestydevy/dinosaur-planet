//May have been stored as "game/anim.c" (based off leftover string at 0x2F1560 in SFA Kiosk's "default.dol")

#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/3_animation.h"
#include "sys/asset_thread.h"
#include "sys/fs.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "game/objects/object.h"

#include "dlls/objects/214_animobj.h"

#include "prevent_bss_reordering.h"

void dll_3_func_7B64(AnimObj_Data*);
s32 dll_3_func_9524(Object * arg0, AnimObj_Data *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

enum AnimCurvesKeyframeChannels {
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
};

enum AnimCurvesEvents {
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
};

typedef struct {
u32 unk0;
u32 unk4;
} ANIMBSSUnk0;

typedef struct{
    Object* object;
    Object* overrideObject;
} ANIMActorOverride;

typedef struct{
    ANIMActorOverride actors[16];
} ANIMUnknownBSS708;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 totalActors; //maybe?
} ANIMUnk698;

typedef struct {
/*00*/ Object* object;
/*04*/ s8 unk4[0xAD - 4];
/*AD*/ s8 unkAD;
} ANIMState;

typedef struct {
/*00*/ s8 unk0[0x80 - 0];
/*80*/ s32 unk80;
/*84*/ s8 unk84[0x9D - 0x84];
/*9D*/ u8 unk9D;
} ANIMUnknown;

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
/*0x24*/ static s32 _data_24 = 0;
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
/*0x20*/ static s8 _bss_20[0x4];
/*0x24*/ static u8 _bss_24[0x4];
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
/*0x8B*/ static u8 _bss_8B[0x1];
/*0x8C*/ static s32 _bss_8C;
/*0x90*/ static s32 _bss_90;
/*0x94*/ static s32 _bss_94;
/*0x98*/ static s32 _bss_98;
/*0x9C*/ static u8 _bss_9C[0x4];
/*0xA0*/ static u8 _bss_A0[0x4];
/*0xA4*/ static s8 _bss_A4[0x4];
/*0xA8*/ static u8 _bss_A8[0x1];
/*0xA9*/ static u8 _bss_A9[0x1];
/*0xAA*/ static u8 _bss_AA[0x2];
/*0xAC*/ static u8 _bss_AC[0x4];
/*0xB0*/ static u8 _bss_B0[0x28];
/*0xD8*/ static s8 _bss_D8[0x1];
/*0xD9*/ static u8 _bss_D9[0x1];
/*0xDA*/ static u8 _bss_DA[0x2];
/*0xDC*/ static u8 _bss_DC[0x4];
/*0xE0*/ static u8 _bss_E0[0x28];
/*0x108*/ static u8 _bss_108[0x1];
/*0x109*/ static u8 _bss_109[0x1];
/*0x10A*/ static u8 _bss_10A[0x2];
/*0x10C*/ static u8 _bss_10C[0x4];
/*0x110*/ static u8 _bss_110[0x28];
/*0x138*/ static s8 _bss_138[0x1];
/*0x139*/ static u8 _bss_139[0x1];
/*0x13A*/ static u8 _bss_13A[0x2];
/*0x13C*/ static u8 _bss_13C[0x4];
/*0x140*/ static u8 _bss_165[0x28];
/*0x168*/ static u8 _bss_168[0x1];
/*0x169*/ static u8 _bss_169[0x1];
/*0x16A*/ static u8 _bss_16A[0x2];
/*0x16C*/ static u8 _bss_16C[0x4];
/*0x170*/ static u8 _bss_170[0x28];
/*0x198*/ static u8 _bss_198[0x1];
/*0x199*/ static u8 _bss_199[0x1];
/*0x19A*/ static u8 _bss_19A[0x2];
/*0x19C*/ static u8 _bss_19C[0x4];
/*0x1A0*/ static u8 _bss_1A0[0x28];
/*0x1C8*/ static u8 _bss_1C8[0x1];
/*0x1C9*/ static u8 _bss_1C9[0x1];
/*0x1CA*/ static u8 _bss_1CA[0x2];
/*0x1CC*/ static u8 _bss_1CC[0x4];
/*0x1D0*/ static u8 _bss_1D0[0x28];
/*0x1F8*/ static u8 _bss_1F8[0x2];
/*0x1FA*/ static u8 _bss_1FA[0x2];
/*0x1FC*/ static u8 _bss_1FC[0x4];
/*0x200*/ static u8 _bss_200[0x58];
/*0x258*/ static u8 _bss_258[0x2];
/*0x25A*/ static u8 _bss_25A[0x2];
/*0x25C*/ static u8 _bss_25C[0x4];
/*0x260*/ static u8 _bss_260[0x58];
/*0x2B8*/ static u8 _bss_2B8[0x8];
/*0x2C0*/ static u8 _bss_2C0[0x58];
/*0x318*/ static u16 _bss_318[0x4];
/*0x320*/ static u8 _bss_320[0x58];
/*0x378*/ static u8 _bss_378[0x1];
/*0x379*/ static u8 _bss_379[0x1];
/*0x37A*/ static u8 _bss_37A[0x2];
/*0x37C*/ static u8 _bss_37C[0x4];
/*0x380*/ static u8 _bss_380[0x28];
/*0x3A8*/ static u8 _bss_3A8[0x8];
/*0x3B0*/ static u8 _bss_3B0[0x28];
/*0x3D8*/ static u8 _bss_3D8[0x4];
/*0x3DC*/ static u8 _bss_3DC[0x4];
/*0x3E0*/ static u8 _bss_3E0[0xb0];
/*0x490*/ static u8 _bss_490[0x1];
/*0x491*/ static u8 _bss_491[0x1];
/*0x492*/ static u8 _bss_492[0x2];
/*0x494*/ static u8 _bss_494[0x4];
/*0x498*/ static u8 _bss_498[0x8];
/*0x4A0*/ static u8 _bss_4A0[0x1d];
/*0x4BD*/ static u8 _bss_4BD[0x1];
/*0x4BE*/ static u8 _bss_4BE[0x2];
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
/*0x6F8*/ static s8 _bss_6F8[0x4];
/*0x6FC*/ static s32 _bss_6FC;
/*0x700*/ static s16 _bss_700[2];
/*0x708*/ static ANIMActorOverride _bss_708[44][16];
/*0x1D0C*/ static u8 _bss_1D0C[0x7C];
/*0x1D88*/ static u8 _bss_1D88[8];

// offset: 0x0 | ctor
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_ctor.s")
#else
// Needs dll_3_func_98 matched 
void dll_3_func_98();

void dll_3_ctor(s32 arg0) {
    _bss_5D8 = mmAlloc(0x10, ALLOC_TAG_ANIMSEQ_COL, 0);
    dll_3_func_98();
}
#endif

// offset: 0x58 | dtor
void dll_3_dtor(s32 arg0) {
    mmFree(_bss_5D8);
}

// offset: 0x98 | func: 0 | export: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_98.s")

// offset: 0x2C0 | func: 1 | export: 1
void dll_3_func_2C0(s32 arg0, s32 arg1, s32 arg2) {   
    if (arg0 >= 0 && arg0 < ANIMCURVES_SCENES_MAX && _bss_6F8[0] < ANIMCURVES_ACTORS_MAX){
        (&_bss_698[_bss_6F8[0]])->unk0 = arg0;
        (&_bss_698[_bss_6F8[0]])->totalActors = arg2;
        (&_bss_698[_bss_6F8[0]])->unk2 = arg1; _bss_6F8[0]++; 
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_2EB4.s")

// offset: 0x2FE8 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_2FE8.s")

// offset: 0x3170 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_3170.s")

// offset: 0x3268 | func: 13
s32 dll_3_func_3268(Object* overrideObject, Object* actor, AnimObj_Data* state) {
    s32 returnVal;

    returnVal = 0;
    if (state->unk9D & 0x40){
        returnVal = 1;
        state->unk9D &= 0xFFBF;
        state->animCurvesCurrentFrameA = state->unk80;
        state->animCurvesCurrentFrameB = state->animCurvesCurrentFrameA;
    }
    state->unk9D = 0;
    return returnVal;
}

// offset: 0x32B0 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_32B0.s")

// offset: 0x3414 | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_3414.s")

// offset: 0x3614 | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_3614.s")

// offset: 0x4158 | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_4158.s")

// offset: 0x422C | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_422C.s")

// offset: 0x4698 | func: 19
void dll_3_func_4698(Object* arg0, Object* arg1, AnimObj_Data* arg2, s8 arg3) {
    int (*temp_v1)(Object*, Object*, AnimObj_Data*, s8);
    s32 temp_v0;
    AnimObjSetup *setup;

    setup = (AnimObjSetup*)arg1->setup;
    arg0->positionMirror2.f[0] = arg0->srt.transl.f[0];
    arg0->positionMirror2.f[1] = arg0->srt.transl.f[1];
    arg0->positionMirror2.f[2] = arg0->srt.transl.f[2];
    arg0->positionMirror3.f[0] = arg0->positionMirror.f[0];
    arg0->positionMirror3.f[1] = arg0->positionMirror.f[1];
    arg0->positionMirror3.f[2] = arg0->positionMirror.f[2];
    if (arg0->animCallback != NULL) {
        // a re-cast to use the actual struct instead of a forward declaration is fine here
        temp_v1 = (int (*)(Object*, Object*, AnimObj_Data*, s8))arg0->animCallback;
        temp_v0 = temp_v1(arg0, arg1, arg2, arg3);
        if (temp_v0 == 4) {
            *_bss_A4 = 1;
        } else if (temp_v0 != 0) {
            if (_bss_D8[arg2->unk63] < 2) {
                _bss_D8[arg2->unk63] = temp_v0;
            }
        }
        arg2->unk98 = 0;
        arg2->unk8D = 0;
    } else {
        if (arg2->unk84[3] != 0) {
            arg2->unk62 = 0;
            return;
        }
        if (arg2->unk62 >= 4) {
            if (dll_3_func_9524(arg0, arg2, 6, 0x1E, 0x50, -1, -1) != 0) {
                if (_bss_D8[arg2->unk63] < 2) {
                    _bss_D8[arg2->unk63] = 1;
                }
            }
        } else if (arg2->unk62 != 0) {
            if (arg2->unk62 != 2) {
                arg2->unk58 = 1.0f;
                arg2->unk4C.x = arg0->srt.transl.f[0] - arg1->srt.transl.f[0];
                arg2->unk4C.y = arg0->srt.transl.f[1] - arg1->srt.transl.f[1];
                arg2->unk4C.z = arg0->srt.transl.f[2] - arg1->srt.transl.f[2];
                arg2->unk62 = 2;
            }
            if (setup->unk20 == 1) {
                arg2->unk24 = 0.016666668f;
                if (_bss_D8[arg2->unk63] < 2) {
                    _bss_D8[arg2->unk63] = 1;
                }
            }
            arg2->unk58 = arg2->unk58 - (arg2->unk24 * delayFloat);
            if (arg2->unk58 <= 0.0f) {
                arg2->unk62 = 0;
            }
        }
    }
    arg0->unk0xaf &= 0xFFF8;
    get_object_child_position(arg0, arg0->positionMirror.f, &arg0->positionMirror.f[1], &arg0->positionMirror.f[2]);
    if (arg0->objhitInfo != NULL) {
        arg0->objhitInfo->unk_0x48 = 0;
        arg0->objhitInfo->unk_0x62 = 0;
    }
    if (arg0->unk0x58 != NULL) {
        arg0->unk0x58->unk10f = 0;
    }
}

// offset: 0x4924 | func: 20
void dll_3_func_4924(Object* arg0, Object** arg1, ModelInstance** arg2) {
    ANIMState *state = arg0->data;
    Object *obj;

    obj = state->object;
    if (!obj){
        obj = arg0;
    }

    *arg2 = obj->modelInsts[obj->modelInstIdx];
    *arg1 = obj;
}

// offset: 0x495C | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_495C.s")

// offset: 0x4A7C | func: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_4A7C.s")

// offset: 0x4B20 | func: 23
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_4B20.s")
#else
s32 dll_3_func_4BAC(Object*, Object*, f32, f32, f32, f32*, f32);

void dll_3_func_4B20(Object* animObj, AnimObjSetup* setup) {
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
#endif

// offset: 0x4BAC | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_4BAC.s")

// offset: 0x4FC4 | func: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_4FC4.s")

// offset: 0x51E0 | func: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_51E0.s")

// offset: 0x5698 | func: 27
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_5698.s")

// offset: 0x57A4 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_57A4.s")

// offset: 0x5A48 | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_5A48.s")

// offset: 0x5D78 | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_5D78.s")

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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_71C0.s")

// offset: 0x72E0 | func: 38
void dll_3_func_72E0(s32 arg0) {
    _bss_6FC = arg0;
    _bss_700[0] = delayByte;
}

// offset: 0x730C | func: 39 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_730C.s")

// offset: 0x7974 | func: 40 | export: 6
void dll_3_func_7974(AnimObj_Data* arg0, AnimObjSetup* setup) {
    s32 animcurves_bin_offset;
    s32 size;
    s32 animCurvesIndex;

    if (setup->sequenceIdBitfield == -1){
        return;
    }

    arg0->animCurvesKeyframeCount = 0;
    arg0->animCurvesEventCount = 0;
    animCurvesIndex = setup->sequenceIdBitfield;

    if (animCurvesIndex & ANIMCURVES_IS_OBJSEQ2CURVE_INDEX){
        queue_load_file_region_to_ptr((void *) _bss_5D8, OBJSEQ2CURVE_TAB, (((s32) (animCurvesIndex & 0x7FF0)) >> 4) * 2, 8);
        animCurvesIndex = ((s16 *) _bss_5D8)[0] + (animCurvesIndex & 0xF);
    } else {
        animCurvesIndex = animCurvesIndex + 1;
    }

    queue_load_file_region_to_ptr((void *) _bss_5D8, ANIMCURVES_TAB, animCurvesIndex * 8, 0x10);
    animcurves_bin_offset = ((s32 *) _bss_5D8)[1];
    size = (((s32 *) _bss_5D8)[0] >> 0x10) & 0xFFFF;
    if (!size){
        return;
    }

    arg0->animCurvesEvents = mmAlloc(size, ALLOC_TAG_ANIMSEQ_COL, 0);
    if (arg0->animCurvesEvents == NULL){
        return;
    }

    queue_load_file_region_to_ptr((void *) arg0->animCurvesEvents, ANIMCURVES_BIN, animcurves_bin_offset, size);
    arg0->animCurvesEventCount = ((s32 *) _bss_5D8)[0] & 0xFFFF;
    arg0->animCurvesKeyframeCount = ((size >> 2) - arg0->animCurvesEventCount) >> 1;
    arg0->animCurvesKeyframes = (AnimCurvesKeyframe *) (&arg0->animCurvesEvents[arg0->animCurvesEventCount]);
    arg0->unk63 = setup->unk1F;

    if (arg0->unk63 >= 0){
        _bss_108[arg0->unk63] = 0;
        _bss_138[arg0->unk63] = 0;
        _bss_198[arg0->unk63] = 0;
    }

    if (setup->unk22 != 0){
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
            state->animCurvesDuration = (&state->animCurvesEvents[index])->params + 1;
        }
    }
}

// offset: 0x7C6C | func: 42 | export: 8
void dll_3_func_7C6C(AnimObj_Data* arg0) {
    s32 temp_a0;
    s32 temp_a2;

    temp_a2 = (s32)arg0->animCurvesEvents;
    if (temp_a2) {
        mmFree((void*)temp_a2);
        arg0->animCurvesEvents = 0;
        arg0->animCurvesKeyframes = 0;
    }

    temp_a0 = arg0->unk2C;
    if (temp_a0) {
        mmFree((void*)temp_a0);
        arg0->unk2C = 0;
    }
}

// offset: 0x7CF0 | func: 43 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_7CF0.s")

// offset: 0x81F8 | func: 44
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_81F8.s")

// offset: 0x8598 | func: 45 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_8598.s")

// offset: 0x8878 | func: 46 | export: 12
s32 dll_3_func_8878(void){
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
s32 dll_3_func_88BC(s32 arg0){
    return 0;
}

// offset: 0x88CC | func: 50 | export: 15
s32 dll_3_func_88CC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return 0;
}

// offset: 0x88E8 | func: 51 | export: 16
void dll_3_func_88E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3){
}

// offset: 0x8900 | func: 52 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_8900.s")

// offset: 0x906C | func: 53 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_906C.s")

// offset: 0x9358 | func: 54 | export: 20
void dll_3_func_9358(s32 arg0, s32 arg1) {
    ANIMBSSUnk0 *temp;
    s8 currentValue;

    currentValue = _bss_20[0];    
    if (currentValue < 4){
        temp = &_bss_0[currentValue];
        temp->unk0 = arg0;
        temp->unk4 = arg1;
        _bss_20[0] = currentValue + 1;
    }
}

// offset: 0x93A0 | func: 55
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_93A0.s")

// offset: 0x9440 | func: 56 | export: 21
void dll_3_func_9440(ANIMUnknown* arg0, s32 arg1) {
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

    for (i = 0; i < ANIMCURVES_ACTORS_MAX; i++){
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

    for (offset = 0; offset < 0x80; offset += 8){
        actor = (ANIMActorOverride *) (actors + offset);
        actorObject = actor->object;
        if (!actorObject || object == actorObject){
            break;
        }
    }

    actor = (ANIMActorOverride *) (actors + offset);
    actor->object = object;
    actor->overrideObject = overrideObject;
}

// offset: 0x9CE8 | func: 68
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_9CE8.s")

// offset: 0x9DD4 | func: 69
void dll_3_func_9DD4(void) {
    if (gDLL_2_Camera->vtbl->func3() == 0x5F) {
        gDLL_2_Camera->vtbl->func6(0x54, 0, 3, 0, NULL, 0, 0);
    }
}

// offset: 0x9E58 | func: 70 | export: 30
s32 dll_3_func_9E58(s32 arg0, s32 arg1, s32 arg2) {
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/3_ANIM/dll_3_func_9EC8.s")

// offset: 0x9F90 | func: 73 | export: 32
s32 dll_3_func_9F90(s32 arg0, s32 arg1) {
    switch (arg1){
        case 0:
            _bss_1D88[0] = (_bss_1D88[0] & 0xFFFF) | 0x80;
            break;
        case 1:
            _bss_1D88[0] =  _bss_1D88[0] & 0xFF7F;
            break;
    }

    return 0;
}
