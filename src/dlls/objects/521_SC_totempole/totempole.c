#include "common.h"

#include "dlls/objects/519_SC_levelcontrol.h"

typedef struct {
    ObjSetup base;
    s16 unused18;
    u8 yaw;
    u8 modelIndex;          //Which model to use: square/diamond/circle/triangle
    f32 vocalsDistance;     //Music track's vocals added/removed based on this radius (stored divided by 400)
} SCTotemPole_Setup;

typedef struct {
    u16 playerDistance;     //Player distance on current tick
    u16 prevPlayerDistance; //Player distance on previous tick
    u16 vocalsDistance;     //Music track's vocals added/removed based on this radius
    u32 soundHandle;        //Sound handle for the spinning pole's wooden creaking loop
    Object* levelControl;   //A pointer to the `SC_levelcontrol` Object
} SCTotemPole_Data;

typedef enum {
    SCTotemPole_ModelIdx_Diamond = 0,
    SCTotemPole_ModelIdx_Circle = 1, //shorter, and without CloudRunner wings  
    SCTotemPole_ModelIdx_Square = 2,
    SCTotemPole_ModelIdx_Triangle = 3
} SCTotemPole_ModelIndices;

typedef enum {
    SCTotemPole_Square   = 0x30C5A, //at the start of the winding pathway around Rubble (spins in place)
    SCTotemPole_Diamond  = 0x30C5C, //close to Discovery Falls' entrance pond
    SCTotemPole_Circle   = 0x31A35, //on the serpent-head island at the end of the winding pathway
    SCTotemPole_Triangle = 0x31A3A //on pond island inside LightFoot Village
} SCTotemPole_UIDs;

typedef enum {
    MUSICACTION_12_SwapStone_Circle_Day = 0x12,
    MUSICACTION_B4_SwapStone_Circle_Vocals = 0xB4,
    MUSICACTION_EC_SwapStone_Circle_Night = 0xEC
} SCTotemPole_MusicActions; //TO-DO: move into main MusicActions enum?

static Object* SCTotemPole_find_level_control(Object* self);

// offset: 0x0 | ctor
void SCTotemPole_ctor(void *dll) { }

// offset: 0xC | dtor
void SCTotemPole_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SCTotemPole_setup(Object* self, SCTotemPole_Setup* objSetup, s32 arg2) {
    SCTotemPole_Data* objData = self->data;
    
    objData->vocalsDistance = (s16)(objSetup->vocalsDistance * 400.0f);
    self->srt.yaw = objSetup->yaw << 8;

    self->modelInstIdx = objSetup->modelIndex;
    if ((self->modelInstIdx < 0) || (self->modelInstIdx >= 4)) {
        self->modelInstIdx = 0;
    }
    
    objData->prevPlayerDistance = objData->playerDistance = 0xFFFF;
    objData->soundHandle = 0;
    objData->levelControl = NULL;
}

// offset: 0x94 | func: 1 | export: 1
void SCTotemPole_control(Object* self) {
    SCTotemPole_Data* objData;
    SCTotemPole_Setup* objSetup;
    s32 hitSphere;
    s32 hitDamage;
    Object* player;
    Object* hitBy;
    f32 dx;
    f32 dz;
    f32 time;

    objData = self->data;
    objSetup = (SCTotemPole_Setup*)self->setup;

    //Find SCLevelControl
    if (objData->levelControl == NULL) {
        objData->levelControl = SCTotemPole_find_level_control(self);
    }
    
    //Get lateral player distance
    player = get_player();
    dx = self->srt.transl.x - player->srt.transl.x;
    dz = self->srt.transl.z - player->srt.transl.z;
    objData->playerDistance = sqrtf(SQ(dx) + SQ(dz));
    
    if (objData->playerDistance < objData->vocalsDistance) {
        //Add music track's vocals as player approaches
        if (objData->prevPlayerDistance >= objData->vocalsDistance) {
            if (((DLL_519_SC_Levelcontrol*)objData->levelControl->dll)->vtbl->func8(objData->levelControl) == 0) {
                gDLL_5_AMSEQ2->vtbl->set(self, MUSICACTION_B4_SwapStone_Circle_Vocals, 0, 0, 0);
            }
            
            //Start a wooden creaking loop when near the spinning pole
            if (objSetup->base.uID == SCTotemPole_Square) {
                objData->soundHandle = gDLL_6_AMSFX->vtbl->play(self, SOUND_786_Wooden_Ratcheting_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
        }
    } else if (objData->prevPlayerDistance < objData->vocalsDistance) {
        //Remove music track's vocals as player leaves
        if (((DLL_519_SC_Levelcontrol*)objData->levelControl->dll)->vtbl->func8(objData->levelControl) == 0) {
            //Swap back to instrumental music arrangement (different instruments depending on time of day)
            if (gDLL_7_Newday->vtbl->func8(&time)) {
                gDLL_5_AMSEQ2->vtbl->set(self, MUSICACTION_EC_SwapStone_Circle_Night, 0, 0, 0);
            } else {
                gDLL_5_AMSEQ2->vtbl->set(self, MUSICACTION_12_SwapStone_Circle_Day, 0, 0, 0);
            }
        }
        
        //Stop the spinning pole's creaking wood loop
        if (objData->soundHandle != 0) {
            gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
            objData->soundHandle = 0;
        }
    }
    
    //Special behaviour for the Square-block pole (positioned at the start of the winding path around Rubble)
    if (objSetup->base.uID == SCTotemPole_Square) {
        //Check for attack collisions, and raise the platform to the serpent-head island
        if (func_80025F40(self, &hitBy, &hitSphere, &hitDamage) != 0) {
            ((DLL_519_SC_Levelcontrol*)objData->levelControl->dll)->vtbl->func7(objData->levelControl, 1);
        }

        //Spin the pole
        self->srt.yaw += 0xB6;
    }
    
    objData->prevPlayerDistance = objData->playerDistance;
}

// offset: 0x3DC | func: 2 | export: 2
void SCTotemPole_update(Object *self) { }

// offset: 0x3E8 | func: 3 | export: 3
void SCTotemPole_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x43C | func: 4 | export: 4
void SCTotemPole_free(Object* self, s32 arg1) {
    SCTotemPole_Data* objData = self->data;
    u32 soundHandle = objData->soundHandle;

    if (soundHandle != 0) {
        gDLL_6_AMSFX->vtbl->stop(soundHandle);
        objData->soundHandle = 0;
    }
}

// offset: 0x4A4 | func: 5 | export: 5
u32 SCTotemPole_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x4B4 | func: 6 | export: 6
u32 SCTotemPole_get_data_size(Object *self, u32 a1) {
    return sizeof(SCTotemPole_Data);
}

// offset: 0x4C8 | func: 7
/**
  * Searches for and returns the `SC_levelcontrol` Object, if it's loaded.
  */
Object* SCTotemPole_find_level_control(Object* self) {
    Object** objects;
    s32 index;
    s32 count;

    for (objects = get_world_objects(&index, &count); index < count; index++) {
        if ((self != objects[index]) && (objects[index]->id == OBJ_SC_levelcontrol)) {
            return objects[index];
        }
    }

    return NULL;
}
