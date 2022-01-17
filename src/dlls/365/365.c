#include <PR/ultratypes.h>

typedef struct
{
	s16 m0000;
	u8 mPad[0xB8];
	void * mFunc;
} TStruct365Func0000;

typedef struct
{
	u8 mPad[0x62];
	s8 m0062;
} TStruct365Func0006;

void init(void* arg)
{
}

int fini(void* arg)
{
}

s32 func_0007(s32 arg0, s32 arg1, TStruct365Func0006 * arg2, s32 arg3);

void func_0000(TStruct365Func0000 * arg0, s32 arg1, s32 arg2)
{
	arg0->m0000 = 0;
	arg0->mFunc = (void *)func_0007;
}

void func_0001(void* arg)
{
}

void func_0002(void* arg)
{
}

extern void draw_actor(s32,s32,s32,s32,s32,float);
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

static s32 func_0007(s32 arg0, s32 arg1, TStruct365Func0006 * arg2, s32 arg3)
{
	arg2->m0062 = 0;
	return 0;
}