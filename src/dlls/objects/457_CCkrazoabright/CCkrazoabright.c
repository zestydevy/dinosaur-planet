#include "common.h"

extern Block_0x28Struct* func_8004A284(Block*, s32);
extern BlockTexture* func_8004A2CC(s32);

typedef struct {
    f32 timer;          //manages delay and blending strength for updating the Krazoa symbol
    f32 iconBlendTimer; //manages crossfading two sets of icon textures on Kyte's lever columns
    u8 updateNeeded;
    u8 state;
    u8 prevPoints[6];   //Booleans: apply colour to each point of courtyard's Krazoa symbol (clockwise from south-east point)
    u8 pointsLit[6];
} CCkrazoabright_Data;

/* Lever & point layout (minimap view)

          Levers      Krazoa symbol points

            3              3     4
            2               \   /
    <-ocean             2 ——  ♢  —— 5
            1               /   \
            0              1     0
*/

typedef enum {
    STATE_Initialise = 0,
    STATE_Lever_Puzzle_One = 1,
    STATE_Lighting_Lanterns = 2,
    STATE_Krazoa_Tablet_Quest = 3,
    STATE_Lever_Puzzle_Two = 4,
    STATE_Finished = 5
} CCkrazoabright_States;

/** Coordinates of Courtyard well's two Blocks (with Krazoa symbol) */
/*0x0*/ static Vec3f data_coords_well_blocks[] = {
    {4488, 0, 17606}, 
    {4344, 0, 17606}
};
/** Coordinates of Courtyard's two ocean-front Blocks (with lever columns) */
/*0x18*/ static Vec3f data_coords_lever_blocks[] = {
    {5506.7002, -93, 17574}, 
    {5587.5, -93, 18058}
};
/*0x30*/ static u8 data_krazoa_symbol_shape_animatorIDs[] = {
    11, 14, 15, 16, 13, 12
};
/*0x38*/ static u16 data_tablet_quest_gamebits[] = {
    BIT_CC_Placed_Fire_Crystal_Courtyard_Beacon,
    BIT_CC_Placed_Krazoa_Tablet_Two,
    BIT_CC_Placed_Fire_Crystal_Island_Beacon,
    BIT_CC_Placed_Krazoa_Tablet_Three,
    BIT_CC_Placed_Fire_Crystal_Cliff_Beacon,
    BIT_CC_Placed_Krazoa_Tablet_One
};

static void CCkrazoabright_clear_point_colours(void);                              
static void CCkrazoabright_colour_krazoa_symbol_tablet_quest(Object* self);                      
static void CCkrazoabright_handle_lever_puzzle_1(Object* self, CCkrazoabright_Data* objData);
static void CCkrazoabright_handle_lighting_lanterns(Object* self, CCkrazoabright_Data* objData);
static void CCkrazoabright_handle_krazoa_tablet_quest(Object* self, CCkrazoabright_Data* objData);
static void CCkrazoabright_handle_lever_puzzle_2(Object* self, CCkrazoabright_Data* objData);
static s32  CCkrazoabright_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3);
static void CCkrazoabright_apply_blending_krazoa_symbol(u8* colours);
static void CCkrazoabright_apply_blending_lever_icons(Object* self, CCkrazoabright_Data* objData);

// offset: 0x0 | ctor
void CCkrazoabright_ctor(void *dll) { }

// offset: 0xC | dtor
void CCkrazoabright_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void CCkrazoabright_setup(Object* self, ObjSetup *setup, s32 arg2) {
    CCkrazoabright_Data* objData;

    objData = self->data;
    objData->timer = 300.0f;
    self->animCallback = (void*)CCkrazoabright_anim_callback;
    self->unkB0 |= 0x2000;

    //Set initial state of lever column icons (blending between two sets of icons)
    if (main_get_bits(BIT_CC_Courtyard_Crossfade_Lever_Icons)) {
        objData->iconBlendTimer = 255.0f;
    } else {
        objData->iconBlendTimer = 0.0f;
    }
}

// offset: 0xB4 | func: 1 | export: 1
void CCkrazoabright_control(Object* self) {
    CCkrazoabright_Data* objData;

    objData = self->data;
    
    if (objData->state == STATE_Initialise) {
        if (main_get_bits(BIT_CC_Courtyard_Crossfade_Lever_Icons)) {
            objData->state = STATE_Lever_Puzzle_Two;
            CCkrazoabright_clear_point_colours();
        } else if (main_get_bits(BIT_CC_Courtyard_Gate_Reopened)) {
            objData->state = STATE_Krazoa_Tablet_Quest;
            CCkrazoabright_colour_krazoa_symbol_tablet_quest(self);
        } else if (main_get_bits(BIT_CC_Kyte_Pulled_All_Four_Levers)) {
            objData->state = STATE_Lighting_Lanterns;
            CCkrazoabright_colour_krazoa_symbol_tablet_quest(self);
        } else {
            objData->state = STATE_Lever_Puzzle_One;
            CCkrazoabright_clear_point_colours();
        }
    }

    switch (objData->state) {
    case STATE_Lever_Puzzle_One:
        CCkrazoabright_handle_lever_puzzle_1(self, objData);
        break;
    case STATE_Lighting_Lanterns:
        CCkrazoabright_handle_lighting_lanterns(self, objData);
        break;
    case STATE_Krazoa_Tablet_Quest:
        CCkrazoabright_handle_krazoa_tablet_quest(self, objData);
        break;
    case STATE_Lever_Puzzle_Two:
        CCkrazoabright_handle_lever_puzzle_2(self, objData);
        break;
    }

    CCkrazoabright_apply_blending_lever_icons(self, objData);
}

// offset: 0x298 | func: 2
/** All Krazoa symbol points are unlit */
void CCkrazoabright_clear_point_colours(void) {
    u8 index;
    u8 colour[6];

    for (index = 0; index < 6; index++){
        colour[index] = 0;
    }
    
    CCkrazoabright_apply_blending_krazoa_symbol((u8*)&colour);
}

// offset: 0x2F8 | func: 3
/** Handles Krazoa symbol points' colours during Krazoa Tablets/Beacon quest */
void CCkrazoabright_colour_krazoa_symbol_tablet_quest(Object* self) {
    CCkrazoabright_Data* objData;
    s8 gamebitValueInverted;
    u8 index;
    u8 colours[6];

    objData = self->data;
    
    gDLL_3_Animation->vtbl->func20(self, 0x258);
    gDLL_3_Animation->vtbl->func17(0, self, 0x78);

    for (index = 0; index < 6; index++){
        gamebitValueInverted = main_get_bits(data_tablet_quest_gamebits[index]) ^ 1;
        objData->prevPoints[index] = gamebitValueInverted;
        objData->pointsLit[index] = gamebitValueInverted;

        if (&objData->prevPoints[0]) {
            colours[index] = 0;
        } else {
            colours[index] = 0xFF;
        }
    }
    
    CCkrazoabright_apply_blending_krazoa_symbol((u8*)&colours);
}

// offset: 0x420 | func: 4
void CCkrazoabright_handle_lever_puzzle_1(Object* self, CCkrazoabright_Data* objData) {
    s32 index;

    if (objData->updateNeeded) {
        //Handle resetting levers and gamebits after each pull
        objData->timer += gUpdateRateF;
        if (objData->timer > 300.0f) {
            main_set_bits(BIT_636, 0);
            main_set_bits(BIT_CC_Courtyard_Kyte_Pulled_4th_Lever, 0);
            main_set_bits(BIT_638, 0);
            main_set_bits(BIT_CC_Courtyard_Kyte_Pulled_3rd_Lever, 0);
            main_set_bits(BIT_4D5, 0);
            main_set_bits(BIT_CC_Courtyard_Kyte_Pulled_2nd_Lever, 0);
            main_set_bits(BIT_637, 0);
            main_set_bits(BIT_CC_Courtyard_Kyte_Pulled_1st_Lever, 0);
            objData->updateNeeded = FALSE;
        }
    } else {
        //Check if Kyte pulls levers (positions noted from left, while facing towards ocean)
        if (main_get_bits(BIT_CC_Courtyard_Kyte_Pulled_4th_Lever)) {
            objData->pointsLit[5] = objData->prevPoints[5] ^ 1;
            objData->pointsLit[0] = objData->prevPoints[0] ^ 1;
            objData->pointsLit[2] = objData->prevPoints[2] ^ 1;
            objData->updateNeeded = TRUE;
        } else {
            if (main_get_bits(BIT_CC_Courtyard_Kyte_Pulled_3rd_Lever)) {
                objData->pointsLit[5] = objData->prevPoints[5] ^ 1;
                objData->pointsLit[1] = objData->prevPoints[1] ^ 1;
                objData->pointsLit[2] = objData->prevPoints[2] ^ 1;
                objData->updateNeeded = TRUE;
            }
            else if (main_get_bits(BIT_CC_Courtyard_Kyte_Pulled_2nd_Lever)) {
                objData->pointsLit[3] = objData->prevPoints[3] ^ 1;
                objData->pointsLit[4] = objData->prevPoints[4] ^ 1;
                objData->updateNeeded = TRUE;
            } else if (main_get_bits(BIT_CC_Courtyard_Kyte_Pulled_1st_Lever)) {
                objData->pointsLit[5] = objData->prevPoints[5] ^ 1;
                objData->pointsLit[2] = objData->prevPoints[2] ^ 1;
                objData->updateNeeded = TRUE;
            }
        }
        
        //Show overhead view of Krazoa symbol
        if (objData->updateNeeded) {
            for (index = 0; index < 6; index++){
                if (!objData->pointsLit[index]){ 
                    break; 
                }
            }
            
            //Check if all points of the Krazoa symbol are lit
            if (index == 6) {
                gDLL_3_Animation->vtbl->func17(0, self, -1);
                objData->state = STATE_Lighting_Lanterns;
                objData->timer = 0.0f;
                for (index = 0; index < 6; index++) { objData->pointsLit[index] = 0; }
                return;
            }

            gDLL_3_Animation->vtbl->func17(2, self, -1);
            objData->timer = 0.0f;
        }
    }
}

// offset: 0x728 | func: 5
void CCkrazoabright_handle_lighting_lanterns(Object* self, CCkrazoabright_Data* objData) {
    if (gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, 5) && 
        main_get_bits(BIT_CC_Courtyard_Gate_Left_Lever_or_Lantern) && 
        main_get_bits(BIT_CC_Courtyard_Gate_Right_Lever_or_Lantern)
    ) {
        main_set_bits(BIT_CC_Courtyard_Gate_Reopened, 1);
        objData->updateNeeded = FALSE;

        //Show courtyard gate reopening
        gDLL_3_Animation->vtbl->func17(1, self, -1);
        objData->state = STATE_Krazoa_Tablet_Quest;
    }
}

// offset: 0x808 | func: 6
void CCkrazoabright_handle_krazoa_tablet_quest(Object* self, CCkrazoabright_Data* objData) {
    u8 colours[6];
    u8 index;
    
    if (objData->updateNeeded){
        //Handle quest ending
        objData->timer += gUpdateRateF;
        if (objData->timer >= 140.0f){
            objData->timer = 300.0f;
            gDLL_3_Animation->vtbl->func17(3, self, -1);
            objData->state = STATE_Lever_Puzzle_Two; 
        } 
    } else {
        //Handle quest
        for (index = 0; index < 6; index++) {
            if (objData->prevPoints[index]){
                if (objData->pointsLit[index] && main_get_bits(data_tablet_quest_gamebits[index])){
                    objData->pointsLit[index] = 0;
                    objData->timer = 0.0f;
                }
            } else if (!objData->pointsLit[index] && !main_get_bits(data_tablet_quest_gamebits[index])){
                objData->pointsLit[index] = 1;
                objData->timer = 0.0f;
            }
            
            if (objData->prevPoints[index] != (objData->pointsLit[index])){
                objData->timer += gUpdateRateF * 5.0f;
                if (objData->timer > 255.0f){
                    objData->prevPoints[index] = objData->pointsLit[index];
                    if (objData->prevPoints[index]){
                        colours[index] = 0xFF;
                    } else {
                        colours[index] = 0;
                    }
                } else {
                    colours[index] = (s8) objData->timer;
                    if (objData->prevPoints[index]){
                        colours[index] = (0xFF - colours[index]);
                    }
                }
            } else if (objData->prevPoints[index] != 0){
                colours[index] = 0xFF;
            } else {
                colours[index] = 0;
            }
        }
        
        //Check if all the Krazoa symbol's points are lit
        for (index = 0; index < 6; index++){
            if (objData->prevPoints[index]){
                break;
            }
        }
        if (index == 6){
            objData->updateNeeded = TRUE;
            objData->timer = 51.0f;
        }
        
        CCkrazoabright_apply_blending_krazoa_symbol((u8*)&colours);
    }
}

// offset: 0xAAC | func: 7
void CCkrazoabright_handle_lever_puzzle_2(Object* self, CCkrazoabright_Data* objData) {
    s32 index;

    if (objData->updateNeeded) {
        //Handle resetting levers and gamebits after each pull
        objData->timer += gUpdateRateF;
        if (objData->timer > 300.0f) {
            main_set_bits(BIT_636, 0);
            main_set_bits(BIT_CC_Courtyard_Kyte_Pulled_4th_Lever, 0);
            main_set_bits(BIT_638, 0);
            main_set_bits(BIT_CC_Courtyard_Kyte_Pulled_3rd_Lever, 0);
            main_set_bits(BIT_4D5, 0);
            main_set_bits(BIT_CC_Courtyard_Kyte_Pulled_2nd_Lever, 0);
            main_set_bits(BIT_637, 0);
            main_set_bits(BIT_CC_Courtyard_Kyte_Pulled_1st_Lever, 0);
            objData->updateNeeded = FALSE;
        }
    } else {
        //Check if Kyte pulls levers (positions noted from left, while facing towards ocean)
        if (main_get_bits(BIT_CC_Courtyard_Kyte_Pulled_4th_Lever)) {
            objData->pointsLit[2] = objData->prevPoints[2] ^ 1;
            objData->pointsLit[3] = objData->prevPoints[3] ^ 1;
            objData->pointsLit[4] = objData->prevPoints[4] ^ 1;
            objData->updateNeeded = TRUE;
        } else if (main_get_bits(BIT_CC_Courtyard_Kyte_Pulled_3rd_Lever)) {
            objData->pointsLit[3] = objData->prevPoints[3] ^ 1;
            objData->pointsLit[4] = objData->prevPoints[4] ^ 1;
            objData->pointsLit[5] = objData->prevPoints[5] ^ 1;
            objData->updateNeeded = TRUE;
        } else if (main_get_bits(BIT_CC_Courtyard_Kyte_Pulled_2nd_Lever)) {
            objData->pointsLit[5] = objData->prevPoints[5] ^ 1;
            objData->pointsLit[0] = objData->prevPoints[0] ^ 1;
            objData->pointsLit[1] = objData->prevPoints[1] ^ 1;
            objData->pointsLit[2] = objData->prevPoints[2] ^ 1;
            objData->updateNeeded = TRUE;
        } else if (main_get_bits(BIT_CC_Courtyard_Kyte_Pulled_1st_Lever)) {
            objData->pointsLit[0] = objData->prevPoints[0] ^ 1;
            objData->pointsLit[1] = objData->prevPoints[1] ^ 1;
            objData->pointsLit[3] = objData->prevPoints[3] ^ 1;
            objData->pointsLit[4] = objData->prevPoints[4] ^ 1;
            objData->updateNeeded = TRUE;
        }

        //Show overhead view of Krazoa symbol
        if (objData->updateNeeded) {
            for (index = 0; index < 6; index++){
                if (!objData->pointsLit[index]){ 
                    break; 
                }
            }
            
            //Check if all points of the Krazoa symbol are lit
            if (index == 6) {
                gDLL_3_Animation->vtbl->func17(5, self, -1);
                objData->state = STATE_Finished;
                objData->timer = 0.0f;
                for (index = 0; index < 6; index++) { objData->pointsLit[index] = 0; }
                return;
            }

            gDLL_3_Animation->vtbl->func17(2, self, -1);
            objData->timer = 0.0f;
        }
    }
}

// offset: 0xDE4 | func: 8 | export: 2
void CCkrazoabright_update(Object *self) { }

// offset: 0xDF0 | func: 9 | export: 3
void CCkrazoabright_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) { }

// offset: 0xE08 | func: 10 | export: 4
void CCkrazoabright_free(Object *self, s32 a1) { }

// offset: 0xE18 | func: 11 | export: 5
u32 CCkrazoabright_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xE28 | func: 12 | export: 6
u32 CCkrazoabright_get_data_size(Object *self, u32 a1) {
    return sizeof(CCkrazoabright_Data);
}

// offset: 0xE3C | func: 13
s32 CCkrazoabright_anim_callback(Object* self, Object* animObj, AnimObj_Data* animObjData, s32 arg3) {
    CCkrazoabright_Data* objData = self->data;
    s32 i;
    u8 colours[6] = {0, 0, 0, 0, 0, 0};

    for (i = 0; i < animObjData->unk98; i++) {
        switch (animObjData->unk8E[i]) {
        case 1:
            for (i = 0; i < 6; i++) {
                objData->prevPoints[i] = 0;
                objData->pointsLit[i] = 1;
                objData->timer = 0.0f;
            }
            break;
        case 2:
            for (i = 0; i < 6; i++) {
                objData->prevPoints[i] = 1;
                objData->pointsLit[i] = 0;
                objData->timer = 0.0f;
            }
            break;
        }
    }
    
    //Blend in/out blue vertex colour of Krazoa symbol's points (hold on colour after 1 second)
    objData->timer += gUpdateRateF;
    if (objData->timer > 60.0f) {
        for (i = 0; i < 6; i++) {
            objData->prevPoints[i] = objData->pointsLit[i];
            if (objData->prevPoints[i]) {
                colours[i] = 0xFF;
            } else {
                colours[i] = 0;
            }
        }
    } else {
        for (i = 0; i < 6; i++) {
            if (objData->prevPoints[i] == objData->pointsLit[i]) {
                if (objData->prevPoints[i] != 0) {
                    colours[i] = 0xFF;
                } else {
                    colours[i] = 0;
                }
            } else {

                colours[i] = ((s32) objData->timer * 0xFF) / 60.0f;
                if (objData->prevPoints[i] != 0) {
                    colours[i] = 0xFF - colours[i];
                }
            }
        }
    }
    
    CCkrazoabright_apply_blending_krazoa_symbol(colours);
    CCkrazoabright_apply_blending_lever_icons(self, objData);
    return 0;
}

// offset: 0x12AC | func: 14
/** Updates the colour of each point in the courtyard's Krazoa symbol (via texture frame blending)*/
void CCkrazoabright_apply_blending_krazoa_symbol(u8* colours) {
    Block *block;
    BlockShape *shapes;
    s32 b; //block index
    s32 s; //shape index
    s32 a; //animatorID index
    
    //Iterate over the courtyard well's two Block model pieces (containing the Krazoa symbol)
    for (b = 0; b < 2; b++){
        block = func_80044BB0(func_8004454C(data_coords_well_blocks[b].x, data_coords_well_blocks[b].y, data_coords_well_blocks[b].z));
        if (block == NULL) {
            continue;
        }
        
        //Iterate over Block's shapes
        shapes = block->shapes;
        for (s = 0; s < block->shapeCount; s++){
            //If the shape has any of the 6 relevant animatorIDs, apply its new texture blend value
            for (a = 0; a < 6; a++){
                if (data_krazoa_symbol_shape_animatorIDs[a] == shapes[s].animatorID){
                    func_8004A2CC(func_8004A284(block, shapes[s].animatorID)->texIdx)->unk4 = colours[a];
                    break;
                }
            }
        }
    }
}

// offset: 0x140C | func: 15
/** Updates the icons on Kyte's lever columns (blends between two sets of textures)*/
void CCkrazoabright_apply_blending_lever_icons(Object* self, CCkrazoabright_Data* objData) {
    Block* block;
    BlockShape* shapes;
    s32 b; //block index
    s32 s; //shape index
    u8 colour;
    s32 colourToSet;

    //Fade in blue colour
    if (objData->iconBlendTimer == 0.0f) {
        if (main_get_bits(BIT_CC_Courtyard_Crossfade_Lever_Icons)) {
            objData->iconBlendTimer = 2.0f * gUpdateRateF;
        }
    } else {
        objData->iconBlendTimer += 2.0f * gUpdateRateF;
        if (objData->iconBlendTimer > 255.0f) {
            objData->iconBlendTimer = 255.0f;
        }
    }
    colour = objData->iconBlendTimer;

    //Iterate over the courtyard's two ocean-side Block models (containing the lever columns) 
    for (b = 0; b < 2; b++) {
        block = func_80044BB0(func_8004454C(data_coords_lever_blocks[b].x, data_coords_lever_blocks[b].y, data_coords_lever_blocks[b].z));
        if (block == NULL) {
            continue;
        }

        //Iterate over Block's shapes
        shapes = block->shapes;
        for (s = 0; s < block->shapeCount; s++) {
            //If the shape's animatorIDs is 11 (used by lever icons), apply new texture blend value
            if (shapes[s].animatorID == 11) {
                colourToSet = colour;
                func_8004A2CC(func_8004A284(block, shapes[s].animatorID)->texIdx)->unk4 = colourToSet;
            }
        }
    }
}

/*0x0*/ static const char str_0[] = "trying to start next seq\n";
/*0x1C*/ static const char str_1C[] = "";
