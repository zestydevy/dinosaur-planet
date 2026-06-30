#include "common.h"
#include "dlls/objects/338_LFXEmitter.h"
#include "sys/objtype.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} DLL609_Setup; //0x24

typedef struct {
    Object* unk0;
    f32 unk4[4];
    f32 unk14[4];
    f32 unk24[4];
    Vec3f unk34;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    Vec3f unk54;
    s16 unk60;
    s16 unk62;
    s16 unk64;
    u8 unk66;
    s8 unk67;
    u8 unk68;
    s8 unk69;
    u8 unk6A;
    s8 unk6B;
} DLL609_Data;

static void dll_609_func_8C8(Object* self);
static void dll_609_func_98C(Object* self);

/*0x0*/ static u32 data_0 = 0;

// offset: 0x0 | ctor
void dll_609_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_609_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_609_setup(Object* self, DLL609_Setup* objSetup, s32 reset) {
    DLL609_Data* objData;
    Vec3f v;
    
    objData = self->data;
    
    obj_add_object_type(self, OBJTYPE_FireFly);

    v.f[0] = self->srt.transl.x;
    v.f[1] = self->srt.transl.y;
    v.f[2] = self->srt.transl.z;
    
    objData->unk4[0] = v.f[0];
    objData->unk14[0] = v.f[1];
    objData->unk24[0] = v.f[2];
    
    objData->unk4[1] = v.f[0];
    objData->unk14[1] = v.f[1];
    objData->unk24[1] = v.f[2];
    
    objData->unk4[2] = v.f[0];
    objData->unk14[2] = v.f[1];
    objData->unk24[2] = v.f[2];
    
    objData->unk4[3] = v.f[0];
    objData->unk14[3] = v.f[1];
    objData->unk24[3] = v.f[2];

    objData->unk0 = 0;
    objData->unk6A = 0;
    objData->unk44 = 0.01f;
    objData->unk48 = 0.0275f;
    objData->unk40 = 1.0f;
    objData->unk68 = 0;
    objData->unk67 = 0;
    objData->unk62 = rand_next(500, 1500);
    objData->unk60 = rand_next(0, 65000);
    objData->unk64 = objSetup->unk18;
    objData->unk66 = objSetup->unk19;
    objData->unk4C = objSetup->unk1A;
    objData->unk50 = objSetup->unk1C;
    
    objData->unk54.x = objSetup->base.x;
    objData->unk54.y = objSetup->base.y;
    objData->unk54.z = objSetup->base.z;
    
    objData->unk6B = 0;
}

// offset: 0x1B4 | func: 1 | export: 1
void dll_609_control(Object* self) {
    DLL609_Data* objData;
    Object* player;
    Object* sidekick;
    LFXEmitter_Setup* lfxSetup;

    objData = self->data;
    player = get_player();
    sidekick = get_sidekick();
    
    if (objData->unk40 > 1.0f) {
        objData->unk40 -= 1.0f;
        if (objData->unk68 >= 4) {
            objData->unk68++;
        } else {
            dll_609_func_98C(self);
        }
        dll_609_func_8C8(self);
    }
    
    self->srt.transl.x = curves_b_spline(objData->unk4, objData->unk40, NULL);
    self->srt.transl.y = curves_b_spline(objData->unk14, objData->unk40, NULL);
    self->srt.transl.z = curves_b_spline(objData->unk24, objData->unk40, NULL);
    objData->unk40 += objData->unk44 * gUpdateRateF;
    self->srt.yaw = arctan2_f(self->srt.transl.x - self->prevLocalPosition.x, self->srt.transl.f[2] - self->prevLocalPosition.f[2]);

    if ((objData->unk0 != NULL) && (objData->unk0->stateFlags & 0x40)) {
        objData->unk6A = 0;
        objData->unk0 = NULL;
    }

    if ((objData->unk66 == 1) || (objData->unk66 == 4)) {
        if ((vec3_distance_xz(&player->globalPosition, (Vec3f* ) &self->setup->x) < 190.0f) || ((sidekick != NULL) && (vec3_distance_xz(&sidekick->globalPosition, (Vec3f* ) &self->setup->x) < 190.0f))) {
            if (objData->unk6A == 0) {
                objData->unk6A = 1;
                lfxSetup = obj_alloc_setup(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
                lfxSetup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
                lfxSetup->base.x = self->srt.transl.x;
                lfxSetup->base.y = self->srt.transl.y;
                lfxSetup->base.z = self->srt.transl.z;
                lfxSetup->unk1E = rand_next(0, 1) + 0x1AA;
                lfxSetup->unk22 = -1;
                objData->unk0 = obj_create((ObjSetup*)lfxSetup, 5, self->mapID, -1, self->parent);
            }
            objData->unk0->srt.transl.x = self->srt.transl.x;
            objData->unk0->srt.transl.y = self->srt.transl.y;
            objData->unk0->srt.transl.z = self->srt.transl.z;
        } else {
            if (((objData->unk66 == 1) || (objData->unk66 == 4)) && (objData->unk6A == 1)) {
                if (objData->unk0 != NULL) {
                    obj_destroy_object(objData->unk0);
                    objData->unk6A = 0;
                    objData->unk0 = NULL;
                }
            }
        }
    }

    if ((objData->unk66 == 1) || (objData->unk66 == 4)) {
        gDLL_17_partfx->vtbl->spawn(self, 0x19E, NULL, 1, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, 0x1A0, NULL, 1, -1, NULL);
    } else {
        gDLL_17_partfx->vtbl->spawn(self, 0x1BB, NULL, 1, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, 0x1BD, NULL, 1, -1, NULL);
    }
    
    if ((vec3_distance_xz(&player->globalPosition, (Vec3f*)&self->setup->x) < objData->unk4C) || 
        (sidekick && (vec3_distance_xz(&sidekick->globalPosition, (Vec3f*)&self->setup->x) < objData->unk4C))
    ) {
        if (objData->unk66 == 4) {
            gDLL_17_partfx->vtbl->spawn(self, 0x19F, NULL, 1, -1, NULL);
        } else if (objData->unk66 == 3) {
            gDLL_17_partfx->vtbl->spawn(self, 0x1BC, NULL, 1, -1, NULL);
        } else if (objData->unk66 == 5) {
            gDLL_17_partfx->vtbl->spawn(self, 0x1BC, NULL, 1, -1, NULL);
        }

        if (objData->unk48 < 0.003f) {
            objData->unk48 += 0.00001f;
            if (objData->unk48 > 0.003f) {
                objData->unk48 = 0.003f;
            }
        }
    } else {
        if (objData->unk48 > 0.001f) {
            objData->unk48 -= 0.00001f;
            if (objData->unk48 < 0.001f) {
                objData->unk48 = 0.001f;
            }
        }
    }
}

// offset: 0x7B8 | func: 2 | export: 2
void dll_609_update(Object* self) { }

// offset: 0x7C4 | func: 3 | export: 3
void dll_609_print(Object* self, Gfx** gfl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gfl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x818 | func: 4 | export: 4
void dll_609_free(Object* self, s32 onlySelf) {
    DLL609_Data* objData = self->data;

    if (onlySelf == FALSE) {
        if (objData->unk0 != NULL) {
            obj_destroy_object(objData->unk0);
        }
    }
    
    obj_free_object_type(self, OBJTYPE_FireFly);
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x8A4 | func: 5 | export: 5
u32 dll_609_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x8B4 | func: 6 | export: 6
u32 dll_609_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DLL609_Data);
}

// offset: 0x8C8 | func: 7
void dll_609_func_8C8(Object* self) {
    DLL609_Data* objData = self->data;
    
    objData->unk4[0] = objData->unk4[1];
    objData->unk14[0] = objData->unk14[1];
    objData->unk24[0] = objData->unk24[1];
    
    objData->unk4[1] = objData->unk4[2];
    objData->unk14[1] = objData->unk14[2];
    objData->unk24[1] = objData->unk24[2];
    
    objData->unk4[2] = objData->unk4[3];
    objData->unk14[2] = objData->unk14[3];
    objData->unk24[2] = objData->unk24[3];
    
    objData->unk44 = rand_next(60, 90) * 0.00015f;
    
    objData->unk4[3] = objData->unk34.x;
    objData->unk14[3] = objData->unk34.y;
    objData->unk24[3] = objData->unk34.z;
}

// offset: 0x98C | func: 8
void dll_609_func_98C(Object* self) {
    DLL609_Data* objData;
    SRT sp2C;

    objData = self->data;
    objData->unk34.x = 0;
    objData->unk34.y = rand_next(-objData->unk64, objData->unk64);
    if (objData->unk50 < 21.0f) {
        objData->unk34.z = 0.0f;
    } else {
        objData->unk34.z = objData->unk50 - rand_next(20, (s16)objData->unk50);
    }

    objData->unk60 += (s16)rand_next(3000, 5000);
    
    sp2C.transl.x = 0.0f;
    sp2C.transl.y = 0.0f;
    sp2C.transl.z = 0.0f;
    sp2C.scale = 1.0f;
    sp2C.roll = 0;
    sp2C.pitch = 0;
    sp2C.yaw = objData->unk60;
    rotate_vec3(&sp2C, objData->unk34.f);
    
    objData->unk34.x = (objData->unk54.x + objData->unk34.x);
    objData->unk34.y = (objData->unk54.y + objData->unk34.y);
    objData->unk34.z = (objData->unk54.z + objData->unk34.z);
}

// offset: 0xADC | func: 9 | export: 7
void dll_609_func_ADC(Object* self, Vec3f* point) {
    DLL609_Data* objData = self->data;
    
    objData->unk34.x = point->x;
    objData->unk34.y = point->y;
    objData->unk34.z = point->z;
    
    objData->unk68 = 4;
}

/*0x0*/ static const char str_0[] = " Sending FireFly ";
