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

static u8 DFCradle_func_99C(Object* self, f32 x, f32 z, u8 isPlayer);

// offset: 0x0 | ctor
void DFCradle_ctor(void* dll) { }

// offset: 0xC | dtor
void DFCradle_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFCradle_setup(Object* self, DFCradle_Setup* setup, s32 reset) {
    DFCradle_Data* objData = self->data;
    /*0x18*/ s32 dCurveTypes[] = { 1 };
    Object* pulley;
    s32 stride;
    CurveSetup* curveSetup;
    s32 curveUID;
    
    self->srt.roll = setup->roll << 8;
    self->srt.pitch = setup->pitch << 8;
    self->srt.yaw = setup->yaw << 8;
    
    curveUID = gDLL_26_Curves->vtbl->func_1E4(self->srt.transl.x, self->srt.transl.f[1], self->srt.transl.f[2], dCurveTypes, ARRAYCOUNT(dCurveTypes), -1);
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
    objData->unkB6 = 0;
    objData->soundTimer = 0;
    objData->unk9C = 0.0f;
    
    //Get lower middle pulley value (closer to SwapStone Circle)
    pulley = objGetObjectByUID(0x208E);
    objData->pulleyValLower = pulley->globalPosition.x + (pulley->globalPosition.z * 0.4f);
    
    //Get upper middle pulley value (closer to DF Shrine)
    pulley = objGetObjectByUID(0x208F);
    objData->pulleyValUpper = pulley->globalPosition.x + (pulley->globalPosition.z * 0.4f);
}

// offset: 0x240 | func: 1 | export: 1
void DFCradle_control(Object* self) {
    DFCradle_Data* objData;
    Object** objects;
    Object* player;
    s32 count;
    s32 idx;
    u8 doDirectionReverse;
    u8 cradleVal;
    u8 playerVal;
    f32 dx;
    f32 dy;
    f32 dz;
    Object* texscroll;
    s32 pad;
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
        
        if (objData->unkB6) {
            objData->unkB6--;
        }
        
        player = objGetPlayer();
        if (player != NULL) {
            playerVal = DFCradle_func_99C(self, player->globalPosition.f[0], player->globalPosition.f[2], TRUE);
            cradleVal = DFCradle_func_99C(self, self->globalPosition.f[0],   self->globalPosition.f[2],   FALSE);

            doDirectionReverse = ((playerVal < cradleVal) && (objData->direction > 0)) || ((playerVal > cradleVal) && (objData->direction < 0));
            
            dx = self->globalPosition.f[0] - player->globalPosition.f[0];
            dy = self->globalPosition.f[1] - player->globalPosition.f[1];
            dz = self->globalPosition.f[2] - player->globalPosition.f[2];
            dx = SQ(dx) + SQ(dy) + SQ(dz);
            
            if (!objData->unkB6 && (cradleVal != objData->prevCradleVal)) {
                if (dx < 40000.0f) {
                    objData->unkB6 = 40;
                } else {
                    objData->unkB6 = 10;
                }
            }
            objData->prevCradleVal = cradleVal;

            if ((objData->unkB6 == 10) && (self != player->parent) && doDirectionReverse) {
                objData->direction = -objData->direction;
                if (objData->direction > 0) {
                    mainSetBits(BIT_1C, 0);
                } else {
                    mainSetBits(BIT_1C, 1);
                }
            }
            
            if (objData->unkB6 <= 10) {
                objects = objGetAllOfType(OBJTYPE_Pulley, &count);
                for (idx = 0; idx < count; idx++) {
                    objects[idx]->srt.yaw -= objData->unk9C * 500.0f * objData->direction * gUpdateRateF * (((objects[idx]->stateFlags & 1) * 2) - 1);
                }                

                cradleVal = mainGetBits(BIT_1C);
                if ((cradleVal == 0) && (objData->direction == 0)) {
                    cradleVal = 0;
                }
                if ((cradleVal != 0) && (objData->direction == -1)) {
                    cradleVal = 0;
                }
                
                if (cradleVal) {
                    if (objData->unk9C < 0.02f) {
                        objData->direction = -objData->direction;
                        objData->unk9C = 0.0f;
                    }
                } else {
                    s32 minDistance = 10000;
                    s32 distance; 
                    
                    for (idx = 0; idx < count; idx++) {
                        distance = vec3Distance(&self->globalPosition, &objects[idx]->globalPosition);
                        if (distance < minDistance) {
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
                    
                    objData->unk9C = (((minDistance - 20.0f) * 1.8f) / 100.0f) + 0.2f;
                }
                
                scrollSpeed = (objData->unk9C * -53.0f) * objData->direction;

                if (curves_func_800053B0((CurvesStruct*)&objData->curves, -scrollSpeed / 53.0f)) {
                    objData->direction = -objData->direction;
                    objData->unk9C = 0/*0.0f*/;
                    
                    if (dx < 40000.0f) {
                        objData->unkB6 = 60;
                    } else {
                        objData->unkB6 = 20;
                    }
                    
                    if (objData->direction > 0) {
                        mainSetBits(BIT_1C, 0);
                    } else {
                        mainSetBits(BIT_1C, 1);
                    }
                } else {
                    if ((objData->curves.unk74.x != 0/*.0f*/) || (objData->curves.unk74.z != 0/*.0f*/)) {
                        self->srt.yaw = mathAtan2f(objData->curves.unk74.x, objData->curves.unk74.z) + M_180_DEGREES;
                    }
                }

                self->srt.transl.x = objData->curves.unk68.x;
                self->srt.transl.y = objData->curves.unk68.y;
                self->srt.transl.z = objData->curves.unk68.z;
            } else {
                scrollSpeed = 0;
            }

            //Animate cradle ropes
            for (i = 0; i < 4; i++) {
                texscroll = objGetObjectByUID(dTexscrollUIDs[i]);
                if (texscroll != NULL) {
                    ((DLL_347_texscroll2*)texscroll->dll)->vtbl->change_scroll_speed(texscroll, scrollSpeed);
                }
            }
        }
        return;
    }

    //Check if the cradle has been powered
    objData->enabled = mainGetBits(BIT_DF_Cradle_Powered);
    
    if (objData->unk9C > 0.0f) {
        objData->unk9C -= 0.02f;
    } else {
        objData->unk9C = 0.0f;
    }
}

// offset: 0x908 | func: 2 | export: 2
void DFCradle_update(Object* self) { }

// offset: 0x914 | func: 3 | export: 3
void DFCradle_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x968 | func: 4 | export: 4
void DFCradle_free(Object* self, s32 onlySelf) { }

// offset: 0x978 | func: 5 | export: 5
u32 DFCradle_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x988 | func: 6 | export: 6
u32 DFCradle_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DFCradle_Data);
}

// offset: 0x99C | func: 7
u8 DFCradle_func_99C(Object* self, f32 x, f32 z, u8 isPlayer) {
    DFCradle_Data* objData;
    f32 playerBias;
    s32 temp_fv1;

    objData = self->data;
    
    playerBias = isPlayer * 80;
    temp_fv1 = (z * 0.4f) + x;
    
    if ((objData->pulleyValLower + playerBias) < temp_fv1) {
        return 0;
    }
    
    if ((objData->pulleyValUpper + playerBias) < temp_fv1) {
        return 1;
    }
    
    return 2;
}
