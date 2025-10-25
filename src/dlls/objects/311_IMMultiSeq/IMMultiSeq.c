#include "game/objects/object.h"
#include "dlls/objects/214_animobj.h"
#include "sys/objtype.h"
#include "dll.h"
#include "sys/main.h"
#include "functions.h"

#define IMMULTISEQ_GAMEBIT_COUNT 4

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebits1[IMMULTISEQ_GAMEBIT_COUNT];
/*20*/ s16 gamebits2[IMMULTISEQ_GAMEBIT_COUNT];
/*28*/ u8 yaw;
/*29*/ u8 unk29;
/*2A*/ u8 modelInstIdx;
/*2B*/ u8 unk2B;
/*2C*/ s8 objectSeqIndex[IMMULTISEQ_GAMEBIT_COUNT];
/*30*/ u8 values;
} IMMultiSeq_Setup;

#define VALUE_GAMEBITS1(i) ((setup->values >> (i + 4)) & 1)
#define VALUE_GAMEBITS2(i) ((setup->values >> (i    )) & 1)

typedef struct {
    u8 setBitsCount; // count of gamebits with changed value
    u8 flags;
} IMMultiSeq_Data;

typedef enum {
    IMMULTISEQ_SET_GAMEBIT = 0x1
} IMMultiSeq_Flags;

static int IMMultiSeq_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, void *arg3);

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
    self->unkBC = (ObjectCallback)IMMultiSeq_anim_callback;
    self->unkB0 |= 0x6000;
    self->modelInstIdx = setup->modelInstIdx;
    if (self->modelInstIdx >= self->def->numModels) {
        // diPrintf("SEQOBJ.c: modelno out of range romdefno=%d\n", self->modelInstIdx);
        self->modelInstIdx = 0;
    }
    obj_add_object_type(self, OBJTYPE_17);
    // find index of first unset gamebit
    for (i = 0; i != IMMULTISEQ_GAMEBIT_COUNT; i++) {
        if (main_get_bits(setup->gamebits1[i]) == VALUE_GAMEBITS1(i)) {
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
        value = VALUE_GAMEBITS1(objdata->setBitsCount) == 0;\
        gamebit = setup->gamebits1[objdata->setBitsCount];
        main_set_bits(gamebit, value);
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
            value = (VALUE_GAMEBITS2(objdata->setBitsCount) == 0);
            if (main_get_bits(setup->gamebits2[objdata->setBitsCount]) == value) {
                if (setup->objectSeqIndex[objdata->setBitsCount] != -1) {
                    gDLL_3_Animation->vtbl->func17(setup->objectSeqIndex[objdata->setBitsCount], self, -1);
                }
            }
        }
        break;
    }

    for (i = objdata->setBitsCount - 1; i >= 0; i--) {
        if (setup->gamebits1[i] == -1)
            break;
        value = main_get_bits(setup->gamebits1[i]);
        if (value != VALUE_GAMEBITS1(i)) {
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
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x318 | func: 4 | export: 4
void IMMultiSeq_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_17);
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
int IMMultiSeq_anim_callback(Object *self, Object *animObj, AnimObj_Data *animObjData, void *arg3) { // NOLINT
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
    if (self->unkB4 == -1) {
        return 0;
    }
    currentSetBitsCount = objdata->setBitsCount;
    nextSetBitsCount = currentSetBitsCount + 1;
    if (currentSetBitsCount != IMMULTISEQ_GAMEBIT_COUNT && nextSetBitsCount < IMMULTISEQ_GAMEBIT_COUNT && setup->gamebits2[nextSetBitsCount] != -1) {
        value = VALUE_GAMEBITS2(nextSetBitsCount) == 0;
        if (main_get_bits(setup->gamebits2[nextSetBitsCount]) == value) {
            gDLL_3_Animation->vtbl->func18(self->unkB4);
        }
    }
    objdata->flags |= IMMULTISEQ_SET_GAMEBIT;
    return 0;
}

/*0x0*/ static const char str_0[] = "SEQOBJ.c: modelno out of range romdefno=%d\n";
/*0x2C*/ static const char str_2C[] = "";
