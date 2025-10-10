#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "game/objects/object.h"

typedef struct {
/*000*/    s8 unk0[0x260 - 0];
/*260*/    Object* unk260; //WCLevelControl
/*264*/    f32 unk264;
/*268*/    f32 unk268;
/*26C*/    u32 unk26C;
/*270*/    s16 unk270;
/*272*/    s16 unk272;
/*274*/    u8 unk274;
/*275*/    u8 unk275;
/*276*/    u8 unk276;
/*277*/    u8 unk277;
} WCPushBlockState;

typedef struct {
/*00*/ ObjCreateInfo base;
/*18*/ u8 unk18;
/*19*/ s8 modelIndex;
/*1A*/ s16 unk1A;
} WCPushBlockCreateInfo;

// offset: 0x0 | ctor
void dll_782_ctor(void* dll){
}

// offset: 0xC | dtor
void dll_782_dtor(void* dll){
}

// offset: 0x18 | func: 0 | export: 0
void dll_782_func_18(Object* self, WCPushBlockCreateInfo* createInfo, s32 arg2) {
    WCPushBlockState* state = self->state;

    self->unk_0x36 = 0;
    self->modelInstIdx = createInfo->modelIndex;
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
    state->unk276 = createInfo->unk1A;
}

// offset: 0x58 | func: 1 | export: 1
// https://decomp.me/scratch/WLIsx
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/782_WCpushblock/dll_782_func_58.s")

// offset: 0xF38 | func: 2 | export: 2
void dll_782_func_F38(Object* self) {
}

// offset: 0xF44 | func: 3 | export: 3
void dll_782_func_F44(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xF98 | func: 4 | export: 4
void dll_782_func_F98(Object* self, s32 arg1) {
}

// offset: 0xFA8 | func: 5 | export: 5
u32 dll_782_get_model_flags(Object* self) {
    WCPushBlockCreateInfo* createInfo;
    s8 modelIndex;

    createInfo = (WCPushBlockCreateInfo*)self->createInfo;
    modelIndex = createInfo->modelIndex;
    if (modelIndex >= self->def->numModels) {
        modelIndex = 0;
    }
    return MODFLAGS_MODEL_INDEX(modelIndex) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0xFDC | func: 6 | export: 6
u32 dll_782_get_state_size(Object* self, s32 arg1) {
    return sizeof(WCPushBlockState);
}

// offset: 0xFF0 | func: 7
s32 dll_782_func_FF0(Object* self, WCPushBlockState* state, Object* player) {
    f32 positionX;
    f32 positionZ;
    f32 min;
    f32 max;
    f32* time;
    u8 isNighttime;

    isNighttime = gDLL_7_Newday->vtbl->func8((f32*)&time);

    if (self->modelInstIdx == 1) {
        //Sun block?
        if (isNighttime) {
            return 0;
        }
        ((DLL_Unknown*)(state->unk260)->dll)->vtbl->func[11].withThreeArgs(state->unk276, (s32)&state->unk270, (s32)&state->unk272);
        ((DLL_Unknown*)(state->unk260)->dll)->vtbl->func[7].withFiveArgs((s32)self, state->unk270, state->unk272, (s32)&positionX, (s32)&positionZ);
    } else {
        //Moon block?
        if (!isNighttime) {
            return 0;
        }

        ((DLL_Unknown*)(state->unk260)->dll)->vtbl->func[18].withThreeArgs(state->unk276, (s32)&state->unk270, (s32)&state->unk272);
        ((DLL_Unknown*)(state->unk260)->dll)->vtbl->func[14].withFiveArgs((s32)self, state->unk270, state->unk272, (s32)&positionX, (s32)&positionZ);
    }

    //Check if player out of range
    max = positionX + 56.0f;
    min = positionX - 56.0f;
    if (max < player->srt.transl.x || player->srt.transl.x < min) {
        return 1;
    }
    max = positionZ + 56.0f;
    min = positionZ - 56.0f;
    if (positionZ + 56.0f < player->srt.transl.z || player->srt.transl.z < positionZ - 56.0f) {
        return 1;
    }

    return 0;
}

