#include "common.h"
#include "game/objects/object_id.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} LanternFireFly_Setup; //0x24 (TO-DO: move into DLL 304)

typedef struct {
    ObjSetup base;
} FireFlyLantern_Setup;

typedef struct {
    Object* fireflies[20];
    u8 fireflyCount;
    union {
        s8 flagDone : 1;
        u8 flags;
    };
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
    if (get_player()->id != OBJ_Sabre) {
        objData->gamebitFireflies = BIT_Krystal_Fireflies;
    } else {
        objData->gamebitFireflies = BIT_Sabre_Fireflies;
    }
    
    //Store player's firefly count
    objData->fireflyCount = main_get_bits(objData->gamebitFireflies);

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
            obj_destroy_object(objData->fireflies[i]);
        }
        
        obj_destroy_object(self);
    }
}

// offset: 0x1B0 | func: 2 | export: 2
void FireFlyLantern_update(Object *self) { }

// offset: 0x1BC | func: 3 | export: 3
void FireFlyLantern_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x1D4 | func: 4 | export: 4
void FireFlyLantern_free(Object* self, s32 onlySelf) {
    obj_free_object_type(self, OBJTYPE_UseObj);
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
                ((DLL_Unknown*)firefly->dll)->vtbl->func[8].withOneArg(firefly);
            }
                
            objData->fireflyCount--;
            main_set_bits(objData->gamebitFireflies, objData->fireflyCount);
        }

        if (0) { } //fake
    }
    
    objData->flags = (objData->flags & 0xFF) | 0x80;

    for (i = 0; i < objData->fireflyCount; i++) {
        firefly = objData->fireflies[i];
        ((DLL_Unknown*)firefly->dll)->vtbl->func[9].withFourArgsCustom3(firefly, self->srt.transl.x, self->srt.transl.y + 5.0f, self->srt.transl.z);
    }
    
    return 0;
}

// offset: 0x39C | func: 8
Object* FireFlyLantern_create_firefly(Object* self) {
    LanternFireFly_Setup* firefly;

    firefly = obj_alloc_setup(sizeof(LanternFireFly_Setup), OBJ_LanternFireFly);
    firefly->base.objId = OBJ_LanternFireFly;
    firefly->base.quarterSize = sizeof(LanternFireFly_Setup)/4;
    firefly->base.loadFlags = OBJSETUP_LOAD_MANUAL;
    firefly->base.loadDistance = 0xFF;
    firefly->base.fadeFlags = OBJSETUP_FADE_CAMERA;
    firefly->base.fadeDistance = 8;
    firefly->base.x = self->srt.transl.x;
    firefly->base.y = self->srt.transl.y + 2.0f;
    firefly->base.z = self->srt.transl.z;
    firefly->unk19 = 4;
    firefly->unk1A = 2600;
    firefly->unk1C = 40;
    firefly->unk18 = 30;
    return obj_create((ObjSetup*)firefly, 5, -1, -1, self->parent);
}
