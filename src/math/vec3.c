#include "common.h"

/**
 * Returns the dot product of v1 and v2.
 */
f32 vec3_dot_product(Vec3f *v1, Vec3f *v2) {
    return v2->z * v1->z + (v1->x * v2->x + v1->y * v2->y);
}

/**
 * Returns the length of v.
 */
f32 vec3_length(Vec3f *v) {
    return sqrtf(v->z * v->z + (v->x * v->x + v->y * v->y));
}

/**
 * Returns the length of v squared.
 */
f32 vec3_length_squared(Vec3f *v) {
    return v->z * v->z + (v->x * v->x + v->y * v->y);
}

/**
 * Computes the cross product of v1 and v2 and stores it in result.
 */ 
void vec3_cross_product(Vec3f *v1, Vec3f *v2, Vec3f *result) {
    result->x = v1->y * v2->z - v1->z * v2->y;
    result->y = v1->z * v2->x - v1->x * v2->z;
    result->z = v1->x * v2->y - v1->y * v2->x;
}

/**
 * Computes the cross product of v1 and v2 and stores it in result.
 */ 
void vec3_cross_product_2(Vec3f *v1, Vec3f *v2, Vec3f *result) {
    result->x = v1->y * v2->z - v1->z * v2->y;
    result->y = v1->z * v2->x - v1->x * v2->z;
    result->z = v1->x * v2->y - v1->y * v2->x;
}

/**
 * Normalizes the vector and returns its length.
 */
f32 vec3_normalize(Vec3f *v) {
    f32 length;
    f32 lengthInv;

    length = sqrtf(v->z * v->z + (v->x * v->x + v->y * v->y));

    if (length != 0.0f) {
        lengthInv = 1.0f / length;
        v->x = v->x * lengthInv;
        v->y = v->y * lengthInv;
        v->z = v->z * lengthInv;
    }

    return length;
}

/**
 * Subtracts v2 from v1 and stores the difference in result.
 */
void vec3_sub(Vec3f *v1, Vec3f *v2, Vec3f *result) {
    result->x = v1->x - v2->x;
    result->y = v1->y - v2->y;
    result->z = v1->z - v2->z;
}

/**
 * Adds v2 * scale to v1 and stores the sum in result.
 * 
 * result = v1 + (scale * v2)
 */
void vec3_add_with_scale(Vec3f *v1, Vec3f *v2, f32 scale, Vec3f *result) {
    result->x = v1->x + scale * v2->x;
    result->y = v1->y + scale * v2->y;
    result->z = v1->z + scale * v2->z;
}

/**
 * Calculates the reflection vector of v1 and v2 and stores it in result.
 * 
 * If the dot product of v1 and v2 is greater than 0, result will be set to v2.
 */
void vec3_reflect(Vec3f *v1, Vec3f *v2, Vec3f *result) {
    // Calculate the angle (dot product) between v1 and v2
    float dot = v2->z * v1->z + (v1->x * v2->x + v1->y * v2->y);

    if (dot > 0.0f) {
        result->x = v2->x;
        result->y = v2->y;
        result->z = v2->z;

        return;
    }

    // Calculate the reflection
    // result = -2 * dot * v1 + v2

    result->x = v1->x;

    dot = dot * -2.0f;

    result->y = v1->y;
    result->z = v1->z;

    result->x = result->x * dot;
    result->y = result->y * dot;
    result->z = result->z * dot;

    result->x = v2->x + result->x;
    result->y = v2->y + result->y;
    result->z = v2->z + result->z;
}
