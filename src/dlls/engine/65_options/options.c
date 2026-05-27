#include "common.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/74_picmenu.h"
#include "game/gametexts.h"
#include "sys/fonts.h"
#include "sys/audio/speaker.h"
#include "sys/menu.h"

#define NONE -1

/*0x0*/ static u8 data_0 = 171;
/*0x4*/ static u8 data_4 = 187;
/*0x8*/ static u8 data_8 = 60;
/*0xC*/ static u8 data_C = 62;

/*0x10*/ static PicMenuItem data_10[] = {
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
/*0x178*/ static s8 data_178[] = {
    5, 6, 2, 3, 4, 7 //Text lineIDs?
};

/*0x180*/ static PicMenuItem data_180[] = {
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
/*0x1F8*/ static s8 data_1F8[] = {8, 11}; //Text lineIDs?

/*0x1FC*/ static char* data_1FC[] = {
    NULL, NULL
};
/*0x204*/ static PicMenuItem data_204[] = {
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
/*0x27C*/ static s8 data_27C[] = {13, 17};  //Text lineIDs?
/*0x280*/ static char* data_280[] = {
    NULL, NULL, NULL
};

#define NEW_PICMENU_ITEM_HASTEXT(textPtr, textX, textY, innerWidth, itemX, itemY, textureID, outerWidth, flags, fontID, highlightFontID, upLink, downLink, leftLink, rightLink, overrideWith) \
    {(void*)textPtr, textX, textY, innerWidth, itemX, itemY, {((void *)textureID)}, outerWidth, flags, fontID, highlightFontID, upLink, downLink, leftLink, rightLink, overrideWith, {0}, 0}

/*0x28C*/ static PicMenuItem data_28C[] = {
    /*0*/ NEW_PICMENU_ITEM_HASTEXT(
        &data_0, 
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
        &data_4, 
        /*textX*/ 310, /*textY*/ 356, /*innerWidth*/ 0, 
        /*itemX*/ 310, /*itemY*/ 356, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_ALIGN_TEXT_CENTER), 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ 4, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    )
};

/*0x3F4*/ static PicMenuItem data_3F4[] = {
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

    /*3*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 326, /*innerWidth*/ 0, 
        /*itemX*/ 83, /*itemY*/ 326, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ 2, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    )
};
/*0x4E4*/ static s8 data_4E4[] = {21, 25, 28, 29};  //Text lineIDs?

/*0x4E8*/ static char* data_4E8[] = {
    NULL, NULL, NULL
};
/*0x4F4*/ static char* data_4F4[] = {
    NULL, NULL
};
/*0x4FC*/ static PicMenuItem data_4FC[] = {
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

    /*3*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 326, /*innerWidth*/ 0, 
        /*itemX*/ 83, /*itemY*/ 326, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ 2, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    )
};
/*0x5EC*/ static s8 data_5EC[] = {30, 35, 36, 37}; //Text lineIDs?

/*0x5F0*/ static char* data_5F0[] = {
    NULL, NULL, NULL, NULL
};
/*0x600*/ static PicMenuItem data_600[] = {
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
/*0x678*/ static u32 data_678 = 0;
/*0x67C*/ static u32 data_67C = 0;
/*0x680*/ static u32 data_680 = 0;
/*0x684*/ static PicMenuItem data_684[] = {
    /*0*/ NEW_PICMENU_ITEM(
        /*textX*/ 265, /*textY*/ 392, /*innerWidth*/ 0, 
        /*itemX*/ 265, /*itemY*/ 380, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | PICMENU_ALIGN_TEXT_CENTER),
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),
};
/*0x6C0*/ static s8 data_6C0[] = {40};
/*0x6C4*/ static PicMenuItem data_6C4[] = {
    /*0*/ NEW_PICMENU_ITEM( 
        /*textX*/ 320, /*textY*/ 237, /*innerWidth*/ 0,
        /*itemX*/ 320, /*itemY*/ 225, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_ALIGN_ITEM_CENTER | PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | PICMENU_ALIGN_TEXT_CENTER), 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),
};
/*0x700*/ static s8 data_700[] = {40};

typedef struct {
    PicMenuItem *menuItems;
    s8 *textIDs;
    u8 count;
    s8 unk9;
    s8 unkA;
    u8 unkB;
    u8 unkC;
    u8 unkD;
} OptionsSubmenu;

/*0x704*/ static OptionsSubmenu data_704[] = {
    {data_10, data_178, ARRAYCOUNT(data_10), -1, 0, 4, 5, 4},
    {data_3F4, data_4E4, 3, 5, 0, 3, 0, 0},
    {data_4FC, data_5EC, 3, 6, 0, 3, 0, 0},
    {data_180, data_1F8, ARRAYCOUNT(data_180), 2, 0, 3, 0, 0},
    {data_204, data_27C, ARRAYCOUNT(data_204), 3, 0, 3, 0, 0},
    {data_28C, NULL, ARRAYCOUNT(data_28C), 4, 0, 3, 0, 0},
    {data_600, NULL, ARRAYCOUNT(data_600), 7, 0, 3, 0, 0},
    {data_684, data_6C0, ARRAYCOUNT(data_684), -1, -1, 5, 5, 4},
    {data_6C4, data_700, ARRAYCOUNT(data_6C4), -1, -1, 5, 5, 4}
};
/*0x794*/ static GameTextChunk* data_794 = NULL;
/*0x798*/ static GameTextChunk* data_798 = NULL; //Controls text
/*0x79C*/ static s8 data_79C = -1;
/*0x7A0*/ static u16 data_7A0[][2] = {
    {0x01d6, 0x00c4}, 
    {0x01d0, 0x011c}, 
    {0x01cc, 0x003e}, 
    {0x019b, 0x0175}, 
    {0x003f, 0x008d}, 
    {0x003f, 0x013f}, 
    {0x013a, 0x003e}, 
    {0x01d9, 0x0096}, 
    {0x01d9, 0x0071}, 
    {0x01d6, 0x005c}, 
    {0x01d6, 0x0082},
// };
// /*0x7CC*/ static u16 data_7CC[][2] = {
    {0x01ea, 0x0059}, 
    {0x01ea, 0x0073}, 
    {0x01ea, 0x0086}, 
    {0x01ea, 0x0098}, 
    {0x01d8, 0x00de}, 
    {0x01d8, 0x00ef}, 
    {0x01ce, 0x0100}, 
    {0x01cc, 0x0133}, 
    {0x01c6, 0x0145}, 
    {0x01ef, 0x0154}, 
    {0x019d, 0x018a}, 
    {0x019d, 0x0199}, 
    {0x0041, 0x0153}, 
    {0x0041, 0x0162}, 
    {0x0041, 0x00a3}, 
    {0x0131, 0x0053}
};
/*0x80C*/ static s16 data_80C[] = {
    0x0320, 0x02f5, 0x02f6, 0x02f7, 0x02f8, 0x02f9, 0x02fa, 0x02fb, 0x02fc, 0x0313, 0x02fd, 0x02fe, 0x02ff, 0x0300, 0x0301, 0x0302, 
    0x0303, 0x0304
};
/*0x830*/ static s16 data_830[] = {
    0x0004, 0x0010, 0x0011, 0x0011, 0x0010, 0x0011, 0x0011, 0x0005, 
    0xffff, 0x0006, 0x0009, 0x000c, 0x0009, 0x000b, 0x000a, 0x0009,
    0x000d, 0xffff, 0x0007, 0x0009, 0x000b, 0x0009, 0x0009, 0x000c, 
    0x000a, 0x000e, 0xffff, 0x0008, 0x000a, 0x0009, 0x000c, 0x000a,
    0x0009, 0x000b, 0x000f, 0xffff, 0x0002, 0x0000, 0x0001, 0x0000, 
    0x0001, 0x0001, 0x0000, 0x0003, 0xffff, 0x0000
};
/*0x88C*/ static u8 data_88C[] = {0x02, 0x04, 0x01, 0x03};

/*0x0*/ static s8 bss_0;
/*0x1*/ static s8 bss_1;
/*0x2*/ static s8 bss_2;
/*0x3*/ static s8 bss_3;
/*0x4*/ static u8 _bss_4[0x4];
/*0x8*/ static char bss_8[4][50];
/*0xD0*/ static  Texture* bss_D0[18];
/*0x118*/ static Texture* bss_118;
/*0x11C*/ static Texture* bss_11C;
/*0x120*/ static Texture* bss_120;
/*0x124*/ static GameTextChunk* bss_124; //Cinema text
/*0x128*/ static GameTextChunk* bss_128; //Cheats text
/*0x12C*/ static s8 bss_12C;
/*0x12D*/ static s8 bss_12D;
/*0x12E*/ static s8 bss_12E;
/*0x12F*/ static s8 bss_12F;
/*0x130*/ static s32 bss_130[6]; //Unknown length
/*0x148*/ static GplayOptions* bss_148;

static void dll_65_func_1718(void);

// offset: 0x0 | ctor
void dll_65_ctor(void* dll) {
    s32 i;

    gDLL_28_ScreenFade->vtbl->fade_reversed(0x14, 1);
    bss_118 = tex_load_deferred(TEXTABLE_2DF_Paper_BG_Sabre_Cape);
    bss_120 = tex_load_deferred(TEXTABLE_30E);
    bss_11C = tex_load_deferred(TEXTABLE_30D);
    
    if (data_794 == 0) {
        data_794 = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_0EF_Menu_Main_Menu);
    }
    
    data_798 = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_1F5_Menu_Controls);
    bss_128 = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_1F6_Menu_Cheats);
    bss_124 = gDLL_21_Gametext->vtbl->get_chunk(GAMETEXT_1F7_Menu_Cinema);

    for (i = 0; i < 18; i++) {
        bss_D0[i] = tex_load_deferred(data_80C[i]);
    }
    
    dll_65_func_1718();
    bss_12C = 0;
    bss_12D = 0;
    bss_148 = gDLL_29_Gplay->vtbl->get_game_options();
    bss_0 = 2;
    bss_2 = 0;
}

// offset: 0x1F0 | dtor
void dll_65_dtor(void *dll) { }

// offset: 0x1FC | func: 0 | export: 0
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_1FC.s")
#else

static void dll_65_func_1718(void);                 //matched
static void dll_65_func_1AE4(s32 arg0);             //matched
static void dll_65_func_2088(s32 arg0);             //matched
static s32 dll_65_func_2A1C(s32 a0, s32 a1);        //matched
static void dll_65_func_2B50(s32 arg0, s32 arg1);   //matched
static void dll_65_func_2C58(s32 arg0, s32 arg1);   //matched
static void dll_65_func_2D50(s32 arg0, s32 arg1);   //matched
static void dll_65_func_320C(s32 arg0, s32 arg1);   //NOT matched
static void dll_65_func_32B8(s32 arg0, s32 arg1);   //matched
static void dll_65_func_3404(s32 arg0, s32 arg1);   //matched
static void dll_65_func_3414(void);                 //matched

s32 dll_65_func_1FC(void) {
    s8 timeBefore;
    s32 selectedIdx; //58
    s32 i;
    s8 sp53; //53
    s8 sp52; //52
    s32 frames;
    s32 action;

    timeBefore = bss_3;
    
    frames = gUpdateRate;
    if (frames > 3) {
        frames = 3;
    }
    
    if (bss_3 > 0) {
        bss_3 -= frames;
    }
    
    if (gDLL_28_ScreenFade->vtbl->is_complete() == 0) {
        gDLL_74_Picmenu->vtbl->redraw_all();
        bss_0 = 2;
    }
    
    if (bss_2 != 0) {
        if ((timeBefore >= 0xD) && (bss_3 < 0xD)) {
            vi_init(1, get_ossched(), 0);
            dll_65_func_3414();
            track_set_z_buffer_on(1);
            track_set_sky_on(1);
        } else if (bss_3 <= 0) {
            main_demo_reset();
            main_start_game(12457.1f, -1474.875f, -6690.398f, 1);
            menu_set(4);
            if (1) { }
        }
        
        if (bss_3 < 0xD) {
            return 1;
        } else {
            return 0;
        }
    }
    
    action = gDLL_74_Picmenu->vtbl->update();
    selectedIdx = gDLL_74_Picmenu->vtbl->get_selected_item();

    switch (data_79C) {
    case 0:
        bss_12D = selectedIdx;
        if (dll_65_func_2A1C(action, selectedIdx) != 0) {
            return 0;
        }
        break;
    case 3:
        dll_65_func_2B50(action, selectedIdx);
        if (action == PICMENU_ACTION_BACK) {
            bss_148->showSubtitles = gDLL_75->vtbl->func8(bss_130[0]);
            bss_148->showInstruments = gDLL_75->vtbl->func8(bss_130[1]);
            dll_65_func_1718();
        }
        break;
    case 4:
        dll_65_func_2C58(action, selectedIdx);
        if (action == PICMENU_ACTION_BACK) {
            bss_148->zTargetMode = gDLL_75->vtbl->func8(bss_130[0]);
            dll_65_func_1718();
        }
        break;
    case 5:
        dll_65_func_2D50(action, selectedIdx);
        if (action == PICMENU_ACTION_BACK) {
            for (i = 0; i < 4; i++) {
                gDLL_29_Gplay->vtbl->set_cheat_enabled((bss_1 + i), gDLL_75->vtbl->func8(bss_130[i + 1]));
            }
            dll_65_func_1718();
        }
        break;
    case 1:
        dll_65_func_320C(action, selectedIdx);
        if (action == PICMENU_ACTION_BACK) {
            bss_148->screenSizeAnamorphic = gDLL_75->vtbl->func8(bss_130[0]);
            bss_148->screenAspectRatio = gDLL_75->vtbl->func8(bss_130[1]);
            dll_65_func_1718();
        }
        break;
    case 2:
        dll_65_func_32B8(action, selectedIdx);
        if (action == PICMENU_ACTION_BACK) {
            bss_148->audioMode = gDLL_75->vtbl->func8(bss_130[0]);
            bss_148->volumeMusic = gDLL_75->vtbl->func8(bss_130[1]);
            bss_148->volumeAudio = gDLL_75->vtbl->func8(bss_130[2]);
            dll_65_func_1718();
        }
        break;
    case 6:
        dll_65_func_3404(action, selectedIdx);
        if (action == PICMENU_ACTION_BACK) {
            dll_65_func_1718();
        }
        break;
    case 7:
        if (action != PICMENU_ACTION_NONE) {
            tex_free(bss_118);
            bss_118 = tex_load_deferred(0x2DF);
            dll_65_func_1AE4(1);
            return 0;
        }
        break;
    case 8:
        joy_get_stick_menu_xy_sign(0, &sp53, &sp52);
        if (sp53 > 0) {
            gDLL_6_AMSFX->vtbl->play(NULL, 0x5D5U, 0x7F, NULL, NULL, 0, NULL);
            bss_148->screenOffsetX += 1;
            if (bss_148->screenOffsetX >= 8) {
                bss_148->screenOffsetX = 7;
                sp53 = 0;
            }
        } else if (sp53 < 0) {
            gDLL_6_AMSFX->vtbl->play(NULL, 0x5D5U, 0x7F, NULL, NULL, 0, NULL);
            bss_148->screenOffsetX -= 1;
            if (bss_148->screenOffsetX < -7) {
                bss_148->screenOffsetX = -7;
                sp53 = 0;
            }
        }
        
        if (sp52 > 0) {
            gDLL_6_AMSFX->vtbl->play(NULL, 0x5D5U, 0x7F, NULL, NULL, 0, NULL);
            bss_148->screenOffsetY -= 1;
            if (bss_148->screenOffsetY < -7) {
                bss_148->screenOffsetY = -7;
                sp52 = 0;
            }
        } else if (sp52 < 0) {
            gDLL_6_AMSFX->vtbl->play(NULL, 0x5D5U, 0x7F, NULL, NULL, 0, NULL);
            bss_148->screenOffsetY += 1;
            if (bss_148->screenOffsetY > 7) {
                bss_148->screenOffsetY = 7;
                sp52 = 0;
            }
        }
        
        if ((sp53 != 0) || (sp52 != 0)) {
            vi_set_modifiers(1, bss_148->screenOffsetX, bss_148->screenOffsetY);
        }
        if (action != PICMENU_ACTION_NONE) {
            joy_reset_menu_joystick_delay();
            dll_65_func_2088(2);
            return 0;
        }
        break;
    default:
        break;
    }

    if (data_79C != 0) {
        for (i = 0; i < bss_12C; i++) {
            if (bss_130[i] != 0) {
                if (i == selectedIdx) {
                    gDLL_75->vtbl->func7.withTwoArgs(bss_130[i], 1);
                } else {
                    gDLL_75->vtbl->func7.withTwoArgs(bss_130[i], 0);
                }
                gDLL_75->vtbl->func4.withOneArg(bss_130[i]);
            }
        }
    }
    
    return 0;
}
#endif

// offset: 0xB84 | func: 1 | export: 1
void dll_65_func_B84(void) {

}

// offset: 0xB8C | func: 2 | export: 2
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_B8C.s")
#else
static void dll_65_func_35D0(Gfx** gdl, s32 initialX, s32 initialY, s32 endX, s32 endY) {}

void dll_65_func_B8C-3(Gfx** gdl, Mtx **mtxs, Vertex **vtxs) {
    s32 uly; //7C
    s32 lry; //78
    s32 ulx; //74
    s32 lrx; //70
    OptionsSubmenu* submenu;
    s32 i2;
    s32 i;
    s32 end;
    s32 pad;

    submenu = &data_704[data_79C];
    
    if ((bss_2 == 0) || (bss_3 >= 0xB)) {
        font_window_set_coords(1, 0, 0, 
            GET_VIDEO_WIDTH(vi_get_current_size()), 
            GET_VIDEO_HEIGHT(vi_get_current_size())
        );
        font_window_flush_strings(1);
        
        if (data_79C == 8) {
            rcp_screen_full_write(gdl, bss_118, 0, 0, 0, 0, 0xFF, 2);
            rcp_screen_full_write(gdl, bss_11C, 0x26, 0x24, 0, 0, 0xFF, 1);
            rcp_screen_full_write(gdl, bss_120, 0x26, 0x24, 0, 0, 0xFF, 1);
            rcp_screen_full_write(gdl, bss_11C, 0x257, 0x24, 0, 0, 0xFF, 1);
            rcp_screen_full_write(gdl, bss_120, 0x26, 0x1B6, 0, 0, 0xFF, 1);
            font_window_use_font(1, 4);
            font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, gDLL_74_Picmenu->vtbl->get_highlight_alpha());
            font_window_add_string_xy(1, 0x140, 0x2A, &data_0, 1, ALIGN_TOP_CENTER);
            font_window_add_string_xy(1, 0x140, 0x190, &data_4, 1, ALIGN_TOP_CENTER);
            font_window_add_string_xy(1, 0x30, 0xEB, &data_8, 1, ALIGN_TOP_LEFT);
            font_window_add_string_xy(1, 0x242, 0xEB, &data_C, 1, ALIGN_TOP_LEFT);
        } else if (data_79C == 7) {
            rcp_screen_full_write(gdl, bss_118, 0, 0, 0, 0, 0xFF, 2);
            font_window_use_font(1, 0);
            
            for (i = 0; i < 0xB; i++) {
                font_window_set_text_colour(1, 0xE1, 0xAB, 0x61, 0xFF, 0xFF);
                font_window_add_string_xy(1, data_7A0[i][0], data_7A0[i][1], data_798->strings[i], 1, ALIGN_TOP_LEFT);
                font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0x96);
                font_window_add_string_xy(1, data_7A0[i][0] - 1, data_7A0[i][1] - 1, data_798->strings[i], 2, ALIGN_TOP_LEFT);
            }
            
            font_window_use_font(1, 1);
            
            for (; i < 0x1B; i++) {
                font_window_set_text_colour(1, 0xB7, 0x8B, 0x61, 0xFF, 0xFF);
                font_window_add_string_xy(1, data_7A0[i][0], data_7A0[i][1], data_798->strings[i], 1, ALIGN_TOP_LEFT);
                font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0x96);
                font_window_add_string_xy(1, data_7A0[i][0] - 1, data_7A0[i][1] - 1, data_798->strings[i], 2, ALIGN_TOP_LEFT);
            }
            
        } else {
            if (bss_0) {
                rcp_screen_full_write(gdl, bss_118, 0, 0, 0, 0, 0xFF, 2);
                font_window_use_font(1, 4);
                font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, 0xFF);
                font_window_add_string_xy(1, 0x238, 0x3F, data_794->strings[1], 1, ALIGN_TOP_RIGHT);
                font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0xFF);
                font_window_add_string_xy(1, 0x233, 0x3A, data_794->strings[1], 2, ALIGN_TOP_RIGHT);
                if (submenu->unk9 != -1) {
                    font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, 0xFF);
                    font_window_add_string_xy(1, 0x53, 0xB2, data_794->strings[submenu->unk9], 1, ALIGN_TOP_LEFT);
                    font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0xFF);
                    font_window_add_string_xy(1, 0x4E, 0xAD, data_794->strings[submenu->unk9], 2, ALIGN_TOP_LEFT);
                }
                if (submenu->unkA != -1) {
                    font_window_use_font(1, 0);
                    font_window_set_text_colour(1, 0xB7, 0x8B, 0x61, 0xFF, 0xFF);
                    font_window_add_string_xy(1, 0x140, 0x195, data_794->strings[submenu->unkA], 1, ALIGN_TOP_CENTER);
                    font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0xFF);
                    font_window_add_string_xy(1, 0x13E, 0x193, data_794->strings[submenu->unkA], 2, ALIGN_TOP_CENTER);
                }
                if (submenu->unk9 != -1) {
                    dll_65_func_35D0(gdl, 0x38, 0xDC, 0, 0x1E0);
                }
            } else {
                func_80010158(&ulx, &lrx, &uly, &lry);
                rcp_screen_scroll_write(gdl, bss_118, 0, 0, uly, lry, 0xFF, 2);
                if (submenu->unk9 != -1) {
                    dll_65_func_35D0(gdl, 0x38, 0xDC, uly, lry);
                }
                if ((data_79C == 0) && (uly < 0x64)) {
                    font_window_use_font(1, 4);
                    font_window_set_text_colour(1, 0xFF, 0xFF, 0xFF, 0, 0xFF);
                    font_window_add_string_xy(1, 0x238, 0x3F, data_794->strings[1], 1, ALIGN_TOP_RIGHT);
                    font_window_set_text_colour(1, 0, 0, 0, 0xFF, 0xFF);
                    font_window_add_string_xy(1, 0x233, 0x3A, data_794->strings[1], 2, ALIGN_TOP_RIGHT);
                }
            }

            for (i = 0; i < bss_12C; i++) {
                if (bss_130[i] != 0) {
                    gDLL_75->vtbl->func5.withTwoArgs(bss_130[i], gdl);
                }
            }
        }
        
        gDLL_74_Picmenu->vtbl->draw(gdl);
        
        font_window_draw(gdl, NULL, NULL, 1);
        
        bss_0--;
        if (bss_0 < 0) {
            bss_0 = 0;
        }
    }
}
#endif

// offset: 0x16A4 | func: 3
static void dll_65_func_16A4(OptionsSubmenu* submenu) {
    s32 i;

    for (i = 0; i < submenu->count; i++) {
        submenu->menuItems[i].text = data_794->strings[submenu->textIDs[i]];
    }
}

// offset: 0x1718 | func: 4
void dll_65_func_1718(void) {
    OptionsSubmenu* submenu;
    s32 i;

    if (data_79C != -1) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }
    
    data_79C = 0;
    submenu = &data_704[data_79C];
    dll_65_func_16A4(submenu);
    gDLL_74_Picmenu->vtbl->set_items(
        submenu->menuItems, submenu->count, bss_12D, NULL, submenu->unkC, submenu->unkD, 
        0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D
    );

    for (i = 0; i < bss_12C; i++) {
        if (bss_130[i]) {
            gDLL_75->vtbl->func3(bss_130[i]);
        }
        bss_130[i] = 0;
    }
    
    bss_12C = 0;
    bss_0 = 2;
}

// offset: 0x1898 | func: 5
static void dll_65_func_1898(void) {
    OptionsSubmenu* submenu;
    char** strings;
    
    if (data_79C != -1) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }
    
    data_79C = 3;
    
    submenu = &data_704[data_79C];
    dll_65_func_16A4(submenu);
    gDLL_74_Picmenu->vtbl->set_items(
        submenu->menuItems, submenu->count, 0, NULL, submenu->unkC, submenu->unkD, 
        0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D
    );

    strings = data_1FC;    
    strings[0] = data_794->strings[10];
    strings[1] = data_794->strings[9];
    
    bss_12C = 0;
    bss_130[bss_12C] = gDLL_75->vtbl->func2.withSevenArgsS32(0x1dc, 0xfe, 0, 1, bss_148->showSubtitles, strings, 0x32);
    bss_12C++;
    bss_130[bss_12C] = gDLL_75->vtbl->func2.withSevenArgsS32(0x1dc, 0x116, 0, 1, bss_148->showInstruments, strings, 0x32);
    bss_12C++;
    
    gDLL_75->vtbl->func7.withTwoArgs(bss_130[0], 1);
    bss_0 = 2;
}

// offset: 0x1AE4 | func: 6
static void dll_65_func_1AE4(s32 arg0) {
    OptionsSubmenu* submenu;

    if (data_79C != -1) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }
    
    data_79C = 4;
    submenu = &data_704[data_79C];
    dll_65_func_16A4(submenu);
    gDLL_74_Picmenu->vtbl->set_items(
        submenu->menuItems, submenu->count, arg0, NULL, submenu->unkC, submenu->unkD, 
        0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D
    );
    
    data_280[0] = data_794->strings[14];
    data_280[1] = data_794->strings[15];
    data_280[2] = data_794->strings[16];
    
    bss_12C = 0;
    bss_130[bss_12C] = gDLL_75->vtbl->func2.withSevenArgsS32(0x1aa,0xfe,0,2,bss_148->zTargetMode, data_280, 0x64);
    bss_12C++;
    bss_130[bss_12C] = 0;
    bss_12C++;
    
    if (bss_130[arg0]) {
        gDLL_75->vtbl->func7.withTwoArgs(bss_130[arg0], 1);
    }
    
    bss_0 = 2;
}

// offset: 0x1CF4 | func: 7
static void dll_65_func_1CF4(void) {
    OptionsSubmenu *submenu;
    s32 enabled;
    s32 unlocked;
    u32 i;
    u32 y;
    
    if (data_79C != -1) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }
    
    data_79C = 5;
    submenu = &data_704[data_79C];
    bss_12C = 0;
    bss_1 = 0;
    y = 0xfc;
    bss_130[bss_12C] = NULL;
    bss_12C++;

    for (i = 0; i < 4; i++) {
        unlocked = gDLL_29_Gplay->vtbl->is_cheat_unlocked(bss_1 + i);
        
        if (unlocked) {
            sprintf(bss_8[i], "%2d: %s", (int) ((bss_1 + i) + 1), bss_128->strings[bss_1 + i]);
            submenu->menuItems[i + 1].text = bss_8[i];
            submenu->menuItems[i + 1].flags &= ~0x820;
        } else {
            sprintf(bss_8[i], "%2d:", (int) ((bss_1 + i) + 1));
            submenu->menuItems[i + 1].text = bss_8[i];
            submenu->menuItems[i + 1].flags |= 0x820;
        }
        
        enabled = gDLL_29_Gplay->vtbl->is_cheat_active(bss_1 + i);
        
        bss_130[bss_12C] = gDLL_75->vtbl->func1.withFiveArgsS32(0x201, (s16) y, 0, 1, enabled ? 1 : 0);
        bss_12C++;
        gDLL_75->vtbl->func11.withTwoArgs(bss_130[bss_12C - 1], unlocked);
        y += 0x1a;
    }
    
    bss_130[bss_12C] = 0;
    bss_12C++;
    
    submenu->menuItems->flags |= 0x1000;
    gDLL_74_Picmenu->vtbl->set_items(submenu->menuItems, submenu->count, 1, NULL, submenu->unkC, submenu->unkD, 0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D);
    gDLL_75->vtbl->func7.withTwoArgs(bss_130[1], 1);
    bss_0 = 2;
}

// offset: 0x2088 | func: 8
static void dll_65_func_2088(s32 arg0) {
    OptionsSubmenu* submenu;
    s8 languageID = gDLL_21_Gametext->vtbl->curr_bank();
    
    if (data_79C != -1) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }
    
    data_79C = 1;
    
    submenu = &data_704[data_79C];
    dll_65_func_16A4(submenu);
    gDLL_74_Picmenu->vtbl->set_items(
        submenu->menuItems, submenu->count, arg0, NULL, submenu->unkC, submenu->unkD, 
        0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D
    );
    
    data_4E8[0] = data_794->strings[22];
    data_4E8[1] = data_794->strings[23];
    data_4E8[2] = data_794->strings[24];
    data_4F4[0] = data_794->strings[26];
    data_4F4[1] = data_794->strings[27];
    
    bss_12C = 0;
    if (((u8) languageID == 3) || ((u8) languageID == 4)) {
        bss_130[bss_12C] = gDLL_75->vtbl->func2.withSevenArgsS32(0x1c3, 0xfe, 0, 2, bss_148->screenSizeAnamorphic, data_4E8, 0x4B);
        bss_12C += 1;
        bss_130[bss_12C] = gDLL_75->vtbl->func2.withSevenArgsS32(0x1c3, 0x116, 0, 1, bss_148->screenAspectRatio, data_4F4, 0x4B);
        bss_12C++;
    } else {
        bss_130[bss_12C] = gDLL_75->vtbl->func2.withSevenArgsS32(0x1aa, 0xfe, 0, 2, bss_148->screenSizeAnamorphic, data_4E8, 0x64);
        bss_12C++;
        bss_130[bss_12C] = gDLL_75->vtbl->func2.withSevenArgsS32(0x1aa, 0x116, 0, 1, bss_148->screenAspectRatio, data_4F4, 0x64);
        bss_12C++;
    }
    
    bss_130[bss_12C] = 0;
    bss_12C += 1;
    
    if (bss_130[arg0] != 0) {
        gDLL_75->vtbl->func7.withTwoArgs(bss_130[arg0], 1);
    }
    
    bss_0 = 2;
}

// offset: 0x2438 | func: 9
static void dll_65_func_2438(void) {
    OptionsSubmenu* submenu;

    if (data_79C != -1) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }
    data_79C = 2;
    
    submenu = &data_704[data_79C];
    dll_65_func_16A4(submenu);
    gDLL_74_Picmenu->vtbl->set_items(
        submenu->menuItems, submenu->count, 0, NULL, submenu->unkC, submenu->unkD, 
        0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D
    );
    
    data_5F0[0] = data_794->strings[31];
    data_5F0[1] = data_794->strings[32];
    data_5F0[2] = data_794->strings[33];
    data_5F0[3] = data_794->strings[34];
    bss_12C = 0;    
    
    bss_130[bss_12C] = gDLL_75->vtbl->func2.withSevenArgsS32(0x1aa, 0xfe, 0, 3, bss_148->audioMode, data_5F0, 100);
    bss_12C++;
    bss_130[bss_12C] = gDLL_75->vtbl->func0.withFiveArgsS32(0x13e, 0x11d, 0, 0xff, bss_148->volumeMusic);
    bss_12C++;
    bss_130[bss_12C] = gDLL_75->vtbl->func0.withFiveArgsS32(0x13e, 0x135, 0, 0x7f, bss_148->volumeAudio);
    bss_12C++;
    
    gDLL_75->vtbl->func7.withTwoArgs(bss_130[0], 1);
    bss_0 = 2;
}

// offset: 0x26D8 | func: 10
static void dll_65_func_26D8(void) {
    OptionsSubmenu* submenu;
    s32 i;

    if (data_79C != -1) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }
    
    data_79C = 7;
    submenu = &data_704[data_79C];
    dll_65_func_16A4(submenu);
    gDLL_74_Picmenu->vtbl->set_items(submenu->menuItems, submenu->count, 0, NULL, submenu->unkC, submenu->unkD, 0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D);
    
    for (i = 0; i < bss_12C; i++) {
        if (bss_130[i] != 0) {
            gDLL_75->vtbl->func3(bss_130[i]);
        }
        bss_130[i] = 0;
    }
    
    tex_free(bss_118);
    bss_118 = tex_load_deferred(0x2E0);
    bss_12C = 0;
    bss_0 = 2;
}

// offset: 0x2888 | func: 11
void dll_65_func_2888(void) {
    OptionsSubmenu* submenu;
    s32 i;

    if (data_79C != -1) {
        gDLL_74_Picmenu->vtbl->clear_items();
    }
    
    data_79C = 8;
    submenu = &data_704[data_79C];
    dll_65_func_16A4(submenu);
    gDLL_74_Picmenu->vtbl->set_items(
        submenu->menuItems, submenu->count, 
        0, NULL, submenu->unkC, submenu->unkD, 
        0xB7, 0x8B, 0x61, 0xFF, 0xD7, 0x3D
    );
    
    for (i = 0; i < bss_12C; i++) {
        if (bss_130[i] != 0) {
            gDLL_75->vtbl->func3(bss_130[i]);
        }
        bss_130[i] = 0;
    }
    
    joy_set_menu_joystick_delay(0xF);
    bss_12C = 0;
    bss_0 = 2;
}

// offset: 0x2A1C | func: 12
s32 dll_65_func_2A1C(s32 arg0, u32 arg1) {
    if (arg0 == 1) {
        switch (arg1) {
        case 2:
            dll_65_func_1898();
            return 1;
        case 3:
            dll_65_func_1AE4(0);
            return 1;
        case 4:
            dll_65_func_1CF4();
            return 1;
        case 0:
            dll_65_func_2088(0);
            return 1;
        case 1:
            dll_65_func_2438();
            return 1;
        }
    } else if (arg0 == 0) {
        gDLL_28_ScreenFade->vtbl->fade(0x14, 1);
        bss_3 = 0x23;
        bss_2 = 1;
    }

    return 0;
}

// offset: 0x2B50 | func: 13
void dll_65_func_2B50(s32 arg0, s32 arg1) {   
    if (!gDLL_75->vtbl->func10.withOneArgS32(bss_130[arg1])) {
        return;
    }
    
    switch (arg1) {
    case 0:
        gDLL_22_Subtitles->vtbl->func_2D0(gDLL_75->vtbl->func8(bss_130[arg1]));
        break;
    case 1:
        gDLL_1_cmdmenu->vtbl->toggle_forced_stats_display(gDLL_75->vtbl->func8(bss_130[arg1]));
        break;
    }
}

// offset: 0x2C58 | func: 14
void dll_65_func_2C58(s32 arg0, s32 arg1) {
    if (arg1 == 0) {
        if (gDLL_75->vtbl->func10.withOneArgS32(bss_130[arg1]) != 0) {
            gDLL_2_Camera->vtbl->apply_target_flags(gDLL_75->vtbl->func8(bss_130[arg1]));
        }
    } else if (arg0 == 1) {
        bss_148->zTargetMode = gDLL_75->vtbl->func8(bss_130[0]);
        dll_65_func_26D8();
    }
}

// offset: 0x2D50 | func: 15
void dll_65_func_2D50(s32 arg0, s32 arg1) {
    OptionsSubmenu* submenu;
    s32 i;
    s32 var_v0;
    s32 enabled;
    u8 bss1_before;

    //Unlock the selected cheat with C-right! (Need to back out and re-enter menu before it shows up)
    if (joy_get_pressed(0) & R_CBUTTONS) {
        gDLL_29_Gplay->vtbl->unlock_cheat(((bss_1 + arg1) - 1));
    }
    
    if (arg0 != 1) {
        return;
    }
    
    var_v0 = 0;

    bss1_before = bss_1;
    if (arg1 == 0) {
        bss_1 -= 4;
        if (bss_1 < 0) {
            bss_1 = 0;
        } else {
            var_v0 = 1;
        }
    } else if (arg1 == 5) {
        bss_1 += 4;
        if (bss_1 > 0x1C) {
            bss_1 = 0x1C;
        } else {
            var_v0 = 1;
        }
    }
    
    if (var_v0 == 0) {
        return;
    }

    submenu = &data_704[data_79C];
    submenu->menuItems[0].flags &= ~0x1000;
    submenu->menuItems[5].flags &= ~0x1000;
    if (bss_1 == 0) {
        submenu->menuItems[0].flags |= 0x1000;
    }
    if (bss_1 == 0x1C) {
        submenu->menuItems[5].flags |= 0x1000;
    }
    
    for (i = 0; i < 4; i++) {
        gDLL_29_Gplay->vtbl->set_cheat_enabled(bss1_before + i, gDLL_75->vtbl->func8(bss_130[i + 1]));
    }

    for (i = 0; i < 4; i++) {
        if (gDLL_29_Gplay->vtbl->is_cheat_unlocked(bss_1 + i)) {
            sprintf(bss_8[i], "%2d: %s", (int)(bss_1 + i + 1), bss_128->strings[bss_1 + i]);
            submenu->menuItems[i + 1].text = bss_8[i];
            submenu->menuItems[i + 1].flags &= ~0x820;
            gDLL_75->vtbl->func11.withTwoArgs(bss_130[i + 1], 1);
            
            enabled = gDLL_29_Gplay->vtbl->is_cheat_active(bss_1 + i);
            gDLL_75->vtbl->func9.withTwoArgs(bss_130[i + 1], enabled ? 1 : 0);
        } else {
            sprintf(bss_8[i], "%2d:", (int)(bss_1 + i + 1));
            submenu->menuItems[i + 1].text = bss_8[i];
            submenu->menuItems[i + 1].flags |= 0x820;
            gDLL_75->vtbl->func11.withTwoArgs(bss_130[i + 1], 0);
            gDLL_75->vtbl->func9.withTwoArgs(bss_130[i + 1], 0);
        }
    }
    
    gDLL_74_Picmenu->vtbl->update_text(submenu->menuItems);
    gDLL_74_Picmenu->vtbl->update_flags(submenu->menuItems);
    
    if (bss_1 == 0) {
        gDLL_75->vtbl->func7.withTwoArgs(bss_130[1], 1);
        gDLL_74_Picmenu->vtbl->set_selected_item(1);
    } else if (bss_1 == 0x1C) {
        gDLL_75->vtbl->func7.withTwoArgs(bss_130[4], 1);
        gDLL_74_Picmenu->vtbl->set_selected_item(4);
    }
    
    bss_0 = 2;
}

// offset: 0x320C | func: 16
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_320C.s")
#else
void dll_65_func_320C(s32 arg0, s32 arg1) {
    if ((arg0 != 1) || (arg1 != 2)) {
        return;
    }
    
    bss_148->screenSizeAnamorphic = gDLL_75->vtbl->func8(bss_130[1]);
    bss_148->screenAspectRatio = gDLL_75->vtbl->func8(bss_130[1]);
    dll_65_func_2888();
}
#endif

// offset: 0x32B8 | func: 17
void dll_65_func_32B8(s32 arg0, s32 arg1) {
    if (gDLL_75->vtbl->func10.withOneArgS32(bss_130[arg1]) == 0) {
        return;
    }
        
    switch (arg1) {
    case 0:
        speaker_set_mode(data_88C[gDLL_75->vtbl->func8(bss_130[arg1])]);
        break;
    case 2:
        gDLL_6_AMSFX->vtbl->func_7E4(gDLL_75->vtbl->func8(bss_130[arg1]));
        break;
    case 1:
        gDLL_5_AMSEQ->vtbl->set_volume_option(gDLL_75->vtbl->func8(bss_130[arg1]));
        break;
    }
}

// offset: 0x3404 | func: 18
void dll_65_func_3404(s32 arg0, s32 arg1) { //cinema menu?

}

// offset: 0x3414 | func: 19
void dll_65_func_3414(void) {
    Texture** textures;
    Texture* tex;
    s32 i;

    if (data_794 != NULL) {
        mmFree(data_794);
        data_794 = NULL;
    }
    
    mmFree(data_798);
    mmFree(bss_128);
    mmFree(bss_124);
    
    if (data_79C != -1) {
        gDLL_74_Picmenu->vtbl->clear_items();
        data_79C = -1;
    }
    
    textures = bss_D0;
    do {
        tex = *textures;
        if (tex != NULL) {
            tex_free(tex);
            *textures = NULL;
        }
        textures++;
    } while ((u32) textures < (u32) &bss_118);
    
    tex_free(bss_118);
    tex_free(bss_120);
    tex_free(bss_11C);
    
    for (i = 0; i < bss_12C; i++) {
        if (bss_130[i] != 0) {
            gDLL_75->vtbl->func3(bss_130[i]);
        }
        bss_130[i] = 0;
    }
    
    bss_12C = 0;
}

// offset: 0x35D0 | func: 20
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_35D0.s")
#else
void dll_65_func_35D0(Gfx** arg0, s32 initialX, s32 initialY, s32 endX, s32 endY) {
    s16 *texIdxPtr;
    s16 *new_var;
    u32 end;
    u32 x;
    u32 y;
    u32 i;
    
    end = 45;

    x = initialX;
    y = initialY;
    
    texIdxPtr = data_830;
    new_var = texIdxPtr++; //fake
    
    for (i = 0; i < end; i++) {
        if (texIdxPtr[i] == -1) {
            x = initialX;
            y += 0x20;
        } else {
            rcp_screen_scroll_write(arg0, bss_D0[texIdxPtr[i]], x, y, endX, endY, 0xFF, SCREEN_WRITE_TRANSLUCENT);
            x += 0x40;
        }
    }
}
#endif
