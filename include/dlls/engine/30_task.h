#ifndef _DLLS_30_H
#define _DLLS_30_H

#include <PR/ultratypes.h>
#include "dll_def.h"

DLL_INTERFACE_BEGIN(30_task)
    void (*load_recently_completed)();
    void (*mark_task_completed)(u8 task);
    u8 (*get_num_recently_completed)();
    void (*func_390)(u8 param1);
    void (*func_3F8)();
    s16 (*get_completion_percentage)();
DLL_INTERFACE_END()

#endif //_DLLS_30_H
