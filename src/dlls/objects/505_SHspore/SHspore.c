#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "functions.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/rand.h"
#include "types.h"


typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 unk18;
/*1A*/ s16 unk1A;
/*1C*/ s16 unk1C;
} SHSpore_Setup;

typedef struct {
/*000*/ DLL27_Data unk0;
/*260*/ f32 lifetime;
/*264*/ f32 unk264;
/*268*/ f32 unk268;
/*26C*/ f32 unk26C;
/*270*/ f32 unk270;
/*274*/ f32 unk274;
/*278*/ f32 unk278;
/*27C*/ f32 unk27C;
/*280*/ f32 unk280;
/*284*/ f32 unk284;
/*288*/ f32 unk288;
/*28C*/ f32 unk28C;
/*290*/ f32 unk290;
/*294*/ s16 unk294;
/*296*/ s16 unk296;
/*298*/ s16 unk298;
/*29A*/ s16 unk29A;
} SHSpore_Data;

static void SHspore_func_750(Object* self, SHSpore_Data* objdata);
static void SHspore_func_A00(Object* arg0, SHSpore_Data* objdata);

/*0x0*/ static Vec3f _data_0 = { 0, 0, 0 };
/*0xC*/ static f32 _data_C = 5.0f;

// offset: 0x0 | ctor
void SHspore_ctor(void *dll) {
}

// offset: 0xC | dtor
void SHspore_dtor(void *dll) {
}

// offset: 0x18 | func: 0 | export: 0
void SHspore_setup(Object* self, s32 arg1, s32 arg2) {
    SHSpore_Data* objdata;
    s8 pad[4];
    u8 sp37;

    objdata = self->data;
    sp37 = 5;
    objdata->lifetime = 1500.0f; //25s
    self->unkB0 |= 0x6000;
    self->speed.y = 3.0f;

    func_800267A4(self);
    objdata->unk298 = rand_next(0, 0xFFFF);
    objdata->unk26C = rand_next(0, 1000) / 1000.0f;

    gDLL_27->vtbl->init(&objdata->unk0, DLL27FLAG_NONE, DLL27FLAG_2 | DLL27FLAG_40000, DLL27MODE_1);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->unk0, 1, &_data_0, &_data_C, &sp37);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3F1, NULL, PARTFXFLAG_4, -1, NULL);
}

// offset: 0x18C | func: 1 | export: 1
void SHspore_control(Object* self) {
    SHSpore_Data* objdata;
    s8 pad[10];
    s32 particleCount;
    f32 temp;
    f32 temp_fv0;
    s32 index;
    Object* collidedObject;

    objdata = (SHSpore_Data*)self->data;
    if (objdata->unk290 != 0.0f) {
        self->srt.yaw += gUpdateRate << 6;
        objdata->unk290 -= gUpdateRateF;
        if (objdata->unk290 <= 0.0f) {
            obj_destroy_object(self);
        }
    } else {
        //Update motion
        objdata->unk270 -= gUpdateRateF;
        if (objdata->unk270 < 0.0f) {
            objdata->unk270 = 0.0f;
        }
        objdata->unk28C -= gUpdateRateF;
        if (objdata->unk28C < 0.0f) {
            objdata->unk28C = 0.0f;
        }
        self->speed.y += -0.009f * gUpdateRateF;
        if (self->speed.y < -0.2f) {
            self->speed.y = -0.2f;
        }
        if (self->speed.y > 0) {
            self->speed.y *= 0.97f;
        }
        if (self->speed.y < 0.0f) {
            func_8002674C(self);
        }
        SHspore_func_750(self, objdata);
        if ((rand_next(0, 100) < 5) && (objdata->unk270 <= 0.0f)) {
            SHspore_func_A00(self, objdata);
        }
        objdata->unk284 -= gUpdateRateF;
        if (objdata->unk284 <= 0.0f) {
            objdata->unk27C *= 0.97f;
            objdata->unk280 *= 0.97f;
            objdata->unk284 = 0.0f;
        } else {
            temp_fv0 = objdata->unk288 - objdata->unk268;
            objdata->unk268 += temp_fv0 * 0.01f * gUpdateRateF;
        }
        self->speed.x = objdata->unk274 + (objdata->unk27C * objdata->unk268);
        self->speed.z = objdata->unk278 + (objdata->unk280 * objdata->unk268);
        obj_integrate_speed(self, self->speed.x * gUpdateRateF, self->speed.y * gUpdateRateF, self->speed.z * gUpdateRateF);
        gDLL_27->vtbl->func_1e8(self, &objdata->unk0, gUpdateRateF);
        gDLL_27->vtbl->func_5a8(self, &objdata->unk0);
        gDLL_27->vtbl->func_624(self, &objdata->unk0, gUpdateRateF);
        func_80026128(self, 0xA, 0, 0);

        //Handle collisions
        collidedObject = (Object*)self->objhitInfo->unk48;
        if (collidedObject) {
            particleCount = 20;
            if (get_player() == collidedObject) {
                //Player collecting purple mushroom
                main_increment_bits(BIT_Inventory_Purple_Mushrooms);
                particleCount = 0;
            }
            if (collidedObject->id != OBJ_SHrocketmushroo) {
                //Other objects (ignoring SHrocketmushroom since the spores emerge out of it)
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B31_Item_Collection_Chime, MAX_VOLUME, NULL, 0, 0, 0);
                gDLL_13_Expgfx->vtbl->func4(self);

                //Create collision particles
                for (index = 0; index < particleCount; index++){
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3F3, NULL, PARTFXFLAG_4, -1, NULL);
                }

                objdata->unk290 = 200.0f;
                self->srt.flags |= 0x4000;
                func_800267A4(self);
            }
        } else {
            objdata->lifetime -= gUpdateRateF;
            //Destroy the spore if its lifetime runs out or it collides with terrain
            if (objdata->lifetime <= 0.0f || objdata->unk0.unk25C & 0x11) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_8A2, MAX_VOLUME, NULL, 0, 0, 0);
                gDLL_13_Expgfx->vtbl->func4(self);

                //Create collision particles
                for (index = 0; index < 20; index++){
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3F3, NULL, PARTFXFLAG_4, -1, NULL);
                }

                objdata->unk290 = 200.0f;
                self->srt.flags |= 0x4000;
                func_800267A4(self);
            }
        }
    }
}

// offset: 0x750 | func: 2
static void SHspore_func_750(Object* self, SHSpore_Data* objdata) {
    s8 pad[4];
    SHSpore_Setup* setup;
    f32 temp_fv1;
    s32 sp20;
    s32 var_v0;

    setup = (SHSpore_Setup*)self->setup;
    sp20 = setup->unk1C;

    if (rand_next(0, 100) < 0xA && objdata->unk28C <= 0.0f) {
        objdata->unk298 = rand_next(2000, 4000);
        if (rand_next(0, 1) != 0) {
            objdata->unk298 = -objdata->unk298;
        }
        objdata->unk298 += objdata->unk294;
        var_v0 = objdata->unk298 - (sp20 & 0xFFFF);
        CIRCLE_WRAP(var_v0)

        if (setup->unk1A < var_v0) {
            objdata->unk298 = setup->unk1A + sp20;
        }
        if (var_v0 < -setup->unk1A) {
            objdata->unk298 = sp20 - setup->unk1A;
        }
        objdata->unk28C = 150.0f;
    }

    if (rand_next(0, 100) < 0xA && objdata->unk28C <= 0.0f) {
        objdata->unk26C = (rand_next(-200, 200) / 1000.0f) + objdata->unk264;
        if (objdata->unk26C < 0.5f) {
            objdata->unk26C = 0.5f;
        } else if (objdata->unk26C > 1.0f) {
            objdata->unk26C = 1.0f;
        }
    }

    var_v0 = objdata->unk298 - (objdata->unk294 & 0xFFFF);
    CIRCLE_WRAP(var_v0)

    objdata->unk294 += (var_v0 * gUpdateRate) >> 4;

    temp_fv1 = objdata->unk26C - objdata->unk264;
    objdata->unk264 += temp_fv1 * 0.006f * gUpdateRateF;
    objdata->unk274 = fsin16_precise(objdata->unk294) * objdata->unk264;
    objdata->unk278 = fcos16_precise(objdata->unk294) * objdata->unk264;
}

// offset: 0xA00 | func: 3
static void SHspore_func_A00(Object* self, SHSpore_Data* objdata) {
    SHSpore_Setup* setup;
    s32 sp20;
    s32 var_v1;

    setup = (SHSpore_Setup*)self->setup;
    sp20 = setup->unk1C;
    objdata->unk284 = rand_next(0x1E, 0x2D);
    objdata->unk270 = rand_next(0x78, 0xB4) + objdata->unk284;
    objdata->unk296 = rand_next(-0x7D0, 0x7D0) + objdata->unk294;

    var_v1 = objdata->unk296 - (sp20 & 0xFFFF);
    CIRCLE_WRAP(var_v1)

    if (setup->unk1A < var_v1) {
        objdata->unk296 = setup->unk1A + sp20;
    }
    if (var_v1 < -setup->unk1A) {
        objdata->unk296 = sp20 - setup->unk1A;
    }

    objdata->unk288 = rand_next(0x384, 0x514) / 1000.0f;
    objdata->unk268 = 0.0f;
    objdata->unk27C = fsin16_precise(objdata->unk296);
    objdata->unk280 = fcos16_precise(objdata->unk296);
}

// offset: 0xB88 | func: 4 | export: 2
void SHspore_update(Object *self) {
}

// offset: 0xB94 | func: 5 | export: 3
void SHspore_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
}

// offset: 0xBAC | func: 6 | export: 4
void SHspore_free(Object* self, s32 arg1) {
    gDLL_13_Expgfx->vtbl->func4(self);
}

// offset: 0xBF4 | func: 7 | export: 5
u32 SHspore_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xC04 | func: 8 | export: 6
u32 SHspore_get_data_size(Object *self, s32 arg1) {
    return sizeof(SHSpore_Data);
}
