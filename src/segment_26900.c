#include "common.h"

void func_80028D90(void);                                  /* extern */
void func_8002B410(Object *, s32);                            /* extern */
void update_object(Object *);

extern f32 D_800B1990;
extern Object **D_800B1994;
extern s32 D_800B1998;
extern s32* D_800B199C; // size: 0x3C / 60, 15 elements
extern void* D_800B20A0;
extern void* D_800B20A8;
extern void* D_800B20AC;
extern void* D_800B20B0;
extern void* D_800B20B4;

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
    ObjectStruct58* temp_v0;
    s32 i;

    temp_v0 = arg0->unk0x58;
    if (temp_v0->unk10f < 3) {
        for (i = 0; i < temp_v0->unk10f; i++) {
            if (arg1 == temp_v0->unk100[i]) {
                return;
            }
        }
        
        temp_v0->unk100[temp_v0->unk10f] = arg1;
        temp_v0 = arg0->unk0x58;
        temp_v0->unk10f += 1;
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
            objHitInfo2->unk_0x48 = arg0;
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
        objHitInfo->unk_0x6c[objHitInfo->unk_0x62] = arg1;
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
        objHitInfo->unk_0x6c[objHitInfo->unk_0x62] = arg1;
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

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002667C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_800266E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80026724.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002674C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_800267A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_800267C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002681C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002683C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80026940.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_800269CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80026A20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80026AB8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80026B84.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80026BD8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/update_obj_hitboxes.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80026DF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/obj_do_hit_detection.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80027934.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80027DAC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80028238.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_800287E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80028D2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80028D90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80028DC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80028DCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002949C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80029A14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80029AB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80029C04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002AD3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B2D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B410.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B5A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B5C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B61C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B68C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B6EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B74C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B7CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002B910.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002BD04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002BF0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002C0C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002C278.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002C3EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002C658.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002C8C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002CBD4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002CEC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002D0DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002D69C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002DC58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002DFB8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002E3D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002E84C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002E88C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002E904.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002E964.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002EA0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002EAD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002EBA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002EE38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F04C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F164.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F214.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F24C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F2D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F3DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F498.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_8002F998.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_800302B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80030338.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_800307C4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80030880.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80030994.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80030A24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80030AEC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/segment_26900/func_80030E2C.s")
