#include "common.h"
#include "sys/gfx/modgfx.h"

typedef struct {
    s16 unk0;
    u32 unk4;
    s8 unk8;
} DLL599_Data;

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E; 
} DLL599_Setup;

static DLL_IModgfx* data_0 = NULL;

static int dll_599_func_48C(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 arg3);

// offset: 0x0 | ctor
void dll_599_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_599_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_599_setup(Object* self, DLL599_Setup* setup, s32 reset) {
    DLL599_Data* objdata;

    objdata = self->data;
    self->animCallback = dll_599_func_48C;
    self->srt.yaw = setup->unk18 << 8;
    objdata->unk0 = (s16) setup->unk1E;
    if (data_0 == 0) {
        data_0 = dllLoad(0x1024U, 1U);
    }
}
// offset: 0x98 | func: 1 | export: 1
void dll_599_control(Object* self) {
    Object* polyhit;
    s32 i;
    ObjSetup* objSetup;
    DLL599_Data* objData; //38
    s8 i_2;
    f32 temp_fv0;
    f32 temp_fv0_2;


    objData = self->data;
    objSetup = self->setup;
    objData->unk8--;
    if (objData->unk8 < 0) {
        objData->unk8 = 0;
    }
    
    if (self->polyhits->unk10F > 0) {
        for (i = 0; i < self->polyhits->unk10F; i++) {
            polyhit = self->polyhits->unk100[i];
            temp_fv0 = polyhit->srt.transl.y - self->srt.transl.y;
            if (temp_fv0 > 3.0f) {
                objData->unk8 = 0x3C;
            }
        }
    }
    
    i_2 = 0;
    if (((objData->unk0 == NO_GAMEBIT) || mainGetBits(objData->unk0)) && (objData->unk8 != 0)) {
        temp_fv0_2 = objSetup->y + 100.0f + 20.0f;
        if (self->srt.transl.f[1] > temp_fv0_2) {
            self->srt.transl.f[1] -= (0.5f * gUpdateRateF);
            if (self->srt.transl.f[1] > temp_fv0_2) {
                self->srt.transl.f[1] = temp_fv0_2;
            }
        } else {
            self->srt.transl.f[1] += (0.25f * gUpdateRateF);
            if (self->srt.transl.f[1] > temp_fv0_2) {
                self->srt.transl.f[1] = temp_fv0_2;
            } else {
                i_2 = 1;
            }
        }
    } else {
        self->srt.transl.f[1] -= 0.125f * gUpdateRateF;
        // temp_fv0 = objSetup->y;
        if (self->srt.transl.f[1] < objSetup->y) {
            self->srt.transl.f[1] = objSetup->y;
        } else {
            i_2 = 1;
        }
    }
    
    if (i_2 != 0) {
        if (objData->unk4 == 0) {
            gDLL_6_AMSFX->vtbl->Play(self, 0x1E1, MAX_VOLUME, &objData->unk4, NULL, 0, NULL);
        }
    } else {
        if (mathRnd(0, 0x28) == 0) {
            data_0->vtbl->func0(self, mathRnd(0, 5), 0, 1, -1, 0);
        }

        if (objData->unk4 != 0) {
            gDLL_6_AMSFX->vtbl->Stop(objData->unk4);
            objData->unk4 = 0;
        }
    }
}

// offset: 0x384 | func: 2 | export: 2
void dll_599_update(Object* self) { }

// offset: 0x390 | func: 3 | export: 3
void dll_599_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x3E4 | func: 4 | export: 4
void dll_599_free(Object* self, s32 onlySelf) {
    DLL599_Data* objdata;
    u32 temp_a0;

    objdata = self->data;
    if (data_0 != 0) {
        dllFree( data_0);
    }
    temp_a0 = objdata->unk4;
    if (temp_a0 != 0) {
        gDLL_6_AMSFX->vtbl->Stop(temp_a0);
    }
}


// offset: 0x468 | func: 5 | export: 5
u32 dll_599_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x478 | func: 6 | export: 6
u32 dll_599_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DLL599_Data);
}

// offset: 0x48C | func: 7
static int dll_599_func_48C(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    animObjData->unk7A = -1;
    animObjData->unk62 = 0;
    return 0;
}
