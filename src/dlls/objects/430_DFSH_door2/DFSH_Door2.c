#include "common.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 gamebit;
} DFSH_Door2_Setup;

typedef struct {
    u16 phase;
    u8 unk2;
    u8 state;
    u8 unk4;
    u8 unk5;
} DFSH_Door2_Data;


typedef enum {
    DFSH_Door2_STATE_0,
    DFSH_Door2_STATE_1,
    DFSH_Door2_STATE_2
} DFSH_Door2_States;

static int DFSH_Door2_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3);

// offset: 0x0 | ctor
void DFSH_Door2_ctor(void *dll) { }

// offset: 0xC | dtor
void DFSH_Door2_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFSH_Door2_setup(Object* self, DFSH_Door2_Setup* objSetup, s32 reset) {
    DFSH_Door2_Data* objData;
    TextureAnimator* texAnim;

    objData = self->data;
    self->animCallback = DFSH_Door2_anim_callback;

    if (main_get_bits(objSetup->gamebit)) {
        objData->state = DFSH_Door2_STATE_2;
    } else {
        objData->state = DFSH_Door2_STATE_0;
    }
    
    texAnim = func_800348A0(self, 0, 0);
    if (texAnim != NULL) {
        if (objData->state == DFSH_Door2_STATE_2) {
            texAnim->frame = 1;
        } else {
            texAnim->frame = 0;
        }
    }
    
    objData->phase = 0;
}

// offset: 0xD4 | func: 1 | export: 1
void DFSH_Door2_control(Object *self) { }

// offset: 0xE0 | func: 2 | export: 2
void DFSH_Door2_update(Object *self) { }

// offset: 0xEC | func: 3 | export: 3
void DFSH_Door2_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x140 | func: 4 | export: 4
void DFSH_Door2_free(Object *self, s32 onlySelf) { }

// offset: 0x150 | func: 5 | export: 5
u32 DFSH_Door2_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x160 | func: 6 | export: 6
u32 DFSH_Door2_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DFSH_Door2_Data);
}

// offset: 0x174 | func: 7
int DFSH_Door2_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
    TextureAnimator* texAnim;
    DFSH_Door2_Setup* objSetup;
    DFSH_Door2_Data* objData;
    s32 frame;

    objData = self->data;
    objSetup = (DFSH_Door2_Setup*)self->setup;
    
    switch (objData->state) {
    case DFSH_Door2_STATE_0:
        if (main_get_bits(objSetup->gamebit)) {
            objData->state = DFSH_Door2_STATE_1;
        }
        break;
    case DFSH_Door2_STATE_1:
        texAnim = func_800348A0(self, 0, 0);
        if (texAnim != NULL) {
            frame = texAnim->frame + (gUpdateRate * 0x10);
            if (frame > 0x100) {
                frame = 0x100;
                objData->state = DFSH_Door2_STATE_2;
            }
            texAnim->frame = frame;
        }
        break;
    default:
    case DFSH_Door2_STATE_2:
        texAnim = func_800348A0(self, 0, 0);
        if (texAnim != NULL) {
            objData->phase += gUpdateRate * 0x320;
            texAnim->frame = 0x100 - ((1.0f - fcos16_precise(objData->phase)) * 50.0f);
        }
        break;
    }
    return 0;
}
