#include "sys/gfx/animseq.h"
#include "sys/main.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "dll.h"
#include "macros.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 playedBits[8];
/*28*/ s16 playBits[8];
/*38*/ u8 rotation;
/*39*/ u8 flags;
/*3A*/ u8 replaySeqIndex;
/*3B*/ u8 replayActorMask;
/*3C*/ s16 preemptTime;
/*3E*/ u8 _unk3E[0x40 - 0x3E];
/*40*/ s8 seqIndices[8];
} NWMultiSeq_Setup;

typedef struct {
/*0*/ u8 state;
/*1*/ u8 flags;
} NWMultiSeq_Data;

enum NWMultiSeqSetupFlags {
    NWMULTISEQ_SETUP_FLAG_Replay = 0x10
};

enum NWMultiSeqFlags {
    NWMULTISEQ_FLAG_SeqPlayed = 0x1
};

static int NWMultiSeq_anim_callback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3);

// offset: 0x0 | ctor
void NWMultiSeq_ctor(void *dll) { }

// offset: 0xC | dtor
void NWMultiSeq_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void NWMultiSeq_setup(Object* self, NWMultiSeq_Setup* setup, s32 reset) {
    NWMultiSeq_Data* objdata;
    s32 i;

    self->srt.yaw = setup->rotation << 8;
    self->animCallback = NWMultiSeq_anim_callback;
    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);
    objdata = self->data;
    obj_add_object_type(self, OBJTYPE_UseObj);
    for (i = 0; i < 8; i++) {
        if ((setup->playedBits[i] == -1) || (mainGetBits(setup->playedBits[i]) == 0)) {
            break;
        }
    }
    if ((i < 8) && (setup->playedBits[i] == -1)) {
        objdata->state = 8;
    } else {
        objdata->state = i;
    }
    if ((objdata->state == 8) && (setup->flags & NWMULTISEQ_SETUP_FLAG_Replay)) {
        objdata->state = 9;
    }
}

// offset: 0x138 | func: 1 | export: 1
void NWMultiSeq_control(Object* self) {
    NWMultiSeq_Data* objdata;
    NWMultiSeq_Setup* setup;
    s32 i;

    objdata = self->data;
    setup = (NWMultiSeq_Setup*)self->setup;

    if (objdata->flags & NWMULTISEQ_FLAG_SeqPlayed) {
        if (setup->playedBits[objdata->state] != -1) {
            mainSetBits(setup->playedBits[objdata->state], 1);
        }
        objdata->flags &= ~NWMULTISEQ_FLAG_SeqPlayed;
        objdata->state++;
    }
    switch (objdata->state) {
    case 8:
    case 10:
        break;
    case 9:
        STUBBED_PRINTF("prempting the sequence\n");
        gDLL_3_Animation->vtbl->preempt_sequence_time(self, setup->preemptTime);
        gDLL_3_Animation->vtbl->start_obj_sequence(setup->replaySeqIndex, self, setup->replayActorMask);
        break;
    default:
        if (setup->playBits[objdata->state] == -1) {
            objdata->state = 8;
        } else {
            if (mainGetBits(setup->playBits[objdata->state]) != 0) {
                if (setup->seqIndices[objdata->state] != -1) {
                    gDLL_3_Animation->vtbl->start_obj_sequence(setup->seqIndices[objdata->state], self, -1);
                }
            }
        }
        break;
    }
    i = objdata->state - 1;
    while (i >= 0) {
        if ((setup->playedBits[i] == -1) || (mainGetBits(setup->playedBits[i]) != 0)) {
            break;
        }
        objdata->state--;
        i--;
    }
}

// offset: 0x338 | func: 2 | export: 2
void NWMultiSeq_update(Object *self) { }

// offset: 0x344 | func: 3 | export: 3
void NWMultiSeq_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x398 | func: 4 | export: 4
void NWMultiSeq_free(Object* self, s32 onlySelf) {
    obj_free_object_type(self, OBJTYPE_UseObj);
}

// offset: 0x3D8 | func: 5 | export: 5
u32 NWMultiSeq_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x3E8 | func: 6 | export: 6
u32 NWMultiSeq_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(NWMultiSeq_Data);
}

// offset: 0x3FC | func: 7
static int NWMultiSeq_anim_callback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3) {
    NWMultiSeq_Setup* setup;
    NWMultiSeq_Data* objdata;
    s32 nextState;

    objdata = actor->data;
    setup = (NWMultiSeq_Setup*)actor->setup;
    animObjData->unk62 = 0;
    animObjData->unk7A = animObjData->unk7C;
    if (actor->seqSlot == -1) {
        return 0;
    }
    switch (objdata->state) {
        case 8:
        case 9:
            break;
        default:
            nextState = objdata->state + 1;
            if (nextState < 8) {
                if ((setup->playBits[nextState] != -1) && (setup->playBits[nextState] != setup->playBits[objdata->state])) {
                    if (mainGetBits(setup->playBits[nextState]) != 0) {
                        gDLL_3_Animation->vtbl->end_obj_sequence(actor->seqSlot);
                    }
                }
            }
            break;
    }
    objdata->flags |= NWMULTISEQ_FLAG_SeqPlayed;
    return 0;
}
