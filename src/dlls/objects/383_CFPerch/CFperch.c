#include "common.h"
#include "sys/objmsg.h"

static int CFPerch_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void CFPerch_ctor(void* dll) { }

// offset: 0xC | dtor
void CFPerch_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFPerch_obj_Setup(Object* self, ObjSetup* objSetup, s32 reset) {
    self->unkDC = 1;
    self->animCallback = CFPerch_animCallback;
}

// offset: 0x48 | func: 1 | export: 1
void CFPerch_obj_Control(Object* self) {
    if ((self->unkDC != 0) && (mainGetBits(BIT_CRF_CloudRunner_Uncle_Escaped) == FALSE)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
    }

    self->unkDC = 0;
}

// offset: 0xC8 | func: 2 | export: 2
void CFPerch_obj_Update(Object* self) { }

// offset: 0xD4 | func: 3 | export: 3
void CFPerch_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x128 | func: 4 | export: 4
void CFPerch_obj_Free(Object* self, s32 onlySelf) {
    objSendMesgMany(OBJ_EnergyGem4, 0, self, 0x40001, 0);
}

// offset: 0x178 | func: 5 | export: 5
u32 CFPerch_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x188 | func: 6 | export: 6
u32 CFPerch_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return 0;
}

// offset: 0x19C | func: 7
int CFPerch_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    if (mainGetBits(BIT_CRF_CloudRunner_Uncle_Freed)) {
        animData->unk9D = 4;
    }
    
    return 0;
}

