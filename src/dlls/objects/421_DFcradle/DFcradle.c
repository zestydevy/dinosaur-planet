#include "common.h"
#include "sys/curves.h"
#include "sys/objtype.h"
#include "dlls/objects/347_texscroll2.h"
#include "dlls/objects/421_DFcradle.h"

/*0x0*/ static s16 sSoundIDs[] = {
    SOUND_783_Rope_Strain, 
    SOUND_784_Rope_Strain, 
    SOUND_785_Rope_Strain
};
/*0x8*/ static u32 dTexscrollUIDs[] = {
    0x0003254b, 
    0x0003254f, 
    0x00032562, 
    0x00032563
};

typedef enum {
    DFCradle_STATION_0, //Lower falls, near the toxic cave entrance
    DFCradle_STATION_1, //Middle falls, mole cave entrance
    DFCradle_STATION_2  //Upper falls, beside turbine
} DFCradle_Stations;

static u8 DFCradle_getStationNumber(Object* self, f32 x, f32 z, u8 isPlayer);

// offset: 0x0 | ctor
void DFCradle_ctor(void* dll) { }

// offset: 0xC | dtor
void DFCradle_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFCradle_obj_Setup(Object* self, DFCradle_Setup* setup, s32 reset) {
    DFCradle_Data* objData = self->data;
    /*0x18*/ s32 dCurveTypes[] = { 1 };
    Object* pulley;
    s32 stride;
    CurveSetup* curveSetup;
    s32 curveUID;
    
    self->srt.roll = setup->roll << 8;
    self->srt.pitch = setup->pitch << 8;
    self->srt.yaw = setup->yaw << 8;
    
    curveUID = gDLL_26_Curves->vtbl->func_1E4(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, dCurveTypes, ARRAYCOUNT(dCurveTypes), -1);
    if (curveUID != 0) {
        curveSetup = gDLL_26_Curves->vtbl->func_39C(curveUID);
        
        stride = (gDLL_26_Curves->vtbl->func_D8C(curveSetup) - 1) << 2;
        
        objData->splineX = mmAlloc((stride * 3) * sizeof(f32*), COLOUR_TAG_BLACK, NULL);
        objData->splineY = objData->splineX + stride;
        objData->splineZ = objData->splineY + stride;
        
        objData->curves.unk84 = objData->splineX;
        objData->curves.unk88 = objData->splineY;
        objData->curves.unk8C = objData->splineZ;
        objData->curves.unk80 = 0;
        
        objData->curves.splineFunc = curvesHermite;
        objData->curves.splineConverterFunc = curvesHermiteConverter;
        objData->curves.numControlPoints = gDLL_26_Curves->vtbl->func_E40(curveSetup, objData->curves.unk84, objData->curves.unk88, objData->curves.unk8C, NULL);
        
        curvesMove((CurvesStruct*)&objData->curves.unk0);
    }
    
    objData->enabled = mainGetBits(BIT_DF_Cradle_Powered);
    objData->direction = 1;
    objData->pauseTimer = 0;
    objData->soundTimer = 0;
    objData->speed = 0.0f;
    
    //Get lower middle pulley value (closer to SwapStone Circle)
    pulley = objGetObjectByUID(0x208E);
    objData->pulleyValLower = pulley->globalPosition.x + (pulley->globalPosition.z * 0.4f);
    
    //Get upper middle pulley value (closer to DF Shrine)
    pulley = objGetObjectByUID(0x208F);
    objData->pulleyValUpper = pulley->globalPosition.x + (pulley->globalPosition.z * 0.4f);
}

// offset: 0x240 | func: 1 | export: 1
void DFCradle_obj_Control(Object* self) {
    DFCradle_Data* objData;
    Object** objects;
    Object* player;
    s32 count;
    s32 idx;
    u8 doDirectionReverse;
    u8 cradleStation;
    u8 playerStation;
    f32 dx;
    f32 dy;
    f32 dz;
    u8 doReverseAfterStopping;
    Object* texscroll;
    u8 i;
    s8 scrollSpeed;

    objData = self->data;
    
    //Handle cradle's motion
    if (objData->enabled) {
        //Play rope straining noise at random intervals
        if (objData->soundTimer-- < 0) {
            dll_amSfx->Play(self, sSoundIDs[mathRnd(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->soundTimer = mathRnd(40, 60);
        }
        
        if (objData->pauseTimer) {
            objData->pauseTimer--; //@framerate-dependent
        }
        
        player = objGetPlayer();
        if (player == NULL) {
            return;
        }

        //Check if the cradle should reverse back to the player once it reaches the next pulley
        {
            playerStation = DFCradle_getStationNumber(self, player->globalPosition.x, player->globalPosition.z, TRUE);
            cradleStation = DFCradle_getStationNumber(self, self->globalPosition.x,   self->globalPosition.z,   FALSE);

            doDirectionReverse = ((playerStation < cradleStation) && (objData->direction > 0)) || //Cradle moving upwards and the player's at a lower station
                                ((playerStation > cradleStation) && (objData->direction < 0));   //Cradle moving downwards and the player's at a higher station
        }

        //Get the player's distance from the cradle
        dx = self->globalPosition.x - player->globalPosition.x;
        dy = self->globalPosition.y - player->globalPosition.y;
        dz = self->globalPosition.z - player->globalPosition.z;
        dx = SQ(dx) + SQ(dy) + SQ(dz);
        
        //Pause when reaching a pulley (longer pause if the player's close)
        if (!objData->pauseTimer && (cradleStation != objData->prevCradleStation)) {
            if (dx < SQ(200)) {
                objData->pauseTimer = 40;
            } else {
                objData->pauseTimer = 10;
            }
        }
        objData->prevCradleStation = cradleStation;

        /* Automatically reverse direction to move back towards the player when all these conditions are met:
         - the player isn't on the cradle
         - the cradle's pauseTimer just reset (finished waiting after slowing down to stop at a pulley)
         - the cradle's moving down but the player's at a higher station, or moving up and the player's at a lower station
         */
        if ((objData->pauseTimer == 10) && (self != player->parent) && doDirectionReverse) {
            objData->direction = -objData->direction;
            if (objData->direction > 0) {
                mainSetBits(BIT_DF_Cradle_Moving_Down, FALSE);
            } else {
                mainSetBits(BIT_DF_Cradle_Moving_Down, TRUE);
            }
        }
        
        if (objData->pauseTimer <= 10) {
            objects = objGetAllOfType(OBJTYPE_Pulley, &count);

            //Rotate pulley objects
            for (idx = 0; idx < count; idx++) {
                objects[idx]->srt.yaw -= objData->speed * 500.0f * objData->direction * gUpdateRateF * (((objects[idx]->stateFlags & 1) * 2) - 1);
            }                

            //Check if the cradle should reverse by gamebit, using the nearby Projectile Switches (@bug: doesn't seem to work correctly)
            doReverseAfterStopping = mainGetBits(BIT_DF_Cradle_Moving_Down);
            if ((doReverseAfterStopping == FALSE) && (objData->direction == 0)) {
                doReverseAfterStopping = FALSE;
            }
            if (doReverseAfterStopping && (objData->direction == -1)) {
                doReverseAfterStopping = FALSE;
            }
            
            if (doReverseAfterStopping) {
                //Skip slowing down when passing pulleys, and change direction once reaching the end of the rope path?
                if (objData->speed < 0.02f) {
                    objData->direction = -objData->direction;
                    objData->speed = 0.0f;
                }

                //@bug: wouldn't it be possible for the cradle to get stuck moving really slowly here (speed at/over 0.02), until reaching the end of the rope?
            } else {
                //Set the cradle's speed based on distance to the nearest pulley
                s32 minDistance = 10000;
                s32 distance; 
                
                for (idx = 0; idx < count; idx++) {
                    distance = vec3Distance(&self->globalPosition, &objects[idx]->globalPosition);
                    if (minDistance > distance) {
                        minDistance = distance;
                    }
                }
                
                if (minDistance <= 20) {
                    minDistance = 20;
                }
                if (minDistance >= 120) {
                    minDistance = 120;
                }

                if (1){}
                
                objData->speed = (((minDistance - 20.0f) * 1.8f) / 100.0f) + 0.2f;
            }
            
            scrollSpeed = (objData->speed * -53.0f) * objData->direction;

            //Handle curves
            {
                if (curves_func_800053B0((CurvesStruct*)&objData->curves, -scrollSpeed / 53.0f)) {
                    //Reverse direction when reaching the end of the pulley path
                    objData->direction = -objData->direction;
                    objData->speed = 0/*0.0f*/;
                    
                    if (dx < SQ(200)) {
                        objData->pauseTimer = 60;
                    } else {
                        objData->pauseTimer = 20;
                    }
                    
                    if (objData->direction > 0) {
                        mainSetBits(BIT_DF_Cradle_Moving_Down, FALSE);
                    } else {
                        mainSetBits(BIT_DF_Cradle_Moving_Down, TRUE);
                    }
                } else if ((objData->curves.unk74.x != 0/*.0f*/) || (objData->curves.unk74.z != 0/*.0f*/)) {
                    //Set yaw using the curve tangent
                    self->srt.yaw = mathAtan2f(objData->curves.unk74.x, objData->curves.unk74.z) + M_180_DEGREES;
                }

                //Set position using the current point on the curve
                self->srt.transl.x = objData->curves.unk68.x;
                self->srt.transl.y = objData->curves.unk68.y;
                self->srt.transl.z = objData->curves.unk68.z;
            }
        } else {
            scrollSpeed = 0;
        }

        //Animate cradle ropes
        for (i = 0; i < 4; i++) {
            texscroll = objGetObjectByUID(dTexscrollUIDs[i]);
            if (texscroll != NULL) {
                dll_TexScroll2(texscroll)->changeScrollSpeed(texscroll, scrollSpeed);
            }
        }
        return;
    }

    //Check if the cradle has been powered
    objData->enabled = mainGetBits(BIT_DF_Cradle_Powered);
    
    if (objData->speed > 0.0f) {
        objData->speed -= 0.02f;
    } else {
        objData->speed = 0.0f;
    }
}

// offset: 0x908 | func: 2 | export: 2
void DFCradle_obj_Update(Object* self) { }

// offset: 0x914 | func: 3 | export: 3
void DFCradle_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x968 | func: 4 | export: 4
void DFCradle_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x978 | func: 5 | export: 5
u32 DFCradle_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x988 | func: 6 | export: 6
u32 DFCradle_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DFCradle_Data);
}

// offset: 0x99C | func: 7
/** Returns the index of an object's current nearby station. 
  * The player uses a value bias, so that their station value will tend to be ahead of the cradle's station value 
  * when both the player and cradle are at the same location (i.e. while the player's aboard the cradle). 
  */
u8 DFCradle_getStationNumber(Object* self, f32 x, f32 z, u8 isPlayer) {
    DFCradle_Data* objData;
    f32 playerBias;
    s32 positionAlongRope;

    objData = self->data;
    
    playerBias = isPlayer * 80;

    //This seems to be an approximate way of describing an object's position along the cradle's Z-shaped rope path
    positionAlongRope = x + (z * 0.4f);
    
    //Approximate the object's position along the rope path
    if (positionAlongRope > (objData->pulleyValLower + playerBias)) {
        return DFCradle_STATION_0; //Between Lower Falls and first pulley
    } else if (positionAlongRope > (objData->pulleyValUpper + playerBias)) {
        return DFCradle_STATION_1; //Between first pulley and second pulley
    } else {   
        return DFCradle_STATION_2; //Between Upper Falls and second pulley
    }
}
