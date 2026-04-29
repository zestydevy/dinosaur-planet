#include "common.h"
#include "sys/map.h"
#include "sys/gfx/modgfx.h"
#include "dlls/objects/common/collectable.h"

#define MASK_SPEED 0.1f

#define MASK_MAX 82.0f
#define MASK_MIN -5.0f

#define MASK_TOP_Y 20
#define MASK_BASE_Y -7
#define MASK_WIDTH_TOP 2.0
#define MASK_WIDTH_BASE 34.5

#define SCALE_Y 20.0f
#define SCALE_X 20.0f
#define SCALE_Z 20.0f

#define THRESHOLD_SNOW_EVAPORATE 60.0f
#define THRESHOLD_END_FADEOUT 68.0f

typedef struct {
    ObjSetup base;
    s8 yaw;
    s16 tentIndex;     //An index (0-11 inclusive) for deciding which tent holds the bridge cog
    u16 unused1C;
    s16 gamebitBurnt;  //Set when the tent's been destroyed, prevents it reappearing
} DIMTent_Setup;

typedef struct {
    f32 maskY;      //Progress of the burn masking effect
    f32 maskSpeed;  //Speed/direction of the burn masking effect
    u8 isBurning;   //Starts the burn mask animation
    s8 hitPoints;   //Attacks needed until the tent begins burning (always starts at 1)
} DIMTent_Data;

/*0x0*/ static DLL_IModgfx* dModGfxDLL = NULL;

#define TRI_FLAG (0x40)
#define MASK_TRI(v0, v1, v2) {TRI_FLAG, v0, v1, v2, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}

/*0x0*/ static const DLTri sMaskTris[12] = {
    MASK_TRI(2, 1, 0),
    MASK_TRI(3, 1, 2),
    MASK_TRI(4, 5, 6),
    MASK_TRI(6, 5, 7),
    MASK_TRI(6, 3, 2),
    MASK_TRI(3, 6, 7),
    MASK_TRI(3, 7, 1),
    MASK_TRI(7, 5, 1),
    MASK_TRI(4, 0, 1),
    MASK_TRI(4, 1, 5),
    MASK_TRI(0, 4, 2),
    MASK_TRI(2, 4, 6)
};

/*0xC0*/ static const Vec3f sMaskVertCoords[8] = {
   {{-MASK_WIDTH_TOP,  MASK_TOP_Y,  -MASK_WIDTH_TOP}},
   {{ MASK_WIDTH_TOP,  MASK_TOP_Y,  -MASK_WIDTH_TOP}},
   {{-MASK_WIDTH_TOP,  MASK_TOP_Y,   MASK_WIDTH_TOP}},
   {{ MASK_WIDTH_TOP,  MASK_TOP_Y,   MASK_WIDTH_TOP}},
   {{-MASK_WIDTH_BASE, MASK_BASE_Y, -MASK_WIDTH_BASE}},
   {{ MASK_WIDTH_BASE, MASK_BASE_Y, -MASK_WIDTH_BASE}},
   {{-MASK_WIDTH_BASE, MASK_BASE_Y,  MASK_WIDTH_BASE}},
   {{ MASK_WIDTH_BASE, MASK_BASE_Y,  MASK_WIDTH_BASE}}
};

/*0x120*/ static const f32 sMaskSpeeds[1] = {
    -MASK_SPEED
};

static void DIMTent_draw_mask(Object* self, Gfx** gdl, Mtx** mtxs, Vtx_t** vtxs, Triangle** pols);

// offset: 0x0 | ctor
void DIMTent_ctor(void *dll) { }

// offset: 0xC | dtor
void DIMTent_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DIMTent_setup(Object* self, DIMTent_Setup* objSetup, s32 arg2) {
    DIMTent_Data* objData = self->data;

    self->srt.yaw = objSetup->yaw << 8;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;

    objData->hitPoints = 1;

    //Check if already burnt
    if (main_get_bits(objSetup->gamebitBurnt)) {
        objData->hitPoints = 0;
        self->objhitInfo->unk58 &= ~1;
        self->opacity = 0;
    }

    objData->maskSpeed = sMaskSpeeds[0];
    dModGfxDLL = dll_load_deferred(DLL_ID_165, 1);
}

// offset: 0xD4 | func: 1 | export: 1
void DIMTent_control(Object* self) {
    Object* listedObject;
    DIMTent_Setup* objSetup;
    DIMTent_Data* objData;
    s32 index;
    s8 damaged;

    objSetup = (DIMTent_Setup*)self->setup;
    objData = self->data;

    //Do nothing if invisible / already burnt
    if (self->opacity == 0) {
        return;
    }

    //When burning, remove collision and gradually mask the model away
    if (objData->hitPoints <= 0) {
        self->objhitInfo->unk58 &= ~1;

        //Handle burn mask speeds/progress (@bug: can ping-pong and continue burning in reverse)
        if (objData->isBurning == TRUE) {
            objData->maskY += objData->maskSpeed * gUpdateRateF;
            if (objData->maskY > MASK_MAX) {
                objData->maskY = MASK_MAX;
                objData->maskSpeed = -MASK_SPEED;
            } else if (objData->maskY < MASK_MIN) {
                objData->maskY = MASK_MIN;
                objData->maskSpeed = MASK_SPEED;
                dModGfxDLL->vtbl->func0(self, 0, 0, 4, -1, 0);
            }
        }
    }

    //Do nothing if this is the unused big tent (probably intended to be burnt a different way)
    if (self->id == OBJ_DIMBigTent) {
        return;
    }

    //Check for damage via polyhits
    damaged = FALSE;
    for (index = 0; index < self->polyhits->unk10F; index++) {
        listedObject = self->polyhits->unk100[index];

        //Curiously, setting fire to the tents via SnowHorn attacks seems to be intentional!
        if ((listedObject->id == OBJ_DIMSnowHorn1) || (listedObject->id == OBJ_DIMCannonBall)) {
            damaged = TRUE;
            break;
        }
    }

    //Handle being damaged
    if (damaged) {
        //Decrement the tent's health (health starts at 1, so it begins burning after any hit anyway)
        objData->hitPoints--;
        if (objData->hitPoints > 0) {
            return;
        }

        //After being fully damaged, track that the tent has been burnt and start the burning effect
        main_set_bits(objSetup->gamebitBurnt, 1);
        objData->isBurning = TRUE;

        //Drop the bridge cog if this tent's index matches the random one picked by DIMLevelControl
        if (main_get_bits(BIT_DIM_Gear_3_Random_Tent) == (u32)objSetup->tentIndex) {
            Collectable_Setup* cogSetup;
            cogSetup = (Collectable_Setup*)obj_alloc_setup(sizeof(Collectable_Setup), OBJ_DIMBridgeCogCol);
            cogSetup->base.x = objSetup->base.x;
            cogSetup->base.y = objSetup->base.y + 8.0f;
            cogSetup->base.z = objSetup->base.z;
            cogSetup->base.loadFlags = objSetup->base.loadFlags;
            cogSetup->base.byte5 = objSetup->base.byte5;
            cogSetup->base.byte6 = objSetup->base.byte6;
            cogSetup->base.fadeDistance = objSetup->base.fadeDistance;
            cogSetup->gamebitCollected = BIT_DIM_Gear_3;
            cogSetup->gamebitSecondary = NO_GAMEBIT;
            cogSetup->gamebitCount = NO_GAMEBIT;
            cogSetup->objHitsValue = 5;
            cogSetup->yaw = self->srt.yaw >> 8;
            obj_create((ObjSetup*)cogSetup, 5, self->mapID, -1, NULL);
        }
    }
}

// offset: 0x37C | func: 2 | export: 2
void DIMTent_update(Object *self) { }

// offset: 0x388 | func: 3 | export: 3
void DIMTent_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DIMTent_Data* objData = self->data;

    if (visibility) {
        if (objData->isBurning == TRUE) {
            DIMTent_draw_mask(self, gdl, mtxs, (Vtx_t**)vtxs, pols);
        }
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x430 | func: 4 | export: 4
void DIMTent_free(s32 self, s32 arg1) {
    if (dModGfxDLL) {
        dll_unload(dModGfxDLL);
    }
}

// offset: 0x480 | func: 5 | export: 5
u32 DIMTent_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x490 | func: 6 | export: 6
u32 DIMTent_get_data_size(Object *self, u32 a1) {
    return sizeof(DIMTent_Data);
}

// offset: 0x4A4 | func: 7
void DIMTent_draw_mask(Object* self, Gfx** gdl, Mtx** mtxs, Vtx_t** vtxs, Triangle** pols) {
    Vtx_t* initVtx;
    DIMTent_Data* objData;
    s32 index;
    Vtx_t* vtx;
    f32 maskY;
    f32 maskProgress;
    SRT srt;
    u32 i;
    s32 opacity;

    objData = self->data;

    maskProgress = maskY = objData->maskY;

    initVtx = *vtxs;
    vtx = *vtxs;

    if (vtx && 1) { } //fake?

    maskProgress /= MASK_MAX; //tValue for the mask animation (0 at top, 1 at bottom)

    //Set up mesh masking draw configs
    dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, 0x80);
    gSPLoadGeometryMode(*gdl, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_SHADING_SMOOTH);
    dl_apply_geometry_mode(gdl);
    gDPSetCombineLERP(*gdl, TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED);
    dl_apply_combine(gdl);

    //Position the cube's vertices (forming a section of a pyramid)
    for (i = 0; i < ARRAYCOUNT(sMaskVertCoords); i++) {
        if (i < 4) {
            vtx->ob[1] = MASK_MAX * SCALE_Y;
        } else {
            //Move the base of the cube with maskY
            vtx->ob[1] = (MASK_MAX - maskY) * SCALE_Y;
        }

        if (i < 4) {
            vtx->ob[0] = sMaskVertCoords[i].x * SCALE_X;
            vtx->ob[2] = sMaskVertCoords[i].z * SCALE_Z;
        } else {
            //Expand the bottom of the cube outwards as the mask moves down
            vtx->ob[0] = sMaskVertCoords[i].x * maskProgress * SCALE_X;
            vtx->ob[2] = sMaskVertCoords[i].z * maskProgress * SCALE_Z;
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
    dl_apply_other_mode(gdl);

    srt.transl.x = self->srt.transl.x;
    srt.transl.y = self->srt.transl.y;
    srt.transl.z = self->srt.transl.z;
    srt.yaw = self->srt.yaw;
    srt.pitch = 0;
    srt.roll = 0;
    srt.scale = 0.05f;
    camera_setup_object_srt_matrix(gdl, mtxs, &srt, 1, 0, NULL);

    gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(initVtx), 8, 0);
    dl_triangles(gdl, (DLTri*)sMaskTris, ARRAYCOUNT(sMaskTris));

    //Set transform for particles
    srt.transl.x = (30.0f * maskProgress) + 2.0f;
    srt.transl.y = (MASK_MAX - maskY) - 4.0f;
    if ((i = 0)) { } //fake
    srt.transl.z = (30.0f * maskProgress) + 2.0f;
    srt.yaw = self->srt.yaw;
    srt.pitch = 0;
    srt.roll = 0;
    srt.scale = 0.001f;

    //Create fire particles until nearly fully burnt (particle count scales with maskY)
    if (objData->maskY < THRESHOLD_END_FADEOUT) {
        for (index = 0; index < (s32)(objData->maskY / 10.0f); index++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_203, &srt, 4, -1, NULL);
        }
    }

    //Randomly create snow evaporation particles approaching the end of the burning
    if (objData->maskY > THRESHOLD_SNOW_EVAPORATE) {
        if (rand_next(0, 2) == 0) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_206, &srt, 4, -1, NULL);
        }
    //Otherwise create fire/smoke/ember particles
    } else {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_203, &srt, 4, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_203, &srt, 4, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_205, &srt, 4, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_204, &srt, 4, -1, NULL);
    }

    //Fade out the tent when nearly fully burnt
    /*@bug: this won't run when the tent is off-screen since this function is called
            by print instead of control, which can cause the burn anim to ping-pong) */
    if (objData->maskY >= THRESHOLD_END_FADEOUT) {
        opacity = self->opacity - (gUpdateRate * 4);
        if (opacity < 0) {
            opacity = 0;
        }
        self->opacity = opacity;
    }

    *vtxs = vtx;
}
