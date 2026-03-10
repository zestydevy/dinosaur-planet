#include "common.h"

#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/638_DFPlevcontrol.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 rowIndex;
    s16 unused1A;
    s16 inverseScale;
    s16 unk1E;
    s16 gamebitLowered;
} DFPfloorbar_Setup;

typedef struct {
    s16 unk0;
    s16 gamebitLowered;
    u8 lowered;
    u8 rowIndex;
    u8 safeTileIndex;
} DFPfloorbar_Data;

/*0x0*/ static Object* dLevelControl = NULL;
/*0x4*/ static u8 dFloorTiles[9] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

static int DFPfloorbar_anim_callback(Object* self, Object* override, AnimObj_Data* aData, s8 arg3);

// offset: 0x0 | ctor
void DFPfloorbar_ctor(void *dll) { }

// offset: 0xC | dtor
void DFPfloorbar_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFPfloorbar_setup(Object* self, DFPfloorbar_Setup* objSetup, s32 arg2) {
    DFPfloorbar_Data* objData;

    objData = self->data;
    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = DFPfloorbar_anim_callback;
    objData->rowIndex = objSetup->rowIndex;
    objData->unk0 = objSetup->unk1E;
    objData->gamebitLowered = objSetup->gamebitLowered;

    if (objSetup->inverseScale != 0) {
        self->srt.scale = 1.0f / ((f32) objSetup->inverseScale / 1000.0f);
    }

    //Sink into ground
    if (main_get_bits(objData->gamebitLowered)) {
        objData->lowered = TRUE;
        self->srt.transl.y = objSetup->base.y - 3.2f;
    }
}

// offset: 0xF8 | func: 1 | export: 1
void DFPfloorbar_control(Object* self) {
    DFPfloorbar_Setup* objSetup;
    DFPfloorbar_Data* objData;
    Object* player;
    Object** objects;
    s32 start;
    s32 count;
    s32 index;
    f32 dx;
    f32 dy;
    f32 dz;
    s16 tileSteppedOnNum;
    u16 setupID;

    objSetup = (DFPfloorbar_Setup*)self->setup;
    objData = self->data;
    
    tileSteppedOnNum = -1;
    
    diPrintf("needbit %d\n", main_get_bits(objData->gamebitLowered));
    
    setupID = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);
    
    switch (setupID) {
    case 1:
        if (objData->rowIndex >= 3) {
            objSetup->base.y = 20.0f;
            self->srt.transl.y = 20.0f;
            return;
        }
        break;
    case 2:
        if (objData->rowIndex >= 6) {
            objSetup->base.y = 20.0f;
            self->srt.transl.y = 20.0f;
            return;
        }
        break;
    }
    

    if (main_get_bits(BIT_DFPT_Zapped_by_Floor_Tiles) || 
        main_get_bits(BIT_DFPT_Puzzle_Pad_Pressed)
    ) {
        objData->lowered = FALSE;
    }
    
    //Find DFPLevelControl
    if (dLevelControl == NULL) { 
        objects = get_world_objects(&start, &count);
        for (index = start; index < count; index++) {
            if (objects[index]->id == OBJ_DFP_LevelContro) {             
                dLevelControl = objects[index];
                
                //Index break
                index = count;
            }
        }
    
        if (dLevelControl == NULL) {
            return;
        }
    }

    //Get the index of the safe/unelectrified tile on this floorbar's row
    ((DLL_638_DFPLevcontrol*)dLevelControl->dll)->vtbl->export_safe_floor_tiles(
        dLevelControl, dFloorTiles);
    objData->safeTileIndex = dFloorTiles[objData->rowIndex];

    //Check if downward motion not yet complete
    if (objData->lowered && ((objSetup->base.y - 3.2f) < self->srt.transl.y)) {
        //Move down into ground
        self->srt.transl.y -= gUpdateRateF / 12.0f;
        if (self->srt.transl.y <= objSetup->base.y - 3.2f) {
            self->srt.transl.y = objSetup->base.y - 3.2f;
            return;
        }
    } else if (objData->safeTileIndex != 0) {
        if (objData->lowered == FALSE) {
            self->srt.transl.y = objSetup->base.y;
        }
        
        if (objData->lowered){
            return;
        }

        player = get_player();
        if (player == NULL){
            return;
        }
        
        //Return early if player too far above/below
        dy = self->srt.transl.y - player->srt.transl.y;
        if (dy < 0.0f) {
            dy *= -1.0f;
        }
        if (!(dy < 100.0f)) {
            return;
        }
        
        dx = player->srt.transl.x - (self->srt.transl.x - 100.0f);

        //Return early if player too far forward/backward
        dz = self->srt.transl.z - player->srt.transl.z;
        if (dz < 0.0f) {
            dz *= -1.0f;
        }
        if (!(dz < 12.0f)) {
            return;
        }

        //Figure out which tile the player stepped on
        if (dx >= 150.0f) {
            tileSteppedOnNum = 4;
        } else if (dx >= 100.0f) {
            tileSteppedOnNum = 3;
        } else if (dx >= 50.0f) {
            tileSteppedOnNum = 2;
        } else if (dx >= 0.0f) {
            tileSteppedOnNum = 1;
        }
        
        if (tileSteppedOnNum == objData->safeTileIndex) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_999_Mechanical_Ratcheting, MAX_VOLUME, 0, 0, 0, 0);
            objData->lowered = TRUE;
        } else {
            main_set_bits(BIT_DFPT_Zapped_by_Floor_Tiles, 1);
        }
    }
}

// offset: 0x560 | func: 2 | export: 2
void DFPfloorbar_update(Object* arg0) {
    if ((dLevelControl != NULL) && (dLevelControl->srt.flags & 0x40)) {
        dLevelControl = NULL;
    }
}

// offset: 0x5A0 | func: 3 | export: 3
void DFPfloorbar_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility){
    if (main_get_bits(BIT_DFPT_Puzzle_Pad_Pressed)) {
        main_set_bits(BIT_DFPT_Puzzle_Pad_Pressed, 0);
    }
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}


// offset: 0x640 | func: 4 | export: 4
void DFPfloorbar_free(Object* self, s32 arg1) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x688 | func: 5 | export: 5
u32 DFPfloorbar_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x698 | func: 6 | export: 6
u32 DFPfloorbar_get_data_size(Object *self, u32 a1) {
    return sizeof(DFPfloorbar_Data);
}

// offset: 0x6AC | func: 7
int DFPfloorbar_anim_callback(Object* self, Object* override, AnimObj_Data* aData, s8 arg3){
    return 0;
}
