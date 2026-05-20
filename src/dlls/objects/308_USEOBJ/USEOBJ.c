#include "game/objects/object.h"
#include "game/objects/interaction_arrow.h"
#include "sys/gfx/animseq.h"
#include "sys/objtype.h"
#include "sys/objprint.h"
#include "sys/main.h"
#include "dll.h"
#include "sys/joypad.h"

enum UseObjFlags {
    USEOBJ_HideIfAlreadyUsed = 0x1,
    USEOBJ_SeqControlsUsedBit = 0x4,
    USEOBJ_DisableAfterUse = 0x8,
    USEOBJ_NoTargetingWhenDisabled = 0x10,
    USEOBJ_ReplayIncludeActor2 = 0x20,
    USEOBJ_ReplayIncludeActor3 = 0x40,
    USEOBJ_ReplayIncludeActor4 = 0x80
};

typedef struct {
    u8 used;
} UseObj_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 yaw;
/*19*/ u8 pitch;
/*1A*/ u8 roll;
/*1B*/ u8 flags;
/*1C*/ s16 gamebitUsed; // gamebit that says whether this object was already used
/*1E*/ s16 gamebitRequiredItem; // inventory item required to use this object
/*20*/ s8 objectSeqIndex;
/*21*/ s8 modelInstIdx;
/*22*/ s16 gamebitEnabled; // gamebit that says whether this object is enabled (and can be used)
/*24*/ s16 replayStartTime; // if not zero, the sequence will be replayed if the object was already used
} UseObj_Setup;

static int UseObj_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3);

// offset: 0x0 | ctor
void UseObj_ctor(void *dll) { }

// offset: 0xC | dtor
void UseObj_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void UseObj_setup(Object *self, UseObj_Setup *setup, s32 arg2) {
    UseObj_Data *objdata;

    self->srt.yaw = setup->yaw << 8;
    self->srt.pitch = setup->pitch << 8;
    self->srt.roll = setup->roll << 8;
    self->animCallback = UseObj_anim_callback;
    self->modelInstIdx = setup->modelInstIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        // diPrintf("USEOBJ.c: modelno out of range romdefno=%d\n"), self->modelInstIdx);
        self->modelInstIdx = 0;
    }
    objdata = self->data;
    objdata->used = main_get_bits(setup->gamebitUsed);
    obj_add_object_type(self, OBJTYPE_UseObj);
    if ((setup->flags & USEOBJ_HideIfAlreadyUsed) && objdata->used != 0) {
        self->opacity = 0;
    }
}

// offset: 0x110 | func: 1 | export: 1
void UseObj_control(Object *self) {
    UseObj_Data *objdata;
    UseObj_Setup *setup;
    s32 actorMask;

    objdata = self->data;
    setup = (UseObj_Setup*)self->setup;
    objdata->used = main_get_bits(setup->gamebitUsed);
    if (objdata->used == 0) {
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        if (setup->gamebitEnabled != -1) {
            if (main_get_bits(setup->gamebitEnabled)) {
                self->unkAF &= ~ARROW_FLAG_10_Greyed_Out;
            } else {
                self->unkAF |= ARROW_FLAG_10_Greyed_Out;
                if (setup->flags & USEOBJ_NoTargetingWhenDisabled) {
                    self->unkAF |= ARROW_FLAG_8_No_Targetting;
                }
            }
        } else {
            self->unkAF &= ~ARROW_FLAG_10_Greyed_Out;
        }
        if ((self->unkAF & ARROW_FLAG_1_Interacted) && (setup->gamebitRequiredItem == -1 || gDLL_1_cmdmenu->vtbl->was_this_item_used(setup->gamebitRequiredItem))) {
            if (setup->objectSeqIndex != -1) {
                gDLL_3_Animation->vtbl->start_obj_sequence(setup->objectSeqIndex, self, -1);
            }
            if (!(setup->flags & USEOBJ_SeqControlsUsedBit)) {
                main_set_bits(setup->gamebitUsed, 1);
            }
            if (setup->flags & USEOBJ_DisableAfterUse) {
                main_set_bits(setup->gamebitEnabled, 0);
            } else {
                objdata->used = 1;
                self->unkDC = 1;
            }
            joy_disable_buttons(0, A_BUTTON);
        }
    } else {
        if (self->unkDC == 0) {
            if (setup->objectSeqIndex != -1) {
                if (setup->replayStartTime != 0) {
                    gDLL_3_Animation->vtbl->preempt_sequence_time(self, setup->replayStartTime);
                    actorMask = 1;
                    if (setup->flags & USEOBJ_ReplayIncludeActor2) {
                        actorMask |= 2;
                    }
                    if (setup->flags & USEOBJ_ReplayIncludeActor3) {
                        actorMask |= 4;
                    }
                    if (setup->flags & USEOBJ_ReplayIncludeActor4) {
                        actorMask |= 8;
                    }
                    gDLL_3_Animation->vtbl->start_obj_sequence(setup->objectSeqIndex, self, actorMask);
                }
            }
            self->unkDC = 1;
        }
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    }
}

// offset: 0x3B8 | func: 2 | export: 2
void UseObj_update(Object *self) {
    if ((self->def->flags & OBJDEF_INVISIBLE) && self->unk74) {
        func_80036438(self);
    }
}

// offset: 0x410 | func: 3 | export: 3
void UseObj_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x464 | func: 4 | export: 4
void UseObj_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_UseObj);
}

// offset: 0x4A4 | func: 5 | export: 5
u32 UseObj_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x4B4 | func: 6 | export: 6
u32 UseObj_get_data_size(Object *self, u32 a1) {
    return sizeof(UseObj_Data);
}

// offset: 0x4C8 | func: 7
static int UseObj_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, s8 arg3) {
    UseObj_Setup *setup;

    setup = (UseObj_Setup*)self->setup;
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    if (animObjData->lastMessage != 0) {
        if ((setup->flags & USEOBJ_SeqControlsUsedBit) && animObjData->lastMessage == 1) {
            main_set_bits(setup->gamebitUsed, 1);
        }
        if (animObjData->lastMessage == 2) {
            if (setup->replayStartTime != 0) {
                gDLL_3_Animation->vtbl->func21(animObjData, setup->replayStartTime);
            }
        }
        animObjData->lastMessage = 0;
    }
    return 0;
}

/*0x0*/ static const char str_0[] = "USEOBJ.c: modelno out of range romdefno=%d\n";
/*0x2C*/ static const char str_2C[] = "";
