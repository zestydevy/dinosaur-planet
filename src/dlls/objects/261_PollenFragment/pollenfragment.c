#include "common.h"

#include "dlls/objects/261_PollenFragment.h"

// offset: 0x0 | ctor
void PollenFragment_ctor(void *dll) { }

// offset: 0xC | dtor
void PollenFragment_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void PollenFragment_setup(Object* self, PollenFragment_Setup* objSetup, s32 arg2) {
    PollenFragment_Data* objData = self->data;
    
    func_800267A4(self);
    objData->lifetime = rand_next(180, 300);
    gDLL_6_AMSFX->vtbl->play(self, SOUND_B02_Gas_Disperse_Burst, MAX_VOLUME, NULL, NULL, 0, NULL);
    gDLL_6_AMSFX->vtbl->play(self, SOUND_B03_Acid_Hiss_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
}

// offset: 0xF4 | func: 1 | export: 1
void PollenFragment_control(Object* self) {
    PollenFragment_Data* objData = self->data;
    
    //Start fading out after lifetime expires
    if (objData->lifetime >= gUpdateRate) {
        objData->lifetime -= gUpdateRate;
    } else {
        objData->lifetime = 0;
    
        //Destroy self once faded out
        if (self->opacity >= gUpdateRate * 4) {
            self->opacity -= gUpdateRate * 4;
        } else {
            self->opacity = 0;
            obj_destroy_object(self);
            return;
        }
    }

    //Apply drag and gravity, fade-out hiss with opacity, and move
    self->velocity.x *= 0.95f;
    self->velocity.z *= 0.95f;
    self->velocity.y *= 0.95f;
    self->velocity.y += 0.001f * gUpdateRateF;
    gDLL_6_AMSFX->vtbl->set_vol(objData->soundHandle, self->opacity / 2);
    obj_move(self, self->velocity.x * gUpdateRateF, self->velocity.y * gUpdateRateF, self->velocity.z * gUpdateRateF);
    
    //Objhits
    func_80026128(self, 21, 1, 0);
    func_80026940(self, 7);
    func_8002674C(self);
    
    //Create gas particles
    if (self->opacity == OBJECT_OPACITY_MAX) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_4BA, NULL, 1, -1, NULL);
    }
}

// offset: 0x2F0 | func: 2 | export: 2
void PollenFragment_update(Object *self) { }

// offset: 0x2FC | func: 3 | export: 3
void PollenFragment_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x350 | func: 4 | export: 4
void PollenFragment_free(Object* self, s32 arg1) {
    camera_disable_y_offset();
}

// offset: 0x390 | func: 5 | export: 5
u32 PollenFragment_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x3A0 | func: 6 | export: 6
u32 PollenFragment_get_data_size(Object *self, u32 a1){
    return sizeof(PollenFragment_Data);
}
