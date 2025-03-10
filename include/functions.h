#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "game/actor/actor.h"

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
TActor * get_player(void);

s32 read_file_region(u32 id, void *dst, u32 offset, s32 size);
void queue_load_file_region_to_ptr(void **dest, s32 fileId, s32 offset, s32 length);

void dl_get_debug_info2(Gfx*, u32*, char**, u32*, s32*, u32*, char**, u32*, s32*);
void dl_segment(Gfx **gdl, u32 segment, void *base);

void stop_alSyn_thread();

void func_80060B94(Gfx**);

OSSched *get_ossched(void);

TActor **TActor_getter(s32 idx, s32 *count);

void transform_point_by_actor(f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz, TActor *actor);

void update_pi_manager_array(s32 index, s32 value);

void add_object_to_array(TActor *actor, s32 param2);

void write_c_file_label_pointers(char *cFileLabel, s32 a1);

void func_800267A4(TActor *actor);
TActor *func_8002394C();
s32 func_80023D30(TActor*,s32,f32,s32);
s32 func_80024108(TActor*,f32,f32,s32);
s32 func_80025F40(TActor*,s32,s32,s32);

u32 get_gplay_bitstring(s32 entry);
void set_gplay_bitstring(s32 entry, u32 value);

void draw_actor(TActor*,s32,s32,s32,s32,float);

void dl_apply_geometry_mode(Gfx **gdl);

u8 func_8001EBE0();
void func_800142A0(f32 arg0, f32 arg1, f32 arg2);
s32 func_8000F44C();
void set_menu_page(s32);
void func_8001442C();
s8 func_80048498();
void set_textures_on_gdl(Gfx **gdl, Texture *tex0, Texture *tex1, u32 flags, s32 level, u32 force, u32 setModes);

void func_80002130(s32 *ulx, s32 *uly, s32 *lrx, s32 *lry);
s32 func_80004A4C();
void dl_apply_combine(Gfx **gdl);
void dl_set_fill_color(Gfx **gdl, u32 color);
void func_80002490(Gfx **gdl);
void dl_apply_other_mode(Gfx **gdl);

#endif //_FUNCTIONS_H
