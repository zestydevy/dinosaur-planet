typedef float Vec2[2];
typedef float Vec3[3];
typedef float Vec4[4];
typedef short Vec3s[3];

typedef f32 quartic[5];

typedef struct {
    f32 x, y, z;
} Vec3f;

//found a 3-array of these, not sure what they're for.
struct Vec3_Int{
	Vec3f f;
	u32 i; //seems to be a 32-bit bool.
};//used for camera?
