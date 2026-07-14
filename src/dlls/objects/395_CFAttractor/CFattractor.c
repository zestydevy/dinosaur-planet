#include "common.h"
#include "sys/objtype.h"
#include "dlls/objects/395_CFAttractor.h"

// offset: 0x0 | ctor
void CFAttractor_ctor(void* dll) { }

// offset: 0xC | dtor
void CFAttractor_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFAttractor_setup(Object* self, CFAttractor_Setup* objSetup, s32 reset) {
    objAddObjectType(self, OBJTYPE_32);
    self->srt.yaw = objSetup->yaw << 8;
}

// offset: 0x74 | func: 1 | export: 1
void CFAttractor_control(Object* self) {
    mapSaveObject(self->setup, self->mapID, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
}

// offset: 0xCC | func: 2 | export: 2
void CFAttractor_update(Object* self) { }

// offset: 0xD8 | func: 3 | export: 3
void CFAttractor_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x12C | func: 4 | export: 4
void CFAttractor_free(Object* self, s32 onlySelf) {
    objFreeObjectType(self, OBJTYPE_32);
}

// offset: 0x16C | func: 5 | export: 5
u32 CFAttractor_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x17C | func: 6 | export: 6
u32 CFAttractor_get_data_size(Object* self, u32 offsetAddr) {
    return 0;
}

// offset: 0x190 | func: 7 | export: 7
s16 CFAttractor_func_190(Object* self) {
    CFAttractor_Setup* objSetup = (CFAttractor_Setup*)self->setup;
    
    if (objSetup->mode == 0) {
        return 0;
    } else {
        return objSetup->unk1A;
    }
}

// offset: 0x1B4 | func: 8 | export: 8
void CFAttractor_func_1B4(Object* self, Object** outObj) {
    Object* player;
    Object* obj;
    CFAttractor_Setup* objSetup;
    f32 dx;
    f32 dz;

    obj = NULL;
    objSetup = (CFAttractor_Setup*)self->setup;
    
    switch (objSetup->mode) {
    case 0:
        break;
    case 1:
        obj = self;
        break;
    case 2:
        player = objGetPlayer();
        dx = player->srt.transl.x - self->srt.transl.x;
        dz = player->srt.transl.z - self->srt.transl.z;
        self->srt.yaw = arctan2s(dx, dz) + M_180_DEGREES;
        obj = self;
        break;
    }
    
    *outObj = obj;
}
