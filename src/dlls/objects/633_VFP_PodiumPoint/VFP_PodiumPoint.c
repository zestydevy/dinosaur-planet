#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "dlls/objects/214_animobj.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "dll.h"
#include "functions.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    u8 _unk19[4];
    s16 gamebit;
    s16 gamebit2;
} VFP_PodiumPoint_Setup;

// size:0x6
typedef struct {
    s16 setGamebit;
    s16 conditionGamebit;
    u8 unk4;
} VFP_PodiumPoint_State;

/*0x0*/ static s32 data_0 = 0;

static void VFP_PodiumPoint_func_1B8(Object* self);
static s32 VFP_PodiumPoint_func_324(Object* a0, Object* a1, AnimObjState* a2, void* a3);

// offset: 0x0 | ctor
void VFP_PodiumPoint_ctor(void *dll) { }

// offset: 0xC | dtor
void VFP_PodiumPoint_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void VFP_PodiumPoint_setup(Object* self, VFP_PodiumPoint_Setup* setup, s32 arg2) {
    VFP_PodiumPoint_State* state;

    state = (VFP_PodiumPoint_State*)self->state;
    self->srt.yaw = setup->unk18 << 8;
    self->unk0xbc = (ObjectCallback)VFP_PodiumPoint_func_324;
    state->setGamebit = setup->gamebit;
    state->conditionGamebit = setup->gamebit2;
    self->unk0xaf |= 8;
}

// offset: 0x68 | func: 1 | export: 1
void VFP_PodiumPoint_control(Object* self) {
    u8 mapSetupID;

    mapSetupID = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);
    switch (mapSetupID) {
    default:
        data_0 = 0x123;
        break;
    case 1:
        data_0 = 0x123;
        break;
    case 2:
        data_0 = 0x83B;
        break;
    case 3:
        data_0 = 0x83C;
        break;
    }
    VFP_PodiumPoint_func_1B8(self);
}

// offset: 0x128 | func: 2 | export: 2
void VFP_PodiumPoint_update(Object *self) {
    if (self->unk0x74 != NULL) {
        func_80036438(self);
    }
}

// offset: 0x16C | func: 3 | export: 3
void VFP_PodiumPoint_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x184 | func: 4 | export: 4
void VFP_PodiumPoint_free(Object *self, s32 a1) { }

// offset: 0x194 | func: 5 | export: 5
u32 VFP_PodiumPoint_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1A4 | func: 6 | export: 6
u32 VFP_PodiumPoint_get_state_size(Object *self, u32 a1) {
    return sizeof(VFP_PodiumPoint_State);
}

// offset: 0x1B8 | func: 7
static void VFP_PodiumPoint_func_1B8(Object* self) {
    VFP_PodiumPoint_State* state;
    s16 conditionGamebitValue;
    s16 gamebitValue;
    Object* player;

    state = self->state;
    conditionGamebitValue = 1;
    player = get_player();

    if (player != NULL) {
        if (state->conditionGamebit != -1) {
            conditionGamebitValue = main_get_bits(state->conditionGamebit);
        }
        gamebitValue = main_get_bits(state->setGamebit);
        if ((gamebitValue == 0) && (state->unk4 == 0) && (conditionGamebitValue != 0)) {
            self->unk0xaf &= ~0x8;
            if ((gDLL_1_UI->vtbl->func7(data_0) != 0) && 
                    (vec3_distance(&self->positionMirror, &player->positionMirror) < 100.0f)) {
                gDLL_3_Animation->vtbl->func17(0, self, -1);
                main_set_bits(state->setGamebit, 1);
                state->unk4 = 1;
                self->unk0xaf |= 8;
            }
        }
    }
}

// offset: 0x324 | func: 8
static s32 VFP_PodiumPoint_func_324(Object* a0, Object* a1, AnimObjState* a2, void* a3) {
    s32 i;
    u8 mapSetupID;

    for (i = 0; i < a2->unk98; i++) {
        if (a2->unk8E[i] == 1) {
            mapSetupID = gDLL_29_Gplay->vtbl->get_map_setup(a0->mapID);
            switch (mapSetupID) {
            case 1:
                main_set_bits(BIT_VFP_Door_Seal_Break, 1);
                break;
            case 2:
                main_set_bits(BIT_515, 1);
                break;
            case 3:
                main_set_bits(BIT_VFP_Door_Open, 1);
                break;
            }
            a2->unk8E[i] = 0;
        }
    }
    return 0;
}
