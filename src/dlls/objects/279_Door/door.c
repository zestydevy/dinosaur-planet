#include "common.h"

typedef struct {
    u8 _unk0 [0xC - 0x00];
    s32 unkC;
    u8 unk10 [0x19- 0x10];
    u8 unk19;
    //[0x1C];
} DLL279_Data;

typedef struct {
    ObjSetup base;
    s8 unk18;
}DLL279_Setup;

// offset: 0x0 | ctor
void dll_279_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_279_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_279_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/279_Door/dll_279_setup.s")

//void dll_279_setup(Object* self, DLL279_Setup* setup, s32 arg2) {
 //   DLL279_Data* objdata; 
 //   s16 temp_v0; 
//
 ///   objdata = self->data;
 //   obj_init_mesg_queue(self, 4U);
 //   self->srt.yaw = setup->unk18 << 8;
 //   self->animCallback = dll_279_func_39C;
 //   self->unkAF |= 8;
 //   self->unkB0 |= 0x6000;
 //   objdata->unkC = (s32) setup->unk1E;
 //   objdata->unk14 = (s32) setup->unk20;
 //   temp_v0 = self->id;
 //   switch (temp_v0) {                              /* irregular */
 //   default:
 //       objdata->unk10 = -1;
 //       break;
 //   case 0xC1:
 //   case 0xC4:
 //       objdata->unk10 = 0x44;
//        break;
//case 0x11B:
//    case 0x11C:
//        objdata->unk10 = 0x98;
//        break;
//    }
//    obj_add_object_type(self, 0x10);
 //   objdata->unk0 = fsin16_precise(self->srt.yaw);
  //  objdata->unk4 = fcos16_precise(self->srt.yaw);
 //   objdata->unk8 = (f32) -((objdata->unk0 * self->srt.transl.x) + (objdata->unk4 * self->srt.transl.z));
//}

// offset: 0x16C | func: 1 | export: 1
void dll_279_control(Object* self) {
    DLL279_Setup* setup;
    s16 temp_v1;
    DLL279_Data* objdata;

    objdata = self->data;
    objdata->unk19 = 0;
    if (self->unkDC == 0) {
        setup = (DLL279_Setup*)self->setup;
        temp_v1 = self->id;
        self->srt.transl.f[0] = setup->base.x;
        self->srt.transl.f[1] = setup->base.y;
        self->srt.transl.f[2] = setup->base.z;
        self->srt.yaw = setup->unk18 << 8;
        if (temp_v1 == 0x13E) {
            if (main_get_bits(objdata->unkC) != 0) {
                gDLL_3_Animation->vtbl->func20(self, 0x82);
                objdata->unk19 = 1;
            }
            gDLL_3_Animation->vtbl->func17(0, self, -1);
        } else if (temp_v1 == 0x151) {
            if (main_get_bits(objdata->unkC) != 0) {
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
    return 1U;
}

// offset: 0x388 | func: 6 | export: 6
u32 dll_279_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL279_Data);
}

// offset: 0x39C | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/279_Door/dll_279_func_39C.s")

// offset: 0x1034 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/279_Door/dll_279_func_1034.s")
