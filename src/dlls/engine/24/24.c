#include "dlls/engine/24.h"

/*0x0*/ static const u32 _rodata_0[] = {
    0x436f756c,
    0x64206e6f,
    0x7420616c,
    0x6c6f6361,
    0x7465206d,
    0x656d6f72,
    0x7920666f,
    0x72207761,
    0x74657266,
    0x7820646c,
    0x6c0a0000};

/*0x00*/ static s32 _bss_0;
/*0x04*/ static s32 *_bss_4;
/*0x08*/ static StructBss8 *_bss_8;
/*0x0C*/ static s32 _bss_C;
/*0x10*/ static StructBss10 *_bss_10;
/*0x14*/ static s32 _bss_14;
/*0x18*/ static s32 _bss_18;
/*0x1C*/ static s32 _bss_1C;
/*0x20*/ static s32 _bss_20; // some sort of counter
/*0x24*/ static StructBss24 *_bss_24;
/*0x28*/ static s32 _bss_28;
/*0x2C*/ static StructBss2C *_bss_2C;
/*0x30*/ static s32 _bss_30;
/*0x34*/ static StructBss34 *_bss_34;
/*0x38*/ static s32 _bss_38; // some sort of counter
/*0x3C*/ static StructBss3C *_bss_3C;
/*0x40*/ static Texture *_bss_40;
/*0x44*/ static Texture *_bss_44;
/*0x48*/ static Texture *_bss_48;
/*0x4C*/ static Texture *_bss_4C;
/*0x50*/ static s32 _bss_50[4];

// offset: 0x0 | ctor
void dll_24_ctor(s32 arg0)
{
    s32 *state;

    state = malloc(0x3E80, 0x13, NULL);
    if (state != NULL)
    {
        _bss_4 = state;
        _bss_C = state + (0x3C0 / 4);
        _bss_14 = state + (0xB40 / 4);
        _bss_1C = state + (0xF00 / 4);
        _bss_0 = state + (0x12C0 / 4);
        _bss_8 = state + (0x1A40 / 4);
        _bss_10 = state + (0x2300 / 4);
        _bss_18 = state + (0x2A80 / 4);
        state += (0x3200 / 4);
        _bss_24 = state;
        state += (0x348 / 4);
        _bss_2C = state;
        state += (0x398 / 4);
        _bss_3C = state;
        state += (0x258 / 4);
        _bss_34 = state;
        _bss_20 = 0;
        _bss_28 = 0;
        _bss_38 = 0;
        _bss_30 = 0;
        _bss_40 = queue_load_texture_proxy(0x56);
        _bss_44 = queue_load_texture_proxy(0x59);
        _bss_48 = queue_load_texture_proxy(0x22);
        _bss_4C = queue_load_texture_proxy(0x57);
        dll_24_func_24C();
    }
}

// offset: 0x160 | dtor
void dll_24_dtor(s32 arg0)
{
    if (_bss_4 != NULL)
    {
        free(_bss_4);
    }
    if (_bss_40 != NULL)
    {
        texture_destroy(_bss_40);
        _bss_40 = NULL;
    }
    if (_bss_44 != NULL)
    {
        texture_destroy(_bss_44);
        _bss_44 = NULL;
    }
    if (_bss_48 != NULL)
    {
        texture_destroy(_bss_48);
        _bss_48 = NULL;
    }
    if (_bss_4C != NULL)
    {
        texture_destroy(_bss_4C);
        _bss_4C = NULL;
    }
}

// offset: 0x24C | func: 0 | export: 6
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_24C.s")
#else
void dll_24_func_24C(void)
{
    s32 *temp_a3_2;
    StructBss4 *var_v0;
    s32 *var_v0_3;
    s32 *var_v0_4;
    s32 var_a3;
    s32 var_a3_2;
    s32 var_t0;
    s32 var_t0_2;
    s32 var_t3;
    s32 var_t4;
    s8 temp_a3;
    s8 temp_t1;
    s8 var_a2;
    StructBss24 *temp_a2;
    void *temp_a3_3;
    void *temp_a3_4;
    void *temp_a3_5;
    void *temp_a3_6;
    void *temp_a3_7;
    void *temp_t2;
    StructBss2C *temp_v0;
    StructBssC *var_v0_2;
    s32 i;
    s32 j;

    for (i = 0; i < 28; i++)
    {
        var_v0 = &_bss_4[i];
        var_v0->unk1 = 3;
        var_v0->unk2 = 1;
        var_v0->unk3 = 0;
        var_v0->unk11 = 3;
        var_v0->unk12 = 2;
        var_v0->unk13 = 1;
        temp_a2 = &_bss24[i];
        temp_a2->unk0 = 0.0f;
        temp_a2->unk4 = 0.0f;
        temp_a2->unk8 = 0.0f;
        temp_a2->unkC = 0.0f;
        temp_a2->unk10 = 0.01f;
        temp_a2->unk16 = 0;
    }
    for (i = 0; i < 10; i++)
    {
        var_a2 = 0;
        for (j = 0; j < 10; j += 2)
        { // target unknown, probably sizeof _bss_C
            temp_a3 = var_a2 + 1;
            temp_t1 = var_a2 + 2;
            var_v0_2 = &_bss_C[j];
            var_v0_2->unk1 = var_a2;
            var_v0_2->unk2 = temp_t1;
            var_v0_2->unk3 = temp_a3;

            var_v0_2 = &_bss_C[j + 1];
            var_v0_2->unk3 = var_a2 + 3;
            var_v0_2->unk2 = temp_t1;
            var_v0_2->unk1 = temp_a3;

            var_a2 = temp_t1;
        }

        temp_v0 = &_bss_2C[i];
        temp_v0->unk0 = 0.0f;
        temp_v0->unk4 = 0.0f;
        temp_v0->unk8 = 0.0f;
        temp_v0->unk54 = 0;
        temp_v0->unk58 = 0;
    }

    for (i = 0; i < 30; i++)
    {
        var_v0 = &_bss_1C[i];
        var_v0->unk1 = 3;
        var_v0->unk2 = 1;
        var_v0->unk3 = 0;
        var_v0->unk11 = 3;
        var_v0->unk12 = 2;
        var_v0->unk13 = 1;
        temp_a2 = &_bss_34[i];
        temp_a2->unk0 = 0.0f;
        temp_a2->unk4 = 0.0f;
        temp_a2->unk8 = 0.0f;
        temp_a2->unkC = 0.0f;
        temp_a2->unk10 = 0.01f;
        temp_a2->unk14 = 0;
        temp_a2->unk16 = 0;
    }
    /*
    var_t0_2 = 0x28;
    _bss_14->unk1 = 0;
    _bss_14->unk2 = 1;
    _bss_14->unk3 = 3;
    _bss_14->unk11 = 0;
    _bss_14->unk12 = 3;
    _bss_14->unk13 = 2;
    var_v0_4 = _bss_14 + 0x40;
    _bss_3C->unk12 = -1;
    _bss_3C->unk0 = 0;
    _bss_3C->unk4 = 0.0f;
    _bss_3C->unk10 = 0;
    _bss_3C->unk8 = 0.0f;
    _bss_3C->unkC = 0.0f;
    var_v0_4->unk-D = 2;
    var_v0_4->unk-E = 3;
    var_v0_4->unk-F = 0;
    var_v0_4->unk-1D = 3;
    var_v0_4->unk-1E = 1;
    var_v0_4->unk-1F = 0;
    temp_a3_2 = _bss_3C + 0x14;
    temp_a3_2->unkC = 0.0f;
    temp_a3_2->unk8 = 0.0f;
    temp_a3_2->unk10 = 0;
    temp_a3_2->unk4 = 0.0f;
    temp_a3_2->unk0 = 0;
    temp_a3_2->unk12 = -1;
    do {
        var_v0_4->unk1 = 0;
        var_v0_4->unk2 = 1;
        var_v0_4->unk3 = 3;
        var_v0_4->unk11 = 0;
        var_v0_4->unk12 = 3;
        var_v0_4->unk13 = 2;
        var_v0_4 += 0x80;
        temp_a3_3 = var_t0_2 + _bss_3C;
        temp_a3_3->unk12 = -1;
        temp_a3_3->unk0 = 0.0f;
        temp_a3_3->unk4 = 0.0f;
        temp_a3_3->unk10 = 0;
        temp_a3_3->unk8 = 0.0f;
        temp_a3_3->unkC = 0.0f;
        var_v0_4->unk-4D = 2;
        var_v0_4->unk-4E = 3;
        var_v0_4->unk-4F = 0;
        var_v0_4->unk-5D = 3;
        var_v0_4->unk-5E = 1;
        var_v0_4->unk-5F = 0;
        temp_a3_4 = var_t0_2 + _bss_3C;
        temp_a3_4->unk20 = 0.0f;
        temp_a3_4->unk1C = 0.0f;
        temp_a3_4->unk24 = 0;
        temp_a3_4->unk18 = 0.0f;
        temp_a3_4->unk14 = 0.0f;
        temp_a3_4->unk26 = -1;
        var_v0_4->unk-2D = 2;
        var_v0_4->unk-2E = 3;
        var_v0_4->unk-2F = 0;
        var_v0_4->unk-3D = 3;
        var_v0_4->unk-3E = 1;
        var_v0_4->unk-3F = 0;
        temp_a3_5 = var_t0_2 + _bss_3C;
        temp_a3_5->unk34 = 0.0f;
        temp_a3_5->unk30 = 0.0f;
        temp_a3_5->unk38 = 0;
        temp_a3_5->unk2C = 0.0f;
        temp_a3_5->unk28 = 0.0f;
        temp_a3_5->unk3A = -1;
        var_v0_4->unk-D = 2;
        var_v0_4->unk-E = 3;
        var_v0_4->unk-F = 0;
        var_v0_4->unk-1D = 3;
        var_v0_4->unk-1E = 1;
        var_v0_4->unk-1F = 0;
        temp_a3_6 = var_t0_2 + _bss_3C;
        var_t0_2 += 0x50;
        temp_a3_7 = temp_a3_6 + 0x3C;
        temp_a3_7->unkC = 0.0f;
        temp_a3_7->unk8 = 0.0f;
        temp_a3_7->unk10 = 0;
        temp_a3_7->unk4 = 0.0f;
        temp_a3_7->unk0 = 0.0f;
        temp_a3_7->unk12 = -1;
    } while (var_t0_2 != 0x258);
     */
}
#endif

// offset: 0x564 | func: 1 | export: 1
void dll_24_func_564(Func564Arg0 *arg0, u16 arg1, Vec3f *arg2, Func564Arg3 *arg3, f32 arg4)
{
    f32 temp_fs1;
    f32 temp_fs2;
    s32 i;

    if (arg1 != 0)
    {
        i = 0;
        do
        {
            if (arg1 & 1)
            {
                temp_fs1 = arg2[i].x;
                temp_fs2 = arg2[i].z;
                if (arg3->unk1B0 < 10.0f)
                {
                    dll_24_func_174C(temp_fs1, arg0->unk10 + arg3->unk1B0, temp_fs2, 0.0f);
                }
                dll_24_func_1CC8(temp_fs1, arg0->unk10 + arg3->unk1B0, temp_fs2, arg0->unk0, 0.0f, 3);
            }
            arg1 >>= 1;
            i += 1;
        } while (arg1);
    }
    if (arg4 > 0.01f)
    {
        dll_24_func_1B28(arg0->unkC, arg0->unk10 + arg3->unk1B0, arg0->unk14, arg0->unk0, 0.0f);
    }
}

// offset: 0x6E8 | func: 2 | export: 0
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_6E8.s")
#else
void dll_24_func_6E8(void)
{
    s32 i; // sp84
    f32 temp_fv1;
    f32 var_fv0;
    s32 temp_ft1;
    s32 temp_ft4;
    s32 temp_ft4_2;
    s32 j;
    StructBss8 *temp_a1;
    StructBss10 *temp_a1_2;
    StructBss2C *temp_t0;
    StructBss24 *temp_v0;
    StructBss34 *temp_v0_2;
    StructBss3C *temp_v0_3;
    StructBss8 *var_v0;

    for (i = 0; i < 30; i++)
    {
        temp_v0 = &_bss_24[i];
        if (temp_v0->unk16 != 0)
        {
            temp_v0->unk10 += 0.0007f * delayFloat;
            temp_v0->unk16 -= (delayByte * temp_v0->unk18);
            if (temp_v0->unk16 < 0)
            {
                temp_v0->unk16 = 0;
                _bss_20 -= 1;
            }
        }
    }

    for (i = 0; i < 10; i++)
    {
        temp_t0 = &_bss_2C[i];
        if (temp_t0->unk54 != 0)
        {
            temp_a1 = &_bss_8[i * 0xE];
            var_fv0 = temp_a1[1].unk2 / 400.0f;
            if (var_fv0 < 0.0f)
            {
                var_fv0 = 0.0f;
            }
            else if (var_fv0 > 1.0f)
            {
                var_fv0 = 1.0f;
            }
            temp_ft1 = (s16)(255.0f - (var_fv0 * 255.0f));
            for (j = 0; j < 0xE; j++)
            {
                if (j & 1)
                {
                    temp_a1[j].unk0 += (s16)(temp_t0[j % 12].unkC * delayFloat * 100.0f);
                    temp_a1[j].unk2 += (s16)(temp_t0[j % 12].unk24 * delayFloat * 100.0f);
                    temp_a1[j].unk4 += (s16)(temp_t0[j % 12].unk3C * delayFloat * 100.0f);
                }
                temp_a1[j].unkF = temp_ft1;
            }
            if (temp_a1[1].unk2 >= 0x191)
            {
                temp_t0->unk54 = 0;
                _bss_28 -= 1;
            }
        }
    }

    for (i = 0; i < 30; i++)
    {
        temp_v0_2 = &_bss_34[i];
        if (temp_v0_2->unk14 != 0)
        {
            temp_v0_2->unk10 += 0.004f * delayFloat;
            temp_v0_2->unk14 -= delayByte * 5;
            if (temp_v0_2->unk14 < 0)
            {
                temp_v0_2->unk14 = 0;
                _bss_30 -= 1;
            }
            if ((u8)temp_v0_2->unk18 == 0)
            {
                temp_v0_2->unk18 = 1;
            }
        }
    }

    for (i = 0; i < 0x1E; i++)
    {
        temp_v0_3 = &_bss_3C[i];
        if (temp_v0_3->unk12 != -1)
        {
            temp_t0 = &_bss_2C[temp_v0_3->unk12];
            var_fv0 = 100.0f * delayFloat;
            temp_fv1 = temp_v0_3->unk8 * var_fv0;
            temp_ft4 = (temp_v0_3->unk0 * temp_fv1);
            temp_ft1 = (temp_v0_3->unkC * var_fv0);
            temp_ft4_2 = (temp_v0_3->unk4 * temp_fv1);

            temp_a1_2 = &_bss_10[i * 4];
            temp_a1_2->unk0 += temp_ft4;
            temp_a1_2->unk2 += temp_ft1;
            temp_a1_2->unk4 += temp_ft4_2;
            temp_a1_2++;
            temp_a1_2->unk0 += temp_ft4;
            temp_a1_2->unk2 += temp_ft1;
            temp_a1_2->unk4 += temp_ft4_2;
            temp_a1_2++;
            temp_a1_2->unk0 += temp_ft4;
            temp_a1_2->unk2 += temp_ft1;
            temp_a1_2->unk4 += temp_ft4_2;
            temp_a1_2++;
            temp_a1_2->unk0 += temp_ft4;
            temp_a1_2->unk2 += temp_ft1;
            temp_a1_2->unk4 += temp_ft4_2;
            temp_a1_2++;

            temp_v0_3->unkC += -0.025f * delayFloat;
            if (temp_t0->unk54 == 0)
            {
                if (temp_a1_2->unk2 < 0)
                {
                    temp_v0_3->unk12 = -1;
                    _bss_38 -= 1;
                    temp_t0->unk58 = ((u8)temp_t0->unk58 - 1);
                    dll_24_func_1CC8(
                        (temp_a1_2->unk0 / 100.0f) + temp_t0->unk0,
                        temp_t0->unk4,
                        (temp_a1_2->unk4 / 100.0f) + temp_t0->unk8,
                        0,
                        0.0f,
                        4);
                }
            }
            else
            {
                temp_a1 = &_bss_8[temp_v0_3->unk12 * 0xE];
                temp_v0_3->unk10 = (0xFF - (u8)temp_a1[1].unkF);
            }
        }
    }
}
#endif

// offset: 0xC7C | func: 3 | export: 2
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_C7C.s")
#else
typedef struct
{
    /*0000*/ u8 unk_0x0;
    /*0001*/ u8 v0;
    /*0002*/ u8 v1;
    /*0003*/ u8 v2;
    /*0004*/ u8 unk_0x4[0x10 - 0x4];
} DLTri;

void _dl_triangles(Gfx **gdl, DLTri *tris, s32 triCount);

void dll_24_func_C7C(Gfx **arg0, Mtx **arg1)
{
    f32 spC0;
    f32 spBC;
    f32 spB8;
    f32 spB4;
    s16 spB0;
    s16 spAE;
    s16 spAC;
    Gfx *temp_v0;
    Gfx *temp_v0_11;
    Gfx *temp_v0_12;
    Gfx *temp_v0_13;
    Gfx *temp_v0_2;
    Gfx *temp_v0_3;
    Gfx *temp_v0_4;
    Gfx *temp_v0_5;
    Gfx *temp_v0_6;
    Gfx *temp_v0_7;
    Gfx *temp_v0_8;
    s32 var_s0;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_s1_4;
    s32 var_s2;
    s32 var_s2_2;
    s32 var_s2_3;
    StructBss24 *temp_s0;
    StructBss2C *temp_s0_2;
    StructBss34 *temp_s0_3;
    StructBss3C *temp_v0_10;
    StructBss3C *temp_v0_9;
    StructBss34 *temp_v1;

    if ((_bss_20 != 0) || (_bss_30 != 0) || (_bss_28 != 0) || (_bss_38 != 0))
    {
        temp_v0 = *arg0;
        temp_v0->words.w1 = 0x200005;
        temp_v0->words.w0 = 0xD9000000;
        dl_apply_geometry_mode(arg0);
        temp_v0_2 = *arg0;
        temp_v0_2->words.w0 = 0xFCFF97FF;
        temp_v0_2->words.w1 = 0xFF10FE3F;
        dl_apply_combine(arg0);
        temp_v0_3 = *arg0;
        temp_v0_3->words.w0 = 0xEF182C00;
        temp_v0_3->words.w1 = 0x104B50;
        dl_apply_other_mode(arg0);
        var_s1 = 0;
        var_s2 = 0;
        if (_bss_20 != 0)
        {
            temp_v0_4 = *arg0;
            *arg0 = temp_v0_4 + 8;
            temp_v0_4->words.w0 = 0xDE000000;
            temp_v0_4->words.w1 = (u32)(_bss_40->gdl + 0x80000000);
        }
        do
        {
            temp_s0 = var_s2 + _bss_24;
            if ((_bss_24 + var_s2)->unk16 != 0)
            {
                dl_set_prim_color(arg0, 0xFFU, 0xFFU, 0xFFU, (u8)(s32)temp_s0->unk16);
                spB8 = temp_s0->unk0;
                spBC = temp_s0->unk4;
                spC0 = temp_s0->unk8;
                spB4 = temp_s0->unk10;
                spB0 = 0;
                spAE = 0;
                spAC = temp_s0->unk14;
                func_800032C4(arg0, arg1, (SRT *)&spAC, 1.0f, 0, NULL);
                temp_v0_5 = *arg0;
                *arg0 = temp_v0_5 + 8;
                temp_v0_5->words.w0 = 0x01004008;
                temp_v0_5->words.w1 = (var_s1 << 6) + _bss_0 + 0x80000000;
                dl_triangles(arg0, (var_s1 << 5) + _bss_4, 2);
            }
            var_s1 += 1;
            var_s2 += 0x1C;
        } while (var_s1 < 0x1E);
        var_s1_2 = 0;
        if (_bss_28 != 0)
        {
            temp_v0_6 = *arg0;
            *arg0 = temp_v0_6 + 8;
            temp_v0_6->words.w0 = 0xDE000000;
            temp_v0_6->words.w1 = (u32)(_bss_44->gdl + 0x80000000);
        }
        var_s2_2 = 0;
        do
        {
            temp_s0_2 = var_s2_2 + _bss_2C;
            if ((_bss_2C + var_s2_2)->unk54 != 0)
            {
                dl_set_prim_color(arg0, 0xFFU, 0xFFU, 0xFFU, (u8)(s32)temp_s0_2->unk54);
                spB8 = temp_s0_2->unk0;
                spBC = temp_s0_2->unk4;
                spB4 = 0.01f;
                spAC = 0;
                spB0 = 0;
                spAE = 0;
                spC0 = temp_s0_2->unk8;
                func_800032C4(arg0, arg1, (SRT *)&spAC, 1.0f, 0, NULL);
                temp_v0_7 = *arg0;
                *arg0 = temp_v0_7 + 8;
                temp_v0_7->words.w0 = 0x0100E01C;
                temp_v0_7->words.w1 = (u32)(&_bss_8[var_s1_2 * 0xE] + 0x80000000);
                dl_triangles(arg0, (var_s1_2 * 0xC * 0x10) + _bss_C, 0xC);
            }
            var_s1_2 += 1;
            var_s2_2 += 0x5C;
        } while (var_s1_2 < 0xA);
        if (_bss_38 != 0)
        {
            temp_v0_8 = *arg0;
            *arg0 = temp_v0_8 + 8;
            temp_v0_8->words.w0 = 0xDE000000;
            temp_v0_8->words.w1 = (u32)(_bss_48->gdl + 0x80000000);
            dl_set_prim_color(arg0, 0xFFU, 0xFFU, 0xFFU, 0xB4U);
        }
        var_s1_3 = 0;
        var_s0 = 0;
        do
        {
            temp_v0_9 = _bss_3C + var_s0;
            if (temp_v0_9->unk12 != -1)
            {
                temp_v0_10 = var_s0 + _bss_3C;
                if (temp_v0_9->unk10 != 0)
                {
                    spB8 = _bss_2C[temp_v0_10->unk12].unk0;
                    spBC = _bss_2C[temp_v0_10->unk12].unk4;
                    spB4 = 0.01f;
                    spAC = 0;
                    spB0 = 0;
                    spAE = 0;
                    spC0 = _bss_2C[temp_v0_10->unk12].unk8;
                    func_800032C4(arg0, arg1, (SRT *)&spAC, 1.0f, 0, NULL);
                    temp_v0_11 = *arg0;
                    *arg0 = temp_v0_11 + 8;
                    temp_v0_11->words.w0 = 0x01004008;
                    temp_v0_11->words.w1 = (u32)((var_s1_3 << 6) + _bss_10 + 0x80000000);
                    dl_triangles(arg0, (var_s1_3 << 5) + _bss_14, 2);
                }
            }
            var_s1_3 += 1;
            var_s0 += 0x14;
        } while (var_s1_3 < 0x1E);
        var_s1_4 = 0;
        var_s2_3 = 0;
        if (_bss_30 != 0)
        {
            temp_v0_12 = *arg0;
            *arg0 = temp_v0_12 + 8;
            temp_v0_12->words.w0 = 0xDE000000;
            temp_v0_12->words.w1 = (u32)(_bss_4C->gdl + 0x80000000);
        }
        do
        {
            temp_v1 = _bss_34 + var_s2_3;
            if (temp_v1->unk14 != 0)
            {
                temp_s0_3 = var_s2_3 + _bss_34;
                if (temp_v1->unk18 == 0)
                {
                    dl_set_prim_color(arg0, 0xFFU, 0xFFU, 0xFFU, (u8)(s32)temp_s0_3->unk14);
                    spB8 = temp_s0_3->unk0;
                    spBC = temp_s0_3->unk4;
                    spC0 = temp_s0_3->unk8;
                    spB4 = temp_s0_3->unk10;
                    spB0 = 0;
                    spAE = 0;
                    spAC = temp_s0_3->unk16;
                    func_800032C4(arg0, arg1, (SRT *)&spAC, 1.0f, 0, NULL);
                    temp_v0_13 = *arg0;
                    *arg0 = temp_v0_13 + 8;
                    temp_v0_13->words.w0 = 0x01004008;
                    temp_v0_13->words.w1 = (var_s1_4 << 6) + _bss_18 + 0x80000000;
                    dl_triangles(arg0, (var_s1_4 << 5) + _bss_1C, 2);
                }
            }
            var_s1_4 += 1;
            var_s2_3 += 0x1C;
        } while (var_s1_4 != 0x1E);
        func_8003DB5C();
    }
}
#endif

// offset: 0x135C | func: 4
s32 dll_24_func_135C(s32 arg0, s32 arg1)
{
    StructBss3C *temp_s1;
    f32 pad;
    s32 temp_v0;
    f32 temp_fs0;
    f32 temp_fs0_2;
    f32 temp_fs1;
    f32 temp_ft4;
    f32 temp_fv0;
    s32 var_fp;
    s32 var_s0;
    StructBss10 *temp_v1;
    s32 sp70[1]; // this needs to be an array for some reason
    s16 var_s5;
    s16 temp_ft1;

    temp_v0 = rand_next(2, 4);
    if ((temp_v0 + _bss_38) >= 0x1F)
    {
        temp_v0 = 0x1E - _bss_38;
    }
    var_s5 = 0;
    if (temp_v0 != 0)
    {
        var_fp = 0;
        if (temp_v0 > 0)
        {
            do
            {
                for (var_s0 = 0; var_s0 < 0x1E && _bss_3C[var_s0].unk12 != -1; var_s0++)
                {
                }

                if (var_s0 < 0x1E)
                {
                    temp_s1 = &_bss_3C[var_s0];
                    sp70[0] = (s16)(0xFFFF / temp_v0);
                    temp_ft1 = rand_next(-0x7D0, 0x7D0);
                    temp_fs0 = fsin16_precise(var_s5 + temp_ft1);
                    temp_fv0 = fcos16_precise(var_s5 + temp_ft1);
                    temp_s1->unk0 = 4.0f * temp_fv0;
                    temp_s1->unk4 = 4.0f * temp_fs0;
                    temp_ft4 = (temp_s1->unk0 * temp_s1->unk0) + (temp_s1->unk4 * temp_s1->unk4);
                    if (temp_ft4 > 0.0f)
                    {
                        temp_ft4 = 1.0f / temp_ft4;
                        temp_s1->unk0 *= temp_ft4;
                        temp_s1->unk4 *= temp_ft4;
                    }
                    temp_s1->unk8 = rand_next(0x258, 0x320) * 0.001f;
                    temp_s1->unkC = rand_next(0x1F4, 0x258) * 0.001f;
                    temp_s1->unk10 = 0;
                    temp_s1->unk12 = arg1;

                    temp_fs0 *= 100.0f;
                    temp_fv0 *= 100.0f;

                    temp_v1 = &_bss_10[var_s0 * 4];
                    temp_v1->unk2 = -300;
                    temp_v1->unkF = -1;
                    temp_v1->unk8 = 992;
                    temp_v1->unkA = 992;
                    temp_v1->unk0 = -2.0f * temp_fv0;
                    temp_v1->unk4 = -2.0f * temp_fs0;

                    temp_v1[1].unk2 = 300;
                    temp_v1[1].unkF = -1;
                    temp_v1[1].unk8 = 992;
                    temp_v1[1].unkA = 0;
                    temp_v1[1].unk0 = temp_v1->unk0;
                    temp_v1[1].unk4 = temp_v1->unk4;
                    temp_v1 += 3;

                    temp_v1[-1].unk2 = -300;
                    temp_v1[-1].unkF = -1;
                    temp_v1[-1].unk8 = 0;
                    temp_v1[-1].unkA = 992;
                    temp_v1[-1].unk0 = 4 * temp_fv0;
                    temp_v1[-1].unk4 = 4 * temp_fs0;

                    temp_v1[0].unk2 = 300;
                    temp_v1[0].unk8 = 0;
                    temp_v1[0].unkA = 0;
                    temp_v1[0].unkF = -1;
                    temp_v1[0].unk0 = temp_v1[-1].unk0;
                    temp_v1[0].unk4 = temp_v1[-1].unk4;
                    _bss_38 += 1;
                    var_s5 += sp70[0];
                }
                var_fp += 1;
            } while (var_fp != temp_v0);
        }
    }
    return temp_v0;
}

// offset: 0x174C | func: 5 | export: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_174C.s")

// offset: 0x1B28 | func: 6 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_1B28.s")

// offset: 0x1C88 | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_1C88.s")

// offset: 0x1CC8 | func: 8 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24/dll_24_func_1CC8.s")
