#include "common.h"
#include "game/objects/object.h"
#include "game/objects/setups/51_Generic.h"

typedef struct {
    s16 gamebit;
    s16 unk2;
} VFP_Statue_Data;

// offset: 0x0 | ctor
void VFP_Statue_ctor(void* dll) { }

// offset: 0xC | dtor
void VFP_Statue_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void VFP_Statue_obj_Setup(Object* self, Generic_Setup* objSetup, s32 reset) {
    VFP_Statue_Data* objData = self->data;
    
    self->srt.yaw = objSetup->yaw << 8;
    objData->gamebit = objSetup->bitA;
    self->stateFlags |= OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x48 | func: 1 | export: 1
void VFP_Statue_obj_Control(Object* self) {
    Generic_Setup* objSetup;
    VFP_Statue_Data* objData;

    objSetup = (Generic_Setup*)self->setup;
    objData = self->data;
    
    switch (self->id) {
    case OBJ_VFP_statuea:
        if (mainGetBits(objData->gamebit) && ((objSetup->base.x - 30.0f) < self->srt.transl.x)) {
            self->srt.transl.x -= 0.4f;
            if (self->srt.transl.x < objSetup->base.x - 30.0f) {
                self->srt.transl.x = objSetup->base.x - 30.0f;
            }
        } else if ((mainGetBits(objData->gamebit) == FALSE) && (self->srt.transl.x < objSetup->base.x)) {
            self->srt.transl.x += 0.2f;
            if (self->srt.transl.x > objSetup->base.x) {
                self->srt.transl.x = objSetup->base.x;
            }
        }
        /* fallthrough */
    case OBJ_VFP_statue:
    case OBJ_VFP_statueb:
        return;
    }
}

// offset: 0x190 | func: 2 | export: 2
void VFP_Statue_obj_Update(Object* self) { }

// offset: 0x19C | func: 3 | export: 3
void VFP_Statue_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x1B4 | func: 4 | export: 4
void VFP_Statue_obj_Free(Object* self, s32 onlySelf) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x1FC | func: 5 | export: 5
u32 VFP_Statue_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x20C | func: 6 | export: 6
u32 VFP_Statue_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(VFP_Statue_Data);
}
