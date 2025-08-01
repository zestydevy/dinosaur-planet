#include "dlls/engine/25.h"

#include "libc/string.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "dll.h"

/* 0x0 */ static const char errorMsg[] = "Error in isPointWithinPatchGroup\n";

/* 0x00 */ static u8 _bss_0[0xdc];
/* 0xDC */ static Dll25BssDC *_bss_DC;
/* 0xE0 */ static Dll25BssE0 *_bss_E0;
/* 0xE4 */ static s32 _bss_E4;
/* 0xE8 */ static u8 _bss_E8[0x8];

#define MAX_FLOAT 3.4028235e38f

static s32 dll_25_func_DF8(Dll25BssDC *arg0, f32 arg1, f32 arg2);
static void dll_25_func_D00(Vec3 arg0, Vec3 arg1, s8 *arg2, f32 *arg3);

// offset: 0x0 | ctor
void dll_25_ctor(s32 arg0)
{
    bzero(_bss_0, 0xD9);
    _bss_DC = (void *)malloc(sizeof(Dll25BssDC) * 217, 5, 0);
    _bss_E0 = (void *)malloc(sizeof(Dll25BssE0) * 256, 5, 0);
}

// offset: 0x84 | dtor
void dll_25_dtor(s32 arg0)
{
    free(_bss_DC);
    free(_bss_E0);
}

// offset: 0xD8 | func: 0 | export: 0
void dll_25_func_D8(void)
{
    Dll25BssDC *var_s3;
    u32 *sp274[256] = {0};
    s32 sp270;
    Dll25BssDC *temp_s0;
    Dll25BssDC *temp_s1_2;
    s32 curvesCount; // sp264
    s32 otherCount;
    Dll25BssDC *temp_s2_2;
    Dll25BssE0 *var_v1_3;
    Vec3 sp24C;
    Vec3 sp240;
    CurvesFunc1BCReturn *curvesResult;
    CurvesFunc1BCReturnInner *temp_s4;
    CurvesFunc1BCReturnInner *var_s7;
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
    CurvesFunc1BCReturnInner *temp_s2;
    CurvesFunc1BCReturnInner *temp_v0_2;
    u8 sp80[388]; // unknown size, variable used for padding
    s32 temp_t1;
    s32 var_v1_2;
    f32 sp70[2];

    curvesResult = gDLL_26_Curves->exports->curves_func_1bc(&curvesCount);
    bzero(_bss_0, 0xD9);
    for (sp270 = 0; sp270 < 256; sp270++)
    {
        _bss_E0[sp270].unk1C = 0;
    }
    _bss_E4 = 1;
    for (sp270 = 0; sp270 < curvesCount; sp270++)
    {
        temp_s4 = curvesResult[sp270].unk4;
        if (temp_s4->unk19 == 0x26)
        {
            temp_v0 = temp_s4->unk3;
            temp_s0 = &_bss_DC[temp_v0];
            _bss_0[temp_v0] = 1;
            sp24C[0] = (temp_s4->unk4.bytes.unk4 * 5.7f) + temp_s4->unk8;
            sp24C[2] = (temp_s4->unk4.bytes.unk5 * 5.7f) + temp_s4->unk10;
            sp240[0] = (temp_s4->unk4.bytes.unk6 * 5.7f) + temp_s4->unk8;
            sp240[2] = (temp_s4->unk4.bytes.unk7 * 5.7f) + temp_s4->unk10;
            dll_25_func_D00(sp24C, sp240, &temp_s0->unk0[0], &temp_s0->unk8[0]);
            sp24C[0] = (temp_s4->unk30.bytes.unk30 * 5.7f) + temp_s4->unk8;
            sp24C[2] = (temp_s4->unk30.bytes.unk31 * 5.7f) + temp_s4->unk10;
            dll_25_func_D00(sp240, sp24C, &temp_s0->unk0[2], &temp_s0->unk8[1]);
            sp240[0] = (temp_s4->unk30.bytes.unk32 * 5.7f) + temp_s4->unk8;
            sp240[2] = (temp_s4->unk30.bytes.unk33 * 5.7f) + temp_s4->unk10;
            dll_25_func_D00(sp24C, sp240, &temp_s0->unk0[4], &temp_s0->unk8[2]);
            sp24C[0] = (temp_s4->unk4.bytes.unk4 * 5.7f) + temp_s4->unk8;
            sp24C[2] = (temp_s4->unk4.bytes.unk5 * 5.7f) + temp_s4->unk10;
            dll_25_func_D00(sp240, sp24C, &temp_s0->unk0[6], &temp_s0->unk8[3]);

            temp_s0->unk18 = temp_s4->unkC + (2.0f * temp_s4->unk18);
            temp_s0->unk1A.word = temp_s4->unkC - (2.0f * temp_s4->unk1A.bytes.unk1A);
            for (var_v1_2 = 0; var_v1_2 < 4; var_v1_2++)
            {
                temp_s0->unk1C[var_v1_2] = 0;
                // ugly casting required for matching
                var_s7 = (CurvesFunc1BCReturnInner *)&((s8 *)temp_s4)[var_v1_2 * 4];
                if (var_s7->unk1C[0] >= 0)
                {
                    temp_v0_2 = gDLL_26_Curves->exports->curves_func_39c(var_s7->unk1C[0]);
                    temp_t1 = temp_s4->unk3 * temp_v0_2->unk3;
                    for (var_s0 = 1; var_s0 < _bss_E4; var_s0++)
                    {
                        if (temp_t1 == _bss_E0[var_s0].unk1C)
                        {
                            temp_s0->unk1C[var_v1_2] = var_s0;
                            break;
                        }
                    }
                    if (temp_s0->unk1C[var_v1_2] == 0)
                    {
                        // asm uses s1, might be @fake use of var_s0
                        for (var_s0 = 0; var_s0 < 4; var_s0++)
                        {
                            if (temp_s4->unk14 == temp_v0_2->unk1C[var_s0])
                            {
                                break;
                            }
                        }
                        temp_s0->unk1C[var_v1_2] = _bss_E4;
                        _bss_E0[_bss_E4].unk1C = temp_t1;
                        sp80[_bss_E4 * 2 + 0] = temp_s4->unk3;
                        sp80[_bss_E4 * 2 + 1] = temp_v0_2->unk3;
                        sp24C[0] = (var_s7->unk34 * 5.7f) + temp_s4->unk8;
                        sp24C[2] = (var_s7->unk35 * 5.7f) + temp_s4->unk10;
                        sp240[0] = (var_s7->unk36 * 5.7f) + temp_s4->unk8;
                        sp240[2] = (var_s7->unk37 * 5.7f) + temp_s4->unk10;
                        _bss_E0[_bss_E4].unk1E = (sp24C[0] + sp240[0]) / 2; // use 2 instead of 2.0f to force a div.s
                        _bss_E0[_bss_E4].unk20 = (sp24C[2] + sp240[2]) / 2; // use 2 instead of 2.0f to force a div.s
                        dll_25_func_D00(sp24C, sp240, &_bss_E0[_bss_E4].unk0[0], &_bss_E0[_bss_E4].unk8[0]);
                        sp24C[0] = (((s8 *)temp_v0_2 + var_s0 * 4)[0x34] * 5.7f) + temp_v0_2->unk8;
                        sp24C[2] = (((s8 *)temp_v0_2 + var_s0 * 4)[0x35] * 5.7f) + temp_v0_2->unk10;
                        dll_25_func_D00(sp240, sp24C, &_bss_E0[_bss_E4].unk0[2], &_bss_E0[_bss_E4].unk8[1]);
                        sp240[0] = (((s8 *)temp_v0_2 + var_s0 * 4)[0x36] * 5.7f) + temp_v0_2->unk8;
                        sp240[2] = (((s8 *)temp_v0_2 + var_s0 * 4)[0x37] * 5.7f) + temp_v0_2->unk10;
                        _bss_E0[_bss_E4].unk22 = (sp24C[0] + sp240[0]) / 2; // use 2 instead of 2.0f to force a div.s
                        _bss_E0[_bss_E4].unk24 = (sp24C[2] + sp240[2]) / 2; // use 2 instead of 2.0f to force a div.s
                        dll_25_func_D00(sp24C, sp240, &_bss_E0[_bss_E4].unk0[4], &_bss_E0[_bss_E4].unk8[2]);
                        sp24C[0] = (var_s7->unk34 * 5.7f) + temp_s4->unk8;
                        sp24C[2] = (var_s7->unk35 * 5.7f) + temp_s4->unk10;
                        dll_25_func_D00(sp240, sp24C, &_bss_E0[_bss_E4].unk0[6], &_bss_E0[_bss_E4].unk8[3]);
                        temp_fv0 = temp_s4->unkC + (2.0f * temp_s4->unk18);
                        temp_fv1 = temp_v0_2->unkC + (2.0f * temp_v0_2->unk18);
                        if (temp_fv1 < temp_fv0)
                        {
                            _bss_E0[_bss_E4].unk18 = temp_fv0;
                        }
                        else
                        {
                            _bss_E0[_bss_E4].unk18 = temp_fv1;
                        }
                        temp_fv0 = temp_s4->unkC - (2.0f * temp_s4->unk1A.bytes.unk1A);
                        temp_fv1 = temp_v0_2->unkC - (2.0f * temp_v0_2->unk1A.bytes.unk1A);
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
    for (sp270 = 1; sp270 < _bss_E4; sp270++)
    {
        temp_s1_2 = &_bss_DC[sp80[sp270 * 2]];
        temp_s2_2 = &_bss_DC[sp80[sp270 * 2 + 1]];
        sp70[0] = _bss_E0[sp270].unk22 - _bss_E0[sp270].unk1E;
        sp70[1] = _bss_E0[sp270].unk24 - _bss_E0[sp270].unk20;
        while (
            dll_25_func_DF8(temp_s1_2, _bss_E0[sp270].unk1E, _bss_E0[sp270].unk20) == 0 &&
            dll_25_func_DF8(temp_s2_2, _bss_E0[sp270].unk1E, _bss_E0[sp270].unk20) == 0)
        {
            _bss_E0[sp270].unk1E += sp70[0] / 20.0f;
            _bss_E0[sp270].unk20 += sp70[1] / 20.0f;
        }
        while (
            dll_25_func_DF8(temp_s1_2, _bss_E0[sp270].unk22, _bss_E0[sp270].unk24) == 0 &&
            dll_25_func_DF8(temp_s2_2, _bss_E0[sp270].unk22, _bss_E0[sp270].unk24) == 0)
        {
            _bss_E0[sp270].unk22 -= sp70[0] / 20.0f;
            _bss_E0[sp270].unk24 -= sp70[1] / 20.0f;
        }
    }
}

// offset: 0xD00 | func: 1
static void dll_25_func_D00(Vec3 arg0, Vec3 arg1, s8 *arg2, f32 *arg3)
{
    f32 noramlized;
    f32 z;
    f32 x;

    z = arg1[2] - arg0[2];
    x = arg0[0] - arg1[0];
    noramlized = sqrtf((z * z) + (x * x));
    if (noramlized != 0.0f)
    {
        z /= noramlized;
        x /= noramlized;
    }
    arg2[0] = z * 127.0f;
    arg2[1] = x * 127.0f;
    arg3[0] = -((arg2[0] * arg0[0]) + (arg2[1] * arg0[2]));
}

// offset: 0xDF8 | func: 2
static s32 dll_25_func_DF8(Dll25BssDC *arg0, f32 arg1, f32 arg2)
{
    u8 var_v0;
    u8 var_v1;
    s8 *temp1;

    for (var_v0 = 0, var_v1 = 0; var_v0 < 4; var_v0++, var_v1 += 2)
    {
        temp1 = &((s8 *)arg0)[var_v1];
        if (((temp1[0] * arg1) + (((s8 *)arg0)[var_v1 + 1] * arg2) + ((f32 *)arg0)[(u32)var_v0 + 2]) > 0.0f)
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

/*0x400*/ static u32 _data_400[] = {0, 0, 0, 0};
// offset: 0xEAC | func: 3
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/25/dll_25_func_EAC.s")
#else
static s32 dll_25_func_EAC(f32 *arg0)
{
    s16 var_s0;
    s16 var_s1;
    u32 v1;
    Dll25BssDC *temp_a0;

    v1 = _data_400[0];
    var_s1 = v1;
    if (_data_400[0] == 0xD8)
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
            if ((arg0[1] < temp_a0->unk18) && (temp_a0->unk1A.bytes.unk1A < arg0[1]) && (dll_25_func_DF8(temp_a0, arg0[0], arg0[2]) != 0))
            {
                _data_400[0] = var_s1;
                return var_s1;
            }
        }
        if (_bss_0[var_s0] != 0)
        {
            temp_a0 = &_bss_DC[var_s0];
            if ((arg0[1] < temp_a0->unk18) && (temp_a0->unk1A.bytes.unk1A < arg0[1]) && (dll_25_func_DF8(temp_a0, arg0[0], arg0[2]) != 0))
            {
                _data_400[0] = var_s0;
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
        if ((arg0[1] < (f32)temp_a0->unk18) && (temp_a0->unk1A.bytes.unk1A < arg0[1]) && (dll_25_func_DF8(temp_a0, arg0[0], arg0[2]) != 0))
        {
            _data_400[0] = var_s1;
            return var_s1;
        }
    }
    return 0;
}
#endif

// offset: 0x1158 | func: 4 | export: 1
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/25/dll_25_func_1158.s")
#else
// Matches on dll_25_func_EAC is static
s32 dll_25_func_1158(f32 *arg0, Dll25BssDC *arg1)
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
                arg1->unk0[var_a3 + 2] = temp_v1->unk1C;
                for (var_v0 = 0, var_a0 = 0; var_v0 < 4; var_v0++, var_a0 += 2)
                {
                    if (((temp_v1->unk0[var_a0] * arg0[0]) + (temp_v1->unk0[var_a0 + 1] * arg0[2]) + ((f32 *)temp_v1)[(u32)var_v0 + 2]) > 0.0f)
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
                arg1->unk0[var_a3 + 2] = 0;
            }
        }
    }
    return temp_v0;
}
#endif

// offset: 0x12FC | func: 5 | export: 2
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/25/dll_25_func_12FC.s")
#else
// Matching if dll_25_func_EAC is static
s32 dll_25_func_12FC(Vec3 arg0)
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
        if (arg0[1] < temp_a1->unk18)
        {
            if (temp_a1->unk1A.bytes.unk1A < arg0[1])
            {
                for (var_a0 = 0, var_v1 = 0; var_a0 < 4;)
                {
                    temp_v0 = &temp_a1->unk0[var_v1];
                    if (((((temp_v0[0] * arg0[0]) + (temp_v0[1] * arg0[2])) + ((f32 *)temp_a1)[var_a0 + 2]) > 0.0f))
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
#endif

// offset: 0x1474 | func: 6 | export: 3
s32 dll_25_func_1474(Vec3 arg0, s32 arg1)
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
                    if ((((temp_a2->unk0[var_t0] * arg0[0]) + (temp_a2->unk0[var_t0 + 1] * arg0[2])) + ((f32 *)temp_a2)[(u32)var_a3 + 2]) > 0.0f)
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
s32 dll_25_func_15B0(Vec3 arg0, s32 arg1, s32 arg2)
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
                    if (((temp[0] * arg0[0]) + (temp_a0->unk0[var_v1 + 1] * arg0[2]) + ((f32 *)temp_a0)[(u32)var_v0_2 + 2]) > 0.0f)
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

    for (i = 0; i < 0x100; i++)
    {
        if (arg2 == _bss_E0[i].unk1C)
        {
            break;
        }
    }
    arg1->x = _bss_E0[i].unk1E;
    arg1->y = arg0->y;
    arg1->z = _bss_E0[i].unk20;
    tempDistance = vec3_distance_squared(arg0, arg1);
    arg1->x = _bss_E0[i].unk22;
    arg1->z = _bss_E0[i].unk24;
    if (vec3_distance_squared(arg0, arg1) < tempDistance)
    {
        return 1;
    }

    arg1->x = _bss_E0[i].unk1E;
    arg1->z = _bss_E0[i].unk20;
    return 1;
}

// offset: 0x1864 | func: 9 | export: 5
CurvesFunc1BCReturnInner *dll_25_func_1864(Vec3 arg0, s32 arg1, s32 arg2)
{
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fs0;
    s32 i;
    s32 count;
    CurvesFunc1BCReturnInner *temp_s0;
    CurvesFunc1BCReturn *result;
    CurvesFunc1BCReturnInner *out;

    result = gDLL_26_Curves->exports->curves_func_1bc(&count);
    out = NULL;
    var_fs0 = MAX_FLOAT;
    for (i = 0; i < count; i++)
    {
        temp_s0 = result[i].unk4;
        if ((temp_s0 != NULL) && (temp_s0->unk19 == 0x24) && ((arg1 == -1) || (arg1 == temp_s0->unk3)) && ((arg2 == -1) || (arg2 == temp_s0->unk1A.bytes.unk1A)) && ((temp_s0->unk1C[0] != -1) || (temp_s0->unk2F == -1)) && ((temp_s0->unk30.words.unk30 == -1) || (get_gplay_bitstring(temp_s0->unk30.words.unk30) != 0)) && ((temp_s0->unk30.words.unk32 == -1) || (get_gplay_bitstring(temp_s0->unk30.words.unk32) == 0)))
        {
            temp_fv0 = arg0[0] - temp_s0->unk8;
            temp_fv1 = arg0[1] - temp_s0->unkC;
            temp_fa0 = arg0[2] - temp_s0->unk10;
            temp_fa1 = (temp_fv0 * temp_fv0) + (temp_fv1 * temp_fv1) + (temp_fa0 * temp_fa0);
            if (temp_fa1 < var_fs0)
            {
                var_fs0 = temp_fa1;
                out = temp_s0;
            }
        }
    }
    return out;
}

// offset: 0x1A2C | func: 10 | export: 6
CurvesFunc1BCReturnInner *dll_25_func_1A2C(Vec3 arg0, s32 arg1, s32 arg2)
{
    CurvesFunc1BCReturn *result;
    CurvesFunc1BCReturnInner *temp_v1;
    CurvesFunc1BCReturnInner *out;
    f32 temp_fa1;
    f32 temp_ft4;
    f32 temp_ft5;
    s32 count;
    f32 var_fs0;
    s32 var_t0;
    s32 var_v1;

    result = gDLL_26_Curves->exports->curves_func_1bc(&count);
    out = NULL;
    var_v1 = 0;
    var_fs0 = MAX_FLOAT;
    for (var_v1 = 0; var_v1 < count; var_v1++)
    {
        temp_v1 = result[var_v1].unk4;
        if ((temp_v1 != NULL) &&
            (temp_v1->unk19 == 0x24) &&
            ((arg1 == -1) || (arg1 == temp_v1->unk3)) &&
            ((arg2 == -1) || (arg2 == temp_v1->unk1A.bytes.unk1A)) &&
            ((temp_v1->unk1C[0] != -1) || (temp_v1->unk2F == -1)))
        {
            temp_fa1 = arg0[0] - temp_v1->unk8;
            temp_ft4 = arg0[1] - temp_v1->unkC;
            temp_ft5 = arg0[2] - temp_v1->unk10;
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
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/25/dll_25_func_1D30.s")
#else
s32 dll_25_func_1D30(UnkCurvesStruct *arg0, CurvesFunc1BCReturnInner *arg1, CurvesFunc1BCReturnInner *arg2, UnkInnerCurvesStruct *arg3)
{
    if (arg0->unk0x80 != 0)
    {
        arg0->unk0xA0 = arg1;
        arg0->unk0xA4 = arg2;
        arg0->unk0xA8[0] = arg2->unk8;
        arg0->unk0xA8[1] = arg1->unk8;
        arg0->unk0xA8[2] = 2.0f * (fsin16_precise(arg2->unk2C << 8) * arg0->unk0xA4->unk2E);
        arg0->unk0xA8[3] = 2.0f * (fsin16_precise(arg0->unk0xA0->unk2C << 8) * arg0->unk0xA0->unk2E);
        arg0->unk0xC8[0] = arg0->unk0xA4->unkC;
        arg0->unk0xC8[1] = arg0->unk0xA0->unkC;
        arg0->unk0xC8[2] = 2.0f * (fsin16_precise(arg0->unk0xA4->unk2D << 8) * arg0->unk0xA4->unk2E);
        arg0->unk0xC8[3] = 2.0f * (fsin16_precise(arg0->unk0xA0->unk2D << 8) * arg0->unk0xA0->unk2E);
        arg0->unk0xE8[0] = arg0->unk0xA4->unk10;
        arg0->unk0xE8[1] = arg0->unk0xA0->unk10;
        arg0->unk0xE8[2] = 2.0f * (fcos16_precise(arg0->unk0xA4->unk2C << 8) * arg0->unk0xA4->unk2E);
        arg0->unk0xE8[3] = 2.0f * (fcos16_precise(arg0->unk0xA0->unk2C << 8) * arg0->unk0xA0->unk2E);
    }
    else
    {
        arg0->unk0xA0 = arg1;
        arg0->unk0xA4 = arg2;
        arg0->unk0xB8[0] = arg1->unk8;
        arg0->unk0xB8[1] = arg2->unk8;
        arg0->unk0xB8[2] = 2.0f * (fsin16_precise(arg1->unk2C << 8) * arg0->unk0xA0->unk2E);
        arg0->unk0xB8[3] = 2.0f * (fsin16_precise(arg0->unk0xA4->unk2C << 8) * arg0->unk0xA4->unk2E);
        arg0->unk0xD8[0] = arg0->unk0xA0->unkC;
        arg0->unk0xD8[1] = arg0->unk0xA4->unkC;
        arg0->unk0xD8[2] = 2.0f * (fsin16_precise(arg0->unk0xA0->unk2D << 8) * arg0->unk0xA0->unk2E);
        arg0->unk0xD8[3] = 2.0f * (fsin16_precise(arg0->unk0xA4->unk2D << 8) * arg0->unk0xA4->unk2E);
        arg0->unk0xF8[0] = arg0->unk0xA0->unk10;
        arg0->unk0xF8[1] = arg0->unk0xA4->unk10;
        arg0->unk0xF8[2] = 2.0f * (fcos16_precise(arg0->unk0xA0->unk2C << 8) * arg0->unk0xA0->unk2E);
        arg0->unk0xF8[3] = 2.0f * (fcos16_precise(arg0->unk0xA4->unk2C << 8) * arg0->unk0xA4->unk2E);
    }
    if (gDLL_25->exports->dll_25_func_21F4(arg0, arg3) != 0)
    {
        return 1;
    }
    arg0->unk0x84 = arg0->unk0xA8;
    arg0->unk0x88 = arg0->unk0xC8;
    arg0->unk0x8C = arg0->unk0xE8;
    arg0->unk0x90 = 8;
    arg0->unk0x94 = func_80004C5C;
    arg0->unk0x98 = func_80004CE8;
    func_80005094(arg0);
    return 0;
}
#endif

// offset: 0x21F4 | func: 12 | export: 9
s32 dll_25_func_21F4(UnkCurvesStruct *arg0, UnkInnerCurvesStruct *arg1)
{
    UnkInnerCurvesStruct *temp_v0;
    UnkInnerCurvesStruct *temp_v1;

    if ((arg0->unk0xA0 == NULL) || (arg0->unk0xA4 == NULL) || (arg1 == NULL))
    {
        return 1;
    }

    if (arg0->unk0x80 != 0)
    {
        arg0->pad0x9C = arg0->unk0xA0;
        arg0->unk0xA0 = arg0->unk0xA4;
        arg0->unk0xA4 = arg1;
        memcpy(arg0->unk0xB8, arg0->unk0xA8, 0x10);
        memcpy(arg0->unk0xD8, arg0->unk0xC8, 0x10);
        memcpy(arg0->unk0xF8, arg0->unk0xE8, 0x10);
        arg0->unk0xA8[0] = arg0->unk0xA4->unk8;
        arg0->unk0xA8[1] = arg0->unk0xA0->unk8;
        arg0->unk0xA8[2] = 2.0f * (fsin16_precise(arg0->unk0xA4->unk2C << 8) * arg0->unk0xA4->unk2E);
        arg0->unk0xA8[3] = 2.0f * (fsin16_precise(arg0->unk0xA0->unk2C << 8) * arg0->unk0xA0->unk2E);
        arg0->unk0xC8[0] = arg0->unk0xA4->unkC;
        arg0->unk0xC8[1] = arg0->unk0xA0->unkC;
        arg0->unk0xC8[2] = 2.0f * (fsin16_precise(arg0->unk0xA4->unk2D << 8) * arg0->unk0xA4->unk2E);
        arg0->unk0xC8[3] = 2.0f * (fsin16_precise(arg0->unk0xA0->unk2D << 8) * arg0->unk0xA0->unk2E);
        arg0->unk0xE8[0] = arg0->unk0xA4->unk10;
        arg0->unk0xE8[1] = arg0->unk0xA0->unk10;
        arg0->unk0xE8[2] = 2.0f * (fcos16_precise(arg0->unk0xA4->unk2C << 8) * arg0->unk0xA4->unk2E);
        arg0->unk0xE8[3] = 2.0f * (fcos16_precise(arg0->unk0xA0->unk2C << 8) * arg0->unk0xA0->unk2E);
        if (arg0->unk0x90 != 0)
        {
            func_8000523C(arg0);
            if (arg0->unk0x0 <= 0.0f)
            {
                arg0->unk0x0 = 0.01f;
            }
        }
    }
    else
    {
        arg0->pad0x9C = arg0->unk0xA0;
        arg0->unk0xA0 = arg0->unk0xA4;
        arg0->unk0xA4 = arg1;
        memcpy(arg0->unk0xA8, arg0->unk0xB8, 0x10U);
        memcpy(arg0->unk0xC8, arg0->unk0xD8, 0x10U);
        memcpy(arg0->unk0xE8, arg0->unk0xF8, 0x10U);
        arg0->unk0xB8[0] = arg0->unk0xA0->unk8;
        arg0->unk0xB8[1] = arg0->unk0xA4->unk8;
        arg0->unk0xB8[2] = 2.0f * (fsin16_precise(arg0->unk0xA0->unk2C << 8) * arg0->unk0xA0->unk2E);
        arg0->unk0xB8[3] = 2.0f * (fsin16_precise(arg0->unk0xA4->unk2C << 8) * arg0->unk0xA4->unk2E);
        arg0->unk0xD8[0] = arg0->unk0xA0->unkC;
        arg0->unk0xD8[1] = arg0->unk0xA4->unkC;
        arg0->unk0xD8[2] = 2.0f * (fsin16_precise(arg0->unk0xA0->unk2D << 8) * arg0->unk0xA0->unk2E);
        arg0->unk0xD8[3] = 2.0f * (fsin16_precise(arg0->unk0xA4->unk2D << 8) * arg0->unk0xA4->unk2E);
        arg0->unk0xF8[0] = arg0->unk0xA0->unk10;
        arg0->unk0xF8[1] = arg0->unk0xA4->unk10;
        arg0->unk0xF8[2] = 2.0f * (fcos16_precise(arg0->unk0xA0->unk2C << 8) * arg0->unk0xA0->unk2E);
        arg0->unk0xF8[3] = 2.0f * (fcos16_precise(arg0->unk0xA4->unk2C << 8) * arg0->unk0xA4->unk2E);
        if (arg0->unk0x90 != 0)
        {
            func_8000523C(arg0);
            if (arg0->unk0x0 >= 1.0f)
            {
                arg0->unk0x0 = 0.99f;
            }
        }
    }
    return 0;
}

// offset: 0x27A8 | func: 13 | export: 10
s32 dll_25_func_27A8(UnkCurvesStruct *arg0, UnkInnerCurvesStruct *arg1)
{
    if ((arg0->unk0xA0 == 0) || (arg0->unk0xA4 == NULL) || (arg1 == NULL))
    {
        return 1;
    }
    arg0->unk0xA4 = arg1;
    if (arg0->unk0x80 != 0)
    {
        arg0->unk0xA8[0] = arg1->unk8;
        arg0->unk0xA8[2] = 2.0f * (fsin16_precise(arg1->unk2C << 8) * arg1->unk2E);
        arg0->unk0xC8[0] = arg1->unkC;
        arg0->unk0xC8[2] = 2.0f * (fsin16_precise(arg1->unk2D << 8) * arg1->unk2E);
        arg0->unk0xE8[0] = arg1->unk10;
        arg0->unk0xE8[2] = 2.0f * (fcos16_precise(arg1->unk2C << 8) * arg1->unk2E);
    }
    else
    {
        arg0->unk0xB8[1] = arg1->unk8;
        arg0->unk0xB8[3] = 2.0f * (fsin16_precise(arg1->unk2C << 8) * arg1->unk2E);
        arg0->unk0xD8[1] = arg1->unkC;
        arg0->unk0xD8[3] = 2.0f * (fsin16_precise(arg1->unk2D << 8) * arg1->unk2E);
        arg0->unk0xF8[1] = arg1->unk10;
        arg0->unk0xF8[3] = 2.0f * (fcos16_precise(arg1->unk2C << 8) * arg1->unk2E);
    }
    return 0;
}

// offset: 0x29FC | func: 14 | export: 11
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/25/dll_25_func_29FC.s")
#else
void dll_25_func_29FC(UnkCurvesStruct *arg0)
{
    s32 temp_t9;

    arg0->pad0x9C ^= (s32)arg0->unk0xA4;
    arg0->pad0x9C ^= (temp_t9 = (s32)arg0->unk0xA4 ^ arg0->pad0x9C);
    arg0->unk0xA4 = (CurvesFunc1BCReturnInner *)temp_t9;
    if (arg0->unk0x0 >= 1.0f)
    {
        arg0->unk0x0 = 0.99f;
    }
}
#endif

// offset: 0x2A50 | func: 15 | export: 12
CurvesFunc1BCReturnInner *dll_25_func_2A50(s32 arg0, s32 arg1)
{
    CurvesFunc1BCReturn *result;
    CurvesFunc1BCReturnInner *temp_s0;
    CurvesFunc1BCReturnInner *out;
    s32 count;
    f32 temp_fv0;
    f32 var_fs0;
    s32 i;

    result = gDLL_26_Curves->exports->curves_func_1bc(&count);
    out = NULL;
    var_fs0 = 1e12f;
    for (i = 0; i < count; i++)
    {
        temp_s0 = result[i].unk4;
        if ((temp_s0->unk19 == 0x22) &&
            ((arg1 == temp_s0->unk4.words.unk4) || ((temp_s0->unk1A.bytes.unk1A < 3) &&
                                                    (arg1 == -1))) &&
            ((temp_s0->unk30.words.unk30 == -1) || (get_gplay_bitstring(temp_s0->unk30.words.unk30) != 0)) &&
            ((temp_s0->unk30.words.unk32 == -1) || (get_gplay_bitstring(temp_s0->unk30.words.unk32) == 0)))
        {
            temp_fv0 = vec3_distance_squared(arg0 + 0x18, (Vec3f *)&temp_s0->unk8);
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
CurvesFunc1BCReturnInner *dll_25_func_2BC4(s32 arg0, s32 arg1)
{
    CurvesFunc1BCReturn *result;
    CurvesFunc1BCReturnInner *temp_s0;
    CurvesFunc1BCReturnInner *out;
    s32 count;
    f32 temp_fv0;
    f32 var_fs0;
    s32 i;

    result = gDLL_26_Curves->exports->curves_func_1bc(&count);
    out = NULL;
    i = 0;
    var_fs0 = 1e12f;
    for (i = 0; i < count; i++)
    {
        temp_s0 = result[i].unk4;
        if (
            (temp_s0->unk19 == 0x22) &&
            ((arg1 == temp_s0->unk4.words.unk4) || ((temp_s0->unk1A.bytes.unk1A < 3) && (arg1 == -1))))
        {
            temp_fv0 = vec3_distance_squared(arg0 + 0x18, (Vec3f *)&temp_s0->unk8);
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
CurvesFunc1BCReturnInner *dll_25_func_2CF8(s32 arg0, s32 arg1)
{
    CurvesFunc1BCReturn *result;
    CurvesFunc1BCReturn *var_s2;
    CurvesFunc1BCReturnInner *temp_s0;
    s32 count;
    CurvesFunc1BCReturnInner *out;
    f32 temp_fv0;
    f32 var_fs0;
    s32 i;

    result = gDLL_26_Curves->exports->curves_func_1bc(&count);
    out = NULL;
    var_fs0 = 1e12f;
    for (i = 0; i < count; i++)
    {
        temp_s0 = result[i].unk4;
        if ((temp_s0->unk19 == 0x22) &&
            (arg1 == temp_s0->unk1A.bytes.unk1A) &&
            ((temp_s0->unk30.words.unk30 == -1) || (get_gplay_bitstring(temp_s0->unk30.words.unk30) != 0)) &&
            ((temp_s0->unk30.words.unk32 == -1) || (get_gplay_bitstring(temp_s0->unk30.words.unk32) == 0)))
        {
            temp_fv0 = vec3_distance_squared(arg0 + 0x18, (Vec3f *)&temp_s0->unk8);
            if (temp_fv0 < var_fs0)
            {
                var_fs0 = temp_fv0;
                out = temp_s0;
            }
        }
    }
    return out;
}
