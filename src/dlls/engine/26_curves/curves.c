#include "game/objects/object.h"
#include "dlls/engine/26_curves.h"
#include "libc/string.h"
#include "sys/objects.h"
#include "sys/rand.h"
#include "sys/voxmap.h"
#include "sys/segment_53F00.h"
#include "macros.h"

#define MAX_ROMCURVES 1300

/*0x0*/ static CurveSetup *_bss_0;
/*0x4*/ static CurveSetup *_bss_4;
/*0x8*/ static CurveNode _bss_8[MAX_ROMCURVES];
/*0x28A8*/ static s32 _bss_28A8;

s32 curves_func_1880(CurveSetup* arg0);
s32 curves_func_1924(CurveSetup* setup);
static s32 curves_func_2AE8(f32 a0, f32 a1, f32 a2, CurveSetup* a3, CurveSetup* a4, f32 a5);
s32 curves_func_4704(UnkCurvesStruct* arg0);
s32 curves_func_5698(CurveSetup* setup, s32 arg1, s32 arg2);
s32 curves_func_577C(CurveSetup* setup, s32 arg1, s32 arg2);

// offset: 0x0 | ctor
void curves_ctor(void *dll) { }

// offset: 0xC | dtor
void curves_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void curves_func_18(void) {
    _bss_28A8 = 0;
}

// offset: 0x34 | func: 1 | export: 1
// official name: addCurveDef
void curves_add_curve_def(CurveSetup* def) {
    s32 var_v1;
    s32 temp;

    if (_bss_28A8 == MAX_ROMCURVES) {
        STUBBED_PRINTF("curves.c: MAX_ROMCURVES exceeded!!\n");
        return;
    }
    
    var_v1 = 0;
    while (var_v1 < _bss_28A8 && _bss_8[var_v1].uID < def->uID) {
        var_v1++;
    }
    
    temp = _bss_28A8;
    while (temp > var_v1) {
        _bss_8[temp].setup = _bss_8[temp - 1].setup;
        _bss_8[temp].uID = _bss_8[temp - 1].uID;
        temp--;
    }

    _bss_28A8 += 1;
    if (_bss_28A8 == MAX_ROMCURVES) {
        STUBBED_PRINTF("addCurveDef: max curves reached %d/%d\n", _bss_28A8, MAX_ROMCURVES);
    }
    _bss_8[var_v1].setup = def;
    _bss_8[var_v1].uID = def->uID;
}

// offset: 0x10C | func: 2 | export: 2
void curves_func_10C(CurveSetup* setup) {
    s32 i;
    
    i = 0;
    while (i < _bss_28A8 && setup->uID != _bss_8[i].uID) {
        i++;
    }

    if (i < _bss_28A8) {
        _bss_28A8--;

        while (i < _bss_28A8) {
            i++;
            _bss_8[i - 1].setup = _bss_8[i].setup;
            _bss_8[i - 1].uID = _bss_8[i].uID;
        }
    }
}

// offset: 0x1BC | func: 3 | export: 3
CurveNode *curves_func_1BC(s32 *count) {
    *count = _bss_28A8;
    return _bss_8;
}

// offset: 0x1E4 | func: 4 | export: 4
/**
  * Finds the closest curveSetup node to a given coordinate 
  * (prioritising any with a matching tag) and returns its uID.
  */
s32 curves_func_1E4(f32 x, f32 y, f32 z, s32 *curveTypes, s32 curveTypeCount, s32 curveTag) {
    CurveSetup* curveSetup;
    CurveSetup* closestCurveSetup;
    CurveSetup* closestCurveSetupTagged;
    f32 zDiff;
    f32 xDiff;
    f32 dist;
    f32 yDiff;
    f32 minDistance;
    f32 minDistanceTagged;
    s32 i;
    s32 k;

    minDistance = 5000000.0f;
    closestCurveSetup = NULL;

    minDistanceTagged = 5000000.0f;
    closestCurveSetupTagged = NULL;
    
    for (i = 0; i < _bss_28A8; i++)  {
        curveSetup =_bss_8[i].setup;
        k = 0;

        do {
            if ((curveTypeCount <= 0) || (curveSetup->curveType == curveTypes[k])) {
                xDiff = curveSetup->pos.x - x;
                yDiff = curveSetup->pos.y - y;
                zDiff = curveSetup->pos.z - z;
                dist = sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));

                if (dist < minDistance) {
                    minDistance = dist;
                    closestCurveSetup = curveSetup;
                }

                if ((curveTag == curveSetup->unk18) && (dist < minDistanceTagged)) {
                    minDistanceTagged = dist;
                    closestCurveSetupTagged = curveSetup;
                }

                k = curveTypeCount;
            }

            k++;
        } while (k < curveTypeCount);
    }

    if (closestCurveSetupTagged != NULL) {
        closestCurveSetup = closestCurveSetupTagged;
    }

    if (closestCurveSetup != NULL) {
        return closestCurveSetup->uID;
    }

    return -1;
}

// offset: 0x39C | func: 5 | export: 6
CurveSetup* curves_func_39C(s32 curveUID) {
    s32 temp_t6;
    s32 max;
    s32 min;

    if (curveUID < 0) {
        return NULL;
    }

    max = _bss_28A8 - 1;
    min = 0;

    while (min <= max) {
        temp_t6 = (max + min) >> 1;

        if (_bss_8[temp_t6].uID < (u32)curveUID) {
            min = temp_t6 + 1;
        } else if (_bss_8[temp_t6].uID > (u32)curveUID) {
            max = temp_t6 - 1;
        } else {
            return _bss_8[temp_t6].setup;
        }
    }

    return NULL;
}

// offset: 0x438 | func: 6 | export: 20
s32 curves_func_438(CurveSetup* arg0, s32 arg1) {
    s32 sp38[4];
    s32 var_t0;
    s32 var_a2;
    s32 var_v1;

    var_v1 = 0;
    var_a2 = 1;
    var_t0 = 0;
    while (var_t0 < 4) {
        if ((arg0->links[var_t0] >= 0) && !(arg0->unk1B & var_a2) && (arg1 != arg0->links[var_t0])) {
            sp38[var_v1] = arg0->links[var_t0];
            var_v1 += 1;
        }
        var_t0++;
        var_a2 <<= 1;
    }

    if (var_v1 != 0) {
        var_t0 = rand_next(0, var_v1 - 1);
        return sp38[var_t0];
    }

    return -1;
}

// offset: 0x4F0 | func: 7 | export: 21
s32 curves_func_4F0(CurveSetup* arg0, s32 arg1, s32* arg2) {
    s32 var_v1;
    s32 i;
    s32 v0;

    for (i = 0, var_v1 = 0, v0 = 1; i < 4; i++, v0 <<= 1) {
        if ((arg0->links[i] >= 0) && (arg1 != arg0->links[i])) {
            arg2[var_v1] = arg0->links[i];
            var_v1++;
        }
    }

    return var_v1;
}

// offset: 0x590 | func: 8 | export: 23
s32 curves_func_590(CurveSetup* arg0, s32 arg1) {
    s32 sp38[4];
    s32 var_t0;
    s32 var_a2;
    s32 var_v1;

    var_v1 = 0;
    var_a2 = 1;
    var_t0 = 0;
    while (var_t0 < 4) {
        if ((arg0->links[var_t0] >= 0) && (arg0->unk1B & var_a2) && (arg1 != arg0->links[var_t0])) {
            sp38[var_v1] = arg0->links[var_t0];
            var_v1 += 1;
        }
        var_t0++;
        var_a2 <<= 1;
    }

    if (var_v1 != 0) {
        var_t0 = rand_next(0, var_v1 - 1);
        return sp38[var_t0];
    }

    return -1;
}

// offset: 0x648 | func: 9
static f32 curves_func_648(f32 arg0, f32 arg1, f32 arg2, Vec3f* arg3) {
    f32 temp_f26;
    f32 temp_f25;
    f32 temp_f24;
    f32 var_f27;
    f32 var_f31;
    f32 var_f30;
    f32 var_f29;
    f32 var_f28;

    temp_f26 = arg3[1].x - arg3[0].x;
    temp_f25 = arg3[1].y - arg3[0].y;
    temp_f24 = arg3[1].z - arg3[0].z;
    if ((0.0f == temp_f26) && (0.0f == temp_f25) && (0.0f == temp_f24)) {
        var_f27 = 0.0f;
    } else {
        var_f27 = ((((arg0 - arg3[0].x) * temp_f26) + ((arg1 - arg3[0].y) * temp_f25) + ((arg2 - arg3[0].z) * temp_f24))) 
            / ((SQ(temp_f26) + SQ(temp_f25) + SQ(temp_f24)));
    }
    if (var_f27 < 0.0f) {
        var_f31 = arg3[0].x;
        var_f30 = arg3[0].y;
        var_f29 = arg3[0].z;
        var_f28 = ((var_f31 - arg0) * (var_f31 - arg0)) + ((var_f30 - arg1) * (var_f30 - arg1)) + ((var_f29 - arg2) * (var_f29 - arg2));
        var_f28 = -var_f28;
    } else if (var_f27 > 1.0f) {
        var_f31 = arg3[1].x;
        var_f30 = arg3[1].y;
        var_f29 = arg3[1].z;
        var_f28 = ((var_f31 - arg0) * (var_f31 - arg0)) + ((var_f30 - arg1) * (var_f30 - arg1)) + ((var_f29 - arg2) * (var_f29 - arg2));
        var_f28 = -var_f28;
    } else {
        var_f31 = (var_f27 * temp_f26) + arg3[0].x;
        var_f30 = (var_f27 * temp_f25) + arg3[0].y;
        var_f29 = (var_f27 * temp_f24) + arg3[0].z;
        var_f28 = ((var_f31 - arg0) * (var_f31 - arg0)) + ((var_f30 - arg1) * (var_f30 - arg1)) + ((var_f29 - arg2) * (var_f29 - arg2));
    }
    arg3[2].x = var_f31;
    arg3[2].y = var_f30;
    arg3[2].z = var_f29;
    return var_f28;
}

// offset: 0x854 | func: 10 | export: 22
s32 curves_func_854(CurveSetup* arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    CurveSetup* temp_v0;
    s32 spAC[2];
    f32 temp_fa0;
    f32 temp_fv0;
    f32 sp9C[2];
    f32 temp_fv1;
    s32 s3;
    s32 temp_s2;
    Vec3f sp6C[3];
    s32 var_s1;

    spAC[0] = spAC[1] = -1;
    sp9C[0] = sp9C[1] = 0.0f;
    sp6C[0].x = arg0->pos.x;
    sp6C[0].y = arg0->pos.y;
    sp6C[0].z = arg0->pos.z;
    for (var_s1 = 0, s3 = 1; var_s1 < 4; var_s1++, s3 <<= 1) {
        if (arg0->links[var_s1] >= 0) {
            temp_v0 = curves_func_39C(arg0->links[var_s1]);
            if (temp_v0 != NULL) {
                temp_s2 = arg1 == arg0->links[var_s1];
                sp6C[1].x = temp_v0->pos.x;
                sp6C[1].y = temp_v0->pos.y;
                sp6C[1].z = temp_v0->pos.z;
                curves_func_648(arg2, arg3, arg4, sp6C);
                temp_fv0 = sp6C[2].x - arg2;
                temp_fv1 = sp6C[2].y - arg3;
                temp_fa0 = sp6C[2].z - arg4;
                temp_fv0 = SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa0);
                if (sp9C[temp_s2] < temp_fv0) {
                    sp9C[temp_s2] = temp_fv0;
                    spAC[temp_s2] = arg0->links[var_s1];
                }
            }
        }
    }
    if (spAC[0] != -1) {
        return spAC[0];
    }
    if (spAC[1] != -1) {
        return spAC[1];
    }
    return -1;
}

// offset: 0xA34 | func: 11 | export: 26
void curves_func_A34(CurveSetup* arg0, s32* arg1) {
    CurveSetup* temp_v0_2;
    s32 temp_v1;
    s32 var_a0;

    arg1[0] = -1;
    arg1[1] = -1;
    arg1[2] = -1;
    arg1[3] = -1;
    if (arg0 != NULL) {
        arg1[1] = arg0->uID;
        
        for (var_a0 = 0; var_a0 < 4; var_a0++) {
            if (arg0->links[var_a0] != -1) {
                temp_v1 = arg0->unk1B & (1 << var_a0);
                if (temp_v1 != 0) {
                    arg1[0] = arg0->links[var_a0];
                } else if (temp_v1 == 0) {
                    arg1[2] = arg0->links[var_a0];
                }
            }
        }

        if (arg1[2] >= 0) {
            temp_v0_2 = curves_func_39C(arg1[2]);
            if (temp_v0_2 != NULL) {
                for (var_a0 = 0; var_a0 < 4; var_a0++) {
                    if ((temp_v0_2->links[var_a0] != -1) && !(temp_v0_2->unk1B & (1 << var_a0))) {
                        arg1[3] = temp_v0_2->links[var_a0];
                    }
                }
            }
        }
    }
}

// offset: 0xB8C | func: 12 | export: 27
s32 curves_func_B8C(s32* arg0, f32 *arg1, f32 *arg2, f32 *arg3) {
    s32 var_s3;
    s32 i;
    CurveSetup* sp50[4];

    var_s3 = 0;
    for (i = 0; i < 4; i++) {
        sp50[i] = curves_func_39C(arg0[i]);
        if (sp50[i] != NULL) {
            arg1[i] = sp50[i]->pos.x;
            arg2[i] = sp50[i]->pos.y;
            arg3[i] = sp50[i]->pos.z;
            var_s3++;
        }
    }
    if (var_s3 < 2 || sp50[1] == NULL || sp50[2] == NULL) {
        return FALSE;
    }

    for (i = 0; i < 4; i++) {
        if (sp50[i] == NULL) {
            if (i == 0) {
                arg1[i] = sp50[1]->pos.x + (sp50[1]->pos.x - sp50[2]->pos.x);
                arg2[i] = sp50[1]->pos.y + (sp50[1]->pos.y - sp50[2]->pos.y);
                arg3[i] = sp50[1]->pos.z + (sp50[1]->pos.z - sp50[2]->pos.z);
            } else if (i == 3) {
                arg1[i] = sp50[2]->pos.x + (sp50[2]->pos.x - sp50[1]->pos.x);
                arg2[i] = sp50[2]->pos.y + (sp50[2]->pos.y - sp50[1]->pos.y);
                arg3[i] = sp50[2]->pos.z + (sp50[2]->pos.z - sp50[1]->pos.z);
            }
        }
    }

    return TRUE;
}

// offset: 0xD8C | func: 13 | export: 28
s32 curves_func_D8C(CurveSetup* setup) {
    s32 nodeCount;

    nodeCount = 1;
    while ((setup != NULL) && (curves_func_1924(setup) == 0)) {
        setup = curves_func_39C(curves_func_438(setup, 0));
        if (setup != NULL) {
            nodeCount++;
        }
    }
    return nodeCount;
}

// offset: 0xE40 | func: 14 | export: 29
s32 curves_func_E40(CurveSetup* arg0, f32* arg1, f32* arg2, f32* arg3, s8* arg4) {
    CurveSetup* temp_v0;
    s32 var_s5;
    s32 var_v0;

    if (curves_func_1924(arg0) != 0) {
        var_v0 = 1;
    } else {
        var_v0 = 0;
    }
    var_s5 = 0;
    if (var_v0 == 0) {
        while ((arg0 != NULL) && (curves_func_1924(arg0) == 0)) {
            temp_v0 = curves_func_39C(curves_func_438(arg0, 0));
            if (temp_v0 != NULL) {
                if (arg4 != NULL) {
                    arg4[var_s5 >> 2] = arg0->curveType;
                }
                arg1[var_s5] = arg0->pos.x;
                arg2[var_s5] = arg0->pos.y;
                arg3[var_s5] = arg0->pos.z;
                var_s5++;
                arg1[var_s5] = temp_v0->pos.x;
                arg2[var_s5] = temp_v0->pos.y;
                arg3[var_s5] = temp_v0->pos.z;
                var_s5++;
                arg1[var_s5] = 2.0f * (fsin16_precise((s16) (arg0->unk2C << 8)) * (f32) arg0->unk2E);
                arg2[var_s5] = 2.0f * (fsin16_precise((s16) (arg0->unk2D << 8)) * (f32) arg0->unk2E);
                arg3[var_s5] = 2.0f * (fcos16_precise((s16) (arg0->unk2C << 8)) * (f32) arg0->unk2E);
                var_s5++;
                arg1[var_s5] = 2.0f * (fsin16_precise((s16) (temp_v0->unk2C << 8)) * (f32) temp_v0->unk2E);
                arg2[var_s5] = 2.0f * (fsin16_precise((s16) (temp_v0->unk2D << 8)) * (f32) temp_v0->unk2E);
                arg3[var_s5] = 2.0f * (fcos16_precise((s16) (temp_v0->unk2C << 8)) * (f32) temp_v0->unk2E);
                var_s5++;
            }
            arg0 = temp_v0;
        }
    } else {
        while ((arg0 != NULL) && (curves_func_1880(arg0) == 0)) {
            temp_v0 = curves_func_39C(curves_func_590(arg0, 0));
            if (temp_v0 != NULL) {
                if (arg4 != NULL) {
                    arg4[var_s5 >> 2] = arg0->curveType;
                }
                arg1[var_s5] = arg0->pos.x;
                arg2[var_s5] = arg0->pos.y;
                arg3[var_s5] = arg0->pos.z;
                var_s5 = var_s5 + 1;
                arg1[var_s5] = temp_v0->pos.x;
                arg2[var_s5] = temp_v0->pos.y;
                arg3[var_s5] = temp_v0->pos.z;
                var_s5 = var_s5 + 1;
                arg1[var_s5] = 2.0f * (fsin16_precise((s16) (arg0->unk2C << 8)) * (f32) arg0->unk2E);
                arg2[var_s5] = 2.0f * (fsin16_precise((s16) (arg0->unk2D << 8)) * (f32) arg0->unk2E);
                arg3[var_s5] = 2.0f * (fcos16_precise((s16) (arg0->unk2C << 8)) * (f32) arg0->unk2E);
                var_s5 = var_s5 + 1;
                arg1[var_s5] = 2.0f * (fsin16_precise((s16) (temp_v0->unk2C << 8)) * (f32) temp_v0->unk2E);
                arg2[var_s5] = 2.0f * (fsin16_precise((s16) (temp_v0->unk2D << 8)) * (f32) temp_v0->unk2E);
                arg3[var_s5] = 2.0f * (fcos16_precise((s16) (temp_v0->unk2C << 8)) * (f32) temp_v0->unk2E);
                var_s5 = var_s5 + 1;
            }
            arg0 = temp_v0;
        }
    }
    return var_s5;
}

// offset: 0x1420 | func: 15
static CurveSetup* curves_func_1420(s32 curveUID, s32* arg1) {
    s32 temp_t6;
    s32 max;
    s32 min;

    *arg1 = -1;

    if (curveUID < 0) {
        return NULL;
    }

    max = _bss_28A8 - 1;
    min = 0;

    while (min <= max) {
        temp_t6 = (max + min) >> 1;

        if (_bss_8[temp_t6].uID < (u32)curveUID) {
            min = temp_t6 + 1;
        } else if (_bss_8[temp_t6].uID > (u32)curveUID) {
            max = temp_t6 - 1;
        } else {
            *arg1 = temp_t6;
            return _bss_8[temp_t6].setup;
        }
    }

    return NULL;
}

// offset: 0x14C8 | func: 16 | export: 10
void curves_func_14C8(CurveSetup** arg0, CurveSetup** arg1) {
    *arg0 = _bss_0;
    *arg1 = _bss_4;
}

// offset: 0x14F4 | func: 17 | export: 7
f32 curves_func_14F4(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32* arg5, f32* arg6, f32* arg7) {
    f32 var_fs0;
    s32 var_s1;
    s32 spAC;
    CurveSetup* temp_s2;
    CurveSetup* temp_v0;
    f32 temp_fv0;
    Vec3f sp7C[3];
    f32 var_fa0;
    f32 var_fv1;

    *arg7 = 0.0f;
    *arg6 = 0.0f;
    *arg5 = 0.0f;
    var_fs0 = 5000000.0f;
    for (spAC = 0; spAC < _bss_28A8; spAC++) {
        temp_s2 = _bss_8[spAC].setup;
        if ((arg1 == temp_s2->unk18) && (arg0 == temp_s2->curveType)) {
            sp7C[0].x = temp_s2->pos.x;
            sp7C[0].y = temp_s2->pos.y;
            sp7C[0].z = temp_s2->pos.z;
            for (var_s1 = 0; var_s1 < 4; var_s1++) {
                if (!(temp_s2->unk1B & (1 << var_s1))) {
                    temp_v0 = curves_func_39C(temp_s2->links[var_s1]);
                    if (temp_v0 != NULL) {
                        sp7C[1].x = temp_v0->pos.x;
                        sp7C[1].y = temp_v0->pos.y;
                        sp7C[1].z = temp_v0->pos.z;
                        temp_fv0 = curves_func_648(arg2, arg3, arg4, sp7C);
                        if (temp_fv0 < 0) {
                            var_fa0 = -temp_fv0;
                        } else {
                            var_fa0 = temp_fv0;
                        }
                        if (var_fs0 < 0) {
                            var_fv1 = -var_fs0;
                        } else {
                            var_fv1 = var_fs0;
                        }
                        if (var_fa0 < var_fv1) {
                            _bss_0 = temp_s2;
                            _bss_4 = temp_v0;
                            var_fs0 = temp_fv0;
                            *arg5 = sp7C[2].x;
                            *arg6 = sp7C[2].y;
                            *arg7 = sp7C[2].z;
                        }
                    }
                }
            }
        }
    }
    return var_fs0;
}

// offset: 0x174C | func: 18 | export: 8
f32 curves_func_174C(Object* arg0, s32 arg1) {
    CurveSetup* temp_v0;
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv1;

    temp_v0 = curves_func_39C(arg1);
    if ((temp_v0 != NULL) && (arg0 != NULL)) {
        temp_fv0 = temp_v0->pos.x - arg0->srt.transl.x;
        temp_fv1 = temp_v0->pos.y - arg0->srt.transl.y;
        temp_fa1 = temp_v0->pos.z - arg0->srt.transl.z;
        return sqrtf((temp_fv0 * temp_fv0) + (temp_fv1 * temp_fv1) + (temp_fa1 * temp_fa1));
    }
    return -1.0f;
}

// offset: 0x17F4 | func: 19 | export: 9
f32 curves_func_17F4(f32 arg0, f32 arg1, s32 arg2) {
    CurveSetup* temp_v0;
    f32 temp_fv0;
    f32 temp_fv1;

    temp_v0 = curves_func_39C(arg2);
    if (temp_v0 != NULL) {
        temp_fv0 = temp_v0->pos.x - arg0;
        temp_fv1 = temp_v0->pos.z - arg1;
        return sqrtf((temp_fv0 * temp_fv0) + (temp_fv1 * temp_fv1));
    }
    return -1.0f;
}

// offset: 0x1880 | func: 20 | export: 11
s32 curves_func_1880(CurveSetup* arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if ((arg0->links[i] != -1) && (arg0->unk1B & (1 << i))) {
            return 0;
        }
    }
    
    return 1;
}

// offset: 0x1924 | func: 21 | export: 12
s32 curves_func_1924(CurveSetup* setup) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if ((setup->links[i] != -1) && !(setup->unk1B & (1 << i))) {
            return 0;
        }
    }
    
    return 1;
}

// offset: 0x19C8 | func: 22 | export: 13
s32 curves_func_19C8(CurveSetup* arg0, s32 *arg1, s32 arg2, s32* arg3) {
    CurveSetup* temp_v0;
    s32 temp_a0_2;
    s32 var_s0;
    s32 var_s2;
    s32 sp50[4];
    s32 var_s6;

    if (arg0 == NULL) {
        return -1;
    }
    
    var_s2 = 0;
    
    for (var_s6 = 0; var_s6 < 4; var_s6++) {
        if (arg0->links[var_s6] >= 0) {
            temp_v0 = curves_func_39C(arg0->links[var_s6]);
            for (var_s0 = 0; var_s0 < arg2; var_s0++) {
                if (temp_v0->curveType == arg1[var_s0]) {
                    sp50[var_s2] = arg0->links[var_s6];
                    var_s2 += 1;
                    var_s0 = arg2;
                }
            }
        }
    }
    
    if (var_s2 == 0) {
        return -1;
    }
    if (var_s2 == 1) {
        *arg3 = arg0->uID;
        return sp50[0];
    }
    if (var_s2 >= 2) {
        for (var_s6 = 0; var_s6 < var_s2; var_s6++) {
            temp_a0_2 = var_s2 - 1;
            if (sp50[var_s6] == *arg3) {
                var_s2 = temp_a0_2;
                while (var_s6 < temp_a0_2) {
                    sp50[var_s6] = sp50[var_s6 + 1];
                    var_s6++;
                }
            }
        }
        *arg3 = arg0->uID;
        var_s2 = rand_next(0, var_s2 - 1);
        return sp50[var_s2];
    }

    return -1;
}

// offset: 0x1B90 | func: 23 | export: 14
s32 curves_func_1B90(CurveSetup* arg0, s32 arg1, s32 arg2, s32* arg3) {
    s32 var_r27;
    s32 var_r26;
    s32 var_r31;
    s32 var_r28;
    s32 var_r24;
    CurveSetup* temp_r30;
    s32 var_r21;
    CurveSetup* temp_r3;
    s32 sp684[4];
    s32 sp680;
    s32 sp67C;
    f32 temp_f31;
    f32 temp_f30;
    f32 sp66C[4];
    s32 var_r23;
    s32 var_r22;
    s8 sp158[MAX_ROMCURVES];
    s32 spB8[40];
    f32 sp18[40];

    if (arg0 == NULL) {
        return -1;
    }
    curves_func_1420(arg0->uID, &sp67C);
    var_r26 = 0;
    var_r23 = 0;
    while (var_r23 < 4) {
        if (arg0->links[var_r23] > -1) {
            var_r31 = 0;
            while (var_r31 < MAX_ROMCURVES) {
                sp158[var_r31] = 0;
                var_r31 += 1;
            }
            sp158[sp67C] = 1;
            temp_r30 = curves_func_1420(arg0->links[var_r23], &sp680);
            var_r28 = 0;
            sp18[var_r28] = SQ(temp_r30->pos.x - arg0->pos.x) + SQ(temp_r30->pos.y - arg0->pos.y) + SQ(temp_r30->pos.z - arg0->pos.z);
            spB8[var_r28] = sp680;
            var_r28++;
            sp158[sp680] = 1;
            var_r21 = 0;
            do {
                if (var_r28 > 0) {
                    var_r28 -= 1;
                    sp680 = spB8[var_r28];
                    temp_r30 = _bss_8[sp680].setup;
                    temp_f31 = sp18[var_r28];
                    if ((temp_r30->curveType == arg1) && ((arg2 == -1) || (arg2 == temp_r30->unk18))) {
                        var_r21 = 1;
                        sp66C[var_r26] = temp_f31;
                        sp684[var_r26] = arg0->links[var_r23];
                        var_r26 += 1;
                    } else {
                        var_r22 = 0;
                        while (var_r22 < 4) {
                            if (temp_r30->links[var_r22] > -1) {
                                temp_r3 = curves_func_1420(temp_r30->links[var_r22], &sp680);
                                if ((temp_r3 != NULL) && (sp158[sp680] == 0) && (var_r28 < 0x28)) {
                                    temp_f30 = temp_f31 
                                            + SQ(temp_r30->pos.x - temp_r3->pos.x)
                                            + SQ(temp_r30->pos.y - temp_r3->pos.y) 
                                            + SQ(temp_r30->pos.z - temp_r3->pos.z);
                                    var_r27 = 0;
                                    while ((var_r27 < var_r28) && (sp18[var_r27] > temp_f30)) {
                                        var_r27 += 1;
                                    }
                                    var_r24 = var_r28;
                                    while (var_r24 > var_r27) {
                                        spB8[var_r24] = spB8[var_r24 - 1];
                                        sp18[var_r24] = sp18[var_r24 - 1];
                                        var_r24 -= 1;
                                    }
                                    var_r28 += 1;
                                    sp18[var_r27] = temp_f30;
                                    spB8[var_r27] = sp680;
                                    sp158[sp680] = 1;
                                }
                            }
                            var_r22 += 1;
                        }
                    }
                } else {
                    var_r21 = 1;
                }
            } while (var_r21 == 0);
        }
        var_r23 += 1;
    }
    if (var_r26 == 0) {
        return -1;
    }
    if (var_r26 == 1) {
        *arg3 = arg0->uID;
        return sp684[0];
    }
    if (var_r26 > 1) {
        var_r31 = 0;
        while (var_r31 < var_r26) {
            if (sp684[var_r31] == *arg3) {
                while (var_r31 < (var_r26 - 1)) {
                    sp684[var_r31] = sp684[var_r31 + 1];
                    sp66C[var_r31] = sp66C[var_r31 + 1];
                    var_r31 += 1;
                }
                var_r26 -= 1;
            }
            var_r31 += 1;
        }
        *arg3 = arg0->uID;
        var_r27 = 0;
        var_r31 = 0;
        while (var_r31 < var_r26) {
            if (sp66C[var_r31] < sp66C[var_r27]) {
                var_r27 = var_r31;
            }
            var_r31 += 1;
        }
        return sp684[var_r27];
    }
    return -1;
}

// offset: 0x218C | func: 24 | export: 15
s32 curves_func_218C(s32 arg0) {
    CurveSetup* temp_a0;
    s32 i;

    STUBBED_PRINTF(" Curve Id %i ", arg0);
    for (i = 0; i < _bss_28A8; i++) {
        temp_a0 = _bss_8[i].setup;
        if (temp_a0->curveType == 0x15) {
            STUBBED_PRINTF(" CI %i ", temp_a0->type15.unk34);
            if (arg0 == temp_a0->type15.unk34) {
                return temp_a0->uID;
            }
        }
    }
    return -1;
}

// offset: 0x21FC | func: 25 | export: 16
s32 curves_func_21FC(s32 arg0, s32 arg1, s32 arg2, s32* arg3) {
    s32 var_r23;
    s32 var_r25;
    CurveSetup* temp_r31;
    s32 var_r30;
    s32 var_r27;
    f32 temp_f31;
    s32 var_r21;
    s32 var_r29;
    s32 sp688[4];
    s32 sp684;
    s32 sp680;
    s32 var_r24;
    CurveSetup* temp_r3;
    f32 sp670[4];
    CurveSetup *new_var;
    s32 var_r22;
    f32 temp_f30;
    s8 sp15C[MAX_ROMCURVES];
    s32 spBC[40];
    f32 sp1C[40];
    s8 sp18[4];

    new_var = curves_func_1420(arg0, &sp680);
    if (new_var == NULL) {
        return -1;
    }
    var_r25 = 0;
    var_r23 = 0;
    while (var_r23 < 4) {
        if (new_var->links[var_r23] > -1) {
            var_r27 = 0;
            while (var_r27 < MAX_ROMCURVES) {
                sp15C[var_r27] = 0;
                var_r27 += 1;
            }
            sp15C[sp680] = 1;
            temp_r3 = curves_func_1420(new_var->links[var_r23], &sp684);
            var_r29 = 0;
            sp1C[var_r29] = SQ(temp_r3->pos.x - new_var->pos.x) 
                + SQ(temp_r3->pos.y - new_var->pos.y) 
                + SQ(temp_r3->pos.z - new_var->pos.z);
            spBC[var_r29] = sp684;
            var_r29++;
            sp15C[sp684] = 1;
            var_r21 = 0;
            do {
                if (var_r29 > 0) {
                    var_r29 -= 1;
                    sp684 = spBC[var_r29];
                    temp_r31 = _bss_8[sp684].setup;
                    temp_f31 = sp1C[var_r29];
                    if (((temp_r31->curveType == arg1) || (arg1 == -1)) 
                            && ((temp_r31->unk.unk31 == arg2) || (temp_r31->unk.unk32 == arg2) || (temp_r31->unk.unk33 == arg2))) {
                        var_r21 = 1;
                        sp670[var_r25] = temp_f31;
                        if (var_r25 < 4) {
                            sp688[var_r25] = temp_r31->uID;
                            sp18[var_r25] = var_r23;
                            var_r25 += 1;
                        }
                    } else {
                        var_r22 = 0;
                        while (var_r22 < 4) {
                            if (temp_r31->links[var_r22] > -1) {
                                temp_r3 = curves_func_1420(temp_r31->links[var_r22], &sp684);
                                if ((temp_r3 != NULL) && (sp15C[sp684] == 0) && (var_r29 < 0x28)) {
                                    temp_f30 = temp_f31
                                        + SQ(temp_r31->pos.x - temp_r3->pos.x) 
                                        + SQ(temp_r31->pos.y - temp_r3->pos.y) 
                                        + SQ(temp_r31->pos.z - temp_r3->pos.z);
                                    var_r30 = 0;
                                    while ((var_r30 < var_r29) && (sp1C[var_r30] > temp_f30)) {
                                        var_r30 += 1;
                                    }
                                    var_r24 = var_r29;
                                    while (var_r24 > var_r30) {
                                        spBC[var_r24] = spBC[var_r24 - 1];
                                        sp1C[var_r24] = sp1C[var_r24 - 1];
                                        var_r24 -= 1;
                                    }
                                    var_r29 += 1;
                                    sp1C[var_r30] = temp_f30;
                                    spBC[var_r30] = sp684;
                                    sp15C[sp684] = 1;
                                }
                            }
                            var_r22 += 1;
                        }
                    }
                } else {
                    var_r21 = 1;
                }
            } while (var_r21 == 0);
        }
        var_r23 += 1;
    }
    if (var_r25 > 0) {
        var_r30 = 0;
        var_r27 = 0;
        while (var_r27 < var_r25) {
            if (sp670[var_r27] < sp670[var_r30]) {
                var_r30 = var_r27;
            }
            var_r27 += 1;
        }
        if (arg3 != NULL) {
            *arg3 = sp18[var_r30];
        }
        return sp688[var_r30];
    }
    return -1;
}

// offset: 0x277C | func: 26 | export: 17
s32 curves_func_277C(f32 arg0, f32 arg1, f32 arg2, s32 arg3) {
    f32 var_fs0;
    f32 var_fs1;
    s32 i;
    s32 sp90;
    s32 objCount;
    Object* obj;
    Object** objs;
    CurveSetup* curveSetup;
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 temp_fv1;
  
    objs = get_world_objects(&sp90, &objCount);
    var_fs1 = 0.0f;
    var_fs0 = -1.0f;
    for (i = 0; i < objCount; i++) {
        obj = objs[i];
        if ((obj->controlNo == OBJCONTROL_Curve) && (arg3 != obj->mapID)) {
            curveSetup = (CurveSetup*)obj->setup;
            if ((curveSetup != NULL) && (curveSetup->curveType == 0x16)) {
                temp_fv0 = obj->globalPosition.x - arg0, 
                temp_fv1 = obj->globalPosition.y - arg1, 
                temp_fa1 = obj->globalPosition.z - arg2, 
                temp_fv0_2 = sqrtf(SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa1));

                if (var_fs0 == -1.0f || temp_fv0_2 < var_fs1) {
                    var_fs1 = temp_fv0_2;
                    var_fs0 = (f32) (u32) curveSetup->uID;
                }
            }
        }
    }
    return (s32) var_fs0;
}

// offset: 0x291C | func: 27 | export: 18
s32 curves_func_291C(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32* arg4) {
    CurveSetup* temp_v0;
    CurveSetup* var_s1;
    s32 sp64;
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 var_s0;
    s32 var_s5;
    s32 var_v0;

    sp64 = arg0;
    var_s1 = curves_func_39C(sp64);
    var_s5 = 0;
    arg4[0] = 200000.0f;

    do {
        var_s0 = -1;
        for (var_v0 = 0; var_v0 < 4 && var_s0 == -1; var_v0++) {
            if (var_s1->unk1B & (1 << var_v0)) {
                continue;
            }

            var_s0 = var_s1->links[var_v0];
        }

        if (var_s0 != -1) {
            temp_v0 = curves_func_39C(var_s0);
            if (curves_func_2AE8(arg1, arg2, arg3, var_s1, temp_v0, 1000.0f) != 0) {
                temp_fv0 = var_s1->pos.x - arg1;
                temp_fv1 = var_s1->pos.y - arg2;
                temp_fa1 = var_s1->pos.z - arg3;
                temp_fv0 = sqrtf(SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa1));
                if (temp_fv0 < arg4[0]) {
                    arg4[0] = temp_fv0;
                }
                var_s5++;
            }

            sp64 = var_s0;
            var_s1 = temp_v0;
        }

        if (sp64 == arg0) {
            break;
        }
    } while (var_s0 != -1);
    
    return var_s5 & 1;
}

// offset: 0x2AE8 | func: 28
static s32 curves_func_2AE8(f32 a0, f32 a1, f32 a2, CurveSetup* a3, CurveSetup* a4, f32 a5) {
    f32 temp_fa1;
    f32 temp_ft4;
    f32 temp_ft5;
    f32 temp_fv0;
    f32 temp_ft1;
    f32 temp_fv1;

    temp_fv0 = a3->pos.x;
    temp_fv1 = a3->pos.z;
    temp_fa1 = a4->pos.x;
    temp_ft4 = a4->pos.z;
    temp_ft5 = (temp_fa1 * temp_fv1) - (temp_fv0 * temp_ft4);
    temp_ft1 = ((temp_ft4 - temp_fv1) * a0) + ((temp_fv0 - temp_fa1) * a2) + temp_ft5;
    if ((temp_ft1 <= 0.0f && temp_ft5 >= 0.0f) || (temp_ft1 >= 0.0f && temp_ft5 < 0.0f)) {
        if (((((-a2 * temp_fv0) + (a0 * temp_fv1)) <= 0.0f) && (((-a2 * temp_fa1) + (a0 * temp_ft4)) >= 0.0f)) || 
            ((((-a2 * temp_fv0) + (a0 * temp_fv1)) >= 0.0f) && (((-a2 * temp_fa1) + (a0 * temp_ft4)) < 0.0f))) {
            return 1;
        }
    }
    return 0;
}

// offset: 0x2C20 | func: 29 | export: 30
s32 curves_func_2C20(s32* arg0, f32 farg0, f32 farg1, f32 farg2, f32* arg4, f32* arg5, f32* arg6) {
    f32 var_f31;
    f32 var_f30;
    f32 temp_f27;
    f32 temp_f25;
    f32 temp_f1;
    f32 var_f24;
    f32 var_f23;
    f32 temp_f29;
    f32 var_f28;
    f32 var_f22;
    CurveSetup* sp34[4];
    Vec3f sp24;
    f32 temp_f26;
    s32 var_r31;

    var_r31 = 0;
    while (var_r31 < 4) {
        sp34[var_r31] = curves_func_39C(arg0[var_r31]);
        var_r31 += 1;
    }
    temp_f27 = sp34[2]->pos.x - sp34[1]->pos.x;
    var_f24 = temp_f27;
    temp_f26 = sp34[2]->pos.z - sp34[1]->pos.z;
    var_f23 = temp_f26;
    if (sp34[0] != NULL) {
        var_f31 = sp34[1]->pos.x - sp34[0]->pos.x;
        var_f30 = sp34[1]->pos.z - sp34[0]->pos.z;
    } else {
        var_f31 = temp_f27;
        var_f30 = temp_f26;
    }
    var_f31 = 0.5f * (var_f31 + temp_f27);
    var_f30 = 0.5f * (var_f30 + temp_f26);
    temp_f1 = sqrtf(SQ(var_f31) + SQ(var_f30));
    if (temp_f1 != 0.0f) {
        var_f31 /= temp_f1;
        var_f30 /= temp_f1;
    }
    sp24.x = var_f31;
    sp24.y = var_f30;
    sp24.z = -((sp24.x * sp34[1]->pos.x) + (sp24.y * sp34[1]->pos.z));
    var_f28 = (sp24.x * var_f24) + (sp24.y * var_f23);
    if (var_f28 != 0.0f) {
        var_f28 = -(sp24.z + ((sp24.x * farg0) + (sp24.y * farg2))) / var_f28;
    }
    temp_f27 = sp34[2]->pos.x - sp34[1]->pos.x;
    temp_f26 = sp34[2]->pos.z - sp34[1]->pos.z;
    if (sp34[3] != NULL) {
        var_f31 = sp34[3]->pos.x - sp34[2]->pos.x;
        var_f30 = sp34[3]->pos.z - sp34[2]->pos.z;
    } else {
        var_f31 = temp_f27;
        var_f30 = temp_f26;
    }
    var_f31 = 0.5f * (var_f31 + temp_f27);
    var_f30 = 0.5f * (var_f30 + temp_f26);
    temp_f1 = sqrtf(SQ(var_f31) + SQ(var_f30));
    if (temp_f1 != 0.0f) {
        var_f31 /= temp_f1;
        var_f30 /= temp_f1;
    }
    sp24.x = var_f31;
    sp24.y = var_f30;
    sp24.z = -((sp24.x * sp34[2]->pos.x) + (sp24.y * sp34[2]->pos.z));
    var_f22 = (sp24.x * var_f24) + (sp24.y * var_f23);
    if (var_f22 != 0.0f) {
        var_f22 = -(sp24.z + ((sp24.x * farg0) + (sp24.y * farg2))) / var_f22;
    }
    var_f28 = -var_f28 / (var_f22 - var_f28);
    if ((var_f28 >= 0.0f) && (var_f28 < 1.0f)) {
        temp_f25 = sp34[2]->pos.y - sp34[1]->pos.y;
        temp_f1 = sqrtf(SQ(temp_f27) + SQ(temp_f25) + SQ(temp_f26));
        if (temp_f1 > 0.0f) {
            temp_f29 = 1.0f / temp_f1;
            var_f24 = -temp_f27 * temp_f29;
            var_f23 = -temp_f26 * temp_f29;
        }
        temp_f27 = ((temp_f27 * var_f28) + sp34[1]->pos.x);
        temp_f25 = ((temp_f25 * var_f28) + sp34[1]->pos.y);
        temp_f26 = ((temp_f26 * var_f28) + sp34[1]->pos.z);
        *arg4 = -((temp_f27 * var_f23) - (temp_f26 * var_f24)) + ((farg0 * var_f23) - (farg2 * var_f24));
        *arg5 = farg1 - temp_f25;
        *arg6 = var_f28;
        return 1;
    }
    return 0;
}

// offset: 0x3018 | func: 30 | export: 31
CurveSetup *curves_func_3018(CurveSetup *arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4) {
    f32 sp8C;
    f32 sp88;
    f32 sp84;
    s32 sp74[4];

    while (!curves_func_1924(arg0)) {
        curves_func_A34(arg0, sp74);
        if ((curves_func_2C20(sp74, arg1, arg2, arg3, &sp8C, &sp88, &sp84) != 0) && (sp8C > -300.0f) && (sp8C < 300.0f) && (sp88 > -100.0f) && (sp88 < 100.0f)) {
            *arg4 = sp84;
            return arg0;
        }
        arg0 = curves_func_39C(curves_func_438(arg0, 0));
    }
    *arg4 = 0.0f;
    return arg0;
}

// offset: 0x31E0 | func: 31 | export: 32
void curves_func_31E0(CurveSetup *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 arg4) {
    CurveSetup *temp_v0;
    f32 xDiff, yDiff, zDiff;

    temp_v0 = curves_func_39C(curves_func_438(arg0, 0));
    if (temp_v0 == NULL) {
        *arg1 = arg0->pos.x;
        *arg2 = arg0->pos.y;
        *arg3 = arg0->pos.z;
        return;
    }

    xDiff = temp_v0->pos.x - arg0->pos.x;
    yDiff = temp_v0->pos.y - arg0->pos.y;
    zDiff = temp_v0->pos.z - arg0->pos.z;
    
    *arg1 = arg0->pos.x + xDiff * arg4;
    *arg2 = arg0->pos.y + yDiff * arg4;
    *arg3 = arg0->pos.z + zDiff * arg4;
}

// offset: 0x32D8 | func: 32 | export: 33
f32 curves_func_32D8(CurveSetup *arg0, CurveSetup *arg1, f32 *arg2, f32 *arg3, f32 arg4, f32 arg5) {
    CurveSetup *temp_v0_4;
    CurveSetup *var_s1;
    f32 *temp_v0_2;
    f32 zDiff;
    f32 xDiff;
    f32 yDiff;
    f32 magnitude;
    s32 var_s2;
    s32 var_s5;

    if (arg0 == arg1) {
        xDiff = arg3[0] - arg2[0];
        yDiff = arg3[1] - arg2[1];
        zDiff = arg3[2] - arg2[2];
        magnitude = sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
        if (arg5 < arg4) {
            magnitude = -magnitude;
        }
    } else {
        var_s5 = 0;
        var_s2 = 0;
        var_s1 = arg0;
        do {
            if (curves_func_1924(var_s1) != 0) {
                var_s2 = 1;
                var_s5 = 0;
            } else {
                var_s1 = curves_func_39C(curves_func_438(var_s1, 0));
                if (var_s1 == arg1) {
                    var_s2 = 1;
                    var_s5 = 1;
                    if ((!arg0) && (!arg0)){} // @fake
                }
            }
        } while (var_s2 == 0);
        var_s2 = 0;
        if (var_s5 == 0) {
            temp_v0_2 = arg2;
            var_s1 = arg0;
            arg0 = arg1;
            arg2 = arg3;
            arg1 = var_s1;
            arg3 = temp_v0_2;
        }
        arg0 = curves_func_39C(curves_func_438(arg0, 0));
        xDiff = arg0->pos.x - arg2[0];
        yDiff = arg0->pos.y - arg2[1];
        zDiff = arg0->pos.z - arg2[2];
        magnitude = sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
        do {
            if (arg0 == arg1) {
                xDiff = arg3[0] - arg0->pos.x;
                yDiff = arg3[1] - arg0->pos.y;
                zDiff = arg3[2] - arg0->pos.z;
                var_s2 = 1;
                magnitude += sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
            } else {
                temp_v0_4 = curves_func_39C(curves_func_438(arg0, 0));
                xDiff = temp_v0_4->pos.x - arg0->pos.x;
                yDiff = temp_v0_4->pos.y - arg0->pos.y;
                zDiff = temp_v0_4->pos.z - arg0->pos.z;
                magnitude += sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff));
                arg0 = temp_v0_4;
            }
        } while (var_s2 == 0);
        if (var_s5 == 0) {
            magnitude = -magnitude;
        }
    }
    return magnitude;
}

// offset: 0x35AC | func: 33 | export: 19
s32 curves_func_35AC(f32 arg0, f32 arg1, f32 arg2) {
    CurveSetup* temp_v1;
    s32 sp6C[20];
    s32 var_s1;
    s8 temp_s2;
    s32 var_v0;
    f32 sp5C;

    var_s1 = 0;
    for (var_v0 = 0; var_v0 < _bss_28A8 && var_s1 < 0x14; var_v0++) {
        temp_v1 = _bss_8[var_v0].setup;
        if (temp_v1->curveType == 0x17) {
            sp6C[var_s1] = temp_v1->uID;
            var_s1 += 1;
        }
    }
   
    while (var_s1 != 0) {
        if (curves_func_291C(sp6C[0], arg0, arg1, arg2, &sp5C) != 0) {
            return sp6C[0];
        }
        temp_s2 = curves_func_39C(sp6C[0])->unk18;
        var_v0 = 0;
        while (var_v0 < var_s1) {
            if (temp_s2 == curves_func_39C(sp6C[var_v0])->unk18) {
                var_s1 -= 1;
                sp6C[var_v0] = sp6C[var_s1];
            } else {
                var_v0++;
            }
        }
    }
    return -1;
}

// offset: 0x374C | func: 34 | export: 24
s32 curves_func_374C(CurveSetup* arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    CurveSetup* temp_v0;
    s32 spAC[2];
    f32 temp_fa0;
    f32 temp_fv0;
    f32 sp9C[2];
    f32 temp_fv1;
    s32 s3;
    s32 temp_s2;
    Vec3f sp6C[3];
    s32 var_s1;

    spAC[0] = spAC[1] = -1;
    sp9C[0] = sp9C[1] = 5000000.0f;
    sp6C[0].x = arg0->pos.x;
    sp6C[0].y = arg0->pos.y;
    sp6C[0].z = arg0->pos.z;
    for (var_s1 = 0, s3 = 1; var_s1 < 4; var_s1++, s3 <<= 1) {
        if (arg0->links[var_s1] >= 0) {
            temp_v0 = curves_func_39C(arg0->links[var_s1]);
            if (temp_v0 != NULL) {
                temp_s2 = arg1 == arg0->links[var_s1];
                sp6C[1].x = temp_v0->pos.x;
                sp6C[1].y = temp_v0->pos.y;
                sp6C[1].z = temp_v0->pos.z;
                curves_func_648(arg2, arg3, arg4, sp6C);
                temp_fv0 = sp6C[2].x - arg2;
                temp_fv1 = sp6C[2].y - arg3;
                temp_fa0 = sp6C[2].z - arg4;
                temp_fv0 = SQ(temp_fv0) + SQ(temp_fv1) + SQ(temp_fa0);
                if (temp_fv0 < sp9C[temp_s2]) {
                    sp9C[temp_s2] = temp_fv0;
                    spAC[temp_s2] = arg0->links[var_s1];
                }
            }
        }
    }
    if (spAC[0] != -1) {
        return spAC[0];
    }
    if (spAC[1] != -1) {
        return spAC[1];
    }
    return -1;
}

// offset: 0x3930 | func: 35 | export: 25
s32 curves_func_3930(CurveSetup* arg0, f32 arg1, f32 arg2, s32* arg3) {
    s32 var_r26;
    s32 var_r28;
    s32 var_r31;
    s32 var_r27;
    s32 var_r23;
    CurveSetup* temp_r30;
    s32 var_r21;
    CurveSetup* temp_r3_2;
    s32 sp684[4];
    s32 sp680;
    s32 sp67C;
    f32 temp_f31;
    f32 temp_f30;
    f32 sp66C[4];
    CurveSetup* temp;
    s32 var_r22;
    s32 var_r25;
    s8 sp158[MAX_ROMCURVES];
    s32 spB8[40];
    f32 sp18[40];

    if (arg0 == NULL) {
        return -1;
    }
    STUBBED_PRINTF("Search\n");
    curves_func_1420(arg0->uID, &sp67C);
    var_r28 = 0;
    var_r25 = 0;
    while (var_r25 < 4) {
        if (arg0->links[var_r25] > -1) {
            STUBBED_PRINTF("link:(%d) %d\n", var_r25, arg0->links[var_r25]);
            var_r31 = 0;
            while (var_r31 < MAX_ROMCURVES) {
                sp158[var_r31] = 0;
                var_r31 += 1;
            }
            sp158[sp67C] = 1;
            temp_r30 = curves_func_1420(arg0->links[var_r25], &sp680);
            var_r27 = 0;
            sp18[var_r27] = SQ(temp_r30->pos.x - arg0->pos.x) + SQ(temp_r30->pos.y - arg0->pos.y) + SQ(temp_r30->pos.z - arg0->pos.z);
            spB8[var_r27] = sp680;
            var_r27++;
            sp158[sp680] = 1;
            var_r21 = 0;
            do {
                if (var_r27 > 0) {
                    var_r27 -= 1;
                    sp680 = spB8[var_r27];
                    temp_r30 = _bss_8[sp680].setup;
                    temp_f31 = sp18[var_r27];
                    temp = temp_r30;
                    if (temp->unk.unk34 == 1) {
                        var_r21 = 1;
                        sp66C[var_r28] = temp_f31;
                        sp684[var_r28] = arg0->links[var_r25];
                        var_r28 += 1;
                        STUBBED_PRINTF("Found: %d\n", temp_r30->uID);
                    } else {
                        var_r22 = 0;
                        while (var_r22 < 4) {
                            if (temp_r30->links[var_r22] > -1) {
                                temp_r3_2 = curves_func_1420(temp_r30->links[var_r22], &sp680);
                                if ((temp_r3_2 != NULL) && (sp158[sp680] == 0) && (var_r27 < 0x28)) {
                                    temp_f30 = temp_f31
                                        + SQ(temp_r30->pos.x - temp_r3_2->pos.x) 
                                        + SQ(temp_r30->pos.y - temp_r3_2->pos.y)
                                        + SQ(temp_r30->pos.z - temp_r3_2->pos.z);
                                    var_r26 = 0;
                                    while ((var_r26 < var_r27) && (sp18[var_r26] > temp_f30)) {
                                        var_r26 += 1;
                                    }
                                    var_r23 = var_r27;
                                    while (var_r23 > var_r26) {
                                        spB8[var_r23] = spB8[var_r23 - 1];
                                        sp18[var_r23] = sp18[var_r23 - 1];
                                        var_r23 -= 1;
                                    }
                                    var_r27 += 1;
                                    sp18[var_r26] = temp_f30;
                                    spB8[var_r26] = sp680;
                                    sp158[sp680] = 1;
                                }
                            }
                            var_r22 += 1;
                        }
                    }
                } else {
                    var_r21 = 1;
                }
            } while (var_r21 == 0);
        }
        var_r25 += 1;
    }
    STUBBED_PRINTF("%d\n", var_r28);
    if (var_r28 == 0) {
        return -1;
    }
    if (var_r28 == 1) {
        *arg3 = arg0->uID;
        return sp684[0];
    }
    if (var_r28 > 1) {
        var_r31 = 0;
        while (var_r31 < var_r28) {
            if (sp684[var_r31] == *arg3) {
                while (var_r31 < (var_r28 - 1)) {
                    sp684[var_r31] = sp684[var_r31 + 1];
                    sp66C[var_r31] = sp66C[var_r31 + 1];
                    var_r31 += 1;
                }
                var_r28 -= 1;
            }
            var_r31 += 1;
        }
        *arg3 = arg0->uID;
        var_r26 = 0;
        var_r31 = 0;
        while (var_r31 < var_r28) {
            if (sp66C[var_r31] < sp66C[var_r26]) {
                var_r26 = var_r31;
            }
            var_r31 += 1;
        }
        return sp684[var_r26];
    }
    return -1;
}

// offset: 0x3F00 | func: 36 | export: 5
s32 curves_func_3F00(Object *arg0, s32 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    f32 temp_fs0;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fs2;
    f32 var_fs3;
    Vec3s16 sp104;
    Vec3s16 spFC;
    Vec3f spF0;
    s32 spEC;
    s32 var_v0_2;
    u8 temp_t5;
    u8 spE6;
    CurveSetup *temp_s0;
    CurveSetup *spDC;
    CurveSetup *spD8;
    Func_80059C40_Struct sp84;

    spDC = NULL;
    spD8 = NULL;
    var_fs2 = 5000000.0f;
    var_fs3 = 5000000.0f;
    spF0.x = arg0->srt.transl.f[0];
    spF0.y = arg0->srt.transl.f[1] + 10.0f;
    spF0.z = arg0->srt.transl.f[2];
    func_80007EE0(&spF0, &spFC);
    for (spEC = 0; spEC < _bss_28A8; spEC++) {
        temp_s0 = _bss_8[spEC].setup;
        var_v0_2 = 0;
        do {
            if ((temp_s0->curveType == arg1[var_v0_2]) || (arg2 <= 0)) {
                temp_fs0 = temp_s0->pos.x - arg0->srt.transl.f[0];
                temp_fv0 = temp_s0->pos.y - arg0->srt.transl.f[1];
                temp_fv1 = temp_s0->pos.z - arg0->srt.transl.f[2];
                temp_fs0 = sqrtf((temp_fs0 * temp_fs0) + (temp_fv0 * temp_fv0) + (temp_fv1 * temp_fv1));
                if ((temp_fs0 < var_fs2)) {
                    spF0.x = temp_s0->pos.x;
                    spF0.y = temp_s0->pos.y + 10.0f;
                    spF0.z = temp_s0->pos.z;
                    func_80007EE0(&spF0, &sp104);
                    temp_t5 = func_80008048(&sp104, &spFC, NULL, &spE6, 0);
    
                    if (((spE6 == 1) || (temp_t5 != 0)) && (func_80059C40(&arg0->srt.transl, &spF0, 1.0f, 0, &sp84, arg0, (s8) arg4, -1, 0U, 0) == 0)) {
                        var_fs2 = temp_fs0;
                        spDC = temp_s0;
                    }
                }
                if ((arg3 == temp_s0->unk18) && (temp_fs0 < var_fs3)) {
                    spF0.x = temp_s0->pos.x;
                    spF0.y = temp_s0->pos.y + 10.0f;
                    spF0.z = temp_s0->pos.z;
                    func_80007EE0(&spF0, &sp104);
                    temp_t5 = func_80008048(&sp104, &spFC, NULL, &spE6, 0);
    
                    if (((spE6 == 1) || (temp_t5 != 0)) && (func_80059C40(&arg0->srt.transl, &spF0, 1.0f, 0, &sp84, arg0, (s8) arg4, -1, 0U, 0) == 0)) {
                        var_fs3 = temp_fs0;
                        spD8 = temp_s0;
                    }
                }
                var_v0_2 = arg2;
            }
            var_v0_2++;
        } while (var_v0_2 < arg2);
    }

    if (spD8 != NULL) {
        spDC = spD8;
    }
    if (spDC != NULL) {
        return spDC->uID;
    } else {
        return -1;
    }
}

// offset: 0x4288 | func: 37 | export: 34
u8 curves_func_4288(UnkCurvesStruct* arg0, Object* arg1, f32 arg2, s32 *arg3, s32 arg4) {
    f32 zDiff;
    f32 xDiff;
    f32 yDiff;
    s32 var_a0;
    s32 var_v0;

    if (arg0 == NULL) {
        return 1;
    }
    
    var_a0 = curves_func_3F00(arg1, arg3, 1, arg4, 0xC);
    if (var_a0 != -1) {
        if (arg0->unk80 != 0) {
            arg0->unkA0 = curves_func_39C(var_a0);
            var_a0 = curves_func_438(arg0->unkA0, -1);
            if (var_a0 == -1) {
                return 1;
            }
        }
        
        arg0->unkA0 = curves_func_39C(var_a0);
        if (arg0->unkA0 == NULL) {
            arg0->unkA0 = NULL;
            return 1;
        }
        if (arg0->unk80 != 0) {
            var_v0 = curves_func_590(arg0->unkA0, -1);
        } else {
            var_v0 = curves_func_438(arg0->unkA0, -1);
        }
        if (var_v0 == -1) {
            return 1;
        }
        
        arg0->unkA4 = curves_func_39C(var_v0);
        if (arg0->unkA4 == NULL) {
            arg0->unkA4 = NULL;
            return 1;
        }
        if (arg0->unk80 != 0) {
            xDiff = arg0->unkA4->pos.x - arg1->srt.transl.x;
            yDiff = arg0->unkA4->pos.y - arg1->srt.transl.y;
            zDiff = arg0->unkA4->pos.z - arg1->srt.transl.z;
        } else {
            xDiff = arg0->unkA0->pos.x - arg1->srt.transl.x;
            yDiff = arg0->unkA0->pos.y - arg1->srt.transl.y;
            zDiff = arg0->unkA0->pos.z - arg1->srt.transl.z;
        }
        
        if (arg2 < sqrtf(SQ(xDiff) + SQ(yDiff) + SQ(zDiff))) {
            return 1;
        }
        
        arg0->unkB8[0] = arg0->unkA0->pos.x;
        arg0->unkB8[1] = arg0->unkA4->pos.x;
        arg0->unkB8[2] = 2.0f * (fsin16_precise((s16) (arg0->unkA0->unk2C << 8)) * (f32) arg0->unkA0->unk2E);
        arg0->unkB8[3] = 2.0f * (fsin16_precise((s16) (arg0->unkA4->unk2C << 8)) * (f32) arg0->unkA4->unk2E);
        
        arg0->unkD8[0] = arg0->unkA0->pos.y;
        arg0->unkD8[1] = arg0->unkA4->pos.y;
        arg0->unkD8[2] = 2.0f * (fsin16_precise((s16) (arg0->unkA0->unk2D << 8)) * (f32) arg0->unkA0->unk2E);
        arg0->unkD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unkA4->unk2D << 8)) * (f32) arg0->unkA4->unk2E);
        
        arg0->unkF8[0] = arg0->unkA0->pos.z;
        arg0->unkF8[1] = arg0->unkA4->pos.z;
        arg0->unkF8[2] = 2.0f * (fcos16_precise((s16) (arg0->unkA0->unk2C << 8)) * (f32) arg0->unkA0->unk2E);
        arg0->unkF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unkA4->unk2C << 8)) * (f32) arg0->unkA4->unk2E);
        
        if (curves_func_4704(arg0) != 0) {
            return 1;
        }
        
        arg0->splineFunc = curves_hermite;
        arg0->splineConverterFunc = curves_hermite_converter;
        arg0->unk84 = arg0->unkA8;
        arg0->unk88 = arg0->unkC8;
        arg0->unk8C = arg0->unkE8;
        arg0->numControlPoints = 8;
        
        curves_move(arg0);
        
        return 0;
    }

    return 1;
}

// offset: 0x4704 | func: 38 | export: 35
s32 curves_func_4704(UnkCurvesStruct* arg0) {
    s32 var_v0;

    if (arg0 == NULL) {
        return 1;
    }
    if ((arg0->unkA0 == NULL) || (arg0->unkA4 == NULL)) {
        return 1;
    }
    arg0->unk9C = arg0->unkA0;
    arg0->unkA0 = arg0->unkA4;
    memcpy(arg0->unkA8, arg0->unkB8, sizeof(f32) * 4);
    memcpy(arg0->unkC8, arg0->unkD8, sizeof(f32) * 4);
    memcpy(arg0->unkE8, arg0->unkF8, sizeof(f32) * 4);
    if (arg0->unk80 != 0) {
        var_v0 = curves_func_590(arg0->unkA0, -1);
    } else {
        var_v0 = curves_func_438(arg0->unkA0, -1);
    }
    if (var_v0 != -1) {
        arg0->unkA4 = curves_func_39C(var_v0);
        if (arg0->unkA4 != NULL) {
            if (arg0->unk80 != 0) {
                arg0->unkB8[0] = arg0->unkA0->pos.x;
                arg0->unkB8[1] = arg0->unk9C->pos.x;
                arg0->unkB8[2] = 2.0f * (fsin16_precise((s16) (arg0->unkA0->unk2C << 8)) * (f32) arg0->unkA0->unk2E);
                arg0->unkB8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk9C->unk2C << 8)) * (f32) arg0->unk9C->unk2E);
                
                arg0->unkD8[0] = arg0->unkA0->pos.y;
                arg0->unkD8[1] = arg0->unk9C->pos.y;
                arg0->unkD8[2] = 2.0f * (fsin16_precise((s16) (arg0->unkA0->unk2D << 8)) * (f32) arg0->unkA0->unk2E);
                arg0->unkD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk9C->unk2D << 8)) * (f32) arg0->unk9C->unk2E);
                
                arg0->unkF8[0] = arg0->unkA0->pos.z;
                arg0->unkF8[1] = arg0->unk9C->pos.z;
                arg0->unkF8[2] = 2.0f * (fcos16_precise((s16) (arg0->unkA0->unk2C << 8)) * (f32) arg0->unkA0->unk2E);
                arg0->unkF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unk9C->unk2C << 8)) * (f32) arg0->unk9C->unk2E);
            } else {
                arg0->unkB8[0] = arg0->unkA0->pos.x;
                arg0->unkB8[1] = arg0->unkA4->pos.x;
                arg0->unkB8[2] = 2.0f * (fsin16_precise((s16) (arg0->unkA0->unk2C << 8)) * (f32) arg0->unkA0->unk2E);
                arg0->unkB8[3] = 2.0f * (fsin16_precise((s16) (arg0->unkA4->unk2C << 8)) * (f32) arg0->unkA4->unk2E);
                
                arg0->unkD8[0] = arg0->unkA0->pos.y;
                arg0->unkD8[1] = arg0->unkA4->pos.y;
                arg0->unkD8[2] = 2.0f * (fsin16_precise((s16) (arg0->unkA0->unk2D << 8)) * (f32)  arg0->unkA0->unk2E);
                arg0->unkD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unkA4->unk2D << 8)) * (f32) arg0->unkA4->unk2E);
                
                arg0->unkF8[0] = arg0->unkA0->pos.z;
                arg0->unkF8[1] = arg0->unkA4->pos.z;
                arg0->unkF8[2] = 2.0f * (fcos16_precise((s16) (arg0->unkA0->unk2C << 8)) * (f32) arg0->unkA0->unk2E);
                arg0->unkF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unkA4->unk2C << 8)) * (f32) arg0->unkA4->unk2E);
            }
            if (arg0->numControlPoints != 0) {
                curves_setup_move_network_curve(arg0);
            }
            if (arg0->unk80 != 0) {
                curves_func_800053B0(arg0, -1.0f);
                return 0;
            }
            curves_func_800053B0(arg0, 1.0f);
            return 0;
        }
    } else {
        arg0->unkA4 = NULL;
    }
    
    return 1;
}

// offset: 0x4CB4 | func: 39 | export: 36
s32 curves_func_4CB4(UnkCurvesStruct* arg0, s32 arg1) {
    f32 sp24;
    CurveSetup* temp_t7;

    if (arg1 == arg0->unk80) {
        return 0;
    }

    if ((arg0->unkA0 == NULL) || (arg0->unk9C == NULL)) {
        return 1;
    }
    
    sp24 = arg0->unk0;

    arg0->unk80 = arg1;
    temp_t7 = arg0->unk9C;
    arg0->unk9C = arg0->unkA4;
    arg0->unkA4 = temp_t7;
    
    arg0->unkB8[0] = arg0->unkA0->pos.x;
    arg0->unkB8[1] = arg0->unkA4->pos.x;
    arg0->unkB8[2] = 2.0f * (fsin16_precise((s16) (arg0->unkA0->unk2C << 8)) * (f32) arg0->unkA0->unk2E);
    arg0->unkB8[3] = 2.0f * (fsin16_precise((s16) (arg0->unkA4->unk2C << 8)) * (f32) arg0->unkA4->unk2E);
    
    arg0->unkD8[0] = arg0->unkA0->pos.y;
    arg0->unkD8[1] = arg0->unkA4->pos.y;
    arg0->unkD8[2] = 2.0f * (fsin16_precise((s16) (arg0->unkA0->unk2D << 8)) * (f32) arg0->unkA0->unk2E);
    arg0->unkD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unkA4->unk2D << 8)) * (f32) arg0->unkA4->unk2E);
    
    arg0->unkF8[0] = arg0->unkA0->pos.z;
    arg0->unkF8[1] = arg0->unkA4->pos.z;
    arg0->unkF8[2] = 2.0f * (fcos16_precise((s16) (arg0->unkA0->unk2C << 8)) * (f32) arg0->unkA0->unk2E);
    arg0->unkF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unkA4->unk2C << 8)) * (f32) arg0->unkA4->unk2E);
    
    if (curves_func_4704(arg0) != 0) {
        return 1;
    }
    
    arg0->splineFunc = curves_hermite;
    arg0->splineConverterFunc = curves_hermite_converter;
    arg0->unk84 = arg0->unkA8;
    arg0->unk88 = arg0->unkC8;
    arg0->unk8C = arg0->unkE8;
    arg0->numControlPoints = 8;
    
    curves_move(arg0);
    arg0->unk0 = sp24;
    return 0;
}

// offset: 0x4FA8 | func: 40 | export: 37
void curves_func_4FA8(UnkCurvesStruct* arg0, CurveSetup* arg1) {
    if ((arg1 != NULL) && (arg1 != arg0->unkA4)) {
        arg0->unkA4 = arg1;
        
        arg0->unkB8[1] = arg1->pos.x;
        arg0->unkB8[3] = 2.0f * (fsin16_precise((s16) (arg1->unk2C << 8)) * (f32) arg0->unkA4->unk2E);
        
        arg0->unkD8[1] = arg0->unkA4->pos.y;
        arg0->unkD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unkA4->unk2D << 8)) * (f32) arg0->unkA4->unk2E);
        
        arg0->unkF8[1] = arg0->unkA4->pos.z;
        arg0->unkF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unkA4->unk2C << 8)) * (f32) arg0->unkA4->unk2E);
    }
}

// offset: 0x50E4 | func: 41 | export: 38
s32 curves_func_50E4(UnkCurvesStruct* arg0, s32 arg1) {
    s32 var_v0;

    if (arg0 == NULL) {
        return 1;
    }

    if ((arg0->unkA0 == NULL) || ((arg0->unkA4 == NULL))) {
        return 1;
    }
    arg0->unk9C = arg0->unkA0;
    arg0->unkA0 = arg0->unkA4;
    memcpy(arg0->unkA8, arg0->unkB8, sizeof(f32)*4);
    memcpy(arg0->unkC8, arg0->unkD8, sizeof(f32)*4);
    memcpy(arg0->unkE8, arg0->unkF8, sizeof(f32)*4);
    if (arg0->unk80 != 0) {
        var_v0 = curves_func_577C(arg0->unkA0, -1, arg1);
    } else {
        var_v0 = curves_func_5698(arg0->unkA0, -1, arg1);
    }
    if (var_v0 != -1) {
        arg0->unkA4 = curves_func_39C(var_v0);
        if (arg0->unkA4 != NULL) {
            if (arg0->unk80 != 0) {
                arg0->unkB8[0] = arg0->unkA0->pos.x;
                arg0->unkB8[1] = arg0->unk9C->pos.x;
                arg0->unkB8[2] = 2.0f * (fsin16_precise((s16) (arg0->unkA0->unk2C << 8)) * (f32) arg0->unkA0->unk2E);
                arg0->unkB8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk9C->unk2C << 8)) * (f32) arg0->unk9C->unk2E);
                
                arg0->unkD8[0] = arg0->unkA0->pos.y;
                arg0->unkD8[1] = arg0->unk9C->pos.y;
                arg0->unkD8[2] = 2.0f * (fsin16_precise((s16) (arg0->unkA0->unk2D << 8)) * (f32) arg0->unkA0->unk2E);
                arg0->unkD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unk9C->unk2D << 8)) * (f32) arg0->unk9C->unk2E);
                
                arg0->unkF8[0] = arg0->unkA0->pos.z;
                arg0->unkF8[1] = arg0->unk9C->pos.z;
                arg0->unkF8[2] = 2.0f * (fcos16_precise((s16) (arg0->unkA0->unk2C << 8)) * (f32) arg0->unkA0->unk2E);
                arg0->unkF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unk9C->unk2C << 8)) * (f32) arg0->unk9C->unk2E);
            } else {
                arg0->unkB8[0] = arg0->unkA0->pos.x;
                arg0->unkB8[1] = arg0->unkA4->pos.x;
                arg0->unkB8[2] = 2.0f * (fsin16_precise((s16) (arg0->unkA0->unk2C << 8)) * (f32) arg0->unkA0->unk2E);
                arg0->unkB8[3] = 2.0f * (fsin16_precise((s16) (arg0->unkA4->unk2C << 8)) * (f32) arg0->unkA4->unk2E);
                
                arg0->unkD8[0] = arg0->unkA0->pos.y;
                arg0->unkD8[1] = arg0->unkA4->pos.y;
                arg0->unkD8[2] = 2.0f * (fsin16_precise((s16) (arg0->unkA0->unk2D << 8)) * (f32) arg0->unkA0->unk2E);
                arg0->unkD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unkA4->unk2D << 8)) * (f32) arg0->unkA4->unk2E);
                
                arg0->unkF8[0] = arg0->unkA0->pos.z;
                arg0->unkF8[1] = arg0->unkA4->pos.z;
                arg0->unkF8[2] = 2.0f * (fcos16_precise((s16) (arg0->unkA0->unk2C << 8)) * (f32) arg0->unkA0->unk2E);
                arg0->unkF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unkA4->unk2C << 8)) * (f32) arg0->unkA4->unk2E);
            }
            if (arg0->numControlPoints != 0) {
                curves_setup_move_network_curve(arg0);
            }
            if (arg0->unk80 != 0) {
                curves_func_800053B0(arg0, -1.0f);
                return 0;
            }
            curves_func_800053B0(arg0, 1.0f);
            return 0;
        }
    } else {
        arg0->unkA4 = NULL;
    }
    
    return 1;
}

// offset: 0x5698 | func: 42 | export: 39
s32 curves_func_5698(CurveSetup* arg0, s32 arg1, s32 arg2) {
    s32 sp40[4];
    s32 var_a3;
    s32 var_t1;
    s32 var_v1;

    var_t1 = 0;
    var_v1 = 1;
    var_a3 = 0;
    while (var_a3 < 4) {
        if ((arg0->links[var_a3] > -1) && !(arg0->unk1B & var_v1) && (arg1 != arg0->links[var_a3])) {
            sp40[var_t1] = arg0->links[var_a3];
            var_t1 += 1;
        }
        var_a3++;
        var_v1 <<= 1;
    }
    if (var_t1 != 0) {
        if (arg2 != -1) {
            STUBBED_PRINTF(" list Branch No %i ", sp40[arg2]);
        }
        if (arg2 > (var_t1 - 1)) {
            arg2 = var_t1 - 1;
        }
        if (arg2 == -1) {
            arg2 = rand_next(0, var_t1 - 1);
        }
        return sp40[arg2];
    }
    return -1;
}

// offset: 0x577C | func: 43 | export: 40
s32 curves_func_577C(CurveSetup* arg0, s32 arg1, s32 arg2) {
    s32 sp40[4];
    s32 var_a3;
    s32 var_t1;
    s32 var_v1;

    var_t1 = 0;
    var_v1 = 1;
    var_a3 = 0;
    while (var_a3 < 4) {
        if ((arg0->links[var_a3] > -1) && (arg0->unk1B & var_v1) && (arg1 != arg0->links[var_a3])) {
            sp40[var_t1] = arg0->links[var_a3];
            var_t1 += 1;
        }
        var_a3++;
        var_v1 <<= 1;
    }
    if (var_t1 != 0) {
        if (arg2 != -1) {
            STUBBED_PRINTF(" list Branch %i Max %i Branch No %i ", arg2, var_t1 - 1, sp40[arg2]);
        }
        if (arg2 > (var_t1 - 1)) {
            arg2 = var_t1 - 1;
        }
        if (arg2 == -1) {
            arg2 = rand_next(0, var_t1 - 1);
        }
        return sp40[arg2];
    }
    return -1;
}

// offset: 0x5860 | func: 44 | export: 41
s32 curves_func_5860(UnkCurvesStruct* arg0, s32 arg1, s32 arg2) {
    s32 var_v0_2;

    if (arg0 == NULL) {
        return 1;
    }
    
    if (arg2 != -1) {
        if (arg0->unk80 != 0) {
            arg0->unkA0 = curves_func_39C(arg2);
            arg2 = curves_func_438(arg0->unkA0, -1);
            if (arg2 == -1) {
                return 1;
            }
        }
        arg0->unkA0 = curves_func_39C(arg2);
        if (arg0->unkA0 == NULL) {
            arg0->unkA0 = NULL;
            return 1;
        }
        if (arg0->unk80 != 0) {
            var_v0_2 = curves_func_590(arg0->unkA0, -1);
        } else {
            var_v0_2 = curves_func_438(arg0->unkA0, -1);
        }
        if (var_v0_2 == -1) {
            return 1;
        }
        arg0->unkA4 = curves_func_39C(var_v0_2);
        if (arg0->unkA4 == NULL) {
            arg0->unkA4 = NULL;
            return 1;
        }
        
        arg0->unkB8[0] = arg0->unkA0->pos.x;
        arg0->unkB8[1] = arg0->unkA4->pos.x;
        arg0->unkB8[2] = 2.0f * (fsin16_precise((s16) (arg0->unkA0->unk2C << 8)) * (f32) arg0->unkA0->unk2E);
        arg0->unkB8[3] = 2.0f * (fsin16_precise((s16) (arg0->unkA4->unk2C << 8)) * (f32) arg0->unkA4->unk2E);
        
        arg0->unkD8[0] = arg0->unkA0->pos.y;
        arg0->unkD8[1] = arg0->unkA4->pos.y;
        arg0->unkD8[2] = 2.0f * (fsin16_precise((s16) (arg0->unkA0->unk2D << 8)) * (f32) arg0->unkA0->unk2E);
        arg0->unkD8[3] = 2.0f * (fsin16_precise((s16) (arg0->unkA4->unk2D << 8)) * (f32) arg0->unkA4->unk2E);
        
        arg0->unkF8[0] = arg0->unkA0->pos.z;
        arg0->unkF8[1] = arg0->unkA4->pos.z;
        arg0->unkF8[2] = 2.0f * (fcos16_precise((s16) (arg0->unkA0->unk2C << 8)) * (f32) arg0->unkA0->unk2E);
        arg0->unkF8[3] = 2.0f * (fcos16_precise((s16) (arg0->unkA4->unk2C << 8)) * (f32) arg0->unkA4->unk2E);
        
        if (curves_func_4704(arg0) != 0) {
            return 1;
        
        }

        arg0->splineFunc = curves_hermite;
        arg0->splineConverterFunc = curves_hermite_converter;
        arg0->unk84 = arg0->unkA8;
        arg0->unk88 = arg0->unkC8;
        arg0->unk8C = arg0->unkE8;
        arg0->numControlPoints = 8;
        
        curves_move(arg0);

        return 0;
    }
    return 1;
}
