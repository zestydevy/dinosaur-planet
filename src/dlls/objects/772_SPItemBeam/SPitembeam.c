#include "common.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/objtype.h"

#include "dlls/objects/768_SPshop.h"

// offset: 0x0 | ctor
void SPItemBeam_ctor(void* dll) { }

// offset: 0xC | dtor
void SPItemBeam_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void SPItemBeam_obj_Setup(Object* self, SPItemBeam_Setup* setup, s32 reset) {
    f32 distance;

    distance = 10000.0f;
    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);

    //Store a pointer to the SPShop object
    self->unkDC = (s32)objGetNearestTypeTo(OBJTYPE_LevelControl, self, &distance);
}

// offset: 0x88 | func: 1 | export: 1
void SPItemBeam_obj_Control(Object* self) {
    s32 pad[2];
    Object* shop;
    SPItemBeam_Setup* objSetup;
    TextureAnimator* texAnim;

    objSetup = (SPItemBeam_Setup*)self->setup;
    shop = (Object*)self->unkDC;

    //@bug: no NULL check for the SPShop object

    //Check if the beam's item isn't visible (and shouldn't be spotlighted)
    if ((dll_SPShop(shop)->IsItemShown(shop, objSetup->itemIndex) == FALSE) ||
        dll_SPShop(shop)->IsItemHidden(shop, objSetup->itemIndex)
    ) {
        self->srt.flags |= OBJFLAG_INVISIBLE;
        self->stateFlags |= OBJSTATE_CONTROL_DISABLED;
    }

    //Scroll the light beam texture
    texAnim = objExprGetTexAnimator(self, 0, 0);
    if (texAnim != NULL) {
        texAnim->positionU += 8;
        if (texAnim->positionU > 0x400) {
            texAnim->positionU -= 0x400;
        }
    }
}

// offset: 0x170 | func: 2 | export: 2
void SPItemBeam_obj_Update(Object* self) { }

// offset: 0x17C | func: 3 | export: 3
void SPItemBeam_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x194 | func: 4 | export: 4
void SPItemBeam_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x1A4 | func: 5 | export: 5
u32 SPItemBeam_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1B4 | func: 6 | export: 6
s32 SPItemBeam_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return 0;
}
