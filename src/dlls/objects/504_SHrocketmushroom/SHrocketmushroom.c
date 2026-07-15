#include "common.h"
#include "game/objects/object.h"
#include "game/objects/interaction_arrow.h"
#include "sys/objtype.h"
#include "dlls/engine/17_partfx.h"
#include "dlls/objects/357_ExplodeAnimator.h"
#include "dlls/objects/358_debris.h"
#include "dlls/objects/505_SHspore.h"

typedef struct {
    ObjSetup base;
    s16 regrowthDelay;
    s16 sporeLaunchInterval;    //Frames between spore launches (actual interval has some randomised variance added to this value)
    s16 gamebitGrow;            //When specified, plant only grows when gamebit set (and gamebit unset upon exploding)
    s8 sporeAngularRange;       //Variance in launched SHspores' flight yaw
    s8 yaw;
} SHrocketmushroom_Setup;

typedef struct {
    f32 timer;          //Countdown timer for various states (waiting, growing, launching spore)
    f32 scaleMax;       //Scale threshold when growing
    f32 timerDefault;   //Stores a copy of the max timer value, but unused otherwise
    f32 scaleInitial;   //Stores a copy of the initial scale
    f32 scaleSpeed;     //Rate of change of scale
    u8 state;           //State Machine value
    u8 flags;           //Tracks animations finishing, entering a state, and launching spores
} SHrocketmushroom_Data;

typedef enum {
    STATE_0_Idle,           //Waiting, counting down to launching a spore
    STATE_1_Hidden,         //Hidden after being destroyed, waiting to regrow
    STATE_2_Growing,        //Growing from ground, into idle state afterwards
    STATE_3_Launch_Spore,   //Launching a spore
    STATE_4_Damaged         //Explodes into pieces
} SHrocketmushroom_States;

typedef enum {
    FLAG_None = 0,
    FLAG_Animation_Finished = 1,
    FLAG_State_Entered = 2,
    FLAG_Spore_Launched = 4
} SHrocketmushroom_Flags;

typedef enum {
    ANIMFLAG_Vulnerable = 1,
    ANIMFLAG_Can_Target = 2,
    ANIMFLAG_Hidden = 4,
    ANIMFLAG_Obstructs = 8,
    ANIMFLAG_Exploding = 0x10
} SHrocketmushroom_AnimFlags;

typedef struct {
    s16 modAnimID;
    f32 animSpeed;
    u8 flags;
} SHrocketmushroom_AnimData;

/*0x0*/ static SHrocketmushroom_AnimData dAnimData[5] = {
    {0, 0,      ANIMFLAG_Obstructs | ANIMFLAG_Vulnerable | ANIMFLAG_Can_Target}, //STATE_0_Idle
    {0, 0,      ANIMFLAG_Hidden},                                                //STATE_1_Hidden
    {0, 0,      ANIMFLAG_Obstructs},                                             //STATE_2_Growing
    {2, 0.01,   ANIMFLAG_Obstructs | ANIMFLAG_Vulnerable | ANIMFLAG_Can_Target}, //STATE_3_Launch_Spore
    {1, 0.008,  ANIMFLAG_Obstructs | ANIMFLAG_Exploding}                         //STATE_4_Damaged
};

static void SHrocketmushroom_handle_state_1_hidden(Object* self, SHrocketmushroom_AnimData* animData, SHrocketmushroom_Data* objData);
static void SHrocketmushroom_handle_state_2_growing(Object* self, SHrocketmushroom_AnimData* animData, SHrocketmushroom_Data* objData);
static void SHrocketmushroom_handle_state_3_launch_spore(Object* self, SHrocketmushroom_AnimData* animData, SHrocketmushroom_Data* objData);
static void SHrocketmushroom_handle_state_4_damaged(Object* self, SHrocketmushroom_AnimData* animData, SHrocketmushroom_Data* objData);
static void SHrocketmushroom_handle_state_0_idle(Object* self, SHrocketmushroom_AnimData* animData, SHrocketmushroom_Data* objData);
static void SHrocketmushroom_set_state(SHrocketmushroom_Data* self, s32 state);
static void SHrocketmushroom_create_spore(Object* self, SHrocketmushroom_Data* objData);
static void SHrocketmushroom_explode(Object* self, SHrocketmushroom_Data* objData);
static void SHrocketmushroom_reset(Object* self, SHrocketmushroom_Data* objData, int startAtZeroScale);
static int SHrocketmushroom_is_player_far_away(Object* self);

// offset: 0x0 | ctor
void SHrocketmushroom_ctor(void *dll) { }

// offset: 0xC | dtor
void SHrocketmushroom_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHrocketmushroom_setup(Object* self, SHrocketmushroom_Setup* objSetup, s32 reset) {
    SHrocketmushroom_Data* objData = self->data;
    
    self->srt.yaw = objSetup->yaw << 8;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
    self->stateFlags |= OBJSTATE_PRINT_DISABLED;
    objData->scaleInitial = self->srt.scale;
    
    if (reset == FALSE) {
        if ((objSetup->gamebitGrow != NO_GAMEBIT) && (mainGetBits(objSetup->gamebitGrow) == FALSE)) {
            //If the plant only grows when a gamebit is set, start out hidden
            SHrocketmushroom_reset(self, objData, TRUE);
            objData->state = STATE_1_Hidden;
        } else {
            //Otherwise start out in idle state
            SHrocketmushroom_reset(self, objData, FALSE);
        }
    }
}

// offset: 0xEC | func: 1 | export: 1
void SHrocketmushroom_control(Object* self) {
    SHrocketmushroom_Data* objData;
    SHrocketmushroom_AnimData* animData;
    s32 hitSphereID;
    s32 hitDamage;
    Object* hitBy;
    SRT srt;
    Vec3f offset;
    s32 i;
    MtxF mtx;

    objData = self->data;
    animData = &dAnimData[objData->state];
    
    switch (objData->state) {
    case STATE_1_Hidden:
        SHrocketmushroom_handle_state_1_hidden(self, animData, objData);
        break;
    case STATE_2_Growing:
        SHrocketmushroom_handle_state_2_growing(self, animData, objData);
        break;
    case STATE_3_Launch_Spore:
        SHrocketmushroom_handle_state_3_launch_spore(self, animData, objData);
        break;
    case STATE_4_Damaged:
        SHrocketmushroom_handle_state_4_damaged(self, animData, objData);
        break;
    case STATE_0_Idle:
    default:
        SHrocketmushroom_handle_state_0_idle(self, animData, objData);
        break;
    }

    //Handle being attacked
    if ((animData->flags & ANIMFLAG_Vulnerable) && 
        func_80025F40(self, &hitBy, &hitSphereID, &hitDamage) && 
        (hitDamage != 0)
    ) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_744_Mushroom_Hit, MAX_VOLUME, NULL, NULL, 0, NULL);
        SHrocketmushroom_set_state(objData, STATE_4_Damaged);

        //Create big explosion effect
        srt.yaw = self->srt.yaw;
        srt.pitch = self->srt.pitch;
        srt.roll = self->srt.roll;
        srt.transl.x = 0.0f;
        srt.transl.y = 0.0f;
        srt.transl.z = 0.0f;
        srt.scale = 1.0f;
        mathYprXyzMtx(&mtx, &srt);
        mathMtxXFMF(&mtx, 0.0f, 1.0f, 0.0f, &offset.x, &offset.y, &offset.z);
        srt.transl.x = self->srt.transl.x + (offset.x * 23.0f);
        srt.transl.y = self->srt.transl.y + (offset.y * 23.0f);
        srt.transl.z = self->srt.transl.z + (offset.z * 23.0f);
        srt.scale = 518.0f;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, &srt, 0x200001, -1, NULL);
        
        //Create lots of small particles
        srt.transl.x -= self->srt.transl.x;
        srt.transl.y -= self->srt.transl.y;
        srt.transl.z -= self->srt.transl.z;
        for (i = 0; i < 20; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_324, &srt, 2, -1, NULL);
        }
        
        //ObjHits
        func_8002683C(self, self->def->hitbox_flagsB6 + 0x50, self->def->unk94 - 0x50, self->def->unk96 + 0x50);
    }
    
    //Handle the animation's flags
    {
        //Apply/remove obstructive collision
        if (animData->flags & ANIMFLAG_Obstructs) {
            func_8002674C(self);
        } else {
            func_800267A4(self);
        }
        
        //Apply/remove damaging collision
        if (animData->flags & ANIMFLAG_Exploding) {
            func_80026128(self, Damage_Type_Sword_Staff_Strike1, 1, 0);
        } else {
            func_80026160(self);
        }
        
        //Allow/disallow targetting
        if (animData->flags & ANIMFLAG_Can_Target) {
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        } else {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
        }
        
        //Hide/show
        if (animData->flags & ANIMFLAG_Hidden) {
            self->srt.flags |= OBJFLAG_INVISIBLE;
        } else {
            self->srt.flags &= ~OBJFLAG_INVISIBLE;
        }
    }
    
    //Change modAnim when needed
    if (self->curModAnimId != animData->modAnimID) {
        objAnimSet(self, animData->modAnimID, 0.0f, 0);
    }
    
    //Track when the current animation is finished
    if (objAnimAdvance(self, animData->animSpeed, gUpdateRateF, NULL) != 0) {
        objData->flags |= FLAG_Animation_Finished;
    } else {
        objData->flags &= ~FLAG_Animation_Finished;
    }
}

// offset: 0x5C8 | func: 2
void SHrocketmushroom_handle_state_1_hidden(Object* self, SHrocketmushroom_AnimData* animData, SHrocketmushroom_Data* objData) {
    SHrocketmushroom_Setup* objSetup = (SHrocketmushroom_Setup*)self->setup;
    
    //On entering state, set up the countdown timer until regrowing
    if (objData->flags & FLAG_State_Entered) {
        objData->flags &= ~FLAG_State_Entered;
        objData->timer = objSetup->regrowthDelay;
    }

    //If the plant uses a gamebit, don't start growing until that gamebit is set (and the player is far away)
    if (objSetup->gamebitGrow != NO_GAMEBIT) {
        if (mainGetBits(objSetup->gamebitGrow) && SHrocketmushroom_is_player_far_away(self)) {
            SHrocketmushroom_set_state(objData, STATE_2_Growing);
        }
        return;
    }
    
    //If the plant doesn't use a gamebit, just count down until regrowing
    objData->timer -= gUpdateRateF;
    if (objData->timer <= 0.0f) {
        if (SHrocketmushroom_is_player_far_away(self)) {
            SHrocketmushroom_set_state(objData, STATE_2_Growing);
        }
        objData->timer = 0.0f;
    }
}

// offset: 0x70C | func: 3
void SHrocketmushroom_handle_state_2_growing(Object* self, SHrocketmushroom_AnimData* animData, SHrocketmushroom_Data* objData) {
    //When entering state, play growing sound and initialise transform-related values
    if (objData->flags & FLAG_State_Entered) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_8A3_Rocket_Mushroom_Grow, MAX_VOLUME, NULL, NULL, 0, NULL);
        objData->flags &= ~FLAG_State_Entered;
        SHrocketmushroom_reset(self, objData, TRUE);
    }
    
    //Stop growing after scale threshold reached
    if (self->srt.scale > objData->scaleMax) {
        objData->scaleSpeed /= 1.1f;
    }
    
    if (objData->scaleSpeed < 0.00001f) {
        objData->scaleSpeed = 0.0f;
    }
    
    //Count down until growing finished
    self->srt.scale += objData->scaleSpeed * gUpdateRateF;
    objData->timer -= gUpdateRateF;    
    if (objData->timer < 0.0f) {
        SHrocketmushroom_set_state(objData, STATE_0_Idle);
    }
}

// offset: 0x86C | func: 4
void SHrocketmushroom_handle_state_3_launch_spore(Object* self, SHrocketmushroom_AnimData* animData, SHrocketmushroom_Data* objData) {
    //Clear launch flag on entering state
    if (objData->flags & FLAG_State_Entered) {
        objData->flags &= ~(FLAG_Spore_Launched | FLAG_State_Entered);
    }
    
    //Launch a spore
    if ((self->animProgress > 0.5f) && !(objData->flags & FLAG_Spore_Launched)) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_8A1_Spore_Launched, MAX_VOLUME, NULL, NULL, 0, NULL);
        SHrocketmushroom_create_spore(self, objData);
        objData->flags |= FLAG_Spore_Launched;
    }
    
    if (objData->flags & FLAG_Animation_Finished) {
        SHrocketmushroom_set_state(objData, STATE_0_Idle);
    }
}

// offset: 0x970 | func: 5
void SHrocketmushroom_handle_state_4_damaged(Object* self, SHrocketmushroom_AnimData* animData, SHrocketmushroom_Data* objData) {
    SHrocketmushroom_Setup* objSetup = (SHrocketmushroom_Setup*)self->setup;
    
    gDLL_6_AMSFX->vtbl->play(self, SOUND_8A0_Deflate_Honk, MAX_VOLUME, NULL, NULL, 0, NULL);
    SHrocketmushroom_explode(self, objData);
    SHrocketmushroom_set_state(objData, STATE_1_Hidden);

    //If the plant only grows when a gamebit is set, switch off that gamebit when the plant's destroyed
    if (objSetup->gamebitGrow != NO_GAMEBIT) {
        mainSetBits(objSetup->gamebitGrow, FALSE);
    }
}

// offset: 0xA40 | func: 6
void SHrocketmushroom_handle_state_0_idle(Object* self, SHrocketmushroom_AnimData* animData, SHrocketmushroom_Data* objData) {
    SHrocketmushroom_Setup* objSetup = (SHrocketmushroom_Setup*)self->setup;

    //Set up countdown to launching a spore
    if (objData->flags & FLAG_State_Entered) {
        objData->flags &= ~FLAG_State_Entered;
        objData->timer = mathRnd(-50, 50) + objSetup->sporeLaunchInterval;
    }
    
    //When timer runs out, change to spore launching state
    objData->timer -= gUpdateRateF;
    if (objData->timer <= 0.0f) {
        SHrocketmushroom_set_state(objData, STATE_3_Launch_Spore);
    }
}

// offset: 0xB08 | func: 7
void SHrocketmushroom_set_state(SHrocketmushroom_Data* objData, s32 state) {
    objData->state = state;
    objData->flags |= FLAG_State_Entered;
}

// offset: 0xB20 | func: 8 | export: 2
void SHrocketmushroom_update(Object *self) { }

// offset: 0xB2C | func: 9 | export: 3
void SHrocketmushroom_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0xB44 | func: 10 | export: 4
void SHrocketmushroom_free(Object *self, s32 onlySelf) { }

// offset: 0xB54 | func: 11 | export: 5
u32 SHrocketmushroom_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xB64 | func: 12 | export: 6
u32 SHrocketmushroom_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(SHrocketmushroom_Data);
}

// offset: 0xB78 | func: 13
void SHrocketmushroom_create_spore(Object* self, SHrocketmushroom_Data* objData) {
    SHSpore_Setup* sporeSetup;
    MtxF mtx;
    SRT srt;
    Vec3f offset;
    SHrocketmushroom_Setup* objSetup;

    objSetup = (SHrocketmushroom_Setup*)self->setup;
    sporeSetup = objAllocSetup(sizeof(SHSpore_Setup), OBJ_SHspore);

    //Calculate initial coordinate
    srt.yaw = self->srt.yaw;
    srt.pitch = self->srt.pitch;
    srt.roll = self->srt.roll;
    srt.transl.x = 0.0f;
    srt.transl.y = 0.0f;
    srt.transl.z = 0.0f;
    srt.scale = 1.0f;
    mathYprXyzMtx(&mtx, &srt);
    mathMtxXFMF(&mtx, 0.0f, 1.0f, 0.0f, &offset.f[0], &offset.f[1], &offset.f[2]);
    srt.transl.x = offset.f[0] * 26.0f;
    srt.transl.y = offset.f[1] * 26.0f;
    srt.transl.z = offset.f[2] * 26.0f;
    
    sporeSetup->base.x = self->srt.transl.x + srt.transl.x;
    sporeSetup->base.y = self->srt.transl.y + srt.transl.y;
    sporeSetup->base.z = self->srt.transl.z + srt.transl.z;
    sporeSetup->base.byte5 = 1;
    sporeSetup->base.loadFlags = 2;
    sporeSetup->angularRange = objSetup->sporeAngularRange << 8;
    sporeSetup->windAngle = self->srt.yaw;
    objSetupObject((ObjSetup*)sporeSetup, 5, -1, -1, NULL);
}

// offset: 0xCF8 | func: 14
void SHrocketmushroom_explode(Object* self, SHrocketmushroom_Data* objData) {
    Debris_Setup* debrisSetup;
    ExplodeAnimator_Setup* exploderSetup;
    MtxF mtx;
    f32 distance = 80.0f;
    Vec3f offset;
    SRT srt;
    Object* obj;
    s32 i;

    srt.yaw = self->srt.yaw;
    srt.pitch = self->srt.pitch;
    srt.roll = self->srt.roll;
    srt.transl.x = 0.0f;
    srt.transl.y = 0.0f;
    srt.transl.z = 0.0f;
    srt.scale = 1.0f;
    mathYprXyzMtx(&mtx, &srt);
    mathMtxXFMF(&mtx, 0.0f, 1.0f, 0.0f, &offset.x, &offset.y, &offset.z);
    srt.transl.x = offset.x * 23.0f;
    srt.transl.y = offset.y * 23.0f;
    srt.transl.z = offset.z * 23.0f;
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3E9, &srt, 2, -1, NULL);

    //Create debris
    for (i = 0; i < 5; i++) {
        debrisSetup = objAllocSetup(sizeof(Debris_Setup), OBJ_SHmushroombit);
        debrisSetup->base.byte5 = 1;
        debrisSetup->base.loadFlags = 2;
        debrisSetup->base.x = self->srt.transl.x + srt.transl.x;
        debrisSetup->base.y = self->srt.transl.y + srt.transl.y;
        debrisSetup->base.z = self->srt.transl.z + srt.transl.z;
        debrisSetup->unk42 = self->srt.transl.x + (offset.x * 13.0f);
        debrisSetup->unk44 = self->srt.transl.y + (offset.y * 13.0f);
        debrisSetup->unk46 = self->srt.transl.z + (offset.z * 13.0f);
        debrisSetup->unk18 = i;
        debrisSetup->unk1A = self->srt.yaw;
        debrisSetup->unk1C = self->srt.pitch;
        debrisSetup->unk1E = self->srt.roll;
        debrisSetup->unk20 = mathRnd(200, 300);
        debrisSetup->unk28 = -50;
        debrisSetup->unk2E = mathRnd(1200, 2000);
        debrisSetup->unk30 = mathRnd(1200, 2000);
        debrisSetup->unk38 = 200;
        debrisSetup->unk3A = 150;
        debrisSetup->unk3C = 7;
        debrisSetup->unk3E = -1;
        debrisSetup->unk40 = -1;
        objSetupObject((ObjSetup*)debrisSetup, 5, -1, -1, NULL);
    }
    
    self->objhitInfo->unk58 |= 0x40;
    
    //Set nearby ExplodeAnimator's gamebit (when used to blast open walls)
    obj = objGetNearestTypeTo(OBJTYPE_ExplodeAnimator, self, &distance);
    if (obj != NULL) {
        exploderSetup = (ExplodeAnimator_Setup*)obj->setup;
        if (exploderSetup->gamebitExplodeTrigger != NO_GAMEBIT) {
            mainSetBits(exploderSetup->gamebitExplodeTrigger, 1);
        }
    }
}

// offset: 0x104C | func: 15
void SHrocketmushroom_reset(Object* self, SHrocketmushroom_Data* objData, int startAtZeroScale) {
    SHrocketmushroom_Setup* objSetup;

    objSetup = (SHrocketmushroom_Setup*)self->setup;
    self->srt.roll = mathRnd(-1500, 1500);
    self->srt.pitch = mathRnd(-1500, 1500);
    self->opacity = OBJECT_OPACITY_MAX;
    self->srt.flags &= ~OBJFLAG_INVISIBLE;
    self->srt.transl.x = objSetup->base.x;
    self->srt.transl.y = objSetup->base.y;
    self->srt.transl.z = objSetup->base.z;
    
    if (startAtZeroScale) {
        self->srt.scale = 0.00001f;
        objData->timerDefault = 135.0f;
        objData->timer = 135.0f;
        objData->scaleMax = objData->scaleInitial;
        objData->scaleSpeed = objData->scaleInitial / 135.0f;
    }
    
    func_800264D0(self);
}

// offset: 0x114C | func: 16
/** Checks whether the player is more than 80 units away. */
int SHrocketmushroom_is_player_far_away(Object* self) {
    return (vec3DistanceSquared(&self->globalPosition, &objGetPlayer()->globalPosition) > SQ(80));
}
