#include "common.h"
#include "sys/fonts.h"
#include "game/objects/interaction_arrow.h"

typedef struct {
    s32 xMin;
    s32 xMax;
    s32 yMin;
    s32 yMax;
} TextWindowCoords;

typedef struct {
    ObjSetup base;
    s16 gametextID;                      //Gametext to print
    s8 unused1A;
    u8 unk1B;                            //Assigned to `objData->unk10` in setup, but otherwise unused
    u8 yaw;
} InfoPoint_Setup;

typedef struct {
    GameTextChunk* gametext;             //Gametext to print
    char* textString;                    //First string from gametext (unused)
    TextWindowCoords* textWindowCoords;  //Bounds for text window
    s32 textCommand;                     //First command from gametext (unused)
    s8 unk10;                            //Assigned `objSetup->unk1B` in setup, but otherwise unused
    s8 unused11;
    s16 unused12;
    s16 unused14;
    s16 textOpacity;                     //Current opacity for the text (fades in if nonzero)
    s32 texAnimSpeed;                    //Animation speed for the model's texture frame blending
    s32 unused1C;
} InfoPoint_Data;

typedef enum {
    InfoPoint_SEQCMD_1_Set_Text_Opacity_Max = 1,
    InfoPoint_SEQCMD_2_Set_Text_Opacity_Zero = 2
} InfoPoint_SeqCommands;

/*0x0*/ static TextWindowCoords dTextWindowCoords[] = {
    {40, 280, 30, 200}, 
    {0, 0, 0, 0}, 
    {0, 0, 0, 0}
};

static int InfoPoint_anim_callback(Object* self, Object* objSetup, AnimObj_Data* animData, s8 arg3);
static void InfoPoint_tex_anim(Object* self, s32 arg1, s32* arg2);

// offset: 0x0 | ctor
void InfoPoint_ctor(void *dll) { }

// offset: 0xC | dtor
void InfoPoint_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void InfoPoint_setup(Object* self, InfoPoint_Setup* objSetup, s32 arg2) {
    InfoPoint_Data* objData;
    GameTextChunk* gametext;
    s32 i;

    objData = self->data;
    
    font_load(FONT_SUBTITLE_FONT);
    font_load(FONT_DINO_SUBTITLE_FONT_1);
    
    self->animCallback = InfoPoint_anim_callback;
    objData->textWindowCoords = dTextWindowCoords;
    gametext = gDLL_21_Gametext->vtbl->get_chunk(objSetup->gametextID);
    objData->textString = *gametext->strings;
    objData->textCommand = *gametext->commands;
    objData->gametext = gametext;
    
    for (i = 0; i < gametext->count; i++) {
    }
    
    self->srt.yaw = objSetup->yaw << 8;
    objData->texAnimSpeed = 2;
    objData->unk10 = objSetup->unk1B;
    objData->textOpacity = 0;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
}

// offset: 0x134 | func: 1 | export: 1
void InfoPoint_control(Object* self) {
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        joy_set_button_mask(0, A_BUTTON);
        gDLL_3_Animation->vtbl->func17(0, self, -1);
    }
}

// offset: 0x1B0 | func: 2 | export: 2
void InfoPoint_update(Object *self) { }

// offset: 0x1BC | func: 3 | export: 3
void InfoPoint_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    s32 i;
    s32 y;
    TextWindowCoords* coords;
    GameTextChunk* gametext;
    InfoPoint_Data* objData;

    objData = self->data;
    coords = objData->textWindowCoords;
    y = coords->yMin;
    
    if (visibility == 0) {
        return;
    }
        
    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    
    if (objData->textOpacity != 0) {
        InfoPoint_tex_anim(self, 1, &objData->texAnimSpeed);

        //Fade in to nearly full opacity
        if (objData->textOpacity < 200) {
            objData->textOpacity += (gUpdateRate * 20);
        }
        
        gametext = objData->gametext;
        font_window_set_coords(6, coords->xMin, coords->yMin, coords->xMax, coords->yMax);
        font_window_use_font(6, FONT_DINO_SUBTITLE_FONT_1);
        font_window_flush_strings(6);
        font_window_set_text_colour(6, 0, 0xFF, 0xFF, 0xFF, objData->textOpacity);
        font_window_draw(gdl, mtxs, vtxs, 6);
        
        //Print gametext strings
        for (i = 0; i < gametext->count; i++, y += 18) {
            font_window_add_string_xy(6, -0x8000, y, gametext->strings[i], 1, ALIGN_TOP_CENTER);
            font_window_use_font(6, FONT_DINO_SUBTITLE_FONT_1);
            font_window_set_text_colour(6, 0xFF, 0xFF, 0xFF, 0xFF, objData->textOpacity);
        }
        
        font_window_draw(gdl, mtxs, vtxs, 6);
    }
}

// offset: 0x40C | func: 4 | export: 4
void InfoPoint_free(Object* self, s32 arg1) {
    InfoPoint_Data* objData = self->data;

    font_unload(FONT_DINO_SUBTITLE_FONT_1);
    font_unload(FONT_SUBTITLE_FONT);
    mmFree(objData->gametext);
}

// offset: 0x484 | func: 5 | export: 5
u32 InfoPoint_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x494 | func: 6 | export: 6
u32 InfoPoint_get_data_size(Object *self, u32 a1) {
    return sizeof(InfoPoint_Data);
}

// offset: 0x4A8 | func: 7
int InfoPoint_anim_callback(Object* self, Object* arg1, AnimObj_Data* animData, s8 arg3) {
    InfoPoint_Data* objData;
    s32 i;

    objData = self->data;
    
    for (i = 0; i < animData->unk98; i++) {
        switch (animData->unk8E[i]) {
        case InfoPoint_SEQCMD_1_Set_Text_Opacity_Max:
            objData->textOpacity = 0xFF;
            break;
        case InfoPoint_SEQCMD_2_Set_Text_Opacity_Zero:
            objData->textOpacity = 0;
            break;
        //NOTE: no option to set partial opacity (to activate fade-in behaviour)
        }
    }
    
    return 0;
}

/*0x0*/ static const char str_0[] = " Name %s Time %i ";
/*0x14*/ static const char str_14[] = " \n";

// offset: 0x520 | func: 8
/**
  * Animates one of the model's textures, blending between frame0 and frame1.
  * (Ping-pong looping)
  */
void InfoPoint_tex_anim(Object* self, s32 animIndex, s32* animSpeed) {
    TextureAnimator* texAnim;
    s32 frameValue;

    texAnim = func_800348A0(self, animIndex - 1, 0);
    if (texAnim == NULL) {
        return;
    }
    
    diPrintf(" Anim %i ", animIndex);
    
    frameValue = texAnim->frame + (*animSpeed * gUpdateRate);

    //Reverse direction after blending up to frame1
    if (frameValue > 0x100) {
        frameValue = 0x100;
        *animSpeed = -*animSpeed;
    }
    
    //Reverse direction after blending down to frame0
    if (frameValue < 0) {
        frameValue = 0;
        *animSpeed = -*animSpeed;
    }
    
    texAnim->frame = frameValue;
}
