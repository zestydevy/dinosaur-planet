#include "common.h"
#include "segment_334F0.h"
#include "sys/memory.h"

void dl_clear_geometry_mode(Gfx **gdl, u32 mode);
void dl_set_geometry_mode(Gfx **gdl, u32 mode);
void func_8001EAA4(s32, s32, s32, f32);

typedef struct {
/*00*/    ObjSetup base;
/*18*/    s8 yaw;
/*19*/    s8 modelIdx;
/*1A*/    s16 unused1A;
/*1C*/    s16 scale;
} WL_Crystal_Setup;

typedef struct {
/*00*/    s16 unused0;
/*02*/    s16 yawSpeed;       //rotation speed for crystal and sun
/*04*/    s16 rollSpeed;      //roll speed for sun
/*06*/    s16 pad6;
/*08*/    s16* inroomBuffer;  //unused aside from setup, seems to store a value for each vertex
/*0C*/    DLL_Unknown *sunFX; //projgfx DLL (2, 18) used by Quan Ata Lachu sun
/*10*/    s8 unused10;
/*11*/    u8 showCrystal;     //unloads crystal once transformation into sun complete
} WL_Crystal_Data;

typedef enum {
    WMSun_Core = 0,
    WMSun_Middle_Shell = 1,
    WMSun_Outer_Shell = 2
} WMSun_Model_Indices;

#define WMinroom_Max_Opacity     250

#define WMSun_Max_Opacity_Core   255
#define WMSun_Max_Opacity_Middle 85
#define WMSun_Max_Opacity_Outer  25

/*0x0*/ static DLL_Unknown* data_sun_modGFX = NULL;

/*0x0*/ static s16 fxTimer1;
/*0x2*/ static s16 fxTimer2;
/*0x4*/ static s16 fxTimer3;
/*0x6*/ static s16 fxTimer4;
/*0x8*/ static s16 fxTimer5;

static void WL_Crystal_handle_sun_flare_effects(Object* self);

// offset: 0x0 | ctor
void WL_Crystal_ctor(void *dll) { }

// offset: 0xC | dtor
void WL_Crystal_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_Crystal_setup(Object* self, WL_Crystal_Setup* objSetup, s32 arg2) {
    WL_Crystal_Data* objData;
    ModelInstance* modelInstance;
    Vtx *vertices;
    s16 i;

    objData = self->data;
    
    if ((gDLL_29_Gplay->vtbl->get_map_setup(self->mapID) == 3) && !main_get_bits(BIT_Set_During_Spirit_Release_1)){
        main_set_bits(BIT_Set_During_Spirit_Release_1, TRUE);
    }
    
    objData->inroomBuffer = NULL;
    objData->sunFX = NULL;
    objData->showCrystal = TRUE;
    
    //Set up Warlock Mountain's Crystal
    if (self->id == OBJ_WL_Crystal){
        self->srt.yaw = objSetup->yaw << 8;
        objData->yawSpeed = 100;
        if (objSetup->scale >= 1000) {
            self->srt.scale = objSetup->scale / 1000.0f;
        } else {
            self->srt.scale = 1.0f;
        }
        return;
    }

    //Set up Quan Ata Lachu sun
    if (self->id == OBJ_WMsun){
        fxTimer1 = 800;
        fxTimer2 = 800;
        fxTimer3 = 800;
        fxTimer4 = 800;
        fxTimer5 = 800;

        self->srt.yaw = objSetup->yaw << 8;
        if (objSetup->scale >= 0) {
            self->srt.scale = objSetup->scale / 1000.0f;
        } else {
            self->srt.scale = 1.0f;
        }

        self->modelInstIdx = objSetup->modelIdx;        
        if (self->modelInstIdx == WMSun_Core) {
            objData->sunFX = dll_load_deferred(0x2012, 1);
            objData->yawSpeed = rand_next(300, 600);
            objData->rollSpeed = rand_next(300, 600);
            data_sun_modGFX = dll_load_deferred(0x1036, 1);
        } else {
            if (self->modelInstIdx == WMSun_Middle_Shell) {
                objData->yawSpeed = rand_next(500, 800);
                objData->rollSpeed = rand_next(500, 800);
            } else if (self->modelInstIdx == WMSun_Outer_Shell) {
                objData->sunFX = dll_load_deferred(0x2012, 1);
                objData->yawSpeed = rand_next(700, 1000);
                objData->rollSpeed = rand_next(700, 1000);
            }
        }

        self->opacity = 0;
        return;
    }

    //Set up room-filling electrified wall effect for main chamber (unused)
    //(Model uses 200x200px animated texture incompatible with N64 TMEM cache)
    if (self->id == OBJ_WMinroom){
        modelInstance = self->modelInsts[self->modelInstIdx];
        objData->inroomBuffer = mmAlloc(160, ALLOC_TAG_OBJECTS_COL, NULL);

        i = 20;
        while (i != 0) {
            i--;
            objData->inroomBuffer[i] = rand_next(0, modelInstance->model->vertexCount - 1);
            objData->inroomBuffer[i + 20] = 0;
            objData->inroomBuffer[i + 40] = rand_next(10, 20);
            objData->inroomBuffer[i + 60] = rand_next(80, 255);
        }
        
        //Set vertex alpha (animated vertex buffer 0)
        modelInstance = self->modelInsts[self->modelInstIdx];
        vertices = modelInstance->vertices[0];
        i = modelInstance->model->vertexCount;
        while (i != 0) {
            i--;
            vertices[i].n.a = 85;
        }        
        
        //Set vertex alpha (animated vertex buffer 1)
        vertices = modelInstance->vertices[1];
        i = modelInstance->model->vertexCount;
        while (i != 0) {
            i--;
            vertices[i].n.a = 85;
        }
        
        //Start at 0 opacity, and set overall scale
        self->opacity = 0;
        if (objSetup->scale) {
            self->srt.scale = 1.0f / (objSetup->scale / 1000.0f);
        }
    }
}

// offset: 0x47C | func: 1 | export: 1
void WL_Crystal_control(Object* self) {
    WL_Crystal_Data* objData;
    TextureAnimator* animTexture;
    s16 pad;
    s16 yawAcceleration;
    s16 goal;
    f32 transformSpeed;
    SRT transform;
    s16 opacity;

    objData = self->data;

    goal = 0;
    yawAcceleration = 1;
    transformSpeed = 0.0f;

    //Handle Warlock Mountain's Crystal
    if (self->id == OBJ_WL_Crystal) {
        //Remove if gamebit 0x38F is set
        if (main_get_bits(BIT_WM_Quan_Ata_Lachu_Sun)) {
            obj_destroy_object(self);
        }
        
        //Scroll texture UVs
        animTexture = func_800348A0(self, 1, 0);
        if (animTexture) {
            animTexture->positionV -= 0x10;
            if (animTexture->positionV < -0x3E0) {
                animTexture->positionV = 0;
            }
        }

        //Rotate faster as more spirits are placed
        //(for first 6 spirits - transforms into sun afterwards)
        if (main_get_bits(BIT_Set_During_Spirit_Release_1)) {
            goal = 100;
        }
        if (main_get_bits(BIT_21C)) {
            goal = 200;
        }
        if (main_get_bits(BIT_21D)) {
            goal = 400;
        }
        if (main_get_bits(BIT_21F_Spirit_Collected)) {
            goal = 800;
        }
        if (main_get_bits(BIT_221)) {
            goal = 1600;
        }
        if (main_get_bits(BIT_222)) {
            yawAcceleration = 3;
            goal = 6400;
            transformSpeed = 0.00375f;
        }
        
        //Increase rotation speed to goal speed
        if (objData->yawSpeed < goal) {
            objData->yawSpeed += gUpdateRate * yawAcceleration;

            //After 6th spirit placed, shrink and rise up
            //(@bug?: applies these float calcs while transformSpeed is zero - could check BIT_222 is set)
            self->srt.scale -= transformSpeed * gUpdateRateF;
            self->srt.transl.y += transformSpeed * gUpdateRateF * 50.0f;
        } else if ((main_get_bits(BIT_222)) && (main_get_bits(BIT_38D) == 0)) {
            //When yawSpeed at goal and 6th spirit is deposited, set gamebits and destroy crystal 
            main_set_bits(BIT_38D, 1);
            main_set_bits(BIT_370, 0);
            objData->showCrystal = FALSE;
        }

        //While gamebit 0x38D not set and crystal spinning rapidly, 1% chance of camera shake(?)
        if (!main_get_bits(BIT_38D) && (objData->yawSpeed > 2400) && !rand_next(0, 100)) {
            func_80003B70(((objData->yawSpeed - 2400) / 2400.0f) * 0.8f);
            main_set_bits(BIT_370, 1);
        }
        self->srt.yaw += objData->yawSpeed;

        //Destroy once 6th spirit is deposited and transformation into sun complete
        if (!objData->showCrystal) {
            obj_destroy_object(self);
        }
        return;
    }

    //Handle room-filling electrified wall effect for main chamber of WM (unused)
    if (self->id == OBJ_WMinroom) {
        if (main_get_bits(BIT_WM_Quan_Ata_Lachu_Sun)) {
            //Increase opacity
            if (self->opacity < WMinroom_Max_Opacity) {
                opacity = self->opacity + gUpdateRate;
            }
            if (opacity > WMinroom_Max_Opacity) {
                opacity = WMinroom_Max_Opacity;
            }
            self->opacity = opacity;

            //Scroll texture UVs
            animTexture = func_800348A0(self, 0, 0);
            if (animTexture != NULL) {
                animTexture->positionU -= gUpdateRate * 8;
                if (animTexture->positionU < -0x3E0) {
                    animTexture->positionU = 0;
                }
            }
        }
        return;
    }

    //Handle WMsun
    if (main_get_bits(BIT_WM_Quan_Ata_Lachu_Sun)) {
        //Fade in the Quan Ata Lachu sun
        if (self->modelInstIdx == WMSun_Core && self->opacity != WMSun_Max_Opacity_Core) {
            if (self->opacity < WMSun_Max_Opacity_Core) {
                opacity = self->opacity + gUpdateRate;
            }
            if (opacity > WMSun_Max_Opacity_Core) {
                opacity = WMSun_Max_Opacity_Core;
            }
            self->opacity = opacity;
        } else if (self->modelInstIdx == WMSun_Middle_Shell && self->opacity != WMSun_Max_Opacity_Middle) {
            if (self->opacity < WMSun_Max_Opacity_Middle) {
                opacity = self->opacity + gUpdateRate;
            }
            if (opacity > WMSun_Max_Opacity_Middle) {
                opacity = WMSun_Max_Opacity_Middle;
            }
            self->opacity = opacity;
        } else if (self->modelInstIdx == WMSun_Outer_Shell && self->opacity != WMSun_Max_Opacity_Outer) {
            if (self->opacity < WMSun_Max_Opacity_Outer) {
                opacity = self->opacity + gUpdateRate;
            }
            if (opacity > WMSun_Max_Opacity_Outer) {
                opacity = WMSun_Max_Opacity_Outer;
            }
            self->opacity = opacity;
        }

        //Create particles and effects
        if (self->modelInstIdx == WMSun_Core) {
            gDLL_17_partfx->vtbl->spawn(self, 0x1A9, NULL, 0x10000, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, 0x1A9, NULL, 0x10000, -1, NULL);

            //25% chance of 3D mesh light ray effect
            if (!rand_next(0, 4)) {
                //TODO: use exact DLL interface
                data_sun_modGFX->vtbl->func[0].withSixArgs((s32)self, 0, 0, 1, -1, 0);
            }

            //~0.6% chance of creating particles and playing sound effect
            if (!rand_next(0, 150)) {
                goal = 50;
                transform.transl.x = 0.0f;
                transform.transl.y = 0.0f;
                transform.transl.z = 0.0f;
                transform.scale = 1.0f;
                transform.roll = rand_next(0, 0xFFFF);
                transform.pitch = rand_next(0, 0xFFFF);
                transform.yaw = rand_next(0, 0xFFFF);
                gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_WM_Sun_Whoosh, 0x43, NULL, NULL, 0, NULL);
                while (goal) {
                    goal--;
                    gDLL_17_partfx->vtbl->spawn(self, 0x1AA, &transform, 0x10000, -1, NULL);
                }
            }
            WL_Crystal_handle_sun_flare_effects(self);
        }
        return;
    }

    self->srt.roll += objData->rollSpeed;
    self->srt.yaw += objData->yawSpeed;
    if (main_get_bits(BIT_38D) == 0) {
        return;
    }

    //Handle crystal-to-sun transformation sequence effects
    if (self->modelInstIdx == WMSun_Core) {
        if (fxTimer4 == 0) {
            if ((fxTimer5 > 600) && !rand_next(0, 10)) {
                func_80003B70(2.8f);
            }
            if ((fxTimer5 < 700) && !rand_next(0, 5)) {
                objData->sunFX->vtbl->func[0].withSevenArgs((s32)self, 0, 0, 0x10000, -1, 0x12, 0);
            }
            if (fxTimer5 > 0) {
                fxTimer5 -= gUpdateRate;
                if ((fxTimer5 < 200) && !rand_next(0, 1)) {
                    transform.transl.x = (fxTimer5 / 200.0f) + 0.1f;
                    gDLL_17_partfx->vtbl->spawn(self, 0x1B1, &transform, 0x10000, -1, NULL);
                }

                goal = 200;
                if (fxTimer5 <= 0) {
                    fxTimer5 = 0;
                    while (goal) {
                        goal--;
                        gDLL_17_partfx->vtbl->spawn(self, 0x1B2, NULL, 0x10000, -1, NULL);
                    }
                    main_set_bits(BIT_38D, 0);
                    main_set_bits(BIT_WM_Quan_Ata_Lachu_Sun, 1);
                    func_80000860(self, self, 0x31, 0);
                    func_80003B70(4.8f);
                }
            }
        }

        if (fxTimer1 == 0) {
            if (!rand_next(0, (fxTimer2 + 2) / 200)) {
                gDLL_17_partfx->vtbl->spawn(self, 0x1AE, &transform, 0x10000, -1, NULL);
            }
            if (!rand_next(0, fxTimer2 / 70)) {
                gDLL_17_partfx->vtbl->spawn(self, 0x1AB, &transform, 0x10000, -1, NULL);
            }
            if (!rand_next(0, fxTimer2 / 70)) {
                gDLL_17_partfx->vtbl->spawn(self, 0x1AB, &transform, 0x10000, -1, NULL);
            }
            if (!rand_next(0, fxTimer2 / 70)) {
                gDLL_17_partfx->vtbl->spawn(self, 0x1AB, &transform, 0x10000, -1, NULL);
            }
            if (!rand_next(0, fxTimer2 / 70)) {
                gDLL_17_partfx->vtbl->spawn(self, 0x1AB, &transform, 0x10000, -1, NULL);
            }
            if (!rand_next(0, fxTimer2 / 70)) {
                gDLL_17_partfx->vtbl->spawn(self, 0x1AB, &transform, 0x10000, -1, NULL);
            }
            if (fxTimer2 > 0) {
                fxTimer2 -= gUpdateRate;
                if (fxTimer2 < 0) {
                    fxTimer2 = 0;
                }
            }
        } else {
            transform.transl.x = 0.1f;
            gDLL_17_partfx->vtbl->spawn(self, 0x1B0, &transform, 0x10000, -1, NULL);
            if ((fxTimer1 > 50) && !rand_next(0, 1)) {
                transform.transl.x = ((fxTimer1 - 50) / 750.0f) + 0.1f;
                gDLL_17_partfx->vtbl->spawn(self, 0x1B0, &transform, 0x10000, -1, NULL);
            }
            if (fxTimer1 < 700) {
                goal = fxTimer1 / 60;
                while (goal) {
                    goal--;
                    transform.transl.x = fxTimer1 / 300.0f;
                    gDLL_17_partfx->vtbl->spawn(self, 0x1AF, &transform, 0x10000, -1, NULL);
                }
            }
            if (fxTimer1 > 0) {
                fxTimer1 -= gUpdateRate;
                if (fxTimer1 <= 0) {
                    fxTimer1 = 0;
                    func_80000860(self, self, 0x30, 0);
                    func_80000860(self, self, 0x34, 0);
                }
            }
            if (rand_next(0, 8) == 0) {
                func_80003B70(2.8f);
            }
        }
    }

    if ((self->modelInstIdx == WMSun_Middle_Shell) && (fxTimer2 == 0)) {
        if (rand_next(0, fxTimer3 / 60) == 0) {
            gDLL_17_partfx->vtbl->spawn(self, 0x1AC, NULL, 0x10000, -1, NULL);
        }
        if (rand_next(0, fxTimer3 / 60) == 0) {
            gDLL_17_partfx->vtbl->spawn(self, 0x1AC, NULL, 0x10000, -1, NULL);
        }
        if (rand_next(0, fxTimer3 / 60) == 0) {
            gDLL_17_partfx->vtbl->spawn(self, 0x1AC, NULL, 0x10000, -1, NULL);
        }
        if (fxTimer3 > 0) {
            fxTimer3 -= gUpdateRate;
            if (fxTimer3 < 0) {
                fxTimer3 = 0;
            }
        }
    }
    
    if ((self->modelInstIdx == WMSun_Outer_Shell) && (fxTimer2 <= 0) && (fxTimer3 <= 0)) {
        if (rand_next(0, fxTimer4 / 60) == 0) {
            gDLL_17_partfx->vtbl->spawn(self, 0x1AD, NULL, 0x10000, -1, NULL);
        }
        if (rand_next(0, fxTimer4 / 60) == 0) {
            gDLL_17_partfx->vtbl->spawn(self, 0x1AD, NULL, 0x10000, -1, NULL);
        }
        if (fxTimer4 > 0) {
            fxTimer4 -= gUpdateRate;
            if (fxTimer4 <= 0) {
                fxTimer4 = 0;
            }
        }
    }
}

// offset: 0x1604 | func: 2 | export: 2
void WL_Crystal_update(Object *self) { }

// offset: 0x1610 | func: 3 | export: 3
void WL_Crystal_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    WL_Crystal_Data* objData = self->data;
    
    if (visibility && objData->showCrystal) {
        dl_clear_geometry_mode(gdl, 0x10000);
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        dl_set_geometry_mode(gdl, 0x10000);
    }
}

// offset: 0x16C4 | func: 4 | export: 4
void WL_Crystal_free(Object* self, s32 arg1) {
    WL_Crystal_Data* objData;

    objData = self->data;
    
    //Free WMsun's flare modgfx DLL
    if (data_sun_modGFX && (self->id == OBJ_WMsun) && (self->modelInstIdx == WMSun_Core)) {
        dll_unload(data_sun_modGFX);
        data_sun_modGFX = NULL;
    }
    
    //Free WMinroom's (unused) vertex-related buffer
    if (objData->inroomBuffer) {
        mmFree(objData->inroomBuffer);
    }
    objData->inroomBuffer = NULL;
    
    //Free WMsun's projgfx DLL
    if (objData->sunFX) {
        dll_unload(objData->sunFX);
    }
    objData->sunFX = NULL;
}

// offset: 0x1794 | func: 5 | export: 5
u32 WL_Crystal_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x17A4 | func: 6 | export: 6
u32 WL_Crystal_get_data_size(Object *self, u32 a1) {
    return sizeof(WL_Crystal_Data);
}

// offset: 0x17B8 | func: 7
void WL_Crystal_handle_sun_flare_effects(Object* self) {
    static f32 dataEffectScale = 0.0f;
    static f32 dimRoomEffect = 0.0f;
    Vec3f unitVector = { 0.0f, 0.0f, -1.0f };
    Vec3f vCameraLook = { 0.0f, 0.0f, -1.0f };
    Vec3f vSunToCamera;
    f32 distanceToCamera2D;
    f32 dotProduct;
    f32 sqMagnitudeA;
    f32 distanceToCamera3D;
    f32 sqMagnitudeB;
    f32 dotProductResult;
    f32 sinTheta;
    SRT transform;
    Camera* camera;

    self->srt.yaw += 400;
    transform.transl.z = 0.0f;
    transform.transl.y = 0.0f;
    transform.transl.x = 0.0f;
    transform.pitch = 0;
    transform.roll = 0;
    transform.scale = 1.0f;
    transform.yaw = self->srt.yaw;

    camera = get_camera();
    if (camera == NULL) {
        return;
    }

    //Get direction camera's facing (unit vector)
    transform.yaw = 0x8000 - camera->srt.yaw;
    rotate_vec3(&transform, &vCameraLook.f[0]);

    //Get direction pointing from sun to camera (unit vector)
    VECTOR_SUBTRACT(self->srt.transl, camera->srt.transl, vSunToCamera)
    VECTOR_NORMALISE(vSunToCamera, distanceToCamera3D)

    //Get dot product between camera direction vector and sun-to-camera vector
    dotProduct = DOT_PRODUCT(vSunToCamera, vCameraLook);

    //Get vectors' square magnitudes
    //(they're normalised so it should generally be 1, but it handles
    // special case where one of the vectors' magnitudes is 0)
    sqMagnitudeA = VECTOR_MAGNITUDE_SQUARED(vSunToCamera); 
    sqMagnitudeB = VECTOR_MAGNITUDE_SQUARED(vCameraLook);
    sqMagnitudeA *= sqMagnitudeB;
    if (sqMagnitudeA != 0.0f) {
        sqMagnitudeB = sqrtf(sqMagnitudeA);
    }

    dotProductResult = sqMagnitudeB != 0.0f ? dotProduct / sqMagnitudeB : 0.0f;
    if (sqMagnitudeB != 0.0f) {
        dotProductResult = dotProduct / sqMagnitudeB;
    } else {
        dotProductResult = 0.0f;
    }

    //If camera facing towards sun (within 90 degrees of facing exactly towards it)
    if (dotProductResult > 0.0f) {
        //Get 2D unit vector pointing from sun to camera (ignore Y component)
        vSunToCamera.x = self->srt.transl.x - camera->srt.transl.x;
        vSunToCamera.y = 0/*.0f*/;
        vSunToCamera.z = self->srt.transl.z - camera->srt.transl.z;
        VECTOR_NORMALISE(vSunToCamera, distanceToCamera2D)

        sqMagnitudeA = VECTOR_MAGNITUDE_SQUARED(unitVector);
        sqMagnitudeB = VECTOR_MAGNITUDE_SQUARED(vSunToCamera);
        sqMagnitudeA *= sqMagnitudeB;
        if (sqMagnitudeA != 0.0f) {
            sqMagnitudeB = sqrtf(sqMagnitudeA);
        }

        //If camera facing within 45 degrees of sun, tint room darker and create flare effects
        if (dotProductResult > 0.5f) {
            transform.transl.x = vSunToCamera.f[0] * 20.0f;
            transform.transl.y = 0/*.0f*/;
            transform.transl.z = vSunToCamera.f[2] * 20.0f;
            
            sinTheta = fsin16_precise((dotProductResult - 0.5f) * 0x7FFF);
            if ((sinTheta - dataEffectScale) > 0.1f || (sinTheta - dataEffectScale) < -0.1f) {
                dataEffectScale += (sinTheta - dataEffectScale) / gUpdateRateF;
            }

            transform.scale = dataEffectScale;
            if (transform.scale > 0.2f) {
                if (dimRoomEffect < 4.0f) {
                    dimRoomEffect += (transform.scale - 0.2f) / 100.0f;
                }
                transform.scale -= dimRoomEffect;
                if (transform.scale < 0.2f) {
                    transform.scale = 0.2f;
                }
            } else {
                dimRoomEffect -= (transform.scale - 0.2f) / 20.0f;
            }

            transform.scale += 0.0005f * rand_next(0, 30);
            if (dataEffectScale > 0.05f) {
                dataEffectScale -= 0.0002f;
            }
            transform.roll = 0;
            transform.pitch = 0;
            transform.yaw = 65535.0f * dotProductResult;

            //Tint the room when looking towards the sun
            if (distanceToCamera2D < 12450.0f) {
                func_8001EAA4(0xFF, 0xFF, 0x9B, transform.scale);
            }

            gDLL_17_partfx->vtbl->spawn(self, 0x6D, &transform, 0x10000, -1, NULL);

            transform.roll = 1;
            transform.scale = 0.08f;
            transform.scale += 0.0005f * rand_next(0, 10);
            gDLL_17_partfx->vtbl->spawn(self, 0x6D, &transform, 0x10000, -1, NULL);
            return;
        }

        if ((0.0f - dataEffectScale) > 0.001f) {
            dataEffectScale += (0.0f - dataEffectScale) / gUpdateRateF;
        } else if ((0.0f - dataEffectScale) < -0.001f) {
            dataEffectScale += (0.0f - dataEffectScale) / gUpdateRateF;
        }

        if (dimRoomEffect > 0.0f) {
            dimRoomEffect -= (0.01f * gUpdateRateF);
            if (dimRoomEffect < 0/*.0f*/) {
                dimRoomEffect = 0.0f;
            }
        }
    } else if (dimRoomEffect > 0.00f) {
        dimRoomEffect -= 0.01f * gUpdateRateF;
        if (dimRoomEffect < 0/*.0f*/) {
            dimRoomEffect = 0/*.0f*/;
        }
    }
}
