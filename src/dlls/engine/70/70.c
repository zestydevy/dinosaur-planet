#include "dll.h"
#include "dlls/engine/73.h"
#include "sys/fonts.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "sys/map_enums.h"

extern DLL_73* dll_throw_fault; //NOTE: BROKEN! This is a function now, not DLL 73.

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

/*0x0*/ static char data_0[][64] = {
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
// };
// /*0xB00*/ static char data_B00[][64] = {
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

static void dll_70_func_B58(s32 mapID, s32 act, s32 playerNo);

// offset: 0x0 | ctor
void dll_70_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_70_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
s32 dll_70_func_18(void) {
    return 0;
}

// offset: 0x24 | func: 1 | export: 1
void dll_70_func_24(void) {

}

// offset: 0x2C | func: 2 | export: 2
void dll_70_func_2C(Gfx** gdl, Mtx** mtx, Vertex** vtx) {
    /*0xD40*/ static s32 data_D40 = 0;
    /*0xD44*/ static s32 data_D44 = 0; //selected textID?
    /*0xD48*/ static s32 data_D48 = FALSE;
    /*0x000*/ static s32 bss_0;
    
    s32 start;
    s32 menuAction;
    s32 i;
    char *string;
    s32 prevSelectedIdx;
    s32 var_a0;
    s32 end;

    func_80014508(20);
    gDLL_20_Screens->vtbl->show_screen(2);
    
    if (data_D48 == FALSE) {
        dll_throw_fault->vtbl->init_text_window(120);
        
        if (data_D44 < 4) { //first page of strings
            for (i = 0, string = data_0[0]; i < 9; i++, string += 0x40) {
                dll_throw_fault->vtbl->add_string(i, string, 0x20, data_D40);
                
                if (gDLL_20_Screens){} //fake
            }
        } else {
            start = data_D44 - 4;
            if ((start + 4) > 48) { //last page of strings
                for (i = 44, string = data_0[44]; i < 53; i++, string += 0x40) {
                    dll_throw_fault->vtbl->add_string(i, string, 0x20, data_D40);
                }
            } else {
                i = start;
                if (start < (data_D44 + 5)) {
                    string = data_0[start];
                    do {
                        dll_throw_fault->vtbl->add_string(i, string, 0x20, data_D40);
                        i += 1;
                        string += 0x40;
                    } while (i < (data_D44 + 5));
                }
                
            }
        }
        
        prevSelectedIdx = data_D40;
        menuAction = dll_throw_fault->vtbl->handle_joystick_and_buttons(&data_D40);
        
        if ((prevSelectedIdx == 0) && (data_D40 == 8)) {
            data_D44 = 52;
        } else if ((prevSelectedIdx == 8) && (data_D40 == 0)) {
            data_D44 = 0;
        } else {
            if (data_D40 < prevSelectedIdx) {             
                do {
                    data_D44--;
                } while (data_0[data_D44][0] == 0);
            } else if (prevSelectedIdx < data_D40) {
                do {
                    data_D44++;
                } while (data_0[data_D44][0] == 0);
            }
            
            if (data_D44 < 4) {
                data_D40 = data_D44;
            } else {
                data_D40 = (data_D44 >= 0x31) ? (data_D44 - 0x2C) : 4;
            }
        }
        
        if (menuAction >= 0) {
            switch (data_D44) {
            case LEVELSELECT_IDX_00_NEW_GAME:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->init_save(0, NULL);
                return;
            case LEVELSELECT_IDX_01_LOAD_GAME:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(0, 1);
                return;
            case 0x2:
                dll_70_func_B58(MAP_FRONT_END, 0, 1);
                return;
            case LEVELSELECT_IDX_03_SWAPSTONE_HOLLOW:
                dll_70_func_B58(MAP_SWAPSTONE_HOLLOW, 0, 0);
                return;
            case LEVELSELECT_IDX_04_SWAPSTONE_CIRCLE:
                dll_70_func_B58(MAP_SWAPSTONE_CIRCLE, 1, 1);
                return;
            case LEVELSELECT_IDX_06_DARKICE_MINES:
                dll_70_func_B58(MAP_DARK_ICE_MINES_1, 0, 0);
                return;
            case LEVELSELECT_IDX_07_DARKICE_MINES_TWO:
                dll_70_func_B58(MAP_ANIMTEST, 0, 0); //DIM2 may have been moved down by one index, after Animtest?
                return;
            case LEVELSELECT_IDX_08_WALLED_CITY:
                data_D48 = TRUE;
                break;
            case LEVELSELECT_IDX_09_DRAGON_ROCK:
                data_D48 = TRUE; //Wasn't created at this stage? (conflicts with how Kev Bayliss mentioned it was one of the earliest areas they created!)
                break;
            case LEVELSELECT_IDX_0A_CLOUDRUNNER_FORTRESS:
                dll_70_func_B58(MAP_CLOUDRUNNER_FORTRESS, 0, 1);
                return;
            case LEVELSELECT_IDX_0B_KRAZOA_PALACE:
                data_D48 = TRUE;  //Wasn't created at this stage?
                break;
            case LEVELSELECT_IDX_0C_BLACKWATER_CANYON:
                data_D48 = TRUE;  //Wasn't created at this stage?
                break;
            case LEVELSELECT_IDX_0E_NORTHERN_WASTES:
                dll_70_func_B58(MAP_SNOWHORN_WASTES, 0, 0);
                return;
            case LEVELSELECT_IDX_0F_EARTHWALKER_TEMPLE:
                dll_70_func_B58(MAP_EARTHWALKER_TEMPLE, 1, 0);
                return;
            case LEVELSELECT_IDX_10_WILLOW_GROVE:
                dll_70_func_B58(MAP_WILLOW_GROVE, 0, 1);
                return;
            case LEVELSELECT_IDX_11_DIAMOND_BAY:
                dll_70_func_B58(MAP_DIAMOND_BAY, 0, 0);
                return;
            case LEVELSELECT_IDX_12_DISCOVERY_FALLS:
                dll_70_func_B58(MAP_DISCOVERY_FALLS, 0, 1);
                return;
            case LEVELSELECT_IDX_13_MOON_MOUNTAIN_PASS:
                dll_70_func_B58(MAP_MOON_MOUNTAIN_PASS, 0, 1);
                return;
            case LEVELSELECT_IDX_14_CAPE_CLAW:
                dll_70_func_B58(MAP_CAPE_CLAW, 0, 1);
                return;
            case LEVELSELECT_IDX_15_GOLDEN_PLAINS:
                dll_70_func_B58(MAP_GOLDEN_PLAINS, 0, 1);
                return;
            case LEVELSELECT_IDX_17_TEST_OF_COMBAT:
                dll_70_func_B58(MAP_SHRINE_DISCOVERY_FALLS, 0, 1);
                return;
            case LEVELSELECT_IDX_18_TEST_OF_STRENGTH:
                dll_70_func_B58(MAP_SHRINE_DIAMOND_BAY, 0, 0);
                return;
            case LEVELSELECT_IDX_19_TEST_OF_FEAR:
                dll_70_func_B58(MAP_SHRINE_MOON_MOUNTAIN_PASS, 0, 1);
                return;
            case LEVELSELECT_IDX_1A_TEST_OF_CHARACTER:
                dll_70_func_B58(MAP_SHRINE_CAPE_CLAW, 0, 1);
                return;
            case LEVELSELECT_IDX_1B_TEST_OF_KNOWLEDGE:
                dll_70_func_B58(MAP_SHRINE_GOLDEN_PLAINS, 0, 1);
                return;
            case LEVELSELECT_IDX_1C_TEST_OF_SACRIFICE:
                dll_70_func_B58(MAP_SHRINE_SNOWHORN_WASTES, 0, 0);
                return;
            case LEVELSELECT_IDX_1D_TEST_OF_SKILL:
                dll_70_func_B58(MAP_SHRINE_WALLED_CITY, 0, 0);
                return;
            case LEVELSELECT_IDX_1E_TEST_OF_MAGIC:
                dll_70_func_B58(MAP_SHRINE_WILLOW_GROVE, 0, 0);
                return;
            case LEVELSELECT_IDX_21_DARKICE_BOSS:
                dll_70_func_B58(MAP_BOSS_GALADON, 0, 1);
                return;
            case LEVELSELECT_IDX_22_GENERAL_SCALES_BOSS:
                data_D48 = TRUE;
                break;
            case LEVELSELECT_IDX_23_BLACKWATER_BOSS:
                data_D48 = TRUE;
                break;
            case LEVELSELECT_IDX_24_CLOUDRUNNER_RACE:
                dll_70_func_B58(MAP_CLOUDRUNNER_RACETRACK, 0, 1); //CRF Trap Rooms scrapped by this stage?
                return;
            case LEVELSELECT_IDX_25_KAMERIA_DRAGON_BOSS:
                data_D48 = TRUE; //Wasn't created at this stage?
                break;
            case LEVELSELECT_IDX_26_DRAKOR_FINAL_BOSS:
                dll_70_func_B58(MAP_BOSS_DRAKOR, 0, 0);
                break;
            case LEVELSELECT_IDX_27_ICE_MOUNTAIN:
                dll_70_func_B58(MAP_ICE_MOUNTAIN_1, 0, 0);
                return;
            case LEVELSELECT_IDX_29_DESERT_FORCE_POINT:
                dll_70_func_B58(MAP_DESERT_FORCE_POINT_TEMPLE_BOTTOM, 0, 1);
                return;
            case LEVELSELECT_IDX_2A_VOLCANO_FORCE_POINT:
                dll_70_func_B58(MAP_VOLCANO_FORCE_POINT_TEMPLE, 0, 1);
                return;
            case LEVELSELECT_IDX_2C_EARTHWALKER_ACT_TWO:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(8, 1);
                return;
            case LEVELSELECT_IDX_2D_ENERGY_DEMO:
                dll_70_func_B58(MAP_EARTHWALKER_TEMPLE, 5, 1);
                return;
            case LEVELSELECT_IDX_2F_WARLOCK_ACT_ONE:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(0xA, 1);
                return;
            case LEVELSELECT_IDX_30_WARLOCK_ACT_TWO:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(0xB, 1);
                return;
            case LEVELSELECT_IDX_31_WARLOCK_ACT_THREE:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(0xC, 1);
                return;
            case LEVELSELECT_IDX_32_WARLOCK_ACT_FOUR:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(0xD, 1);
                return;
            case LEVELSELECT_IDX_33_WARLOCK_ACT_FIVE:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(0xE, 1);
                return;
            case LEVELSELECT_IDX_34_WARLOCK_ACT_SIX:
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
                gDLL_29_Gplay->vtbl->load_save(0xF, 1);
                return;
            default:
                data_D48 = TRUE;
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
        bss_0 = 0;
        dll_throw_fault->vtbl->init_text_window(180);
        dll_throw_fault->vtbl->add_string(0, "LEVEL NOT AVAILABLE", 12, 0);
        if (dll_throw_fault->vtbl->handle_joystick_and_buttons(&bss_0) >= 0) {
            data_D48 = FALSE;
        }
    }

    font_window_draw(gdl, 0, 0, 1);
}

// offset: 0xB58 | func: 3
void dll_70_func_B58(s32 mapID, s32 act, s32 playerNo) {
    gDLL_5_AMSEQ2->vtbl->set(NULL, 0x23, 0, 0, 0);
    gDLL_29_Gplay->vtbl->init_save(0, NULL);
    main_change_map(mapID, act, playerNo, MENU_GAMEPLAY);
}
