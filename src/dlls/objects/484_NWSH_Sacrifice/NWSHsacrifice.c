#include "game/objects/setups/51_Generic.h"
#include "common.h"

typedef struct {
    f32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
} NWSH_Sacrifice_Data;

/*0x0*/ static u32 data_0 = 0;

// offset: 0x0 | ctor
void NWSH_Sacrifice_ctor(void* dll) { }

// offset: 0xC | dtor
void NWSH_Sacrifice_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void NWSH_Sacrifice_obj_Setup(Object* self, NWSH_Sacrifice_Setup* objSetup, s32 reset) {
    NWSH_Sacrifice_Data* objData = self->data;
    
    self->srt.yaw = 0;
    objData->unk10 = 10;
    
    if (objSetup->unk1A > 0) {
        objData->unk10 = objSetup->unk1A >> 8;
    }
    
    objData->unk0 = objSetup->unk18 * 4;
    objData->unk4 = objSetup->unk19;
    objData->unk8 = objSetup->unk1C;
    self->srt.scale = 0.1f;
    objData->unkC = objSetup->unk1E;
    objData->unkE = 0;
    
    mainSetBits(BIT_1CC, FALSE);
}

// offset: 0xC4 | func: 1 | export: 1
void NWSH_Sacrifice_obj_Control(Object* self) {
    NWSH_Sacrifice_Data* objData;
    Object* player;

    objData = self->data;
    player = objGetPlayer();
    if (player == NULL) {
        return;
    } 
    
    if ((vec3Distance(&self->globalPosition, &player->globalPosition) < objData->unk10) && mainGetBits(BIT_1CB)) {
        mainSetBits(BIT_1CC, TRUE);
    }
}

// offset: 0x178 | func: 2 | export: 2
void NWSH_Sacrifice_obj_Update(Object* self) { }

// offset: 0x184 | func: 3 | export: 3
void NWSH_Sacrifice_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x19C | func: 4 | export: 4
void NWSH_Sacrifice_obj_Free(Object* self, s32 onlySelf) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x1E4 | func: 5 | export: 5
u32 NWSH_Sacrifice_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1F4 | func: 6 | export: 6
u32 NWSH_Sacrifice_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(NWSH_Sacrifice_Data);
}
