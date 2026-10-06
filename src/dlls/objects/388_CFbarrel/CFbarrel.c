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

/** @file official filename: CFbarrel.c */

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw8;
/*19*/ s8 unk19;
} CFbarrel_Setup;

typedef struct {
/*00*/ Pickup pickup;
/*0C*/ Object* mobileMap;
/*10*/ u8 _unk10;
/*11*/ u8 isHeld;
/*12*/ u8 hits; // how many times the barrel has been hit
/*13*/ u8 explodedTime; // how long it's been since the barrel exploded
/*14*/ s32 respawnTimer; // time remaining until respawn
/*18*/ Vec3f targVelocity;
/*24*/ f32 unk24;
/*28*/ f32 unk28;
/*2C*/ f32 unk2C;
/*30*/ s16 unk30;
/*32*/ s8 unk32;
/*34*/ s32 unk34;
/*38*/ s16 unk38;
/*3A*/ s16 unk3A;
/*3C:0*/ u8 isLevelObj : 1; // part of the level, has a respawn point, etc
/*3D*/ u8 flags;
/*3E:0*/ u8 inWindLift : 1;
/*3E:1*/ u8 unk3E_1 : 1;
} CFbarrel_Data;

enum CFbarrelFlags {
    CFBARREL_Physics_Enabled = 0x1
};

/*0x0*/ static u32 data_0 = 0x00000000; // unused

static void CFbarrel_doPhysics(Object* self);
static void CFbarrel_checkHit(Object* self);
static void CFbarrel_moveToAttractor(Object* self, s16 a1, s16 a2);
static void CFbarrel_processMesgLoop(Object* self);
static void CFbarrel_setWindLiftState(Object* self, u8 a1);

// offset: 0x0 | ctor
void CFbarrel_ctor(void *dll) { }

// offset: 0xC | dtor
void CFbarrel_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFbarrel_obj_Setup(Object* self, CFbarrel_Setup* setup, s32 reset) {
    CFbarrel_Data* objdata = self->data;
    Pickup* pickup = self->data;
    
    pickup->flags |= PICKUPFLAG_NoGravity;
    gDLL_54_pickup->vtbl->setup(self, pickup, 90);
    objAddObjectType(self, OBJTYPE_Barrel);
    objAddObjectType(self, OBJTYPE_WindLiftable);
    objInitMesgQueue(self, 4);
    self->srt.yaw = setup->yaw8 << 8;
    self->unkE0 = 0;
    objdata->unk38 = 0;
    objdata->isHeld = FALSE;
    objdata->unk30 = 0;
    objdata->hits = 0;
    objdata->explodedTime = 0;
    objdata->unk32 = 0;
    objdata->unk34 = 0;
    objdata->flags = 0;
    objdata->unk28 = 0.0f;
    objdata->unk3A = objdata->unk38;
    if (setup->unk19 > 0) {
        objdata->isLevelObj = FALSE;
    } else {
        objdata->isLevelObj = TRUE;
    }
    self->objhitInfo->unk58 = self->objhitInfo->unk58;
    func_8002674C(self);
    objdata->unk24 = (f32) self->objhitInfo->unk52;
    mainCreateTempDLL(DLL_ID_MOVELIB);
}

// offset: 0x190 | func: 1 | export: 1
void CFbarrel_obj_Control(Object* self) {
    CFbarrel_Data* objdata = self->data;
    
    if (objdata->respawnTimer != 0) {
        objdata->respawnTimer -= gUpdateRate;
        // @bug: won't ever be true if the respawn timer is divisible by the update rate
        if (objdata->respawnTimer < 0) {
            objdata->respawnTimer = 0;
            objdata->explodedTime = 0;
            objdata->hits = 0;
            objdata->flags |= CFBARREL_Physics_Enabled;
        }
        return;
    }

    if ((self->parent != NULL) && (self->parent->id == OBJ_DR_PushCart)) {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    } else {
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    }

    if (mapWorldCoordsToBlockIndex(self->globalPosition.x, self->globalPosition.y, self->globalPosition.z) == -1) {
        // not in a loaded block
        return;
    }

    if (objdata->explodedTime != 0) {
        objdata->explodedTime += gUpdateRate;
        objdata->unk24 = (objdata->unk2C * (f32) objdata->explodedTime) + 1.0f;
        func_8002683C(self, (s16) objdata->unk24, (s16) (-objdata->unk24 * 0.5f), (s16) (objdata->unk24 * 0.5f));
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_353, NULL, 0, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_353, NULL, 0, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_353, NULL, 0, -1, NULL);
        if (objdata->explodedTime > 20) {
            objdata->inWindLift = FALSE;
            // Move to "create point" node
            ((DLL_53_movelib*)gTempDLLInsts[1])->vtbl->func7(0x1A, &self->srt);
            STUBBED_PRINTF(" Should has Set position  %i %f %f \n", 0x1A, &self->srt.transl.x, &self->srt.transl.z);
            self->srt.transl.x += (f32) mathRnd(-30, 30) * 0.1f;
            self->srt.transl.z += (f32) mathRnd(-30, 30) * 0.1f;
            bzero(&objdata->targVelocity, sizeof(Vec3f));
            bzero(&self->velocity, sizeof(Vec3f));
            if (objdata->isLevelObj) {
                // prep for respawn
                mapSaveObject(self->setup, self->mapID, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
                objdata->respawnTimer = 600;
                func_80026160(self);
                func_8002683C(self, 8, -2, 0x19);
            } else {
                // no respawn, just disable
                objDisable(self);
                func_800267A4(self);
                self->srt.flags |= OBJSTATE_PRINT_DISABLED;
            }
        }
    } else {
        if (gDLL_54_pickup->vtbl->control(self, &objdata->pickup) == PICKUP_NotHeld) {
            if (objdata->isHeld) {
                objAddObjectType(self, OBJTYPE_WindLiftable);
            }
            objdata->isHeld = FALSE;
            func_8002674C(self);
            if (objdata->flags & CFBARREL_Physics_Enabled) {
                CFbarrel_doPhysics(self);
            }
            CFbarrel_checkHit(self);
        } else {
            objdata->flags |= CFBARREL_Physics_Enabled;
            objdata->isHeld = TRUE;
            objdata->unk3E_1 = TRUE;
        }
        CFbarrel_processMesgLoop(self);
        if (objdata->inWindLift) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
            if (objdata->unk3E_1 && objdata->inWindLift) {
                objdata->targVelocity.x = self->velocity.x;
                objdata->targVelocity.y = self->velocity.y;
                objdata->targVelocity.z = self->velocity.z;
                objdata->targVelocity.y = 0.0f;
                objdata->unk3E_1 = FALSE;
            }
        }
        if (objdata->isLevelObj) {
            mapSaveObject(self->setup, self->mapID, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
        }
    }
}

// offset: 0x6B8 | func: 2 | export: 2
void CFbarrel_obj_Update(Object* self) {
    CFbarrel_Data* objdata = self->data;
    f32 vel[3];
    TrackLineIntersectResult result;

    if (objdata->explodedTime == 0 && objdata->respawnTimer == 0) {
        if (objdata->mobileMap != NULL) {
            trackIntersect_func_8005B5B8(self, objdata->mobileMap, 1);
            objdata->mobileMap = NULL;
        }
        if (objdata->inWindLift) {
            diPrintf(" floating ");
            vel[0] = self->srt.transl.x - self->prevLocalPosition.x;
            vel[1] = self->srt.transl.y - self->prevLocalPosition.y;
            vel[2] = self->srt.transl.z - self->prevLocalPosition.z;
            vel[0] *= 0.99f * (1.0f / gUpdateRateF);
            vel[1] *= 0.99f * (1.0f / gUpdateRateF);
            vel[2] *= 0.99f * (1.0f / gUpdateRateF);
            objdata->targVelocity.x += vel[0];
            objdata->targVelocity.y += vel[1];
            objdata->targVelocity.z += vel[2];
            vel[1] = 0.0f;
            objdata->flags |= CFBARREL_Physics_Enabled;
            objdata->targVelocity.x *= 0.5f;
            objdata->targVelocity.y = 0.0f;
            objdata->targVelocity.z *= 0.5f;
        }
        if (!objdata->isHeld) {
            if (trackGetLineIntersect(&self->prevLocalPosition, &self->srt.transl, 4.0f, 1, &result, self, 8, -1, 0xFF, 0) != 0) {
                STUBBED_PRINTF(" Line IDNO %i \n", result.unk51);
                if (objdata->inWindLift && result.unk51 == 3) {
                    // animator ID 3 hitlines are a failsafe to exit the windlift
                    CFbarrel_setWindLiftState(self, FALSE);
                } else {
                    self->velocity.x *= -1.1f;
                    self->velocity.z *= -1.1f;
                    objdata->targVelocity.x *= -1.1f;
                    objdata->targVelocity.z *= -1.1f;
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
void CFbarrel_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CFbarrel_Data* objdata = self->data;
    if (objdata->explodedTime == 0) {
        if (objdata->isHeld) {
            self->srt.roll = 0;
            self->srt.pitch = 0;
        }
        if (gDLL_54_pickup->vtbl->should_print(self, visibility) != 0) {
            objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0x9DC | func: 4 | export: 4
void CFbarrel_obj_Free(Object* self, s32 onlySelf) {
    CFbarrel_Data* objdata = self->data;
    gDLL_54_pickup->vtbl->free(self);
    objFreeObjectType(self, OBJTYPE_Barrel);
    objFreeObjectType(self, OBJTYPE_WindLiftable);
    if (objdata->explodedTime != 0) {
        gDLL_13_Expgfx->vtbl->func5(self);
    }
    mainRemoveTempDLL(DLL_ID_MOVELIB);
}

// offset: 0xAB0 | func: 5 | export: 5
u32 CFbarrel_obj_GetModelFlags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xAC0 | func: 6 | export: 6
u32 CFbarrel_obj_GetDataSize(Object *self, u32 offsetAddr) {
    return sizeof(CFbarrel_Data);
}

/*0x50*/ static const char str_50[] = " No check ";

// offset: 0xAD4 | func: 7
static void CFbarrel_doPhysics(Object* self) {
    CFbarrel_Data* objdata = self->data;
    f32 height;
    f32 bestHeight;
    s16 angle;
    s32 numResults;
    s32 floors[4];
    s32 bestResultIdx;
    s32 i;
    s32 numFloors;
    s32 resultIdx;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    f32 smallestHeight;
    TrackHeightResult** results;
    s16 sp98[] = {-1, 0, 0, 1, 1, 0, 0, -1};
    s16 sp90[] = {2, 3, 0, 1};
    Object* obj;

    self->srt.yaw = 0;
    if (objdata->targVelocity.y > 0.01f) {
        objdata->targVelocity.y -= 0.1f; // @bug: framerate dependent
        self->velocity.x = objdata->targVelocity.x * gUpdateRateF;
        self->velocity.y = objdata->targVelocity.y * gUpdateRateF;
        self->velocity.z = objdata->targVelocity.z * gUpdateRateF;
        objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);
        self->srt.roll += (objdata->targVelocity.y * 500.0f);
        return;
    }
    smallestHeight = 0.0f;
    angle = 0;
    numFloors = 0;
    for (i = 0; i < 4; i++) {
        dirX = (10.0f * mathSinfInterp(angle)) * mathCosfInterp(self->srt.roll);
        dirZ = (10.0f * mathCosfInterp(angle)) * mathCosfInterp(self->srt.pitch);
        dirY = (10.0f * mathSinfInterp(self->srt.roll) * mathSinfInterp(angle)) 
            + (10.0f * mathSinfInterp(self->srt.pitch) * mathCosfInterp(angle));

        angle += 0x3FD2; // almost 90 degrees

        numResults = trackGetHeight(self, 
                                self->srt.transl.x + dirX, 
                                self->srt.transl.y + dirY,
                                self->srt.transl.z + dirZ, 
                                &results, 0, 1);
        if (numResults != 0) {
            bestHeight = 10000.0f;

            for (resultIdx = 0; resultIdx < numResults; resultIdx++) {
                height = self->srt.transl.y - results[resultIdx]->y;
                if (
                    (height > 0.0f && height < ABS_EXPR(bestHeight)) || 
                    (height <= 0.0f && height > -20.0f)
                ) {
                    bestHeight = height;
                    bestResultIdx = resultIdx;
                }
            }
          
            if (bestHeight <= 0.0f) { // wait until we pass a floor
                if (bestHeight < smallestHeight) {
                    smallestHeight = bestHeight;
                }
                objdata->targVelocity.y = 0.0f;
                if (bestHeight > 0.0f) {
                    floors[numFloors] = i;
                } else {
                    floors[numFloors] = sp90[i];
                }
                numFloors += 1;
            }
        }
    }
    if (numFloors != 0) {
        // zip up to floor
        self->velocity.y = -smallestHeight;
    }
    if (numFloors == 0) {
        objdata->targVelocity.y -= 0.1f;
        self->velocity.y = objdata->targVelocity.y * gUpdateRateF;
        self->velocity.x = objdata->targVelocity.x * gUpdateRateF;
        self->velocity.z = objdata->targVelocity.z * gUpdateRateF;
        if (objdata->targVelocity.y < -0.2f) {
            CFbarrel_moveToAttractor(self, objdata->unk38, objdata->unk3A);
        }
    }
    if (numFloors > 0) {
        STUBBED_PRINTF(" Landed On World Obj ");
        objdata->flags &= ~CFBARREL_Physics_Enabled;
        // @bug: bestResultIdx might not be valid here, the results array could have changed by now
        obj = results[bestResultIdx]->obj;
        if (obj != NULL) {
            if ((obj->def->flags & OBJDEF_IS_MOBILE_MAP) && !(obj->def->flags & OBJDEF_MOBILE_MAP_NEVER_PLAYER_PARENT)) {
                objdata->mobileMap = obj;
                STUBBED_PRINTF(" ob Obj %x  Defno %i \n\n", obj, obj->id);
            }
        }
        objdata->targVelocity.x = 0.0f;
        objdata->targVelocity.y = 0.0f;
        objdata->targVelocity.z = 0.0f;
        self->srt.roll = 0;
        self->srt.pitch = 0;
    }
    if (objdata->inWindLift && numFloors == 4) {
        // all 4 test points reported a floor, so we're not in a windlift anymore even if
        // the windlift hasn't said so yet
        CFbarrel_setWindLiftState(self, FALSE);
    }
    objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);
}

// offset: 0x1214 | func: 8
static void CFbarrel_checkHit(Object* self) {
    CFbarrel_Data* objdata = self->data;
    Object* hitBy;
    DIMExplosion_Setup* explSetup;
    u32 damageType;

    damageType = func_80025F40(self, &hitBy, NULL, NULL);
    if (damageType != 0 && hitBy != self->parent) {
        objdata->hits++;
        if ((damageType == Damage_Type_Explosion) || (damageType == Damage_Type_Projectile)) {
            objdata->hits = 3;
        }
        STUBBED_PRINTF(" Object Hit ");
        if (objdata->hits == 3) {
            // kaboom
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
            objSetupObject(&explSetup->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, self->parent);
            objdata->explodedTime = 1;
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
static void CFbarrel_moveToAttractor(Object* self, s16 arg1, s16 arg2) {
    Object* obj;
    f32 xDist;
    f32 yDist;
    f32 zDist;
    f32 angle;
    f32 var_fv1;
    f32 dist;
    Object* player;

    dist = 300.0f;
    player = objGetPlayer();
    obj = objGetNearestTypeTo(OBJTYPE_32, self, &dist);
    if (obj == NULL) {
        return;
    }
    if (ABS_EXPR(obj->srt.transl.y - player->srt.transl.y) < 30.0f) {
        return;
    }
    xDist = obj->srt.transl.x - self->srt.transl.x;
    yDist = obj->srt.transl.y - self->srt.transl.y;
    if (yDist > 0.0f) {
        // attractor must be below or level with the barrel
        return;
    }
    zDist = obj->srt.transl.z - self->srt.transl.z;
    if (yDist != 0/*.0f*/) {
        // compare how much distance is left to how far we're moving this tick
        var_fv1 = self->velocity.y / yDist;
    } else {
        var_fv1 = 0.0f;
    }
    if (var_fv1 >= 1.0f) {
        STUBBED_PRINTF(" landed ");
        dll_amSfx->Play(self, SOUND_8E2, MAX_VOLUME, NULL, NULL, 0, NULL);
        var_fv1 = 1.0f;
        self->velocity.y = yDist; // snap y
        // move attractor over (so a second barrel would land somewhere else?)
        obj->srt.transl.x += 20.0f;
        obj->velocity.z += 20.0f;
        if (obj->velocity.z > 180.0f) {
            obj->srt.transl.x -= obj->velocity.z;
            obj->velocity.z = 0.0f;
        }
        self->srt.pitch = 0;
        self->srt.roll = 0;
        arg1 = 0;
        arg2 = 0;
    }
    self->velocity.x = xDist * var_fv1;
    self->velocity.z = zDist * var_fv1;
    if (arg1 != 0) {
        if (arg1 == 1) {
            angle = (u16)self->srt.pitch;
            angle = (65536.0f - angle) * var_fv1;
        } else {
            angle = (u16)self->srt.pitch;
            angle *= var_fv1 * arg1;
        }
        self->srt.pitch += angle;
    }
    if (arg2 != 0) {
        if (arg2 == 1) {
            angle = (u16)self->srt.roll;
            angle *= 0.0f;
        } else {
            angle = (u16)self->srt.roll;
            angle *= var_fv1 * arg2;
        }
        self->srt.roll += angle;
    }
}

// offset: 0x17F4 | func: 10 | export: 7
void CFbarrel_Func_17F4(Object* self, Vec3f* arg1) {
    CFbarrel_Data* objdata = self->data;
    if (!objdata->isHeld) {
        STUBBED_PRINTF(" Force Applied x %f y %f z%f \n", &arg1->x, &arg1->y, &arg1->z);
        objdata->targVelocity.y += arg1->y;
        objdata->targVelocity.x += arg1->x;
        objdata->targVelocity.z += arg1->z;
        objdata->flags |= CFBARREL_Physics_Enabled;
    }
}

// offset: 0x1848 | func: 11
static void CFbarrel_processMesgLoop(Object* self) {
    u32 mesgID = 0;
    void* mesgArg = NULL;
    
    while (objRecvMesg(self, &mesgID, NULL, &mesgArg) != 0) {
        switch (mesgID) {
        case 15:
            CFbarrel_setWindLiftState(self, TRUE);
            break;
        case 16:
            CFbarrel_setWindLiftState(self, FALSE);
            break;
        }
    }
}

// offset: 0x1948 | func: 12
static void CFbarrel_setWindLiftState(Object* self, u8 inWindLift) {
    CFbarrel_Data* objdata = self->data;
    ObjectHitInfo* hit = self->objhitInfo;
    
    if (inWindLift) {
        // Let barrel be pushed
        hit->unk5B = 1;
        hit->unk5C = 1;
        STUBBED_PRINTF(" In ELEVATOR");
        self->objhitInfo->unk58 |= 0x400;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        objdata->inWindLift = TRUE;
    } else {
        // Disable push
        hit->unk5B = self->def->unk91;
        hit->unk5C = self->def->unk92;
        objFreeObjectType(self, OBJTYPE_WindLiftable);
        STUBBED_PRINTF("Out of ELEVATOR");
        objdata->inWindLift = FALSE;
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        self->objhitInfo->unk58 &= ~0x400;
        objdata->flags |= CFBARREL_Physics_Enabled;
    }
}
