#include "common.h"
#include "sys/objtype.h"

#include "dlls/objects/768_SPshop.h"

typedef struct {
    ObjSetup base;
    s16 unused18;
    s16 itemIndex; //The kind of shop item being highlighted (see `ShopItemIndices`)
} SPitembeam_Setup;

// offset: 0x0 | ctor
void SPitembeam_ctor(void *dll) { }

// offset: 0xC | dtor
void SPitembeam_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SPitembeam_setup(Object* self, SPitembeam_Setup* setup, s32 arg2) {
    f32 distance;

    distance = 10000.0f;
    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);

    //Store a pointer to the SPShop object
    self->unkDC = (s32)obj_get_nearest_type_to(OBJTYPE_10, self, &distance);
}

// offset: 0x88 | func: 1 | export: 1
void SPitembeam_control(Object* self) {
    s32 pad[2];
    Object* shop;
    SPitembeam_Setup* objSetup;
    TextureAnimator* texAnim;

    objSetup = (SPitembeam_Setup*)self->setup;
    shop = (Object*)self->unkDC;

    //@bug: no NULL check for the SPShop object

    //Check if the beam's item isn't visible (and shouldn't be spotlighted)
    if ((((DLL_768_SPShop*)shop->dll)->vtbl->is_item_shown(shop, objSetup->itemIndex) == FALSE) ||
        (((DLL_768_SPShop*)shop->dll)->vtbl->is_item_hidden(shop, objSetup->itemIndex))
    ) {
        self->srt.flags |= OBJFLAG_INVISIBLE;
        self->stateFlags |= OBJSTATE_CONTROL_DISABLED;
    }

    //Scroll the light beam texture
    texAnim = func_800348A0(self, 0, 0);
    if (texAnim != NULL) {
        texAnim->positionU += 8;
        if (texAnim->positionU > 0x400) {
            texAnim->positionU -= 0x400;
        }
    }
}

// offset: 0x170 | func: 2 | export: 2
void SPitembeam_update(Object *self) { }

// offset: 0x17C | func: 3 | export: 3
void SPitembeam_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x194 | func: 4 | export: 4
void SPitembeam_free(Object *self, s32 a1) { }

// offset: 0x1A4 | func: 5 | export: 5
u32 SPitembeam_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1B4 | func: 6 | export: 6
s32 SPitembeam_get_data_size(s32 arg0, s32 arg1) {
    return 0;
}
