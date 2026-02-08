#include "game/objects/object.h"
#include "sys/curves.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/objanim.h"
#include "sys/objhits.h"
#include "sys/rand.h"
#include "dll.h"
#include "functions.h"

/*0x0*/ static u32 _data_0[] = { // how many?
    0x00000002, 0x00000003, 0x00000000, 0x00000000
};

/*0x0*/ static s32 _bss_0; // stores unk10 of curveStruct

typedef struct {
/*00*/ UnkCurvesStruct *curveStruct;
/*04*/ Object *player;
/*08*/ f32 unk8;
/*0C*/ f32 unkC;
/*10*/ f32 distToPlayer;
/*14*/ f32 distToCurve;
/*18*/ f32 fadeoutTimer;
/*1C*/ f32 particleTimer;
/*20*/ f32 attackRange;
/*24*/ s32 soundHandle;
/*28*/ u16 theta[3];
/*2E*/ u8 flags;
} BalloonBaddie_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 unk18;
/*19*/ s8 attackRange;
/*1A*/ s16 unk1A;
/*1C*/ s16 respawnTimer;
} BalloonBaddie_Setup;

typedef enum {
    BALLOONBADDIE_1 = 0x1,
    BALLOONBADDIE_CHASE = 0x2,
    BALLOONBADDIE_RETREAT = 0x4,
    BALLOONBADDIE_RESPAWNED = 0x8,
    BALLOONBADDIE_DEATHFX1 = 0x10,
    BALLOONBADDIE_DEATHFX2 = 0x20
} BalloonBaddie_Flags;

typedef enum {
    BALLOONBADDIE_ALIVE,
    BALLOONBADDIE_DEAD
} BalloonBaddie_Status;

#define BALLOONBADDIE_MAX_SPEED 0.5f

static void BalloonBaddie_more_control(Object* self, BalloonBaddie_Data* objdata);

// offset: 0x0 | ctor
void BalloonBaddie_ctor(void *dll) { }

// offset: 0xC | dtor
void BalloonBaddie_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void BalloonBaddie_setup(Object *self, BalloonBaddie_Setup *setup, s32 arg2) {
    BalloonBaddie_Data *objdata;

    objdata = self->data;
    objdata->unk8 = setup->unk1A / 100.0f;
    objdata->unkC = 0.005f;
    objdata->attackRange = setup->attackRange * 4.0f;
    self->unkAF &= ~0x8;
    if (arg2 == 0) {
        objdata->curveStruct = mmAlloc(sizeof(UnkCurvesStruct), ALLOC_TAG_TEST_COL, NULL);
        if (objdata->curveStruct) {
            bzero(objdata->curveStruct, sizeof(UnkCurvesStruct));
        }
        if (gDLL_26_Curves->vtbl->func_4288(objdata->curveStruct, self, objdata->attackRange, (s32*)&_data_0, -1) == 0) {
            objdata->flags |= BALLOONBADDIE_1;
        }
        objdata->soundHandle = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B74_Gentle_Magic_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    self->unkB0 |= 0x2000;
}

// offset: 0x178 | func: 1 | export: 1
void BalloonBaddie_control(Object* self) {
    BalloonBaddie_Data* objdata;
    UnkCurvesStruct* curveStruct;
    BalloonBaddie_Setup* setup;
    Object *sp60;
    Vec3f delta;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    s32 _pad;
    s32 sp40;
    s32 sp3C;

    objdata = self->data;
    curveStruct = objdata->curveStruct;
    setup = (BalloonBaddie_Setup*)self->setup;
    if (self->unkDC != BALLOONBADDIE_ALIVE) {
        // check respawn timer
        if (gDLL_29_Gplay->vtbl->did_time_expire(setup->base.uID)) {
            self->unkDC = BALLOONBADDIE_ALIVE;
            self->opacity = OBJECT_OPACITY_MAX;
            self->unkAF &= ~8;
            objdata->flags |= BALLOONBADDIE_RESPAWNED;
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B20_Low_Grunt, MAX_VOLUME, NULL, NULL, 0, NULL);
            objdata->soundHandle = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B74_Gentle_Magic_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    } else {
        if (objdata->fadeoutTimer > 0.0f) {
            objdata->fadeoutTimer += gUpdateRateF * 0.01f;
            if (objdata->fadeoutTimer > 3.0f) {
                // delay dead state until shortly after fully invisible
                self->unkDC = BALLOONBADDIE_DEAD;
                objdata->fadeoutTimer = 0.0f;
            } else if (objdata->fadeoutTimer > 2.0f) {
                objdata->flags &= ~(BALLOONBADDIE_DEATHFX1 | BALLOONBADDIE_DEATHFX2);
            } else {
                // fadeout from 1.0 (fully opaque) to 2.0 (fully transparent)
                self->opacity = ((2.0f - objdata->fadeoutTimer) * 255.0f);
            }
            func_800267A4(self);
        } else {
            if (func_8002601C(self, &sp60, &sp40, &sp3C, &sp50, &sp4C, &sp48) != 0) {
                // balloonbaddie killed, start fade timer, start respawn timer
                if (objdata->soundHandle) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
                    objdata->soundHandle = 0;
                }
                objdata->flags |= (BALLOONBADDIE_DEATHFX1 | BALLOONBADDIE_DEATHFX2);
                objdata->fadeoutTimer = 1.0f;
                self->unkAF |= 8;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B21_Dissipating_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B1F_Slow_Magic_Chimes, MAX_VOLUME, NULL, NULL, 0, NULL);
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_B75_Water_Splash_Big, MAX_VOLUME, NULL, NULL, 0, NULL);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_331, NULL, PARTFXFLAG_2, -1, NULL);
                gDLL_33_BaddieControl->vtbl->func18(self, setup->unk18, -1, 0);
                gDLL_29_Gplay->vtbl->add_time(setup->base.uID, setup->respawnTimer * 60);
            }
            func_80026128(self, 0xA, 1, 0);
            func_8002674C(self);
            objdata->particleTimer -= gUpdateRateF;
            if (objdata->particleTimer < 0.0f) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_Green_Slime_Drop, NULL, PARTFXFLAG_2, -1, NULL);
                objdata->particleTimer = rand_next(30, 120);
            }
        }
        objdata->player = get_player();
        if (objdata->player) {
            VECTOR_SUBTRACT(objdata->player->positionMirror, self->positionMirror, delta);
            objdata->distToPlayer = VECTOR_MAGNITUDE(delta);
        }
        if (curveStruct) {
            VECTOR_SUBTRACT(*((Vec3f*)&curveStruct->unk68), self->positionMirror, delta);
            objdata->distToCurve = VECTOR_MAGNITUDE(delta);
        }
        // start retreating
        if (objdata->flags & BALLOONBADDIE_CHASE) {
            if (objdata->distToCurve > 250.0f) {
                objdata->flags &= ~BALLOONBADDIE_CHASE;
                objdata->flags |= BALLOONBADDIE_RETREAT;
            }
        }
        // stop retreating
        if (objdata->flags & BALLOONBADDIE_RETREAT) {
            if (objdata->distToCurve < 30.0f) {
                objdata->flags &= ~BALLOONBADDIE_RETREAT;
            }
        }
        // start chasing
        if (!(objdata->flags & (BALLOONBADDIE_CHASE | BALLOONBADDIE_RETREAT)) && objdata->player && (objdata->distToPlayer < objdata->attackRange)) {
            objdata->flags |= BALLOONBADDIE_CHASE;
        }
        BalloonBaddie_more_control(self, objdata);
    }
}

// offset: 0x808 | func: 2 | export: 2
void BalloonBaddie_update(Object *self) { }

// offset: 0x814 | func: 3 | export: 3
void BalloonBaddie_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    BalloonBaddie_Data* objdata;

    objdata = self->data;
    if (visibility && self->unkDC == BALLOONBADDIE_ALIVE) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        // dying particles
        if (objdata->flags & (BALLOONBADDIE_DEATHFX1 | BALLOONBADDIE_DEATHFX2)) {
            if (objdata->flags & BALLOONBADDIE_DEATHFX1) {
                gDLL_32->vtbl->func2(self, PARTICLE_330, &objdata->fadeoutTimer);
                gDLL_32->vtbl->func2(self, PARTICLE_330, &objdata->fadeoutTimer);
                objdata->flags &= ~BALLOONBADDIE_DEATHFX1;
            }
            gDLL_32->vtbl->func2(self, PARTICLE_32F, &objdata->fadeoutTimer);
        }
        // respawning particles
        if (objdata->flags & BALLOONBADDIE_RESPAWNED) {
            gDLL_32->vtbl->func2(self, PARTICLE_333, &objdata->fadeoutTimer);
            gDLL_32->vtbl->func2(self, PARTICLE_334, &objdata->fadeoutTimer);
            objdata->flags &= ~BALLOONBADDIE_RESPAWNED;
        }
    }
}

// offset: 0x99C | func: 4 | export: 4
void BalloonBaddie_free(Object* self, s32 a1) {
    BalloonBaddie_Data* objdata;

    objdata = self->data;
    obj_free_object_type(self, 4);
    if (objdata->curveStruct) {
        mmFree(objdata->curveStruct);
        objdata->curveStruct = NULL;
    }
    if (objdata->soundHandle != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
    }
}

// offset: 0xA38 | func: 5 | export: 5
u32 BalloonBaddie_get_model_flags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_8;
}

// offset: 0xA48 | func: 6 | export: 6
u32 BalloonBaddie_get_data_size(Object *self, u32 a1) {
    return sizeof(BalloonBaddie_Data);
}

// offset: 0xA5C | func: 7
void BalloonBaddie_more_control(Object* self, BalloonBaddie_Data* objdata) {
    s32 angle;
    UnkFunc_80024108Struct sp50;
    /*0x10*/ const f32 _rodata_10 = 0.001f;
    /*0x18*/ const f32 _rodata_18 = 0.001f;
    /*0x1C*/ const f32 _rodata_1C = 0.9f;
    UnkCurvesStruct* curveStruct;

    curveStruct = objdata->curveStruct;
    if (((func_800053B0(curveStruct, objdata->unk8) != 0) || (_bss_0 != curveStruct->unk10))
        && (gDLL_26_Curves->vtbl->func_4704(curveStruct) != 0)
        && (gDLL_26_Curves->vtbl->func_4288(objdata->curveStruct, self, 400.0f, (s32*) _data_0, -1) != 0)) {
        objdata->flags &= ~1;
    }
    _bss_0 = curveStruct->unk10;

    objdata->theta[0] += (u16)(128.0f * gUpdateRateF);
    objdata->theta[1] += (u16)(256.0f * gUpdateRateF);
    objdata->theta[2] += (u16)(512.0f * gUpdateRateF);

    self->srt.roll = (fsin16_precise(objdata->theta[0]) + fsin16_precise(objdata->theta[1])) * 1000.0f;
    self->srt.pitch = (fsin16_precise(objdata->theta[0]) + fsin16_precise(objdata->theta[2])) * 1000.0f;

    if (objdata->flags & BALLOONBADDIE_CHASE) {
        self->speed.x += (objdata->player->srt.transl.x - self->srt.transl.x) * 0.001f;
        self->speed.y += ((objdata->player->srt.transl.y + 60.0f) - self->srt.transl.y) * 0.001f;
        self->speed.z += (objdata->player->srt.transl.z - self->srt.transl.z) * 0.001f;
    } else if (objdata->flags & BALLOONBADDIE_RETREAT) {
        self->speed.x += (curveStruct->unk68.x - self->srt.transl.x) * 0.001f;
        self->speed.y += (curveStruct->unk68.y - self->srt.transl.y) * 0.001f;
        self->speed.z += (curveStruct->unk68.z - self->srt.transl.z) * 0.001f;
    } else {
        self->speed.x += (curveStruct->unk68.x - self->srt.transl.x) * 0.001f;
        self->speed.y += (curveStruct->unk68.y + ((fsin16_precise((s16) objdata->theta[0]) + fsin16_precise((s16) objdata->theta[1])) * 10.0f) - self->srt.transl.y) * 0.001f;
        self->speed.z += (curveStruct->unk68.z - self->srt.transl.z) * 0.001f;
    }

    VECTOR_SCALE(self->speed, 0.9f);

    if (self->speed.x > BALLOONBADDIE_MAX_SPEED) {
        self->speed.x = BALLOONBADDIE_MAX_SPEED;
    }
    if (self->speed.y > BALLOONBADDIE_MAX_SPEED) {
        self->speed.y = BALLOONBADDIE_MAX_SPEED;
    }
    if (self->speed.z > BALLOONBADDIE_MAX_SPEED) {
        self->speed.z = BALLOONBADDIE_MAX_SPEED;
    }
    if (self->speed.x < -BALLOONBADDIE_MAX_SPEED) {
        self->speed.x = -BALLOONBADDIE_MAX_SPEED;
    }
    if (self->speed.y < -BALLOONBADDIE_MAX_SPEED) {
        self->speed.y = -BALLOONBADDIE_MAX_SPEED;
    }
    if (self->speed.z < -BALLOONBADDIE_MAX_SPEED) {
        self->speed.z = -BALLOONBADDIE_MAX_SPEED;
    }

    obj_integrate_speed(self, self->speed.x * gUpdateRateF, self->speed.y * gUpdateRateF, self->speed.z * gUpdateRateF);
    func_80024108(self, objdata->unkC, gUpdateRateF, &sp50);

    angle = arctan2_f(self->positionMirror.x - objdata->player->positionMirror.x, self->positionMirror.z - objdata->player->positionMirror.z) - ((u16)self->srt.yaw);
    CIRCLE_WRAP(angle)
    self->srt.yaw += (s32)((angle * gUpdateRateF) / 12.0f);
}

