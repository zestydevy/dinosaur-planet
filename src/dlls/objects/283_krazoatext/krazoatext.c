#include "PR/gbi.h"
#include "PR/os.h"
#include "PR/ultratypes.h"
#include "dlls/engine/21_gametext.h"
#include "libc/string.h"
#include "dll.h"
#include "functions.h"
#include "game/objects/object.h"
#include "sys/joypad.h"
#include "sys/gfx/texture.h"
#include "sys/gfx/model.h"
#include "sys/memory.h"
#include "sys/objmsg.h"
#include "game/gametexts.h"
#include "sys/rcp.h"
#include "types.h"

#define KRAZOA_ALPHABET_ICONS 0x187
#define KRAZOA_ALPHABET_LENGTH 26
#define KRAZOA_STRING_MAX_LENGTH 64

#define KRAZOA_PRINT_BASE_X 160
#define KRAZOA_PRINT_BASE_Y 195
#define KRAZOA_PRINT_KERNING 18

#define ASCII_A 0x41
#define ASCII_Z 0x5A

typedef struct {
/*00*/ s8 state;
/*01*/ s8 stringLoaded;
/*02*/ s8 glyphsLoaded;
/*03*/ char text[KRAZOA_STRING_MAX_LENGTH + 1];
/*44*/ Texture* glyphs[KRAZOA_ALPHABET_LENGTH];
/*AC*/ u32 sound;
} KText_Data;

typedef struct {
 /*00*/ ObjSetup base;
 /*18*/ s16 unused18;
 /*1A*/ s16 stringID;
} KText_Setup;

/*0x0*/ static u8 currentGlyph[0x10];

static void krazoatext_load_required_glyph_textures(Object* self, KText_Data* objdata);
static void krazoatext_unload_all_glyph_textures(Object* self, KText_Data* objdata);

// offset: 0x0 | ctor
void krazoatext_ctor(void* dll){ }

// offset: 0xC | dtor
void krazoatext_dtor(void* dll){ }

// offset: 0x18 | func: 0 | export: 0
void krazoatext_setup(Object* self, s32 arg1, s32 arg2) {  
    obj_init_mesg_queue(self, 2);
    krazoatext_unload_all_glyph_textures(self, self->data);
    self->unkB0 |= 0x2000;
}

// offset: 0x88 | func: 1 | export: 1
void krazoatext_control(Object* self) {
    KText_Data* objdata;
    KText_Setup* setup;
    u32 message;
    GameTextChunk* gametext;
    u16 length;

    objdata = self->data;
    setup = ((KText_Setup*)self->setup);

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

    while (obj_recv_mesg(self, &message, 0, 0)) {
    }

    if (objdata->state != 0) {
        if (!(self->unkAF & 4) || (joy_get_pressed(0) & B_BUTTON)) {
            objdata->state = 0;
        }
    } else if (self->unkAF & 1) {
        objdata->state = 1;
        if (objdata->sound == 0) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_286, 0x7F, (u32*)&objdata->sound, 0, 0, 0);
        }
    }

    if (objdata->state && objdata->stringLoaded) {
        krazoatext_load_required_glyph_textures(self, objdata);
    } else {
        krazoatext_unload_all_glyph_textures(self, objdata);
    }

    if (objdata->sound && !gDLL_6_AMSFX->vtbl->func_B48(objdata->sound)) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->sound);
        objdata->sound = NULL;
    }
}

// offset: 0x324 | func: 2 | export: 2
void krazoatext_update(Object* self){ }

// offset: 0x330 | func: 3 | export: 3
void krazoatext_print(Object* self, Gfx** gfx, Mtx** mtx, Vertex** vtx, Triangle** pols, s8 visibility) {
    if (visibility) {
        self->srt.scale = 0.001f;
        draw_object(self, gfx, mtx, vtx, pols, 1.0f);
        self->srt.scale = self->def->scale;
    }
}

// offset: 0x3A4 | func: 4 | export: 4
void krazoatext_free(Object* self, s32 arg1) {
    KText_Data* objdata = self->data;

    krazoatext_unload_all_glyph_textures(self, objdata);
    if (objdata->sound) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->sound);
    }
}

// offset: 0x414 | func: 5 | export: 5
u32 dll_283_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x424 | func: 6 | export: 6
u32 dll_283_get_data_size(Object *self, u32 a1) {
    return sizeof(KText_Data);
}

// offset: 0x438 | func: 7 | export: 7
void krazoatext_print_text(Object* self, Gfx** gfx) {
    KText_Data* objdata;
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
        if ((objdata->text[charCount] >= ASCII_A) && (objdata->text[charCount] <= ASCII_Z)) {
            glyphIndex = objdata->text[charCount] - ASCII_A;
            if (objdata->glyphs[glyphIndex]) {
                ((Func_80037F9C_Struct*)currentGlyph)->unk0 = objdata->glyphs[glyphIndex];
                ((Func_80037F9C_Struct*)currentGlyph)->unk8 = 0;
                ((Func_80037F9C_Struct*)currentGlyph)->unkA = 0;
                ((Func_80037F9C_Struct*)currentGlyph)->pad4 = 0;
                func_80037F9C(gfx, (Func_80037F9C_Struct*)&currentGlyph, xCoord, KRAZOA_PRINT_BASE_Y, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }

        xCoord += KRAZOA_PRINT_KERNING;
        charCount++;
    }        
}

// offset: 0x564 | func: 8
void krazoatext_load_required_glyph_textures(Object* self, KText_Data* objdata) {
    s32 charIndex;
    u32 glyphIndex;

    if (objdata->glyphsLoaded){
        return;
    }

    charIndex = 0;
    while (objdata->text[charIndex]){
        if (ASCII_A <= objdata->text[charIndex] && objdata->text[charIndex] <= ASCII_Z){
            glyphIndex = (u32)objdata->text[charIndex] - ASCII_A;
            if (objdata->glyphs[glyphIndex] == NULL) {
                objdata->glyphs[glyphIndex] = tex_load_deferred(KRAZOA_ALPHABET_ICONS + glyphIndex);
            }
        }
        charIndex++;
    }
    objdata->glyphsLoaded = TRUE;

}

// offset: 0x61C | func: 9
void krazoatext_unload_all_glyph_textures(Object* self, KText_Data* objdata) {
    s32 index;

    if (objdata->glyphsLoaded) {
        for (index = 0; index < KRAZOA_ALPHABET_LENGTH; index++){
            if (objdata->glyphs[index]) {
                tex_free(objdata->glyphs[index]);
            }
            objdata->glyphs[index] = NULL;
        }
        objdata->glyphsLoaded = FALSE;  
    }
}
