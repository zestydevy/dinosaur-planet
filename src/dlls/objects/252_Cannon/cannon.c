#include "common.h"
#include "dlls/engine/17_partfx.h"
#include "dlls/engine/6_amsfx.h"
#include "sys/gfx/model.h"
#include "sys/objmsg.h"

#include "dlls/objects/253_CannonBall.h"

typedef struct {
    ObjSetup base;
    u8 yaw;
    u8 unk19;       //Unknown purpose. Maybe meant as a lifetime for the cannon or its cannonballs?
} Cannon_Setup;

typedef struct {
    f32 unk0;                         //Set to 0 at when changing states, but otherwise unused
    Vec3f basePosition;               //Initial lowered position, under the Galleon's decks
    s16 unused10; 
    s16 timer;                        //Delay before rising/firing/lowering, and the number of frames to rise for
    s16 loweringTimer;                //The number of frames to lower for
    s16 state;                        //State Machine value
    u8 unk18;                         //Set to 0 in setup and used for a setup condition - unknown purpose
    u8 unk19;                         //Stores a copy of Cannon_Setup->unk19, otherwise unused
} Cannon_Data;

typedef enum {
    Cannon_STATE_Lowered = 0,         //Cannon hidden below deck
    Cannon_STATE_Rising = 1,          //Cannon rising up to deck level
    Cannon_STATE_Waiting_to_Fire = 2, //Cannon pausing briefly before firing (vulnerable to attack)
    Cannon_STATE_Firing = 3,          //Cannon firing a cannonball, shaking the screen
    Cannon_STATE_Lowering = 4         //Brief pause (vulnerable to attack), then cannon goes back below deck 
} Cannon_States;

/** Delay before the cannon first rises up from below deck */
/*0x0*/ static s16 dInitialPause = 70;

/** 
  * Stores the last Object the cannon was hit by (but only during the "lowering" state).
  *
  * Maybe intended to store a reference to the cannon's own CannonBall object to avoid self-collisions, but implemented incorrectly? 
  */
/*0x4*/ static Object* dObjectRef = NULL;
                                            

// offset: 0x0 | ctor
void Cannon_ctor(void *dll) { }

// offset: 0xC | dtor
void Cannon_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void Cannon_setup(Object* self, Cannon_Setup* objSetup, s32 arg2) {
    Cannon_Data* objData = self->data;
    
    self->srt.yaw = (objSetup->yaw << 4) << 4;
    self->srt.transl.y -= 100.0f; //start off underground
    self->unkDC = objSetup->unk19;
    
    objData->unk18 = 0;

    if (objData->unk18 == 0) { //NOTE: always true
        objData->timer = dInitialPause;
        dInitialPause = 70 - dInitialPause;
        
        objData->unk19 = objSetup->unk19;
        objData->state = Cannon_STATE_Lowered;
        objData->unk0 = 0.0f;

        //Store initial lowered position
        objData->basePosition.x = self->srt.transl.x;
        objData->basePosition.y = self->srt.transl.y;
        objData->basePosition.z = self->srt.transl.z;
    }
}

// offset: 0xB4 | func: 1 | export: 1
void Cannon_control(Object* self) {
    Object* obj;
    s32 pad;
    ObjSetup* cannonballSetup;
    f32 x;
    f32 y;
    f32 z;
    s16 yaw;
    Cannon_Data* objData;

    objData = self->data;
    
    diPrintf("CANNON\n");

    switch (objData->state) {
    case Cannon_STATE_Lowered:
        //Cannon waiting below deck
        if (objData->timer >= 0) {
            objData->timer--; //@bug: framerate dependent
            return;
        }

        objData->timer = 30;
        objData->state = Cannon_STATE_Rising;
        objData->unk0 = 0.0f;

        self->srt.transl.x = objData->basePosition.x;
        self->srt.transl.y = objData->basePosition.y;
        self->srt.transl.z = objData->basePosition.z;
        return;
    case Cannon_STATE_Rising:
        //Rising up to deck level
        if (objData->timer >= 0) {
            self->srt.transl.x = self->srt.transl.x;
            self->srt.transl.y += 2.2f; //@bug: framerate dependent
            self->srt.transl.z = self->srt.transl.z;
            objData->timer--; //@bug: framerate dependent
            return;
        }

        //Preparing to fire
        objData->state = Cannon_STATE_Waiting_to_Fire;
        objData->timer = 20;
        objData->unk0 = 0.0f;
        return;
    default:
        return;
    case Cannon_STATE_Waiting_to_Fire:
        //Brief pause before cannon fire, vulnerable to attacks
        if (objData->timer >= 0) {
            objData->timer--;

            //Return early if no collision has occurred
            if (!self || !self->objhitInfo || (self->objhitInfo->unk50 >= 70)) {
                break;
            }

            //Do nothing if the collided object is the referenced object, or not an attack
            obj = self->objhitInfo->unk0;
            if (obj == dObjectRef || obj->group != GROUP_UNK41){
                break;
            }

            if (self->srt.yaw & 0x8000) {
                gDLL_3_Animation->vtbl->func25(gDLL_3_Animation->vtbl->func24() - 1);
dummy_label1: ;
            } else {
                gDLL_3_Animation->vtbl->func27(gDLL_3_Animation->vtbl->func26() - 1);
            }
            
            //Create explosion of debris
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_9, NULL, 4, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_5, NULL, 4, -1, NULL);
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_18, MAX_VOLUME, NULL, NULL, 0, NULL);

            //Send an object message, then destroy both the attacking object and the cannon
            obj_send_mesg_many(GROUP_UNK54, 0, 0, 0xE0000, self);
            obj_destroy_object(obj);
            obj_destroy_object(self);
            return;

        //Fire a cannonball once the timer runs down
        } else {
            objData->timer = 0;
            objData->state = Cannon_STATE_Firing;
            return;
        }
        break;
    case Cannon_STATE_Firing:
        //Fire a cannonball
        get_object_child_position(self, &x, &y, &z);
        cannonballSetup = obj_alloc_setup(sizeof(CannonBall_Setup), OBJ_CannonBall);
        cannonballSetup->x = x;
        cannonballSetup->y = y + 30.0f;
        cannonballSetup->z = z;
        obj = obj_create(cannonballSetup, 5, -1, -1, NULL);

        //Get the cannon's worldSpace yaw
        yaw = self->srt.yaw;
        if (self->parent != NULL) {
            yaw += self->parent->srt.yaw;
        }
        yaw += 0x4000;

        //Set the cannonball's lateral velocity and position offset based on the cannon's yaw
        obj->velocity.f[0] = fsin16_precise(yaw) * 7.0f;
        obj->velocity.f[2] = fcos16_precise(yaw) * 7.0f;
        obj->srt.transl.f[0] += fsin16_precise(yaw) * 80.0f;
        obj->srt.transl.f[2] += fcos16_precise(yaw) * 80.0f;

        //Set the cannonball's max lifetime
        obj->unkDC = 300;

        //Shake the screen
        camera_enable_y_offset();
        camera_set_shake_offset(0.9f);

        //Create blast sound/particle effects
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_69, NULL, 4, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_2, NULL, 4, -1, NULL);
        gDLL_6_AMSFX->vtbl->play(NULL, SOUND_96_Cannon, MAX_VOLUME, NULL, NULL, 0, NULL);
    
        objData->state = Cannon_STATE_Lowering;
        objData->timer = 20;
        objData->loweringTimer = 30;
        objData->unk0 = 0.0f;
        return;
    case Cannon_STATE_Lowering:
        //Brief pause before going below deck, vulnerable to attacks
        if (objData->timer >= 0) {
            objData->timer--;

            //Return early if no collision has occurred
            if (!self || !self->objhitInfo || (self->objhitInfo->unk50 >= 70)) {
                break;
            }

            //Do nothing if the collided object is the referenced object, or not an attack
            obj = self->objhitInfo->unk0;
            if (obj == dObjectRef || (obj->group != GROUP_UNK41)) {
                break;
            }

            if (self->srt.yaw & 0x8000) {
                gDLL_3_Animation->vtbl->func25(gDLL_3_Animation->vtbl->func24() - 1);
dummy_label3: ;
            } else {
                gDLL_3_Animation->vtbl->func27(gDLL_3_Animation->vtbl->func26() - 1);
            }
            
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_9, NULL, 4, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_5, NULL, 4, -1, NULL);
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_18, MAX_VOLUME, NULL, NULL, 0, NULL);

            //@bug? Possibly meant for `Cannon_STATE_Firing`, so the cannon doesn't get destroyed by its own attack?
            dObjectRef = obj;

            //Send an object message, then destroy both the attacking object and the cannon
            obj_send_mesg_many(GROUP_UNK54, 0, 0, 0xE0000, self);
            obj_destroy_object(obj);
            obj_destroy_object(self);
            return;

        //Go back down below deck once the timer runs down
        } else {
            //Moving down
            if (objData->loweringTimer >= 0) {
                self->srt.transl.x = self->srt.transl.x;
                self->srt.transl.y -= 2.2f; //@bug: framerate dependent
                self->srt.transl.z = self->srt.transl.z;
                objData->loweringTimer--; //@bug: framerate dependent
                return;
            }

            objData->state = Cannon_STATE_Lowered;
            objData->timer = 20;
            objData->loweringTimer = 0;
            objData->unk0 = 0.0f;
            return;
        }
        break;
    }
}

// offset: 0x888 | func: 2 | export: 2
void Cannon_update(Object *self) { }

// offset: 0x894 | func: 3 | export: 3
void Cannon_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x8E8 | func: 4 | export: 4
void Cannon_free(Object* arg0, s32 arg1) {
    gDLL_13_Expgfx->vtbl->func5(arg0);
}

// offset: 0x930 | func: 5 | export: 5
u32 Cannon_get_model_flags(Object *self){
    return MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x940 | func: 6 | export: 6
u32 Cannon_get_data_size(Object *self, u32 a1){
    return sizeof(Cannon_Data);
}
