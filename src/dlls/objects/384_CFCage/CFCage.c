#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object_id.h"
#include "sys/gfx/animseq.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "dll.h"

/** @file For both the CFPrisonCage and CFCageSwitch! */

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebit;
/*1A*/ u8 yaw8;
} CFCage_Setup;

static int CFCage_animCallback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8);

// offset: 0x0 | ctor
void CFCage_ctor(void* dll) { }

// offset: 0xC | dtor
void CFCage_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFCage_obj_Setup(Object* self, CFCage_Setup* setup, s32 reset) {
    objInitMesgQueue(self, 1);
    self->srt.yaw = setup->yaw8 << 8;
    self->unkDC = 1;
    self->animCallback = CFCage_animCallback;
    if (self->id == OBJ_CFCageSwitch) {
        if (mainGetBits(setup->gamebit) != 0) {
            objAnimSet(self, 1, 0.0f, 0);
        } else {
            objAnimSet(self, 0, 0.0f, 0);
        }
    } else {
        if (mainGetBits(setup->gamebit) != 0) {
            gDLL_3_Animation->vtbl->preempt_sequence_time(self, 60);
        }
    }
}

// offset: 0x134 | func: 1 | export: 1
void CFCage_obj_Control(Object* self) {
    s32 seqno;

    if (self->unkDC != 0) {
        switch (self->id) {
        case OBJ_CFPrisonCage:
            seqno = 0;
            break;
        case OBJ_CFCageSwitch:
        default:
            seqno = 1;
            break;
        }
        gDLL_3_Animation->vtbl->start_obj_sequence(seqno, self, -1);
        self->unkDC = 0;
    }
}

// offset: 0x1C4 | func: 2 | export: 2
void CFCage_obj_Update(Object* self) { }

// offset: 0x1D0 | func: 3 | export: 3
void CFCage_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x224 | func: 4 | export: 4
void CFCage_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x234 | func: 5 | export: 5
u32 CFCage_obj_GetModelFlags(Object* self) {
    if (self->id == OBJ_CFCageSwitch) {
        return MODFLAGS_8;
    }
    return 0;
}

// offset: 0x258 | func: 6 | export: 6
u32 CFCage_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return 0;
}

// offset: 0x26C | func: 7
static int CFCage_animCallback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    CFCage_Setup* setup;
    u32 mesgID;
    Object* sender;
    void* mesgArg;

    mesgArg = NULL;
    setup = (CFCage_Setup*)actor->setup;
    if (mainGetBits(setup->gamebit) != 0) {
        actor->unkAF |= ARROW_FLAG_8_No_Targetting;
        animObjData->unk9D |= 4;
        return 0;
    }
    if (actor->id == OBJ_CFPrisonCage) {
        return 0;
    } else {
        while (objRecvMesg(actor, &mesgID, &sender, &mesgArg) != 0) {
            if (mesgID == 0xA0005) {
                mainSetBits(setup->gamebit, 1);
            }
            if (1){} // @fake
        }
        if (mainGetBits(BIT_CRF_Prison_Key_1) != 0) {
            actor->unkAF &= ~ARROW_FLAG_10_Greyed_Out;
        } else {
            actor->unkAF |= ARROW_FLAG_10_Greyed_Out;
        }
        if ((actor->unkAF & ARROW_FLAG_1_Interacted) && (gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_CRF_Prison_Key_1) != 0)) {
            actor->unkAF |= ARROW_FLAG_8_No_Targetting;
            gDLL_3_Animation->vtbl->start_obj_sequence(0, actor, -1);
        }
        return 0;
    }
}
