#include "common.h"
#include "sys/gfx/modgfx.h"



typedef struct {
    s16 _unk0;
    s16 unk2;
    s8 unk4;
    s8 _unk5;
    s16 unk6;
    u8 unk8; 
    s8 _unk9;
    s16 _unkA;
    s32 _unkC;
    SRT unk10;
}DLL664_Data;

typedef struct {
    ObjSetup base;
    s16 unk18;
    u8 unk1A;
    u8 unk1B;
} DLL664_Setup;

// offset: 0x0 | ctor
void dll_664_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_664_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_664_obj_Setup(Object* self, DLL664_Setup* setup, s32 reset) {
    DLL664_Data* sp24;
    f32 var_ft1;
    u8 temp_v0;

    temp_v0 = setup->unk1A;
    sp24 = self->data;
    if (temp_v0 != 0) {
        var_ft1 =  temp_v0;
        self->srt.scale = var_ft1 * 0.01f;
    }
    sp24->unk4 = (u8) setup->unk1B;
    sp24->unk6 = 0;
    sp24->unk8 = 0;
    sp24->unk2 = mathRnd(0, 0x3E8);
    self->visRadius = 200.0f;
}

// offset: 0xD0 | func: 1 | export: 1
void dll_664_obj_Control(Object* self);
void dll_664_obj_Control(Object* self) {
    DLL664_Data* objdata = self->data;
    DLL_IModgfx* dll;
    
    if (objdata->unk8 < 2) {
        objdata->unk6 -= gUpdateRate;
        if (objdata->unk2 > 0) {
            objdata->unk2 -= gUpdateRate;
        }
        if (objdata->unk6 < 0) {
            if (objdata->unk8 != 0) {
                mathRnd(11, 12);
                dll = dllLoad(0x103A, 1);
                dll->vtbl->func0(self, 0, 0, 1, -1, 0);
                dllFree(dll);
            } else {
                mathRnd(11, 12);
                dll = dllLoad(0x103A, 1);
                dll->vtbl->func0(self, 0, 0, 1, -1, 0);
                dllFree(dll);
            }
            objdata->unk8 ^= 1;
            objdata->unk6 = 100;
        }
        if (objdata->unk2 <= 0) {
            objdata->unk2 = 1500;
        }
        if (mainGetBits(0x432) != 0) {
            objdata->unk8 = 2;
            objdata->unk10.transl.y = mathRnd(0, 10) + 70.0f;
            objdata->unk10.transl.x = 0;
            objdata->unk10.transl.z = 0;
        }
    } else if (objdata->unk8 == 2) {
        objdata->unk10.roll = mathRnd(0, 0x28) + 0x64;
        objdata->unk10.yaw = 0;
        objdata->unk10.transl.y -= 1.0f;
        if (((s32) objdata->unk10.transl.y % 10) == 0) {
            gDLL_14_Modgfx->vtbl->Func10(self);
            dll = dllLoad(0x1020, 1);
            dll->vtbl->func0(self, 0x20, &objdata->unk10, 0, -1, 0);
            dllFree(dll);
        }
        if (objdata->unk10.transl.y < 0.0f) {
            objdata->unk2 = 1500;
            objdata->unk8 = 3;
        }
        if (mathRnd(0, 10) == 0) {
            gDLL_17_partfx->vtbl->spawn(self, 0x3B9, NULL, 0, -1, NULL);
        }
    } else if (objdata->unk8 == 3) {
        objdata->unk2 -= gUpdateRate;
        if ((objdata->unk2 < 0) && (mainGetBits(0x437) == 0)) {
            objdata->unk2 = mathRnd(0, 1000);
            objdata->unk6 = 0;
            objdata->unk8 = 0;
        }
    }
}

// offset: 0x49C | func: 2 | export: 2
void dll_664_obj_Update(Object* self) { }

// offset: 0x4A8 | func: 3 | export: 3
void dll_664_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x4C0 | func: 4 | export: 4
void dll_664_obj_Free(Object* self, s32 onlySelf);
void dll_664_obj_Free(Object* self, s32 onlySelf) {
    gDLL_14_Modgfx->vtbl->Func5(self);
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x530 | func: 5 | export: 5
s32 dll_664_obj_GetModelFlags(s32 arg0) {
    return MODFLAGS_1;
}

// offset: 0x540 | func: 6 | export: 6
s32 dll_664_obj_GetDataSize(s32 arg0, s32 arg1) {
    return sizeof(DLL664_Data);
}

