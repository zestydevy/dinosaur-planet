#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "game/objects/object.h"
#include "sys/scheduler.h"
#include "libc/stdarg.h"
#include "unktypes.h"

void bootproc(void);
void idle(void * arg);
void mainproc(void * arg);

void test_write(void);
void main_no_expPak(void);
void main_expPak(void);
void threadTimerTick(void);
void check_dongle(void);

void func_80017254(s32);
void func_800175D4(s32 a0);
void update_PlayerPosBuffer(void);

void queue_load_file_region_to_ptr(void **dest, s32 fileId, s32 offset, s32 length);

void dl_get_debug_info2(Gfx*, u32*, char**, u32*, s32*, u32*, char**, u32*, s32*);
void dl_segment(Gfx **gdl, u32 segment, void *base);

void stop_alSyn_thread();

OSSched *get_ossched(void);

void transform_point_by_object(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz, Object *obj);

void update_pi_manager_array(s32 index, s32 value);

void write_c_file_label_pointers(char *cFileLabel, s32 a1);

void func_800267A4(Object *obj);
s32 func_80023D30(Object*,s32,f32,s32);
s32 func_80024108(Object*,f32,f32,s32);
s32 func_80025F40(Object*,s32,s32,s32);

u32 get_gplay_bitstring(s32 entry);
void set_gplay_bitstring(s32 entry, u32 value);

void draw_object(Object*,s32,s32,s32,s32,float);

void dl_apply_geometry_mode(Gfx **gdl);

u8 func_8001EBE0();
void func_800142A0(f32 arg0, f32 arg1, f32 arg2);
void func_8001442C();
s8 func_80048498();
void set_textures_on_gdl(Gfx **gdl, Texture *tex0, Texture *tex1, u32 flags, s32 level, u32 force, u32 setModes);

void func_80002130(s32 *ulx, s32 *uly, s32 *lrx, s32 *lry);
s16 func_80004A4C();
void dl_apply_combine(Gfx **gdl);
void dl_set_fill_color(Gfx **gdl, u32 color);
void dl_set_prim_color(Gfx **gdl, u8 r, u8 g, u8 b, u8 a);
void func_80002490(Gfx **gdl);
void dl_apply_other_mode(Gfx **gdl);

void get_object_child_position(Object *obj, float *ox, float *oy, float *oz);

s16 map_get_map_id_from_xz_ws(f32 arg0, f32 arg1);

ModelInstance *model_load_create_instance(s32 id, u32 flags);
void destroy_model_instance(ModelInstance *modelInst);
u32 func_8004D8A4(Object *obj, u32 addr, s32);

u32 func_8002667C(Object *obj, u32 addr);
u32 func_80026BD8(Object *obj, u32 addr);

u32 func_80026A20(s16 objId, ModelInstance*, ObjectHitInfo*, u32, Object*);

f32 func_80003A60(f32, f32, f32);


void func_80045F48(s32);
void func_8004D974(s32);
void texture_destroy(Texture *texture);

void func_8003273C(Object *obj);
s32 func_80048024();
void func_8004A67C();

void dl_set_all_dirty();
void func_8003DB5C();

void draw_pause_screen_freeze_frame(Gfx** gdl);

void func_8000E7D8(s32);
s8 func_8001441C(void);

void func_8000DAA4(UNK_TYPE_32, UNK_TYPE_32, UNK_TYPE_32, UNK_TYPE_32, UNK_TYPE_32);
void func_8000DC0C(UNK_TYPE_32, UNK_TYPE_32, UNK_TYPE_32, UNK_TYPE_32, UNK_TYPE_32);
void func_8000DB50(UNK_TYPE_32, UNK_TYPE_32);
void func_8000E49C(UNK_TYPE_32, UNK_TYPE_32, UNK_TYPE_32, char *, UNK_TYPE_32, UNK_TYPE_32);
void func_8000EF18(Gfx **gdl, UNK_TYPE_32, UNK_TYPE_32, UNK_TYPE_32);

void func_8000CB00(UNK_TYPE_32);

void func_80014B1C(void);
void func_80014B6C(void);
void func_800141A4(s32, s32, s32, s32);

void func_80037A14(Gfx**, UNK_PTR **, s32);

UNK_TYPE_32 func_8000E9D8(UNK_TYPE_32);
void func_8000DC54(UNK_TYPE_32, UNK_TYPE_32, UNK_TYPE_32, UNK_TYPE_32, UNK_TYPE_32, UNK_TYPE_32);
void func_8003825C(Gfx **gdl, Texture*, UNK_TYPE_32, UNK_TYPE_32, UNK_TYPE_32, UNK_TYPE_32, s32, UNK_TYPE_32);
void func_8000EA04(UNK_TYPE_32);

void func_800379D0(u32 red, u32 green, s32 blue);

#endif //_FUNCTIONS_H
