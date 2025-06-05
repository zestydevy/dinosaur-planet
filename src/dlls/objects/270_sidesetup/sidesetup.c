#include "PR/ultratypes.h"
#include "dll.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "variables.h"
#include "functions.h"
#include "types.h"

typedef struct {
/*00*/	ObjCreateInfo base;
/*18*/	union Unk18{
        u8 byte[2];
        s16 halfword;
    } Unk18;
/*1A*/	u8 unk1A;
} SideSetupObjCreateInfo;

void sidesetup_ctor(void *dll) {
}

void sidesetup_dtor(void *dll) {
}

void sidesetup_create(Object *self, SideSetupObjCreateInfo *createInfo, s32 arg2) {
    createInfo->unk1A = createInfo->Unk18.byte[0] - 1;
    createInfo->Unk18.halfword = -1;
}

void sidesetup_update(Object *self) {
}

void sidesetup_func_48(Object *self) {
}

void sidesetup_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
}

void sidesetup_destroy(Object *self, s32 arg1) {
}

u32 sidesetup_func_7C(Object *self) {
    return 0;
}

u32 sidesetup_get_state_size(Object *self, u32 arg1) {
    return 0;
}
