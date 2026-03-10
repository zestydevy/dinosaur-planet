#include "common.h"
#include "macros.h"

#define MASK_SPEED 0.13f

#define MASK_HEIGHT 200
#define MASK_WIDTH 150
#define MASK_DEPTH 150

#define SCALE_Y 20
#define SCALE_X 20.0f
#define SCALE_Z 20.0f

#define TRI_FLAGS (0x40 | 0x1)
#define MASK_TRI(v0, v1, v2) {TRI_FLAGS, v0, v1, v2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

/*0x0*/ static const DLTri sMaskTris[12] = {
    MASK_TRI(2, 0, 1),
    MASK_TRI(3, 2, 1),
    MASK_TRI(4, 6, 5),
    MASK_TRI(6, 7, 5),
    MASK_TRI(6, 2, 3),
    MASK_TRI(3, 7, 6),
    MASK_TRI(3, 1, 7),
    MASK_TRI(7, 1, 5),
    MASK_TRI(4, 1, 0),
    MASK_TRI(4, 5, 1),
    MASK_TRI(0, 2, 4),
    MASK_TRI(2, 6, 4)
};

/*0xC0*/ static const Vec3f sMaskVertCoords[8] = {
    {-MASK_WIDTH,   0,            -MASK_DEPTH},
    { MASK_WIDTH,   0,            -MASK_DEPTH},
    {-MASK_WIDTH,   0,             MASK_DEPTH},
    { MASK_WIDTH,   0,             MASK_DEPTH},
    {-MASK_WIDTH,   MASK_HEIGHT,  -MASK_DEPTH},
    { MASK_WIDTH,   MASK_HEIGHT,  -MASK_DEPTH},
    {-MASK_WIDTH,   MASK_HEIGHT,   MASK_DEPTH},
    { MASK_WIDTH,   MASK_HEIGHT,   MASK_DEPTH}
};

typedef struct {
    ObjSetup base;
} DLL618_Setup;

typedef struct {
    f32 maskY;
} WM_TransTop_Data;

static void WM_TransTop_draw_mask(Object* self, Gfx** gdl, Mtx** mtxs, Vtx_t** vtxs, Triangle** pols);

// offset: 0x0 | ctor
void WM_TransTop_ctor(void *dll) { }

// offset: 0xC | dtor
void WM_TransTop_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WM_TransTop_setup(Object* self, DLL618_Setup* objSetup, s32 arg2) {
    WM_TransTop_Data* objData = self->data;

    if (main_get_bits(BIT_WM_Sabre_Transporter_Visible)) {
        objData->maskY = MASK_HEIGHT;
    } else {
        objData->maskY = 4;
    }
}

// offset: 0x90 | func: 1 | export: 1
void WM_TransTop_control(Object *self) { }

// offset: 0x9C | func: 2 | export: 2
void WM_TransTop_update(Object *self) { }

// offset: 0xA8 | func: 3 | export: 3
void WM_TransTop_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** triangles, s8 visibility) {
    WM_TransTop_Data* objData = self->data;

    if (main_get_bits(BIT_WM_Sabre_Transporter_Visible)) {
        if (objData->maskY < MASK_HEIGHT) {
            WM_TransTop_draw_mask(self, gdl, mtxs, (Vtx_t**)vtxs, triangles);
        }
        if (visibility) {
            draw_object(self, gdl, mtxs, vtxs, triangles, 1.0f);
        }
    }
}

// offset: 0x184 | func: 4 | export: 4
void WM_TransTop_free(Object* self, s32 arg1) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x1CC | func: 5 | export: 5
u32 WM_TransTop_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1DC | func: 6 | export: 6
u32 WM_TransTop_get_data_size(Object *self, u32 a1) {
    return sizeof(WM_TransTop_Data);
}

// offset: 0x1F0 | func: 7
/**
  * Creates and animates an invisible cube that masks away the transporter's mesh.
  * Used for a special effect where the transporter gradually assembles itself, built from the ground up!
  */
void WM_TransTop_draw_mask(Object* self, Gfx** gdl, Mtx** mtxs, Vtx_t** vtxs, Triangle** pols) {
    Vtx_t* initVtx;
    WM_TransTop_Data* objData;
    Vtx_t* vtx;
    SRT srt;
    u32 i;

    objData = self->data;
    initVtx = *vtxs;
    vtx = *vtxs;

    //Set up mesh masking draw configs
    dl_set_prim_color(gdl, 0xFF, 0xFF, 0xFF, 0x80);
    gSPLoadGeometryMode(*gdl, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_SHADING_SMOOTH);
    dl_apply_geometry_mode(gdl);
    gDPSetCombineLERP(*gdl, TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED);
    dl_apply_combine(gdl);

    //Position the cube's vertices
    for (i = 0; i < ARRAYCOUNT(sMaskVertCoords); i++){
        if (i < 4) {
            //Move the base of the cube with maskY
            vtx->ob[1] = objData->maskY * SCALE_Y;
        } else {
            vtx->ob[1] = MASK_HEIGHT * SCALE_Y;
        }

        if (i < 4) {
            vtx->ob[0] = sMaskVertCoords[i].x * SCALE_X;
            vtx->ob[2] = sMaskVertCoords[i].z * SCALE_Z;
        } else {
            vtx->ob[0] = sMaskVertCoords[i].x * SCALE_X;
            vtx->ob[2] = sMaskVertCoords[i].z * SCALE_Z;
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
    camera_setup_object_srt_matrix(gdl, mtxs, &srt, 1.0f, 0.0f, NULL);

    gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(initVtx), 8, 0);
    dl_triangles(gdl, (DLTri*)sMaskTris, ARRAYCOUNT(sMaskTris));

    //Unmask the transporter by moving the base vertices of the cube upwards
    objData->maskY += MASK_SPEED * gUpdateRateF;
    if (objData->maskY < 0) {
        objData->maskY = 0;
    } else if (objData->maskY >= MASK_HEIGHT) {
        objData->maskY = MASK_HEIGHT;
    }

    *vtxs = vtx;
}
