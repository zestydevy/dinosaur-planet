#include "dlls/engine/17_partfx.h"
#include "dlls/objects/332_FXEmit.h"
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
/*19*/ u8 _unk19[0x20 - 0x19];
/*20*/ s16 gamebitEnabled;
} DR_EarthCallPad_Setup;

typedef struct {
    Object* fxemit;
} DR_EarthCallPad_Data;

static Object* DR_EarthCallPad_create_fx(Object* self, s32 partID);

// offset: 0x0 | ctor
void DR_EarthCallPad_ctor(void *dll) { }

// offset: 0xC | dtor
void DR_EarthCallPad_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DR_EarthCallPad_setup(Object* self, DR_EarthCallPad_Setup* setup, s32 reset) {
    obj_add_object_type(self, OBJTYPE_DinoCallSpot);
    self->srt.yaw = setup->rotation << 8;
    self->stateFlags |= (OBJSTATE_CONTROL_DISABLED | OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED);
}

// offset: 0x7C | func: 1 | export: 1
void DR_EarthCallPad_control(Object *self) { }

// offset: 0x88 | func: 2 | export: 2
void DR_EarthCallPad_update(Object *self) { }

// offset: 0x94 | func: 3 | export: 3
void DR_EarthCallPad_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0xAC | func: 4 | export: 4
void DR_EarthCallPad_free(Object* self, s32 onlySelf) {
    DR_EarthCallPad_Data* objdata = self->data;
    if (objdata->fxemit != NULL) {
        obj_destroy_object(objdata->fxemit);
    }
    obj_free_object_type(self, OBJTYPE_DinoCallSpot);
}

// offset: 0x120 | func: 5 | export: 5
u32 DR_EarthCallPad_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x130 | func: 6 | export: 6
u32 DR_EarthCallPad_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DR_EarthCallPad_Data);
}

// offset: 0x144 | func: 7 | export: 7
s32 DR_EarthCallPad_call(Object* self) {
    DR_EarthCallPad_Setup* setup = (DR_EarthCallPad_Setup*)self->setup;
    DR_EarthCallPad_Data* objdata = self->data;

    STUBBED_PRINTF(" Playing Sequence for Calling Dino ");

    if (mainGetBits(setup->gamebitEnabled) == 0) {
        return 1;
    } else {
        gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_DRAGON_ROCK_BOTTOM, 2, 1);
        if (objdata->fxemit == NULL) {
            objdata->fxemit = DR_EarthCallPad_create_fx(self, PARTICLE_675);
        }
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        return 1;
    }
}

// offset: 0x224 | func: 8
static Object* DR_EarthCallPad_create_fx(Object* self, s32 partID) {
    FXEmit_Setup* fxSetup;

    STUBBED_PRINTF(" Creating Effect Emitter ");

    fxSetup = obj_alloc_setup(sizeof(FXEmit_Setup), OBJ_FXEmit);
    fxSetup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
    fxSetup->base.x = self->srt.transl.x;
    fxSetup->base.y = self->srt.transl.y;
    fxSetup->base.z = self->srt.transl.z;
    fxSetup->indexInBank = partID;
    fxSetup->fxRate = 0;
    fxSetup->toggleGamebit = -1;
    return obj_create((ObjSetup*)fxSetup, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, self->parent);
}
