#include "dll.h"
#include "sys/main.h"
#include "game/gamebits.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "dlls/objects/212_kyte.h"

typedef struct {
/*00*/ f32 timer;
/*04*/ s32 flightCurve;
/*08*/ u8 state;
/*0C*/ CurveSetup *curveSetup;
} FindKyteObject_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 unk18;
/*19*/ u8 unk19;
/*1A*/ u16 unk1A;
/*1C*/ u8 unk1C;
/*1D*/ u8 unk1D;
/*1E*/ u8 unk1E;
} FindKyteObject_Setup;

// offset: 0x0 | ctor
void FindKyteObject_ctor(void *dll) { }

// offset: 0xC | dtor
void FindKyteObject_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void FindKyteObject_setup(Object *self, ObjSetup *setup, s32 arg2) {
    FindKyteObject_Data *objdata;

    objdata = self->data;
    objdata->state = 0;
    obj_add_object_type(self, OBJTYPE_48);
}

// offset: 0x64 | func: 1 | export: 1
void FindKyteObject_control(Object *self) {
    CurveSetup *curveSetup;
    FindKyteObject_Setup *setup;
    Object *kyte;
    Object *player;
    f32 dist;
    s16 gamebit;
    FindKyteObject_Data *objdata;

    objdata = self->data;
    setup = (FindKyteObject_Setup*)self->setup;
    switch (objdata->state) {
    case 0:
        curveSetup = gDLL_25->vtbl->func_2A50(self, setup->unk1A);
        objdata->curveSetup = curveSetup;
        if (curveSetup) {
            objdata->state = 1;
        }
        break;
    case 1:
        gamebit = objdata->curveSetup->type22.usedBit;
        if ((gamebit != -1) && (main_get_bits(gamebit))) {
            main_set_bits(BIT_Kyte_Flight_Curve, objdata->flightCurve);
            objdata->state = 0;
            break;
        }
        kyte = get_sidekick();
        if (kyte) {
            player = get_player();
            if (setup->unk1E != 0) {
                dist = vec3_distance_xz_squared(&player->positionMirror, &self->positionMirror);
            } else {
                dist = vec3_distance_squared(&player->positionMirror, &self->positionMirror);
            }
            if (dist <= SQ(setup->unk18 * 2)) {
                ((DLL_212_Kyte*)kyte->dll)->vtbl->base.func14(kyte, 1);
                if (gDLL_1_UI->vtbl->func_DF4(1)) {
                    objdata->flightCurve = main_get_bits(BIT_Kyte_Flight_Curve);
                    main_set_bits(BIT_Kyte_Flight_Curve, setup->unk1A);
                    if (setup->unk1C != 0) {
                        main_set_bits(BIT_488, setup->unk1C);
                    }
                    objdata->timer = setup->unk19 * 60.0f;
                    objdata->state = 2;
                }
            }
        }
        break;
    case 2:
        objdata->timer -= gUpdateRateF;
        if ((objdata->timer <= 0.0f) || ((gamebit = objdata->curveSetup->type22.usedBit, (gamebit != -1)) && (main_get_bits(gamebit)))) {
            if (setup->unk1C != 0) {
                main_set_bits(BIT_488, -1);
            }
            objdata->state = 1;
            if (main_get_bits(BIT_Kyte_Flight_Curve) == setup->unk1A) {
                main_set_bits(BIT_Kyte_Flight_Curve, objdata->flightCurve);
            }
        }
        break;
    case 3:
        break;
    }
}

// offset: 0x3AC | func: 2 | export: 2
void FindKyteObject_update(Object *self) { }

// offset: 0x3B8 | func: 3 | export: 3
void FindKyteObject_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x3D0 | func: 4 | export: 7
s32 FindKyteObject_func_3D0(s32 arg0, s32 arg1) {
    return 0;
}

// offset: 0x3E4 | func: 5 | export: 8
s32 FindKyteObject_func_3E4(s32 arg0, s32 arg1, s32 arg2) {
    return 0;
}

// offset: 0x3FC | func: 6 | export: 4
void FindKyteObject_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_48);
}

// offset: 0x43C | func: 7 | export: 5
u32 FindKyteObject_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x44C | func: 8 | export: 6
u32 FindKyteObject_get_data_size(Object *self, u32 a1) {
    return sizeof(FindKyteObject_Data);
}

// offset: 0x460 | func: 9 | export: 9
s32 FindKyteObject_func_460(s32 arg0, s32 arg1, s32 arg2) {
    return 0;
}

// offset: 0x478 | func: 10 | export: 10
s32 FindKyteObject_func_478(s32 arg0) {
    return 1;
}
