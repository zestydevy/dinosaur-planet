#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/rand.h"
#include "types.h"

#include "dlls/objects/210_player.h"

typedef struct {
ObjSetup base;
s16 unused18;
s16 lifetime;
} Scarab_Setup;

typedef struct {
/*00*/ u32 soundHandle;         //Manages crawling sound loop
/*04*/ f32 speedX;              //Scurry speed
/*08*/ f32 speedZ;              //Scurry speed
/*0C*/ f32 goldClimbTimer;      //Amount of time Gold Scarab has been climbing wall
/*10*/ f32 initialY;            //Y value from objSetup - used as fallback height
/*14*/ s16 destructDelayTimer;  //Used to delay deletion when collected
/*16*/ s16 unused16;
/*18*/ s16 lifetime;            //Decrements while scurrying, Scarab disappears when 0
/*1A*/ s16 rollSpeed;           //Rate of change of roll
/*1C*/ s16 scurryInitialYaw;    //Reference yaw
/*1E*/ s16 stunTimer;           //Scarab can't continue scurrying while active. Rainbow Scarabs can only be collected while stunned!
/*20*/ s16 goldClimbDuration;   //Amount of time Gold Scarab should climb wall before stopping (randomised)
/*22*/ s16 collectSoundID;      //Sound to play when collected
/*24*/ s16 collectFXScale;      //Size of effect created when collected
/*26*/ u8 state;                //State machine value
/*27*/ s8 unused27;
/*27*/ s8 unused28;
/*29*/ u8 scarabTypeIndex;      //0) Green, 1) Red, 2) Gold, 3) Rainbow
/*2A*/ u8 flags;                //Receive messages from other objects
} Scarab_Data;

typedef enum {
    Scarab_STATE_Tumbling_Through_Air = 0,
    Scarab_STATE_Scurrying = 1,
    Scarab_STATE_Climb_Wall_Gold = 2 //Special Gold Scarab behaviour: climb vertically, can't be collected until shot down
} Scarab_States;

typedef enum {
    Scarab_FLAG_Wait_for_Message = 1
} Scarab_Flags;

typedef enum {
    Scarab_TYPE_Green = 0,
    Scarab_TYPE_Red = 1,
    Scarab_TYPE_Gold = 2,
    Scarab_TYPE_Rainbow = 3
} Scarab_Types;

/** Different sequenceIDs for each Scarab type to play when the player first collects a Scarab */
/*0x0*/ static s16 dSequenceIDs[4] = {
    0x0451, 0x0452, 0x0453, 0x0453
};

static Vec4f _bss_0;

static void scarab_collect(Object* self, Object* player, Scarab_Data* objData);
static void scarab_rotate_with_surface(Object* self, Func_80057F1C_Struct* arg1, u8 arg2, Unk80027934* arg3);
static s32 scarab_was_ground_reached(Object* self);

// offset: 0x0 | ctor
void scarab_ctor(void *dll) { }

// offset: 0xC | dtor
void scarab_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void scarab_setup(Object* self, Scarab_Setup* setup, s32 arg2) {
    Scarab_Data* objData = self->data;

    objData->state = Scarab_STATE_Tumbling_Through_Air;
    objData->lifetime = setup->lifetime;
    objData->rollSpeed = rand_next(1000, 4000);
    objData->goldClimbDuration = rand_next(50, 100);
    objData->initialY = setup->base.y;

    self->unkB0 |= 0x2000;

    switch (self->id) {
        case OBJ_Green_scarab:
            objData->collectSoundID = SOUND_B6E_Green_Scarab_Collected;
            objData->collectFXScale = 0x1FF;
            objData->scarabTypeIndex = Scarab_TYPE_Green;
            break;
        case OBJ_Red_scarab:
            objData->collectSoundID = SOUND_B6F_Red_Scarab_Collected;
            objData->collectFXScale = 0x79;
            objData->scarabTypeIndex = Scarab_TYPE_Red;
            break;
        case OBJ_Gold_scarab:
            objData->collectSoundID = SOUND_B70_Gold_Scarab_Collected;
            objData->collectFXScale = 0x1FE;
            objData->scarabTypeIndex = Scarab_TYPE_Gold;
            break;
        default:
        case OBJ_Rain_scarab:
            objData->collectSoundID = SOUND_B71_Rainbow_Scarab_Collected;
            objData->collectFXScale = 0x7B;
            objData->scarabTypeIndex = Scarab_TYPE_Rainbow;
            break;
    }

    obj_init_mesg_queue(self, 2);
}

// offset: 0x174 | func: 1 | export: 1
void scarab_control(Object* self) {
    Object* player;
    Scarab_Data* objData;
    s8 state;
    SRT transform;
    Func_80057F1C_Struct** collisionInfo = NULL;
    f32 magnitude;
    s32 count;
    s32 minIndex = 0;
    f32 pad;
    f32 minValue;
    s16 id;
    Vec3f pointNow = {0, 0, 0};
    Vec3f pointFuture = {0, 0, 0};
    s32 collided = 0;
    s32 i;
    s32 yaw;
    u32 messageID;
    AABBs32 aabb;
    Unk80027934 sp4C;
    
    objData = self->data;
    player = get_player();

    //Handle flags
    if (objData->flags & Scarab_FLAG_Wait_for_Message) {
        while (obj_recv_mesg(self, &messageID, 0, 0)){
            if (messageID == 0x7000B) {
                scarab_collect(self, player, objData);
                objData->flags &= ~Scarab_FLAG_Wait_for_Message;
            }
        } 
        
        if (objData->flags & Scarab_FLAG_Wait_for_Message) {
            return;
        }
    }
    
    //Handle deletion timer
    if (objData->lifetime == 0) {
        objData->destructDelayTimer -= gUpdateRate;
        if (objData->destructDelayTimer <= 0) {
            objData->destructDelayTimer = 0;
            obj_destroy_object(self);
        }
        return;
    }    

    //STATE MACHINE
    state = objData->state;
    if (state == Scarab_STATE_Tumbling_Through_Air) {
        func_8002674C(self);
        self->srt.transl.x += self->speed.x * gUpdateRateF;
        self->srt.transl.y += self->speed.y * gUpdateRateF;
        self->srt.transl.z += self->speed.z * gUpdateRateF;

        //Apply gravity, with terminal velocity
        if (self->speed.y > -15.0f) {
            self->speed.y += -0.06f * gUpdateRateF;
        }
        self->srt.roll += objData->rollSpeed * gUpdateRate;

        //Check for when scarab lands on terrain
        if (scarab_was_ground_reached(self)) {
            collided = TRUE;
        }

        //Advance state
        if (collided) {
            self->srt.roll = 0;
            objData->state = Scarab_STATE_Scurrying;
            objData->scurryInitialYaw = self->srt.yaw;
            
            id = self->id;
            if (id == OBJ_Green_scarab) {
                objData->speedX = self->speed.x * 0.15f;
                objData->speedZ = self->speed.z * 0.15f;
            } else if (id == OBJ_Red_scarab) {
                objData->speedX = self->speed.x * 0.65f;
                objData->speedZ = self->speed.z * 0.65f;
            } else if (id == OBJ_Gold_scarab) {
                objData->speedX = self->speed.x * 1.2f;
                objData->speedZ = self->speed.z * 1.2f;
            } else if (id == OBJ_Rain_scarab) {
                objData->speedX = self->speed.x * 0.45f;
                objData->speedZ = self->speed.z * 0.45f;
            } else if (id == OBJ_Blue_bean) { //Strangely "OBJ_Blue_bean" does use this DLL!
                objData->speedX = 0/*.0f*/;
                objData->speedZ = 0/*.0f*/;
            }
        }

        //Create particles while tumbling through air
        if (rand_next(0, 2) == 0) {
            gDLL_17_partfx->vtbl->spawn(self, 0x519, NULL, 1, -1, 0);
        }

    } else if ((state == Scarab_STATE_Climb_Wall_Gold) && (objData->lifetime != 0)) {
        //(Gold Scarabs only) Climb wall vertically for a little while 
        if (objData->goldClimbTimer < objData->goldClimbDuration) {
            self->srt.transl.y += 0.2f * gUpdateRateF;
            objData->goldClimbTimer += 0.2f * gUpdateRateF;
        }

        //Fall off when hit by Projectile Spell / Grenade collision
        if (func_80025F40(self, 0, 0, 0) == 15) {
            //Become stunned (delays scurry behaviour after landing)
            objData->stunTimer = 250;

            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_6BC_Creature_Cry, MAX_VOLUME, 0, 0, 0, 0);
            if (objData->soundHandle) {
                gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandle);
                objData->soundHandle = 0;
            }
            
            self->speed.x = player->srt.transl.x - self->srt.transl.x;
            self->speed.z = player->srt.transl.z - self->srt.transl.z;
            self->srt.yaw = 0;

            magnitude = SQ(self->speed.x) + SQ(self->speed.z);
            if (magnitude != 0.0f) {
                magnitude = sqrtf(magnitude);
            }

            self->srt.pitch = 0;
            self->speed.x /= 2.0f * magnitude;
            self->speed.z /= 2.0f * magnitude;
            self->speed.y = 2.2f;
            transform.roll = 0;
            transform.pitch = 0;
            transform.scale = 1.0f;
            transform.transl.x = 0;
            transform.transl.y = 0;
            transform.transl.z = 0;
            transform.yaw = rand_next(-10000, 10000);
            rotate_vec3(&transform, self->speed.f);
            yaw = self->srt.yaw - (u16)arctan2_f(self->speed.x, -self->speed.z);
            CIRCLE_WRAP(yaw)
            self->srt.yaw = yaw;
            objData->state = Scarab_STATE_Tumbling_Through_Air;
            objData->goldClimbTimer = 0;
        }

    } else if ((state == Scarab_STATE_Scurrying) && (objData->lifetime != 0)) {
        //Not stunned, can move
        if (objData->stunTimer == 0) { 
            //Play scurrying sound
            if (objData->soundHandle == 0) {
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_669_Insect_Scurry_Loop, 0x39, &objData->soundHandle, 0, 0, 0);
            }
            
            //Find nearest ground distance
            minValue = 10000.0f;
            count = func_80057F1C(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &collisionInfo, 1, 0);
            for (i = 0; i < count; i++){
                magnitude = collisionInfo[i]->unk0[0] - self->srt.transl.y;
                if (magnitude < 0.0f) {
                    magnitude *= -1.0f;
                }
                if (magnitude < minValue) {
                    minIndex = i;
                    minValue = magnitude;
                }
            }
            
            //Stick to ground
            if (collisionInfo != NULL) {
                self->srt.transl.y = collisionInfo[minIndex]->unk0[0];
                scarab_rotate_with_surface(self, collisionInfo[minIndex], 1, &sp4C);
            } else {
                self->srt.transl.y = objData->initialY;
            }
            
            //Random yaw/roll jitter (in Rainbow Scarab's case, go towards player as well)
            if (self->id == OBJ_Rain_scarab) {
                self->srt.yaw = arctan2_f(player->srt.transl.x - self->srt.transl.x, player->srt.transl.z - self->srt.transl.z) + 0x7FFF;
                self->srt.yaw += rand_next(-1460, 1460);
            } else {
                self->srt.yaw += rand_next(-1460, 1460);
            }
            self->srt.roll += rand_next(-1000, 1000);
            
            self->speed.x = objData->speedX;
            self->speed.y = 0/*.0f*/;
            self->speed.z = objData->speedZ;
            
            transform.scale = 1.0f;
            transform.transl.x = 0/*.0f*/;
            transform.transl.y = 0/*.0f*/;
            transform.transl.z = 0/*.0f*/;
            transform.roll = 0;
            transform.pitch = 0;
            transform.yaw = self->srt.yaw - objData->scurryInitialYaw;
            rotate_vec3(&transform, (f32*)&self->speed);
            
            //Handle lifetime timer
            objData->lifetime -= gUpdateRate;
            if (objData->lifetime <= 0) {
                objData->lifetime = 0;
            }
            
            //Check for wall collisions
            pointFuture.x = self->srt.transl.x + (self->speed.x * gUpdateRateF * 1.5f);
            pointFuture.y = self->srt.transl.y + 30.0f;
            pointFuture.z = self->srt.transl.z + (self->speed.z * gUpdateRateF * 1.5f);
            pointNow.x = self->srt.transl.x;
            pointNow.y = self->srt.transl.y + 30.0f;
            pointNow.z = self->srt.transl.z;
            sp4C.unk40[0] = 0;
            sp4C.unk50[0] = -1;
            sp4C.unk54[0] = 0;
            
            fit_aabb_around_cubes(&aabb, &pointNow, &pointFuture, sp4C.unk40, 1);
            func_80053750(self, &aabb, 0);
            collided = func_8005509C(self, (f32*)&pointNow, (f32*)&pointFuture, 1, &sp4C, 0);
            
            //Gold Scarab: start climbing a wall
            if (collided && (self->id == OBJ_Gold_scarab)) {
                objData->state = Scarab_STATE_Climb_Wall_Gold;
                objData->goldClimbTimer = 0;
                scarab_rotate_with_surface(self, NULL, 0, &sp4C);
            }
            
            //Continue moving
            if (!collided) {
                self->srt.transl.x += self->speed.x * gUpdateRateF;
                self->srt.transl.z += self->speed.z * gUpdateRateF;
            }
        } else {
            //STUNNED STATE (Gold Scarabs can't move for a while after being shot onto ground)

            //Find nearest ground distance
            minValue = 10000.0f;
            count = func_80057F1C(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &collisionInfo, 1, 0);
            for (i = 0; i < count; i++){
                magnitude = collisionInfo[i]->unk0[0] - self->srt.transl.y;
                if (magnitude < 0.0f) {
                    magnitude *= -1.0f;
                }
                if (magnitude < minValue) {
                    minIndex = i;
                    minValue = magnitude;
                }
            }
            
            //Stick to ground
            if (collisionInfo != NULL) {
                self->srt.transl.y = collisionInfo[minIndex]->unk0[0];
                scarab_rotate_with_surface(self, collisionInfo[minIndex], 1, &sp4C);
            } else {
                self->srt.transl.y = objData->initialY;
            }
            
            //Run down timer until can scurry freely
            objData->stunTimer -= gUpdateRate;
            if (objData->stunTimer <= 0) {
                objData->stunTimer = 0;
            }
        }

        //Collect scurrying Scarab when player is nearby (Rainbow Scarabs must also be stunned)
        if ((objData->stunTimer || (self->id != OBJ_Rain_scarab)) && (vec3_distance_xz(&player->positionMirror, &self->positionMirror) < 25.0f)) {
            //Play an item collection sequence the first time a Scarab is collected
            if (!main_get_bits(BIT_Tutorial_Collected_Scarab)) {
                gDLL_3_Animation->vtbl->func30(dSequenceIDs[objData->scarabTypeIndex], 0, 0);
                messageID = 0;
                obj_send_mesg(player, 0x7000A, self, 0);
                main_set_bits(BIT_Tutorial_Collected_Scarab, 1);
                objData->flags |= 1;
            } else {
                scarab_collect(self, player, objData);
            }
            
            //Remove objHits, play sound, create effects
            func_800267A4(self);
            gDLL_6_AMSFX->vtbl->play_sound(self, objData->collectSoundID, MAX_VOLUME, 0, 0, 0, 0);
            transform.scale = objData->collectFXScale;
            gDLL_17_partfx->vtbl->spawn(self, 0x51A, &transform, 1, -1, 0);
            gDLL_17_partfx->vtbl->spawn(self, 0x51A, &transform, 1, -1, 0);
            gDLL_17_partfx->vtbl->spawn(self, 0x51A, &transform, 1, -1, 0);
            
            //Stop scurrying sound loop
            if (objData->soundHandle) {
                gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandle);
                objData->soundHandle = 0;
            }
        }

        //Rainbow Scarab: attack behaviour (when not stunned)
        if ((objData->stunTimer == 0) && (self->id == OBJ_Rain_scarab)) {
            //Colliding with player
            if (vec3_distance_xz(&player->positionMirror, &self->positionMirror) < 20.0f) {
                //Hurt player (unless special Rainbow Scarab immunity gamebit is set!)
                if (main_get_bits(BIT_Player_Immune_to_Rainbow_Scarabs) == 0) {
                    obj_send_mesg(player, 0x60004, self, (void*)1);
                }

                //Jolt backwards slightly
                self->srt.transl.x += (-self->speed.x * 26.0f);
                self->srt.transl.z += (-self->speed.z * 26.0f);
                
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_6BB_Creature_Cry, MAX_VOLUME, 0, 0, 0, 0);
            }

            //Become stunned when hit by Projectile Spell / Grenade
            if (func_80025F40(self, 0, 0, 0) == 15) {
                objData->stunTimer = 250;
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_6BC_Creature_Cry, MAX_VOLUME, 0, 0, 0, 0);
                if (objData->soundHandle) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandle);
                    objData->soundHandle = 0;
                }
            }

        //Rainbow Scarab: handle stunned behaviour
        } else if ((objData->stunTimer) && (self->id == OBJ_Rain_scarab)){
            //Check for a follow-up Projectile Spell / Grenade attack while stunned
            if (func_80025F40(self, 0, 0, 0) == 15) {
                gDLL_17_partfx->vtbl->spawn(self, 0x51A, NULL, 1, -1, 0);
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_6BD_Creature_Death_Cry, MAX_VOLUME, 0, 0, 0, 0);
                if (objData->soundHandle) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandle);
                    objData->soundHandle = 0;
                }

                //Collect Rainbow Scarab regardless of player distance
                scarab_collect(self, player, objData);
            }
        }
    }

}

// offset: 0x1164 | func: 2 | export: 2
void scarab_update(Object *self) { }

// offset: 0x1170 | func: 3 | export: 3
void scarab_print(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    Scarab_Data* objData = self->data;

    if (objData->destructDelayTimer) {
        return;
    }

    if (self->unkE0) {
        if (visibility != -1) {
            return;
        }
    } else if (!visibility){
        return;
    }

    draw_object(self, gfx, mtx, vtx, pols, 1.0f);
}

// offset: 0x11F8 | func: 4 | export: 4
void scarab_free(Object* self, s32 arg1) {
    Scarab_Data* objData;

    objData = self->data;
    if (objData->soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandle);
        objData->soundHandle = 0;
    }
}

// offset: 0x1260 | func: 5 | export: 5
u32 scarab_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1270 | func: 6 | export: 6
u32 scarab_get_data_size(Object *self, u32 a1) {
    return sizeof(Scarab_Data);
}

// offset: 0x1284 | func: 7
void scarab_collect(Object* self, Object* player, Scarab_Data* objData) {
    /** The amount of currency each Scarab type adds to the player's counter */
    u8 values[4] = {1, 5, 10, 50};

    ((DLL_210_Player*)player->dll)->vtbl->add_scarab(player, values[objData->scarabTypeIndex]);

    objData->destructDelayTimer = 80;
    objData->lifetime = 0;
}

// offset: 0x1304 | func: 8
void scarab_rotate_with_surface(Object* self, Func_80057F1C_Struct* arg1, u8 arg2, Unk80027934* arg3) {
    SRT transform;
    f32 speed;
    Vec3f v;
    u32 angle;
    f32 doubleSpeed;
    Scarab_Data *objData;

    objData = self->data;

    if (arg2 == 1){
        v.x = arg1->unk0[1];
        v.y = arg1->unk0[2];
        v.z = arg1->unk0[3];
    }

    else if (arg2 == 0){
        v.x = arg3->unk0->x;
        v.y = arg3->unk0->y;
        v.z = arg3->unk0->z;
    }

    else if (arg2 == 2){
        self->speed.x = arg3->unk0->x;
        self->speed.z = arg3->unk0->z;

        speed = (self->speed.x * self->speed.x) + (self->speed.z * self->speed.z);
        if (speed){
            speed = sqrtf(speed);
        }
        doubleSpeed = 2.0f * speed;

        self->speed.x /= doubleSpeed;
        self->speed.z /= doubleSpeed;
        objData->speedX = self->speed.x;
        objData->speedZ = self->speed.z;
        self->srt.yaw = arctan2_f(-arg3->unk0->x, -arg3->unk0->z);
        return;
    }

    transform.transl.x = 0.0f;
    transform.transl.y = 0.0f;
    transform.transl.z = 0.0f;
    transform.scale = 1.0f;
    transform.roll = 0;
    transform.pitch = 0;
    transform.yaw = self->srt.yaw;
    rotate_vec3(&transform, v.f);

    if (arg1){
        angle = arctan2_f(v.x, v.y);
        self->srt.pitch = arctan2_f(v.z, v.y);
        self->srt.roll = angle;
        return;
    }

    self->srt.roll = 0;
    self->srt.pitch = arctan2_f(arg3->unk0->z + arg3->unk0->x, arg3->unk0->y);
    if (self->srt.pitch < 0){
        self->srt.pitch = -self->srt.pitch;
    }
    self->srt.yaw = arctan2_f(arg3->unk0->x, arg3->unk0->z);
    return;
}

// offset: 0x1544 | func: 9
s32 scarab_was_ground_reached(Object* self) {
    Vec3f spF0[4];
    Vec3f spC0[4];
    Unk80027934 sp54;
    AABBs32 sp3C;
    ObjectHitInfo* objHits;
    u8 i;
    f32* temp;

    objHits = self->objhitInfo;
    if (objHits != NULL) {
        spF0[0].x = self->srt.transl.x;
        spF0[0].y = self->srt.transl.y;
        spF0[0].z = self->srt.transl.z;
        spC0[0].x = self->positionMirror2.x;
        spC0[0].y = self->positionMirror2.y;
        spC0[0].z = self->positionMirror2.z;
        sp54.unk40[0] = objHits->unk52;
        sp54.unk50[0] = -1;
        sp54.unk54[0] = 3;
    } else {
        return FALSE;
    }
        
    fit_aabb_around_cubes(&sp3C, spC0, spF0, sp54.unk40, 1);
    func_80053750(self, &sp3C, objHits->unkA1);
    i = func_8005509C(self, (f32*)&spC0, (f32*)&spF0, 1, &sp54, 0);
    if (i) {
        if (i & 1) {
            i = 0;
        } else if (i & 2) {
            i = 1;
        } else if (i & 4) {
            i = 2;
        } else {
            i = 3;
        }

        objHits->unk9C = sp54.unk50[i];
        temp = (f32*)spF0;
        objHits->unk34 = temp[i * 3 + 0];
        objHits->unk38 = temp[i * 3 + 1];
        objHits->unk3C = temp[i * 3 + 2];

        _bss_0.x = sp54.unk0[i].x;
        _bss_0.y = sp54.unk0[i].y;
        _bss_0.z = sp54.unk0[i].z;
        _bss_0.w = sp54.unk0[i].w;
        
        if (sp54.unk58[i] != 0) {
            objHits->unk9D |= 2;
            self->srt.transl.x = objHits->unk34;
            self->srt.transl.y = objHits->unk38;
            self->srt.transl.z = objHits->unk3C;
            objHits->unk10.x = self->positionMirror2.x;
            objHits->unk10.y = self->positionMirror2.y;
            objHits->unk10.z = self->positionMirror2.z;
            return TRUE;
        } else {
            objHits->unk9D |= 1;
            self->srt.transl.x = objHits->unk34;
            self->srt.transl.y = objHits->unk38;
            self->srt.transl.z = objHits->unk3C;
            objHits->unk10.x = self->positionMirror2.x;
            objHits->unk10.y = self->positionMirror2.y;
            objHits->unk10.z = self->positionMirror2.z;
            return TRUE;
        }
    } else {
        return FALSE;
    }
}

/*0x0*/ static const char str_0[] = "fy %f %f\n";
/*0xC*/ static const char str_C[] = "Crashed\n";
