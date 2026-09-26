#include "common.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 index;
    s16 dropoffRange;
    s16 _unk1C;   //Gamebit field? Always -1
    s16 gamebit; //Unused, but this the same gamebit that the DLL sets upon placing this item
} GPSH_Scene_Setup;

typedef struct {
    f32 dropoffRange;   //Range for placing the item
    s16 gamebit;        //Unused, but this the same gamebit that the DLL sets upon placing this item
    u8 index;           //The scene's index, determining the item it requires and the gamebit to set when placed
} GPSH_Scene_Data;

typedef enum {
    GPSH_Scene_INDEX_SnowHorn_Wastes,
    GPSH_Scene_INDEX_SwapStone_Circle,
    GPSH_Scene_INDEX_Diamond_Bay,
    GPSH_Scene_INDEX_CloudRunner_Fortress,
    GPSH_Scene_INDEX_Moon_Mountain_Pass,
    GPSH_Scene_INDEX_DarkIce_Mines
} GPSH_Scene_Indices;

// offset: 0x0 | ctor
void GPSH_Scene_ctor(void* dll) { }

// offset: 0xC | dtor
void GPSH_Scene_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void GPSH_Scene_obj_Setup(Object* self, GPSH_Scene_Setup* objSetup, s32 reset) {
    GPSH_Scene_Data* objData = self->data;
    objData->gamebit = objSetup->gamebit;
    objData->index = objSetup->index;
    objData->dropoffRange = objSetup->dropoffRange / 100;
    self->srt.yaw = objSetup->yaw << 8;
    self->globalPosition.x = self->srt.transl.x;
    self->globalPosition.y = self->srt.transl.y;
    self->globalPosition.z = self->srt.transl.z;
}

// offset: 0x7C | func: 1 | export: 1
void GPSH_Scene_obj_Control(Object* self) {
    GPSH_Scene_Data* objData;
    Object* player;
    s32 i;
    f32 distance;
    Pickup* pickupData;
    Object* pickup;

    objData = self->data;
    distance = 10000.0f;
    self->globalPosition.x = self->srt.transl.x;
    self->globalPosition.y = self->srt.transl.y;
    self->globalPosition.z = self->srt.transl.z;
    
    player = objGetPlayer();
    pickup = objGetNearestTypeTo(OBJTYPE_Pickup, self, &distance);
    
    if (player == NULL) {
        return;
    }
    if (pickup != NULL) {
        pickupData = pickup->data;
    }
    if (pickup == NULL) {
        return;
    }

    i = objData->index;
    if ((OBJ_GPSHpickobjroot + i) != pickup->id) {
        return;
    }
    
    if (distance < objData->dropoffRange) {
        if (i == GPSH_Scene_INDEX_SnowHorn_Wastes)      { STUBBED_PRINTF("Put the Root here\n"); }
        if (i == GPSH_Scene_INDEX_SwapStone_Circle)     { STUBBED_PRINTF("Put the Nugget here\n"); }
        if (i == GPSH_Scene_INDEX_Diamond_Bay)          { STUBBED_PRINTF("Put the Egg here\n"); }
        if (i == GPSH_Scene_INDEX_CloudRunner_Fortress) { STUBBED_PRINTF("Put the Gem here\n"); }
        if (i == GPSH_Scene_INDEX_Moon_Mountain_Pass)   { STUBBED_PRINTF("Put the Barrel here\n"); }

        // This one's interesting, since there's no Plant in the test (aside from the aforementioned Root)! 
        // DIM uses the Horn of Truth in December 2000, albeit with an older design for its model.
        // Maybe an earlier version of this test (and by extension DIM itself) really did involve a plant?
        // Or maybe it could mean a different location was once represented, like Willow Grove.
        if (i == GPSH_Scene_INDEX_DarkIce_Mines)        { STUBBED_PRINTF("Put the Plant here\n"); } 

        if ((pickupData->state == PICKUP_NotHeld) && (mainGetBits(BIT_GPSH_Placed_SW_Scene_Root + objData->index) == FALSE)) {
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_424_Flame_Lighting, MAX_VOLUME, NULL, NULL, 0, NULL);
            mainSetBits(BIT_GPSH_Placed_SW_Scene_Root + objData->index, 1);
            objFreeObject(pickup);
            
            for (i = 0; i < 200; i++) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_289, NULL, 0, -1, NULL);
            }
        }
    }
}

// offset: 0x240 | func: 2 | export: 2
void GPSH_Scene_obj_Update(Object* self) { }

// offset: 0x24C | func: 3 | export: 3
void GPSH_Scene_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x264 | func: 4 | export: 4
void GPSH_Scene_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x274 | func: 5 | export: 5
u32 GPSH_Scene_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x284 | func: 6 | export: 6
u32 GPSH_Scene_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(GPSH_Scene_Data);
}
