#include "dlls/engine/6_amsfx.h"
#include "game/gamebits.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/objexpr.h"
#include "sys/objprint.h"
#include "sys/rand.h"
#include "dll.h"

typedef struct {
/*0*/ f32 modMinZ;
/*4*/ f32 modMaxZ;
/*8*/ u32 waterLoopHandle;
/*C*/ u8 state;
/*D*/ u8 unkD;
/*E*/ s16 unkE;
} DBwater_Data;

typedef struct {
/*00*/ f32 unk0;
/*04*/ f32 unk4;
/*08*/ f32 unk8;
/*0C*/ s16 unkC;
/*0E*/ s16 unkE;
/*10*/ f32 unk10;
/*14*/ s32 unk14;
/*18*/ u8 unk18;
} Data0;

// SRT?
typedef struct {
    u8 _unk0[0xC - 0x0];
    f32 unkC;
    f32 unk10;
    f32 unk14;
} Func924Struct;

enum DBwaterState {
    DB_WATER_STATE_0 = 0,
    DB_WATER_STATE_1 = 1,
    DB_WATER_STATE_2 = 2,
    DB_WATER_STATE_3 = 3,
    DB_WATER_STATE_4 = 4,
    DB_WATER_STATE_5 = 5
};

/*0x0*/ static Data0 data_0[] = {
    {20.0f, 20.0f, 10.0f, 0x03bd, 0x0000, -12.0f, 0x00000080, 0}, 
    {50.0f, 20.0f, 10.0f, 0x03c9, 0x0000,  95.0f, 0x00000028, 0}
};

/*0x0*/ static u32 data_38[] = {0xff0f002b, 0x79ff0000};

static void DBwater_animateTexture(Object* self, DBwater_Data* arg1, s32 arg2);
static void DBwater_animateVertices(Object* self, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, Func924Struct* arg8);

// offset: 0x0 | ctor
void DBwater_ctor(void* dll) { }

// offset: 0xC | dtor
void DBwater_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBwater_obj_Setup(Object* self, Generic_Setup* setup, s32 reset) {
    ModelInstance* modelInst;
    Model* model;
    s16 z;
    f32 zf;
    DBwater_Data* objdata = self->data;
    s32 i;
    f32 tmp;

    self->srt.yaw = setup->yaw << 8;
    self->srt.pitch = setup->genericB << 8;
    objdata->state = DB_WATER_STATE_0;
    if (mainGetBits(setup->bitA) != 0) {
        objdata->state = DB_WATER_STATE_5;
    }
    modelInst = self->modelInsts[self->modelInstIdx];
    model = modelInst->model;
    objdata->modMaxZ = -100000.0f;
    objdata->modMinZ = 100000.0f;
    for (i = 0; i < model->vertexCount; i++) {
        z = model->vertices[i].v.ob[2];
        zf = (f32) z;
        if (objdata->modMaxZ < zf) {
            objdata->modMaxZ = zf;
        }
        if (objdata->modMinZ > zf) {
            objdata->modMinZ = zf;
        }
    }
    if (setup->genericA == 0) {
        tmp = objdata->modMaxZ;
        objdata->modMaxZ = objdata->modMinZ;
        objdata->modMinZ = tmp;
    }
}

// offset: 0x15C | func: 1 | export: 1
void DBwater_obj_Control(Object* self) {
    Generic_Setup* setup;
    DBwater_Data* objdata;
    s32 sp64;
    f32 sp60;
    Func924Struct sp48;
    Data0* sp44;

    setup = (Generic_Setup*)self->setup;
    objdata = self->data;
    sp44 = &data_0[setup->genericA];
    if (objdata->unkE <= 0) {
        objdata->unkE = sp44->unk14 + gUpdateRate;
        if (objdata->state != DB_WATER_STATE_0) {
            if (objdata->state < DB_WATER_STATE_4) {
                objdata->state++;
                if ((objdata->state == DB_WATER_STATE_2) && (setup->genericA == 0)) {
                    mainSetBits(BIT_DB_Update_Flood_Level, 1);
                }
            }
        }
    }
    objdata->unkE -= gUpdateRate;
    if (objdata->unkE < 0) {
        objdata->unkE = 0;
    }
    DBwater_animateTexture(self, objdata, setup->genericA);
    sp64 = sp44->unk14;
    switch (objdata->state) {
    case DB_WATER_STATE_0:
        if (mainGetBits(setup->bitB) != 0) {
            objdata->state = DB_WATER_STATE_1;
            dll_amSfx->Play(self, SOUND_A45_DB_Water_Spray, MAX_VOLUME, NULL, NULL, 0, NULL);
            objdata->unkD = sp44->unk18;
            objdata->unkE = sp64;
            DBwater_animateVertices(self, 
                (f32) objdata->unkE / (f32) sp64, 
                objdata->modMinZ, objdata->modMaxZ, -1.0f, 
                sp44->unk0, sp44->unk4, sp44->unk8, &sp48);
        }
        break;
    case DB_WATER_STATE_1:
        if (objdata->waterLoopHandle == 0) {
            dll_amSfx->Play(self, SOUND_A44_DB_Water_Loop, MAX_VOLUME, &objdata->waterLoopHandle, NULL, 0, NULL);
        }
        sp60 = (f32) objdata->unkE / (f32) sp64;
        DBwater_animateVertices(self, sp60, 
            objdata->modMinZ, objdata->modMaxZ, -1.0f, 
            sp44->unk0, sp44->unk4, sp44->unk8, &sp48);
        if (setup->genericA != 0) {
            objdata->unkE += (s32) (SQ(sp60) * gUpdateRateF * 0.6f);
        }
        break;
    case DB_WATER_STATE_2:
        mainSetBits(setup->bitA, 1);
        sp60 = (f32) objdata->unkE / (f32) sp64;
        DBwater_animateVertices(self, sp60, 
            objdata->modMinZ, objdata->modMaxZ, 0.0f, 
            sp44->unk0 * ((sp60 * 0.5f) + 0.5f), sp44->unk4, sp44->unk8, &sp48);
        if (objdata->unkE == 0) {
            if (objdata->unkD-- == 0) {
                objdata->unkE = sp64;
            }
        }
        break;
    case DB_WATER_STATE_3:
        DBwater_animateVertices(self, 
            (f32) objdata->unkE / (f32) sp64, 
            objdata->modMinZ, objdata->modMaxZ, 1.0f, 
            sp44->unk0 * 0.2f, sp44->unk4, sp44->unk8, &sp48);
        if (objdata->waterLoopHandle != 0) {
            dll_amSfx->SetVol(objdata->waterLoopHandle, ((f32) objdata->unkE * 127.0f) / (f32) sp64);
        }
        break;
    case DB_WATER_STATE_4:
        if (objdata->waterLoopHandle != 0) {
            dll_amSfx->Stop(objdata->waterLoopHandle);
            objdata->waterLoopHandle = 0;
        }
        if (setup->genericA == 0) {
            objdata->state = DB_WATER_STATE_5;
        } else {
            mainSetBits(setup->bitB, 0);
            mainSetBits(setup->bitA, 0);
            objdata->state = DB_WATER_STATE_0;
        }
        break;
    case DB_WATER_STATE_5:
        self->opacity = 0;
        break;
    }
}

// offset: 0x744 | func: 2 | export: 2
void DBwater_obj_Update(Object* self) { }

// offset: 0x750 | func: 3 | export: 3
void DBwater_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vtx** vtxs, DLTri** pols, s8 visibility) {
    DBwater_Data* objdata = self->data;

    if (visibility != 0) {
        if ((objdata->state != 0) && (objdata->state < 4)) {
            objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0x7B8 | func: 4 | export: 4
void DBwater_obj_Free(Object* self, s32 onlySelf) {
    DBwater_Data* objdata = self->data;
    
    if (objdata->waterLoopHandle != 0) {
        dll_amSfx->Stop(objdata->waterLoopHandle);
        objdata->waterLoopHandle = 0;
    }
}

// offset: 0x820 | func: 5 | export: 5
u32 DBwater_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x830 | func: 6 | export: 6
u32 DBwater_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DBwater_Data);
}

// offset: 0x844 | func: 7
static void DBwater_animateTexture(Object* self, DBwater_Data* objdata, s32 arg2) {
    TextureAnimator* texAnim = objExprGetTexAnimator(self, 0, 0);
    
    if (texAnim != NULL) {
        if (arg2 == 0) {
            texAnim->positionV -= 0x40;
            if (texAnim->positionV < 0x800) {
                texAnim->positionV += 0x800;
            }
            texAnim->positionU -= 4;
            if (texAnim->positionU < 0x800) {
                texAnim->positionU += 0x800;
            }
        } else {
            texAnim->positionV += 0x40;
            if (texAnim->positionV >= 0x801) {
                texAnim->positionV -= 0x800;
            }
            texAnim->positionU += 4;
            if (texAnim->positionU >= 0x801) {
                texAnim->positionU -= 0x800;
            }
        }
    }
}

// offset: 0x924 | func: 8
static void DBwater_animateVertices(Object* self, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, Func924Struct* arg8) {
    f32 temp_fs1;
    f32 temp_fv1;
    f32 var_fs0;
    f32 var_fs4;
    f32 var_fv1;
    Vtx* vtxs;
    Model* model;
    ModelInstance* modelInst;
    f32 sp7C;
    s32 i;
    f32 sp74;
    s16 var_s5;
    s16 temp_s0;

    if (arg1 > 1.0f) {
        sp74 = 1.0f;
    } else {
        sp74 = arg1;
    }
    sp7C = 50.0f;
    modelInst = self->modelInsts[0];
    var_s5 = 0;
    model = modelInst->model;
    vtxs = modelInst->vertices[((modelInst->unk34 >> 1) & 1) ^ 1];
    if (arg4 == -1.0f) {
        var_fs4 = arg1;
    } else {
        var_fs4 = (f32) mathRnd(0, 0x14) * 0.01f;
    }
    temp_fv1 = arg2 - arg3;
    for (i = 0; i < model->vertexCount; i++) {
        temp_fs1 = ((f32) (model->vertices[i].v.ob[2]) - arg3) / temp_fv1;
        var_fs0 = (arg1 - temp_fs1) * 5.0f * arg4;
        if (var_fs0 > 1.0f) {
            var_fs0 = 1.0f;
        }
        if (var_fs0 < 0.0f) {
            var_fs0 = 0.0f;
        }
        if ((var_fs0 * 512.0f) < 255.0f) {
            var_fv1 = var_fs0 * 512.0f;
        } else {
            var_fv1 = 255.0f;
        }
        if (arg4 == 0) {
            var_fv1 = 255.0f;
            var_fs0 = 1.0f;
        }
        if (temp_fs1 == 0.0f) {
            vtxs[i].v.cn[3] = 0;
        } else {
            vtxs[i].v.cn[3] = (u8) var_fv1;
        }
        temp_s0 = (u32) ((temp_fs1 + sp74) * 65535.0f);
        vtxs[i].v.ob[1] = ((f32) (model->vertices[i].v.ob[1]) + (var_fs0 * arg5)) + (mathCosfInterp(temp_s0 * 2) * arg7);
        vtxs[i].v.ob[0] = (f32) (model->vertices[i].v.ob[0]) + (mathSinfInterp(temp_s0) * arg6);
        if (var_fs4 <= temp_fs1) {
            var_fs0 = temp_fs1 - var_fs4;
        } else {
            var_fs0 = -(temp_fs1 - var_fs4);
        }
        if (var_fs0 < sp7C) {
            sp7C = var_fs0;
            var_s5 = i;
        }
        if ((model->vertices[i].v.cn[3]) == 0) {
            vtxs[i].v.cn[3] = 0;
        }
    }
    if (arg8 != NULL) {
        temp_fs1 = (((f32) model->vertices[var_s5].v.ob[2] - arg3) / temp_fv1);
        arg8->unk14 = ((temp_fv1 * temp_fs1) + arg3);
        arg8->unk14 = ((temp_fv1 * temp_fs1) + arg3) * self->srt.scale;
        arg8->unkC = vtxs[var_s5].v.ob[0] * self->srt.scale;
        arg8->unk10 = vtxs[var_s5].v.ob[1] * self->srt.scale;
    }
}
