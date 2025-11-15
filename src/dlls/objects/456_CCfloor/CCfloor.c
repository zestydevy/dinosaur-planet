#include "common.h"

typedef struct {
    ObjSetup base;
    s16 unk18; //unused
    u8 radius;
    u8 yOffset;
    f32 scale;
} CCfloor_Setup;

// offset: 0x0 | ctor
void CCfloor_ctor(void *dll) { }

// offset: 0xC | dtor
void CCfloor_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCfloor_setup(Object* self, CCfloor_Setup* objSetup, s32 arg2) {
    self->srt.scale = objSetup->scale * self->def->scale * 100.0f;
    self->unkB0 |= 0xA000;
    self->opacity = 0xC0;
}

// offset: 0x64 | func: 1 | export: 1
void CCfloor_control(Object *self) { }

// offset: 0x70 | func: 2 | export: 2
void CCfloor_update(Object *self) { }

// offset: 0x7C | func: 3 | export: 3
void CCfloor_print(Object* self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    Object* camera;
    CCfloor_Setup* objSetup;

    camera = gDLL_2_Camera->vtbl->func2();
    objSetup = (CCfloor_Setup*)self->setup;
    
    //Rotate object with the camera
    self->srt.yaw = -1*camera->srt.yaw + 0x8000;   

    //Lock object's X/Z coords to camera, with an offset based on the camera's yaw
    //The idea seems to be to create an ocean plane following the camera
    self->srt.transl.x = camera->srt.transl.f[0] - (fsin16_precise(camera->srt.yaw) * (objSetup->radius * 0x10));
    self->srt.transl.y = objSetup->base.y - objSetup->yOffset; //locked y coord, ignoring camera
    self->srt.transl.z = camera->srt.transl.f[2] + (fcos16_precise(camera->srt.yaw) * (objSetup->radius * 0x10));
   
    //Undo previous by just locking the object's coords directly to the camera
    self->srt.transl.x = camera->srt.transl.x;
    self->srt.transl.y = camera->srt.transl.y;
    self->srt.transl.z = camera->srt.transl.z;

    //@bug?: doesn't draw the object in the end (likely just incomplete/abandoned) 
}

// offset: 0x1B4 | func: 4 | export: 4
void CCfloor_free(Object *self, s32 a1) { }

// offset: 0x1C4 | func: 5 | export: 5
u32 CCfloor_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1D4 | func: 6 | export: 6
u32 CCfloor_get_data_size(Object *self, u32 a1){
    return 0;
}
