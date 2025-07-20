#ifndef _SYS_CAMERA_H
#define _SYS_CAMERA_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/math.h"

#define CAMERA_COUNT 12

typedef struct
{
/*0000*/    SRT srt;
/*0018*/    f32 unk_0x18;
/*001C*/    u32 unk_0x1c;
/*0020*/    f32 unk_0x20;
/*0024*/    f32 unk_0x24;
/*0028*/    f32 unk_0x28;
/*002C*/    f32 dty;
/*0030*/    f32 unk_0x30;
/*0034*/    f32 unk_0x34;
/*0038*/    f32 unk_0x38;
/*003C*/    f32 unk_0x3c;
/*0040*/    Object *object;
/*0044*/    f32 tx;
/*0048*/    f32 ty;
/*004C*/    f32 tz;
/*0050*/    s16 yaw;
/*0052*/    s16 pitch;
/*0054*/    s16 roll;
/*0056*/    u16 unk_0x56;
/*0058*/    u16 unk_0x58;
/*005A*/    s16 dpitch;
/*005C*/    s8 unk_0x5c;
/*005D*/    s8 unk_0x5d;
} Camera;

typedef union
{
    Mtx m;
    MtxF mf;
} Mtx_MtxF;

typedef struct
{
/*0000*/    Mtx_MtxF *mtx;
/*0004*/    s16 count;
/*0006*/    u8 _pad[2];
} MatrixSlot;

typedef struct
{
/*0000*/    s32 x1;
/*0004*/    s32 y1;
/*0008*/    s32 x2;
/*000C*/    s32 y2;
/*0010*/    s32 posX;
/*0014*/    s32 posY;
/*0018*/    s32 width;
/*001C*/    s32 height;
/*0020*/    s32 ulx;
/*0024*/    s32 uly;
/*0028*/    s32 lrx;
/*002C*/    s32 lry;
/*0030*/    u32 flags;
} Viewport;


void camera_init();
void func_800013BC();
void func_800013D0();
u32 func_800013E0();
f32 camera_get_fov();
void camera_set_fov(f32 fovY);
void camera_set_ortho_projection_matrix(f32 l, f32 r, f32 b, f32 t);
void camera_set_aspect(f32 aspect);
void camera_set_near_plane(f32 near);
void func_80001660(f32 farPlane, s32 param2);
f32 camera_get_far_plane();
void func_800016B8();
void func_800016E0();
void func_80001708();
MtxF *camera_get_aux_mtx();
u32 func_80001788();
u32 get_camera_selector();
void func_800017A8(Gfx **gdl, Mtx **rspMtxs);
f32 func_80001884(f32 x, f32 y, f32 z);
void func_80001914(s32 tx, s32 ty, s32 tz, s32 roll, s32 pitch, s32 yaw);
void func_800019BC(s32 selector, f32 x, f32 y, f32 z, s16 yaw, s16 pitch, s16 roll);
s8 func_80001A2C();
void func_80001A3C();
s32 func_80001A5C(s32 num);
void set_camera_selector(s32 selector);
void func_80001B40();
void func_80001CE0(s32 selector, u32 param_2);
void func_80001D58(s32 selector, u32 param_2);
u32 func_80001DD4(s32 selector);
void func_80001E04(s32 viewPortIndex, s32 x1, s32 y1, s32 x2, s32 y2);
void func_80001F70(s32 viewPortIndex, s32 posX, s32 posY, s32 width, s32 height);
u32 func_8000206C(s32 selector, s32 *ulx, s32 *uly, s32 *lrx, s32 *lry);
void func_800020E4(s32 selector, s32 *x1, s32 *y1, s32 *x2, s32 *y2);
void func_80002130(s32 *ulx, s32 *uly, s32 *lrx, s32 *lry);
void func_800021A0(Gfx **gdl, Mtx **rspMtxs);
void func_80002490(Gfx **gdl);
void setup_rsp_camera_matrices(Gfx **gdl, Mtx **rspMtxs);
void func_800029C4(f32 param1);
void func_800029D4(Gfx **gdl, Mtx **rspMtxs);
void func_80002B2C(Gfx **gdl, Mtx **rspMtxs);
void func_80002C0C(Gfx **gdl, s32 scaleX, s32 scaleY, s32 transX, s32 transY);
void func_80002D14(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
void func_80002DE4(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
void func_80002E94(f32 x, f32 y, f32 z, s32 *ox, s32 *oy, s32 *oz);
void func_80002F88(s32 x, s32 y, s32 z, f32 *ox, f32 *oy, f32 *oz);
void func_8000302C(Gfx **gdl);
void func_80003168(Gfx **gdl, Mtx **rspMtxs, s32 x, s32 y, s32 z, f32 scale);
void func_800032C4(Gfx **gdl, Mtx **rspMtxs, SRT *param3, f32 param4, f32 param5, MtxF *param6);
void func_800034CC(f32 x, f32 y, f32 z);
void func_8000356C(f32 a, f32 b, f32 c);
void get_vec3_to_camera_normalized(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
void rotate_camera(s32 yaw, s32 pitch, s32 roll);
Camera *get_main_camera();
Camera *get_camera();
Camera *get_camera_array();
MtxF *camera_get_view_mtx2();
MtxF *camera_get_viewproj_mtx();
MtxF *camera_get_projection_mtx();
Mtx *camera_get_rsp_projection_mtx();
MtxF *camera_get_view_mtx();
u32 func_800038DC(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, u8 param_6);
f32 camera_get_angle_to_point(f32, f32, f32);
void func_80003AA0(f32 x, f32 y, f32 z, f32 distance, f32 param_5);
void func_80003B70(f32 dty);
void func_80003bb0(f32 param_1, f32 param_2, f32 param_3);
void add_matrix_to_pool(MtxF *mf, s32 count);
void camera_tick();
void setup_rsp_matrices_for_object(Gfx **gdl, Mtx **rspMtxs, Object *object);
void func_80004224(Gfx **gdl);
s32 func_80004258(Object *object);
void func_800042A8(Object *object);
void func_800042C8(Object *object, int matrixIdx);
void get_object_child_position(Object *object, float *ox, float *oy, float *oz);
void transform_point_by_object(float x, float y, float z, float *ox, float *oy, float *oz, Object *object);
void inverse_transform_point_by_object(float x, float y, float z, float *ox, float *oy, float *oz, Object *object);
void inverse_rotate_point_by_object(float x, float y, float z, float *ox, float *oy, float * oz, Object *object);
void rotate_point_by_object(float x, float y, float z, float *ox, float *oy, float *oz, Object *object);
void transform_srt_by_object(SRT *srt, SRT *out, Object *object);
void func_800047C8(SRT *a, SRT *b, SRT *out);
void update_camera_for_object(Camera *camera);
void transform_point_by_object_matrix(Vec3f *v, Vec3f *ov, s8 matrixIdx);
void func_80004A30(s16 param1);
s16 func_80004A4C(void);

#endif
