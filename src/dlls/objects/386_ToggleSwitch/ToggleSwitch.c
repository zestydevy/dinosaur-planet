#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "sys/gfx/animseq.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/objhits.h"
#include "sys/objprint.h"
#include "dll.h"

/** @file official filename: CFtoggleswitch.c (also used for Dragon Rock objects tho) */

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 unk18;
/*1A*/ s16 preemptTime;
/*1C*/ u8 _unk1C[0x1E - 0x1C];
/*1E*/ s16 enabledGamebit;
/*20*/ s16 stateGamebit;
} ToggleSwitch_Setup;

typedef struct {
    u8 flags;
    u8 state;
    u8 enabled; // unused
} ToggleSwitch_Data;

enum ToggleSwitchFlags {
    TOGGLESWITCH_Moving = 1,
    TOGGLESWITCH_Init = 2
};

static int ToggleSwitch_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8);

// offset: 0x0 | ctor
void ToggleSwitch_ctor(void* dll) { }

// offset: 0xC | dtor
void ToggleSwitch_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void ToggleSwitch_obj_Setup(Object* self, ToggleSwitch_Setup* setup, s32 reset) {
    ToggleSwitch_Data* objdata;

    self->srt.yaw = setup->unk18 << 8;
    self->animCallback = ToggleSwitch_animCallback;
    objdata = self->data;
    objdata->enabled = mainGetBits(setup->enabledGamebit);
    objdata->state = mainGetBits(setup->stateGamebit);
    objdata->flags = TOGGLESWITCH_Init;
}

// offset: 0xB4 | func: 1 | export: 1
void ToggleSwitch_obj_Control(Object* self) {
    ToggleSwitch_Data* objdata;
    ToggleSwitch_Setup* setup;
    s32 damageType;

    damageType = 0;
    objdata = self->data;
    setup = (ToggleSwitch_Setup*)self->setup;
    if (objdata->flags & TOGGLESWITCH_Moving) {
        objdata->flags &= ~TOGGLESWITCH_Moving;
        return;
    }
    if (mainGetBits(setup->stateGamebit) != objdata->state) {
        objdata->state ^= 1;
        if (mainGetBits(setup->enabledGamebit) != 0) {
            gDLL_3_Animation->vtbl->preempt_sequence_time(self, setup->preemptTime);
        }
        gDLL_3_Animation->vtbl->start_obj_sequence(objdata->state, self, 1);
        objdata->flags |= TOGGLESWITCH_Moving;
    }
    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    if (!(self->unkAF & ARROW_FLAG_10_Greyed_Out)) {
        damageType = func_80025F40(self, NULL, NULL, NULL);
    }
    if ((damageType == Damage_Type_Projectile) || (self->unkAF & ARROW_FLAG_1_Interacted)) {
        objdata->state ^= 1;
        if (damageType != 0) {
            gDLL_3_Animation->vtbl->start_obj_sequence(objdata->state, self, 1);
        } else {
            gDLL_3_Animation->vtbl->start_obj_sequence(objdata->state, self, -1);
        }
        objdata->flags |= TOGGLESWITCH_Moving;
        // @bug: still disables the A-button even if a projectile activated the switch
        joyDisableButtons(0, A_BUTTON);
    }
    if (objdata->flags & TOGGLESWITCH_Init) {
        gDLL_3_Animation->vtbl->preempt_sequence_time(self, setup->preemptTime);
        gDLL_3_Animation->vtbl->start_obj_sequence(objdata->state, self, 1);
        objdata->flags &= ~TOGGLESWITCH_Init;
    }
}

// offset: 0x2F8 | func: 2 | export: 2
void ToggleSwitch_obj_Update(Object* self) { }

// offset: 0x304 | func: 3 | export: 3
void ToggleSwitch_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x358 | func: 4 | export: 4
void ToggleSwitch_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x368 | func: 5 | export: 5
u32 ToggleSwitch_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x378 | func: 6 | export: 6
u32 ToggleSwitch_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(ToggleSwitch_Data);
}

// offset: 0x38C | func: 7
static int ToggleSwitch_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    ToggleSwitch_Data* objdata;
    ToggleSwitch_Setup* setup;

    objdata = self->data;
    setup = (ToggleSwitch_Setup*)self->setup;
    if ((animObjData->lastMessage == 1) && (objdata->flags & TOGGLESWITCH_Moving)) {
        if (mainGetBits(setup->enabledGamebit) != 0) {
            objdata->state = mainGetBits(setup->stateGamebit) ^ 1;
            mainSetBits(setup->stateGamebit, objdata->state);
        }
    }
    return 0;
}

/*0x0*/ static const char str_0[] = " Setting Wait Bit %i ";
/*0x18*/ static const char str_18[] = " Setting Wait Bit %i ";
