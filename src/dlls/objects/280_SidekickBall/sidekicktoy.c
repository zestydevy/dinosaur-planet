#include "PR/ultratypes.h"
#include "common.h"
#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "sys/math.h"
#include "sys/objmsg.h"

#include "dlls/objects/280_sidekicktoy.h"

/*0x0*/ static Vec3f data_collisionPoint = {
    0, 0, 0
};

// offset: 0x0 | ctor
void SidekickToy_ctor(void *dll) { }

// offset: 0xC | dtor
void SidekickToy_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SidekickToy_setup(Object* self, SidekickToy_Setup* objsetup, s32 arg2) {
    SidekickToy_Data* objdata = self->data;
    Object* player;
    u8 colliderArg = 5;

    bzero(objdata, sizeof(SidekickToy_Data));
    player = get_player();

    objdata->state = TOY_STATE_0_Carried;
    objdata->timer = 0.0f;
    objdata->interactionTimer = 0.0f;
    self->srt.flags |= 0x4000;
    self->unkB0 |= 0x6000;
    if (player) {
        ((DLL_210_Player*)player->dll)->vtbl->func9(player, self);
    }
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    objdata->collisionRadius = self->objhitInfo->unk52;
    gDLL_27->vtbl->init(&objdata->collision, 0, DLL27FLAG_40000 | DLL27FLAG_2, 1);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->collision, 1, &data_collisionPoint, &objdata->collisionRadius, &colliderArg);
    func_800267A4(self);
    objdata->collision.mode = 0;
    obj_init_mesg_queue(self, 1);
    main_set_bits(BIT_3F8, 0);
}

// offset: 0x1A4 | func: 1
static void SidekickToy_tick_collision(Object* self, SidekickToy_Data* objdata) {
    gDLL_27->vtbl->func_1E8(self, &objdata->collision, gUpdateRateF);
    gDLL_27->vtbl->func_5A8(self, &objdata->collision);
    gDLL_27->vtbl->func_624(self, &objdata->collision, gUpdateRateF);
}

// offset: 0x250 | func: 2
static s32 SidekickToy_tick_flight(Object* self) {
    Vec3f speed;
    Vec3f sp40;
    f32 absSpeed;
    f32 temp_ft0;
    f32 dotProductDouble;
    f32 tempAbsSpeed;
    SidekickToy_Data* objdata;

    objdata = self->data;

    self->speed.y -= 0.05f * gUpdateRateF;
    obj_integrate_speed(self,
        self->speed.x * gUpdateRateF,
        self->speed.y * gUpdateRateF,
        self->speed.z * gUpdateRateF
    );
    SidekickToy_tick_collision(self, objdata);
    if (objdata->collision.unk25D) {
        speed.f[0] = -self->speed.x;
        speed.f[1] = -self->speed.y;
        speed.f[2] = -self->speed.z;
        absSpeed = VECTOR_MAGNITUDE(speed);
        tempAbsSpeed = absSpeed;
        if (tempAbsSpeed > 0.5f) {
            if (tempAbsSpeed > 2.0f) {
                tempAbsSpeed = 2.0f;
            }
            gDLL_6_AMSFX->vtbl->play_sound(self, 0x161, tempAbsSpeed * 32.0f, 0, 0, 0, 0);
        }
        //Getting normalised direction vector from speed
        if (absSpeed != 0.0f) {
            speed.f[0] *= 1.0f / absSpeed;
            speed.f[1] *= 1.0f / absSpeed;
            speed.f[2] *= 1.0f / absSpeed;
        }
        sp40.f[0] = objdata->collision.unk68.unk0[0].x;
        sp40.f[1] = objdata->collision.unk68.unk0[0].y;
        sp40.f[2] = objdata->collision.unk68.unk0[0].z;
        dotProductDouble = 2.0f * DOT_PRODUCT(speed, sp40);

        self->speed.x = sp40.x*dotProductDouble;
        self->speed.y = sp40.y*dotProductDouble;
        self->speed.z = sp40.z*dotProductDouble;
        VECTOR_SUBTRACT(self->speed, speed, self->speed)

        if (absSpeed < 0.3f) {
            if (gDLL_25->vtbl->func_12FC(self->srt.transl.f)) {
                return TOY_STATE_1_At_Rest;
            }
            self->speed.x *= 10.0f;
            self->speed.y *= 10.0f;
            self->speed.z *= 10.0f;
        }
        self->speed.x *= absSpeed * 0.7f;
        self->speed.y *= absSpeed * 0.70f; //NOTE: compiles as separate rodata
        self->speed.z *= absSpeed * 0.7f;
    }
    self->srt.pitch = self->speed.z * 1000.0f;
    self->srt.roll -= self->speed.x * 1000.0f;
    return TOY_STATE_2_In_Flight;
}

// offset: 0x664 | func: 3 | export: 1
void SidekickToy_control(Object* self) {
    u32 receivedMessage;
    SidekickToy_Data* objdata;

    objdata = self->data;

    //Enable interaction and collision a half-second after being thrown
    if (objdata->interactionTimer != 0.0f) {
        objdata->interactionTimer -= gUpdateRateF;
        if (objdata->interactionTimer <= 0.0f) {
            objdata->interactionTimer = 0.0f;
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
            func_8002674C(self);
            objdata->collision.mode = 1;
        }
    }
    
    //Check for message from player DLL, sent when ball collected
    while (obj_recv_mesg(self, &receivedMessage, 0, 0)){
        if (receivedMessage == 0x7000B) {
            objdata->timer = 0.0f;
            objdata->state = TOY_STATE_4_Vanish;
        }
    }
    
    switch (objdata->state) {
    case TOY_STATE_2_In_Flight:
        objdata->timer += gUpdateRateF;
        if (objdata->timer >= 6000.0f) {
            objdata->timer = 0.0f;
            objdata->state = TOY_STATE_4_Vanish;
            return;
        }
        objdata->state = SidekickToy_tick_flight(self);
        return;
    case TOY_STATE_1_At_Rest:
        objdata->timer += gUpdateRateF;
        if (objdata->timer >= 3000.0f) {
            objdata->timer = 0.0f;
            objdata->state = TOY_STATE_4_Vanish;
            return;
        }
        if (self->unkAF & ARROW_FLAG_1_Interacted) {
            ((DLL_Unknown*)gDLL_3_Animation)->vtbl->func[30].withThreeArgs(0x5BA, 0, 0);
            obj_send_mesg(get_player(), 0x7000A, self, (void*)BIT_ALWAYS_1);
            objdata->state = TOY_STATE_3_Collected;
        }
        break;
    case TOY_STATE_0_Carried:
        break;
    case TOY_STATE_4_Vanish:
        objdata->timer += gUpdateRateF;
        if (objdata->timer >= 60.0f) {
            obj_destroy_object(self);
        } else {
            self->opacity = 0xFF - (s32) ((objdata->timer * 255.0f) / 60.0f);
            break;
        }
        return;
    case TOY_STATE_3_Collected:
        break;
    }

    SidekickToy_tick_collision(self, objdata);
}

// offset: 0x954 | func: 4 | export: 2
void SidekickToy_update(Object *self) { }

// offset: 0x960 | func: 5 | export: 3
void SidekickToy_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility){
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x9B4 | func: 6 | export: 4
void SidekickToy_free(Object* self, s32 arg1) {
    Object* player;
    Object* collectedObject;

    STUBBED_PRINTF("the side kick toy is unloading\n");
    
    player = get_player();
    if (player) {
        ((DLL_210_Player*)player->dll)->vtbl->func8(player, &collectedObject);
        if (collectedObject == self) {
            ((DLL_210_Player*)player->dll)->vtbl->func9(player, 0);
        }
    }
    main_set_bits(BIT_3F8, 1);
}

// offset: 0xA64 | func: 7 | export: 5
u32 SidekickToy_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xA74 | func: 8 | export: 6
u32 SidekickToy_get_data_size(Object *self, u32 a1) {
    return sizeof(SidekickToy_Data);
}

// offset: 0xA88 | func: 9 | export: 7
void SidekickToy_throw(Object* self, Object* thrownBy, f32 speedX, f32 speedY, f32 speedZ) {
    SidekickToy_Data* objdata = self->data;
    objdata->state = TOY_STATE_2_In_Flight;
    objdata->timer = 0.0f;
    self->speed.x = speedX;
    self->speed.y = speedY;
    self->speed.z = speedZ;
    self->srt.flags = 0;
    objdata->interactionTimer = 30.0f;
}

// offset: 0xADC | func: 10 | export: 8
void SidekickToy_carry(Object* self, SidekickToy_Data* objdata) {
    SidekickToy_Data* objdata2 = self->data;
    objdata2->state = TOY_STATE_0_Carried;
    objdata2->timer = 0.0f;
    
    self->srt.flags |= 0x4000;

    func_800267A4(self);
    objdata2->collision.mode = 0;
}

// offset: 0xB3C | func: 11 | export: 9
s32 SidekickToy_is_stopped(Object* self) {
    SidekickToy_Data* objdata = self->data;
    return objdata->state == TOY_STATE_1_At_Rest;
}
