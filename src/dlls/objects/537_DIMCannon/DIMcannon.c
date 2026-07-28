#include "PR/ultratypes.h"
#include "common.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/537_DIMcannon.h"
#include "dlls/objects/541_DIMexplosion.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/math.h"
#include "sys/objanim.h"
#include "sys/objexpr.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "sys/objlib.h"
#include "sys/gfx/modgfx.h"

/*0x0*/ static DLL_IModgfx* dModGfxDLL = NULL;

/* ObjData for DIMCannon */
typedef struct {
    Object* targetObj;
    Vec3f targetCoords;
    f32 targetDistSq;
    Vec3f muzzleCoords;
    s16 fireCooldown;
    s16 enemyAimCooldown;
    u8 state;
    u8 fire;
    u8 distracted;
    s8 interactLockTimer;
} DIMCannon_Data;

typedef enum {
    DIMCannon_STATE_1_Controlled_by_CannonClaw_Aim_Hostile = 1, //CannonClaw aims and fires - sets silo-hiding gamebit when a target approaches
    DIMCannon_STATE_2_Controlled_by_CannonClaw_Idle,            //Unused state! No aiming - just sets silo-hiding gamebit when a target approaches
    DIMCannon_STATE_3_Controlled_by_Player,                     //The player has mounted the cannon turret
    DIMCannon_STATE_4_Controlled_by_CannonClaw_Aim_Nonhostile,  //CannonClaw aims but doesn't fire - DOESN'T retreat into silo when a target approaches
    DIMCannon_STATE_5_Idle,                                     //No CannonClaw, player can interact and mount the cannon turret
    DIMCannon_STATE_6_Retreated_into_Silo                       //Sets a silo-exiting gamebit when the target backs off
} DIMCannon_States;

/* ObjSetup for DIMCannonBall */
typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 unk19;
    s16 velocityY;
    s16 velocityX;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
} DIMCannonBall_Setup;

/* ObjData for DIMCannonBall */
typedef struct {
    u8 createModGfx;
} DIMCannonBall_Data;

static int DIMCannon_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);
static void DIMCannon_aimCannonClaw(Object* self, f32 x, f32 y, f32 z, f32 targetDist);
static void DIMCannon_fireWhenReady(Object* self);
static void DIMCannon_setupCannonBall(Object* self, DIMCannonBall_Setup* objSetup);
static void DIMCannon_tickCannonBall(Object* self);
static void DIMCannon_freeCannonBall(Object* self);
static Object* DIMCannon_createCannonBallExplosion(Object* self);

// offset: 0x0 | ctor
void DIMCannon_ctor(void *dll) { }

// offset: 0xC | dtor
void DIMCannon_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIMCannon_obj_Setup(Object* self, DIMCannon_Setup* objSetup, s32 reset) {
    DIMCannon_Data* objData;

    objInitMesgQueue(self, 4);

    if (self->id == OBJ_DIMCannonBall) {
        //Cannonball
        DIMCannon_setupCannonBall(self, (DIMCannonBall_Setup*)objSetup);
    } else {
        //Cannon

        objSetPriority(self, OBJPRIORITY_MOBILE_MAP);
        objData = self->data;

        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        self->animCallback = DIMCannon_animCallback;
        self->srt.yaw = objSetup->yaw << 8;

        dModGfxDLL = dllLoad(DLL_ID_137, 1);
        
        if (mainGetBits(objSetup->gamebitCannonClawDead)) {
            objData->interactLockTimer = 60;
            objData->state = DIMCannon_STATE_5_Idle;
        } else if (mainGetBits(objSetup->gamebitCannonClawAboard)) {
            objData->state = DIMCannon_STATE_4_Controlled_by_CannonClaw_Aim_Nonhostile;
        }
    }

    objAddObjectType(self, OBJTYPE_Baddie);
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x178 | func: 1 | export: 1
void DIMCannon_obj_Control(Object* self) {
    DIMCannon_Data* objData;
    DIMCannon_Setup* objSetup;
    Object* player;
    Object* sidekick;
    f32 animSpeed;
    u32 distracted;
    Object* cannonPtr;

    objSetup = (DIMCannon_Setup*)self->setup;
    sidekick = objGetSidekick();
    
    if (self->id == OBJ_DIMCannonBall) {
        DIMCannon_tickCannonBall(self);
        return;
    }
    
    if (self->unkAF & ARROW_FLAG_8_No_Targetting) {
        if (mainGetBits(objSetup->gamebitCannonClawDead)) {
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        }
    }
    
    objData = self->data;

    distracted = FALSE;
    if (sidekick != NULL) {
        objData->distracted = ((DLL_ISidekick*)sidekick->dll)->vtbl->func24(sidekick);
        if (objData->state != DIMCannon_STATE_6_Retreated_into_Silo) {
            ((DLL_ISidekick*)sidekick->dll)->vtbl->enable_command(sidekick, Sidekick_Command_INDEX_2_Distract);
        } else {
            distracted = objData->distracted;
            if (distracted) {
                ((DLL_ISidekick*)sidekick->dll)->vtbl->func21(sidekick, 0, NULL);
                distracted = objData->distracted = FALSE;
            }
        }
    } else {
        objData->distracted = distracted;
    }
    
    //Choose the CannonClaw's target (pick the sidekick when distracted, or else the player - if they're not on a vehicle)
    if ((distracted = objData->distracted)) {
        objData->targetObj = sidekick;
    } else {
        player = objGetPlayer();
        if (((DLL_210_Player*)player->dll)->vtbl->get_vehicle(player)) {
            objData->targetObj = NULL;
        } else {
            objData->targetObj = player;
        }
    }
    
    //Return to idle animation when recoil animation ends
    if ((self->curModAnimId == 1) && (self->animProgress >= 1.0f)) {
        objAnimSet(self, 0, 0, 0);
    }
    
    self->srt.flags &= ~OBJFLAG_INVISIBLE;
    
    switch (objData->state) {
    case DIMCannon_STATE_5_Idle:
        //No gunner, player can mount cannon

        if (objData->interactLockTimer > 0) {
            objData->interactLockTimer -= gUpdateRate;
        } else if (self->unkAF & ARROW_FLAG_1_Interacted) {
            cannonPtr = self;
            gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMCANNON, TRUE, 0, sizeof(&cannonPtr), &cannonPtr, 50, Cam_Ease_All);
            objData->state = DIMCannon_STATE_3_Controlled_by_Player;
            gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
            objData->interactLockTimer = 60;
            self->unkAF |= ARROW_FLAG_8_No_Targetting;

            //@bug: doesn't block A_BUTTON, so the player attacks while mounting the cannon
        }

        objData->fire = FALSE;
        objData->fireCooldown = 0;
        objData->enemyAimCooldown = 0;
        break;
    case DIMCannon_STATE_4_Controlled_by_CannonClaw_Aim_Nonhostile:
        //CannonClaw aims, but doesn't fire

        DIMCannon_aimCannonClaw(self, objData->targetCoords.x, objData->targetCoords.y, objData->targetCoords.z, objData->targetDistSq);
        
        if (mainGetBits(objSetup->gamebitCannonClawDead)) {
            //Check if the CannonClaw died
            objData->state = DIMCannon_STATE_5_Idle;
        } else if (objData->targetObj && 
            (mainGetBits(objSetup->gamebitCannonClawTruce) == FALSE) && 
            (vec3DistanceXZSquared(&self->globalPosition, &objData->targetObj->globalPosition) < (objSetup->hostileRange * SQ(500.0f) / SQ(10)))
        ) {
            //Become hostile if the target comes into range and the truce gamebit isn't set
            objData->state = DIMCannon_STATE_1_Controlled_by_CannonClaw_Aim_Hostile;
        }

        objData->fire = FALSE;
        objData->fireCooldown = 0;
        objData->enemyAimCooldown = 0;
        break;
    case DIMCannon_STATE_1_Controlled_by_CannonClaw_Aim_Hostile:
        if (mainGetBits(objSetup->gamebitCannonClawDead)) {
            objData->state = DIMCannon_STATE_5_Idle;
        } else if (mainGetBits(objSetup->gamebitCannonClawTruce)) {
            objData->state = DIMCannon_STATE_4_Controlled_by_CannonClaw_Aim_Nonhostile;
        } else if (objData->targetObj != NULL) {
            objData->targetCoords.x = objData->targetObj->srt.transl.x;
            objData->targetCoords.y = objData->targetObj->srt.transl.y;
            objData->targetCoords.z = objData->targetObj->srt.transl.z;
            
            if (objData->fireCooldown > 0) {
                objData->fireCooldown -= gUpdateRate;
            }
            
            if (objData->enemyAimCooldown > 0) {
                objData->enemyAimCooldown -= gUpdateRate;
            }
            
            objData->targetDistSq = vec3DistanceXZSquared(&self->globalPosition, &objData->targetObj->globalPosition);
            
            if ((objData->targetDistSq < SQ(objSetup->rangeSiloRetreat)) && (objData->distracted == FALSE)) {
                //If the target comes close while the CannonClaw isn't distracted, set a gamebit so the cannon can retreat into its silo
                sidekick = objGetSidekick();
                if (sidekick != NULL) {
                    ((DLL_ISidekick*)sidekick->dll)->vtbl->func21(sidekick, 0, 0);
                }

                mainSetBits(objSetup->gamebitSiloEnter, TRUE);
                objData->state = DIMCannon_STATE_6_Retreated_into_Silo;
            } else {
                DIMCannon_aimCannonClaw(self, objData->targetCoords.x, objData->targetCoords.y, objData->targetCoords.z, objData->targetDistSq);
                DIMCannon_fireWhenReady(self);

                //Become nonhostile when the target's out of firing range
                if (objData->targetDistSq > ((objSetup->hostileRange * SQ(510.0f)) / SQ(10))) {
                    objData->state = DIMCannon_STATE_4_Controlled_by_CannonClaw_Aim_Nonhostile;
                }
            }
        } else {
            objData->state = DIMCannon_STATE_4_Controlled_by_CannonClaw_Aim_Nonhostile;
        }

        break;
    case DIMCannon_STATE_6_Retreated_into_Silo:
        if (objData->targetObj) {
            objData->targetCoords.x = objData->targetObj->srt.transl.x;
            objData->targetCoords.y = objData->targetObj->srt.transl.y;
            objData->targetCoords.z = objData->targetObj->srt.transl.z;
            objData->targetDistSq = vec3DistanceXZSquared(&self->globalPosition, &objData->targetObj->globalPosition);
            
            if (objData->targetDistSq > ((objSetup->hostileRange * SQ(300.0f)) / SQ(10))) {
                mainSetBits(objSetup->gamebitSiloExit, TRUE);
                objData->state = DIMCannon_STATE_1_Controlled_by_CannonClaw_Aim_Hostile;
            }
        }
        break;
    case DIMCannon_STATE_2_Controlled_by_CannonClaw_Idle:
        if ((objData->targetDistSq < SQ(objSetup->rangeSiloRetreat)) && (objData->distracted == FALSE)) {
            sidekick = objGetSidekick();
            if (sidekick != NULL) {
                ((DLL_ISidekick*)sidekick->dll)->vtbl->func21(sidekick, 0, 0);
            }

            mainSetBits(objSetup->gamebitSiloEnter, TRUE);
            objData->state = DIMCannon_STATE_6_Retreated_into_Silo;
        }
        break;
    }

    //Handle animation
    {
        //Use a faster animSpeed during the recoil animation
        if ((self->curModAnimId == 0) || (self->curModAnimId != 1)) {
            animSpeed = 0.01f;
        } else {
            animSpeed = 0.025f;
        }
        objAnimAdvance(self, animSpeed, gUpdateRateF, 0);
    }
}

// offset: 0x8F0 | func: 2 | export: 2
void DIMCannon_obj_Update(Object *self) { }

// offset: 0x8FC | func: 3 | export: 3
void DIMCannon_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DIMCannon_Data* objData;

    if (self->id != OBJ_DIMCannonBall) {
        //Cannon
        objData = self->data;
        if (visibility) {
            objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);

            //Store muzzle coords (as a reference point for firing)
            objGetAttachPointWorldSpace(self, 1, &objData->muzzleCoords.x, &objData->muzzleCoords.y, &objData->muzzleCoords.z, 0);
        }
    } else {
        //Cannonball
        if (visibility) {
            objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0x9BC | func: 4 | export: 4
void DIMCannon_obj_Free(Object* self, s32 onlySelf) {
    if (self->id == OBJ_DIMCannonBall) {
        //Cannonball
        DIMCannon_freeCannonBall(self);
    } else {
        //Cannon
        dllFree(dModGfxDLL);
    }
    
    objFreeObjectType(self, OBJTYPE_Baddie);
}

// offset: 0xA48 | func: 5 | export: 5
u32 DIMCannon_obj_GetModelFlags(Object* self) {
    if (self->id == OBJ_DIMCannonBall) {
        return MODFLAGS_NONE;
    } else {
        return MODFLAGS_NONE;
    }
}

// offset: 0xA6C | func: 6 | export: 6
s32 DIMCannon_obj_GetDataSize(Object* self, s32 offsetAddr) {
    if (self->id == OBJ_DIMCannonBall) {
        return sizeof(DIMCannonBall_Data);
    } else {
        return sizeof(DIMCannon_Data);
    }
}

// offset: 0xA94 | func: 7
int DIMCannon_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    DIMCannon_Data* objData;
    DIMCannon_Setup* objSetup;
    s32 pad;

    animData->unk62 = 0;
    animData->unk7A &= ~(0x400 | 0x200 | 8);
    
    objData = self->data;

    switch (objData->state) {
    case DIMCannon_STATE_3_Controlled_by_Player:
        if (objData->interactLockTimer > 0) {
            objData->interactLockTimer -= gUpdateRate;
        } else {      
            SeqJoint* barrelJoint;
            s32 angle;
            
            barrelJoint = objExpr_func_80034804(self, 0);
            angle = -barrelJoint->pitch;
            self->srt.yaw -= joyGetStickX(0) * 4; //@framerate-dependent

            //Decrement the fire cooldown timer
            if (objData->fireCooldown > 0) {
                objData->fireCooldown -= gUpdateRate;
            }
    
            //Decrement the enemy's aim cooldown timer (doesn't affect player aiming)
            if (objData->enemyAimCooldown > 0) {
                objData->enemyAimCooldown -= gUpdateRate;
            }
            
            //Raise the cannon barrel while holding A
            if ((joyGetButtons(0) & A_BUTTON) && (objData->fireCooldown <= 0)) {
                angle += 800; //@framerate-dependent
            } else {
                angle -= 1200; //@framerate-dependent
            }
            
            if (angle > M_45_DEGREES) {
                angle = M_45_DEGREES;
            }
            if (angle < 0) {
                angle = 0;
            }
            
            //Fire when letting go of A
            if ((joyGetReleased(0) & A_BUTTON) && (objData->fireCooldown <= 0)) {
                objData->fire = TRUE;
            }
            DIMCannon_fireWhenReady(self);
            
            //Exit the cannon when pressing Z
            if (joyGetPressed(0) & Z_TRIG) {
                gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, FALSE, 1, 0, NULL, 0, Cam_Ease_All);
                objData->state = DIMCannon_STATE_5_Idle;
                objData->interactLockTimer = 60;
                animData->unk9D |= 4;
                self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
            }
            
            //Ease the barrel towards its goal pitch
            angle = -angle;
            angle -= (u16)(barrelJoint->pitch);
            CIRCLE_WRAP(angle);            
            barrelJoint->pitch += angle >> 2;
        }
        break;
    default:
        self->srt.flags &= ~OBJFLAG_INVISIBLE;
        if (animData->lastMessage == 1) {
            objSetup = (DIMCannon_Setup*)self->setup;
            mainSetBits(objSetup->gamebitSiloCoverOpen, TRUE);
        }
        
        animData->lastMessage = 0;
        if (objData->state != DIMCannon_STATE_6_Retreated_into_Silo) {
            objData->state = DIMCannon_STATE_4_Controlled_by_CannonClaw_Aim_Nonhostile;
        }
        break;
    } 

    return 0;  
}

// offset: 0xDAC | func: 8
void DIMCannon_aimCannonClaw(Object* self, f32 x, f32 y, f32 z, f32 targetDist) {
    s32 pad[5];
    s32 dPitchAngle;
    s32 aimFinished;
    SeqJoint* barrelJoint;
    s32 dYaw;
    f32 aimVectorY;
    f32 targetDiffY;
    s32 aimShift;
    f32 aimVectorX;
    f32 sqAimVectorY;
    f32 distance;
    DIMCannon_Data* objData;
    
    objData = self->data;

    //Don't adjust aim if the CannonClaw fired recently
    if (objData->enemyAimCooldown > 0) {
        return;
    }

    //Calculate the barrel pitch angle needed for the cannon to hit its target, and animate towards it 
    {
        barrelJoint = objExpr_func_80034804(self, 0);
        
        sqAimVectorY = 2500.0f;

        //Get actual target distance (distance is passed as square)
        targetDist = sqrtf(targetDist);

        aimVectorX = targetDist * 2.2f;
        sqAimVectorY = SQ(sqAimVectorY) - SQ(aimVectorX);
        
        aimFinished = FALSE;
        if (sqAimVectorY >= 0) {
            dPitchAngle = mathAtan2f(aimVectorX, sqrtf(sqAimVectorY)) >> 1;
        } else {
            dPitchAngle = M_45_DEGREES;
            aimFinished = TRUE;
        }
        
        targetDiffY = (objData->muzzleCoords.y - y) - 10.0f;
        if (targetDiffY > 0.0f) {
            aimShift = -M_1_DEGREE;
        } else {
            aimShift = M_1_DEGREE;
        }
        
        while (aimFinished == FALSE) {
            f32 aimCoefficient = 4.0f;

            aimVectorY = mathSinfInterp(dPitchAngle) * 50.0f;
            aimVectorX = SQ(aimVectorY) - ((aimCoefficient * -1.1f) * targetDiffY);

            if (SQ(aimVectorY) >= aimCoefficient * -1.1f * targetDiffY) {
                f32 temp1 = -1.1f;
                f32 temp2 = -1.1f;

                aimVectorX = sqrtf(aimVectorX);
                if (aimVectorY) {}
                if ((temp1 + temp2) != 0.0f) { 
                }
            }

            distance = mathCosfInterp(dPitchAngle) * 50.0f * aimVectorX;
            
            dPitchAngle += aimShift;
            
            if ((distance > targetDist) && (aimShift > 0)) {
                aimFinished = TRUE;
            }
            
            if ((distance < targetDist) && (aimShift < 0)) {
                aimFinished = TRUE;
            }
            
            if (dPitchAngle > M_45_DEGREES) {
                dPitchAngle = M_45_DEGREES;
                aimFinished = TRUE;
            } else if (dPitchAngle < 0) {
                dPitchAngle = 0;
                aimFinished = TRUE;
            }
        }

        dPitchAngle = -dPitchAngle;
        dPitchAngle -= (barrelJoint->pitch & 0xFFFF);
        CIRCLE_WRAP(dPitchAngle);
        
        //Ease barrel joint towards goal angle
        barrelJoint->pitch += dPitchAngle >> 2; //@framerate-dependent
    }

    //Calculate the yaw needed for the cannon to hit its target, animate towards it, and decide whether to fire
    {
        //Get the angle between the cannon and the target
        x -= self->srt.transl.x;
        z -= self->srt.transl.z;
        dYaw = ((s16)mathAtan2f(x, z)) - (self->srt.yaw & 0xFFFF);
        CIRCLE_WRAP(dYaw);

        if (dYaw > M_45_DEGREES/2) {
            dYaw = M_45_DEGREES/2;
        }
        if (dYaw < -M_45_DEGREES/2) {
            dYaw = -M_45_DEGREES/2;
        }

        if (aimVectorX) {}
        
        //Fire if the cannon's yaw is within ~11 degrees of aiming directly at the target
        if ((M_45_DEGREES/4 > dYaw) && (dYaw > -M_45_DEGREES/4)) {
            objData->fire = TRUE;
        }
        
        //Don't fire if the target is too close
        if (objData->targetDistSq < SQ(100)) {
            objData->fire = FALSE;
        }
        
        self->srt.yaw += dYaw >> 2; //@framerate-dependent
    }
}

// offset: 0x1150 | func: 9
void DIMCannon_fireWhenReady(Object* self) {
    DIMCannonBall_Setup* shotSetup;
    Object* shot;
    SeqJoint* barrelJoint;
    DIMCannon_Data* objData;
    DIMCannon_Setup* objSetup;

    objData = self->data;
    objSetup = (DIMCannon_Setup*)self->setup;
    
    //Return early if the cannon doesn't need to fire
    if ((objData->fire == FALSE) || (objData->fireCooldown > 0)) {
        return;
    }

    //Get the cannon's barrel seqJoint
    barrelJoint = objExpr_func_80034804(self, 0);
    
    //Create a cannonball
    {
        shotSetup = (DIMCannonBall_Setup*)objAllocSetup(sizeof(DIMCannonBall_Setup), OBJ_DIMCannonBall);
        shotSetup->base.loadFlags    = objSetup->base.loadFlags;
        shotSetup->base.loadDistance = objSetup->base.byte6;
        shotSetup->base.fadeFlags    = objSetup->base.byte5;
        shotSetup->base.fadeDistance = objSetup->base.fadeDistance;
        shotSetup->base.x = objData->muzzleCoords.x;
        shotSetup->base.y = objData->muzzleCoords.y;
        shotSetup->base.z = objData->muzzleCoords.z;
        shotSetup->yaw = self->srt.yaw >> 8;
        shotSetup->velocityY = mathSinfInterp(barrelJoint->pitch) * 50.0f;
        shotSetup->velocityX = mathCosfInterp(barrelJoint->pitch) * 50.0f;
        
        shot = objSetupObject(&shotSetup->base, (OBJINIT_STANDALONE | OBJINIT_FLAG4), self->mapID, -1, NULL);
        shot->unkC4 = self;
    }
    
    objData->fire = FALSE;
    objData->enemyAimCooldown = 50;
    
    if (objData->state == DIMCannon_STATE_3_Controlled_by_Player) {
        objData->fireCooldown = 100;
    } else {
        objData->fireCooldown = mathRnd(objSetup->cooldownMin, objSetup->cooldownMax);
    }
    
    //Play recoil animation
    objAnimSet(self, 1, 0, 0);
}

// offset: 0x1314 | func: 10
void DIMCannon_setupCannonBall(Object* self, DIMCannonBall_Setup* objSetup) {
    f32 verticalSpeed;
    f32 lateralSpeed;
    DIMCannonBall_Data* objData;
    ObjectHitInfo* objHits;

    self->srt.yaw = objSetup->yaw << 8;
    verticalSpeed = objSetup->velocityY * 0.1f;
    lateralSpeed = objSetup->velocityX * 0.1f;

    self->velocity.x = mathSinfInterp(self->srt.yaw) * lateralSpeed;
    self->velocity.y = -verticalSpeed;
    self->velocity.z = mathCosfInterp(self->srt.yaw) * lateralSpeed;
    
    self->unkDC = 0;

    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_OPACITY | OBJ_SHADOW_FLAG_CUSTOM_DIR;
        self->shadow->flags |= OBJ_SHADOW_FLAG_8000;
        self->shadow->maxDistScale = 2.0f * self->shadow->scale;
    }

    objData = self->data;
    objData->createModGfx = TRUE;

    objHits = self->objhitInfo;
    if (objHits != NULL) {
        objHits->unkA1 = 1;
    }
    
    self->stateFlags |= OBJSTATE_PRINT_DISABLED;
}

// offset: 0x1430 | func: 11
void DIMCannon_tickCannonBall(Object* self) {
    ObjectHitInfo* objHits;
    s32 pad;
    DIMCannonBall_Data* objData;
    
    //Apply gravity and move
    self->velocity.y += -0.022f * gUpdateRateF;
    objMove(self, self->velocity.x * gUpdateRateF, self->velocity.y * gUpdateRateF, self->velocity.z * gUpdateRateF);

    //Handle colliding with objects (ignoring the parent cannon object)
    objHits = self->objhitInfo;
    if (objHits != NULL) {
        func_80026128(self, 5, 1, 0);
        if ((objHits->unk48 != NULL) && (objHits->unk48 != self->unkC4)) {
            DIMCannon_createCannonBallExplosion(self);
            objFreeObject(self);
        }
    }
    
    //Handle colliding with terrain
    if (self->objhitInfo->unk9D != 0) {
        DIMCannon_createCannonBallExplosion(self);
        objFreeObject(self);
    }
    
    //Unload after 20 seconds
    self->unkDC += gUpdateRate;
    if (self->unkDC > 20 * 60) {
        objFreeObject(self);
    }
    
    objData = self->data;
    
    //Align model with velocity vector
    self->srt.pitch = mathAtan2f(self->velocity.y, sqrtf(SQ(self->velocity.x) + SQ(self->velocity.z)));
    
    //Create modGfx
    if (objData->createModGfx) {
        dModGfxDLL->vtbl->func0(self, 2, 0, 0x10002, -1, 0);
        objData->createModGfx = FALSE;
    }
}

// offset: 0x1640 | func: 12
void DIMCannon_freeCannonBall(Object* self) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x16AC | func: 13
Object* DIMCannon_createCannonBallExplosion(Object* self) {
    DIMCannonBall_Setup* objSetup; 
    DIMExplosion_Setup* boomSetup;

    //@bug: accidentally reading from `self->data` instead of `self->setup` (causes explosions to become invisible)!
    // We know `DIMCannonBall_Data` is only 1 byte long, and the parent->child
    // inheritance pattern below suggests Rare expected to read from an ObjSetup:
    objSetup = (DIMCannonBall_Setup*)self->data; 
    
    boomSetup = (DIMExplosion_Setup*)objAllocSetup(sizeof(DIMExplosion_Setup), OBJ_DIMExplosion);
    boomSetup->base.loadFlags = objSetup->base.loadFlags;
    boomSetup->base.loadDistance = objSetup->base.loadDistance;
    boomSetup->base.fadeFlags = objSetup->base.fadeFlags;
    boomSetup->base.fadeDistance = objSetup->base.fadeDistance;
    boomSetup->base.x = self->srt.transl.x;
    boomSetup->base.y = self->srt.transl.y;
    boomSetup->base.z = self->srt.transl.z;
    return objSetupObject(&boomSetup->base, (OBJINIT_STANDALONE | OBJINIT_FLAG4), self->mapID, -1, self->parent);
}
