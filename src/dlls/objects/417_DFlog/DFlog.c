#include "PR/os.h"
#include "dlls/engine/27.h"
#include "dlls/objects/common/weapon.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/418_DFriverflow.h"
#include "dlls/objects/419_DFdockpoint.h"
#include "dlls/objects/common/vehicle.h"
#include "sys/gfx/animseq.h"
#include "sys/gfx/modgfx.h"
#include "sys/joypad.h"
#include "sys/dll.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/objprint.h"
#include "sys/intersect.h"
#include "dll.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
} DFlog_Setup;

typedef struct {
    u8 _unk0[0x240 - 0x0];
    Vec3f endPoints[2];
    Vec3f velocity[2];
    f32 flowX[2];
    f32 flowZ[2];
    f32 unk280;
    f32 unk284;
    f32 unk288;
    DLL27_Data unk28C;
    u8 mountState;
    u8 paddleSoundCooldown;
    u8 unk4EE;
    s16 unk4F0[2];
    Object* dockpoint; // dockpoint
} DFlog_Data;

/*0x0*/ static DLL_IModgfx* dModGfxDLL = NULL;
/*0x4*/ static u32 _data_4[] = {
    0x00000000, 
    0x40000102, 0x00000000, 0x00000000, 
    0x00000000, 0x40010302, 0x00000000, 
    0x00000000, 0x00000000, 0x40020304, 
    0x00000000, 0x00000000, 0x00000000, 
    0x40030504, 0x00000000, 0x00000000, 
    0x00000000, 0x40040506, 0x00000000, 
    0x00000000, 0x00000000, 0x40050706, 
    0x00000000, 0x00000000, 0x00000000, 
    0x40060708, 0x00000000, 0x00000000, 
    0x00000000, 0x40070908, 0x00000000, 
    0x00000000, 0x00000000, 0x4008090a, 
    0x00000000, 0x00000000, 0x00000000, 
    0x40090b0a, 0x00000000, 0x00000000, 
    0x00000000, 0x400a0b0c, 0x00000000, 
    0x00000000, 0x00000000, 0x400b0d0c, 
    0x00000000, 0x00000000, 0x00000000
};
/*0xC8*/ static Vec3f _data_C8[] = {
    VEC3F(0.0f, 0.0f, -30.0f), 
    VEC3F(0.0f, 0.0f, 30.0f)
};
/*0xE0*/ static f32 _data_E0[] = {8.0f, 8.0f};
/*0xE8*/ static u8 _data_E8[] = {1, 1};
/*0xEC*/ static Vec3f _data_EC[] = {
    VEC3F(0.0f, 0.0f, -30.0f), 
    VEC3F(0.0f, 0.0f, 30.0f)
};
/*0x104*/ static f32 _data_104[] = {8.0f, 8.0f};

static int DFlog_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);
static void DFlog_func_A90(Object* self);
static void DFlog_findRiverflows(Object* self);
static void DFlog_func_E8C(Object* self);
static f32 DFlog_func_1E9C(Object* arg0, f32 arg1, f32 arg2, f32 arg3);

// offset: 0x0 | ctor
void DFlog_ctor(void* dll) { }

// offset: 0xC | dtor
void DFlog_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFlog_obj_Setup(Object* self, DFlog_Setup* setup, s32 reset) {
    DFlog_Data* objdata;
    s32 i;

    objdata = self->data;
    dModGfxDLL = dllLoad(DLL_ID_136, 1);

    objAddObjectType(self, OBJTYPE_Vehicle);

    self->animCallback = DFlog_animCallback;
    self->srt.yaw = (setup->yaw & 0xFFFF) << 8;
    
    gDLL_27->vtbl->init(&objdata->unk28C, 
        DLL27FLAG_NONE, 
        DLL27FLAG_40000 | DLL27FLAG_2, 
        DLL27MODE_1);
    gDLL_27->vtbl->setup_hits_collider(&objdata->unk28C, 2, _data_EC, _data_104, 8);
    gDLL_27->vtbl->setup_terrain_collider(&objdata->unk28C, 2, _data_C8, _data_E0, _data_E8);
    gDLL_27->vtbl->reset(self, &objdata->unk28C);

    self->objhitInfo->unk58 |= 1;
    self->objhitInfo->unk58 |= 4;

    objdata->mountState = VEHICLE_NoRider;
    objdata->paddleSoundCooldown = 0;
    objdata->unk4EE = 0;

    for (i = 0; i < 2; i++) {
        objdata->endPoints[i].x = self->srt.transl.x;
        objdata->endPoints[i].y = self->srt.transl.y;
        objdata->endPoints[i].z = self->srt.transl.z;
    }

    DFlog_func_A90(self);

    objdata->unk4F0[1] = 0;
    objdata->unk4F0[0] = 0;
    objdata->unk288 = 0.0f;
}

// offset: 0x208 | func: 1 | export: 1
void DFlog_obj_Control(Object* self) {
    DFlog_Data* objdata;
    Vec3f sp2C[2];

    objdata = self->data;
    func_8002674C(self);
    DFlog_func_A90(self);
    DFlog_findRiverflows(self);
    DFlog_func_E8C(self);
    // below is nonsense (and probably not whatever the original was)
    sp2C[1].x = objdata->endPoints[0].x;
    sp2C[1].y = objdata->endPoints[0].y;
    sp2C[1].z = objdata->endPoints[0].z;
    sp2C[0].x = -objdata->velocity[0].z * 10.0f;
    sp2C[0].y = 0.0f;
    sp2C[0].y = -objdata->velocity[0].y * 10.0f;
}

// offset: 0x2D8 | func: 2 | export: 2
void DFlog_obj_Update(Object* self) { }

// offset: 0x2E4 | func: 3 | export: 3
void DFlog_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x338 | func: 4 | export: 4
void DFlog_obj_Free(Object* self, s32 onlySelf) {
    dllFree(dModGfxDLL);
    objFreeObjectType(self, OBJTYPE_Vehicle);
}

// offset: 0x394 | func: 5 | export: 5
u32 DFlog_obj_GetModelFlags(Object* self) {
    return MODFLAGS_1;
}

// offset: 0x3A4 | func: 6 | export: 6
u32 DFlog_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DFlog_Data);
}

// offset: 0x3B8 | func: 7
static int DFlog_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    func_800267A4(self);
    return 0;
}

// offset: 0x400 | func: 8 | export: 7
s32 DFlog_vehicle_CanMount(Object* self, Object* player) {
    DFlog_Data* objdata = self->data;
    if ((objdata->mountState == VEHICLE_NoRider) && (objdata->dockpoint != NULL)) {
        return vec3Distance(&player->globalPosition, &self->globalPosition) < 50.0f;
    }
    return 0;
}

// offset: 0x490 | func: 9 | export: 8
VehicleMountSide DFlog_vehicle_GetMountSide(Object* self) {
    SRT srt;
    MtxF mtx;
    f32 x;
    f32 y;
    f32 z;
    f32 sign;
    f32 minusOrigin;
    Object* player;

    player = objGetPlayer();
    if (player != NULL) {
        //Get the log's directional unit vector
        srt.yaw = self->srt.yaw + M_90_DEGREES; //Plus 90, so pointing out its side
        srt.pitch = self->srt.pitch;
        srt.roll = self->srt.roll;
        srt.transl.x = 0.0f;
        srt.transl.y = 0.0f;
        srt.transl.z = 0.0f;
        srt.scale = 1.0f;
        mathYprXyzMtx(&mtx, &srt);
        mathMtxXFMF(&mtx, 0.0f, 0.0f, 1.0f, &x, &y, &z);

        //Check the player's position along the sideways axis, and get the side from the sign
        minusOrigin = -((self->srt.transl.x * x) + (y * self->srt.transl.y) + (z * self->srt.transl.z));
        sign = (player->srt.transl.x * x) + (y * player->srt.transl.y) + (z * player->srt.transl.z) + minusOrigin;
        if (sign < 0) {
            return VEHICLE_SIDE_Left;
        }
    }
    return VEHICLE_SIDE_Right;
}

// offset: 0x5D8 | func: 10 | export: 9
void DFlog_vehicle_GetRiderPosition(Object* self, f32* ox, f32* oy, f32* oz) {
    *ox = self->srt.transl.x;
    *oy = self->srt.transl.y;
    *oz = self->srt.transl.z;
}

// offset: 0x5F8 | func: 11 | export: 10
s32 DFlog_vehicle_CanDismount(Object* self, Object* player) {
    DFlog_Data* objdata;
    f32 endpointSpeeds;
    s32 camDLLID;
    s32 i;

    objdata = self->data;

    if ((objdata->dockpoint != NULL) && (objdata->mountState == VEHICLE_Mounted)) {
        camDLLID = gDLL_2_Camera->vtbl->get_dll_ID();
        if ((camDLLID != DLL_ID_CAM1STPERSON) && (camDLLID != DLL_ID_CAM1STPERSON2) && 
            (gDLL_1_cmdmenu->vtbl->was_any_item_used() == FALSE) && 
            (joyGetPressed(0) & B_BUTTON)
        ) {
            endpointSpeeds = 0.0f;
            for (i = 0; i < 2; i++) {
                endpointSpeeds += sqrtf(SQ(objdata->velocity[i].x) + SQ(objdata->velocity[i].z));
            }
            
            return endpointSpeeds < 2.0f;
        }
    }

    dll_player(player)->func28(player, 1);

    return FALSE;
}

// offset: 0x75C | func: 12 | export: 11
VehicleMountSide DFlog_vehicle_GetDismountSide(Object* self) {
    return 0;
}

// offset: 0x76C | func: 13 | export: 12
void DFlog_vehicle_GetCameraPosition(Object* self, f32* ox, f32* oy, f32* oz) {
    MtxF mtx;
    SRT srt;

    srt.transl.x = self->srt.transl.x;
    srt.transl.y = self->srt.transl.y;
    srt.transl.z = self->srt.transl.z;
    srt.yaw = self->srt.yaw;
    srt.pitch = self->srt.pitch;
    srt.roll = self->srt.roll;
    srt.scale = 1.0f;
    mathYprXyzMtx(&mtx, &srt);
    mathMtxXFMF(&mtx, 0.0f, 1.5f, -1.0f, ox, oy, oz);
    *ox = self->srt.transl.x;
    *oy = self->srt.transl.y + 30.0f;
    *oz = self->srt.transl.z;
}

// offset: 0x85C | func: 14 | export: 13
VehicleMountState DFlog_vehicle_GetMountState(Object* self) {
    DFlog_Data* objdata = self->data;
    return objdata->mountState;
}

// offset: 0x86C | func: 15 | export: 14
void DFlog_vehicle_SetMountState(Object* self, s32 mountState) {
    DFlog_Data* objdata = self->data;
    Object* player;

    player = objGetPlayer();
    objdata->mountState = mountState;

    if (mountState != VEHICLE_NoRider) {
        dll_player(player)->func28(player, 1);
        gDLL_2_Camera->vtbl->change_mode(0, 0x2B);
    } else {
        dll_player(player)->func29(player, 1);
        gDLL_2_Camera->vtbl->change_mode(0, 1);
    }
}

// offset: 0x944 | func: 16 | export: 15
void DFlog_vehicle_GetPlayerAnim(Object* self, f32* tValue, u32* animIndex) {
    f32 tilt = mathSinfInterp(self->srt.pitch);

    if (self->srt.pitch > 0) {
        *animIndex = 0;
    } else {
        *animIndex = 1;
    }

    if (tilt >= 0.0f) {
        *tValue = tilt;
    } else {
        *tValue = -tilt;
    }
}

// offset: 0x9D8 | func: 17 | export: 16
f32 DFlog_vehicle_Func16(Object* self, f32* a1) {
    DFlog_Data* objdata = self->data;
    f32 var_fa0;
    f32 var_fv1;

    var_fv1 = objdata->unk288 * 1.2f;

    if (objdata->unk288 >= 0.0f) {
        var_fa0 = objdata->unk288;
    } else {
        var_fa0 = -objdata->unk288;
    }
    *a1 = var_fa0 * 0.02f;

    if (var_fv1 < -1.0f) {
        var_fv1 = -1.0f;
    }
    if (var_fv1 > 1.0f) {
        var_fv1 = 1.0f;
    }

    return var_fv1;
}


// offset: 0xA64 | func: 18 | export: 17
s32 DFlog_vehicle_GetRacePosition(Object* self) {
    return 0;
}

// offset: 0xA74 | func: 19 | export: 18
void DFlog_vehicle_Func18(Object* self) { }

// offset: 0xA80 | func: 20 | export: 19
void DFlog_vehicle_HandleRiderScale(Object* self, f32 scale) { }

// offset: 0xA90 | func: 21
static void DFlog_func_A90(Object* self) {
    DFlog_Data* objdata;
    DLL27_Data* d27data;
    SRT srt;
    MtxF mtx;
    s32 i;
    s32 j;

    objdata = (DFlog_Data*)self->data;

    srt.yaw = self->srt.yaw;
    srt.pitch = self->srt.pitch;
    srt.roll = self->srt.roll;
    srt.scale = 1.0f;
    srt.transl.x = self->srt.transl.x;
    srt.transl.y = self->srt.transl.y;
    srt.transl.z = self->srt.transl.z;
    mathYprXyzMtx(&mtx, &srt);

    d27data = &objdata->unk28C;

    for (i = 0, j = 0; i < (d27data->numTestPoints & 0xF); i++, j++) {
        mathMtxXFMF(&mtx, 
            _data_EC[j].x, _data_EC[j].y, _data_EC[j].z, 
            &objdata->endPoints[i].x, &objdata->endPoints[i].y, &objdata->endPoints[i].z);
    }
}

// offset: 0xBC8 | func: 22
static void DFlog_findRiverflows(Object* self) {
    DFlog_Data* objdata;
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

    objdata = (DFlog_Data*)self->data;

    for (i = 0; i < 2; i++) {
        objdata->flowX[i] = 0.0f;
        objdata->flowZ[i] = 0.0f;
        flowInfluences[i] = 0;
    }

    // Get DFriverflow objects
    objList = objGetAllOfType(OBJTYPE_Riverflow, &objListLength);

    for (i = 0; i < objListLength; i++) {
        obj = objList[i];
        //Check that the riverflow is intended to affect logs (NOTE: BWlog doesn't check this)
        if (((DFriverflow_Setup*)obj->setup)->flags & 1) {
            for (k = 0; k < 2; k++) {
                dy = obj->srt.transl.y - objdata->endPoints[k].y;
                if ((dy <= 200.0f) && (dy >= -200.0f)) {
                    dx = obj->srt.transl.x - objdata->endPoints[k].x;
                    dz = obj->srt.transl.z - objdata->endPoints[k].z;
                    dx = sqrtf(SQ(dx) + SQ(dz));
                    pushRadius = ((DFriverflow_Setup*)obj->setup)->range * 1.5f;
                    if (dx < pushRadius) {
                        dx = (pushRadius - dx) / pushRadius;
                        dx *= obj->srt.scale * 10.0f;
                        objdata->flowX[k] += mathSinfInterp(obj->srt.yaw) * dx;
                        objdata->flowZ[k] += mathCosfInterp(obj->srt.yaw) * dx;
                        flowInfluences[k]++;
                    }
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
}

// offset: 0xE8C | func: 23
static void DFlog_func_E8C(Object* self) {
    f32 var_fa0;
    f32 var_fa1;
    SRT fxTransform;
    f32 dz;
    f32 dy;
    f32 dx;
    f32 spD0;
    f32 spC8[2];
    f32 spC4;
    DFdockpoint_Setup* dockpointSetup;
    f32 distance;
    Object* weapon;
    Vec3f spAC;
    Vec3f spA0;
    f32 sp9C;
    f32 sp98;
    s32 sp90[2];
    u8 soundFlags;
    u8 temp_s2;
    u8 volume;
    s32 i;
    DFlog_Data* objdata;
    f32 damp;

    objdata = (DFlog_Data*)self->data;
    volume = 0;

    distance = 10000.0f;
    objdata->dockpoint = objGetNearestTypeTo(OBJTYPE_Dockpoint, self, &distance);
    if (objdata->dockpoint != NULL) {
        dockpointSetup = (DFdockpoint_Setup*)objdata->dockpoint->setup;
        distance = vec3Distance(&self->globalPosition, &objdata->dockpoint->globalPosition);
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

    if ((objdata->mountState == VEHICLE_Mounted) && (gDLL_2_Camera->vtbl->get_dll_ID() != DLL_ID_CAM1STPERSON2)) {
        objdata->unk280 = joyGetStickX(0) * 0.01f;
        if (joyGetPressed(0) & A_BUTTON) {
            var_fa0 = ((1.3f - objdata->unk288) / 1.3f) * 0.8f;
        } else {
            var_fa0 = -0.05f;
        }
        objdata->unk288 += var_fa0;

        if (joyGetPressed(0) & A_BUTTON) {
            objdata->velocity[0].y = objdata->velocity[0].y + 0.08f;
            objdata->velocity[1].y = objdata->velocity[1].y - 0.06f;
        }

        if (joyGetPressed(0) & A_BUTTON) {
            var_fa0 = 0.08f;
        } else {
            var_fa0 = -0.02f;
        }
        objdata->unk284 -= var_fa0;

        if (objdata->unk288 < 0.0f) {
            objdata->unk288 = 0.0f;
        } else {
            if (objdata->unk288 > 1.3f) {
                var_fa0 = 1.3f;
            } else {
                var_fa0 = objdata->unk288;
            }
            objdata->unk288 = var_fa0;
        }

        if (objdata->unk284 < -0.1f) {
            objdata->unk284 = -0.1f;
        } else {
            if (objdata->unk284 > 0.0f) {
                var_fa0 = 0.0f;
            } else {
                var_fa0 = objdata->unk284;
            }
            objdata->unk284 = var_fa0;
        }
    }

    spC4 = mathSinfInterp(self->srt.yaw) * objdata->unk284;
    sp98 = mathCosfInterp(self->srt.yaw) * objdata->unk284;
    objdata->velocity[0].x += spC4;
    objdata->velocity[0].z += sp98;
    objdata->velocity[1].x += spC4;
    objdata->velocity[1].z += sp98;
    spC4 = mathSinfInterp(self->srt.yaw + M_90_DEGREES) * objdata->unk280;
    sp98 = mathCosfInterp(self->srt.yaw + M_90_DEGREES) * objdata->unk280;
    objdata->velocity[0].x += spC4 * 0.05f;
    objdata->velocity[0].z += sp98 * 0.05f;
    objdata->velocity[1].x -= spC4 * 0.025f;
    objdata->velocity[1].z -= sp98 * 0.025f;
    spD0 = DFlog_func_1E9C(self, self->srt.transl.y, self->srt.transl.x, self->srt.transl.z);
    
    for (i = 0; i < 2; i++) {
        objdata->velocity[i].x -= objdata->flowX[i] * 0.05f;
        objdata->velocity[i].z -= objdata->flowZ[i] * 0.050f;
        objdata->velocity[i].x *= 0.99f;
        objdata->velocity[i].z *= 0.99f;
        distance = sqrtf(SQ(objdata->velocity[i].x) + SQ(objdata->velocity[i].z));
        if (distance > 0.95f) {
            distance = 0.95f / distance;
            objdata->velocity[i].x *= distance;
            objdata->velocity[i].z *= distance;
        }
        spC8[i] = DFlog_func_1E9C(self, objdata->endPoints[i].y, objdata->endPoints[i].x, objdata->endPoints[i].z);
        if (((spC8[i] - objdata->endPoints[i].y) > -2.0f) && ((spC8[i] - objdata->endPoints[i].y) < 2.0f)) {
            var_fa0 = spD0 - self->srt.transl.y;
            if ((var_fa0 > -2.0f) && (var_fa0 < 2.0f)) {
                objdata->velocity[i].y += ((spC8[i] - objdata->endPoints[i].y) * 0.02f);
                objdata->velocity[i].y *= 0.95f;
                sp90[i] = 1;
                if (objdata->unk4F0[i] > 0) {
                    sp90[i] = objdata->unk4F0[i];
                }
                objdata->unk4F0[i] >>= 1;
            }
        } else {
            if ((spC8[i] - objdata->endPoints[i].y) > 2.0f) {
                if (objdata->velocity[i].y > 0.0f) {
                    var_fa0 = objdata->velocity[i].y;
                } else {
                    var_fa0 = -objdata->velocity[i].y;
                }
                objdata->velocity[i].y -= objdata->velocity[i].y * var_fa0 * 0.1f;
                objdata->velocity[i].y += (spC8[i] - objdata->endPoints[i].y) * 0.01f;
                sp90[i] = 1;
                if (objdata->unk4F0[i] > 0) {
                    sp90[i] = objdata->unk4F0[i];
                }
                objdata->unk4F0[i] >>= 1;
            } else {
                if ((spC8[i] - objdata->endPoints[i].y) < -2.0f) {
                    objdata->velocity[i].y *= 0.95f;
                    objdata->velocity[i].y -= 0.18f;
                    objdata->unk4F0[i] += gUpdateRate;
                } else {
                    objdata->endPoints[i].y = spC8[i];
                    sp90[i] = 1;
                    if (objdata->unk4F0[i] > 0) {
                        sp90[i] = objdata->unk4F0[i];
                    }
                    objdata->unk4F0[i] >>= 1;
                }
            }
        }
        objdata->endPoints[i].x += objdata->velocity[i].x * gUpdateRateF;
        objdata->endPoints[i].y += objdata->velocity[i].y * gUpdateRateF;
        objdata->endPoints[i].z += objdata->velocity[i].z * gUpdateRateF;
    }

    self->srt.transl.x = (objdata->endPoints[0].x + objdata->endPoints[1].x) * 0.5f;
    self->srt.transl.y = (objdata->endPoints[0].y + objdata->endPoints[1].y) * 0.5f;
    self->srt.transl.z = (objdata->endPoints[0].z + objdata->endPoints[1].z) * 0.5f;
    dx = objdata->endPoints[1].x - objdata->endPoints[0].x;
    dy = objdata->endPoints[1].y - objdata->endPoints[0].y;
    dz = objdata->endPoints[1].z - objdata->endPoints[0].z;
    distance = sqrtf(SQ(dx) + SQ(dz));
    self->srt.pitch = -mathAtan2f(dy, distance);
    self->srt.yaw = mathAtan2f(dx, dz);
    gDLL_27->vtbl->func_1E8(self, &objdata->unk28C, gUpdateRateF);
    gDLL_27->vtbl->func_5A8(self, &objdata->unk28C);
    gDLL_27->vtbl->func_624(self, &objdata->unk28C, gUpdateRateF);

    temp_s2 = (objdata->unk28C.hitsTouchBits | objdata->unk28C.unk25C) & 3;
    soundFlags = temp_s2 & (temp_s2 ^ objdata->unk4EE);
    if (soundFlags & 1) {
        volume = (sqrtf(SQ(objdata->velocity[0].x) + SQ(objdata->velocity[0].z)) * 127.0f) / 0.95f;
    }
    if (soundFlags & 2) {
        sp9C = sqrtf(SQ(objdata->velocity[1].x) + SQ(objdata->velocity[1].z));
        if (((sp9C * 127.0f) / 0.95f) < volume) {
            volume = (f32) volume;
        } else {
            volume = ((sqrtf(SQ(objdata->velocity[1].x) + SQ(objdata->velocity[1].z)) * 127.0f) / 0.95f);
        }
    }
    if (volume > 10) {
        dll_amSfx->Play(self, SOUND_76D_Log_Bump, volume, NULL, NULL, 0, NULL);
    }
    objdata->unk4EE = temp_s2;

    if ((objdata->mountState == VEHICLE_Mounted) && (sp90[0] != 0) && (sp90[1] != 0)) {
        weapon = objGetPlayer()->linkedObject;
        if (weapon != NULL) {
            ((DLL_IWeapon*)weapon->dll)->vtbl->func17(weapon, &spAC, &spA0);
            sp9C = (spAC.y - spD0) / (spAC.y - spA0.y);
            if ((sp9C >= 0.0f) && (sp9C <= 1.0f)) {
                if (objdata->paddleSoundCooldown == 0) {
                    dll_amSfx->Play(self, SOUND_8F_Water_Paddle, MAX_VOLUME, NULL, NULL, 0, NULL);
                    objdata->paddleSoundCooldown = 2;
                }
                fxTransform.transl.x = ((spA0.x - spAC.x) * sp9C) + spAC.x;
                fxTransform.transl.y = spC8[0];
                fxTransform.transl.z = ((spA0.z - spAC.z) * sp9C) + spAC.z;
                sp98 = sqrtf(SQ(objdata->velocity[i].x) + SQ(objdata->velocity[i].z)) * 5.0f;
                if (sp98 < 0.0f) {
                    fxTransform.scale = 0.0f;
                } else {
                    if (sp98 > 1.0f) {
                        var_fa0 = 1.0f;
                    } else {
                        var_fa0 = sp98;
                    }
                    fxTransform.scale = var_fa0;
                }
                fxTransform.yaw = (mathAtan2f(objdata->endPoints[0].x - objdata->endPoints[1].x, objdata->endPoints[0].z - objdata->endPoints[1].z) + M_180_DEGREES) & 0xFFFF & 0xFFFF;
                fxTransform.transl.x -= self->srt.transl.x;
                fxTransform.transl.y -= self->srt.transl.y;
                fxTransform.transl.z -= self->srt.transl.z;
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3C4, &fxTransform, PARTFXFLAG_NONE, -1, NULL);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3C5, &fxTransform, PARTFXFLAG_NONE, -1, NULL);
            } else if (objdata->paddleSoundCooldown) {
                objdata->paddleSoundCooldown >>= 1;
            }
        }
    }
    
    for (i = 0; i < 2; i++) {
        if (sp90[i] != 0) {
            if (sp90[i] > 10) {
                if (self->srt.transl.y != objdata->endPoints[i].y) {
                    sp9C = (objdata->endPoints[i].y - spC8[i]) / (objdata->endPoints[i].y - self->srt.transl.y);
                } else {
                    sp9C = 1.0f;
                }
                if (sp9C < 0.0f){
                    sp9C = 0.0f;
                }
                if (sp9C > 1.0f) {
                    sp9C = 1.0f;
                }
            }
            if (mathRnd(0, 5) == 0) {
                fxTransform.transl.x = objdata->endPoints[i].x;
                fxTransform.scale = sqrtf(SQ(objdata->velocity[i].x) + SQ(objdata->velocity[i].z));
                if (fxTransform.scale > 0.1f) {
                    fxTransform.transl.y = self->srt.transl.y;
                    fxTransform.transl.z = objdata->endPoints[i].z;
                    fxTransform.yaw = (mathAtan2f(objdata->velocity[i].x, objdata->velocity[i].z) + M_180_DEGREES) & 0xFFFF & 0xFFFF;
                    fxTransform.transl.x -= self->srt.transl.x;
                    fxTransform.transl.y -= self->srt.transl.y;
                    fxTransform.transl.z -= self->srt.transl.z;
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_3C4, &fxTransform, PARTFXFLAG_NONE, -1, NULL);
                }
            }
        }
    }
}

// offset: 0x1E9C | func: 24
static f32 DFlog_func_1E9C(Object* self, f32 y, f32 x, f32 z) {
    TrackHeightResult** trackResults;
    TrackHeightResult* closestResult;
    f32 trackHeightDefaultY;
    f32 waterY;
    f32 yDiff;
    f32 minYDist;
    f32 minYDistWater;
    f32 yDistance;
    f32 yDist;
    s32 count;
    s32 minYDistWaterIdx;
    s32 minYDistIdx;
    s32 i;

    trackHeightDefaultY = 2.0f * y;
    if (trackHeightDefaultY >= 0.0f) {
        waterY = -trackHeightDefaultY;
    } else {
        waterY = -(-trackHeightDefaultY);
    }

    count = trackGetHeight(self, x, self->srt.transl.y, z, &trackResults, 0, 0);
    if (count != 0) {
        minYDistIdx = 0;
        minYDistWater = 0.0f;
        minYDistWaterIdx = -1;
        if (y >= trackResults[0]->y) {
            minYDist = y - trackResults[0]->y;
        } else {
            minYDist = -(y - trackResults[0]->y);
        }

        for (i = 0; i < count; i++) {
            //Check closest water
            if (trackResults[i]->unk14 == 14) { //water
                if (trackResults[i]->y <= y) {
                    yDist = y - trackResults[i]->y;
                } else {
                    yDist = -(y - trackResults[i]->y);
                }
                if (minYDistWater > yDist || minYDistWaterIdx == -1) {
                    minYDistWater = yDist;
                    minYDistWaterIdx = i;
                }
            }

            //Check closest terrain in general
            yDist = y - trackResults[i]->y;
            if (trackResults[i]->y <= y) {
                yDistance = yDist;
            } else {
                yDistance = -yDist;
            }
            if (minYDist > yDistance) {
                minYDist = yDistance;
                minYDistIdx = i;
            }
        }
        
        if (minYDistWaterIdx != -1) {
            waterY = trackResults[minYDistWaterIdx]->y;
        }

        closestResult = trackResults[minYDistIdx];
        yDiff = y - closestResult->y;

        //Check if the closest water is under another surface
        if (waterY < closestResult->y) {
            if (yDiff >= 0.0f) {
                yDist = yDiff;
            } else {
                yDist = -yDiff;
            }

            if ((yDist < 10.0f) && (closestResult->unk14 != 0xE)) {
                waterY = closestResult->y + 4.0f;
            }
        }
    } else {
        waterY = y;
    }

    return waterY;
}

// offset: 0x21E4 | func: 25 | export: 20
void DFlog_vehicle_Func20(Object* self, s32 a1, s32 a2) { }
