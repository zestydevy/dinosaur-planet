#include "PR/ultratypes.h"
#include "functions.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

s16* func_80034804(Object* obj, s32 sequenceBoneID);

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s32 unk18;
/*1C*/ s16 flag1;
/*1C*/ s16 unk1E;
/*1C*/ s16 unk20;
/*1C*/ s16 unk22;
/*1C*/ s16 flag2;
} DBDiamond_Setup;

typedef struct {
/*000*/ u8 unk0;
} DBDiamondState;

// offset: 0x0 | ctor
void DBdiamond_ctor(void *dll) {
}

// offset: 0xC | dtor
void DBdiamond_dtor(void *dll) {
}

// offset: 0x18 | func: 0 | export: 0
void DBdiamond_setup(Object *self, DBDiamond_Setup *setup, s32 arg2) {
    DBDiamondState *state = self->state;

    if (main_get_bits(setup->flag1)){
        state->unk0 = 1;
        return;
    }

    if (main_get_bits(setup->flag2)) {
        state->unk0 = 2;
        obj_add_object_type(self, OBJTYPE_39);
        return;
    }
    
    state->unk0 = 4;
}

// offset: 0xD0 | func: 1 | export: 1
void DBdiamond_control(Object *self) {
    Object *player;
    DBDiamond_Setup *setup;
    DBDiamondState *state;
    s16 *sequenceBone;

    player = get_player();
    setup = (DBDiamond_Setup *)self->setup;
    state = self->state;

    if (state->unk0 != 1 && state->unk0 == 2 && self->unk0xaf & 1) {
        if (vec3_distance_xz(&self->positionMirror, &player->positionMirror) < 60.0f) {
            obj_free_object_type(self, OBJTYPE_39);
            self->unk0xaf |= 8;
            main_set_bits(setup->flag1, 1);
            obj_send_mesg(player, 0x7000A, self, (void*)0x10000);
            state->unk0 = 1;
        }
    }
    sequenceBone = func_80034804(self, 0);
    sequenceBone[1] += 0x650;
}

// offset: 0x204 | func: 2 | export: 2
void DBdiamond_update(Object *self) {
}

// offset: 0x210 | func: 3 | export: 3
void DBdiamond_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    DBDiamondState *state = self->state;
    
    if (!visibility)
        return;
    
    if (state->unk0 & 3)
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
}

// offset: 0x27C | func: 4 | export: 4
void DBdiamond_free(Object *self, s32 arg1) {
    obj_free_object_type(self, OBJTYPE_39);
}

// offset: 0x2BC | func: 5 | export: 5
u32 DBdiamond_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x2CC | func: 6 | export: 6
u32 DBdiamond_get_state_size(Object *self, s32 arg1) {
    return sizeof(DBDiamondState);
}
