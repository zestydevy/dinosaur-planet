#include "common.h"

#include "sys/gfx/modgfx.h"
#include "sys/gfx/projgfx.h"

/*0x0*/ static s16 dataTickCounter = 0;
/*0x4*/ static DLL_IProjgfx* dataDLLProjGfx = NULL;
/*0x8*/ static DLL_IModgfx* dataDLLModGfx = NULL;

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 fxRadius;        //2D player range for creating magic wave effects (stored halved)
    s16 roll;
    s16 pitch;
} WMwallpower_Setup;

typedef struct {
    f32 fxRadius;        //2D player range for creating magic wave effects
    s16 yaw;
    s16 roll;
    s16 pitch;
    s16 timerAndFXCount; //Control function delay timer when negative, particle count when positive?
    s16 timer;           //Unused half-second timer, counts down when player leaves fxRadius
    s16 unusedE;
    s32 unused10;
} WMwallpower_Data;

// offset: 0x0 | ctor
void WMwallpower_ctor(void *dll) { }

// offset: 0xC | dtor
void WMwallpower_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WMwallpower_setup(Object* self, WMwallpower_Setup* objsetup, s32 arg2) {
    WMwallpower_Data* objdata;

    self->srt.yaw = 0;
    self->srt.pitch = 0;
    self->srt.roll = 0;
    self->srt.scale = 0.25f;
    
    objdata = self->data;
    objdata->fxRadius = 2.0f * objsetup->fxRadius;
    objdata->yaw = objsetup->yaw << 8;
    objdata->roll = objsetup->roll;
    objdata->pitch = objsetup->pitch;
    self->srt.yaw = objdata->yaw;
    self->srt.roll = objdata->roll;
    self->srt.pitch = objdata->pitch;
    objdata->timerAndFXCount = -400;
    objdata->timer = 0;
    self->srt.scale = 1.0f;

    dataDLLProjGfx = dll_load_deferred(DLL_ID_202, 1);
    dataDLLModGfx =  dll_load_deferred(DLL_ID_133, 1);

    if (objdata->timerAndFXCount <= 0) {
        self->unkDC = objdata->timerAndFXCount;
    } else {
        self->unkDC = 0;
    }
}

// offset: 0x12C | func: 1 | export: 1
void WMwallpower_control(Object* self) {
    WMwallpower_Data* objdata;
    Vec3f delta;
    Object* player;
    f32 distance;
    s16 i;

    objdata = self->data;
    player = get_player();
    if (player == NULL) {
        return;
    }

    //Get exact distance to player 
    delta.f[0] = self->positionMirror.x - player->positionMirror.x;
    delta.f[1] = self->positionMirror.y - player->positionMirror.y;
    delta.f[2] = self->positionMirror.z - player->positionMirror.z;
    distance = sqrtf(SQ(delta.f[0]) + SQ(delta.f[1]) + SQ(delta.f[2])); //@bug?: unused

    if ((objdata->timerAndFXCount >= 0) || 
        ((objdata->timerAndFXCount < 0) && (self->unkDC <= 0))) 
    {
        delta.f[0] = self->positionMirror.x - player->positionMirror.x;
        delta.f[2] = self->positionMirror.z - player->positionMirror.z;

        //Create magic wave effect when player within 2D radius
        if (sqrtf(SQ(delta.f[0]) + SQ(delta.f[2])) <= objdata->fxRadius) { //@optimisation: could use distanceSquared
            if (objdata->timerAndFXCount > 0) {
                for (i = 0; i < objdata->timerAndFXCount; i++){
                    dataDLLProjGfx->vtbl->func0(self, 0, NULL, 4, -1, 0x10, 0);
                }

                dataDLLModGfx->vtbl->func0(self, 7, NULL, 1, -1, 0);
                objdata->timer = 30;
            } else {
                dataTickCounter++;

                dataDLLProjGfx->vtbl->func0(self, 0, NULL, 4, -1, 0x10, 0);

                //Load and call modgfx (@bug?: DLL should already be loaded?)
                dataDLLModGfx = dll_load_deferred(DLL_ID_133, 1);
                dataDLLModGfx->vtbl->func0(self, 7, NULL, 1, -1, 0);

                objdata->timer = 30;
            }
        }
        self->unkDC = -objdata->timerAndFXCount;
    } else if (objdata->timerAndFXCount < 0) {
        if (self->unkDC > 0) {
            self->unkDC -= gUpdateRate;
        }
    }

    //Decrement timer
    if (objdata->timer > 0) {
        objdata->timer -= gUpdateRate;
    }
}

// offset: 0x420 | func: 2 | export: 2
void WMwallpower_update(Object *self) { }

// offset: 0x42C | func: 3 | export: 3
void WMwallpower_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x444 | func: 4 | export: 4
void WMwallpower_free(Object* self, s32 arg1) {
    WMwallpower_Data *objdata = self->data;

    gDLL_15_Projgfx->vtbl->func4(self);
    gDLL_13_Expgfx->vtbl->func5(self);

    if (objdata) {
    }

    dll_unload(dataDLLProjGfx);
    dll_unload(dataDLLModGfx);
}

// offset: 0x4E0 | func: 5 | export: 5
u32 WMwallpower_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x4F0 | func: 6 | export: 6
u32 WMwallpower_get_data_size(Object *self, u32 a1){
    return sizeof(WMwallpower_Data);
}
