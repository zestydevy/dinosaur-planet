#ifndef _SYS_DL_DEBUG_H
#define _SYS_DL_DEBUG_H

#include "PR/ultratypes.h"
#include "PR/gbi.h"

void dl_init_debug_infos();
void dl_next_debug_info_set();
void dl_add_debug_info(Gfx *gdl, u32 param_2, char *file, u32 param_4);

#endif
