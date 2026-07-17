#include "PR/gbi.h"
#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "sys/gfx/animseq.h"
#include "sys/gfx/model.h"
#include "sys/map.h"
#include "sys/objects.h"
#include "sys/objprint.h"

typedef struct {
    f32 maskY;
    u8 state;
} ModelMask;

typedef enum {
    ModelMask_STATE_0_Zero_Masking,
    ModelMask_STATE_1_Fully_Masked,
    ModelMask_STATE_2_Contracting,
    ModelMask_STATE_3_Expanding
} ModelMask_States;

typedef struct {
    AnimObj_Data animData;
} GeneralScales_Data;

static ModelMask sVanishMask; //Used for Scales' belt teleport effect

#define TRI_FLAG (0x40 | 1)
#define MASK_HEIGHT_DEFAULT 200 //Generic height for DLLs using this mask effect, gets overridden by MASK_HEIGHT
#define MASK_HEIGHT 800
#define MASK_WIDTH 150
#define MASK_DEPTH 150

static void GeneralScales_handleMaskMessages(Object* self);
static void GeneralScales_drawMask(Object* self, Gfx** gdl, Mtx** mtxs, Vtx_t** vtxs, Triangle** pols);

// offset: 0x0 | ctor
void GeneralScales_ctor(void* dll) { }

// offset: 0xC | dtor
void GeneralScales_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void GeneralScales_obj_Setup(Object* self, AnimObj_Setup* objSetup, s32 reset) {
    AnimObj_Data* objData;

    objData = self->data;
    objData->eventGamebit = objSetup->unk1A;
    objData->unk7A = -1;
    objData->unk24 = 1.0f / (objSetup->camEaseDuration + 1.0f);
    objData->unk28 = -1;

    if ((self->unkDC == 0) && (objSetup->sequenceIdBitfield != 1)) {
        dll_anim->init_curve(objData, objSetup);
        self->unkDC = objSetup->sequenceIdBitfield + 1;
    } else if ((self->unkDC != 0) && ((objSetup->sequenceIdBitfield + 1) != self->unkDC)) {
        dll_anim->free_curve(objData);
        if (objSetup->sequenceIdBitfield != -1) {
            dll_anim->init_curve(objData, objSetup);
        }
        self->unkDC = objSetup->sequenceIdBitfield + 1;
    }
    
    sVanishMask.state = ModelMask_STATE_0_Zero_Masking;
    sVanishMask.maskY = 0.0f;
}

// offset: 0x194 | func: 1 | export: 1
void GeneralScales_obj_Control(Object* self) {
    s32 index;
    AnimObj_Data* objData;
    AnimObj_Setup* objSetup;
    Object* obj;
    Object** objects;
    Object* matchObj;
    s32 matches;
    s32 count;
    s8 seqSlot;
    u32 i;

    objSetup = (AnimObj_Setup*)self->setup;
    
    if ((objSetup == NULL) || (objSetup->sequenceIdBitfield == -1)) {
        return;
    }
    
    index = dll_anim->tick_obj(self, gUpdateRateMirror);

    GeneralScales_handleMaskMessages(self);

    if ((index == 0) || (self->seqSlot != SEQSLOT_ANIMOBJ)) {
        return;
    }
    
    objData = self->data;
    seqSlot = objData->seqSlot;
    
    matchObj = NULL;
    seqSlot = objData->seqSlot;
    objects = objGetObjects(&index, &count);

    for (matches = 0, index = 0; index < count; index++) {
        obj = objects[index];
        if (seqSlot == obj->seqSlot) {
            matchObj = obj;
        }
        
        if (obj->seqSlot == SEQSLOT_ANIMOBJ && obj->controlNo == 0x10) {
            objData = obj->data;    
            if (seqSlot == objData->seqSlot) {
                matches++;
            }
        }
    }
    
    if ((matches < 2) && (matchObj != NULL) && (matchObj->seqSlot != SEQSLOT_NONE)) {
        matchObj->seqSlot = SEQSLOT_NONE;
        dll_anim->end_obj_sequence(seqSlot);
    }
    
    self->seqSlot = SEQSLOT_NONE;
    
    objFreeObject(self);
}

// offset: 0x340 | func: 2 | export: 2
void GeneralScales_obj_Update(Object* self) { }

// offset: 0x34C | func: 3 | export: 3
void GeneralScales_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if ((visibility == FALSE) || (sVanishMask.state == ModelMask_STATE_1_Fully_Masked)) {
        return;
    }
    
    if ((sVanishMask.state == ModelMask_STATE_2_Contracting) || (sVanishMask.state == ModelMask_STATE_3_Expanding)) {
        GeneralScales_drawMask(self, gdl, mtxs, (Vtx_t**)vtxs, pols);
    }
    
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x418 | func: 4 | export: 4
void GeneralScales_obj_Free(Object* self, s32 onlySelf) {
    AnimObj_Data* objData;
    s32 i;

    objData = self->data;
    dll_anim->free_curve(objData);

    for (i = 0; i < 4; i++) {
        if (objData->sfxHandles[i] != 0) {
            dll_amSfx->Stop(objData->sfxHandles[i]);
        }
    }
    
    gDLL_5_AMSEQ2->vtbl->free(self, 0xFFFF, 0, 0, 0);

    if (objData->unk30 != 0) {
        dll_amSfx->Stop(objData->unk30);
    }
}

// offset: 0x524 | func: 5 | export: 5
u32 GeneralScales_obj_GetModelFlags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_8;
}

// offset: 0x534 | func: 6 | export: 6
u32 GeneralScales_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(GeneralScales_Data);
}

// offset: 0x548 | func: 7
void GeneralScales_handleMaskMessages(Object* self) {
    AnimObj_Data* objData;
    s32 fxParams;

    objData = self->data;

    switch (objData->lastMessage) {
    case 0:
        break;
    case 1:
        sVanishMask.state = ModelMask_STATE_1_Fully_Masked;
        break;
    case 2:
        sVanishMask.state = ModelMask_STATE_2_Contracting;
        dll_partfx->spawn(self, PARTICLE_556, NULL, 2, -1, NULL);
        dll_amSfx->Play(self, SOUND_B63, MAX_VOLUME, NULL, NULL, 0, NULL);
        dll_amSfx->Play(self, SOUND_B64, MAX_VOLUME, NULL, NULL, 0, NULL);
        sVanishMask.maskY = 0.0f;
        break;
    case 3:
        sVanishMask.state = ModelMask_STATE_3_Expanding;
        dll_partfx->spawn(self, PARTICLE_556, NULL, 2, -1, &fxParams);
        dll_amSfx->Play(self, SOUND_B63, MAX_VOLUME, NULL, NULL, 0, NULL);
        dll_amSfx->Play(self, SOUND_B64, MAX_VOLUME, NULL, NULL, 0, NULL);
        sVanishMask.maskY = MASK_HEIGHT;
        break;
    case 4:
        sVanishMask.state = ModelMask_STATE_0_Zero_Masking;
        break;
    }
    
    objData->lastMessage = 0;
}

// offset: 0x75C | func: 8
/**
  * Creates and animates an invisible cube that masks away Scales's mesh.
  */
void GeneralScales_drawMask(Object* self, Gfx** gdl, Mtx** mtxs, Vtx_t** vtxs, Triangle** pols) {
    /*0x0*/ static const DLTri sMaskTris[] = {
        MASK_TRI(TRI_FLAG, 2, 0, 1), 
        MASK_TRI(TRI_FLAG, 3, 2, 1),
        MASK_TRI(TRI_FLAG, 4, 6, 5),
        MASK_TRI(TRI_FLAG, 6, 7, 5),
        MASK_TRI(TRI_FLAG, 6, 2, 3),
        MASK_TRI(TRI_FLAG, 3, 7, 6),
        MASK_TRI(TRI_FLAG, 3, 1, 7),
        MASK_TRI(TRI_FLAG, 7, 1, 5),
        MASK_TRI(TRI_FLAG, 4, 1, 0),
        MASK_TRI(TRI_FLAG, 4, 5, 1),
        MASK_TRI(TRI_FLAG, 0, 2, 4),
        MASK_TRI(TRI_FLAG, 2, 6, 4)
    };
    /*0xC0*/ static const Vec3f sMaskVertCoords[] = {
        VEC3F(-MASK_WIDTH, 0, -MASK_DEPTH), 
        VEC3F( MASK_WIDTH, 0, -MASK_DEPTH), 
        VEC3F(-MASK_WIDTH, 0,  MASK_DEPTH), 
        VEC3F( MASK_WIDTH, 0,  MASK_DEPTH),
        VEC3F(-MASK_WIDTH, MASK_HEIGHT_DEFAULT, -MASK_DEPTH), 
        VEC3F( MASK_WIDTH, MASK_HEIGHT_DEFAULT, -MASK_DEPTH), 
        VEC3F(-MASK_WIDTH, MASK_HEIGHT_DEFAULT,  MASK_DEPTH), 
        VEC3F( MASK_WIDTH, MASK_HEIGHT_DEFAULT,  MASK_DEPTH)
    };

    Vtx_t* initVtx;
    Vtx_t* vtx;
    u32 i;
    SRT srt;

    initVtx = *vtxs;
    vtx = initVtx;
    
    //Set up mesh masking draw configs
    dlSetEnvColor(gdl, 0xFF, 0xFF, 0xFF, 0xFF);
    dlSetPrimColor(gdl, 0xFF, 0xFF, 0xFF, 0x80);
    
    gSPLoadGeometryMode(*gdl, G_SHADE | G_ZBUFFER | G_FOG | G_SHADING_SMOOTH);
    dlApplyGeometryMode(gdl);
    
    gDPSetCombineLERP(*gdl, TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED);
    dlApplyCombine(gdl);

    //Position the cube's vertices
    for (i = 0; i < ARRAYCOUNT(sMaskVertCoords); i++) {
        if (i < 4) {
            //Move the base of the cube with maskY
            vtx->ob[1] = sVanishMask.maskY;
        } else {
            vtx->ob[1] = MASK_HEIGHT;
        }

        if (i < 4) {
            vtx->ob[0] = sMaskVertCoords[i].x * 3.0f;
            vtx->ob[2] = sMaskVertCoords[i].z * 3.0f;
        } else {
            vtx->ob[0] = sMaskVertCoords[i].x * 3.0f;
            vtx->ob[2] = sMaskVertCoords[i].z * 3.0f;
        }

        vtx->cn[0] = 0xFF;
        vtx->cn[1] = 0;
        vtx->cn[2] = 0;
        vtx->cn[3] = 0x40;

        vtx++;
    }
    
    gDPSetOtherMode(*gdl,
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP |
        G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE, G_AC_NONE | G_ZS_PIXEL | Z_CMP | Z_UPD | IM_RD | CVG_DST_SAVE |
        ZMODE_XLU | FORCE_BL | G_RM_FOG_SHADE_A | GBL_c2(G_BL_CLR_IN, G_BL_0, G_BL_CLR_MEM, G_BL_1MA));
    dlApplyOtherMode(gdl);
    
    srt.transl.f[0] = self->globalPosition.f[0];
    srt.transl.f[1] = self->globalPosition.f[1] + 30.0f;
    srt.transl.f[2] = self->globalPosition.f[2];
    srt.yaw = self->srt.yaw;
    srt.pitch = 0;
    srt.roll = 0;
    srt.scale = 0.05f;
    camSetupObjectSRTMatrix(gdl, mtxs, &srt, 1, 0, NULL);
    
    gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(initVtx), 8, 0);
    dlTriangles(gdl, (DLTri*)sMaskTris, ARRAYCOUNT(sMaskTris));
    
    srt.transl.x = self->globalPosition.x;
    srt.transl.y = self->globalPosition.y + 30.0f;
    srt.transl.z = self->globalPosition.z;
    srt.yaw = self->srt.yaw;
    srt.pitch = 0x7FFF;
    srt.roll = 0;
    srt.scale = 0.05f;
    camSetupObjectSRTMatrix(gdl, mtxs, &srt, 1.0f, 0.0f, NULL);

    gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(initVtx), 8, 0);
    dlTriangles(gdl, (DLTri*)sMaskTris, ARRAYCOUNT(sMaskTris));
    
    //Create particles at the mask's edge
    {
        srt.yaw = 0;
        srt.pitch = 0;
        srt.roll = 0;
        srt.transl.x = 0.0f;
        srt.transl.y = (sVanishMask.maskY * 0.05f) + 30.0f;
        srt.transl.z = 0.0f;
        srt.scale = 1.0f;
        
        i = 8;
        while (i--) {
            dll_partfx->spawn(self, PARTICLE_55E, &srt, 2, -1, NULL);
        }
        
        srt.transl.y = 30.0f - (sVanishMask.maskY * 0.05f);

        i = 8;
        while (i--) {
            dll_partfx->spawn(self, PARTICLE_55E, &srt, 2, -1, NULL);
        }
    }
    
    //Animate the maskY value, based on the mask's current state
    {
        if (sVanishMask.state == ModelMask_STATE_2_Contracting) {
            sVanishMask.maskY += 4.0f * gUpdateRateF;
        } else if (sVanishMask.state == ModelMask_STATE_3_Expanding) {
            sVanishMask.maskY -= 4.0f * gUpdateRateF;
        }
        
        if ((sVanishMask.state == ModelMask_STATE_2_Contracting) && (sVanishMask.maskY >= MASK_HEIGHT)) {
            sVanishMask.maskY = MASK_HEIGHT;
            sVanishMask.state = ModelMask_STATE_0_Zero_Masking;
        } else if ((sVanishMask.state == ModelMask_STATE_3_Expanding) && (sVanishMask.maskY <= 0.0f)) {
            sVanishMask.state = ModelMask_STATE_1_Fully_Masked;
            sVanishMask.maskY = 0.0f;
        }
    }
    
    *vtxs = vtx;
}
