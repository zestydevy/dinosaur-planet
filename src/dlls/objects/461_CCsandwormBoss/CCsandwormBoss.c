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
    u8 state;                       //State Machine value
    u8 particleTickCount;           //Extra particles should be created for this many ticks after emerging from the sand
    u8 isUnderHome;                 //SandWorm Boss is under the sand, and directly under its home position
    u8 facePlayerDuringSequence;    //Boss rotates to face the player while the animCallback function is active
    Object* player;     // Krystal
    Object* sidekick;   // Kyte
    Object* seqObj;     // CCnewseqobj
    Object* barrel;
    f32 animSpeed;
    f32 timer;
    f32 flashRedTimer;
} CCsandwormBoss_Data;

typedef enum {
    /* PRE-BATTLE SHARPCLAW FIGHT */
    CCsandwormBoss_STATE_0_Leadup_Fighting_One_SharpClaw,
    CCsandwormBoss_STATE_1_Leadup_Fighting_Two_SharpClaw,
    CCsandwormBoss_STATE_2_Leadup_Already_Completed, //Used during setup to skip directly to the battle when revisiting
    CCsandwormBoss_STATE_3_Revisit_Restore_Fire_Crystal,

    /* MAIN BATTLE */
    CCsandwormBoss_STATE_4_Idle, //Emerged from sand, stationary, attacks player when nearby.
    CCsandwormBoss_STATE_5_Idle_Attacking_Krystal,
    CCsandwormBoss_STATE_6_Distracted_Attacking_Krystal,
    CCsandwormBoss_STATE_7_Distracted_Attacking_Kyte,
    CCsandwormBoss_STATE_8_Hurt_Attacking_Krystal,
    CCsandwormBoss_STATE_9_Distracted_by_Kyte,
    CCsandwormBoss_STATE_10_Diving_Under_Sand,
    CCsandwormBoss_STATE_11_Burrowing_Under_Sand,
    CCsandwormBoss_STATE_12_Underground_Attacking_Krystal, //Re-emerging from sand
    CCsandwormBoss_STATE_13_Hurt_by_Barrel, //Vulnerable
    CCsandwormBoss_STATE_14_Dying,
    CCsandwormBoss_STATE_15_Defeated
} CCsandwormBoss_States;

typedef enum {
    CCsandwormBoss_ANIMCMD_1_Burst_Through_Sand = 1,     //NOTE: also invokes ANIMCMD_2
    CCsandwormBoss_ANIMCMD_2_Enable_Extra_Particles = 2,
    CCsandwormBoss_ANIMCMD_3_Turn_Towards_Player = 3
} CCsandwormBoss_AnimCommands;

typedef enum {
    CCnewseqobj_ObjSeq_4_Boss_Intro_Emerge = 4, //Emerging from the ground after defeating 3 SharpClaw
    CCnewseqobj_ObjSeq_5_Eating_Explosive_Barrel = 5, //Popping up, eating barrel, and being hurt by the explosion
    CCnewseqobj_ObjSeq_8_Eating_Player = 8 //Player eaten by boss (Game Over)
} CCsandwormBoss_CCSeqObjIndices;

typedef enum {
    CCsandwormBoss_ObjSeq_0_Dying = 0 //Dying, coughing up Fire Crystal
} CCsandwormBoss_BossSeqIndices;

typedef enum {
    //Bank 0
    CCsandwormBoss_MODANIM_A_0_Clutching_at_Throat = 0,     //Raising claws to neck in pain (intended for after swallowing explosive barrel?)
    CCsandwormBoss_MODANIM_A_1_Fall_Over,
    CCsandwormBoss_MODANIM_A_2_Diving,                      //[BURROWING INTRO]
    CCsandwormBoss_MODANIM_A_3_Snap_Attack_Sweep,           //Clockwise sweep
    CCsandwormBoss_MODANIM_A_4_Snap_Attack_Sweep_and_Dive,  //Clockwise sweep
    CCsandwormBoss_MODANIM_A_5_Idle_LOOP,
    CCsandwormBoss_MODANIM_A_6_Snap_Attack_Forward,
    CCsandwormBoss_MODANIM_A_7_Double_Snap_Attack,
    CCsandwormBoss_MODANIM_A_8_Emerge_and_Snap_Attack,      //[BURROWING EXIT]
    CCsandwormBoss_MODANIM_A_9_Burrowing_Head_Peep_LOOP,    //[BURROWING] Head briefly breaches surface while burrowing in a sinusoidal cycle
    CCsandwormBoss_MODANIM_A_10_Headbutt_Attack,
    CCsandwormBoss_MODANIM_A_11_Emerge,                     //[BURROWING EXIT] 
    CCsandwormBoss_MODANIM_A_12_Burrowing_Peek,             //[BURROWING] Pop up partially, then dive back underground
    CCsandwormBoss_MODANIM_A_13_Dive_and_Peek,              //[BURROWING INTRO] Dive underground, then briefly peek back up
    CCsandwormBoss_MODANIM_A_14_Retreat_Underground,        //[BURROWING INTRO] Goes back to burrowing, without a dive
    CCsandwormBoss_MODANIM_A_15_Idle_Fidget_LOOP,           //Arches back slightly, and raises claws

    //Bank 1
    CCsandwormBoss_MODANIM_B_0_Claw_Swipe_Attack = 0x100,
    CCsandwormBoss_MODANIM_B_1_Dying,
    CCsandwormBoss_MODANIM_B_2_Boss_Intro_Emerging_from_Sand
} CCsandwormBoss_ModAnim;

typedef enum {
    CC_OBJGROUP11_Boss_Beach_Two_SharpClaw = 11,
    CC_OBJGROUP12_Boss_Beach_One_SharpClaw = 12
} CC_ObjGroups;

/*0x0*/ static u32 dAttackModanimIDs[] = {
    CCsandwormBoss_MODANIM_A_3_Snap_Attack_Sweep, 
    CCsandwormBoss_MODANIM_A_6_Snap_Attack_Forward, 
    CCsandwormBoss_MODANIM_A_7_Double_Snap_Attack
};
/*0xC*/ static u32 dAttackSoundIDs[] = {
    SOUND_5A_SandWorm_Roar, 
    SOUND_5B_SandWorm_Roar, 
    SOUND_32B_SandWorm_Snarl, 
    SOUND_335_SandWorm_Snarl
};

static void CCsandwormBoss_tick_sharpclaw_leadup(Object* self, CCsandwormBoss_Data* objData);
static void CCsandwormBoss_init_boss(Object* self, CCsandwormBoss_Data* objData);
static void CCsandwormBoss_tick_battle(Object* self, CCsandwormBoss_Data *objData);
static void CCsandwormBoss_enter_idle_state(Object* self, CCsandwormBoss_Data* objData);
static void CCsandwormBoss_attack(Object* self, Object* obj, CCsandwormBoss_Data* objData, s32 newState);
static void CCsandwormBoss_enter_distracted_state(Object* self, CCsandwormBoss_Data* objData);
static void CCsandwormBoss_turn_towards_object(Object* self, Object* obj);
static void CCsandwormBoss_move_towards_point(Object* self, Vec3f* point, f32 speed);
static void CCsandwormBoss_check_for_projectile_spell(Object* self, CCsandwormBoss_Data* objData);
static void CCsandwormBoss_create_particles(Object* self, CCsandwormBoss_Data* objData);
static int CCsandwormBoss_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);

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
    self->animCallback = CCsandwormBoss_anim_callback;
    obj_add_object_type(self, OBJTYPE_Baddie);
    
    if (main_get_bits(BIT_CC_SandWormBoss_Defeated)) {
        //Preempt the Fire Crystal into position if it hasn't been collected (unfinished)
        if (main_get_bits(BIT_CC_Fire_Crystal_Collected_SandWorm_Boss) == FALSE) {
            objData->state = CCsandwormBoss_STATE_3_Revisit_Restore_Fire_Crystal;
            STUBBED_PRINTF("need to prempt fire crystal into correct position\n");
        } else {
            objData->state = CCsandwormBoss_STATE_15_Defeated;
        }
        return;
    }
    
    //Check whether the pre-boss SharpClaw fight has already been completed
    if (main_get_bits(BIT_CC_SandWormBoss_SharpClaw_Solo_Defeated)) {
        if (main_get_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_1)) {
            if (main_get_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_2)) {
                objData->state = CCsandwormBoss_STATE_2_Leadup_Already_Completed;
            } else {
                main_set_bits(BIT_CC_SandWormBoss_SharpClaw_Solo_Defeated, FALSE);
                main_set_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_2, TRUE);
                objData->state = CCsandwormBoss_STATE_0_Leadup_Fighting_One_SharpClaw;
                gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, CC_OBJGROUP12_Boss_Beach_One_SharpClaw, 1);
            }
        } else if (main_get_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_2)) {
            main_set_bits(BIT_CC_SandWormBoss_SharpClaw_Solo_Defeated, FALSE);
            main_set_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_1, TRUE);
            objData->state = CCsandwormBoss_STATE_0_Leadup_Fighting_One_SharpClaw;
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, CC_OBJGROUP12_Boss_Beach_One_SharpClaw, 1);
        } else {
            main_set_bits(BIT_CC_SandWormBoss_SharpClaw_Solo_Defeated, FALSE);
            main_set_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_2, TRUE);
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
    
    if (objData->state <= CCsandwormBoss_STATE_2_Leadup_Already_Completed) {
        CCsandwormBoss_tick_sharpclaw_leadup(self, objData);
    } else {
        CCsandwormBoss_tick_battle(self, objData);
    }
}

// offset: 0x330 | func: 2
/**
  * State Machine for the boss battle preamble, where you fight a single SharpClaw and then a pair of SharpClaw.
  */
void CCsandwormBoss_tick_sharpclaw_leadup(Object* self, CCsandwormBoss_Data* objData) {
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
    case CCsandwormBoss_STATE_3_Revisit_Restore_Fire_Crystal:
        return;
    case CCsandwormBoss_STATE_1_Leadup_Fighting_Two_SharpClaw:
        //Start the main boss battle when both SharpClaw have been defeated
        if (main_get_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_1) && 
            main_get_bits(BIT_CC_SandWormBoss_SharpClaw_Pair_Defeated_2)
        ) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, CC_OBJGROUP11_Boss_Beach_Two_SharpClaw, 0);
            CCsandwormBoss_init_boss(self, objData);
            gDLL_3_Animation->vtbl->start_obj_sequence(CCnewseqobj_ObjSeq_4_Boss_Intro_Emerge, objData->seqObj, -1);
        }
        break;
    case CCsandwormBoss_STATE_2_Leadup_Already_Completed:
        //Skip to the main boss battle when revisiting (after both SharpClaw were previously defeated)
        CCsandwormBoss_init_boss(self, objData);
        gDLL_3_Animation->vtbl->start_obj_sequence(CCnewseqobj_ObjSeq_4_Boss_Intro_Emerge, objData->seqObj, 2);
        break;
    }
}

// offset: 0x564 | func: 3
void CCsandwormBoss_init_boss(Object* self, CCsandwormBoss_Data* objData) {
    objData->state = CCsandwormBoss_STATE_4_Idle;
    objData->animSpeed = 0.005f;
    self->srt.flags &= ~OBJFLAG_INVISIBLE;
    func_8002674C(self);
    func_80026128(self, Damage_Type_Sword_Staff_Strike1, 1, -1);
}

// offset: 0x5E0 | func: 4
/**
  * State Machine for the main boss battle.
  */
void CCsandwormBoss_tick_battle(Object *self, CCsandwormBoss_Data *objData) {
    ObjSetup* setup;
    f32 dist;
  
    setup = self->setup;
    objData->timer += gUpdateRateF;
    dist = M_INFINITY_F;
    obj_get_nearest_type_to(OBJTYPE_Pickup, self, &dist);

    diPrintf("worm %d, barrel %d\n", (s32) vec3_distance_xz(&self->globalPosition, &objData->player->globalPosition), (s32) dist);
    
    switch (objData->state) {
    case CCsandwormBoss_STATE_4_Idle:
        CCsandwormBoss_turn_towards_object(self, objData->player);

        //Attack the player when nearby
        if (vec3_distance_xz_squared(&self->globalPosition, &objData->player->globalPosition) < SQ(180)) {
            CCsandwormBoss_attack(self, objData->player, objData, CCsandwormBoss_STATE_5_Idle_Attacking_Krystal);
        
        //Become distracted when Kyte uses her Distract Command
        } else {
            if (((DLL_ISidekick*)objData->sidekick->dll)->vtbl->func24(objData->sidekick)) {
                diPrintf("kyte dist %d interest range 50.0F\n", (s32) vec3_distance_xz(&self->globalPosition, &objData->sidekick->globalPosition));
                if (vec3_distance_xz_squared(&self->globalPosition, &objData->sidekick->globalPosition) < SQ(60)) {
                    CCsandwormBoss_enter_distracted_state(self, objData);
                    objData->timer = 0.0f;
                }
            }
        }
        break;
    case CCsandwormBoss_STATE_5_Idle_Attacking_Krystal:
        CCsandwormBoss_turn_towards_object(self, objData->player);
        if (self->animProgress > 0.95f) {
            CCsandwormBoss_enter_idle_state(self, objData);
        }
        break;
    case CCsandwormBoss_STATE_6_Distracted_Attacking_Krystal:
        CCsandwormBoss_turn_towards_object(self, objData->player);
        if (self->animProgress > 0.95f) {
            CCsandwormBoss_enter_distracted_state(self, objData);
        }
        break;
    case CCsandwormBoss_STATE_7_Distracted_Attacking_Kyte:
        CCsandwormBoss_turn_towards_object(self, objData->sidekick);
        if (self->animProgress > 0.95f) {
            CCsandwormBoss_enter_distracted_state(self, objData);
        }
        break;
    case CCsandwormBoss_STATE_8_Hurt_Attacking_Krystal:
        CCsandwormBoss_turn_towards_object(self, objData->player);
        if (self->animProgress > 0.95f) {
            objData->state = CCsandwormBoss_STATE_13_Hurt_by_Barrel;
            objData->animSpeed = 0.005f;
            func_80023D30(self, CCsandwormBoss_MODANIM_A_5_Idle_LOOP, 0, 0);
        }
        CCsandwormBoss_check_for_projectile_spell(self, objData);
        break;
    case CCsandwormBoss_STATE_9_Distracted_by_Kyte:
        CCsandwormBoss_turn_towards_object(self, objData->sidekick);

        if (objData->timer > 300.0f) {
            STUBBED_PRINTF("setting flight group to %d\n", 0x65);
            main_set_bits(BIT_Kyte_Flight_Curve, 0x65);
        } else {
            STUBBED_PRINTF("setting flight group to %d\n", 0xC3);
            main_set_bits(BIT_Kyte_Flight_Curve, 0xC3);
        }

        //Attack the player when they're in range
        if (vec3_distance_xz_squared(&self->globalPosition, &objData->player->globalPosition) < SQ(180)) {
            CCsandwormBoss_attack(self, objData->player, objData, CCsandwormBoss_STATE_6_Distracted_Attacking_Krystal);
        
        //Attack Kyte when she's in range
        } else if (vec3_distance_xz_squared(&self->globalPosition, &objData->sidekick->globalPosition) < SQ(180)) {
            CCsandwormBoss_attack(self, objData->sidekick, objData, CCsandwormBoss_STATE_7_Distracted_Attacking_Kyte);
        
        //Follow Kyte if she's still nearby or using the Distract Command
        } else if (
            (((DLL_ISidekick*)objData->sidekick->dll)->vtbl->func24(objData->sidekick)) || 
            (vec3_distance_xz_squared(&self->globalPosition, &objData->sidekick->globalPosition) < SQ(300))
        ) {
            func_8002493C(self, 1.5f, &objData->animSpeed);
            CCsandwormBoss_move_towards_point(self, &objData->sidekick->srt.transl, 1.5f);
        
        //Otherwise, return to idle state if the worm's base position is nearby
        } else if (vec3_distance_xz_squared(&self->globalPosition, (Vec3f* ) &setup->x) < SQ(100)) {
            CCsandwormBoss_enter_idle_state(self, objData);

        //Otherwise, stop being distracted and dive under the sand
        } else {
            objData->state = CCsandwormBoss_STATE_10_Diving_Under_Sand;
            objData->animSpeed = 0.01f;
            func_80023D30(self, CCsandwormBoss_MODANIM_A_2_Diving, 0, 0);
        }
        break;
    case CCsandwormBoss_STATE_10_Diving_Under_Sand:
        if (self->animProgress > 0.95f) {
            objData->state = CCsandwormBoss_STATE_11_Burrowing_Under_Sand;
        }
        break;
    case CCsandwormBoss_STATE_11_Burrowing_Under_Sand:
        //Check if the worm is exactly under its home position (in X/Z)
        if (((s32) setup->x == (s32) self->srt.transl.f[0]) && ((s32) setup->z == (s32) self->srt.transl.f[2])) {
            
            //Get distance to player
            dist = vec3_distance_xz_squared(&self->globalPosition, &objData->player->globalPosition);

            //Eat the player if they're directly above the home position (Game Over)
            if (dist < SQ(50)) {
                STUBBED_PRINTF("eat player sequence\n");

                objData->facePlayerDuringSequence = FALSE;
                gDLL_3_Animation->vtbl->start_obj_sequence(CCnewseqobj_ObjSeq_8_Eating_Player, objData->seqObj, -1);
            
            //Attack the player if they're close to the home position (but not fatally close)
            } else if (dist < SQ(180)) {
                STUBBED_PRINTF("attack player from under ground\n");

                objData->state = CCsandwormBoss_STATE_12_Underground_Attacking_Krystal;
                objData->animSpeed = 0.005f;
                func_80023D30(self, CCsandwormBoss_MODANIM_A_8_Emerge_and_Snap_Attack, 0, 0);
                gDLL_6_AMSFX->vtbl->play(self, dAttackSoundIDs[rand_next(0, 3)], MAX_VOLUME, NULL, NULL, 0, NULL);
                objData->isUnderHome = FALSE;
                objData->particleTickCount = 3;
            
            } else {
                //Otherwise, check if there's a barrel directly above the worm's home position
                dist = 50.0f;
                objData->barrel = obj_get_nearest_type_to(OBJTYPE_Pickup, self, &dist);
                
                //If the barrel's not being held, eat it and become vulnerable
                if (objData->barrel && (gDLL_54_pickup->vtbl->get_state(objData->barrel->data) == PICKUP_NotHeld)) {
                    STUBBED_PRINTF("eat barrel\n");
                    
                    objData->state = CCsandwormBoss_STATE_13_Hurt_by_Barrel;
                    objData->facePlayerDuringSequence = FALSE;
                    objData->timer = 0.0f;
                    gDLL_3_Animation->vtbl->start_obj_sequence(CCnewseqobj_ObjSeq_5_Eating_Explosive_Barrel, objData->seqObj, -1);
                
                //If there's no edible barrel, just emerge from the sand
                } else {
                    STUBBED_PRINTF("get up without attack\n");

                    objData->state = CCsandwormBoss_STATE_12_Underground_Attacking_Krystal;
                    objData->animSpeed = 0.01f;
                    func_80023D30(self, CCsandwormBoss_MODANIM_A_11_Emerge, 0, 0);
                    objData->isUnderHome = FALSE;
                    objData->particleTickCount = 3;
                }
            }
        } else {
            //Return home
            CCsandwormBoss_move_towards_point(self, (Vec3f* ) &setup->x, 3.0f);
            if (vec3_distance_xz_squared(&self->globalPosition, (Vec3f* ) &setup->x) < SQ(100)) {
                objData->isUnderHome = TRUE;
            }
        }
        break;
    case CCsandwormBoss_STATE_12_Underground_Attacking_Krystal:
        CCsandwormBoss_turn_towards_object(self, objData->player);
        if (self->animProgress > 0.95f) {
            CCsandwormBoss_enter_idle_state(self, objData);
        }
        break;
    case CCsandwormBoss_STATE_13_Hurt_by_Barrel:
        //Move the barrel back to its base position
        if (objData->barrel != NULL) {
            setup = objData->barrel->setup;
            objData->barrel->srt.transl.f[0] = setup->x;
            objData->barrel->srt.transl.f[1] = setup->y;
            objData->barrel->srt.transl.f[2] = setup->z;

            STUBBED_PRINTF("barrel %x put to %f %f %f\n", setup->uID, &setup->x, &setup->y, &setup->z);
            objData->barrel = NULL;
        }

        //Return to idle after 50 seconds
        if (objData->timer > 3000.0f) {
            objData->state = CCsandwormBoss_STATE_4_Idle;
        
        //Attack the player when nearby
        } else if (vec3_distance_xz_squared(&self->globalPosition, &objData->player->globalPosition) < SQ(180)) {
            CCsandwormBoss_attack(self, objData->player, objData, CCsandwormBoss_STATE_8_Hurt_Attacking_Krystal);
        }

        //Check for finishing blow
        CCsandwormBoss_check_for_projectile_spell(self, objData);
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

    CCsandwormBoss_create_particles(self, objData);
}

// offset: 0x1030 | func: 5
void CCsandwormBoss_enter_idle_state(Object* self, CCsandwormBoss_Data* objData) {
    objData->state = CCsandwormBoss_STATE_4_Idle;
    objData->animSpeed = 0.005f;
    func_80023D30(self, CCsandwormBoss_MODANIM_A_5_Idle_LOOP, 0.0f, 0);
}

// offset: 0x1090 | func: 6
void CCsandwormBoss_attack(Object* self, Object* obj, CCsandwormBoss_Data* objData, s32 nextState) {
    if (vec3_distance_xz_squared(&self->globalPosition, &obj->globalPosition) < SQ(90)) {
        //Claw swipe when nearby
        objData->state = nextState;
        objData->animSpeed = 0.02f;
        func_80023D30(self, CCsandwormBoss_MODANIM_B_0_Claw_Swipe_Attack, 0.0f, 0);
        gDLL_6_AMSFX->vtbl->play(self, dAttackSoundIDs[rand_next(0, 3)], MAX_VOLUME, NULL, NULL, 0, NULL);
    } else {
        //Bite attack when further away
        objData->state = nextState;
        objData->animSpeed = 0.009f;
        func_80023D30(self, dAttackModanimIDs[rand_next(0, 2)], 0.0f, 0);
        gDLL_6_AMSFX->vtbl->play(self, dAttackSoundIDs[rand_next(0, 3)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
}

// offset: 0x1250 | func: 7
void CCsandwormBoss_enter_distracted_state(Object* self, CCsandwormBoss_Data* objData) {
    objData->state = CCsandwormBoss_STATE_9_Distracted_by_Kyte;
    objData->animSpeed = 0.005f;
    func_80023D30(self, CCsandwormBoss_MODANIM_A_15_Idle_Fidget_LOOP, 0.0f, 0);
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

    STUBBED_PRINTF("worm at %f %f %f\n", &self->srt.transl.x, &self->srt.transl.y, &self->srt.transl.z);
}

// offset: 0x14B0 | func: 10
void CCsandwormBoss_check_for_projectile_spell(Object* self, CCsandwormBoss_Data* objData) {
    //Die when struck by the Projectile Spell
    if (func_80025F40(self, NULL, NULL, NULL) == Damage_Type_Projectile) {
        STUBBED_PRINTF("boss dead\n");
        objData->facePlayerDuringSequence = FALSE;
        gDLL_3_Animation->vtbl->start_obj_sequence(CCsandwormBoss_ObjSeq_0_Dying, self, -1);
        objData->state = CCsandwormBoss_STATE_14_Dying;
    }
}

// offset: 0x1540 | func: 11
static void CCsandwormBoss_create_particles(Object* self, CCsandwormBoss_Data* objData) {
    SRT fxTransform;

    if (objData->isUnderHome == FALSE) {
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
/*0x1C*/ u8 dFXColour[] = {0xff, 0xc0, 0x96}; //Sandy orange colour
    SRT fxTransform;

    if (!visibility) {
        return;
    }
    
    //Flash red when injured
    if (objData->state == CCsandwormBoss_STATE_13_Hurt_by_Barrel) {
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
    
    //Create sandy particles
    if (objData->isUnderHome == FALSE) {
        if (objData->state == CCsandwormBoss_STATE_11_Burrowing_Under_Sand) {
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
        
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_56, &fxTransform, 0x200001, -1, &dFXColour);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_56, &fxTransform, 0x200001, -1, &dFXColour);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_57, &fxTransform, 0x200001, -1, &dFXColour);
        
        if (objData->particleTickCount) {
            STUBBED_PRINTF("turning on parts\n");

            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_58, &fxTransform, 0x200001, -1, &dFXColour);
            objData->particleTickCount--; //@framerate-dependent

            if (objData->particleTickCount == 0) {
                STUBBED_PRINTF("Parts all over\n");
            }
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
int CCsandwormBoss_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    CCsandwormBoss_Data *objData;
    CCsandwormBoss_Data *objData2;
    s32 i;
    
    objData = self->data;

    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
        case CCsandwormBoss_ANIMCMD_1_Burst_Through_Sand:
            objData->isUnderHome = FALSE;
            /* fallthrough */
        case CCsandwormBoss_ANIMCMD_2_Enable_Extra_Particles:
            objData->particleTickCount = 3;
            break;
        case CCsandwormBoss_ANIMCMD_3_Turn_Towards_Player:
            objData->facePlayerDuringSequence = TRUE;
            break;
        }
    }
    
    CCsandwormBoss_create_particles(self, self->data);
    
    //Rotate towards the player when needed
    objData2 = objData;
    if (objData2->facePlayerDuringSequence) {
        STUBBED_PRINTF("yaw to player\n");
        CCsandwormBoss_turn_towards_object(self, objData2->player);
    }
    
    return 0;
}
