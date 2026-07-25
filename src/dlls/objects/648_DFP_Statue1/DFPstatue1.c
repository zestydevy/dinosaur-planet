#include "common.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    u16 unk1A;
    u16 unk1C;
    s16 unk1E;
    s16 unk20;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    s8 unk26;
    u8 unk27;
    s8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
} DFP_Statue1_Setup;

typedef struct {
    s16 unk0;
    s16 gamebit;
    u8 unk4;
    u8 unk5;
} DFP_Statue1_Data;

/*0x0*/ static s16 data_0 = 0;
/*0x4*/ static u8 data_4 = 0;

static void DFP_Statue1_tick(Object* self);
static int DFP_Statue1_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void DFP_Statue1_ctor(void* dll) { }

// offset: 0xC | dtor
void DFP_Statue1_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFP_Statue1_obj_Setup(Object* self, DFP_Statue1_Setup* objSetup, s32 reset) {
    DFP_Statue1_Data* objData;

    objData = self->data;
    self->srt.yaw = objSetup->unk18 << 8;
    self->animCallback = DFP_Statue1_animCallback;
    objData->unk5 = objSetup->unk19;
    objData->unk0 = objSetup->unk1E;
    objData->gamebit = objSetup->unk20;

    if (mainGetBits(objData->gamebit)) {
        objData->unk4 = 1;
    }
    
    self->stateFlags |= OBJSTATE_PRINT_DISABLED;
}

// offset: 0xB4 | func: 1 | export: 1
void DFP_Statue1_obj_Control(Object* self) {
    DFP_Statue1_tick(self);
}

// offset: 0xF0 | func: 2 | export: 2
void DFP_Statue1_obj_Update(Object* self) {
    if (data_0 == 4) {
        data_0 = 0;
    }
}

// offset: 0x124 | func: 3 | export: 3
void DFP_Statue1_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x13C | func: 4 | export: 4
void DFP_Statue1_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x14C | func: 5 | export: 5
u32 DFP_Statue1_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x15C | func: 6 | export: 6
u32 DFP_Statue1_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DFP_Statue1_Data);
}

// offset: 0x170 | func: 7
void DFP_Statue1_tick(Object* self) {
    DFP_Statue1_Data* objData = self->data;
    s16 gamebitValue = mainGetBits(objData->gamebit);
    
    if ((objData->unk4 == 0) && (gamebitValue != 0) && mainGetBits(BIT_671)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        objData->unk4 = 1;
    }
    
    if ((data_4 != 0) && (objData->unk4 != 0) && mainGetBits(BIT_671)) {
        mainSetBits(objData->gamebit, 0);
        gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
        objData->unk4 = 0;
        data_0--;
        if (data_0 == 0) {
            data_4 = 0;
        }
    }
}

// offset: 0x2C0 | func: 8
int DFP_Statue1_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    DFP_Statue1_Data* objData;
    s32 i;

    objData = self->data;
    animData->unk7A = -1;
    animData->unk62 = 0;

    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
        case 1:
            mainSetBits(objData->gamebit + 5, 1);
            break;
        case 2:
            mainSetBits(objData->gamebit + 5, 0);
            data_0++;
            if (data_0 == 4) {
                data_4 = 1;
            }
            break;
        }
        animData->messages[i] = 0;
    }
    
    return 0;
}
