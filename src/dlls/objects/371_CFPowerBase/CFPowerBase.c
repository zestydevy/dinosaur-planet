#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "sys/gfx/animseq.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "dll.h"

typedef struct {
    ObjSetup base;
    s8 yaw8;
    u8 _unk19[0x1E - 0x19];
    s16 gamebit;
} CFPowerBase_Setup;

typedef struct {
    s16 gamebit;
    s16 preempted;
    s16 crystalBit;
    s8 seqno;
} CFPowerBase_Data;

static int CFPowerBase_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8);

// offset: 0x0 | ctor
void CFPowerBase_ctor(void* dll) { }

// offset: 0xC | dtor
void CFPowerBase_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFPowerBase_obj_Setup(Object* self, CFPowerBase_Setup* setup, s32 reset) {
    CFPowerBase_Data* objdata = self->data;

    self->srt.yaw = setup->yaw8 << 8;
    objdata->gamebit = setup->gamebit;
    switch (objdata->gamebit) {
    case 0x54:
        objdata->seqno = 0;
        break;
    case 0x55:
        objdata->seqno = 1;
        objSetModel(self, 2);
        break;
    case 0x56:
        objdata->seqno = 2;
        objSetModel(self, 1);
        break;
    }
    self->animCallback = CFPowerBase_animCallback;
    objInitMesgQueue(self, 2);
    if (mainGetBits(objdata->crystalBit) != 0) {
        self->unkAF &= ~ARROW_FLAG_10_Greyed_Out;
    } else {
        self->unkAF |= ARROW_FLAG_10_Greyed_Out;
    }
}

// offset: 0x134 | func: 1 | export: 1
void CFPowerBase_obj_Control(Object* self) {
    CFPowerBase_Data* objdata = self->data;
    
    switch (objdata->gamebit) {
    case BIT_54:
        objdata->crystalBit = BIT_CRF_Red_Power_Crystal;
        break;
    case BIT_55:
        objdata->crystalBit = BIT_CRF_Green_Power_Crystal;
        break;
    case BIT_56:
        objdata->crystalBit = BIT_CRF_Blue_Power_Crystal;
        break;
    }
    if (mainGetBits(objdata->crystalBit) != 0) {
        self->unkAF &= ~ARROW_FLAG_10_Greyed_Out;
    } else {
        self->unkAF |= ARROW_FLAG_10_Greyed_Out;
    }
    if (!objdata->preempted && (mainGetBits(objdata->gamebit) != 0)) {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        objdata->preempted = TRUE;
        gDLL_3_Animation->vtbl->preempt_sequence_time(self, 478);
        gDLL_3_Animation->vtbl->start_obj_sequence(objdata->seqno, self, 3);
    }
    if ((self->unkAF & ARROW_FLAG_1_Interacted) && (gDLL_1_cmdmenu->vtbl->was_this_item_used(objdata->crystalBit) != 0)) {
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        mainSetBits(objdata->crystalBit, 0);
        gDLL_3_Animation->vtbl->start_obj_sequence(objdata->seqno, self, -1);
    }
}

// offset: 0x2EC | func: 2 | export: 2
void CFPowerBase_obj_Update(Object* self) { }

// offset: 0x2F8 | func: 3 | export: 3
void CFPowerBase_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x34C | func: 4 | export: 4
void CFPowerBase_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x35C | func: 5 | export: 5
u32 CFPowerBase_obj_GetModelFlags(Object* self) {
    return MODFLAGS_1;
}

// offset: 0x36C | func: 6 | export: 6
u32 CFPowerBase_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFPowerBase_Data);
}

// offset: 0x380 | func: 7
static int CFPowerBase_animCallback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    CFPowerBase_Data* objdata = self->data;
    Object* sender;
    u32 mesgID;
    void* mesgArg;
    
    mesgArg = NULL;
    while (objRecvMesg(self, &mesgID, &sender, &mesgArg) != 0) {
        switch (mesgID) {
        case 0x110001:
            if ((objdata->gamebit == BIT_54) && (animObjData->time > 460)) {
                objSendMesg(sender, 0x110001, self, NULL);
            }
            break;
        case 0x110002:
            if ((objdata->gamebit == BIT_55) && (animObjData->time > 460)) {
                objSendMesg(sender, 0x110002, self, NULL);
            }
            break;
        case 0x110003:
            if ((objdata->gamebit == BIT_56) && (animObjData->time > 460)) {
                objSendMesg(sender, 0x110003, self, NULL);
            }
            break;
        case 0xA0005:
            mainSetBits(objdata->gamebit, 1);
            break;
        }
    }
    if (objdata->preempted) {
        return 1;
    }
    return 0;
}
