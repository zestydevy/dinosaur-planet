#include "common.h"

typedef struct {
    u8 fadeOut;
} SHboulder_Data;

// offset: 0x0 | ctor
void SHboulder_ctor(void *dll) { }

// offset: 0xC | dtor
void SHboulder_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHboulder_setup(Object *self, ObjSetup *setup, s32 arg2) { }

// offset: 0x2C | func: 1 | export: 1
void SHboulder_control(Object* self) {
    s32 hitSphereID;
    s32 hitDamage;
    u8 boulderStruck;
    s32 opacity;
    Object* hitBy;
    SHboulder_Data* objData;

    objData = self->data;

    //Handle fading out
    boulderStruck = objData->fadeOut;
    if (boulderStruck != FALSE && boulderStruck == TRUE) {
        //@bug: probably supposed to be "self->opacity - gUpdateRate*4"?
        opacity = (self->opacity - gUpdateRate) * 4; 
        if (opacity < 0) {
            opacity = 0;
        }
        self->opacity = opacity;
        return;
    }

    //Check for collisions (specifically the kind created by barrels' explosions!)
    if (func_80025F40(self, &hitBy, &hitSphereID, &hitDamage) == 5) {
        objData->fadeOut = TRUE;
    }
}

// offset: 0xC8 | func: 2 | export: 2
void SHboulder_update(Object *self) { }

// offset: 0xD4 | func: 3 | export: 3
void SHboulder_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x128 | func: 4 | export: 4
void SHboulder_free(Object *self, s32 a1) { }

// offset: 0x138 | func: 5 | export: 5
u32 SHboulder_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x148 | func: 6 | export: 6
u32 SHboulder_get_data_size(Object *self, u32 a1){
    return 0; //@bug: not enough space for SHboulder_Data, which causes a crash
}
