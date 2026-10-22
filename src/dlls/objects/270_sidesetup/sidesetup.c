#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "game/objects/objsetups.h"
#include "sys/gfx/model.h"
#include "types.h"

void sidesetup_ctor(void *dll) { }

void sidesetup_dtor(void *dll) { }

void sidesetup_setup(Object *self, SideSetupObj_Setup *objsetup, s32 arg2) {
    objsetup->unk1A = objsetup->unk18.byte[0] - 1;
    objsetup->unk18.halfword = -1;
}

void sidesetup_control(Object *self) { }

void sidesetup_update(Object *self) { }

void sidesetup_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

void sidesetup_free(Object *self, s32 arg1) { }

u32 sidesetup_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

u32 sidesetup_get_data_size(Object *self, u32 arg1) {
    return 0;
}
