#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "PR/gbi.h"
#include "game/objects/object.h"
#include "segment_334F0.h"
#include "sys/camera.h"
#include "sys/scheduler.h"
#include "libc/stdarg.h"
#include "types.h"
#include "unktypes.h"

void func_800267A4(Object *obj);

void draw_object(Object *obj, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, f32 yPrescale);
void func_80036FBC(s16 arg0, s16 arg1, s16 arg2, u8 arg3);

void dl_apply_geometry_mode(Gfx **gdl);

u8 func_8001EBE0();

s8 map_get_layer();
void set_textures_on_gdl(Gfx **gdl, Texture *tex0, Texture *tex1, u32 flags, s32 level, u32 force, u32 setModes);

void dl_apply_combine(Gfx **gdl);
void dl_set_fill_color(Gfx **gdl, u32 color);
void dl_set_prim_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
void dl_apply_other_mode(Gfx **gdl);
void dl_set_env_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);

s16 map_get_map_id_from_xz_ws(f32 arg0, f32 arg1);

ModelInstance *model_load_create_instance(s32 id, u32 flags);
void destroy_model_instance(ModelInstance *modelInst);
u32 func_8004D8A4(Object *obj, u32 addr, s32);

u32 func_8002667C(Object *obj, u32 addr);
u32 func_80026BD8(Object *obj, u32 addr);

u32 func_80026A20(s32 objId, ModelInstance* modelInstance, ObjectHitInfo* objHitInfo, u32 arg3, Object* obj);


void func_80045F48(s32);
void func_8004D974(s32);
void texture_destroy(Texture *texture);

/** check if object talked to by player? */
s32 func_80032538(Object* arg0);
void func_8003273C(Object *obj);
s32 map_get_type();
void func_8004A67C();

void dl_set_all_dirty();
void func_8003DB5C();

void func_8003CD6C(s32);

void map_func_80046B58(f32 x, f32 y, f32 z);
s32 func_8004454C(f32 x, f32 y, f32 z);

void convert_mtxf_to_mtx_in_pool(MatrixSlot *);

void func_8005BBF0(u32 arg0);
void func_80052DC0(u32* arg0, u32 arg1, u32 arg2, s32 arg3);
void func_800529C0(u32* arg0, u32 arg1);

/**
 * If param1 isn't null, sets it to gPossiblyScreenWidthOrHeight.
 *
 * Returns gSomeVideoFlag.
 */
int func_8005BC38(s32*);

void map_func_80048054(s32 mapID, s32, f32 *, f32 *, f32 *, s8 *);
void map_func_800484A8();
void map_func_8004773C();

void func_80041C6C(s32);
void func_80041D20(s32);

void init_textures();
void init_maps();
void func_8001CD00();
void init_models();
void func_80053300();
void func_8004D470();
void func_8005C780();

void init_global_map();

void func_8003E9F0(Gfx **gdl, s32 updateRate);
s32 func_80041D5C();
u32 func_80041D74();
void func_80042174(s32);
void func_8004225C(Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, Vertex **vtxs2, Triangle **pols2);
void func_800591EC();
void map_update_streaming();
void map_func_80048034();
void map_func_800483BC(f32, f32, f32);

f32 _depth2Cents(u8 arg0);

void mp3_init(s32 arg0);
void mp3_play_file(s32 romAddr, s32 size);

ModelInstance* func_80017D2C(s32 arg0, s32 arg1);
Texture *func_8003E960(s32);

void func_8001EBD0(s32);
void func_80041CA8(s32 arg0);
void func_80041CE4(s32 arg0);
void func_80041E24(s32 arg0);
void map_increment_layer(void);
void map_decrement_layer(void);

/** Applies envFxAction */
s32 func_80000860(Object *arg0, Object *arg1, u16 arg2, s32 arg3);
s32 func_80000450(Object*,Object*,u16,s8,s32,s32);

void func_80036438(Object*);

s32 func_80031F6C(Object* obj, s32 attachIdx, f32* ox, f32* oy, f32* oz, s32 arg5);
void func_80059038(s32, Object*, s32);

s32 func_80057F1C(Object*, f32, f32, f32, Func_80057F1C_Struct***, s32, s32);
s32 func_80059C40(Vec3f*, Vec3f*, f32, s32, Func_80059C40_Struct*, Object*, s8, s8, u8, s8);

MtxF *func_80032170(Object* obj, s32 arg1);

s16 func_80000824(s32 arg0);

void func_80058680(Object*, f32, f32, f32, f32*, s32);
void func_80032C0C(Object* obj, Object* otherObj, HeadAnimation* arg2, s32 arg3);
s32 func_80056BCC(Vec3f*, Vec3f*, Vec3f*, Vec4f*, Vec3f*, f32);

#endif //_FUNCTIONS_H
