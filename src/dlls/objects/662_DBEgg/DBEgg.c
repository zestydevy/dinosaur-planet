#include "dlls/engine/26_curves.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/319_fishingnet.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/setups/55_Riverflow.h"
#include "game/objects/setups/6_Collectable.h"
#include "game/objects/interaction_arrow.h"
#include "sys/joypad.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/print.h"
#include "dll.h"

typedef struct {
/*000*/ f32 yOffset;
/*004*/ UnkCurvesStruct unk4;
/*10C*/ u8 state; // DBEggState
/*10D*/ u8 flags; // DBEggFlags
/*10E*/ s16 unk10E;
} DBEgg_Data;

enum DBEggState {
    DB_EGG_STATE_1 = 1,
    DB_EGG_STATE_Floating = 2,
    DB_EGG_STATE_Collected = 3,
    DB_EGG_STATE_4 = 4,
    DB_EGG_STATE_5 = 5,
    DB_EGG_STATE_6 = 6,
    DB_EGG_STATE_7 = 7,
    DB_EGG_STATE_8 = 8,
    DB_EGG_STATE_9 = 9,
    DB_EGG_STATE_10 = 10,
    DB_EGG_STATE_11 = 11,
    DB_EGG_STATE_12 = 12
};

enum DBEggFlags {
    DB_EGG_FLAG_UNK1 = 0x1,
    DB_EGG_FLAG_UNK2 = 0x2,
    DB_EGG_FLAG_UNK4 = 0x4,
};

enum DBEggDepthCheckMode {
    DB_EGG_CHECK_WATER = 0,
    DB_EGG_CHECK_FLOOR = 1
};

static void DBEgg_getRiverflowInfluence(Object* self, Vec3f* velocity);
static s32 DBEgg_depthCheck(Object* self, f32* depth, f32 xOffset, f32 zOffset, s32 mode);
static void DBEgg_collect(Object* self);
static void DBEgg_init(Object* self, DBEgg_Data* objData);
static void DBEgg_processObjMessages(Object* self);

// offset: 0x0 | ctor
void DBEgg_ctor(void *dll) { }

// offset: 0xC | dtor
void DBEgg_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBEgg_obj_Setup(Object* self, Collectable_Setup* setup, s32 arg2) {
    DBEgg_init(self, self->data);
    objInitMesgQueue(self, 4);
}

// offset: 0x7C | func: 1 | export: 1
void DBEgg_obj_Control(Object* self) {
    Collectable_Setup* setup = (Collectable_Setup*)self->setup;
    Object* player = objGetPlayer();
    s32 _pad;
    DBEgg_Data* objdata = self->data;
    f32 depth;
    Vec3f riverflowVelocity;
    s32 _pad2;
    s16 targetPitch;
    s16 targetRoll;
    Object* fishingnet;
    s32 curveTypes[2] = {1, 0};

    if (mapWorldCoordsToBlockIndex(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z) == -1) {
        return;
    }
    DBEgg_processObjMessages(self);
    self->objhitInfo->unk58 &= ~ObjHitInfo_FLAG_400;
    switch (objdata->state) {
    case DB_EGG_STATE_5:
        if (self->unkE0 == 0) {
            self->objhitInfo->unk58 |= ObjHitInfo_FLAG_1;
        }
        if (!DBEgg_depthCheck(self, &depth, 0.0f, 0.0f, DB_EGG_CHECK_FLOOR)) {
            // In water
            objdata->state = DB_EGG_STATE_Floating;
            break;
        }
        if (FABS_EXPR(depth) < 0.05f) {
            // On floor
            objdata->state = DB_EGG_STATE_1;
            self->velocity.x = 0;
            self->velocity.z = 0;
            break;
        }
        self->velocity.y += -0.5f; // gravity
        if (depth > 0) {
            self->velocity.y = -self->velocity.y * 0.35f;
        }
        objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);
        break;
    case DB_EGG_STATE_1:
        if (self->unkE0 == 0) {
            self->objhitInfo->unk58 |= ObjHitInfo_FLAG_1;
        }
        break;
    case DB_EGG_STATE_Floating:
        if (objdata->flags & DB_EGG_FLAG_UNK4) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
            self->velocity.x += (setup->base.x - self->srt.transl.x) / 1000.0f;
            self->velocity.y += (setup->base.y - self->srt.transl.y) / 1000.0f;
            self->velocity.z += (setup->base.z - self->srt.transl.z) / 1000.0f;
            if (mainGetBits(BIT_44D) != 0) {
                objdata->state = DB_EGG_STATE_10;
            }
        }
        self->objhitInfo->unk58 |= ObjHitInfo_FLAG_400;
        riverflowVelocity.z = 0;
        riverflowVelocity.y = 0;
        riverflowVelocity.x = 0;
        DBEgg_getRiverflowInfluence(self, &riverflowVelocity);
        self->velocity.x += riverflowVelocity.x;
        self->velocity.y += riverflowVelocity.y;
        self->velocity.z += riverflowVelocity.z;
        if (DBEgg_depthCheck(self, &depth, self->velocity.x * gUpdateRateF, self->velocity.z * gUpdateRateF, DB_EGG_CHECK_FLOOR)) {
            // On floor
            self->velocity.x *= -0.95f;
            self->velocity.z *= -0.95f;
            DBEgg_depthCheck(self, &depth, self->velocity.x * gUpdateRateF, self->velocity.z * gUpdateRateF, DB_EGG_CHECK_FLOOR);
        }
        depth += objdata->yOffset;
        if (gUpdateRateInverseF != 0) {
            self->velocity.y = gUpdateRateInverseF * 0.3f * depth;
        } else {
            self->velocity.y = 0.0f;
        }
        targetPitch = mathRnd(100, 5000) * depth;
        targetRoll = mathRnd(100, 5000) * depth;
        self->srt.pitch += ((targetPitch - self->srt.pitch) >> 8);
        self->srt.roll += ((targetRoll - self->srt.roll) >> 8);
        objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);
        if (mathRnd(0, 10) == 0) {
            // @bug? what is this nonsense conditional?
            if (ABS_EXPR(depth < 0.05f) != 0) {
                gDLL_24_Waterfx->vtbl->spawn_circular_ripple(
                    self->srt.transl.x, 
                    self->srt.transl.y - objdata->yOffset, 
                    self->srt.transl.z, 
                    self->srt.yaw, 
                    mathRnd(1, 10), 
                    1);
            }
        }
        if (mainGetBits(BIT_426) != 0) {
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
            objdata->yOffset -= (0.1f * gUpdateRateF);
            if (objdata->yOffset < -7.0f) {
                mainSetBits(BIT_428, mainGetBits(BIT_428) + 1);
                objdata->state = DB_EGG_STATE_7;
                self->velocity.y = 0;
                self->velocity.x = 0;
                self->velocity.z = 0;
                self->unkAF |= ARROW_FLAG_8_No_Targetting;
            }
        } else if (objdata->flags & DB_EGG_FLAG_UNK2) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
        }
        break;
    case DB_EGG_STATE_4:
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        fishingnet = dll_player(player)->func78(player);
        if (((DLL_319_fishingnet*)fishingnet->dll)->vtbl->State(fishingnet, FISHINGNET_GetFlags, NULL) == FISHINGNET_FLAG_Unk4) {
            objdata->state = DB_EGG_STATE_5;
            self->velocity.x = 0;
            self->velocity.z = 0;
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        }
        break;
    case DB_EGG_STATE_6:
        if ((vec3DistanceXZ(&self->globalPosition, (Vec3f* ) &setup->base.x) > 150.0f) 
                && !(objdata->flags & DB_EGG_FLAG_UNK2)) {
            DBEgg_collect(self);
            self->unkE0 = 0;
            break;
        }
        if (joyGetPressed(0) & A_BUTTON) {
            objdata->state = DB_EGG_STATE_5;
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
            break;
        }
        self->objhitInfo->unk58 &= ~ObjHitInfo_FLAG_1;
        objSendMesg(player, 0x100008, self, (void* )0x38000);
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        break;
    case DB_EGG_STATE_11:
        diPrintf(" Stolen ");
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        break;
    case DB_EGG_STATE_7:
        DBEgg_depthCheck(self, &depth, 0.0f, 0.0f, DB_EGG_CHECK_WATER);
        if (FABS_EXPR(depth) < 0.05f) {
            objdata->state = DB_EGG_STATE_8;
            self->velocity.x = 0;
            self->velocity.z = 0;
            break;
        }
        self->velocity.y += -0.01f;
        if (depth > 0) {
            self->velocity.y = -self->velocity.y * 0.4f;
        }
        objMove(self, self->velocity.x, self->velocity.y, self->velocity.z);
        break;
    case DB_EGG_STATE_8:
        if (mainGetBits(BIT_42A) != 0) {
            DBEgg_init(self, objdata);
            break;
        }
        if (mathRnd(0, 10) == 0) {
            dll_partfx->spawn(self, PARTICLE_3BE, NULL, 0, -1, NULL);
        }
        break;
    case DB_EGG_STATE_10:
        if (gDLL_26_Curves->vtbl->func_4288(&objdata->unk4, self, 300.0f, curveTypes, 2) != 0) {
            objdata->state = DB_EGG_STATE_5;
        } else {
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
            objdata->state = DB_EGG_STATE_9;
            if (objdata->flags & DB_EGG_FLAG_UNK4) {
                objdata->flags &= ~DB_EGG_FLAG_UNK4;
            }
        }
        break;
    case DB_EGG_STATE_9:
        if ((curves_func_800053B0(&objdata->unk4.unk0, 0.6f) != 0) || (objdata->unk4.unk0.unk10 != 0)) {
            if (gDLL_26_Curves->vtbl->func_4704(&objdata->unk4) != 0) {
                objdata->state = DB_EGG_STATE_5;
            }
            break;
        }
        self->velocity.x = objdata->unk4.unk0.unk68.x - self->srt.transl.x;
        self->velocity.y = objdata->unk4.unk0.unk68.y - self->srt.transl.y;
        self->velocity.z = objdata->unk4.unk0.unk68.z - self->srt.transl.z;
        if ((4.6f * gUpdateRateF) < sqrtf(SQ(self->velocity.f[0]) + SQ(self->velocity.f[1]) + SQ(self->velocity.f[2]))) {
            vec3Normalize(&self->velocity);
            self->velocity.x *= 4.6f * gUpdateRateF;
            self->velocity.y *= 4.6f * gUpdateRateF;
            self->velocity.z *= 4.6f * gUpdateRateF;
            diPrintf(" GREATER \n");
        }
        self->srt.transl.x = self->velocity.x + self->srt.transl.x;
        self->srt.transl.y = self->velocity.y + self->srt.transl.y;
        self->srt.transl.z = self->velocity.z + self->srt.transl.z;
        break;
    case DB_EGG_STATE_12:
        if (mainGetBits(setup->gamebitSecondary) != 0) {
            objdata->state = DB_EGG_STATE_5;
        }
        break;
    }
    if ((self->unkAF & ARROW_FLAG_1_Interacted) 
            && (mainGetBits(BIT_DB_Picked_Up_Egg) == 0) 
            && (vec3DistanceXZ(&self->globalPosition, &player->globalPosition) < 25.0f)) {
        if (!(objdata->flags & DB_EGG_FLAG_UNK1)) {
            DBEgg_collect(self);
        } else if (ABS_EXPR(self->srt.transl.y - player->srt.transl.y) < 12.0f) {
            self->unkAF |= ARROW_FLAG_8_No_Targetting;
            objdata->state = DB_EGG_STATE_6;
            self->objhitInfo->unk58 &= ~ObjHitInfo_FLAG_1;
        }
    }
}

// offset: 0xD28 | func: 2 | export: 2
void DBEgg_obj_Update(Object* self) {
    s32 damageType;
    DBEgg_Data* objData;
    Object* player;
    Object* fishingnet;

    damageType = func_80025F40(self, NULL, NULL, NULL);
    
    objData = self->data;
    
    if ((damageType == Damage_Type_Fishing_Net) && (objData->state != DB_EGG_STATE_4)) {
        player = objGetPlayer();
        fishingnet = dll_player(player)->func78(player);
        if (((DLL_319_fishingnet*)fishingnet->dll)->vtbl->State(fishingnet, FISHINGNET_GetFlags, NULL) != FISHINGNET_FLAG_Unk2) {
            self->objhitInfo->unk58 &= ~ObjHitInfo_FLAG_1;
            ((DLL_319_fishingnet*)fishingnet->dll)->vtbl->State(fishingnet, FISHINGNET_FLAG_Unk2, self);
            objData->state = DB_EGG_STATE_4;
            STUBBED_PRINTF("Caught");
        }
    }
    
    if ((objData->state != DB_EGG_STATE_9) && (trackGetLineIntersect(&self->prevLocalPosition, &self->srt.transl, 8.0f, 1, NULL, self, 8, -1, 0xFF, 0) != 0)) {
        self->velocity.x -= (0.95f * self->velocity.x);
        self->velocity.z -= (0.95f * self->velocity.z);
        STUBBED_PRINTF(" Hit Line ");
    }
    
    self->prevLocalPosition.x = self->srt.transl.x;
    self->prevLocalPosition.y = self->srt.transl.y;
    self->prevLocalPosition.z = self->srt.transl.z;
}

// offset: 0xED0 | func: 3 | export: 3
void DBEgg_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** pols, s8 visibility) {
    DBEgg_Data* objdata = self->data;

    if (visibility) {
        if ((objdata->state != DB_EGG_STATE_12) && (objdata->state != DB_EGG_STATE_4) && (objdata->state != DB_EGG_STATE_11)) {
            objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0xF44 | func: 4 | export: 4
void DBEgg_obj_Free(Object* self, s32 a1) {
    objFreeObjectType(self, OBJTYPE_38);
}

// offset: 0xF84 | func: 5 | export: 5
u32 DBEgg_obj_GetModelFlags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xF94 | func: 6 | export: 6
u32 DBEgg_obj_GetDataSize(Object *self, u32 offsetAddr) {
    return sizeof(DBEgg_Data);
}

// offset: 0xFA8 | func: 7
// Get swept away by DFriverflow objects
static void DBEgg_getRiverflowInfluence(Object* self, Vec3f* velocity) {
    DBEgg_Data* objData;
    Object* riverFlow;
    s32 count;
    Object** objects;
    s32 i;
    f32 delta;
    f32 dy;
    f32 dz;
    f32 range;
    f32 accelerationX;
    f32 accelerationZ;

    objData = self->data;

    accelerationX = 0.0f;
    accelerationZ = 0.0f;

    for (objects = objGetAllOfType(OBJTYPE_Riverflow, &count), i = 0; i < count; i++) {
        riverFlow = objects[i];
        dy = riverFlow->srt.transl.y - self->srt.transl.y;
        if ((dy <= 7.0f) && (dy >= -7.0f)) {
            delta = riverFlow->srt.transl.x - self->srt.transl.x;
            dz = riverFlow->srt.transl.z - self->srt.transl.z;
            delta = sqrtf(SQ(delta) + SQ(dz));
            
            range = ((Riverflow_Setup*)riverFlow->setup)->range * 1.5f;
            if (delta < range) {
                delta = (range - delta) / range;
                delta *= riverFlow->srt.scale * 10.0f;
                
                accelerationX += mathSinfInterp(riverFlow->srt.yaw) * delta;
                accelerationZ += mathCosfInterp(riverFlow->srt.yaw) * delta;
            }
        }
    }
    
    if (count != 0) {
        accelerationX /= count;
        accelerationZ /= count;
        velocity->x -= (accelerationX * 0.05f);
        velocity->z -= (accelerationZ * 0.05f);
        velocity->x *= 0.99f;
        velocity->z *= 0.99f;
        
        delta = sqrtf(SQ(velocity->z) + SQ(velocity->x));
        if (delta > 1.85f) {
            velocity->x *= 1.85f / delta;
            velocity->z *= 1.85f / delta;
        }
    }
}

// offset: 0x1240 | func: 8
static s32 DBEgg_depthCheck(Object* self, f32* depth, f32 xOffset, f32 zOffset, s32 mode) {
    f32 floorDepth;
    f32 waterDepth;
    f32 depthTmp;
    s32 i;
    s32 heightsCount;
    TrackHeightResult** heights;

    *depth = 0.0f;
    heightsCount = trackGetHeight(self, 
        self->srt.transl.x + xOffset, 
        self->srt.transl.y, 
        self->srt.transl.z + zOffset, 
        &heights, 0, 0);
    if (heightsCount != 0) {
        floorDepth = 10000.0f;
        waterDepth = 10000.0f;
        for (i = 0; i < heightsCount; i++) {
            depthTmp = heights[i]->y - self->srt.transl.y;
            if (heights[i]->unk14 == 0xE) {
                // Water
                if (ABS_EXPR(depthTmp) < ABS_EXPR(waterDepth)) {
                    waterDepth = depthTmp;
                }
            } else {
                if (ABS_EXPR(depthTmp) < ABS_EXPR(floorDepth)) {
                    floorDepth = depthTmp;
                }
            }
        }
        if (mode == DB_EGG_CHECK_WATER) {
            if (floorDepth != 10000.0f) {
                *depth = floorDepth;
                return FALSE;
            }
            if (waterDepth != 10000.0f) {
                *depth = waterDepth;
                return TRUE;
            }
            *depth = 1000.0f;
        } else { // DB_EGG_CHECK_FLOOR
            if (waterDepth != 10000.0f) {
                if ((ABS_EXPR(waterDepth) <= ABS_EXPR(floorDepth)) || waterDepth > 0.0f) {
                    *depth = waterDepth;
                    return FALSE;
                } else {
                    *depth = floorDepth;
                    return TRUE;
                }
            } else if (floorDepth != 10000.0f) {
                *depth = floorDepth;
                return TRUE;
            }
            *depth = 1000.0f;
        }
    }
    return FALSE;
}

// offset: 0x1670 | func: 9
static void DBEgg_collect(Object* self) {
    Object* player;
    DBEgg_Data* objdata;
    Collectable_Setup* setup;

    player = objGetPlayer();
    objdata = self->data;
    setup = (Collectable_Setup*)self->setup;

    objFreeObjectType(self, OBJTYPE_38);
    objdata->state = DB_EGG_STATE_Collected;
    mainSetBits(BIT_DB_Picked_Up_Egg, 1);
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    mainSetBits(setup->gamebitCollected, 1);

    objSendMesg(player, 0x7000A, self, (void* )0x10000);
}

// offset: 0x1744 | func: 10
static void DBEgg_init(Object* self, DBEgg_Data* objData) {
    f32 sp3C;
    Collectable_Setup* objSetup;

    objSetup = (Collectable_Setup*)self->setup;
    
    self->srt.transl.x = objSetup->base.x;
    self->srt.transl.y = objSetup->base.y;
    self->srt.transl.z = objSetup->base.z;
    
    objData->flags = 0;
    self->srt.yaw = objSetup->yaw8 << 8;
    self->srt.pitch = objSetup->pitch8 << 8;
    self->srt.roll = objSetup->roll8 << 8;

    self->srt.scale = objSetup->scale * (1.0f / 64.0f);
    self->srt.scale *= self->def->scale;

    if (mainGetBits(objSetup->gamebitCollected)) {
        objData->state = DB_EGG_STATE_Collected;
    } else {
        objData->state = DB_EGG_STATE_1;
    }
    
    if (objData->state == DB_EGG_STATE_1) {
        if (!DBEgg_depthCheck(self, &sp3C, 0, 0, DB_EGG_CHECK_FLOOR)) {
            // In water
            objData->state = DB_EGG_STATE_Floating;
        }
    }
    
    if (objSetup->modelIdx != 0) {
        objData->flags |= DB_EGG_FLAG_UNK1;
        if (objSetup->modelIdx == 2) {
            objData->flags |= DB_EGG_FLAG_UNK2;
        }
        
        if (objSetup->modelIdx == 3) {
            STUBBED_PRINTF(" Curve Egg");
            objData->state = DB_EGG_STATE_10;
        }
        
        if (objSetup->modelIdx == 4) {
            objData->flags |= DB_EGG_FLAG_UNK4;
            objData->flags &= ~DB_EGG_FLAG_UNK1;
        }

        STUBBED_PRINTF(" CARRY ONLY EGG");
    }

    objAddObjectType(self, OBJTYPE_38);
    
    if (mainGetBits(objSetup->gamebitSecondary)) {
        objData->state = DB_EGG_STATE_5;
    } else {
        objData->state = DB_EGG_STATE_12;
    }
    
    self->unkE0 = 0;
    self->velocity.x = 0.0f;
    self->velocity.y = 0.0f;
    self->velocity.z = 0.0f;
    
    objData->yOffset = 0.0f;
}

// offset: 0x1954 | func: 11
static void DBEgg_processObjMessages(Object* self) {
    u32 messageID = 0;
    u32 mesgArg = 0;
    DBEgg_Data* objData = self->data;
    Object* sender;
    f32 temp;

    while (objRecvMesg(self, &messageID, &sender, (void*)&mesgArg)) {
        STUBBED_PRINTF(" Message Op State %i %i \n", messageID, mesgArg);
        if (messageID == 0x11) {
            STUBBED_PRINTF(" Op State %i \n", mesgArg);

            switch (mesgArg) {
            case 0x12:  
                objData->state = DB_EGG_STATE_11;
                STUBBED_PRINTF(" Egg Stolen \n");
                break;
            case 0x11:   
                temp = mathSinfInterp(sender->srt.yaw);
                self->velocity.x = mathRnd(17, 27) * -temp * 0.3f;
                temp = mathCosfInterp(sender->srt.yaw);
                self->velocity.z = mathRnd(17, 27) * -temp * 0.3f;
                self->velocity.y = mathRnd(17, 27) * 0.3f;
                /* fallthrough */
            case 0x10: 
                objData->state = DB_EGG_STATE_5;
                self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                STUBBED_PRINTF(" Egg Trown Away \n");
                break;
            default:
                break;
            }
        }
    }
}
