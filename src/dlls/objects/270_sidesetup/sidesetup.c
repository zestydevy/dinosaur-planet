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

void dll_270_ctor(s32 arg0) {
}

void dll_270_dtor(s32 arg0) {
}

void dll_270_func_18(s32 arg0, SideSetupObjCreateInfo* createInfo, s32 arg2) {
    createInfo->unk1A = createInfo->Unk18.byte[0] - 1;
    createInfo->Unk18.halfword = -1;
}

void dll_270_func_3C(s32 arg0) {
}

void dll_270_func_48(s32 arg0) {
}

void dll_270_func_54(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
}

void dll_270_func_6C(s32 arg0, s32 arg1) {
}

s32 dll_270_func_7C(s32 arg0) {
    return 0;
}

s32 dll_270_func_8C(s32 arg0, s32 arg1) {
    return 0;
}
