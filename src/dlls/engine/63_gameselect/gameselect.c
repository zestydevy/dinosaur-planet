#include "dlls/engine/63_gameselect.h"

#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/28_screen_fade.h"
#include "dlls/engine/74_pic_menu.h"
#include "sys/gfx/gx.h"
#include "sys/gfx/texture.h"
#include "sys/fonts.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "sys/memory.h"
#include "dll.h"
#include "types.h"
#include "functions.h"
#include "macros.h"

static PicMenuItem sGameSelectMenuItems[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 158, /*innerWidth*/ 320,
        /*itemX*/ 58, /*itemY*/ 146, /*textureID*/ -1, /*outerWidth*/ 320,
        /*flags*/ PICMENU_HAS_BACKGROUND,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ -1, /*downLink*/ 1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 212, /*innerWidth*/ 320,
        /*itemX*/ 58, /*itemY*/ 200, /*textureID*/ -1, /*outerWidth*/ 320,
        /*flags*/ PICMENU_HAS_BACKGROUND,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 0, /*downLink*/ 2, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1),
    /*2*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 266, /*innerWidth*/ 320,
        /*itemX*/ 58, /*itemY*/ 254, /*textureID*/ -1, /*outerWidth*/ 320,
        /*flags*/ PICMENU_HAS_BACKGROUND,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 1, /*downLink*/ 3, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1),
    /*3*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 324, /*innerWidth*/ 0,
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0,
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_AUTO_WIDTH,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 2, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ 4, /*overrideWith*/ -1),
    /*4*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 324, /*innerWidth*/ 0,
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0,
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_INLINE | PICMENU_AUTO_WIDTH,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 2, /*downLink*/ -1, /*leftLink*/ 3, /*rightLink*/ -1, /*overrideWith*/ -1)};
static s8 sGameSelectMenuText[] = {-1, -2, -3, 5, 6};

static PicMenuItem sCopySrcSelectMenuItems[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 158, /*innerWidth*/ 320,
        /*itemX*/ 58, /*itemY*/ 146, /*textureID*/ -1, /*outerWidth*/ 320,
        /*flags*/ PICMENU_HAS_BACKGROUND,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ -1, /*downLink*/ 1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 212, /*innerWidth*/ 320,
        /*itemX*/ 58, /*itemY*/ 200, /*textureID*/ -1, /*outerWidth*/ 320,
        /*flags*/ PICMENU_HAS_BACKGROUND,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 0, /*downLink*/ 2, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1),
    /*2*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 266, /*innerWidth*/ 320,
        /*itemX*/ 58, /*itemY*/ 254, /*textureID*/ -1, /*outerWidth*/ 320,
        /*flags*/ PICMENU_HAS_BACKGROUND,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 1, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1)};
static s8 sCopySrcSelectMenuText[] = {-1, -2, -3};

static PicMenuItem sCopyDstSelectMenuItems[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 284, /*innerWidth*/ 320,
        /*itemX*/ 58, /*itemY*/ 272, /*textureID*/ -1, /*outerWidth*/ 320,
        /*flags*/ PICMENU_HAS_BACKGROUND,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ -1, /*downLink*/ 1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 338, /*innerWidth*/ 320,
        /*itemX*/ 58, /*itemY*/ 326, /*textureID*/ -1, /*outerWidth*/ 320,
        /*flags*/ PICMENU_HAS_BACKGROUND,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 0, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1)};
static s8 sCopyDstSelectMenuText[] = {-1, -2};

static PicMenuItem sCopyConfirmMenuItems[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 58, /*textY*/ 324, /*innerWidth*/ 0,
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0,
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_AUTO_WIDTH,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ 1, /*overrideWith*/ -1),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 58, /*textY*/ 324, /*innerWidth*/ 0,
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0,
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_INLINE | PICMENU_AUTO_WIDTH,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 0, /*rightLink*/ -1, /*overrideWith*/ -1)};
static s8 sCopyConfirmMenuText[] = {1, 2};

static PicMenuItem sEraseSelectMenuItems[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 158, /*innerWidth*/ 320,
        /*itemX*/ 58, /*itemY*/ 146, /*textureID*/ -1, /*outerWidth*/ 320,
        /*flags*/ PICMENU_HAS_BACKGROUND,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ -1, /*downLink*/ 1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 212, /*innerWidth*/ 320,
        /*itemX*/ 58, /*itemY*/ 200, /*textureID*/ -1, /*outerWidth*/ 320,
        /*flags*/ PICMENU_HAS_BACKGROUND,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 0, /*downLink*/ 2, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1),
    /*2*/ NEW_PICMENU_ITEM(
        /*textX*/ 119, /*textY*/ 266, /*innerWidth*/ 320,
        /*itemX*/ 58, /*itemY*/ 254, /*textureID*/ -1, /*outerWidth*/ 320,
        /*flags*/ PICMENU_HAS_BACKGROUND,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 1, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ -1, /*overrideWith*/ -1)};
static s8 sEraseSelectMenuText[] = {-1, -2, -3};

static PicMenuItem sEraseConfirmMenuItems[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 58, /*textY*/ 324, /*innerWidth*/ 0,
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0,
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_AUTO_WIDTH,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ 1, /*overrideWith*/ -1),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 58, /*textY*/ 324, /*innerWidth*/ 0,
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0,
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_INLINE | PICMENU_AUTO_WIDTH,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 0, /*rightLink*/ -1, /*overrideWith*/ -1)};
static s8 sEraseConfirmMenuText[] = {1, 2};

static PicMenuItem sGameConfirmMenuItems[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 58, /*textY*/ 324, /*innerWidth*/ 0,
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0,
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_AUTO_WIDTH,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ 1, /*overrideWith*/ -1),
    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 58, /*textY*/ 324, /*innerWidth*/ 0,
        /*itemX*/ 58, /*itemY*/ 312, /*textureID*/ -1, /*outerWidth*/ 0,
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_INLINE | PICMENU_AUTO_WIDTH,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ 0, /*rightLink*/ -1, /*overrideWith*/ -1)};
static s8 sGameConfirmMenuText[] = {1, 2};

static PicMenuItem sGameRecapMenuItems[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 320, /*textY*/ 382, /*innerWidth*/ 0,
        /*itemX*/ 320, /*itemY*/ 370, /*textureID*/ -1, /*outerWidth*/ 0,
        /*flags*/ PICMENU_ALIGN_TEXT_CENTER | PICMENU_HAS_BACKGROUND | PICMENU_AUTO_WIDTH | PICMENU_ALIGN_ITEM_CENTER,
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ -1, /*downLink*/ -1, /*leftLink*/ -1, /*rightLink*/ -1,
        /*overrideWith*/ -1)};
static s8 sGameRecapMenuText[] = {14};

static GameSelectSubmenu sSubmenus[] = {
    /*0*/ {sGameSelectMenuItems, sGameSelectMenuText, /*count*/ 5, /*title*/ 3, /*button legend*/ 0, 2},
    /*1*/ {sGameConfirmMenuItems, sGameConfirmMenuText, /*count*/ 2, /*title*/ 7, /*button legend*/ 0, 2},
    /*2*/ {sCopySrcSelectMenuItems, sCopySrcSelectMenuText, /*count*/ 3, /*title*/ 8, /*button legend*/ 0, 2},
    /*3*/ {sCopyDstSelectMenuItems, sCopyDstSelectMenuText, /*count*/ 2, /*title*/ 9, /*button legend*/ 0, 2},
    /*4*/ {sCopyConfirmMenuItems, sCopyConfirmMenuText, /*count*/ 2, /*title*/ 10, /*button legend*/ 0, 2},
    /*5*/ {sEraseSelectMenuItems, sEraseSelectMenuText, /*count*/ 3, /*title*/ 11, /*button legend*/ 0, 2},
    /*6*/ {sEraseConfirmMenuItems, sEraseConfirmMenuText, /*count*/ 2, /*title*/ 12, /*button legend*/ 0, 2},
    /*7*/ {sGameRecapMenuItems, sGameRecapMenuText, /*count*/ 1, /*title*/ 13, /*button legend*/ -1, 2}};

static PicMenuSounds sGameRecapMenuSounds = {
    /*move*/ 0x5D5,
    /*select*/ 0x5D6,
    /*back*/ 0x5D4};
static GameTextChunk *sGameTextChunk = NULL;
static s8 sSubmenuIdx = -1;
static s8 sSelectedSaveIdx = -1; // currently selected save file
static s8 sCopyDstIdx = -1;      // save file to copy game to
static s16 sSaveGameTextureIDs[] = {
    0x031d, 0x031c,
    0x031f, 0x031e};
static s16 sSaveGameBgTextureIDs[] = {
    0x0320, 0x02f5,
    0x02f6, 0x02f7,
    0x02f8, 0x02f9,
    0x02fa, 0x02fb,
    0x02fc, 0x0313,
    0x02fd, 0x02fe,
    0x02ff, 0x0300,
    0x0301, 0x0302,
    0x0303, 0x0304};
static s16 sSaveGameBgIndices[] = {
    4, 17, 16, 17, 5, -1,
    6, 10, 9, 11, 13, -1,
    7, 11, 12, 9, 14, -1,
    2, 1, 0, 1, 3, -1};

/*0*/ static GameSelectSaveInfo sSaveGameInfo[3];
/*36*/ static s8 sCopyDstOptions[2];
/*38*/ static s8 sExitTransitionTimer;
/*39*/ static s8 sRedrawFrames;
/*3A*/ static s16 sSaveGameBoxX;
/*3C*/ static s16 sSaveGameBoxY;
/*40*/ static char sSaveGameTimeStr[10];
/*4A*/ static char sSpiritCountStr[2];
/*4C*/ static char sSpellStoneCountStr[2];
/*4E*/ static u8 sExitToGame;
/*4F*/ static u8 sExitToMainMenu;
/*50*/ static Texture *sBackgroundTexture;
/*54*/ static Texture *sLogoTexture;
/*58*/ static Texture *sLogoShadowTexture;
/*60*/ static char sRecentTaskNumStrs[4][4];
/*70*/ static Texture *sSaveGameTextures[4];    // Textures used in the save game box (character icons, spirit icon, spellstone icon)
/*80*/ static Texture *sSaveGameBgTextures[18]; // 32x32 tile textures making up the save game box background

static void dll_63_clean_up(s32 leavingMenus);
static void dll_63_goto_game_select(s32 param1);
static void dll_63_load_save_game_info();
static void dll_63_goto_game_confirm();
static void dll_63_init_submenu(GameSelectSubmenu *submenu);

static void dll_63_act_game_select(PicMenuAction action, s32 selected);
static void dll_63_act_game_confirm(PicMenuAction action, s32 selected);
static void dll_63_act_game_recap(PicMenuAction action, s32 selected);
static void dll_63_act_copy_src_select(PicMenuAction action, s32 selected);
static void dll_63_act_copy_dst_select(PicMenuAction action, s32 selected);
static void dll_63_act_copy_confirm(PicMenuAction action, s32 selected);
static void dll_63_act_erase_select(PicMenuAction action, s32 selected);
static void dll_63_act_erase_confirm(PicMenuAction action, s32 selected);

static void dll_63_draw_save_game_box(Gfx **gdl, s32 x, s32 y, GameSelectSaveInfo *saveInfo);

void dll_63_ctor(void *self)
{
    s32 i;

    sBackgroundTexture = queue_load_texture_proxy(0x2DD);
    sLogoTexture = queue_load_texture_proxy(0xC5);
    sLogoShadowTexture = queue_load_texture_proxy(0x2E1);

    if (sGameTextChunk == NULL)
    {
        sGameTextChunk = gDLL_21_Gametext->exports->get_chunk(0xEC);
    }

    for (i = 0; i < 18; i++)
    {
        sSaveGameBgTextures[i] = queue_load_texture_proxy(sSaveGameBgTextureIDs[i]);
    }

    for (i = 0; i < 4; i++)
    {
        sSaveGameTextures[i] = queue_load_texture_proxy(sSaveGameTextureIDs[i]);
    }

    if (menu_get_previous() != MENU_6)
    {
        gDLL_28_ScreenFade->exports->fade_reversed(20, SCREEN_FADE_BLACK);
        dll_63_goto_game_select(1);
    }
    else
    {
        sSelectedSaveIdx = get_save_game_idx();
        sSaveGameBoxX = 56;
        sSaveGameBoxY = 179;
        dll_63_load_save_game_info();
        dll_63_goto_game_confirm();
    }

    sExitToGame = sExitToMainMenu = FALSE;
    sExitTransitionTimer = 0;
    sRedrawFrames = 4;
}

void dll_63_dtor(void *self) {}

// func 0 export 0
s32 dll_63_update1()
{
    PicMenuAction action;
    s32 selected;
    s8 prevExitTransitionTimer;
    s32 delay;

    prevExitTransitionTimer = sExitTransitionTimer;

    delay = delayByte;
    if (delay > 3)
    {
        delay = 3;
    }

    if (sExitTransitionTimer > 0)
    {
        sExitTransitionTimer -= delay;
    }

    if (!gDLL_28_ScreenFade->exports->is_complete())
    {
        gDLL_74_PicMenu->exports->redraw_all();
        sRedrawFrames = 4;
    }

    if (sExitToGame || sExitToMainMenu)
    {
        if (prevExitTransitionTimer >= 13 && sExitTransitionTimer <= 12)
        {
            func_8005D410(1, get_ossched(), 0);
            if (sExitToGame)
            {
                dll_63_clean_up(/*leavingMenus*/ TRUE);
            }
            else
            {
                dll_63_clean_up(/*leavingMenus*/ FALSE);
            }
            func_80041D20(1);
            func_80041C6C(1);
        }
        else if (sExitTransitionTimer < 1)
        {
            if (sExitToGame)
            {
                gDLL_29_Gplay->exports->start_game();
            }
            else
            {
                // Exit to main menu
                func_80014BBC();
                func_800142F0(12457.1f, -1474.875f, -6690.398f, 1);
                menu_set(MENU_4);
            }
        }

        if (sExitTransitionTimer <= 12)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        action = gDLL_74_PicMenu->exports->update();
        if (action != PICMENU_ACTION_NONE)
        {
            selected = gDLL_74_PicMenu->exports->get_selected_item();

            switch (sSubmenuIdx)
            {
            case SUBMENU_GAME_SELECT:
                dll_63_act_game_select(action, selected);
                break;
            case SUBMENU_GAME_CONFIRM:
                dll_63_act_game_confirm(action, selected);
                break;
            case SUBMENU_COPY_SRC_SELECT:
                dll_63_act_copy_src_select(action, selected);
                break;
            case SUBMENU_COPY_DST_SELECT:
                dll_63_act_copy_dst_select(action, selected);
                break;
            case SUBMENU_COPY_CONFIRM:
                dll_63_act_copy_confirm(action, selected);
                break;
            case SUBMENU_ERASE_SELECT:
                dll_63_act_erase_select(action, selected);
                break;
            case SUBMENU_ERASE_CONFIRM:
                dll_63_act_erase_confirm(action, selected);
                break;
            case SUBMENU_GAME_RECAP:
                dll_63_act_game_recap(action, selected);
                break;
            }
        }

        return 0;
    }
}

// func 1 export 1
void dll_63_update2() {}

// func 2 export 2
void dll_63_draw(Gfx **gdl, Mtx **mtxs, Vertex **vtxs)
{
    s32 numRecentTasks;
    s32 uly;
    s32 lry;
    s32 ulx;
    s32 lrx;
    s32 y;
    s32 i;
    GameSelectSubmenu *submenu;

    submenu = &sSubmenus[sSubmenuIdx];

    if ((!sExitToGame && !sExitToMainMenu) || sExitTransitionTimer > 10)
    {
        font_window_set_coords(1, 0, 0,
                               RESOLUTION_WIDTH(get_some_resolution_encoded()) - 100,
                               RESOLUTION_HEIGHT(get_some_resolution_encoded()));
        font_window_flush_strings(1);

        font_window_set_coords(3, 105, 0,
                               RESOLUTION_WIDTH(get_some_resolution_encoded()) - 200,
                               RESOLUTION_HEIGHT(get_some_resolution_encoded()));
        font_window_flush_strings(3);

        if (sRedrawFrames != 0)
        {
            func_8003825C(gdl, sBackgroundTexture, 0, 0, 0, 0, 0xFF, 2);

            if (sSubmenuIdx == SUBMENU_GAME_RECAP)
            {
                func_8003825C(gdl, sLogoShadowTexture, 119, 92, 0, 0, 0xFF, 0);
                func_8003825C(gdl, sLogoTexture, 129, 100, 0, 0, 0xFF, 0);

                numRecentTasks = gDLL_30_Task->exports->get_num_recently_completed();
                if (numRecentTasks > 3)
                {
                    numRecentTasks = 3;
                }

                font_window_enable_wordwrap(3);
                font_window_use_font(1, FONT_FUN_FONT);
                font_window_use_font(3, FONT_FUN_FONT);
                font_window_set_text_colour(1, 183, 139, 97, 255, 255);
                font_window_set_text_colour(3, 183, 139, 97, 255, 255);

                y = 232;
                for (i = 0; i < numRecentTasks; i++)
                {
                    sprintf(sRecentTaskNumStrs[i], "%1d.", (int)(i + 1));
                    font_window_add_string_xy(1, 75, y, sRecentTaskNumStrs[i], 1, ALIGN_TOP_LEFT);
                    font_window_add_string_xy(3, 2, y, gDLL_30_Task->exports->get_recently_completed_task_text(i), 1, ALIGN_TOP_LEFT);
                    y += 40;
                }

                y = 232;
                font_window_set_text_colour(1, 0, 0, 0, 255, 255);
                font_window_set_text_colour(3, 0, 0, 0, 255, 255);
                for (i = 0; i < numRecentTasks; i++)
                {
                    sprintf(sRecentTaskNumStrs[i], "%1d.", (int)(i + 1));
                    font_window_add_string_xy(1, 73, y - 2, sRecentTaskNumStrs[i], 1, ALIGN_TOP_LEFT);
                    font_window_add_string_xy(3, 0, y - 2, gDLL_30_Task->exports->get_recently_completed_task_text(i), 1, ALIGN_TOP_LEFT);
                    y += 40;
                }
            }
            else
            {
                if (sSelectedSaveIdx != -1)
                {
                    dll_63_draw_save_game_box(gdl, sSaveGameBoxX, sSaveGameBoxY, &sSaveGameInfo[sSelectedSaveIdx]);
                }

                font_window_use_font(1, FONT_FUN_FONT);
                font_window_set_text_colour(1, 183, 139, 97, 255, 255);

                font_window_add_string_xy(1, 320, 405, sGameTextChunk->strings[submenu->buttonLegendTextIdx], 1, ALIGN_TOP_CENTER);
                font_window_set_text_colour(1, 0, 0, 0, 255, 255);
                font_window_add_string_xy(1, 318, 403, sGameTextChunk->strings[submenu->buttonLegendTextIdx], 2, ALIGN_TOP_CENTER);
            }

            font_window_set_coords(2, 0, 0,
                                   RESOLUTION_WIDTH(get_some_resolution_encoded()) - 100,
                                   RESOLUTION_HEIGHT(get_some_resolution_encoded()));
            font_window_flush_strings(2);
            font_window_use_font(2, FONT_DINO_MEDIUM_FONT_IN);
            font_window_enable_wordwrap(2);
            font_window_set_text_colour(2, 255, 255, 255, 0, 255);

            font_window_add_string_xy(2, 69, 61, sGameTextChunk->strings[submenu->titleTextIdx], 1, ALIGN_TOP_LEFT);
            font_window_set_text_colour(2, 0, 0, 0, 255, 255);
            font_window_add_string_xy(2, 64, 56, sGameTextChunk->strings[submenu->titleTextIdx], 2, ALIGN_TOP_LEFT);

            font_window_draw(gdl, NULL, NULL, 2);
        }
        else
        {
            // Always redraw background in case picmenu redraws
            func_80010158(&ulx, &lrx, &uly, &lry);
            func_800382AC(gdl, sBackgroundTexture, 0, 0, uly, lry, 0xFF, 2);
        }

        gDLL_74_PicMenu->exports->draw(gdl);

        font_window_draw(gdl, NULL, NULL, 1);
        font_window_draw(gdl, NULL, NULL, 3);

        sRedrawFrames -= 1;
        if (sRedrawFrames < 0)
        {
            sRedrawFrames = 0;
        }
    }
}

// func 3
static void dll_63_clean_up(s32 leavingMenus)
{
    s32 i;

    if (sGameTextChunk != NULL)
    {
        free(sGameTextChunk);
        sGameTextChunk = NULL;
    }

    if (sSubmenuIdx != -1)
    {
        gDLL_74_PicMenu->exports->clear_items();
        sSubmenuIdx = -1;
    }

    for (i = 0; i < 18; i++)
    {
        if (sSaveGameBgTextures[i] != NULL)
        {
            texture_destroy(sSaveGameBgTextures[i]);
            sSaveGameBgTextures[i] = NULL;
        }
    }

    for (i = 0; i < 4; i++)
    {
        if (sSaveGameTextures[i] != NULL)
        {
            texture_destroy(sSaveGameTextures[i]);
            sSaveGameTextures[i] = NULL;
        }
    }

    texture_destroy(sBackgroundTexture);
    texture_destroy(sLogoTexture);
    texture_destroy(sLogoShadowTexture);

    if (leavingMenus)
    {
        font_unload(FONT_DINO_MEDIUM_FONT_IN);
        font_unload(FONT_DINO_MEDIUM_FONT_OUT);
        font_unload(FONT_FUN_FONT);
        func_80014B6C();
    }
}

// func 4
static void dll_63_goto_game_select(s32 param1)
{
    GameSelectSubmenu *submenu;

    if (sSubmenuIdx != -1)
    {
        gDLL_74_PicMenu->exports->clear_items();
    }

    sSubmenuIdx = SUBMENU_GAME_SELECT;
    sSelectedSaveIdx = -1;
    submenu = &sSubmenus[sSubmenuIdx];

    dll_63_load_save_game_info();

    dll_63_init_submenu(submenu);
    gDLL_74_PicMenu->exports->set_items(submenu->menuItems, submenu->count,
                                        /*defaultItem*/ 0,
                                        /*sounds*/ NULL,
                                        /*param5*/ 5,
                                        /*param6*/ 4,
                                        /*textColor*/ 0, 0, 0,
                                        /*textHighlight*/ 0, 0, 0);

    sRedrawFrames = 2;
}

// func 5
static void dll_63_load_save_game_info()
{
    s32 i;
    GplayStruct8 *saveFile;
    char *filenamePtr;

    for (i = 0; i < 3; i++)
    {
        if ((u8)gDLL_29_Gplay->exports->load_save(i, /*startGame*/ FALSE) == 0)
        {
            // failed to load save?
            gDLL_29_Gplay->exports->erase_save(i);
            bzero(&sSaveGameInfo[i], sizeof(GameSelectSaveInfo));
            sSaveGameInfo[i].isEmpty = TRUE;
        }
        else
        {
            saveFile = &gDLL_29_Gplay->exports->func_E74()->unk0.unk0.unk0;

            if (saveFile->unk0x2f6 == 0)
            {
                sSaveGameInfo[i].character = saveFile->character;
                sSaveGameInfo[i].spiritBits = get_gplay_bitstring(0x489);
                sSaveGameInfo[i].unk3 = 0;

                filenamePtr = sSaveGameInfo[i].filename;

                gDLL_7_Newday->exports->convert_ticks_to_real_time(
                    saveFile->unk0x2fc,
                    &sSaveGameInfo[i].timeHours, &sSaveGameInfo[i].timeMinutes, &sSaveGameInfo[i].timeSeconds);

                sSaveGameInfo[i].unkA = 0;
                sSaveGameInfo[i].isEmpty = FALSE;

                bcopy(saveFile->saveFilename, filenamePtr, 5); // 1 less to preserve null terminator
            }
            else
            {
                bzero(&sSaveGameInfo[i], sizeof(GameSelectSaveInfo));
                sSaveGameInfo[i].isEmpty = TRUE;
            }
        }
    }
}

// func 6
static void dll_63_goto_game_confirm()
{
    GameSelectSubmenu *submenu;

    if (sSubmenuIdx != -1)
    {
        gDLL_74_PicMenu->exports->clear_items();
    }

    sSubmenuIdx = SUBMENU_GAME_CONFIRM;
    submenu = &sSubmenus[sSubmenuIdx];

    dll_63_init_submenu(submenu);
    gDLL_74_PicMenu->exports->set_items(submenu->menuItems, submenu->count,
                                        /*defaultItem*/ 0,
                                        /*sounds*/ NULL,
                                        /*param5*/ 5,
                                        /*param6*/ 4,
                                        /*textColor*/ 0, 0, 0,
                                        /*textHighlight*/ 0, 0, 0);

    sRedrawFrames = 2;
}

// func 7
static void dll_63_goto_copy_src_select()
{
    GameSelectSubmenu *submenu;
    s32 i;

    if (sSubmenuIdx != -1)
    {
        gDLL_74_PicMenu->exports->clear_items();
    }

    sSubmenuIdx = SUBMENU_COPY_SRC_SELECT;
    sSelectedSaveIdx = -1;
    submenu = &sSubmenus[sSubmenuIdx];

    for (i = 0; i < 3; i++)
    {
        if (sSaveGameInfo[i].isEmpty)
        {
            submenu->menuItems[i].flags |= (PICMENU_TRANSPARENT | PICMENU_DISABLED);
        }
        else
        {
            submenu->menuItems[i].flags &= ~(PICMENU_TRANSPARENT | PICMENU_DISABLED);
        }
    }

    dll_63_init_submenu(submenu);
    gDLL_74_PicMenu->exports->set_items(submenu->menuItems, submenu->count,
                                        /*defaultItem*/ 0,
                                        /*sounds*/ NULL,
                                        /*param5*/ 5,
                                        /*param6*/ 4,
                                        /*textColor*/ 0, 0, 0,
                                        /*textHighlight*/ 0, 0, 0);

    sRedrawFrames = 2;
}

// func 8
static void dll_63_goto_copy_dst_select()
{
    GameSelectSubmenu *submenu;
    s32 i;
    s32 k;
    s32 temp;

    if (sSubmenuIdx != -1)
    {
        gDLL_74_PicMenu->exports->clear_items();
    }

    sSubmenuIdx = SUBMENU_COPY_DST_SELECT;
    submenu = &sSubmenus[sSubmenuIdx];
    k = 0;
    for (i = 0; i < 3; i++)
    {
        if (i != sSelectedSaveIdx)
        {
            submenu->textIDs[k] = -i - 1;

            if (!sSaveGameInfo[i].isEmpty)
            {
                submenu->menuItems[k].flags |= (PICMENU_TRANSPARENT | PICMENU_DISABLED);
            }
            else
            {
                submenu->menuItems[k].flags &= ~(PICMENU_TRANSPARENT | PICMENU_DISABLED);
            }

            sCopyDstOptions[k] = i;
            k++;
        }
    }

    dll_63_init_submenu(submenu);
    gDLL_74_PicMenu->exports->set_items(submenu->menuItems, submenu->count,
                                        /*defaultItem*/ 0,
                                        /*sounds*/ NULL,
                                        /*param5*/ 5,
                                        /*param6*/ 4,
                                        /*textColor*/ 0, 0, 0,
                                        /*textHighlight*/ 0, 0, 0);

    sRedrawFrames = 2;
}

// func 9
static void dll_63_goto_copy_confirm()
{
    GameSelectSubmenu *submenu;

    if (sSubmenuIdx != -1)
    {
        gDLL_74_PicMenu->exports->clear_items();
    }

    sSubmenuIdx = SUBMENU_COPY_CONFIRM;
    submenu = &sSubmenus[sSubmenuIdx];

    dll_63_init_submenu(submenu);
    gDLL_74_PicMenu->exports->set_items(submenu->menuItems, submenu->count,
                                        /*defaultItem*/ 0,
                                        /*sounds*/ NULL,
                                        /*param5*/ 5,
                                        /*param6*/ 4,
                                        /*textColor*/ 0, 0, 0,
                                        /*textHighlight*/ 0, 0, 0);

    sRedrawFrames = 2;
}

// func 10
static void dll_63_goto_erase_select()
{
    GameSelectSubmenu *submenu;
    s32 i;

    if (sSubmenuIdx != -1)
    {
        gDLL_74_PicMenu->exports->clear_items();
    }

    sSubmenuIdx = SUBMENU_ERASE_SELECT;
    sSelectedSaveIdx = -1;
    submenu = &sSubmenus[sSubmenuIdx];

    dll_63_init_submenu(submenu);

    for (i = 0; i < 3; i++)
    {
        if (sSaveGameInfo[i].isEmpty)
        {
            submenu->menuItems[i].flags |= (PICMENU_TRANSPARENT | PICMENU_DISABLED);
        }
        else
        {
            submenu->menuItems[i].flags &= ~(PICMENU_TRANSPARENT | PICMENU_DISABLED);
        }
    }

    gDLL_74_PicMenu->exports->set_items(submenu->menuItems, submenu->count,
                                        /*defaultItem*/ 0,
                                        /*sounds*/ NULL,
                                        /*param5*/ 5,
                                        /*param6*/ 4,
                                        /*textColor*/ 0, 0, 0,
                                        /*textHighlight*/ 0, 0, 0);

    sRedrawFrames = 2;
}

// func 11
static void dll_63_goto_erase_confirm()
{
    GameSelectSubmenu *submenu;

    if (sSubmenuIdx != -1)
    {
        gDLL_74_PicMenu->exports->clear_items();
    }

    sSubmenuIdx = SUBMENU_ERASE_CONFIRM;
    submenu = &sSubmenus[sSubmenuIdx];

    dll_63_init_submenu(submenu);
    gDLL_74_PicMenu->exports->set_items(submenu->menuItems, submenu->count,
                                        /*defaultItem*/ 1,
                                        /*sounds*/ NULL,
                                        /*param5*/ 5,
                                        /*param6*/ 4,
                                        /*textColor*/ 0, 0, 0,
                                        /*textHighlight*/ 0, 0, 0);

    sRedrawFrames = 2;
}

// func 12
static void dll_63_goto_game_recap()
{
    GameSelectSubmenu *submenu;

    if (sSubmenuIdx != -1)
    {
        gDLL_74_PicMenu->exports->clear_items();
    }

    sSubmenuIdx = SUBMENU_GAME_RECAP;
    submenu = &sSubmenus[sSubmenuIdx];

    dll_63_init_submenu(submenu);
    gDLL_74_PicMenu->exports->set_items(submenu->menuItems, submenu->count,
                                        /*defaultItem*/ 0,
                                        /*sounds*/ &sGameRecapMenuSounds,
                                        /*param5*/ 5,
                                        /*param6*/ 4,
                                        /*textColor*/ 0, 0, 0,
                                        /*textHighlight*/ 0, 0, 0);

    sRedrawFrames = 2;
}

// func 13
static void dll_63_init_submenu(GameSelectSubmenu *submenu)
{
    s32 i;
    s32 textID;
    GameSelectSaveInfo *saveGame;

    for (i = 0; i < submenu->count; i++)
    {
        textID = submenu->textIDs[i];

        if (textID >= 0)
        {
            submenu->menuItems[i].text = sGameTextChunk->strings[textID];
        }
        else
        {
            saveGame = &sSaveGameInfo[-textID - 1];

            if (saveGame->isEmpty)
            {
                submenu->menuItems[i].text = sGameTextChunk->strings[4];
                submenu->menuItems[i].flags &= ~1;
                submenu->menuItems[i].flags |= PICMENU_ALIGN_TEXT_CENTER;
                submenu->menuItems[i].texture.asID = -1;
            }
            else
            {
                submenu->menuItems[i].text = saveGame->filename;
                submenu->menuItems[i].flags &= ~PICMENU_ALIGN_TEXT_CENTER;
                submenu->menuItems[i].flags |= 1;
                submenu->menuItems[i].texture.asID = sSaveGameTextureIDs[saveGame->character];
            }
        }
    }
}

// func 14
static void dll_63_act_game_select(PicMenuAction action, s32 selected)
{
    switch (selected)
    {
    case 4:
        if (action == PICMENU_ACTION_SELECT)
        {
            dll_63_goto_erase_select();
        }
        break;
    case 3:
        if (action == PICMENU_ACTION_SELECT)
        {
            dll_63_goto_copy_src_select();
        }
        break;
    default:
        // Selected a save file button
        if (action == PICMENU_ACTION_SELECT)
        {
            if (sSaveGameInfo[selected].isEmpty)
            {
                // Go to name entry menu
                dll_63_clean_up(0);
                set_save_game_idx(selected);
                menu_set(MENU_6);
            }
            else
            {
                sSelectedSaveIdx = selected;
                sSaveGameBoxX = 56;
                sSaveGameBoxY = 179;
                dll_63_goto_game_confirm();
            }
        }
        break;
    }
}

// func 15
static void dll_63_act_game_confirm(PicMenuAction action, s32 selected)
{
    if (action == PICMENU_ACTION_BACK || selected == 1)
    {
        dll_63_goto_game_select(0);
    }
    else
    {
        gDLL_29_Gplay->exports->load_save(sSelectedSaveIdx, /*startGame*/ FALSE);

        gDLL_30_Task->exports->load_recently_completed();

        if (gDLL_30_Task->exports->get_num_recently_completed() != 0)
        {
            dll_63_goto_game_recap();
        }
        else
        {
            sExitToGame = TRUE;
            gDLL_28_ScreenFade->exports->fade(20, SCREEN_FADE_BLACK);
            gDLL_5_AMSEQ->exports->func6(0);
            gDLL_5_AMSEQ->exports->func6(1);
            gDLL_5_AMSEQ->exports->func6(2);
            gDLL_5_AMSEQ->exports->func6(3);
            sExitTransitionTimer = 35;
        }
    }
}

// func 16
static void dll_63_act_game_recap(PicMenuAction action, s32 selected)
{
    if (action == PICMENU_ACTION_BACK)
    {
        sSelectedSaveIdx = selected;
        sSaveGameBoxX = 56;
        sSaveGameBoxY = 179;
        dll_63_goto_game_confirm();
    }
    else if (action == PICMENU_ACTION_SELECT)
    {
        sExitToGame = TRUE;
        gDLL_28_ScreenFade->exports->fade(20, SCREEN_FADE_BLACK);
        gDLL_5_AMSEQ->exports->func6(0);
        gDLL_5_AMSEQ->exports->func6(1);
        gDLL_5_AMSEQ->exports->func6(2);
        gDLL_5_AMSEQ->exports->func6(3);
        sExitTransitionTimer = 35;
    }
}

// func 17
static void dll_63_act_copy_src_select(PicMenuAction action, s32 selected)
{
    if (action == PICMENU_ACTION_BACK)
    {
        dll_63_goto_game_select(0);
    }
    else
    {
        sSelectedSaveIdx = selected;
        sSaveGameBoxX = 58;
        sSaveGameBoxY = 146;
        dll_63_goto_copy_dst_select();
    }
}

// func 18
static void dll_63_act_copy_dst_select(PicMenuAction action, s32 selected)
{
    if (action == PICMENU_ACTION_BACK)
    {
        dll_63_goto_copy_src_select();
    }
    else
    {
        sCopyDstIdx = sCopyDstOptions[selected];
        dll_63_goto_copy_confirm();
    }
}

// func 19
static void dll_63_act_copy_confirm(PicMenuAction action, s32 selected)
{
    if (action == PICMENU_ACTION_BACK || selected == 1)
    {
        dll_63_goto_copy_dst_select();
    }
    else
    {
        gDLL_29_Gplay->exports->copy_save(sSelectedSaveIdx, sCopyDstIdx);
        dll_63_goto_game_select(0);
    }
}

// func 20
static void dll_63_act_erase_select(PicMenuAction action, s32 selected)
{
    if (action == PICMENU_ACTION_BACK)
    {
        dll_63_goto_game_select(0);
    }
    else
    {
        sSelectedSaveIdx = selected;
        sSaveGameBoxX = 56;
        sSaveGameBoxY = 179;
        dll_63_goto_erase_confirm();
    }
}

// func 21
static void dll_63_act_erase_confirm(PicMenuAction action, s32 selected)
{
    if (action == PICMENU_ACTION_BACK || selected == 1)
    {
        dll_63_goto_erase_select();
    }
    else
    {
        gDLL_29_Gplay->exports->erase_save(sSelectedSaveIdx);
        dll_63_goto_game_select(0);
    }
}

// func 22
static void dll_63_draw_save_game_box(Gfx **gdl, s32 x, s32 y, GameSelectSaveInfo *saveInfo)
{
    s32 i;
    s32 x2;
    s32 y2;
    s32 len;

    // Draw background
    x2 = x;
    y2 = y;
    len = ARRAYCOUNT(sSaveGameBgIndices);

    for (i = 0; i < len; i++)
    {
        if (sSaveGameBgIndices[i] == -1)
        {
            x2 = x;
            y2 += 32;
        }
        else
        {
            func_8003825C(gdl, sSaveGameBgTextures[sSaveGameBgIndices[i]], x2, y2, 0, 0, 0xFF, 0);
            x2 += 64;
        }
    }

    // Draw character icon
    func_8003825C(gdl, sSaveGameTextures[saveInfo->character], x + 14, y + 8, 0, 0, 0xFF, 0);
    // Draw spirit icon
    func_8003825C(gdl, sSaveGameTextures[2], x + 241, y + 71, 0, 0, 0xFF, 0);
    // Draw spell stone icon
    func_8003825C(gdl, sSaveGameTextures[3], x2 + 14, y + 71, 0, 0, 0xFF, 0);

    // Draw text
    font_window_use_font(1, FONT_DINO_MEDIUM_FONT_IN);
    font_window_set_text_colour(1, 255, 255, 255, 0, 255);

    font_window_add_string_xy(1, x + 64, y + 18, saveInfo->filename, 1, ALIGN_TOP_LEFT);

    sprintf(sSaveGameTimeStr, "%3d:%02d:%02d", saveInfo->timeHours, saveInfo->timeMinutes, saveInfo->timeSeconds);
    font_window_add_string_xy(1, x + 156, y + 49, sSaveGameTimeStr, 1, ALIGN_TOP_CENTER);

    sprintf(sSpiritCountStr, "%1d", 0);
    font_window_add_string_xy(1, x + 234, y + 81, sSpiritCountStr, 1, ALIGN_TOP_CENTER);

    sprintf(sSpellStoneCountStr, "%1d", 0);
    font_window_add_string_xy(1, x + 84, y + 81, sSpellStoneCountStr, 1, ALIGN_TOP_CENTER);
}
