#include "dlls/engine/17_partfx.h"
#include "dlls/engine/6_amsfx.h"
#include "game/objects/object.h"
#include "sys/gfx/animseq.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/objprint.h"
#include "dll.h"

typedef struct {
    f32 y;
    u8 state;
} WMGeneralScale_Data;

/*0x0*/ static u32 data_0[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};

static int WMGeneralScale_animCallback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8);
static void WMGeneralScale_printMask(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, Triangle** pols);

// offset: 0x0 | ctor
void WMGeneralScale_ctor(void* dll) { }

// offset: 0xC | dtor
void WMGeneralScale_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WMGeneralScale_obj_Setup(Object* self, ObjSetup* setup, s32 reset) {
    WMGeneralScale_Data* objdata = self->data;
    
    self->animCallback = WMGeneralScale_animCallback;
    objdata->state = 1;
    objdata->y = 0.0f;
}

// offset: 0x54 | func: 1 | export: 1
void WMGeneralScale_obj_Control(Object* self) { }

// offset: 0x60 | func: 2 | export: 2
void WMGeneralScale_obj_Update(Object* self) { }

// offset: 0x6C | func: 3 | export: 3
void WMGeneralScale_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, Triangle** pols, s8 visibility) {
    WMGeneralScale_Data* objdata = self->data;

    if (objdata->state != 1) {
        if ((objdata->state == 2) || (objdata->state == 3)) {
            WMGeneralScale_printMask(self, gdl, mtxs, vtxs, pols);
        }
        if (visibility != 0) {
            objprintDrawModel(self, gdl, mtxs, (Vertex**)vtxs, pols, 1.0f);
        }
    }
}

// offset: 0x124 | func: 4 | export: 4
void WMGeneralScale_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x134 | func: 5 | export: 5
u32 WMGeneralScale_obj_GetModelFlags(Object* self) {
    return MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x144 | func: 6 | export: 6
u32 WMGeneralScale_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WMGeneralScale_Data);
}

// offset: 0x158 | func: 7
static int WMGeneralScale_animCallback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    WMGeneralScale_Data* objdata = actor->data;
    s32 i;
    f32 sp64;
    
    for (i = 0; i < animObjData->messageCount; i++) {
        switch (animObjData->messages[i]) {
        case 0:
            break;
        case 1:
            objdata->state = 1;
            break;
        case 2:
            objdata->state = 2;
            dll_partfx->spawn(actor, PARTICLE_556, NULL, PARTFXFLAG_2, -1, &sp64);
            dll_amSfx->Play(actor, SOUND_B63, MAX_VOLUME, NULL, NULL, 0, NULL);
            dll_amSfx->Play(actor, SOUND_B64, MAX_VOLUME, NULL, NULL, 0, NULL);
            objdata->y = 0.0f;
            break;
        case 3:
            objdata->state = 3;
            dll_partfx->spawn(actor, PARTICLE_556, NULL, PARTFXFLAG_2, -1, NULL);
            dll_amSfx->Play(actor, SOUND_B63, MAX_VOLUME, NULL, NULL, 0, NULL);
            dll_amSfx->Play(actor, SOUND_B64, MAX_VOLUME, NULL, NULL, 0, NULL);
            objdata->y = 800.0f;
            break;
        case 4:
            objdata->state = 0;
            break;
        default:
            break;
        }
        animObjData->messages[i] = 0;
    }
    return 0;
}

/*0x0*/ static const DLTri rodata_0[] = {
    {0x41, 0x02, 0x00, 0x01, {0}}, 
    {0x41, 0x03, 0x02, 0x01, {0}}, 
    {0x41, 0x04, 0x06, 0x05, {0}}, 
    {0x41, 0x06, 0x07, 0x05, {0}}, 
    {0x41, 0x06, 0x02, 0x03, {0}}, 
    {0x41, 0x03, 0x07, 0x06, {0}}, 
    {0x41, 0x03, 0x01, 0x07, {0}}, 
    {0x41, 0x07, 0x01, 0x05, {0}}, 
    {0x41, 0x04, 0x01, 0x00, {0}}, 
    {0x41, 0x04, 0x05, 0x01, {0}}, 
    {0x41, 0x00, 0x02, 0x04, {0}}, 
    {0x41, 0x02, 0x06, 0x04, {0}}
};
/*0xC0*/ static const Vec3f rodata_C0[] = {
    VEC3F(-150.0f, 0.0f, -150.0f), 
    VEC3F(150.0f, 0.0f, -150.0f), 
    VEC3F(-150.0f, 0.0f, 150.0f), 
    VEC3F(150.0f, 0.0f, 150.0f),
    VEC3F(-150.0f, 200.0f, -150.0f), 
    VEC3F(150.0f, 200.0f, -150.0f), 
    VEC3F(-150.0f, 200.0f, 150.0f), 
    VEC3F(150.0f, 200.0f, 150.0f)
};

// offset: 0x3B4 | func: 8
static void WMGeneralScale_printMask(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, Triangle** pols) {
    Vtx* vtxStart;
    WMGeneralScale_Data* objdata;
    Vtx* vtx;
    SRT srt;
    s32 i;

    objdata = self->data;
    vtxStart = *vtxs;
    vtx = vtxStart;
    dlSetEnvColor(gdl, 255, 255, 255, 255);
    dlSetPrimColor(gdl, 255, 255, 255, 128);
    gSPLoadGeometryMode(*gdl, G_ZBUFFER | G_SHADE | G_FOG | G_SHADING_SMOOTH);
    dlApplyGeometryMode(gdl);
    gDPSetCombineLERP(*gdl, TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED);
    dlApplyCombine(gdl);
    for (i = 0; i < 8; i++) {
        if (i < 4) {
            vtx->v.ob[1] = (s16) objdata->y;
        } else {
            vtx->v.ob[1] = 800;
        }
        if (i < 4) {
            vtx->v.ob[0] = (s16) (rodata_C0[i].x * 3.0f);
            vtx->v.ob[2] = (s16) (rodata_C0[i].z * 3.0f);
        } else {
            vtx->v.ob[0] = (s16) (rodata_C0[i].x * 3.0f);
            vtx->v.ob[2] = (s16) (rodata_C0[i].z * 3.0f);
        }

        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0;
        vtx->v.cn[2] = 0;
        vtx->v.cn[3] = 0x40;
        vtx++;
    }
    gDPSetOtherMode(*gdl,
        G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE, 
        G_AC_NONE | G_ZS_PIXEL | Z_CMP | Z_UPD | IM_RD | CVG_DST_SAVE | ZMODE_XLU | FORCE_BL | G_RM_FOG_SHADE_A | GBL_c2(G_BL_CLR_IN, G_BL_0, G_BL_CLR_MEM, G_BL_1MA));
    dlApplyOtherMode(gdl);
    srt.transl.x = self->globalPosition.x;
    srt.transl.y = self->globalPosition.y + 30.0f;
    srt.transl.z = self->globalPosition.z;
    srt.yaw = self->srt.yaw;
    srt.pitch = 0;
    srt.roll = 0;
    srt.scale = 0.05f;
    camSetupObjectSRTMatrix(gdl, mtxs, &srt, 1.0f, 0.0f, NULL);
    gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(vtxStart), 8, 0);
    dlTriangles(gdl, rodata_0, ARRAYCOUNT(rodata_0));
    srt.transl.x = self->globalPosition.x;
    srt.transl.y = self->globalPosition.y + 30.0f;
    srt.transl.z = self->globalPosition.z;
    srt.yaw = self->srt.yaw;
    srt.pitch = 0x7FFF;
    srt.roll = 0;
    srt.scale = 0.05f;
    camSetupObjectSRTMatrix(gdl, mtxs, &srt, 1.0f, 0.0f, NULL);
    gSPVertex((*gdl)++, OS_PHYSICAL_TO_K0(vtxStart), 8, 0);
    dlTriangles(gdl, rodata_0, ARRAYCOUNT(rodata_0));
    srt.transl.x = 0.0f;
    srt.transl.y = (objdata->y * 0.05f) + 30.0f;
    srt.transl.z = 0.0f;
    srt.yaw = 0;
    srt.pitch = 0;
    srt.roll = 0;
    srt.scale = 1.0f;
    i = 8;
    while (i--) {
        dll_partfx->spawn(self, PARTICLE_55E, &srt, PARTFXFLAG_2, -1, NULL);
    }
    srt.transl.y = 30.0f - (objdata->y * 0.05f);
    i = 8;
    while (i--) {
        dll_partfx->spawn(self, PARTICLE_55E, &srt, PARTFXFLAG_2, -1, NULL);
    }
    if (objdata->state == 2) {
        objdata->y += (4.0f * gUpdateRateF);
    } else if (objdata->state == 3) {
        objdata->y -= (4.0f * gUpdateRateF);
    }
    if ((objdata->state == 2) && (objdata->y >= 800.0f)) {
        objdata->y = 800.0f;
        objdata->state = 0;
    } else if ((objdata->state == 3) && (objdata->y <= 0.0f)) {
        objdata->state = 1;
        objdata->y = 0.0f;
    }
    *vtxs = vtx;
}
