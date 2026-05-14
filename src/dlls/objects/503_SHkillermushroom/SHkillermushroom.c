#include "common.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "sys/objlib.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/210_player.h"

#include "dlls/objects/503_SHkillermushroom.h"

/*0x0*/ static s16 dStateModAnimIDs[] = {
    SHkillermushroom_MODANIM_0_Idle_LOOP,     //SHkillermushroom_STATE_0_Idle
    SHkillermushroom_MODANIM_0_Idle_LOOP,     //SHkillermushroom_STATE_1_Regrow
    SHkillermushroom_MODANIM_4_Topple_End,    //SHkillermushroom_STATE_2_Dying_Outro
    SHkillermushroom_MODANIM_1_Spores_Intro,  //SHkillermushroom_STATE_3_Spore_Attack_Intro
    SHkillermushroom_MODANIM_2_Spores_LOOP,   //SHkillermushroom_STATE_4_Spore_Attack
    SHkillermushroom_MODANIM_3_Spores_Outro,  //SHkillermushroom_STATE_5_Spore_Attack_Outro
    SHkillermushroom_MODANIM_5_Topple_Start,  //SHkillermushroom_STATE_6_Dying_Intro
    SHkillermushroom_MODANIM_6_Angry,         //SHkillermushroom_STATE_7_Angry_Fast
    SHkillermushroom_MODANIM_6_Angry,         //SHkillermushroom_STATE_8_Angry_Slow
    SHkillermushroom_MODANIM_9_Stunned_LOOP,  //SHkillermushroom_STATE_9_Stunned
    SHkillermushroom_MODANIM_0_Idle_LOOP     //SHkillermushroom_STATE_10_Hidden
};

/*0x18*/ static f32 dStateAnimSpeeds[] = {
    0.0f,       //SHkillermushroom_STATE_0_Idle
    0.0f,       //SHkillermushroom_STATE_1_Regrow
    0.008f,     //SHkillermushroom_STATE_2_Dying_Outro
    0.025f,     //SHkillermushroom_STATE_3_Spore_Attack_Intro
    0.018f,     //SHkillermushroom_STATE_4_Spore_Attack
    0.015f,     //SHkillermushroom_STATE_5_Spore_Attack_Outro
    0.006f,     //SHkillermushroom_STATE_6_Dying_Intro
    0.008f,     //SHkillermushroom_STATE_7_Angry_Fast
    0.005f,     //SHkillermushroom_STATE_8_Angry_Slow
    0.012f,     //SHkillermushroom_STATE_9_Stunned
    0.005f     //SHkillermushroom_STATE_10_Hidden
};

static void SHkillermushroom_reset(Object* self, SHkillermushroom_Data* objData, int startAtZeroScale);

// offset: 0x0 | ctor
void SHkillermushroom_ctor(void *dll) { }

// offset: 0xC | dtor
void SHkillermushroom_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHkillermushroom_setup(Object* self, SHkillermushroom_Setup* objSetup, s32 arg2) {
    SHkillermushroom_Data* objData;
    ObjectShadow* shadow;

    objData = self->data;

    objData->timer = 0;
    objData->sporeInhaleRange = 0;
    objData->baseScale = self->srt.scale;

    //Set minimum regrowth wait period
    objData->regrowWaitDuration = objSetup->regrowWaitDuration;
    if (objData->regrowWaitDuration < 1800) {
        objData->regrowWaitDuration = 1800;
    }

    self->srt.transl.y = objSetup->base.y - 2.0f;

    //Set up object shadow
    shadow = self->shadow;
    if (shadow != NULL) {
        shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR;
    }

    if (arg2 == 0) {
        SHkillermushroom_reset(self, objData, FALSE);
    }
}

// offset: 0xC4 | func: 1 | export: 1
void SHkillermushroom_control(Object* self) {
    SHkillermushroom_Data* objData;
    Object* player;
    Object* hitBy;
    s32 hitSphereID;
    s32 hitDamage;
    f32 dz;
    f32 dx;
    f32 dy;
    s32 opacity;
    s32 pad;
    u16 playerDistance;
    u8 i;
    SHkillermushroom_Setup* objSetup;
    SRT fxTransform;

    objData = self->data;
    player = get_player();
    objSetup = (SHkillermushroom_Setup*)self->setup;

    func_80026160(self);
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    objData->flags |= SHkillermushroom_FLAG_Vulnerable;

    switch (objData->state) {
    case SHkillermushroom_STATE_6_Dying_Intro:
        //Inaccessible state: spraying spores, then toppling over and fading out!

        //Start spore sound loop
        if (objData->soundHandle == 0) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_53B_Spore_Spray_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
        }

        //Become invulnerable
        objData->flags &= ~SHkillermushroom_FLAG_Vulnerable;

        //Increase spore damage radius
        objData->sporeInhaleRange += INHALE_RADIUS_SPEED * gUpdateRateF;
        if (objData->sporeInhaleRange > INHALE_RADIUS_MAX) {
            objData->sporeInhaleRange = INHALE_RADIUS_MAX;
        }

        //Harm the player if they're in range
        if (!(objData->flags & SHkillermushroom_FLAG_Disable_Spore_Damage) &&
            (vec3_distance(&self->globalPosition, &player->globalPosition) <= objData->sporeInhaleRange) &&
            (((DLL_210_Player*)player->dll)->vtbl->func42(player) == 0) &&
            (((DLL_210_Player*)player->dll)->vtbl->func43(player) == 0)
        ) {
            func_8002635C(player, self, Damage_Type_Toxic, 1, 0);
            objData->flags |= SHkillermushroom_FLAG_Disable_Spore_Damage;
        }

        //Go to topple outro when animation finished
        if (objData->flags & SHkillermushroom_FLAG_Animation_Finished) {
            objData->state = SHkillermushroom_STATE_2_Dying_Outro;
            objData->timer = 0.0f;

            //Stop sound loop
            if (objData->soundHandle != 0) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
                objData->soundHandle = 0;
            }
        }

        //Create spore particles
        fxTransform.transl.x = objData->attachX;
        fxTransform.transl.y = objData->attachY;
        fxTransform.transl.z = objData->attachZ;
        for (i = 5; i != 0; i--) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3EB, &fxTransform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
        }
        break;
    case SHkillermushroom_STATE_2_Dying_Outro:
        //Stay invulnerable
        objData->flags &= ~SHkillermushroom_FLAG_Vulnerable;

        //Fade out when animation finished
        if (objData->flags & SHkillermushroom_FLAG_Animation_Finished) {
            opacity = self->opacity - (gUpdateRate * 4);
            if (opacity < 0) {
                opacity = 0;
            }
            self->opacity = opacity;

            //Regrow
            objData->timer += gUpdateRateF;
            if (objData->timer > objData->regrowWaitDuration) {
                SHkillermushroom_reset(self, objData, TRUE);
                objData->state = SHkillermushroom_STATE_1_Regrow;
            }
        }
        break;

    case SHkillermushroom_STATE_3_Spore_Attack_Intro:
        //Wait for attack intro animation to finish
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        if (objData->flags & SHkillermushroom_FLAG_Animation_Finished) {
            objData->state = SHkillermushroom_STATE_4_Spore_Attack;
        }
        break;
    case SHkillermushroom_STATE_4_Spore_Attack:
        //Start spore sound loop
        if (objData->soundHandle == 0) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_53B_Spore_Spray_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
        }

        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;

        //Increase spore damage radius
        objData->sporeInhaleRange += INHALE_RADIUS_SPEED * gUpdateRateF;

        //Harm the player if they're in range
        if (!(objData->flags & SHkillermushroom_FLAG_Disable_Spore_Damage) &&
            (vec3_distance(&self->globalPosition, &player->globalPosition) <= objData->sporeInhaleRange) &&
            (((DLL_210_Player*)player->dll)->vtbl->func42(player) == 0) &&
            (((DLL_210_Player*)player->dll)->vtbl->func43(player) == 0)
        ) {
            func_8002635C(player, self, Damage_Type_Toxic, 1, 0);
            objData->flags |= SHkillermushroom_FLAG_Disable_Spore_Damage;
        }

        //Limit spore damage radius
        if (objData->sporeInhaleRange > INHALE_RADIUS_MAX) {
            objData->sporeInhaleRange = INHALE_RADIUS_MAX;
        }

        //End attack after two seconds
        objData->timer += gUpdateRateF;
        if (objData->timer > SPORE_ATTACK_DURATION) {
            objData->state = SHkillermushroom_STATE_5_Spore_Attack_Outro;
            objData->timer = 0.0f;

            //Stop sound loop
            if (objData->soundHandle != 0) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
                objData->soundHandle = 0;
            }
        }

        //Create spore particles
        fxTransform.transl.x = objData->attachX;
        fxTransform.transl.y = objData->attachY;
        fxTransform.transl.z = objData->attachZ;
        for (i = 5; i != 0; i--) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3EB, &fxTransform, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
        }
        break;
    case SHkillermushroom_STATE_5_Spore_Attack_Outro:
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;

        //Return to idle after a waiting period (and after animation's finished)
        objData->timer += gUpdateRateF;
        if ((objData->timer > objSetup->attackCooldown) &&
            (objData->flags & SHkillermushroom_FLAG_Animation_Finished)
        ) {
            objData->state = SHkillermushroom_STATE_0_Idle;
            objData->flags &= ~SHkillermushroom_FLAG_Disable_Spore_Damage;
            objData->sporeInhaleRange = 0.0f;
        }
        break;

    case SHkillermushroom_STATE_1_Regrow:
        //Regrow after being collected/killed

        //Stay invulnerable
        objData->flags &= ~SHkillermushroom_FLAG_Vulnerable;

        //Limit scale speed
        if (self->srt.scale > objData->scaleMax) {
            objData->scaleSpeed /= 1.1f;
        }
        if (objData->scaleSpeed < 0.00001f) {
            objData->scaleSpeed = 0.0f;
        }

        //Increase timer and scale
        objData->timer += gUpdateRateF;
        self->srt.scale += objData->scaleSpeed * gUpdateRateF;

        //Return to idle once grown
        if (objData->timer > objData->growDuration) {
            objData->state = SHkillermushroom_STATE_0_Idle;
        }
        break;

    case SHkillermushroom_STATE_9_Stunned:
        //Configure LockIcon, start stunned sound loop, and pick stun duration
        if (objData->timer <= 0.0f) {
            func_80023BF8(self, 0x19, 0, 0, 0, 6);
            gDLL_6_AMSFX->vtbl->play(self, SOUND_745_Mushroom_Stunned_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
            objData->timer = rand_next(240, 300);
        }

        //Run down stun timer
        objData->timer -= gUpdateRateF;

        //Return to idle once stun wears off
        if (objData->timer <= 0.0f) {
            gDLL_13_Expgfx->vtbl->func4(self);
            objData->state = SHkillermushroom_STATE_0_Idle;
            func_80023C6C(self);

            //Stop sound loop
            if (objData->soundHandle != 0) {
                gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
                objData->soundHandle = 0;
            }

        //If still stunned, emit particles periodically
        } else {
            objData->stunFxTimer -= gUpdateRateF;
            if (objData->stunFxTimer <= 0.0f) {
                fxTransform.transl.x = 14.0f;
                fxTransform.transl.y = 25.0f;
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_51D, &fxTransform, PARTFXFLAG_2, -1, NULL);
                objData->stunFxTimer = 20.0f;
            }

            //Handle player collecting mushroom (@incomplete: doesn't add any inventory items)
            if (self->unkAF & ARROW_FLAG_1_Interacted) {
                //Send message to player, displaying Red Mushroom's tutorial box
                //@bug: should use unique Red Mushroom tutorial gamebit, not the Blue Mushroom gamebit?
                obj_send_mesg(player,
                    0x7000A,
                    self,
                    (void*)BIT_Tutorial_Collected_Blue_Mushroom
                );

                objData->state = SHkillermushroom_STATE_10_Hidden;

                //Stop sound loop
                if (objData->soundHandle != 0) {
                    gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
                    objData->soundHandle = 0;
                }
            }

            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        }
        break;

    case SHkillermushroom_STATE_10_Hidden:
        //Wait to regrow after being collected
        func_800267A4(self);

        objData->timer += gUpdateRateF;
        if (objData->timer > objData->regrowWaitDuration) {
            SHkillermushroom_reset(self, objData, TRUE);
            objData->state = SHkillermushroom_STATE_1_Regrow;
            func_80023C6C(self);
        }
        break;

    case SHkillermushroom_STATE_0_Idle:
    default:
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        dx = player->srt.transl.x - self->srt.transl.x;
        dy = player->srt.transl.y - self->srt.transl.y;
        dz = player->srt.transl.z - self->srt.transl.z;

        playerDistance = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));

        //Attack the player when they're nearby
        if (playerDistance < objSetup->aggroRadius &&
            (((DLL_210_Player*)player->dll)->vtbl->func56(player) >= 0.54f) //Player can avoid detection by sneaking
        ) {
            objData->flags &= ~SHkillermushroom_FLAG_Disable_Spore_Damage;
            objData->state = SHkillermushroom_STATE_3_Spore_Attack_Intro;
            objData->timer = 0.0f;
            gDLL_6_AMSFX->vtbl->play(self, SOUND_53A_Spore_Spray_Intro, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        break;
    }

    //React to attacks
    if (func_80025F40(self, &hitBy, &hitSphereID, &hitDamage) && (objData->flags & SHkillermushroom_FLAG_Vulnerable)) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_744_Mushroom_Hit, MAX_VOLUME, NULL, NULL, 0, NULL);
        objData->flags &= ~SHkillermushroom_FLAG_Disable_Spore_Damage;

        //Optionally set a gamebit when the mushroom is attacked
        if (objSetup->gamebitAttacked != NO_GAMEBIT) {
            main_set_bits(objSetup->gamebitAttacked, TRUE);
        }

        objData->state = SHkillermushroom_STATE_9_Stunned;
        objData->timer = 0.0f;

        //Stop active sound loop
        if (objData->soundHandle != 0) {
            gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
            objData->soundHandle = 0;
        }
    }

    //Change animation if needed
    if (self->curModAnimId != dStateModAnimIDs[objData->state]) {
        func_80023D30(self, dStateModAnimIDs[objData->state], 0.0f, 0);
    }

    //Advance animation
    if (func_80024108(self, dStateAnimSpeeds[objData->state], gUpdateRateF, NULL)) {
        objData->flags |= SHkillermushroom_FLAG_Animation_Finished;
    } else {
        objData->flags &= ~SHkillermushroom_FLAG_Animation_Finished;
    }
}

// offset: 0xCDC | func: 2 | export: 2
void SHkillermushroom_update(Object *self) { }

// offset: 0xCE8 | func: 3 | export: 3
void SHkillermushroom_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    SHkillermushroom_Data* objData = self->data;

    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);

        //Store attach point coords (for particle effects)
        func_80031F6C(self, 0,
            &objData->attachX,
            &objData->attachY,
            &objData->attachZ,
            0
        );
    }
}

// offset: 0xD70 | func: 4 | export: 4
void SHkillermushroom_free(Object* self, s32 a1) {
    gDLL_13_Expgfx->vtbl->func4(self);
}

// offset: 0xDB8 | func: 5 | export: 5
u32 SHkillermushroom_get_model_flags(Object* self) {
    SHkillermushroom_Setup* setup = (SHkillermushroom_Setup*)self->setup;
    return MODFLAGS_MODEL_INDEX(setup->modelIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0xDD0 | func: 6 | export: 6
u32 SHkillermushroom_get_data_size(Object *self, u32 a1) {
    return sizeof(SHkillermushroom_Data);
}

// offset: 0xDE4 | func: 7
static void SHkillermushroom_reset(Object* self, SHkillermushroom_Data* objData, int startAtZeroScale) {
    SHkillermushroom_Setup* objSetup = (SHkillermushroom_Setup*)self->setup;

    self->srt.roll = rand_next(-1500, 1500);
    self->srt.pitch = rand_next(-1500, 1500);
    self->srt.yaw = rand_next(-1500, 1500);
    self->opacity = OBJECT_OPACITY_MAX;
    self->srt.flags &= ~OBJFLAG_INVISIBLE;
    self->srt.transl.x = objSetup->base.x;
    self->srt.transl.y = objSetup->base.y;
    self->srt.transl.z = objSetup->base.z;

    if (startAtZeroScale) {
        self->srt.scale = 0.00001f;
        objData->timer = 0.0f;
        objData->growDuration = rand_next(0, 100) + 200.0f;
        objData->scaleMax = (rand_next(-100, 100) * 0.001f) + objData->baseScale;
        objData->scaleSpeed = objData->scaleMax / objData->growDuration;
    }

    func_8002674C(self);
    func_800264D0(self);
}
