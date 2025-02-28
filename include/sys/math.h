/** Math and trigonometry
 */
#ifndef _SYS_MATH_H
#define _SYS_MATH_H

#include <PR/gbi.h>

#define M_PI_F    3.141592741f
#define DEG_TO_RAD(deg)  ((deg) * M_PI_F / 180.0f)

// typedef f32 MtxF[4][4];
// For clarity:
typedef struct {
    f32 m[4][4];
} MtxF;

typedef f32 quartic[5];

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    f32 x, y, z, w;
} Vec4f;

typedef struct {
    s32 x, y, z;
} Vec3s32;

typedef f32 Vec2[2];
typedef f32 Vec3[3];
typedef f32 Vec4[4];
typedef s16 Vec3s16[3];

typedef struct {
    Vec3s32 min;
    Vec3s32 max;
} AABBs32;

// Scale, rotation, translation. Rotations are applied in the order: Yaw -> Pitch -> Roll. (TODO: verify)
typedef struct
{
/*0000*/    s16 yaw; // 16-bit angle in the range [-32768..32768) => [-pi..pi)
/*0002*/    s16 pitch;
/*0004*/    s16 roll;
/*0006*/    s16 flags;
/*0008*/    f32 scale;
/*000C*/	Vec3f transl;
} SRT;

void vec3_transform(MtxF *mtx, float x, float y, float z, float *ox, float *oy, float *oz);
void vec3_transform_no_translate(MtxF *mf, Vec3f *v, Vec3f *ov);
void matrix_concat(MtxF *m1, MtxF *m2, MtxF *output);
void matrix_concat_4x3(MtxF *m1, MtxF *m2, MtxF *output);
void matrix_concat_3x3(MtxF *m1, MtxF *m2, MtxF *output);
void matrix_f2l(MtxF *mf, Mtx *ml);
void matrix_f2l_4x3(MtxF *mf, Mtx *ml);
void matrix_f2l_translate_only(MtxF *mf, Mtx *ml);

void s16_matrix_rotate(Mtx *m, s16 *);
void matrix_from_srt(MtxF *mf, SRT *srt);
void matrix_prescale_y(MtxF *mf, f32 scale);
void matrix_trans_y(MtxF *mf, f32 scale);
void matrix_from_srt_reversed(MtxF *mf, SRT *srt);
void matrix_from_srt_rotation(MtxF *mf, SRT *srt);
void matrix_from_yaw(s16 theta, MtxF *mf);
void s16_vec3_apply_srt_rotation(SRT *srt, s16 *vec);
void rotate_vec3(SRT *srt, Vec3f *vec);
void rotate_vec_inv(SRT *srt, Vec3f *vec);
void vec_from_rotation(SRT *srt, Vec3f *vec);
void matrix_translation(MtxF *mf, f32 x, f32 y, f32 z);
Mtx *get_some_model_view_mtx();
void matrix_scaling(MtxF *mf, f32 sx, f32 sy, f32 sz);
s16 rotation16_sub_wrap(s32, s32);
f32 func_800164A0(f32, f32, f32, f32, f32);

f32 fsin16_precise(s16 theta);
f32 fcos16_precise(s16 theta);

// theta: [-32768..32768) => [-pi..pi)
// returns: [-65536..65536] => [-1..1]
s32 cos16_precise(s16 theta);

// theta: [-32768..32768) => [-pi..pi)
// returns: [-65536..65536] => [-1..1]
s32 sin16_precise(s16 theta);

// theta: [-32768..32768) => [-pi..pi)
// returns: [-65536..65536] => [-1..1]
s32 cos16(s16 theta);

// theta: [-32768..32768) => [-pi..pi)
// returns: [-65536..65536] => [-1..1]
s32 sin16(s16 theta);

f32 fcos16(s16 theta);
f32 fsin16(s16 theta);

f32 sinf(f32);

s16 arctan2s(s16 x, s16 z);
s16 arctan2_f(f32 x, f32 z);
s16 atan2f_to_s(f32 x, f32 z);
f32 atan2f(f32 x, f32 z);

f32 acosf(f32);
f32 asinf(f32);
u16 acos(s16);
s16 asin(s16);

s32 floor_f(f32);
s32 ceil_f(f32);

f32 vec3_distance(Vec3f *v1, Vec3f *v2);
f32 vec3_distance_squared(Vec3f *v1, Vec3f *v2);
f32 vec3_distance_xz(Vec3f *v1, Vec3f *v2);
f32 vec3_distance_xz_squared(Vec3f *v1, Vec3f *v2);

f32 vec3_dot_product(Vec3f *v1, Vec3f *v2);
f32 vec3_length(Vec3f *v);
f32 vec3_length_squared(Vec3f *v);
void vec3_cross_product(Vec3f *v1, Vec3f *v2, Vec3f *result);
void vec3_cross_product_2(Vec3f *v1, Vec3f *v2, Vec3f *result);
f32 vec3_normalize(Vec3f *v);
void vec3_sub(Vec3f *v1, Vec3f *v2, Vec3f *result);
void vec3_add_with_scale(Vec3f *v1, Vec3f *v2, f32 scale, Vec3f *result);
void vec3_reflect(Vec3f *v1, Vec3f *v2, Vec3f *result);

#endif //_SYS_MATH_H
