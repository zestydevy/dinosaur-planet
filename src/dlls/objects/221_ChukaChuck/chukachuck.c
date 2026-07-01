#include "common.h"
#include "dlls/engine/17_partfx.h"
#include "game/objects/object.h"
#include "sys/math.h"
#include "sys/objhits.h"

    //TODO: move into headers:
    DLL_INTERFACE(DLL_221_Chuka) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*07*/ u8 (*func7)(Object* self, u8 arg1);
    };

    DLL_INTERFACE(DLL_100) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*07*/ UnknownDLLFunc func7;
    /*08*/ u8 (*func8)(Object* self, u8 arg1);
    };

    DLL_INTERFACE(DLL_778) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*07*/ UnknownDLLFunc func7;
    /*08*/ u8 (*func8)(Object* self, u8 arg1, s32 arg2);
    };

typedef struct {
    s16 unk0;
} DLL221_Data;

static void dll_221_func_398(Object* self);
static void dll_221_func_620(Object* self);
static int dll_221_func_77C(Object* self);

// offset: 0x0 | ctor
void dll_221_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_221_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_221_setup(Object* self, ObjSetup* setup, s32 reset) {
    self->unkDC = 180;
    func_800267A4(self);
    self->opacity = OBJECT_OPACITY_MAX;
    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR;
        self->shadow->maxDistScale = self->shadow->scale * 2.5f;
    }
}

// offset: 0xA0 | func: 1 | export: 1
void dll_221_control(Object* self) {
    self->unkDC -= gUpdateRateF;
    if (self->unkDC < 0) {
        obj_destroy_object(self);
        return;
    }
    
    if (self->opacity == 0) {
        return;
    }
    
    self->velocity.y -= 0.07f * gUpdateRateF;
    self->velocity.y *= 0.97f;
    self->srt.yaw += M_5_DEGREES;
    self->srt.roll += M_5_DEGREES;
    self->srt.pitch += M_5_DEGREES;
    obj_move(self, self->velocity.x * gUpdateRateF, self->velocity.y * gUpdateRateF, self->velocity.z * gUpdateRateF);
    
    func_80026128(self, Damage_Type_Sword_Staff_Strike1, 1, 0);
    func_80026940(self, 5);
    func_8002674C(self);
    
    if (self->objhitInfo->unk48 && 
        ((get_player() == self->objhitInfo->unk48) || (get_sidekick() == self->objhitInfo->unk48))
    ) {
        dll_221_func_398(self);
        self->opacity = 0;
        self->unkDC = 120;
        self->objhitInfo->unk58 &= ~1;
        return;
    }
    
    if (self->objhitInfo->unk9D != 0) {
        dll_221_func_620(self);
        self->opacity = 0;
        self->unkDC = 120;
        self->objhitInfo->unk58 &= ~1;
    }
}

// offset: 0x2D4 | func: 2 | export: 2
void dll_221_update(Object* self) { }

// offset: 0x2E0 | func: 3 | export: 3
void dll_221_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x334 | func: 4 | export: 4
void dll_221_free(Object* self, s32 onlySelf) {
    camera_disable_y_offset();
}

// offset: 0x374 | func: 5 | export: 5
u32 dll_221_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x384 | func: 6 | export: 6
u32 dll_221_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DLL221_Data);
}

// offset: 0x398 | func: 7
void dll_221_func_398(Object* self) {
    s32 i;

    camera_enable_y_offset();
    camera_set_shake_offset(1.0f);
    gDLL_6_AMSFX->vtbl->play(self, SOUND_4B1, MAX_VOLUME, NULL, NULL, 0, NULL);
    
    if (self->id == OBJ_ChukaChuck) {
        if (self->unkC4 && dll_221_func_77C(self->unkC4)) {
            ((DLL_221_Chuka*)self->unkC4->dll)->vtbl->func7(self->unkC4, 0x80);
        }
        
        for (i = 0; i < 25; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x340, NULL, 1, -1, NULL);
        }
        return;
    }
    
    if (self->id == OBJ_IceBall) {
        if (self->unkC4 && dll_221_func_77C(self->unkC4)) {
            ((DLL_100*)self->unkC4->dll)->vtbl->func8(self->unkC4, 0x80);
        }
        
        for (i = 0; i < 25; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x343, NULL, 1, -1, NULL);
        }
        return;
    }
    
    if (self->id == OBJ_IceBallSmall) {
        if (self->unkC4 && dll_221_func_77C(self->unkC4)) {
            ((DLL_778*)self->unkC4->dll)->vtbl->func8(self->unkC4, 0x80, 0);
        }
        
        for (i = 0; i < 25; i++) {
            gDLL_17_partfx->vtbl->spawn(self, 0x343, NULL, 1, -1, NULL);
        }
        return;
    }
}

// offset: 0x620 | func: 8
void dll_221_func_620(Object* self) {
    s32 i;

    if (self->id == OBJ_ChukaChuck) {
        for (i = 0; i < 25; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_342, NULL, 1, -1, NULL);
        }
    } else {
        if ((self->id == OBJ_IceBall) || (self->id == OBJ_IceBallSmall)) {
            for (i = 0; i < 25; i++) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_344, NULL, 1, -1, NULL);
            }
        }
    }
    
    gDLL_6_AMSFX->vtbl->play(self, SOUND_4B1, 0x40, NULL, NULL, 0, NULL);
    camera_enable_y_offset();
    camera_set_shake_offset(1.0f);
}

// offset: 0x77C | func: 9
int dll_221_func_77C(Object* self) {
    s32 index;
    s32 count;
    Object** objects;

    objects = get_world_objects(&index, &count);
    while (index < count) {
        if (self == objects[index++]) {
            return TRUE;
        }
    }
    return FALSE;
}
