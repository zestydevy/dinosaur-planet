#include "common.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"

#include "sys/gfx/modgfx.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 seqIndex;                //sequence index (in objDef) to play when translated
    s16 gametextID;             //gametext fileID of subtitles to display when translated
    s16 unused1C;
    s16 gamebitTranslated;      //gamebit set when Krazoa Translator is used on textblock
    s16 gamebitInteractable;    //gamebit queried to tell if textblock should be powered on
} Textblock_Setup;

typedef struct {
    f32 baseY;
    s16 gamebitTranslated;
    s16 gamebitInteractable;
    u16 gametextID;
    s16 textureBlend;
    u8 activated;
    u8 seqIndex;
    s8 textureBlendSpeed;
} Textblock_Data;

static DLL_IModgfx* modGfxDLL = NULL;

static void Textblock_tick(Object* self);
static int Textblock_anim_callback(Object* self, Object* arg1, AnimObj_Data* arg2, s8 arg3);

// offset: 0x0 | ctor
void Textblock_ctor(void *dll) { }

// offset: 0xC | dtor
void Textblock_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void Textblock_setup(Object* self, Textblock_Setup* objSetup, s32 arg2) {
    Textblock_Data* objData;

    objData = self->data;
    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = Textblock_anim_callback;
    objData->baseY = self->srt.transl.y;
    objData->seqIndex = objSetup->seqIndex;
    objData->gametextID = objSetup->gametextID;
    objData->gamebitTranslated = objSetup->gamebitTranslated;
    objData->gamebitInteractable = objSetup->gamebitInteractable;
    objData->textureBlend = 0;
    objData->textureBlendSpeed = 4;
    modGfxDLL = dll_load_deferred(DLL_ID_107, 1);
    self->unkB0 |= 0x6000;
}

// offset: 0xD0 | func: 1 | export: 1
void Textblock_control(Object* self) {
    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    Textblock_tick(self);
}

// offset: 0x114 | func: 2 | export: 2
void Textblock_update(Object *self) { }

// offset: 0x120 | func: 3 | export: 3
void Textblock_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0x138 | func: 4 | export: 4
void Textblock_free(Object* self, s32 arg1) {
    if (modGfxDLL != NULL) {
        dll_unload(modGfxDLL);
    }
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x1AC | func: 5 | export: 5
u32 Textblock_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1BC | func: 6 | export: 6
u32 Textblock_get_data_size(Object *self, u32 a1) {
    return sizeof(Textblock_Data);
}

// offset: 0x1D0 | func: 7
void Textblock_tick(Object* self) {
    Textblock_Data* objData;
    s16 interactable;
    s16 translatorUsed;
    TextureAnimator* texAnim;

    interactable = TRUE;

    objData = self->data;

    //Check gamebits to tell if textblock is switched on, and whether it's been translated
    if (objData->gamebitInteractable != NO_GAMEBIT) {
        interactable = main_get_bits(objData->gamebitInteractable);
    }
    translatorUsed = main_get_bits(objData->gamebitTranslated);

    if (translatorUsed || !interactable) {
        return;
    }

    //Enable targetting
    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;

    //Glow effect: ping-pong blend between texture frames 0 and 1
    texAnim = func_800348A0(self, 0, 0);
    if (texAnim != NULL) {
        objData->textureBlend += objData->textureBlendSpeed * (s16) gUpdateRateF;

        if (objData->textureBlend > 0xFF) {
            objData->textureBlend = 0xFF;
            objData->textureBlendSpeed = -objData->textureBlendSpeed;
        } else if (objData->textureBlend < 0) {
            objData->textureBlend = 0;
            objData->textureBlendSpeed = -objData->textureBlendSpeed;
        }

        texAnim->frame = objData->textureBlend;
    }
    
    //Check if player used Krazoa Translator (@bug?: no player distance check?)
    if (((DLL_1_UI*)gDLL_1_UI)->vtbl->func_DF4(BIT_Krazoa_Translator)) {
        ((DLL_3_animation*)gDLL_3_Animation)->vtbl->func17(objData->seqIndex, self, -1);
        objData->activated = TRUE;
        return;
    }
    
    //Set gamebit after using Krazoa Translator on textblock
    if ((objData->activated) && (main_get_bits(objData->gamebitTranslated) == 0)) {
        main_set_bits(objData->gamebitTranslated, 1);
    }
}

// offset: 0x3A0 | func: 8
static int Textblock_anim_callback(Object* self, Object* arg1, AnimObj_Data* animObjData, s8 arg3) {
    Textblock_Data *objData;
    u8 *currentValue;
    s32 i;
    u8 *base;
    
    objData = self->data;
    
    for (i = 0, base = (u8*)animObjData; i < animObjData->unk98; base++, i++) {
        currentValue = base + 0x8E;
        switch (*currentValue){
        case 0:
            continue;
        case 1:
            gDLL_22_Subtitles->vtbl->func_368(objData->gametextID);
            break;
        case 2:
            gDLL_22_Subtitles->vtbl->func_448();
            break;
        }

        *currentValue = 0;
    }
    
    return 0;
}
