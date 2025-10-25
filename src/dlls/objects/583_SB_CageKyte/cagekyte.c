#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/214_animobj.h"
#include "functions.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/rand.h"
#include "types.h"

// This is the baby Kyte object ("DLL 581 KyteCage" is the cage)

typedef struct {
    u32 soundHandle;
    s16 randomSoundDelay;
} CageKyte_Data;

static int SBCageKyte_anim_callback(Object* self, Object* arg1, AnimObj_Data* arg2, s8 arg3);

// offset: 0x0 | ctor
void SBCageKyte_ctor(void *dll) { }

// offset: 0xC | dtor
void SBCageKyte_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SBCageKyte_setup(Object* self, s32 arg1, s32 arg2) {
    self->animCallback = (AnimationCallback)SBCageKyte_anim_callback;
}

// offset: 0x40 | func: 1 | export: 1
void SBCageKyte_control(Object* self) {
    s32 pad1[4];
    CageKyte_Data* objData;
    s32 randomSoundIndex;
    u16 soundIDs[2] = {
        SOUND_284_SB_Kyte_Heeelp, 
        SOUND_285_SB_Kyte_Find_the_wizard_and_get_me_out_of_here};
    u16 dataUnused[2] = {0x0066, 0x0067};
    s32 pad2;

    objData = self->data;
    if (self->unk0xdc > 0) {
        self->unk0xdc--;
    }
    self->unk0xaf |= 8;

    if (self->parent->unk0xdc == 7) {
        //Randomly call out to Krystal
        objData->randomSoundDelay -= delayByte;
        if ((vec3_distance(&self->positionMirror, &get_player()->positionMirror) < 280.0f) && (objData->randomSoundDelay <= 0)) {
            if (rand_next(0, 10) < 8) {
                randomSoundIndex = 0;
            } else {
                randomSoundIndex = 1;
            }
            gDLL_6_AMSFX->vtbl->play_sound(self, soundIDs[randomSoundIndex], MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->randomSoundDelay = rand_next(400, 600);
        }
    } else if (self->parent->unk0xdc >= 9) {
        //Play empty sound? (May have been removed)
        self->unk0xaf &= 0xFFF7;
        if (self->unk0xaf & 1) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_287_SB_Kyte_Empty, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
        }
        if (objData->soundHandle) {
            self->unk0xaf |= 8;
            if (!gDLL_6_AMSFX->vtbl->func_B48(objData->soundHandle)) {
                gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandle);
                objData->soundHandle = 0;
            }
        }
    }
}

// offset: 0x26C | func: 2 | export: 2
void SBCageKyte_update(Object *self) { }

// offset: 0x278 | func: 3 | export: 3
void SBCageKyte_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility && !main_get_bits(BIT_WM_Played_Randorn_First_Meeting) && !self->unk0xdc) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x30C | func: 4 | export: 4
void SBCageKyte_free(Object* self, s32 arg1) {
    CageKyte_Data* data = self->data;

    if (data->soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(data->soundHandle);
    }
}

// offset: 0x368 | func: 5 | export: 5
u32 SBCageKyte_get_model_flags(Object *self) { 
    return 1;
}

// offset: 0x378 | func: 6 | export: 6
u32 SBCageKyte_get_data_size(Object *self, u32 a1) {
    return sizeof(CageKyte_Data);
}

// offset: 0x38C | func: 7
int SBCageKyte_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 arg3) {
    if (self->unk0xdc > 0) {
        self->unk0xdc--;
    }
    self->unk0xaf |= 8;
    animObjData->unk7A = -2;
    animObjData->unk62 = 0;
    return 0;
}
