#include "game/objects/setups/51_Generic.h"
#include "sys/curves.h"
#include "sys/main.h"
#include "sys/objprint.h"
#include "dll.h"

// size:0x118
typedef struct {
/*000*/ UnkCurvesStruct unk0;
/*108*/ f32 unk108;
/*10C*/ f32 unk10C;
/*110*/ f32 unk110;
/*114*/ u8 _unk114[0x118 - 0x114];
} WaterLily_Data;

// offset: 0x0 | ctor
void dll_509_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_509_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_509_setup(Object* self, Generic_Setup* setup, s32 arg2) {
    s32 sp34 = 0x19;
    WaterLily_Data* objdata = self->data;
  
    if (setup->type == 1) {
        gDLL_26_Curves->vtbl->func_4288(&objdata->unk0, self, 1000.0f, &sp34, -1);
        self->srt.transl.x = objdata->unk0.unk0.unk68.x;
        self->srt.transl.z = objdata->unk0.unk0.unk68.z;
        objdata->unk108 = (setup->genericA / 10.0f);
    }
    
    self->srt.yaw = setup->yaw << 8;
    self->srt.transl.y = setup->base.y - 1.5f;
    objdata->unk10C = self->srt.transl.y;
}

// offset: 0x110 | func: 1 | export: 1
void dll_509_control(Object* self) {
    WaterLily_Data* objdata = self->data;
    s32 pad;
    Generic_Setup *setup = (Generic_Setup*)self->setup;
    f32 temp;

    if (setup->type == 1) {
        if ((curves_func_800053B0(&objdata->unk0.unk0, objdata->unk108) != 0) || (objdata->unk0.unk0.unk10 != 0)) {
            gDLL_26_Curves->vtbl->func_4704(&objdata->unk0);
        }
        self->srt.transl.x = objdata->unk0.unk0.unk68.x;
        self->srt.transl.z = objdata->unk0.unk0.unk68.z;
    }
    if (objdata->unk110 > 0.0f) {
        objdata->unk110 = (f32) (objdata->unk110 - gUpdateRateF);
        if (objdata->unk110 <= 0.0f) {
            self->velocity.y = -0.25f;
        }
    }

    temp = setup->base.y - self->srt.transl.y;
    self->velocity.y += temp * 0.25f * 0.08f * gUpdateRateF;
    self->velocity.y -= 0.05f * gUpdateRateF;
    self->velocity.y *= 0.9f;
    if (self->velocity.y > 1.4f) {
        self->velocity.y = 1.4f;
    }
    self->srt.transl.y += self->velocity.y * gUpdateRateF;
}

// offset: 0x2C8 | func: 2 | export: 2
void dll_509_update(Object *self) { }

// offset: 0x2D4 | func: 3 | export: 3
void dll_509_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x328 | func: 4 | export: 4
void dll_509_free(Object *self, s32 a1) { }

// offset: 0x338 | func: 5 | export: 5
u32 dll_509_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x348 | func: 6 | export: 6
u32 dll_509_get_data_size(Object* self, u32 a1) {
    return sizeof(WaterLily_Data);
}

/*0x0*/ static const char str_0[] = "LILY ERROR: no node found\n";
