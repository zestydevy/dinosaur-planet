#include "common.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/685_DBbonedust.h"

// offset: 0x0 | ctor
void DBBoneDust_ctor(void *dll) { }

// offset: 0xC | dtor
void DBBoneDust_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBBoneDust_setup(Object* self, DBBoneDust_Setup* objSetup, s32 arg2) {
    DBBoneDust_Data* objData;

    objData = self->data;
    objData->state = DBBoneDust_STATE_Hidden;
    objData->baseY = objSetup->base.y;
    self->srt.transl.x = objSetup->base.x;
    self->srt.transl.y = objSetup->base.y;
    self->srt.transl.z = objSetup->base.z;
    self->speed.x = 0.0f;
    self->speed.z = 0.0f;
    self->speed.y = -4.0f;
    objData->rotation.asWord = rand_next(0, 0xFFFF);
}

// offset: 0xB0 | func: 1 | export: 1
void DBBoneDust_control(Object* self) {
    DBBoneDust_Data* objData;
    Object* player;
    f32 var_fv1;
    Vec3f uPlayer;
    f32 sp60;
    f32 absoluteSpeed;
    SRT fxTrans;
    f32 distance;
    f32 height;

    objData = self->data;
    player = get_player();

    switch (objData->state) {
    case DBBoneDust_STATE_Hidden:
        break;
    case DBBoneDust_STATE_Launched: 
        //Flying into air after being dug up by Tricky
        height = objData->baseY - self->srt.transl.y; //negative value when in air, positive when under ground
        if (height > 0) {
            //Bounce when hitting ground, losing some momentum
            self->speed.y = -self->speed.y * 0.7f;
            if (self->speed.y >= 0.0f) {
                absoluteSpeed = self->speed.y;
            } else {
                absoluteSpeed = -self->speed.y;
            }
            
            //Start floating when nearly stopped
            if (absoluteSpeed < 0.1f) {
                objData->state = DBBoneDust_STATE_Hovering;
                self->speed.x = self->speed.z = 0.0;
                break;
            }
        }

        //Decelerate
        self->speed.y += -0.4f;
        obj_integrate_speed(self, self->speed.x, self->speed.y, self->speed.z);

        fxTrans.roll = 0xFF;
        fxTrans.pitch = 0xFF - (objData->rotation.asWord % 1280);
        fxTrans.yaw = 0xFF;
        gDLL_17_partfx->vtbl->spawn(self, 0x357, &fxTrans, 0, -1, 0);
        break;
    case DBBoneDust_STATE_Hovering:
        //Oscillating over ground, ready to be collected
        self->speed.y = fsin16_precise(objData->rotation.asHalfwords[1]) * 0.3f;
        obj_integrate_speed(self, self->speed.x, self->speed.y, self->speed.z);

        //Check if player is close by
        if (vec3_distance(&self->positionMirror, &player->positionMirror) < 100.0f) {
            objData->state = DBBoneDust_STATE_Fly_Towards_Player;
        }
        break;
    case DBBoneDust_STATE_Fly_Towards_Player:
        distance = vec3_distance_xz(&self->positionMirror, &player->positionMirror);
        if (distance < 6.0f) {
            //Collected
            ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 3);
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_8E_Magic_Chime, MAX_VOLUME, 0, 0, 0, 0);
            objData->state = DBBoneDust_STATE_Hidden;
        } else {
            //Get unit vector pointing towards the player
            absoluteSpeed = 4.0f;
            uPlayer.z = player->srt.transl.x - self->srt.transl.x;
            uPlayer.y = player->srt.transl.y - (self->srt.transl.y - 25.0f);
            uPlayer.x = player->srt.transl.z - self->srt.transl.z;
            if (distance < 4.0f) {
                absoluteSpeed = distance;
            }
            guNormalize(&uPlayer.z, &uPlayer.y, &uPlayer.x);

            //Go towards the player
            self->speed.x = uPlayer.z * absoluteSpeed;
            self->speed.y = uPlayer.y * absoluteSpeed;
            self->speed.z = uPlayer.x * absoluteSpeed;
            obj_integrate_speed(self, self->speed.x, self->speed.y, self->speed.z);

            //Motion trail effect
            fxTrans.roll = 0xFF;
            fxTrans.pitch = 0;
            fxTrans.yaw = 0xFF;
            gDLL_17_partfx->vtbl->spawn(self, 0x357, &fxTrans, 0, -1, 0);
        }
        break;
    }

    objData->rotation.asWord += gUpdateRate * 1280;
}

// offset: 0x488 | func: 2 | export: 2
void DBBoneDust_update(Object *self) { }

// offset: 0x494 | func: 3 | export: 3
void DBBoneDust_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    DBBoneDust_Data *objData = self->data;
    
    if (visibility && objData->state != DBBoneDust_STATE_Hidden) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4F8 | func: 4 | export: 4
void DBBoneDust_free(Object *self, s32 a1) { }

// offset: 0x508 | func: 5 | export: 5
u32 DBBoneDust_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x518 | func: 6 | export: 6
u32 DBBoneDust_get_data_size(Object *self, u32 a1) {
    return sizeof(DBBoneDust_Data);
}

// offset: 0x52C | func: 7 | export: 7
int DBBoneDust_is_hidden(Object *self) {
    DBBoneDust_Data *objData = self->data;
    return objData->state == DBBoneDust_STATE_Hidden;
}

// offset: 0x544 | func: 8 | export: 8
void DBBoneDust_launch(Object* self, Vec3f* position, Vec3f* speed, f32 baseY) {
    DBBoneDust_Data* objData = self->data;
    
    if (position != NULL) {
        self->srt.transl.x = position->x;
        self->srt.transl.y = position->y;
        self->srt.transl.z = position->z;
    }
    
    if (speed != NULL) {
        self->speed.x = speed->x * 4.0f;
        self->speed.y = speed->y * 4.0f;
        self->speed.z = speed->z * 4.0f;
    }
    
    objData->rotation.asWord = 0;
    objData->baseY = baseY;
    objData->state = DBBoneDust_STATE_Launched;

    STUBBED_PRINTF(" Attached ");
}
