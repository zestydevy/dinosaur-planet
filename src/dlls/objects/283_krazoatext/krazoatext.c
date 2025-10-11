#include "PR/os.h"
#include "PR/ultratypes.h"
#include "libc/string.h"
#include "dll.h"
#include "functions.h"
#include "game/objects/object.h"
#include "sys/controller.h"
#include "sys/memory.h"
#include "sys/objmsg.h"

#define KRAZOA_ALPHABET_ICONS 0x187
#define KRAZOA_ALPHABET_LENGTH 26
#define KRAZOA_STRING_MAX_LENGTH 64

#define KRAZOA_PRINT_BASE_X 160
#define KRAZOA_PRINT_BASE_Y 195
#define KRAZOA_PRINT_KERNING 18

#define ASCII_A 0x41
#define ASCII_Z 0x5A

typedef struct {
/*00*/ s8 stateIndex;
/*01*/ s8 stringLoaded;
/*02*/ s8 glyphsLoaded;
/*03*/ char text[KRAZOA_STRING_MAX_LENGTH + 1];
/*44*/ Texture* glyphs[KRAZOA_ALPHABET_LENGTH];
/*AC*/ u32 sound;
} KTextState;

typedef struct {
 /*00*/ ObjSetup base;
 /*18*/ s16 unused18;
 /*1A*/ s16 stringID;
} KText_Setup;

/*0x0*/ static u8 currentGlyph[0x10];

static void krazoatext_load_required_glyph_textures(Object* self, KTextState* state);
static void krazoatext_unload_all_glyph_textures(Object* self, KTextState* state);

// offset: 0x0 | ctor
void krazoatext_ctor(void* dll){
}

// offset: 0xC | dtor
void krazoatext_dtor(void* dll){
}

// offset: 0x18 | func: 0 | export: 0
void krazoatext_setup(Object* self, s32 arg1, s32 arg2) {  
    obj_init_mesg_queue(self, 2);
    krazoatext_unload_all_glyph_textures(self, self->state);
    self->unk0xb0 |= 0x2000;
}

// offset: 0x88 | func: 1 | export: 1
void krazoatext_control(Object* self) {
    KTextState* state;
    KText_Setup* setup;
    u32 message;
    GameTextChunk* gametext;
    u16 length;

    state = self->state;
    setup = ((KText_Setup*)self->setup);

    if (state->stringLoaded == FALSE) {
        gametext = gDLL_21_Gametext->vtbl->get_chunk(1);
        length = strlen(gametext->strings[setup->stringID]) + 1;
        if (length > KRAZOA_STRING_MAX_LENGTH) {
            length = KRAZOA_STRING_MAX_LENGTH;
            gametext->strings[setup->stringID][KRAZOA_STRING_MAX_LENGTH - 1] = 0;
        }
        bcopy(gametext->strings[setup->stringID], &state->text, length);
        state->text[KRAZOA_STRING_MAX_LENGTH - 1] = 0;
        mmFree(gametext);
        state->stringLoaded = TRUE;
    }

    while (obj_recv_mesg(self, &message, 0, 0)) {
    }

    if (state->stateIndex != 0) {
        if (!(self->unk0xaf & 4) || (get_masked_button_presses(0) & B_BUTTON)) {
            state->stateIndex = 0;
        }
    } else if (self->unk0xaf & 1) {
        state->stateIndex = 1;
        if (state->sound == 0) {
            gDLL_6_AMSFX->vtbl->play_sound(self, 0x286, 0x7F, (u32*)&state->sound, 0, 0, 0);
        }
    }

    if (state->stateIndex && state->stringLoaded) {
        krazoatext_load_required_glyph_textures(self, state);
    } else {
        krazoatext_unload_all_glyph_textures(self, state);
    }

    if (state->sound && !gDLL_6_AMSFX->vtbl->func_B48(state->sound)) {
        gDLL_6_AMSFX->vtbl->func_A1C(state->sound);
        state->sound = NULL;
    }
}

// offset: 0x324 | func: 2 | export: 2
void krazoatext_update(Object* self){
}

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
    KTextState* state = self->state;

    krazoatext_unload_all_glyph_textures(self, state);
    if (state->sound) {
        gDLL_6_AMSFX->vtbl->func_A1C(state->sound);
    }
}

// offset: 0x414 | func: 5 | export: 5
u32 dll_283_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x424 | func: 6 | export: 6
u32 dll_283_get_state_size(Object *self, u32 a1) {
    return sizeof(KTextState);
}

// offset: 0x438 | func: 7 | export: 7
void krazoatext_print_text(Object* self, Gfx** gfx) {
    KTextState* state;
    Texture* glyph;
    s32 charCount;
    s32 glyphIndex;
    s32 xCoord;

    state = self->state;
    if (!state->glyphsLoaded)
        return;

    charCount = 0;
    while (state->text[charCount] != '\0'){
        charCount += 1;
    }

    xCoord = KRAZOA_PRINT_BASE_X - (charCount * (KRAZOA_PRINT_KERNING/2));

    charCount = 0;
    while (state->text[charCount] != '\0'){
        if ((state->text[charCount] >= ASCII_A) && (state->text[charCount] <= ASCII_Z)) {
            glyphIndex = state->text[charCount] - ASCII_A;
            if (state->glyphs[glyphIndex]) {
                ((Func_80037F9C_Struct*)currentGlyph)->unk0 = state->glyphs[glyphIndex];
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
void krazoatext_load_required_glyph_textures(Object* self, KTextState* state) {
    s32 charIndex;
    u32 glyphIndex;

    if (state->glyphsLoaded){
        return;
    }

    charIndex = 0;
    while (state->text[charIndex]){
        if (ASCII_A <= state->text[charIndex] && state->text[charIndex] <= ASCII_Z){
            glyphIndex = (u32)state->text[charIndex] - ASCII_A;
            if (state->glyphs[glyphIndex] == NULL) {
                state->glyphs[glyphIndex] = queue_load_texture_proxy(KRAZOA_ALPHABET_ICONS + glyphIndex);
            }
        }
        charIndex++;
    }
    state->glyphsLoaded = TRUE;

}

// offset: 0x61C | func: 9
void krazoatext_unload_all_glyph_textures(Object* self, KTextState* state) {
    s32 index;

    if (state->glyphsLoaded) {
        for (index = 0; index < KRAZOA_ALPHABET_LENGTH; index++){
            if (state->glyphs[index]) {
                texture_destroy(state->glyphs[index]);
            }
            state->glyphs[index] = NULL;
        }
        state->glyphsLoaded = FALSE;  
    }
}
