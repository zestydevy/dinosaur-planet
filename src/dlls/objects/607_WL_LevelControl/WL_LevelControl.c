#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/519_SC_levelcontrol.h"
#include "dlls/objects/common/foodbag.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/map_enums.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "sys/objprint.h"
#include "sys/segment_53F00.h"
#include "sys/segment_1050.h"
#include "sys/segment_1460.h"
#include "dll.h"
#include "types.h"
#include "sys/rand.h"

#include "dlls/objects/607_WL_LevelControl.h"

static void WL_LevelControl_setup1_tick(Object* self);
static void WL_LevelControl_setup2_tick(Object* self);
static void WL_LevelControl_setup3_tick(Object* self);
static void WL_LevelControl_setup4_tick(Object* self);
static void WL_LevelControl_setup5_tick(Object* self);
static void WL_LevelControl_setup6_tick(Object* self);
static void WL_LevelControl_setup7_tick(Object* self);

// offset: 0x0 | ctor
void WL_LevelControl_ctor(void *dll) { }

// offset: 0xC | dtor
void WL_LevelControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_LevelControl_setup(Object* self, ObjSetup* setup, s32 arg2) {
    WL_LevelControl_Data* objData;

    obj_add_object_type(self, OBJTYPE_10);
    objData = self->data;

    objData->galleonIsLoaded = FALSE;
    objData->timer = 30;

    switch (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID)) {
    case 0:
        break;
    case WM_Setup1_Krystal_Meeting_Randorn:
        gDLL_29_Gplay->vtbl->set_map_setup(MAP_SWAPSTONE_CIRCLE, SC_Act1_Meeting_Rubble_Autoswap);
        gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_SWAPSTONE_CIRCLE, SC_ObjGroup0_Main_SwapStone_Area, 1);
        break;
    case WM_Setup2_Spirit1_Krystal_DF:
        break;
    case WM_Setup3_Spirit2_Sabre_DB:
        break;
    case WM_Setup4_Spirit3_Krystal_MMP:
        func_80000860(self, self, 0xA5, 0);
        func_80000860(self, self, 0xA6, 0);
        objData->enemiesDefeated = -1;
        break;
    case WM_Setup5_Spirit4_Sabre_WC:
        func_80000860(self, self, 0xE4, 0);
        func_80000860(self, self, 0xE5, 0);
        break;
    case WM_Setup6_Spirit5_6_Krystal_CC_Sabre_WG:
        func_80000860(self, self, 0xA5, 0);
        func_80000860(self, self, 0xA6, 0);
        main_set_bits(BIT_Krystal_Foodbag_M, 1);
        break;
    case WM_Setup7_Spirit7_8_Krystal_GP_Sabre_SW:
        func_80000860(self, self, 0xE4, 0);
        func_80000860(self, self, 0xE5, 0);
        objData->intervalBehaviourTimer = 700;
        objData->interval = 30;
        objData->timer = objData->interval;
        break;
    default:
        break;
    }
}

// offset: 0x25C | func: 1 | export: 1
void WL_LevelControl_control(Object* self) {
    switch (gDLL_29_Gplay->vtbl->get_map_setup(self->mapID)) {
    case 0:
        break;
    case WM_Setup1_Krystal_Meeting_Randorn:
        WL_LevelControl_setup1_tick(self);
        break;
    case WM_Setup2_Spirit1_Krystal_DF:
        WL_LevelControl_setup2_tick(self);
        break;
    case WM_Setup3_Spirit2_Sabre_DB:
        WL_LevelControl_setup3_tick(self);
        break;
    case WM_Setup4_Spirit3_Krystal_MMP:
        WL_LevelControl_setup4_tick(self);
        break;
    case WM_Setup5_Spirit4_Sabre_WC:
        WL_LevelControl_setup5_tick(self);
        break;
    case WM_Setup6_Spirit5_6_Krystal_CC_Sabre_WG:
        WL_LevelControl_setup6_tick(self);
        break;
    case WM_Setup7_Spirit7_8_Krystal_GP_Sabre_SW:
        WL_LevelControl_setup7_tick(self);
        break;
    }
}

// offset: 0x394 | func: 2 | export: 2
void WL_LevelControl_update(Object *self) { }

// offset: 0x3A0 | func: 3 | export: 3
void WL_LevelControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x3F4 | func: 4 | export: 4
void WL_LevelControl_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_10);
    if (self){} // @fake
}

// offset: 0x43C | func: 5 | export: 5
u32 WL_LevelControl_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x44C | func: 6 | export: 6
u32 WL_LevelControl_get_data_size(Object *self, u32 a1) {
    return sizeof(WL_LevelControl_Data);
}

// offset: 0x460 | func: 7
static void WL_LevelControl_load_galleon_if_needed(Object* self, WL_LevelControl_Data* objData) {
    Object** objs;
    s32 count;
    s32 i;
    s32 galleonNotFound;

    //Return early if Krystal already saw the Galleon leave with Kyte
    if (main_get_bits(BIT_Play_Seq_00EF_Scales_Escapes_With_Kyte)) {
        return;
    }

    //Try to find the Galleon
    objs = obj_get_all_of_type(OBJTYPE_MOBILE_MAP, &count);
    for (galleonNotFound = TRUE, i = 0; i < count; i++) {
        if ((objs[i]->id == OBJ_WL_Galleon) || (objs[i]->id == OBJ_SB_Galleon)) {
            galleonNotFound = FALSE;
            //@bug: continues iterating
        }
    }

    //If the Galleon couldn't be found, load it
    if (galleonNotFound) {
        if (gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, WM_ObjGroup1_Galleon) == 0) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, WM_ObjGroup1_Galleon, 1);
        }
        main_set_bits(BIT_WL_Load_Unload_Galleon, 1);
        objData->galleonIsLoaded = TRUE;
        objData->flags |= WL_LevelControl_FLAG_Galleon_Docked;
    }
}

// offset: 0x648 | func: 8
static void WL_LevelControl_unload_galleon_if_needed(Object* self, WL_LevelControl_Data* objData) {
    u32 krystalIsLeaving;
    s32 count;
    Object** objs;
    s32 i;

    //Check a gamebit that's set as Krystal leaves via the cave corridor after meeting with Randorn
    krystalIsLeaving = main_get_bits(BIT_WM_Near_Cave_Exit_Krystal_Side);

    //Unload the Galleon if its load gamebit is false
    if ((objData->flags & WL_LevelControl_FLAG_Galleon_Docked) && 
        (main_get_bits(BIT_WL_Load_Unload_Galleon) == FALSE)
    ) {
        if (gDLL_29_Gplay->vtbl->get_obj_group_status(self->mapID, WM_ObjGroup1_Galleon)) {
            gDLL_29_Gplay->vtbl->set_obj_group_status(self->mapID, WM_ObjGroup1_Galleon, 0);
        }
        objData->flags &= ~WL_LevelControl_FLAG_Galleon_Docked;
    }

    //Search for the Galleon once (@bug?: does nothing with this)
    if (((objData->flags & WL_LevelControl_FLAG_Galleon_Leaving_Search) == FALSE) && krystalIsLeaving) {
        objs = obj_get_all_of_type(OBJTYPE_MOBILE_MAP, &count);
        for (i = 0; i < count; i++) {
            if (objs[i]->id == OBJ_WL_Galleon) {
                i = count; //index break
            }
        }

        objData->flags |= WL_LevelControl_FLAG_Galleon_Leaving_Search;
    }
}

// offset: 0x7A4 | func: 9
/** 
  * For Krystal's 1st visit, meeting Randorn for the first time etc.
  */
static void WL_LevelControl_setup1_tick(Object* self) {
    /*0x0*/ static u8 dSetup1KrystalSwitchesHit = 0;

    WL_LevelControl_Data* objData;
    u8 switchesHit;

    objData = self->data;

    //Count how many of the switches have been hit in Krystal's laser corridor
    switchesHit = 0;
    switchesHit += main_get_bits(BIT_WM_Force_Field_1_Disabled);
    switchesHit += main_get_bits(BIT_WM_Force_Field_2_Disabled);
    switchesHit += main_get_bits(BIT_WM_Force_Field_3_Disabled);
    if ((switchesHit == 3) && (dSetup1KrystalSwitchesHit != 3)) {
        gDLL_6_AMSFX->vtbl->play(self, SOUND_B89_Puzzle_Solved, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
    dSetup1KrystalSwitchesHit = switchesHit;

    //Handle the Galleon
    switch (objData->galleonIsLoaded) {
        case TRUE:
            if (objData->setup == WM_Setup1_Krystal_Meeting_Randorn) {
                WL_LevelControl_unload_galleon_if_needed(self, objData);
            }
            break;
        case FALSE:
            objData->setup = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);
            switch (objData->setup) {
            case WM_Setup1_Krystal_Meeting_Randorn:
                WL_LevelControl_load_galleon_if_needed(self, objData);
                break;
            case WM_Setup2_Spirit1_Krystal_DF:
                break;
            }
            break;
    }
}

// offset: 0x940 | func: 10
/** 
  * For Krystal's 2nd visit, depositing Spirit 1 (Discovery Falls)
  */
static void WL_LevelControl_setup2_tick(Object* self) {
    static u8 dInitSpirit1Visit = TRUE;
    
    Object* player;
    Object* foodbag;

    player = get_player();

    //Set up the visit (only runs once)
    if (dInitSpirit1Visit && (main_get_bits(BIT_Play_Seq_0180_Release_Spirit_1) == FALSE)) {
        //Ensure the player has relevant spells
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);

        player = get_player(); //@bug: already assigned

        main_set_bits(BIT_FC, 1);

        //Restore some magic and make sure the player has the 1st Spirit
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 20);
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, SPIRIT_INDEX(1), TRUE);

        //Use envFxActions
        func_80000860(self, self, 0x204, 0);
        func_80000860(self, self, 0x205, 0);
        func_80000860(self, self, 0x206, 0);

        dInitSpirit1Visit = FALSE;
    }

    //If Krystal has the Medium Food Bag and it doesn't have any green apples, add 10 of them (@debug code related to Randorn?)
    if (main_get_bits(BIT_Krystal_Foodbag_M)) {
        foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 15);
        if (foodbag && (main_get_bits(BIT_Green_Apple_Count) == 0)) {
            ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
            ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
            ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
            ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
            ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
            ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
            ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
            ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
            ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
            ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
        }
    }
}

// offset: 0xC50 | func: 11
/** 
  * For Sabre's 1st visit, depositing Spirit 2 (Diamond Bay)
  */
static void WL_LevelControl_setup3_tick(Object* self) {
    static u8 dInitSpirit2Visit = TRUE;
    
    Object* player;

    //Set up the visit (only runs once)
    if ((main_get_bits(BIT_Spirit_2_Release_Sabre) == FALSE) & dInitSpirit2Visit) {
        //Use envFxActions
        func_80000860(self, self, 0xE4, 0);
        func_80000860(self, self, 0xE5, 0);
        func_80000450(self, self, 0x275, 0, 0, 0);
        func_80000450(self, self, 0x278, 0, 0, 0);

        //Ensure the player has relevant spells
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);

        //Restore some magic and make sure the player has the 2nd Spirit
        player = get_player();
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, SPIRIT_INDEX(2), TRUE);
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 20);

        dInitSpirit2Visit = FALSE;
    } else if (dInitSpirit2Visit) {
        //Use envFxActions
        func_80000860(self, self, 0xE4, 0);
        func_80000860(self, self, 0xE5, 0);
        func_80000450(self, self, 0x275, 0, 0, 0);
        dInitSpirit2Visit = FALSE;
    }
}

// offset: 0xE34 | func: 12
/** 
  * For Krystal's 3rd visit, depositing Spirit 3 (Moon Mountain Pass)
  */
static void WL_LevelControl_setup4_tick(Object* self) {
    /*0xC*/ static u8 dInitSpirit3Visit = TRUE;
    /*0x10*/ static s32 dUseGradualEnvFx = -1;

    WL_LevelControl_Data* objData;
    Object* player;
    s16 useGradualEnvFx;
    s16 enemiesDefeated = 0;
    u8 i;

    objData = self->data;

    //Set up the visit (only runs once)
    if (dInitSpirit3Visit && (main_get_bits(BIT_317) == FALSE)) {
        //Ensure the player has relevant spells, and set other gamebit
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        main_set_bits(BIT_Set_During_Spirit_Release_1, 1);

        //Restore some magic and make sure the player has the 3rd Spirit
        player = get_player();
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, SPIRIT_INDEX(3), TRUE);
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 20);

        dInitSpirit3Visit = FALSE;
    }

    /* Check 6 gamebits
    
       NOTE: The 1st queried gamebit is set when defeating the SharpClaw in 
       Sabre's equivalent of Randorn's hall, so the base gamebitID may be a mistake
       here given that this function runs during one of Krystal's visits.

       The loop may be intended to count how many Skeetlas have been defeated by Krystal,
       since they do start appearing during this trip! There are also exactly 6 of them
       positioned in Randorn's room, which might be relevant.
    */
    for (i = 0; i < 6; i++) {
        enemiesDefeated += main_get_bits(i + BIT_WM_Killed_SharpClaw_2573);
    }

    //Use different envFxActions based on how many enemies (likely Skeetlas?) were defeated
    //(Dim the player and reintroduce fog as value increases, making the room gradually brighter)
    useGradualEnvFx = main_get_bits(BIT_2C4);
    if (useGradualEnvFx && (enemiesDefeated != objData->enemiesDefeated)) {
        objData->enemiesDefeated = enemiesDefeated;
        func_80000860(self, self, (objData->enemiesDefeated + 0xF4), 0);
        func_80000450(self, self, ((objData->enemiesDefeated / 2) + 0x13C), 0, 0, 0);
    } else if (useGradualEnvFx != dUseGradualEnvFx) {
        if (useGradualEnvFx) {
            func_80000860(self, self, (objData->enemiesDefeated + 0xF4), 0);
            func_80000450(self, self, ((objData->enemiesDefeated / 2) + 0x13C), 0, 0, 0);
        } else {
            func_80000860(self, self, 0xFB, 0);
            func_80000450(self, self, 0x144, 0, 0, 0);
        }
    }

    //Store previous state, so envFxActions are only called when BIT_2C4 or enemiesDefeated changes
    dUseGradualEnvFx = useGradualEnvFx;
}

// offset: 0x1118 | func: 13
/** 
  * For Sabre's 2nd visit, depositing Spirit 4 (Walled City)
  */
static void WL_LevelControl_setup5_tick(Object* self) {
    /*0x14*/ static u8 dInitSpirit4Visit = TRUE;

    WL_LevelControl_Data* objData;
    f32 distance;
    Object* guardClaw;
    Object** objects;
    s32 count;
    s16 i;
    s16 lastUsedSpell;
    ObjSetup *someObjsetup;
    Object* player;
    
    count = 0;
    distance = 10000.0f;
    player = get_player();
    objData = self->data;

    //Set up the visit (only runs once)
    if (dInitSpirit4Visit && (main_get_bits(BIT_318) == FALSE)) {
        //Ensure the player has relevant spells
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);
        main_set_bits(BIT_Spell_Illusion, 1);

        //Restore some magic and make sure the player has the 4th Spirit
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, SPIRIT_INDEX(4), TRUE);
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 20);

        main_set_bits(BIT_WM_Setup5_Sabre_Dock_Pushed_Crate_Onto_GuardClaw, 0);

        dInitSpirit4Visit = FALSE;
    }

    //Disable HITS line (TO-DO: find where this line is)
    if (main_get_bits(BIT_2DB)) {
        func_80059038(0x18, 0, 0);
    }

    //Delete the dock's GuardClaw after dropping a crate from above
    if (main_get_bits(BIT_WM_Setup5_Sabre_Dock_Pushed_Crate_Onto_GuardClaw)) {
        main_set_bits(BIT_CFExplodeTunnel_Trigger_31B6F, 1);
        main_set_bits(BIT_WM_Setup5_Sabre_Dock_Pushed_Crate_Onto_GuardClaw, 0);

        guardClaw = obj_get_nearest_type_to(OBJTYPE_4, self, &distance);
        if (guardClaw != NULL) {
            //@bug: may potentially delete a Skeetla instead, since they're also objType4
            obj_destroy_object(guardClaw);
        }

        objData->timer = 30;
    }

    //Search through the objects, and delete the hall's SharpClaw and GuardClaw
    if (main_get_bits(BIT_WM_Setup5_Sabre_Hall_Delete_Claws)) {
        objects = obj_get_all_of_type(OBJTYPE_4, &count);
        for (i = 0; i < count; i++) {
            someObjsetup = objects[i]->setup;
            if ((someObjsetup->uID == 0x296E) ||    //SharpClaw
                (someObjsetup->uID == 0x296F)       //GuardClaw
            ) {
                obj_destroy_object(objects[i]);
            }
        }
        main_set_bits(BIT_WM_Setup5_Sabre_Hall_Delete_Claws, 0);
    }

    //Handle Sabre entering the hall with the GuardClaw
    if (main_get_bits(BIT_WM_Setup5_Sabre_Entered_GuardClaw_Hall)) {
        lastUsedSpell = ((DLL_210_Player*)player->dll)->vtbl->func50(player);

        //Warp the player away if they're not using the Illusion or Forcefield Spells
        if ((lastUsedSpell != BIT_Spell_Illusion) && 
            (lastUsedSpell != BIT_Spell_Forcefield) && 
            (main_get_bits(BIT_WM_Setup5_Sabre_Hall_Disable_GuardClaw_Warp) == FALSE)
        ) {
            warpPlayer(WARP_WM_SABRE_KRAZOA_CORRIDOR, /*fadeToBlack=*/FALSE);
        }

        main_set_bits(BIT_WM_Setup5_Sabre_Entered_GuardClaw_Hall, 0);
    }

    /* Handle removing the GuardClaw hall's warp-away behaviour (and deleting the SharpClaw)

       NOTE: BIT_2FA intended to be set upon depositing Spirit 4?
             Doesn't seem to get set in practice.
    */
    if (main_get_bits(BIT_WM_Setup5_Sabre_Hall_GuardClaw_Gone)) {
        if (main_get_bits(BIT_WM_Setup5_Sabre_Hall_Delete_Claws) == 0) {
            main_set_bits(BIT_WM_Setup5_Sabre_Hall_Delete_Claws, 1);
        }

        objData->timer -= (s16)gUpdateRate;
        if (objData->timer <= 0) {
            objData->timer = 0;
            main_set_bits(BIT_WM_Setup5_Sabre_Hall_GuardClaw_Gone, 0);
            main_set_bits(BIT_WM_Setup5_Sabre_Hall_Disable_GuardClaw_Warp, 1);
            objData->timer = 30;
        }
    }
}

// offset: 0x14FC | func: 14
/** 
  * Seems to be for Sabre's 3rd visit, depositing Spirit 6 (Willow Grove),
  * but the foodbag code suggests Krystal's 4th visit, depositing Spirit 5 (Cape Claw)
  *
  * Given that setup7 seems to be the final setup (handling Spirits 7 and 8), 
  * maybe setup6 is also meant to handle two visits: Spirits 5 and 6.
  */
static void WL_LevelControl_setup6_tick(Object* self) {
    /*0x18*/ static u8 dInitSpirit6Visit = TRUE;

    Object* player;
    Object* foodbag;

    player = get_player();

    //Set up the visit (only runs once)
    if (dInitSpirit6Visit && (main_get_bits(BIT_Play_Seq_020D) == FALSE)) {
        //Add 10 green apples, and 1 red and brown apple to the food bag (@debug code related to Randorn?)
        foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 15);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->set_capacity(foodbag);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Green_Apple);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Red_Apple);
        ((DLL_IFoodbag*)foodbag->dll)->vtbl->collect_food(foodbag, FOOD_Brown_Apple);

        //Ensure the player has relevant spells
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);

        //Restore some magic and make sure the player has the 6th Spirit
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, SPIRIT_INDEX(6), TRUE);

        dInitSpirit6Visit = FALSE;
    }
}

// offset: 0x1788 | func: 15
/** 
  * For Krystal's 5th visit, depositing Spirit 7 (Test of Knowledge)
  *
  * May also be for Sabre's 4th visit, depositing Spirit 8 (Test of Sacrifice)
  */
static void WL_LevelControl_setup7_tick(Object* self) {
    /*0x1C*/ static u8 dInitSpirit7Visit = TRUE;

    WL_LevelControl_Data* objData;
    Object* player;

    get_player();
    objData = (WL_LevelControl_Data*)self->data;

    //Set up the visit (only runs once)
    if (dInitSpirit7Visit && (main_get_bits(BIT_Play_Seq_020D) == FALSE)) {
        //Ensure the player has relevant spells
        main_set_bits(BIT_Spell_Projectile, 1);
        main_set_bits(BIT_Spell_Forcefield, 1);

        //Restore some magic and make sure the player has the 7th Spirit
        player = get_player();
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, SPIRIT_INDEX(7), TRUE);
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 20);

        dInitSpirit7Visit = FALSE;

        objData->timer = 1;

        //Use envFxActions
        func_80000860(self, self, 0x32, 0);
        func_80000860(self, self, 0x33, 0);
        
        main_set_bits(BIT_221, 1);
    }

    if (main_get_bits(BIT_WM_Setup5_Interval_Behaviour)) {
        /* Over 11.666 seconds, set BIT_36D at rapid intervals:
           starting with period of 0.5s, and getting one frame more frequent each time.

           TO-DO: what's this gamebit used for?
        */
        if (objData->intervalBehaviourTimer > 0) {
            objData->intervalBehaviourTimer -= (s16)gUpdateRate;

            if (objData->timer != 0) {
                objData->timer -= (s16)gUpdateRate;
                if (objData->timer <= 0) {
                    main_set_bits(BIT_36D, 1);
                    if (objData->interval > 10) {
                        objData->interval -= 1;
                    }
                    objData->timer = objData->interval;
                }
            }
        }
    }

    //Open/close the door to Randorn's hall erratically
    //NOTE: Randorn's door isn't set up to appear in setup 7, so this behaviour can't be seen
    if (rand_next(0, 30) == 0) {
        main_set_bits(BIT_WM_Randorn_Door_OpenClose, 1);
    }
    if (rand_next(0, 10) == 0) {
        main_set_bits(BIT_WM_Randorn_Door_OpenClose, 0);
    }
}
