#include "PR/ultratypes.h"
#include "common.h"

typedef struct {
    ObjSetup base;
    s8 scaleMultiplier;
    s8 orbitalRadius; //divided by 16
    s16 modelIdx;
} WMPlanets_Setup;

typedef struct {
    s16 orbitSpeed;
    s16 spinSpeed;
    s16 orbitYaw;
    s16 unused6;
    s16 inclination;
    s16 unusedA;
    f32 orbitRadius;
    Vec3f orbitOrigin;  //centrepoint the planet is orbiting around (ObjSetup position)
} WMPlanets_Data; //0x1C

// offset: 0x0 | ctor
void WMPlanets_ctor(void *dll) { }

// offset: 0xC | dtor
void WMPlanets_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WMPlanets_setup(Object* self, WMPlanets_Setup* objsetup, s32 arg2) {
    WMPlanets_Data* objdata = self->data;

    self->srt.scale = self->def->scale * 0.1f * (objsetup->scaleMultiplier + 1.0f);

    if (objsetup->base.objId) {
        objdata->orbitRadius = - (f32)(objsetup->orbitalRadius * 16);
    } else {
        //Set orbit radius to 0 if objID is 0? But 0 is OBJ_Sabre!
        //Seems intended for Dinosaur Planet's sun, but maybe unfinished?
        objdata->orbitRadius = 0.0f;
    }

    objdata->orbitSpeed = rand_next(100, 200);
    objdata->spinSpeed = rand_next(200, 400);
    objdata->orbitYaw = 0;
    objdata->inclination = rand_next(0, 2400);
    objdata->orbitOrigin.x = self->srt.transl.x;
    objdata->orbitOrigin.y = self->srt.transl.y;
    objdata->orbitOrigin.z = self->srt.transl.z;

    func_80023A18(self, objsetup->modelIdx);
    self->srt.transl.z = objsetup->base.z + objdata->orbitRadius;
}

// offset: 0x168 | func: 1 | export: 1
void WMPlanets_control(Object* self) {
    WMPlanets_Data* objdata = self->data;
    SRT srt;
    Vec3f v = {0,0,0};

    v.z = objdata->orbitRadius;

    //Advance orbital position (yaw component)
    objdata->orbitYaw += objdata->orbitSpeed; //@bug: framerate-dependent
    srt.transl.x = 0.0f;
    srt.transl.y = 0.0f;
    srt.transl.z = 0.0f;
    srt.scale = 1.0f;
    srt.roll = 0;
    srt.pitch = 0;
    srt.yaw = objdata->orbitYaw;
    rotate_vec3(&srt, v.f);

    //Apply orbital inclination (pitch of orbital plane)
    srt.transl.x = 0.0f;
    srt.transl.y = 0.0f;
    srt.transl.z = 0.0f;
    srt.scale = 1.0f;
    srt.roll = 0;
    srt.pitch = objdata->inclination;
    srt.yaw = 0;
    rotate_vec3(&srt, v.f);

    //Set planet's coords in world
    VECTOR_ADD(v, objdata->orbitOrigin, self->srt.transl);

    //Advance planet's axial rotation
    self->srt.yaw += objdata->spinSpeed * (s16)gUpdateRateF;
}

// offset: 0x2C4 | func: 2 | export: 2
void WMPlanets_update(Object *self) { }

// offset: 0x2D0 | func: 3 | export: 3
void WMPlanets_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility){
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x324 | func: 4 | export: 4
void WMPlanets_free(Object *self, s32 a1) { }

// offset: 0x334 | func: 5 | export: 5
u32 WMPlanets_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x344 | func: 6 | export: 6
u32 WMPlanets_get_data_size(Object* self, s32 arg1) {
    return sizeof(WMPlanets_Data);
}
