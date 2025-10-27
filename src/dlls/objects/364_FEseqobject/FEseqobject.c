#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "sys/main.h"
#include "game/gamebits.h"
#include "dll.h"
#include "functions.h"
#include "game/objects/object_id.h"

typedef struct {
    u8 unk0;
} FEseqobject_Data;

/*0x0*/ static u32 _data_0[] = { 0x00000000, 0x0019ffff, 0x80000000, 0x00190000 }; // ?

static int FEseqobject_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);

// offset: 0x0 | ctor
void FEseqobject_ctor(void *dll) { }

// offset: 0xC | dtor
void FEseqobject_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void FEseqobject_setup(Object *self, ObjSetup *setup, s32 arg2) {
    self->srt.yaw = 0;
    self->animCallback = FEseqobject_anim_callback;
    obj_init_mesg_queue(self, 10);
}

// offset: 0x6C | func: 1 | export: 1
void FEseqobject_control(Object *self) {
    self->srt.yaw = 0x2000;
    if (!main_get_bits(BIT_75)) {
        gDLL_3_Animation->vtbl->func17(0, self, -1);
    }
}

// offset: 0xE4 | func: 2 | export: 2
void FEseqobject_update(Object *self) { }

// offset: 0xF0 | func: 3 | export: 3
void FEseqobject_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x144 | func: 4 | export: 4
void FEseqobject_free(Object *self, s32 a1) { }

// offset: 0x154 | func: 5 | export: 5
u32 FEseqobject_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x164 | func: 6 | export: 6
u32 FEseqobject_get_data_size(Object *self, u32 a1) {
    return sizeof(FEseqobject_Data);
}

// offset: 0x178 | func: 7
int FEseqobject_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    Object *mesgSender;
    s32 i;
    u32 mesgID;
    void *mesg;
    Object **objects;
    Object *obj;
    s32 objCount;
    SRT transform;
    Object *receiver;

    receiver = NULL;

    for (i = 0; i < animObjData->unk98; i++) {
        transform.transl.x = 0.0f;
        transform.transl.y = 0.0f;
        transform.transl.z = 0.0f;
        transform.pitch = 0;
        transform.yaw = 0;
        transform.roll = 0;
        transform.scale = 1.0f;
        switch (animObjData->unk8E[i]) {
        case 1:
            main_set_bits(BIT_75, 1);
            break;
        case 2:
            gDLL_17->vtbl->func1(self, PARTICLE_85, &transform, 1, -1, NULL);
            break;
        case 3:
            transform.roll = 1;
            gDLL_17->vtbl->func1(self, PARTICLE_85, &transform, 1, -1, NULL);
            break;
        case 4:
            transform.roll = 2;
            gDLL_17->vtbl->func1(self, PARTICLE_85, &transform, 1, -1, NULL);
            break;
        case 5:
            transform.roll = 3;
            gDLL_17->vtbl->func1(self, PARTICLE_85, &transform, 1, -1, NULL);
            break;
        case 6:
            transform.roll = 4;
            gDLL_17->vtbl->func1(self, PARTICLE_85, &transform, 1, -1, NULL);
            break;
        }
    }
    while (obj_recv_mesg(self, &mesgID, &mesgSender, &mesg)) {
        if (!(animObjData->unk9D & 0x80)) {
            switch (mesgID) {
            case 0xF000B:
                objects = obj_get_all_of_type(4, &objCount);
                for (i = 0; i < objCount; i++) {
                    obj = objects[i];
                    if (obj->id == OBJ_SB_ShipHead) {
                        receiver = obj;
                        i = objCount;
                    }
                }
                if (receiver) {
                    obj_send_mesg(receiver, 0x130001, self, NULL);
                }
                break;
            case 0xF000C:
                objects = obj_get_all_of_type(4, &objCount);
                for (i = 0; i < objCount; i++) {
                    obj = objects[i];
                    if (obj->id == OBJ_SB_ShipHead) {
                        receiver = obj;
                        i = objCount;
                    }
                }
                if (receiver) {
                    obj_send_mesg(receiver, 0x130002, self, NULL);
                }
                break;
            case 0xF000D:
                objects = obj_get_all_of_type(4, &objCount);
                for (i = 0; i < objCount; i++) {
                    obj = objects[i];
                    if (obj->id == OBJ_SB_ShipHead) {
                        receiver = obj;
                        i = objCount;
                    }
                }
                if (receiver) {
                    obj_send_mesg(receiver, 0x130003, self, NULL);
                }
                break;
            }
        }
    }
    animObjData->unk62 = 0;
    return 0;
}
