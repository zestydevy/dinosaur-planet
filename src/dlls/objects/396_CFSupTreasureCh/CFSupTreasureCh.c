#include "dlls/objects/214_animobj.h"
#include "sys/objtype.h"
#include "dll.h"
#include "sys/main.h"
#include "functions.h"
#include "sys/objects.h"
#include "game/gamebits.h"
#include "dlls/objects/373_CFCloudBaby.h"
#include "macros.h"

typedef struct {
    s32 unk0;
    u8 _unk4[0x8-0x4];
    s16 unk8;
    s16 unkA;
} Struct_800348A0;

typedef struct {
/*00*/ f32 yMin;
/*04*/ s32 objectSeqIndex;
/*08*/ u8 flags;
/*0A*/ s16 yTheta;
} CFSupTreasureCh_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 hasBaby;
/*19*/ u8 unk19;
/*1A*/ u8 yaw;
} CFSupTreasureCh_Setup;

typedef enum {
    CFSUPTREASURECH_PLAY_SEQ = 1,
    CFSUPTREASURECH_FREED_BABY_1 = 2,
    CFSUPTREASURECH_FREED_BABY_2 = 4,
    CFSUPTREASURECH_8 = 8,
    CFSUPTREASURECH_16 = 16
} CFSupTreasureCh_Flags;

// /*0x0*/ static const char str_0[] = " Treasure Created  With the Baby";

static int CFSupTreasureCh_func_424(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);
static void CFSupTreasureCh_func_9B8(Object *arg0, Object *arg1, s16 *arg2);
static s32 CFSupTreasureCh_func_BD8(s32 arg0, SRT *arg1);
static f32 CFSupTreasureCh_func_C80(Object *arg0, s32 arg1);

// offset: 0x0 | ctor
void CFSupTreasureCh_ctor(void *dll) { }

// offset: 0xC | dtor
void CFSupTreasureCh_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFSupTreasureCh_setup(Object *self, CFSupTreasureCh_Setup *setup, s32 arg2) {
    CFSupTreasureCh_Data *objdata;

    objdata = self->data;
    obj_add_object_type(self, OBJTYPE_33);
    self->animCallback = CFSupTreasureCh_func_424;
    self->srt.yaw = setup->yaw << 8;
    if (setup->hasBaby) {
        if (main_get_bits(BIT_CF_Free_Cloudrunner_From_Chest)) {
            objdata->objectSeqIndex = -1;
        } else {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 4, 1);
            objdata->objectSeqIndex = 1;
            STUBBED_PRINTF(" Treasure Created  With the Baby");
        }
    } else {
        objdata->objectSeqIndex = 0;
    }
    if (!main_get_bits(BIT_Play_Seq_02C7_Scales_Takes_Baby_Cloudrunner_Away)) {
        objdata->flags |= CFSUPTREASURECH_PLAY_SEQ;
    }
}

/*0x24*/ static const char str_24[] = " NO TREASURE FAIL ";

// offset: 0x124 | func: 1 | export: 1
void CFSupTreasureCh_control(Object *self) {
    CFSupTreasureCh_Data *objdata;
    Object *cloudbaby;
    f32 distance;

    objdata = self->data;
    distance = 500.0f;
    if (objdata->flags & CFSUPTREASURECH_PLAY_SEQ) {
        if (objdata->objectSeqIndex != -1) {
            gDLL_3_Animation->vtbl->func17(objdata->objectSeqIndex, self, -1);
            objdata->flags &= ~CFSUPTREASURECH_PLAY_SEQ;
            return;
        }
    }
    if (objdata->objectSeqIndex == 1) {
        cloudbaby = obj_get_nearest_type_to(OBJTYPE_34, self, &distance);
        if (cloudbaby) {
            cloudbaby->srt.transl.x = self->srt.transl.x;
            cloudbaby->srt.transl.y = self->srt.transl.y + 2.0f;
            cloudbaby->srt.transl.z = self->srt.transl.z;
        }
    }
    if ((objdata->flags & CFSUPTREASURECH_FREED_BABY_1) && (objdata->flags & CFSUPTREASURECH_FREED_BABY_2)) {
        if (main_get_bits(BIT_Play_Seq_01CF_CF_Baby_Cloudrunner_Lands_On_Perch_Five)) {
            objdata->flags &= ~CFSUPTREASURECH_FREED_BABY_2;
        }
    }
    if (objdata->flags & CFSUPTREASURECH_16) {
        self->opacity = OBJECT_OPACITY_MAX;
        self->speed.y -= 0.03f;
        self->srt.transl.y += self->speed.y;
        if (self->srt.yaw != 0) {
            self->srt.yaw >>= 1;
        }
        if (self->srt.transl.y <= objdata->yMin) {
            self->srt.transl.y = objdata->yMin;
            objdata->yTheta = 0;
            objdata->flags &= ~CFSUPTREASURECH_16;
        }
        if (main_get_bits(BIT_Play_Seq_02C6_CF_Sharpclaw_Only_Four_Chests_Left)) {
            map_save_object(self->setup, self->mapID, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
        }
    }
}

// offset: 0x314 | func: 2 | export: 2
void CFSupTreasureCh_update(Object *self) { }

// offset: 0x320 | func: 3 | export: 3
void CFSupTreasureCh_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x374 | func: 4 | export: 4
void CFSupTreasureCh_free(Object *self, s32 a1) {
    CFSupTreasureCh_Setup *setup;

    setup = (CFSupTreasureCh_Setup*)self->setup;
    obj_free_object_type(self, OBJTYPE_33);
    STUBBED_PRINTF(" Treasure Freeded");
    if (setup->hasBaby) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, 4, 0);
    }
}

// /*0x38*/ static const char str_38[] = " Treasure Freeded";
// /*0x4C*/ static const char str_4C[] = " RET ";

// offset: 0x400 | func: 5 | export: 5
u32 CFSupTreasureCh_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x410 | func: 6 | export: 6
u32 CFSupTreasureCh_get_data_size(Object *self, u32 a1) {
    return sizeof(CFSupTreasureCh_Data);
}

// offset: 0x424 | func: 7
int CFSupTreasureCh_func_424(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    CFSupTreasureCh_Data *objdata;
    Object *cloudbaby;
    f32 distance;

    objdata = self->data;
    distance = 500.0f;
    if (main_get_bits(BIT_Play_Seq_02C7_Scales_Takes_Baby_Cloudrunner_Away)) {
        self->opacity = OBJECT_OPACITY_MAX;
        self->srt.scale = self->def->scale;
        return 0;
    }
    if (objdata->objectSeqIndex == 2) {
        STUBBED_PRINTF(" RET ");
        return 0;
    }
    animObjData->unk7A &= ~1;
    animObjData->unk7A &= ~2;
    if (objdata->objectSeqIndex == 1) {
        cloudbaby = obj_get_nearest_type_to(OBJTYPE_34, self, &distance);
        if (cloudbaby) {
            cloudbaby->unkAF |= 8;
            cloudbaby->srt.transl.x = self->srt.transl.x;
            cloudbaby->srt.transl.y = self->srt.transl.y;
            cloudbaby->srt.transl.z = self->srt.transl.z;
            cloudbaby->srt.transl.y += 2.0f;
        }
        self->unkAF &= ~8;
        self->unkAF &= ~0x10;
        if (self->unkAF & 1) {
            if (gDLL_1_UI->vtbl->func_DF4(BIT_CRF_Treasure_Chest_Key)) {
                main_set_bits(BIT_CF_Free_Cloudrunner_From_Chest, 1);
                main_set_bits(BIT_CRF_Treasure_Chest_Key, 0);
                if (cloudbaby) {
                    STUBBED_PRINTF(" FLY AWAY BADY ");
                    ((DLL_373_CFCloudBaby*)cloudbaby->dll)->vtbl->func8(cloudbaby);
                    objdata->flags |= CFSUPTREASURECH_FREED_BABY_1;
                    objdata->flags |= CFSUPTREASURECH_FREED_BABY_2;
                }
                objdata->objectSeqIndex = 2;
                self->unkAF |= 8;
                objdata->flags |= CFSUPTREASURECH_PLAY_SEQ;
                return 4;
            }
        }
    } else {
        self->unkAF |= 8;
        self->unkAF &= ~0x10;
    }
    if (objdata->objectSeqIndex == 1) {
        cloudbaby = obj_get_nearest_type_to(OBJTYPE_34, self, &distance);
        if (cloudbaby) {
            cloudbaby->srt.transl.x = self->srt.transl.x;
            cloudbaby->srt.transl.y = self->srt.transl.y + 2.0f;
            cloudbaby->srt.transl.z = self->srt.transl.z;
        }
    }
    if (objdata->flags & CFSUPTREASURECH_16) {
        return 4;
    }
    // @fake
    if (objdata) {}
    if (objdata->flags & (CFSUPTREASURECH_16 | CFSUPTREASURECH_8)) {
        map_save_object(self->setup, self->mapID, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z);
    }
    return 0;
}

// offset: 0x700 | func: 8 | export: 7
s32 CFSupTreasureCh_func_700(Object *self) {
    CFSupTreasureCh_Data *objdata;

    objdata = self->data;
    return (objdata->flags & CFSUPTREASURECH_FREED_BABY_2) == 0;
}

// offset: 0x718 | func: 9 | export: 8
void CFSupTreasureCh_func_718(Object *arg0, Object *arg1) {
    CFSupTreasureCh_Data *objdata;

    objdata = arg0->data;
    if (!(objdata->flags & CFSUPTREASURECH_8)) {
        func_80023A18(arg0, 1);
        func_800267A4(arg0);
    }
    objdata->flags |= (CFSUPTREASURECH_8 | CFSUPTREASURECH_FREED_BABY_2);
    CFSupTreasureCh_func_9B8(arg0, arg1, &objdata->yTheta);
}

// offset: 0x7B8 | func: 10 | export: 9
void CFSupTreasureCh_func_7B8(Object *self, s32 arg1, f32 arg2) {
    CFSupTreasureCh_Data *objdata;
    f32 float2;
    SRT transform;
    f32 float1;

    objdata = self->data;
    objdata->yMin = arg2;
    objdata->flags &= ~(CFSUPTREASURECH_8 | CFSUPTREASURECH_FREED_BABY_2);
    objdata->flags |= CFSUPTREASURECH_16;
    self->srt.scale = self->def->scale;
    float1 = CFSupTreasureCh_func_C80(self, 10);
    if (float1 < CFSupTreasureCh_func_C80(self, 12)) {
        float2 = CFSupTreasureCh_func_C80(self, 10);
    } else {
        float2 = CFSupTreasureCh_func_C80(self, 12);
    }
    if ((float2 != -1.0f) && (float2 < 100.0f)) {
        float1 = CFSupTreasureCh_func_C80(self, 11);
        if (float1 < CFSupTreasureCh_func_C80(self, 13)) {
            CFSupTreasureCh_func_BD8(11, &transform);
        } else {
            CFSupTreasureCh_func_BD8(13, &transform);
        }
        self->srt.transl.x = transform.transl.x;
        self->srt.transl.z = transform.transl.z;
        self->srt.yaw = transform.yaw;
    }
    func_8002674C(self);
    func_80023A18(self, 0);
    self->opacity = OBJECT_OPACITY_MAX;
    self->speed.y = 0.0f;
}

// offset: 0x9B8 | func: 11
void CFSupTreasureCh_func_9B8(Object *arg0, Object *arg1, s16 *arg2) {
    f32 x1;
    f32 z1;
    f32 y1;
    ObjDef *temp_v0;
    f32 s1;
    f32 var_fa0;
    Struct_800348A0 *unkstruct;

    s1 = arg0->def->scale * 0.6f;
    if (s1 != arg0->srt.scale) {
        arg0->srt.scale = (arg0->srt.scale + s1) * 0.5f;
        s1 = arg0->def->scale * 0.6f;
        if (s1 <= arg0->srt.scale) {
            var_fa0 = arg0->srt.scale - s1;
        } else {
            var_fa0 = -(arg0->srt.scale - s1);
            arg2 += 0; // ?
        }
        if (var_fa0 < 0.1f) {
            arg0->srt.scale = s1;
        }
    }
    unkstruct = func_800348A0(arg0, 0, 0);
    if (unkstruct) {
        unkstruct->unkA = ((unkstruct->unkA + 0x40)               % 2048);
        unkstruct->unk8 = ((unkstruct->unkA + 8)                  % 2048);
        unkstruct->unk0 = ((unkstruct->unk0 + (gUpdateRate << 6)) % 2048);
    }
    x1 = arg1->srt.transl.x;
    y1 = (fsin16_precise(*arg2) * 5.0f) + (arg1->srt.transl.y - 30.0f);
    z1 = arg1->srt.transl.z;
    arg0->srt.transl.x += (x1 - arg0->srt.transl.x) * 0.5f;
    arg0->srt.transl.y += (y1 - arg0->srt.transl.y) * 0.125f;
    arg0->srt.transl.z += (z1 - arg0->srt.transl.z) * 0.5f;
    arg0->unkAF |= 8;
    arg0->unkAF &= ~0x10;
    arg0->srt.yaw += 0x100;
    *arg2 += 0x500;
}

// offset: 0xBD8 | func: 12
s32 CFSupTreasureCh_func_BD8(s32 arg0, SRT *transform) {
    CurveSetup *curveSetup;
    s32 uID;

    uID = gDLL_26_Curves->vtbl->func_218C(arg0);
    if (uID >= 0) {
        curveSetup = gDLL_26_Curves->vtbl->func_39C(uID);
        transform->transl.x = curveSetup->pos.x;
        transform->transl.y = curveSetup->pos.y;
        transform->transl.z = curveSetup->pos.z;
        transform->yaw = curveSetup->unk2C << 8;
        return 1;
    }
    STUBBED_PRINTF(" Error Could not find node ");
    return 0;
}

// offset: 0xC80 | func: 13
f32 CFSupTreasureCh_func_C80(Object *self, s32 arg1) {
    s32 uID;

    uID = gDLL_26_Curves->vtbl->func_218C(arg1);
    if (uID >= 0) {
        return gDLL_26_Curves->vtbl->func_174C(self, uID);
    }
    STUBBED_PRINTF("CURVEPOINT :Error Could not find node ");
    return -1.0f;
}

// /*0x54*/ static const char str_54[] = " FLY AWAY BADY ";
// /*0x64*/ static const char str_64[] = " Error Could not find node ";
// /*0x80*/ static const char str_80[] = "CURVEPOINT :Error Could not find node ";
