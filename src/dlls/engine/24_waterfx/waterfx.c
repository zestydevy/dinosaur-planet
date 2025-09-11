#include "dlls/engine/24_waterfx.h"

#include "macros.h"
#include "sys/gfx/map.h"
#include "sys/gfx/texture.h"
#include "sys/memory.h"
#include "sys/rand.h"
#include "sys/main.h"
#include "sys/map.h"
#include "functions.h"
#include "prevent_bss_reordering.h"

// size: 0x10
typedef struct StructBss8
{
    s16 unk0; // unk0 - unk4 xyz?
    s16 unk2;
    s16 unk4;
    s16 unk6; // unk6 - unkA pitch/yaw/roll?
    s16 unk8;
    s16 unkA;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
} StructBss8;

// size 0x1C
typedef struct StructBss24
{
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
} StructBss24;

// size: 0x5C
typedef struct StructBss2C
{
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    u8 pad10[0x24 - 0x10];
    f32 unk24;
    u8 pad28[0x3C - 0x28];
    f32 unk3C;
    u8 pad40[0x54 - 0x40];
    s16 unk54;
    s16 pad56;
    s8 unk58;
} StructBss2C;

// size: 0x1C
typedef struct StructBss34
{
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    s8 unk18;
} StructBss34;

// size: 0x14
typedef struct StructBss3C
{
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    s16 unk10;
    s8 unk12;
} StructBss3C;

static const u8 allocateMemoryError[] = "Could not allocate memory for waterfx dll\n";

void waterfx_func_24C(void);
void waterfx_func_564(Func564Arg0 *arg0, u16 arg1, Vec3f *arg2, Func564Arg3 *arg3, f32 arg4);
void waterfx_func_174C(f32, f32, f32, f32);
void waterfx_func_1B28(f32, f32, f32, s16, f32);
void waterfx_func_1CC8(f32, f32, f32, s16, f32, s32);

static StructBss8 *_bss_0; // 120 items
static DLTri *_bss_4; // 60 items
static StructBss8 *_bss_8; // 140 items
static DLTri *_bss_C; // 120 items
static StructBss8 *_bss_10; // 120 items
static DLTri *_bss_14; // 60 items
static StructBss8 *_bss_18; // 120 items
static DLTri *_bss_1C; // 60 items
static s32 _bss_20; // some sort of counter related to _bss_24
static StructBss24 *_bss_24; // 30 items
static s32 _bss_28; // some sort of counter related to _bss_8
static StructBss2C *_bss_2C; // 10 items
static s32 _bss_30; // some sort of counter related to _bss_34 and _bss_18
static StructBss34 *_bss_34; // 30 items
static s32 _bss_38; // some sort of counter related to _bss_2C and _bss_10
static StructBss3C *_bss_3C; // 30 items
static Texture *_bss_40; // texture if _bss_20 is not null
static Texture *_bss_44; // texture if _bss_28 is not null
static Texture *_bss_48; // texture if _bss_38 is not null
static Texture *_bss_4C; // texture if _bss_30 is not null
static f32 _bss_50;

// offset: 0x0 | ctor
void waterfx_ctor(s32 arg0)
{
    s32 *state;

    state = mmAlloc(0x3E80, 0x13, NULL);
    if (state == NULL) {
        STUBBED_PRINTF(allocateMemoryError); // Not sure this was actually printed but it would make the most sense
        return;
    }

    _bss_4 = (DLTri *) state;
    _bss_C = (DLTri *) state + 60;
    _bss_14 = (DLTri *) state + 180;
    _bss_1C = (DLTri *) state + 240;
    _bss_0 = (StructBss8 *) state + 300;
    _bss_8 = (StructBss8 *) state + 420;
    _bss_10 = (StructBss8 *) state + 560;
    _bss_18 = (StructBss8 *) state + 680;
    state += 3200;
    _bss_24 = (StructBss24 *) state;
    state += 210;
    _bss_2C = (StructBss2C *) state;
    state += 230;
    _bss_3C = (StructBss3C *) state;
    state += 150;
    _bss_34 = (StructBss34 *) state;
    _bss_20 = 0;
    _bss_28 = 0;
    _bss_38 = 0;
    _bss_30 = 0;
    _bss_40 = queue_load_texture_proxy(0x56);
    _bss_44 = queue_load_texture_proxy(0x59);
    _bss_48 = queue_load_texture_proxy(0x22);
    _bss_4C = queue_load_texture_proxy(0x57);
    waterfx_func_24C();
}

// offset: 0x160 | dtor
void waterfx_dtor(s32 arg0)
{
    if (_bss_4 != NULL)
    {
        mmFree(_bss_4);
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
void waterfx_func_24C(void) {
    DLTri *tempTri;
    StructBss3C *temp_a3_2;
    DLTri *tri;
    s32 var_a2;
    StructBss24 *temp_a2;
    StructBss2C *temp_v0;
    DLTri *bssC;
    s32 i;
    u32 targetU;
    s32 temp_var;

    tri = _bss_4;
    for (i = 0; i < 30; i++) {
        tri->v0 = 3;
        tri->v1 = 1;
        tri->v2 = 0;
        tri++;
        tri->v0 = 3;
        tri->v1 = 2;
        tri->v2 = 1;
        tri++;
        temp_a2 = &_bss_24[i];
        temp_a2->unk0 = 0.0f;
        temp_a2->unk4 = 0.0f;
        temp_a2->unk8 = 0.0f;
        temp_a2->unkC = 0.0f;
        temp_a2->unk10 = 0.01f;
        temp_a2->unk16 = 0;
    }
    for (i = 0; i < 10; i++) {
        var_a2 = 0;

        tempTri = &_bss_C[i * 12];
        tri = (DLTri *) tempTri;
        targetU = (u32)&_bss_C[i * 12 + 12];
        while ((u32)tri < targetU) {
            temp_var = var_a2 + 1;
            tri->v0 = var_a2;
            tri->v2 = var_a2 + 2;
            // @fake
            if (1) { }
            tri->v1 = temp_var;
            tri++;
            tri->v1 = var_a2 + 3;
            tri->v2 = var_a2 + 2;
            tri->v0 = temp_var;
            tri++;
            var_a2 += 2;
        }

        temp_v0 = &_bss_2C[i];
        temp_v0->unk0 = 0.0f;
        temp_v0->unk4 = 0.0f;
        temp_v0->unk8 = 0.0f;
        temp_v0->unk54 = 0;
        temp_v0->unk58 = 0;
    }

    tri = _bss_1C;
    for (i = 0; i < 30; i++) {
        tri->v0 = 3;
        tri->v1 = 1;
        tri->v2 = 0;
        tri++;
        tri->v0 = 3;
        tri->v1 = 2;
        tri->v2 = 1;
        tri++;
        temp_a2 = (StructBss24 *) &_bss_34[i];
        temp_a2->unk0 = 0.0f;
        temp_a2->unk4 = 0.0f;
        temp_a2->unk8 = 0.0f;
        temp_a2->unkC = 0.0f;
        temp_a2->unk10 = 0.01f;
        temp_a2->unk14 = 0;
        temp_a2->unk16 = 0;
    }

    tri = _bss_14;
    for (i = 0; i < 30; i++) {
        tri->v0 = 0;
        tri->v1 = 1;
        tri->v2 = 3;
        tri++;
        tri->v0 = 0;
        tri->v1 = 3;
        tri->v2 = 2;
        tri++;

        temp_a3_2 = &_bss_3C[i];
        temp_a3_2->unk12 = -1;
        temp_a3_2->unk0 = 0.0f;
        temp_a3_2->unk4 = 0.0f;
        temp_a3_2->unk10 = 0;
        temp_a3_2->unk8 = 0.0f;
        temp_a3_2->unkC = 0.0f;
    }
}

// offset: 0x564 | func: 1 | export: 1
void waterfx_func_564(Func564Arg0 *arg0, u16 arg1, Vec3f *arg2, Func564Arg3 *arg3, f32 arg4)
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
                    waterfx_func_174C(temp_fs1, arg0->unk10 + arg3->unk1B0, temp_fs2, 0.0f);
                }
                waterfx_func_1CC8(temp_fs1, arg0->unk10 + arg3->unk1B0, temp_fs2, arg0->unk0, 0.0f, 3);
            }
            arg1 >>= 1;
            i += 1;
        } while (arg1);
    }
    if (arg4 > 0.01f)
    {
        waterfx_func_1B28(arg0->unkC, arg0->unk10 + arg3->unk1B0, arg0->unk14, arg0->unk0, 0.0f);
    }
}

// offset: 0x6E8 | func: 2 | export: 0
void waterfx_func_6E8() {
    s32 i;
    f32 temp_fv1;
    f32 var_fv0;
    s16 temp_ft1;
    s16 temp;
    s16 temp_ft4;
    s16 temp_ft4_2;
    s32 j;
    StructBss8* temp_a1;
    StructBss8* temp_a1_2;
    StructBss2C* temp_t0;
    StructBss24* temp_v0;
    StructBss34* temp_v0_2;
    StructBss3C* temp_v0_3;
    StructBss2C *temp2;
    s32 pad[1];

    for (i = 0; i < 30; i++) {
        temp_v0 = &_bss_24[i];
        if (temp_v0->unk16 != 0) {
            temp_v0->unk10 += 0.0007f * delayFloat;
            temp_v0->unk16 -= (delayByte * temp_v0->unk18);
            if (temp_v0->unk16 < 0) {
                temp_v0->unk16 = 0;
                _bss_20 -= 1;
            }
        }
    }

    for (i = 0; i < 10; i++) {
        temp_t0 = &_bss_2C[i];
        if (temp_t0->unk54 != 0) {
            temp_a1 = &_bss_8[i * 0xE];
            var_fv0 = temp_a1[1].unk2 / 400.0f;
            if (var_fv0 < 0.0f) {
                var_fv0 = 0.0f;
            } else if (var_fv0 > 1.0f) {
                var_fv0 = 1.0f;
            } 
            // The backslashes are required to force everything on the same line
            for (j = 0; j < 14; j++) {\
                if (j & 1) {\
                    temp2 = ((StructBss2C *) (((s32 *) temp_t0) + ((j % 12) >> 1)));\
                    temp_a1[j].unk0 += (s16) (temp2->unkC * delayFloat * 100.0f); \
                    temp_a1[j].unk2 += (s16) (temp2->unk24 * delayFloat * 100.0f); \
                    temp_a1[j].unk4 += (s16) (temp2->unk3C * delayFloat * 100.0f); \
                }\
                temp = 255.0f - (var_fv0 * 255.0f);\
                temp_a1[j].unkF = temp;
            }
            if (temp_a1[1].unk2 >= 0x191) {
                temp_t0->unk54 = 0;
                _bss_28 -= 1;
            }
        }
    }

    for (i = 0; i < 30; i++) {
        temp_v0_2 = &_bss_34[i];
        if (temp_v0_2->unk14 != 0) {
            temp_v0_2->unk10 += 0.004f * delayFloat;
            temp_v0_2->unk14 -= delayByte * 5;
            if (temp_v0_2->unk14 < 0) {
                temp_v0_2->unk14 = 0;
                _bss_30 -= 1;
            }
            if ((u8)temp_v0_2->unk18 == 0) {
                temp_v0_2->unk18 = 1;
            }
        }
    }

    for (i = 0; i < 0x1E; i++) {
        temp_v0_3 = &_bss_3C[i];
        if (temp_v0_3->unk12 != -1) {
            temp_t0 = &_bss_2C[temp_v0_3->unk12];
            temp_a1 = &_bss_10[i * 4];

            var_fv0 = 100.0f * delayFloat;
            temp_fv1 = temp_v0_3->unk8 * var_fv0;
            temp_ft4 = (temp_v0_3->unk0 * temp_fv1);
            temp_ft1 = (temp_v0_3->unkC * var_fv0);
            temp_ft4_2 = (temp_v0_3->unk4 * temp_fv1);

            temp_a1->unk0 += temp_ft4;
            temp_a1->unk2 += temp_ft1;
            temp_a1->unk4 += temp_ft4_2;
            temp_a1 += 3;
            temp_a1[-2].unk0 += temp_ft4;
            temp_a1[-2].unk2 += temp_ft1;
            temp_a1[-2].unk4 += temp_ft4_2;
            temp_a1[-1].unk0 += temp_ft4;
            temp_a1[-1].unk2 += temp_ft1;
            temp_a1[-1].unk4 += temp_ft4_2;
            temp_a1->unk0 += temp_ft4;
            temp_a1->unk2 += temp_ft1;
            temp_a1->unk4 += temp_ft4_2;

            temp_v0_3->unkC += -0.025f * delayFloat;
            if (temp_t0->unk54 == 0) {
                if (temp_a1->unk2 < 0) {
                    temp_v0_3->unk12 = -1;
                    _bss_38 -= 1;
                    temp_t0->unk58 =  ((u8)temp_t0->unk58 - 1);
                    waterfx_func_1CC8((temp_a1->unk0 / 100.0f) + temp_t0->unk0, temp_t0->unk4, (temp_a1->unk4 / 100.0f) + temp_t0->unk8, 0, 0.0f, 4);
                }
            } else {
                temp_a1_2 = &_bss_8[temp_v0_3->unk12 * 0xE];
                temp_v0_3->unk10 = (0xFF - (u8)temp_a1_2[1].unkF);
            }
        }
    }
}


// offset: 0xC7C | func: 3 | export: 2
void waterfx_func_C7C(Gfx** gdl, Mtx** arg1) {
    s32 i;
    SRT spAC;
    StructBss24* temp_s0;
    StructBss2C* temp_s0_2;
    StructBss34 *temp_s0_3;
    StructBss3C* temp_v0_9;

    if ((_bss_20 != 0) || (_bss_30 != 0) || (_bss_28 != 0) || (_bss_38 != 0)) {
        gSPLoadGeometryMode(*gdl, G_SHADE | G_ZBUFFER | G_SHADING_SMOOTH);
        dl_apply_geometry_mode(gdl);

        // macro correct, invalid params
        /*
        gDPSetCombineLERP(*gdl, 
            0, 0, 0, ENVIRONMENT, 
            0, 0, 0, ENVIRONMENT, 
            0, 0, 0, ENVIRONMENT, 
            0, 0, 0, ENVIRONMENT);
        */
        {
            Gfx *_g = *gdl;
            _g->words.w0 = 0xFCFF97FF;\
            _g->words.w1 = 0xFF10FE3F;
        }
        dl_apply_combine(gdl);
        
        // TODO: Find/use correct flags
        gDPSetOtherMode(*gdl, 0xEF182C00, 0x104B50);
        dl_apply_other_mode(gdl);
        if (_bss_20 != 0) {
            gSPDisplayList((*gdl)++, OS_PHYSICAL_TO_K0(_bss_40->gdl));
        }
        for (i = 0; i < 30; i++) {
            if (_bss_24[i].unk16 != 0) {
                temp_s0 = &_bss_24[i];
                dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, temp_s0->unk16);
                spAC.transl.x = temp_s0->unk0;
                spAC.transl.y = temp_s0->unk4;
                spAC.transl.z = temp_s0->unk8;
                spAC.scale = temp_s0->unk10;
                spAC.yaw = temp_s0->unk14;
                spAC.roll = 0;
                spAC.pitch = 0;
                func_800032C4(gdl, arg1, &spAC, 1.0f, 0.0f, NULL);
                {
                    // TODO: Find correct macro
                    Gfx *_g = (Gfx *)((*gdl)++);
                    _g->words.w0 = 0x01004008; 
                    _g->words.w1 = (unsigned int)OS_PHYSICAL_TO_K0(&_bss_0[i << 2]); 
                }
                dl_triangles(gdl, &_bss_4[i << 1], 2);
            }
        }
        if (_bss_28 != 0) {
            gSPDisplayList((*gdl)++, OS_PHYSICAL_TO_K0(_bss_44->gdl));
        }
        for (i = 0; i < 10; i++) {
            if (_bss_2C[i].unk54 != 0) {
                temp_s0_2 = &_bss_2C[i];
                dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, temp_s0_2->unk54);
                spAC.transl.x = temp_s0_2->unk0;
                spAC.transl.y = temp_s0_2->unk4;
                spAC.transl.z = temp_s0_2->unk8;
                spAC.scale = 0.01f;
                spAC.yaw = 0;
                spAC.roll = 0;
                spAC.pitch = 0;
                func_800032C4(gdl, arg1, &spAC, 1.0f, 0.0f, NULL);
                {
                    // TODO: Find correct macro
                    Gfx *_g = (Gfx *)((*gdl)++);
                    _g->words.w0 = 0x0100E01C;
                    _g->words.w1 = (unsigned int)OS_PHYSICAL_TO_K0(&_bss_8[i * 0xE]);
                }
                dl_triangles(gdl, &_bss_C[i * 0xC], 0xC);
            }
        }
        if (_bss_38 != 0) {
            gSPDisplayList((*gdl)++, OS_PHYSICAL_TO_K0(_bss_48->gdl));
            dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, 0xB4U);
        }
        for (i = 0; i < 30; i++) {
            if (_bss_3C[i].unk12 != -1 && _bss_3C[i].unk10 != 0) {
                temp_v0_9 = &_bss_3C[i];
                spAC.transl.x = _bss_2C[temp_v0_9->unk12].unk0;
                spAC.transl.y = _bss_2C[temp_v0_9->unk12].unk4;
                spAC.transl.z = _bss_2C[temp_v0_9->unk12].unk8;
                spAC.scale = 0.01f;
                spAC.yaw = 0;
                spAC.roll = 0;
                spAC.pitch = 0;
                func_800032C4(gdl, arg1, &spAC, 1.0f, 0.0f, NULL);
                {
                    // TODO: Find correct macro
                    Gfx *_g = (Gfx *)((*gdl)++);
                    _g->words.w0 = 0x01004008;
                    _g->words.w1 = (unsigned int)OS_PHYSICAL_TO_K0(&_bss_10[i << 2]);
                }
                dl_triangles(gdl, &_bss_14[i << 1], 2);
            }
        }
        if (_bss_30 != 0) {
            gSPDisplayList((*gdl)++, OS_PHYSICAL_TO_K0(_bss_4C->gdl));
        }
        for (i = 0; i < 30; i++) {
            if (_bss_34[i].unk14 != 0 && (u8)_bss_34[i].unk18 == 0) {
                temp_s0_3 = &_bss_34[i];
                dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, temp_s0_3->unk14);
                spAC.transl.x = temp_s0_3->unk0;
                spAC.transl.y = temp_s0_3->unk4;
                spAC.transl.z = temp_s0_3->unk8;
                spAC.scale = temp_s0_3->unk10;
                spAC.yaw = temp_s0_3->unk16;
                spAC.roll = 0;
                spAC.pitch = 0;
                func_800032C4(gdl, arg1, &spAC, 1.0f, 0.0f, NULL);
                {
                    // TODO: Find correct macro
                    Gfx *_g = (Gfx *)((*gdl)++);
                    _g->words.w0 = 0x01004008;
                    _g->words.w1 = (unsigned int)OS_PHYSICAL_TO_K0(&_bss_18[i << 2]);
                }
                dl_triangles(gdl, &_bss_1C[i << 1], 2);
            }
        }
        func_8003DB5C();
    }
}

// offset: 0x135C | func: 4
/* static */ s32 waterfx_func_135C(StructBss2C *arg0, s32 arg1)
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
    StructBss8 *temp_v1;
    s32 sp70[1]; // this needs to be an array for some reason
    s16 var_s5;
    s16 temp_ft1;

    temp_v0 = rand_next(2, 4);
    if ((temp_v0 + _bss_38) >= 0x1F)
    {
        temp_v0 = 30 - _bss_38;
    }
    var_s5 = 0;
    if (temp_v0 != 0)
    {
        var_fp = 0;
        if (temp_v0 > 0)
        {
            do
            {
                for (var_s0 = 0; var_s0 < 30 && _bss_3C[var_s0].unk12 != -1; var_s0++)
                {
                }

                if (var_s0 < 30)
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
// Single sp offset problem, non matching, requires waterfx_func_135C to be static
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/24_waterfx/waterfx_func_174C.s")
#else
void waterfx_func_174C(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    StructBss2C* temp_fp;
    f32 temp_fs1;
    f32 temp_ft4;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32* var_s1; // this might be incorrect
    s16 var_s2;
    s16 var_s4;
    s32 var_s3;
    s32 var_s5;
    StructBss8* var_s0;

    if (arg3 == 0.0f) {
        arg3 = 4.0f;
    }
    for ( var_s5 = 0; var_s5 < 0xA && (u8) _bss_2C[var_s5].unk58 != 0; var_s5++) {}
    if (var_s5 >= 0xA) {
        return;
    }

    temp_fp = &_bss_2C[var_s5];
    var_s0 = &_bss_8[var_s5 * 0xE];
    temp_fs1 = arg3 * 100.0f;
    arg3 = 4.0f;
    var_s0->unk0 = temp_fs1;

    var_s0->unk2 = 0;
    var_s0->unk4 = 0;
    var_s0->unkC = 0xFF;
    var_s0->unkD = 0;
    var_s0->unkE = 0;
    var_s0->unkF = 0xFF;
    var_s0->unk8 = 0;
    var_s0->unkA = 0;
    var_s0++;
    var_s0->unk0 = temp_fs1;
    var_s0->unk2 = 0;
    var_s0->unk4 = 0;
    var_s0->unkC = 0xFF;
    var_s0->unkD = 0;
    var_s0->unkE = 0;
    var_s0->unkF = 0xFF;
    var_s0->unk8 = 0;
    var_s0->unkA = 0x3A0;
    var_s0++;
    temp_fp->unkC = arg3;
    temp_fp->unk24 = arg3;
    temp_fp->unk3C = 0.0f;
    temp_ft4 = 2.0f * (arg3 * arg3);
    var_s4 = 0x2AAA;
    if (temp_ft4 > 0.0f) {
        // @fake
        if (1) {}
        temp_ft4 = 1.0f / temp_ft4;
        temp_fp->unkC *= temp_ft4;
        temp_fp->unk24 *= temp_ft4;
    }
    var_s2 = 0;
    var_s3 = 4;
    var_s1 = &temp_fp->unk4;
    do {
        temp_fv0 = fcos16_precise(var_s4);
        temp_fv0_2 = fsin16_precise(var_s4);
        ((StructBss2C *)var_s1)->unkC = arg3 * temp_fv0;
        ((StructBss2C *)var_s1)->unk24 = arg3;
        ((StructBss2C *)var_s1)->unk3C = arg3 * temp_fv0_2;
        temp_ft4 = (((StructBss2C *)var_s1)->unk3C * ((StructBss2C *)var_s1)->unk3C) + ((((StructBss2C *)var_s1)->unkC * ((StructBss2C *)var_s1)->unkC) + (((StructBss2C *)var_s1)->unk24 * ((StructBss2C *)var_s1)->unk24));
        if (temp_ft4 > 0.0f) {
            temp_ft4 = 1.0f / temp_ft4;
            ((StructBss2C *)var_s1)->unkC *= temp_ft4;
            ((StructBss2C *)var_s1)->unk24 *= temp_ft4;
            ((StructBss2C *)var_s1)->unk3C *= temp_ft4;
        }
        var_s0->unk2 = 0;
        var_s0->unkF = 0xFF;
        var_s2 += 0xC00;
        var_s0->unk8 = var_s2;
        var_s0->unkA = 0;
        var_s0[1].unk2 = 0;
        var_s0[1].unkF = 0xFF;
        var_s0[1].unk8 = var_s2;
        if (0) { }
        var_s0[1].unkA = 0x3A0;
        var_s0->unk0 = temp_fs1 * temp_fv0;
        var_s0->unk4 = temp_fs1 * temp_fv0_2;
        var_s0[1].unk0 = var_s0->unk0;
        var_s0[1].unk4 = var_s0->unk4;
        var_s4 += 0x2AAA;
        var_s0 += 2;
        var_s1++;
        var_s3 += 4;
    } while (var_s3 != 0x18);
    var_s2 += 0xC00;
    var_s0->unk0 = temp_fs1;
    var_s0->unk4 = 0;
    var_s0->unk2 = 0;
    var_s0->unkF = 0xFF;
    var_s0->unkE = 0;
    var_s0->unkD = 0;
    var_s0->unkC = 0xFF;
    var_s0->unk8 = var_s2;
    var_s0->unkA = 0;
    var_s0++;
    var_s0->unk0 = temp_fs1;
    var_s0->unk4 = 0;
    var_s0->unk2 = 0;
    var_s0->unkF = 0xFF;
    var_s0->unkE = 0;
    var_s0->unkD = 0;
    var_s0->unkC = 0xFF;
    var_s0->unk8 = var_s2;
    var_s0->unkA = 0x3A0;
    temp_fp->unk54 = 0xFF;
    temp_fp->unk0 = arg0;
    temp_fp->unk4 = arg1;
    temp_fp->unk8 = arg2;
    _bss_28 += 1;
    temp_fp->unk58 = waterfx_func_135C(&_bss_2C[var_s5], var_s5);
}
#endif

// offset: 0x1B28 | func: 6 | export: 5
void waterfx_func_1B28(f32 arg0, f32 arg1, f32 arg2, s16 arg3, f32 arg4)
{
    StructBss34 *temp_v0;
    s32 var_v0;
    StructBss8 *temp_a0;

    for (var_v0 = 0; var_v0 < 30 && _bss_34[var_v0].unk14 != 0; var_v0++)
    {
    }
    if (var_v0 >= 30)
    {
        return;
    }

    temp_a0 = &_bss_18[var_v0 * 4];
    temp_a0->unk0 = -200;
    temp_a0->unk2 = 0;
    temp_a0->unk4 = 400;
    temp_a0->unkF = -1;
    temp_a0->unk8 = 0;
    temp_a0->unkA = 0;
    temp_a0++;
    temp_a0->unk0 = -200;
    temp_a0->unk2 = 0;
    temp_a0->unk4 = -200;
    temp_a0->unkF = -1;
    temp_a0->unk8 = 0;
    temp_a0->unkA = 992;
    temp_a0++;
    temp_a0->unk0 = 200;
    temp_a0->unk2 = 0;
    temp_a0->unk4 = -200;
    temp_a0->unkF = -1;
    temp_a0->unk8 = 2016;
    temp_a0->unkA = 992;
    temp_a0++;
    temp_a0->unk0 = 200;
    temp_a0->unk2 = 0;
    temp_a0->unk4 = 400;
    temp_a0->unkF = -1;
    temp_a0->unk8 = 2016;
    temp_a0->unkA = 0;
    temp_v0 = &_bss_34[var_v0];
    temp_v0->unk0 = arg0;
    temp_v0->unk4 = arg1;
    temp_v0->unk8 = arg2;
    temp_v0->unkC = arg4;
    temp_v0->unk10 = 0.01f;
    temp_v0->unk14 = 128;
    temp_v0->unk16 = arg3;
    temp_v0->unk18 = 0;
    _bss_30 += 1;
}

// offset: 0x1C88 | func: 7 | export: 7
// Some sort of setter. Sets _bss_50 to arg1 if first argument is not zero
void waterfx_func_1C88(s32 arg0, f32 arg1) {
    if (arg0 != 0) {
        _bss_50 = 0.01f;
        return;
    }
    _bss_50 = arg1;
}

// offset: 0x1CC8 | func: 8 | export: 4
void waterfx_func_1CC8(f32 arg0, f32 arg1, f32 arg2, s16 arg3, f32 arg4, s32 arg5) {
    s32 var_v0;
    s32 i;
    StructBss8* temp_a1;

    for (var_v0 = 0; var_v0 < 30 && _bss_24[var_v0].unk16 != 0; var_v0++) {}
    if (var_v0 >= 30) {
        return;
    }

    i = var_v0 * 4;
    temp_a1 = &_bss_0[i];
    temp_a1->unk0 = -0xC8;
    temp_a1->unk2 = 0;
    temp_a1->unk4 = 0xC8;
    temp_a1->unkF = 0xFF;
    temp_a1->unk8 = 0;
    temp_a1->unkA = 0;
    temp_a1 = &_bss_0[i + 1];
    temp_a1->unk0 = -0xC8;
    temp_a1->unk2 = 0;
    temp_a1->unk4 = -0xC8;
    temp_a1->unkF = 0xFF;
    temp_a1->unk8 = 0;
    temp_a1->unkA = 0x7E0;
    temp_a1 = &_bss_0[i + 2];
    temp_a1->unk0 = 0xC8;
    temp_a1->unk2 = 0;
    temp_a1->unk4 = -0xC8;
    temp_a1->unkF = 0xFF;
    temp_a1->unk8 = 0x7E0;
    temp_a1->unkA = 0x7E0;
    temp_a1 = &_bss_0[i + 3];
    temp_a1->unk0 = 0xC8;
    temp_a1->unk2 = 0;
    temp_a1->unk4 = 0xC8;
    temp_a1->unkF = 0xFF;
    temp_a1->unk8 = 0x7E0;
    temp_a1->unkA = 0;
    _bss_24[var_v0].unkC = arg4;
    _bss_24[var_v0].unk16 = 0xFF;
    _bss_24[var_v0].unk0 = arg0;
    _bss_24[var_v0].unk4 = arg1;
    _bss_24[var_v0].unk8 = arg2;
    _bss_24[var_v0].unk14 = arg3;
    _bss_24[var_v0].unk10 = _bss_50;
    _bss_24[var_v0].unk18 = arg5;
    _bss_20 += 1;
}
