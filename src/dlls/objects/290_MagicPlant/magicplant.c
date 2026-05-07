#include "common.h"
#include "macros.h"
#include "game/objects/interaction_arrow.h"
#include "sys/objtype.h"
#include "sys/objanim.h"
#include "sys/objprint.h"
#include "sys/objlib.h"
#include "sys/gfx/modgfx.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/290_magicplant.h"
#include "dlls/objects/291_magicdust.h"

#define TWINKLE_START_DISTANCE 50.0f
#define TWINKLE_STOP_DISTANCE 70.0f
#define ATTACH_JOINT_ID 5

/*0x0*/ static s16 dMagicDustObjIDs[] = {
    OBJ_MagicDustSmall, OBJ_MagicDustMid, OBJ_MagicDustLarge, OBJ_MagicDustHuge
};
/*0x8*/ static f32 dMagicDustY[] = {
    -40, -35, -30, -25
};

static void MagicPlant_handle_state_growing(Object* self, MagicPlant_Setup* objSetup, MagicPlant_Data* objData);
static void MagicPlant_handle_state_idle(Object* self, MagicPlant_Setup* objSetup, MagicPlant_Data* objData);
static void MagicPlant_handle_state_damaged(Object* self, MagicPlant_Setup* objSetup, MagicPlant_Data* objData);
static void MagicPlant_handle_state_wilting(Object* self, MagicPlant_Setup* objSetup, MagicPlant_Data* objData);
static void MagicPlant_handle_state_bud(Object* self, MagicPlant_Setup* objSetup, MagicPlant_Data* objData);
static f32 MagicPlant_get_growth_tvalue(MagicPlant_Setup* objSetup);

// offset: 0x0 | ctor
void MagicPlant_ctor(void *dll) { }

// offset: 0xC | dtor
void MagicPlant_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void MagicPlant_setup(Object* self, MagicPlant_Setup* objSetup, s32 arg2) {
    MagicPlant_Data* objData = self->data;

    obj_add_object_type(self, OBJTYPE_54);
    obj_add_object_type(self, OBJTYPE_63);

    if (gDLL_29_Gplay->vtbl->did_time_expire(objSetup->base.uID) == FALSE) {
        objData->growProgress = MagicPlant_get_growth_tvalue(objSetup);
    } else {
        objData->growProgress = 1.0f;
    }

    objData->animProgress = 0.0f;
    objData->state = MagicPlant_STATE_Growing;
    func_800240BC(self, objData->growProgress);
    self->srt.yaw = objSetup->yaw << 8;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;

    self->modelInstIdx = objSetup->modelInstIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        STUBBED_PRINTF("MAGICPLANT: modelno error\n");
        self->modelInstIdx = 0;
    }

    if (self->shadow != NULL) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR;
    }
}

// offset: 0x160 | func: 1 | export: 1
void MagicPlant_control(Object* self) {
    MagicPlant_Setup* objSetup;
    MagicPlant_Data* objData;

    objSetup = (MagicPlant_Setup*)self->setup;
    objData = self->data;

    //Handle LockIcon behaviours
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    if (self->unkAF & ARROW_FLAG_4_Highlighted) {
        //Display a tutorial box when the player first approaches a Magic Plant
        if (main_get_bits(BIT_Tutorial_Magic_Plant) == 0) {
            gDLL_3_Animation->vtbl->func17(0, self, -1);
            main_set_bits(BIT_Tutorial_Magic_Plant, 1);
            return;
        }
    }

    //State Machine
    switch (objData->state) {
    case MagicPlant_STATE_Growing:
        MagicPlant_handle_state_growing(self, objSetup, objData);
        break;
    case MagicPlant_STATE_Idle:
        MagicPlant_handle_state_idle(self, objSetup, objData);
        break;
    case MagicPlant_STATE_Damaged:
        MagicPlant_handle_state_damaged(self, objSetup, objData);
        break;
    case MagicPlant_STATE_Wilting:
        MagicPlant_handle_state_wilting(self, objSetup, objData);
        break;
    case MagicPlant_STATE_Bud:
        MagicPlant_handle_state_bud(self, objSetup, objData);
        break;
    }

    //Advance current animation's tValue
    func_80024108(self, objData->animProgress, gUpdateRateF, NULL);
}

// offset: 0x300 | func: 2 | export: 2
void MagicPlant_update(Object *self) { }

// offset: 0x30C | func: 3 | export: 3
void MagicPlant_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    MtxF* jointMtx;
    f32 x;
    f32 y;
    f32 z;
    MtxF mtx;
    SRT srt;
    Object* pad;
    MagicPlant_Setup* objSetup;
    MagicPlant_Data* objData;
    ModelInstance *modelInstance;
    s32 dustIdx;

    objData = self->data;
    objSetup = (MagicPlant_Setup*)self->setup;
    if (visibility == 0) {
        return;
    }

    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    modelInstance = self->modelInsts[self->modelInstIdx];

    //Position the MagicDust gem at the centre of the plant's petals
    if ((objData->magic != NULL) && (modelInstance->unk34 & 8) && (objData->magic->unkC4 != NULL)) {
        //Get the matrix for the end joint of the main joint chain
        jointMtx = func_80032170(self, ATTACH_JOINT_ID);
        if (jointMtx == NULL) {
            return;
        }

        //Get coords for the end joint of the main joint chain
        func_800321E4(self, ATTACH_JOINT_ID, &x, &y, &z);

        dustIdx = objSetup->dustIdx;
        srt.transl.x = x;
        srt.transl.y = dMagicDustY[dustIdx & 3];
        srt.transl.z = z;
        srt.yaw = 0;
        srt.pitch = 0;
        srt.roll = 0;
        srt.scale = objData->magic->srt.scale / self->srt.scale;
        matrix_from_srt(&mtx, &srt);
        matrix_concat_4x3(&mtx, jointMtx, &mtx);

        objData->magic->srt.transl.x = mtx.m[3][0] + gWorldX;
        objData->magic->srt.transl.y = mtx.m[3][1];
        objData->magic->srt.transl.z = mtx.m[3][2] + gWorldZ;

        func_80034FF0(&mtx);
        draw_object(objData->magic, gdl, mtxs, vtxs, pols, 1.0f);
        func_80034FF0(NULL);
    }
}

// offset: 0x52C | func: 4 | export: 4
void MagicPlant_free(Object* self, s32 a1) {
    MagicPlant_Data* objData = self->data;
    u32 soundHandle;

    //Stop twinkling loop
    soundHandle = objData->soundHandle;
    if (soundHandle != 0) {
        gDLL_6_AMSFX->vtbl->stop(soundHandle);
        objData->soundHandle = 0;
    }

    obj_free_object_type(self, OBJTYPE_54);
    obj_free_object_type(self, OBJTYPE_63);
}

// offset: 0x5CC | func: 5 | export: 5
u32 MagicPlant_get_model_flags(Object* self) {
    MagicPlant_Setup* objSetup = (MagicPlant_Setup*)self->setup;
    return MODFLAGS_MODEL_INDEX(objSetup->modelInstIdx) | MODFLAGS_LOAD_SINGLE_MODEL;
}

// offset: 0x5E4 | func: 6 | export: 6
u32 MagicPlant_get_data_size(Object *self, u32 a1) {
    return sizeof(MagicPlant_Data);
}

// offset: 0x5F8 | func: 7
static void MagicPlant_create_magic_dust(Object* self, s32 objectID) {
    MagicDust_Setup* dustSetup;
    MagicPlant_Setup* objSetup;
    Object* magicDust;
    MagicPlant_Data* objData;

    objSetup = (MagicPlant_Setup*)self->setup;
    objData = self->data;

    dustSetup = obj_alloc_setup(sizeof(MagicDust_Setup), objectID);
    dustSetup->unk1A = 20;
    dustSetup->unk2C = -1;
    dustSetup->unk1C = -1;
    dustSetup->base.x = self->srt.transl.x;
    dustSetup->base.y = self->srt.transl.y;
    dustSetup->base.z = self->srt.transl.z;
    dustSetup->unk24 = -1;
    dustSetup->base.loadFlags = objSetup->base.loadFlags;
    dustSetup->base.byte5 = objSetup->base.byte5;
    dustSetup->base.byte6 = objSetup->base.byte6;
    dustSetup->base.fadeDistance = objSetup->base.fadeDistance - 15;

    magicDust = obj_create(
        &dustSetup->base,
        OBJINIT_STANDALONE | OBJINIT_FLAG4,
        self->mapID,
        -1,
        self->parent
    );
    magicDust->unkC4 = self;
    objData->magic = magicDust;
}

// offset: 0x6CC | func: 8
/**
  * Bud waits for a gplay regrowth timer to expire (if active),
  * then grows up into the idle state and creates a new MagicDust gem Object.
  */
void MagicPlant_handle_state_growing(Object* self, MagicPlant_Setup* objSetup, MagicPlant_Data* objData) {
    //Wait for the regrowth timer to expire (if it's active), then create a new MagicDust gem
    if (gDLL_29_Gplay->vtbl->did_time_expire(objSetup->base.uID)) {
        MagicPlant_create_magic_dust(self, dMagicDustObjIDs[objSetup->dustIdx & 3]);
        objData->state = MagicPlant_STATE_Idle;
        objData->idleAnimDelay = rand_next(300, 600);
    } else {
        objData->growProgress = MagicPlant_get_growth_tvalue(objSetup);
    }

    //Use the growing-from-bud animation
    if (self->curModAnimId != MagicPlant_MODANIM_Growing) {
        func_80023D30(self, MagicPlant_MODANIM_Growing, objData->growProgress, 0);
    }

    //Set animation tValue
    func_800240BC(self, objData->growProgress);
}


// offset: 0x7E8 | func: 9
/**
  * Plant sways in the wind, reacts to damage, and plays twinkling sound when nearby.
  */
void MagicPlant_handle_state_idle(Object *self, MagicPlant_Setup* objSetup, MagicPlant_Data* objData) {
    s32 hitType;
    s32 hitDamage;
    s32 i;
    s32 hitSphereID;
    Object *hitBy;
    f32 playerDistance;
    s32 random;
    Object *player = get_player();
    u32 dModGfxParams[4] = { 8, 0xff, 0xff, 0x78 };
    DLL_IModgfx* dll;
    SRT transform;

    //Allow targetting
    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;

    //Check for damage
    hitType = func_8002601C(
        self,
        &hitBy,
        &hitSphereID,
        &hitDamage,
        &transform.transl.x,
        &transform.transl.y,
        &transform.transl.z
    );

    //React to damage
    if ((hitType != 0) && (hitDamage != 0) && (hitType != 0)) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_618_Slice_Impact, MAX_VOLUME, 0, NULL, 0, NULL);
        objData->state = MagicPlant_STATE_Damaged;
        objData->animProgress = 0.03f;
        func_80023D30(self, MagicPlant_MODANIM_Damage_Shake, 0.0f, 0);
        for (i = 20; i > 0; i--) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_34E, NULL, 2, -1, NULL);
        }
        transform.transl.x += gWorldX;
        transform.transl.z += gWorldZ;
        transform.roll = 0;
        transform.pitch = 0;
        transform.yaw = 0;
        transform.scale = 1.0f;
        dll = dll_load(DLL_ID_106, 1, FALSE);
        ((DLL_IModgfx*)dll)->vtbl->func0(NULL, 1, &transform, 0x401, -1, dModGfxParams);
        if (dll) {
            dll_unload(dll);
        }
    }

    //Handle animations
    if (objData->state == MagicPlant_STATE_Idle) {
        if (self->curModAnimId == MagicPlant_MODANIM_Rustled) {
            //Return to idle loop after playing (unused?) heavy sway idle variant!
            if (self->animProgress >= 1.0f) {
                objData->animProgress = 0.005f;
                func_80023D30(self, MagicPlant_MODANIM_Sway_Loop, 0.0f, 0);
            } else {
                objData->animProgress = 0.01f;
            }
        } else {
            //Start idle loop after a random delay
            objData->idleAnimDelay -= gUpdateRate;
            if (objData->idleAnimDelay <= 0) {
                objData->idleAnimDelay = rand_next(300, 600);
            } else if (self->curModAnimId != MagicPlant_MODANIM_Sway_Loop) {
                objData->animProgress = 0.005f;
                random = rand_next(0, 99); //Start at random point in animation (likely to desync nearby idling plants)
                func_80023D30(self, MagicPlant_MODANIM_Sway_Loop, random * 0.01f, 0);
            }
        }
    }

    //Start/stop twinkling sound loop as player approaches/leaves
    playerDistance = vec3_distance(&self->globalPosition, &player->globalPosition);
    if (objData->soundHandle == 0) {
        if (playerDistance < TWINKLE_START_DISTANCE) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_619_Twinkle_Loop, 96, &objData->soundHandle, NULL, 0, NULL);
        }
    } else if (playerDistance > TWINKLE_STOP_DISTANCE) {
        gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
        objData->soundHandle = 0;
    }
}

// offset: 0xC00 | func: 10
/**
  * Plant shakes and drops the MagicDust gem.
  */
void MagicPlant_handle_state_damaged(Object* self, MagicPlant_Setup *objSetup, MagicPlant_Data* objData) {
    Object* magicDust;
    Object* player;
    f32 speed;
    s16 angle;
    f32 dx;
    f32 dz;

    player = get_player();

    //Stop twinkling sound
    if (objData->soundHandle != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
        objData->soundHandle = 0;
    }

    //Drop crystal gem (MagicDust)
    magicDust = objData->magic;
    if ((magicDust != NULL) && (magicDust->unkC4 != NULL)) {
        if (self->animProgress >= 0.8f) {
            //Clear plant's reference to gem, and gem's reference to plant
            objData->magic = NULL;
            magicDust->unkC4 = NULL;

            //Set gem's speed so it flies in direction plant was facing (with slight variance)
            speed = rand_next(39, 44) / 100.0f;
            dx = self->srt.transl.x - player->srt.transl.x;
            dz = self->srt.transl.z - player->srt.transl.z;
            angle = arctan2_f(dx, dz);
            rand_next(angle - 0x1000, angle + 0x1000); //@bug? result not used
            magicDust->velocity.x = fsin16_precise(self->srt.yaw) * speed;
            magicDust->velocity.z = fcos16_precise(self->srt.yaw) * speed;

            //Play ringing sound
            gDLL_6_AMSFX->vtbl->play(self, SOUND_61A_Crystal_Ringing, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    }

    //Advance state when hit shake animation finished
    if (self->animProgress >= 1.0f) {
        objData->state = MagicPlant_STATE_Wilting;
        objData->animProgress = 0.004f;
        func_80023D30(self, MagicPlant_MODANIM_Wilting, 0.0f, 0);
    }
}


// offset: 0xE04 | func: 11
/**
  * Plant falls over and fades out.
  */
void MagicPlant_handle_state_wilting(Object* self, MagicPlant_Setup* objSetup, MagicPlant_Data* objData) {
    s32 opacity;

    //Fade out when wilting animation finished
    if (self->animProgress >= 1.0f) {
        opacity = self->opacity;
        opacity -= (gUpdateRate * 2);

        //Advance state when fully faded out
        if (opacity < 0) {
            opacity = 0;
            objData->state = MagicPlant_STATE_Bud;
            objData->growProgress = 0.0f;
            objData->animProgress = 0.0f;
            func_80023D30(self, MagicPlant_MODANIM_Growing, 0.0f, 0);
            func_800240BC(self, 0.0f);
        }
        self->opacity = opacity;
    }

    self->objhitInfo->unk58 &= ~1;
}

// offset: 0xED8 | func: 12
/**
  * Bud fades in, and starts a gplay regrowth timer.
  */
void MagicPlant_handle_state_bud(Object* self, MagicPlant_Setup* objSetup, MagicPlant_Data* objData) {
    s32 opacity;

    //Fade in and set a gplay regrowth timer
    opacity = self->opacity;
    opacity += gUpdateRate;
    if (opacity >= OBJECT_OPACITY_MAX) {
        objData->state = MagicPlant_STATE_Growing;
        opacity = OBJECT_OPACITY_MAX;
        gDLL_29_Gplay->vtbl->add_time(objSetup->base.uID, objSetup->regrowthTime);
    }
    self->opacity = opacity;
    self->objhitInfo->unk58 |= 1;
}

// offset: 0xF94 | func: 13
/**
  * Returns the animation tValue to use while the plant's growing
  */
f32 MagicPlant_get_growth_tvalue(MagicPlant_Setup* objSetup) {
    f32 tValue;
    f32 remaining;
    s32 duration;

    remaining = gDLL_29_Gplay->vtbl->get_time_remaining(objSetup->base.uID);

    duration = objSetup->regrowthTime;
    if (duration < 100) {
        duration = 100;
    }

    tValue = remaining / duration;
    if (tValue > 1.0f) {
        tValue = 1.0f;
    } else if (tValue < 0.0f) {
        tValue = 0.0f;
    }

    return tValue = 1.0f - tValue;
}
