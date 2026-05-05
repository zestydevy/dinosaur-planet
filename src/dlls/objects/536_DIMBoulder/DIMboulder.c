#include "common.h"
#include "sys/gfx/model.h"
#include "sys/math.h"
#include "sys/objhits.h"

/*0x0*/ static Vec3f data_0[] = {
    VEC3F(-0.5, 0.6, -0.2), 
    VEC3F(-0.5, 1.2, -0.2), 
    VEC3F(-1, 2, -2), 
    VEC3F(0, 0, 0)
};

typedef struct {
    ObjSetup base;
    s8 unk18;
} DIMBoulder_Setup;

typedef struct {
    f32 unk0;
    s16 unk4;
    u8 unk6;
    u8 unk7;
} DIMBoulder_Data;

// offset: 0x0 | ctor
void dll_536_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_536_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_536_setup(Object* self, DIMBoulder_Setup* objSetup, s32 arg2) {
    DIMBoulder_Data* objData;

    objData = self->data;
    self->srt.yaw = objSetup->unk18 << 8;
    if (self->objhitInfo != NULL) {
        self->objhitInfo->unk5B = 0;
    }
    
    objData->unk0 = 1.0f;
    if (self->shadow != NULL) {
        self->shadow->flags |= 0x800 | 0x10;
    }
    
    self->stateFlags |= OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED;
    
    objData->unk7 = main_get_bits(BIT_4CE);
    if ((objData->unk7) == 3) {
        objData->unk6 = 2;
    }
}

// offset: 0xD0 | func: 1 | export: 1
void dll_536_control(Object* self) {
    f32 pad2;
    f32 var_fa1;
    s32 pad[2];
    f32 var_fv1;
    f32 var_fv0;
    Vec3f spEC;
    Vec3f spE0;
    Vec3f spD4;
    Vec3f spC8;
    DIMBoulder_Data* objData;
    f32 pad1;
    s32 spBC;
    s32 volume;
    Unk80027934 sp4C;
    AABBs32 sp34;

    objData = self->data;
    spBC = gUpdateRate;
    var_fv0 = gUpdateRateF;

    if (objData->unk6 == 0) {
        if ((func_80025F40(self, NULL, NULL, NULL) == Damage_Type_Explosion) || 
            ((joy_get_buttons(1) & A_BUTTON)) //Move the snowball with A button presses on Controller 2!
        ) {
            objData->unk6 = 1;
            self->velocity.f[0] = data_0[objData->unk7].f[0];
            self->velocity.f[1] = data_0[objData->unk7].f[1];
            self->velocity.f[2] = data_0[objData->unk7].f[2];
            objData->unk7++;
            main_set_bits(BIT_4CE, objData->unk7);
        }
    } else if (objData->unk6 != 2) {
        self->velocity.f[1] -= 0.09f * var_fv0;
        obj_move(self, 
            self->velocity.f[0] * var_fv0, 
            self->velocity.f[1] * var_fv0, 
            self->velocity.f[2] * var_fv0
        );
        
        spEC.x = self->prevLocalPosition.x - 80.0f;
        spEC.y = self->prevLocalPosition.y - 80.0f;
        spEC.z = self->prevLocalPosition.z - 80.0f;
        spE0.x = self->srt.transl.x + 80.0f;
        spE0.y = self->srt.transl.y + 80.0f;
        spE0.z = self->srt.transl.z + 80.0f;
        sp4C.unk50[0] = -1;
        sp4C.unk54[0] = 5;
        sp4C.unk40[0] = 55.0f;
        fit_aabb_around_cubes(&sp34, &spEC, &spE0, &sp4C.unk40[0], 1);
        func_80053750(self, &sp34, 0);
        if (func_8005509C(self, &self->prevLocalPosition.x, &self->srt.transl.x, 1, &sp4C, 0) != 0) {
            spD4.f[0] = -self->velocity.f[0];
            spD4.f[1] = -self->velocity.f[1];
            spD4.f[2] = -self->velocity.f[2];
            var_fa1 = sqrtf(SQ(spD4.f[0]) + SQ(spD4.f[1]) + SQ(spD4.f[2]));
            if (var_fa1 < 0.3f) {
                var_fa1 = 0.0f;
                spD4.f[0] = 0.0f;
                spD4.f[1] = 0.0f;
                spD4.f[2] = 0.0f;
                if (objData->unk7 == 3) {
                    objData->unk6 = 2;
                } else {
                    objData->unk6 = 0;
                }
            }
            if (objData->unk4 <= 0) {
                volume = (var_fa1 / 2.0f) * 127.0f;
                if (volume > MAX_VOLUME) {
                    volume = MAX_VOLUME;
                }
                
                gDLL_6_AMSFX->vtbl->play_sound(self, 0x111, volume, NULL, NULL, 0, NULL);
                objData->unk4 = 30;
            }
            if (var_fa1 != 0/*.0f*/) {
                spD4.f[0] *= 1.0f / var_fa1;
                spD4.f[1] *= 1.0f / var_fa1;
                spD4.f[2] *= 1.0f / var_fa1;
            }
            
            spC8.f[0] = sp4C.unk0[0].f[0];
            spC8.f[1] = sp4C.unk0[0].f[1];
            spC8.f[2] = sp4C.unk0[0].f[2];

            var_fv0 = 2.0f * DOT_PRODUCT(spD4, spC8);
            
            self->velocity.f[0] = spC8.f[0] * var_fv0;
            self->velocity.f[1] = spC8.f[1] * var_fv0;
            self->velocity.f[2] = spC8.f[2] * var_fv0;
            
            self->velocity.f[0] -= spD4.f[0];
            self->velocity.f[1] -= spD4.f[1];
            self->velocity.f[2] -= spD4.f[2];

            self->velocity.f[0] *= var_fa1 * 0.7f;
            self->velocity.f[1] *= var_fa1 * 0.5f;
            self->velocity.f[2] *= var_fa1 * 0.7f;
        }

        if (objData->unk4 > 0) {
            objData->unk4 -= spBC;
        }

        var_fv1 = -self->velocity.f[2];
        self->srt.pitch -= var_fv1 * 1000.0f;
        
        var_fv0 = self->velocity.f[0];
        self->srt.roll -= var_fv0  * 1000.0f;
        
        self->srt.scale = self->def->scale * objData->unk0;
        
        map_save_object(self->setup, self->mapID, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
    }
}

// offset: 0x634 | func: 2 | export: 2
void dll_536_update(Object *self) { }

// offset: 0x640 | func: 3 | export: 3
void dll_536_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x694 | func: 4 | export: 4
void dll_536_free(Object *self, s32 onlySelf) { }

// offset: 0x6A4 | func: 5 | export: 5
u32 dll_536_get_model_flags(Object *self) {
    return MODFLAGS_SHADOW;
}

// offset: 0x6B4 | func: 6 | export: 6
u32 dll_536_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DIMBoulder_Data);
}
