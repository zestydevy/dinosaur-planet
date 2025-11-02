#include "PR/gbi.h"
#include "PR/ultratypes.h"

#include "game/objects/object.h"
#include "types.h"
#include "sys/gfx/model.h"
#include "functions.h"
#include "dlls/objects/214_animobj.h"

static int dll_365_func_E4(Object *arg0, Object *arg1, AnimObj_Data *arg2, s8 arg3);

// offset: 0x0 | ctor
void dll_365_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_365_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_365_setup(Object *self, ObjSetup *setup, s32 arg2)
{
	self->srt.yaw = 0;
	self->animCallback = dll_365_func_E4;
}

// offset: 0x44 | func: 1 | export: 1
void dll_365_control(Object *self) { }

// offset: 0x50 | func: 2 | export: 2
void dll_365_update(Object *self) { }

// offset: 0x5C | func: 3 | export: 3
void dll_365_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility)
{
	if (visibility != 0) {
		draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
	}
}

// offset: 0xB0 | func: 4 | export: 4
void dll_365_free(Object *self, s32 arg1) { }

// offset: 0xC0 | func: 5 | export: 5
u32 dll_365_get_model_flags(void* arg)
{
	return MODFLAGS_NONE;
}

// offset: 0xD0 | func: 6 | export: 6
u32 dll_365_get_data_size(Object *self, u32 arg1)
{
	return 0;
}

static int dll_365_func_E4(Object *arg0, Object *arg1, AnimObj_Data *arg2, s8 arg3)
{
	arg2->unk62 = 0;
	return 0;
}
