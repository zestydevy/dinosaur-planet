#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/21_gametext.h"
#include "game/objects/object.h"
#include "sys/fonts.h"
#include "sys/main.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    s16 flagID;
    s16 unk1A;
    s16 unk1C;
    s16 textID; //1E
    u8 activationRadius;
} LevelName_Setup;

typedef struct {
/*00*/ GameTextChunk* gametext;
/*04*/ char* strings;
/*08*/ u32 displayDuration;
/*0C*/ u8 activationRadius;
/*0D*/ s8 unusedD;
/*0E*/ s16 flagID;
/*10*/ s16 timer;
/*12*/ s16 opacity;
/*14*/ u8 state;
/*15*/ u8 pad[3];
} LevelName_Data;

typedef struct {
    s8 unk0[0x8E - 0];
    u8 unk8E[0x98 - 0x8E];
    u8 unk98; //count for unk8E
} Object0xbcCallbackStruct;

enum LevelNameStates{
    LEVELNAME_STATE_0_WAITING = 0,
    LEVELNAME_STATE_1_FADING_IN = 1,
    LEVELNAME_STATE_2_HOLDING = 2,
    LEVELNAME_STATE_3_FADING_OUT = 3,
    LEVELNAME_STATE_4_FINISHED = 4
};

//Unused textureIDs?
/*0x0*/ static u16 _data_0[] = {
    0x4449, 
    0x414d, 
    0x4f4e, 
    0x4420, 
    0x4241, 
    0x5900, 
    0x01ef, 
    0x01f0, 
    0x01f1,
    0x01f2,
    0x01f3, 
    0x01f4, 
    0x01f5, 
    0x01f6
};

static s32 levelname_anim_callback(Object* arg0, s32 arg1, Object0xbcCallbackStruct* arg2, s32 arg3);

// offset: 0x0 | ctor
void levelname_ctor(void* dll){
}

// offset: 0xC | dtor
void levelname_dtor(void* dll){
}

// offset: 0x18 | func: 0 | export: 0
void levelname_setup(Object* self, LevelName_Setup* setup, s32 arg2) {
    LevelName_Data* objdata;
    GameTextChunk* gametext;

    objdata = self->data;

    font_load(FONT_DINO_MEDIUM_FONT_IN);
    self->unk0xbc = (void*)&levelname_anim_callback;

    gametext = gDLL_21_Gametext->vtbl->get_chunk(setup->textID);

    objdata->strings = gametext->strings[0];
    objdata->displayDuration = gametext->commands[0];
    objdata->gametext = gametext;
    objdata->activationRadius = setup->activationRadius;
    objdata->flagID = setup->flagID;
    objdata->opacity = 0;

    objdata->state = LEVELNAME_STATE_0_WAITING;
    objdata->timer = objdata->opacity;

    if (objdata->flagID != -1) {
        if (main_get_bits(objdata->flagID)) {
            objdata->state = LEVELNAME_STATE_4_FINISHED;
        }
    }
    self->unk0xb0 |= 0x2000;
}

// offset: 0x120 | func: 1 | export: 1
void levelname_control(Object* self) {
    LevelName_Data* objdata;
    f32 distance;

    objdata = self->data;

    switch (objdata->state) {
        case LEVELNAME_STATE_0_WAITING:
            distance = vec3_distance(&self->positionMirror, &get_player()->positionMirror);
            if (distance < objdata->activationRadius) {
                if (objdata->flagID != -1) {
                    main_set_bits(objdata->flagID, 1);
                }
                objdata->state = 1;
            }
            break;
        case LEVELNAME_STATE_1_FADING_IN:
            objdata->opacity += delayByte * 4;
            if (objdata->opacity > 220) {
                objdata->opacity = 220;
                objdata->state = 2;
            }
            break;
        case LEVELNAME_STATE_2_HOLDING:
            objdata->timer += delayByte;
            if ((u32)objdata->timer > objdata->displayDuration) {
                objdata->state = 3;
            }    
            objdata->opacity = (s32) (fsin16_precise(objdata->timer * 0x500) * 30.0f) + 0xDC;
            break;
        case LEVELNAME_STATE_3_FADING_OUT:
            objdata->opacity -= delayByte * 4;
            if (objdata->opacity < 0) {
                objdata->opacity = 0;
                objdata->state = 4;
            }
            break;
        case LEVELNAME_STATE_4_FINISHED:
            break;
    }
}

// offset: 0x304 | func: 2 | export: 2
void levelname_update(Object* self){
}

// offset: 0x310 | func: 3 | export: 3
void levelname_print(Object* self, Gfx** gfx, Mtx** mtx, Vtx** vtx) {
    s32 index;
    s32 yCoord;
    LevelName_Data* objdata;
    GameTextChunk* gametext;

    yCoord = 80;
    objdata = self->data;
    if (objdata->opacity == 0)
        return;

    gametext = objdata->gametext;
    font_window_set_coords(6, 0, 0, 320, 240);
    font_window_use_font(6, FONT_DINO_MEDIUM_FONT_IN);
    font_window_flush_strings(6);
    font_window_set_text_colour(6, 
        0xFF, 0xFF, 0xFF, 0, objdata->opacity);

    for (index = 0; index < gametext->count; index++, yCoord += 0x1E){
        font_window_add_string_xy(6, -0x8000, yCoord, 
            gametext->strings[index], 1, 4);
        font_window_draw(gfx, (void*)mtx, (void*)vtx, 6);
    }
}

// offset: 0x480 | func: 4 | export: 4
void levelname_free(Object* self, s32 arg1) {
    LevelName_Data* objdata = self->data;

    font_unload(FONT_DINO_MEDIUM_FONT_IN);
    mmFree(objdata->gametext);
}

// offset: 0x4E4 | func: 5 | export: 5
u32 levelname_get_model_flags(Object* self){
    return MODFLAGS_NONE;
}

// offset: 0x4F4 | func: 6 | export: 6
u32 levelname_get_data_size(Object* self, s32 arg1){
    return sizeof(LevelName_Data);
}

// offset: 0x508 | func: 7
s32 levelname_anim_callback(Object* arg0, s32 arg1, Object0xbcCallbackStruct* arg2, s32 arg3) {
    LevelName_Data* objdata;
    s32 i;

    objdata = arg0->data;

    for (i = 0; i < arg2->unk98; i++){
        if (arg2->unk8E[i] == 1) {
            if (objdata->flagID != -1) {
                main_set_bits(objdata->flagID, 1);
            }
            objdata->state = LEVELNAME_STATE_1_FADING_IN;
            return 4;
        } 
    }

    return 0;
}
