#include "common.h"
#include "sys/objmsg.h"

#include "dlls/objects/253_CannonBall.h"

// offset: 0x0 | ctor
void CannonBall_ctor(void *dll) { }

// offset: 0xC | dtor
void CannonBall_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CannonBall_setup(Object *self, CannonBall_Setup *setup, s32 arg2) { }

// offset: 0x2C | func: 1 | export: 1
void CannonBall_control(Object* self) {
    s32 max;
    s32 message;
    CannonBall_Data* objData;
    s32 x;
    s32 z;

    objData = self->data;
    
    //Apply gravity
    if (self->unkE0 == 1) {
        self->objhitInfo->unk50 = 8;
        self->velocity.y -= 0.07f * gUpdateRateF;
        self->velocity.y *= 0.97f;
    }
    
    //Integrate velocity
    self->prevLocalPosition.x = self->srt.transl.x;
    self->prevLocalPosition.y = self->srt.transl.y;
    self->prevLocalPosition.z = self->srt.transl.z;    
    self->srt.transl.x += self->velocity.x * gUpdateRateF;
    self->srt.transl.y += self->velocity.y * gUpdateRateF;
    self->srt.transl.z += self->velocity.z * gUpdateRateF;
    
    //Destroy self when lifetime runs out
    self->unkDC -= gUpdateRate;
    if (self->unkDC < 0) {
        obj_destroy_object(self);
    }
    
    //Set up damaging collision
    if (objData->timer >= 0x10) {
        func_80026128(self, 5, 1, 0);
    }
    
    //Become destroyed upon colliding
    if ((self->unkE0 == 1) && (self->objhitInfo->unk9D != 0)) {
        gDLL_17_partfx->vtbl->spawn(self, 6, NULL, 1, -1, NULL);
        gDLL_6_AMSFX->vtbl->play(NULL, SOUND_95_Explosion, MAX_VOLUME, NULL, NULL, 0, NULL);
        obj_destroy_object(self);
        
        if (
            self->unkE0 == 1 && 
            (x = self->srt.transl.x - gWorldX, (x < 0x7FFF) && (x >= -0x7FFF)) && 
            (z = self->srt.transl.z - gWorldZ, (z < 0x7FFF) && (z >= -0x7FFF))
        ) {
            x = x & 0xFFFF;
            x = (x << 0x10) | (z & 0xFFFF);

            //Send an object message with the cannonball's last coordinates (in local mapSpace)
            obj_send_mesg_many(GROUP_UNK29, 0, self, 0xD0004, (void*)x);
        }
    }
    
    objData->timer += gUpdateRate;
}

// offset: 0x2C4 | func: 2 | export: 2
void CannonBall_update(Object *self) { }

// offset: 0x2D0 | func: 3 | export: 3
void CannonBall_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x324 | func: 4 | export: 4
void CannonBall_free(Object* self, s32 arg1) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x36C | func: 5 | export: 5
u32 CannonBall_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x37C | func: 6 | export: 6
u32 CannonBall_get_data_size(Object *self, u32 a1) {
    return sizeof(CannonBall_Data);
}
