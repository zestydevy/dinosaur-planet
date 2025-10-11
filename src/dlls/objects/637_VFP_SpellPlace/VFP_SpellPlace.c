#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "dll.h"

typedef struct {
    ObjSetup base;
    u8 _unk18[6];
    s16 unk1E;
    s16 unk20;
} VFP_SpellPlace_Setup;

// size:0x6
typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
} VFP_SpellPlace_State;

static void VFP_SpellPlace_do_act1(Object* self);
static void VFP_SpellPlace_do_act2(Object* self);

// offset: 0x0 | ctor
void VFP_SpellPlace_ctor(void *dll) { }

// offset: 0xC | dtor
void VFP_SpellPlace_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void VFP_SpellPlace_setup(Object* self, VFP_SpellPlace_Setup* setup, s32 a2) {
    VFP_SpellPlace_State* state;

    state = self->state;
    state->unk0 = setup->unk1E;
    state->unk2 = setup->unk20;
    
    if (main_get_bits(state->unk2) != 0 || main_get_bits(state->unk0) != 0) {
        state->unk4 = 1;
    } else {
        self->unk0xaf |= 8;
    }
    self->unk0xb0 |= 0x6000;
}

// offset: 0xC8 | func: 1 | export: 1
void VFP_SpellPlace_control(Object* self) {
    Object* player;
    u8 mapSetupID;

    player = get_player();
    
    if ((player != NULL) && !(vec3_distance(&self->positionMirror, &player->positionMirror) > 100.0f)) {
        mapSetupID = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);
        switch (mapSetupID) {
        case 1:
            // act 1
            VFP_SpellPlace_do_act1(self);
            break;
        case 2:
            // act 2
            VFP_SpellPlace_do_act2(self);
            break;
        }
    }
}

// offset: 0x1AC | func: 2 | export: 2
void VFP_SpellPlace_update(Object *self) { }

// offset: 0x1B8 | func: 3 | export: 3
void VFP_SpellPlace_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x1D0 | func: 4 | export: 4
void VFP_SpellPlace_free(Object *self, s32 a1) { }

// offset: 0x1E0 | func: 5 | export: 5
u32 VFP_SpellPlace_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1F0 | func: 6 | export: 6
u32 VFP_SpellPlace_get_state_size(Object *self, u32 a1) {
    return sizeof(VFP_SpellPlace_State);
}

// offset: 0x204 | func: 7
static void VFP_SpellPlace_do_act1(Object* self) {
    VFP_SpellPlace_State* state;
    s16 bits2;
    s16 bits1;

    state = (VFP_SpellPlace_State*)self->state;
    
    bits2 = main_get_bits(state->unk2);
    bits1 = main_get_bits(state->unk0);
    
    if ((bits1 == 0) && (bits2 != 0)) {
        self->unk0xaf &= ~0x8;
        
        if ((bits2 != 0) && (gDLL_1_UI->vtbl->func7(0x123) != 0)) {
            main_set_bits(state->unk0, 1);
            state->unk4 = 1;
            self->unk0xaf |= 8;
        }
    }
}

// offset: 0x2E0 | func: 8
static void VFP_SpellPlace_do_act2(Object* self) {
    VFP_SpellPlace_State* state;
    s16 bits2;
    s16 bits1;

    state = (VFP_SpellPlace_State*)self->state;
    
    bits2 = main_get_bits(state->unk2);
    bits1 = main_get_bits(state->unk0);
    
    if ((bits1 == 0) && (bits2 != 0)) {
        self->unk0xaf &= ~0x8;
        
        if ((bits2 != 0) && (gDLL_1_UI->vtbl->func7(0x83B) != 0)) {
            main_set_bits(state->unk0, 1);
            state->unk4 = 1;
            self->unk0xaf |= 8;
        }
    }
}
