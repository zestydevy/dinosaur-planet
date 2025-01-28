#include <PR/ultratypes.h>

#include "game/actor/actor.h"
#include "variables.h"
#include "functions.h"
#include "dlls/objects/365.h"

static s32 func_0007(TActor *arg0, TActor *arg1, TStruct365Func0006 *arg2, s32 arg3);

void init(void* arg)
{
}

int fini(void* arg)
{
}

void func_0000(TActor *actor, s32 arg1, s32 arg2)
{
	actor->srt.yaw = 0;
	actor->unk0xbc = (TActorCallback *)func_0007;
}

void func_0001(void* arg)
{
}

void func_0002(void* arg)
{
}

void func_0003(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s8 arg5)
{
	if (arg5 != 0) {
		draw_actor(arg0, arg1, arg2, arg3, arg4, 1.0f);
	}
}

void func_0004(void* arg0, s32 arg1)
{
}

s32 func_0005(void* arg)
{
	return 0;
}

s32 func_0006(void* arg0, s32 arg1)
{
	return 0;
}

static s32 func_0007(TActor *arg0, TActor *arg1, TStruct365Func0006 *arg2, s32 arg3)
{
	arg2->m0062 = 0;
	return 0;
}