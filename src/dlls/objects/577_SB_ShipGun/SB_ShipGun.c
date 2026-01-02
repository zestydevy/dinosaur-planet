#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/572_SB_Galleon.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "segment_334F0.h"
#include "functions.h"
#include "dll.h"

typedef struct {
/*00*/ Object *cloudrunner;
/*04*/ s16 _unk4;
/*06*/ s16 unk6;
/*08*/ s16 unk8;
/*0A*/ s16 unkA;
/*0C*/ s8 state;
/*0D*/ s8 unkD;
/*0E*/ s8 unkE;
/*0F*/ u8 unkF;
/*10*/ u8 unk10;
} SB_ShipGun_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 _unk18;
/*19*/ s8 gunIndex;
} SB_ShipGun_Setup;

typedef enum {
    STATE_0,
    STATE_1,
    STATE_2,
    STATE_3,
    STATE_4
} SB_ShipGun_State;

// offset: 0x0 | ctor
void SB_ShipGun_ctor(void *dll) { }

// offset: 0xC | dtor
void SB_ShipGun_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SB_ShipGun_setup(Object *self, SB_ShipGun_Setup *setup, s32 arg2) {
    SB_ShipGun_Data *objdata;

    objdata = self->data;
    objdata->unkF = 0;
    objdata->unkE = 2;
    objdata->unk10 = 0;
}

#ifndef NON_MATCHING
// offset: 0x3C | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/577_SB_ShipGun/SB_ShipGun_control.s")
#else
// regalloc+stack
// https://decomp.me/scratch/pTrdM
void SB_ShipGun_control(Object *self) {
    f32 *new_var3;
    f32 new_var;
    f32 spA4;
    Object *parent;
    Object *player;
    f32 spA0;
    SB_ShipGun_Setup *setup;
    f32 sp9C;
    ObjSetup *cannonballSetup;
    ObjSetup *debrisSetup;
    Object **objects;
    Object *cannonballObj;
    f32 temp_fa0;
    f32 temp_fa1;
    s32 var_s0_2;
    f32 temp_ft0;
    f32 dx1;
    f32 dy1;
    f32 dz1;
    f32 sp98;
    f32 sp94;
    f32 sp90;
    Vec3f sp84;
    f32 dy;
    f32 dx;
    f32 dz;
    s32 objIndex;
    s32 objCount;
    s32 _pad[2];
    s32 i;
    SB_ShipGun_Data *objdata;
    SRT transform;
    s16 *sp44;

    player = get_player();
    objdata = self->data;
    setup = (SB_ShipGun_Setup*)self->setup;
    sp44 = func_80034804(self, 0);
    if (self->parent->id == OBJ_WL_Galleon) {
        self->objhitInfo->unk58 &= ~1;
        objdata->unkF = 0;
        return;
    }
    if (objdata->cloudrunner == NULL) {
        objects = get_world_objects(&objIndex, &objCount);
        for (i = objIndex; i < objCount; i++) {
            if (objects[i]->id == OBJ_SB_Cloudrunner) {
                objdata->cloudrunner = objects[i];
                i = objCount;
            }
        }
    }
    parent = self->parent;
    var_s0_2 = 0;
    if ((parent != NULL) && (parent->id == OBJ_SB_Galleon)) {
        var_s0_2 = ((DLL_572_SB_Galleon*)parent->dll)->vtbl->func8(parent);
    } else {
        objdata->state = STATE_4;
    }
    objdata->unkF = 1;
    switch (objdata->state) {
    case STATE_0:
        objdata->unkF = 0;
        if (self->curModAnimId != 1) {
            func_80023D30(self, 1, 0.0f, 0);
        }
        func_80024108(self, 0.0f, gUpdateRate, NULL);
        if ((parent != NULL) && (((DLL_572_SB_Galleon*)parent->dll)->vtbl->func9(parent) == 0)) {
            if (setup->gunIndex == 0) { // right gun
                if ((var_s0_2 == 0) || (var_s0_2 == 3)) {
                    objdata->state = STATE_1;
                }
            } else { // left gun
                if ((var_s0_2 == 1) || (var_s0_2 == 4)) {
                    objdata->state = STATE_1;
                }
            }
        }
        if (objdata->state == STATE_1) {
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_174_Machinery_Move_A, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        self->objhitInfo->unk58 &= ~1;
        break;
    case STATE_1:
        if ((parent != NULL) && (((DLL_572_SB_Galleon*)parent->dll)->vtbl->func9(parent) != 0)) {
            objdata->state = STATE_3;
        }
        self->objhitInfo->unk58 &= ~1;
        if (self->curModAnimId != 1) {
            func_80023D30(self, 1, 0.0f, 0);
        }
        if (func_80024108(self, 0.01f, gUpdateRate, NULL) != 0) {
            objdata->state = STATE_2;
        }
        objdata->unk8 = 0x50;
        break;
    case STATE_2:
        if (self->curModAnimId != 0) {
            func_80023D30(self, 0, 0.5f, 0);
        }
        self->objhitInfo->unk58 |= 1;
        if (func_80025F40(self, NULL, NULL, NULL) != 0) {
            objdata->unkA = 0xFF;
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_176_Explosion_A, MAX_VOLUME, NULL, NULL, 0, NULL);
            objdata->unkD++;
            if (objdata->unkD == 2) {
                objdata->state = STATE_3;
                if (parent != NULL) {
                    ((DLL_572_SB_Galleon*)parent->dll)->vtbl->func7(parent);
                }
            } else if (objdata->unkD == 4) {
                gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_177_Explosion_B, MAX_VOLUME, NULL, NULL, 0, NULL);
                objdata->state = STATE_3;
                for (i = 5; i != 0; i--) {
                    gDLL_17_partfx->vtbl->spawn(self, 0x9B, NULL, 2, -1, NULL);
                }
                if (parent != NULL) {
                    ((DLL_572_SB_Galleon*)parent->dll)->vtbl->func7(parent);
                }
            }
        }
        if ((parent != NULL) && (((DLL_572_SB_Galleon*)parent->dll)->vtbl->func9(parent) != 0)) {
            objdata->state = STATE_3;
        }
        dx1 = player->positionMirror.f[0] - self->positionMirror.f[0];
        dz1 = player->positionMirror.f[2] - self->positionMirror.f[2];
        self->srt.yaw = arctan2_f(-dz1, dx1) * 2;
        new_var = self->positionMirror.f[1];
        dy1 = player->positionMirror.f[1] - new_var;
        objdata->unk6 = arctan2_f(-dy1, sqrtf((dx1 * dx1) + (dz1 * dz1)));
        if (objdata->unk6 >= 0x1F41) {
            objdata->unk6 = 0x1F40;
        } else if (objdata->unk6 < (-0x1F40)) {
            objdata->unk6 = -0x1F40;
        }
        *sp44 = objdata->unk6;
        objdata->unk8 -= gUpdateRate;
        if (objdata->unk8 < 0) {
            get_object_child_position(self, &sp98, &sp94, &sp90);
            transform.transl.x = 0.0f;
            transform.transl.y = 0.0f;
            transform.transl.z = 0.0f;
            transform.scale = 1.0f;
            transform.yaw = self->srt.yaw;
            transform.pitch = 0;
            transform.roll = 0;
            sp84.x = 100.0f;
            sp84.y = 135.f;
            sp84.z = 0.0f;
            rotate_vec3(&transform, (f32*)(&sp84));
            cannonballSetup = obj_alloc_create_info(sizeof(ObjSetup), OBJ_SB_CannonBall);
            cannonballSetup->x = sp84.x + sp98;
            cannonballSetup->y = sp84.y + sp94;
            cannonballSetup->z = sp84.z + sp90;
            cannonballSetup->loadFlags = 2;
            cannonballSetup->fadeFlags = 1;
            cannonballSetup->loadDistance = 0xFF;
            cannonballSetup->fadeDistance = 0xFF;
            cannonballObj = obj_create(cannonballSetup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, -1, -1, NULL);
            if (0) { }
            dx = (dx1 = objdata->cloudrunner->positionMirror.x - self->positionMirror.x);
            dy = objdata->cloudrunner->positionMirror.y - (self->positionMirror.y + 100.0f);
            dz1 = objdata->cloudrunner->positionMirror.z - self->positionMirror.z;
            dz = dz1;
            temp_ft0 = 10.0f / sqrtf(((dx * dx) + (dy * dy)) + (dz * dz));
            new_var3 = &temp_ft0;
            cannonballObj->speed.x = dx * (*new_var3);
            cannonballObj->speed.y = dy * (*(&temp_ft0));
            dy = cannonballObj->speed.x;
            cannonballObj->speed.z = dz * (*(&temp_ft0));
            cannonballObj->srt.transl.x += dy * 8.0f;
            cannonballObj->srt.transl.y += cannonballObj->speed.y * 8.0f;
            cannonballObj->srt.transl.z += cannonballObj->speed.z * 8.0f;
            cannonballObj->srt.yaw = arctan2_f(cannonballObj->speed.x, cannonballObj->speed.z);
            cannonballObj->unkDC = 0xB4;
            cannonballObj->unkE0 = (s32) objdata->cloudrunner;
            func_800013BC();
            func_80003B70(0.1f);
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_96_Cannon, MAX_VOLUME, NULL, NULL, 0, NULL);
            objdata->unk10++;
            if (objdata->unk10 == 3) {
                objdata->unk8 = 0x10E;
                objdata->unk10 = 0U;
            } else {
                objdata->unk8 = 0xA0;
            }
        }
        break;
    case STATE_3:
        self->objhitInfo->unk58 &= ~1;
        get_object_child_position(self, &sp98, &sp94, &sp90);
        if (self->curModAnimId != 1) {
            for (i = 0; i != 6; i++) {
                if (objdata->unkE == 1) {
                    debrisSetup = obj_alloc_create_info(sizeof(ObjSetup), OBJ_SB_Debris);
                    debrisSetup->x = sp98;
                    debrisSetup->y = sp94 + 135.0f;
                    debrisSetup->z = sp90;
                    debrisSetup->loadFlags = 2;
                    debrisSetup->fadeFlags = 2;
                    debrisSetup->loadDistance = 0xFF;
                    debrisSetup->fadeDistance = 0xFF;
                    obj_create(debrisSetup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, -1, -1, NULL);
                }
                gDLL_17_partfx->vtbl->spawn(self, 0x9B, NULL, PARTFXFLAG_2, -1, NULL);
                main_set_bits(setup->gunIndex + BIT_SB_Destroyed_Right_Gun, 1);
            }
            if (objdata->unkE > 0) {
                objdata->unkE--;
            }
            func_80023D30(self, 1, 1.0f, 0U);
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_175_Machinery_Move_B, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        if (func_80024108(self, -0.01f, gUpdateRate, NULL) != 0) {
            objdata->state = STATE_0;
        }
        break;
    case STATE_4:
        obj_destroy_object(self);
        break;
    }
}
#endif

// offset: 0xB40 | func: 2 | export: 2
void SB_ShipGun_update(Object *self) {
    SB_ShipGun_Data *objdata;

    objdata = self->data;
    if (objdata->unkA > 0) {
        objdata->unkA -= gUpdateRate * 3;
        if (objdata->unkA < 0) {
            objdata->unkA = 0;
        }
    }
}

// offset: 0xB8C | func: 3 | export: 3
void SB_ShipGun_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    SB_ShipGun_Data *objdata;

    objdata = self->data;
    if (self->parent && self->parent->id == OBJ_WL_Galleon)
        return;
    if (visibility && objdata->unkE != 0 && objdata->unkF != 0) {
        if (objdata->unkA != 0) {
            func_80036F6C(0xFF, 0xFF - objdata->unkA, 0xFF - objdata->unkA);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xC70 | func: 4 | export: 4
void SB_ShipGun_free(Object *self, s32 a1) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0xCB8 | func: 5 | export: 5
u32 SB_ShipGun_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xCC8 | func: 6 | export: 6
u32 SB_ShipGun_get_data_size(Object *self, u32 a1) {
    return sizeof(SB_ShipGun_Data);
}
