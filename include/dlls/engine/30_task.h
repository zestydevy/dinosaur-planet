#ifndef _DLLS_30_H
#define _DLLS_30_H

#include "PR/ultratypes.h"
#include "dll_def.h"

DLL_INTERFACE(DLL_30_task) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ void (*load_recently_completed)();
    /*1*/ void (*mark_task_completed)(u8 task);
    /*2*/ u8 (*get_num_recently_completed)();
    /*3*/ char *(*get_recently_completed_task_text)(u8 idx);
    /*4*/ char *(*get_completion_task_text)();
    /*5*/ s16 (*get_completion_percentage)();
};

#endif //_DLLS_30_H
