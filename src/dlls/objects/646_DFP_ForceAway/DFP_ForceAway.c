#include "dlls/engine/17_partfx.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "dll.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yRadiusQuarter;
/*19*/ s8 type;
/*1A*/ s16 xRadius;
/*1C*/ s16 zRadius;
} DFP_ForceAway_Setup;

typedef struct {
/*0*/ s16 xRadius;
/*2*/ s16 zRadius;
/*4*/ s16 yRadius;
/*6*/ s16 cooldownTimer;
/*8*/ u8 unk8; // unused
/*9*/ u8 _unk9;
/*A*/ u8 type;
/*B*/ u8 _unkB;
/*C*/ u8 prevPlayerXSide;
/*D*/ u8 prevPlayerYSide;
/*E*/ u8 prevPlayerZSide;
} DFP_ForceAway_Data;

static void DFP_ForceAway_type0_control(Object* self);
static void DFP_ForceAway_type1_control(Object* self);
static void DFP_ForceAway_type2_control(Object* self);
static void DFP_ForceAway_type3_control(Object* self);

// offset: 0x0 | ctor
void DFP_ForceAway_ctor(void *dll) { }

// offset: 0xC | dtor
void DFP_ForceAway_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFP_ForceAway_setup(Object* self, DFP_ForceAway_Setup* setup, s32 reset) {
    DFP_ForceAway_Data* objdata = self->data;
    objdata->unk8 = setup->type;
    objdata->yRadius = (setup->yRadiusQuarter * 4);
    objdata->xRadius = setup->xRadius;
    objdata->zRadius = setup->zRadius;
    objdata->type = setup->type;
    objdata->prevPlayerXSide = 0;
    objdata->prevPlayerYSide = 0;
    objdata->prevPlayerZSide = 0;
    objdata->cooldownTimer = 0;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x70 | func: 1 | export: 1
void DFP_ForceAway_control(Object* self) {
    DFP_ForceAway_Data* objdata = self->data;

    if (objdata->type == 0) {
        DFP_ForceAway_type0_control(self);
    } else if (objdata->type == 1) {
        DFP_ForceAway_type1_control(self);
    } else if (objdata->type == 2) {
        DFP_ForceAway_type2_control(self);
    } else if (objdata->type == 3) {
        DFP_ForceAway_type3_control(self);
    }
}

// offset: 0x128 | func: 2 | export: 2
void DFP_ForceAway_update(Object *self) { }

// offset: 0x134 | func: 3 | export: 3
void DFP_ForceAway_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x14C | func: 4 | export: 4
void DFP_ForceAway_free(Object* self, s32 onlySelf) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x194 | func: 5 | export: 5
u32 DFP_ForceAway_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1A4 | func: 6 | export: 6
u32 DFP_ForceAway_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DFP_ForceAway_Data);
}

// offset: 0x1B8 | func: 7
static void DFP_ForceAway_type0_control(Object* self) {
    DFP_ForceAway_Data* objdata;
    Object* player;
    u8 axisCounter;
    u8 xSide;
    u8 ySide;
    u8 zSide;
    f32 zDist;
    f32 xDist;
    f32 yDist;
    SRT partSRT;

    objdata = self->data;
    player = get_player();
    xDist = player->srt.transl.x - self->srt.transl.x;
    yDist = player->srt.transl.y - self->srt.transl.y;
    zDist = player->srt.transl.z - self->srt.transl.z;
    axisCounter = 0;
    xSide = 0;
    ySide = 0;
    zSide = 0;
    if ((xDist <= 0.0f) && (-(f32) objdata->xRadius < xDist)) {
        axisCounter++;
        xSide++;
    }
    if ((xDist > 0.0f) && (xDist < (f32) objdata->xRadius)) {
        axisCounter++;
        xSide--;
    }
    if ((zDist <= 0.0f) && (-(f32) objdata->zRadius < zDist)) {
        axisCounter++;
        zSide++;
    }
    if ((zDist > 0.0f) && (zDist < (f32) objdata->zRadius)) {
        axisCounter++;
        zSide--;
    }
    if ((yDist <= 0.0f) && (-(f32) objdata->yRadius < yDist)) {
        axisCounter++;
        ySide++;
    }
    if ((yDist > 0.0f) && (yDist < (f32) objdata->yRadius)) {
        axisCounter++;
        ySide--;
    }
    if (axisCounter == 3) {
        partSRT.transl.x = xDist;
        partSRT.transl.y = yDist;
        partSRT.transl.z = zDist;
        partSRT.roll = 0;
        partSRT.pitch = 0;
        partSRT.yaw = 0;
        partSRT.scale = 1.0f;
        if (xSide != objdata->prevPlayerXSide) {
            partSRT.yaw = 0x3FFF;
        }
        if (mainGetBits(BIT_Player_Immune_to_Rainbow_Scarabs) != 0) {
            mainSetBits(BIT_468, 1);
            obj_send_mesg(player, 0x60004, self, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_5ED, &partSRT, PARTFXFLAG_2, -1, NULL);
        } else {
            obj_send_mesg(player, 0x60004, self, (void* )1);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_5ED, &partSRT, PARTFXFLAG_2, -1, NULL);
        }
        gDLL_6_AMSFX->vtbl->play(self, SOUND_837, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    objdata->prevPlayerXSide = xSide;
    objdata->prevPlayerYSide = ySide;
    objdata->prevPlayerZSide = zSide;
}

// offset: 0x548 | func: 8
static void DFP_ForceAway_type1_control(Object* self) {
    DFP_ForceAway_Data* objdata;
    Object* player;
    f32 yDist;
    f32 xDist;
    f32 zDist;
    u8 axisCounter;
    f32 sp24;

    objdata = self->data;
    player = get_player();
    axisCounter = 0;
    xDist = player->srt.transl.x - self->srt.transl.x;
    yDist = player->srt.transl.y - self->srt.transl.y;
    zDist = player->srt.transl.z - self->srt.transl.z;
    if ((xDist <= 0.0f) && (-(f32) objdata->xRadius < xDist)) {
        axisCounter++;
    }
    if ((xDist > 0.0f) && (xDist < (f32) objdata->xRadius)) {
        axisCounter++;
    }
    if ((zDist <= 0.0f) && (-(f32) objdata->zRadius < zDist)) {
        axisCounter++;
    }
    if ((zDist > 0.0f) && (zDist < (f32) objdata->zRadius)) {
        axisCounter++;
    }
    if ((yDist <= 0.0f) && (-(f32) objdata->yRadius < yDist)) {
        axisCounter++;
    }
    if ((yDist > 0.0f) && (yDist < (f32) objdata->yRadius)) {
        axisCounter++;
    }
    if (axisCounter == 3) {
        sp24 = (f32) rand_next(-0x17, 0x17) * 0.01f;
        ((DLL_210_Player*)player->dll)->vtbl->func73(player, sp24, (f32) rand_next(-0x17, 0x17) * 0.01f);
    }
}

// offset: 0x7A8 | func: 9
static void DFP_ForceAway_type2_control(Object* self) {
    /*0x0*/ static u8 data_0 = 0;
    DFP_ForceAway_Data* objdata;
    Object* player;
    u8 axisCounter;
    u8 xSide;
    u8 ySide;
    u8 zSide;
    f32 zDist;
    f32 xDist;
    f32 yDist;
    SRT partSRT;
    s32 _pad;

    objdata = self->data;
    player = get_player();
    axisCounter = 0;
    xSide = 0;
    ySide = 0;
    zSide = 0;
    xDist = player->srt.transl.x - self->srt.transl.x;
    yDist = player->srt.transl.y - self->srt.transl.y;
    zDist = player->srt.transl.z - self->srt.transl.z;
    data_0 += 1;
    if ((xDist <= 0.0f) && (-(f32) objdata->xRadius < xDist)) {
        axisCounter++;
        xSide++;
    }
    if ((xDist > 0.0f) && (xDist < (f32) objdata->xRadius)) {
        axisCounter++;
        xSide--;
    }
    if ((zDist <= 0.0f) && (-(f32) objdata->zRadius < zDist)) {
        axisCounter++;
        zSide++;
    }
    if ((zDist > 0.0f) && (zDist < (f32) objdata->zRadius)) {
        axisCounter++;
        zSide--;
    }
    if ((yDist <= 0.0f) && (-(f32) objdata->yRadius < yDist)) {
        axisCounter++;
        ySide++;
    }
    if ((yDist > 0.0f) && (yDist < (f32) objdata->yRadius)) {
        axisCounter++;
        ySide--;
    }
    if (axisCounter == 3) {
        partSRT.transl.x = xDist;
        partSRT.transl.y = yDist;
        partSRT.transl.z = zDist;
        partSRT.scale = 1.0f;
        partSRT.roll = 0;
        partSRT.pitch = 0;
        partSRT.yaw = 0;
        if (xSide != objdata->prevPlayerXSide) {
            partSRT.yaw = 0x3FFF;
        }
        if (((DLL_210_Player*)player->dll)->vtbl->func50(player) == 0x1D7) {
            if (data_0 > 20) {
                data_0 = 0;
                mainSetBits(BIT_468, 1);
                gDLL_6_AMSFX->vtbl->play(self, SOUND_837, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            gDLL_17_partfx->vtbl->spawn(player, PARTICLE_397, NULL, PARTFXFLAG_2, -1, NULL);
        } else {
            mainSetBits(BIT_468, 1);
            obj_send_mesg(player, 0x60004, self, (void* )2);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_399, &partSRT, PARTFXFLAG_2, -1, NULL);
            gDLL_6_AMSFX->vtbl->play(self, SOUND_837, MAX_VOLUME, NULL, NULL, 0, NULL);

        }
    }
    objdata->prevPlayerXSide = xSide;
    objdata->prevPlayerYSide = ySide;
    objdata->prevPlayerZSide = zSide;
}

// offset: 0xBB4 | func: 10
static void DFP_ForceAway_type3_control(Object* self) {
    DFP_ForceAway_Data* objdata;
    Object* player;
    u8 axisCounter;
    u8 xSide;
    u8 ySide;
    u8 zSide;
    f32 yDist;
    f32 xDist;
    f32 zDist;

    objdata = self->data;
    player = get_player();
    xDist = player->srt.transl.x - self->srt.transl.x;
    yDist = player->srt.transl.y - self->srt.transl.y;
    zDist = player->srt.transl.z - self->srt.transl.z;
    axisCounter = 0;
    xSide = 0;
    ySide = 0;
    zSide = 0;
    if ((xDist <= 0.0f) && (-(f32) objdata->xRadius < xDist)) {
        axisCounter++;
        xSide++;
    }
    if ((xDist > 0.0f) && (xDist < (f32) objdata->xRadius)) {
        axisCounter++;
        xSide--;
    }
    if ((zDist <= 0.0f) && (-(f32) objdata->zRadius < zDist)) {
        axisCounter++;
        zSide++;
    }
    if ((zDist > 0.0f) && (zDist < (f32) objdata->zRadius)) {
        axisCounter++;
        zSide--;
    }
    if ((yDist <= 0.0f) && (-(f32) objdata->yRadius < yDist)) {
        axisCounter++;
        ySide++;
    }
    if ((yDist > 0.0f) && (yDist < (f32) objdata->yRadius)) {
        axisCounter++;
        ySide--;
    }
    if (objdata->cooldownTimer >= 0) {
        objdata->cooldownTimer -= (s16) gUpdateRateF;
    }
    if ((axisCounter == 3) && (objdata->cooldownTimer <= 0)) {
        if (((DLL_210_Player*)player->dll)->vtbl->func50(player) == 0x1D7) {
            mainSetBits(BIT_468, 1);
            gDLL_6_AMSFX->vtbl->play(player, SOUND_B47, MAX_VOLUME, NULL, NULL, 0, NULL);
            gDLL_17_partfx->vtbl->spawn(player, PARTICLE_397, NULL, PARTFXFLAG_2, -1, NULL);
            objdata->cooldownTimer = 200;
        } else {
            func_8002635C(player, NULL, Damage_Type_13, /*damage*/2, 0);
            gDLL_6_AMSFX->vtbl->play(player, SOUND_B47, MAX_VOLUME, NULL, NULL, 0, NULL);
            objdata->cooldownTimer = 200;
        }
    }
    objdata->prevPlayerXSide = xSide;
    objdata->prevPlayerYSide = ySide;
    objdata->prevPlayerZSide = zSide;
}
