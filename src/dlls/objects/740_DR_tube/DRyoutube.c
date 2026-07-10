/* 
    "DRyoutube.c" seems to have been the official name, based on Kiosk strings (really)!

    "A walkthrough video a day keeps Dragon Rock at bay," says Dr. Youtube, MD.
*/

#include "common.h"

/* Gamebits enabling texture animation at three stages along each tube:
    - root
    - middle
    - top
 */
/*0x0*/ static s16 dAnimGamebits[3][3] = {
    {BIT_DR_Anim_Tube1_Section1, BIT_DR_Anim_Tube1_Section2, BIT_DR_Anim_Tube1_Section3}, //Tube at the CloudRunner prison side of the rock
    {BIT_DR_Anim_Tube2_Section1, BIT_DR_Anim_Tube2_Section2, BIT_DR_Anim_Tube2_Section3}, //Tube at the HighTop prison side of the rock
    {BIT_DR_Anim_Tube3_Section1, BIT_DR_Anim_Tube3_Section2, BIT_DR_Anim_Tube3_Section3}, //Tube at the quarry side of the rock
};

// Gamebits for the top section of the three DRtubes
/*0x14*/ static s16 dTubeTopGamebits[] = {
    BIT_DR_Anim_Tube1_Section3, BIT_DR_Anim_Tube2_Section3, BIT_DR_Anim_Tube3_Section3
};

// Tracks whether all three tubes have been activated
/*0x0*/ static u8 sAllTubesActivated;

#define TEX_ANIM_ON_START 0x100
#define TEX_ANIM_ON_LENGTH 0x600
#define TEX_ANIM_ON_END (TEX_ANIM_ON_START + TEX_ANIM_ON_LENGTH)
#define TEX_ANIM_ON_SPEED 0x10

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 tubeIndex;           //The index of the tube instance (0: at CloudRunner side, 1: at HighTop side, 2: at quarry side)
} DR_Tube_Setup;

typedef struct {
    s16* animGamebits;      //An array of the 3 gamebits that store the activation state of the tube's sections
    u32 soundHandle;        //Controls the eerie alarm/thrumming sound loops
    u8 sectionsBitfield;    //A bitfield of the activation state of the tube's three sections (DR_Tube_Sections)
    u8 tubeIndex : 4;       //The index of the tube instance (0: at CloudRunner side, 1: at HighTop side, 2: at quarry side)
} DR_Tube_Data;

typedef enum {
    DR_Tube_SECTION_Base    = 0x1,
    DR_Tube_SECTION_Middle  = 0x2,
    DR_Tube_SECTION_Top     = 0x4
} DR_Tube_Sections;

#define DR_Tube_ALL_SECTIONS (DR_Tube_SECTION_Base | DR_Tube_SECTION_Middle | DR_Tube_SECTION_Top)

static void DR_Tube_animate_all_sections(Object* self, u8 sectionsBitfield);
static void DR_Tube_section_anim_on(Object* self, s32 sectionIndex);
static void DR_Tube_section_anim_off(Object* self, s32 sectionIndex);
static s32 DR_Tube_get_sections_bitfield(s16* gamebits);

// offset: 0x0 | ctor
void DR_Tube_ctor(void *dll) { }

// offset: 0xC | dtor
void DR_Tube_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DR_Tube_setup(Object* self, DR_Tube_Setup* objSetup, s32 arg2) {
    DR_Tube_Data* objData;
    TextureAnimator* texAnim;
    s32 i;

    objData = self->data;
    
    objData->tubeIndex = objSetup->tubeIndex;
    
    objData->animGamebits = &dAnimGamebits[objData->tubeIndex][0];
    objData->sectionsBitfield = DR_Tube_get_sections_bitfield(objData->animGamebits);
    self->srt.yaw = objSetup->yaw << 8;

    for (i = 0; i < 3; i++) {
        texAnim = func_800348A0(self, i, 0);
        if (texAnim != NULL) {
            texAnim->frame = 0;
        }
    }
    
    sAllTubesActivated = mainGetBits(BIT_DR_Tubes_All_Activated);

    if (sAllTubesActivated) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_8DC_Ominous_Thrumming_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
    }
}

// offset: 0x15C | func: 1 | export: 1
void DR_Tube_control(Object* self) {
    DR_Tube_Data* objData;

    objData = self->data;
    
    //Check if all 3 tubes are powered on
    if (sAllTubesActivated) {
        //Play pulsing model animation
        if (self->curModAnimId != 0) {
            func_80023D30(self, 0, 0.0f, 0);
        }
        func_80024108(self, 0.01f, gUpdateRateF, NULL);

        //Continue animating tube's section textures
        DR_Tube_animate_all_sections(self, DR_Tube_ALL_SECTIONS);
    } else {
        //Play an eerie alarm loop when the tube is partly activated
        if (objData->sectionsBitfield && (objData->soundHandle == 0)) {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_8DB_Eerie_Alarm_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
        }

        //Animate the tube's section textures
        objData->sectionsBitfield = DR_Tube_get_sections_bitfield(objData->animGamebits);
        DR_Tube_animate_all_sections(self, objData->sectionsBitfield);
    }
    
    //Detect when all three tubes are powered on
    if ((objData->tubeIndex == 0) && (sAllTubesActivated == FALSE) && 
        (DR_Tube_get_sections_bitfield(dTubeTopGamebits) >= DR_Tube_ALL_SECTIONS)
    ) {
        mainSetBits(BIT_DR_Tubes_All_Activated, TRUE);
        sAllTubesActivated = TRUE;

        //Stop the eerie alarm loop
        if (objData->soundHandle != 0) {
            gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
        }

        //Start the thrumming sound loop
        gDLL_6_AMSFX->vtbl->play(self, SOUND_8DC_Ominous_Thrumming_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
    }
}

// offset: 0x364 | func: 2 | export: 2
void DR_Tube_update(Object *self) { }

// offset: 0x370 | func: 3 | export: 3
void DR_Tube_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x3C4 | func: 4 | export: 4
void DR_Tube_free(Object *self, s32 onlySelf) { }

// offset: 0x3D4 | func: 5 | export: 5
u32 DR_Tube_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x3E4 | func: 6 | export: 6
u32 DR_Tube_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DR_Tube_Data);
}

// offset: 0x3F8 | func: 7
void DR_Tube_animate_all_sections(Object* self, u8 sectionsBitfield) {
    s32 i;

    if (!sectionsBitfield) {
        return;
    }
    
    for (i = 0; i < 3; i++) {
        if (sectionsBitfield & (1 << i)) {
            DR_Tube_section_anim_on(self, i);
        } else {
            DR_Tube_section_anim_off(self, i);
        }
    }
}

// offset: 0x4B8 | func: 8
/** 
  * Advances the texture animation of one of the tube's three sections, when powered on.
  */
void DR_Tube_section_anim_on(Object* self, s32 sectionIndex) {
    TextureAnimator* texAnim;
    s32 frameValue;

    texAnim = func_800348A0(self, sectionIndex, 0);
    if (texAnim != NULL) {
        diPrintf(" Anim %i ", sectionIndex);
        frameValue = texAnim->frame + (gUpdateRate * TEX_ANIM_ON_SPEED);
        if (frameValue > TEX_ANIM_ON_END) {
            frameValue -= TEX_ANIM_ON_LENGTH;
        }
        texAnim->frame = frameValue;
    }
}

// offset: 0x548 | func: 9
/** 
  * Resets the texture animation of one of the tube's three sections, returning it to its powered off state. 
  */
void DR_Tube_section_anim_off(Object* self, s32 sectionIndex) {
    TextureAnimator* texAnim;

    texAnim = func_800348A0(self, sectionIndex, 0);
    if (texAnim != NULL) {
        texAnim->frame = 0;
    }
}

// offset: 0x58C | func: 10
/**
  * Returns the activation state of the tube's three sections, as a bitfield.
  *
  * The activation state of each section is queried using its gamebit.
  */
s32 DR_Tube_get_sections_bitfield(s16* gamebits) {
    u8 bitfield;
    u8 i;

    for (bitfield = 0, i = 0; i < 3; i++, gamebits++) {
        bitfield |= mainGetBits(*gamebits) << i;
    }
    
    return bitfield;
}
