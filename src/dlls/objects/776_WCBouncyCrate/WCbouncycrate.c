#include "common.h"
#include "sys/objtype.h"

typedef struct {
    f32 initialY;
    s32 _unk4;
    s16 timer;
    u8 flags;
    u8 bounceCount;
} WCBouncyCrate_Data;

typedef enum {
    WCBouncyCrate_FLAG_Moving = 1
} WCBouncyCrate_Flags;

static f32 WCBouncyCrate_getInitialVelocityY(Object* self);

// offset: 0x0 | ctor
void WCBouncyCrate_ctor(void* dll) { }

// offset: 0xC | dtor
void WCBouncyCrate_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WCBouncyCrate_obj_Setup(Object* self, ObjSetup* setup, s32 reset) {
    WCBouncyCrate_Data* objData = self->data;
    objData->initialY = setup->y;
    objData->timer = 40;
}

// offset: 0x38 | func: 1 | export: 1
void WCBouncyCrate_obj_Control(Object* self) {
    WCBouncyCrate_Data* objData = self->data;

    //Start moving after a short delay
    if ((objData->flags & WCBouncyCrate_FLAG_Moving) == FALSE) {
        objData->timer -= gUpdateRateF;
        if (objData->timer <= 0) {
            // Get initial velocity based on Baddie's distance
            // This may suggest the crate was supposed to bounce as a heavy Baddie passed close by, 
            // like one of Walled City's RedEyes!)
            self->velocity.y = WCBouncyCrate_getInitialVelocityY(self);
            objData->bounceCount = 0;
            objData->flags |= WCBouncyCrate_FLAG_Moving;
        }
        return; 
    }

    //Apply gravity and move vertically
    self->velocity.y += -0.14f * gUpdateRateF;
    self->srt.transl.y += self->velocity.y * gUpdateRateF;

    //Bounce when lower than initial elevation
    if (self->srt.transl.y <= objData->initialY) {
        self->srt.transl.y += objData->initialY - self->srt.transl.y; //Equivalent to `self->srt.transl.y = objData->initialY`
        self->velocity.y = -self->velocity.y * 0.8f; //Lose some momentum
        objData->bounceCount++;

        //Stop after a few bounces
        if (objData->bounceCount > 10) {
            objData->flags &= ~WCBouncyCrate_FLAG_Moving;
            objData->timer = 40; //Will be ready to bounce anew after a short delay
            self->srt.transl.y = objData->initialY;
            self->velocity.y = 0.0f;
        }
    }
}


// offset: 0x190 | func: 2 | export: 2
void WCBouncyCrate_obj_Update(Object* self) { }

// offset: 0x19C | func: 3 | export: 3
void WCBouncyCrate_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1F0 | func: 4 | export: 4
void WCBouncyCrate_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x200 | func: 5 | export: 5
u32 WCBouncyCrate_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x210 | func: 6 | export: 6
u32 WCBouncyCrate_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WCBouncyCrate_Data);
}

// offset: 0x224 | func: 7
static f32 WCBouncyCrate_getInitialVelocityY(Object* self) {
    f32 distance;

    //If there's no Baddie nearby, start falling with 0 velocity
    distance = 10000.0f;
    if (objGetNearestTypeTo(OBJTYPE_Baddie, self, &distance) == NULL) {
        return 0.0f;
    }

    //Upward initial velocity if a Baddie is close
    if (distance < 200.0f) {
        return 2.0f;
    }

    //Zero initial velocity if the Baddie is far away
    if (distance > 500.0f) {
        return 0.0f;
    }

    //Initial velocity fades out with Baddie's distance
    return 2.0f * (1.0f - ((distance - 200.0f) / 300.0f));
}

