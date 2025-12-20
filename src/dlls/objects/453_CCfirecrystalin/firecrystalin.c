#include "common.h"

#include "dlls/objects/453_CCfirecrystalin.h"

typedef struct {
    s8 unk0[0xA - 0x0];
    s16 unkA; //UV scroll offset?
} UnkStruct_func_800348A0; //TODO: verify/use correct struct

// offset: 0x0 | ctor
void CCfirecrystalin_ctor(void *dll) { }

// offset: 0xC | dtor
void CCfirecrystalin_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCfirecrystalin_setup(Object* self, CCfirecrystalin_Setup* objSetup, s32 arg2) {
    self->srt.yaw = objSetup->yaw << 8;
    self->unkB0 |= 0x6000;
}

// offset: 0x3C | func: 1 | export: 1
void CCfirecrystalin_control(Object* self) {
    s32 pad[2];
    CCfirecrystalin_Setup *objSetup;
    Object *fireCrystal;
    UnkStruct_func_800348A0 *unkStruct;
    s16 scroll;
    f32 scaleFactor;
    
    objSetup = (CCfirecrystalin_Setup *) self->setup;
    unkStruct = func_800348A0(self, 0, 0);

    //Update model texture UV scroll, maybe?
    scroll = -unkStruct->unkA;
    scroll += gUpdateRateF * objSetup->scrollSpeed;
    if (scroll > 2048){
        scroll -= 2048;
    }
    unkStruct->unkA = -scroll;

    //Update Object's transform
    self->srt.yaw += gUpdateRateF * objSetup->rotateSpeed;
    
    fireCrystal = objSetup->fireCrystal;

    scaleFactor = fireCrystal->srt.scale / fireCrystal->def->scale;
    self->srt.scale = self->def->scale * scaleFactor;
    self->srt.transl.x = objSetup->fireCrystal->srt.transl.x;
    self->srt.transl.y = objSetup->fireCrystal->srt.transl.y;
    self->srt.transl.z = objSetup->fireCrystal->srt.transl.z;
    self->srt.roll = objSetup->fireCrystal->srt.roll;
    self->srt.pitch = objSetup->fireCrystal->srt.pitch;
}

// offset: 0x198 | func: 2 | export: 2
void CCfirecrystalin_update(Object *self) { }

// offset: 0x1A4 | func: 3 | export: 3
void CCfirecrystalin_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1F8 | func: 4 | export: 4
void CCfirecrystalin_free(Object *self, s32 a1) { }

// offset: 0x208 | func: 5 | export: 5
u32 CCfirecrystalin_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x218 | func: 6 | export: 6
u32 CCfirecrystalin_get_data_size(Object *self, u32 a1){
    return 0;
}
