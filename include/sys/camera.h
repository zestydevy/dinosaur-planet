#ifndef _SYS_CAMERA_H
#define _SYS_CAMERA_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/math.h"

#define CAMERA_COUNT 12

typedef struct {
/*0000*/    SRT srt;
/*0018*/    f32 fov;
/*001C*/    u32 unk1C;
/*0020*/    f32 velX;
/*0024*/    f32 velY;
/*0028*/    f32 velZ;
/*002C*/    f32 dty;           // vertical shake offset
/*0030*/    f32 shakeAmplitude;
/*0034*/    f32 shakeFrequency;
/*0038*/    f32 shakeTime;
/*003C*/    f32 shakeDamping;
/*0040*/    Object *object;    // attached object
/*0044*/    f32 tx;            // resolved world X
/*0048*/    f32 ty;            // resolved world Y
/*004C*/    f32 tz;            // resolved world Z
/*0050*/    s16 yaw;           // resolved yaw
/*0052*/    s16 pitch;         // resolved pitch
/*0054*/    s16 roll;          // resolved roll
/*0056*/    u16 mapRegion;
/*0058*/    u16 unk58;
/*005A*/    s16 dpitch;        // pitch delta (shake)
/*005C*/    s8 shakeCooldown;
/*005D*/    s8 shakeMode;      // 0 = bounce, 1 = dampened oscillation, -1 = inactive
} Camera;

typedef union
{
    Mtx m;
    MtxF mf;
} Mtx_MtxF;

typedef struct {
/*0000*/    Mtx_MtxF *mtx;
/*0004*/    s16 count;
/*0006*/    u8 _pad[2];
} MatrixSlot;

typedef struct {
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


void camera_init(void);
void camera_enable_y_offset(void);
void camera_disable_y_offset(void);
u32 camera_is_y_offset_enabled(void);
f32 camera_get_fov(void);
void camera_set_fov(f32 fovY);
void camera_set_ortho_projection_matrix(f32 l, f32 r, f32 b, f32 t);
void camera_set_aspect(f32 aspect);
void camera_set_near_plane(f32 near);
void camera_set_far_plane_lerp(f32 farPlane, s32 duration);
f32 camera_get_far_plane(void);
void camera_reset_near_plane(void);
void camera_reset_far_plane(void);
void camera_reset_projection(void);
MtxF *camera_get_aux_mtx(void);
u32 camera_get_viewport_mode(void);
u32 get_camera_selector(void);
void camera_setup_skybox(Gfx **gdl, Mtx **rspMtxs);
f32 camera_get_distance_to_point(f32 x, f32 y, f32 z);
void camera_reset(s32 tx, s32 ty, s32 tz, s32 roll, s32 pitch, s32 yaw);
void camera_set_alternate(s32 selector, f32 x, f32 y, f32 z, s16 yaw, s16 pitch, s16 roll);
s8 camera_is_alternate_active(void);
void camera_apply_alternate_trigger(void);
s32 camera_set_viewport_mode(s32 num);
void set_camera_selector(s32 selector);
void camera_update_rsp_viewports(void);
void viewport_enable(s32 selector, u32 immediate);
void viewport_disable(s32 selector, u32 immediate);
u32 viewport_is_active(s32 selector);
void viewport_set_scissor_rect(s32 viewPortIndex, s32 x1, s32 y1, s32 x2, s32 y2);
void viewport_set_custom_transform(s32 viewPortIndex, s32 posX, s32 posY, s32 width, s32 height);
u32 viewport_get_scissor_rect(s32 selector, s32 *ulx, s32 *uly, s32 *lrx, s32 *lry);
void viewport_get_rect(s32 selector, s32 *x1, s32 *y1, s32 *x2, s32 *y2);
void viewport_get_full_rect(s32 *ulx, s32 *uly, s32 *lrx, s32 *lry);
void camera_setup_viewport_and_matrices(Gfx **gdl, Mtx **rspMtxs);
void camera_apply_scissor(Gfx **gdl);
void setup_rsp_camera_matrices(Gfx **gdl, Mtx **rspMtxs);
void camera_set_2d_scale_y(f32 scaleY);
void camera_setup_2d_projection(Gfx **gdl, Mtx **rspMtxs);
void camera_setup_static_projection(Gfx **gdl, Mtx **rspMtxs);
void camera_apply_rsp_viewport(Gfx **gdl, s32 scaleX, s32 scaleY, s32 transX, s32 transY);
void camera_project_point(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
void camera_unproject_point(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
void camera_clip_to_screen(f32 x, f32 y, f32 z, s32 *ox, s32 *oy, s32 *oz);
void camera_screen_to_clip(s32 x, s32 y, s32 z, f32 *ox, f32 *oy, f32 *oz);
void camera_setup_fullscreen_viewport(Gfx **gdl);
void camera_setup_world_matrix(Gfx **gdl, Mtx **rspMtxs, s32 x, s32 y, s32 z, f32 scale);
u32 camera_check_convex_hull(u32 a0, u32 a1, u32 a2, u32 a3, s32 a4, s32 a5, s32 a6);
void camera_setup_object_srt_matrix(Gfx **gdl, Mtx **rspMtxs, SRT *srt, f32 yScale, f32 unused, MtxF *outMtx);
void camera_translate(f32 x, f32 y, f32 z);
void camera_translate_relative(f32 forward, f32 b, f32 strafe);
void get_vec3_to_camera_normalized(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
void rotate_camera(s32 yaw, s32 pitch, s32 roll);
Camera *get_main_camera(void);
Camera *get_camera(void);
Camera *get_camera_array(void);
MtxF *camera_get_view_mtx2(void);
MtxF *camera_get_viewmtx2(void);
MtxF *camera_get_viewproj_mtx(void);
MtxF *camera_get_projection_mtx(void);
Mtx *camera_get_rsp_projection_mtx(void);
MtxF *camera_get_view_mtx(void);
u32 camera_world_to_screen(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, u8 useViewMtx);
f32 camera_get_angle_to_point(f32, f32, f32);
void camera_apply_shake_radial(f32 x, f32 y, f32 z, f32 radius, f32 intensity);
void camera_set_shake_offset(f32 dty);
void camera_start_shake_dampened(f32 amplitude, f32 frequency, f32 damping);
void add_matrix_to_pool(MtxF *mf, s32 count);
void camera_tick(void);
void setup_rsp_matrices_for_object(Gfx **gdl, Mtx **rspMtxs, Object *object);
void camera_load_parent_projection(Gfx **gdl);
s32 camera_alloc_object_matrix(Object *object);
void camera_update_object_matrix(Object *object);
void camera_build_object_matrix(Object *object, int matrixIdx);
void get_object_child_position(Object *object, float *ox, float *oy, float *oz);
void transform_point_by_object(float x, float y, float z, float *ox, float *oy, float *oz, Object *object);
void inverse_transform_point_by_object(float x, float y, float z, float *ox, float *oy, float *oz, Object *object);
void inverse_rotate_point_by_object(float x, float y, float z, float *ox, float *oy, float * oz, Object *object);
void rotate_point_by_object(float x, float y, float z, float *ox, float *oy, float *oz, Object *object);
void transform_srt_by_object(SRT *srt, SRT *out, Object *object);
void camera_transform_srt_relative(SRT *a, SRT *b, SRT *out);
void update_camera_for_object(Camera *camera);
void transform_point_by_object_matrix(Vec3f *v, Vec3f *ov, s8 matrixIdx);
void camera_set_letterbox(s16 size);
s16 camera_get_letterbox(void);

#endif
