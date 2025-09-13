#ifndef _SYS_OBJHITS_H
#define _SYS_OBJHITS_H

#include "common.h"
#include "sys/map.h"
#include "sys/gfx/animation.h"
#include "sys/gfx/model.h"
#include "dlls/modgfx/106.h"

// size: 0x14
typedef struct Unk80026DF4 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 pad6;
    u8 unk8;
    f32 unkC;
    s32 pad10;
} Unk80026DF4;

// size: 0x6C
typedef struct Unk80027934 {
    u8 pad0[64];
    f32 unk40[4];
    s8 unk50[4];
    s8 unk54[4];
    s32 unk58[4];
    s16 unk68;
} Unk80027934;

// siuze: 0x1C
typedef struct Unk800B19A0 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 unk18;
    u8 unk19;
    s8 unk1A;
    s8 unk1B;
} Unk800B19A0;

// size: 0x3C
typedef struct Unk800B199C {
    s32 unk0;
    Object *unk4;
    Object *unk8;
    u8 padC[0x3C - 0xC];
} Unk800B199C;


// size: 0x30
typedef struct Unk80030A24 {
    Vec3f *unk0;
    Vec3f *unk4;
    Vec3f unk8;
    Vec3f unk14;
    f32 unk20;
    f32 unk24;
    s32 unk28;
    s32 unk2C;
} Unk80030A24;

// size: 0x10
typedef struct Unk8002F498_20 {
    s8 unk0;
    u8 pad1[0x10 - 0x1];
} Unk8002F498_20;

// size: 0x70
typedef struct Unk8002F498 {
    Vec3f *unk0;
    f32 *unk4;
    s32 pad8;
    f32 *unkC;
    f32 *unk10;
    s32 pad14;
    u8 *unk18;
    s32 pad1C;
    Unk8002F498_20* unk20;
    u8 pad24[0x48 - 0x24];
    s32 pad48;
    s32 pad4C;
    s32 pad50;
    f32 *unk54;
    u8 pad58[0x6F - 0x58];
    u8 unk6F;
} Unk8002F498;

// size: 0xC
typedef struct Unk800B20B8 {
    f32 unk0;
    f32 unk4;
    Object *unk8;
} Unk800B20B8;


void func_80028D90(void);
void func_8002B410(Object *, s32);
void update_object(Object *);
void func_8001943C(Object *object, MtxF *mf, f32 yPrescale, f32 arg3);
void func_8001A1D4(Model *model, AnimState *animState, s32 count);
void func_80026AB8(Object *obj, ModelInstance *modelInstance, s32 arg2, ObjectHitInfo *objHitInfo, s32 arg4, s32 arg5);
void func_8002B5C0(Object *obj);
u8 func_8005509C(s32 arg0, f32* arg1, f32* arg2, s32 arg3, Unk80027934* arg4, u8 arg5);
void func_800287E4(Object *obj, Object *otherObj, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
void func_80032804(Object*);
u8 func_80029C04(Object *obj, Object *obj2, Object *obj3, s8 arg3, s8 arg4, u32 arg5, u32 arg6);
void func_8002949C(Object *obj, Object *obj2, Object *obj3, ObjectHitInfo *objHitInfo, ObjectHitInfo *objHitInfo2, f32 arg5);
void func_80029AB4(ModelJoint *joints, s32 jointsCount, HitSphere *hitSpheres, s32 hitSpheresCount, s32 arg4, s32 arg5);
void func_8001AC44(ModelInstance*, Model*, Object*, MtxF*, MtxF*, u32, f32);
void func_8001B4F0(MtxF**, MtxF*, AnimState*, ModelJoint*, s32, s16*, s32, s32);
void func_80029A14(Model *model, AnimState *animState, AnimState *arg2, f32 arg3, u8 arg4);
void func_80030994(ModelInstance* modelInstance, f32 arg1, f32 arg2);
void func_80030E2C(Object *obj, f32 *arg1, Vec3f *arg2, Vec3f *arg3, Vec3f *arg4);
void func_80027DAC(Object* obj, Object* obj2, Unk80030A24* arg2, ModelInstance_0x14_0x14* arg3, ModelInstance_0x14_0x14* arg4, s32* arg5, s32* arg6, s32 arg7);
s32 func_8002D0DC(Vec3f* arg0, f32 arg1, Object* obj, Unk80030A24* arg3, ModelInstance_0x14* arg4, Model *arg5, Unk80030A24* arg6, f32 arg7, f32 arg8, Vec3f* arg9);
s32 func_8002D69C(Vec3f* arg0, f32 arg1, Object* obj, Unk80030A24* arg3, ModelInstance_0x14* arg4, Model *arg5, Unk80030A24* arg6, f32 arg7, f32 arg8, Vec3f* arg9);
s32 func_8002DFB8(Vec3f arg0, f32 arg1, ModelInstance_0x14* arg2, Model* model, Unk80030A24* arg4, Unk80030A24** arg5, f32* arg6);
s32 func_8002E3D0(Vec3f arg0, f32 arg1, ModelInstance_0x14* arg2, Model* model, Unk80030A24* arg4, Unk80030A24** arg5, f32 arg6, f32 arg7, f32* arg8);
void func_8002F498(Vec3f* arg0, ModelInstance_0x14* arg1, Model* model, ModelInstance_0x14_0x14* arg3, ModelInstance_0x14_0x14* arg4);
s32 func_8002F998(ModelInstance_0x14 *arg0, Model* arg1, Vec3f *arg2, ModelInstance_0x14_0x14* arg3, ModelInstance_0x14_0x14* arg4, s32* arg5, s32* arg6, ModelInstance_0x14 *arg7, Model* arg8, Vec3f* arg9, ModelInstance_0x14_0x14* argA, ModelInstance_0x14_0x14* argB, Unk80030A24* argC, f32* argD);
void func_80030A24(Object *arg0, Object *arg1, Unk80030A24* arg2, f32 arg3, s32 arg4, Vec3f* arg5);
void func_80027934(Object *obj, Object *otherObj);
void func_80028DCC(Object* obj, Object* obj2, Object* obj3, Object* obj4, f32 arg4);
void func_80028238(Object *obj, Object *otherObj);
void func_8002B7CC(Unk800B20B8** arg0, s32 arg1);

#endif
