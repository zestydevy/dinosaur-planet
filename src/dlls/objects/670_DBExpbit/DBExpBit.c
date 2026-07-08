#include "common.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s16 _unk18;
    s16 modelIdx;
    s16 doDraw;
} DBExpBit_Setup;

typedef struct {
    u8 reset;
} DBExpBit_Data;

// offset: 0x0 | ctor
void DBExpBit_ctor(void *dll) { }

// offset: 0xC | dtor
void DBExpBit_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBExpBit_setup(Object* self, DBExpBit_Setup* objSetup, s32 reset) {
    DBExpBit_Data* objData = self->data;
    
    objData->reset = reset;
    
    obj_set_model(self, objSetup->modelIdx);
    
    self->srt.transl.x = objSetup->base.x;
    self->srt.transl.y = objSetup->base.y;
    self->srt.transl.z = objSetup->base.z;
    
    obj_add_object_type(self, OBJTYPE_DBExpbit);
}

// offset: 0x98 | func: 1 | export: 1
void DBExpBit_control(Object* self) {
    DBExpBit_Data* objData = self->data;
    objData->reset = 0;
}

// offset: 0xA8 | func: 2 | export: 2
void DBExpBit_update(Object *self) { }

// offset: 0xB4 | func: 3 | export: 3
void DBExpBit_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DBExpBit_Data* objData;
    DBExpBit_Setup* objSetup;

    objSetup = (DBExpBit_Setup*)self->setup;
    objData = self->data;
    
    if (visibility && (objSetup->doDraw || objData->reset)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}


// offset: 0x12C | func: 4 | export: 4
void DBExpBit_free(Object* self, s32 onlySelf) {
    obj_free_object_type(self, OBJTYPE_DBExpbit);
}

// offset: 0x16C | func: 5 | export: 5
u32 DBExpBit_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x17C | func: 6 | export: 6
u32 DBExpBit_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DBExpBit_Data);
}
