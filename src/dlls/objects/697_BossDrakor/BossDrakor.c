#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/33.h"
#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/gfx/model.h"
#include "sys/objects.h"
#include "sys/objanim.h"
#include "sys/objhits.h"
#include "sys/objtype.h"
#include "functions.h"
#include "dll.h"

typedef struct {
/*0*/ u16 unk0;
/*2*/ s16 unk2;
/*4*/ u16 unk4;
/*6*/ u16 unk6;
/*8*/ Object *laser;
} BossDrakor_ActualData;

typedef struct {
/*000*/ DLL33_Data unk0;
/*3FC*/ BossDrakor_ActualData actualData;
} DLL697_Data;

/*0x0*/ static s16 _data_0[] = {
    0x0007, 0x0007, 0x0008, 0x0007, 0x0008, 0x0007, 0x0008, 0x0007, 
    0x0008, 0x0007, 0x0008, 0x0007, 0x0008, 0x0000
};
/*0x1C*/ static f32 _data_1C[] = {
    0.008f, 0.008f, 0.008f, 0.008f, 0.008f, 0.008f, 0.008f, 0.008f, 
    0.008f, 0.008f, 0.008f, 0.008f, 0.008f
};
/*0x50*/ static s16 _data_50[] = {
    0x0001, 0x0001, 0x0006, 0x0001, 0x0001, 0x0006, 0x0001, 0x0006
};
/*0x60*/ static f32 _data_60[] = {
    0.008f, 0.009f, 0.002f, 0.01f, 0.009f, 0.002f, 0.008f, 0.002f
};

/*0x0*/ static ObjFSA_StateCallback sAnimStateCallbacks[2];
/*0x8*/ static ObjFSA_StateCallback sLogicStateCallbacks[2];

static void dll_697_func_454(Object *self, BossDrakor_ActualData *objdata);
static int dll_697_func_50C(Object *self, Object *arg1, AnimObj_Data *arg2, s8 arg3);
static void dll_697_func_55C(Object *self, DLL33_Data *arg1, ObjFSA_Data *fsa);
static void dll_697_func_768(Object *self, DLL33_Data *arg1, BossDrakor_ActualData *objdata, ObjFSA_Data *fsa);
static void dll_697_func_838(Object *self, DLL33_Data *arg1, ObjFSA_Data *fsa);
static void dll_697_func_8E0(u8 arg0);
static s32 dll_697_func_99C(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 dll_697_func_ABC(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 dll_697_func_BC8(Object *self, ObjFSA_Data *fsa, f32 updateRate);
static s32 dll_697_func_C54(Object *self, ObjFSA_Data *fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void dll_697_func_0(void) {
    sAnimStateCallbacks[0] = dll_697_func_99C;
    sAnimStateCallbacks[1] = dll_697_func_ABC;
    sLogicStateCallbacks[0] = dll_697_func_BC8;
    sLogicStateCallbacks[1] = dll_697_func_C54;
}

// offset: 0x54 | ctor
void dll_697_ctor(void *dll) {
    dll_697_func_0();
}

// offset: 0x94 | dtor
void dll_697_dtor(void *dll) { }

// offset: 0xA0 | func: 1 | export: 0
void dll_697_setup(Object *self, DLL33_ObjSetup *setup, s32 arg2) {
    DLL33_Data *sp34;
    s32 var_v0;

    sp34 = self->data;
    var_v0 = 6;
    if (arg2 != 0) {
        var_v0 = 7;
    }
    gDLL_33->vtbl->func21(self, setup, sp34, 2, 2, 0x108, (u8) var_v0, 20.0f);
    self->animCallback = dll_697_func_50C;
    sp34->fsa.animState = 0;
    sp34->fsa.logicState = 0;
    if (self->unk64 != NULL) {
        self->unk64->unk3A = 0x64;
        self->unk64->unk3B = 0x96;
    }
}

// offset: 0x160 | func: 2 | export: 1
void dll_697_control(Object *self) {
    DLL33_Data *dll33Data;
    BossDrakor_ActualData *objdata;

    dll33Data = self->data;
    if (self->unkDC == 0) {
        if (dll33Data->unk3B2 & 2) {
            dll33Data->unk3B2 &= ~2;
        }
        if (gDLL_33->vtbl->func11(self, dll33Data, 1) != 0) {
            objdata = (BossDrakor_ActualData*)dll33Data->unk3F4;
            if (objdata->laser == NULL) {
                dll_697_func_454(self, objdata);
            }
            dll_697_func_838(self, dll33Data, &dll33Data->fsa);
            dll_697_func_55C(self, dll33Data, &dll33Data->fsa);
        }
    }
}

// offset: 0x24C | func: 3 | export: 2
void dll_697_update(Object *self) { }

// offset: 0x258 | func: 4 | export: 3
void dll_697_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    DLL33_Data *dll33Data;
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

    dll33Data = (DLL33_Data*)self->data;
    if ((visibility != 0) && (self->unkDC == 0)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        objdata = (BossDrakor_ActualData*)dll33Data->unk3F4;
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
void dll_697_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_4);
}

// offset: 0x430 | func: 6 | export: 5
u32 dll_697_get_model_flags(Object *self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW | MODFLAGS_8 | MODFLAGS_EVENTS;
}

// offset: 0x440 | func: 7 | export: 6
u32 dll_697_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL697_Data);
}

// offset: 0x454 | func: 8
static void dll_697_func_454(Object *self, BossDrakor_ActualData *objdata) {
    ObjSetup *laserSetup;
    DLL33_ObjSetup *drakorSetup;

    drakorSetup = (DLL33_ObjSetup*)self->setup;
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
static int dll_697_func_50C(Object *self, Object *arg1, AnimObj_Data *arg2, s8 arg3) {
    DLL33_Data *temp_v1;
    u8 i;
    u8 temp;

    temp_v1 = self->data;
    temp_v1->unk3B2 |= 2;
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
    return temp_v1->unk3B4 != 0;
}

// offset: 0x55C | func: 10
static void dll_697_func_55C(Object *self, DLL33_Data *arg1, ObjFSA_Data *fsa) {
    BossDrakor_ActualData *objdata = arg1->unk3F4;
    DLL33_ObjSetup *setup = (DLL33_ObjSetup*)self->setup;

    gDLL_33->vtbl->func4(self, get_player(), 0x10, &objdata->unk0, &objdata->unk2, &objdata->unk4);
    fsa->targetDist = (f32) objdata->unk4;
    if (self->unkE0 == 0) {
        gDLL_3_Animation->vtbl->func17(setup->unk2E, self, -1);
        self->unkE0 = 1;
        return;
    }
    fsa->target = get_player();
    if (fsa->logicState == 1) {
        gDLL_33->vtbl->func10(self, fsa, 0.17f, 1);
    } else {
        gDLL_33->vtbl->func10(self, fsa, 0.0f, 1);
    }
    arg1->unk3AC = self->unkC0;
    self->unkC0 = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    self->unkC0 = arg1->unk3AC;
    dll_697_func_768(self, arg1, objdata, fsa);
}

// offset: 0x768 | func: 11
static void dll_697_func_768(Object *self, DLL33_Data *arg1, BossDrakor_ActualData *objdata, ObjFSA_Data *fsa) {
    Object *laser;

    if (fsa->unk308 & 0x40) {
        if (fsa->animState == 0) {
            laser = objdata->laser;
            ((DLL_Unknown*)laser->dll)->vtbl->func[7].withThreeArgs((s32)laser, 0x78, 0xF0);
        } else if (fsa->animState == 1) {
            laser = objdata->laser;
            ((DLL_Unknown*)laser->dll)->vtbl->func[7].withThreeArgs((s32)laser, 0x78, 0xF0);
        }
        fsa->unk308 &= ~0x40;
    }
}

// offset: 0x838 | func: 12
static void dll_697_func_838(Object *self, DLL33_Data *arg1, ObjFSA_Data *fsa) {
    BossDrakor_ActualData *objdata = arg1->unk3F4;
    Object *sp30;
    s32 _pad;
    s32 sp28;
    s32 sp24;

    if (func_80025F40(self, &sp30, &sp28, &sp24) != 0) {
        if ((sp30->id != OBJ_BossDrakorAster) && (sp30->id != OBJ_BossDrakorFlatR) && (fsa->logicState != 1)) {
            fsa->logicState = 1;
            objdata->unk6 = 0;
            dll_697_func_8E0(1);
        }
    }
}

// offset: 0x8E0 | func: 13
static void dll_697_func_8E0(u8 arg0) {
    Object** objList;
    s32 i;
    s32 numObjs;
    Object* obj;    

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
static s32 dll_697_func_99C(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    DLL33_Data *dll33Data = self->data;
    BossDrakor_ActualData *objdata = dll33Data->unk3F4;

    if (fsa->target != NULL) {
        func_80032C0C(self, fsa->target, &dll33Data->unk3BC, 0x19);
    }
    if ((fsa->enteredAnimState != 0) || (fsa->unk33A != 0)) {
        func_80023D30(self, (s32) _data_50[objdata->unk6], 0.0f, 0);
    }
    fsa->animTickDelta = _data_60[objdata->unk6];
    self->srt.yaw += objdata->unk2;
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    return 0;
}

// offset: 0xABC | func: 15
static s32 dll_697_func_ABC(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    DLL33_Data *dll33Data = self->data;
    BossDrakor_ActualData *objdata = dll33Data->unk3F4;

    if (fsa->target != NULL) {
        func_80032C0C(self, fsa->target, &dll33Data->unk3BC, 0x19);
    }
    if ((fsa->enteredAnimState != 0) || (fsa->unk33A != 0)) {
        func_80023D30(self, (s32) _data_0[objdata->unk6], 0.0f, 0);
    }
    fsa->animTickDelta = _data_1C[objdata->unk6];
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    return 0;
}

// offset: 0xBC8 | func: 16
static s32 dll_697_func_BC8(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    DLL33_Data *dll33Data = (DLL33_Data*)self->data;
    BossDrakor_ActualData *objdata = (BossDrakor_ActualData*)dll33Data->unk3F4;

    if (fsa->enteredLogicState != 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0);
    } else if (fsa->unk33A != 0) {
        objdata->unk6 += 1;
        if (objdata->unk6 >= 8) {
            objdata->unk6 = 0;
        }
    }
    return 0;
}

// offset: 0xC54 | func: 17
static s32 dll_697_func_C54(Object *self, ObjFSA_Data *fsa, f32 updateRate) {
    DLL33_Data *dll33Data = (DLL33_Data*)self->data;
    BossDrakor_ActualData *objdata = (BossDrakor_ActualData*)dll33Data->unk3F4;

    if (fsa->enteredLogicState != 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);

    } else if (fsa->unk33A != 0) {
        objdata->unk6 += 1;
        if (objdata->unk6 >= 13) {
            dll_697_func_8E0(0);
            objdata->unk6 = 0;
            return 1;
        }
    }

    return 0;
}
