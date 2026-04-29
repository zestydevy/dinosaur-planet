#include "common.h"
#include "game/objects/object.h"

typedef struct {
    ObjSetup base;
} DIMExplosion_Setup;

typedef struct {
    s32 unused0;
    s32 fadeOut;
    f32 initialScale;  //Set during setup, but otherwise unused
} DIMExplosion_Data;

// offset: 0x0 | ctor
void DIMExplosion_ctor(void *dll) { }

// offset: 0xC | dtor
void DIMExplosion_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIMExplosion_setup(Object* self, DIMExplosion_Setup* objSetup, s32 arg2) {
    DIMExplosion_Data* objData = self->data;
    
    objData->initialScale = 0.01f;
    self->srt.scale = 0.01f;
    gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_389_Explosion, MAX_VOLUME, NULL, NULL, 0, NULL);
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0xA0 | func: 1 | export: 1
void DIMExplosion_control(Object* self) {
    TextureAnimator* texAnim;

    //Scroll texture
    texAnim = func_800348A0(self, 0, 0);
    if (texAnim != NULL) {
        texAnim->positionV -= 0x80;
        if (texAnim->positionV < -0x3E0) {
            texAnim->positionV = 0;
        }
    }
    
    //Scale up, destroying self after a while
    self->srt.scale += 0.008f * gUpdateRateF;
    if (self->srt.scale > 0.6f) {
        obj_destroy_object(self);
    }
    
    //Spin
    self->srt.yaw += gUpdateRate * 0x190;
}

// offset: 0x180 | func: 2 | export: 2
void DIMExplosion_update(Object *self) { }

// offset: 0x18C | func: 3 | export: 3
void DIMExplosion_print(Object* self, Gfx** arg1, Mtx** arg2, Vertex** arg3, Triangle** arg4, s8 visibility) {
    DIMExplosion_Data* objData;
    Gfx mode;
    Model* model;
    ModelInstance* modelInstance;
    ModelDLInfo *dlInfo;
    ModelDLInfo *dlInfoEnd;

    if (!visibility) {
        return;
    }
    
    objData = self->data;

    gDPSetOtherMode(&mode,
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_XLU_SURF2);

    //Animate a fade-out correlated with scale
    if (self->srt.scale > 0.25f) {
        objData->fadeOut = ((self->srt.scale - 0.25f) / 0.25f) * 255;
    } else {
        objData->fadeOut = 0;
    }
    
    if (objData->fadeOut > OBJECT_OPACITY_MAX) {
        objData->fadeOut = OBJECT_OPACITY_MAX;
    }
    
    //Draw model 1 (inner explosion)
    {
        self->modelInstIdx = 1;
        
        if (objData->fadeOut > 0x80) {
            self->opacityWithFade = 0x1FF - (objData->fadeOut * 2);
        } else {
            self->opacityWithFade = 0xFF;
        }
        
        draw_object(self, arg1, arg2, arg3, arg4, 1.0f);
    }
    
    //Draw model 0 (outer explosion)
    {
        self->modelInstIdx = 0;
        modelInstance = self->modelInsts[self->modelInstIdx];
        model = modelInstance->model;
        modelInstance->unk34 &= ~8;

        for (dlInfo = model->drawModes, dlInfoEnd = dlInfo + model->drawModesCount; dlInfo < dlInfoEnd; dlInfo++) {
            modelInstance->displayList[dlInfo->idx] = mode;
        }
        
        draw_object(self, arg1, arg2, arg3, arg4, 1.0f);
    }
}

// offset: 0x354 | func: 4 | export: 4
void DIMExplosion_free(Object *self, s32 a1) { }

// offset: 0x364 | func: 5 | export: 5
u32 DIMExplosion_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x374 | func: 6 | export: 6
u32 DIMExplosion_get_data_size(Object *self, u32 a1){
    return sizeof(DIMExplosion_Data);
}
