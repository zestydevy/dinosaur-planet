#include "common.h"
extern f32 _sqrtf(f32);

f32 vec3_dot(Vec3f *v1, Vec3f *v2) {
    return (v2->z * v1->z) + ((v1->x * v2->x) + (v1->y * v2->y));
}


f32 vec3_hypotenouse(Vec3f *v) {
    return _sqrtf((v->z * v->z) + ((v->x * v->x) + (v->y * v->y)));
}

f32 vec3_squared(Vec3f *v) {
    return (v->z * v->z) + ((v->x * v->x) + (v->y * v->y));
}

void func_80016620(Vec3f *v1, Vec3f *v2, Vec3f *dst) {
    dst->x = (v1->y * v2->z) - (v1->z * v2->y);
    dst->y = (v1->z * v2->x) - (v1->x * v2->z);
    dst->z = (v1->x * v2->y) - (v1->y * v2->x);
}

void func_80016694(Vec3f *v1, Vec3f *v2, Vec3f *dst) {
    dst->x = (v1->y * v2->z) - (v1->z * v2->y);
    dst->y = (v1->z * v2->x) - (v1->x * v2->z);
    dst->z = (v1->x * v2->y) - (v1->y * v2->x);
}

f32 possible_vec3_normalize(Vec3f *v) {
    f32 mag;

    mag = _sqrtf((v->z * v->z) + ((v->x * v->x) + (v->y * v->y)));
    if (mag != 0.0f) {
    	f32 mag_inverse = 1.0f / mag;
        v->x *= mag_inverse;
        v->y *= mag_inverse;
        v->z *= mag_inverse;
    }
    return mag;
}

void vec3_c_AminB(Vec3f *v1, Vec3f *v2, Vec3f *dst) {
    dst->x = v1->x - v2->x;
    dst->y = v1->y - v2->y;
    dst->z = v1->z - v2->z;
}

void vec3_AplusBtimesC(Vec3f *v1, Vec3f *v2, f32 C, Vec3f *dst) {
    dst->x = v1->x + (C * v2->x);
    dst->y = v1->y + (C * v2->y);
    dst->z = v1->z + (C * v2->z);
}

void func_80016820(Vec3f *v1, Vec3f *v2, Vec3f *dst) {
    f32 mag_square = (v2->z * v1->z) + ((v1->x * v2->x) + (v1->y * v2->y));

    if (mag_square > 0.0f) {
        dst->x = v2->x;
        dst->y = v2->y;
        dst->z = v2->z;
    } else {
	    mag_square = mag_square * (-2.0f);

	    dst->x = v1->x;
	    dst->y = v1->y;
	    dst->z = v1->z;
	    dst->x *= mag_square;
	    dst->y *= mag_square;
	    dst->z *= mag_square;
	    dst->x = v2->x + dst->x;
	    dst->y = v2->y + dst->y;
	    dst->z = v2->z + dst->z;
	}
}
