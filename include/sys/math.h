/** Math and trigonometry
 */
#ifndef _SYS_MATH_H
#define _SYS_MATH_H

#include "PR/gbi.h"

#define M_PI_F          3.141592741f
#define M_INFINITY_F    3.4028235e38f   //largest positive number that can be represented by a float
#define ONE_OVER_SIXTY_F 0.01666666f

#define DEG_TO_RAD(deg) ((deg) * M_PI_F / 180.0f)
#define SQ(a) ((a) * (a))

#define M_5_DEGREES   0x38E     // 5 degrees in the game's angle system
#define M_20_DEGREES  0xE38     // 20 degrees in the game's angle system
#define M_45_DEGREES  0x2000    // 45 degrees in the game's angle system
#define M_90_DEGREES  0x4000    // 90 degrees in the game's angle system
#define M_180_DEGREES 0x8000    //180 degrees in the game's angle system
#define M_360_DEGREES 0x10000   //360 degrees in the game's angle system

#define M_45_DEGREES_F 8192.0f   //45 degrees in the game's angle system
#define M_90_DEGREES_F 16384.0f  //90 degrees in the game's angle system
#define M_180_DEGREES_F 32768.0f //180 degrees in the game's angle system
#define M_360_DEGREES_F 65535.0f //360 degrees in the game's angle system

/**
 * Keeps the value within the range.
 */
#define CLAMP(x, low, high) {       \
    if ((x) > (high)) (x) = (high); \
    if ((x) < (low)) (x) = (low);   \
}
#define CLAMP_EXPR(x, low, high) ((x) < (low) ? (low) : ((x) > (high) ? (high) : (x)))

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
#ifdef AVOID_UB
// There are many cases where CIRCLE_WRAP is given a 16-bit value, 
// which causes incorrect behavior on other compilers.
#define CIRCLE_WRAP(x) { \
    s32 __angle = (s32)x; \
    WRAP(__angle, -0x8000, 0x8000) \
    x = __angle; \
}
#else
#define CIRCLE_WRAP(x) WRAP(x, -0x8000, 0x8000)
#endif

#define DOT_PRODUCT(vA, vB) ((vA.f[0] * vB.f[0]) + (vA.f[1] * vB.f[1]) + (vA.f[2] * vB.f[2]))

#define VECTOR_ADD(vInA, vInB, vOut) (vOut).f[0] = (vInA).f[0] + (vInB).f[0];\
    (vOut).f[1] = (vInA).f[1] + (vInB).f[1];\
    (vOut).f[2] = (vInA).f[2] + (vInB).f[2];

#define VECTOR_SUBTRACT(vInA, vInB, vOut) (vOut).f[0] = (vInA).f[0] - (vInB).f[0];\
    (vOut).f[1] = (vInA).f[1] - (vInB).f[1];\
    (vOut).f[2] = (vInA).f[2] - (vInB).f[2];

#define VECTOR_MULTIPLY(vInA, vInB, vOut) (vOut).f[0] = (vInA).f[0] * (vInB).f[0];\
    (vOut).f[1] = (vInA).f[1] * (vInB).f[1];\
    (vOut).f[2] = (vInA).f[2] * (vInB).f[2];

#define VECTOR_SCALE(vIn, factor) (vIn).f[0] *= (factor);\
    (vIn).f[1] *= (factor);\
    (vIn).f[2] *= (factor);

#define VECTOR_MAGNITUDE_SQUARED(v) (SQ((v).f[0]) + SQ((v).f[1]) + SQ((v).f[2]))

#define VECTOR_MAGNITUDE(v) sqrtf(VECTOR_MAGNITUDE_SQUARED(v))

#define VECTOR_MULTIPLY_BY_SCALAR(v, scalar) v.x *= scalar;\
    v.y *= scalar;\
    v.z *= scalar;
    
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

typedef union Vec2f {
    struct {
        f32 x;
        f32 y;
    };
    f32 f[2];
} Vec2f;

#define VEC2F(x, y) {{ x, y }}

typedef union Vec3f {
    struct {
        f32 x;
        f32 y;
        f32 z;
    };
    f32 f[3];
} Vec3f;

#define VEC3F(x, y, z) {{ x, y, z }}

typedef union Vec4f {
    struct {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };
    f32 f[4];
} Vec4f;

#define VEC4F(x, y, z, w) {{ x, y, z, w }}

typedef union {
    struct { s32 x, y, z; };
    s32 s[3];
} Vec3s32;

typedef f32 Vec2[2];
typedef f32 Vec3[3];
typedef f32 Vec4[4];
typedef union {
    struct { s16 x, y, z; };
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
/*0006*/    s16 flags; // meaning depends on context. for object SRTs, this is ObjectFlags
/*0008*/    f32 scale;
/*000C*/	Vec3f transl;
} SRT;

// official name: mathMtxXFMF
void mathMtxXFMF(const MtxF *mtx, float x, float y, float z, float *ox, float *oy, float *oz);
// official name: mathMtxFastXFMF
void mathMtxFastXFMF(const MtxF *mf, const Vec3f *v, Vec3f *ov);
// official name: mathMtxCatF
void mathMtxCatF(const MtxF *m1, const MtxF *m2, MtxF *output);
void mathMtxCat4x3F(const MtxF *m1, const MtxF *m2, MtxF *output);
void mathMtxCat3x3F(const MtxF *m1, const MtxF *m2, MtxF *output);
// official name: mathMtxF2L
void mathMtxF2L(const MtxF *mf, Mtx *ml);
void mathMtx4x3F2L(const MtxF *mf, Mtx *ml);
void mathMtxTransF2L(const MtxF *mf, Mtx *ml);

// official name: fastShortReflection
void fastShortReflection(s16 *v, const s16 *);
void mathYprXyzMtx(MtxF *mf, const SRT *srt);
// official name: mathSquashY
void mathSquashY(MtxF *mf, f32 scale);
// official name: mathTransY
void mathTransY(MtxF *mf, f32 scale);
// official name: mathRpyXyzMtx
void mathRpyXyzMtx(MtxF *mf, const SRT *srt);
/**
 * Does not work!
 * Will only set the final matrix row to [0, 0, 0, 1] and nothing else,
 * despite doing the initial calculations for the rotation part of the matrix.
 * Function name was inferred.
 */
void mathRpyMtx_BROKEN(MtxF *mf, const SRT *srt);
/**
 * Applies Y-axis rotation to an existing matrix.
 * 
 * mf[0][0] = (mf[0][0] * Cosf(theta)) - (mf[2][0] * Sinf(theta))
 * mf[0][1] = (mf[0][1] * Cosf(theta)) - (mf[2][1] * Sinf(theta))
 * mf[0][2] = (mf[0][2] * Cosf(theta)) - (mf[2][2] * Sinf(theta))
 * mf[2][0] = (mf[2][0] * Cosf(theta)) + (mf[0][0] * Sinf(theta))
 * mf[2][1] = (mf[2][1] * Cosf(theta)) + (mf[0][1] * Sinf(theta))
 * mf[2][2] = (mf[2][2] * Cosf(theta)) + (mf[0][2] * Sinf(theta))
 */
void mathMtxApplyYaw(s16 theta, MtxF *mf);
/** Apply rotation from an SRT to another YPR (yaw-pitch-roll) vector. */
void mathYprCat(const SRT *srt, s16 *vec);
/** 
 * Applies SRT rotation to a vector in the order: roll -> pitch -> yaw.
 * Official name: mathOneFloatRPY
 */
void mathRotateRPY(const SRT *srt, f32 vec[3]);
/** 
 * Applies SRT rotation to a vector in the order: yaw -> pitch -> roll.
 * Official name: mathOneFloatYPR
 */
void mathRotateYPR(const SRT *srt, Vec3f *vec);
/**
 * Applies SRT yaw and pitch to a vector in the order: pitch -> yaw.
 * Official name: mathOneFloatPY
 */
void mathRotatePY(const SRT *srt, Vec3f *vec);
/**
 * Creates a floating-point translation matrix.
 * Official name: mathTranslateMtx
 */
void mathTranslateMtx(MtxF *mf, f32 x, f32 y, f32 z);
/** Gets a pointer to a readonly identity fixed-point matrix. */
Mtx *mathIdentityMtxL(void);
/**
 * Creates a floating-point scale matrix.
 * Official name: mathScaleMtx
 */
void mathScaleMtx(MtxF *mf, f32 sx, f32 sy, f32 sz);

f32 mathSinfInterp(s16 theta);
f32 mathCosfInterp(s16 theta);

// theta: [-32768..32768) => [-pi..pi)
// returns: [-65536..65536] => [-1..1]
// official name: mathCosInterp
s32 mathCosInterp(s16 theta);

// theta: [-32768..32768) => [-pi..pi)
// returns: [-65536..65536] => [-1..1]
// official name: mathSinInterp
s32 mathSinInterp(s16 theta);

// theta: [-32768..32768) => [-pi..pi)
// returns: [-65536..65536] => [-1..1]
// official name: mathCos
s32 mathCos(s16 theta);

// theta: [-32768..32768) => [-pi..pi)
// returns: [-65536..65536] => [-1..1]
// official name: mathSin
s32 mathSin(s16 theta);

// official name: Cosf
f32 Cosf(s32 theta);
// official name: Sinf
f32 Sinf(s32 theta);

// official name: mathDiffAngle
s16 mathDiffAngle(s32, s32);
f32 splinePos(f32, f32, f32, f32, f32);

// Returning s16 causes good compilers to optimise out parts of the resultant, causing bugs. They most likely intended u16.
#ifdef AVOID_UB
u16 mathAtan2(s32 x, s32 z);
u16 mathAtan2f(f32 x, f32 z);
u16 Arctanf(f32 x, f32 z);
#else
s16 mathAtan2(s32 x, s32 z);
s16 mathAtan2f(f32 x, f32 z);
// official name: Arctanf
s16 Arctanf(f32 x, f32 z);
#endif
f32 atan2f(f32 x, f32 z);

f32 acosf(f32);
f32 asinf(f32);
u16 acoss(s16);
s16 asins(s16);

f32 sinf(f32);

s32 floorf(f32);
s32 ceilf(f32);

f32 vec3Distance(const Vec3f *v1, const Vec3f *v2);
f32 vec3DistanceSquared(const Vec3f *v1, const Vec3f *v2);
f32 vec3DistanceXZ(const Vec3f *v1, const Vec3f *v2);
f32 vec3DistanceXZSquared(const Vec3f *v1, const Vec3f *v2);

f32 vec3DotProduct(const Vec3f *v1, const Vec3f *v2);
f32 vec3Length(const Vec3f *v);
f32 vec3LengthSquared(const Vec3f *v);
void vec3CrossProduct(const Vec3f *v1, const Vec3f *v2, Vec3f *result);
void vec3CrossProduct2(const Vec3f *v1, const Vec3f *v2, Vec3f *result);
f32 vec3Normalize(Vec3f *v);
void vec3Sub(const Vec3f *v1, const Vec3f *v2, Vec3f *result);
void vec3AddWithScale(const Vec3f *v1, const Vec3f *v2, f32 scale, Vec3f *result);
void vec3Reflect(const Vec3f *v1, const Vec3f *v2, Vec3f *result);

#endif //_SYS_MATH_H
