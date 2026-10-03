#include "sys/dll.h"
#include "sys/main.h"
#include "sys/gfx/modgfx.h"
#include "sys/objprint.h"
#include "sys/rand.h"
#include "game/gamebits.h"
#include "dll.h"

typedef struct {
    s16 unk0;
    u32 unk4;
    s8 unk8;
} WLColrise_Data;

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E; 
} WLColrise_Setup;

static DLL_IModgfx* data_0 = NULL;

static int WL_colrise_animCallback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 arg3);

// offset: 0x0 | ctor
void WL_colrise_ctor(void* dll) { }

// offset: 0xC | dtor
void WL_colrise_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_colrise_setup(Object* self, WLColrise_Setup* setup, s32 reset) {
    WLColrise_Data* objdata = self->data;

    self->animCallback = WL_colrise_animCallback;
    self->srt.yaw = setup->unk18 << 8;
    objdata->unk0 = setup->unk1E;
    if (data_0 == 0) {
        data_0 = dllLoad(DLL_ID_140, 1);
    }
}

// offset: 0x98 | func: 1 | export: 1
void WL_colrise_control(Object* self) {
    Object* polyhit;
    s32 i;
    ObjSetup* objSetup;
    WLColrise_Data* objData;
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
        if (self->srt.transl.f[1] < objSetup->y) {
            self->srt.transl.f[1] = objSetup->y;
        } else {
            i_2 = 1;
        }
    }
    
    if (i_2 != 0) {
        if (objData->unk4 == 0) {
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_1E1_Stone_Moving_Loop, MAX_VOLUME, &objData->unk4, NULL, 0, NULL);
        }
    } else {
        if (mathRnd(0, 40) == 0) {
            data_0->vtbl->Spawn(self, mathRnd(0, 5), 0, 1, -1, 0);
        }

        if (objData->unk4 != 0) {
            gDLL_6_AMSFX->vtbl->Stop(objData->unk4);
            objData->unk4 = 0;
        }
    }
}

// offset: 0x384 | func: 2 | export: 2
void WL_colrise_update(Object* self) { }

// offset: 0x390 | func: 3 | export: 3
void WL_colrise_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x3E4 | func: 4 | export: 4
void WL_colrise_free(Object* self, s32 onlySelf) {
    WLColrise_Data* objdata;

    objdata = self->data;
    if (data_0 != 0) {
        dllFree( data_0);
    }
    if (objdata->unk4 != 0) {
        gDLL_6_AMSFX->vtbl->Stop(objdata->unk4);
    }
}


// offset: 0x468 | func: 5 | export: 5
u32 WL_colrise_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x478 | func: 6 | export: 6
u32 WL_colrise_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(WLColrise_Data);
}

// offset: 0x48C | func: 7
static int WL_colrise_animCallback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    animObjData->unk7A = -1;
    animObjData->unk62 = 0;
    return 0;
}
