#include "game/objects/object.h"
#include "game/objects/setups/91_IMMultiSeq.h"
#include "sys/gfx/animseq.h"
#include "sys/objtype.h"
#include "dll.h"
#include "sys/main.h"
#include "sys/objprint.h"
#include "macros.h"

#define VALUE_GAMEBITS1(setup, i) (((setup)->values >> ((i) + 4)) & 1)
#define VALUE_GAMEBITS2(setup, i) (((setup)->values >> ((i)    )) & 1)

typedef struct {
    u8 setBitsCount; // count of gamebits with changed value
    u8 flags;
} IMMultiSeq_Data;

typedef enum {
    IMMULTISEQ_SET_GAMEBIT = 0x1
} IMMultiSeq_Flags;

static int IMMultiSeq_anim_callback(Object *self, UNUSED Object *animObj, AnimObj_Data *animObjData, s8 arg3);

// offset: 0x0 | ctor
void IMMultiSeq_ctor(void *dll) { }

// offset: 0xC | dtor
void IMMultiSeq_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void IMMultiSeq_setup(Object *self, IMMultiSeq_Setup *setup, s32 arg2) {
    IMMultiSeq_Data *objdata;
    s32 i;

    objdata = self->data;
    self->srt.yaw = setup->yaw << 8;
    self->animCallback = IMMultiSeq_anim_callback;
    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);
    self->modelInstIdx = setup->modelInstIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        // diPrintf("SEQOBJ.c: modelno out of range romdefno=%d\n", self->modelInstIdx);
        self->modelInstIdx = 0;
    }
    objAddObjectType(self, OBJTYPE_UseObj);
    // find index of first unset gamebit
    for (i = 0; i != IMMULTISEQ_GAMEBIT_COUNT; i++) {
        if (mainGetBits(setup->gamebits1[i]) == VALUE_GAMEBITS1(setup, i)) {
            break;
        }
    }
    objdata->setBitsCount = i;
}

// offset: 0x114 | func: 1 | export: 1
void IMMultiSeq_control(Object *self) {
    s32 i;
    IMMultiSeq_Data *objdata;
    IMMultiSeq_Setup *setup;
    s16 gamebit;
    u32 value;

    objdata = self->data;
    setup = (IMMultiSeq_Setup*)self->setup;
    if (objdata->flags & IMMULTISEQ_SET_GAMEBIT) {
        value = VALUE_GAMEBITS1(setup, objdata->setBitsCount) == 0;\
        gamebit = setup->gamebits1[objdata->setBitsCount];
        mainSetBits(gamebit, value);
        objdata->flags &= ~IMMULTISEQ_SET_GAMEBIT;
        objdata->setBitsCount++;
    }

    switch (objdata->setBitsCount) {
    case IMMULTISEQ_GAMEBIT_COUNT:
        break;
    default:
        if (setup->gamebits2[objdata->setBitsCount] == -1) {
            objdata->setBitsCount = IMMULTISEQ_GAMEBIT_COUNT;
        } else {
            value = (VALUE_GAMEBITS2(setup, objdata->setBitsCount) == 0);
            if (mainGetBits(setup->gamebits2[objdata->setBitsCount]) == value) {
                if (setup->objectSeqIndex[objdata->setBitsCount] != -1) {
                    gDLL_3_Animation->vtbl->start_obj_sequence(setup->objectSeqIndex[objdata->setBitsCount], self, -1);
                }
            }
        }
        break;
    }

    for (i = objdata->setBitsCount - 1; i >= 0; i--) {
        if (setup->gamebits1[i] == -1)
            break;
        value = mainGetBits(setup->gamebits1[i]);
        if (value != VALUE_GAMEBITS1(setup, i)) {
            break;
        }
        objdata->setBitsCount--;
        if (i) {}
    }
}

// offset: 0x2B8 | func: 2 | export: 2
void IMMultiSeq_update(Object *self) { }

// offset: 0x2C4 | func: 3 | export: 3
void IMMultiSeq_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x318 | func: 4 | export: 4
void IMMultiSeq_free(Object *self, s32 a1) {
    objFreeObjectType(self, OBJTYPE_UseObj);
}


// offset: 0x358 | func: 5 | export: 5
u32 IMMultiSeq_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x368 | func: 6 | export: 6
u32 IMMultiSeq_get_data_size(Object *self, u32 a1) {
    return sizeof(IMMultiSeq_Data);
}

// offset: 0x37C | func: 7
int IMMultiSeq_anim_callback(Object *self, UNUSED Object *animObj, AnimObj_Data *animObjData, s8 arg3) { // NOLINT
    s32 currentSetBitsCount;
    IMMultiSeq_Data *objdata;
    s32 nextSetBitsCount;
    u32 value;
    IMMultiSeq_Setup *setup;

    // ???
    animObj = self;
    objdata = self->data;
    setup = (IMMultiSeq_Setup*)self->setup;
    animObjData->unk62 = 0;
    animObjData->unk7A = animObjData->unk7C;
    if (self->seqSlot == SEQSLOT_NONE) {
        return 0;
    }
    currentSetBitsCount = objdata->setBitsCount;
    nextSetBitsCount = currentSetBitsCount + 1;
    if (currentSetBitsCount != IMMULTISEQ_GAMEBIT_COUNT && nextSetBitsCount < IMMULTISEQ_GAMEBIT_COUNT && setup->gamebits2[nextSetBitsCount] != -1) {
        value = VALUE_GAMEBITS2(setup, nextSetBitsCount) == 0;
        if (mainGetBits(setup->gamebits2[nextSetBitsCount]) == value) {
            gDLL_3_Animation->vtbl->end_obj_sequence(self->seqSlot);
        }
    }
    objdata->flags |= IMMULTISEQ_SET_GAMEBIT;
    return 0;
}

/*0x0*/ static const char str_0[] = "SEQOBJ.c: modelno out of range romdefno=%d\n";
/*0x2C*/ static const char str_2C[] = "";
