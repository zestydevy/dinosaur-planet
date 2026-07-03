#include "common.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/objhits.h"
#include "sys/objlib.h"
#include "sys/objtype.h"
#include "sys/objanim.h"
#include "dlls/objects/common/sidekick.h"

typedef struct {
    ObjSetup base;
} CCsandwormBoss_Setup;

typedef struct {
    u8 state;
    u8 unk1;
    u8 unk2;
    u8 facePlayerDuringSequence;
    Object* player;     // Krystal
    Object* sidekick;   // Kyte
    Object* seqObj;     // CCnewseqobj
    Object* unk10;
    f32 animSpeed;
    f32 unk18;
    f32 flashRedTimer;
} CCsandwormBoss_Data;

typedef enum {
    CCsandwormBoss_STATE_0_Leadup_Fighting_One_SharpClaw,
    CCsandwormBoss_STATE_1_Leadup_Fighting_Two_SharpClaw,
    CCsandwormBoss_STATE_2,
    CCsandwormBoss_STATE_3,
    CCsandwormBoss_STATE_4_Emerged, //Emerged from sand, stationary?
    CCsandwormBoss_STATE_5_Attacking, //Attacking (from state 4?)
    CCsandwormBoss_STATE_6,
    CCsandwormBoss_STATE_7,
    CCsandwormBoss_STATE_8,
    CCsandwormBoss_STATE_9,
    CCsandwormBoss_STATE_10,
    CCsandwormBoss_STATE_11,
    CCsandwormBoss_STATE_12,
    CCsandwormBoss_STATE_13,
    CCsandwormBoss_STATE_14_Dying,
    CCsandwormBoss_STATE_15_Defeated
} CCsandwormBoss_States;

typedef enum {
    CCnewseqobj_ObjSeq_4_Boss_Intro_Emerge = 4, //Emerging from the ground (after defeating 3 SharpClaw)
    CCnewseqobj_ObjSeq_5 = 5, //Popping up, eating barrel?
    CCnewseqobj_ObjSeq_8_Eating_Player = 8 //Player eaten by boss (Game Over)
} CCsandwormBoss_CCSeqObjIndices;

typedef enum {
    CCsandwormBoss_ObjSeq_0_Dying = 0 //Dying, coughing up Fire Crystal
} CCsandwormBoss_BossSeqIndices;

typedef enum {
    CC_OBJGROUP11_Boss_Beach_Two_SharpClaw = 11,
    CC_OBJGROUP12_Boss_Beach_One_SharpClaw = 12
} CC_ObjGroups;

/*0x0*/ static u32 dAttackModanimIDs[] = {
    0x00000003, 0x00000006, 0x00000007
};
/*0xC*/ static u32 dAttackSoundIDs[] = {
    0x0000005a, 0x0000005b, 0x0000032b, 0x00000335
};

static void CCsandwormBoss_func_330(Object* self, CCsandwormBoss_Data* objData);
static void CCsandwormBoss_func_564(Object* self, CCsandwormBoss_Data* objData);
static void CCsandwormBoss_func_5E0(Object* self, CCsandwormBoss_Data *objData);
static void CCsandwormBoss_func_1030(Object* self, CCsandwormBoss_Data* objData);
static void CCsandwormBoss_attack(Object* self, Object* obj, CCsandwormBoss_Data* objData, s32 newState);
static void CCsandwormBoss_func_1250(Object* self, CCsandwormBoss_Data* objData);
static void CCsandwormBoss_turn_towards_object(Object* self, Object* obj);
static void CCsandwormBoss_move_towards_point(Object* self, Vec3f* point, f32 speed);
static void CCsandwormBoss_func_14B0(Object* aself0, CCsandwormBoss_Data* objData);
static void CCsandwormBoss_func_1540(Object* self, CCsandwormBoss_Data* objData);
static int CCsandwormBoss_func_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void CCsandwormBoss_ctor(void *dll) { }

// offset: 0xC | dtor
void CCsandwormBoss_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCsandwormBoss_setup(Object* self, CCsandwormBoss_Setup* objSetup, s32 reset) {
    CCsandwormBoss_Data* objData;

    objData = self->data;
    
    self->srt.flags |= OBJFLAG_INVISIBLE;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
    
    func_800267A4(self);
    self->animCallback = CCsandwormBoss_func_anim_callback;
    obj_add_object_type(self, OBJTYPE_Baddie);
    
    if (main_get_bits(BIT_CC_SandWormBoss_Defeated)) {
        if (main_get_bits(BIT_3D4) == FALSE) {
            objData->state = CCsandwormBoss_STATE_3;
        } else {
            objData->state = CCsandwormBoss_STATE_15_Defeated;
        }
        return;
    }
    
    if (main_get_bits(BIT_CC_SandWormBoss_SharpClaw_Solo_Defeated)) {
        
        if (main_get_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_1)) {
            if (main_get_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_2)) {
                objData->state = CCsandwormBoss_STATE_2;
            } else {
                main_set_bits(BIT_CC_SandWormBoss_SharpClaw_Solo_Defeated, 0);
                main_set_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_2, 1);
                objData->state = CCsandwormBoss_STATE_0_Leadup_Fighting_One_SharpClaw;
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, CC_OBJGROUP12_Boss_Beach_One_SharpClaw, 1);
            }
        } else if (main_get_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_2)) {
            main_set_bits(BIT_CC_SandWormBoss_SharpClaw_Solo_Defeated, 0);
            main_set_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_1, 1);
            objData->state = CCsandwormBoss_STATE_0_Leadup_Fighting_One_SharpClaw;
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, CC_OBJGROUP12_Boss_Beach_One_SharpClaw, 1);
        } else {
            main_set_bits(BIT_CC_SandWormBoss_SharpClaw_Solo_Defeated, 0);
            main_set_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_2, 1);
            objData->state = CCsandwormBoss_STATE_0_Leadup_Fighting_One_SharpClaw;
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, CC_OBJGROUP12_Boss_Beach_One_SharpClaw, 1);
        }

        return;
    }
    
    objData->state = CCsandwormBoss_STATE_0_Leadup_Fighting_One_SharpClaw;
    gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, CC_OBJGROUP12_Boss_Beach_One_SharpClaw, 1);
}

// offset: 0x2C8 | func: 1 | export: 1
void CCsandwormBoss_control(Object* self) {
    CCsandwormBoss_Data* objData = self->data;
    
    if (objData->state < CCsandwormBoss_STATE_3) {
        CCsandwormBoss_func_330(self, objData);
    } else {
        CCsandwormBoss_func_5E0(self, objData);
    }
}

// offset: 0x330 | func: 2
void CCsandwormBoss_func_330(Object* self, CCsandwormBoss_Data* objData) {
    //Find Krystal
    if (objData->player == NULL) {
        objData->player = get_player();
    }
    
    //Find Kyte
    if (objData->sidekick == NULL) {
        objData->sidekick = get_sidekick();
    }
    
    //Find the nearby `CCnewseqobj` 
    if (objData->seqObj == NULL) {
        objData->seqObj = obj_get_nearest_type_to(OBJTYPE_UseObj, self, NULL);
    }

    switch (objData->state) {
    case CCsandwormBoss_STATE_0_Leadup_Fighting_One_SharpClaw:
        if (main_get_bits(BIT_CC_SandWormBoss_SharpClaw_Solo_Defeated)) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, CC_OBJGROUP11_Boss_Beach_Two_SharpClaw, 1);
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, CC_OBJGROUP12_Boss_Beach_One_SharpClaw, 0);
            objData->state = CCsandwormBoss_STATE_1_Leadup_Fighting_Two_SharpClaw;
            return;
        }
    case CCsandwormBoss_STATE_3:
        return;
    case CCsandwormBoss_STATE_1_Leadup_Fighting_Two_SharpClaw:
        //Check if both SharpClaw have been defeated
        if (main_get_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_1) && 
            main_get_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_2)
        ) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, CC_OBJGROUP11_Boss_Beach_Two_SharpClaw, 0);
            CCsandwormBoss_func_564(self, objData);
            gDLL_3_Animation->vtbl->start_obj_sequence(CCnewseqobj_ObjSeq_4_Boss_Intro_Emerge, objData->seqObj, -1);
        }
        break;
    case CCsandwormBoss_STATE_2:
        CCsandwormBoss_func_564(self, objData);
        gDLL_3_Animation->vtbl->start_obj_sequence(CCnewseqobj_ObjSeq_4_Boss_Intro_Emerge, objData->seqObj, 2);
        break;
    }
}

// offset: 0x564 | func: 3
void CCsandwormBoss_func_564(Object* self, CCsandwormBoss_Data* objData) {
    objData->state = CCsandwormBoss_STATE_4_Emerged;
    objData->animSpeed = 0.005f;
    self->srt.flags &= ~OBJFLAG_INVISIBLE;
    func_8002674C(self);
    func_80026128(self, Damage_Type_Sword_Staff_Strike1, 1, -1);
}

/*0x0*/ static const char str_0[] = "need to prempt fire crystal into correct position\n";

// offset: 0x5E0 | func: 4
void CCsandwormBoss_func_5E0(Object *self, CCsandwormBoss_Data *objData) {
    ObjSetup* setup;
    f32 dist;
  
    setup = self->setup;
    objData->unk18 += gUpdateRateF;
    dist = M_INFINITY_F;
    obj_get_nearest_type_to(OBJTYPE_Pickup, self, &dist);

    diPrintf("worm %d, barrel %d\n", (s32) vec3_distance_xz(&self->globalPosition, &objData->player->globalPosition), (s32) dist);
    
    switch (objData->state) {
    case CCsandwormBoss_STATE_4_Emerged:
        CCsandwormBoss_turn_towards_object(self, objData->player);
        if (vec3_distance_xz_squared(&self->globalPosition, &objData->player->globalPosition) < 32400.0f) {
            CCsandwormBoss_attack(self, objData->player, objData, CCsandwormBoss_STATE_5_Attacking);
        } else {
            if (((DLL_ISidekick*)objData->sidekick->dll)->vtbl->func24(objData->sidekick) != 0) {
                diPrintf("kyte dist %d interest range 50.0F\n", (s32) vec3_distance_xz(&self->globalPosition, &objData->sidekick->globalPosition));
                if (vec3_distance_xz_squared(&self->globalPosition, &objData->sidekick->globalPosition) < 3600.0f) {
                    CCsandwormBoss_func_1250(self, objData);
                    objData->unk18 = 0.0f;
                }
            }
        }
        break;
    case CCsandwormBoss_STATE_5_Attacking:
        CCsandwormBoss_turn_towards_object(self, objData->player);
        if (self->animProgress > 0.95f) {
            CCsandwormBoss_func_1030(self, objData);
        }
        break;
    case CCsandwormBoss_STATE_6:
        CCsandwormBoss_turn_towards_object(self, objData->player);
        if (self->animProgress > 0.95f) {
            CCsandwormBoss_func_1250(self, objData);
        }
        break;
    case CCsandwormBoss_STATE_7:
        CCsandwormBoss_turn_towards_object(self, objData->sidekick);
        if (self->animProgress > 0.95f) {
            CCsandwormBoss_func_1250(self, objData);
        }
        break;
    case CCsandwormBoss_STATE_8:
        CCsandwormBoss_turn_towards_object(self, objData->player);
        if (self->animProgress > 0.95f) {
            objData->state = CCsandwormBoss_STATE_13;
            objData->animSpeed = 0.005f;
            func_80023D30(self, 5, 0, 0);
        }
        CCsandwormBoss_func_14B0(self, objData);
        break;
    case CCsandwormBoss_STATE_9:
        CCsandwormBoss_turn_towards_object(self, objData->sidekick);

        if (objData->unk18 > 300.0f) {
            STUBBED_PRINTF("setting flight group to %d\n", 0x65);
            main_set_bits(BIT_Kyte_Flight_Curve, 0x65);
        } else {
            STUBBED_PRINTF("setting flight group to %d\n", 0xC3);
            main_set_bits(BIT_Kyte_Flight_Curve, 0xC3);
        }

        if (vec3_distance_xz_squared(&self->globalPosition, &objData->player->globalPosition) < 32400.0f) {
            CCsandwormBoss_attack(self, objData->player, objData, CCsandwormBoss_STATE_6);
        } else if (vec3_distance_xz_squared(&self->globalPosition, &objData->sidekick->globalPosition) < 32400.0f) {
            CCsandwormBoss_attack(self, objData->sidekick, objData, CCsandwormBoss_STATE_7);
        } else {
            if ((((DLL_ISidekick*)objData->sidekick->dll)->vtbl->func24(objData->sidekick) != 0) || (vec3_distance_xz_squared(&self->globalPosition, &objData->sidekick->globalPosition) < 90000.0f)) {
                func_8002493C(self, 1.5f, &objData->animSpeed);
                CCsandwormBoss_move_towards_point(self, &objData->sidekick->srt.transl, 1.5f);
            } else if (vec3_distance_xz_squared(&self->globalPosition, (Vec3f* ) &setup->x) < 10000.0f) {
                CCsandwormBoss_func_1030(self, objData);
            } else {
                objData->state = CCsandwormBoss_STATE_10;
                objData->animSpeed = 0.01f;
                func_80023D30(self, 2, 0, 0);
            }
        }
        break;
    case CCsandwormBoss_STATE_10:
        if (self->animProgress > 0.95f) {
            objData->state = CCsandwormBoss_STATE_11;
        }
        break;
    case CCsandwormBoss_STATE_11:
        if (((s32) setup->x == (s32) self->srt.transl.f[0]) && ((s32) setup->z == (s32) self->srt.transl.f[2])) {
            dist = vec3_distance_xz_squared(&self->globalPosition, &objData->player->globalPosition);
            if (dist < 2500.0f) {
                objData->facePlayerDuringSequence = FALSE;
                
                STUBBED_PRINTF("eat player sequence\n");
                gDLL_3_Animation->vtbl->start_obj_sequence(CCnewseqobj_ObjSeq_8_Eating_Player, objData->seqObj, -1);
            } else if (dist < 32400.0f) {
                objData->state = CCsandwormBoss_STATE_12;
                objData->animSpeed = 0.005f;
                func_80023D30(self, 8, 0, 0);
                gDLL_6_AMSFX->vtbl->play(self, dAttackSoundIDs[rand_next(0, 3)], MAX_VOLUME, NULL, NULL, 0, NULL);
                objData->unk2 = FALSE;
                objData->unk1 = 3;
            } else {
                dist = 50.0f;
                objData->unk10 = obj_get_nearest_type_to(OBJTYPE_Pickup, self, &dist);
                if ((objData->unk10 != NULL) && (gDLL_54_pickup->vtbl->get_state(objData->unk10->data) == PICKUP_NotHeld)) {
                    objData->state = CCsandwormBoss_STATE_13;
                    objData->facePlayerDuringSequence = FALSE;
                    objData->unk18 = 0.0f;
                    gDLL_3_Animation->vtbl->start_obj_sequence(CCnewseqobj_ObjSeq_5, objData->seqObj, -1);
                } else {
                    objData->state = CCsandwormBoss_STATE_12;
                    objData->animSpeed = 0.01f;
                    func_80023D30(self, 0xB, 0, 0);
                    objData->unk2 = FALSE;
                    objData->unk1 = 3;
                }
            }
        } else {
            //Return home
            CCsandwormBoss_move_towards_point(self, (Vec3f* ) &setup->x, 3.0f);
            if (vec3_distance_xz_squared(&self->globalPosition, (Vec3f* ) &setup->x) < SQ(100)) {
                objData->unk2 = TRUE;
            }
        }
        break;
    case CCsandwormBoss_STATE_12:
        CCsandwormBoss_turn_towards_object(self, objData->player);
        if (self->animProgress > 0.95f) {
            CCsandwormBoss_func_1030(self, objData);
        }
        break;
    case CCsandwormBoss_STATE_13:
        if (objData->unk10 != NULL) {
            setup = objData->unk10->setup;
            objData->unk10->srt.transl.f[0] = setup->x;
            objData->unk10->srt.transl.f[1] = setup->y;
            objData->unk10->srt.transl.f[2] = setup->z;
            objData->unk10 = NULL;
        }

        if (objData->unk18 > 3000.0f) {
            objData->state = CCsandwormBoss_STATE_4_Emerged;
        } else if (vec3_distance_xz_squared(&self->globalPosition, &objData->player->globalPosition) < 32400.0f) {
            CCsandwormBoss_attack(self, objData->player, objData, CCsandwormBoss_STATE_8);
        }

        CCsandwormBoss_func_14B0(self, objData);
        break;
    case CCsandwormBoss_STATE_14_Dying:
        objData->state = CCsandwormBoss_STATE_15_Defeated;
        self->srt.flags |= OBJFLAG_INVISIBLE;
        func_800267A4(self);
        func_80026160(self);
        main_set_bits(BIT_CC_SandWormBoss_Defeated, TRUE);
        break;
    case CCsandwormBoss_STATE_15_Defeated:
        return;
    }

    func_80024108(self, objData->animSpeed, gUpdateRateF, 0);
    CCsandwormBoss_func_1540(self, objData);
}

// static const char str_1[] = "setting flight group to %d\n";
// static const char str_2[] = "setting flight group to %d\n";
// static const char str_3[] = "eat player sequence\n";
static const char str_4[] = "attack player from under ground\n";
static const char str_5[] = "eat barrel\n";
static const char str_5b[] = "get up without attack\n";
static const char str_6[] = "barrel %x put to %f %f %f\n";
static const char str_7[] = "worm at %f %f %f\n";
static const char str_8[] = "boss dead\n";
static const char str_9[] = "turning on parts\n";
static const char str_10[] = "Parts all over\n";
static const char str_11[] = "yaw to player\n";

// offset: 0x1030 | func: 5
void CCsandwormBoss_func_1030(Object* self, CCsandwormBoss_Data* objData) {
    objData->state = CCsandwormBoss_STATE_4_Emerged;
    objData->animSpeed = 0.005f;
    func_80023D30(self, 5, 0.0f, 0);
}

// offset: 0x1090 | func: 6
void CCsandwormBoss_attack(Object* self, Object* obj, CCsandwormBoss_Data* objData, s32 newState) {
    if (vec3_distance_xz_squared(&self->globalPosition, &obj->globalPosition) < SQ(90)) {
        objData->state = newState;
        objData->animSpeed = 0.02f;
        func_80023D30(self, 0x100, 0.0f, 0);
        gDLL_6_AMSFX->vtbl->play(self, dAttackSoundIDs[rand_next(0, 3)], MAX_VOLUME, NULL, NULL, 0, NULL);
    } else {
        objData->state = newState;
        objData->animSpeed = 0.009f;
        func_80023D30(self, dAttackModanimIDs[rand_next(0, 2)], 0.0f, 0);
        gDLL_6_AMSFX->vtbl->play(self, dAttackSoundIDs[rand_next(0, 3)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
}

// offset: 0x1250 | func: 7
void CCsandwormBoss_func_1250(Object* self, CCsandwormBoss_Data* objData) {
    objData->state = CCsandwormBoss_STATE_9;
    objData->animSpeed = 0.005f;
    func_80023D30(self, 0xF, 0.0f, 0);
}

// offset: 0x12B0 | func: 8
static void CCsandwormBoss_turn_towards_object(Object* self, Object* obj) {
    s16 angle;
    s32 yawDiff;
    s16 yaw;

    angle = (u16)arctan2_f(self->srt.transl.x - obj->srt.transl.x, self->srt.transl.z - obj->srt.transl.z);
    yaw = self->srt.yaw;
    yawDiff = yaw - ((u16)angle);
    CIRCLE_WRAP(yawDiff);
    
    if ((yawDiff >> 8) > 0x10) {
        self->srt.yaw = yaw - 0x1000; //@framerate-dependent
    } else if ((yawDiff >> 8) < -0x10) {
        self->srt.yaw = yaw + 0x1000; //@framerate-dependent
    } else {
        self->srt.yaw = angle;
    }
}

// offset: 0x1384 | func: 9
void CCsandwormBoss_move_towards_point(Object* self, Vec3f* point, f32 speed) {
    f32 d[2];
    f32 magnitude;
    f32 initialDistance;
    s32 pad;

    initialDistance = vec3_distance_xz_squared(&self->srt.transl, point);
 
    //Get 2D unit vector towards the point 
    d[0] = point->f[0] - self->srt.transl.x;
    d[1] = point->f[2] - self->srt.transl.z;
    magnitude = sqrtf(SQ(d[0]) + SQ(d[1]));
    
    //Move along the unit vector
    self->srt.transl.f[0] += (d[0] / magnitude) * speed * gUpdateRateF;
    self->srt.transl.f[2] += (d[1] / magnitude) * speed * gUpdateRateF;
    
    //If the worm was closer to the destination beforehand, restore that position
    if (initialDistance < vec3_distance_xz_squared(&self->srt.transl, point)) {
        self->srt.transl.x = point->f[0];
        self->srt.transl.z = point->f[2];
    }
}

// offset: 0x14B0 | func: 10
void CCsandwormBoss_func_14B0(Object* self, CCsandwormBoss_Data* objData) {
    if (func_80025F40(self, NULL, NULL, NULL) == Damage_Type_Projectile) {
        objData->facePlayerDuringSequence = FALSE;
        gDLL_3_Animation->vtbl->start_obj_sequence(CCsandwormBoss_ObjSeq_0_Dying, self, -1);
        objData->state = CCsandwormBoss_STATE_14_Dying;
    }
}

// offset: 0x1540 | func: 11
static void CCsandwormBoss_func_1540(Object* self, CCsandwormBoss_Data* objData) {
    SRT fxTransform;

    if (objData->unk2 == FALSE) {
        fxTransform.transl.y = 15.0f;
        fxTransform.transl.x = rand_next(-40, 40);
        fxTransform.transl.z = rand_next(-40, 40);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3DE, &fxTransform, 0, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3DE, &fxTransform, 0, -1, NULL);
    }
}

// offset: 0x1634 | func: 12 | export: 2
void CCsandwormBoss_update(Object *self) { }

// offset: 0x1640 | func: 13 | export: 3
void CCsandwormBoss_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CCsandwormBoss_Data* objData = self->data;
    u8 alpha;
/*0x1C*/ u8 _data_1C[] = {0xff, 0xc0, 0x96};
    SRT fxTransform;

    if (!visibility) {
        return;
    }
    
    //Flash red when injured
    if (objData->state == CCsandwormBoss_STATE_13) {
        objData->flashRedTimer += gUpdateRateF * 5.0f;

        if (objData->flashRedTimer > 382.0f) {
            objData->flashRedTimer -= 382.0f;
        }
        
        if (objData->flashRedTimer >= 191.5f) {
            alpha = 382 - objData->flashRedTimer;
        } else {
            alpha = (s32) objData->flashRedTimer;
        }
        func_80036FBC(0xC8, 0, 0, alpha);
    }
    
    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    
    if (objData->unk2 == FALSE) {
        if (objData->state == CCsandwormBoss_STATE_11) {
            func_80031F6C(self, 2, &fxTransform.transl.x, &fxTransform.transl.y, &fxTransform.transl.z, 0);
        } else {
            func_80031F6C(self, 0, &fxTransform.transl.x, &fxTransform.transl.y, &fxTransform.transl.z, 0);
        }
        
        fxTransform.yaw = 0;
        fxTransform.pitch = 0;
        fxTransform.roll = 0;
        fxTransform.flags = 0;
        fxTransform.transl.y = self->srt.transl.y + 15.0f;
        fxTransform.transl.x -= fsin16_precise(self->srt.yaw) * 10.0f;
        fxTransform.transl.z -= fcos16_precise(self->srt.yaw) * 10.0f;
        fxTransform.scale = 1.7f;
        
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_56, &fxTransform, 0x200001, -1, &_data_1C);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_56, &fxTransform, 0x200001, -1, &_data_1C);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_57, &fxTransform, 0x200001, -1, &_data_1C);
        
        if (objData->unk1) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_58, &fxTransform, 0x200001, -1, &_data_1C);
            objData->unk1--;
        }
    }
}

// offset: 0x1A20 | func: 14 | export: 4
void CCsandwormBoss_free(Object* self, s32 onlySelf) {
    gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, CC_OBJGROUP11_Boss_Beach_Two_SharpClaw, 0); //pair of SharpClaw
    gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, CC_OBJGROUP12_Boss_Beach_One_SharpClaw, 0); //single SharpClaw
    obj_free_object_type(self, OBJTYPE_Baddie);
}

// offset: 0x1AC0 | func: 15 | export: 5
u32 CCsandwormBoss_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1AD0 | func: 16 | export: 6
u32 CCsandwormBoss_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(CCsandwormBoss_Data);
}

// offset: 0x1AE4 | func: 17
int CCsandwormBoss_func_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    CCsandwormBoss_Data *objData;
    CCsandwormBoss_Data *objData2;
    s32 i;
    
    objData = self->data;

    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
        case 1:
            objData->unk2 = FALSE;
            /* fallthrough */
        case 2:
            objData->unk1 = 3;
            break;
        case 3:
            objData->facePlayerDuringSequence = TRUE;
            break;
        }
    }
    
    CCsandwormBoss_func_1540(self, self->data);
    
    objData2 = objData;
    if (objData2->facePlayerDuringSequence) {
        CCsandwormBoss_turn_towards_object(self, objData2->player);
    }
    
    return 0;
}
