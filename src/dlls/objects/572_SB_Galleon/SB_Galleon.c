#include "PR/ultratypes.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "game/objects/object_id.h"
#include "functions.h"
#include "dll.h"
#include "sys/main.h"
#include "sys/map_enums.h"
#include "sys/dll.h"
#include "dlls/modgfx/139.h"
#include "dlls/objects/573_SB_Cloudrunner.h"
#include "sys/joypad.h"
#include "sys/rand.h"
#include "game/gamebits.h"
#include "game/gametexts.h"

#define HINT_COUNTER_MAX 200
// 80 in map_enums.h is MOBILE_MAP_DIM2ICEFLOE2...
#define SOME_MAP_ID 80

typedef struct {
/*00*/ f32 x1;
/*04*/ f32 y1;
/*08*/ f32 z1;
/*0C*/ f32 cloudrunnerZ;
/*10*/ s8 _unk10[0x1C - 0x10];
/*1C*/ f32 unk1C;
/*20*/ s16 unk20;
/*22*/ s16 unk22;
/*24*/ s16 unk24;
/*26*/ s16 unk26;
/*28*/ s8 unk28;
/*29*/ s8 unk29;
/*2A*/ s8 unk2A;
/*2B*/ s8 unk2B;
/*2C*/ f32 x2;
/*30*/ f32 y2;
/*34*/ f32 z2;
/*38*/ f32 unk38;
/*3C*/ f32 unk3C;
/*40*/ f32 unk40;
/*44*/ f32 unk44;
/*48*/ Object *cloudrunner;
/*4C*/ Object *shiphead;
/*50*/ void *dll;
/*54*/ f32 x3;
/*58*/ f32 y3;
/*5C*/ f32 z3;
/*60*/ f32 x4;
/*64*/ f32 y4;
/*68*/ f32 z4;
/*6C*/ u32 soundHandle;
/*70*/ u32 soundHandle2;
/*74*/ s16 unk74;
/*76*/ s16 unk76;
/*78*/ s16 unk78;
/*7A*/ s16 unk7A;
/*7C*/ s8 state;
/*7D*/ s8 unk7D;
/*7E*/ s16 mapID;
/*80*/ u8  unk80;
/*82*/ s16 unk82;
/*84*/ s8 unk84;
/*85*/ s8 unk85;
/*86*/ s8 unk86;
/*88*/ s16 _unk88;
/*8A*/ u8 unk8A;
/*8B*/ u8 unk8B;
/*8C*/ s16 hintCounter;
/*8E*/ s8 unk8E;
} SB_Galleon_Data;

typedef struct {
/*00*/ Texture *tex1;
/*04*/ Texture *tex2;
/*08*/ s8 fadeoutStarted;
} Data0;

/*0x0*/ static Data0 _data_0 = { NULL, NULL, FALSE };

enum SB_Galleon_State {
    STATE_0,
    STATE_1,
    STATE_2,
    STATE_3,
    STATE_4
};

static int SB_Galleon_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);
static void SB_Galleon_func_B5C(Object *self, Object *override, AnimObj_Data *arg2);

// offset: 0x0 | ctor
void SB_Galleon_ctor(void *dll) { }

// offset: 0xC | dtor
void SB_Galleon_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SB_Galleon_setup(Object *self, ObjSetup *setup, s32 arg2) {
    SB_Galleon_Data *objdata;

    objdata = self->data;
    obj_add_object_type(self, OBJTYPE_4);
    obj_set_update_priority(self, 90);
    self->animCallback = SB_Galleon_anim_callback;
    objdata->x2 = self->srt.transl.x;
    objdata->y2 = self->srt.transl.y;
    objdata->z2 = self->srt.transl.z;
    objdata->unk2A = 1;
    objdata->unk26 = 240;
    objdata->unk7A = 240;
    objdata->unk80 = 0;
    objdata->hintCounter = HINT_COUNTER_MAX;
    objdata->mapID = self->mapID;
    self->srt.yaw = M_90_DEGREES;
    self->srt.pitch = 0;
    self->srt.roll = 0;
    _data_0.tex1 = queue_load_texture_proxy(365);
    _data_0.tex2 = queue_load_texture_proxy(137);
    if (0) { }
    objdata->soundHandle2 = 0;
    objdata->unk8E = 100;
    gDLL_29_Gplay->vtbl->set_map_setup(self->mapID, 1);
    func_80000450(self, self, 88, 0, 0, 0);
}

#ifndef NON_MATCHING
// offset: 0x16C | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/572_SB_Galleon/SB_Galleon_control.s")
#else
// matches but needs other functions
void SB_Galleon_func_B88(Object *self);
void SB_Galleon_func_EAC(Object *self);
void SB_Galleon_control(Object *self) {
    SB_Galleon_Data *objdata = self->data;

    self->mapID = objdata->mapID;
    if (!main_get_bits(BIT_SB_Battle_Started)) {
        if (!gDLL_29_Gplay->vtbl->get_obj_group_status(self->unk34, 1)) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, 1, 1);
        }
        if (!gDLL_29_Gplay->vtbl->get_obj_group_status(self->unk34, 3)) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->unk34, 3, 1);
        }
        self->unkDC = 0;
        return;
    }
    if ((objdata->unk8A == 0) && (objdata->state > STATE_0)) {
        objdata->unk8A = 1;
    }
    switch (objdata->state) {
    case STATE_0:
        SB_Galleon_func_EAC(self);
        break;
    case STATE_1:
        gDLL_28_ScreenFade->vtbl->fade_reversed(10, SCREEN_FADE_BLACK);
        gDLL_3_Animation->vtbl->func17(3, self, -1);
        objdata->state = STATE_2;
        break;
    case STATE_2:
        SB_Galleon_func_B88(self);
        break;
    case STATE_3:
        gDLL_29_Gplay->vtbl->set_map_setup(MAP_WARLOCK_MOUNTAIN, 1);
        self->mapID = -1;
        gDLL_28_ScreenFade->vtbl->fade_reversed(80, SCREEN_FADE_BLACK);
        gDLL_3_Animation->vtbl->func17(2, self, -1);
        if (gDLL_29_Gplay->vtbl->get_obj_group_status(SOME_MAP_ID, 5)) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(SOME_MAP_ID, 5, 0);
        }
        objdata->state = STATE_4;
        break;
    }
}

#endif

// offset: 0x42C | func: 2 | export: 2
void SB_Galleon_update(Object *self) { }

// offset: 0x438 | func: 3 | export: 3
void SB_Galleon_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x48C | func: 4 | export: 4
void SB_Galleon_free(Object *self, s32 a1) {
    SB_Galleon_Data *objdata;

    objdata = self->data;
    if (_data_0.tex1) {
        texture_destroy(_data_0.tex1);
        _data_0.tex1 = NULL;
    }
    if (_data_0.tex2) {
        texture_destroy(_data_0.tex2);
        _data_0.tex2 = NULL;
    }
    if (objdata->soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
    }
    if (objdata->soundHandle2) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle2);
    }
    if (_data_0.tex1) { }
    obj_free_object_type(self, OBJTYPE_4);
    if ((objdata->unk8A != 0) && (a1 == 0)) {
        objdata->unk8A = 0;
    }
}

// offset: 0x5A4 | func: 5 | export: 5
u32 SB_Galleon_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x5B4 | func: 6 | export: 6
u32 SB_Galleon_get_data_size(Object *self, u32 a1) {
    return sizeof(SB_Galleon_Data);
}

// offset: 0x5C8 | func: 7 | export: 7
s32 SB_Galleon_func_5C8(Object *self) {
    SB_Galleon_Data *objdata;

    objdata = self->data;
    if (objdata->unk29 != 1) {
        if (objdata->unk29 >= 2) {
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_10A, 97, NULL, NULL, 0, NULL);
        }
        objdata->unk2B++;
        return 1;
    }
    if ((objdata->unk84 == 0) || (objdata->unk84 == 1)) {
        objdata->unk86++;
        return 1;
    }
    return 0;
}

// offset: 0x68C | func: 8 | export: 8
s8 SB_Galleon_func_68C(Object *self) {
    SB_Galleon_Data *objdata;

    objdata = self->data;
    return objdata->unk2B;
}

// offset: 0x69C | func: 9 | export: 9
s8 SB_Galleon_func_69C(Object *self) {
    SB_Galleon_Data *objdata;

    objdata = self->data;
    if ((objdata->unk29 == 0) && (objdata->unk26 > 0)) {
        return -2;
    }
    if (objdata->unk29 == 1 && (objdata->unk84 == 2 || objdata->unk84 == 3 || objdata->unk84 == 5)) {
        return -1;
    }
    return objdata->unk29;
}

// offset: 0x700 | func: 10 | export: 10
u8 SB_Galleon_func_700(Object *self) {
    SB_Galleon_Data *objdata;

    objdata = self->data;
    return objdata->unk80;
}

#ifndef NON_MATCHING
// offset: 0x710 | func: 11 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/572_SB_Galleon/SB_Galleon_func_710.s")
#else
s32 SB_Galleon_func_710(Object *self) {
    SB_Galleon_Data *objdata;

    objdata = self->data;
    if (objdata->unk29 == 1) {
        if (objdata->unk86 >= 5)
            objdata->unk86 -= 5;

        return 540 + -90 * objdata->unk86;
    }

    return 1600;
}
#endif

// offset: 0x770 | func: 12
int SB_Galleon_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    s32 i;
    Object **objects;
    SB_Galleon_Data *objdata;
    DLL_139 *tempDll;
    s32 objIndex;
    s32 objCount;

    objdata = self->data;
    self->mapID = -1;
    objdata->unk44 = 0.0f;
    objdata->unk38 = 0.0f;
    objdata->unk3C = 0.0f;
    objdata->unk40 = 0.0f;
    animObjData->unkF4 = SB_Galleon_func_B5C;
    for (i = 0; i < animObjData->unk98; i++) {
        switch (animObjData->unk8E[i]) {
        case 1:
            objdata->unk7D = 1-objdata->unk7D;
            break;
        case 2:
            if (objdata->unk80 == 1)
                objdata->unk80 = 0;
            else
                objdata->unk80 = 1;
            break;
        case 3:
            objdata->dll = tempDll = dll_load_deferred(DLL_ID_139, 1);
            objects = get_world_objects(&objIndex, &objCount);
            for (i = objIndex; i < objCount; i++) {
                if (objects[i]->id == OBJ_SB_ShipHead) {
                    objdata->shiphead = objects[i];
                    i = objCount;
                }
            }
            if (objdata->shiphead) {
                tempDll->vtbl->base.func0(objdata->shiphead, 1, 0, 0x10000 | 0x2, -1, NULL);
            }
            objdata->unk82 = 0;
            break;
        case 4:
            dll_unload(objdata->dll);
            objdata->dll = NULL;
            break;
        case 5:
            if (objdata->unk80 == 2)
                objdata->unk80 = 0;
            else
                objdata->unk80 = 2;
            break;
        case 6:
            if (objdata->soundHandle == 0)
                objdata->soundHandle = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_17B, MAX_VOLUME, NULL, NULL, 0, NULL);
            break;
            // @fake
            if (&self) {}
        case 7:
            if (objdata->soundHandle) {
                gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
                objdata->soundHandle = 0;
            }
            break;
        case 8:
            if (objdata->unk80 == 8) {
                objdata->unk80 = 1;
            } else {
                objdata->unk80 = 8;
            }
            break;
        }
    }
    if (objdata->unk7D == 0) {
        self->unkDC = 7;
    } else {
        self->unkDC = 8;
    }
    if (self->unkDC == 8) {
        gDLL_12_Minic->vtbl->func9(-25.0f, 0.0f);
        gDLL_12_Minic->vtbl->func7(0);
    }
    animObjData->unk7A = -1;
    animObjData->unk62 = 0;
    objdata->unk82 -= gUpdateRate;
    if (objdata->unk82 <= 0 && objdata->dll) {
        objdata->unk82 = 50;
        if (objdata->shiphead) {
            tempDll = (DLL_139*)objdata->dll;
            tempDll->vtbl->base.func0(objdata->shiphead, 0, 0, 0x10000 | 0x2, -1, NULL);
        }
    }
    return 0;
}

// offset: 0xB5C | func: 13
void SB_Galleon_func_B5C(Object *self, Object *override, AnimObj_Data *arg2) {
    SB_Galleon_Data *objdata;

    objdata = self->data;
    objdata->x2 = self->srt.transl.x;
    objdata->y2 = self->srt.transl.y;
    objdata->z2 = self->srt.transl.z;
}

// offset: 0xB88 | func: 14
void SB_Galleon_func_B88(Object *self) {
    s32 _pad[2];
    f32 roll;
    SB_Galleon_Data *objdata;
    Object *player;
    s32 new_var;

    self->unkDC = 7;
    objdata = self->data;
    if (main_get_bits(BIT_SB_Scales_Intro) && (!main_get_bits(BIT_SB_Found_Kyte))) {
        _data_0.fadeoutStarted = TRUE;
        main_set_bits(BIT_SB_Found_Kyte, 1);
        gDLL_28_ScreenFade->vtbl->fade(10, SCREEN_FADE_BLACK);
    }
    if (objdata->unk8E != 0) {
        player = get_player();
        func_80000860(player, get_player(), 148, 0);
        objdata->unk8E = 0;
    }
    if ((_data_0.fadeoutStarted) && gDLL_28_ScreenFade->vtbl->is_complete()) {
        gDLL_28_ScreenFade->vtbl->fade_reversed(80, SCREEN_FADE_BLACK);
        gDLL_3_Animation->vtbl->func17(1, self, -1);
        objdata->state = STATE_3;
        _data_0.fadeoutStarted = FALSE;
    }
    gDLL_12_Minic->vtbl->func9(-25.0f, 0.0f);
    gDLL_12_Minic->vtbl->func7(0);
    roll = fsin16_precise(objdata->unk74);
    if (objdata->unk8B == 0) {
        if (roll < -0.9f) {
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_27A, MAX_VOLUME, NULL, NULL, 0, NULL);
            objdata->unk8B = 1;
        } else if (roll > 0.9f) {
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_27B, MAX_VOLUME, NULL, NULL, 0, NULL);
            objdata->unk8B = 1;
        }
    } else if ((roll > -0.1f) && (roll < 0.1f)) {
        objdata->unk8B = 0;
    }
    self->srt.roll = (s16) ((s32) (roll * 432.0f));
    new_var = ((u16)objdata->unk74) + (gUpdateRate << 7);
    objdata->unk74 = (s16) new_var;
}

#ifndef NON_MATCHING
// offset: 0xEAC | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/572_SB_Galleon/SB_Galleon_func_EAC.s")
#else
// https://decomp.me/scratch/o3oXg
// WIP
void SB_Galleon_func_EAC(Object *self) {
    f32 spF4;
    f32 spF0;
    f32 spEC;
    f32 spE8;
    f32 dx4;
    f32 dy4;
    f32 dz4;
    f32 spD8;
    f32 spD4;
    f32 spD0;
    f32 spCC;
    f32 spC8;
    s32 objIndex;
    s32 objCount;
    s32 spB0;
    ObjSetup *setup;
    SRT transform;
    f32 sp84;
    MtxF mtx;
    Object **objects;
    Object *obj1;
    SB_Galleon_Data *objdata;
    f32 dy2;
    f32 temp_fa1_7;
    f32 temp_ft1;
    f32 temp_ft1_2;
    f32 temp_fv0;
    f32 mag6;
    f32 dz1;
    f32 dz2;
    f32 mag2;
    f32 temp_fv0_8;
    f32 temp_fv0_9;
    f32 temp_fv1;
    f32 dx2;
    f32 temp_fv1_7;
    f32 dx1;
    f32 var_fa0;
    f32 var_fa0_2;
    f32 var_fa1;
    f32 var_ft2;
    f32 var_ft5;
    s16 temp_a0_4;
    s16 temp_a1_3;
    s16 temp_t4;
    s32 temp_v0_14;
    s16 temp_v1_3;
    s16 var_a0_2;
    s16 var_v1;
    s32 temp_t3_3;
    s32 temp_v1_2;
    s32 i1;
    s32 var_a0_3;
    s32 var_a0_4;
    s32 var_v0_2;
    s8 temp_a1_2;
    s8 temp_v0_10;
    Object *cloudrunner;

    f32 dx7;
    f32 dy7;
    f32 dz7;

    setup = self->setup;
    objdata = self->data;
    sp84 = 120.0f;
    self->mapID = -1;
    if ((objdata->cloudrunner != NULL) && (objdata->cloudrunner->srt.flags & 0x40)) {
        objdata->cloudrunner = NULL;
        objdata->cloudrunner = NULL;
    }
    if (objdata->cloudrunner == NULL) {
        objects = get_world_objects(&objIndex, &objCount);
        for (i1 = objIndex; i1 < objCount; i1++) {
            if (objects[i1]->id == OBJ_SB_Cloudrunner) {
                objdata->cloudrunner = objects[i1];
                i1 = objCount;
            }
        }
    }
    if (objdata->unk29 >= 2) {
        if (objdata->soundHandle == 0) {
            objdata->soundHandle = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_17B, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    } else {
        if (objdata->soundHandle != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
            objdata->soundHandle = 0;
        }
    }
    cloudrunner = objdata->cloudrunner;
    if (objdata->cloudrunner != NULL) {
        if ((cloudrunner != NULL) && (cloudrunner->unkDC == 0)) {
            ((DLL_573_SB_Cloudrunner*)cloudrunner->dll)->vtbl->func7(cloudrunner, &objdata->x3, &objdata->y3, &objdata->z3);
        }
        objdata->unk26 -= gUpdateRate;
        if (objdata->unk26 < 0) {
            objdata->unk26 = 0;
        }
        if (objdata->unk2B == 7) {
            objdata->unk80 = 3;
        } else if (objdata->unk2B == 8) {
            objdata->unk80 = 4;
        } else if (objdata->unk2B == 9) {
            objdata->unk80 = 5;
        }
        switch (objdata->unk29) {
        case 0:                                     /* switch 1 */
            sp84 = 120.0f;
            if (objdata->soundHandle2 != 0) {
                gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle2);
                objdata->soundHandle2 = 0;
            }
            gDLL_2_Camera->vtbl->func22(&sp84, 0);
            self->unkDC = 1;
            dx4 = objdata->x3 - 1600.0f;

            dz4 = (fcos16_precise(objdata->unk20) * 150.0f) + objdata->z3;
            temp_ft1 = fsin16_precise(objdata->unk20) * 60.0f;

            objdata->unk20 = (s16) (objdata->unk20 + (gUpdateRate * 0xB6));
            var_fa1 = (dx4 - self->srt.transl.x) * 0.03125f;

            objdata->unk1C = 3.0f;

            var_fa0 = ((temp_ft1 + (objdata->y3 - 300.0f)) - self->srt.transl.y) * 0.03125f;
            var_ft5 = (dz4 - self->srt.transl.z) * 0.03125f;

            if (var_fa1 > 3.0f) {
                var_fa1 = objdata->unk1C;
            }
            temp_fv0 = objdata->unk1C;
            temp_fv1 = -temp_fv0;
            if (var_fa1 < temp_fv1) {
                var_fa1 = temp_fv1;
            }
            if (temp_fv0 < var_fa0) {
                var_fa0 = temp_fv0;
            }
            if (var_fa0 < temp_fv1) {
                var_fa0 = temp_fv1;
            }
            if (temp_fv0 < var_ft5) {
                var_ft5 = temp_fv0;
            }
            if (var_ft5 < temp_fv1) {
                var_ft5 = temp_fv1;
            }
            // temp_v0_3 = objdata->unk7A;
            if (objdata->unk7A < 120) {
                var_fa0_2 = 0.0f;
                spF4 = var_fa1;
                spEC = var_ft5;
                // goto block_45;
                spF0 = var_fa0_2;
            } else {
            spF0 = var_fa0;
            spF4 = var_fa1;
            spEC = var_ft5;
            if (objdata->unk7A < 180) {
                spF4 = var_fa1;
                spEC = var_ft5;
                temp_ft1_2 = (f32) (objdata->unk7A - 120) / 60.0f;
                var_fa0_2 = var_fa0 * temp_ft1_2;
                spD4 = temp_ft1_2;
// block_45:
                spF0 = var_fa0_2;
            }
            }

            // spF0 = var_fa0_2;


            // temp_fv1_2 = objdata->x1;
            objdata->unk7A += gUpdateRate;
            // temp_fa0 = objdata->y1;
            // temp_fa1 = objdata->z1;

            //dx7 = (spF4 - objdata->x1);
            //dy7 = (spF0 - objdata->y1);
            //dz7 = (spEC - objdata->z1);

            objdata->x1 = objdata->x1 + ((spF4 - objdata->x1) * 0.0625f);
            objdata->y1 = objdata->y1 + ((spF0 - objdata->y1) * 0.0625f);
            objdata->z1 = objdata->z1 + ((spEC - objdata->z1) * 0.0625f);
            spD8 = 637.0f;
            spD4 = 0.01f;
            spD0 = 10.0f;
            if (joy_get_pressed(0) & U_JPAD) {
                objdata->unk2B++;
            }
            if (objdata->unk28 == 0) {
                if (objdata->unk2B != 0) {
                    if (objdata->unk2B != 1) {
                        objdata->hintCounter = HINT_COUNTER_MAX;
                        objdata->unk2B = 2;
                        objdata->unk7A = 0;
                        objdata->unk29 = 1;
                        objdata->unk28 = 1;
                        objdata->unk86 = 0;
                        objdata->unk84 = 0;
                    }
                } else {
                    if (objdata->hintCounter != 0) {
                        objdata->hintCounter--;
                        if (objdata->hintCounter <= 0) {
                            gDLL_22_Subtitles->vtbl->func_368(GAMETEXT_007_SB_CloudRunner_battle_tips_1);
                            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_25F_SB_CloudRunner_battle_tips_1, MAX_VOLUME, NULL, NULL, 0, NULL);
                            objdata->hintCounter = HINT_COUNTER_MAX;
                        }
                    }
                }
            } else {
                if (objdata->unk2B != 3) {
                    if (objdata->unk2B != 4) {
                        objdata->hintCounter = HINT_COUNTER_MAX;
                        objdata->unk28 = 2;
                        objdata->unk29 = 1;
                        objdata->unk2B = 5;
                        objdata->unk7A = 0;
                        objdata->unk84 = 0;
                    }
                } else {
                    if (objdata->hintCounter != 0) {
                        objdata->hintCounter--;
                        if (objdata->hintCounter <= 0) {
                            gDLL_22_Subtitles->vtbl->func_368(GAMETEXT_007_SB_CloudRunner_battle_tips_1);
                            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_25F_SB_CloudRunner_battle_tips_1, MAX_VOLUME, NULL, NULL, 0, NULL);
                            objdata->hintCounter = HINT_COUNTER_MAX;
                        }
                    }
                }
            }
            break;
        case 1:
            self->unkDC = 2;
            sp84 = 120.0f;
            gDLL_2_Camera->vtbl->func22(&sp84, 0);
            if (objdata->hintCounter != 0) {
                objdata->hintCounter--;
            }
            switch (objdata->unk84) {
            case 0:
                dx4 = objdata->x3 - 1700.0f;
                dz4 = objdata->z3;
                dy4 = cloudrunner->srt.transl.y + 260.0f;
                if ((objdata->hintCounter <= 0) && ((temp_v0_10 = objdata->unk86, (temp_v0_10 == 0)) || (temp_v0_10 == 5))) {
                    gDLL_22_Subtitles->vtbl->func_368(GAMETEXT_065_SB_CloudRunner_battle_tips_2);
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_260_SB_CloudRunner_battle_tips_2, MAX_VOLUME, NULL, NULL, 0, NULL);
                    objdata->hintCounter = HINT_COUNTER_MAX;
                }
                if (objdata->soundHandle2 == 0) {
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_140, 80, &objdata->soundHandle2, NULL, 0, NULL);
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_277, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
                break;
            case 1:
                dx4 = objdata->x3 - 800.0f;
                dz4 = objdata->z3;
                dy4 = cloudrunner->srt.transl.y + 260.0f;
                break;
            case 2:
                dx4 = cloudrunner->srt.transl.x - 535.0f;
                dz4 = objdata->z3 - 100.0f;
                dy4 = cloudrunner->srt.transl.y + 250.0f;
                objdata->unk85 = 0;
                break;
            case 3:
                dx4 = cloudrunner->srt.transl.x - 535.0f;
                dz4 = objdata->z3 + 220.0f;
                dy4 = cloudrunner->srt.transl.y + 250.0f;
                dz4 += cloudrunner->srt.transl.z - objdata->z2;
                objdata->unk85 = 0;
                break;
            case 4:
                dx4 = cloudrunner->srt.transl.x - 535.0f;
                dz4 = objdata->z3 + 100.0f;
                dy4 = cloudrunner->srt.transl.y + 250.0f;
                objdata->unk85 = 0;
                break;
            case 5:
                dx4 = cloudrunner->srt.transl.x - 535.0f;
                dz4 = objdata->z3 - 220.0f;
                dy4 = cloudrunner->srt.transl.y + 250.0f;
                dz4 += cloudrunner->srt.transl.z - objdata->z2;
                objdata->unk85 = 0;
                break;
            default:
                objdata->unk85 = 0;
                dx4 = objdata->x3 - 880.0f;
                dz4 = objdata->z3;
                dy4 = cloudrunner->srt.transl.y + 260.0f;
                break;
            }
            dx2 = dx4 - self->srt.transl.x;
            dy2 = dy4 - self->srt.transl.y;
            dz2 = dz4 - self->srt.transl.z;
            objdata->unk1C = 3.0f;
            spF0 = dy2;
            spF4 = dx2;
            spEC = dz2;
            // spA8 = var_t0_2;
            mag2 = sqrtf(SQ(dx2) + SQ(dy2) + SQ(dz2));
            spF4 = spF4 * 0.03125f;
            spEC = spEC * 0.03125f;
            // objdata = var_t0_2;
            // spF4 = temp_ft2;
            spF0 *= 0.03125f;
            if (spF4 > 4.0f) {
                spF4 = 4.0f;
            }
            if (spF4 < -4.0f) {
                spF4 = -4.0f;
            }
            if (spF0 > 1.5f) {
                spF0 = 1.5f;
            }
            if (spF0 < -1.5f) {
                spF0 = -1.5f;
            }
            // spEC = temp_fa1_5;
            if (spEC > 3.5f) {
                spEC = 3.5f;
            }
            // var_ft5_2 = spEC;
            if (spEC < -3.5f) {
                spEC = -3.5f;
            }
            // temp_fv1_6 = objdata->x1;
            objdata->unk7A = (s16) (objdata->unk7A + gUpdateRate);
            // temp_fa0_4 = objdata->y1;
            // temp_fa1_6 = objdata->z1;
            objdata->x1 = objdata->x1 + ((spF4 - objdata->x1) * 0.125f);
            objdata->y1 = objdata->y1 + ((spF0 - objdata->y1) / 14.0f);
            objdata->z1 = objdata->z1 + ((spEC - objdata->z1) / 24.0f);
            spD0 = 0.0f;
            spD8 = 637.0f;
            spD4 = 0.005f;
            // temp_t3 = objdata->unk84;
            switch (objdata->unk84) {
            case 0:
                if (mag2 < 15.0f) {
                    objdata->unk84 = 1;
                    objdata->unk7A = 0;
                }
                break;
            case 1:
                if (mag2 < 10.0f) {
                    objdata->unk84 = 2;
                    temp_fv0_8 = cloudrunner->srt.transl.z - objdata->z2;
                    if (!(temp_fv0_8 > 0.0f)) {
                        if (temp_fv0_8 < 0.0f) {
                            objdata->unk84 = 4;
                        } else {
                            // spA8 = objdata;
                            objdata->unk84 = (s8) (rand_next(/*temp_fa0_4, temp_fa1_6,*/ 1, 2) * 2);
                        }
                    }
                }
                break;
            case 2:
                if (mag2 < 10.0f) {
                    objdata->unk84 = 3;
                    objdata->unk7A = 3;
                }
                break;
            case 3:
                if (mag2 < 10.0f) {
                    objdata->unk84 = 0;
                    objdata->unk7A = 0;
                }
                break;
            case 4:
                if (mag2 < 10.0f) {
                    objdata->unk84 = 5;
                    objdata->unk7A = 3;
                }
                break;
            case 5:
                if (mag2 < 10.0f) {
                    objdata->unk84 = 0;
                    objdata->unk7A = 0;
                }
                break;
            default:
                if (mag2 < 25.0f) {
                    // temp_v0_11 = objdata->unk2B;
                    if (objdata->unk2B == 2) {
                        objdata->unk7A = 0;
                        objdata->unk29 = 0;
                        objdata->unk2B = 3;
                    } else if (objdata->unk2B == 5) {
                        objdata->unk29 = 2;
                        objdata->unk2B = 6;
                    }
                }
                break;
            }
            // temp_v0_12 = objdata->unk86;
            objdata->unk26 = 300;
            if ((objdata->unk86 >= 5) && (objdata->unk2B < 3)) {
                objdata->unk29 = 0;
                objdata->unk28 = 1;
                objdata->unk2B = 3;
                objdata->unk86 = 5;
                objdata->hintCounter = HINT_COUNTER_MAX;
            } else if (objdata->unk86 >= 5) {
                objdata->unk29 = 2;
                objdata->unk28 = 3;
                objdata->unk2B = 6;
                objdata->hintCounter = HINT_COUNTER_MAX;
                objdata->cloudrunnerZ = cloudrunner->srt.transl.z;
            }
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            sp84 = 120.0f;
            // temp_a0_3 = objdata->soundHandle2;
            if (objdata->soundHandle2 != 0) {
                // spA8 = objdata;
                gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle2);
                objdata->soundHandle2 = 0;
            }
            // spA8 = objdata;
            gDLL_2_Camera->vtbl->func22(&sp84, 0);
            self->unkDC = 3;
            // temp_v0_13 = objdata->unk8C;
            if (objdata->hintCounter != 0) {
                objdata->hintCounter--;
            }
            // temp_t3_2 = objdata->unk29;
            switch (objdata->unk29) {
            case 2:
                spC8 = 20.0f;
                dx4 = objdata->x3 - 4700.0f;
                dz4 = objdata->z3 - (1500.0f * (f32) objdata->unk2A);
                spB0 = 3;
                spCC = 1000.0f;
                dy4 = objdata->y3;
                break;
            case 3:
                spC8 = 12.0f;
                dx4 = objdata->x3 - 5000.0f;
                dz4 = objdata->z3 - (1000.0f * (f32) objdata->unk2A);
                spB0 = 4;
                var_ft2 = objdata->y3 + 100.0f;
                spCC = 700.0f;
// block_133:
                dy4 = var_ft2;
                break;
            case 4:
                spC8 = 12.0f;
                dx4 = objdata->x3 - 4700.0f;
                dz4 = objdata->z3 - (10.0f * (f32) objdata->unk2A);
                spB0 = 5;
                spCC = 700.0f;
                dy4 = objdata->y3 + 100.0f;
                break;
            case 5:
                spC8 = 10.0f;
                self->unkDC = 4;
                dx4 = objdata->x3 - 1100.0f;
                dz4 = objdata->z3;
                spB0 = 6;
                spCC = 700.0f;
                dy4 = objdata->y3 - 100.0f;
                if ((objdata->hintCounter <= 0) && (objdata->unk2B == 6)) {
                    // spA8 = objdata;
                    gDLL_22_Subtitles->vtbl->func_368(GAMETEXT_069_SB_CloudRunner_battle_tips_3);
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_261_SB_CloudRunner_battle_tips_3, MAX_VOLUME, NULL, NULL, 0, NULL);
                    objdata->hintCounter = HINT_COUNTER_MAX;
                }
                break;
            case 6:
                spC8 = 4.0f;
                dx4 = objdata->x3 + 200.0f;
                dz4 = objdata->z3 - (1500.0f * (f32) objdata->unk2A);
                spB0 = 7;
                spCC = 100.0f;
                dy4 = objdata->y3 + 250.0f;
                break;
            case 7:
                spC8 = 4.0f;
                dx4 = objdata->x3 + 1400.0f;
                dz4 = objdata->z3;
                spB0 = 8;
                spCC = 100.0f;
                dy4 = cloudrunner->srt.transl.y + 280.0f;
                break;
            case 8:
                spC8 = 8.0f;
                dx4 = objdata->x3 - 1200.0f;
                dz4 = objdata->z3;
                spB0 = 2;
                var_ft2 = cloudrunner->srt.transl.y + 100.0f;
                spCC = 200.0f;
                // goto block_133;
                dy4 = var_ft2;
            }
            temp_fa1_7 = dx4 - objdata->x2;
            spF0 = dy4 - objdata->y2;
            temp_fv0_9 = objdata->unk1C;
            temp_fv1_7 = dz4 - objdata->z2;
            objdata->unk1C = (f32) (temp_fv0_9 + ((spC8 - temp_fv0_9) / 30.0f));
            spF4 = temp_fa1_7;
            spEC = temp_fv1_7;
            // spA8 = objdata;
            mag6 = sqrtf(SQ(temp_fa1_7) + SQ(temp_fv1_7));
            spE8 = mag6;
            if ((objdata->unk29 == 5) && (mag6 < 3000.0f)) {
                self->unkDC = 5;
            }
            if (mag6 < spCC) {
                if (objdata->unk29 == 5) {
                    objdata->unk2A = -objdata->unk2A;
                }
                objdata->unk29 = (s8) spB0;
            }
            // spA8 = objdata;
            temp_v1_2 = arctan2_f(spF4, spEC) + M_180_DEGREES;
            // spB8 = temp_v1_2;
            temp_v0_14 = arctan2_f(spF0, spE8);
            var_a0_3 = temp_v1_2 - (self->srt.yaw & 0xFFFF);
            if (var_a0_3 > M_180_DEGREES) {
                var_a0_3 += -0xFFFF;
            }
            if (var_a0_3 < -M_180_DEGREES) {
                var_a0_3 += 0xFFFF;
            }
            temp_v1_3 = objdata->unk24;
            temp_a1_2 = objdata->unk29;
            objdata->unk24 = (s16) (temp_v1_3 + ((s32) ((var_a0_3 - temp_v1_3) * gUpdateRate) >> 4));
            if ((temp_a1_2 == 3) || (temp_a1_2 == 4)) {
                self->srt.yaw += (s32) (objdata->unk24 * gUpdateRate) / 60;
            } else if ((temp_a1_2 == 6) || (temp_a1_2 == 2)) {
                self->srt.yaw += (s32) (objdata->unk24 * gUpdateRate) / 120;
            } else {
                self->srt.yaw += (s32) (objdata->unk24 * gUpdateRate) / 60;
            }
            temp_a1_3 = self->srt.pitch;
            var_a0_4 = temp_v0_14 - ((u16)temp_a1_3);
            if (var_a0_4 > M_180_DEGREES) {
                var_a0_4 += -0xFFFF;
            }
            if (var_a0_4 < -M_180_DEGREES) {
                var_a0_4 += 0xFFFF;
            }
            self->srt.pitch = temp_a1_3 + ((s32) (var_a0_4 * gUpdateRate) >> 6);
            dx1 = objdata->x3 - self->srt.transl.x;
            // spA8 = objdata;
            dz1 = objdata->z3 - self->srt.transl.z;
            sqrtf(SQ(dx1) + SQ(dz1));
            temp_a0_4 = self->srt.roll;
            temp_t3_3 = (s32) ((s32) ((f32) objdata->unk24 * 0.45f) - temp_a0_4) >> 3;
            // var_v0_2 = temp_t3_3;
            if (temp_t3_3 > 0x3C) {
                temp_t3_3 = 0x3C;
            }
            if (temp_t3_3 < -0x3C) {
                temp_t3_3 = -0x3C;
            }
            self->srt.roll = (s16) (s32) ((f32) temp_a0_4 + ((f32) temp_t3_3 * gUpdateRateF));
            transform.transl.x = 0.0f;
            transform.transl.y = 0.0f;
            transform.transl.z = 0.0f;
            transform.scale = 1.0f;
            transform.yaw = self->srt.yaw;
            transform.pitch = self->srt.pitch;
            // spA8 = objdata;
            transform.roll = self->srt.roll;
            matrix_from_srt(&mtx, &transform);
            vec3_transform(&mtx, 0.0f, 0.0f, -objdata->unk1C * gUpdateRateF, &objdata->x1, &objdata->y1, &objdata->z1);
            // objdata = spA8;
            if (objdata->unk29 == 7) {
                objdata->x2 = dx4;
                objdata->y2 = dy4;
                objdata->z2 = dz4;
                objdata->unk38 = 0/*.0f*/;
                objdata->unk3C = 0/*.0f*/;
                objdata->unk40 = 0/*.0f*/;
            } else {
                objdata->x2 += objdata->x1;
                objdata->y2 += objdata->y1;
                objdata->z2 += objdata->z1;
            }
            spD4 = 0.17f;
            self->srt.transl.x = objdata->x2 + objdata->unk38;
            self->srt.transl.y = objdata->y2 + objdata->unk3C;
            self->srt.transl.z = objdata->z2 + objdata->unk40 + (cloudrunner->srt.transl.z - objdata->cloudrunnerZ);
            if (objdata->unk2B >= 7) {
                // var_v0_3 = objdata->unk78;
                if (objdata->unk78 == 0) {
                    gDLL_28_ScreenFade->vtbl->fade(65, SCREEN_FADE_BLACK);
                    // var_v0_3 = objdata->unk78;
                }
                objdata->unk78 += gUpdateRate;
                if (objdata->unk78 >= 66) {
                    self->srt.yaw = M_90_DEGREES;
                    objdata->unk29 = 6;
                    // spA8 = objdata;
                    gDLL_12_Minic->vtbl->func7(0);
                    gDLL_12_Minic->vtbl->func8(0);
                    gDLL_12_Minic->vtbl->func9(0.0f, 25.0f);
                    if (objdata->unk8A == 0) {
                        objdata->unk8A = 1;
                    }
                    objdata->state = STATE_1;
                    // spA8 = objdata;
                    gDLL_28_ScreenFade->vtbl->fade_reversed(25, SCREEN_FADE_BLACK);
                    self->srt.transl.x = setup->x;
                    self->srt.transl.y = setup->y;
                    self->srt.transl.z = setup->z;
                    if (objdata->soundHandle != 0) {
                        // spA8 = objdata;
                        gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
                        objdata->soundHandle = 0;
                    }
                    gDLL_29_Gplay->vtbl->set_obj_group_status(SOME_MAP_ID, 3, 0);
                    gDLL_29_Gplay->vtbl->set_obj_group_status(SOME_MAP_ID, 2, 1);
                    gDLL_29_Gplay->vtbl->set_obj_group_status(SOME_MAP_ID, 5, 1);
                    gDLL_3_Animation->vtbl->func17(0, self, -1);
                    gDLL_30_Task->vtbl->mark_task_completed(1);
                    return;
                }
            }
            break;
        default:
            self->unkDC = 7;
            break;
        }
        if (objdata->unk29 < 2) {
            objdata->x2 += ((objdata->x1 * gUpdateRateF * objdata->unk44));
            objdata->y2 += ((objdata->y1 * gUpdateRateF * objdata->unk44));
            objdata->z2 += ((objdata->z1 * gUpdateRateF * objdata->unk44));
            objdata->unk44 += 0.004166667f;
            if (objdata->unk44 > 1.0f) {
                objdata->unk44 = 1.0f;
            }
            objdata->x4 = (objdata->x4 + ((spD8 - objdata->x4) * gUpdateRateF * 0.0625f));
            objdata->y4 = (objdata->y4 + ((spD0 - objdata->y4) * gUpdateRateF * 0.0625f));
            objdata->z4 = (objdata->z4 + ((spD4 - objdata->z4) * gUpdateRateF * 0.0625f));
            if (objdata->unk29 == 0) {
                objdata->unk40 -= (((f32) cloudrunner->srt.roll / objdata->x4));
                objdata->unk3C -= (((f32) cloudrunner->srt.pitch / objdata->x4));
                objdata->unk40 -= ((objdata->unk40 * objdata->z4 * gUpdateRateF));
                objdata->unk3C -= ((objdata->unk3C * objdata->z4 * gUpdateRateF));
                var_v1 = (s16) (s32) (-objdata->unk40 * objdata->y4);
                var_a0_2 = (s16) (s32) (-objdata->unk3C * objdata->y4 * 0.5f);
            } else {
                var_v1 = 0;
                var_a0_2 = 0;
                objdata->unk40 -= ((objdata->unk40 * objdata->z4 * gUpdateRateF));
                objdata->unk3C -= ((objdata->unk3C * objdata->z4 * gUpdateRateF));
            }
            self->srt.transl.x = objdata->x2 + (objdata->unk38 * objdata->unk44);
            self->srt.transl.y = objdata->y2 + (objdata->unk3C * objdata->unk44);
            self->srt.transl.z = objdata->z2 + (objdata->unk40 * objdata->unk44);
            objdata->unk22 = (s16) (objdata->unk22 + ((s32) ((var_v1 - objdata->unk22) * gUpdateRate) >> 4));
            self->srt.pitch = self->srt.pitch + ((s32) ((var_a0_2 - self->srt.pitch) * gUpdateRate) >> 4);
            temp_t4 = objdata->unk22 + M_90_DEGREES;
            self->srt.yaw = temp_t4;
            self->srt.roll = temp_t4 - M_90_DEGREES;
        }
    }
}
#endif
