#include "common.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
} DLL302_Setup;

typedef struct {
    f32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} DLL302_Data;

static void dll_302_func_344(Object* self);
static int dll_302_func_42C(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 arg3);

// offset: 0x0 | ctor
void dll_302_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_302_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_302_setup(Object* self, DLL302_Setup* objSetup, s32 reset) {
    DLL302_Data* objData = self->data;
    
    self->srt.yaw = objSetup->unk18 << 8;
    
    objData->unk6 = objSetup->unk20;
    objData->unk4 = objSetup->unk1E;
    objData->unk0 = objSetup->unk1A;
    
    self->stateFlags |= OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED;
    self->animCallback = dll_302_func_42C;
    self->srt.transl.y = objSetup->base.y + objData->unk0;
    
    obj_set_model(self, objSetup->unk19);
    
    objData->unk8 = 0;
}

// offset: 0xC4 | func: 1 | export: 1
void dll_302_control(Object* self) {
    DLL302_Setup* objSetup;
    DLL302_Data* objData;
    f32 magnitude;

    objSetup = (DLL302_Setup*)self->setup;
    objData = self->data;
    
    if (self->id == OBJ_VFP_Ladders2) {
        dll_302_func_344(self);
        return;
    }
    
    if (objData->unkA != 0) {
        objData->unkA -= (s16)gUpdateRateF;
        if (objData->unkA <= 0) {
            objData->unk8 = 1;
            gDLL_6_AMSFX->vtbl->play(self, 0x6E9, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->unkA = 0;
        }
        return;
    }
        
    if (objData->unk8 == 0) {
        if (main_get_bits(objData->unk6)) {
            objData->unkA = 10;
        }
    }
    
    if (objData->unk8 != 1) {
        return;
    }

    if (objSetup->base.y <= self->srt.transl.y) {
        self->velocity.y -= 0.9f;
        self->srt.transl.y += self->velocity.y * gUpdateRateF;

        if (self->srt.transl.y <= objSetup->base.y) {
            self->srt.transl.y = objSetup->base.y;
            self->velocity.y = -self->velocity.y * 0.3f;

            if (self->velocity.y >= 0.0f) {
                magnitude = self->velocity.y;
            } else {
                magnitude = -self->velocity.y;
            }
            if (magnitude < 0.01f) {
                objData->unk8 = 2;
            }
        }
    }
}

// offset: 0x2B4 | func: 2 | export: 2
void dll_302_update(Object *self) { }

// offset: 0x2C0 | func: 3 | export: 3
void dll_302_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x2D8 | func: 4 | export: 4
void dll_302_free(Object* self, s32 onlySelf) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x320 | func: 5 | export: 5
u32 dll_302_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x330 | func: 6 | export: 6
u32 dll_302_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DLL302_Data);
}

// offset: 0x344 | func: 7
void dll_302_func_344(Object* self) {
    DLL302_Data* objData = self->data;
    
    if (main_get_bits(objData->unk6) && (main_get_bits(objData->unk4) == FALSE)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
    }
    
    if ((main_get_bits(objData->unk6) == FALSE) && main_get_bits(objData->unk4)) {
        gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
    }
}

// offset: 0x42C | func: 8
int dll_302_func_42C(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    return 0;
}
