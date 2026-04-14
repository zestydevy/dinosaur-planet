#include "common.h"
#include "game/gamebits.h"
#include "sys/gfx/model.h"

typedef struct { 
    ObjSetup base;
    s8 unk18;
    u8 _unk19;
    s16 unk1A;
    s16 unk1C;
    s16 _unk1E; 
    s16 unk20;
} DLL279_Setup;

typedef struct {
    u8 _unk0 [0xC - 0x00];
    s32 unkC;
    u8 unk10 [0x19- 0x10];
    u8 unk19;
    //[0x1C];
} DLL279_Data;

// offset: 0x0 | ctor
void dll_279_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_279_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_279_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/279_Door/dll_279_setup.s")

// offset: 0x16C | func: 1 | export: 1
void dll_279_control(Object* self) {
    DLL279_Setup* setup;
    DLL279_Data* objdata;

    objdata = self->data;
    objdata->unk19 = 0;
    if (self->unkDC == 0) {
        setup = (DLL279_Setup*)self->setup;
        self->srt.transl.x = setup->base.x;
        self->srt.transl.y = setup->base.y;
        self->srt.transl.z = setup->base.z;
        self->srt.yaw = setup->unk18 << 8;

        if (self->id == OBJ_WL_Door1) {
            if (main_get_bits(objdata->unkC)) {
                gDLL_3_Animation->vtbl->func20(self, 0x82);
                objdata->unk19 = 1;
            }
            gDLL_3_Animation->vtbl->func17(0, self, -1);
        } else if (self->id == OBJ_WL_WarpDoor) {
            if (main_get_bits(objdata->unkC)) {
                gDLL_3_Animation->vtbl->func20(self, 0x75);
                objdata->unk19 = 1;
            }
            gDLL_3_Animation->vtbl->func17(0, self, -1);
        } else {
            gDLL_3_Animation->vtbl->func17(0, self, -1);
        }
        self->unkDC = 1;
    }
}

// offset: 0x308 | func: 2 | export: 2
void dll_279_update(Object *self) { }

// offset: 0x314 | func: 3 | export: 3
void dll_279_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x368 | func: 4 | export: 4
void dll_279_free(Object *self, s32 a1) { }

// offset: 0x378 | func: 5 | export: 5
u32 dll_279_get_model_flags(Object* self) {
    return MODFLAGS_1;
}

// offset: 0x388 | func: 6 | export: 6
u32 dll_279_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL279_Data);
}

// offset: 0x39C | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/279_Door/dll_279_func_39C.s")

// offset: 0x1034 | func: 8
void dll_279_func_1034(Vec3f* arg0, DLL279_Setup* objSetup) {
    Camera* camera;
    f32 f0;
    u8 temp;

    camera = get_main_camera();
    
    f0 = (arg0->x * camera->srt.transl.x) + (arg0->y * camera->srt.transl.z) + arg0->z;
    if (f0 < 0.0f) {
        if (objSetup->unk20 != -1) {
            temp = main_get_bits(objSetup->unk20) ^ (u8)objSetup->unk1C;
            if ((u8)objSetup->unk1C) {}
            main_set_bits(objSetup->unk20, temp & 0xFF & 0xFF);
        }
    } else {
        if (objSetup->unk1A != -1) {
            temp = main_get_bits(objSetup->unk1A) ^ (u8)(objSetup->unk1C >> 8);
            if (objSetup->unk1C) {}
            main_set_bits(objSetup->unk1A, temp & 0xFF & 0xFF);
        }
    }
}
