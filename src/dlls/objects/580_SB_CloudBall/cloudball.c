#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "sys/objhits.h"

typedef struct {
    s32 _unk0;
    Object* unk4;
    u8 _unk8 [0x34 - 0x08];   
} CloudBall_Data;

// offset: 0x0 | ctor
void dll_580_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_580_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
// offset: 0x18 | func: 0 | export: 0
void dll_580_Setup(Object* self, ObjSetup* setup, s32 reset) {
    CloudBall_Data* objdata;
    ObjectHitInfo* temp_v1;

    objdata = self->data;
    temp_v1 = self->objhitInfo;
    temp_v1->unk58 &= 0xFFFE;
    objdata->unk4 = 0;
}

// offset: 0x40 | func: 1 | export: 1
void dll_580_obj_Control(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/580_SB_CloudBall/dll_580_obj_Control.s")

// offset: 0x4C0 | func: 2 | export: 2
void dll_580_Update(Object* self) {
    ObjectHitInfo* temp_v0;
    s32 var_s0;

    var_s0 = 0x14;
    if (self->objhitInfo->unk48 != NULL) {
        gDLL_6_AMSFX->vtbl->Play(NULL, SOUND_95_Explosion, MAX_VOLUME, NULL, NULL, 0, NULL);
        temp_v0 = self->objhitInfo;
        temp_v0->unk58 &= 0xFFFE;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_A7, NULL, 1, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_A7, NULL, 1, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_A7, NULL, 1, -1, NULL);
        do {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_AA, NULL, 1, -1, NULL);
            var_s0 -= 1;
        } while (var_s0 != 0);
        objFreeObject (self);
    }
}

// offset: 0x644 | func: 3 | export: 3
void dll_580_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x65C | func: 4 | export: 4
void dll_580_free(Object* self, s32 onlySelf) {
    CloudBall_Data* objdata;
    Object* temp_a0;

    objdata = self->data;
    gDLL_14_Modgfx->vtbl->Func5(self);
    gDLL_13_Expgfx->vtbl->func5(self);
    if (onlySelf == 0) {
        temp_a0 = objdata->unk4;
        if (temp_a0 != NULL) {
            objFreeObject (temp_a0);
            objdata->unk4 = NULL;
        }
    }
}

// offset: 0x704 | func: 5 | export: 5
u32 dll_580_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x714 | func: 6 | export: 6
u32 dll_580_get_data_size(Object* self, u32 offsetAddr){
    return sizeof (CloudBall_Data);
}