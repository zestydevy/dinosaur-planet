#include "common.h"
#include "dlls/objects/332_FXEmit.h"
#include "game/gamebits.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
} DFP_RotatePuzzle_Setup;

typedef struct {
    f32 unk0;
    u32 unk4;
    s16 unk8;
    s16 unkA;
    u16 unkC;
    s8 unkE;
    s8 unkF;
    s32 unk10;
} DFP_RotatePuzzle_Data;

/*0x0*/ static s16 data_0[] = {
    0, 0, 0, 0
};

/*0x0*/ static Object* bss_0[4];

static int DFP_RotatePuzzle_func_784(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);
static s32 DFP_RotatePuzzle_func_7A0(Object* self, u8 index);

// offset: 0x0 | ctor
void DFP_RotatePuzzle_ctor(void* dll) { }

// offset: 0xC | dtor
void DFP_RotatePuzzle_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFP_RotatePuzzle_obj_Setup(Object* self, DFP_RotatePuzzle_Setup* objSetup, s32 reset) {
    DFP_RotatePuzzle_Data* objData;
    s32 i;

    objData = self->data;
    self->srt.yaw = objSetup->unk18 << 8;
    self->animCallback = DFP_RotatePuzzle_func_784;
    objData->unk0 = self->srt.transl.y;
    objData->unkF = objSetup->unk19;
    objData->unk8 = 0x681;
    objData->unkA = objSetup->unk20;
    objData->unkC = 1;
    
    data_0[0] = 0;
    data_0[1] = 0;
    data_0[2] = 0;
    data_0[3] = 0;
    
    if (mainGetBits(objData->unk8)) {
        objData->unkE = 1;
    } else {
        for (i = 0; i < 4; i++) {
            DFP_RotatePuzzle_func_7A0(self, i);
        }
    }
    
    self->stateFlags |= OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x114 | func: 1 | export: 1
void DFP_RotatePuzzle_obj_Control(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/647_DFP_RotatePuzzle/DFP_RotatePuzzle_obj_Control.s")

// offset: 0x670 | func: 2 | export: 2
void DFP_RotatePuzzle_obj_Update(Object* self) { }

// offset: 0x67C | func: 3 | export: 3
void DFP_RotatePuzzle_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x694 | func: 4 | export: 4
void DFP_RotatePuzzle_obj_Free(Object* self, s32 onlySelf) {
    DFP_RotatePuzzle_Data* objData;
    s16 i;

    objData = self->data;
    
    for (i = 0; i < 4; i++) {
        if ((bss_0[i] != NULL) && (onlySelf == FALSE)) {
            objFreeObject(bss_0[i]);
        }
    }
    
    if (objData->unk4 != 0) {
        gDLL_6_AMSFX->vtbl->Stop(objData->unk4);
        objData->unk4 = 0;
    }
}

// offset: 0x760 | func: 5 | export: 5
u32 DFP_RotatePuzzle_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x770 | func: 6 | export: 6
u32 DFP_RotatePuzzle_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DFP_RotatePuzzle_Data);
}

// offset: 0x784 | func: 7
int DFP_RotatePuzzle_func_784(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    return 0;
}

// offset: 0x7A0 | func: 8
s32 DFP_RotatePuzzle_func_7A0(Object* self, u8 index) {
    FXEmit_Setup* fxSetup;

    fxSetup = objAllocSetup(sizeof(FXEmit_Setup), OBJ_FXEmit);
    fxSetup->base.loadDistance = 0xFF;
    fxSetup->base.fadeDistance = 0xFF;
    fxSetup->base.loadFlags = 2;
    fxSetup->base.fadeFlags = 1;
    fxSetup->base.x = self->srt.transl.x;
    fxSetup->base.y = self->srt.transl.y;
    fxSetup->base.z = self->srt.transl.z;
    fxSetup->toggleGamebit = NO_GAMEBIT;
    fxSetup->disableGamebit = NO_GAMEBIT;
    fxSetup->yaw = 0;
    fxSetup->flagConfig = 1;
    fxSetup->pitch = 0;
    fxSetup->roll = 0;
    fxSetup->rollSpeed = 0;
    fxSetup->pitchSpeed = 0;
    fxSetup->yawSpeed = 0;
    fxSetup->activationRange = 0;
    fxSetup->bank = 1;
    fxSetup->indexInBank = 17;
    fxSetup->fxRate = 1;
    fxSetup->interval = 0xFF;
    bss_0[index] = objSetupObject(&fxSetup->base, 4 | 1, self->mapID, -1, self->parent);
    return 1;
}

// offset: 0x8A0 | func: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/647_DFP_RotatePuzzle/DFP_RotatePuzzle_func_8A0.s")
