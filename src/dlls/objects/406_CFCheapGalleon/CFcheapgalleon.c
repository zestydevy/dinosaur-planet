#include "common.h"
#include "game/objects/object.h"
#include "macros.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/math.h"

typedef struct {
    u8 flags;
    s8 timer;
} CFCheapGalleon_Data;

typedef enum {
    CFCheapGalleon_FLAG_4_Unloaded_Detailed_Galleon = 4
} CFCheapGalleon_Flags;

// offset: 0x0 | ctor
void CFCheapGalleon_ctor(void* dll) { }

// offset: 0xC | dtor
void CFCheapGalleon_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFCheapGalleon_obj_Setup(Object* self, CFCheapGalleon_Setup* objSetup, s32 reset) {
    CFCheapGalleon_Data* objData = self->data;
    
    self->srt.yaw = objSetup->yaw << 8;
    objData->flags = CFCheapGalleon_FLAG_4_Unloaded_Detailed_Galleon;
    self->srt.roll = M_1_DEGREE;
    objData->timer = -1;
    self->stateFlags |= OBJSTATE_PRINT_DISABLED;

    STUBBED_PRINTF(" Loaded Cheap Galleon ");
}

// offset: 0x58 | func: 1 | export: 1
void CFCheapGalleon_obj_Control(Object* self) {
    CFCheapGalleon_Setup* objSetup;
    CFCheapGalleon_Data* objData;
    f32 distance;

    objSetup = (CFCheapGalleon_Setup*)self->setup;
    objData = self->data;
    
    //If the low-detail Galleon's forced on, stay at max opacity and bail out
    //@bug: doesn't ensure the high-detail Galleon is unloaded, so they can overlap here
    if (mainGetBits(objSetup->gamebitForceLowDetail)) {
        self->opacity = OBJECT_OPACITY_MAX;
        return;
    }
    
    distance = camDistance(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
    
    //Handle low-detail/high-detail crossfading
    if (objData->flags & CFCheapGalleon_FLAG_4_Unloaded_Detailed_Galleon) {
        if (mainGetBits(BIT_CRF_Galleon_Fade_to_High_Detail) || ((distance < 40.0f) && (objData->timer == -1))) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, objSetup->fullGalleonObjGroup, TRUE);
            objData->timer = 60;
            mainSetBits(BIT_CRF_Galleon_Fade_to_High_Detail, FALSE);
            STUBBED_PRINTF(" Ended Simple Cheap Galleon ");
        }
    } else if (mainGetBits(BIT_CRF_Galleon_Fade_to_Low_Detail)) {
        objData->timer = 30;
        mainSetBits(BIT_CRF_Galleon_Fade_to_Low_Detail, FALSE);
    }
    
    //Animate low-detail Galleon
    objAnimAdvance(self, 0.005f, gUpdateRateF, NULL);
    
    //If the timer's active, count down to flipping the "unload full-detail Galleon flag"
    if (objData->timer >= 0) {
        objData->timer -= gUpdateRate;
        if (objData->timer <= 0) {
            objData->timer = -1;
            objData->flags ^= CFCheapGalleon_FLAG_4_Unloaded_Detailed_Galleon;
            STUBBED_PRINTF(" FLIP FLIP \n\n ");

            //Unload the full-detail Galleon objectGroup
            if (objData->flags & CFCheapGalleon_FLAG_4_Unloaded_Detailed_Galleon) {
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, objSetup->fullGalleonObjGroup, FALSE);
            }
        }
    }
}

// offset: 0x250 | func: 2 | export: 2
void CFCheapGalleon_obj_Update(Object* self) {
    CFCheapGalleon_Data* objData = self->data;
    
    //Fade opacity in/out
    if (objData->flags & CFCheapGalleon_FLAG_4_Unloaded_Detailed_Galleon) {
        if (objData->timer >= 0) {
            self->opacity = (objData->timer * OBJECT_OPACITY_MAX) / 60;
        } else {
            self->opacity = OBJECT_OPACITY_MAX;
        }
    } else {
        if (objData->timer >= 0) {
            self->opacity = ((-objData->timer * OBJECT_OPACITY_MAX) + 15300) / 30;
        } else {
            self->opacity = 0;
        }   
    }
}

// offset: 0x2D4 | func: 3 | export: 3
void CFCheapGalleon_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x2EC | func: 4 | export: 4
void CFCheapGalleon_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x2FC | func: 5 | export: 5
u32 CFCheapGalleon_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x30C | func: 6 | export: 6
u32 CFCheapGalleon_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFCheapGalleon_Data);
}

/*0x48*/ static const char str_48[] = "";
/*0x4C*/ static const char str_4C[] = "";
