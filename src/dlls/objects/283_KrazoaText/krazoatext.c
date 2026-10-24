#include "PR/gbi.h"
#include "PR/os.h"
#include "PR/ultratypes.h"
#include "libc/string.h"
#include "dll.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/objects/283_KrazoaText.h"
#include "game/gametexts.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "game/objects/setups/51_Generic.h"
#include "sys/gfx/texture.h"
#include "sys/gfx/model.h"
#include "sys/joypad.h"
#include "sys/memory.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "sys/rcp.h"
#include "types.h"

#define KRAZOA_ALPHABET_ICONS 0x187
#define KRAZOA_ALPHABET_LENGTH 26
#define KRAZOA_STRING_MAX_LENGTH 64

#define KRAZOA_PRINT_BASE_X 160
#define KRAZOA_PRINT_BASE_Y 195
#define KRAZOA_PRINT_KERNING 18

typedef struct {
/*00*/ s8 state;
/*01*/ s8 stringLoaded;
/*02*/ s8 glyphsLoaded;
/*03*/ char text[KRAZOA_STRING_MAX_LENGTH + 1];
/*44*/ Texture* glyphs[KRAZOA_ALPHABET_LENGTH];
/*AC*/ u32 soundHandle;
} KrazoaText_Data;

typedef enum {
    KrazoaText_STATE_0_Awaiting_Interact,
    KrazoaText_STATE_1_Interacted
} KrazoaText_States;

/*0x0*/ static struct {
    TextureTile tile;
    u32 null; // terminates the texture tile list
} currentGlyph;

static void KrazoaText_loadRequiredGlyphTextures(Object* self, KrazoaText_Data* objdata);
static void KrazoaText_unloadAllGlyphTextures(Object* self, KrazoaText_Data* objdata);

// offset: 0x0 | ctor
void KrazoaText_ctor(void* dll){ }

// offset: 0xC | dtor
void KrazoaText_dtor(void* dll){ }

// offset: 0x18 | func: 0 | export: 0
void KrazoaText_obj_Setup(Object* self, KrazoaText_Setup* setup, s32 reset) {  
    objInitMesgQueue(self, 2);
    KrazoaText_unloadAllGlyphTextures(self, self->data);
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x88 | func: 1 | export: 1
void KrazoaText_obj_Control(Object* self) {
    KrazoaText_Data* objdata;
    KrazoaText_Setup* setup;
    u32 message;
    GameTextChunk* gametext;
    u16 length;

    objdata = self->data;
    setup = ((KrazoaText_Setup*)self->setup);

    if (objdata->stringLoaded == FALSE) {
        gametext = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_001_Krazoa_Translator);
        length = strlen(gametext->strings[setup->stringID]) + 1;
        if (length > KRAZOA_STRING_MAX_LENGTH) {
            length = KRAZOA_STRING_MAX_LENGTH;
            gametext->strings[setup->stringID][KRAZOA_STRING_MAX_LENGTH - 1] = 0;
        }
        bcopy(gametext->strings[setup->stringID], &objdata->text, length);
        objdata->text[KRAZOA_STRING_MAX_LENGTH - 1] = 0;
        mmFree(gametext);
        objdata->stringLoaded = TRUE;
    }

    while (objRecvMesg(self, &message, 0, 0)) {
    }

    if (objdata->state != KrazoaText_STATE_0_Awaiting_Interact) {
        //Dismiss text with B button, or when no longer highlighted
        if (((self->unkAF & ARROW_FLAG_4_Highlighted) == FALSE) || 
             (joyGetPressed(0) & B_BUTTON)
        ) {
            objdata->state = KrazoaText_STATE_0_Awaiting_Interact;
        }
    } else if (self->unkAF & ARROW_FLAG_1_Interacted) {
        objdata->state = KrazoaText_STATE_1_Interacted;
        if (objdata->soundHandle == 0) {
            dll_amSfx->Play(self, SOUND_286, MAX_VOLUME, &objdata->soundHandle, 0, 0, 0);
        }
    }

    if ((objdata->state != KrazoaText_STATE_0_Awaiting_Interact) && objdata->stringLoaded) {
        KrazoaText_loadRequiredGlyphTextures(self, objdata);
    } else {
        KrazoaText_unloadAllGlyphTextures(self, objdata);
    }

    if (objdata->soundHandle && (dll_amSfx->IsPlaying(objdata->soundHandle) == FALSE)) {
        dll_amSfx->Stop(objdata->soundHandle);
        objdata->soundHandle = NULL;
    }
}

// offset: 0x324 | func: 2 | export: 2
void KrazoaText_obj_Update(Object* self){ }

// offset: 0x330 | func: 3 | export: 3
void KrazoaText_obj_Print(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    if (visibility) {
        self->srt.scale = 0.001f;
        objprintDrawModel(self, gfx, mtx, vtx, pols, 1.0f);
        self->srt.scale = self->def->scale;
    }
}

// offset: 0x3A4 | func: 4 | export: 4
void KrazoaText_obj_Free(Object* self, s32 onlySelf) {
    KrazoaText_Data* objdata = self->data;

    KrazoaText_unloadAllGlyphTextures(self, objdata);
    if (objdata->soundHandle) {
        dll_amSfx->Stop(objdata->soundHandle);
    }
}

// offset: 0x414 | func: 5 | export: 5
u32 KrazoaText_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x424 | func: 6 | export: 6
u32 KrazoaText_obj_GetDataSize(Object *self, u32 offsetAddr) {
    return sizeof(KrazoaText_Data);
}

// offset: 0x438 | func: 7 | export: 7
void KrazoaText_PrintText(Object* self, Gfx** gfx) {
    KrazoaText_Data* objdata;
    Texture* glyph;
    s32 charCount;
    s32 glyphIndex;
    s32 xCoord;

    objdata = self->data;
    if (!objdata->glyphsLoaded)
        return;

    charCount = 0;
    while (objdata->text[charCount] != '\0'){
        charCount += 1;
    }

    xCoord = KRAZOA_PRINT_BASE_X - (charCount * (KRAZOA_PRINT_KERNING/2));

    charCount = 0;
    while (objdata->text[charCount] != '\0'){
        if ((objdata->text[charCount] >= 'A') && (objdata->text[charCount] <= 'Z')) {
            glyphIndex = objdata->text[charCount] - 'A';
            if (objdata->glyphs[glyphIndex]) {
                currentGlyph.tile.tex = objdata->glyphs[glyphIndex];
                currentGlyph.tile.x = 0;
                currentGlyph.tile.y = 0;
                currentGlyph.tile.animProgress = 0;
                rcpTileWrite(gfx, &currentGlyph.tile, xCoord, KRAZOA_PRINT_BASE_Y, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }

        xCoord += KRAZOA_PRINT_KERNING;
        charCount++;
    }        
}

// offset: 0x564 | func: 8
void KrazoaText_loadRequiredGlyphTextures(Object* self, KrazoaText_Data* objdata) {
    s32 charIndex;
    u32 glyphIndex;

    if (objdata->glyphsLoaded){
        return;
    }

    charIndex = 0;
    while (objdata->text[charIndex]){
        if ('A' <= objdata->text[charIndex] && objdata->text[charIndex] <= 'Z'){
            glyphIndex = (u32)objdata->text[charIndex] - 'A';
            if (objdata->glyphs[glyphIndex] == NULL) {
                objdata->glyphs[glyphIndex] = texLoadTexture(KRAZOA_ALPHABET_ICONS + glyphIndex);
            }
        }
        charIndex++;
    }
    objdata->glyphsLoaded = TRUE;

}

// offset: 0x61C | func: 9
void KrazoaText_unloadAllGlyphTextures(Object* self, KrazoaText_Data* objdata) {
    s32 index;

    if (objdata->glyphsLoaded) {
        for (index = 0; index < KRAZOA_ALPHABET_LENGTH; index++){
            if (objdata->glyphs[index]) {
                texFreeTexture(objdata->glyphs[index]);
            }
            objdata->glyphs[index] = NULL;
        }
        objdata->glyphsLoaded = FALSE;  
    }
}
