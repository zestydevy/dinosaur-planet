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

void func_80028D90(void);
void func_8002B410(Object *, s32);
void update_object(Object *);
void func_8001943C(Object *object, MtxF *mf, f32 yPrescale);
void func_8001A1D4(Model *model, AnimState *animState, s32 count);
void func_80026AB8(Object *obj, ModelInstance *modelInstance, s32 arg2, ObjectHitInfo *objHitInfo, s32 arg4, s32 arg5);
void func_8002B5C0(Object *obj);
u8 func_8005509C(s32 arg0, f32* arg1, f32* arg2, s32 arg3, Unk80027934* arg4, u8 arg5);

extern f32 D_800B1990;
extern Object **D_800B1994;
extern s32 D_800B1998;
extern s32* D_800B199C; // size: 0x3C / 60, 15 elements
extern void* D_800B20A0;
extern void* D_800B20A8;
extern void* D_800B20AC;
extern void* D_800B20B0;
extern void* D_800B20B4;
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
        D_800B199C[i * 15] = 0;
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
    objHitInfo->unk_0x8 = align_8(arg3);
    arg3 = objHitInfo->unk_0x8 + objHitInfo->unk_0x6;
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
        sp17C = (u32) objectHitInfo->unk_0x40 >> 4;
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

void func_80028238(Object *obj, Object *otherObj);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80028238.s")

void func_800287E4(Object *obj, Object *otherObj, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_800287E4.s")

void func_80028D2C(void *arg0);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80028D2C.s")

void func_80028D90(void);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80028D90.s")

void func_80028DC4(void) {

}

void func_80028DCC(Object *obj, Object *obj2, Object *obj3, Object *obj4, f32 arg4);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80028DCC.s")

void func_8002949C(Object *obj, Object *obj2, Object *obj3, ObjectHitInfo *objHitInfo, ObjectHitInfo *objHitInfo2, f32 arg5);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002949C.s")

void func_80029A14(Object *obj, Object *otherObj, f32 arg3, u8 arg4);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80029A14.s")

void func_80029AB4(void *arg0, s32 arg1, void *arg2, s32 arg3, u32 arg4, s32 arg5);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80029AB4.s")

u8 func_80029C04(Object *obj, Object *obj2, Object *obj3, s8 arg3, s8 arg4, u32 arg5, u32 arg6);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80029C04.s")

s32 func_8002AD3C(Object *obj, Vec3f *arg1, Vec3f *arg2, Vec3f *arg3, f32 *arg4);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002AD3C.s")

void func_8002B2D0(s32 *arg0, s32 arg1, s32 arg2, s32 arg3);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B2D0.s")

void func_8002B410(Object *obj, s32 arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B410.s")

s32 func_8002B5A4(s32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2, UNK_TYPE_32 arg3) {
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B5C0.s")

void *func_8002B61C(s32 arg0, s32 arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B61C.s")

void *func_8002B68C(s32 arg0, s32 arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B68C.s")

void func_8002B6EC(void);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B6EC.s")

void func_8002B74C(s32 *arg0, s32 arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B74C.s")

void func_8002B7CC(f32 **arg0, s32 arg1);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B7CC.s")

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

s32 func_8002D0DC(Vec3f *arg0, f32 arg1, Object *obj, Object *otherObj, s32 *arg4, void *arg6, f32 arg8, Vec3f *arg9);
#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002D0DC.s")

s32 func_8002D69C(Vec3f *arg0, f32 arg1, Object *obj, Object *otherObj, s32 *arg4, void *arg6, f32 arg7, f32 arg8, Vec3f *arg9);
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
