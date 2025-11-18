#include "common.h"

void dl_clear_geometry_mode(Gfx **gdl, u32 mode);
void dl_set_geometry_mode(Gfx **gdl, u32 mode);

typedef struct {
    ObjSetup base;
    s8 unk18; //yaw
    s8 unk19; //modelIdx
    s16 unk1A;
    s16 unk1C;
} DLL612_Setup;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16* unk8;
    void* unkC;
    s8 unk10;
    u8 unk11;
} DLL612_Data; //0x14

/*0x0*/ static void* _data_0 = NULL; //dll ptr
/*0x4*/ static f32 _data_4[] = {
    0, 0, -1
};
/*0x10*/ static f32 _data_10[] = {
    0, 0, -1
};
/*0x1C*/ static f32 _data_1C = 0.0;
/*0x20*/ static f32 _data_20[] = {
    0.0, 0.0, 0.0, 0.0
};

/*0x0*/ static s16 _bss_0;
/*0x2*/ static s16 _bss_2;
/*0x4*/ static s16 _bss_4;
/*0x6*/ static s16 _bss_6;
/*0x8*/ static s16 _bss_8;

// offset: 0x0 | ctor
void dll_612_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_612_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_612_setup(Object* self, DLL612_Setup* objSetup, s32 arg2) {
    DLL612_Data* objData;
    s16 i;
    ModelInstance* temp_s3;
    Vtx_t *ctx;

    objData = self->data;
    
    if ((gDLL_29_Gplay->vtbl->get_map_setup(self->mapID) == 3) && (main_get_bits(0x21B) == 0)) {
        main_set_bits(0x21B, 1);
    }
    
    objData->unk8 = NULL;
    objData->unkC = NULL;
    
    objData->unk11 = 1;
    
    if (self->id == OBJ_WL_Crystal){
        self->srt.yaw = objSetup->unk18 << 8;
        objData->unk2 = 0x64;
        if (objSetup->unk1C >= 0x3E8) {
            self->srt.scale = objSetup->unk1C / 1000.0f;
        } else {
            self->srt.scale = 1.0f;
        }
        return;
    }

    if (self->id == OBJ_WMsun){
        _bss_0 = 0x320;
        _bss_2 = 0x320;
        _bss_4 = 0x320;
        _bss_6 = 0x320;
        _bss_8 = 0x320;
        self->srt.yaw = objSetup->unk18 << 8;
        if (objSetup->unk1C >= 0) {
            self->srt.scale = objSetup->unk1C / 1000.0f;
        } else {
            self->srt.scale = 1.0f;
        }
        self->modelInstIdx = objSetup->unk19;
        
        if (self->modelInstIdx == 0) {
            objData->unkC = dll_load_deferred(0x2012, 1);
            objData->unk2 = rand_next(0x12C, 0x258);
            objData->unk4 = rand_next(0x12C, 0x258);
            _data_0 = dll_load_deferred(0x1036, 1);
        } else {
            if (self->modelInstIdx == 1) {
                objData->unk2 = rand_next(0x1F4, 0x320);
                objData->unk4 = rand_next(0x1F4, 0x320);
            } else if (self->modelInstIdx == 2) {
                objData->unkC = dll_load_deferred(0x2012, 1);
                objData->unk2 = rand_next(0x2BC, 0x3E8);
                objData->unk4 = rand_next(0x2BC, 0x3E8);
            }
        }
        self->opacity = 0;
        return;
    }

    //Unused room-filling electrified wall effect for main chamber of WM
    //(model uses massive-resolution animated texture incompatible with N64 TMEM cache)
    if (self->id == OBJ_WMinroom){
        temp_s3 = self->modelInsts[self->modelInstIdx];
        objData->unk8 = mmAlloc(0xA0, 0xE, NULL);

        i = 0x14;
        while (i != 0) {
            i--;
            objData->unk8[i] = rand_next(0, temp_s3->model->vertexCount - 1);
            objData->unk8[i + 20] = 0;
            objData->unk8[i + 40] = rand_next(0xA, 0x14);
            objData->unk8[i + 60] = rand_next(0x50, 0xFF);
        }
        
        temp_s3 = self->modelInsts[self->modelInstIdx];
        ctx = (Vtx_t *)temp_s3->unk4[0];
        i = temp_s3->model->vertexCount;
        while (i != 0) {
            i--;
            ctx[i].cn[3] = 0x55;
        }
        
        
        ctx = (Vtx_t *)temp_s3->unk4[1];
        i = temp_s3->model->vertexCount;
        while (i != 0) {
            i--;
            ctx[i].cn[3] = 0x55;
        }
        
        self->opacity = 0;
        if (objSetup->unk1C != 0) {
            self->srt.scale = 1.0f / (objSetup->unk1C / 1000.0f);
        }
    }
}

// offset: 0x47C | func: 1 | export: 1
void dll_612_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/612_WL_Crystal/dll_612_control.s")

// offset: 0x1604 | func: 2 | export: 2
void dll_612_update(Object *self) { }

// offset: 0x1610 | func: 3 | export: 3
void dll_612_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    DLL612_Data* objData = self->data;
    
    if (visibility && objData->unk11) {
        dl_clear_geometry_mode(gdl, 0x10000);
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        dl_set_geometry_mode(gdl, 0x10000);
    }
}

// offset: 0x16C4 | func: 4 | export: 4
void dll_612_free(Object* self, s32 arg1) {
    DLL612_Data* objData;

    objData = self->data;
    
    if (_data_0 && (self->id == OBJ_WMsun) && (self->modelInstIdx == 0)) {
        dll_unload(_data_0);
        _data_0 = NULL;
    }
    
    if (objData->unk8 != NULL) {
        mmFree(objData->unk8);
    }
    objData->unk8 = 0;
    
    if (objData->unkC != NULL) {
        dll_unload(objData->unkC);
    }
    objData->unkC = 0;
}

// offset: 0x1794 | func: 5 | export: 5
u32 dll_612_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x17A4 | func: 6 | export: 6
u32 dll_612_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL612_Data);
}

// offset: 0x17B8 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/612_WL_Crystal/dll_612_func_17B8.s")
