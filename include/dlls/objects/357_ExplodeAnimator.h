#ifndef _DLLS_357_H
#define _DLLS_357_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
    s8 unused0;
    s8 unused1;
    u8 flags;
    s8 unused3;
} ExplodeAnimator_Data;

typedef enum {
    ExplodeAnimator_FLAG_Waiting_to_Explode = 0,
    ExplodeAnimator_FLAG_Finished = 1
} ExplodeAnimator_Flags;

#endif // _DLLS_357_H

