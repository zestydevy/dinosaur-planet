#include "PR/ultratypes.h"
#include "dll.h"
#include "PR/gbi.h"
#include "dlls/objects/214_animobj.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/main.h"
#include "functions.h"
#include "sys/gfx/model.h"
#include "segment_334F0.h"
#include "sys/print.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    s16 gamebitRise;      //plays sequence of pillar rising
    s16 gamebitRaised;    //set when pillar rising sequence has played
    s16 unk1C;            //sequence-related, value of 0x96 on each pillar instance 
    s8 sequenceIndex;     //sequence index to play (always index 0 on instances)
    u8 yaw;
    u8 unk20;             //used as arg2 when playing sequence (values for the 3 pillar instances: 0xFF, 0, 1)
    s16 gamebitDoorOpen;  //GP_PillarDoor already open, but seems to be intended for playing Seq 0x39A
} GP_ShrinePillar_Setup;

typedef struct {
    u8 state;
    u8 startSequence;   //used by the control func to invoke sequence, letting animCallback func take over
    f32 cooledTimer;    //time until pillar heats up after using Ice Blast Spell
} GP_ShrinePillar_Data;

typedef enum {
    STATE_Underground = 0,               //under sand
    STATE_Rising = 1,                    //emerging from sand
    STATE_Raised = 6,                    //enters this state at end of pillar rise sequence
    STATE_Waiting_for_Door_Open = 7,     //likely waiting for GP_PillarDoor to open via sequence, but advances immediately
    STATE_Hot = 2,                       //waiting for player to cool pillar with Ice Blast Spell
    STATE_Fade_Texture_to_Cooled = 4,    //blending texture to iced-over stone
    STATE_Cooled = 3,                    //timer counts down until pillar returns to hot state
    STATE_Fade_Texture_to_Hot = 5,       //time ran out, blending back to hot stone
    STATE_Finished = 8                   //unused, but presumably for when Shrine appears (3 pillars cooled)
} GP_ShrinePillar_States;

static int GP_ShrinePillar_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 a3);

// offset: 0x0 | ctor
void GP_ShrinePillar_ctor(void *dll) { }

// offset: 0xC | dtor
void GP_ShrinePillar_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void GP_ShrinePillar_setup(Object* self, GP_ShrinePillar_Setup* setup, s32 arg2) {
    TextureAnimator* animTexture;
    GP_ShrinePillar_Data* objdata;

    objdata = (GP_ShrinePillar_Data*)self->data;
    if (setup->gamebitRaised != NO_GAMEBIT) {
        if (main_get_bits(setup->gamebitRaised)) {
            objdata->state = STATE_Raised;
        }
    } else {
        objdata->state = STATE_Underground;
    }

    objdata->startSequence = TRUE;
    self->srt.yaw = setup->yaw << 8;
    self->animCallback = GP_ShrinePillar_anim_callback;

    //Set pillar's animated stone texture to frame 0
    animTexture = func_800348A0(self, 0, 0);
    if (animTexture != NULL) {
        animTexture->frame = 0;
    }
}

// offset: 0xE0 | func: 1 | export: 1
void GP_ShrinePillar_control(Object* self) {
    GP_ShrinePillar_Data* objdata;
    GP_ShrinePillar_Setup* setup;
    s32 seqArg2;

    objdata = (GP_ShrinePillar_Data*)self->data;
    setup = (GP_ShrinePillar_Setup*)self->setup;

    diPrintf("control\n");
    if (objdata->startSequence) {
        if ((setup->unk1C != 0) && (objdata->state != STATE_Underground)) {
            seqArg2 = setup->unk20;
            gDLL_3_Animation->vtbl->func20(self, setup->unk1C);
        } else {
            seqArg2 = -1; 
        }

        if (setup->sequenceIndex != -1) {
            gDLL_3_Animation->vtbl->func17(setup->sequenceIndex, self, seqArg2);
        }
        objdata->startSequence = FALSE;
    }
}

static const char str_1[] = " PREMPT %i seqtime %i \n\n";

// offset: 0x1D0 | func: 2 | export: 2
void GP_ShrinePillar_update(Object *self) { }

// offset: 0x1DC | func: 3 | export: 3
void GP_ShrinePillar_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x230 | func: 4 | export: 4
void GP_ShrinePillar_free(Object *self, s32 a1) { }

// offset: 0x240 | func: 5 | export: 5
u32 GP_ShrinePillar_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x250 | func: 6 | export: 6
u32 GP_ShrinePillar_get_data_size(Object *self, u32 a1) {
    return sizeof(GP_ShrinePillar_Data);
}

// offset: 0x264 | func: 7
int GP_ShrinePillar_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s8 a3) {
    GP_ShrinePillar_Setup* setup;
    GP_ShrinePillar_Data* objdata;
    TextureAnimator* animatedTexture1;
    TextureAnimator* animatedTexture2;
    s32 opacity;
    s32 index;
    s32 pad;

    objdata = (GP_ShrinePillar_Data*)self->data;
    setup = (GP_ShrinePillar_Setup*)self->setup;

    diPrintf("override %d\n", objdata->state);

    switch (objdata->state) {
    case STATE_Underground:
        //Waiting for gamebit (set when leaving each act of Desert Force Point Temple)
        if (main_get_bits(setup->gamebitRise)) {
            objdata->state = STATE_Rising;
        }
        break;
    case STATE_Rising:
        //Waiting for pillar rising sequence to call subcommand
        for (index = 0; index < animObjData->unk98; index++) {
            if (animObjData->unk8E[index] == 1) {
                objdata->state = STATE_Raised;
                if (setup->gamebitRaised != NO_GAMEBIT) {
                    main_set_bits(setup->gamebitRaised, 1);
                }
            }
        }
        break;
    case STATE_Raised:
        //Waiting for door opening gamebit to be set
        if (main_get_bits(setup->gamebitDoorOpen)) {
            objdata->state = STATE_Waiting_for_Door_Open;
        }
        break;
    case STATE_Waiting_for_Door_Open:
        //Waiting for door opening sequence to call subcommand
        for (index = 0; index < animObjData->unk98; index++) {
            if (animObjData->unk8E[index] == 2) {
                objdata->state = STATE_Hot;
            }
        }
        break;
    case STATE_Hot:
        //Waiting for Ice Blast Spell to be used on pillar
        if (func_80025F40(self, NULL, NULL, NULL) == 0x19) {
            objdata->state = STATE_Fade_Texture_to_Cooled;
        }
        break;
    case STATE_Cooled:
        //Counting down until returning to hot state
        objdata->cooledTimer -= gUpdateRateF;
        if (objdata->cooledTimer <= 0.0f) {
            objdata->state = STATE_Fade_Texture_to_Hot;
        }
        break;
    case STATE_Fade_Texture_to_Cooled:
        //Fading animated texture from hot stone frame to iced-over stone frame
        animatedTexture1 = func_800348A0(self, 0, 0);
        if (animatedTexture1 != NULL) {
            opacity = animatedTexture1->frame + (gUpdateRate * 8);
            if (opacity > 0x100) {
                opacity = 0x100;
                objdata->state = STATE_Cooled;
                objdata->cooledTimer = 800.0f;
            }
            animatedTexture1->frame = opacity;
        }
        break;
    case STATE_Fade_Texture_to_Hot:
        //Fading texture from hot stone frame to iced-over stone frame
        animatedTexture2 = func_800348A0(self, 0, 0);
        if (animatedTexture2 != NULL) {
            opacity = animatedTexture2->frame - (gUpdateRate * 8);
            if (opacity < 0) {
                opacity = 0;
                objdata->state = STATE_Hot;
            }
            animatedTexture2->frame = opacity;
        }
        break;
    }

    if (objdata->state == STATE_Rising ||
        objdata->state == STATE_Waiting_for_Door_Open || 
        objdata->state == STATE_Finished
    ){
        return 0;
    } else {
        return 1;
    }
}
