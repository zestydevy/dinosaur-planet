#ifndef _SYS_DL_DEBUG_H
#define _SYS_DL_DEBUG_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"

void dl_init_debug_infos(void);
void dl_next_debug_info_set(void);
void dl_add_debug_info(Gfx *gdl, u32 objID, const char *file, u32 line);
void dl_get_debug_info2(Gfx* arg0, u32* arg1, const char** arg2, u32* arg3, Gfx** arg4, u32* arg5, const char** arg6, u32* arg7, Gfx** arg8);

#endif
