#include "game/objects/object.h"
#include "sys/math.h"
#include "sys/voxmap.h"
#include "macros.h"

s16 projlib_Func_304(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 arg6, f32 arg7, s32 arg8);

// offset: 0x0 | ctor
void projlib_ctor(void* dll) {
    STUBBED_PRINTF("\n PROJLIB LOADED \n");
}

// offset: 0xC | dtor
void projlib_dtor(void* dll) {
    STUBBED_PRINTF("\n PROJLIB FREEDED \n");
}

// offset: 0x18 | func: 0 | export: 0
s32 projlib_Func_18(Object* obj, Vec3f* arg1, f32 arg2, f32 arg3, s32 arg4, Vec3f* arg5) {
    Vec3s16 sp68;
    Vec3s16 sp60;
    Vec3s16 sp58;
    s16 sp56;
    s16 sp54;
    f32 sp48[3];
    f32 sp44;

    if (!(arg4 & 0x10)) {
        vox_func_80007EE0(&obj->srt.transl, &sp68);
        vox_func_80007EE0(arg1, &sp60);
        if (vox_func_80008048(&sp68, &sp60, &sp58, NULL, 0) == 0) {
            return 0;
        }
    }
    sp54 = projlib_Func_304(obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z, arg1->x, arg1->y, arg1->z, arg2, arg3, arg4 & 1);
    sp56 = mathAtan2f(arg1->x - obj->srt.transl.x, arg1->z - obj->srt.transl.z);
    arg5->x = mathCosfInterp(sp54) * arg2 * mathSinfInterp(sp56);
    arg5->y = mathSinfInterp(sp54) * arg2;
    arg5->z = mathCosfInterp(sp54) * arg2 * mathCosfInterp(sp56);
    if (arg4 & 2) {
        obj->srt.yaw = sp56;
    }
    if (arg4 & 4) {
        obj->srt.pitch = sp54;
    }
    if (arg4 & 0x20) {
        sp48[0] = arg1->x - obj->srt.transl.x;
        sp48[1] = arg1->y - obj->srt.transl.y;
        sp48[2] = arg1->z - obj->srt.transl.z;
        sp44 = sqrtf(SQ(sp48[0]) + SQ(sp48[2]));
        sp44 = 65535.0f * (sqrtf(SQ(arg5->f[0]) + SQ(arg5->f[2])) / sp44);
        sp44 = CLAMP_EXPR(sp44, 0, 65535);
        obj->srt.pitch = (s16) sp44;
    }
    return 1;
}

// offset: 0x304 | func: 1 | export: 1
s16 projlib_Func_304(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 arg6, f32 arg7, s32 arg8) {
    f32 sp6C;
    f32 sp5C;
    f32 temp_fs3;
    f32 temp_fs5;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fa1;
    f32 temp;
    s32 var_s1;
    s32 var_s2;
    s16 sp7C[2];

    var_s2 = 0;
    temp_fv0 = x1 - x2;
    temp_fv1 = z1 - z2;
    temp_fv0 = sqrtf(SQ(temp_fv0) + SQ(temp_fv1));
    temp_fs5 = y2 - y1;
    sp6C = (temp_fs5 * arg7) + SQ(arg6);
    temp = SQ(sp6C) - (SQ(arg7) * (SQ(temp_fv0) + SQ(temp_fs5)));
    if (temp_fv0 == 0) {
        return 0x2000;
    }
    if (temp >= 0.0f) {
        var_s1 = 2;
        sp5C = 2.0f *((SQ(temp_fs5) / SQ(temp_fv0)) + 1.0f);
        temp_fs3 = sqrtf(temp);
        while (var_s1--) {
            var_fa1 = (((var_s1 != 0) ? temp_fs3 : -temp_fs3) + sp6C) / sp5C;
            if (var_fa1 >= 0.0f) {
                var_fa1 = sqrtf(var_fa1);
                if (temp_fv0 < 0.0f) {
                    var_fa1 = -var_fa1;
                }
                sp7C[var_s2] = mathAtan2f(((temp_fs5 / temp_fv0) * var_fa1) - ((arg7 * temp_fv0) / (2.0f * var_fa1)), var_fa1);
                var_s2 += 1;
            }
        }
    }
    switch (var_s2) {
    case 2:
        if (sp7C[0] < sp7C[1]) {
            return (arg8 != 0) ? sp7C[1] : sp7C[0];
        } else {
            return (arg8 != 0) ? sp7C[0] : sp7C[1];
        }
        return sp7C[1];
    case 1:
        return sp7C[0];
    default:
        return 0x2000;
    }
}

// offset: 0x580 | func: 2 | export: 2
f32 projlib_Func_580(s32 arg0, f32 arg1, f32 arg2) {
    f32 var_fa0;
    f32 var_fv0;
    f32 var_fv1;
    s32 temp_v0;
    s32 temp;

    temp = 0;
    var_fv0 = 0.0f;
    var_fv1 = 0.0f;
    temp_v0 = arg0 < 0;
    var_fa0 = arg1;
    if (temp_v0 != 0) {
        arg0 = -arg0;
        arg1 = -arg1;
        var_fa0 = arg1;
    }
    if (arg1 < 0.0f) {
        return (temp_v0 != 0) ? -arg2 : arg2;
    }
    do {
        var_fv1 += arg2;
        var_fv0 += var_fv1;
        temp++;
    } while ((var_fv0 + var_fv1) < arg0);
    if ((arg0 <= arg2) && (var_fa0 <= arg2) && (((arg0 >= 0) && (var_fa0 >= 0.0f)) || ((arg0 <= 0) && (var_fa0 <= 0.0f)))) {
        var_fa0 = 0.0f;
    } else {
        if ((var_fa0 + arg2) <= (var_fv1 - arg2)) {
            var_fa0 = (var_fa0 + arg2);
        } else if ((var_fa0 - arg2) < var_fv1) {
            var_fa0 = (var_fv1 - arg2);
            if (var_fv1 == arg2) {
                var_fa0 = arg2;
            }
        } else {
            var_fa0 = (var_fa0 - arg2);
            if (var_fa0 == 0.0f) {
                var_fa0 = arg2;
            }
        }
    }
    if (temp_v0 != 0) {
        var_fa0 = -var_fa0;
        arg1 = -arg1;
    }
    return var_fa0 - arg1;
}
