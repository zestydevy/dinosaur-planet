#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "functions.h"
#include "sys/objtype.h"
#include "dlls/objects/210_player.h"
#include "game/gamebits.h"

// 272?
DLL_INTERFACE(DLL_BoneDust) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ UnknownDLLFunc func7[3];
    /*10*/ void (*func10)(Object*, f32, f32, f32);
};

typedef struct {
/*00*/ObjSetup base;
/*18*/ u8 _unk18[0x1A-0x18];
/*1A*/ u8 unk1A;
/*1B*/ u8 _unk1B;
/*1C*/ s16 unk1C;
/*1E*/ u8 _unk1E[0x24-0x1E];
/*24*/ s16 unk24;
/*26*/ u8 _unk26[0x2C-0x26];
/*2C*/ s16 unk2C;
} BoneDust_Setup;

// offset: 0x0 | ctor
void MAGICMaker_ctor(void *dll) { }

// offset: 0xC | dtor
void MAGICMaker_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void MAGICMaker_setup(Object *self, ObjSetup *objsetup, s32 arg2) { }

// offset: 0x2C | func: 1 | export: 1
void MAGICMaker_control(Object *self) {
    BoneDust_Setup *bonedustSetup;
    Object **objects;
    s32 foundObjs;
    s32 i;
    s32 count;
    Object *obj;
    Object *player;
    Object *bonedustObj;

    if (main_get_bits(BIT_26B)) {
        main_set_bits(BIT_26B, 0);
        player = get_player();
        if (((DLL_210_Player*)player->dll)->vtbl->get_magic(player) < 10) {
            objects = obj_get_all_of_type(OBJTYPE_5, &count);
            foundObjs = 0;
            for (i = 0; i < count; i++) {
                obj = objects[i];
                if (obj->id == OBJ_BoneDust) {
                    foundObjs++;
                }
            }
            if (foundObjs < 5) {
                bonedustSetup = obj_alloc_create_info(sizeof(BoneDust_Setup), OBJ_BoneDust);
                bonedustSetup->unk1A = 0x14;
                bonedustSetup->unk2C = -1;
                bonedustSetup->unk1C = -1;
                bonedustSetup->base.x = self->srt.transl.x + rand_next(-300, 300);
                bonedustSetup->base.y = self->srt.transl.y;
                bonedustSetup->base.z = self->srt.transl.z + rand_next(-300, 300);
                bonedustSetup->unk24 = -1;
                bonedustObj = obj_create((ObjSetup*)bonedustSetup, 5, self->mapID, -1, self->parent);
                if (bonedustObj) {
                    ((DLL_BoneDust*)bonedustObj->dll)->vtbl->func10(bonedustObj, 0, -0.75f, 0);
                }
            }
        }
    }
}

// offset: 0x2A0 | func: 2 | export: 2
void MAGICMaker_update(Object *self) { }

// offset: 0x2AC | func: 3 | export: 3
void MAGICMaker_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x300 | func: 4 | export: 4
void MAGICMaker_free(Object *self, s32 arg1) { }

// offset: 0x310 | func: 5 | export: 5
u32 MAGICMaker_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x320 | func: 6 | export: 6
u32 MAGICMaker_get_data_size(Object *self, u32 arg1) {
    return 0;
}
