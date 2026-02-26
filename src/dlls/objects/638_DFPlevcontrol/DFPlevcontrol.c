//Official name unknown, but VFPT's equivalent was named "VFlevcontrol.c"

#include "PR/ultratypes.h"
#include "dll.h"
#include "functions.h"
#include "game/gamebits.h"
#include "sys/main.h"
#include "sys/map_enums.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/objmsg.h"
#include "dlls/engine/17_partfx.h"
#include "dlls/objects/210_player.h"

#include "dlls/objects/638_DFPlevcontrol.h"

/** Textures for electric arcing effect? */
/*0x0*/ static Texture* dTexElectricity = NULL;

/** Index of the safe floor tile on each row of electrified floor tiles */
/*0x4*/ static u8 dFloorTiles[9] = {
    1, 2, 3, 0, 0, 0, 0, 0, 0
};

/** Tracks whether extra LightAction has been applied during Visit 1 to DFPT */
/*0x10*/ static u32 dLitVisit1 = FALSE;

static int DFP_LevelControl_anim_callback(Object* self, Object *override, AnimObj_Data *aData, s8 arg3);
static void DFP_LevelControl_handle_visit_1(Object* self);
static void DFP_LevelControl_handle_visit_2(Object* self);
static void DFP_LevelControl_handle_visit_3(Object* self);

// offset: 0x0 | ctor
void DFP_LevelControl_ctor(void *dll) { }

// offset: 0xC | dtor
void DFP_LevelControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFP_LevelControl_setup(Object* self, DFPTLevelControl_Setup* objSetup, s32 arg2) {
    u8 mapSetup;

    obj_add_object_type(self, OBJTYPE_10);
    dTexElectricity = tex_load_deferred(1132);
    self->animCallback = DFP_LevelControl_anim_callback;
    gDLL_29_Gplay->vtbl->set_map_setup(self->mapID, 1);
    mapSetup = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);

    switch (mapSetup) { 
        case 0:
            func_80000860(self, self, 261, 0);
            func_80000860(self, self, 262, 0);
            func_80000860(self, self, 263, 0);
            gDLL_29_Gplay->vtbl->set_obj_group_status(
                MAP_DESERT_FORCE_POINT_TEMPLE_TOP, DFPT_ObjGroup2_Bottom_BigDoor, 1);
            break;
        case 1:
            func_80000860(self, self, 415, 0);
            main_set_bits(BIT_SpellStone_CRF, 1);
            break;
        case 2:
            func_80000860(self, self, 415, 0);
            main_set_bits(BIT_SpellStone_BWC, 1);
            main_set_bits(BIT_Spell_Grenade, 1);
            break;
        case 3:
            func_80000860(self, self, 415, 0);
            main_set_bits(BIT_SpellStone_KP, 1);
            main_set_bits(BIT_Spell_Grenade, 1);
            break;
    }
    self->unkB0 |= 0x6000;
}

// offset: 0x260 | func: 1 | export: 1
void DFP_LevelControl_control(Object* self) {
    static s16 dTimerControl = 130;
    static s16 dMagicRefillTimer = 0;
    Object *player;
    s32 mapID;
    s32 setupID;
    
    player = get_player();
    
    //Auto-refill player's magic every ~16 seconds
    dMagicRefillTimer++; //@bug: framerate dependent
    if (dMagicRefillTimer > 1000) {
        dMagicRefillTimer = 0;
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 20);
    }
    
    map_get_map_id_from_xz_ws(player->srt.transl.x, player->srt.transl.z); //@bug: unused
    setupID = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);
    
    switch (setupID) {
    case 0:
        return;
    case 1:
        if (dTimerControl != 0) {
            dTimerControl -= (s16)gUpdateRateF;
            if (dTimerControl <= 0) {
                dTimerControl = 0;
            }
        }
        DFP_LevelControl_handle_visit_1(self);
        return;
    case 2:
        //Refill magic, apply light/envFxActions, and grant Forcefield spell after ~2s
        if (dTimerControl != 0) {
            dTimerControl -= (s16)gUpdateRateF;
            if (dTimerControl <= 0) {
                dTimerControl = 0;
                func_80000860(self, self, 0x19F, 0);
                func_80000450(self, self, 0x166, 0, 0, 0);
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_78C, MAX_VOLUME, 0, 0, 0, 0);
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_793, MAX_VOLUME, 0, 0, 0, 0);
                ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 20);
                main_set_bits(BIT_Spell_Forcefield, 1);
            }
        }
        DFP_LevelControl_handle_visit_2(self);
        break;
    case 3:
        DFP_LevelControl_handle_visit_3(self);
        break;
    }
}

// offset: 0x51C | func: 2 | export: 2
void DFP_LevelControl_update(Object *self) { }

// offset: 0x528 | func: 3 | export: 3
void DFP_LevelControl_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
}

// offset: 0x540 | func: 4 | export: 4
void DFP_LevelControl_free(Object* self, s32 arg1) {
    obj_free_object_type(self, OBJTYPE_10);
    gDLL_29_Gplay->vtbl->set_obj_group_status(
        MAP_DESERT_FORCE_POINT_TEMPLE_TOP, DFPT_ObjGroup2_Bottom_BigDoor, 0);
    tex_free(dTexElectricity);
    dTexElectricity = NULL;
}

// offset: 0x5C0 | func: 5 | export: 5
u32 DFP_LevelControl_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x5D0 | func: 6 | export: 6
u32 DFP_LevelControl_get_data_size(Object *self, u32 a1) {
    return sizeof(DFPTLevelControl_Data);
}

// offset: 0x5E4 | func: 7 | export: 7
/** Copies the indices of the safe (non-electrified) floor tiles to an argument array */
void DFP_LevelControl_export_safe_floor_tiles(Object* self, u8* floorTileIndices) {
    s16 i;

    for (i = 0; i < 9; i++){
        floorTileIndices[i] = dFloorTiles[i];
    }
}

// offset: 0x62C | func: 8
static int DFP_LevelControl_anim_callback(Object* self, Object *override, AnimObj_Data *aData, s8 arg3) {
    DFPTLevelControl_Data *objdata;
    Object *player;

    objdata = self->data;
    player = get_player();

    //Create electrocution effects over player model
    if (objdata->zappedTimer > 0){
        objdata->zappedTimer -= (s16)gUpdateRateF;
        ((DLL_210_Player*)player->dll)->vtbl->func72(player, PARTICLE_51E);
    }

    return 0;
}

// offset: 0x6C4 | func: 9
void DFP_LevelControl_handle_visit_1(Object* self) {
    static u8 dInitialiseVisit1 = TRUE;
    DFPTLevelControl_Data* objData;
    s16 i;
    Object* player;

    objData = self->data;
    player = get_player();
    
    //Set up DFPT Act 1
    if (dInitialiseVisit1) {
        //Grant required Spells and refill magic
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 20);

        //Zero out all floor tile rows
        for (i = 3; i < 9; i++){ 
            dFloorTiles[i] = 0; 
        }

        //Set safe tile indices for first 3 rows
        for (i = 0; i < 3; i++){ 
            dFloorTiles[i] = rand_next(1, 4); 
        }
        
        main_set_bits(BIT_DFPT_Puzzle_Pad_Show_Solution, 0);

        //Apply lighting/envFX, and play sounds
        func_80000860(self, self, 0x19F, 0);
        func_80000450(self, self, 0x166, 0, 0, 0);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_78C, MAX_VOLUME, 0, 0, 0, 0);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_793, MAX_VOLUME, 0, 0, 0, 0);

        objData->zappedTimer = 0;
        dInitialiseVisit1 = FALSE;
    }
    
    //Apply extra lighting
    if ((dLitVisit1 == FALSE) && (main_get_bits(BIT_5E3))) {
        func_80000450(self, self, 0x27A, 0, 0, 0);
        dLitVisit1 = TRUE;
    }
    
    //Reroll safe floor tiles when player steps on switch
    if (main_get_bits(BIT_DFPT_Puzzle_Pad_Pressed) && (objData->previousPadState == FALSE)) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_96B_Magic_Whir, MAX_VOLUME, 0, 0, 0, 0);
        
        for (i = 0; i < 3; i++){ 
            dFloorTiles[i] = rand_next(1, 4); 
        }
        
        main_set_bits(BIT_DFPT_Puzzle_Pad_Show_Solution, 1);
        objData->previousPadState = TRUE;
    } else if (!main_get_bits(BIT_DFPT_Puzzle_Pad_Pressed) && (objData->previousPadState == TRUE)) {
        objData->previousPadState = FALSE;
        main_set_bits(BIT_DFPT_Puzzle_Pad_Show_Solution, 0);
    }
    
    //Zap player
    if (main_get_bits(BIT_DFPT_Zapped_by_Floor_Tiles)) {
        objData->zappedTimer = 300;
        obj_send_mesg(player, 0x60005, self, 0);
    }
    
    //Enable object groups
    if (main_get_bits(BIT_DFPT_Load_Block_Slide_Puzzle_1) && 
        !gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, DFPT_ObjGroup6_Bottom_Block_Slide_Puzzle)
    ) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, DFPT_ObjGroup6_Bottom_Block_Slide_Puzzle, 1);
    }
    
    if (main_get_bits(BIT_DFPT_Load_SpellStone_Holder_1_Interior) && 
        !gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, DFPT_ObjGroup4_Under_SpellStone_Holder_1)
    ) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, DFPT_ObjGroup4_Under_SpellStone_Holder_1, 1);
    }
}

// offset: 0xB2C | func: 10
void DFP_LevelControl_handle_visit_2(Object* self) {
    static u8 dInitialiseVisit2 = TRUE;
    DFPTLevelControl_Data* objData;
    s16 i;
    Object* player;

    objData = self->data;
    player = get_player();
    
    //Set up DFPT Act 2
    if (dInitialiseVisit2) {
        //Grant required Spells and refill magic
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 20);

        //Zero out all floor tile rows
        for (i = 6; i < 9; i++){ 
            dFloorTiles[i] = 0; 
        }

        //Set safe tile indices for first 6 rows
        for (i = 0; i < 6; i++){ 
            dFloorTiles[i] = rand_next(1, 4); 
        }
        
        main_set_bits(BIT_DFPT_Puzzle_Pad_Show_Solution, 0);
        objData->zappedTimer = 0;

        //Apply lighting/envFX, and play sounds
        func_80000860(self, self, 0x19F, 0);
        func_80000450(self, self, 0x166, 0, 0, 0);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_78C, MAX_VOLUME, 0, 0, 0, 0);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_793, MAX_VOLUME, 0, 0, 0, 0);
        
        dInitialiseVisit2 = FALSE;
    }
    
    //Reroll safe floor tiles when player steps on switch
    if (main_get_bits(BIT_DFPT_Puzzle_Pad_Pressed) && (objData->previousPadState == FALSE)) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_96B_Magic_Whir, MAX_VOLUME, 0, 0, 0, 0);
        
        for (i = 0; i < 6; i++){ 
            dFloorTiles[i] = rand_next(1, 4); 
        }
        
        main_set_bits(BIT_DFPT_Puzzle_Pad_Show_Solution, 1);
        objData->previousPadState = TRUE;
    } else if (!main_get_bits(BIT_DFPT_Puzzle_Pad_Pressed) && (objData->previousPadState == TRUE)) {
        objData->previousPadState = FALSE;
        main_set_bits(BIT_DFPT_Puzzle_Pad_Show_Solution, 0);
    }

    //Zap player
    if (main_get_bits(BIT_DFPT_Zapped_by_Floor_Tiles)) {
        objData->zappedTimer = 300;
        obj_send_mesg(player, 0x60005, self, 0);
    }
    
    //Enable object groups
    if (main_get_bits(DFPT_Load_Block_Slide_Puzzle_2) && 
        !(gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, DFPT_ObjGroup6_Bottom_Block_Slide_Puzzle))
    ) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, DFPT_ObjGroup6_Bottom_Block_Slide_Puzzle, 1);
    }
}

// offset: 0xECC | func: 11
void DFP_LevelControl_handle_visit_3(Object* self) {
    static u8 dInitialiseVisit3 = TRUE;
    DFPTLevelControl_Data* objData;
    s16 i;
    Object* player;

    objData = self->data;
    player = get_player();

    //Set up DFPT Act 3
    if (dInitialiseVisit3) {
        //Grant required Spells and refill magic
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        main_set_bits(BIT_Krazoa_Translator, 1);
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 20);

        //Set safe tile indices for all 9 rows
        for (i = 0; i < 9; i++){ 
            dFloorTiles[i] = rand_next(1, 4); 
        }
        
        main_set_bits(BIT_DFPT_Puzzle_Pad_Show_Solution, 0);
        main_set_bits(BIT_5E7, 1);
        main_set_bits(BIT_5E8, 1);
        main_set_bits(BIT_76D, 1);
        main_set_bits(BIT_792, 1);

        //Apply lighting/envFX, and play sounds
        func_80000860(self, self, 0x19F, 0);
        func_80000860(self, self, 0x1A0, 0);
        func_80000450(self, self, 0x166, 0, 0, 0);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_78C, MAX_VOLUME, 0, 0, 0, 0);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_793, MAX_VOLUME, 0, 0, 0, 0);

        objData->zappedTimer = 0;
        dInitialiseVisit3 = FALSE;
    }

    //Reroll safe floor tiles when player steps on switch
    if (main_get_bits(BIT_DFPT_Puzzle_Pad_Pressed) && (objData->previousPadState == FALSE)) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_96B_Magic_Whir, MAX_VOLUME, 0, 0, 0, 0);

        for (i = 0; i < 9; i++){ 
            dFloorTiles[i] = rand_next(1, 4); 
        }
        
        main_set_bits(BIT_DFPT_Puzzle_Pad_Show_Solution, 1);
        objData->previousPadState = TRUE;
    } else if (!main_get_bits(BIT_DFPT_Puzzle_Pad_Pressed) && (objData->previousPadState == TRUE)) {
        objData->previousPadState = FALSE;
        main_set_bits(BIT_DFPT_Puzzle_Pad_Show_Solution, 0);
    }

    //Zap player
    if (main_get_bits(BIT_DFPT_Zapped_by_Floor_Tiles)) {
        objData->zappedTimer = 300;
        obj_send_mesg(player, 0x60005, self, 0);
    }

    //Enable object groups
    if (main_get_bits(DFPT_Load_Block_Slide_Puzzle_2) && 
        !gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, DFPT_ObjGroup6_Bottom_Block_Slide_Puzzle)
    ) {
        gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, DFPT_ObjGroup6_Bottom_Block_Slide_Puzzle, 1);
    }
}
