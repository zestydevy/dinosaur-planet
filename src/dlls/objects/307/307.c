#include "PR/ultratypes.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/objprint.h"
#include "dll.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 gamebitRestoreState;
    s16 preemptTime;
    s8 objSeqIdx;
    u8 yaw;
    u8 enabledActors;
    u8 scale;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    u8 unk28;
    u8 unk29;
} DLL307_Setup;

typedef struct {
    f32 sinYaw;
    f32 cosYaw;
    f32 unk8;
    u8 state;
    u8 startSequence;
    u8 flags;
} DLL307_Data;

static int DLL307_animCallback(Object *self, Object *animObj, AnimObj_Data *animData, s8 prevCallbackValue);
static void DLL307_func_6E4(DLL307_Data *objdata, DLL307_Setup *setup);

// offset: 0x0 | ctor
void DLL307_ctor(void* dll) { }

// offset: 0xC | dtor
void DLL307_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DLL307_obj_Setup(Object* self, DLL307_Setup* setup, s32 reset) {
    DLL307_Data* objdata = self->data;
    
    objdata->startSequence = TRUE;
    self->srt.yaw = setup->yaw << 8;
    self->animCallback = DLL307_animCallback;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;

    self->srt.scale = setup->scale * (1.0f / 64.0f);
    if (self->srt.scale == 0.0f) {
        self->srt.scale = 1.0f;
    }
    self->srt.scale *= self->def->scale;

    if (setup->gamebitRestoreState != NO_GAMEBIT) {
        objdata->state = mainGetBits(setup->gamebitRestoreState);
    } else {
        objdata->state = 0;
    }

    objdata->sinYaw = mathSinfInterp(self->srt.yaw);
    objdata->cosYaw = mathCosfInterp(self->srt.yaw);
    objdata->unk8 = -((objdata->sinYaw * self->srt.transl.x) + (objdata->cosYaw * self->srt.transl.z));

    objdata->flags = 0;
    if (mainGetBits(setup->unk18)) {
        objdata->flags |= 1;
    }
    if (mainGetBits(setup->unk22)) {
        objdata->flags |= 2;
    }
}

// offset: 0x1C8 | func: 1 | export: 1
void DLL307_obj_Control(Object* self) {
    DLL307_Data* objdata;
    DLL307_Setup* setup;
    s32 enabledActors;

    setup = (DLL307_Setup*)self->setup;
    objdata = (DLL307_Data*)self->data;

    if (objdata->startSequence) {
        if (setup->preemptTime && (objdata->state != 0)) {
            enabledActors = setup->enabledActors;
            gDLL_3_Animation->vtbl->preempt_sequence_time(self, setup->preemptTime);
        } else {
            enabledActors = -1;
        }
        
        if (setup->objSeqIdx != -1) {
            gDLL_3_Animation->vtbl->start_obj_sequence(setup->objSeqIdx, self, enabledActors);
        }

        objdata->startSequence = FALSE;
    }
}

// offset: 0x2A8 | func: 2 | export: 2
void DLL307_obj_Update(Object *self) { }

// offset: 0x2B4 | func: 3 | export: 3
void DLL307_obj_Print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x308 | func: 4 | export: 4
void DLL307_obj_Free(Object *self, s32 a1) { }

// offset: 0x318 | func: 5 | export: 5
u32 DLL307_obj_GetModelFlags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x328 | func: 6 | export: 6
u32 DLL307_obj_GetDataSize(Object *self, u32 offsetAddr) {
    return sizeof(DLL307_Data);
}

// offset: 0x33C | func: 7
static int DLL307_animCallback(Object *self, Object *animObj, AnimObj_Data *animData, s8 prevCallbackValue) {
    DLL307_Data *objdata;
    DLL307_Setup *setup;
    TextureAnimator *animator;
    u32 temp_v0_3;
    s32 var_s1;
    s32 i;

    objdata = (DLL307_Data*)self->data;
    setup = (DLL307_Setup*)self->setup;

    if (self->def->numAnimatedFrames != 0) {
        if (objdata->flags & 1) {
            animator = objExprGetTexAnimator(self, 0, 0);
            if (animator != NULL) {
                animator->frame = 0x100;
            }
        }

        if (objdata->flags & 2) {
            animator = objExprGetTexAnimator(self, 1, 0);
            if (animator != NULL) {
                animator->frame = 0x100;
            }
        }
    }

    if (objdata->state == 0) {
        temp_v0_3 = mainGetBits(setup->unk18);
        var_s1 = 0;
        if ((setup->unk22 == NO_GAMEBIT) || mainGetBits(setup->unk22)) {
            var_s1 = 1;
        }

        if ((temp_v0_3 != 0) && !(objdata->flags & 1)) {
            if (self->def->numAnimatedFrames != 0) {
                dll_amSfx->Play(self, SOUND_9A3_Magic_Reverse_Cymbal, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            objdata->flags |= 1;
        }

        if ((var_s1 != 0) && !(objdata->flags & 2)) {
            if (self->def->numAnimatedFrames != 0) {
                dll_amSfx->Play(self, SOUND_9A3_Magic_Reverse_Cymbal, MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            objdata->flags |= 2;
        }

        if (objdata->flags == 3) {
            DLL307_func_6E4(objdata, setup);
            objdata->state = 2;
        }
    } else if ((objdata->state == 1) && (mainGetBits(setup->unk18) == 0)) {
        objdata->state = 3;
    }

    if (objdata->state == 2) {
        for (i = 0; i < animData->messageCount; i++) {
            if (animData->messages[i] == 2) {
                objdata->state = 1;
                if (setup->gamebitRestoreState != NO_GAMEBIT) {
                    mainSetBits(setup->gamebitRestoreState, 1);
                }
            }
        }
    } else if (objdata->state == 3) {
        for (i = 0; i < animData->messageCount; i++) {
            if (animData->messages[i] == 1) {
                DLL307_func_6E4(objdata, setup);
                objdata->state = 0;
                objdata->flags = 0;
                if (setup->gamebitRestoreState != NO_GAMEBIT) {
                    mainSetBits(setup->gamebitRestoreState, 0);
                }
            }
        }
    }

    return !(objdata->state == 2) && !(objdata->state == 3);
}

// offset: 0x6E4 | func: 8
static void DLL307_func_6E4(DLL307_Data *objdata, DLL307_Setup *setup) {
    Camera *camera;
    f32 temp;
    u8 value;

    camera = camGetMain();
    temp = (objdata->sinYaw * camera->srt.transl.x) + (objdata->cosYaw * camera->srt.transl.z) + objdata->unk8;
    if (temp < 0.0f) {
        if (setup->unk24 != NO_GAMEBIT) {
            value = mainGetBits(setup->unk24);
            value ^= setup->unk28;
            mainSetBits(setup->unk24, value);
        }
    } else {
        if (setup->unk26 != NO_GAMEBIT) {
            value = mainGetBits(setup->unk26);
            value ^= setup->unk29;
            mainSetBits(setup->unk26, value);
        }
    }
}
