#include "PR/ultratypes.h"
#include "dll.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "game/gamebits.h"

typedef struct {
/*00*/ u8 keyUsed;
/*01*/ u8 canCheckKeyUsed;
} CClevcontrol_Data;

// offset: 0x0 | ctor
void CClevcontrol_ctor(void *dll) { }

// offset: 0xC | dtor
void CClevcontrol_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CClevcontrol_setup(Object *self, ObjSetup *objsetup, s32 arg2) {
    CClevcontrol_Data *objdata = self->data;

    if (!main_get_bits(BIT_CC_Used_Cell_Door_Key)) {
        main_set_bits(BIT_Kyte_Flight_Curve, 2);
    } else {
        objdata->keyUsed = TRUE;
    }

    if (main_get_bits(BIT_CC_Kyte_Pulled_All_Four_Levers) &&
       !main_get_bits(BIT_CC_Gate_Opened)) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 5, 1);
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 14, 1);
    }

    self->unkB0 |= 0x2000;
}

// offset: 0x120 | func: 1 | export: 1
void CClevcontrol_control(Object *self) {
    CClevcontrol_Data *objdata = self->data;

    if (!objdata->keyUsed) {
        if (objdata->canCheckKeyUsed) {
            if (main_get_bits(BIT_CC_Used_Cell_Door_Key)) {
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 13, 0);
                objdata->keyUsed = TRUE;
            } else if (!gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, 4)) {
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 13, 0);
                objdata->canCheckKeyUsed = FALSE;
            }
        } else if (gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, 4)) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 13, 1);
            objdata->canCheckKeyUsed = TRUE;
        }
    }
}

// offset: 0x270 | func: 2 | export: 2
void CClevcontrol_update(Object *self) { }

// offset: 0x27C | func: 3 | export: 3
void CClevcontrol_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x294 | func: 4 | export: 4
void CClevcontrol_free(Object *self, s32 arg1) { }

// offset: 0x2A4 | func: 5 | export: 5
u32 CClevcontrol_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x2B4 | func: 6 | export: 6
u32 CClevcontrol_get_data_size(Object *self, u32 arg1) {
    return sizeof(CClevcontrol_Data);
}
