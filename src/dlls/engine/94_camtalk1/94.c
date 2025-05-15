#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "variables.h"
#include "functions.h"
#include "types.h"

typedef struct {
/*0000*/    SRT srt;
/*0018*/    f32 unk_0x18;
/*001C*/    u32 unk_0x1c;
/*0020*/    f32 unk_0x20;
/*0024*/    f32 unk_0x24;
/*0028*/    f32 unk_0x28;
/*002C*/    f32 dty;
/*0030*/    f32 unk_0x30;
/*0034*/    f32 unk_0x34;
/*0038*/    f32 unk_0x38;
/*003C*/    f32 unk_0x3c;
/*0040*/    Object *object;
/*0044*/    f32 tx;
/*0048*/    f32 ty;
/*004C*/    f32 tz;
/*0050*/    s16 yaw;
/*0052*/    s16 pitch;
/*0054*/    s16 roll;
/*0056*/    u16 unk_0x56;
/*0058*/    u16 unk_0x58;
/*005A*/    s16 dpitch;
/*005C*/    s8 unk_0x5c;
/*005D*/    s8 unk_0x5d;
/*005E*/    s8 unk_0x5e;
/*005F*/    s8 unk_0x5f;
/*0060*/    s32 unk60;
/*0064*/    s32 unk64;
/*0068*/    s32 unk68;
/*006C*/    s32 unk6C;
/*0070*/    s32 unk70;
/*0074*/    s32 unk74;
/*0078*/    s32 unk78;
/*007C*/    s32 unk7C;
/*0080*/    s32 unk80;
/*0084*/    s32 unk84;
/*0088*/    s32 unk88;
/*008C*/    s32 unk8C;
/*0090*/    s32 unk90;
/*0094*/    s32 unk94;
/*0098*/    s32 unk98;
/*009C*/    s32 unk9C;
/*00A0*/    Object* aimAtObject;
/*00A4*/    f32 unkA4; //x
/*00A8*/    f32 unkA8; //y
/*00AC*/    f32 unkAC; //z
/*00AC*/    f32 fov;
} TalkCamera;

void dll_94_ctor(s32 arg0) {
}

void dll_94_dtor(s32 arg0) {
}

void dll_94_func_18(TalkCamera* camA, s32 unused, TalkCamera* camB) {
    if (camB != NULL) {
        camA->srt.transl.x = camB->srt.transl.x;
        camA->srt.transl.y = camB->srt.transl.y;
        camA->srt.transl.z = camB->srt.transl.z;
        camA->srt.yaw = camB->srt.yaw;
        camA->srt.pitch = camB->srt.pitch;
        camA->srt.roll = camB->srt.roll;
        camA->fov = camB->fov;
    }
}

void dll_94_func_64(s32 arg0) {
}

void dll_94_func_70(s32 arg0) {
}

void dll_94_func_7C(s32 arg0, s32 arg1) {
}
