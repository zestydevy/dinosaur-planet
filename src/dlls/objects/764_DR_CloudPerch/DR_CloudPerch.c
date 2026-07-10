#include "dlls/engine/17_partfx.h"
#include "dlls/objects/332_FXEmit.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/map_enums.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "dll.h"
#include "macros.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 rotation;
/*18*/ s8 unk19;
/*19*/ u8 _unk1A[0x1E - 0x1A];
/*1E*/ s16 gamebitEnabled;
} DR_CloudPerch_Setup;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} DR_CloudPerch_Data;

// offset: 0x0 | ctor
void DR_CloudPerch_ctor(void *dll) { }

// offset: 0xC | dtor
void DR_CloudPerch_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DR_CloudPerch_setup(Object* self, DR_CloudPerch_Setup* setup, s32 reset) {
    DR_CloudPerch_Data* objdata;

    obj_add_object_type(self, OBJTYPE_DismountPoint);
    obj_add_object_type(self, OBJTYPE_DinoCallSpot);
    objdata = self->data;
    self->srt.yaw = setup->rotation << 8;
    objdata->unk0 = fsin16_precise(self->srt.yaw);
    objdata->unk4 = 0.0f;
    objdata->unk8 = fcos16_precise(self->srt.yaw);
    objdata->unkC = (f32) -((self->srt.transl.z * objdata->unk8) + ((objdata->unk0 * self->srt.transl.x) + (objdata->unk4 * self->srt.transl.y)));
    self->stateFlags |= (OBJSTATE_CONTROL_DISABLED | OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED);
    if ((s32)mainGetBits(BIT_7A9) == setup->unk19) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 12, 1);
    }
}

// offset: 0x164 | func: 1 | export: 1
void DR_CloudPerch_control(Object *self) { }

// offset: 0x170 | func: 2 | export: 2
void DR_CloudPerch_update(Object *self) { }

// offset: 0x17C | func: 3 | export: 3
void DR_CloudPerch_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x194 | func: 4 | export: 4
void DR_CloudPerch_free(Object* self, s32 onlySelf) {
    obj_free_object_type(self, OBJTYPE_DismountPoint);
    obj_free_object_type(self, OBJTYPE_DinoCallSpot);
}

// offset: 0x1F4 | func: 5 | export: 5
u32 DR_CloudPerch_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x204 | func: 6 | export: 6
u32 DR_CloudPerch_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DR_CloudPerch_Data);
}

// offset: 0x218 | func: 7 | export: 8
s32 DR_CloudPerch_land(Object* self) {
    DR_CloudPerch_Setup* setup = (DR_CloudPerch_Setup*)self->setup;
    mainSetBits(BIT_7A9, setup->unk19);
    gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
    return 0;
}

// offset: 0x28C | func: 8 | export: 7
s32 DR_CloudPerch_call(Object* self) {
    DR_CloudPerch_Setup* setup = (DR_CloudPerch_Setup*)self->setup;
    if (mainGetBits(setup->gamebitEnabled) == 0) {
        return 0;
    }
    STUBBED_PRINTF(" Playing Sequence for Calling Dino ");
    mainSetBits(BIT_7A9, setup->unk19);
    gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 12, 1);
    gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
    return 1;
}
