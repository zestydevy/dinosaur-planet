#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

void bootproc(void);
void idle(void * arg);
void mainproc(void * arg);

void test_write(void);
void main_no_expPak(void);
void main_expPak(void);
void threadTimerTick(void);
void check_dongle(void);

void func_80017254(u32 arg0);
void func_800175D4(s32 a0);
void update_PlayerPosBuffer(void);
TActor * get_player(void);

void replace_loaded_dll_list(DLLInst arg0[], s32 arg1);

s32 read_file_region(u32 id, void *dst, u32 offset, s32 size);

DLLFile * dll_load_from_tab(u16, u32 *);
void dll_relocate(DLLFile* dll);

#endif //_FUNCTIONS_H
