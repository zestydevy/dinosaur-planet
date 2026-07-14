#include "common.h"
#include "dlls/objects/304_LanternFireFly.h"
#include "game/objects/object_id.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
} FireFlyLantern_Setup;

typedef struct {
    Object* fireflies[20];
    u8 fireflyCount;
    u8 flagDone : 1;
    s16 gamebitFireflies;
} FireFlyLantern_Data;

static int FireFlyLantern_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackResult);
static Object* FireFlyLantern_create_firefly(Object* self);

// offset: 0x0 | ctor
void FireFlyLantern_ctor(void *dll) { }

// offset: 0xC | dtor
void FireFlyLantern_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void FireFlyLantern_setup(Object* self, FireFlyLantern_Setup* objSetup, s32 reset) {
    FireFlyLantern_Data* objData;
    s32 i;

    objData = self->data;
    self->animCallback = FireFlyLantern_anim_callback;
    
    //Store player's lantern gamebitID
    if (objGetPlayer()->id != OBJ_Sabre) {
        objData->gamebitFireflies = BIT_Krystal_Fireflies;
    } else {
        objData->gamebitFireflies = BIT_Sabre_Fireflies;
    }
    
    //Store player's firefly count
    objData->fireflyCount = mainGetBits(objData->gamebitFireflies);

    //Create as many fireflies as the player's firefly count (to show inside the lantern)
    for (i = 0; i < objData->fireflyCount; i++) {
        objData->fireflies[i] = FireFlyLantern_create_firefly(self);
    }
    
    self->stateFlags |= OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x10C | func: 1 | export: 1
void FireFlyLantern_control(Object* self) {
    FireFlyLantern_Data* objData;
    s32 i;
    s32 count;
    
    objData = self->data;

    //Unload fireflies and self when finished
    if (objData->flagDone) {    
        for (i = 0; i < objData->fireflyCount; i++) {
            objFreeObject(objData->fireflies[i]);
        }
        
        objFreeObject(self);
    }
}

// offset: 0x1B0 | func: 2 | export: 2
void FireFlyLantern_update(Object *self) { }

// offset: 0x1BC | func: 3 | export: 3
void FireFlyLantern_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x1D4 | func: 4 | export: 4
void FireFlyLantern_free(Object* self, s32 onlySelf) {
    objFreeObjectType(self, OBJTYPE_UseObj);
}

// offset: 0x214 | func: 5 | export: 5
u32 FireFlyLantern_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x224 | func: 6 | export: 6
u32 FireFlyLantern_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(FireFlyLantern_Data);
}

// offset: 0x238 | func: 7
int FireFlyLantern_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackResult) {
    FireFlyLantern_Data* objData;
    Object* firefly;
    s32 i;

    if (self){} //fake

    objData = self->data;
    for (i = 0; i < animData->messageCount; i++) {
        if ((animData->messages[i] == 1) && (objData->fireflyCount)) {
            firefly = objData->fireflies[objData->fireflyCount - 1];
            if (firefly) {
                ((DLL_304_LanternFireFly*)firefly->dll)->vtbl->send(firefly);
            }
                
            objData->fireflyCount--;
            mainSetBits(objData->gamebitFireflies, objData->fireflyCount);
        }

        if (0) { } //fake
    }
    
    objData->flagDone = TRUE;

    for (i = 0; i < objData->fireflyCount; i++) {
        firefly = objData->fireflies[i];
        ((DLL_304_LanternFireFly*)firefly->dll)->vtbl->set_home(firefly, self->srt.transl.x, self->srt.transl.y + 5.0f, self->srt.transl.z);
    }
    
    return 0;
}

// offset: 0x39C | func: 8
Object* FireFlyLantern_create_firefly(Object* self) {
    LanternFireFly_Setup* firefly;

    firefly = objAllocSetup(sizeof(LanternFireFly_Setup), OBJ_LanternFireFly);
    firefly->base.objId = OBJ_LanternFireFly;
    firefly->base.quarterSize = sizeof(LanternFireFly_Setup)/4;
    firefly->base.loadFlags = OBJSETUP_LOAD_MANUAL;
    firefly->base.loadDistance = 0xFF;
    firefly->base.fadeFlags = OBJSETUP_FADE_CAMERA;
    firefly->base.fadeDistance = 8;
    firefly->base.x = self->srt.transl.x;
    firefly->base.y = self->srt.transl.y + 2.0f;
    firefly->base.z = self->srt.transl.z;
    firefly->effectType = 4;
    firefly->lifetime = 2600;
    firefly->varianceZ = 40;
    firefly->varianceY = 30;
    return objSetupObject((ObjSetup*)firefly, 5, -1, -1, self->parent);
}
