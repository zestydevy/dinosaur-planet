#include "common.h"
#include "sys/map.h"
#include "sys/gfx/animation.h"
#include "sys/gfx/model.h"
#include "dlls/objects/1002.h"

typedef struct Unk80026DF4{
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 pad6;
    u8 unk8;
    f32 unkC;
    s32 pad10;
} Unk80026DF4;

typedef struct Unk80027934 {
    u8 pad0[64];
    f32 unk40[4];
    s8 unk50[4];
    s8 unk54[4];
    s32 unk58[4];
    s16 unk68;
} Unk80027934;

typedef struct Unk800B19A0 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    s8 unk18;
    s8 unk19;
    s8 unk1A;
    s8 unk1B;
} Unk800B19A0;
extern Unk800B19A0 *D_800B19A0[]; // 0x40 (64) elements?

typedef struct Unk800B199C {
    s32 unk0;
    Object *unk4;
    Object *unk8;
    u8 padC[0x3C - 0xC];
} Unk800B199C;

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

extern f32 D_800B1990;
extern Object **D_800B1994;
extern s32 D_800B1998;
extern Unk800B199C* D_800B199C; // 30 elements
extern MtxF* D_800B20A0;
extern void* D_800B20A8;
extern void* D_800B20AC;
extern void* D_800B20B0;
extern void* D_800B20B4;
extern s32 *D_800916E0[5];
extern s32 D_800916F4[4];
extern f32 D_80099AC8;
extern f32 D_80099ACC;

void alloc_some_object_arrays(void) {
    D_800B1994 = malloc(0xA0, 0xE, NULL);
    D_800B199C = malloc(0x708, 0xE, NULL);
    D_800B20A0 = malloc(0x1900, 0xE, NULL);
    D_800B20A8 = malloc(0x400, 0xE, NULL);
    D_800B20AC = malloc(0x400, 0xE, NULL);
    D_800B20B0 = malloc(0x400, 0xE, NULL);
    D_800B20B4 = malloc(0x400, 0xE, NULL);
    D_800B1990 = 2.0f;
    func_80028D90();
}

Object **func_80025DD4(s32 *arg0) {
    *arg0 = D_800B1998;
    return D_800B1994;
}

void func_80025DF0(void) {
    s32 i;

    for (i = 0; i < 30; i++) {
        D_800B199C[i].unk0 = 0;
    }

    D_800B1998 = 0;
}

void func_80025E58(void) {
    s32 i;

    i = 0;
    for (i = 0; i < D_800B1998; i++) {
        if (!(D_800B1994[i]->def->flags & 0x40) && (D_800B1994[i]->updatePriority != 0x64)) {
            update_object(D_800B1994[i]);
        }
    }
    for (i = 0; i < D_800B1998; i++) {
        func_8002B410(D_800B1994[i], 1);
    }
}

s32 func_80025F40(Object* obj, s32 *arg1, s32 *arg2, s32 *arg3) {
    ObjectHitInfo* objHitInfo;
    s8 var_a0;
    s8 var_a1;
    s32 i;

    objHitInfo = obj->objhitInfo;
    if (objHitInfo == NULL) {
        return 0;
    }
    if (objHitInfo->unk_0x62 != 0) {
        var_a0 = 0x7F;
        var_a1 = -1;
        for (i = 0; i < objHitInfo->unk_0x62; i++) {
            if (objHitInfo->unk_0x66[i] < var_a0) {
                var_a0 = objHitInfo->unk_0x66[i];
                var_a1 = i;
            }
        }
        if (var_a1 != -1) {
            if (arg1 != 0) {
                *arg1 = objHitInfo->unk_0x6c[var_a1];
            }
            if (arg2 != 0) {
                *arg2 = objHitInfo->unk_0x63[var_a1];
            }
            if (arg3 != 0) {
                *arg3 = objHitInfo->unk_0x69[var_a1];
            }
            return var_a0;
        }
    }
    return 0;
}

s8 func_8002601C(Object* arg0, s32* arg1, s32* arg2, s32* arg3, f32* arg4, f32* arg5, f32* arg6) {
    ObjectHitInfo* objHitInfo;
    s8 var_a0;
    s8 var_a1;
    s32 i;

    objHitInfo = arg0->objhitInfo;
    if (objHitInfo == NULL) {
        return 0;
    }

    if (objHitInfo->unk_0x62 != 0) {
        var_a0 = 0x7F;
        var_a1 = -1;
        for (i = 0; i < objHitInfo->unk_0x62; i++) {
            if (objHitInfo->unk_0x66[i] < var_a0) {
                var_a0 = objHitInfo->unk_0x66[i];
                var_a1 = i;
            }
        }
        if (var_a1 != -1) {
            if (arg1 != 0) {
                *arg1 = objHitInfo->unk_0x6c[var_a1];
            }
            if (arg2 != 0) {
                *arg2 = objHitInfo->unk_0x63[var_a1];
            }
            if (arg3 != 0) {
                *arg3 = objHitInfo->unk_0x69[var_a1];
            }
            if (arg4 != NULL) {
                *arg4 = objHitInfo->unk_0x78[var_a1];
                *arg5 = objHitInfo->unk_0x84[var_a1];
                *arg6 = objHitInfo->unk_0x90[var_a1];
            }
            return var_a0;
        }
    }
    return 0;
}

void func_80026128(Object* arg0, s32 arg1, s32 arg2, s32 arg3) {
    ObjectHitInfo* objHitInfo;

    objHitInfo = arg0->objhitInfo;
    if (objHitInfo == NULL) {
        return;
    }

    objHitInfo->unk_0x5f = arg1;
    objHitInfo->unk_0x60 = arg2;
    if (arg3 == -1) {
        return;
    }

    objHitInfo->unk_0x40 = 1 << (arg3 + 4);
    objHitInfo->unk_0x44 = 1 << (arg3 + 4);
}

void func_80026160(Object* obj) {
    ObjectHitInfo* objHitInfo;

    objHitInfo = obj->objhitInfo;
    if (objHitInfo != NULL) {
        objHitInfo->unk_0x5f = 0;
        objHitInfo->unk_0x60 = 0;
        objHitInfo->unk_0x40 = 0;
        objHitInfo->unk_0x44 = 0;
    }
}

void func_80026184(Object* arg0, s32 arg1) {
    ObjectStruct58* obj58;
    s32 i;

    obj58 = arg0->unk0x58;
    if (obj58->unk10f < 3) {
        for (i = 0; i < obj58->unk10f; i++) {
            if (arg1 == obj58->unk100[i]) {
                return;
            }
        }
        
        obj58->unk100[obj58->unk10f] = arg1;
        obj58 = arg0->unk0x58;
        obj58->unk10f += 1;
    }
}

s32 func_800261E8(Object* arg0, Object* arg1, s8 arg2, s8 arg3, s8 arg4, f32 arg5, f32 arg6, f32 arg7) {
    ObjectHitInfo* objHitInfo;
    ObjectHitInfo* objHitInfo2;
    s32 i;

    if (arg2 == 0) {
        return FALSE;
    }

    objHitInfo = arg0->objhitInfo;
    if (!(objHitInfo->unk_0x58 & 1)) {
        return FALSE;
    }

    if (arg1 != NULL) {
        objHitInfo2 = arg1->objhitInfo;
        if (objHitInfo2 != NULL) {
            objHitInfo2->unk_0x48 = (s32)arg0;
        }
    }

    for (i = 0; i < objHitInfo->unk_0x62; i++) {
        if ((s32)arg1 == objHitInfo->unk_0x6c[i]) {
            if (arg2 < objHitInfo->unk_0x66[i]) {
                objHitInfo->unk_0x63[i] = arg4;
                objHitInfo->unk_0x66[i] = arg2;
                objHitInfo->unk_0x69[i] = arg3;
                objHitInfo->unk_0x78[i] = arg5;
                objHitInfo->unk_0x84[i] = arg6;
                objHitInfo->unk_0x90[i] = arg7;
            }
            i = objHitInfo->unk_0x62 + 1;
        }
    }

    if (i == objHitInfo->unk_0x62 && objHitInfo->unk_0x62 < 3) {
        objHitInfo->unk_0x63[objHitInfo->unk_0x62] = arg4;
        objHitInfo->unk_0x66[objHitInfo->unk_0x62] = arg2;
        objHitInfo->unk_0x69[objHitInfo->unk_0x62] = arg3;
        objHitInfo->unk_0x6c[objHitInfo->unk_0x62] = (s32)arg1;
        objHitInfo->unk_0x78[objHitInfo->unk_0x62] = arg5;
        objHitInfo->unk_0x84[objHitInfo->unk_0x62] = arg6;
        objHitInfo->unk_0x90[objHitInfo->unk_0x62] = arg7;
        objHitInfo->unk_0x62 += 1;
    }

    return TRUE;
}

s32 func_8002635C(Object* arg0, Object* arg1, s8 arg2, s8 arg3, s8 arg4) {
    ObjectHitInfo* objHitInfo;
    ObjectHitInfo* objHitInfo2;
    s32 i;

    if (arg2 == 0) {
        return FALSE;
    }

    objHitInfo = arg0->objhitInfo;
    if (!(objHitInfo->unk_0x58 & 1)) {
        return FALSE;
    }

    if (arg1 != NULL) {
        objHitInfo2 = arg1->objhitInfo;
        if (objHitInfo2 != NULL) {
            objHitInfo2->unk_0x48 = (s32) arg0;
        }
    }

    for (i = 0; i < objHitInfo->unk_0x62; i++) {
        if ((s32)arg1 == objHitInfo->unk_0x6c[i]) {
            if (arg2 < objHitInfo->unk_0x66[i]) {
                objHitInfo->unk_0x63[i] = arg4;
                objHitInfo->unk_0x66[i] = arg2;
                objHitInfo->unk_0x69[i] = arg3;
                objHitInfo->unk_0x78[i] = arg0->srt.transl.x;
                objHitInfo->unk_0x84[i] = arg0->srt.transl.y;
                objHitInfo->unk_0x90[i] = arg0->srt.transl.z;
            }
            i = objHitInfo->unk_0x62 + 1;
        }
    }

    if ((i == objHitInfo->unk_0x62) && (objHitInfo->unk_0x62 < 3)) {
        objHitInfo->unk_0x63[objHitInfo->unk_0x62] = arg4;
        objHitInfo->unk_0x66[objHitInfo->unk_0x62] = arg2;
        objHitInfo->unk_0x69[objHitInfo->unk_0x62] = arg3;
        objHitInfo->unk_0x6c[objHitInfo->unk_0x62] = (s32)arg1;
        objHitInfo->unk_0x78[objHitInfo->unk_0x62] = arg0->srt.transl.x;
        objHitInfo->unk_0x84[objHitInfo->unk_0x62] = arg0->srt.transl.y;
        objHitInfo->unk_0x90[objHitInfo->unk_0x62] = arg0->srt.transl.z;
        objHitInfo->unk_0x62 += 1;
    }

    return TRUE;
}

void func_800264D0(Object* arg0) {
    ObjectHitInfo* objHitInfo;
    f32 temp_fv0;
    s16 var_a3;
    s16 var_t0;

    objHitInfo = arg0->objhitInfo;
    if (objHitInfo == NULL) {
        return;
    }

    objHitInfo->unk_0x58 = arg0->def->helpTexts[0];
    objHitInfo->unk_0x5a = arg0->def->unk93;
    objHitInfo->unk_0x5b = arg0->def->_unk91[0];
    objHitInfo->unk_0x5c = arg0->def->_unk91[1];
    objHitInfo->unk_0x52 = arg0->def->hitbox_flagsB6;
    objHitInfo->unk_0x54 = arg0->def->unk94;
    objHitInfo->unk_0x56 = arg0->def->unk96;
    objHitInfo->unk_0xa0 = arg0->def->colorIdx;
    objHitInfo->unk_0x50 = 0x400;
    temp_fv0 = (f32) objHitInfo->unk_0x52;
    objHitInfo->unk_0xc = temp_fv0 * temp_fv0;
    objHitInfo->unk_0x2c = arg0->unk_0xa8;
    if (objHitInfo->unk_0x5a & 2) {
        if (objHitInfo->unk_0x54 < 0) {
            var_a3 = objHitInfo->unk_0x54 * -1;
        } else {
            var_a3 = objHitInfo->unk_0x54;
        }
        if (objHitInfo->unk_0x56 < 0) {
            var_t0 = objHitInfo->unk_0x56 * -1;
        } else {
            var_t0 = objHitInfo->unk_0x56;
        }
        if (var_t0 < var_a3) {
            var_t0 = var_a3;
        }
        if (objHitInfo->unk_0x2c < var_t0) {
            objHitInfo->unk_0x2c = var_t0;
        }
    } else if (objHitInfo->unk_0x5a & 1) {
        if (objHitInfo->unk_0x2c < objHitInfo->unk_0x52) {
            objHitInfo->unk_0x2c = objHitInfo->unk_0x52;
        }
    }

    objHitInfo->unk_0x30 = arg0->unk_0xa8;
    if ((objHitInfo->unk_0x5a & 2) || (objHitInfo->unk_0x5a & 1)) {
        if (objHitInfo->unk_0x30 < objHitInfo->unk_0x52) {
            objHitInfo->unk_0x30 = objHitInfo->unk_0x52;
        }
    }
}

u32 func_8002667C(Object* obj, u32 addr) {
    ObjectHitInfo* objHitInfo;

    objHitInfo = (ObjectHitInfo *)align_4(addr);
    obj->objhitInfo = objHitInfo;
    addr = (u32)(objHitInfo + 1);
    func_800264D0(obj);
    objHitInfo->unk_0x9e = 1;
    if (objHitInfo->unk_0x5a & 0x30) {
        objHitInfo->unk_0x9f = 2;
    }
    return addr;
}

void func_800266E0(Object* obj) {
    ObjectHitInfo* objhitInfo;

    objhitInfo = obj->objhitInfo;
    if (objhitInfo != NULL) {
        objhitInfo->unk_0x10.x = obj->srt.transl.x;
        objhitInfo->unk_0x10.y = obj->srt.transl.y;
        objhitInfo->unk_0x10.z = obj->srt.transl.z;
        objhitInfo->unk_0x20.x = obj->positionMirror.x;
        objhitInfo->unk_0x20.y = obj->positionMirror.y;
        objhitInfo->unk_0x20.z = obj->positionMirror.z;
    }
}

s32 func_80026724(Object* obj) {
    ObjectHitInfo* objhitInfo;

    objhitInfo = obj->objhitInfo;
    if (objhitInfo != NULL) {
        return objhitInfo->unk_0x58 & 1;
    }

    return 0;
}

void func_8002674C(Object* obj) {
    ObjectHitInfo* objhitInfo;

    objhitInfo = obj->objhitInfo;
    if (objhitInfo == NULL) {
        return;
    }

    if (!(objhitInfo->unk_0x58 & 1)) {
        objhitInfo->unk_0x58 |= 1;
        objhitInfo->unk_0x10.x = obj->srt.transl.x;
        objhitInfo->unk_0x10.y = obj->srt.transl.y;
        objhitInfo->unk_0x10.z = obj->srt.transl.z;
        objhitInfo->unk_0x20.x = obj->positionMirror.x;
        objhitInfo->unk_0x20.y = obj->positionMirror.y;
        objhitInfo->unk_0x20.z = obj->positionMirror.z;
    }
}

void func_800267A4(Object* obj) {
    ObjectHitInfo* objhitInfo;

    objhitInfo = obj->objhitInfo;
    if (objhitInfo != NULL) {
        objhitInfo->unk_0x58 &= ~1;
    }
}

void func_800267C4(Object* obj) {
    ObjectHitInfo* objhitInfo;

    objhitInfo = obj->objhitInfo;
    if (objhitInfo == NULL) {
        return;
    }

    if (objhitInfo->unk_0x58 & 0x40) {
        objhitInfo->unk_0x58 &= ~0x40;
        objhitInfo->unk_0x10.x = obj->srt.transl.x;
        objhitInfo->unk_0x10.y = obj->srt.transl.y;
        objhitInfo->unk_0x10.z = obj->srt.transl.z;
        objhitInfo->unk_0x20.x = obj->positionMirror.x;
        objhitInfo->unk_0x20.y = obj->positionMirror.y;
        objhitInfo->unk_0x20.z = obj->positionMirror.z;
    }
}

void func_8002681C(Object* obj) {
    ObjectHitInfo* objhitInfo;

    objhitInfo = obj->objhitInfo;
    if (objhitInfo != NULL) {
        objhitInfo->unk_0x58 |= 0x40;
    }
}

void func_8002683C(Object* arg0, s16 arg1, s16 arg2, s16 arg3) {
    ObjectHitInfo* objhitInfo;
    f32 var_fv0;

    objhitInfo = arg0->objhitInfo;
    if (objhitInfo == NULL) {
        return;
    }
    
    if (!(objhitInfo->unk_0x5a & 2)) {
        return;
    }

    objhitInfo->unk_0x52 = arg1;
    objhitInfo->unk_0x54 = arg2;
    objhitInfo->unk_0x56 = arg3;
    var_fv0 = objhitInfo->unk_0x52;
    objhitInfo->unk_0x50 = 0x400;
    objhitInfo->unk_0xc = var_fv0 * var_fv0;
    objhitInfo->unk_0x2c = arg0->unk_0xa8;
    if (arg2 < 0) {
        arg2 = -arg2;
    }
    if (arg3 < 0) {
        arg3 = -arg3;
    }
    if (arg3 < arg2) {
        arg3 = arg2;
    }
    if (objhitInfo->unk_0x2c < arg3) {
        objhitInfo->unk_0x2c = arg3;
    }
    objhitInfo->unk_0x30 = arg0->unk_0xa8;
    var_fv0 = objhitInfo->unk_0x52;
    if (objhitInfo->unk_0x30 < var_fv0) {
        objhitInfo->unk_0x30 = var_fv0;
    }
}

void func_80026940(Object* obj, s16 arg1) {
    ObjectHitInfo* objhitInfo;
    f32 temp_fv0;

    objhitInfo = obj->objhitInfo;
    if ((objhitInfo != NULL) && (objhitInfo->unk_0x5a & 1)) {
        objhitInfo->unk_0x52 = arg1;
        temp_fv0 = (f32) objhitInfo->unk_0x52;
        objhitInfo->unk_0xc = temp_fv0 * temp_fv0;
        objhitInfo->unk_0x2c = obj->unk_0xa8;
        if (objhitInfo->unk_0x2c < temp_fv0) {
            objhitInfo->unk_0x2c = temp_fv0;
        }
        objhitInfo->unk_0x30 = obj->unk_0xa8;
        if (objhitInfo->unk_0x30 < temp_fv0) {
            objhitInfo->unk_0x30 = temp_fv0;
        }
    }
}

void func_800269CC(Object* obj, ObjectHitInfo* objHitInfo, s32 arg2) {
    if (arg2 >= obj->def->numModels) {
        arg2 = obj->def->numModels - 1;
    } else if (arg2 < 0) {
        arg2 = 0;
    }

    func_8002B5C0(obj);
    objHitInfo->unk_0xa0 = arg2;
}

u32 func_80026A20(s32 objId, ModelInstance* modelInstance, ObjectHitInfo* objHitInfo, u32 arg3, Object* obj) {
    if (modelInstance == NULL) {
        return arg3;
    }

    objHitInfo->unk_0x6 = 0x12C;
    objHitInfo->unk_0x8 = (u32 *)align_8(arg3);
    arg3 = (s32)objHitInfo->unk_0x8 + objHitInfo->unk_0x6;
    objHitInfo->unk_0x9e = 1;
    if (objHitInfo->unk_0x5a & 0x30) {
        objHitInfo->unk_0x9f = 2;
    }
    func_80026AB8(obj, modelInstance, objId, objHitInfo, 0, 1);
    return arg3;
}

void func_80026AB8(Object* obj, ModelInstance* modelInstance, s32 arg2, ObjectHitInfo* objHitInfo, s32 arg4, s32 arg5) {
    ObjDefHit* temp_a2;
    s32 temp_a2_2;
    s32 i;

    temp_a2 = obj->def->pHits;
    objHitInfo->unk_0x4 = 0;
    if (temp_a2 == NULL) {
        return;
    }

    for (i = 0; temp_a2[i].id != -1; i++) {
        if (arg4 != temp_a2[i].id) {
            continue;
        }

        temp_a2_2 = temp_a2[i].fileOffsetInBytes;
        objHitInfo->unk_0x4 = temp_a2[i].fileSizeInBytes;
        if (objHitInfo->unk_0x6 < objHitInfo->unk_0x4) {
            objHitInfo->unk_0x4 = objHitInfo->unk_0x6;
        }

        if (arg5 == 0) {
            queue_load_file_region_to_ptr((void **)objHitInfo->unk_0x8, 0x45, temp_a2_2, objHitInfo->unk_0x4);
        } else {
            read_file_region(0x45U, (void *)objHitInfo->unk_0x8, temp_a2_2, objHitInfo->unk_0x4);
        }
        return;
    }
}

void func_80026B84(Object* obj) {
    ObjectStruct58* obj58;

    obj58 = obj->unk0x58;
    if (obj58 != NULL) {
        obj58->unk10c = 0;
        obj->unk0x58->unk10d = 0xA;
        obj->unk0x58->unk10f = 0;
        func_8002B410(obj, 1);
        func_8002B410(obj, 1);
    }
}

u32 func_80026BD8(Object* obj, u32 addr) {
    obj->unk0x58 = (ObjectStruct58 *)align_4(addr);
    addr = (u32)(obj->unk0x58 + 1);
    func_80026B84(obj);
    return addr;
}

#if 1
void update_obj_hitboxes(s32 arg0);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/update_obj_hitboxes.s")
#else
// https://decomp.me/scratch/vqoW9
void update_obj_hitboxes(s32 arg0) {
    Object* curObj;
    Object** objects;
    Object** var_t0;
    ObjectHitInfo* objhitInfo;
    s32 numObjs;
    s32 i;
    s32 pad[2];

    objects = get_world_objects(&i, &numObjs);
    D_800B1998 = 0;

    if (arg0 > 0) {
        var_t0 = &objects[0];
        do {
            curObj = var_t0[0];
            objhitInfo = curObj->objhitInfo;
            if (objhitInfo != NULL && (objhitInfo->unk_0x58 & 1) && (objhitInfo->unk_0x5a & 8)) {
                if (D_800B1998 < 40) {
                    D_800B1994[D_800B1998] = curObj;
                    D_800B1998 += 1;
                }
                objhitInfo->unk_0x0 = 0;
                objhitInfo->unk_0x50 = 0x400;
                objhitInfo->unk_0x58 &= ~8;
            }
            var_t0 += 1;
        } while (var_t0 != &objects[arg0]);
    }
}
#endif

u8 func_80026DF4(Object* obj, Unk80026DF4* arg1, u8 arg2, u8 arg3, f32* arg4) {
    SRT sp70;
    s32 i;
    Unk80026DF4* sp68;
    DLL_1002* loadedDLL;
    ModelInstance *modelInst;
    s32 sp5C;
    f32 sp58;
    s32 sp48[4] = D_800916F4;

    if (arg3 != 0 && func_80024108(obj, *arg4, delayFloat, 0)) {
        arg3 = 0;
    }
    i = func_8002601C(obj, NULL, &sp5C, NULL, &sp70.transl.x, &sp70.transl.y, &sp70.transl.z);
    if (i != 0) {
        modelInst = obj->modelInsts[obj->modelInstIdx];
        sp70.transl.x += gWorldX;
        sp70.yaw = 0;
        sp70.pitch = 0;
        sp70.transl.z += gWorldZ;
        sp70.roll = 0;
        sp70.scale = 1.0f;
        sp5C = modelInst->model->hitSpheres[sp5C].unkC;
        if (sp5C >= arg2) {
            sp5C = 0;
        }
        sp68 = &arg1[sp5C];
        if (i != 15) {
            if (sp68->unk0 >= 0) {
                gDLL_6_AMSFX->vtbl->func2(obj, sp68->unk0, 0x7F, 0, 0, 0, 0);
            }
            if (sp68->unk2 >= 0) {
                gDLL_6_AMSFX->vtbl->func2(obj, sp68->unk2, 0x7F, 0, 0, 0, 0);
            }
            // @fake
            if (!i) {}
            if (sp68->unk8 == 1) {
                loadedDLL = dll_load(0x1002U, 1U, FALSE);
                // @fake
                if (1) {}
                loadedDLL->vtbl->func0(0, 1, &sp70, 0x401, -1, sp48);
                if (loadedDLL != NULL) {
                    dll_unload(loadedDLL);
                }
            } else {
                sp58 = D_80099AC8;
                gDLL_17->vtbl->func1(obj, 0x325, &sp70, 0x200001, -1, &sp58);
                sp70.scale = 92.0f;
                gDLL_17->vtbl->func1(obj, 0x323, &sp70, 0x200001, -1, NULL);
                sp58 = D_80099ACC;
                sp70.scale = 231.0f;
                gDLL_17->vtbl->func1(obj, 0x323, &sp70, 0x200001, -1, &sp58);
                sp70.transl.x -= obj->positionMirror.x;
                sp70.transl.y -= obj->positionMirror.y;
                sp70.transl.z -= obj->positionMirror.z;
                sp70.scale = 123.0f;
                for (i = 0; i < 15; i++) {
                    gDLL_17->vtbl->func1(obj, 0x324, &sp70, 2, -1, NULL);
                }
            }
        }
        if (!arg3) {
            if (sp68->unk4 >= 0) {
                func_80023D30(obj, sp68->unk4, 0.0f, 0);
                *arg4 = sp68->unkC;
                arg3 = 1;
            }
        }
    }
    return arg3;
}

void obj_do_hit_detection(s32 arg0);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/obj_do_hit_detection.s")

#if 1
void func_80027934(Object *obj, Object *otherObj) ;
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80027934.s")
#else
// https://decomp.me/scratch/R1u2I
void func_80027934(Object* obj, Object* otherObj) {
    u32 sp17C;
    Model* temp_fp;
    ModelInstance* temp_v0_2;
    s32 new_var;
    ObjectHitInfo* objectHitInfo; // sp16C
    f32 *temp_s2;
    f32 *temp_s3;
    u16 temp_t7_2;
    s32 var_t2;
    u8 temp_v1_3;
    Unk80027934 spF0;
    AABBs32 spD8;
    f32 spA8[4 * 3];
    f32 sp78[4 * 3];
    s32 temp_t8;
    s32 var_s0;
    s32 i;
    HitSphere* temp_v0_3;

    objectHitInfo = obj->objhitInfo;
    if (otherObj == obj) {
        sp17C = objectHitInfo->unk_0x40 >> 4;
    } else {
        sp17C = objectHitInfo->unk_0x40 & 0xF;
    }
    if (sp17C == 0) {
        return;
    }
    if (objectHitInfo->unk_0x61 != 0) {
        return;
    }

    objectHitInfo = otherObj->objhitInfo;
    if (objectHitInfo->unk_0x5a & 0x10) {
        var_s0 = 0;
        temp_v0_2 = otherObj->modelInsts[otherObj->modelInstIdx];
        temp_fp = temp_v0_2->model;
        temp_t8 = (temp_v0_2->unk_0x34 >> 2) & 1;
        temp_s2 = temp_v0_2->unk_0x1c[temp_t8];
        temp_s3 = temp_v0_2->unk_0x1c[temp_t8 ^ 1];
        for (i = 0; i < temp_fp->hitSphereCount; i++) {
            temp_v0_3 = &temp_fp->hitSpheres[i];
            if ((i == temp_v0_3->unkC) && ((1 << temp_v0_3->unkD) & sp17C)) {
                if (temp_v0_3->unkA != 0) {
                    for (var_t2 = temp_v0_3->unkA; (u16)var_t2 != 0; var_t2 <<= 4) {
                        if (var_s0 < 4) {
                            new_var = var_s0 * 3;
                            temp_t7_2 = (((var_t2 & 0xF000) >> 0xC) + i);
                            spA8[new_var + 0] = temp_s2[temp_t7_2 * 4 + 1] + gWorldX;
                            spA8[new_var + 1] = temp_s2[temp_t7_2 * 4 + 2];
                            spA8[new_var + 2] = temp_s2[temp_t7_2 * 4 + 3] + gWorldZ;
                            sp78[new_var + 0] = temp_s3[temp_t7_2 * 4 + 1] + gWorldX;
                            sp78[new_var + 1] = temp_s3[temp_t7_2 * 4 + 2];
                            sp78[new_var + 2] = temp_s3[temp_t7_2 * 4 + 3] + gWorldZ;
                            spF0.unk40[var_s0] = temp_s2[temp_t7_2 * 4 + 0];
                            spF0.unk50[var_s0] = -1;
                            spF0.unk54[var_s0] = 6;
                            var_s0++;
                        }
                    }
                } else if (var_s0 < 4) {
                    new_var = var_s0 * 3;
                    spA8[new_var + 0] = temp_s2[i * 4 + 1] + gWorldX;
                    spA8[new_var + 1] = temp_s2[i * 4 + 2];
                    spA8[new_var + 2] = temp_s2[i * 4 + 3] + gWorldZ;
                    sp78[new_var + 0] = temp_s3[i * 4 + 1] + gWorldX;
                    sp78[new_var + 1] = temp_s3[i * 4 + 2];
                    sp78[new_var + 2] = temp_s3[i * 4 + 3] + gWorldZ;
                    spF0.unk40[var_s0] = temp_s2[i * 4 + 0];
                    spF0.unk50[var_s0] = -1;
                    spF0.unk54[var_s0] = 6;
                    var_s0++;
                }
            }
            
        }
    } else {
        var_s0 = 1;
        spA8[0] = obj->srt.transl.x;
        spA8[1] = obj->srt.transl.y;
        spA8[2] = obj->srt.transl.z;
        sp78[0] = objectHitInfo->unk_0x10.x;
        sp78[1] = objectHitInfo->unk_0x10.y;
        sp78[2] = objectHitInfo->unk_0x10.z;
        spF0.unk40[0] = (f32) objectHitInfo->unk_0x52;
        spF0.unk50[0] = -1;
        spF0.unk54[0] = 6;
    }

    if (var_s0 == 0) {
        return;
    }

    fit_aabb_around_cubes(&spD8, (Vec3f *) sp78, (Vec3f *) spA8, spF0.unk40, var_s0);
    func_80053750(otherObj, (Vec3f** ) &spD8, objectHitInfo->unk_0xa1);
    temp_v1_3 = func_8005509C(otherObj, (Vec3f *) sp78, (Vec3f *) spA8, var_s0, &spF0, 0);
    if (!temp_v1_3) {
        return;
    }

    if (temp_v1_3 & 1) {
        var_s0 = 0;
    } else if (temp_v1_3 & 2) {
        var_s0 = 1;
    } else if (temp_v1_3 & 4) {
        var_s0 = 2;
    } else {
        var_s0 = 3;
    }
    objectHitInfo->unk_0x9c = spF0.unk50[var_s0];
    objectHitInfo->unk_0x34 = spA8[var_s0 * 3 + 0];
    objectHitInfo->unk_0x38 = spA8[var_s0 * 3 + 1];
    objectHitInfo->unk_0x3c = spA8[var_s0 * 3 + 2];
    if (spF0.unk58[var_s0] != 0) {
        objectHitInfo->unk_0x9d |= 2;
    } else {
        objectHitInfo->unk_0x9d |= 1;
    }
}
#endif

void func_80027DAC(Object *obj, Object *obj2, Object *obj3, s32 *arg3, s32 *arg4, s32 *arg5, s32 *arg6, s32 arg7);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80027DAC.s")

#ifndef NON_EQUIVALENT
void func_80028238(Object *obj, Object *otherObj);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80028238.s")
#else
// https://decomp.me/scratch/Zv5Yv
void func_80028238(Object* obj, Object* otherObj) {
    f32 spA4;
    f32 temp_fa1;
    f32 sp9C;
    f32 sp98;
    f32 sp94;
    f32 temp_ft4_2;
    f32 sp8C;
    f32 sp88;
    f32 sp84;
    f32 temp_fv0_2;
    f32 temp_fv1;
    f32 var_fa1;
    f32 var_ft4_2;
    f32 sp70;
    f32 sp6C;
    f32 sp68;
    s32 var_v0;
    f32 sp60;
    f32 sp5C;
    ObjectHitInfo* sp58;
    ObjectHitInfo* sp54;
    s8 sp53;
    f32 var_fv0;

    sp58 = obj->objhitInfo;
    sp54 = otherObj->objhitInfo;
    if (sp58->unk_0x9e != 0) {
        return;
    }

    if (sp54->unk_0x9e != 0) {
        return;
    }

    sp70 = otherObj->positionMirror.x - obj->positionMirror.x;
    sp6C = otherObj->positionMirror.y - obj->positionMirror.y;
    sp68 = otherObj->positionMirror.z - obj->positionMirror.z;
    sp60 = sp58->unk_0x52;
    sp5C = sp54->unk_0x52;
    sp53 = 0;
    if (sp54->unk_0x5a & 2 || sp58->unk_0x5a & 2) {
        if (sp6C > 0.0f) {
            if (sp58->unk_0x5a & 2) {
                var_fa1 = obj->positionMirror.y + sp58->unk_0x56;
            } else {
                var_fa1 = obj->positionMirror.y + sp60;
            }
            if (sp54->unk_0x5a & 2) {
                var_fv0 = otherObj->positionMirror.y + sp54->unk_0x54;
            } else {
                var_fv0 = otherObj->positionMirror.y - sp5C;
            }
            if (var_fa1 < var_fv0) {
                return;
            }
        } else {
            if (sp54->unk_0x5a & 2) {
                var_fa1 = otherObj->positionMirror.y + sp54->unk_0x56;
            } else {
                var_fa1 = otherObj->positionMirror.y + sp5C;
            }
            if (sp58->unk_0x5a & 2) {
                var_fv0 = obj->positionMirror.y + sp58->unk_0x54;
            } else {
                var_fv0 = obj->positionMirror.y - sp60;
            }

            if (var_fa1 < var_fv0) {
                return;
            }
        }
        sp53 = 1;
        sp6C = 0.0f;
    }

    spA4 = sqrtf((sp70 * sp70) + (sp6C * sp6C) + (sp68 * sp68));
    var_v0 = (s32) (f32) (s32) spA4;
    if (var_v0 > 0x400) {
        var_v0 = 0x400;
    }
    if (sp58->unk_0x50 >= var_v0) {
        sp58->unk_0x0 = otherObj;
        sp58->unk_0x50 = var_v0;
    }
    var_v0 = (s32) (f32) (s32) spA4;
    if (var_v0 > 0x400) {
        var_v0 = 0x400;
    }
    if (sp54->unk_0x50 >= var_v0) {
        sp54->unk_0x0 = obj;
        sp54->unk_0x50 = var_v0;
    }
    if (!(sp54->unk_0x58 & 1)) {
        return;
    }

    sp8C = obj->positionMirror.x - sp58->unk_0x20.x;
    sp88 = obj->positionMirror.y - sp58->unk_0x20.y;
    sp84 = obj->positionMirror.z - sp58->unk_0x20.z;
    if (sp53 != 0) {
        sp88 = 0.0f;
    }
    temp_fa1 = (sp8C * sp8C) + (sp88 * sp88) + (sp84 * sp84);
    if (temp_fa1 > 1.0f) {
        temp_fv1 = (((otherObj->positionMirror.x - sp58->unk_0x20.x) * sp8C) + ((otherObj->positionMirror.y - sp58->unk_0x20.y) * sp88) + ((otherObj->positionMirror.z - sp58->unk_0x20.z) * sp84)) / temp_fa1;
        if (temp_fv1 >= 0.0f && temp_fv1 <= 1.0f) {
            sp9C = sp58->unk_0x20.x + (temp_fv1 * sp8C);
            sp98 = sp58->unk_0x20.y + (temp_fv1 * sp88);
            sp94 = sp58->unk_0x20.z + (temp_fv1 * sp84);
            temp_fv0_2 = sp9C - otherObj->positionMirror.x;
            temp_fa1 = sp98 - otherObj->positionMirror.y;
            temp_ft4_2 = sp94 - otherObj->positionMirror.z;
            spA4 = sqrtf((temp_fv0_2 * temp_fv0_2) + (temp_fa1 * temp_fa1) + (temp_ft4_2 * temp_ft4_2));
        }
    }
    if ((spA4 < (sp5C + sp60)) && (spA4 > 0.0f)) {
        func_8002635C(otherObj, obj, sp58->unk_0x5d, sp58->unk_0x5e, 0);
        func_8002635C(obj, otherObj, sp54->unk_0x5d, sp54->unk_0x5e, 0);
        if (!(sp54->unk_0x58 & 2) && !(sp58->unk_0x58 & 2)) {
            sp9C = sp54->unk_0x20.x - sp58->unk_0x20.x;
            sp98 = sp54->unk_0x20.y - sp58->unk_0x20.y;
            sp94 = sp54->unk_0x20.z - sp58->unk_0x20.z;
            if (sp53 != 0) {
                sp98 = 0.0f;
            }
            temp_fv0_2 = sqrtf((sp9C * sp9C) + (sp98 * sp98) + (sp94 * sp94));
            if (temp_fv0_2 > 0.0f) {
                sp70 = sp9C / temp_fv0_2;
                sp6C = sp98 / temp_fv0_2;
                sp68 = sp94 / temp_fv0_2;
            } else {
                sp70 /= spA4;
                sp6C /= spA4;
                sp68 /= spA4;
            }
            spA4 = temp_fv0_2 - spA4;
            if (spA4 < 0.0f) {
                spA4 = -spA4;
            }
            sp70 *= spA4;
            sp6C *= spA4;
            sp68 *= spA4;
            func_800287E4(obj, otherObj, sp70, sp6C, sp68, 0);
        }
    }
}
#endif

#ifndef NON_MATCHING
void func_800287E4(Object *obj, Object *otherObj, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_800287E4.s")
#else
void func_800287E4(Object* obj, Object* otherObj, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    f32 sp74;
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_ft0;
    ObjectHitInfo* objhitInfo;
    ObjectHitInfo* otherObjhitInfo;
    s32 v0;
    s32 var_a2;
    f32 temp_fv0;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 temp_fv1;
    s32 var_v1;

    func_80032804(obj);
    objhitInfo = obj->objhitInfo;
    otherObjhitInfo = otherObj->objhitInfo;
    objhitInfo->unk_0x58 |= 8;
    otherObjhitInfo->unk_0x58 |= 8;
    objhitInfo->unk_0x0 = otherObj;
    otherObjhitInfo->unk_0x0 = obj;
    if (obj->parent != NULL) {
        inverse_rotate_point_by_object(arg2, arg3, arg4, &sp50, &sp4C, &sp48, obj->parent);
    } else {
        sp50 = arg2;
        sp4C = arg3;
        sp48 = arg4;
    }

    if (otherObj->parent != NULL) {
        inverse_rotate_point_by_object(arg2, arg3, arg4, &sp44, &sp40, &sp3C, otherObj->parent);
    } else {
        sp44 = arg2;
        sp40 = arg3;
        sp3C = arg4;
    }

    if ((obj->group == 1) && !(otherObjhitInfo->unk_0x58 & 0x400)) {
        obj->srt.transl.x -= sp50;
        obj->srt.transl.y -= sp4C;
        obj->srt.transl.z -= sp48;
        if (arg5 == 0) {
            return;
        }

        obj->positionMirror.x -= arg2;
        obj->positionMirror.y -= arg3;
        obj->positionMirror.z -= arg4;
        return;
    }

    if ((otherObj->group == 1) && !(objhitInfo->unk_0x58 & 0x400)) {
        otherObj->srt.transl.x += sp44;
        otherObj->srt.transl.y += sp40;
        otherObj->srt.transl.z += sp3C;
        if (arg5 == 0) {
            return;
        }

        obj->positionMirror.x += arg2;
        obj->positionMirror.y += arg3;
        obj->positionMirror.z += arg4;
        return;
    }

    var_v1 = otherObjhitInfo->unk_0x5b;
    if (var_v1 == 0) {
        if (objhitInfo->unk_0x5b == 0) {
            return;
        }

        obj->srt.transl.x -= sp50;
        obj->srt.transl.y -= sp4C;
        obj->srt.transl.z -= sp48;
        if (arg5 == 0) {
            return;
        }

        obj->positionMirror.x -= arg2;
        obj->positionMirror.y -= arg3;
        obj->positionMirror.z -= arg4;
        return;
    }

    if (objhitInfo->unk_0x5b == 0) {
        if (var_v1 == 0) {
            return;
        }

        otherObj->srt.transl.x += sp44;
        otherObj->srt.transl.y += sp40;
        otherObj->srt.transl.z += sp3C;
        if (arg5 == 0) {
            return;
        }

        otherObj->positionMirror.x += arg2;
        otherObj->positionMirror.y += arg3;
        otherObj->positionMirror.z += arg4;
        return;
    }

    v0 = arctan2_f(-arg2, -arg4);
    var_v1 = obj->srt.yaw - ((u16)v0);
    if (var_v1 >= 0x8001) {
        var_v1 += 0xFFFF0001;
    }
    if (var_v1 < -0x8000) {
        var_v1 += 0xFFFF;
    }
    var_a2 = otherObj->srt.yaw - ((v0 + 0x8000) & 0xFFFF);
    if (var_a2 >= 0x8001) {
        var_a2 += 0xFFFF0001;
    }
    if (var_a2 < -0x8000) {
        var_a2 += 0xFFFF;
    }
    temp_fv0 = fcos16_precise(var_v1);
    temp_fa0 = temp_fv0 * temp_fv0;
    sp74 = (objhitInfo->unk_0x5b * temp_fa0) + (objhitInfo->unk_0x5c * (1.0f - temp_fa0));
    temp_fv0 = fcos16_precise((s16) var_a2);
    temp_fa0 = temp_fv0 * temp_fv0;
    temp_ft0 = 1.0f - temp_fa0;
    temp_fa1 = (otherObjhitInfo->unk_0x5b * temp_fa0) + (otherObjhitInfo->unk_0x5c * temp_ft0);
    temp_fv1 = temp_fa1 / (sp74 + temp_fa1);
    obj->srt.transl.x -= sp50 * temp_fv1;
    obj->srt.transl.y -= sp4C * temp_fv1;
    obj->srt.transl.z -= sp48 * temp_fv1;
    otherObj->srt.transl.x += sp44 * (1.0f - temp_fv1);
    otherObj->srt.transl.y += sp40 * (1.0f - temp_fv1);
    otherObj->srt.transl.z += sp3C * (1.0f - temp_fv1);
}

#endif

void func_80028D2C(s32* val) {
    s32 i = 0;

    while (i < 5 && D_800916E0[i] != NULL) {
        i++;
    }

    if (i == 5) {
        D_800916E0[0] = val;
        return;
    }

    D_800916E0[i] = val;
}

void func_80028D90(void) {
    s32 i = 0;

    while (i < 5) {
        D_800916E0[i++] = NULL;
    }
}

void func_80028DC4(void) {

}

void func_80028DCC(Object* obj, Object* obj2, Object* obj3, Object* obj4, f32 arg4) {
    ObjectHitInfo* sp44;
    ObjectHitInfo* sp40;
    ObjectHitInfo* sp3C;
    ObjectHitInfo* sp38;
    u32 var_v1;
    s32 sp30;
    u8 sp2F;
    ModelInstance* sp28;
    u8 temp_v0_4;

    sp3C = obj->objhitInfo;
    sp38 = obj2->objhitInfo;
    if (obj3 != NULL) {
        sp44 = obj3->objhitInfo;
    } else {
        sp44 = NULL;
    }
    if (obj4 != NULL) {
        sp40 = obj4->objhitInfo;
    } else {
        sp40 = NULL;
    }
    sp2F = 0;
    var_v1 = sp3C->unk_0x40;
    if ((var_v1 != 0) && (sp3C->unk_0x61 == 0)) {
        if (obj->group == 1) {
            sp28 = obj->modelInsts[obj->modelInstIdx];
            sp30 = ((s32) sp28->unk_0x34 >> 2) & 1;
            if (sp3C->unk_0x58 & 0x2000) {
                bcopy(D_800B20A8, (void* ) sp28->unk_0x1c[sp30], sp28->model->hitSphereCount * 0x10);
                bcopy(D_800B20AC, (void* ) sp28->unk_0x1c[sp30 ^ 1], sp28->model->hitSphereCount * 0x10);
            } else {
                bcopy((void* ) sp28->unk_0x1c[sp30], D_800B20A8, sp28->model->hitSphereCount * 0x10);
                bcopy((void* ) sp28->unk_0x1c[sp30 ^ 1], D_800B20AC, sp28->model->hitSphereCount * 0x10);
            }
            sp28 = obj3->modelInsts[obj3->modelInstIdx];
            sp30 = ((s32) sp28->unk_0x34 >> 2) & 1;
            if (sp3C->unk_0x58 & 0x2000) {
                bcopy(D_800B20B0, (void* ) sp28->unk_0x1c[sp30], sp28->model->hitSphereCount * 0x10);
                bcopy(D_800B20B4, (void* ) sp28->unk_0x1c[sp30 ^ 1], sp28->model->hitSphereCount * 0x10);
                var_v1 = sp3C->unk_0x40;
            } else {
                bcopy((void* ) sp28->unk_0x1c[sp30], D_800B20B0, sp28->model->hitSphereCount * 0x10);
                bcopy((void* ) sp28->unk_0x1c[sp30 ^ 1], D_800B20B4, sp28->model->hitSphereCount * 0x10);
                var_v1 = sp3C->unk_0x40;
                sp3C->unk_0x58 |= 0x2000;
            }
        }
        if (var_v1 >> 4) {
            sp2F = func_80029C04(obj, obj2, obj, 1, 0, var_v1 >> 4, sp3C->unk_0x44 >> 4);
        }
        if ((obj3 != NULL) && (sp2F == 0)) {
            if (sp3C->unk_0x40 & 0xF) {
                sp2F = func_80029C04(obj3, obj2, obj, 1, 0, sp3C->unk_0x40 & 0xF, sp3C->unk_0x44 & 0xF);
            }
        }
        temp_v0_4 = sp2F;
        sp2F = 0;
        if ((((u32)sp2F < temp_v0_4) == 0) && (obj->group == 1)) {
            func_8002949C(obj, obj2, obj3, sp3C, sp44, arg4);
        }
    }
    if (obj2->def->_unk98[1] & 0x80) {
        return;
    }

    var_v1 = sp38->unk_0x40;
    if ((var_v1 != 0) && (sp38->unk_0x61 == 0)) {
        if (obj2->group == 1) {
            sp28 = obj2->modelInsts[obj2->modelInstIdx];
            sp30 = (sp28->unk_0x34 >> 2) & 1;
            if (sp38->unk_0x58 & 0x2000) {
                bcopy(D_800B20A8, (void* ) sp28->unk_0x1c[sp30], sp28->model->hitSphereCount * 0x10);
                bcopy(D_800B20AC, (void* ) sp28->unk_0x1c[sp30 ^ 1], sp28->model->hitSphereCount * 0x10);
            } else {
                bcopy((void* ) sp28->unk_0x1c[sp30], D_800B20A8, sp28->model->hitSphereCount * 0x10);
                bcopy((void* ) sp28->unk_0x1c[sp30 ^ 1], D_800B20AC, sp28->model->hitSphereCount * 0x10);
            }
            sp28 = obj4->modelInsts[obj4->modelInstIdx];
            sp30 = (sp28->unk_0x34 >> 2) & 1;
            if (sp38->unk_0x58 & 0x2000) {
                bcopy(D_800B20B0, (void* ) sp28->unk_0x1c[sp30], sp28->model->hitSphereCount * 0x10);
                bcopy(D_800B20B4, (void* ) sp28->unk_0x1c[sp30 ^ 1], sp28->model->hitSphereCount * 0x10);
                var_v1 = sp38->unk_0x40;
            } else {
                bcopy((void* ) sp28->unk_0x1c[sp30], D_800B20B0, sp28->model->hitSphereCount * 0x10);
                bcopy((void* ) sp28->unk_0x1c[sp30 ^ 1], D_800B20B4, sp28->model->hitSphereCount * 0x10);
                var_v1 = sp38->unk_0x40;
                sp38->unk_0x58 |= 0x2000;
            }
        }
        if (var_v1 >> 4) {
            sp2F = func_80029C04(obj2, obj, obj2, 1, 0, var_v1 >> 4, sp38->unk_0x44 >> 4);
        }
        if ((obj4 != NULL) && (sp2F == 0)) {
            if (sp38->unk_0x40 & 0xF) {
                sp2F = func_80029C04(obj4, obj, obj2, 1, 0, sp38->unk_0x40 & 0xF, sp38->unk_0x44 & 0xF);
            }
        }
        if ((sp2F == 0) && (obj2->group == 1)) {
            func_8002949C(obj2, obj, obj4, sp38, sp40, arg4);
        }
    }
}

void func_8002949C(Object* obj, Object* obj2, Object* obj3, ObjectHitInfo* objHitInfo, ObjectHitInfo* objHitInfo2, f32 arg5) {
    Model* model;
    f32 sp288;
    f32 sp284;
    f32 sp280;
    f32 temp_fv0;
    f32 var_fs0;
    f32 var_fs1;
    s32 attachPointIdx;
    s32 sp26C;
    s32 matrixIdx;
    s32 sp264;
    u32 *sp260;
    u32 pad;
    s32 sp258;
    s32 i;
    s32 var_s4;
    s32 var_v1;
    ModelInstance* sp248;
    ModelInstance* sp244;
    u32 temp_v0;
    AnimState animState; // sp1DC
    s32 stopLoop;
    Model* model3; // sp1D4
    MtxF sp194;
    MtxF* sp190;
    MtxF sp150;
    MtxF sp110;
    MtxF* sp10C;
    s32 tempJointCount;
    s16 sp106;
    SRT spEC;
    s8 sp80[0xEC - 0x80];
    s32 pad2;

    sp26C = arg5;
    sp264 = objHitInfo->unk_0x4 >> 2;
    if (sp264 > 0) {
        sp260 = objHitInfo->unk_0x8;
    } else {
        sp260 = 0;
    }

    if (sp260 == 0) {
        return;
    }

    sp288 = obj->srt.transl.x - objHitInfo->unk_0x10.x;
    sp284 = obj->srt.transl.y - objHitInfo->unk_0x10.y;
    sp280 = obj->srt.transl.z - objHitInfo->unk_0x10.z;
    temp_fv0 = objHitInfo->unk_0x1c;\
    var_fs1 = obj->animTimer - temp_fv0;
    if (var_fs1 < 0.0f) {
        var_fs1 = 0.0f;
    }
    var_fs1 /= arg5;
    sp248 = obj->modelInsts[obj->modelInstIdx];
    model = sp248->model;
    matrixIdx = sp248->unk_0x34 & 1;
    animState.unk_0x58 = 0;
    animState.unk_0x5a = 0;
    animState.unk_0x5c = 0;
    var_fs0 = temp_fv0 + var_fs1;
    tempJointCount = model->jointCount;
    i = 0;
    if (tempJointCount > 0) {
        do {
            sp80[i] = model->joints[i].jointLayer_plusID;
            i++;
        } while (i < model->jointCount);
    }
    sp10C = sp248->matrices[matrixIdx];
    sp248->matrices[matrixIdx] = D_800B20A0;
    func_8001943C(obj, &sp194, 1.0f, 0.0f);
    sp106 = 0x1000;
    if (obj3 != NULL) {
        sp244 = obj3->modelInsts[obj3->modelInstIdx];
        model3 = sp244->model;
        attachPointIdx = obj->unk0xb0 & 0xFFFF & 3;
        sp258 = obj->def->pAttachPoints[attachPointIdx].bones[obj->modelInstIdx];
        spEC.transl.x = obj->def->pAttachPoints[attachPointIdx].pos.x;
        spEC.transl.y = obj->def->pAttachPoints[attachPointIdx].pos.y;
        spEC.transl.z = obj->def->pAttachPoints[attachPointIdx].pos.z;
        spEC.yaw = obj->def->pAttachPoints[attachPointIdx].rot[0];
        spEC.pitch = obj->def->pAttachPoints[attachPointIdx].rot[1];
        spEC.roll = obj->def->pAttachPoints[attachPointIdx].rot[2];
        spEC.scale = 1.0f;
        sp190 = (MtxF*)(((f32*)sp248->matrices[matrixIdx]) + (sp258 << 4));
        matrix_from_srt(&sp150, &spEC);
    } else {
        sp258 = -1;
    }
    stopLoop = FALSE;
    for (var_s4 = 1; var_s4 < sp26C; var_s4++) {
        var_v1 = (s32) (sp264 * var_fs0);
        if (var_v1 >= sp264) {
            var_v1 = sp264 - 1;
        }
        objHitInfo->unk_0x44 = objHitInfo->unk_0x40;
        temp_v0 = sp260[var_v1];
        objHitInfo->unk_0x40 = temp_v0;
        temp_v0 = objHitInfo->unk_0x40;
        if (temp_v0) {
            pad =  temp_v0 >> 4;
            pad2 = temp_v0 & 0xF;
            func_80029A14(model, sp248->animState0, &animState, var_fs0, 1);
            // @fake temp_v0 needs to be set to any value to match
            temp_v0 = 0;
            func_80029AB4(model->joints, model->jointCount, model->hitSpheres, model->hitSphereCount, pad, pad2 ? sp258 : -1);
            temp_fv0 = var_s4 / arg5;
            sp194.m[3][0] = (objHitInfo->unk_0x10.x - gWorldX) + (temp_fv0 * sp288);
            sp194.m[3][1] = objHitInfo->unk_0x10.y + (temp_fv0 * sp284);
            sp194.m[3][2] = (objHitInfo->unk_0x10.z - gWorldZ) + (temp_fv0 * sp280);
            func_8001B4F0(&sp248->matrices[matrixIdx], &sp194, &animState, model->joints, (s32) model->jointCount, &sp106, -1, 0);
            if (pad) {
                func_8001AC44(sp248, model, obj, NULL, sp248->matrices[matrixIdx], pad, obj->srt.scale);
                stopLoop = func_80029C04(obj, obj2, obj, 1, 0, pad, objHitInfo->unk_0x44 >> 4);
            }
            if (stopLoop == 0 && objHitInfo2 != NULL && (pad2)) {
                matrix_concat_4x3(&sp150, sp190, &sp110);
                func_8001AC44(sp244, model3, obj3, &sp110, sp248->matrices[matrixIdx], pad2, obj->srt.scale);
                stopLoop = func_80029C04(obj3, obj2, obj, 1, 0, pad2, objHitInfo->unk_0x44 & 0xF);
            }
        }
        if (stopLoop) {
            // break
            var_s4 = sp26C;
        }
        var_fs0 += var_fs1;
    }
    // @fake
    if ((!obj->def) && (!obj->def)) {}
    sp248->matrices[matrixIdx] = sp10C;
    for (i = 0; i < model->jointCount; i++) {
        model->joints[i].jointLayer_plusID = sp80[i];
    }
}

void func_80029A14(Model* model, AnimState* animState, AnimState* arg2, f32 arg3, u8 arg4) {
    arg2->anims[0] = animState->anims[0];
    arg2->anims[1] = animState->anims[1];
    arg2->anims2[0] = animState->anims2[0];
    arg2->anims2[1] = animState->anims2[1];
    arg2->animIndexes[0] = animState->animIndexes[0];
    arg2->unk_0x60[0] = animState->unk_0x60[0];
    arg2->unk_0x14[0] = animState->unk_0x14[0];
    if (arg4 != 0) {
        arg2->unk_0x4[0] = animState->unk_0x14[0] * arg3;
    } else {
        arg2->unk_0x4[0] = animState->unk_0x4[0];
    }
    arg2->unk_0x34[0] = animState->unk_0x34[0];
    func_8001A1D4(model, arg2, 1);
}

void func_80029AB4(ModelJoint* joints, s32 jointsCount, HitSphere* hitSpheres, s32 hitSpheresCount, s32 arg4, s32 arg5) {
    s16 jointIndex;
    s32 i;

    for (i = 0; i < jointsCount; i++) {
        joints[i].jointLayer_plusID |= 0x80;
    }

    if (arg4 != 0) {
        for (i = 0; i < hitSpheresCount; i++) {
            if ((1 << hitSpheres[i].unkD) & arg4) {
                jointIndex = hitSpheres[i].jointIndex;
                if ((joints[jointIndex].jointLayer_plusID & 0x80)) {
                    while (jointIndex != -1) {
                        joints[jointIndex].jointLayer_plusID &= ~0x80;
                        jointIndex = joints[jointIndex].parentJointID;
                    }
                }
            }
        }
    }

    if (arg5 != -1 && (joints[arg5].jointLayer_plusID & 0x80)) {
        while (arg5 != -1) {
            joints[arg5].jointLayer_plusID &= ~0x80;
            arg5 = joints[arg5].parentJointID;
        }
    }
}

#if 1
u8 func_80029C04(Object *obj, Object *obj2, Object *obj3, s8 arg3, s8 arg4, u32 arg5, u32 arg6);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80029C04.s")
#else
// https://decomp.me/scratch/aFdcV
u8 func_80029C04(Object* obj, Object* obj2, Object* obj3, s8 arg3, s8 arg4, u32 arg5, u32 arg6) {
    f32 sp1CC;
    f32 sp1C8;
    f32 sp1C0;
    f32 sp1BC;
    f32 sp1B8;
    f32 sp1B4;
    f32 sp1B0;
    f32 sp1AC;
    f32 sp1A8;
    f32 sp1A0;
    f32 sp18C;
    f32 sp188;
    f32 sp184;
    f32 sp180;
    f32 sp17C;
    f32 sp174;
    f32* sp170;                                     /* compiler-managed */
    f32* sp16C;
    f32* sp168;                                     /* compiler-managed */
    f32 sp164;
    f32 sp160;
    f32 sp15C;
    f32 sp158;
    Vec3f sp14C;
    f32 sp148;
    Vec3f sp13C;
    f32 sp138;
    Vec3f sp12C;
    f32 sp128;
    HitSphere sp11A;
    HitSphere sp10C;
    HitSphere* sp108;
    HitSphere* sp104;
    s32 sp100;
    s32 spFC;
    s32 spF8;
    s32 spF4;
    s8 spE6;
    ObjectHitInfo* spE0;
    ObjectHitInfo* spDC;
    ObjectHitInfo* spD8;
    s32 spB0[6];
    s32 sp94;
    s32 sp90;
    f32 sp80;
    Model* model;
    ModelInstance* temp_v1;
    ObjectHitInfo* temp_t0;
    ObjectHitInfo* temp_t1;
    ObjectHitInfo* temp_t4;
    ObjectHitInfo* var_t0;
    ObjectHitInfo* var_t1;
    f32 temp_fa0;
    f32 temp_fa0_2;
    f32 temp_fa1;
    f32 temp_fa1_2;
    f32 temp_fs0;
    f32 temp_fs0_2;
    f32 temp_fs3;
    f32 temp_ft3;
    f32 temp_ft4;
    f32 temp_ft5;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fa0;
    f32 var_fa1;
    f32 var_fs0;
    f32 var_fs1;
    f32 var_fs2;
    f32 var_fs3;
    f32 var_fs4;
    f32 var_fs5;
    f32 var_ft1;
    f32 var_ft2;
    f32 var_ft4;
    f32 var_ft5;
    f32* var_t2;
    s32 temp_a0;
    u16 temp_t5;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_t9;
    s32 temp_v0_4;
    s32 temp_v1_3;
    s32 var_fp;
    s32 var_s3;
    s32 var_s4;
    s32 var_s5;
    s32 var_s6;
    s32 var_s7;
    u16 var_v1_4;
    s32 var_s2;
    s32 var_t3;
    u16 temp_a0_2;
    u16 temp_a1;
    u16 var_t5_7;
    u16 var_v1_3;
    u8 var_v0;
    Unk800B19A0* temp_s0;
    f32* temp_s1;
    f32* temp_v0_10;
    f32* temp_v0_11;
    f32* temp_v0_9;
    Unk800B19A0* var_s0;

    temp_t1 = obj2->objhitInfo;
    temp_t0 = obj->objhitInfo;
    temp_t4 = obj3->objhitInfo;
    if ((temp_t1->unk_0x9e != 0) || (temp_t4->unk_0x9e != 0)) {
        return 0U;
    }
    if ((temp_t1->unk_0x9f != 0) || (var_fp = 0, (temp_t4->unk_0x9f != 0))) {
        return 0U;
    }
    var_t3 = 0;
    if (((arg3 != 0) && (temp_t0->unk_0x5a & 0x10)) || ((arg4 != 0) && (temp_t0->unk_0x5a == 0x10))) {
        temp_v1 = obj->modelInsts[obj->modelInstIdx];
        model = temp_v1->model;
        spFC = (s32) model->hitSphereCount;
        sp170 = temp_v1->unk_0x24;
        sp168 = temp_v1->unk_0x1c[(((s32) temp_v1->unk_0x34 >> 2) & 1) ^ 1];
        sp108 = model->hitSpheres;
        if (obj3 != obj) {
            sp1AC = (f32) model->maxAnimatedVertDistance * obj3->srt.scale;
        } else {
            sp1AC = (f32) model->maxAnimatedVertDistance * obj->srt.scale;
        }
        if (obj->srt.flags & 0x4000) {
            return 0U;
        }
    } else {
        spFC = 1;
        sp170 = &sp128;
        sp168 = &sp148;
        sp108 = &sp10C;
        if (temp_t0->unk_0x5a & 2) {
            var_fp = 1;
        }
        sp128 = (f32) temp_t0->unk_0x52;
        sp12C.x = obj->positionMirror.x - gWorldX;
        sp12C.y = obj->positionMirror.y;
        sp148 = sp128;
        sp12C.z = obj->positionMirror.z - gWorldZ;
        sp14C.x = temp_t0->unk_0x20.x - gWorldX;
        sp14C.y = temp_t0->unk_0x20.y;
        sp10C.unkC = 0;
        sp10C.unkD = 0;
        sp10C.unkA = 0;
        sp1AC = sp128;
        sp14C.z = temp_t0->unk_0x20.z - gWorldZ;
    }
    if (((arg3 != 0) && (temp_t1->unk_0x5a & 0x10)) || ((var_t2 = &sp138, (arg4 != 0)) && (temp_t1->unk_0x5a == 0x10))) {
        temp_v1 = obj2->modelInsts[obj2->modelInstIdx];
        model = temp_v1->model;
        sp100 = (s32) model->hitSphereCount;
        var_t2 = (f32* ) temp_v1->unk_0x24;
        sp104 = model->hitSpheres;
        sp1B0 = (f32) model->maxAnimatedVertDistance * obj2->srt.scale;
        if (obj2->srt.flags & 0x4000) {
            return 0U;
        }
    } else {
        sp100 = 1;
        sp104 = &sp11A;
        if (temp_t1->unk_0x5a & 2) {
            var_t3 = 1;
        }
        temp_ft3 = (f32) temp_t1->unk_0x52;
        sp138 = temp_ft3;
        sp13C.x = obj2->positionMirror.x - gWorldX;
        sp13C.y = obj2->positionMirror.y;
        sp158 = sp128;
        sp13C.z = obj2->positionMirror.z - gWorldZ;
        sp15C = temp_t0->unk_0x20.x - gWorldX;
        sp160 = temp_t0->unk_0x20.y;
        sp11A.unkC = 0;
        sp11A.unkD = 0;
        sp11A.unkA = 0;
        sp1B0 = temp_ft3;
        sp164 = temp_t0->unk_0x20.z - gWorldZ;
    }
    var_fs1 = obj->positionMirror.x - obj2->positionMirror.x;
    var_fs4 = obj->positionMirror.y - obj2->positionMirror.y;
    spD8 = temp_t4;
    var_fs2 = obj->positionMirror.z - obj2->positionMirror.z;
    spE6 = var_t3;
    sp16C = var_t2;
    spDC = temp_t1;
    spE0 = temp_t0;
    arg3 = arg3;
    temp_fv0 = sqrtf((var_fs1 * var_fs1) + (var_fs4 * var_fs4) + (var_fs2 * var_fs2));
    var_fs5 = temp_fv0;
    var_s6 = 0;
    if ((sp1B0 + sp1AC + 100.0f) < temp_fv0) {
        return 0U;
    }
    spB0[4] = 0;
    spB0[5] = 0;
    spB0[2] = 0;
    spB0[3] = 0;
    spB0[0] = 0;
    spB0[1] = 0;
    for (var_s6 = 0; var_s6 < spFC; var_s6++) {
        if (var_s6 == sp108[var_s6].unkC) {
            temp_v0_4 = 1 << sp108[var_s6].unkD;
            if (temp_v0_4 & arg5) {
                spB0[5] |= 1 << var_s6;
                spB0[4] |= (1 << var_s6) >> 0x1F;
            }
            if (temp_v0_4 & arg6) {
                spB0[1] |= 1 << var_s6;
                spB0[0] |= (1 << var_s6) >> 0x1F;
            }
        }
    }
    for (var_s6 = 0; var_s6 < sp100; var_s6++) {
        if (var_s6 == sp104[var_s6].unkC) {
            spB0[3] |= 1 << var_s6;
            spB0[2] |= (1 << var_s6) >> 0x1F;
        }
    }

    sp188 = -1.0f;
    var_fs0 = sp18C;
    var_s7 = spF8;
    var_s3 = spF4;
    do {
        var_s4 = 0;
        for (var_s6 = 0; var_s6 < spFC; var_s6++) {
            temp_t6 = 1 << var_s6;
            temp_t8 = temp_t6 >> 0x1F;
            sp94 = temp_t6;
            sp90 = temp_t8;
            if ((temp_t8 & spB0[4]) || (temp_t6 & spB0[5])) {
                temp_v1_3 = var_s6 * 4;
                temp_v0_9 = (f32*)sp170 + temp_v1_3;
                sp1CC = temp_v0_9[0];
                var_s5 = 1;
                var_s2 = 0;
                sp1C8 = temp_v0_9[1];
                temp_fs3 = temp_v0_9[2];
                sp1C0 = temp_v0_9[3];
                if ((temp_t8 & spB0[0]) || (temp_t6 & spB0[1])) {
                    var_s5 = 0;
                }
                if (var_s5 == 0) {
                    temp_v0_10 = (f32*)sp168 + temp_v1_3;
                    temp_ft4 = temp_v0_10[1];
                    temp_ft5 = temp_v0_10[2];
                    temp_fv0 = sp1C8 - temp_ft4;
                    sp1B4 = temp_v0_10[3];
                    temp_fv1 = temp_fs3 - temp_ft5;
                    sp184 = temp_fv0;
                    sp180 = temp_fv1;
                    temp_fa0 = sp1C0 - sp1B4;
                    sp1BC = temp_ft4;
                    sp1B8 = temp_ft5;
                    sp17C = temp_fa0;
                    temp_fa1 = (temp_fv0 * temp_fv0) + (temp_fv1 * temp_fv1) + (temp_fa0 * temp_fa0);
                    sp174 = temp_fa1;
                    if (temp_fa1 > 0.0f) {
                        sp174 = 1.0f / sp174;
                    } else {
                        var_s5 = 1;
                    }
                }
                for (var_s2 = 0; var_s2 < sp100; var_s2++) {
                    temp_t8 = 1 << var_s2;
                    if (((temp_t8 >> 0x1F) & spB0[2]) || (temp_t8 & spB0[3])) {
                        var_s3 = 0;
                        temp_s1 = var_t2 + (var_s2 * 4);
                        if (((var_s6 == 0) && (var_fp != 0)) || ((var_s2 == 0) && (var_t3 != 0))) {
                            if (var_fp != 0) {
                                temp_fv1 = temp_s1[2];
                                temp_fv0 = temp_s1[0];
                                var_fa0 = (f32) temp_t0->unk_0x54 + temp_fs3;
                                var_fa1 = (f32) temp_t0->unk_0x56 + temp_fs3;
                                var_ft4 = temp_fv1 - temp_fv0;
                                var_ft5 = temp_fv0 + temp_fv1;
                            } else {
                                temp_fv1 = temp_s1[2];
                                var_fa0 = temp_fs3 - sp1CC;
                                var_fa1 = temp_fs3 + sp1CC;
                                var_ft4 = temp_fv1 + (f32) temp_t1->unk_0x54;
                                var_ft5 = temp_fv1 + (f32) temp_t1->unk_0x56;
                            }
                            if ((!(var_ft4 < var_fa0) || !(var_ft5 < var_fa0)) && (!(var_fa1 < var_ft4) || !(var_fa1 < var_ft5))) {
                                temp_fs0 = temp_s1[0] + sp1CC;
                                var_fs1 = sp1C8 - temp_s1[1];
                                var_fs0 = temp_fs0 * temp_fs0;
                                temp_fv0 = var_fs1 * var_fs1;
                                var_fs5 = temp_fv0;
                                if (temp_fv0 < var_fs0) {
                                    var_fs2 = sp1C0 - temp_s1[3];
                                    var_fs5 = temp_fv0 + (var_fs2 * var_fs2);
                                    if (var_fs5 < var_fs0) {
                                        var_fs4 = 0.0f;
                                        var_s3 = 1;
                                    }
                                }
                            }
                        } else {
                            temp_fs0_2 = temp_s1[0] + sp1CC;
                            var_fs0 = temp_fs0_2 * temp_fs0_2;
                            if (var_s5 != 0) {
                                var_fs1 = sp1C8 - temp_s1[1];
                                temp_fv0 = var_fs1 * var_fs1;
                                var_fs5 = temp_fv0;
                                if (temp_fv0 < var_fs0) {
                                    var_fs4 = temp_fs3 - temp_s1[2];
                                    var_fs5 = temp_fv0 + (var_fs4 * var_fs4);
                                    if (var_fs5 < var_fs0) {
                                        var_fs2 = sp1C0 - temp_s1[3];
                                        var_fs5 += var_fs2 * var_fs2;
                                        if (var_fs5 < var_fs0) {
                                            var_s3 = 1;
                                        }
                                    }
                                }
                            } else {
                                temp_fa0_2 = temp_s1[1];
                                temp_fv1 = temp_s1[2];
                                temp_fa1_2 = temp_s1[3];
                                sp80 = sp1BC;
                                temp_fv0 = -((sp1BC * sp184) + (sp1B8 * sp180) + (sp1B4 * sp17C) + -((temp_fa1_2 * sp17C) + ((temp_fa0_2 * sp184) + (temp_fv1 * sp180)))) * sp174;
                                if (temp_fv0 <= 0.0f) {
                                    var_fs1 = sp1BC - temp_fa0_2;
                                    var_fs4 = sp1B8 - temp_fv1;
                                    var_fs2 = sp1B4 - temp_fa1_2;
                                } else {
                                    var_ft2 = sp1C0;
                                    if (temp_fv0 >= 1.0f) {
                                        var_fs1 = sp1C8 - temp_fa0_2;
                                        var_fs4 = temp_fs3 - temp_fv1;
                                    } else {
                                        var_fs1 = (sp1BC + (temp_fv0 * sp184)) - temp_fa0_2;
                                        var_fs4 = (sp1B8 + (temp_fv0 * sp180)) - temp_fv1;
                                        var_ft2 = sp1B4 + (temp_fv0 * sp17C);
                                    }
                                    var_fs2 = var_ft2 - temp_fa1_2;
                                }
                                var_fs5 = (var_fs1 * var_fs1) + (var_fs4 * var_fs4) + (var_fs2 * var_fs2);
                                if (var_fs5 < var_fs0) {
                                    var_s3 = 1;
                                }
                            }
                        }
                        if ((var_s3 != 0) && (var_s4 < 0x40)) {
                            temp_s0 = (var_s4) + D_800B19A0;
                            if (arg4 != 0) {
                                if (var_fs5 > 0.0f) {
                                    var_fs0 = (var_fs0 - var_fs5) / var_fs0;
                                    temp_s0->unk14 = var_fs0;
                                    temp_s0->unk0 = (f32) (var_fs1 * var_fs0);
                                    temp_s0->unk4 = (f32) (var_fs2 * var_fs0);
                                }
                            } else {
                                spE0 = temp_t0;
                                spDC = temp_t1;
                                sp16C = var_t2;
                                spE6 = var_t3;
                                temp_fv0 = sqrtf((var_fs1 * var_fs1) + (var_fs4 * var_fs4) + (var_fs2 * var_fs2));
                                var_fs0 = temp_fv0;
                                if (temp_fv0 > 0.0f) {
                                    var_fs1 /= temp_fv0;
                                    var_fs4 /= temp_fv0;
                                    var_fs2 /= temp_fv0;
                                }
                                temp_s0->unk8 = (f32) (temp_s1[0] * var_fs1);
                                temp_s0->unkC = (f32) (temp_s1[0] * var_fs4);
                                temp_s0->unk10 = (f32) (temp_s1[0] * var_fs2);
                            }
                            temp_s0->unk18 = var_s6;
                            temp_s0->unk19 = var_s2;
                            var_s4 += 1;
                        }
                    } 
                }
            }
        }
        spB0[5] = 0;
        spB0[4] = 0;
        spB0[3] = 0;
        spB0[2] = 0;
        for (var_s6 = 0; var_s6 < var_s4; var_s6++) {
            var_s0 = &D_800B19A0[var_s6];
            var_s7 = (s32) var_s0->unk18;
            var_s3 = (s32) var_s0->unk19;
            temp_a0_2 = sp108[var_s7].unkA;
            var_v1_4 = temp_a0_2;
            temp_a1 = sp104[var_s3].unkA;
            while (var_v1_4) {
                spB0[4] |= (1 << (var_s7 + (((s32) (var_v1_4 & 0xF000) >> 0xC) & 0xFFFF))) >> 0x1F;
                spB0[5] |= (var_v1_4 * 0x10);
                var_v1_4 = (var_v1_4 * 0x10);
            }
            var_v1_4 = temp_a1;
            while (var_v1_4) {
                spB0[2] |= (1 << (var_s3 + (((s32) (var_v1_4 & 0xF000) >> 0xC) & 0xFFFF))) >> 0x1F;
                spB0[3] |= (var_v1_4 * 0x10);
                var_v1_4 = (var_v1_4 * 0x10);
            }
            if ((temp_a0_2 == 0) && (temp_a1 == 0)) {
                if (arg3 != 0) {
                    temp_v0_11 = var_t2 + (var_s3);
                    if (var_t3 != 0) {
                        var_fs3 = (sp170 + (var_s7 ))[2];
                    } else {
                        var_fs3 = var_s0->unkC + temp_v0_11[2];
                    }
                    spE6 = var_t3;
                    sp16C = var_t2;
                    spDC = temp_t1;
                    spE0 = temp_t0;
                    func_800261E8(obj2, obj, (s8) spD8->unk_0x5f, (s8) spD8->unk_0x60, (s8) var_s3, var_s0->unk8 + temp_v0_11[1], var_fs3, var_s0->unk10 + temp_v0_11[3]);
                } else if (arg4 != 0) {
                    temp_fv0 = var_s0->unk14;
                    if (sp188 < temp_fv0) {
                        sp188 = temp_fv0;
                        sp1A8 = var_s0->unk0;
                        sp1A0 = var_s0->unk4;
                    }
                }
            } else if (temp_a0_2 == 0) {
                spB0[5] |= 1 << var_s7;
                spB0[4] |= (1 << var_s7) >> 0x1F;
            } else if (temp_a1 == 0) {
                spB0[3] |= 1 << var_s3;
                spB0[2] |= (1 << var_s3) >> 0x1F;
            }
        }
    } while (var_s4 != 0);
    sp18C = var_fs0;
    if ((arg3 != 0) && (temp_t1->unk_0x62 != 0)) {
        if (temp_t0->unk_0x58 & 0x80) {
            spDC = temp_t1;
            func_800267A4(obj);
        }
        if (temp_t1->unk_0x58 & 0x80) {
            func_800267A4(obj2);
        }
        return 1U;
    }
    var_v0 = 0;
    if ((arg4 != 0) && (sp188 > 0.0f) && (obj == obj3)) {
        spDC = temp_t1;
        func_8002635C(obj2, obj, spD8->unk_0x5d, spD8->unk_0x5e, (s8) var_s3);
        func_8002635C(obj, obj2, temp_t1->unk_0x5d, temp_t1->unk_0x5e, (s8) var_s7);
        func_800287E4(obj, obj2, -sp1A8, 0.0f, -sp1A0, 0);
        var_v0 = 1;
    }
    return var_v0;
}
#endif

#if 1
s32 func_8002AD3C(Object *obj, Vec3f *arg1, Vec3f *arg2, Vec3f *arg3, f32 *arg4);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002AD3C.s")
#else
// https://decomp.me/scratch/z2C6y
s32 func_8002AD3C(Object* obj, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, f32* arg4) {
    s32 spEC;
    s32 spE8;
    f32 spE4;
    f32 spE0;
    f32 spDC;
    f32 spD8;
    f32 spC0;
    f32 spBC;
    f32 spA4;
    f32 spA0;
    f32 sp9C;
    HitSphere* temp_s4;
    Model* model;
    ModelInstance* modelInstance;
    ObjectHitInfo* objHitInfo;
    Unk800B19A0* var_a1;
    s32 pad[4];
    f32 temp_fa0;
    f32 temp_fs0;
    f32 temp_fs0_2;
    f32 temp_fs1;
    f32 temp_ft5;
    f32 temp_fv0_2;
    f32 temp_fv0_3;
    f32 temp_fv0_4;
    f32 temp_fv1;
    f32 var_ft5;
    f32 var_fv1;
    s32 temp_a0;
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_t2;
    s32 temp_t8;
    s32 temp_t8_2;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 i;
    s32 var_s1;
    u16 var_v1;
    s32 temp_s3;
    Vec4f* temp_s5;
    Vec4f* temp_v0_3;
    s32 temp;

    objHitInfo = obj->objhitInfo;
    modelInstance = obj->modelInsts[obj->modelInstIdx];
    if ((objHitInfo->unk_0x9e != 0) || (objHitInfo->unk_0x9f != 0)) {
        return 0;
    }
    if (objHitInfo->unk_0x5a & 0x10) {
        model = modelInstance->model;
        spE4 = 10.0f;
        spE0 = arg2->x - arg1->x;
        spDC = arg2->y - arg1->y;
        spD8 = arg2->z - arg1->z;
        spA4 = arg1->x - gWorldX;
        spA0 = arg1->y;
        sp9C = arg1->z - gWorldZ;
        temp_s3 = model->hitSphereCount;
        temp_s5 = modelInstance->unk_0x24;
        temp_s4 = model->hitSpheres;
        spEC = 0;
        spE8 = 0;
        for (i = 0; i < temp_s3; i++) {
            if (i != temp_s4[i].unkC) {
                spE8 |= (1 << i) >> 0x1F;
                spEC |= 1 << i;
            }
        }
        do {
            var_s1 = 0;
            for (i = 0; i < temp_s3; i++) {
                if ((((1 << i) >> 0x1F) & spE8) || ((1 << i) & spEC)) {
                    temp_v0_3 = &temp_s5[i];
                    temp_fv0_2 = temp_v0_3->x;
                    temp_fs1 = (spE0 * spE0) + (spDC * spDC) + (spD8 * spD8);
                    temp_fv0_3 = spA4 - temp_v0_3->y;
                    temp_fv1 = spA0 - temp_v0_3->z;
                    temp_fa0 = sp9C - temp_v0_3->w;
                    spC0 = 2.0f * ((spE0 * temp_fv0_3) + (spDC * temp_fv1) + (spD8 * temp_fa0));
                    temp_ft5 = spC0 * spC0;
                    temp_fs0 = (4.0f * temp_fs1) * (((temp_fv0_3 * temp_fv0_3) + (temp_fv1 * temp_fv1) + (temp_fa0 * temp_fa0)) - (temp_v0_3->x * temp_v0_3->x));
                    spBC = temp_ft5 - temp_fs0;
                    if (temp_fs0 < temp_ft5) {
                        temp_fs0_2 = 2.0f * temp_fs1;
                        temp_fv0_4 = sqrtf(spBC);
                        if (temp_fs0_2 != 0.0f) {
                            spC0 = -spC0;
                            var_ft5 = (spC0 + temp_fv0_4) / temp_fs0_2;
                            if (var_ft5 > 0.0f) {
                                var_fv1 = (spC0 - temp_fv0_4) / temp_fs0_2;
                                if ((var_fv1 > 0.0f) && !(var_ft5 < var_fv1)) {
                                    var_ft5 = var_fv1;
                                }
                            } else {
                                var_fv1 = (spC0 - temp_fv0_4) / temp_fs0_2;
                                if (var_fv1 > 0.0f) {
                                    var_ft5 = var_fv1;
                                }
                            }
                            if ((var_ft5 >= 0.0f) && (var_ft5 <= 1.0f)) {
                                if (var_ft5 < spE4) {
                                    spE4 = var_ft5;
                                }
                                D_800B19A0[var_s1]->unk18 = i;
                                var_s1 += 1;
                            }
                        }
                    }
                }
            }
            spEC = 0;
            spE8 = 0;
            var_a1 = D_800B19A0[0];
            for (i = 0; i < var_s1; i++) {
                temp_a0 = var_a1[i].unk18;
                var_v1 = temp_s4[temp_a0].unkA;
                while (var_v1) {
                    spEC |= (1 << (temp_a0 + (u16)((var_v1 & 0xF000) >> 0xC)));
                    spE8 |= (1 << (temp_a0 + (u16)((var_v1 & 0xF000) >> 0xC))) >> 0x1F;
                    var_v1 <<= 4;
                }
            }
        } while (var_s1 != 0);
    }
    if (spE4 != 10.0f) {
        arg3->x = arg1->x + (spE4 * spE0);
        arg3->y = arg1->y + (spE4 * spDC);
        arg3->z = arg1->z + (spE4 * spD8);
        *arg4 = spE4;
        return TRUE;
    }
    *arg4 = 10.0f;
    return FALSE;
}
#endif

void func_8002B2D0(s32* arg0, s32 arg1, s32 arg2, HitSphere *hitSpheres) {
    s32 sp20[48];
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < arg1; i++) {
        if (hitSpheres[i].unkA == 0) {
            sp20[count] = i;
            count += 1;
        }
    }

    for (i = 0; i < count; arg0++, i++) {
        arg0[0] = arg2;
    }
}

void func_8002B410(Object* obj, s32 arg1) {
    SRT srt;
    ObjectStruct58* sp24;
    MtxF *a0;

    sp24 = obj->unk0x58;
    if (sp24 != NULL) {
        if (arg1 != 0) {
            sp24->unk10c = (sp24->unk10c + 1) & 1;
        }
        a0 = (MtxF *)((f32 *)sp24 + (sp24->unk10c << 4));
        srt.yaw = -obj->srt.yaw;
        if (obj->objhitInfo->unk_0x58 & 0x800) {
            srt.pitch = 0;
        } else {
            srt.pitch = -obj->srt.pitch;
        }
        if (obj->objhitInfo->unk_0x58 & 0x1000) {
            srt.roll = 0;
        } else {
            srt.roll = -obj->srt.roll;
        }
        srt.scale = 1.0f;
        srt.transl.x = -obj->srt.transl.x;
        srt.transl.y = -obj->srt.transl.y;
        srt.transl.z = -obj->srt.transl.z;
        matrix_from_srt_reversed(a0, &srt);
        srt.yaw = obj->srt.yaw;
        if (obj->objhitInfo->unk_0x58 & 0x800) {
            srt.pitch = 0;
        } else {
            srt.pitch = obj->srt.pitch;
        }
        if (obj->objhitInfo->unk_0x58 & 0x1000) {
            srt.roll = 0;
        } else {
            srt.roll = obj->srt.roll;
        }
        srt.scale = 1.0f;
        srt.transl.x = obj->srt.transl.x;
        srt.transl.y = obj->srt.transl.y;
        srt.transl.z = obj->srt.transl.z;
        matrix_from_srt((MtxF *)((f32 *)sp24 + ((sp24->unk10c + 2) << 4)), &srt);
        if (sp24->unk10d > 0) {
            sp24->unk10d--;
        }
    }
}

s32 func_8002B5A4(s32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2, UNK_TYPE_32 arg3) {
    return 0;
}

void func_8002B5C0(Object* obj) {
    s16 i;
    Unk800B199C* temp_v1;

    i = 0;
    while (i < 30) {
        temp_v1 = &D_800B199C[i];
        if ((temp_v1->unk0 != 0) && (obj == temp_v1->unk8)) {
            temp_v1->unk0 = 0;
        }
        i += 1;
    }
}

Unk800B199C* func_8002B61C(Object* arg0, Object* arg1) {
    Unk800B199C* temp_v1;
    s16 i;

    i = 0;
    while(i < 30) {
        temp_v1 = &D_800B199C[i];
        if ((temp_v1->unk0 != 0) && (arg0 == temp_v1->unk4) && (arg1 == temp_v1->unk8)) {
            temp_v1->unk0 = 0;
            return temp_v1;
        }
        i++;
    }
    return NULL;
}

Unk800B199C* func_8002B68C(Object* arg0, Object* arg1) {
    Unk800B199C* temp_v1;
    s16 i;

    i = 0;
    while (i < 30) {
        temp_v1 = &D_800B199C[i];
        if (temp_v1->unk0 == 0) {
            temp_v1->unk4 = arg0;
            temp_v1->unk8 = arg1;
            temp_v1->unk0 = 2;
            return temp_v1;
        }
        i++;
    }

    return NULL;
}

void func_8002B6EC(void) {
    s16 i;
    Unk800B199C* temp_v1;

    i = 0;
    while (i < 30) {
        temp_v1 = &D_800B199C[i];
        if (temp_v1->unk0 != 0) {
            temp_v1->unk0--;
        }
        i++;
    }

    D_800B1990 = delayFloat;
}

void func_8002B74C(f32** arg0, s32 arg1) {
    f32 temp_fv0;
    f32 *temp_a3;
    s32 var_t0;
    f32 *temp;
    s32 i;

    for (i = 1; i < arg1; i++) {
        temp = arg0[i];
        temp_a3 = arg0[i];
        var_t0 = i;
        temp_fv0 = temp[1];
        while (temp_fv0 < arg0[var_t0-1][1]) {
            arg0[var_t0] = arg0[var_t0-1];
            var_t0--;
        }
        arg0[var_t0] = temp_a3;
    }
}

#ifndef NON_MATCHING
void func_8002B7CC(f32 **arg0, s32 arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B7CC.s")
#else
// https://decomp.me/scratch/5wFq1
void func_8002B7CC(f32** arg0, s32 arg1) {
    f32* temp_a0_2;
    f32* temp_a0_3;
    s32 var_t0;
    s32 i;
    s32 var_v1;

    for (i = 1; (s32)((arg1 - 1) / 9) >= i; i = (i * 3) + 1) {}

    while (i > 0) {
        var_v1 = i + 1;
        while (var_v1 < arg1) {
            temp_a0_2 = arg0[var_v1];
            var_t0 = var_v1;
            while (i < var_t0 && temp_a0_2[1] < (&arg0[var_t0])[-i][1]) {
                arg0[var_t0] = arg0[var_t0 - i];
                var_t0 -= i;
            }
            var_v1++;
            arg0[var_t0] = temp_a0_2;
        }
        i /= 3;
    }
}
#endif

s32 func_8002B910(Object *obj, Vec3f *arg1, s32 *arg2, Vec3f *arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 *argA, f32 *argB, s32 *argC, f32 *argD, Vec3f *argE);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B910.s")

s32 func_8002BD04(Vec3f *arg0, f32 arg1, s32 *arg2, f32 *arg3, s32 *arg4, f32 arg5, f32 arg6, f32 arg7, f32 *arg8, f32 *arg9, f32 *argA);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002BD04.s")

s32 func_8002BF0C(Object *obj, f32 arg1, s32 *arg2, void *arg3, void *arg4, f32 arg5, f32 arg6, f32 arg7, f32 *arg8, f32 *arg9, f32 *argA);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002BF0C.s")

s32 func_8002C0C4(s32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2, f32 arg3, UNK_TYPE_32 arg4, f32 arg7, f32 arg8, f32 arg9, f32 argA, f32 argB, f32 argC, f32 *argD);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002C0C4.s")

s32 func_8002C278(s32 arg0, Vec3f *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 argA, f32 argB, f32 argC, f32 argD, f32 argE, f32 argF, Object *arg10, f32 arg11, f32 *arg12, f32 *arg13);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002C278.s")

void *func_8002C3EC(Object *obj, void *arg1, f32 arg2, f32 arg3, f32 arg4, s32 *arg5, s32 *arg6, f32 *arg7, f32 arg8, f32 arg9, f32 argA, f32 *argB, f32 *argC, Vec3f *argD);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002C3EC.s")

Vec3f *func_8002C658(Vec3f *arg0, f32 arg1, Vec3f *arg2, Vec3f *arg3, f32 arg4, f32 arg5, f32 arg6, Vec3f *arg7);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002C658.s")

Vec3f *func_8002C8C0(Vec3f *arg0, f32 arg1, f32 arg2, Vec3f *arg3, Vec3f *arg4, f32 arg5, f32 arg6, f32 arg7, Vec3f *arg8);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002C8C0.s")

Vec3f *func_8002CBD4(Vec3f *arg0, f32 arg1, f32 arg2, Vec3f *arg3, Vec3f *arg4, f32 arg5, f32 arg6, f32 arg7, Vec3f *arg8);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002CBD4.s")

void func_8002CEC8(Object *obj, f32 arg1, f32 arg2, Vec3f *arg3, Vec3f *arg4, f32 arg7, f32 arg8, f32 arg9, Vec3f *argA);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002CEC8.s")

s32 func_8002D0DC(Vec3f *arg0, f32 arg1, Object *obj, Object *otherObj, s32 *arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, Vec3f *arg9);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002D0DC.s")

s32 func_8002D69C(Vec3f *arg0, f32 arg1, Object *obj, Object *otherObj, s32 *arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, Vec3f *arg9);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002D69C.s")

s32 func_8002DC58(Object *obj, Object *arg1, void *arg2, void *arg3);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002DC58.s")

s32 func_8002DFB8(s32 arg0, f32 arg1, f32 arg2, f32 arg3, void **arg4, void *arg5, Object *arg6, Object *arg7, f32 *arg8);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002DFB8.s")

s32 func_8002E3D0(s32 arg0, s32 arg1, f32 arg2, f32 arg3, void **arg4, void *arg5, Object *arg6, Object *arg7, f32 arg8, f32 arg9, f32 *argA);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002E3D0.s")

void func_8002E84C(Object *obj, Object *otherObj);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002E84C.s")

s32 func_8002E88C(Vec3f *arg0, Vec3f *arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002E88C.s")

s32 func_8002E904(Object *obj, Object *otherObj);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002E904.s")

s32 func_8002E964(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002E964.s")

s32 func_8002EA0C(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002EA0C.s")

s32 func_8002EAD0(Object *obj, Vec3f *arg1, Vec3f *arg2, f32 arg3, f32 arg4);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002EAD0.s")

s32 func_8002EBA8(Vec3f *arg0, Vec3f *arg1, f32 arg2, f32 arg3, Vec3f *arg4, Vec3f *arg5, f32 arg6, f32 arg7);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002EBA8.s")

s32 func_8002EE38(Vec3f *obj, Vec3f *arg1, f32 arg2, f32 arg3, Vec3f *arg4, Vec3f *arg5, f32 arg6, f32 arg7);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002EE38.s")

void func_8002F04C(Object *obj, Vec3f *arg1, Vec3f *arg2, f32 arg3, f32 arg4);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F04C.s")

void func_8002F164(Object *obj, Object *otherObj);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F164.s")

f32 func_8002F214(Object *obj);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F214.s")

void func_8002F24C(Object *arg0, Object *arg1, s32 *arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F24C.s")

s32 func_8002F2D8(Vec3f *arg0, f32 arg1, Object *obj);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F2D8.s")

s32 func_8002F3DC(Object *obj, f32 arg1, Object *otherObj);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F3DC.s")

void func_8002F498(Object *obj, f32 **arg1, void *arg2, Object *obj2, Object *obj3);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F498.s")

s32 func_8002F998(s32 *arg0, void *arg1, s32 arg2, s32 *arg3, s32 *arg4, s32 *arg5, s32 *arg6, void **arg7, void *arg8, void *arg9, s32 *argA, s32 *argB, Object *argC, void **argD);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F998.s")

void func_800302B4(u8 *arg0, s32 arg1, Object *arg2, Vec3f *arg3, Vec3f *arg4, f32 arg5, f32 arg6);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_800302B4.s")

void func_80030338(void *arg0, void *arg1, s32 arg2, Object *obj2, Object *obj3, Vec3f *arg6, f32 arg7);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80030338.s")

void func_800307C4(f32 arg0, f32 arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_800307C4.s")

void func_80030880(void);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80030880.s")

void func_80030994(ModelInstance *modelInstance, f32 arg1, f32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80030994.s")

void func_80030A24(s32 arg0, s32 arg1, Object *arg2, f32 arg3, s32 arg4, Vec3f *arg5);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80030A24.s")

void func_80030AEC(Object *obj, Object *otherObj);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80030AEC.s")

void func_80030E2C(Object *obj, f32 *arg1, Vec3f *arg2, Vec3f *arg3, Vec3f *arg4);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80030E2C.s")
