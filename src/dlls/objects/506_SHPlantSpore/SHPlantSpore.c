#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/main.h"
#include "sys/objprint.h"
#include "dll.h"

// offset: 0x0 | ctor
void dll_506_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_506_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_506_setup(Object* self, ObjSetup* setup, s32 arg2) {
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x34 | func: 1 | export: 1
void dll_506_control(Object* self) {
    Generic_Setup* objSetup = (Generic_Setup*) self->setup;

    if ((objSetup->bitB != NO_GAMEBIT) && (mainGetBits(objSetup->bitB) == 0)) {
        self->unkAF |= 8;
        return;
    }
    if (mainGetBits(BIT_Inventory_Purple_Mushrooms) == 0) {
        self->unkAF |= 0x10;
    } else {
        self->unkAF &= ~0x10;
    }
    if ((self->unkAF & 1) && gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_Inventory_Purple_Mushrooms)) {
        mainDecrementBits(BIT_Inventory_Purple_Mushrooms);
        mainSetBits(objSetup->bitA, 1);
        self->unkAF |= 0x8;
    }
    if (mainGetBits(objSetup->bitA) == 0) {
        self->unkAF &= ~0x8;
    }
    objprintUpdateLockIconCoords(self);
}

// offset: 0x184 | func: 2 | export: 2
void dll_506_update(Object *self) { }

// offset: 0x190 | func: 3 | export: 3
void dll_506_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x1A8 | func: 4 | export: 4
void dll_506_free(Object *self, s32 a1) { }

// offset: 0x1B8 | func: 5 | export: 5
u32 dll_506_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1C8 | func: 6 | export: 6
u32 dll_506_get_data_size(Object* self, u32 a1) {
    return 0;
}

