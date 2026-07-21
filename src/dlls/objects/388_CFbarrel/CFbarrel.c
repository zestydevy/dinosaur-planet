#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/17_partfx.h"
#include "dlls/engine/53_movelib.h"
#include "dlls/engine/54_pickup.h"
#include "dlls/objects/541_DIMexplosion.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/objmsg.h"
#include "sys/print.h"
#include "sys/rand.h"
#include "sys/intersect.h"
#include "dll.h"
#include "macros.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 unk18;
/*19*/ s8 unk19;
} CFbarrel_Setup;

typedef struct {
/*00*/ Pickup pickup;
/*0C*/ Object* unkC;
/*10*/ u8 _unk10;
/*11*/ u8 unk11;
/*12*/ u8 unk12;
/*13*/ u8 unk13;
/*14*/ s32 unk14;
/*18*/ Vec3f unk18;
/*24*/ f32 unk24;
/*28*/ f32 unk28;
/*2C*/ f32 unk2C;
/*30*/ s16 unk30;
/*32*/ s8 unk32;
/*34*/ s32 unk34;
/*38*/ s16 unk38;
/*3A*/ s16 unk3A;
/*3C:0*/ u8 unk3C_0 : 1;
/*3C:1*/ u8 unk3C_1 : 1;
/*3C:2*/ u8 unk3C_2 : 1;
/*3D*/ u8 unk3D;
/*3E:0*/ u8 unk3E_0 : 1;
/*3E:1*/ u8 unk3E_1 : 1;
} CFbarrel_Data;

/*0x0*/ static u32 data_0 = 0x00000000; // unused

static void CFbarrel_func_AD4(Object* self);
static void CFbarrel_func_1214(Object* self);
static void CFbarrel_func_14A0(Object* self, s16 a1, s16 a2);
static void CFbarrel_func_1848(Object* self);
static void CFbarrel_func_1948(Object* self, u8 a1);

// offset: 0x0 | ctor
void CFbarrel_ctor(void *dll) { }

// offset: 0xC | dtor
void CFbarrel_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFbarrel_setup(Object* self, CFbarrel_Setup* setup, s32 reset) {
    CFbarrel_Data* objdata;
    Pickup* pickup;

    objdata = self->data;
    pickup = self->data;
    pickup->flags |= PICKUPFLAG_NoGravity;
    gDLL_54_pickup->vtbl->setup(self, pickup, 90);
    objAddObjectType(self, OBJTYPE_Barrel);
    objAddObjectType(self, OBJTYPE_24);
    objInitMesgQueue(self, 4);
    self->srt.yaw = setup->unk18 << 8;
    self->unkE0 = 0;
    objdata->unk38 = 0;
    objdata->unk11 = 0;
    objdata->unk30 = 0;
    objdata->unk12 = 0;
    objdata->unk13 = 0;
    objdata->unk32 = 0;
    objdata->unk34 = 0;
    objdata->unk3D = 0;
    objdata->unk28 = 0.0f;
    objdata->unk3A = objdata->unk38;
    if (setup->unk19 > 0) {
        objdata->unk3C_0 = 0;
    } else {
        objdata->unk3C_0 = 1;
    }
    self->objhitInfo->unk58 = self->objhitInfo->unk58;
    func_8002674C(self);
    objdata->unk24 = (f32) self->objhitInfo->unk52;
    mainCreateTempDLL(DLL_ID_53_MOVELIB);
}

// offset: 0x190 | func: 1 | export: 1
void CFbarrel_control(Object* self) {
    CFbarrel_Data* objdata = self->data;
    
    if (objdata->unk14 != 0) {
        objdata->unk14 = objdata->unk14 - gUpdateRate;
        if (objdata->unk14 < 0) {
            objdata->unk14 = 0;
            objdata->unk13 = 0;
            objdata->unk12 = 0;
            objdata->unk3D |= 1;
        }
    } else {
        if ((self->parent != NULL) && (self->parent->id == OBJ_DR_PushCart)) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
        } else {
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        }
        if (mapWorldCoordsToBlockIndex(self->globalPosition.x, self->globalPosition.y, self->globalPosition.z) != -1) {
            if (objdata->unk13 != 0) {
                objdata->unk13 = objdata->unk13 + gUpdateRate;
                objdata->unk24 = (objdata->unk2C * (f32) objdata->unk13) + 1.0f;
                func_8002683C(self, (s16) objdata->unk24, (s16) (-objdata->unk24 * 0.5f), (s16) (objdata->unk24 * 0.5f));
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_353, NULL, 0, -1, NULL);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_353, NULL, 0, -1, NULL);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_353, NULL, 0, -1, NULL);
                if (objdata->unk13 >= 0x15) {
                    objdata->unk3E_0 = 0;
                    // Move to "create point" node
                    ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func7(0x1A, &self->srt);
                    STUBBED_PRINTF(" Should has Set position  %i %f %f \n", 0x1A, &self->srt.transl.x, &self->srt.transl.z);
                    self->srt.transl.x += (f32) mathRnd(-30, 30) * 0.1f;
                    self->srt.transl.z += (f32) mathRnd(-30, 30) * 0.1f;
                    bzero(&objdata->unk18, sizeof(Vec3f));
                    bzero(&self->velocity, sizeof(Vec3f));
                    if (objdata->unk3C_0) {
                        mapSaveObject(self->setup, self->mapID, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
                        objdata->unk14 = 0x258;
                        func_80026160(self);
                        func_8002683C(self, 8, -2, 0x19);
                    } else {
                        objDisable(self);
                        func_800267A4(self);
                        self->srt.flags |= OBJSTATE_PRINT_DISABLED;
                    }
                }
            } else {
                if (gDLL_54_pickup->vtbl->control(self, &objdata->pickup) == 0) {
                    if (objdata->unk11 != 0) {
                        objAddObjectType(self, OBJTYPE_24);
                    }
                    objdata->unk11 = 0;
                    func_8002674C(self);
                    if (objdata->unk3D & 1) {
                        CFbarrel_func_AD4(self);
                    }
                    CFbarrel_func_1214(self);
                } else {
                    objdata->unk3D |= 1;
                    objdata->unk11 = 1;
                    objdata->unk3E_1 = 1;
                }
                CFbarrel_func_1848(self);
                if (objdata->unk3E_0) {
                    self->unkAF |= ARROW_FLAG_8_No_Targetting;
                    if ((objdata->unk3E_1) && (objdata->unk3E_0)) {
                        objdata->unk18.x = self->velocity.x;
                        objdata->unk18.y = self->velocity.y;
                        objdata->unk18.z = self->velocity.z;
                        objdata->unk18.y = 0.0f;
                        objdata->unk3E_1 = 0;
                    }
                }
                if (objdata->unk3C_0) {
                    mapSaveObject(self->setup, self->mapID, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
                }
            }
        }
    }
}

// offset: 0x6B8 | func: 2 | export: 2
void CFbarrel_update(Object* self) {
    CFbarrel_Data* objdata = self->data;
    f32 sp90[3];
    TrackLineIntersectResult sp3C;

    if ((objdata->unk13 == 0) && (objdata->unk14 == 0)) {
        if (objdata->unkC != NULL) {
            trackIntersect_func_8005B5B8(self, objdata->unkC, 1);
            objdata->unkC = 0;
        }
        if (objdata->unk3E_0) {
            diPrintf(" floating ");
            sp90[0] = self->srt.transl.x - self->prevLocalPosition.x;
            sp90[1] = self->srt.transl.y - self->prevLocalPosition.y;
            sp90[2] = self->srt.transl.z - self->prevLocalPosition.z;
            sp90[0] *= 0.99f * (1.0f / gUpdateRateF);
            sp90[1] *= 0.99f * (1.0f / gUpdateRateF);
            sp90[2] *= 0.99f * (1.0f / gUpdateRateF);
            objdata->unk18.x += sp90[0];
            objdata->unk18.y += sp90[1];
            objdata->unk18.z += sp90[2];
            sp90[1] = 0.0f;
            objdata->unk3D |= 1;
            objdata->unk18.x *= 0.5f;
            objdata->unk18.y = 0.0f;
            objdata->unk18.z *= 0.5f;
        }
        if (objdata->unk11 == 0) {
            if (trackGetLineIntersect(&self->prevLocalPosition, &self->srt.transl, 4.0f, 1, &sp3C, self, 8, -1, 0xFF, 0) != 0) {
                STUBBED_PRINTF(" Line IDNO %i \n", sp3C.unk51);
                if ((objdata->unk3E_0) && (sp3C.unk51 == 3)) {
                    CFbarrel_func_1948(self, 0);
                } else {
                    self->velocity.x *= -1.1f;
                    self->velocity.z *= -1.1f;
                    objdata->unk18.x *= -1.1f;
                    objdata->unk18.z *= -1.1f;
                    STUBBED_PRINTF(" Hit Line ");
                }
            }
        }
        self->prevLocalPosition.x = self->srt.transl.x;
        self->prevLocalPosition.y = self->srt.transl.y;
        self->prevLocalPosition.z = self->srt.transl.z;
    }
}

// offset: 0x92C | func: 3 | export: 3
void CFbarrel_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CFbarrel_Data* objdata = self->data;
    if (objdata->unk13 == 0) {
        if (objdata->unk11 != 0) {
            self->srt.roll = 0;
            self->srt.pitch = 0;
        }
        if (gDLL_54_pickup->vtbl->should_print(self, visibility) != 0) {
            objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0x9DC | func: 4 | export: 4
void CFbarrel_free(Object* self, s32 onlySelf) {
    CFbarrel_Data* objdata = self->data;
    gDLL_54_pickup->vtbl->free(self);
    objFreeObjectType(self, OBJTYPE_Barrel);
    objFreeObjectType(self, OBJTYPE_24);
    if (objdata->unk13 != 0) {
        gDLL_13_Expgfx->vtbl->func5(self);
    }
    mainRemoveTempDLL(DLL_ID_53_MOVELIB);
}

// offset: 0xAB0 | func: 5 | export: 5
u32 CFbarrel_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xAC0 | func: 6 | export: 6
u32 CFbarrel_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(CFbarrel_Data);
}

/*0x50*/ static const char str_50[] = " No check ";

// offset: 0xAD4 | func: 7
static void CFbarrel_func_AD4(Object* self) {
    CFbarrel_Data* objdata = self->data;
    Object* obj;
    f32 temp_fa1;
    f32 temp_fv1;
    f32 var_fa0;
    f32 pad;
    s16 var_s2;
    s32 temp_v0;
    s32 spCC[1]; // size?
    s32 spC8; // s0
    s32 var_s3;
    s32 var_s4;
    s32 var_v1;
    f32 spB8;
    f32 temp;
    f32 spB0;
    f32 spAC;
    TrackHeightResult** spA8;
    s16 sp98[] = {0xffff, 0x0000, 0x0000, 0x0001, 0x0001, 0x0000, 0x0000, 0xffff};
    s16 sp90[] = {0x0002, 0x0003, 0x0000, 0x0001};
    s32* temp_v0_2;

    self->srt.yaw = 0;
    var_s2 = 0;
    var_s4 = 0;
    if (objdata->unk18.y > 0.01f) {
        objdata->unk18.y -= 0.1f;
        self->velocity.x = objdata->unk18.x * gUpdateRateF;
        self->velocity.y = objdata->unk18.y * gUpdateRateF;
        self->velocity.z = objdata->unk18.z * gUpdateRateF;
        objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);
        self->srt.roll += (objdata->unk18.y * 500.0f);
        return;
    }
    spAC = 0.0f;
    for (var_s3 = 0; var_s3 < 4; var_s3++) {
        spB8 =  (10.0f * mathSinfInterp(var_s2)) * mathCosfInterp(self->srt.roll);
        spB0 = (10.0f * mathCosfInterp(var_s2)) * mathCosfInterp(self->srt.pitch);
        temp = ((10.0f * mathSinfInterp(self->srt.roll)) * mathSinfInterp(var_s2)) + (10.0f * mathSinfInterp(self->srt.pitch) * mathCosfInterp(var_s2));

        var_s2 += 0x3FD2;

        temp_v0 = trackGetHeight(self, 
                                self->srt.transl.x + spB8, 
                                self->srt.transl.y + temp,
                                self->srt.transl.z + spB0, 
                                &spA8, 0, 1);
        if (temp_v0 != 0) {
            var_fa0 = 10000.0f;

            for (var_v1 = 0; var_v1 < temp_v0; var_v1++) {
                temp_fv1 = self->srt.transl.y - spA8[var_v1]->y;
                if (
                    (temp_fv1 > 0.0f && temp_fv1 < (var_fa0 >= 0 ? var_fa0 : -var_fa0)) || 
                    (temp_fv1 <= 0.0f && temp_fv1 > -20.0f)
                ) {
                    var_fa0 = temp_fv1;
                    spC8 = var_v1;
                }
            }
          
            if (var_fa0 <= 0.0f) {
                temp_v0_2 = &spCC[var_s4];
                var_s4 += 1;
                if (var_fa0 < spAC) {
                    spAC = var_fa0;
                }
                objdata->unk18.y = 0.0f;
                if (var_fa0 > 0.0f) {
                    *temp_v0_2 = var_s3;
                } else {
                    *temp_v0_2 = sp90[var_s3];
                }
            }
        }
    }
    if (var_s4 != 0) {
        self->velocity.y = -spAC;
    }
    if (var_s4 == 0) {
        objdata->unk18.y -= 0.1f;
        self->velocity.y = objdata->unk18.y * gUpdateRateF;
        self->velocity.x = objdata->unk18.x * gUpdateRateF;
        self->velocity.z = objdata->unk18.z * gUpdateRateF;
        if (objdata->unk18.y < -0.2f) {
            CFbarrel_func_14A0(self, objdata->unk38, objdata->unk3A);
        }
    }
    if (var_s4 > 0) {
        STUBBED_PRINTF(" Landed On World Obj ");
        objdata->unk3D &= ~0x1;
        obj = spA8[spC8]->obj;
        if (obj != NULL) {
            if ((obj->def->flags & OBJDEF_IS_MOBILE_MAP) && !(obj->def->flags & OBJDEF_MOBILE_MAP_NEVER_PLAYER_PARENT)) {
                objdata->unkC = obj;
                STUBBED_PRINTF(" ob Obj %x  Defno %i \n\n", obj, obj->id);
            }
        }
        objdata->unk18.x = 0.0f;
        objdata->unk18.y = 0.0f;
        objdata->unk18.z = 0.0f;
        self->srt.roll = 0;
        self->srt.pitch = 0;
    }
    if ((objdata->unk3E_0) && (var_s4 == 4)) {
        CFbarrel_func_1948(self, 0);
    }
    objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);
}

// offset: 0x1214 | func: 8
static void CFbarrel_func_1214(Object* self) {
    CFbarrel_Data* objdata = self->data;
    Object* hitBy;
    DIMExplosion_Setup* explSetup;
    u32 damageType;

    damageType = func_80025F40(self, &hitBy, NULL, NULL);
    if ((damageType != 0) && (hitBy != self->parent)) {
        objdata->unk12++;
        if ((damageType == Damage_Type_Explosion) || (damageType == Damage_Type_Projectile)) {
            objdata->unk12 = 3;
        }
        STUBBED_PRINTF(" Object Hit ");
        if (objdata->unk12 == 3) {
            self->objhitInfo->unk58 |= 4;
            self->objhitInfo->unk5F = 5;
            self->objhitInfo->unk60 = 4;
            self->objhitInfo->unk44 = 0x10;
            self->objhitInfo->unk40 = 0x10;
            self->objhitInfo->unk58 |= 1;
            self->objhitInfo->unk52 = 0x14;
            self->visRadius = 100.0f;
            dll_amSfx->Play(self, SOUND_860_Explosion_Mid, MAX_VOLUME, NULL, NULL, 0, NULL);
            self->srt.transl.y += 10.0f;
            explSetup = objAllocSetup(sizeof(DIMExplosion_Setup), OBJ_DIMExplosion);
            explSetup->base.x = self->srt.transl.x;
            explSetup->base.y = self->srt.transl.y;
            explSetup->base.z = self->srt.transl.z;
            objSetupObject((ObjSetup*)explSetup, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, self->parent);
            objdata->unk13 = 1;
            if (self->parent != NULL) {
                objdata->unk2C = 3.5f;
            } else {
                objdata->unk2C = 3.5f;
            }
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_355, NULL, 0, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_352, NULL, 0, -1, NULL);
        } else {
            dll_amSfx->Play(self, SOUND_372_Crate_Struck, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    }
}

// offset: 0x14A0 | func: 9
static void CFbarrel_func_14A0(Object* self, s16 arg1, s16 arg2) {
    Object* sp5C;
    f32 sp58[1];
    f32 var_fa0;
    f32 temp_ft4;
    f32 var_fv0;
    f32 var_fv1;
    f32 sp44;
    Object* player;
    s32 pad[2];

    sp44 = 300.0f;
    player = objGetPlayer();
    sp5C = objGetNearestTypeTo(OBJTYPE_32, self, &sp44);
    if (sp5C == NULL) {
        return;
    }
    var_fv0 = sp5C->srt.transl.y;
    var_fv1 = player->srt.transl.y;
    if (var_fv1 <= var_fv0) {
        var_fa0 = var_fv0 - var_fv1;
    } else {
        var_fa0 = -(var_fv0 - var_fv1);
    }
    if (var_fa0 < 30.0f) {
        return;
    }
    sp58[0] = sp5C->srt.transl.x - self->srt.transl.x;
    var_fa0 = var_fv0 - self->srt.transl.y;
    if (var_fa0 > 0.0f) {
        return;
    }
    temp_ft4 = sp5C->srt.transl.z - self->srt.transl.z;
    if (var_fa0 != 0/*.0f*/) {
        var_fv1 = self->velocity.y / var_fa0;
    } else {
        var_fv1 = 0.0f;
    }
    if (var_fv1 >= 1.0f) {
        STUBBED_PRINTF(" landed ");
        dll_amSfx->Play(self, SOUND_8E2, MAX_VOLUME, NULL, NULL, 0, NULL);
        arg1 = 0;
        arg2 = 0;
        var_fv1 = 1.0f;
        self->velocity.y = var_fa0;
        sp5C->srt.transl.x += 20.0f;
        sp5C->velocity.z += 20.0f;
        var_fv0 = sp5C->velocity.z;
        if (var_fv0 > 180.0f) {
            sp5C->velocity.z = 0.0f;
            sp5C->srt.transl.x -= var_fv0;
        }
        self->srt.pitch = 0;
        self->srt.roll = 0;
    }
    self->velocity.x = sp58[0] * var_fv1;
    self->velocity.z = temp_ft4 * var_fv1;
    if (arg1 != 0) {
        if (arg1 == 1) {
            var_fv0 = (u16)self->srt.pitch;
            var_fv0 = (65536.0f - var_fv0) * var_fv1;
        } else {
            var_fv0 = (u16)self->srt.pitch;
            var_fv0 *= var_fv1 * arg1;
        }
        self->srt.pitch += var_fv0;
    }
    if (arg2 != 0) {
        if (arg2 == 1) {
            var_fv0 = (u16)self->srt.roll;
            var_fv0 *= 0.0f; // lol
        } else {
            var_fv0 = (u16)self->srt.roll;
            var_fv0 *= var_fv1 * arg2;
        }
        self->srt.roll += var_fv0;
    }
}

// offset: 0x17F4 | func: 10 | export: 7
void CFbarrel_func_17F4(Object* self, Vec3f* arg1) {
    CFbarrel_Data* objdata = self->data;
    if (objdata->unk11 == 0) {
        STUBBED_PRINTF(" Force Applied x %f y %f z%f \n", &arg1->x, &arg1->y, &arg1->z);
        objdata->unk18.y += arg1->y;
        objdata->unk18.x += arg1->x;
        objdata->unk18.z += arg1->z;
        objdata->unk3D |= 1;
    }
}

// offset: 0x1848 | func: 11
static void CFbarrel_func_1848(Object* self) {
    u32 mesgID;
    void* mesgArg;

    mesgID = 0;
    mesgArg = NULL;
    while (objRecvMesg(self, &mesgID, NULL, &mesgArg) != 0) {
        switch (mesgID) {
        case 15:
            CFbarrel_func_1948(self, 1);
            break;
        case 16:
            CFbarrel_func_1948(self, 0);
            break;
        }
    }
}

// offset: 0x1948 | func: 12
static void CFbarrel_func_1948(Object* self, u8 a1) {
    CFbarrel_Data* objdata = self->data;
    ObjectHitInfo* hit = self->objhitInfo;
    
    if (a1) {
        // Let barrel be pushed
        hit->unk5B = 1;
        hit->unk5C = 1;
        STUBBED_PRINTF(" In ELEVATOR");
        self->objhitInfo->unk58 |= 0x400;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        objdata->unk3E_0 = 1;
    } else {
        // Disable push
        hit->unk5B = self->def->unk91;
        hit->unk5C = self->def->unk92;
        objFreeObjectType(self, OBJTYPE_24);
        STUBBED_PRINTF("Out of ELEVATOR");
        objdata->unk3E_0 = 0;
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        self->objhitInfo->unk58 &= ~0x400;
        objdata->unk3D |= 1;
    }
}
