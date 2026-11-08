#include "dlls/engine/25.h"

#include "PR/ultratypes.h"
#include "PR/os.h"
#include "dlls/engine/26_curves.h"
#include "game/objects/object.h"
#include "sys/curves.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "dll.h"
#include "constants.h"
#include "libc/string.h"
#include "macros.h"

/* 0x0 */ static const char errorMsg[] = "Error in isPointWithinPatchGroup\n";

/* 0x00 */ static u8 _bss_0[0xdc];
/* 0xDC */ static Dll25BssDC *_bss_DC;
/* 0xE0 */ static Dll25BssE0 *_bss_E0;
/* 0xE4 */ static s32 _bss_E4;
/* 0xE8 */ static u8 _bss_E8[0x8];

static s32 dll_25_func_DF8(Dll25BssDC *arg0, f32 arg1, f32 arg2);
static void dll_25_func_D00(Vec3f *arg0, Vec3f *arg1, s8 *arg2, f32 *arg3);

// offset: 0x0 | ctor
void dll_25_ctor(s32 arg0)
{
    bzero(_bss_0, 0xD9);
    _bss_DC = (void *)mmAlloc(sizeof(Dll25BssDC) * 217, ALLOC_TAG_TRACK_COL, 0);
    _bss_E0 = (void *)mmAlloc(sizeof(Dll25BssE0) * 256, ALLOC_TAG_TRACK_COL, 0);
}

// offset: 0x84 | dtor
void dll_25_dtor(s32 arg0)
{
    mmFree(_bss_DC);
    mmFree(_bss_E0);
}

// offset: 0xD8 | func: 0 | export: 0
void dll_25_func_D8(void)
{
    Dll25BssDC *var_s3;
    u32 *sp274[256] = {0};
    s32 i;
    Dll25BssDC *temp_s0;
    Dll25BssDC *temp_s1_2;
    s32 curveListCount; // sp264
    s32 otherCount;
    Dll25BssDC *temp_s2_2;
    Dll25BssE0 *var_v1_3;
    Vec3f sp24C;
    Vec3f sp240;
    CurveNode *curveList;
    CurveSetup *curve;
    CurveSetup *var_s7;
    f32 temp_ft1;
    f32 temp_ft3;
    f32 temp_ft5;
    f32 temp_fv0;
    f32 temp_fv1;
    s32 temp_t7;
    s32 var_s1;
    s32 var_s0;
    s8 *temp_t4;
    s32 temp_v0;
    s32 _pad;
    CurveSetup *linkCurve;
    u8 sp80[388]; // unknown size, variable used for padding
    s32 temp_t1;
    s32 linkno;
    f32 sp70[2];

    curveList = gDLL_26_Curves->vtbl->GetCurves(&curveListCount);
    bzero(_bss_0, 0xD9);
    for (i = 0; i < 256; i++)
    {
        _bss_E0[i].unk1C = 0;
    }
    _bss_E4 = 1;
    for (i = 0; i < curveListCount; i++)
    {
        curve = curveList[i].setup;
        if (curve->curveType == 0x26)
        {
            temp_v0 = curve->unk3;
            temp_s0 = &_bss_DC[temp_v0];
            _bss_0[temp_v0] = 1;
            sp24C.f[0] = (curve->base_type26.unk4 * 5.7f) + curve->pos.x;
            sp24C.f[2] = (curve->base_type26.unk5 * 5.7f) + curve->pos.z;
            sp240.f[0] = (curve->base_type26.unk6 * 5.7f) + curve->pos.x;
            sp240.f[2] = (curve->base_type26.unk7 * 5.7f) + curve->pos.z;
            dll_25_func_D00(&sp24C, &sp240, &temp_s0->unk0[0], &temp_s0->unk8[0]);
            sp24C.f[0] = (curve->type26.unk30 * 5.7f) + curve->pos.x;
            sp24C.f[2] = (curve->type26.unk31 * 5.7f) + curve->pos.z;
            dll_25_func_D00(&sp240, &sp24C, &temp_s0->unk0[2], &temp_s0->unk8[1]);
            sp240.f[0] = (curve->type26.unk32 * 5.7f) + curve->pos.x;
            sp240.f[2] = (curve->type26.unk33 * 5.7f) + curve->pos.z;
            dll_25_func_D00(&sp24C, &sp240, &temp_s0->unk0[4], &temp_s0->unk8[2]);
            sp24C.f[0] = (curve->base_type26.unk4 * 5.7f) + curve->pos.x;
            sp24C.f[2] = (curve->base_type26.unk5 * 5.7f) + curve->pos.z;
            dll_25_func_D00(&sp240, &sp24C, &temp_s0->unk0[6], &temp_s0->unk8[3]);

            temp_s0->unk18 = curve->pos.y + (2.0f * curve->unk18);
            temp_s0->unk1A.word = curve->pos.y - (2.0f * curve->unk1A);
            for (linkno = 0; linkno < 4; linkno++)
            {
                temp_s0->unk1C[linkno] = 0;
                if (curve->links[linkno] >= 0)
                {
                    linkCurve = gDLL_26_Curves->vtbl->FindByUID(curve->links[linkno]);
                    temp_t1 = curve->unk3 * linkCurve->unk3;
                    for (var_s0 = 1; var_s0 < _bss_E4; var_s0++)
                    {
                        if (temp_t1 == _bss_E0[var_s0].unk1C)
                        {
                            temp_s0->unk1C[linkno] = var_s0;
                            break;
                        }
                    }
                    if (temp_s0->unk1C[linkno] == 0)
                    {
                        for (var_s0 = 0; var_s0 < 4; var_s0++)
                        {
                            if ((s32)curve->uID == linkCurve->links[var_s0])
                            {
                                break;
                            }
                        }
                        temp_s0->unk1C[linkno] = _bss_E4;
                        _bss_E0[_bss_E4].unk1C = temp_t1;
                        sp80[_bss_E4 * 2 + 0] = curve->unk3;
                        sp80[_bss_E4 * 2 + 1] = linkCurve->unk3;
                        sp24C.f[0] = (curve->type26.unk34[linkno][0] * 5.7f) + curve->pos.x;
                        sp24C.f[2] = (curve->type26.unk34[linkno][1] * 5.7f) + curve->pos.z;
                        sp240.f[0] = (curve->type26.unk34[linkno][2] * 5.7f) + curve->pos.x;
                        sp240.f[2] = (curve->type26.unk34[linkno][3] * 5.7f) + curve->pos.z;
                        _bss_E0[_bss_E4].unk1E = (sp24C.f[0] + sp240.f[0]) / 2; // use 2 instead of 2.0f to force a div.s
                        _bss_E0[_bss_E4].unk20 = (sp24C.f[2] + sp240.f[2]) / 2; // use 2 instead of 2.0f to force a div.s
                        dll_25_func_D00(&sp24C, &sp240, &_bss_E0[_bss_E4].unk0[0], &_bss_E0[_bss_E4].unk8[0]);
                        sp24C.f[0] = (linkCurve->type26.unk34[var_s0][0] * 5.7f) + linkCurve->pos.x;
                        sp24C.f[2] = (linkCurve->type26.unk34[var_s0][1] * 5.7f) + linkCurve->pos.z;
                        dll_25_func_D00(&sp240, &sp24C, &_bss_E0[_bss_E4].unk0[2], &_bss_E0[_bss_E4].unk8[1]);
                        sp240.f[0] = (linkCurve->type26.unk34[var_s0][2] * 5.7f) + linkCurve->pos.x;
                        sp240.f[2] = (linkCurve->type26.unk34[var_s0][3] * 5.7f) + linkCurve->pos.z;
                        _bss_E0[_bss_E4].unk22 = (sp24C.f[0] + sp240.f[0]) / 2; // use 2 instead of 2.0f to force a div.s
                        _bss_E0[_bss_E4].unk24 = (sp24C.f[2] + sp240.f[2]) / 2; // use 2 instead of 2.0f to force a div.s
                        dll_25_func_D00(&sp24C, &sp240, &_bss_E0[_bss_E4].unk0[4], &_bss_E0[_bss_E4].unk8[2]);
                        sp24C.f[0] = (curve->type26.unk34[linkno][0] * 5.7f) + curve->pos.x;
                        sp24C.f[2] = (curve->type26.unk34[linkno][1] * 5.7f) + curve->pos.z;
                        dll_25_func_D00(&sp240, &sp24C, &_bss_E0[_bss_E4].unk0[6], &_bss_E0[_bss_E4].unk8[3]);
                        temp_fv0 = curve->pos.y + (2.0f * curve->unk18);
                        temp_fv1 = linkCurve->pos.y + (2.0f * linkCurve->unk18);
                        if (temp_fv1 < temp_fv0)
                        {
                            _bss_E0[_bss_E4].unk18 = temp_fv0;
                        }
                        else
                        {
                            _bss_E0[_bss_E4].unk18 = temp_fv1;
                        }
                        temp_fv0 = curve->pos.y - (2.0f * curve->unk1A);
                        temp_fv1 = linkCurve->pos.y - (2.0f * linkCurve->unk1A);
                        if (temp_fv0 < temp_fv1)
                        {
                            _bss_E0[_bss_E4].unk1A.word = temp_fv0;
                        }
                        else
                        {
                            _bss_E0[_bss_E4].unk1A.word = temp_fv1;
                        }
                        _bss_E4 += 1;
                    }
                }
            }
        }
    }
    for (i = 1; i < _bss_E4; i++)
    {
        temp_s1_2 = &_bss_DC[sp80[i * 2]];
        temp_s2_2 = &_bss_DC[sp80[i * 2 + 1]];
        sp70[0] = _bss_E0[i].unk22 - _bss_E0[i].unk1E;
        sp70[1] = _bss_E0[i].unk24 - _bss_E0[i].unk20;
        while (
            dll_25_func_DF8(temp_s1_2, _bss_E0[i].unk1E, _bss_E0[i].unk20) == 0 &&
            dll_25_func_DF8(temp_s2_2, _bss_E0[i].unk1E, _bss_E0[i].unk20) == 0)
        {
            _bss_E0[i].unk1E += sp70[0] / 20.0f;
            _bss_E0[i].unk20 += sp70[1] / 20.0f;
        }
        while (
            dll_25_func_DF8(temp_s1_2, _bss_E0[i].unk22, _bss_E0[i].unk24) == 0 &&
            dll_25_func_DF8(temp_s2_2, _bss_E0[i].unk22, _bss_E0[i].unk24) == 0)
        {
            _bss_E0[i].unk22 -= sp70[0] / 20.0f;
            _bss_E0[i].unk24 -= sp70[1] / 20.0f;
        }
    }
}

// offset: 0xD00 | func: 1
static void dll_25_func_D00(Vec3f *arg0, Vec3f *arg1, s8 *arg2, f32 *arg3)
{
    f32 magnitude;
    f32 z;
    f32 x;

    z = arg1->z - arg0->z;
    x = arg0->x - arg1->x;
    magnitude = sqrtf((z * z) + (x * x));
    if (magnitude != 0.0f) {
        z /= magnitude;
        x /= magnitude;
    }
    arg2[0] = z * 127.0f;
    arg2[1] = x * 127.0f;
    arg3[0] = -((arg2[0] * arg0->f[0]) + (arg2[1] * arg0->f[2])); // negated dot product
}

// offset: 0xDF8 | func: 2
static s32 dll_25_func_DF8(Dll25BssDC *arg0, f32 x, f32 z)
{
    u8 var_v0;
    u8 var_v1;

    for (var_v0 = 0, var_v1 = 0; var_v0 < 4; var_v0++, var_v1 += 2)
    {
        if (((arg0->unk0[var_v1] * x) + (arg0->unk0[var_v1 + 1] * z) + arg0->unk8[var_v0]) > 0.0f)
        {
            break;
        }
    }
    if (var_v0 == 4)
    {
        return 1;
    }
    return 0;
}

// offset: 0xEAC | func: 3
static s32 dll_25_func_EAC(Vec3f *arg0)
{
    /*0x400*/ static u32 _data_400 = 0;
    s16 var_s0;
    s16 var_s1;
    u32 v1;
    Dll25BssDC *temp_a0;

    v1 = _data_400;
    var_s1 = v1;
    if (_data_400 == 0xD8)
    {
        var_s0 = 0;
    }
    else
    {
        var_s0 = v1 + 1;
    }
    while (var_s1 != var_s0)
    {
        if (_bss_0[var_s1] != 0)
        {
            temp_a0 = &_bss_DC[var_s1];
            if ((arg0->y < temp_a0->unk18) && (temp_a0->unk1A.word < arg0->y) && (dll_25_func_DF8(temp_a0, arg0->x, arg0->z) != 0))
            {
                _data_400 = var_s1;
                return var_s1;
            }
        }
        if (_bss_0[var_s0] != 0)
        {
            temp_a0 = &_bss_DC[var_s0];
            if ((arg0->y < temp_a0->unk18) && (temp_a0->unk1A.word < arg0->y) && (dll_25_func_DF8(temp_a0, arg0->x, arg0->z) != 0))
            {
                _data_400 = var_s0;
                return var_s0;
            }
        }
        var_s1--;
        if (var_s1 == -1)
        {
            var_s1 = 0xD8;
        }
        var_s0++;
        if (var_s0 == 0xD9)
        {
            var_s0 = 0;
        }
    }

    if (_bss_0[var_s1] != 0)
    {
        temp_a0 = &_bss_DC[var_s1];
        if ((arg0->y < (f32)temp_a0->unk18) && (temp_a0->unk1A.word < arg0->y) && (dll_25_func_DF8(temp_a0, arg0->x, arg0->z) != 0))
        {
            _data_400 = var_s1;
            return var_s1;
        }
    }
    return 0;
}

// offset: 0x1158 | func: 4 | export: 1
s32 dll_25_func_1158(Vec3f *arg0, DLL25Func1158Struct *arg1)
{
    u8 temp_v0;
    u8 var_a0;
    u8 var_a3;
    u8 var_t0;
    u8 temp_a1;
    u8 var_v0;
    Dll25BssE0 *temp_v1;

    temp_v0 = dll_25_func_EAC(arg0);
    if (arg1 != NULL && temp_v0 != 0)
    {
        arg1->unk0[0] = temp_v0;
        arg1->unk0[1] = 0;
        for (var_a3 = 0, var_t0 = 1; var_a3 < 4; var_a3++, var_t0 <<= 1)
        {
            temp_a1 = _bss_DC[temp_v0].unk1C[var_a3];
            if (temp_a1 != 0)
            {
                temp_v1 = &_bss_E0[temp_a1];
                arg1->unk2[var_a3] = temp_v1->unk1C;
                for (var_v0 = 0, var_a0 = 0; var_v0 < 4; var_v0++, var_a0 += 2)
                {
                    if (((temp_v1->unk0[var_a0] * arg0->x) + (temp_v1->unk0[var_a0 + 1] * arg0->z) + temp_v1->unk8[var_v0]) > 0.0f)
                    {
                        break;
                    }
                }
                if (var_v0 == 4)
                {
                    arg1->unk0[1] |= var_t0;
                }
            }
            else
            {
                arg1->unk2[var_a3] = 0;
            }
        }
    }
    return temp_v0;
}

// offset: 0x12FC | func: 5 | export: 2
s32 dll_25_func_12FC(Vec3f *arg0)
{
    Dll25BssE0 *temp_a1;
    s16 var_a0;
    s32 temp;
    s16 var_a2;
    s16 var_v1;
    s8 *temp_v0;

    if (dll_25_func_EAC(arg0) != 0)
    {
        return 1;
    }
    var_a2 = 1;
    while (var_a2 < _bss_E4)
    {
        temp_a1 = &_bss_E0[var_a2];
        if (arg0->y < temp_a1->unk18)
        {
            if (temp_a1->unk1A.word < arg0->y)
            {
                for (var_a0 = 0, var_v1 = 0; var_a0 < 4;)
                {
                    if (((((temp_a1->unk0[var_v1] * arg0->x) + (temp_a1->unk0[var_v1 + 1] * arg0->z)) + temp_a1->unk8[var_a0]) > 0.0f))
                    {
                        break;
                    }

                    var_a0 += 1;
                    var_v1 += 2;
                }
                if (var_a0 == 4)
                {
                    return 1;
                }
            }
        }
        var_a2++;
    }
    return 0;
}

// offset: 0x1474 | func: 6 | export: 3
s32 dll_25_func_1474(Vec3f *arg0, s32 arg1)
{
    Dll25BssE0 *temp_a2;
    u8 var_a3;
    u8 var_t0;
    u8 var_v0;
    u8 temp_a0;

    for (var_v0 = 0; var_v0 < 4; var_v0++)
    {
        if (_bss_0[arg1] != 0)
        {
            temp_a0 = _bss_DC[arg1].unk1C[var_v0];
            if (temp_a0 != 0)
            {
                temp_a2 = &_bss_E0[temp_a0];
                for (var_a3 = 0, var_t0 = 0; var_a3 < 4; var_a3++, var_t0 += 2)
                {
                    if ((((temp_a2->unk0[var_t0] * arg0->x) + (temp_a2->unk0[var_t0 + 1] * arg0->z)) + ((f32 *)temp_a2)[(u32)var_a3 + 2]) > 0.0f)
                    {
                        break;
                    }
                }
                if (var_a3 == 4)
                {
                    return temp_a2->unk1C;
                }
            }
        }
    }

    return 0;
}

// offset: 0x15B0 | func: 7 | export: 4
s32 dll_25_func_15B0(Vec3f *arg0, s32 arg1, s32 arg2)
{
    Dll25BssE0 *temp_a0;
    u8 var_v0;
    u8 var_v0_2;
    u8 var_v1;
    u8 temp_v1;
    s8 *temp;

    for (var_v0 = 0; var_v0 < 4; var_v0++)
    {
        temp_v1 = _bss_DC[arg1].unk1C[var_v0];
        if (temp_v1 != 0)
        {
            temp_a0 = &_bss_E0[temp_v1];
            if (arg2 == temp_a0->unk1C)
            {
                for (var_v0_2 = 0, var_v1 = 0; var_v0_2 < 4; var_v0_2++, var_v1 += 2)
                {
                    temp = &temp_a0->unk0[var_v1];
                    if (((temp[0] * arg0->x) + (temp_a0->unk0[var_v1 + 1] * arg0->z) + ((f32 *)temp_a0)[(u32)var_v0_2 + 2]) > 0.0f)
                    {
                        break;
                    }
                }
                if (var_v0_2 == 4)
                {
                    return 1;
                }
                return 0;
            }
        }
    }

    return 0;
}

// offset: 0x16D4 | func: 8 | export: 7
s32 dll_25_func_16D4(Vec3f *arg0, Vec3f *arg1, s32 arg2)
{
    u8 i;
    f32 tempDistance;
    s32 pad;
    PRAGMA_IGNORE_PUSH("-Wtype-limits");
    for (i = 0; i < 256; i++) // @bug: This condition is always true!
    {
        if (arg2 == _bss_E0[i].unk1C)
        {
            break;
        }
    }
    PRAGMA_IGNORE_POP()
    arg1->x = _bss_E0[i].unk1E;
    arg1->y = arg0->y;
    arg1->z = _bss_E0[i].unk20;
    tempDistance = vec3DistanceSquared(arg0, arg1);
    arg1->x = _bss_E0[i].unk22;
    arg1->z = _bss_E0[i].unk24;
    if (vec3DistanceSquared(arg0, arg1) < tempDistance)
    {
        return 1;
    }

    arg1->x = _bss_E0[i].unk1E;
    arg1->z = _bss_E0[i].unk20;
    return 1;
}

// offset: 0x1864 | func: 9 | export: 5
CurveSetup* dll_25_func_1864(Vec3f* pos, s32 arg1, s32 arg2)
{
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fs0;
    s32 i;
    s32 curveListCount;
    CurveSetup *curve;
    CurveNode *curveList;
    CurveSetup *out;

    curveList = gDLL_26_Curves->vtbl->GetCurves(&curveListCount);
    out = NULL;
    var_fs0 = F32_MAX;
    for (i = 0; i < curveListCount; i++)
    {
        curve = curveList[i].setup;
        if ((curve != NULL) && 
            (curve->curveType == 0x24) && 
            ((arg1 == -1) || (arg1 == curve->unk3)) &&
            ((arg2 == -1) || (arg2 == curve->unk1A)) &&
            ((curve->links[0] != -1) || (curve->unk2F == -1)) &&
            ((curve->type24.activeBit == -1) || (mainGetBits(curve->type24.activeBit) != 0)) &&
            ((curve->type24.usedBit == -1) || (mainGetBits(curve->type24.usedBit) == 0)))
        {
            temp_fv0 = pos->f[0] - curve->pos.x;
            temp_fv1 = pos->f[1] - curve->pos.y;
            temp_fa0 = pos->f[2] - curve->pos.z;
            temp_fa1 = (temp_fv0 * temp_fv0) + (temp_fv1 * temp_fv1) + (temp_fa0 * temp_fa0);
            if (temp_fa1 < var_fs0)
            {
                var_fs0 = temp_fa1;
                out = curve;
            }
        }
    }
    return out;
}

// offset: 0x1A2C | func: 10 | export: 6
CurveSetup *dll_25_func_1A2C(Vec3f *arg0, s32 arg1, s32 arg2)
{
    CurveNode *result;
    CurveSetup *temp_v1;
    CurveSetup *out;
    f32 temp_fa1;
    f32 temp_ft4;
    f32 temp_ft5;
    s32 count;
    f32 var_fs0;
    s32 var_t0;
    s32 var_v1;

    result = gDLL_26_Curves->vtbl->GetCurves(&count);
    out = NULL;
    var_v1 = 0;
    var_fs0 = F32_MAX;
    for (var_v1 = 0; var_v1 < count; var_v1++)
    {
        temp_v1 = result[var_v1].setup;
        if ((temp_v1 != NULL) &&
            (temp_v1->curveType == 0x24) &&
            ((arg1 == -1) || (arg1 == temp_v1->unk3)) &&
            ((arg2 == -1) || (arg2 == temp_v1->unk1A)) &&
            ((temp_v1->links[0] != -1) || (temp_v1->unk2F == -1)))
        {
            temp_fa1 = arg0->f[0] - temp_v1->pos.x;
            temp_ft4 = arg0->f[1] - temp_v1->pos.y;
            temp_ft5 = arg0->f[2] - temp_v1->pos.z;
            if (((temp_fa1 * temp_fa1) + (temp_ft4 * temp_ft4) + (temp_ft5 * temp_ft5)) < var_fs0)
            {
                out = temp_v1;
                var_fs0 = (temp_fa1 * temp_fa1) + (temp_ft4 * temp_ft4) + (temp_ft5 * temp_ft5);
            }
        }
    }
    return out;
}

// offset: 0x1D30 | func: 11 | export: 8
s32 dll_25_func_1D30(UnkCurvesStruct *arg0, CurveSetup *arg1, CurveSetup *arg2, CurveSetup *arg3)
{
    if (arg0->unk0.unk80 != 0)
    {
        arg0->unkA0 = arg1;
        arg0->unkA4 = arg2;
        arg0->unkA8[0] = arg2->pos.x;
        arg0->unkA8[1] = arg1->pos.x;
        arg0->unkA8[2] = 2.0f * (mathSinfInterp(arg2->unk2C << 8) * arg0->unkA4->unk2E);
        arg0->unkA8[3] = 2.0f * (mathSinfInterp(arg0->unkA0->unk2C << 8) * arg0->unkA0->unk2E);
        arg0->unkC8[0] = arg0->unkA4->pos.y;
        arg0->unkC8[1] = arg0->unkA0->pos.y;
        arg0->unkC8[2] = 2.0f * (mathSinfInterp(arg0->unkA4->unk2D << 8) * arg0->unkA4->unk2E);
        arg0->unkC8[3] = 2.0f * (mathSinfInterp(arg0->unkA0->unk2D << 8) * arg0->unkA0->unk2E);
        arg0->unkE8[0] = arg0->unkA4->pos.z;
        arg0->unkE8[1] = arg0->unkA0->pos.z;
        arg0->unkE8[2] = 2.0f * (mathCosfInterp(arg0->unkA4->unk2C << 8) * arg0->unkA4->unk2E);
        arg0->unkE8[3] = 2.0f * (mathCosfInterp(arg0->unkA0->unk2C << 8) * arg0->unkA0->unk2E);
    }
    else
    {
        arg0->unkA0 = arg1;
        arg0->unkA4 = arg2;
        arg0->unkB8[0] = arg1->pos.x;
        arg0->unkB8[1] = arg2->pos.x;
        arg0->unkB8[2] = 2.0f * (mathSinfInterp(arg1->unk2C << 8) * arg0->unkA0->unk2E);
        arg0->unkB8[3] = 2.0f * (mathSinfInterp(arg0->unkA4->unk2C << 8) * arg0->unkA4->unk2E);
        arg0->unkD8[0] = arg0->unkA0->pos.y;
        arg0->unkD8[1] = arg0->unkA4->pos.y;
        arg0->unkD8[2] = 2.0f * (mathSinfInterp(arg0->unkA0->unk2D << 8) * arg0->unkA0->unk2E);
        arg0->unkD8[3] = 2.0f * (mathSinfInterp(arg0->unkA4->unk2D << 8) * arg0->unkA4->unk2E);
        arg0->unkF8[0] = arg0->unkA0->pos.z;
        arg0->unkF8[1] = arg0->unkA4->pos.z;
        arg0->unkF8[2] = 2.0f * (mathCosfInterp(arg0->unkA0->unk2C << 8) * arg0->unkA0->unk2E);
        arg0->unkF8[3] = 2.0f * (mathCosfInterp(arg0->unkA4->unk2C << 8) * arg0->unkA4->unk2E);
    }
    if (gDLL_25->vtbl->func_21F4(arg0, arg3) != 0)
    {
        return 1;
    }
    arg0->unk0.splineFunc = curvesHermite;
    arg0->unk0.splineConverterFunc = curvesHermiteConverter;
    arg0->unk0.unk84 = arg0->unkA8;
    arg0->unk0.unk88 = arg0->unkC8;
    arg0->unk0.unk8C = arg0->unkE8;
    arg0->unk0.numControlPoints = 8;
    curvesMove(&arg0->unk0);
    return 0;
}

// offset: 0x21F4 | func: 12 | export: 9
s32 dll_25_func_21F4(UnkCurvesStruct *arg0, CurveSetup *arg1)
{
    if ((arg0->unkA0 == NULL) || (arg0->unkA4 == NULL) || (arg1 == NULL))
    {
        return 1;
    }

    if (arg0->unk0.unk80 != 0)
    {
        arg0->unk9C = arg0->unkA0;
        arg0->unkA0 = arg0->unkA4;
        arg0->unkA4 = arg1;
        memcpy(arg0->unkB8, arg0->unkA8, sizeof(arg0->unkA8));
        memcpy(arg0->unkD8, arg0->unkC8, sizeof(arg0->unkC8));
        memcpy(arg0->unkF8, arg0->unkE8, sizeof(arg0->unkE8));
        arg0->unkA8[0] = arg0->unkA4->pos.x;
        arg0->unkA8[1] = arg0->unkA0->pos.x;
        arg0->unkA8[2] = 2.0f * (mathSinfInterp(arg0->unkA4->unk2C << 8) * arg0->unkA4->unk2E);
        arg0->unkA8[3] = 2.0f * (mathSinfInterp(arg0->unkA0->unk2C << 8) * arg0->unkA0->unk2E);
        arg0->unkC8[0] = arg0->unkA4->pos.y;
        arg0->unkC8[1] = arg0->unkA0->pos.y;
        arg0->unkC8[2] = 2.0f * (mathSinfInterp(arg0->unkA4->unk2D << 8) * arg0->unkA4->unk2E);
        arg0->unkC8[3] = 2.0f * (mathSinfInterp(arg0->unkA0->unk2D << 8) * arg0->unkA0->unk2E);
        arg0->unkE8[0] = arg0->unkA4->pos.z;
        arg0->unkE8[1] = arg0->unkA0->pos.z;
        arg0->unkE8[2] = 2.0f * (mathCosfInterp(arg0->unkA4->unk2C << 8) * arg0->unkA4->unk2E);
        arg0->unkE8[3] = 2.0f * (mathCosfInterp(arg0->unkA0->unk2C << 8) * arg0->unkA0->unk2E);
        if (arg0->unk0.numControlPoints != 0)
        {
            curvesSetupMoveNetworkCurve(&arg0->unk0);
            if (arg0->unk0.unk0 <= 0.0f)
            {
                arg0->unk0.unk0 = 0.01f;
            }
        }
    }
    else
    {
        arg0->unk9C = arg0->unkA0;
        arg0->unkA0 = arg0->unkA4;
        arg0->unkA4 = arg1;
        memcpy(arg0->unkA8, arg0->unkB8, 0x10U);
        memcpy(arg0->unkC8, arg0->unkD8, 0x10U);
        memcpy(arg0->unkE8, arg0->unkF8, 0x10U);
        arg0->unkB8[0] = arg0->unkA0->pos.x;
        arg0->unkB8[1] = arg0->unkA4->pos.x;
        arg0->unkB8[2] = 2.0f * (mathSinfInterp(arg0->unkA0->unk2C << 8) * arg0->unkA0->unk2E);
        arg0->unkB8[3] = 2.0f * (mathSinfInterp(arg0->unkA4->unk2C << 8) * arg0->unkA4->unk2E);
        arg0->unkD8[0] = arg0->unkA0->pos.y;
        arg0->unkD8[1] = arg0->unkA4->pos.y;
        arg0->unkD8[2] = 2.0f * (mathSinfInterp(arg0->unkA0->unk2D << 8) * arg0->unkA0->unk2E);
        arg0->unkD8[3] = 2.0f * (mathSinfInterp(arg0->unkA4->unk2D << 8) * arg0->unkA4->unk2E);
        arg0->unkF8[0] = arg0->unkA0->pos.z;
        arg0->unkF8[1] = arg0->unkA4->pos.z;
        arg0->unkF8[2] = 2.0f * (mathCosfInterp(arg0->unkA0->unk2C << 8) * arg0->unkA0->unk2E);
        arg0->unkF8[3] = 2.0f * (mathCosfInterp(arg0->unkA4->unk2C << 8) * arg0->unkA4->unk2E);
        if (arg0->unk0.numControlPoints != 0)
        {
            curvesSetupMoveNetworkCurve(&arg0->unk0);
            if (arg0->unk0.unk0 >= 1.0f)
            {
                arg0->unk0.unk0 = 0.99f;
            }
        }
    }
    return 0;
}

// offset: 0x27A8 | func: 13 | export: 10
s32 dll_25_func_27A8(UnkCurvesStruct *arg0, CurveSetup *arg1)
{
    if ((arg0->unkA0 == 0) || (arg0->unkA4 == NULL) || (arg1 == NULL))
    {
        return 1;
    }
    arg0->unkA4 = arg1;
    if (arg0->unk0.unk80 != 0)
    {
        arg0->unkA8[0] = arg1->pos.x;
        arg0->unkA8[2] = 2.0f * (mathSinfInterp(arg1->unk2C << 8) * arg1->unk2E);
        arg0->unkC8[0] = arg1->pos.y;
        arg0->unkC8[2] = 2.0f * (mathSinfInterp(arg1->unk2D << 8) * arg1->unk2E);
        arg0->unkE8[0] = arg1->pos.z;
        arg0->unkE8[2] = 2.0f * (mathCosfInterp(arg1->unk2C << 8) * arg1->unk2E);
    }
    else
    {
        arg0->unkB8[1] = arg1->pos.x;
        arg0->unkB8[3] = 2.0f * (mathSinfInterp(arg1->unk2C << 8) * arg1->unk2E);
        arg0->unkD8[1] = arg1->pos.y;
        arg0->unkD8[3] = 2.0f * (mathSinfInterp(arg1->unk2D << 8) * arg1->unk2E);
        arg0->unkF8[1] = arg1->pos.z;
        arg0->unkF8[3] = 2.0f * (mathCosfInterp(arg1->unk2C << 8) * arg1->unk2E);
    }
    return 0;
}

// offset: 0x29FC | func: 14 | export: 11
void dll_25_func_29FC(UnkCurvesStruct* arg0) {
    arg0->unk9C = (CurveSetup *)((s32)arg0->unk9C ^ (s32)arg0->unkA4);\
    arg0->unkA4 = (CurveSetup *)((s32)arg0->unkA4 ^ (s32)arg0->unk9C);\
    arg0->unk9C = (CurveSetup *)((s32)arg0->unk9C ^ (s32)arg0->unkA4);
    if (arg0->unk0.unk0 >= 1.0f) {
        arg0->unk0.unk0 = 0.99f;
    }
}

// offset: 0x2A50 | func: 15 | export: 12
// returns a CurveSetup of type 0x22
CurveSetup *dll_25_func_2A50(Object *obj, s32 arg1)
{
    CurveNode *result;
    CurveSetup *temp_s0;
    CurveSetup *out;
    s32 count;
    f32 temp_fv0;
    f32 var_fs0;
    s32 i;

    result = gDLL_26_Curves->vtbl->GetCurves(&count);
    out = NULL;
    var_fs0 = 1e12f;
    for (i = 0; i < count; i++)
    {
        temp_s0 = result[i].setup;
        if ((temp_s0->curveType == 0x22) &&
            ((arg1 == temp_s0->base_type22.unk4) || ((temp_s0->unk1A < 3) &&
                                                    (arg1 == -1))) &&
            ((temp_s0->type22.activeBit == -1) || (mainGetBits(temp_s0->type22.activeBit) != 0)) &&
            ((temp_s0->type22.usedBit == -1) || (mainGetBits(temp_s0->type22.usedBit) == 0)))
        {
            temp_fv0 = vec3DistanceSquared(&obj->globalPosition, &temp_s0->pos);
            if (temp_fv0 < var_fs0)
            {
                var_fs0 = temp_fv0;
                out = temp_s0;
            }
        }
    }
    return out;
}

// offset: 0x2BC4 | func: 16 | export: 13
// returns a CurveSetup of type 0x22
CurveSetup *dll_25_func_2BC4(Object *obj, s32 arg1)
{
    CurveNode *result;
    CurveSetup *temp_s0;
    CurveSetup *out;
    s32 count;
    f32 temp_fv0;
    f32 var_fs0;
    s32 i;

    result = gDLL_26_Curves->vtbl->GetCurves(&count);
    out = NULL;
    i = 0;
    var_fs0 = 1e12f;
    for (i = 0; i < count; i++)
    {
        temp_s0 = result[i].setup;
        if (
            (temp_s0->curveType == 0x22) &&
            ((arg1 == temp_s0->base_type22.unk4) || ((temp_s0->unk1A < 3) && (arg1 == -1))))
        {
            temp_fv0 = vec3DistanceSquared(&obj->globalPosition, &temp_s0->pos);
            if (temp_fv0 < var_fs0)
            {
                var_fs0 = temp_fv0;
                out = temp_s0;
            }
        }
    }
    return out;
}

// offset: 0x2CF8 | func: 17 | export: 14
CurveSetup *dll_25_func_2CF8(Object *obj, s32 arg1)
{
    CurveNode *result;
    s32 _pad;
    CurveSetup *temp_s0;
    s32 count;
    CurveSetup *out;
    f32 temp_fv0;
    f32 var_fs0;
    s32 i;

    result = gDLL_26_Curves->vtbl->GetCurves(&count);
    out = NULL;
    var_fs0 = 1e12f;
    for (i = 0; i < count; i++)
    {
        temp_s0 = result[i].setup;
        if ((temp_s0->curveType == 0x22) &&
            (arg1 == temp_s0->unk1A) &&
            ((temp_s0->type22.activeBit == -1) || (mainGetBits(temp_s0->type22.activeBit) != 0)) &&
            ((temp_s0->type22.usedBit == -1) || (mainGetBits(temp_s0->type22.usedBit) == 0)))
        {
            temp_fv0 = vec3DistanceSquared(&obj->globalPosition, &temp_s0->pos);
            if (temp_fv0 < var_fs0)
            {
                var_fs0 = temp_fv0;
                out = temp_s0;
            }
        }
    }
    return out;
}
