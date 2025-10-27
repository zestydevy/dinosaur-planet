#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "dll.h"
#include "functions.h"
#include "sys/objects.h"
#include "sys/memory.h"
#include "sys/fs.h"
#include "sys/asset_thread.h"

DLL_INTERFACE(DLL_529_DIMLavaBall) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ void (*func7)(Object*, u8, s32);
    /*8*/ s32 (*func8)(Object*);
};

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    s16 unk1A;
    s16 unk1C;
    u8 unk1E[0x24-0x1E];
} DIMLavaBall_Setup;

typedef struct {
    NewLfxStruct *lfxStructs[2];
    Object *lavaball;
    s16 timer;
    s16 timerMax;
    s16 index;
    s16 unk12;
    u8 unk14;
    u8 loaded;
    u8 unk16;
    u8 unk17;
} DIMLavaBallGenerator_Data;

typedef struct {
    ObjSetup base;
    s16 timerMax;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 index;
    s16 gamebit1;
    u8 unk20;
    u8 unk21;
    s16 gamebit2;
    s16 gamebit3;
} DIMLavaBallGenerator_Setup;

typedef struct {
    NewLfxStruct base;
    u8 unk24[0x28-0x24];
} NewLfxStruct_Local;

// offset: 0x0 | ctor
void DIMLavaBallGenerator_ctor(void *dll) { }

// offset: 0xC | dtor
void DIMLavaBallGenerator_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIMLavaBallGenerator_setup(Object *self, DIMLavaBallGenerator_Setup *setup, s32 arg2) {
    DIMLavaBallGenerator_Data *objdata;

    objdata = self->data;
    self->srt.yaw = setup->unk1C << 8;

    objdata->timerMax = setup->timerMax;
    objdata->timer = 0;
    objdata->index = setup->index;
    objdata->unk14 = main_get_bits(setup->gamebit2);
    if ((setup->gamebit3 == -1) && !objdata->unk14) {
        objdata->unk17 = 1;
    }
    if ((objdata->index != 0) && (arg2 == 0)) {
        objdata->lfxStructs[0] = mmAlloc(sizeof(NewLfxStruct_Local), ALLOC_TAG_OBJECTS_COL, NULL);
        objdata->lfxStructs[1] = mmAlloc(sizeof(NewLfxStruct_Local), ALLOC_TAG_OBJECTS_COL, NULL);
    }
    self->unkB0 |= 0x6000;
}

// offset: 0x114 | func: 1 | export: 1
void DIMLavaBallGenerator_control(Object *self) {
    u8 var_a1;
    DIMLavaBallGenerator_Data *objdata;
    DIMLavaBallGenerator_Setup *setup;
    Object *lavaball;
    DIMLavaBall_Setup *lavaballSetup;
    s32 size;

    objdata = self->data;
    setup = (DIMLavaBallGenerator_Setup*)self->setup;

    if (objdata->index != 0 && !objdata->loaded) {
        size = sizeof(NewLfxStruct_Local);
        queue_load_file_region_to_ptr((void**)objdata->lfxStructs[0], LACTIONS_BIN,
            (objdata->index * size), size);
        queue_load_file_region_to_ptr((void**)objdata->lfxStructs[1], LACTIONS_BIN,
            ((objdata->index + 1) * size), size);
        objdata->loaded = TRUE;
    }
    objdata->unk16 = main_get_bits(setup->gamebit3);
    if (objdata->unk17 != 0) {
        if (main_get_bits(setup->gamebit1)) {
            objdata->unk16 = 1;
            objdata->unk17 = 0;
            objdata->timer = 0;
        } else {
            objdata->unk16 = 0;
        }
    }
    if (objdata->lavaball == NULL) {
        lavaballSetup = obj_alloc_create_info(sizeof(DIMLavaBall_Setup), OBJ_DIMLavaBall);
        lavaballSetup->base.quarterSize = 9;
        lavaballSetup->base.loadParamA = 2;
        lavaballSetup->base.loadDistance = 0xFF;
        lavaballSetup->base.loadParamB = 4;
        lavaballSetup->base.fadeDistance = 80;
        lavaballSetup->base.x = self->srt.transl.x;
        lavaballSetup->base.y = self->srt.transl.y;
        lavaballSetup->base.z = self->srt.transl.z;
        lavaballSetup->unk18 = setup->unk1C;
        lavaballSetup->unk1A = setup->unk1A;
        lavaballSetup->unk1C = setup->unk1B;
        lavaballSetup->base.uID = setup->base.uID;

        objdata->lavaball = obj_create((ObjSetup*)lavaballSetup, 5, self->mapID, -1, NULL);
    }

    lavaball = objdata->lavaball;
    objdata->timer -= gUpdateRate;
    if (objdata->timer > 0) {
        return;
    }

    if (((DLL_529_DIMLavaBall*)lavaball->dll)->vtbl->func8(lavaball) == 0) {
        return;
    }

    if (objdata->unk16 != 0) {
        if (main_get_bits(setup->gamebit1) && (objdata->unk14 == 0)) {
            var_a1 = setup->unk20;
            objdata->unk14 = 1;
        } else {
            var_a1 = setup->unk1A;
        }
        ((DLL_529_DIMLavaBall*)lavaball->dll)->vtbl->func7(lavaball, var_a1, setup->unk1B);
    }

    objdata->timer = objdata->timerMax;
}

// offset: 0x388 | func: 2 | export: 2
void DIMLavaBallGenerator_update(Object *self) { }

// offset: 0x394 | func: 3 | export: 3
void DIMLavaBallGenerator_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x3E8 | func: 4 | export: 4
void DIMLavaBallGenerator_free(Object *self, s32 a1) {
    DIMLavaBallGenerator_Data *objdata;

    objdata = self->data;
    if ((objdata->index != 0) && (objdata->unk12 != 0)) {
        gDLL_11_Newlfx->vtbl->func0(self, self, objdata->lfxStructs[1], 0, 0, 0);
        if (objdata->lfxStructs[0]) {
            mmFree(objdata->lfxStructs[0]);
        }
        if (objdata->lfxStructs[1]) {
            mmFree(objdata->lfxStructs[1]);
        }
    }
    if (a1 == 0) {
        if (objdata->lavaball) {
            obj_destroy_object(objdata->lavaball);
        }
    }
}

// offset: 0x4D8 | func: 5 | export: 5
u32 DIMLavaBallGenerator_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x4E8 | func: 6 | export: 6
u32 DIMLavaBallGenerator_get_data_size(Object *self, u32 a1) {
    return sizeof(DIMLavaBallGenerator_Data);
}

// offset: 0x4FC | func: 7 | export: 7
s32 DIMLavaBallGenerator_func_4FC(Object *self, Object *arg1) {
    DIMLavaBallGenerator_Data *objdata;

    objdata = self->data;
    if (objdata->index == 0) {
        return 0;
    }
    if (objdata->unk12 == 0) {
        gDLL_11_Newlfx->vtbl->func0(arg1, arg1, objdata->lfxStructs[0], 0, 0, 0);
        objdata->unk12 = 1;
        return 1;
    }
    return 0;
}

// offset: 0x59C | func: 8 | export: 8
void DIMLavaBallGenerator_func_59C(Object *self, Object *arg1) {
    DIMLavaBallGenerator_Data *objdata;

    objdata = self->data;
    if ((objdata->index != 0) && (objdata->unk12 != 0)) {
        gDLL_11_Newlfx->vtbl->func0(arg1, arg1, objdata->lfxStructs[1], 0, 0, 0);
        objdata->unk12 = 0;
    }
}
