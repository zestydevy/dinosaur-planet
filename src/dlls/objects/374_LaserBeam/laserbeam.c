#include "common.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/map.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

/*0x0*/ static DLTri dLaserTris[] = {
    MASK_TRI(0x40, 0, 1, 4), 
    MASK_TRI(0x40, 1, 5, 4),
    MASK_TRI(0x40, 2, 0, 4), 
    MASK_TRI(0x40, 4, 6, 2), 
    MASK_TRI(0x40, 3, 2, 6), 
    MASK_TRI(0x40, 6, 7, 3), 
    MASK_TRI(0x40, 1, 3, 7), 
    MASK_TRI(0x40, 7, 5, 1)
};

typedef struct {
    s32 gamebit;
    u32 soundHandle;
    f32 x1;
    f32 x2;
    f32 y1;
    f32 y2;
    f32 z1;
    f32 z2;
    u8 colourR;
    u8 colourG;
    u8 colourB;
    u8 drawLaser;
    s8 beamHalfWidth;
    s8 playerZapCooldown;
    SRT playerKnockbackDir;
} LaserBeam_Data;

typedef enum {
    LaserBeam_MODE_Fire_Laser_While_Gamebit_Set = 0,
    LaserBeam_MODE_Fire_Laser_While_Gamebit_Unset = 1 //Or any nonzero mode value
} LaserBeam_Modes;

// offset: 0x0 | ctor
void LaserBeam_ctor(void* dll) { }

// offset: 0xC | dtor
void LaserBeam_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void LaserBeam_obj_Setup(Object* self, LaserBeam_Setup* objSetup, s32 reset) {
    LaserBeam_Data* objData = self->data;
    
    objInitMesgQueue(self, 2);
    
    self->srt.yaw = objSetup->yaw << 8;
    
    objData->gamebit = objSetup->gamebit;
}

// offset: 0x80 | func: 1 | export: 1
void LaserBeam_obj_Control(Object* self) {
    MtxF* mtx;
    LaserBeam_Setup* objSetup;
    Object* obj;
    Object** objects;
    f32 dy;
    f32 range;
    f32 sqDistance;
    f32 cos;
    f32 position;
    f32 blockRadius;
    Model* model;
    f32 sin;
    f32 worldOriginInObjectSpace;
    f32 pad1;
    s32 i;
    f32 wsBeamEndX;
    f32 wsBeamEndY;
    f32 wsBeamEndZ;
    f32 sqBeamLength;
    f32 beamLength;
    f32 distance;
    s32 volume;
    s32 count;
    f32 pad2;
    f32 dz;
    f32 dx;
    s8 gamebitIsSet;
    LaserBeam_Data* objData;
    Vec3f transformedBeamStart;
    Vec3f transformedBeamEnd;

    objData = self->data;
    objSetup = (LaserBeam_Setup*)self->setup;

    //Check the gamebit's gamebit, if it has one
    if (objData->gamebit != NO_GAMEBIT) {
        gamebitIsSet = mainGetBits(objData->gamebit);
    } else {
        gamebitIsSet = TRUE;
    }

    //Check if the gamebit has switched off the laser
    //(Two modes: gamebit activates laser when set, or gamebit deactivates laser when set)
    if (((objSetup->mode != LaserBeam_MODE_Fire_Laser_While_Gamebit_Set) && gamebitIsSet) || 
        ((objSetup->mode == LaserBeam_MODE_Fire_Laser_While_Gamebit_Set) && (gamebitIsSet == FALSE))
    ) {
        objData->drawLaser = FALSE;

        if (objData->soundHandle != 0) {
            dll_amSfx->Stop(objData->soundHandle);
            objData->soundHandle = 0;
        }

        return;
    }

    //Start the laser's hum sound loop
    if (objData->soundHandle == 0) {
        dll_amSfx->Play(self, SOUND_135_Laser_Hum, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
    }
    
    beamLength = objSetup->beamLength;
    sqBeamLength = SQ(beamLength);

    cos = mathCosfInterp(self->srt.yaw);
    sin = mathSinfInterp(self->srt.yaw);

    worldOriginInObjectSpace = -((self->srt.transl.x * cos) + (self->srt.transl.z * sin));
    wsBeamEndX = self->srt.transl.x + (beamLength * sin);
    wsBeamEndY = self->srt.transl.y;
    wsBeamEndZ = self->srt.transl.z - (beamLength * cos);
    
    //Get all pushblock type objects, check if they're touching the laser, and adjust the beam's length
    objects = objGetAllOfType(OBJTYPE_PushBlock, &count);
    for (i = 0; i < count; i++) {
        obj = objects[i];
        range = objData->beamHalfWidth;
        dy = obj->srt.transl.y - self->srt.transl.y;
        if (obj->modelInsts[0] != NULL) {
            model = obj->modelInsts[0]->model;
            blockRadius = ((model->maxAnimatedVertDistance * 0.8f) * obj->srt.scale) + range;
        }

        if ((dy < range) && (-(range + 40.0f) < dy)) {
            //Check whether the pushblock's distance is less than the beam length
            dx = obj->srt.transl.x - self->srt.transl.x;
            dz = obj->srt.transl.z - self->srt.transl.z;
            sqDistance = SQ(dx) + SQ(dz);
            if (sqDistance < sqBeamLength) {
                //Check the pushblock's position along the laser's objectSpace X axis (i.e. distance perpendicular to the beam)
                position = (obj->srt.transl.x * cos) + (sin * obj->srt.transl.z) + worldOriginInObjectSpace;
                if ((blockRadius > position) && (position > -blockRadius)) {
                    if (obj->polyhits != NULL && obj->polyhits->unk10D == 0) {
                        //Unused calculations: getting an accurate beam length from the pushblock's polyhits, maybe?
                        mtx = &obj->polyhits->unk0[obj->polyhits->unk10C & 0xFF];
                        mathMtxXFMF(mtx, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &transformedBeamStart.x, &transformedBeamStart.y, &transformedBeamStart.z);
                        mathMtxXFMF(mtx, wsBeamEndX, wsBeamEndY, wsBeamEndZ, &transformedBeamEnd.x, &transformedBeamEnd.y, &transformedBeamEnd.z);
                    } else {
                        //Otherwise, adjust laser beam length using pushblock's coords
                        beamLength = sqrtf(sqDistance);
                        sqBeamLength = sqDistance;
                    }
                }
            }
        }
    }
    
    //Determining if the laser should hurt the player
    {
        obj = objGetPlayer();

        objData->playerZapCooldown -= gUpdateRate;
        if (objData->playerZapCooldown < 0) {
            objData->playerZapCooldown = 0;
        }
        
        volume = 0;

        //Check if the player is close enough to be zapped by the laser
        if (obj != NULL && objData->playerZapCooldown == 0) {
            //Check the player's vertical position in relation to the beam
            range = objData->beamHalfWidth + 5.0f;
            dy = obj->srt.transl.y - self->srt.transl.y;
            if ((dy < range) && (-(range + 25.0f) < dy)) {
                //Check whether the player's distance is less than the beam length
                dx = obj->srt.transl.x - self->srt.transl.x;
                dz = obj->srt.transl.z - self->srt.transl.z;
                sqDistance = SQ(dx) + SQ(dz);
                if (sqDistance < sqBeamLength) {
                    position = (obj->srt.transl.x * cos) + (sin * obj->srt.transl.z) + worldOriginInObjectSpace;

                    //Set laser volume based on perpendicular distance
                    if (objData->soundHandle != 0) {
                        distance = position;
                        if (position < 0.0f) {
                            distance = -position;
                        }
                        if (distance > 63.0f) {
                            distance = 63.0f;
                        }
                        distance = 63.0f - distance;
                        volume = 2.0f * distance;
                    }
                    
                    //Check the player's position along the laser's objectSpace Z axis (i.e. distance along the beam)
                    if ((range > position) && (position > -range)) {
                        distance = (obj->prevLocalPosition.x * cos) + (sin * obj->prevLocalPosition.z) + worldOriginInObjectSpace;
                        if (distance < 0.0f) {
                            distance = -20.0f;
                        } else {
                            distance = 20.0f;
                        }

                        objData->playerKnockbackDir.transl.x = obj->srt.transl.x + (cos * distance);
                        objData->playerKnockbackDir.transl.z = obj->srt.transl.z + (sin * distance);
                        objSendMesg(obj, 0x60003, (Object*)&objData->playerKnockbackDir, NULL);
                        objData->playerZapCooldown = 20;
                    }
                }
            }
        }
    }

    if (objData->soundHandle != 0) {
        dll_amSfx->SetVol(objData->soundHandle, volume);
    }
    
    objData->colourR = 0xFF;
    objData->colourG = 0;
    objData->colourB = 0;

    objData->x1 = 0.0f;
    objData->y1 = 0.0f;
    objData->z1 = 0.0f;
    objData->x2 = 0.0f;
    objData->y2 = 0.0f;
    objData->z2 = beamLength;
    objData->beamHalfWidth = 4;
    objData->drawLaser = TRUE;
}

// offset: 0x6AC | func: 2 | export: 2
void LaserBeam_obj_Update(Object* self) { }

// offset: 0x6B8 | func: 3 | export: 3
void LaserBeam_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, DLTri** pols, s32 visibility) {
    Vtx_t* vtx;
    LaserBeam_Data* objData;
    s16 y1;
    s16 y2;
    s16 z1;
    s16 z2;
    s16 x1;
    s16 x2;
    u8 colourR;
    u8 colourG;
    u8 colourB;
    SRT trans;

    objData = self->data;
    
    if (objData->drawLaser == FALSE) {
        return;
    }
    
    trans.transl.x = self->srt.transl.x;
    trans.transl.y = self->srt.transl.y;
    trans.transl.z = self->srt.transl.z;
    trans.yaw = self->srt.yaw;
    trans.pitch = self->srt.pitch;
    trans.roll = self->srt.roll;
    trans.scale = 1.0f;
    camSetupObjectSRTMatrix(gdl, mtxs, &trans, 1.0f, 0.0f, NULL);
    
    texDPTextures(gdl, NULL, NULL, 0x10 | 4 | 2 | 1, 0, 0, 1);
    
    vtx = (Vtx_t*)*vtxs;
    bcopy(dLaserTris, *pols, sizeof(dLaserTris));
    
    colourR = objData->colourR;
    colourG = objData->colourG;
    colourB = objData->colourB;

    gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(*vtxs), 8, 0);
    dlTriangles(gdl, *pols, 8);
    *pols += 8;
    
    x1 = objData->x1;
    y1 = objData->y1;
    z1 = objData->z1;
    
    x2 = objData->x2;
    y2 = objData->y2;
    z2 = objData->z2;
    
    vtx->ob[0] = x1 + objData->beamHalfWidth;
    vtx->ob[1] = y1 + objData->beamHalfWidth;
    vtx->ob[2] = z1;
    vtx->cn[0] = colourR;\
    vtx->cn[1] = colourG;\
    vtx->cn[2] = colourB;\
    vtx->cn[3] = 0x50;
    vtx++;
    
    vtx->ob[0] = x1 - objData->beamHalfWidth;
    vtx->ob[1] = y1 + objData->beamHalfWidth;
    vtx->ob[2] = z1;
    vtx->cn[0] = colourR;\
    vtx->cn[1] = colourG;\
    vtx->cn[2] = colourB;\
    vtx->cn[3] = 0x50;
    vtx++;
    
    vtx->ob[0] = x1 + objData->beamHalfWidth;
    vtx->ob[1] = y1 - objData->beamHalfWidth;
    vtx->ob[2] = z1;
    vtx->cn[0] = colourR;\
    vtx->cn[1] = colourG;\
    vtx->cn[2] = colourB;\
    vtx->cn[3] = 0x50;
    vtx++;
    
    vtx->ob[0] = x1 - objData->beamHalfWidth;
    vtx->ob[1] = y1 - objData->beamHalfWidth;
    vtx->ob[2] = z1;
    vtx->cn[0] = colourR;\
    vtx->cn[1] = colourG;\
    vtx->cn[2] = colourB;\
    vtx->cn[3] = 0x50;
    vtx++;
    
    vtx->ob[0] = x2 + objData->beamHalfWidth;
    vtx->ob[1] = y2 + objData->beamHalfWidth;
    vtx->ob[2] = z2;
    vtx->cn[0] = colourR;\
    vtx->cn[1] = colourG;\
    vtx->cn[2] = colourB;\
    vtx->cn[3] = 0x50;
    vtx++;
    
    vtx->ob[0] = x2 - objData->beamHalfWidth;
    vtx->ob[1] = y2 + objData->beamHalfWidth;
    vtx->ob[2] = z2;
    vtx->cn[0] = colourR;\
    vtx->cn[1] = colourG;\
    vtx->cn[2] = colourB;\
    vtx->cn[3] = 0x50;
    vtx++;
    
    vtx->ob[0] = x2 + objData->beamHalfWidth;
    vtx->ob[1] = y2 - objData->beamHalfWidth;
    vtx->ob[2] = z2;
    vtx->cn[0] = colourR;\
    vtx->cn[1] = colourG;\
    vtx->cn[2] = colourB;\
    vtx->cn[3] = 0x50;
    vtx++;
    
    vtx->ob[0] = x2 - objData->beamHalfWidth;
    vtx->ob[1] = y2 - objData->beamHalfWidth;
    vtx->ob[2] = z2;
    vtx->cn[0] = colourR;\
    vtx->cn[1] = colourG;\
    vtx->cn[2] = colourB;\
    vtx->cn[3] = 0x50;
    vtx++;

    *vtxs = (Vertex*)vtx;
}

// offset: 0xA04 | func: 4 | export: 4
void LaserBeam_obj_Free(Object* self, s32 onlySelf) {
    LaserBeam_Data* objData = self->data;
    
    if (objData->soundHandle != 0) {
        dll_amSfx->Stop(objData->soundHandle);
    }
}

// offset: 0xA60 | func: 5 | export: 5
u32 LaserBeam_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0xA70 | func: 6 | export: 6
u32 LaserBeam_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(LaserBeam_Data);
}
