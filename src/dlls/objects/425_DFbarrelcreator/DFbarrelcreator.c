#include "common.h"
#include "game/objects/setups/103.h"
#include "game/objects/setups/104_DFbarrelcreator.h"
#include "sys/objtype.h"

// offset: 0x0 | ctor
void DFbarrelcreator_ctor(void *dll) { }

// offset: 0xC | dtor
void DFbarrelcreator_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFbarrelcreator_setup(Object *self, DFBarrelCreator_Setup *setup, s32 reset) { }

// offset: 0x2C | func: 1 | export: 1
void DFbarrelcreator_control(Object* self) {
    DFBarrelCreator_Setup* objSetup;
    Ctrl103_Setup* barrel;
    f32 distance;

    objSetup = (DFBarrelCreator_Setup*)self->setup;

    //Don't create a barrel if the specified gamebit is set
    if ((objSetup->gamebitStop != NO_GAMEBIT) && mainGetBits(objSetup->gamebitStop)) {
        return;
    }

    //Don't create a barrel if there's already one nearby
    distance = objSetup->searchDistance * 4;
    if (objGetNearestTypeTo(OBJTYPE_Barrel, self, &distance)) {
        return;
    }

    //Create a barrel
    barrel = (Ctrl103_Setup*)objAllocSetup(sizeof(Ctrl103_Setup), OBJ_DFbarrel);
    barrel->base.loadDistance = 100;
    barrel->base.fadeDistance = 80;
    barrel->base.loadFlags = 4;
    barrel->base.fadeFlags = 4;
    barrel->base.x = self->srt.transl.x;
    barrel->base.y = self->srt.transl.y - 30.0f;
    barrel->base.z = self->srt.transl.z;
    objSetupObject((ObjSetup*)barrel, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, NULL);
}

// offset: 0x140 | func: 2 | export: 2
void DFbarrelcreator_update(Object *self) { }

// offset: 0x14C | func: 3 | export: 3
void DFbarrelcreator_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1A0 | func: 4 | export: 4
void DFbarrelcreator_free(Object *self, s32 onlySelf) { }

// offset: 0x1B0 | func: 5 | export: 5
u32 DFbarrelcreator_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1C0 | func: 6 | export: 6
u32 DFbarrelcreator_get_data_size(Object *self, u32 offsetAddr) {
    return 0;
}
