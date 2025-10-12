#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objanim.h"
#include "functions.h"
#include "dll.h"
#include "sys/rand.h"
#include "game/gamebits.h"

s32 func_80059C40(Vec3f*, Vec3f*, f32, s32, s32*, Object*, s32, s32, s32, s32);
s32 func_80057F1C(Object*, f32, f32, f32, f32***, s32, s32);

/*0x0*/ static const char str_0[] = "hit wall\n";

#define DUSTER_RANDOM_MAX 4
#define DUSTER_TOTAL_COUNT 100

typedef struct {
/*00*/ f32 unk0;
/*04*/ s16 timer1;
/*06*/ s16 timer2;
/*08*/ s16 gamebitCollected;
/*0A*/ s16 gamebitDiscovered;
/*0C*/ u8 rand;
/*0D*/ u8 resetTimer2;
/*0E*/ u8 unkE;
/*0F*/ u8 discovered;
/*10*/ u8 collected;
/*11*/ u8 unk11;
} Duster_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 _unk18[0x1B-0x18];
/*1B*/ u8 unk1B;
/*1C*/ u8 _unk1C[0x24-0x1C];
/*24*/ s16 gamebit;
} Duster_Setup;

// offset: 0x0 | ctor
void Duster_ctor(void *dll) { }

// offset: 0xC | dtor
void Duster_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void Duster_setup(Object *self, Duster_Setup *setup, s32 arg2) {
    Duster_Data *objdata = self->data;

    objdata->timer1 = rand_next(0, 50);
    objdata->unk0 = 0.02f;

    objdata->gamebitDiscovered = setup->gamebit;
    if (objdata->gamebitDiscovered >= BIT_Collected_Duster_1) {
        // this is not a hidden duster
        objdata->discovered = TRUE;
        objdata->gamebitCollected = objdata->gamebitDiscovered;
    } else {
        // this is a hidden duster, check if it has been discovered
        objdata->discovered = main_get_bits(objdata->gamebitDiscovered);
        objdata->gamebitCollected = objdata->gamebitDiscovered + DUSTER_TOTAL_COUNT;
    }
    objdata->collected = main_get_bits(objdata->gamebitCollected);
    objdata->unk11 = setup->unk1B;

    if (self->objhitInfo && !objdata->discovered) {
        self->objhitInfo->unk_0x58 |= 1;
    }

    if (objdata->collected || !objdata->discovered) {
        func_800267A4(self);
    }
}

// offset: 0x140 | func: 1 | export: 1
void Duster_control(Object *self) {
    s32 count;
    Object *player;
    SRT srt;
    f32 **sp54;
    s32 i;
    Duster_Data *objdata;
    PlayerStats *stats;

    objdata = self->data;
    player = get_player();
    if (!objdata->discovered || objdata->collected == TRUE) {
        if (!objdata->discovered) {
            objdata->discovered = main_get_bits(objdata->gamebitDiscovered);
            objdata->timer1 = 0;
        }
        return;
    }

    if (self->speed.y > -4.0f) {
        self->speed.y += -0.12f * delayFloat;
    }
    objdata->unkE = FALSE;
    count = func_80057F1C(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &sp54, 0, 0);

    for (i = 0; i < count; i++) {
        if (*(sp54[i]) < self->srt.transl.y + 140.0f) {
            self->srt.transl.y = *(sp54[i]);
            break;
        }
    }

    if (objdata->timer1 == 0 && objdata->timer2 == 0) {
        if (func_80024108(self, objdata->unk0, delayFloat, NULL) || objdata->unkE) {
            gDLL_6_AMSFX->vtbl->play_sound(self, 0x87B, 0x25, NULL, NULL, 0, NULL);
            gDLL_17->vtbl->func1(self, 0x51F, 0, 2, -1, 0);
            gDLL_17->vtbl->func1(self, 0x51F, 0, 2, -1, 0);

            objdata->rand = rand_next(0, DUSTER_RANDOM_MAX);
            if (objdata->rand == DUSTER_RANDOM_MAX) {
                gDLL_6_AMSFX->vtbl->play_sound(self, 0x87E, 0x25, NULL, NULL, 0, NULL);
            }
            if (objdata->unk11 != 0) {
                self->speed.x = 0.2f;
                self->speed.z = 0.0f;

                srt.roll = 0;
                srt.pitch = 0;
                srt.transl.x = 0.0f;
                srt.transl.y = 0.0f;
                srt.transl.z = 0.0f;
                srt.scale = 1.0f;
                srt.yaw = self->srt.yaw;
                rotate_vec3(&srt, &self->speed);
            } else {
                self->speed.x = 0.0f;
                self->speed.z = 0.0f;
            }
            if (objdata->resetTimer2) {
                objdata->timer2 = 250;
            }
        } else {
            self->srt.transl.x += self->speed.x * delayFloat;
            self->srt.transl.z += self->speed.z * delayFloat;
        }
        if (func_80025F40(self, NULL, NULL, NULL) == 0xF) {
            objdata->resetTimer2 = TRUE;
            gDLL_6_AMSFX->vtbl->play_sound(self, 0x6BC, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    } else {
        if (objdata->timer1 != 0) {
            objdata->timer1 = objdata->timer1 - (s16)delayFloat;
            if (objdata->timer1 <= 0) {
                objdata->timer1 = 0;
            }
        }
        if (objdata->timer2 != 0) {
            objdata->timer2 = objdata->timer2 - (s16)delayFloat;
            if (objdata->timer2 <= 0) {
                objdata->timer2 = 0;
                objdata->resetTimer2 = FALSE;
            }
        }
    }
    if (objdata->rand == DUSTER_RANDOM_MAX) {
        if (objdata->unkE) {
            self->srt.yaw -= 0x7FFF;
            objdata->rand = 0;
        }
        self->srt.yaw += 0xBB8 * (s16)delayFloat;
    }
    if (vec3_distance_xz(&player->positionMirror, &self->positionMirror) < 20.0f) {
        gDLL_6_AMSFX->vtbl->play_sound(self, 0x6CA, 0x7F, 0, 0, 0, 0);
        gDLL_17->vtbl->func1(self, 0x51A, 0, 1, -1, 0);
        gDLL_17->vtbl->func1(self, 0x51A, 0, 1, -1, 0);
        gDLL_17->vtbl->func1(self, 0x51A, 0, 1, -1, 0);
        main_set_bits(objdata->gamebitCollected, 1);
        objdata->collected = TRUE;
        func_800267A4(self);
        stats = gDLL_29_Gplay->vtbl->get_player_stats();
        stats->dusters++;
    }
    self->srt.transl.y += self->speed.y;
}

// offset: 0x754 | func: 2 | export: 2
void Duster_update(Object *self) {
    Duster_Data *objdata;
    s32 _pad1[21];
    s32 sp3C;
    s32 _pad2;

    objdata = self->data;
    if (func_80059C40(
        &self->positionMirror2,
        &self->srt.transl,
        30.0f,
        2,
        &sp3C,
        self,
        8,
        -1,
        0xFF,
        0) != 0) {
        objdata->unkE = TRUE;
    }

    self->positionMirror2.x = self->srt.transl.x;
    self->positionMirror2.y = self->srt.transl.y;
    self->positionMirror2.z = self->srt.transl.z;
}

// offset: 0x7FC | func: 3 | export: 3
void Duster_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    Duster_Data *objdata = self->data;

    if (visibility && objdata->discovered && !objdata->collected) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x870 | func: 4 | export: 4
void Duster_free(Object *self, s32 arg1) { }

// offset: 0x880 | func: 5 | export: 5
u32 Duster_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x890 | func: 6 | export: 6
u32 Duster_get_data_size(Object *self, u32 arg1) {
    return sizeof(Duster_Data);
}
