#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/418_DFriverflow.h"
#include "dlls/objects/419_DFdockpoint.h"
#include "dlls/objects/common/vehicle.h"
#include "macros.h"
#include "sys/joypad.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/objprint.h"
#include "sys/print.h"
#include "dll.h"

typedef enum {
    BWLog_STATE_0_Main,
    BWLog_STATE_1_Roll_Left,
    BWLog_STATE_2_Roll_Right,
    BWLog_STATE_3_Placeholder1,
    BWLog_STATE_4_Placeholder2
} BWlog_States;

typedef struct {
/*000*/ DLL27_Data collider;
/*260*/ Vec3f endPoints[2];
/*278*/ Vec3f velocity[2];
/*290*/ f32 powerZ[2]; // forward/backward
/*298*/ f32 powerX[2]; // lateral
/*2A0*/ f32 rollCurve[4]; //Roll speed spline
/*2B0*/ f32 rollSpeed; // wobble (smoothed)
/*2B4*/ f32 targetWaterYOffset;
/*2B8*/ f32 paddleTimer; // move timer (when > 0, move forward)
/*2BC*/ f32 rollTimer;
/*2C0*/ f32 rollAcceleration; // wobble
/*2C4*/ s32 _unk2C4;
/*2C8*/ f32 rollCurveProgress;
/*2CC*/ f32 tValueRoll;
/*2D0*/ f32 flowX[2]; //DFriverflow objects' combined push strength in X (values for both ends of log)
/*2D8*/ f32 flowY[2]; //DFriverflow objects' combined push strength in Y (values for both ends of log)
/*2E0*/ f32 flowZ[2]; //DFriverflow objects' combined push strength in Z (values for both ends of log)
/*2E8*/ u8 _unk2E8[0x2F8 - 0x2E8];
/*2F8*/ f32 joyATimer;
/*2FC*/ f32 paddlePower;
/*300*/ f32 unk300[2];
/*308*/ f32 soundPitch;
/*30C*/ f32 soundVolume;
/*310*/ f32 riverflowMagnitude;
/*314*/ u32 soundHandle; //Controls rushing water sound loop
/*318*/ s32 rollAngle; // when rolling, the current roll rotation
/*31C*/ u16 wiggleYOffsets[2]; // y offset for each end point to pitch the log up/down to simulate small waves
/*320*/ u16 joyPressed; // controller buttons pressed
/*322*/ s16 joyStickX; // joystick x
/*324*/ s16 joyStickY; // joystick y
/*326*/ s16 soundPitchPhase;
/*328*/ s16 soundVolumePhase;
/*32A*/ u8 state; // roll state (0 = not rolling, 1 = left, 2 = right)
/*32B*/ u8 playerVehicleAnim;
/*32C*/ u8 joyARecentTap; // a pressed (turns off automatically after a time or if a is pressed again)
/*32D*/ u8 unk32D; // bitfield of which side of the log is touching terrain (0x1 = front, 0x2 = back, 0x3 = both)
/*32E*/ u8 mountState; // see VehicleMountState
/*32F*/ u8 _unk32F[0x338 - 0x32F];
/*338*/ Object *dockpoint;
} BWlog_Data;

/*0x0*/ static Vec3f dLocalEndpointCoords[] = {
    VEC3F(0.0f, 0.0f, -30.0f),
    VEC3F(0.0f, 0.0f, 30.0f)
};
/*0x18*/ static f32 dCollisionRadii[] = {8.0f, 8.0f};
/*0x20*/ static u8 dColliderParams[] = {1, 1};
/*0x24*/ static f32 _data_24[] = {500.0f, 500.0f, 0.0f, 0.0f};

static void BWlog_handle_water(Object* self, BWlog_Data* objdata, s32 arg2);
static void BWlog_handle_controls_a_button(Object* self, BWlog_Data* objdata);
static void BWlog_start_roll(Object* self, BWlog_Data* objdata, s32 rollLeft);
static void BWlog_handle_roll(Object* self, BWlog_Data* objdata);
static void BWlog_handle_unknown_state(Object* self, BWlog_Data* objdata);
static void BWlog_handle_paddle_motion(Object* self, BWlog_Data* objdata);
static void BWlog_find_riverflows(Object* self, BWlog_Data* objdata);
static void BWlog_handle_sounds(Object* self, BWlog_Data* objdata);
static void BWlog_handle_fx(Object* self, BWlog_Data* objdata);

// offset: 0x0 | ctor
void BWlog_ctor(void *dll) { }

// offset: 0xC | dtor
void BWlog_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void BWlog_setup(Object *self, ObjSetup *setup, s32 arg2) {
    BWlog_Data *objdata = (BWlog_Data*)self->data;
    s32 i;

    gDLL_27->vtbl->init(&objdata->collider, 
        DLL27FLAG_NONE, 
        DLL27FLAG_8000000 | DLL27FLAG_40000 | DLL27FLAG_20 | DLL27FLAG_2 | DLL27FLAG_1, 
        DLL27MODE_1);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->collider, ARRAYCOUNT(dLocalEndpointCoords), dLocalEndpointCoords, dCollisionRadii, dColliderParams);
    objdata->collider.boundsYExtension = 100;
    obj_add_object_type(self, OBJTYPE_Vehicle);
    objdata->wiggleYOffsets[1] = 0x2000;
    objdata->targetWaterYOffset = 15.0f;
    
    //Use a white "shadow" to create wave effect around the log
    self->shadow->flags |= (OBJ_SHADOW_FLAG_WATER_SURFACE | OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_USE_OBJ_YAW | OBJ_SHADOW_FLAG_CUSTOM_COLOR | OBJ_SHADOW_FLAG_CUSTOM_DIR);
    self->shadow->r = 0xFF;
    self->shadow->g = 0xFF;
    self->shadow->b = 0xFF;
    self->shadow->a = 0x7F;

    for (i = 0; i < 2; i++) {
        objdata->endPoints[i].x = self->srt.transl.x;
        objdata->endPoints[i].y = self->srt.transl.y;
        objdata->endPoints[i].z = self->srt.transl.z;
    }
}

// offset: 0x170 | func: 1 | export: 1
void BWlog_control(Object* self) {
    BWlog_Data* objdata;
    f32 damp;
    f32 vel[3];
    Vec3f vec;
    SRT srt;
    MtxF logMtx;
    MtxF pitchYawMtx;
    MtxF invPitchYawMtx;
    f32 sp9C;
    f32 distance;
    DFdockpoint_Setup* dockpointSetup;
    s32 i;

    objdata = (BWlog_Data*)self->data;

    distance = 10000.0f;
    objdata->dockpoint = obj_get_nearest_type_to(OBJTYPE_Dockpoint, self, &distance);
    if (objdata->dockpoint != NULL) {
        dockpointSetup = (DFdockpoint_Setup*)objdata->dockpoint->setup;
        distance = vec3_distance(&self->globalPosition, &objdata->dockpoint->globalPosition);
        // Reduce velocity while near a dockpoint
        if (objdata->mountState == VEHICLE_Mounted) {
            damp = 0.95f;
        } else {
            damp = 0.5f;
        }
        if (distance < dockpointSetup->range) {
            for (i = 0; i < 2; i++) {
                objdata->velocity[i].x *= damp;
                objdata->velocity[i].z *= damp;
            }
        } else {
            objdata->dockpoint = NULL;
        }
    }

    BWlog_find_riverflows(self, objdata);

    if (objdata->mountState == VEHICLE_Mounted) {
        BWlog_handle_controls_a_button(self, objdata);

        switch (objdata->state) {
        case BWLog_STATE_1_Roll_Left:
        case BWLog_STATE_2_Roll_Right:
            BWlog_handle_roll(self, objdata);
            break;
        case BWLog_STATE_3_Placeholder1:
        case BWLog_STATE_4_Placeholder2:
            BWlog_handle_unknown_state(self, objdata); //Placeholder for other left/right state? Maybe getting hurt?
            break;
        default:
            BWlog_handle_paddle_motion(self, objdata);
            break;
        }

        // Apply turning
        self->srt.yaw -= (s32) (objdata->joyStickX * (60.0f - (objdata->joyStickY * 0.05f)) * 0.2f) & 0xFFFF & 0xFFFF;
    }

    srt.yaw = self->srt.yaw;
    srt.pitch = self->srt.pitch;
    srt.roll = self->srt.roll;
    srt.scale = 1.0f;
    srt.transl.x = self->srt.transl.x;
    srt.transl.y = self->srt.transl.y;
    srt.transl.z = self->srt.transl.z;
    matrix_from_srt(&logMtx, &srt);
    srt.roll = 0;
    srt.transl.x = 0;
    srt.transl.y = 0;
    srt.transl.z = 0;
    matrix_from_srt(&pitchYawMtx, &srt);
    srt.yaw = -srt.yaw;
    srt.pitch = -srt.pitch;
    matrix_from_srt_reversed(&invPitchYawMtx, &srt);

    //Update the log's two endpoints
    for (i = 0; i < 2; i++) {
        // Recalculate log end points from current matrix
        vec3_transform(&logMtx, 
                       dLocalEndpointCoords[i].x, dLocalEndpointCoords[i].y, dLocalEndpointCoords[i].z, 
                       &objdata->endPoints[i].x, &objdata->endPoints[i].y, &objdata->endPoints[i].z);
        // Do water physics for this side of the log
        BWlog_handle_water(self, objdata, i);
        // Factor in riverflow influence
        vec3_transform(&invPitchYawMtx, 
                       objdata->flowX[i], 0.0f, objdata->flowZ[i], 
                       &vel[0], &vel[1], &vel[2]);
        vel[0] *= -0.5f;
        vel[2] *= 0.5f;
        objdata->riverflowMagnitude = sqrtf(SQ(vel[2]) + SQ(vel[0]));
        // Factor in paddle forward power
        vel[2] += objdata->paddlePower;
        // Interpolate to target forward/lateral powers
        objdata->powerZ[i] += ((vel[2] - objdata->powerZ[i]) * gUpdateRateF * 0.1f);
        if (objdata->state == BWLog_STATE_0_Main) {
            objdata->powerX[i] += ((vel[0] - objdata->powerX[i]) * gUpdateRateF * 0.1f);
        }
        // Convert powers to velocity in world space
        vec3_transform(&pitchYawMtx, 
                       objdata->powerX[i], 0.0f, -objdata->powerZ[i], 
                       &objdata->velocity[i].x, &sp9C, &objdata->velocity[i].z);
        // Apply velocity
        vel[0] = objdata->velocity[i].x * gUpdateRateF;
        vel[1] = objdata->velocity[i].y * gUpdateRateF;
        vel[2] = objdata->velocity[i].z * gUpdateRateF;
        objdata->endPoints[i].x = objdata->endPoints[i].x + vel[0];
        objdata->endPoints[i].y = objdata->endPoints[i].y + vel[1];
        objdata->endPoints[i].z = objdata->endPoints[i].z + vel[2];
    }

    // Set object position to average of end point positions
    VECTOR_ADD(objdata->endPoints[0], objdata->endPoints[1], vec);
    self->srt.transl.x = vec.f[0] * 0.5f;
    self->srt.transl.y = vec.f[1] * 0.5f;
    self->srt.transl.z = vec.f[2] * 0.5f;
    // Align object pitch with pitch of end points line
    VECTOR_SUBTRACT(objdata->endPoints[1], objdata->endPoints[0], vec);
    self->srt.pitch = -arctan2_f(vec.f[1], sqrtf(SQ(vec.f[2]) + SQ(vec.f[0])));
    // Collider updates
    gDLL_27->vtbl->func_1E8(self, &objdata->collider, gUpdateRateF);
    gDLL_27->vtbl->func_5A8(self, &objdata->collider);
    gDLL_27->vtbl->func_624(self, &objdata->collider, gUpdateRateF);

    BWlog_handle_sounds(self, objdata);
    BWlog_handle_fx(self, objdata);
}

// offset: 0x85C | func: 2 | export: 2
void BWlog_update(Object *self) { }

// offset: 0x868 | func: 3 | export: 3
void BWlog_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x8BC | func: 4 | export: 4
void BWlog_free(Object *self, s32 a1) {
    BWlog_Data *objdata;

    objdata = self->data;
    obj_free_object_type(self, OBJTYPE_Vehicle);
    //Stop sound loop
    if (objdata->soundHandle != 0) {
        gDLL_6_AMSFX->vtbl->stop(objdata->soundHandle);
    }
}


// offset: 0x92C | func: 5 | export: 5
u32 BWlog_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x93C | func: 6 | export: 6
u32 BWlog_get_data_size(Object *self, u32 a1) {
    return sizeof(BWlog_Data);
}

// offset: 0x950 | func: 7 | export: 7
s32 BWlog_vehicle_can_mount(Object *self, Object *rider) {
    BWlog_Data *objdata = (BWlog_Data*)self->data;
    if ((objdata->mountState == VEHICLE_NoRider) && (objdata->dockpoint != NULL)) {
        return vec3_distance(&rider->globalPosition, &self->globalPosition) < 50.0f;
    }
    return FALSE;
}

// offset: 0x9E0 | func: 8 | export: 8
s32 BWlog_vehicle_get_mount_side(Object *self) {
    SRT srt;
    MtxF mtx;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 temp;
    f32 temp2;
    Object *player;

    player = get_player();
    if (player != NULL) {
        //Transform a unit vector (along Z) into the log's local coordinate space
        srt.yaw = self->srt.yaw + M_90_DEGREES;
        srt.pitch = self->srt.pitch;
        srt.roll = self->srt.roll;
        srt.transl.x = 0.0f;
        srt.transl.y = 0.0f;
        srt.transl.z = 0.0f;
        srt.scale = 1.0f;
        matrix_from_srt(&mtx, &srt);
        vec3_transform(&mtx, 0.0f, 0.0f, 1.0f, &sp44, &sp40, &sp3C);

        //Get dot products for self and player
        temp2 = -((self->srt.transl.x * sp44) + (sp40 * self->srt.transl.y) + (sp3C * self->srt.transl.z));
        temp = (player->srt.transl.x * sp44) + (sp40 * player->srt.transl.y) + (sp3C * player->srt.transl.z) + temp2;
        if (temp < 0) {
            return 1; // left
        }
    }
    return 2; // right
}

// offset: 0xB28 | func: 9 | export: 9
void BWlog_vehicle_get_rider_position(Object *self, f32 *ox, f32 *oy, f32 *oz) {
    *ox = self->srt.transl.x;
    *oy = self->srt.transl.y;
    *oz = self->srt.transl.z;
}

// offset: 0xB48 | func: 10 | export: 10
s32 BWlog_vehicle_can_dismount(Object *self, Object *rider) {
    BWlog_Data *objdata;
    f32 speed;
    s32 i;

    objdata = (BWlog_Data*)self->data;
    if ((objdata->dockpoint != NULL) && (objdata->mountState == VEHICLE_Mounted) && (joyGetPressed(0) & B_BUTTON)) {
        speed = 0.0f;
        for (i = 0; i < 2; i++) {
            speed += sqrtf(SQ(objdata->velocity[i].x) + SQ(objdata->velocity[i].z));
        }
        return speed < 2.0f;
    }
    return FALSE;
}

// offset: 0xC3C | func: 11 | export: 11
s32 BWlog_vehicle_get_dismount_side(Object *self) {
    return 0;
}

// offset: 0xC4C | func: 12 | export: 12
void BWlog_vehicle_get_camera_position(Object *self, f32 *x, f32 *y, f32 *z) {
    MtxF mtx;
    SRT srt;

    srt.yaw = self->srt.yaw;
    srt.pitch = 0;
    srt.roll = 0;
    srt.scale = 1.0f;
    srt.transl.x = self->srt.transl.x;
    srt.transl.y = self->srt.transl.y;
    srt.transl.z = self->srt.transl.z;
    matrix_from_srt(&mtx, &srt);
    vec3_transform(&mtx, 0.0f, 0.0f, -10.0f, x, y, z);
}

// offset: 0xD08 | func: 13 | export: 13
s32 BWlog_vehicle_get_mount_state(Object *self) {
    BWlog_Data *objdata = (BWlog_Data*)self->data;
    return objdata->mountState;
}

// offset: 0xD18 | func: 14 | export: 14
void BWlog_vehicle_set_mount_state(Object *self, s32 state) {
    Object *player = get_player();
    BWlog_Data *objdata = (BWlog_Data*)self->data;

    if (state != VEHICLE_NoRider) {
        obj_clear_map_id(self);
        ((DLL_210_Player*)player->dll)->vtbl->func28(player, 1);
        gDLL_2_Camera->vtbl->change_mode(0, 0x2B);
    } else {
        obj_infer_map_id(self);
        ((DLL_210_Player*)player->dll)->vtbl->func29(player, 1);
        gDLL_2_Camera->vtbl->change_mode(0, 1);
    }

    objdata->mountState = state;
}

// offset: 0xE2C | func: 15 | export: 15
void BWlog_vehicle_get_player_anim(Object *self, f32 *tValue, s32 *animIndex) {
    BWlog_Data *objdata = (BWlog_Data*)self->data;
    *tValue = objdata->tValueRoll;
    *animIndex = objdata->playerVehicleAnim;
}

// offset: 0xE48 | func: 16 | export: 16
f32 BWlog_func_E48(Object *self, f32 *a1) {
    *a1 = -0.006f;
    return 0.0f;
}

// offset: 0xE70 | func: 17 | export: 17
s32 BWlog_vehicle_get_race_position(Object *self) {
    return 0;
}

// offset: 0xE80 | func: 18 | export: 18
void BWlog_func_E80(Object *self) { }

// offset: 0xE8C | func: 19 | export: 19
void BWlog_vehicle_handle_rider_scale(Object *self, f32 scale) { }

// offset: 0xE9C | func: 20 | export: 20
void BWlog_func_E9C(s32 a0, s32 a1, s32 a2) { }

// offset: 0xEB0 | func: 21
static void BWlog_handle_water(Object* self, BWlog_Data* objdata, s32 side) {
    f32 temp_fa1;
    f32 temp_ft5;
    f32 temp_fv0;
    f32 sp60;
    s32 sp50;
    f32 volume;
    f32 var_fv1;
    s32 i;
    f32 targetY;

    // Calculate target Y coordinate
    targetY = objdata->targetWaterYOffset + objdata->collider.waterYList[side];
    if ((objdata->endPoints[side].y + 30.0f) < targetY) {
        diPrintf("Water too high\n");
    }

    // Pitch log slightly to simulate small waves
    targetY += (fsin16_precise(objdata->wiggleYOffsets[side]) * 1.5f);
    objdata->wiggleYOffsets[side] += (gUpdateRateF * 512.0f);

    sp60 = targetY - objdata->endPoints[side].y;
    if ((sp60 > 0.0f) && (objdata->unk300[side] < 0.0f)) {
        volume = objdata->velocity[side].y * 127.0f;
        if (volume < 0.0f) {
            volume = -volume;
        }
        if (volume > 127.0f) {
            volume = 127.0f;
        }
        if (volume > 20.0f) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_A75, (u8)volume, NULL, NULL, 0, NULL);
        }
    } else if ((sp60 < 0.0f) && (objdata->unk300[side] > 0.0f)) {
        volume = objdata->velocity[side].y * 127.0f;
        if (volume < 0.0f) {
            volume = -volume;
        }
        if (volume > 127.0f) {
            volume = 127.0f;
        }
        if (volume > 20.0f) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_A74, (u8)volume, NULL, NULL, 0, NULL);
        }
    }

    objdata->unk300[side] = sp60;
    if (sp60 > 25.0f) {
        sp60 = 25.0f;
    }
    if (sp60 < 0.0f) {
        sp60 = 0.0f;
    }
    objdata->velocity[side].y += ((sp60 / 15.0f) * 0.15f * gUpdateRateF);
    objdata->velocity[side].y -= (0.1f * gUpdateRateF);
    i = (s32) gUpdateRate;
    diPrintf("[%d]=%f\n", side, &sp60);
    if (sp60 > 0.0f) {
        if (objdata->velocity[side].y < 0.0f) {
            var_fv1 = sp60 / 25.0f;
            temp_ft5 = 1.0f + var_fv1;
            if (var_fv1 > 1.0f) {
                var_fv1 = 1.0f;
            }
            var_fv1 = (1.0f - var_fv1);
            temp_fa1 = (0.007000029f * var_fv1) + 0.988f;
            while (i--) {
                temp_fv0 = objdata->velocity[side].y;
                if (temp_fv0 > 0/*.0f*/) {
                    var_fv1 = temp_fv0;
                } else {
                    var_fv1 = -temp_fv0;
                }
                objdata->velocity[side].y = temp_fv0 - (temp_fv0 * var_fv1 * 0.1f * temp_ft5);
                objdata->powerZ[side] *= temp_fa1;
                objdata->powerX[side] *= temp_fa1;
                objdata->paddlePower *= 0.99f;
            }
        } else {
            var_fv1 = sp60 / 25.0f;
            temp_ft5 = 1.0f + var_fv1;
            if (var_fv1 > 1.0f) {
                var_fv1 = 1.0f;
            }
            var_fv1 = (1.0f - var_fv1);
            temp_fa1 = (0.007000029f * var_fv1) + 0.988f;
            while (i--) {
                temp_fv0 = objdata->velocity[side].y;
                if (temp_fv0 > 0/*.0f*/) {
                    var_fv1 = temp_fv0;
                } else {
                    var_fv1 = -temp_fv0;
                }
                objdata->velocity[side].y = temp_fv0 - (temp_fv0 * var_fv1 * 0.1f * temp_ft5);
                objdata->powerZ[side] *= temp_fa1;
                objdata->powerX[side] *= temp_fa1;
                objdata->paddlePower *= 0.99f;
            }
        }
    } else {
        while (i--) {
            objdata->velocity[side].y *= 0.994f;
            objdata->powerZ[side] *= 0.995f;
            objdata->powerX[side] *= 0.99f;
            objdata->paddlePower *= 0.99f;
        }
    }
}

// offset: 0x1600 | func: 22
static void BWlog_handle_controls_a_button(Object* self, BWlog_Data* objdata) {
    s32 doubleTappedA;

    objdata->joyPressed = joyGetPressed(0);
    objdata->joyStickX = joyGetStickX(0);
    objdata->joyStickY = joyGetStickY(0);

    //Decrement timer (used for detecting A button double-tap)
    objdata->joyATimer -= gUpdateRateF;
    if (objdata->joyATimer <= 0.0f) {
        objdata->joyARecentTap = FALSE;
        objdata->joyATimer = 0.0f;
    }

    //Check for A button presses
    doubleTappedA = FALSE;
    if (objdata->joyPressed & A_BUTTON) {
        if (objdata->joyARecentTap) {
            //If A button was already tapped recently, register this A press as a double-tap
            doubleTappedA = TRUE;
            objdata->joyARecentTap = FALSE;
            objdata->joyATimer = 0.0f;
        } else {
            //Otherwise, remember that there was a recent A-tap and start a timer until it's forgotten
            objdata->joyARecentTap = TRUE;
            objdata->joyATimer = 15.0f;
        }
    }

    if (doubleTappedA) {
        //Roll when double-tapping A
        if (objdata->joyStickX > 20) {
            BWlog_start_roll(self, objdata, FALSE);
            objdata->state = BWLog_STATE_2_Roll_Right;
            return;
        }
        if (objdata->joyStickX < -20) {
            BWlog_start_roll(self, objdata, TRUE);
            objdata->state = BWLog_STATE_1_Roll_Left;
        }
    } else if (objdata->joyPressed & A_BUTTON) {
        //Paddle with single A-press
        objdata->paddleTimer = 30.0f;
    }
}

// offset: 0x178C | func: 23
static void BWlog_start_roll(Object* self, BWlog_Data* objdata, s32 rollLeft) {
    if (rollLeft) {
        objdata->rollCurve[0] = 1500.0f;
        objdata->rollCurve[1] = 500.0f;
        objdata->rollCurve[2] = 2000.0f;
        objdata->rollCurve[3] = 4000.0f;
        objdata->playerVehicleAnim = 2;
        objdata->powerX[0] = -2.5f;
        objdata->powerX[1] = -2.5f;
    } else {
        objdata->rollCurve[0] = -1500.0f;
        objdata->rollCurve[1] = -500.0f;
        objdata->rollCurve[2] = -2000.0f;
        objdata->rollCurve[3] = -4000.0f;
        objdata->playerVehicleAnim = 3;
        objdata->powerX[0] = 2.5f;
        objdata->powerX[1] = 2.5f;
    }

    objdata->rollAngle = 0;
    objdata->rollTimer = 0.0f;
    objdata->rollSpeed = objdata->rollCurve[0];
    objdata->rollCurveProgress = 0.0f;
    objdata->tValueRoll = 0.0f;
}

// offset: 0x1858 | func: 24
static void BWlog_handle_roll(Object* self, BWlog_Data* objdata) {
    s32 i;

    objdata->rollCurveProgress = objdata->rollTimer / 100.0f;
    objdata->rollTimer += gUpdateRateF;
    if (objdata->rollCurveProgress < 0.0f) {
        objdata->rollCurveProgress = 0.0f;
    } else if (objdata->rollCurveProgress > 1.0f) {
        objdata->state = BWLog_STATE_0_Main;
        objdata->rollCurveProgress = 1.0f;
    }

    //Get the log's roll speed via a spline
    objdata->rollSpeed = curves_hermite(objdata->rollCurve, objdata->rollCurveProgress, NULL);

    i = gUpdateRate;
    while (i--) {
        objdata->powerX[0] *= 0.985f;
        objdata->powerX[1] *= 0.985f;
    }

    objdata->rollAngle += gUpdateRateF * objdata->rollSpeed;
    objdata->tValueRoll = objdata->rollAngle / M_360_DEGREES_F;

    //Make sure the roll tValue is positive, and clamped from 0 to 1
    if (objdata->tValueRoll < 0.0f) {
        objdata->tValueRoll = -objdata->tValueRoll;
    }
    if (objdata->tValueRoll > 1.0f) {
        objdata->tValueRoll = 1.0f;
    }

    //Finish rolling after a full 360 rotation
    if ((objdata->rollAngle >= M_360_DEGREES) || (objdata->rollAngle <= -M_360_DEGREES)) {
        objdata->state = BWLog_STATE_0_Main;
    }

    if (objdata->rollCurveProgress > 0.7f) {
        objdata->targetWaterYOffset = 15.0f;
    } else {
        objdata->targetWaterYOffset = -2.0f;
    }

    self->srt.roll = objdata->rollAngle;
}

// offset: 0x1A4C | func: 25
static void BWlog_handle_unknown_state(Object* self, BWlog_Data* objdata) { }

// offset: 0x1A5C | func: 26
static void BWlog_handle_paddle_motion(Object* self, BWlog_Data* objdata) {
    s32 i;

    objdata->targetWaterYOffset = 15.0f;
    objdata->rollSpeed += objdata->rollAcceleration * gUpdateRateF;

    i = gUpdateRate;
    while (i--) {
        objdata->rollSpeed *= 0.97f;
    }
    self->srt.roll += (s32) (objdata->rollSpeed * gUpdateRateF);

    objdata->paddleTimer -= gUpdateRateF;
    if (objdata->paddleTimer < 0.0f) {
        objdata->paddleTimer = 0.0f;
    }

    objdata->rollAcceleration = -self->srt.roll / 100.0f;
    if (objdata->paddleTimer > 0.0f) {
        objdata->playerVehicleAnim = 1;
        objdata->targetWaterYOffset = 13.5f;
        objdata->rollAcceleration -= 5.0f;
        objdata->paddlePower = 2.0f;
    } else {
        objdata->playerVehicleAnim = 0;
    }

    objdata->rollAcceleration -= objdata->joyStickX * 0.5f;
    objdata->powerX[0] += (( objdata->powerZ[0] * objdata->joyStickX * 0.03f) - objdata->powerX[0]) * gUpdateRateF * 0.02f;
    objdata->powerX[1] += ((-objdata->powerZ[1] * objdata->joyStickX * 0.05f) - objdata->powerX[1]) * gUpdateRateF * 0.02f;
}

// offset: 0x1C18 | func: 27
static void BWlog_find_riverflows(Object* self, BWlog_Data* objdata) {
    s32 i;
    s32 k;
    s32 flowInfluences[2];
    s32 objListLength;
    Object* obj;
    Object** objList;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 pushRadius;
    f32 sp100;
    f32 spFC;
    f32 spF8;
    SRT srt;
    MtxF spA0;

    for (i = 0; i < 2; i++) {
        objdata->flowX[i] = 0.0f;
        objdata->flowZ[i] = 0.0f;
        flowInfluences[i] = 0;
    }

    // Grabs DFriverflow instances (and possibly more)
    objList = obj_get_all_of_type(OBJTYPE_Riverflow, &objListLength);

    for (i = 0; i < objListLength; i++) {
        obj = objList[i];
        for (k = 0; k < 2; k++) {
            dy = obj->srt.transl.y - objdata->endPoints[k].y;
            if ((dy <= 200.0f) && (dy >= -200.0f)) {
                dx = obj->srt.transl.x - objdata->endPoints[k].x;
                dz = obj->srt.transl.z - objdata->endPoints[k].z;
                dx = sqrtf(SQ(dx) + SQ(dz));
                pushRadius = ((DFriverflow_Setup*)obj->setup)->range * 1.5f;
                if (dx < pushRadius) {
                    dx = ((pushRadius - dx) / pushRadius);
                    dx *= (obj->srt.scale * 10.0f);
                    objdata->flowX[k] += fsin16_precise(obj->srt.yaw) * dx;
                    objdata->flowZ[k] += fcos16_precise(obj->srt.yaw) * dx;
                    flowInfluences[k]++;
                }
            }
        }
    }

    for (i = 0; i < 2; i++) {
        if (flowInfluences[i] != 0) {
            objdata->flowX[i] /= flowInfluences[i];
            objdata->flowZ[i] /= flowInfluences[i];
        }
    }
    
    for (i = 0; i < 2; i++) {
        srt.yaw = arctan2_f(objdata->flowX[i], objdata->flowZ[i]);
        srt.pitch = 0;
        srt.roll = 0;
        srt.scale = 1.0f;
        srt.transl.x = 0;
        srt.transl.y = 0;
        srt.transl.z = 0;
        matrix_from_srt_reversed(&spA0, &srt);
        vec3_transform(&spA0, 
            objdata->collider.waterNormalXList[i], objdata->collider.waterNormalYList[i], objdata->collider.waterNormalZList[i], 
            &sp100, &spFC, &spF8);
        srt.yaw = 0;
        srt.pitch = M_90_DEGREES - arctan2_f(spFC, spF8);
        srt.roll = -(M_90_DEGREES - arctan2_f(spFC, sp100));
        matrix_from_srt_reversed(&spA0, &srt);
        vec3_transform(&spA0, 
            objdata->flowX[i], 0.0f, objdata->flowZ[i], 
            &objdata->flowX[i], &objdata->flowY[i], &objdata->flowZ[i]);
    }
}

// offset: 0x2020 | func: 28
static void BWlog_handle_sounds(Object* self, BWlog_Data* objdata) {
    u8 bumpSide;
    u8 colliderFlags;
    s32 volume;

    if (objdata->soundHandle == 0) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_A77, MAX_VOLUME, &objdata->soundHandle, NULL, 0, NULL);
    } else {
        //Adjust sound volume sinusoidally
        objdata->soundVolume = objdata->riverflowMagnitude * 127.0f;
        objdata->soundVolume += fsin16_precise(objdata->soundVolumePhase) * 30.0f;
        if (objdata->soundVolume < 30.0f) {
            objdata->soundVolume = 30.0f;
        } else if (objdata->soundVolume > 127.0f) {
            objdata->soundVolume = 127.0f;
        }
        gDLL_6_AMSFX->vtbl->set_vol(objdata->soundHandle, objdata->soundVolume);

        //Adjust sound pitch sinusoidally
        objdata->soundPitch = ((objdata->unk300[0] + objdata->unk300[1]) * 0.5f) / 25.0f;
        if (objdata->soundPitch < 0.0f) {
            objdata->soundPitch = 0.0f;
        }
        objdata->soundPitch = 1.0f - objdata->soundPitch;
        objdata->soundPitch = (objdata->soundPitch * 0.2f) + 0.2f;
        objdata->soundPitch += fsin16_precise(objdata->soundPitchPhase) * 0.1f;
        gDLL_6_AMSFX->vtbl->set_pitch(objdata->soundHandle, objdata->soundPitch);

        objdata->soundPitchPhase += gUpdateRate << 8;
        objdata->soundVolumePhase += gUpdateRate << 9;
    }

    //Play bump sound when colliding
    volume = 0;
    colliderFlags = objdata->collider.unk25C & 3;
    bumpSide = colliderFlags & (colliderFlags ^ objdata->unk32D);
    if (bumpSide & 1) {
        volume = (s32) ((sqrtf(SQ(objdata->powerZ[0]) + SQ(objdata->powerX[0])) * 127.0f) / 0.95f);
    }
    if (bumpSide & 2) {
        if (volume > ((sqrtf(SQ(objdata->powerZ[1]) + SQ(objdata->powerX[1])) * 127.0f) / 0.95f)) {
            volume = (s32) (f32) volume; // what
        } else {
            volume = (s32) ((sqrtf(SQ(objdata->powerZ[1]) + SQ(objdata->powerX[1])) * 127.0f) / 0.95f);
        }
    }
    if (volume > 10) {
        if (volume > MAX_VOLUME) {
            volume = MAX_VOLUME;
        }
        gDLL_6_AMSFX->vtbl->play(self, SOUND_76D_Log_Bump, volume, NULL, NULL, 0, NULL);
    }

    objdata->unk32D = colliderFlags;
}

// offset: 0x2444 | func: 29
static void BWlog_handle_fx(Object* self, BWlog_Data* objdata) {
    f32 x;
    f32 z;
    f32 sin;
    f32 cos;
    f32 magnitude;
    Vec3f spE0;
    Vec3f particleConfig;
    Vec3f spC8;
    f32 temp_fv0;
    s32 particleCount;
    SRT srt;
    MtxF mtx;
    s32 i;

    x = (objdata->flowX[0] + objdata->flowX[1]) * 0.5f;
    z = (objdata->flowZ[0] + objdata->flowZ[1]) * 0.5f;
    magnitude = sqrtf(SQ(x) + SQ(z));
    if (magnitude != 0.0f) {
        x /= magnitude;
        z /= magnitude;
    }

    sin = fsin16_precise(self->srt.yaw);
    cos = -fcos16_precise(self->srt.yaw);
    spE0.y = cos;
    spE0.x = sin;
    particleConfig.x = -400.0f;
    particleConfig.y = 400.0f;
    srt.yaw = arctan2_f(x, z);
    srt.pitch = 0;
    srt.roll = 0;
    srt.scale = 1.0f;
    srt.transl.x = 0.0f;
    srt.transl.y = 0.0f;
    srt.transl.z = 0.0f;
    spC8.y = (objdata->velocity[0].x + objdata->velocity[1].x) * 0.5f;
    spC8.x = (objdata->velocity[0].z + objdata->velocity[1].z) * 0.5f;
    matrix_from_srt_reversed(&mtx, &srt);
    vec3_transform(&mtx, spC8.y, 0, spC8.x, &sin, &spC8.z, &spE0.z);
    magnitude *= 1.5f;
    magnitude -= spE0.z;
    if (magnitude < 0.0f) {
        magnitude = 0.0f;
    }

    particleCount = magnitude;
    particleConfig.z = (0.001f * magnitude) + 0.004f;
    srt.transl.x = (x * spE0.y) + (z * spE0.x);
    if (srt.transl.x < 0.0f) {
        for (i = 0; i < particleCount; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_76C, &srt, PARTFXFLAG_4, -1, &particleConfig);
        }
    } else {
        for (i = 0; i < particleCount; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_76C, &srt, PARTFXFLAG_4, -1, &particleConfig);
        }
    }
}
