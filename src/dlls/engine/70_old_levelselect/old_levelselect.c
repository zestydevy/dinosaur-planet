#include "dll.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/engine/73.h"
#include "macros.h"
#include "sys/fonts.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "sys/map_enums.h"

extern DLL_73* dll_throw_fault; //NOTE: BROKEN! This is a function now, not DLL 73.

#define LINE_HEIGHT 0x20
#define TOTAL_STRINGS 53
#define STRING_LENGTH 0x40
#define STRINGS_PER_PAGE 9
#define LAST_PAGE_FIRST_IDX (TOTAL_STRINGS - STRINGS_PER_PAGE)

typedef enum {
    LEVELSELECT_IDX_00_NEW_GAME,
    LEVELSELECT_IDX_01_LOAD_GAME,
    LEVELSELECT_IDX_02,
    LEVELSELECT_IDX_03_SWAPSTONE_HOLLOW,
    LEVELSELECT_IDX_04_SWAPSTONE_CIRCLE,
    LEVELSELECT_IDX_05,
    LEVELSELECT_IDX_06_DARKICE_MINES,
    LEVELSELECT_IDX_07_DARKICE_MINES_TWO,
    LEVELSELECT_IDX_08_WALLED_CITY,
    LEVELSELECT_IDX_09_DRAGON_ROCK,
    LEVELSELECT_IDX_0A_CLOUDRUNNER_FORTRESS,
    LEVELSELECT_IDX_0B_KRAZOA_PALACE,
    LEVELSELECT_IDX_0C_BLACKWATER_CANYON,
    LEVELSELECT_IDX_0D,
    LEVELSELECT_IDX_0E_NORTHERN_WASTES,
    LEVELSELECT_IDX_0F_EARTHWALKER_TEMPLE,
    LEVELSELECT_IDX_10_WILLOW_GROVE,
    LEVELSELECT_IDX_11_DIAMOND_BAY,
    LEVELSELECT_IDX_12_DISCOVERY_FALLS,
    LEVELSELECT_IDX_13_MOON_MOUNTAIN_PASS,
    LEVELSELECT_IDX_14_CAPE_CLAW,
    LEVELSELECT_IDX_15_GOLDEN_PLAINS,
    LEVELSELECT_IDX_16,
    LEVELSELECT_IDX_17_TEST_OF_COMBAT,
    LEVELSELECT_IDX_18_TEST_OF_STRENGTH,
    LEVELSELECT_IDX_19_TEST_OF_FEAR,
    LEVELSELECT_IDX_1A_TEST_OF_CHARACTER,
    LEVELSELECT_IDX_1B_TEST_OF_KNOWLEDGE,
    LEVELSELECT_IDX_1C_TEST_OF_SACRIFICE,
    LEVELSELECT_IDX_1D_TEST_OF_SKILL,
    LEVELSELECT_IDX_1E_TEST_OF_MAGIC,
    LEVELSELECT_IDX_1F,
    LEVELSELECT_IDX_20,
    LEVELSELECT_IDX_21_DARKICE_BOSS,
    LEVELSELECT_IDX_22_GENERAL_SCALES_BOSS,
    LEVELSELECT_IDX_23_BLACKWATER_BOSS,
    LEVELSELECT_IDX_24_CLOUDRUNNER_RACE,
    LEVELSELECT_IDX_25_KAMERIA_DRAGON_BOSS,
    LEVELSELECT_IDX_26_DRAKOR_FINAL_BOSS,
    LEVELSELECT_IDX_27_ICE_MOUNTAIN,
    LEVELSELECT_IDX_28,
    LEVELSELECT_IDX_29_DESERT_FORCE_POINT,
    LEVELSELECT_IDX_2A_VOLCANO_FORCE_POINT,
    LEVELSELECT_IDX_2B,
    LEVELSELECT_IDX_2C_EARTHWALKER_ACT_TWO, 
    LEVELSELECT_IDX_2D_ENERGY_DEMO,
    LEVELSELECT_IDX_2E, 
    LEVELSELECT_IDX_2F_WARLOCK_ACT_ONE,
    LEVELSELECT_IDX_30_WARLOCK_ACT_TWO,
    LEVELSELECT_IDX_31_WARLOCK_ACT_THREE,
    LEVELSELECT_IDX_32_WARLOCK_ACT_FOUR,
    LEVELSELECT_IDX_33_WARLOCK_ACT_FIVE,
    LEVELSELECT_IDX_34_WARLOCK_ACT_SIX
} LevelSelectOld_Indices;

/*0x0*/ static char dStrings[TOTAL_STRINGS][STRING_LENGTH] = {
    "NEW GAME",
    "LOAD GAME",
    "",
    "SWAPSTONE HOLLOW",
    "SWAPSTONE CIRCLE",
    "",
    "DARKICE MINES",
    "DARKICE MINES TWO",
    "WALLED CITY",
    "DRAGON ROCK",
    "CLOUDRUNNER FORTRESS",
    "KRAZOA PALACE",
    "BLACKWATER CANYON",
    "",
    "NORTHERN WASTES",
    "EARTHWALKER TEMPLE",
    "WILLOW GROVE",
    "DIAMOND BAY",
    "DISCOVERY FALLS",
    "MOON MOUNTAIN PASS",
    "CAPE CLAW",
    "GOLDEN PLAINS",
    "",
    "TEST OF COMBAT",
    "TEST OF STRENGTH",
    "TEST OF FEAR",
    "TEST OF CHARACTER",
    "TEST OF KNOWLEDGE",
    "TEST OF SACRIFICE",
    "TEST OF SKILL",
    "TEST OF MAGIC",
    "",
    "",
    "DARKICE BOSS",
    "GENERAL SCALES BOSS",
    "BLACKWATER BOSS",
    "CLOUDRUNNER RACE",
    "KAMERIA DRAGON BOSS",
    "DRAKOR FINAL BOSS",
    "ICE MOUNTAIN",
    "",
    "DESERT FORCE POINT",
    "VOLCANO FORCE POINT",
    "",
    "EARTHWALKER ACT TWO", 
    "ENERGY DEMO",
    "", 
    "WARLOCK ACT ONE",
    "WARLOCK ACT TWO",
    "WARLOCK ACT THREE",
    "WARLOCK ACT FOUR",
    "WARLOCK ACT FIVE",
    "WARLOCK ACT SIX"
};

static void old_levelselect_start(MapIDs mapID, s32 act, PlayerNo playerNo);

// offset: 0x0 | ctor
void old_levelselect_ctor(void *dll) { }

// offset: 0xC | dtor
void old_levelselect_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 old_levelselect_update1(void) {
    return 0;
}

// offset: 0x24 | func: 1 | export: 1
void old_levelselect_update2(void) {

}

// offset: 0x2C | func: 2 | export: 2
void old_levelselect_draw(Gfx** gdl, Mtx** mtx, Vertex** vtx) {
    /*0xD40*/ static s32 dSelectedPrintIdx = 0;         //Selected index within 9 strings currently shown
    /*0xD44*/ static s32 dSelectedListIdx = 0;          //Selected index within full list of strings
    /*0xD48*/ static s32 dShowLevelUnavailable = FALSE; //Shows a "LEVEL NOT AVAILABLE" message instead of the level list
    /*0x000*/ static s32 sButtonsEnabled;
  
    s32 start;
    s32 menuAction;
    s32 i;
    char *string;
    s32 prevSelectedPrintIdx;

    func_80014508(20);
    gDLL_20_Screens->vtbl->show_screen(2); //NOTE: this screen is missing - maybe it contained the old Dinosaur Planet logo?
    
    //Display either the Level Select list, or a "LEVEL NOT AVAILABLE" message
    if (dShowLevelUnavailable == FALSE) {
        dll_throw_fault->vtbl->init_text_window(120);
    
        //Print 9 strings at a time
        if (dSelectedListIdx < 4) { 
            //Upper half of first page of strings
            for (i = 0, string = dStrings[0]; i < STRINGS_PER_PAGE; i++, string += STRING_LENGTH) {
                dll_throw_fault->vtbl->add_string(i, string, LINE_HEIGHT, dSelectedPrintIdx);
                
                if (gDLL_20_Screens){} //fake
            }
        } else if (dSelectedListIdx >= (TOTAL_STRINGS - 4)) { 
            //Lower half of last page of strings
            for (i = LAST_PAGE_FIRST_IDX, string = dStrings[LAST_PAGE_FIRST_IDX]; i < TOTAL_STRINGS; i++, string += STRING_LENGTH) {
                dll_throw_fault->vtbl->add_string(i, string, LINE_HEIGHT, dSelectedPrintIdx);
            }
        } else {
            //Scrolling between top/bottom of list
            start = dSelectedListIdx - 4;
            i = start;
            if (start < (dSelectedListIdx + 5)) {
                string = dStrings[start];
                do {
                    dll_throw_fault->vtbl->add_string(i, string, LINE_HEIGHT, dSelectedPrintIdx);
                    i++;
                    string += STRING_LENGTH;
                } while (i < (dSelectedListIdx + 5));
            }
        }
        
        //Handle moving up/down the list
        {
            prevSelectedPrintIdx = dSelectedPrintIdx;
            menuAction = dll_throw_fault->vtbl->handle_joystick_and_buttons(&dSelectedPrintIdx);
            
            if ((prevSelectedPrintIdx == 0) && (dSelectedPrintIdx == (STRINGS_PER_PAGE - 1))) {
                //Wrapping from top to bottom
                dSelectedListIdx = TOTAL_STRINGS - 1;
            } else if ((prevSelectedPrintIdx == (STRINGS_PER_PAGE - 1)) && (dSelectedPrintIdx == 0)) {
                //Wrapping from bottom to top
                dSelectedListIdx = 0;
            } else {
                //Moving up/down (starting on nearest string that isn't blank)
                if (prevSelectedPrintIdx > dSelectedPrintIdx) {             
                    do {
                        dSelectedListIdx--;
                    } while (dStrings[dSelectedListIdx][0] == '\0');
                } else if (prevSelectedPrintIdx < dSelectedPrintIdx) {
                    do {
                        dSelectedListIdx++;
                    } while (dStrings[dSelectedListIdx][0] == '\0');
                }
                
                //Determine which string is selected (out of the 9 shown)
                if (dSelectedListIdx < 4) {
                    dSelectedPrintIdx = dSelectedListIdx;
                } else if (dSelectedListIdx > (TOTAL_STRINGS - 5)) {
                    dSelectedPrintIdx = dSelectedListIdx - (TOTAL_STRINGS - STRINGS_PER_PAGE);
                } else {
                    dSelectedPrintIdx = 4;
                }
            }
        }

        //Handle selecting an item
        if (menuAction >= 0) {
            switch (dSelectedListIdx) {
            case LEVELSELECT_IDX_00_NEW_GAME:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->init_save(0, NULL);
                return;
            case LEVELSELECT_IDX_01_LOAD_GAME:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(0, 1);
                return;
            case LEVELSELECT_IDX_02: //NOTE: blank string
                old_levelselect_start(MAP_FRONT_END, 0, PLAYER_KRYSTAL);
                return;
            case LEVELSELECT_IDX_03_SWAPSTONE_HOLLOW:
                old_levelselect_start(MAP_SWAPSTONE_HOLLOW, 0, PLAYER_SABRE);
                return;
            case LEVELSELECT_IDX_04_SWAPSTONE_CIRCLE:
                old_levelselect_start(MAP_SWAPSTONE_CIRCLE, 1, PLAYER_KRYSTAL);
                return;
            case LEVELSELECT_IDX_06_DARKICE_MINES:
                old_levelselect_start(MAP_DARK_ICE_MINES_1, 0, PLAYER_SABRE);
                return;
            case LEVELSELECT_IDX_07_DARKICE_MINES_TWO:
                old_levelselect_start(MAP_ANIMTEST, 0, PLAYER_SABRE); //DIM2 may have been moved down by one index, after Animtest?
                return;
            case LEVELSELECT_IDX_08_WALLED_CITY:
                dShowLevelUnavailable = TRUE;
                break;
            case LEVELSELECT_IDX_09_DRAGON_ROCK:
                dShowLevelUnavailable = TRUE; //Wasn't created at this stage?
                break;
            case LEVELSELECT_IDX_0A_CLOUDRUNNER_FORTRESS:
                old_levelselect_start(MAP_CLOUDRUNNER_FORTRESS, 0, PLAYER_KRYSTAL);
                return;
            case LEVELSELECT_IDX_0B_KRAZOA_PALACE:
                dShowLevelUnavailable = TRUE;  //Wasn't created at this stage?
                break;
            case LEVELSELECT_IDX_0C_BLACKWATER_CANYON:
                dShowLevelUnavailable = TRUE;  //Wasn't created at this stage?
                break;
            case LEVELSELECT_IDX_0E_NORTHERN_WASTES:
                old_levelselect_start(MAP_SNOWHORN_WASTES, 0, PLAYER_SABRE);
                return;
            case LEVELSELECT_IDX_0F_EARTHWALKER_TEMPLE:
                old_levelselect_start(MAP_EARTHWALKER_TEMPLE, 1, PLAYER_SABRE);
                return;
            case LEVELSELECT_IDX_10_WILLOW_GROVE:
                old_levelselect_start(MAP_WILLOW_GROVE, 0, PLAYER_KRYSTAL); //Starting as Krystal by mistake?
                return;
            case LEVELSELECT_IDX_11_DIAMOND_BAY:
                old_levelselect_start(MAP_DIAMOND_BAY, 0, PLAYER_SABRE);
                return;
            case LEVELSELECT_IDX_12_DISCOVERY_FALLS:
                old_levelselect_start(MAP_DISCOVERY_FALLS, 0, PLAYER_KRYSTAL);
                return;
            case LEVELSELECT_IDX_13_MOON_MOUNTAIN_PASS:
                old_levelselect_start(MAP_MOON_MOUNTAIN_PASS, 0, PLAYER_KRYSTAL);
                return;
            case LEVELSELECT_IDX_14_CAPE_CLAW:
                old_levelselect_start(MAP_CAPE_CLAW, 0, PLAYER_KRYSTAL);
                return;
            case LEVELSELECT_IDX_15_GOLDEN_PLAINS:
                old_levelselect_start(MAP_GOLDEN_PLAINS, 0, PLAYER_KRYSTAL);
                return;
            case LEVELSELECT_IDX_17_TEST_OF_COMBAT:
                old_levelselect_start(MAP_SHRINE_DISCOVERY_FALLS, 0, PLAYER_KRYSTAL);
                return;
            case LEVELSELECT_IDX_18_TEST_OF_STRENGTH:
                old_levelselect_start(MAP_SHRINE_DIAMOND_BAY, 0, PLAYER_SABRE);
                return;
            case LEVELSELECT_IDX_19_TEST_OF_FEAR:
                old_levelselect_start(MAP_SHRINE_MOON_MOUNTAIN_PASS, 0, PLAYER_KRYSTAL);
                return;
            case LEVELSELECT_IDX_1A_TEST_OF_CHARACTER:
                old_levelselect_start(MAP_SHRINE_CAPE_CLAW, 0, PLAYER_KRYSTAL);
                return;
            case LEVELSELECT_IDX_1B_TEST_OF_KNOWLEDGE:
                old_levelselect_start(MAP_SHRINE_GOLDEN_PLAINS, 0, PLAYER_KRYSTAL);
                return;
            case LEVELSELECT_IDX_1C_TEST_OF_SACRIFICE:
                old_levelselect_start(MAP_SHRINE_SNOWHORN_WASTES, 0, PLAYER_SABRE);
                return;
            case LEVELSELECT_IDX_1D_TEST_OF_SKILL:
                old_levelselect_start(MAP_SHRINE_WALLED_CITY, 0, PLAYER_SABRE);
                return;
            case LEVELSELECT_IDX_1E_TEST_OF_MAGIC:
                old_levelselect_start(MAP_SHRINE_WILLOW_GROVE, 0, PLAYER_SABRE);
                return;
            case LEVELSELECT_IDX_21_DARKICE_BOSS:
                old_levelselect_start(MAP_BOSS_GALADON, 0, PLAYER_KRYSTAL); //Starting as Krystal, just like in the old trailer!
                return;
            case LEVELSELECT_IDX_22_GENERAL_SCALES_BOSS:
                dShowLevelUnavailable = TRUE;
                break;
            case LEVELSELECT_IDX_23_BLACKWATER_BOSS:
                dShowLevelUnavailable = TRUE;
                break;
            case LEVELSELECT_IDX_24_CLOUDRUNNER_RACE:
                old_levelselect_start(MAP_CLOUDRUNNER_RACETRACK, 0, PLAYER_KRYSTAL); //CRF Trap Rooms scrapped by this stage?
                return;
            case LEVELSELECT_IDX_25_KAMERIA_DRAGON_BOSS:
                dShowLevelUnavailable = TRUE; //Wasn't created at this stage?
                break;
            case LEVELSELECT_IDX_26_DRAKOR_FINAL_BOSS:
                old_levelselect_start(MAP_BOSS_DRAKOR, 0, PLAYER_SABRE);
                break;
            case LEVELSELECT_IDX_27_ICE_MOUNTAIN:
                old_levelselect_start(MAP_ICE_MOUNTAIN_1, 0, PLAYER_SABRE);
                return;
            case LEVELSELECT_IDX_29_DESERT_FORCE_POINT:
                old_levelselect_start(MAP_DESERT_FORCE_POINT_TEMPLE_BOTTOM, 0, PLAYER_KRYSTAL);
                return;
            case LEVELSELECT_IDX_2A_VOLCANO_FORCE_POINT:
                old_levelselect_start(MAP_VOLCANO_FORCE_POINT_TEMPLE, 0, PLAYER_KRYSTAL); //Starting as Krystal by mistake?
                return;
            case LEVELSELECT_IDX_2C_EARTHWALKER_ACT_TWO:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(8, 1);
                return;
            case LEVELSELECT_IDX_2D_ENERGY_DEMO:
                old_levelselect_start(MAP_EARTHWALKER_TEMPLE, 5, PLAYER_KRYSTAL);
                return;
            case LEVELSELECT_IDX_2F_WARLOCK_ACT_ONE:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                
                //NOTE: crashes due to out-of-bounds save index? They must've had some extra debug/demo saves at some stage!
                //All the following entries crash for the same reason, too.
                gDLL_29_Gplay->vtbl->load_save(10, 1);
                return;
            case LEVELSELECT_IDX_30_WARLOCK_ACT_TWO:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(11, 1);
                return;
            case LEVELSELECT_IDX_31_WARLOCK_ACT_THREE:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(12, 1);
                return;
            case LEVELSELECT_IDX_32_WARLOCK_ACT_FOUR:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(13, 1);
                return;
            case LEVELSELECT_IDX_33_WARLOCK_ACT_FIVE:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(14, 1);
                return;
            case LEVELSELECT_IDX_34_WARLOCK_ACT_SIX:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(15, 1);
                return;
            default:
                dShowLevelUnavailable = TRUE;
                break;
            case LEVELSELECT_IDX_05:
            case LEVELSELECT_IDX_0D:
            case LEVELSELECT_IDX_16:
            case LEVELSELECT_IDX_1F:
            case LEVELSELECT_IDX_20:
            case LEVELSELECT_IDX_28:
            case LEVELSELECT_IDX_2B:
            case LEVELSELECT_IDX_2E:
                break;
            }
        }
    } else {
        //Display message if option/level can't be accessed
        sButtonsEnabled = FALSE;
        dll_throw_fault->vtbl->init_text_window(180);
        dll_throw_fault->vtbl->add_string(0, "LEVEL NOT AVAILABLE", 12, 0);
        if (dll_throw_fault->vtbl->handle_joystick_and_buttons(&sButtonsEnabled) >= 0) {
            dShowLevelUnavailable = FALSE;
        }
    }

    font_window_draw(gdl, 0, 0, 1);
}

// offset: 0xB58 | func: 3
/**
  * Starts the game with a specific mapID, Act number, and player character.
  */
void old_levelselect_start(MapIDs mapID, s32 act, PlayerNo playerNo) {
    gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
    gDLL_29_Gplay->vtbl->init_save(0, NULL);
    main_change_map(mapID, act, playerNo, MENU_GAMEPLAY);
}
