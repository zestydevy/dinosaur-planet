#include "common.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/74_picmenu.h"
#include "sys/fonts.h"
#include "game/gametexts.h"

#define NONE -1

/*0x0*/ static u32 data_0 = 0xab000000;
/*0x4*/ static u32 data_4 = 0xbb000000;
/*0x8*/ static u32 data_8 = 0x3c000000;
/*0xC*/ static u32 data_C = 0x3e000000;

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
/*0x1F8*/ static s8 data_1F8[] = {0x08, 0x0b}; //Text lineIDs?

/*0x1FC*/ static u32 data_1FC[] = {
    0x00000000, 0x00000000
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
/*0x27C*/ static s8 data_27C[] = {0x0d, 0x11};  //Text lineIDs?
/*0x280*/ static u32 data_280[] = {
    0x00000000, 0x00000000, 0x00000000
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
/*0x4E4*/ static s8 data_4E4[] = {0x15, 0x19, 0x1c, 0x1d};  //Text lineIDs?

/*0x4E8*/ static char* data_4E8[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0x4F4*/ static char* data_4F4[] = {
    0x00000000, 0x00000000
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
/*0x5EC*/ static s8 data_5EC[] = {0x1e, 0x23, 0x24, 0x25}; //Text lineIDs?

/*0x5F0*/ static char* data_5F0[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
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
/*0x6C0*/ static s8 data_6C0[] = {0x28};
/*0x6C4*/ static PicMenuItem data_6C4[] = {
    /*0*/ NEW_PICMENU_ITEM( 
        /*textX*/ 320, /*textY*/ 237, /*innerWidth*/ 0,
        /*itemX*/ 320, /*itemY*/ 225, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_ALIGN_ITEM_CENTER | PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | PICMENU_ALIGN_TEXT_CENTER), 
        /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
        /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),
};
/*0x700*/ static s8 data_700[] = {0x28};

typedef struct {
    PicMenuItem *menuItems;
    s8 *textIDs;
    u8 count;
    u8 unk9;
    u8 unkA;
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
/*0x7A0*/ static u16 data_7A0[] = {
    0x01d6, 0x00c4, 0x01d0, 0x011c, 0x01cc, 0x003e, 0x019b, 0x0175, 0x003f, 0x008d, 0x003f, 0x013f, 0x013a, 0x003e, 0x01d9, 0x0096, 
    0x01d9, 0x0071, 0x01d6, 0x005c, 0x01d6, 0x0082
};
/*0x7CC*/ static u16 data_7CC[] = {
    0x01ea, 0x0059, 0x01ea, 0x0073, 0x01ea, 0x0086, 0x01ea, 0x0098, 0x01d8, 0x00de, 0x01d8, 0x00ef, 0x01ce, 0x0100, 0x01cc, 0x0133, 
    0x01c6, 0x0145, 0x01ef, 0x0154, 0x019d, 0x018a, 0x019d, 0x0199, 0x0041, 0x0153, 0x0041, 0x0162, 0x0041, 0x00a3, 0x0131, 0x0053
};
/*0x80C*/ static s16 data_80C[] = {
    0x0320, 0x02f5, 0x02f6, 0x02f7, 0x02f8, 0x02f9, 0x02fa, 0x02fb, 0x02fc, 0x0313, 0x02fd, 0x02fe, 0x02ff, 0x0300, 0x0301, 0x0302, 
    0x0303, 0x0304
};
/*0x830*/ static u32 data_830[] = {
    0x00040010, 0x00110011, 0x00100011, 0x00110005, 0xffff0006, 0x0009000c, 0x0009000b, 0x000a0009, 
    0x000dffff, 0x00070009, 0x000b0009, 0x0009000c, 0x000a000e, 0xffff0008, 0x000a0009, 0x000c000a, 
    0x0009000b, 0x000fffff, 0x00020000, 0x00010000, 0x00010001, 0x00000003, 0xffff0000
};
/*0x88C*/ static u32 data_88C = 0x02040103;

/*0x0*/ static s8 bss_0;
/*0x1*/ static s8 bss_1;
/*0x2*/ static s8 bss_2;
/*0x3*/ static s8 bss_3;
/*0x4*/ static u8 _bss_4[0x4];
/*0x8*/ static u8 bss_8[0x8];
/*0x10*/ static u8 _bss_10[0xc0];
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
/*0x130*/ static s32 bss_130[4]; //Unknown length
/*0x140*/ static u8 bss_140[0x8];
/*0x148*/ static GplayOptions* bss_148;

static void dll_65_func_1718(void);

// offset: 0x0 | ctor
void dll_65_ctor(void* dll) {
    s32 i;

    gDLL_28_ScreenFade->vtbl->fade_reversed(0x14, 1);
    bss_118 = tex_load_deferred(0x2DF);
    bss_120 = tex_load_deferred(0x30E);
    bss_11C = tex_load_deferred(0x30D);
    
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_1FC.s")

// offset: 0xB84 | func: 1 | export: 1
void dll_65_func_B84(void) {

}

// offset: 0xB8C | func: 2 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_B8C.s")

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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_1898.s")

// offset: 0x1AE4 | func: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_1AE4.s")

// offset: 0x1CF4 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_1CF4.s")

// offset: 0x2088 | func: 8
void dll_65_func_2088(s32 arg0) {
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
void dll_65_func_2438(void) {
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_2A1C.s")

// offset: 0x2B50 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_2B50.s")

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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_2D50.s")

// offset: 0x320C | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_320C.s")

// offset: 0x32B8 | func: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_32B8.s")

// offset: 0x3404 | func: 18
void dll_65_func_3404(s32 arg0, s32 arg1) {

}

// offset: 0x3414 | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_3414.s")

// offset: 0x35D0 | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_35D0.s")
