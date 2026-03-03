#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/common/collectable.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "sys/gfx/model.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

typedef struct {
    f32 distanceToPlayer;
    f32 interactionRadius;
    s16 gamebitCollected;
    s16 gamebitCount;
    s16 gamebitFall;            //(Optional) Collectable falls from tree when set
    u8 fallFlags;               //For Gold Nugget falling out of tree and bouncing on ground
    s8 delayInteractionTimer;
} SCCollectables_Data;

/** For the Gold Nugget falling out of tree and bouncing on ground */
typedef enum {
    FLAG_Fall_Start = 1,
    FLAG_Fall_Finished = 2
} SCcollectables_FallFlags;

/** Collectable's height offset from ground */
#define REST_HEIGHT 9.0f

static void SCcollectables_handle_motion(Object* self, u8 alreadyOnGround);
static void SCcollectables_collect(Object* self);

// offset: 0x0 | ctor
void SCcollectables_ctor(void *dll) { }

// offset: 0xC | dtor
void SCcollectables_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SCcollectables_setup(Object* self, Collectable_Setup* objsetup, UNK_TYPE_32 arg2) {
    SCCollectables_Data* objdata;

    objdata = self->data;

    obj_add_object_type(self, OBJTYPE_5);
    obj_init_mesg_queue(self, 2);

    self->srt.yaw = objsetup->yaw << 8;
    self->srt.pitch = objsetup->pitch << 8;
    self->srt.roll = objsetup->roll << 8;
    self->srt.scale = self->def->scale;
    self->modelInstIdx = objsetup->modelIdx;
    self->speed.y = 0.0f;
    
    if (self->objhitInfo) {
        self->objhitInfo->unk52 = objsetup->objHitsValue;
    }
    
    objdata->gamebitCount = objsetup->gamebitCount;
    objdata->delayInteractionTimer = 60;
    objdata->gamebitFall = objsetup->gamebitSecondary;

    //Check if already knocked out of tree
    if (objdata->gamebitFall != NO_GAMEBIT) {
        if (main_get_bits(objdata->gamebitFall)) {
            SCcollectables_handle_motion(self, TRUE);
        }
    } else {
        SCcollectables_handle_motion(self, TRUE);
    }
    
    //Check if already collected
    objdata->gamebitCollected = objsetup->gamebitCollected;
    if (objdata->gamebitCollected != NO_GAMEBIT) {
        self->unkDC = main_get_bits(objdata->gamebitCollected);
    } else {
        self->unkDC = 0;
    }
    
    if (self->unkDC) {
        return;
    }
    
    //Set interaction radius based on objDef collectable data, or objDef lock-on data
    if (self->def->collectableDef) {
        objdata->interactionRadius = self->def->collectableDef->interactionRadius;
    } else {
        objdata->interactionRadius = 50.0f;
    }
    if (self->def->unk40) {
        objdata->interactionRadius = self->def->unk40->unk0c * 4;
    }

    //Set shadow flags
    if (self->shadow) {
        self->shadow->flags |= 0x800 | 0x10;
    }
}

// offset: 0x224 | func: 1 | export: 1
void SCcollectables_control(Object* self) {
    SCCollectables_Data* objdata;
    s32 collisionOut2;
    s32 collisionOut3;
    Object* messageSender;
    Object* player;
    Object* collidedObject;
    CollectableDef* collectableDef;
    f32 distance;
    u32 outMessage;
    s32 index;

    objdata = self->data;
    
    self->unkAF |= ARROW_FLAG_8_No_Targetting;

    //Check if should fall from tree
    if (objdata->gamebitFall != NO_GAMEBIT && main_get_bits(objdata->gamebitFall)) {
        objdata->fallFlags |= FLAG_Fall_Start;
    }
    
    //Decrement interaction delay
    objdata->delayInteractionTimer -= gUpdateRate;
    if (objdata->delayInteractionTimer < 0) {
        objdata->delayInteractionTimer = 0;
    }
    
    //Check for collection message (set gamebits and hide self)
    while (obj_recv_mesg(self, &outMessage, &messageSender, 0)){
        if (outMessage == 0x7000B) {
            SCcollectables_collect(self);
        }
    }
    
    if (self->srt.flags & 0x4000) {
        return;
    }
    
    //Remove collision after collection
    if (self->unkDC) {
        if (self->objhitInfo) {
            self->objhitInfo->unk58 |= 0x100;
        }

        if ((objdata->gamebitCollected != NO_GAMEBIT) && (main_get_bits(objdata->gamebitCollected) == FALSE)) {
            self->unkDC = 0;
        }
        return;
    }
    
    //Disable interaction until fall completed
    if (objdata->fallFlags & FLAG_Fall_Finished) {
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    } else {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    }

    //Handle falling/bouncing behaviour (Gold Nugget falling from tree)
    if ((objdata->fallFlags & FLAG_Fall_Start) && !(objdata->fallFlags & FLAG_Fall_Finished)) {
        SCcollectables_handle_motion(self, 0);
    }
    
    //Handle player pressing A when arrow over collectable
    player = get_player();
    if (player) {
        collectableDef = self->def->collectableDef;
        if (!collectableDef) {
            return;
        }
        
        distance = vec3_distance(&self->positionMirror, &player->positionMirror);
        if ((distance < objdata->interactionRadius) && (objdata->delayInteractionTimer == 0)) {
            outMessage = collectableDef->collectMessage << 0x10;
            if (self->unkAF & ARROW_FLAG_1_Interacted) {
                obj_send_mesg(player, 0x7000A, self, (void*)outMessage);
            }
        }
        objdata->distanceToPlayer = distance;
    }
    
    //Handle projectile collisions (bounce sound and sparkles fly out)
    if (func_80025F40(self, &collidedObject, &collisionOut2, &collisionOut3)) {
        for (index = 20; index > 0; index--){
            gDLL_17_partfx->vtbl->spawn(self, 0x424, 0, 2, -1, 0);
        }
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_613_Gold_Bounce, MAX_VOLUME, 0, 0, 0, 0);
    }
}

// offset: 0x548 | func: 2 | export: 2
void SCcollectables_update(Object *self) { }

// offset: 0x554 | func: 3 | export: 3
void SCcollectables_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility){
    SCCollectables_Data* objdata;

    objdata = self->data;
    if (visibility <= 0 || self->unkDC){
        return;
    }

    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);

    //Handle Gold Nugget's sparkles and gradual rotation
    if (self->def->collectableDef && (self->id == OBJ_SC_golden_nugge) && (objdata->distanceToPlayer < 250.0f)) {
        if (rand_next(0, 10) == 0) {
            gDLL_17_partfx->vtbl->spawn(self, 0x423, 0, 2, -1, 0);
        }
        self->srt.yaw += (s16)(182.0f * gUpdateRateF);
    }
}

// offset: 0x684 | func: 4 | export: 4
void SCcollectables_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_5);
}

// offset: 0x6C4 | func: 5 | export: 5
u32 SCcollectables_get_model_flags(Object *self){
    return MODFLAGS_10;
}

// offset: 0x6D4 | func: 6 | export: 6
u32 SCcollectables_get_data_size(Object *self, u32 a1) {
    return sizeof(SCCollectables_Data);
}

// offset: 0x6E8 | func: 7
void SCcollectables_handle_motion(Object* self, u8 alreadyOnGround) {
    SCCollectables_Data* objdata;
    f32** samples;
    f32 min;
    u32 vol;
    f32 dy;
    s32 index;
    s32 sampleCount;
    u32 minIndex;

    objdata = self->data;

    samples = NULL;
    min = 10000.0f;
    sampleCount = func_80057F1C(self, 
        self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, 
        (void*)&samples, 0, 0);
    
    //Find the floor height
    for (index = 0, minIndex = 0; index < sampleCount; index++){
        dy = *samples[index] - self->srt.transl.y;
        if (dy < 0.0f) {
            dy *= -1.0f;
        }
        if (dy < min) {
            minIndex = index;
            min = dy;
        }
    }
    
    if (samples) {        
        if (alreadyOnGround) {
            self->srt.transl.y = *samples[minIndex] + REST_HEIGHT;
            objdata->fallFlags |= FLAG_Fall_Finished;
            self->speed.y = 0.0f;
        } else {
            if (self->srt.transl.y > *samples[minIndex] + REST_HEIGHT - (self->speed.y * gUpdateRateF)) {
            //Falling
                if (self->speed.y > -4.0f) {
                    self->speed.y -= 0.17f; //@bug: framerate dependent
                }
            } else {
            //Bouncing
                self->srt.transl.y = *samples[minIndex] + REST_HEIGHT - (self->speed.y * gUpdateRateF);
                self->speed.y = 0.0f - (self->speed.y * 0.4f); //lose kinetic energy on bounce

                //End fall when bounce speed near 0
                dy = sqrtf(SQ(self->speed.y));
                if (dy < 0.25f) {
                    self->srt.transl.y = *samples[minIndex] + REST_HEIGHT;
                    objdata->fallFlags |= FLAG_Fall_Finished;
                    self->speed.y = 0.0f;
                }

                //Play bounce sound (volume dependent on speed)
                vol = dy * MAX_VOLUME_F;
                if (vol > MAX_VOLUME){ 
                    vol = MAX_VOLUME; 
                }
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_613_Gold_Bounce, vol, 0, 0, 0, 0);
            }
        }
    }
    
    obj_integrate_speed(self, 0.0f, self->speed.y * gUpdateRateF, 0.0f);
}

// offset: 0xA2C | func: 8
void SCcollectables_collect(Object* self) {
    SCCollectables_Data* objdata = self->data;
    Collectable_Setup* objsetup;

    objsetup = (Collectable_Setup*)self->setup;
    if (!self->def->collectableDef) {
        return;
    }
    
    //Set collection gamebit (if it's in use)
    if (objdata->gamebitCollected != NO_GAMEBIT) {
        main_set_bits(objdata->gamebitCollected, 1);
    }

    //Increment counter gamebit (if it's in use)
    if (objsetup->gamebitCount > 0) {
        main_set_bits(objsetup->gamebitCount, main_get_bits(objsetup->gamebitCount) + 1);
    }
    
    //Hide self
    self->srt.scale = self->def->scale;
    self->unkDC = 1;
}

// offset: 0xAF8 | func: 9 | export: 7
void SCcollectables_start_falling(Object* self) {
    SCCollectables_Data* objdata;

    objdata = self->data;
    objdata->fallFlags |= FLAG_Fall_Start;
}

// offset: 0xB10 | func: 10 | export: 8
void SCcollectables_copy_object_position(Object* self, Object* other) {
    self->srt.transl.x = other->srt.transl.x;
    self->srt.transl.y = other->srt.transl.y;
    self->srt.transl.z = other->srt.transl.z;
    self->positionMirror.x = other->positionMirror.x;
    self->positionMirror.y = other->positionMirror.y;
    self->positionMirror.z = other->positionMirror.z;
}
