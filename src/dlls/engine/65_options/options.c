#include "common.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/29_gplay.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/engine/74_picmenu.h"
#include "dlls/engine/75_frontend.h"
#include "game/gametexts.h"
#include "macros.h"
#include "sys/fonts.h"
#include "sys/audio/speaker.h"
#include "sys/menu.h"

extern void vi_set_modifiers(u8 updateViMode, s8 hStartMod, s8 vScaleMod);

#define NONE -1
#define MAX_CONTROLS_PER_PAGE 6
#define MAIN_PAGE_INDEX(pageID) (pageID - 1)

#define CHEATS_PER_SCREEN 4

//NOTE: causes a crash because the Gametext file only has 7 strings
#define TOTAL_CHEATS 32

//The index of the first cheat shown when scrolled all the way to the bottom of the cheats list
#define CHEATS_FIRST_IDX_LAST_GROUP (TOTAL_CHEATS - CHEATS_PER_SCREEN)

typedef struct {
    PicMenuItem *menuItems;
    s8 *textIDs;
    u8 count;               //Number of PicMenuItems on
    s8 boxLabel;            //LineID for the page title shown on pages that use a control box
    s8 navigationInfo;      //LineID for the navigation controls shown at the bottom of most pages
    u8 unkB;
    u8 unkC;
    u8 unkD;
} OptionsPage;

typedef enum {
    OPTIONS_PAGE_0_Main_Page,       //Top-level options page, navigating to Video/Audio/Display/Control/Cheats/Cinema pages
    OPTIONS_PAGE_1_Video,           //Choices for screen size/ratio, and navigating to Screen Position page
    OPTIONS_PAGE_2_Audio,           //Choice for setup (headphones/stereo/mono/surround), volume sliders for music/SFX
    OPTIONS_PAGE_3_Display,         //Toggles for subtitles/instruments (UI elements like the minimap)
    OPTIONS_PAGE_4_Control,         //Choice for Z-button lock (tap/hold/combo), and navigating to View Layout page
    OPTIONS_PAGE_5_Cheats,          //Scrollable box showing 4 cheats at a time
    OPTIONS_PAGE_6_Cinema,          //Inaccessible, but would display a control box like the other pages
    OPTIONS_PAGE_7_View_Layout,     //Diagram of control scheme
    OPTIONS_PAGE_8_Screen_Position  //Adjusting screen X/Y offset
} OptionsMenuPageIDs;

typedef enum {
    OPTIONS_VIDEO_0_Screen_Size,
    OPTIONS_VIDEO_1_Ratio,
    OPTIONS_VIDEO_2_Screen_Position,
    OPTIONS_VIDEO_3_Brightness //Unused
} OptionsMenu_VideoItems;

typedef enum {
    OPTIONS_AUDIO_0_Setup,
    OPTIONS_AUDIO_1_Music,
    OPTIONS_AUDIO_2_SFX,
    OPTIONS_AUDIO_3_Speech //Unused
} OptionsMenu_AudioItems;

typedef enum {
    OPTIONS_DISPLAY_0_Subtitles,
    OPTIONS_DISPLAY_1_Instruments
} OptionsMenu_DisplayItems;

typedef enum {
    OPTIONS_CONTROL_0_Z_Button,
    OPTIONS_CONTROL_1_View_Layout
} OptionsMenu_ControlItems;

typedef enum {
    OPTIONS_CHEATS_0_Up,
    OPTIONS_CHEATS_1_CheatShown1,
    OPTIONS_CHEATS_2_CheatShown2,
    OPTIONS_CHEATS_3_CheatShown3,
    OPTIONS_CHEATS_4_CheatShown4,
    OPTIONS_CHEATS_5_Down
} OptionsMenu_CheatsItems;

typedef enum {
    LINE_0_A_Select_B_Cancel,
    LINE_1_Options,

    //Main page
    LINE_2_Display,
    LINE_3_Control,
    LINE_4_Cheats,
    LINE_5_Video,
    LINE_6_Audio,
    LINE_7_Cinema,

    //Display page
    LINE_8_Subtitles,
        LINE_9_On,
        LINE_10_Off,
    LINE_11_Instruments,
        LINE_12_R_Button_Only,    //Unused, intended as a choice for "Instruments"?

    //Control page
    LINE_13_Z_Button,
        LINE_14_Combo,
        LINE_15_Tap,
        LINE_16_Hold,
    LINE_17_View_Layout,

    //Cheats page (unused, has a separate Gametext file instead)
    LINE_18_CheatA,
    LINE_19_CheatB,
    LINE_20_CheatC,

    //Video page
    LINE_21_Screen_Size,
        LINE_22_Full,
        LINE_23_Wide,
        LINE_24_Cinema,
    LINE_25_Ratio,
        LINE_26_Normal,
        LINE_27_16_by_9,
    LINE_28_Screen_Position,
    LINE_29_Brightness,     //Unused!

    //Audio page
    LINE_30_Setup,
        LINE_31_Stereo,
        LINE_32_Surround,
        LINE_33_Mono,
        LINE_34_Headphones,
    LINE_35_Music,
    LINE_36_SFX,
    LINE_37_Speech,         //Unused!

    //Cinema page (unused, has a separate Gametext file instead)
    LINE_38_Cinema0,
    LINE_39_Cinema1,

    LINE_40_OK
} MenuLines;

/*0x0*/ static char dDownArrowChar = 0xab;
/*0x4*/ static char dUpArrowChar = 0xbb;
/*0x8*/ static char dLeftArrowChar = 0x3c;
/*0xC*/ static char dRightArrowChar = 0x3e;

/*0x10*/ static PicMenuItem dItemsMain[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 64, /*innerWidth*/ 0,
        /*itemX*/ 59, /*itemY*/ 52, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | 1),  //TODO: add flag 1 to enum?
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ NONE, /*downLink*/ 1, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 118, /*innerWidth*/ 0,
        /*itemX*/ 59, /*itemY*/ 106, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | 1),
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 0, /*downLink*/ 2, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*2*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 172, /*innerWidth*/ 0,
        /*itemX*/ 59, /*itemY*/ 160, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | 1),
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 1, /*downLink*/ 3, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*3*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 226, /*innerWidth*/ 0,
        /*itemX*/ 59, /*itemY*/ 214, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | 1),
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 2, /*downLink*/ 4, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*4*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 280, /*innerWidth*/ 0,
        /*itemX*/ 59, /*itemY*/ 268, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | 1),
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 3, /*downLink*/ 5, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*5*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 334, /*innerWidth*/ 0,
        /*itemX*/ 59, /*itemY*/ 322, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | 1),
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 4, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    )
};
/*0x178*/ static s8 dTextIDsMain[] = {
    LINE_5_Video,
    LINE_6_Audio,
    LINE_2_Display,
    LINE_3_Control,
    LINE_4_Cheats,
    LINE_7_Cinema
};

/*0x180*/ static PicMenuItem dItemsDisplay[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 254, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 254, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ NONE, /*downLink*/ 1, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 278, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 278, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ 0, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    )
};
/*0x1F8*/ static s8 dTextIDsDisplay[] = {
    LINE_8_Subtitles,
    LINE_11_Instruments
};
/*0x1FC*/ static char* dDisplayChoiceStrings[] = {
    NULL, NULL
};

/*0x204*/ static PicMenuItem dItemsControl[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 254, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 254, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ NONE, /*downLink*/ 1, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 278, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 278, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | 1),               //NOTE: ENABLED
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ 0, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    )
};
/*0x27C*/ static s8 dTextIDsControl[] = {
    LINE_13_Z_Button,
    LINE_17_View_Layout
};
/*0x280*/ static char* dControlZButtonStrings[] = {
    NULL, NULL, NULL
};

#define NEW_PICMENU_ITEM_HASTEXT(textPtr, textX, textY, innerWidth, itemX, itemY, textureID, outerWidth, flags, fontID, highlightFontID, upLink, downLink, leftLink, rightLink, overrideWith) \
    {(void*)textPtr, textX, textY, innerWidth, itemX, itemY, {((void *)textureID)}, outerWidth, flags, fontID, highlightFontID, upLink, downLink, leftLink, rightLink, overrideWith, {0}, 0}

/*0x28C*/ static PicMenuItem dItemsCheats[] = {
    /*0*/ NEW_PICMENU_ITEM_HASTEXT(
        &dDownArrowChar,
        /*textX*/ 310, /*textY*/ 210, /*innerWidth*/ 0,
        /*itemX*/ 310, /*itemY*/ 210, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_ALIGN_TEXT_CENTER),
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ NONE, /*downLink*/ 1, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 254, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 254, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ 0, /*downLink*/ 2, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*2*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 280, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 280, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ 1, /*downLink*/ 3, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*3*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 306, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 306, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ 2, /*downLink*/ 4, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*4*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 332, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 332, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ 3, /*downLink*/ 5, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*5*/ NEW_PICMENU_ITEM_HASTEXT(
        &dUpArrowChar,
        /*textX*/ 310, /*textY*/ 356, /*innerWidth*/ 0,
        /*itemX*/ 310, /*itemY*/ 356, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_ALIGN_TEXT_CENTER),
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ 4, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    )
};

/*0x3F4*/ static PicMenuItem dItemsVideo[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 254, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 254, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ NONE, /*downLink*/ 1, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 278, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 278, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ 0, /*downLink*/ 2, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*2*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 302, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 302, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | 1),                  //NOTE: ENABLED
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ 1, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /* UNUSED BRIGHTNESS OPTION */
    /*3*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 326, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 326, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ 2, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    )
};
/*0x4E4*/ static s8 dTextIDsVideo[] = {
    LINE_21_Screen_Size,
    LINE_25_Ratio,
    LINE_28_Screen_Position,
    LINE_29_Brightness              //UNUSED
};
/*0x4E8*/ static char* dVideoSizeStrings[] = {
    NULL, NULL, NULL
};
/*0x4F4*/ static char* dVideoRatioStrings[] = {
    NULL, NULL
};

/*0x4FC*/ static PicMenuItem dItemsAudio[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 254, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 254, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ NONE, /*downLink*/ 1, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 278, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 278, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ 0, /*downLink*/ 2, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*2*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 302, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 302, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ 1, /*1*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /* UNUSED SPEECH VOLUME OPTION */
    /*3*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 326, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 326, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ 2, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    )
};
/*0x5EC*/ static s8 dTextIDsAudio[] = {
    LINE_30_Setup,
    LINE_35_Music,
    LINE_36_SFX,
    LINE_37_Speech  //UNUSED
};
/*0x5F0*/ static char* dAudioSetupStrings[] = {
    NULL, NULL, NULL, NULL
};

/*0x600*/ static PicMenuItem dItemsCinema[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 254, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 254, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ NONE, /*downLink*/ 1, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),

    /*1*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 278, /*innerWidth*/ 0,
        /*itemX*/ 83, /*itemY*/ 278, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | 1),
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT,
        /*upLink*/ 0, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    )
};
/*0x678*/ static u32 _data_678 = NULL; //Unused, maybe texture pointers for showing 3 sequence thumbnails?
/*0x67C*/ static u32 _data_67C = NULL;
/*0x680*/ static u32 _data_680 = NULL;

/*0x684*/ static PicMenuItem dItemsViewLayout[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 265, /*textY*/ 392, /*innerWidth*/ 0,
        /*itemX*/ 265, /*itemY*/ 380, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | PICMENU_ALIGN_TEXT_CENTER),
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),
};
/*0x6C0*/ static s8 dTextIDsViewLayout[] = {LINE_40_OK};

/*0x6C4*/ static PicMenuItem dItemsScreenPosition[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 320, /*textY*/ 237, /*innerWidth*/ 0,
        /*itemX*/ 320, /*itemY*/ 225, /*textureID*/ NONE, /*outerWidth*/ 0,
        /*flags*/ (PICMENU_ALIGN_ITEM_CENTER | PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | PICMENU_ALIGN_TEXT_CENTER),
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN,
        /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),
};
/*0x700*/ static s8 dTextIDsScreenPosition[] = {LINE_40_OK};

/*0x704*/ static OptionsPage dMenus[] = {
    {dItemsMain,            dTextIDsMain,           ARRAYCOUNT(dItemsMain),           NONE,             LINE_0_A_Select_B_Cancel,  4, 5, 4},
    {dItemsVideo,           dTextIDsVideo,          ARRAYCOUNT(dItemsVideo) - 1,      LINE_5_Video,     LINE_0_A_Select_B_Cancel,  3, 0, 0}, //NOTE: one unused item
    {dItemsAudio,           dTextIDsAudio,          ARRAYCOUNT(dItemsAudio) - 1,      LINE_6_Audio,     LINE_0_A_Select_B_Cancel,  3, 0, 0}, //NOTE: one unused item
    {dItemsDisplay,         dTextIDsDisplay,        ARRAYCOUNT(dItemsDisplay),        LINE_2_Display,   LINE_0_A_Select_B_Cancel,  3, 0, 0},
    {dItemsControl,         dTextIDsControl,        ARRAYCOUNT(dItemsControl),        LINE_3_Control,   LINE_0_A_Select_B_Cancel,  3, 0, 0},
    {dItemsCheats,          NULL,                   ARRAYCOUNT(dItemsCheats),         LINE_4_Cheats,    LINE_0_A_Select_B_Cancel,  3, 0, 0},
    {dItemsCinema,          NULL,                   ARRAYCOUNT(dItemsCinema),         LINE_7_Cinema,    LINE_0_A_Select_B_Cancel,  3, 0, 0},
    {dItemsViewLayout,      dTextIDsViewLayout,     ARRAYCOUNT(dItemsViewLayout),     NONE,             NONE,                      5, 5, 4},
    {dItemsScreenPosition,  dTextIDsScreenPosition, ARRAYCOUNT(dItemsScreenPosition), NONE,             NONE,                      5, 5, 4}
};

/*0x794*/ static GameTextChunk* dGametextMenu = NULL;
/*0x798*/ static GameTextChunk* dGametextControls = NULL;
/*0x79C*/ static s8 dMenuID = NONE;

/* Coords for the View Layout screen's heading labels */
/*0x7A0*/ static s16 dLayoutCoordsHeadings[][2] = {
    {470,   196},   //A BUTTON
    {464,   284},   //B BUTTON
    {460,   62},    //C BUTTONS
    {411,   373},   //Z BUTTON
    {63,    141},   //START
    {63,    319},   //CONTROL STICK
    {314,   62},    //R BUTTON
    {473,   150},   //<
    {473,   113},   //>
    {470,   92},    //▲
    {470,  130}    //▼
};

/* Coords for the View Layout screen's description labels */
/*0x7CC*/ static s16 dLayoutCoordsDescriptions[][2] = {
    {490,   89},    //Head Cam
    {490,   115},   //Objects
    {490,   134},   //Sidekick
    {490,   152},   //Magic Spells
    {472,   222},   //Action
    {472,   239},   //Select
    {462,   256},   //Attack
    {460,   307},   //Cancel
    {454,   325},   //When Locked -
    {495,   340},   //Jump/Avoid
    {413,  394},   //Camera
    {413,  409},   //Tap Or Hold To Lock
    {65,   339},   //Guide Character
    {65,   354},   //Aim Spell
    {65,   163},   //Pause
    {305,  83}     //Status & Info Display
};

/*0x80C*/ static s16 dBoxTextureIDs[] = {
    TEXTABLE_320_Textbox_Edge_Bottom_1,
    TEXTABLE_2F5_Textbox_Edge_Bottom_2,
    TEXTABLE_2F6_Textbox_Corner_Bottom_Left,
    TEXTABLE_2F7_Textbox_Corner_Bottom_Right,
    TEXTABLE_2F8_Textbox_Corner_Top_Left,
    TEXTABLE_2F9_Textbox_Corner_Top_Right,
    TEXTABLE_2FA_Textbox_Edge_Left_1,
    TEXTABLE_2FB_Textbox_Edge_Left_2,
    TEXTABLE_2FC_Textbox_Edge_Left_3,
    TEXTABLE_313_Textbox_Centre_1,
    TEXTABLE_2FD_Textbox_Centre_2,
    TEXTABLE_2FE_Textbox_Centre_3,
    TEXTABLE_2FF_Textbox_Centre_4,
    TEXTABLE_300_Textbox_Edge_Right_1,
    TEXTABLE_301_Textbox_Edge_Right_2,
    TEXTABLE_302_Textbox_Edge_Right_3,
    TEXTABLE_303_Textbox_Edge_Top_1,
    TEXTABLE_304_Textbox_Edge_Top_2
};

#define END_OF_ROW -1

typedef enum {
    BoxTexture_Edge_Bottom_1 = 0,
    BoxTexture_Edge_Bottom_2 = 1,
    BoxTexture_Corner_Bottom_Left = 2,
    BoxTexture_Corner_Bottom_Right = 3,
    BoxTexture_Corner_Top_Left = 4,
    BoxTexture_Corner_Top_Right = 5,
    BoxTexture_Edge_Left_1 = 6,
    BoxTexture_Edge_Left_2 = 7,
    BoxTexture_Edge_Left_3 = 8,
    BoxTexture_Centre_1 = 9,
    BoxTexture_Centre_2 = 10,
    BoxTexture_Centre_3 = 11,
    BoxTexture_Centre_4 = 12,
    BoxTexture_Edge_Right_1 = 13,
    BoxTexture_Edge_Right_2 = 14,
    BoxTexture_Edge_Right_3 = 15,
    BoxTexture_Edge_Top_1 = 16,
    BoxTexture_Edge_Top_2 = 17
} BoxTextureIndices;

/*0x830*/ static s16 dBoxTextureIndices[] = {
    BoxTexture_Corner_Top_Left,     BoxTexture_Edge_Top_1,     BoxTexture_Edge_Top_2,     BoxTexture_Edge_Top_2,     BoxTexture_Edge_Top_1,     BoxTexture_Edge_Top_2,     BoxTexture_Edge_Top_2,     BoxTexture_Corner_Top_Right,      END_OF_ROW,
    BoxTexture_Edge_Left_1,         BoxTexture_Centre_1,      BoxTexture_Centre_4,      BoxTexture_Centre_1,      BoxTexture_Centre_3,      BoxTexture_Centre_2,      BoxTexture_Centre_1,      BoxTexture_Edge_Right_1,        END_OF_ROW,
    BoxTexture_Edge_Left_2,        BoxTexture_Centre_1,      BoxTexture_Centre_3,      BoxTexture_Centre_1,      BoxTexture_Centre_1,      BoxTexture_Centre_4,      BoxTexture_Centre_2,      BoxTexture_Edge_Right_2,         END_OF_ROW,
    BoxTexture_Edge_Left_3,        BoxTexture_Centre_2,      BoxTexture_Centre_1,      BoxTexture_Centre_4,      BoxTexture_Centre_2,      BoxTexture_Centre_1,      BoxTexture_Centre_3,      BoxTexture_Edge_Right_3,         END_OF_ROW,
    BoxTexture_Corner_Bottom_Left, BoxTexture_Edge_Bottom_1, BoxTexture_Edge_Bottom_2, BoxTexture_Edge_Bottom_1, BoxTexture_Edge_Bottom_2, BoxTexture_Edge_Bottom_2, BoxTexture_Edge_Bottom_1, BoxTexture_Corner_Bottom_Right,  END_OF_ROW
};
/*0x88C*/ static u8 dSpeakerModes[] = {SPEAKERMODE_STEREO, SPEAKERMODE_SURROUND, SPEAKERMODE_MONO, SPEAKERMODE_HEADPHONE};

/*0x0*/ static s8 sRedrawFrames;                    //The number of updates that should be drawn before idling
/*0x1*/ static s8 sCheatsTopIdx;                    //The index of the first cheat shown in the scrollable box (4 cheats are shown at a time)
/*0x2*/ static s8 sFadeOutActive;                   //For fading out and cutting back to the Title Screen
/*0x3*/ static s8 sFadeOutTimer;                    //Timer for backing out to the Title Screen
/*0x8*/ static char sCheatStrings[CHEATS_PER_SCREEN][50]; //A buffer for the displayed cheats' text
/*0xD0*/ static  Texture* dBoxTextures[18];         //Textures for the multi-tile picmenu boxes, used to frame Front End UI controls
/*0x118*/ static Texture* sBGTex;                   //The page's high-res background image
/*0x11C*/ static Texture* sCropFrameVertical;       //A white frame shown while adjusting the screen position
/*0x120*/ static Texture* sCropFrameHorizontal;     //A white frame shown while adjusting the screen position
/*0x124*/ static GameTextChunk* sGametextCinema;    //Text for the cinema menu
/*0x128*/ static GameTextChunk* sGametextCheats;    //Text for the cheats menu
/*0x12C*/ static s8 sCtrlCount;                     //The number of Front End controllers currently being used (checkboxes/sliders/lists)
/*0x12D*/ static s8 sTopLevelItemIdx;               //The selected item index on the main top-level page of the Options menu (used to restore selection when backing out of a submenu)
/*0x130*/ static FrontEndControl* sCtrls[MAX_CONTROLS_PER_PAGE]; //Pointers to Front End controls (checkboxes/sliders/lists)
/*0x148*/ static GplayOptions* sGameOptions;        //Player's saved game options

static void options_goto_main_page(void);
static void options_goto_control_page(s32 selectedItemIdx);
static void options_goto_video_page(s32 selectedItemIdx);
static int options_exit_main_page(s32 action, s32 selectedItemIdx);
static void options_handle_action_display_page(s32 action, s32 selectedItemIdx);
static void options_handle_action_control_page(s32 action, s32 selectedItemIdx);
static void options_handle_action_cheats_page(s32 action, s32 selectedItemIdx);
static void options_handle_action_video_page(s32 action, s32 selectedItemIdx);
static void options_handle_action_audio_page(s32 action, s32 selectedItemIdx);
static void options_handle_action_cinema_page(s32 action, s32 selectedItemIdx);
static void options_clean_up(void);
static void options_draw_box(Gfx** gdl, s32 initialX, s32 initialY, s32 endX, s32 endY);

// offset: 0x0 | ctor
void options_ctor(void* dll) {
    u32 i;

    gDLL_28_ScreenFade->vtbl->fade_reversed(20, 1);

    sBGTex = tex_load_deferred(TEXTABLE_2DF_Paper_BG_Sabre_Cape);
    sCropFrameHorizontal = tex_load_deferred(TEXTABLE_30E);
    sCropFrameVertical = tex_load_deferred(TEXTABLE_30D);

    if (dGametextMenu == NULL) {
        dGametextMenu = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_0EF_Menu_Main_Menu);
    }

    dGametextControls = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_1F5_Menu_Controls);
    sGametextCheats = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_1F6_Menu_Cheats);
    sGametextCinema = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_1F7_Menu_Cinema);

    for (i = 0; i < ARRAYCOUNT(dBoxTextureIDs); i++) {
        dBoxTextures[i] = tex_load_deferred(dBoxTextureIDs[i]);
    }

    options_goto_main_page();
    sCtrlCount = 0;
    sTopLevelItemIdx = 0;
    sGameOptions = gDLL_29_Gplay->vtbl->get_game_options();
    sRedrawFrames = 2;
    sFadeOutActive = FALSE;
}

// offset: 0x1F0 | dtor
void options_dtor(void *dll) { }

#define FADE_THRESHOLD 13

// offset: 0x1FC | func: 0 | export: 0
s32 options_update1(void) {
    s8 timeBefore;
    s32 selectedIdx;
    s32 i;
    s8 joyX;
    s8 joyY;
    s32 frames;
    s32 action;

    timeBefore = sFadeOutTimer;

    frames = gUpdateRate;
    if (frames > 3) {
        frames = 3;
    }

    if (sFadeOutTimer > 0) {
        sFadeOutTimer -= frames;
    }

    if (gDLL_28_ScreenFade->vtbl->is_complete() == FALSE) {
        gDLL_74_Picmenu->vtbl->redraw_all();
        sRedrawFrames = 2;
    }

    //Handle leaving the Options menu and returning to the Title Screen
    if (sFadeOutActive) {
        if ((timeBefore >= FADE_THRESHOLD) && (sFadeOutTimer < FADE_THRESHOLD)) {
            vi_init(1, get_ossched(), 0);
            options_clean_up();
            track_set_z_buffer_on(1);
            track_set_sky_on(1);
        } else if (sFadeOutTimer <= 0) {
            main_demo_reset();
            main_start_game(12457.1f, -1474.875f, -6690.398f, PLAYER_KRYSTAL);
            menu_set(MENU_TITLE_SCREEN);
            if (1) { }
        }

        if (sFadeOutTimer < FADE_THRESHOLD) {
            return 1;
        } else {
            return 0;
        }
    }

    action = gDLL_74_Picmenu->vtbl->update();
    selectedIdx = gDLL_74_Picmenu->vtbl->get_selected_item();

    switch (dMenuID) {
    case OPTIONS_PAGE_0_Main_Page:
        sTopLevelItemIdx = selectedIdx;
        if (options_exit_main_page(action, selectedIdx)) {
            return 0;
        }
        break;
    case OPTIONS_PAGE_3_Display:
        options_handle_action_display_page(action, selectedIdx);
        if (action == PICMENU_ACTION_BACK) {
            sGameOptions->showSubtitles = gDLL_75->vtbl->get_value(sCtrls[OPTIONS_DISPLAY_0_Subtitles]);
            sGameOptions->showInstruments = gDLL_75->vtbl->get_value(sCtrls[OPTIONS_DISPLAY_1_Instruments]);
            options_goto_main_page();
        }
        break;
    case OPTIONS_PAGE_4_Control:
        options_handle_action_control_page(action, selectedIdx);
        if (action == PICMENU_ACTION_BACK) {
            sGameOptions->zTargetMode = gDLL_75->vtbl->get_value(sCtrls[OPTIONS_CONTROL_0_Z_Button]);
            options_goto_main_page();
        }
        break;
    case OPTIONS_PAGE_5_Cheats:
        options_handle_action_cheats_page(action, selectedIdx);
        if (action == PICMENU_ACTION_BACK) {
            for (i = 0; i < 4; i++) {
                gDLL_29_Gplay->vtbl->set_cheat_enabled(
                    (sCheatsTopIdx + i),
                    gDLL_75->vtbl->get_value(sCtrls[i + 1])
                );
            }
            options_goto_main_page();
        }
        break;
    case OPTIONS_PAGE_1_Video:
        options_handle_action_video_page(action, selectedIdx);
        if (action == PICMENU_ACTION_BACK) {
            sGameOptions->screenSizeAnamorphic = gDLL_75->vtbl->get_value(sCtrls[OPTIONS_VIDEO_0_Screen_Size]);
            sGameOptions->screenAspectRatio = gDLL_75->vtbl->get_value(sCtrls[OPTIONS_VIDEO_1_Ratio]);
            options_goto_main_page();
        }
        break;
    case OPTIONS_PAGE_2_Audio:
        options_handle_action_audio_page(action, selectedIdx);
        if (action == PICMENU_ACTION_BACK) {
            sGameOptions->audioMode = gDLL_75->vtbl->get_value(sCtrls[OPTIONS_AUDIO_0_Setup]);
            sGameOptions->volumeMusic = gDLL_75->vtbl->get_value(sCtrls[OPTIONS_AUDIO_1_Music]);
            sGameOptions->volumeAudio = gDLL_75->vtbl->get_value(sCtrls[OPTIONS_AUDIO_2_SFX]);
            options_goto_main_page();
        }
        break;
    case OPTIONS_PAGE_6_Cinema:
        options_handle_action_cinema_page(action, selectedIdx);
        if (action == PICMENU_ACTION_BACK) {
            options_goto_main_page();
        }
        break;
    case OPTIONS_PAGE_7_View_Layout:
        if (action != PICMENU_ACTION_NONE) {
            tex_free(sBGTex);
            sBGTex = tex_load_deferred(TEXTABLE_2DF_Paper_BG_Sabre_Cape);
            options_goto_control_page(OPTIONS_CONTROL_1_View_Layout);
            return 0;
        }
        break;
    case OPTIONS_PAGE_8_Screen_Position:
        //TODO: crashes in recomp currently!

        joy_get_stick_menu_xy_sign(0, &joyX, &joyY);
        if (joyX > 0) {
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_PICMENU_MOVE, MAX_VOLUME, NULL, NULL, 0, NULL);
            sGameOptions->screenOffsetX += 1;
            if (sGameOptions->screenOffsetX > 7) {
                sGameOptions->screenOffsetX = 7;
                joyX = 0;
            }
        } else if (joyX < 0) {
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_PICMENU_MOVE, MAX_VOLUME, NULL, NULL, 0, NULL);
            sGameOptions->screenOffsetX -= 1;
            if (sGameOptions->screenOffsetX < -7) {
                sGameOptions->screenOffsetX = -7;
                joyX = 0;
            }
        }

        if (joyY > 0) {
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_PICMENU_MOVE, MAX_VOLUME, NULL, NULL, 0, NULL);
            sGameOptions->screenOffsetY -= 1;
            if (sGameOptions->screenOffsetY < -7) {
                sGameOptions->screenOffsetY = -7;
                joyY = 0;
            }
        } else if (joyY < 0) {
            gDLL_6_AMSFX->vtbl->play(NULL, SOUND_PICMENU_MOVE, MAX_VOLUME, NULL, NULL, 0, NULL);
            sGameOptions->screenOffsetY += 1;
            if (sGameOptions->screenOffsetY > 7) {
                sGameOptions->screenOffsetY = 7;
                joyY = 0;
            }
        }

        if ((joyX != 0) || (joyY != 0)) {
            vi_set_modifiers(1, sGameOptions->screenOffsetX, sGameOptions->screenOffsetY);
        }

        if (action != PICMENU_ACTION_NONE) {
            joy_reset_menu_joystick_delay();
            options_goto_video_page(OPTIONS_VIDEO_2_Screen_Position);
            return 0;
        }
        break;
    default:
        break;
    }

    //Update Front End UI controls
    if (dMenuID != OPTIONS_PAGE_0_Main_Page) {
        for (i = 0; i < sCtrlCount; i++) {
            if (sCtrls[i] != NULL) {
                if (i == selectedIdx) {
                    gDLL_75->vtbl->set_selection_state(sCtrls[i], TRUE);
                } else {
                    gDLL_75->vtbl->set_selection_state(sCtrls[i], FALSE);
                }
                gDLL_75->vtbl->update(sCtrls[i]);
            }
        }
    }

    return 0;
}

// offset: 0xB84 | func: 1 | export: 1
void options_update2(void) {

}

// offset: 0xB8C | func: 2 | export: 2
void options_draw(Gfx** gdl, Mtx **mtxs, Vertex **vtxs) {
    s32 i;
    s32 i2;
    s32 uly;
    s32 lry;
    s32 ulx;
    s32 lrx;
    s32 end;
    OptionsPage *submenu;

    submenu = &dMenus[dMenuID];

    if (sFadeOutActive && (sFadeOutTimer <= (FADE_THRESHOLD - 3))) {
        return;
    }

    font_window_set_coords(1, 0, 0,
        GET_VIDEO_WIDTH(vi_get_current_size()),
        GET_VIDEO_HEIGHT(vi_get_current_size())
    );

    font_window_flush_strings(1);

    if (dMenuID == OPTIONS_PAGE_8_Screen_Position) {
        rcp_screen_full_write(gdl, sBGTex,               0,   0,   0, 0, 0xFF, 2);

        rcp_screen_full_write(gdl, sCropFrameVertical,   38,  36,  0, 0, 0xFF, 1);
        rcp_screen_full_write(gdl, sCropFrameHorizontal, 38,  36,  0, 0, 0xFF, 1);
        rcp_screen_full_write(gdl, sCropFrameVertical,   599, 36,  0, 0, 0xFF, 1);
        rcp_screen_full_write(gdl, sCropFrameHorizontal, 38,  438, 0, 0, 0xFF, 1);

        font_window_use_font(1, FONT_DINO_MEDIUM_FONT_IN);
        font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, gDLL_74_Picmenu->vtbl->get_highlight_alpha());
        font_window_add_string_xy(1, 320, 42,  &dDownArrowChar,  1, ALIGN_TOP_CENTER);
        font_window_add_string_xy(1, 320, 400, &dUpArrowChar,    1, ALIGN_TOP_CENTER);
        font_window_add_string_xy(1, 48,  235, &dLeftArrowChar,  1, ALIGN_TOP_LEFT);
        font_window_add_string_xy(1, 578, 235, &dRightArrowChar, 1, ALIGN_TOP_LEFT);
    } else if (dMenuID == OPTIONS_PAGE_7_View_Layout) {
        rcp_screen_full_write(gdl, sBGTex, 0, 0, 0, 0, 0xFF, 2);

        //Headings (and 4 C-button arrows)
        font_window_use_font(1, FONT_FUN_FONT);
        i2 = ARRAYCOUNT(dLayoutCoordsHeadings) + ARRAYCOUNT(dLayoutCoordsDescriptions);
        for (end = ARRAYCOUNT(dLayoutCoordsHeadings), i = 0; i < end;) {
            //Print text
            font_window_set_text_colour(1, 0xE1, 0xAB, 0x61, 0xFF, 0xFF);
            font_window_add_string_xy(1, dLayoutCoordsHeadings[i][0], dLayoutCoordsHeadings[i][1], dGametextControls->strings[i], 1, ALIGN_TOP_LEFT);

            //Print drop-shadow
            font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0x96);
            font_window_add_string_xy(1, dLayoutCoordsHeadings[i][0] - 1, dLayoutCoordsHeadings[i][1] - 1, dGametextControls->strings[i], 2, ALIGN_TOP_LEFT);

            i++;
        }

        //Descriptions
        font_window_use_font(1, FONT_DINO_SUBTITLE_FONT_1);
        for (end = i2, i2 = 0; i < end;) {
            //Print text
            font_window_set_text_colour(1, 0xB7, 0x8B, 0x61, 0xFF, 0xFF);
            font_window_add_string_xy(1, dLayoutCoordsDescriptions[i2][0], dLayoutCoordsDescriptions[i2][1], dGametextControls->strings[i], 1, ALIGN_TOP_LEFT);

            //Print drop-shadow
            font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0x96);
            font_window_add_string_xy(1, dLayoutCoordsDescriptions[i2][0] - 1, dLayoutCoordsDescriptions[i2][1] - 1, dGametextControls->strings[i], 2, ALIGN_TOP_LEFT);

            i++;
            i2++;
        }

    } else {
        if (sRedrawFrames) {
            rcp_screen_full_write(gdl, sBGTex, 0, 0, 0, 0, 0xFF, 2);

            font_window_use_font(1, FONT_DINO_MEDIUM_FONT_IN);

            //Print "OPTIONS" title, with drop-shadow
            {
                font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, 0xFF);
                font_window_add_string_xy(1, 568, 63, dGametextMenu->strings[LINE_1_Options], 1, ALIGN_TOP_RIGHT);

                font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0xFF);
                font_window_add_string_xy(1, 563, 58, dGametextMenu->strings[LINE_1_Options], 2, ALIGN_TOP_RIGHT);
            }

            //Print a heading label for the page's box, if it uses one
            if (submenu->boxLabel != NONE) {
                font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, 0xFF);
                font_window_add_string_xy(1, 83, 178, dGametextMenu->strings[submenu->boxLabel], 1, ALIGN_TOP_LEFT);

                font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0xFF);
                font_window_add_string_xy(1, 78, 173, dGametextMenu->strings[submenu->boxLabel], 2, ALIGN_TOP_LEFT);
            }

            //Print the menu navigation info, if the page uses it (A-SELECT, B-CANCEL)
            if (submenu->navigationInfo != NONE) {
                font_window_use_font(1, FONT_FUN_FONT);

                font_window_set_text_colour(1, 0xB7, 0x8B, 0x61, 0xFF, 0xFF);
                font_window_add_string_xy(1, 320, 405, dGametextMenu->strings[submenu->navigationInfo], 1, ALIGN_TOP_CENTER);

                font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0xFF);
                font_window_add_string_xy(1, 318, 403, dGametextMenu->strings[submenu->navigationInfo], 2, ALIGN_TOP_CENTER);
            }

            //Draw the box for the page's controls, if it uses one
            if (submenu->boxLabel != NONE) {
                options_draw_box(gdl, 56, 220, 0, 480);
            }
        } else {
            func_80010158(&ulx, &lrx, &uly, &lry);
            rcp_screen_scroll_write(gdl, sBGTex, 0, 0, uly, lry, 0xFF, 2);

            if (submenu->boxLabel != NONE) {
                options_draw_box(gdl, 56, 220, uly, lry);
            }

            if ((dMenuID == OPTIONS_PAGE_0_Main_Page) && (uly < 100)) {
                font_window_use_font(1, FONT_DINO_MEDIUM_FONT_IN);

                font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, 0xFF);
                font_window_add_string_xy(1, 568, 63, dGametextMenu->strings[LINE_1_Options], 1, ALIGN_TOP_RIGHT);

                font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0xFF);
                font_window_add_string_xy(1, 563, 58, dGametextMenu->strings[LINE_1_Options], 2, ALIGN_TOP_RIGHT);
            }
        }

        for (i = 0; i < sCtrlCount; i++) {
            if (sCtrls[i] != NULL) {
                gDLL_75->vtbl->draw(sCtrls[i], gdl);
            }
        }

    }

    gDLL_74_Picmenu->vtbl->draw(gdl);
    font_window_draw(gdl, NULL, NULL, 1);

    sRedrawFrames--;
    if (sRedrawFrames < 0) {
        sRedrawFrames = 0;
    }
}

// offset: 0x16A4 | func: 3
static void options_set_up_menu_strings(OptionsPage* submenu) {
    s32 i;

    for (i = 0; i < submenu->count; i++) {
        submenu->menuItems[i].text = dGametextMenu->strings[submenu->textIDs[i]];
    }
}

// offset: 0x1718 | func: 4
void options_goto_main_page(void) {
    OptionsPage* submenu;
    s32 i;

    if (dMenuID != NONE) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }

    dMenuID = OPTIONS_PAGE_0_Main_Page;
    submenu = &dMenus[dMenuID];
    options_set_up_menu_strings(submenu);
    gDLL_74_Picmenu->vtbl->set_items(
        submenu->menuItems, submenu->count,
        sTopLevelItemIdx, NULL,
        submenu->unkC, submenu->unkD,
        0xB7, 0x8B, 0x61,
        0xFF, 0xD7, 0x3D
    );

    for (i = 0; i < sCtrlCount; i++) {
        if (sCtrls[i]) {
            gDLL_75->vtbl->free(sCtrls[i]);
        }
        sCtrls[i] = NULL;
    }

    sCtrlCount = 0;
    sRedrawFrames = 2;
}

// offset: 0x1898 | func: 5
static void options_goto_display_page(void) {
    OptionsPage* submenu;
    char** strings;

    if (dMenuID != NONE) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }

    dMenuID = OPTIONS_PAGE_3_Display;
    submenu = &dMenus[dMenuID];
    options_set_up_menu_strings(submenu);
    gDLL_74_Picmenu->vtbl->set_items(
        submenu->menuItems, submenu->count,
        0, NULL,
        submenu->unkC, submenu->unkD,
        0xB7, 0x8B, 0x61,
        0xFF, 0xD7, 0x3D
    );

    strings = dDisplayChoiceStrings;
    strings[0] = dGametextMenu->strings[LINE_10_Off];
    strings[1] = dGametextMenu->strings[LINE_9_On];

    sCtrlCount = 0;
    sCtrls[sCtrlCount] = (FrontEndControl*)gDLL_75->vtbl->create_list(476, 254, 0, 1, sGameOptions->showSubtitles, strings, 50);
    sCtrlCount++;
    sCtrls[sCtrlCount] = (FrontEndControl*)gDLL_75->vtbl->create_list(476, 278, 0, 1, sGameOptions->showInstruments, strings, 50);
    sCtrlCount++;

    gDLL_75->vtbl->set_selection_state(sCtrls[0], 1);
    sRedrawFrames = 2;
}

// offset: 0x1AE4 | func: 6
static void options_goto_control_page(s32 selectedItemIdx) {
    OptionsPage* submenu;

    if (dMenuID != NONE) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }

    dMenuID = OPTIONS_PAGE_4_Control;
    submenu = &dMenus[dMenuID];
    options_set_up_menu_strings(submenu);
    gDLL_74_Picmenu->vtbl->set_items(
        submenu->menuItems, submenu->count,
        selectedItemIdx, NULL,
        submenu->unkC, submenu->unkD,
        0xB7, 0x8B, 0x61,
        0xFF, 0xD7, 0x3D
    );

    dControlZButtonStrings[0] = dGametextMenu->strings[LINE_14_Combo];
    dControlZButtonStrings[1] = dGametextMenu->strings[LINE_15_Tap];
    dControlZButtonStrings[2] = dGametextMenu->strings[LINE_16_Hold];

    sCtrlCount = 0;
    sCtrls[sCtrlCount] = (FrontEndControl*)gDLL_75->vtbl->create_list(426, 254, 0, 2, sGameOptions->zTargetMode, dControlZButtonStrings, 100);
    sCtrlCount++;
    sCtrls[sCtrlCount] = NULL;
    sCtrlCount++;

    if (sCtrls[selectedItemIdx]) {
        gDLL_75->vtbl->set_selection_state(sCtrls[selectedItemIdx], 1);
    }

    sRedrawFrames = 2;
}

// offset: 0x1CF4 | func: 7
static void options_goto_cheats_page(void) {
    OptionsPage *submenu;
    s32 enabled;
    s32 unlocked;
    u32 i;
    u32 y;

    if (dMenuID != NONE) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }

    dMenuID = OPTIONS_PAGE_5_Cheats;
    submenu = &dMenus[dMenuID];

    sCtrlCount = 0;
    sCheatsTopIdx = 0;
    y = 252;
    sCtrls[sCtrlCount] = NULL;
    sCtrlCount++;

    //Set up the four cheats displayed in the box
    for (i = 0; i < CHEATS_PER_SCREEN; i++) {
        unlocked = gDLL_29_Gplay->vtbl->is_cheat_unlocked(sCheatsTopIdx + i);

        if (unlocked) {
            sprintf(sCheatStrings[i], "%2d: %s", (int) ((sCheatsTopIdx + i) + 1), sGametextCheats->strings[sCheatsTopIdx + i]);
            submenu->menuItems[i + 1].text = sCheatStrings[i];
            submenu->menuItems[i + 1].flags &= ~(PICMENU_TRANSPARENT | PICMENU_DISABLED);
        } else {
            sprintf(sCheatStrings[i], "%2d:", (int) ((sCheatsTopIdx + i) + 1));
            submenu->menuItems[i + 1].text = sCheatStrings[i];
            submenu->menuItems[i + 1].flags |= (PICMENU_TRANSPARENT | PICMENU_DISABLED);
        }

        enabled = gDLL_29_Gplay->vtbl->is_cheat_active(sCheatsTopIdx + i);
        sCtrls[sCtrlCount] = (FrontEndControl*)gDLL_75->vtbl->create_checkbox(
            513, (s16) y, 
            0, 1, 
            enabled ? 1 : 0
        );
        sCtrlCount++;
        gDLL_75->vtbl->set_lock_state(sCtrls[sCtrlCount - 1], unlocked);

        y += 26;
    }

    sCtrls[sCtrlCount] = NULL;
    sCtrlCount++;

    //Don't allow scrolling up when starting at the top
    submenu->menuItems[0].flags |= PICMENU_INTANGIBLE;

    gDLL_74_Picmenu->vtbl->set_items(
        submenu->menuItems, submenu->count,
        1, NULL,
        submenu->unkC, submenu->unkD,
        0xB7, 0x8B, 0x61,
        0xFF, 0xD7, 0x3D
    );

    gDLL_75->vtbl->set_selection_state(sCtrls[OPTIONS_CHEATS_1_CheatShown1], 1);

    sRedrawFrames = 2;
}

// offset: 0x2088 | func: 8
static void options_goto_video_page(s32 selectedItemIdx) {
    OptionsPage* submenu;
    u8 languageID = gDLL_21_Gametext->vtbl->curr_bank();

    if (dMenuID != NONE) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }

    dMenuID = OPTIONS_PAGE_1_Video;
    submenu = &dMenus[dMenuID];
    options_set_up_menu_strings(submenu);
    gDLL_74_Picmenu->vtbl->set_items(
        submenu->menuItems, submenu->count,
        selectedItemIdx, NULL,
        submenu->unkC, submenu->unkD,
        0xB7, 0x8B, 0x61,
        0xFF, 0xD7, 0x3D
    );

    dVideoSizeStrings[0] = dGametextMenu->strings[LINE_22_Full];
    dVideoSizeStrings[1] = dGametextMenu->strings[LINE_23_Wide];
    dVideoSizeStrings[2] = dGametextMenu->strings[LINE_24_Cinema];

    dVideoRatioStrings[0] = dGametextMenu->strings[LINE_26_Normal];
    dVideoRatioStrings[1] = dGametextMenu->strings[LINE_27_16_by_9];

    sCtrlCount = 0;
    if ((languageID == LANGUAGE_ESPANOL) || (languageID == LANGUAGE_ITALIANO)) {
        sCtrls[sCtrlCount] = (FrontEndControl*)gDLL_75->vtbl->create_list(451, 254, 0, 2, sGameOptions->screenSizeAnamorphic, dVideoSizeStrings, 75);
        sCtrlCount++;
        sCtrls[sCtrlCount] = (FrontEndControl*)gDLL_75->vtbl->create_list(451, 278, 0, 1, sGameOptions->screenAspectRatio, dVideoRatioStrings, 75);
        sCtrlCount++;
    } else {
        sCtrls[sCtrlCount] = (FrontEndControl*)gDLL_75->vtbl->create_list(426, 254, 0, 2, sGameOptions->screenSizeAnamorphic, dVideoSizeStrings, 100);
        sCtrlCount++;
        sCtrls[sCtrlCount] = (FrontEndControl*)gDLL_75->vtbl->create_list(426, 278, 0, 1, sGameOptions->screenAspectRatio, dVideoRatioStrings, 100);
        sCtrlCount++;
    }

    sCtrls[sCtrlCount] = NULL;
    sCtrlCount++;

    if (sCtrls[selectedItemIdx]) {
        gDLL_75->vtbl->set_selection_state(sCtrls[selectedItemIdx], 1);
    }

    sRedrawFrames = 2;
}

// offset: 0x2438 | func: 9
static void options_goto_audio_page(void) {
    OptionsPage* submenu;

    if (dMenuID != NONE) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }
    dMenuID = OPTIONS_PAGE_2_Audio;

    submenu = &dMenus[dMenuID];
    options_set_up_menu_strings(submenu);
    gDLL_74_Picmenu->vtbl->set_items(
        submenu->menuItems, submenu->count,
        0, NULL,
        submenu->unkC, submenu->unkD,
        0xB7, 0x8B, 0x61,
        0xFF, 0xD7, 0x3D
    );

    dAudioSetupStrings[0] = dGametextMenu->strings[LINE_31_Stereo];
    dAudioSetupStrings[1] = dGametextMenu->strings[LINE_32_Surround];
    dAudioSetupStrings[2] = dGametextMenu->strings[LINE_33_Mono];
    dAudioSetupStrings[3] = dGametextMenu->strings[LINE_34_Headphones];

    sCtrlCount = 0;
    sCtrls[sCtrlCount] = (FrontEndControl*)gDLL_75->vtbl->create_list(426, 254, 0, 3, sGameOptions->audioMode, dAudioSetupStrings, 100);
    sCtrlCount++;
    sCtrls[sCtrlCount] = (FrontEndControl*)gDLL_75->vtbl->create_slider(318, 285, 0, 0xff, sGameOptions->volumeMusic);
    sCtrlCount++;
    sCtrls[sCtrlCount] = (FrontEndControl*)gDLL_75->vtbl->create_slider(318, 309, 0, MAX_VOLUME, sGameOptions->volumeAudio);
    sCtrlCount++;

    gDLL_75->vtbl->set_selection_state(sCtrls[0], 1);
    sRedrawFrames = 2;
}

// offset: 0x26D8 | func: 10
static void options_goto_view_layout_page(void) {
    OptionsPage* submenu;
    s32 i;

    if (dMenuID != NONE) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }

    dMenuID = OPTIONS_PAGE_7_View_Layout;
    submenu = &dMenus[dMenuID];
    options_set_up_menu_strings(submenu);
    gDLL_74_Picmenu->vtbl->set_items(
        submenu->menuItems, submenu->count,
        0, NULL,
        submenu->unkC, submenu->unkD,
        0xB7, 0x8B, 0x61,
        0xFF, 0xD7, 0x3D
    );

    for (i = 0; i < sCtrlCount; i++) {
        if (sCtrls[i] != NULL) {
            gDLL_75->vtbl->free(sCtrls[i]);
        }
        sCtrls[i] = NULL;
    }

    tex_free(sBGTex);
    sBGTex = tex_load_deferred(TEXTABLE_2E0_Paper_BG_Nintendo64_Controller);
    sCtrlCount = 0;
    sRedrawFrames = 2;
}

// offset: 0x2888 | func: 11
static void options_goto_screen_position_page(void) {
    OptionsPage* submenu;
    s32 i;

    if (dMenuID != NONE) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }

    dMenuID = OPTIONS_PAGE_8_Screen_Position;
    submenu = &dMenus[dMenuID];
    options_set_up_menu_strings(submenu);
    gDLL_74_Picmenu->vtbl->set_items(
        submenu->menuItems, submenu->count,
        0, NULL,
        submenu->unkC, submenu->unkD,
        0xB7, 0x8B, 0x61,
        0xFF, 0xD7, 0x3D
    );

    for (i = 0; i < sCtrlCount; i++) {
        if (sCtrls[i] != NULL) {
            gDLL_75->vtbl->free(sCtrls[i]);
        }
        sCtrls[i] = NULL;
    }

    joy_set_menu_joystick_delay(15);
    sCtrlCount = 0;
    sRedrawFrames = 2;
}

// offset: 0x2A1C | func: 12
int options_exit_main_page(s32 action, s32 selectedItemIdx) {
    if (action == PICMENU_ACTION_SELECT) {
        switch (selectedItemIdx) {
        case MAIN_PAGE_INDEX(OPTIONS_PAGE_3_Display):
            options_goto_display_page();
            return 1;
        case MAIN_PAGE_INDEX(OPTIONS_PAGE_4_Control):
            options_goto_control_page(OPTIONS_CONTROL_0_Z_Button);
            return 1;
        case MAIN_PAGE_INDEX(OPTIONS_PAGE_5_Cheats):
            options_goto_cheats_page();
            return 1;
        case MAIN_PAGE_INDEX(OPTIONS_PAGE_1_Video):
            options_goto_video_page(OPTIONS_VIDEO_0_Screen_Size);
            return 1;
        case MAIN_PAGE_INDEX(OPTIONS_PAGE_2_Audio):
            options_goto_audio_page();
            return 1;
        //NOTE: Cinema page not handled
        }
    } else if (action == PICMENU_ACTION_BACK) {
        //Start fadeout back to Title Screen
        gDLL_28_ScreenFade->vtbl->fade(20, 1);
        sFadeOutTimer = 35;
        sFadeOutActive = TRUE;
    }

    return 0;
}

// offset: 0x2B50 | func: 13
void options_handle_action_display_page(s32 action, s32 selectedItemIdx) {
    if (!gDLL_75->vtbl->did_value_change(sCtrls[selectedItemIdx])) {
        return;
    }

    switch (selectedItemIdx) {
    case OPTIONS_DISPLAY_0_Subtitles:
        gDLL_22_Subtitles->vtbl->func_2D0(gDLL_75->vtbl->get_value(sCtrls[selectedItemIdx]));
        break;
    case OPTIONS_DISPLAY_1_Instruments:
        gDLL_1_cmdmenu->vtbl->toggle_forced_stats_display(gDLL_75->vtbl->get_value(sCtrls[selectedItemIdx]));
        break;
    }
}

// offset: 0x2C58 | func: 14
void options_handle_action_control_page(s32 action, s32 selectedItemIdx) {
    if (selectedItemIdx == OPTIONS_CONTROL_0_Z_Button) {
        if (gDLL_75->vtbl->did_value_change(sCtrls[selectedItemIdx])) {
            gDLL_2_Camera->vtbl->apply_target_flags(gDLL_75->vtbl->get_value(sCtrls[selectedItemIdx]));
        }
    } else if (action == PICMENU_ACTION_SELECT) {
        sGameOptions->zTargetMode = gDLL_75->vtbl->get_value(sCtrls[OPTIONS_CONTROL_0_Z_Button]);
        options_goto_view_layout_page();
    }
}

// offset: 0x2D50 | func: 15
void options_handle_action_cheats_page(s32 action, s32 selectedItemIdx) {
    OptionsPage* submenu;
    s32 i;
    s32 cheatsHaveScrolled;
    s32 enabled;
    u8 previousCheatsTopIdx;

    //@debug: Unlock the selected cheat with C-right! (Need to back out and re-enter menu before it shows up)
    if (joy_get_pressed(0) & R_CBUTTONS) {
        gDLL_29_Gplay->vtbl->unlock_cheat(((sCheatsTopIdx + selectedItemIdx) - 1));
    }

    if (action != PICMENU_ACTION_SELECT) {
        return;
    }

    previousCheatsTopIdx = sCheatsTopIdx;

    //Check if the user pressed a scroll arrow
    cheatsHaveScrolled = FALSE;
    if (selectedItemIdx == 0) {
        //Scroll up arrow selected
        sCheatsTopIdx -= CHEATS_PER_SCREEN;
        if (sCheatsTopIdx < 0) {
            sCheatsTopIdx = 0;
        } else {
            cheatsHaveScrolled = TRUE;
        }
    } else if (selectedItemIdx == (CHEATS_PER_SCREEN + 1)) {
        //Scroll down arrow selected (NOTE: causes a crash because the Gametext file only has 7 strings)
        sCheatsTopIdx += CHEATS_PER_SCREEN;
        if (sCheatsTopIdx > CHEATS_FIRST_IDX_LAST_GROUP) {
            sCheatsTopIdx = CHEATS_FIRST_IDX_LAST_GROUP;
        } else {
            cheatsHaveScrolled = TRUE;
        }
    }

    if (cheatsHaveScrolled == FALSE) {
        return;
    }

    submenu = &dMenus[dMenuID];

    //Show the up/down navigation arrows when it's possible to scroll
    submenu->menuItems[0].flags &= ~PICMENU_INTANGIBLE;
    submenu->menuItems[(CHEATS_PER_SCREEN + 1)].flags &= ~PICMENU_INTANGIBLE;
    if (sCheatsTopIdx == 0) {
        submenu->menuItems[0].flags |= PICMENU_INTANGIBLE;
    }
    if (sCheatsTopIdx == CHEATS_FIRST_IDX_LAST_GROUP) {
        submenu->menuItems[(CHEATS_PER_SCREEN + 1)].flags |= PICMENU_INTANGIBLE;
    }

    //Enable/disable the 4 visible cheats based on their checkbox control's state 
    for (i = 0; i < CHEATS_PER_SCREEN; i++) {
        gDLL_29_Gplay->vtbl->set_cheat_enabled(
            previousCheatsTopIdx + i, 
            gDLL_75->vtbl->get_value(sCtrls[i + 1])
        );
    }

    //Update the cheats' strings and checkbox controls
    for (i = 0; i < CHEATS_PER_SCREEN; i++) {
        if (gDLL_29_Gplay->vtbl->is_cheat_unlocked(sCheatsTopIdx + i)) {
            sprintf(sCheatStrings[i], "%2d: %s", (int)(sCheatsTopIdx + i + 1), sGametextCheats->strings[sCheatsTopIdx + i]);
            submenu->menuItems[i + 1].text = sCheatStrings[i];
            submenu->menuItems[i + 1].flags &= ~(PICMENU_TRANSPARENT | PICMENU_DISABLED);
            gDLL_75->vtbl->set_lock_state(sCtrls[i + 1], 1);

            enabled = gDLL_29_Gplay->vtbl->is_cheat_active(sCheatsTopIdx + i);
            gDLL_75->vtbl->set_value(sCtrls[i + 1], enabled ? 1 : 0);
        } else {
            sprintf(sCheatStrings[i], "%2d:", (int)(sCheatsTopIdx + i + 1));
            submenu->menuItems[i + 1].text = sCheatStrings[i];
            submenu->menuItems[i + 1].flags |= (PICMENU_TRANSPARENT | PICMENU_DISABLED);
            gDLL_75->vtbl->set_lock_state(sCtrls[i + 1], 0);
            gDLL_75->vtbl->set_value(sCtrls[i + 1], 0);
        }
    }

    gDLL_74_Picmenu->vtbl->update_text(submenu->menuItems);
    gDLL_74_Picmenu->vtbl->update_flags(submenu->menuItems);

    if (sCheatsTopIdx == 0) {
        gDLL_75->vtbl->set_selection_state(sCtrls[OPTIONS_CHEATS_1_CheatShown1], 1);
        gDLL_74_Picmenu->vtbl->set_selected_item(OPTIONS_CHEATS_1_CheatShown1);
    } else if (sCheatsTopIdx == CHEATS_FIRST_IDX_LAST_GROUP) {
        gDLL_75->vtbl->set_selection_state(sCtrls[OPTIONS_CHEATS_4_CheatShown4], 1);
        gDLL_74_Picmenu->vtbl->set_selected_item(OPTIONS_CHEATS_4_CheatShown4);
    }

    sRedrawFrames = 2;
}

// offset: 0x320C | func: 16
void options_handle_action_video_page(s32 action, s32 selectedItemIdx) {
    if ((action != PICMENU_ACTION_SELECT) || (selectedItemIdx != OPTIONS_VIDEO_2_Screen_Position)) {
        return;
    }

    sGameOptions->screenSizeAnamorphic = gDLL_75->vtbl->get_value(sCtrls[OPTIONS_VIDEO_0_Screen_Size]);
    sGameOptions->screenAspectRatio = gDLL_75->vtbl->get_value(sCtrls[OPTIONS_VIDEO_1_Ratio]);
    options_goto_screen_position_page();
}

// offset: 0x32B8 | func: 17
void options_handle_action_audio_page(s32 action, s32 selectedItemIdx) {
    if (gDLL_75->vtbl->did_value_change(sCtrls[selectedItemIdx]) == FALSE) {
        return;
    }

    switch (selectedItemIdx) {
    case OPTIONS_AUDIO_0_Setup:
        speaker_set_mode(dSpeakerModes[gDLL_75->vtbl->get_value(sCtrls[selectedItemIdx])]);
        break;
    case OPTIONS_AUDIO_2_SFX:
        gDLL_6_AMSFX->vtbl->func_7E4(gDLL_75->vtbl->get_value(sCtrls[selectedItemIdx]));
        break;
    case OPTIONS_AUDIO_1_Music:
        gDLL_5_AMSEQ->vtbl->set_volume_option(gDLL_75->vtbl->get_value(sCtrls[selectedItemIdx]));
        break;
    }
}

// offset: 0x3404 | func: 18
void options_handle_action_cinema_page(s32 action, s32 selectedItemIdx) {

}

// offset: 0x3414 | func: 19
void options_clean_up(void) {
    Texture** textures;
    Texture* tex;
    s32 i;

    if (dGametextMenu != NULL) {
        mmFree(dGametextMenu);
        dGametextMenu = NULL;
    }

    mmFree(dGametextControls);
    mmFree(sGametextCheats);
    mmFree(sGametextCinema);

    if (dMenuID != NONE) {
        gDLL_74_Picmenu->vtbl->clear_items();
        dMenuID = NONE;
    }

    textures = dBoxTextures;
    do {
        tex = *textures;
        if (tex != NULL) {
            tex_free(tex);
            *textures = NULL;
        }
        textures++;
    } while ((u32) textures < (u32) &sBGTex);

    tex_free(sBGTex);
    tex_free(sCropFrameHorizontal);
    tex_free(sCropFrameVertical);

    for (i = 0; i < sCtrlCount; i++) {
        if (sCtrls[i] != NULL) {
            gDLL_75->vtbl->free(sCtrls[i]);
        }
        sCtrls[i] = NULL;
    }

    sCtrlCount = 0;
}

// offset: 0x35D0 | func: 20
void options_draw_box(Gfx** gdl, s32 initialX, s32 initialY, s32 endX, s32 endY) {
    u32 end;
    u32 x;
    u32 y;
    s16 *texID;
    u32 i;

    end = 45;

    x = initialX;
    y = initialY;

    for (i = 0; i < end; i++) {
        texID = &dBoxTextureIndices[(s32)i];
        if ((*texID) == END_OF_ROW) {
            x = initialX;
            y += 32;
        } else {
            rcp_screen_scroll_write(gdl, dBoxTextures[*texID], x, y, endX, endY, 0xFF, SCREEN_WRITE_TRANSLUCENT);
            x += 64;
        }
    }
}
