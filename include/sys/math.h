/** Math and trigonometry
 */
#ifndef _SYS_MATH_H
#define _SYS_MATH_H

#include "PR/gbi.h"

#define M_PI_F    3.141592741f
#define DEG_TO_RAD(deg)  ((deg) * M_PI_F / 180.0f)
#define SQ(a) (a) * (a)

#define M_45_DEGREES  0x2000    // 45 degrees in the game's angle system
#define M_90_DEGREES  0x4000    // 90 degrees in the game's angle system
#define M_180_DEGREES 0x8000    //180 degrees in the game's angle system

/**
 * Keeps the value within the range.
 */
#define CLAMP(x, low, high) {       \
    if ((x) > (high)) (x) = (high); \
    if ((x) < (low)) (x) = (low);   \
}

/**
 * Allows an arbitrary range the number can wrap around.
 * Often used for angles, to keep them within s16 bounds while stored in an s32.
 */
#define WRAP(x, low, high) {                    \
    if ((x) > (high)) (x) -= ((high) * 2) - 1;  \
    if ((x) < (low)) (x) += ((high) * 2) - 1;   \
}

/*
 * CIRCLE_WRAP - Wraps a value within a circular range
 *
 * Specifically designed for the game's angle system where:
 * - Angles use a signed 16-bit range: -0x8000 to 0x7FFF
 * - This represents a full circle from -180° to just under +180°
 * - The wrap boundary is at 0x8000, which wraps to -0x8000
 *
 * Usage for angles: CIRCLE_WRAP(angle)
 * Usage for other ranges: WRAP(value, low, high)
 */
#define CIRCLE_WRAP(x) WRAP(x, -0x8000, 0x8000)

#define DOT_PRODUCT(vA, vB) ((vA.f[0] * vB.f[0]) + (vA.f[1] * vB.f[1]) + (vA.f[2] * vB.f[2]))

#define VECTOR_ADD(vInA, vInB, vOut) vOut.f[0] = vInA.f[0] + vInB.f[0];\
    vOut.f[1] = vInA.f[1] + vInB.f[1];\
    vOut.f[2] = vInA.f[2] + vInB.f[2];

#define VECTOR_SUBTRACT(vInA, vInB, vOut) vOut.f[0] = vInA.f[0] - vInB.f[0];\
    vOut.f[1] = vInA.f[1] - vInB.f[1];\
    vOut.f[2] = vInA.f[2] - vInB.f[2];

#define VECTOR_MAGNITUDE_SQUARED(v) (SQ(v.f[0]) + SQ(v.f[1]) + SQ(v.f[2]))

#define VECTOR_MAGNITUDE(v) sqrtf(VECTOR_MAGNITUDE_SQUARED(v))

#define VECTOR_DIVIDE_BY_SCALAR(v, scalar) if (scalar != 0.0f) {\
    v.x /= scalar;\
    v.y /= scalar;\
    v.z /= scalar;\
}

/*
 * VECTOR_NORMALISE - Converts a vector into a unit vector by dividing it by its own magnitude.
 *
 * The vector's magnitude (prior to normalisation) will also be stored to vMagnitude
 */
#define VECTOR_NORMALISE(v, vMagnitude) vMagnitude = VECTOR_MAGNITUDE(v);\
    VECTOR_DIVIDE_BY_SCALAR(v, vMagnitude)

// typedef f32 MtxF[4][4];
// For clarity:
typedef struct {
    f32 m[4][4];
} MtxF;

typedef f32 quartic[5];

typedef struct Vec2f {
    union {
        struct {
            f32 x;
            f32 y;
        };
        f32 f[2];
    };
} Vec2f;

typedef struct Vec3f {
    union {
        struct {
            f32 x;
            f32 y;
            f32 z;
        };
        f32 f[3];
    };
} Vec3f;

typedef struct Vec4f {
    union {
        struct {
            f32 x;
            f32 y;
            f32 z;
            f32 w;
        };
        f32 f[4];
    };
} Vec4f;

typedef struct {
    s32 x, y, z;
} Vec3s32;

typedef f32 Vec2[2];
typedef f32 Vec3[3];
typedef f32 Vec4[4];
typedef union {
    struct {
        s16 unk0;
        s16 unk2;
        s16 unk4;
    };
    s16 s[3];
} Vec3s16;

typedef struct {
    Vec3s32 min;
    Vec3s32 max;
} AABBs32;

// Scale, rotation, translation. Rotations are applied in the order: Yaw -> Pitch -> Roll. (TODO: verify)
// size: 0x18
typedef struct {
/*0000*/    s16 yaw; // 16-bit angle in the range [-32768..32768) => [-pi..pi)
/*0002*/    s16 pitch;
/*0004*/    s16 roll;
/*0006*/    s16 flags;
/*0008*/    f32 scale;
/*000C*/	Vec3f transl;
} SRT;

void vec3_transform(const MtxF *mtx, float x, float y, float z, float *ox, float *oy, float *oz);
void vec3_transform_no_translate(const MtxF *mf, const Vec3f *v, Vec3f *ov);
void matrix_concat(const MtxF *m1, const MtxF *m2, MtxF *output);
void matrix_concat_4x3(const MtxF *m1, const MtxF *m2, MtxF *output);
void matrix_concat_3x3(const MtxF *m1, const MtxF *m2, MtxF *output);
void matrix_f2l(const MtxF *mf, Mtx *ml);
void matrix_f2l_4x3(const MtxF *mf, Mtx *ml);
void matrix_f2l_translate_only(const MtxF *mf, Mtx *ml);

void s16_matrix_rotate(Mtx *m, const s16 *);
void matrix_from_srt(MtxF *mf, const SRT *srt);
void matrix_prescale_y(MtxF *mf, f32 scale);
void matrix_trans_y(MtxF *mf, f32 scale);
void matrix_from_srt_reversed(MtxF *mf, const SRT *srt);
void matrix_from_srt_rotation(MtxF *mf, const SRT *srt);
void matrix_from_yaw(s16 theta, MtxF *mf);
void s16_vec3_apply_srt_rotation(const SRT *srt, s16 *vec);
void rotate_vec3(const SRT *srt, f32 vec[3]);
void rotate_vec_inv(const SRT *srt, Vec3f *vec);
void vec_from_rotation(const SRT *srt, Vec3f *vec);
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

// Returning s16 causes good compilers to optimise out parts of the resultant, causing bugs. They most likely intended u16.
#ifdef AVOID_UB
u16 arctan2s(s32 x, s32 z);
u16 arctan2_f(f32 x, f32 z);
u16 atan2f_to_s(f32 x, f32 z);
#else
s16 arctan2s(s32 x, s32 z);
s16 arctan2_f(f32 x, f32 z);
s16 atan2f_to_s(f32 x, f32 z);
#endif
f32 atan2f(f32 x, f32 z);

f32 acosf(f32);
f32 asinf(f32);
u16 acos(s16);
s16 asin(s16);

s32 floor_f(f32);
s32 ceil_f(f32);

f32 vec3_distance(const Vec3f *v1, const Vec3f *v2);
f32 vec3_distance_squared(const Vec3f *v1, const Vec3f *v2);
f32 vec3_distance_xz(const Vec3f *v1, const Vec3f *v2);
f32 vec3_distance_xz_squared(const Vec3f *v1, const Vec3f *v2);

f32 vec3_dot_product(const Vec3f *v1, const Vec3f *v2);
f32 vec3_length(const Vec3f *v);
f32 vec3_length_squared(const Vec3f *v);
void vec3_cross_product(const Vec3f *v1, const Vec3f *v2, Vec3f *result);
void vec3_cross_product_2(const Vec3f *v1, const Vec3f *v2, Vec3f *result);
f32 vec3_normalize(Vec3f *v);
void vec3_sub(const Vec3f *v1, const Vec3f *v2, Vec3f *result);
void vec3_add_with_scale(const Vec3f *v1, const Vec3f *v2, f32 scale, Vec3f *result);
void vec3_reflect(const Vec3f *v1, const Vec3f *v2, Vec3f *result);

#endif //_SYS_MATH_H
