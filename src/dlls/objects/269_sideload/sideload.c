#include "common.h"
#include "game/gamebits.h"

typedef struct {
    ObjSetup base;
    s16 gamebitUnlocked;    //GamebitID determining whether the sidekick is unlocked (i.e. Kyte rescued)
    u8 sidekickIndex;       //Whether this should load Tricky/Kyte (see `SideLoad_Indices`)
} SideLoad_Setup;

typedef struct {
    u8 loaded;              //Boolean: whether the sidekick has been loaded by the SideLoad object
} SideLoad_Data;

typedef enum {
    Sidekick_Index_Tricky,
    Sidekick_Index_Kyte
} SideLoad_Indices;

// offset: 0x0 | ctor
void sideload_ctor(void *dll) { }

// offset: 0xC | dtor
void sideload_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void sideload_setup(Object* self, SideLoad_Setup* objSetup, s32 reset) {
    SideLoad_Data* objData = self->data;
    
    objData->loaded = FALSE;
    
    self->stateFlags |= OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED;
    
    if ((objSetup->sidekickIndex == Sidekick_Index_Tricky) && (objSetup->gamebitUnlocked <= NO_GAMEBIT)) {
        objSetup->gamebitUnlocked = BIT_Tricky_Spawns;
    } else if (objSetup->gamebitUnlocked <= NO_GAMEBIT) {
        objSetup->gamebitUnlocked = BIT_CC_Rescued_Kyte;
    }
}

// offset: 0x70 | func: 1 | export: 1
void sideload_control(Object* self) {
    SideLoad_Data* objData = self->data;
    SideLoad_Setup* objSetup = (SideLoad_Setup*)self->setup;
    s16 dSidekickObjIDs[] = {OBJ_Tricky, OBJ_Kyte};
    s16 gamebit;
    
    if (get_player() && (get_sidekick() == FALSE) && (objData->loaded == FALSE) && 
        ((gamebit = objSetup->gamebitUnlocked, gamebit == NO_GAMEBIT) || main_get_bits(gamebit))
    ) {
        objData->loaded = TRUE;
        func_80023894(self, dSidekickObjIDs[objSetup->sidekickIndex]);
    }
}

// offset: 0x148 | func: 2 | export: 2
void sideload_update(Object *self) { }

// offset: 0x154 | func: 3 | export: 3
void sideload_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}


// offset: 0x1A8 | func: 4 | export: 4
void sideload_free(Object *self, s32 onlySelf) { }

// offset: 0x1B8 | func: 5 | export: 5
u32 sideload_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1C8 | func: 6 | export: 6
u32 sideload_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(SideLoad_Data);
}
