#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/390_CFExplodeWall.h"
#include "dlls/objects/404_CFExplodePieces.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/objtype.h"

#define MAX_PIECES 15

typedef struct {
    Object* unk0; //Unused
    Vec3f centrepoint;
    Vec3f positionOffset;
    f32 yawSpeed;
    f32 pitchSpeed;
    f32 rollSpeed;
    f32 yawAcceleration;
    f32 pitchAcceleration;
    f32 rollAcceleration;
    Vec3f acceleration;
    Vec3f velocity;
    Vec3f initialPosition;
    f32 floorOffset;
    s32 lifetimeMax;
    s32 fadeEndTime;
    s16 roll;
    s16 pitch;
    s16 yaw;
    u8 state;
    u8 unk6B; //Unused in practice, but possibly meant as opacity?
    u8 flags;
    u8 rotateFactor;
} CFExplodeWall_PieceData;

typedef struct {
    CFExplodeWall_PieceData pieceData[MAX_PIECES];
    Object* pieceObjs[MAX_PIECES];
    s32 piecesSoundBitfield;
    s32 explodeSoundID;
    u8 pieceCount;
    u8 piecesConfigured[MAX_PIECES];
    u8 state;
} CFExplodeWall_Data;

/* Unused, but seem to be worldSpace coords for the pieces! */
/*0x0*/ static Vec3f dPieceCoords[4][MAX_PIECES] = {
    {
        VEC3F(-14657.105, 1870.912, 22338.988), 
        VEC3F(-14653.412, 1868.940, 22350.191), 
        VEC3F(-14664.002, 1872.833, 22332.936), 
        VEC3F(-14666.821, 1872.891, 22322.693), 
        VEC3F(-14650.792, 1873.479, 22332.201), 
        VEC3F(-14664.084, 1869.271, 22325.994), 
        VEC3F(-14647.538, 1870.727, 22332.354), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
    },
    {
        VEC3F(-14629.397, 1899.117, 22331.582), 
        VEC3F(-14628.425, 1896.516, 22370.727), 
        VEC3F(-14603.391, 1894.752, 22351.193), 
        VEC3F(-14605.199, 1899.383, 22373.939), 
        VEC3F(-14610.144, 1894.525, 22322.656), 
        VEC3F(-14650.631, 1896.519, 22319.727), 
        VEC3F(-14656.056, 1896.761, 22350.285), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
    },
    {
        VEC3F(3194.0510, 1941.219, 40863.609), 
        VEC3F(3195.0510, 1935.219, 40911.609), 
        VEC3F(3222.0510, 1939.219, 40886.609), 
        VEC3F(3225.0510, 1944.219, 40917.609), 
        VEC3F(3214.0510, 1912.219, 40855.609), 
        VEC3F(3168.0510, 1935.219, 40849.609), 
        VEC3F(3164.0510, 1941.219, 40885.609), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0)
    },
    {
        VEC3F(3197.656, 1935.569, 40856.269), 
        VEC3F(3198.656, 1929.569, 40904.269), 
        VEC3F(3225.656, 1933.569, 40879.269), 
        VEC3F(3228.656, 1938.569, 40910.269), 
        VEC3F(3217.656, 1906.569, 40848.269), 
        VEC3F(3171.656, 1929.569, 40842.269), 
        VEC3F(3167.656, 1935.569, 40878.269), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
        VEC3F(0, 0, 0), 
    }
};

/*0x2D0*/ static f32 data_2D0 = 203631;
/*0x2D4*/ static f32 data_2D4 = 203633;
/*0x2D8*/ static f32 data_2D8 = 203638;
/*0x2DC*/ static f32 data_2DC = 203641;
/*0x2E0*/ static u8 data_2E0 = 2;

typedef struct {
    s32 objIDWhole;
    s32 objIDPieces;
    s32 soundID;
    u8 rotateSpeed;
} ExplodeObjDef;

/*0x2E4*/ static ExplodeObjDef dExplodeDefs[11] = {
    { OBJ_CFExplodeFloor,  OBJ_CFExplodePieces, 0x858, 1 },
    { OBJ_DFdebris1,       OBJ_DFdebris1piece,  1,     50 },
    { OBJ_DFdebris2,       OBJ_DFdebris2piece,  1,     100 },
    { OBJ_DFdebris3,       OBJ_DFdebris3piece,  1,     100 }, 
    { OBJ_CFExplodeTunnel, OBJ_CFExplodeTunnPi, 1,     20 }, 
    { OBJ_DRSmallExplodeW, OBJ_DRSmallExplodeP, 0x858, 1 }, 
    { OBJ_DRExplodeWall,   OBJ_DRExplodePieces, 0x858, 1 }, 
    { OBJ_CCrockDoor,      OBJ_CCrockDoorPiece, 1,     1 }, 
    { OBJ_CCgrassfloor,    OBJ_CCgrassfloorPie, 1,     1 }, 
    { OBJ_CFExplodeWall,   OBJ_CFWallPieaces,   0x858, 50 }, 
    { OBJ_CCexplodedoor,  OBJ_CCexplodedoorPi, 1,     1 }
};

static void CFExplodeWall_explode(Object* self, CFExplodeWall_Setup* objSetup, s32 skipModelCentrepointCalc, CFExplodeWall_Data* objData);
static Object* CFExplodeWall_createPiece(Object* self, s32 objectID, CFExplodeWall_PieceData* arg2, s32 index);

// offset: 0x0 | ctor
void CFExplodeWall_ctor(void* dll) { }

// offset: 0xC | dtor
void CFExplodeWall_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFExplodeWall_obj_Setup(Object* self, CFExplodeWall_Setup* objSetup, s32 reset) {
    CFExplodeWall_Data* objData;
    u32 i;

    objAddObjectType(self, OBJTYPE_ExplodeObj);
    
    objData = self->data;
    
    if (objSetup->pieceCount == 0) {
        objData->pieceCount = 1;
    } else {
        objData->pieceCount = objSetup->pieceCount;
    }
    
    objData->piecesSoundBitfield = 0;
    
    for (i = 0; i < ARRAYCOUNT(objData->pieceObjs); i++) {
        objData->pieceObjs[i] = NULL;
    }
    
    self->srt.yaw = objSetup->yaw;
    self->srt.pitch = objSetup->pitch;
    self->srt.roll = objSetup->roll;
    
    if (mainGetBits(objSetup->gamebitFinished)) {
        objData->state = CFExplodeWall_STATE_2_Finished;
    }
}

// offset: 0x108 | func: 1 | export: 1
void CFExplodeWall_obj_Control(Object* self) {
    u32 i;
    CFExplodeWall_Setup* objSetup;
    CFExplodeWall_Data* objData;
    s32 pieceVal;
    Object* obj;

    objData = self->data;
    objSetup = (CFExplodeWall_Setup*)self->setup;

    //Finished
    if (objData->state == CFExplodeWall_STATE_2_Finished) {
        return;
    }

    //Waiting to explode
    if (objData->state == CFExplodeWall_STATE_0_Waiting) {
        if (mainGetBits(objSetup->gamebitExplode)) {
            CFExplodeWall_explode(self, objSetup, FALSE, objData);
            if (objData->explodeSoundID != NO_SOUND) {
                dll_amSfx->Play(self, objData->explodeSoundID, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            objData->state = CFExplodeWall_STATE_1_Exploding;
            self->opacity = 0;
        }
        return;
    }

    //Exploding
    for (i = 0; i < ARRAYCOUNT(objData->pieceObjs); i++) {
        obj = objData->pieceObjs[i];
        if (obj) {
            pieceVal = dll_CFExplodePieces(obj)->GetState(obj);
            
            switch (pieceVal) {
            case CFExplodePieces_STATE_2_Finished:
                //Free piece
                mainSetBits(objSetup->gamebitFinished, TRUE);
                objFreeObject(objData->pieceObjs[i]);
                objData->pieceObjs[i] = NULL;
                break;
            case CFExplodePieces_STATE_0_Stopped:
                //Play piece impact sound
                mainSetBits(objSetup->gamebitFinished, TRUE);
                if ((objData->piecesSoundBitfield & (1 << i)) == FALSE) {
                    dll_amSfx->Play(self, SOUND_5B5_Explosion_Debris_Crash, MAX_VOLUME, NULL, NULL, 0, NULL);
                    objData->piecesSoundBitfield |= 1 << i;
                }
                break;
            }
        }
    }
}

// offset: 0x310 | func: 2 | export: 2
void CFExplodeWall_obj_Update(Object* self) { }

// offset: 0x31C | func: 3 | export: 3
void CFExplodeWall_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x334 | func: 4 | export: 4
void CFExplodeWall_obj_Free(Object* self, s32 onlySelf) {
    s32 i = OBJTYPE_ExplodeObj;
    CFExplodeWall_Data* objData;

    objData = self->data;
    
    objFreeObjectType(self, i);
    
    if (onlySelf) {
        return;
    }
    
    for (i *= 0; i < ARRAYCOUNT_S(objData->pieceObjs); i++) {
        if (objData->pieceObjs[i] != NULL) {
            objFreeObject(objData->pieceObjs[i]); 
        } 
    }
}

// offset: 0x3C4 | func: 5 | export: 5
u32 CFExplodeWall_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x3D4 | func: 6 | export: 6
u32 CFExplodeWall_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFExplodeWall_Data);
}

/*0x0*/ static const char str_0[] = " Simple Exploder ";

// offset: 0x3E8 | func: 7
static void CFExplodeWall_calculatePieceData(Object* self, CFExplodeWall_PieceData* piece, CFExplodeWall_Setup* objSetup) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 force;
    f32 pad;
    f32 displacement;
    s32 maxAngle;

    mathRotateRPY((SRT*)&objSetup->yaw, piece->positionOffset.f);
    
    piece->initialPosition.x = objSetup->base.x + (piece->positionOffset.x * self->srt.scale);
    piece->initialPosition.y = objSetup->base.y + (piece->positionOffset.y * self->srt.scale);
    piece->initialPosition.z = objSetup->base.z + (piece->positionOffset.z * self->srt.scale);
    
    piece->yaw   = objSetup->yaw;
    piece->pitch = objSetup->pitch;
    piece->roll  = objSetup->roll;
    
    dx = piece->positionOffset.x - objSetup->displacementOrigin.x;
    dy = piece->positionOffset.y - objSetup->displacementOrigin.y;
    dz = piece->positionOffset.z - objSetup->displacementOrigin.z;
    displacement = sqrtf(SQ(dx) + SQ(dy) + SQ(dz));
    
    if (displacement == 0.0f) {
        STUBBED_PRINTF(" Sorry You have no displacement for the force ");
        return;
    }
    
    force = objSetup->explosionPower / (5.0f * displacement);
    if ((dx != 0.0f) || (dy != 0.0f) || (dz != 0.0f)) {
        guNormalize(&dx, &dy, &dz);
    }
    
    piece->velocity.x = dx * force;
    piece->velocity.y = dy * force;
    piece->velocity.z = dz * force;

    maxAngle = (force + 0.5f) * 200.0f;    
    piece->yawSpeed   = mathRnd(0, maxAngle) / 50.0f;
    piece->pitchSpeed = mathRnd(0, maxAngle) / 50.0f;
    piece->rollSpeed  = mathRnd(0, maxAngle) / 50.0f;
    
    force = objSetup->acceleration / 1000.0f;
    maxAngle = (force + 0.5f) * 200.0f;
    
    if (self->velocity.x > 0) {
        piece->flags |= CFExplodePiece_FLAG_Translating_X;
    }
    if (self->velocity.z > 0) {
        piece->flags |= CFExplodePiece_FLAG_Translating_Z;
    }
    if (piece->yawSpeed > 0) {
        piece->flags |= CFExplodePiece_FLAG_Rotating_Yaw;
    }
    if (piece->pitchSpeed > 0) {
        piece->flags |= CFExplodePiece_FLAG_Rotating_Pitch;
    }
    if (piece->rollSpeed > 0) {
        piece->flags |= CFExplodePiece_FLAG_Rotating_Roll;
    }
    
    piece->yawAcceleration   = mathRnd(0, maxAngle) / 200.0f;
    piece->pitchAcceleration = mathRnd(0, maxAngle) / 200.0f;
    piece->rollAcceleration  = mathRnd(0, maxAngle) / 200.0f;
    piece->acceleration.x =  dx * force;
    piece->acceleration.y = (dy * force) - 0.07f;
    piece->acceleration.z =  dz * force;

    if (objSetup->floorOffset != 0) {
        piece->floorOffset = objSetup->floorOffset;
    }
    
    piece->lifetimeMax = objSetup->lifetimeMax;
    
    if (objSetup->lifetimeMax != 0) {
        piece->fadeEndTime = ((mathRnd(0, 100) + 100) * objSetup->lifetimeMax) / 200;
    } else {
        piece->fadeEndTime = -1;
    }
}

// offset: 0x884 | func: 8
void CFExplodeWall_explode(Object* self, CFExplodeWall_Setup* objSetup, s32 skipModelCentrepointCalc, CFExplodeWall_Data* objData) {
    #define PIECE (&objData->pieceData[i])
    s32 sumX;
    s32 sumY;
    s32 sumZ;
    Vtx* vertices;
    s32 i;
    s32 vtxIdx;
    s32 objectID;
    s16 count;
    u8 rotateFactor;
    ModelInstance* modelInstance;
    Model* model;
    CFExplodeWall_PieceData* piece;

    objectID = -1;
    rotateFactor = 1;

    if (1) { //fake?
        i = 0;
    }
    for (; i < ARRAYCOUNT_S(dExplodeDefs); i++) {
        if (self->id == dExplodeDefs[i].objIDWhole) {
            objectID = dExplodeDefs[i].objIDPieces;
            objData->explodeSoundID = dExplodeDefs[i].soundID;
            rotateFactor = dExplodeDefs[i].rotateSpeed;
            //@bug? doesn't break out after finding the matching objIDWhole
        }
    }
    
    if (objectID == -1) {
        STUBBED_PRINTF(" Warning : DefNumber Lists in Exploder DLL is not set up correctly for this object");
        return;
    }

    for (i = 0; i < objData->pieceCount; i++) {
        objData->piecesConfigured[i] = TRUE;
        PIECE->rotateFactor = rotateFactor;
        
        if (skipModelCentrepointCalc == FALSE) {
            PIECE->centrepoint.x = 0.0f;
            PIECE->centrepoint.y = 0.0f;
            PIECE->centrepoint.z = 0.0f;
            
            sumX = 0;
            sumY = 0;
            sumZ = 0;
            modelInstance = self->modelInsts[i];
            model = modelInstance->model;
            for (vtxIdx = 0; vtxIdx < model->vertexCount; vtxIdx++) {
                sumX += model->vertices[vtxIdx].v.ob[0];
                sumY += model->vertices[vtxIdx].v.ob[1];
                sumZ += model->vertices[vtxIdx].v.ob[2];
            }
            sumX /= model->vertexCount;
            sumY /= model->vertexCount;
            sumZ /= model->vertexCount;
            PIECE->centrepoint.x = sumX;
            PIECE->centrepoint.y = sumY;
            PIECE->centrepoint.z = sumZ;
        }
        
        PIECE->positionOffset.x = PIECE->centrepoint.x;
        PIECE->positionOffset.y = PIECE->centrepoint.y;
        PIECE->positionOffset.z = PIECE->centrepoint.z;

        CFExplodeWall_calculatePieceData(self, PIECE, objSetup);

        PIECE->unk6B = 0xFF;
        
        if (mainGetBits(objSetup->gamebitFinished)) {
            PIECE->state = CFExplodeWall_STATE_2_Finished;
        } else {
            PIECE->state = CFExplodeWall_STATE_0_Waiting;
        }
        
        objData->pieceObjs[i] = CFExplodeWall_createPiece(self, objectID, PIECE, i);
    }

    if (mainGetBits(objSetup->gamebitFinished)) {
        objData->state = CFExplodeWall_STATE_1_Exploding;
        return;
    }
    
    objData->state = CFExplodeWall_STATE_0_Waiting;
}

/*0x98*/ static const char str_98[] = " Creating Object ";

// offset: 0xC60 | func: 9
Object* CFExplodeWall_createPiece(Object* self, s32 objectID, CFExplodeWall_PieceData* pieceData, s32 index) {
    CFExplodePieces_Setup* setup;

    setup = objAllocSetup(sizeof(CFExplodePieces_Setup), objectID);
    setup->base.objId = objectID;
    setup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
    setup->base.loadDistance = 0xFF;
    setup->base.fadeFlags = OBJSETUP_FADE_MANUAL;
    setup->base.fadeDistance = 0xFF;
    setup->base.x = self->srt.transl.x;
    setup->base.y = self->srt.transl.y;
    setup->base.z = self->srt.transl.z;
    setup->velocity.x = pieceData->velocity.x * 100.0f;
    setup->velocity.y = pieceData->velocity.y * 100.0f;
    setup->velocity.z = pieceData->velocity.z * 100.0f;
    setup->yaw   = pieceData->yaw;
    setup->pitch = pieceData->pitch;
    setup->roll  = pieceData->roll;
    setup->yawSpeed   = pieceData->yawSpeed   * pieceData->rotateFactor;
    setup->pitchSpeed = pieceData->pitchSpeed * pieceData->rotateFactor;
    setup->rollSpeed  = pieceData->rollSpeed  * pieceData->rotateFactor;
    setup->yawAcceleration   = pieceData->yawAcceleration   * 10.0f;
    setup->rollAcceleration  = pieceData->rollAcceleration  * 10.0f;
    setup->pitchAcceleration = pieceData->pitchAcceleration * 10.0f;
    setup->acceleration.x = pieceData->acceleration.x * 1000.0f;
    setup->acceleration.y = pieceData->acceleration.y * 1000.0f;
    setup->acceleration.z = pieceData->acceleration.z * 1000.0f;
    setup->modelIdx = index;
    setup->lifetimeMax = pieceData->lifetimeMax;
    setup->floorOffset = (s16)pieceData->floorOffset;
    return objSetupObject(&setup->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, -1, -1, NULL);
}
