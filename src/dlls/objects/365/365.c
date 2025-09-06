#include "PR/ultratypes.h"

#include "game/objects/object.h"
#include "variables.h"
#include "functions.h"
#include "dlls/objects/365.h"

static s32 func_0007(Object *arg0, Object *arg1, TStruct365Func0006 *arg2, s32 arg3);

void init(void *dll) { }

void fini(void *dll) { }

void dll_365_create(Object *self, ObjCreateInfo *createInfo, s32 arg2)
{
	self->srt.yaw = 0;
	self->unk0xbc = (ObjectCallback)func_0007;
}

void dll_365_update(Object *self)
{
}

void func_0002(Object *self)
{
}

void dll_365_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility)
{
	if (visibility != 0) {
		draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
	}
}

void dll_365_destroy(Object *self, s32 arg1)
{
}

u32 func_0005(void* arg)
{
	return 0;
}

u32 dll_365_get_state_size(Object *self, u32 arg1)
{
	return 0;
}

static s32 func_0007(Object *arg0, Object *arg1, TStruct365Func0006 *arg2, s32 arg3)
{
	arg2->m0062 = 0;
	return 0;
}
