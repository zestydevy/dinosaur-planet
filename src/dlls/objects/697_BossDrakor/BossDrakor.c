#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/33_BaddieControl.h"
#include "dlls/objects/214_animobj.h"
#include "dlls/objects/698_BossDrakor_Laser.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objanim.h"
#include "sys/objhits.h"
#include "sys/objtype.h"
#include "functions.h"
#include "dll.h"

enum DrakorLogicStates {
    DRAKOR_LSTATE_0 = 0, // flying (upper area with space background)
    DRAKOR_LSTATE_1 = 1  // on ground, racing towards the crystal 
};

enum DrakorAnimStates {
    DRAKOR_ASTATE_0 = 0,
    DRAKOR_ASTATE_1 = 1
};

typedef struct {
/*0*/ s16 unk0;
/*2*/ s16 unk2; // rate of yaw spin in logic state 0
/*4*/ u16 unk4; // dist to target?
/*6*/ u16 animIndex; // not a mod anim index, index into below anim related statics
/*8*/ Object *laser;
} BossDrakor_ActualData;

typedef struct {
/*000*/ Baddie baddie;
/*3FC*/ BossDrakor_ActualData actualData;
} DLL697_Data;

// anim state 1 model animation indices
/*0x0*/ static s16 sAnimState1ModAnims[] = {
    0x0007, 0x0007, 0x0008, 0x0007, 0x0008, 0x0007, 0x0008, 0x0007, 
    0x0008, 0x0007, 0x0008, 0x0007, 0x0008
};
// anim state 1 animTickDelta values
/*0x1C*/ static f32 sAnimState1AnimTickDeltas[] = {
    0.008f, 0.008f, 0.008f, 0.008f, 0.008f, 0.008f, 0.008f, 0.008f, 
    0.008f, 0.008f, 0.008f, 0.008f, 0.008f
};
// anim state 0 model animation indices
/*0x50*/ static s16 sAnimState0ModAnims[] = {
    0x0001, 0x0001, 0x0006, 0x0001, 0x0001, 0x0006, 0x0001, 0x0006
};
// anim state 0 animTickDelta values
/*0x60*/ static f32 sAnimState0AnimTickDeltas[] = {
    0.008f, 0.009f, 0.002f, 0.01f, 0.009f, 0.002f, 0.008f, 0.002f
};

/*0x0*/ static ObjFSA_StateCallback sAnimStateCallbacks[2];
/*0x8*/ static ObjFSA_StateCallback sLogicStateCallbacks[2];

static void BossDrakor_create_laser(Object *self, BossDrakor_ActualData *objdata);
static int BossDrakor_anim_callback(Object *self, Object *arg1, AnimObj_Data *arg2, s8 arg3);
static void BossDrakor_func_55C(Object *self, Baddie *baddie, ObjFSA_Data *fsa);
static void BossDrakor_func_768(Object *self, Baddie *baddie, BossDrakor_ActualData *objdata, ObjFSA_Data *fsa);
static void BossDrakor_func_838(Object *self, Baddie *baddie, ObjFSA_Data *fsa);
static void BossDrakor_func_8E0(u8 arg0);
static s32 BossDrakor_anim_state_0(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 BossDrakor_anim_state_1(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 BossDrakor_logic_state_0(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 BossDrakor_logic_state_1(Object *self, ObjFSA_Data *fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void BossDrakor_init_fsa_callbacks(void) {
    sAnimStateCallbacks[0] = BossDrakor_anim_state_0;
    sAnimStateCallbacks[1] = BossDrakor_anim_state_1;
    sLogicStateCallbacks[0] = BossDrakor_logic_state_0;
    sLogicStateCallbacks[1] = BossDrakor_logic_state_1;
}

// offset: 0x54 | ctor
void BossDrakor_ctor(void *dll) {
    BossDrakor_init_fsa_callbacks();
}

// offset: 0x94 | dtor
void BossDrakor_dtor(void *dll) { }

// offset: 0xA0 | func: 1 | export: 0
void BossDrakor_setup(Object *self, Baddie_Setup *setup, s32 arg2) {
    Baddie *baddie;
    s32 var_v0;

    baddie = self->data;
    var_v0 = 6;
    if (arg2 != 0) {
        var_v0 = 7;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, setup, baddie, 2, 2, 0x108, (u8) var_v0, 20.0f);
    self->animCallback = BossDrakor_anim_callback;
    baddie->fsa.animState = DRAKOR_ASTATE_0;
    baddie->fsa.logicState = DRAKOR_LSTATE_0;
    if (self->shadow != NULL) {
        self->shadow->distFadeMaxOpacity = 0x64;
        self->shadow->distFadeMinOpacity = 0x96;
    }
}

// offset: 0x160 | func: 2 | export: 1
void BossDrakor_control(Object *self) {
    Baddie *baddie;
    BossDrakor_ActualData *objdata;

    baddie = self->data;
    if (self->unkDC == 0) {
        if (baddie->unk3B2 & 2) {
            baddie->unk3B2 &= ~2;
        }
        if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 1) != 0) {
            objdata = (BossDrakor_ActualData*)baddie->objdata;
            if (objdata->laser == NULL) {
                BossDrakor_create_laser(self, objdata);
            }
            BossDrakor_func_838(self, baddie, &baddie->fsa);
            BossDrakor_func_55C(self, baddie, &baddie->fsa);
        }
    }
}

// offset: 0x24C | func: 3 | export: 2
void BossDrakor_update(Object *self) { }

// offset: 0x258 | func: 4 | export: 3
void BossDrakor_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    Baddie *baddie;
    BossDrakor_ActualData *objdata;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    f32 sp34;
    f32 sp30;
    f32 sp2C;

    baddie = (Baddie*)self->data;
    if ((visibility != 0) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        objdata = (BossDrakor_ActualData*)baddie->objdata;
        if (objdata->laser != NULL) {
            func_80031F6C(self, 0, &sp4C, &sp48, &sp44, 0);
            func_80031F6C(self, 1, &sp40, &sp3C, &sp38, 0);
            sp34 = sp40 - sp4C;
            sp30 = sp3C - sp48;
            sp2C = sp38 - sp44;
            objdata->laser->srt.transl.x = sp40;
            objdata->laser->srt.transl.y = sp3C;
            objdata->laser->srt.transl.z = sp38;
            objdata->laser->srt.yaw = arctan2_f(sp34, sp2C);
            objdata->laser->srt.pitch = (s16) (arctan2_f(sqrtf(SQ(sp34) + SQ(sp2C)), sp30) - 0x4000);
            objdata->laser->srt.roll += 0x1000;
        }
    }
}

// offset: 0x3F0 | func: 5 | export: 4
void BossDrakor_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_4);
}

// offset: 0x430 | func: 6 | export: 5
u32 BossDrakor_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_8 | MODFLAGS_EVENTS;
}

// offset: 0x440 | func: 7 | export: 6
u32 BossDrakor_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL697_Data);
}

// offset: 0x454 | func: 8
static void BossDrakor_create_laser(Object *self, BossDrakor_ActualData *objdata) {
    ObjSetup *laserSetup;
    Baddie_Setup *drakorSetup;

    drakorSetup = (Baddie_Setup*)self->setup;
    laserSetup = obj_alloc_create_info(0x24, OBJ_BossDrakor_Lase);
    laserSetup->x = self->srt.transl.x;
    laserSetup->y = self->srt.transl.y;
    laserSetup->z = self->srt.transl.z;
    laserSetup->loadFlags = drakorSetup->base.loadFlags;
    laserSetup->byte5 = drakorSetup->base.byte5;
    laserSetup->byte6 = drakorSetup->base.byte6;
    laserSetup->fadeDistance = drakorSetup->base.fadeDistance;
    objdata->laser = obj_create(laserSetup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, -1, -1, NULL);
}

// offset: 0x50C | func: 9
static int BossDrakor_anim_callback(Object *self, Object *arg1, AnimObj_Data *arg2, s8 arg3) {
    Baddie *baddie;
    u8 i;
    u8 temp;

    baddie = self->data;
    baddie->unk3B2 |= 2;
    for (i = 0; i < arg2->unk98; i++) {
        temp = arg2->unk8E[i];
        // @fake: These case numbers are made up for the match. Check the relevant sequences 
        //        to see what commands would be sent to this DLL.
        switch (temp) {
            case 0:
                break;
            case 1:
                break;
            default:
                break;
        }
    }
    return baddie->unk3B4 != 0;
}

// offset: 0x55C | func: 10
static void BossDrakor_func_55C(Object *self, Baddie *arg1, ObjFSA_Data *fsa) {
    BossDrakor_ActualData *objdata = arg1->objdata;
    Baddie_Setup *setup = (Baddie_Setup*)self->setup;

    gDLL_33_BaddieControl->vtbl->func4(self, get_player(), 0x10, &objdata->unk0, &objdata->unk2, &objdata->unk4);
    fsa->targetDist = (f32) objdata->unk4;
    if (self->unkE0 == 0) {
        gDLL_3_Animation->vtbl->func17(setup->unk2E, self, -1);
        self->unkE0 = 1;
        return;
    }
    fsa->target = get_player();
    if (fsa->logicState == DRAKOR_LSTATE_1) {
        gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.17f, 1);
    } else {
        gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.0f, 1);
    }
    arg1->unk3AC = self->unkC0;
    self->unkC0 = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    self->unkC0 = arg1->unk3AC;
    BossDrakor_func_768(self, arg1, objdata, fsa);
}

// offset: 0x768 | func: 11
static void BossDrakor_func_768(Object *self, Baddie *arg1, BossDrakor_ActualData *objdata, ObjFSA_Data *fsa) {
    Object *laser;

    if (fsa->unk308 & 0x40) {
        if (fsa->animState == DRAKOR_ASTATE_0) {
            laser = objdata->laser;
            ((DLL_698_BossDrakor_Laser*)laser->dll)->vtbl->func7(laser, 0x78, 0xF0);
        } else if (fsa->animState == DRAKOR_ASTATE_1) {
            laser = objdata->laser;
            ((DLL_698_BossDrakor_Laser*)laser->dll)->vtbl->func7(laser, 0x78, 0xF0);
        }
        fsa->unk308 &= ~0x40;
    }
}

// offset: 0x838 | func: 12
static void BossDrakor_func_838(Object *self, Baddie *arg1, ObjFSA_Data *fsa) {
    BossDrakor_ActualData *objdata = arg1->objdata;
    Object *sp30;
    s32 _pad;
    s32 sp28;
    s32 sp24;

    if (func_80025F40(self, &sp30, &sp28, &sp24) != 0) {
        if ((sp30->id != OBJ_BossDrakorAster) && (sp30->id != OBJ_BossDrakorFlatR) && (fsa->logicState != DRAKOR_LSTATE_1)) {
            fsa->logicState = DRAKOR_LSTATE_1;
            objdata->animIndex = 0;
            BossDrakor_func_8E0(1);
        }
    }
}

// offset: 0x8E0 | func: 13
static void BossDrakor_func_8E0(u8 arg0) {
    Object **objList;
    s32 i;
    s32 numObjs;
    Object *obj;    

    objList = get_world_objects(&i, &numObjs);
    while (i < numObjs) {
        if (objList[i]->id == OBJ_BossDrakorLevel) {
            obj = objList[i];
            ((DLL_Unknown*)obj->dll)->vtbl->func[7].withTwoArgs((s32)obj, arg0);
            return;
        }
        i++;
    }
}

// offset: 0x99C | func: 14
static s32 BossDrakor_anim_state_0(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    Baddie *baddie = self->data;
    BossDrakor_ActualData *objdata = baddie->objdata;

    if (fsa->target != NULL) {
        func_80032C0C(self, fsa->target, &baddie->unk3BC, 0x19);
    }
    if ((fsa->enteredAnimState != 0) || (fsa->unk33A != 0)) {
        func_80023D30(self, (s32) sAnimState0ModAnims[objdata->animIndex], 0.0f, 0);
    }
    fsa->animTickDelta = sAnimState0AnimTickDeltas[objdata->animIndex];
    self->srt.yaw += objdata->unk2;
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    return 0;
}

// offset: 0xABC | func: 15
static s32 BossDrakor_anim_state_1(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    Baddie *baddie = self->data;
    BossDrakor_ActualData *objdata = baddie->objdata;

    if (fsa->target != NULL) {
        func_80032C0C(self, fsa->target, &baddie->unk3BC, 0x19);
    }
    if ((fsa->enteredAnimState != 0) || (fsa->unk33A != 0)) {
        func_80023D30(self, (s32) sAnimState1ModAnims[objdata->animIndex], 0.0f, 0);
    }
    fsa->animTickDelta = sAnimState1AnimTickDeltas[objdata->animIndex];
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    return 0;
}

// offset: 0xBC8 | func: 16
static s32 BossDrakor_logic_state_0(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    Baddie *baddie = (Baddie*)self->data;
    BossDrakor_ActualData *objdata = (BossDrakor_ActualData*)baddie->objdata;

    if (fsa->enteredLogicState != 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0);
    } else if (fsa->unk33A != 0) {
        objdata->animIndex += 1;
        if (objdata->animIndex >= 8) {
            objdata->animIndex = 0;
        }
    }
    return 0;
}

// offset: 0xC54 | func: 17
static s32 BossDrakor_logic_state_1(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    Baddie *baddie = (Baddie*)self->data;
    BossDrakor_ActualData *objdata = (BossDrakor_ActualData*)baddie->objdata;

    if (fsa->enteredLogicState != 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);

    } else if (fsa->unk33A != 0) {
        objdata->animIndex += 1;
        if (objdata->animIndex >= 13) {
            BossDrakor_func_8E0(0);
            objdata->animIndex = 0;
            return DRAKOR_LSTATE_0 + 1;
        }
    }

    return 0;
}
