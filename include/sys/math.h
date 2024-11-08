/** Math and trigonometry
 */
#ifndef _SYS_MATH_H
#define _SYS_MATH_H

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

void matrix_from_srt(MtxF *mf, SRT *srt);

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

#endif //_SYS_MATH_H
