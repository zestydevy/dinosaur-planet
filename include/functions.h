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

void replace_loaded_dll_list(DLLInst arg0[], s32 arg1);

s32 read_file_region(u32 id, void *dst, u32 offset, s32 size);

DLLFile * dll_load_from_tab(u16 id, s32 * sizeOut);
void dll_relocate(DLLFile* dll);

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

#endif //_FUNCTIONS_H
