#include "PR/gbi.h"
#include "PR/ultratypes.h"

#include "game/objects/object.h"
#include "types.h"
#include "sys/gfx/model.h"
#include "functions.h"
#include "dlls/objects/365.h"
#include "dlls/objects/214_animobj.h"

static int func_0007(Object *arg0, Object *arg1, AnimObj_Data *arg2, s8 arg3);

void init(void *dll) { }

void fini(void *dll) { }

void dll_365_setup(Object *self, ObjSetup *setup, s32 arg2)
{
	self->srt.yaw = 0;
	self->animCallback = func_0007;
}

void dll_365_control(Object *self)
{
}

void dll_365_update(Object *self)
{
}

void dll_365_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility)
{
	if (visibility != 0) {
		draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
	}
}

void dll_365_free(Object *self, s32 arg1)
{
}

u32 dll_365_get_model_flags(void* arg)
{
	return MODFLAGS_NONE;
}

u32 dll_365_get_data_size(Object *self, u32 arg1)
{
	return 0;
}

static int func_0007(Object *arg0, Object *arg1, AnimObj_Data *arg2, s8 arg3)
{
	arg2->unk62 = 0;
	return 0;
}
