#include "common.h"
#include "sys/objtype.h"
#include "sys/objanim.h"

s32 func_80024108(Object*, f32, f32 updateRate, UnkFunc_80024108Struct*);

typedef struct {
    ObjSetup base;
    u16 unk18;                  //Unused
    u8 yaw;
} CCcage_Setup;

typedef struct {
    u8 timesHit;                //How many times cage's been hit with Projectile Spell
    u8 state;
    u8 pitchAcceleration;
    u8 rollAcceleration;
    f32 pitchSpeed;
    f32 rollSpeed;
    f32 pitch;
    f32 roll;
    Object* children[2];        //Objects inside cage: CClightfoot, CCspellpage
    Vec3f childPositions[2];    //Local positions of child objects inside cage
    f32 fallTimer;
    f32 fallDuration;
    f32 unk3C;
} CCcage_Data;

typedef enum {
    STATE_Dangling = 0,
    STATE_Already_On_Ground = 1,
    STATE_Falling = 2,
    STATE_Finished = 3
} CCcage_States;

static void CCcage_transform_child(Object* self, s32 index);
static void CCcage_hit_ground(Object* self);
static void CCcage_impact_sway(Object* self);

// offset: 0x0 | ctor
void CCcage_ctor(void *dll) { }

// offset: 0xC | dtor
void CCcage_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCcage_setup(Object* self, CCcage_Setup* objSetup, s32 arg2) {
    CCcage_Data* objData;

    objData = self->data;
    
    objData->pitchAcceleration = 2;
    objData->pitch = 0.0f;
    objData->rollAcceleration = 2;
    objData->roll = 0.0f;
    objData->pitchSpeed = 6.0f;
    objData->rollSpeed = 7.0f;
    
    self->srt.yaw = objSetup->yaw << 8;
    
    //Handle initial state
    if (main_get_bits(BIT_CC_Shot_Down_Lightfoot_Cage)) {
        objData->state = STATE_Already_On_Ground;
    } else {
        objData->state = STATE_Dangling;
    }
    
    objData->unk3C = 0.0f;
    
    self->unkB0 |= 0x6000;
}

// offset: 0xD8 | func: 1 | export: 1
void CCcage_control(Object* self) {
    f32 searchDistance;
    Object* nearbyObject;
    f32 tValue;
    s16 index;
    CCcage_Setup* objSetup;
    CCcage_Data* objData;

    searchDistance = 100;
    objData = self->data;
    objSetup = (CCcage_Setup*)self->setup;
    
    //Look for nearby CClightfoot
    if (objData->children[0] == NULL) {
        nearbyObject = obj_get_nearest_type_to(0x28, self, &searchDistance);
        objData->children[0] = nearbyObject;
        if (!nearbyObject) {
            STUBBED_PRINTF("can't find lightfoot\n");
            return;
        }
        objData->childPositions[0].x = nearbyObject->srt.transl.f[0] - self->srt.transl.f[0];
        objData->childPositions[0].y = nearbyObject->srt.transl.f[1] - self->srt.transl.f[1];
        objData->childPositions[0].z = nearbyObject->srt.transl.f[2] - self->srt.transl.f[2];
    }

    //Look for nearby CCspellpage
    if (objData->children[1] == NULL) {
        nearbyObject = obj_get_nearest_type_to(5, self, &searchDistance);
        objData->children[1] = nearbyObject;
        if (!nearbyObject) {
            STUBBED_PRINTF("can't find lightfoot\n"); //might've meant to say "SpellPage"
            return;
        }
        objData->childPositions[1].x = nearbyObject->srt.transl.f[0] - self->srt.transl.f[0];
        objData->childPositions[1].y = nearbyObject->srt.transl.f[1] - self->srt.transl.f[1];
        objData->childPositions[1].z = nearbyObject->srt.transl.f[2] - self->srt.transl.f[2];
    }

    //State Machine
    switch (objData->state) {
    case STATE_Finished:
        break;
    case STATE_Already_On_Ground:
        CCcage_hit_ground(self);
        objData->unk3C = 1.0f;
        if (main_get_bits(BIT_Play_Seq_022F_CC_Lightfoot_Gives_Spellpage) == FALSE) {
            for (index = 0; index < 2; index++){
                CCcage_transform_child(self, index);
            }
            
            //Play sequence
            gDLL_3_Animation->vtbl->func17(0, objData->children[0], -1);
        }
        objData->state = STATE_Finished;
        break;
    case STATE_Dangling:
        //Sway gently
        if (self->srt.pitch <= 0) {
            objData->pitchSpeed += (objData->pitchAcceleration * gUpdateRateF) / 10.0f;
        } else {
            objData->pitchSpeed -= (objData->pitchAcceleration * gUpdateRateF) / 10.0f;
        }
        self->srt.pitch += objData->pitchSpeed * gUpdateRateF;
        
        if (self->srt.roll <= 0) {
            objData->rollSpeed += (objData->rollAcceleration * gUpdateRateF) / 10.0f;
        } else {
            objData->rollSpeed -= (objData->rollAcceleration * gUpdateRateF) / 10.0f;
        }
        self->srt.roll += objData->rollSpeed * gUpdateRateF;
        
        if ((objData->pitch < 0.0f) && (self->srt.pitch >= 0)) {
            if (objData->pitchAcceleration >= 3) {
                objData->pitchAcceleration -= 2;
                objData->pitchSpeed = (objData->pitchSpeed * 83.0f) / 100.0f;
            } else {
                self->srt.pitch = -objData->pitch;
                objData->pitchSpeed = 6.0f;
            }
        } else if ((objData->pitch > 0.0f) && (self->srt.pitch <= 0)) {
            if (objData->pitchAcceleration >= 3) {
                objData->pitchAcceleration -= 2;
                objData->pitchSpeed = (objData->pitchSpeed * 82.0f) / 100.0f;
            } else {
                self->srt.pitch = -objData->pitch;
                objData->pitchSpeed = -6.0f;
            }
        }
        objData->pitch = self->srt.pitch;
        
        if ((objData->roll < 0.0f) && (self->srt.roll >= 0)) {
            if (objData->rollAcceleration >= 3) {
                objData->rollAcceleration -= 2;
                objData->rollSpeed = (objData->rollSpeed * 82.0f) / 100.0f;
            } else {
                self->srt.roll = -objData->roll;
                objData->rollSpeed = 7.0f;
            }
        } else if ((objData->roll > 0.0f) && (self->srt.roll <= 0)) {
            if (objData->rollAcceleration >= 3) {
                objData->rollSpeed = (objData->rollSpeed * 81.0f) / 100.0f;
                objData->rollAcceleration -= 2;
            } else {
                self->srt.roll = -objData->roll;
                objData->rollSpeed = -7.0f;
            }
        }
        objData->roll = self->srt.roll;
        
        //Check for Projectile Spell collision
        if (func_80025F40(self, NULL, NULL, NULL) == 0xF) {
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_LightFoot_Shout, 0x7F, NULL, NULL, 0, NULL);
            if (objData->timesHit != 0) {
                objData->fallDuration = 35.355f;
                objData->fallTimer = 0;
                objData->unk3C = 0.03f;
                objData->state = STATE_Falling;
            } else {
                objData->timesHit = 1;
                CCcage_impact_sway(self);
            }
        }

        //Update child objects
        for (index = 0; index < 2; index++){
            CCcage_transform_child(self, index);
        }
        break;
    case STATE_Falling:
        objData->fallTimer += gUpdateRateF;
        if (objData->fallTimer >= objData->fallDuration) {
            //Hit ground
            CCcage_hit_ground(self);

            for (index = 0; index < 2; index++){
                CCcage_transform_child(self, index);
            }
            
            //Play sequence: cage door opening and LightFoot hopping out
            main_set_bits(BIT_CC_Shot_Down_Lightfoot_Cage, TRUE);
            gDLL_3_Animation->vtbl->func17(0, objData->children[0], -1);
            objData->state = STATE_Finished;
        } else {
            //Continue falling
            tValue = objData->fallTimer / objData->fallDuration;
            self->srt.transl.y = objSetup->base.y + (-0.04f * objData->fallTimer * objData->fallTimer);
            self->srt.pitch = objData->pitch - (objData->pitch * tValue);
            self->srt.roll = objData->roll - (objData->roll * tValue);

            for (index = 0; index < 2; index++){
                CCcage_transform_child(self, index);
            }
        }
        break;
    }

    func_80024108(self, objData->unk3C, gUpdateRateF, 0);
}

// offset: 0x8A4 | func: 2 | export: 2
void CCcage_update(Object *self) { }

// offset: 0x8B0 | func: 3 | export: 3
void CCcage_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x904 | func: 4 | export: 4
void CCcage_free(Object *self, s32 a1) { }

// offset: 0x914 | func: 5 | export: 5
u32 CCcage_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x924 | func: 6 | export: 6
u32 CCcage_get_data_size(Object *self, u32 a1) {
    return sizeof(CCcage_Data);
}

// offset: 0x938 | func: 7
void CCcage_transform_child(Object* self, s32 index) {
    CCcage_Data* objData;
    s16 transform[3];
    f32 position[3];

    objData = self->data;
    
    transform[0] = 0;
    transform[1] = self->srt.pitch;
    transform[2] = self->srt.roll;
    
    position[0] = objData->childPositions[index].x;
    position[1] = objData->childPositions[index].y;
    position[2] = objData->childPositions[index].z;
    
    rotate_vec_inv((SRT*)&transform, (Vec3f*)&position);
    
    objData->children[index]->srt.transl.x = position[0] + self->srt.transl.x;
    objData->children[index]->srt.transl.y = position[1] + self->srt.transl.y;
    objData->children[index]->srt.transl.z = position[2] + self->srt.transl.z;
    objData->children[index]->srt.pitch = -self->srt.pitch;
    objData->children[index]->srt.roll = -self->srt.roll;
}

// offset: 0xA34 | func: 8
void CCcage_hit_ground(Object* self) {
    CCcage_Data* objData;
    ObjSetup *setup;
    SRT transform;
    s32 i;
    s32 j;

    setup = self->setup;
    objData = self->data;
    
    //Position cage at ground level
    self->srt.transl.y = setup->y - 50.0f;
    self->srt.pitch = 0;
    self->srt.roll = 0;
    
    //Create dust cloud effects (9 of them: at base's corners, centre, and edge midpoints)
    transform.transl.y = objData->children[0]->srt.transl.y - self->srt.transl.y;
    for (i = -1; i < 2; i++){
        for (j = -1; j < 2; j++){
            transform.transl.x = i * 14.0f;
            transform.transl.z = j * 14.0f;
            gDLL_17_partfx->vtbl->spawn(self, 0x3DE, &transform, 0, -1, NULL);
        }  
    }    
}

// offset: 0xB5C | func: 9
void CCcage_impact_sway(Object* self) {
    f32 positionDelta[2];
    CCcage_Data *objData;
    Object *player;
    
    player = get_player();
    objData = self->data;
    
    positionDelta[0] = player->srt.transl.x - self->srt.transl.x;
    positionDelta[1] = player->srt.transl.z - self->srt.transl.z;
    
    if (positionDelta[0] > 0){
        self->srt.pitch += 0x1000;
    } else {
        self->srt.pitch -= 0x1000;
    }
    
    if (positionDelta[1] > 0){
        self->srt.roll += 0x1000;
    } else {
        self->srt.roll -= 0x1000;
    }
    
    objData->pitchAcceleration *= 20;
    objData->rollAcceleration *= 20;
}
