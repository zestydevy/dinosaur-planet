
#include "PR/ultratypes.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/491_WGSH_projball.h"
#include "PR/gbi.h"
#include "PR/os.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/curves.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/objhits.h"
#include "sys/rand.h"
#include "dll.h"
#include "types.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 unk18;
/*1A*/ s16 unk1A;
} WGSH_flybaddie_Setup;

typedef struct {
/*00*/ f32 xCurve[4];
/*10*/ f32 yCurve[4];
/*20*/ f32 zCurve[4];
/*30*/ f32 curveT;
/*34*/ f32 unk34;
/*38*/ f32 unk38;
/*3C*/ f32 unk3C;
/*40*/ f32 unk40;
/*44*/ f32 unk44;
/*48*/ s16 unk48;
/*4A*/ s16 unk4A;
/*4C*/ s16 unk4C;
/*4E*/ s16 unk4E;
/*50*/ s8 unk50;
/*51*/ s8 unk51;
/*52*/ u8 unk52;
/*53*/ s8 unk53;
} WGSH_flybaddie_Data;

/*0x0*/ static u8 data_0 = 0;

static void WGSH_flybaddie_func_700(Object* self);
static void WGSH_flybaddie_func_8A4(Object* self);

// offset: 0x0 | ctor
void WGSH_flybaddie_ctor(void *dll) { }

// offset: 0xC | dtor
void WGSH_flybaddie_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WGSH_flybaddie_setup(Object* self, WGSH_flybaddie_Setup* setup, s32 arg2) {
    WGSH_flybaddie_Data* objdata;
    f32 pos[3];
    Object* player;

    objdata = self->data;
    player = objGetPlayer();
    if (player != NULL) {
        objdata->unk38 = player->srt.transl.x;
        objdata->unk3C = player->srt.transl.y;
        objdata->unk40 = player->srt.transl.z;
    }
    pos[0] = self->srt.transl.x;
    pos[1] = self->srt.transl.y;
    pos[2] = self->srt.transl.z;
    objdata->xCurve[0] = pos[0];
    objdata->yCurve[0] = pos[1];
    objdata->zCurve[0] = pos[2];
    objdata->xCurve[1] = pos[0];
    objdata->yCurve[1] = pos[1];
    objdata->zCurve[1] = pos[2];
    objdata->xCurve[2] = pos[0];
    objdata->yCurve[2] = pos[1];
    objdata->zCurve[2] = pos[2];
    objdata->xCurve[3] = pos[0];
    objdata->yCurve[3] = pos[1];
    objdata->zCurve[3] = pos[2];
    objdata->unk34 = 0.0055f;
    objdata->curveT = 1.0f;
    objdata->unk51 = 0;
    objdata->unk50 = 0;
    objdata->unk48 = mathRnd(0, 65000);
    objdata->unk4A = 10000;
    objdata->unk53 = 1;
    objdata->unk4C = (s16) (mathRnd(0, 1000) + 1000);
    objdata->unk4E = mathRnd(0, 1000);
    if (setup->unk1A == 0) {
        objdata->unk44 = -130.0f;
    } else {
        objdata->unk44 = (f32) setup->unk1A;
    }
}

// offset: 0x1C0 | func: 1 | export: 1
void WGSH_flybaddie_control(Object* self) {
    /*0x4*/ static u8 data_4 = 1;
    WGSH_flybaddie_Data* objdata;
    Object* player;
    Object* hitBy;
    s32 _pad;
    s32 i;

    objdata = self->data;
    hitBy = NULL;
    player = objGetPlayer();
    if ((data_4 != 0) && (player != NULL)) {
        objdata->unk38 = player->srt.transl.x;
        objdata->unk3C = player->srt.transl.y;
        objdata->unk40 = player->srt.transl.z;
        data_4 = 0;
    }
    if (mainGetBits(BIT_1D4) != 0) {
        objFreeObject(self);
    } else {
        if (objdata->unk4E > 0) {
            objdata->unk4E -= (s16) gUpdateRateF;
        }
        if ((objdata->unk4E != -999) && (objdata->unk4E <= 0)) {
            dll_amSfx->Play(self, SOUND_72F_Harsh_Magical_Thrum_Loop, 0x28, NULL, NULL, 0, NULL);
            objdata->unk4E = -999;
        }
        self->srt.yaw += (s16) (objdata->unk48 / 10);
        self->srt.pitch += (s16) (objdata->unk4A / 10);
        if (objdata->unk4C != 0) {
            objdata->unk4C -= (s16) gUpdateRateF;
        }
        if (objdata->unk4C <= 0) {
            WGSH_flybaddie_func_8A4(self);
            objdata->unk4C = (s16) (mathRnd(0, 1000) + 1000);
        }
        if (objdata->curveT > 1.0f) {
            objdata->curveT -= 1.0f;
            WGSH_flybaddie_func_700(self);
        }
        self->srt.transl.x = curvesBSpline(objdata->xCurve, objdata->curveT, NULL);
        self->srt.transl.y = curvesBSpline(objdata->yCurve, objdata->curveT, NULL);
        self->srt.transl.z = curvesBSpline(objdata->zCurve, objdata->curveT, NULL);
        objdata->curveT += (objdata->unk34 * (f32) (data_0 + 1) * gUpdateRateF);
        gDLL_17_partfx->vtbl->spawn(self, self->modelInstIdx + PARTICLE_286, NULL, 1, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, self->modelInstIdx + PARTICLE_286, NULL, 1, -1, NULL);
        if ((func_80025F40(self, &hitBy, NULL, NULL) == Damage_Type_Projectile) && (hitBy != NULL)) {
            if (hitBy->id != OBJ_WGSH_projball) {
                for (i = 15; i != 0; i--) {
                    gDLL_17_partfx->vtbl->spawn(self, self->modelInstIdx + PARTICLE_286, NULL, PARTFXFLAG_1, -1, NULL);
                }
                objdata->unk52++;
                objdata->unk34 *= 1.2f;
                if (objdata->unk52 >= 3) {
                    objFreeObject(self);
                    data_0++;
                    if (data_0 >= 3) {
                        mainSetBits(BIT_1D8, 1);
                    }
                } else {
                    objSetModel(self, self->modelInstIdx + 1);
                }
            }
        }
    }
}

// offset: 0x66C | func: 2 | export: 2
void WGSH_flybaddie_update(Object *self) { }

// offset: 0x678 | func: 3 | export: 3
void WGSH_flybaddie_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x6CC | func: 4 | export: 4
void WGSH_flybaddie_free(Object *self, s32 a1) { }

// offset: 0x6DC | func: 5 | export: 5
u32 WGSH_flybaddie_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x6EC | func: 6 | export: 6
u32 WGSH_flybaddie_get_data_size(Object *self, u32 a1) {
    return sizeof(WGSH_flybaddie_Data);
}

// offset: 0x700 | func: 7
static void WGSH_flybaddie_func_700(Object* self) {
    WGSH_flybaddie_Data* objdata;
    f32 sp48[3];
    SRT sp30;

    objdata = self->data;
    objdata->xCurve[0] = objdata->xCurve[1];
    objdata->yCurve[0] = objdata->yCurve[1];
    objdata->zCurve[0] = objdata->zCurve[1];
    objdata->xCurve[1] = objdata->xCurve[2];
    objdata->yCurve[1] = objdata->yCurve[2];
    objdata->zCurve[1] = objdata->zCurve[2];
    objdata->xCurve[2] = objdata->xCurve[3];
    objdata->yCurve[2] = objdata->yCurve[3];
    objdata->zCurve[2] = objdata->zCurve[3];
    sp48[0] = 0.0f;
    sp48[1] = 0.0f;
    sp48[2] = objdata->unk44;
    objdata->unk48 += (s16)mathRnd(1000, 2000);
    if (((objdata->unk4A < 0xFA1) && (objdata->unk53 == -1)) || ((objdata->unk4A >= 28000) && (objdata->unk53 == 1))) {
        objdata->unk53 = -objdata->unk53;
    }
    objdata->unk4A += ((s16)mathRnd(2000, 3000) * objdata->unk53);
    sp30.roll = 0;
    sp30.transl.x = 0.0f;
    sp30.transl.y = 0.0f;
    sp30.transl.z = 0.0f;
    sp30.scale = 1.0f;
    sp30.pitch = objdata->unk4A;
    sp30.yaw = objdata->unk48;
    mathRotateRPY(&sp30, sp48);
    objdata->xCurve[3] = (sp48[0] + objdata->unk38);
    objdata->yCurve[3] = (sp48[1] + (objdata->unk3C + 20.0f));
    objdata->zCurve[3] = (sp48[2] + objdata->unk40);
}

// offset: 0x8A4 | func: 8
static void WGSH_flybaddie_func_8A4(Object* self) {
    ObjSetup* projballSetup;
    Object* projball;
    Object* player;
    f32 dirVec[3];
    f32 magnitude;

    player = objGetPlayer();
    self->globalPosition.x = self->srt.transl.x;
    self->globalPosition.y = self->srt.transl.y;
    self->globalPosition.z = self->srt.transl.z;
    projballSetup = objAllocSetup(sizeof(WGSH_projball_Setup), OBJ_WGSH_projball);
    projballSetup->loadFlags = OBJSETUP_LOAD_MANUAL;
    projballSetup->fadeFlags = OBJSETUP_FADE_MANUAL;
    projballSetup->loadDistance = 0xFF;
    projballSetup->fadeDistance = 0xFF;
    projballSetup->x = self->srt.transl.x;
    projballSetup->y = self->srt.transl.y;
    projballSetup->z = self->srt.transl.z;
    projball = objSetupObject(projballSetup, OBJINIT_STANDALONE, -1, -1, NULL);
    if (projball != NULL) {
        projball->srt.flags |= OBJFLAG_OWNS_SETUP;
        dirVec[0] = player->srt.transl.x - self->srt.transl.x;
        dirVec[1] = (player->srt.transl.y + 20.0f) - self->srt.transl.y;
        dirVec[2] = player->srt.transl.z - self->srt.transl.z;
        magnitude = SQ(dirVec[0]) + SQ(dirVec[1]) + SQ(dirVec[2]);
        if (magnitude != 0.0f) {
            magnitude = sqrtf(magnitude);
            dirVec[0] /= magnitude;
            dirVec[1] /= magnitude;
            dirVec[2] /= magnitude;
        }
        projball->srt.transl.x += dirVec[0] * 10.0f;
        projball->srt.transl.y += dirVec[1] * 10.0f;
        projball->srt.transl.z += dirVec[2] * 10.0f;
        projball->velocity.x = 2.0f * dirVec[0];
        projball->velocity.y = 2.0f * dirVec[1];
        projball->velocity.z = 2.0f * dirVec[2];
        projball->unkDC = 0xBE;
        projball->unkE0 = 0;
        projball->globalPosition.x = projball->srt.transl.x;
        projball->globalPosition.y = projball->srt.transl.y;
        projball->globalPosition.z = projball->srt.transl.z;
        dll_amSfx->Play(projball, SOUND_730_Electrified_Blast, 0x50, NULL, NULL, 0, NULL);
    }
}
