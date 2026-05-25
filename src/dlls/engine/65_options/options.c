#include "common.h"
#include "dlls/engine/21_gametext.h"
#include "dlls/engine/74_picmenu.h"
#include "sys/fonts.h"

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
/*0x178*/ static u8 data_178[] = {
    5, 6, 2, 3, 4, 7
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
/*0x1F8*/ static u32 data_1F8 = 0x080b0000;
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
/*0x27C*/ static u32 data_27C = 0x0d110000;
/*0x280*/ static u32 data_280[] = {
    0x00000000, 0x00000000, 0x00000000
};

//Submenu?
/*0x28C*/ static u32 data_28C[] = {
    (u32)&data_0, 

    //More PicMenuItems?
    0x013600d2, 0x00000136, 0x00d20000, 0xffffffff, 0x00000182, 0x0504ff01, 0xffffff00, 0, 0, 0, 0, 0, 0, 0, 0, 
    0x005300fe, 0x00000053, 0x00fe0000, 0xffffffff, 0x00000181, 0x00000002, 0xffffff00, 0, 0, 0, 0, 0, 0, 0, 0, 
    0x00530118, 0x00000053, 0x01180000, 0xffffffff, 0x00000181, 0x00000103, 0xffffff00, 0, 0, 0, 0, 0, 0, 0, 0,
    0x00530132, 0x00000053, 0x01320000, 0xffffffff, 0x00000181, 0x00000204, 0xffffff00, 0, 0, 0, 0, 0, 0, 0, 0, 
    0x0053014c, 0x00000053, 0x014c0000, 0xffffffff, 0x00000181, 0x00000305, 0xffffff00, 0, 0, 0, 0, 0, 0, 0
};

//Submenu?
/*0x3B8*/ static u32 data_3B8[] = {
    (u32)&data_4, 
    0x01360164, 0x00000136, 0x01640000, 0xffffffff, 0x00000182, 0x050404ff, 0xffffff00, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
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
        /*upLink*/ 0x00, /*downLink*/ 2, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ), 
    
    /*2*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 302, /*innerWidth*/ 0, 
        /*itemX*/ 83, /*itemY*/ 302, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | 1),                  //NOTE: ENABLED
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ 0x01, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ), 

    /*3*/ NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 326, /*innerWidth*/ 0, 
        /*itemX*/ 83, /*itemY*/ 326, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ 0x02, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    )
};
/*0x4E4*/ static u32 data_4E4 = 0x15191c1d;
/*0x4E8*/ static u32 data_4E8[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0x4F4*/ static u32 data_4F4[] = {
    0x00000000, 0x00000000
};
/*0x4FC*/ static PicMenuItem data_4FC[] = {
    NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 254, /*innerWidth*/ 0, 
        /*itemX*/ 83, /*itemY*/ 254, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ NONE, /*downLink*/ 1, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ), 

    NEW_PICMENU_ITEM( 
        /*textX*/ 83, /*textY*/ 278, /*innerWidth*/ 0, 
        /*itemX*/ 83, /*itemY*/ 278, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ 0, /*downLink*/ 2, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ), 

    NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 302, /*innerWidth*/ 0, 
        /*itemX*/ 83, /*itemY*/ 302, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ 0x01, /*1*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ), 

    NEW_PICMENU_ITEM(
        /*textX*/ 83, /*textY*/ 326, /*innerWidth*/ 0, 
        /*itemX*/ 83, /*itemY*/ 326, /*textureID*/ NONE, /*outerWidth*/ 0, 
        /*flags*/ (PICMENU_RAISED_EFFECT | PICMENU_USE_TEXT_COLOR | PICMENU_DISABLED | 1), 
        /*fontID*/ FONT_FUN_FONT, /*highlightFontID*/ FONT_FUN_FONT, 
        /*upLink*/ 2, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    )
};
/*0x5EC*/ static u32 data_5EC = 0x1e232425;
/*0x5F0*/ static u32 data_5F0[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/*0x600*/ static PicMenuItem data_600[] = {
    NEW_PICMENU_ITEM(
        0x0053, 0x00fe, 0x0000, 
        0x0053, 0x00fe, 0xffffffff, 0x0000, 
        0x0181, 
        0x00, 0x00, 
        0xff, 0x01, 0xff, 0xff, 0xff
    ), 

    NEW_PICMENU_ITEM(
        0x0053, 0x0116, 0x0000, 
        0x0053, 0x0116, 0xffffffff, 0x0000, 
        0x0181, 
        0x00, 0x00, 
        0x00, 0xff, 0xff, 0xff, 0xff
    )
};
/*0x678*/ static u32 data_678 = 0;
/*0x67C*/ static u32 data_67C = 0;
/*0x680*/ static u32 data_680 = 0;
/*0x684*/ static PicMenuItem data_684[] = {
    NEW_PICMENU_ITEM(
    /*textX*/ 265, /*textY*/ 392, /*innerWidth*/ 0, 
    /*itemX*/ 265, /*itemY*/ 380, /*textureID*/ NONE, /*outerWidth*/ 0, 
    /*flags*/ (PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | PICMENU_ALIGN_TEXT_CENTER),
    /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
    /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),
};
/*0x6C0*/ static u32 data_6C0 = 0x28000000;
/*0x6C4*/ static PicMenuItem data_6C4[] = {
    NEW_PICMENU_ITEM( 
    /*textX*/ 320, /*textY*/ 237, /*innerWidth*/ 0,
    /*itemX*/ 320, /*itemY*/ 225, /*textureID*/ NONE, /*outerWidth*/ 0, 
    /*flags*/ (PICMENU_ALIGN_ITEM_CENTER | PICMENU_AUTO_WIDTH | PICMENU_HAS_BACKGROUND | PICMENU_ALIGN_TEXT_CENTER), 
    /*fontID*/ FONT_DINO_MEDIUM_FONT_OUT, /*highlightFontID*/ FONT_DINO_MEDIUM_FONT_IN, 
    /*upLink*/ NONE, /*downLink*/ NONE, /*leftLink*/ NONE, /*rightLink*/ NONE, /*overrideWith*/ NONE
    ),
};
/*0x700*/ static u32 data_700 = 0x28000000;
/*0x704*/ static u32 data_704 = (u32)&data_10;
/*0x708*/ static u32 data_708[] = {
    (u32)&data_178, 0x06ff0004, 0x05040000
};
/*0x714*/ static u32 data_714 = (u32)&data_3F4;
/*0x718*/ static u32 data_718[] = {
    (u32)&data_4E4, 0x03050003, 0x00000000
};
/*0x724*/ static u32 data_724 = (u32)&data_4FC;
/*0x728*/ static u32 data_728[] = {
    (u32)&data_5EC, 0x03060003, 0x00000000
};
/*0x734*/ static u32 data_734 = (u32)&data_180;
/*0x738*/ static u32 data_738[] = {
    (u32)&data_1F8, 0x02020003, 0x00000000
};
/*0x744*/ static u32 data_744 = (u32)&data_204;
/*0x748*/ static u32 data_748[] = {
    (u32)&data_27C, 0x02030003, 0x00000000
};
/*0x754*/ static u32 data_754[] = {
    (u32)&data_28C, 0x00000000, 0x06040003, 0x00000000
};
/*0x764*/ static u32 data_764[] = {
    (u32)&data_600, 0x00000000, 0x02070003, 0x00000000
};
/*0x774*/ static u32 data_774 = (u32)&data_684;
/*0x778*/ static u32 data_778[] = {
    (u32)&data_6C0, 0x01ffff05, 0x05040000
};
/*0x784*/ static u32 data_784 = (u32)&data_6C4;
/*0x788*/ static u32 data_788[] = {
    (u32)&data_700, 0x01ffff05, 0x05040000
};
/*0x794*/ static u32 data_794 = 0x00000000;
/*0x798*/ static u32 data_798 = 0x00000000;
/*0x79C*/ static u32 data_79C = 0xff000000;
/*0x7A0*/ static u16 data_7A0[] = {
    0x01d6, 0x00c4, 0x01d0, 0x011c, 0x01cc, 0x003e, 0x019b, 0x0175, 0x003f, 0x008d, 0x003f, 0x013f, 0x013a, 0x003e, 0x01d9, 0x0096, 
    0x01d9, 0x0071, 0x01d6, 0x005c, 0x01d6, 0x0082
};
/*0x7CC*/ static u16 data_7CC[] = {
    0x01ea, 0x0059, 0x01ea, 0x0073, 0x01ea, 0x0086, 0x01ea, 0x0098, 0x01d8, 0x00de, 0x01d8, 0x00ef, 0x01ce, 0x0100, 0x01cc, 0x0133, 
    0x01c6, 0x0145, 0x01ef, 0x0154, 0x019d, 0x018a, 0x019d, 0x0199, 0x0041, 0x0153, 0x0041, 0x0162, 0x0041, 0x00a3, 0x0131, 0x0053
};
/*0x80C*/ static u16 data_80C[] = {
    0x0320, 0x02f5, 0x02f6, 0x02f7, 0x02f8, 0x02f9, 0x02fa, 0x02fb, 0x02fc, 0x0313, 0x02fd, 0x02fe, 0x02ff, 0x0300, 0x0301, 0x0302, 
    0x0303, 0x0304
};
/*0x830*/ static u32 data_830[] = {
    0x00040010, 0x00110011, 0x00100011, 0x00110005, 0xffff0006, 0x0009000c, 0x0009000b, 0x000a0009, 
    0x000dffff, 0x00070009, 0x000b0009, 0x0009000c, 0x000a000e, 0xffff0008, 0x000a0009, 0x000c000a, 
    0x0009000b, 0x000fffff, 0x00020000, 0x00010000, 0x00010001, 0x00000003, 0xffff0000
};
/*0x88C*/ static u32 data_88C = 0x02040103;

/*0x0*/ static u8 bss_0[0x1];
/*0x1*/ static u8 bss_1[0x1];
/*0x2*/ static u8 bss_2[0x1];
/*0x3*/ static u8 bss_3[0x1];
/*0x4*/ static u8 _bss_4[0x4];
/*0x8*/ static u8 bss_8[0x8];
/*0x10*/ static u8 _bss_10[0xc0];
/*0xD0*/ static u8 bss_D0[0x48];
/*0x118*/ static u8 bss_118[0x4];
/*0x11C*/ static u8 bss_11C[0x4];
/*0x120*/ static u8 bss_120[0x4];
/*0x124*/ static u8 bss_124[0x4];
/*0x128*/ static u8 bss_128[0x4];
/*0x12C*/ static u8 bss_12C[0x1];
/*0x12D*/ static u8 bss_12D[0x1];
/*0x12E*/ static u8 _bss_12E[0x2];
/*0x130*/ static u8 bss_130[0x4];
/*0x134*/ static u8 bss_134[0x4];
/*0x138*/ static u8 _bss_138[0x8];
/*0x140*/ static u8 bss_140[0x8];
/*0x148*/ static u8 bss_148[0x8];

// offset: 0x0 | ctor
void dll_65_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_ctor.s")

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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_16A4.s")

// offset: 0x1718 | func: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_1718.s")

// offset: 0x1898 | func: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_1898.s")

// offset: 0x1AE4 | func: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_1AE4.s")

// offset: 0x1CF4 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_1CF4.s")

// offset: 0x2088 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_2088.s")

// offset: 0x2438 | func: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_2438.s")

// offset: 0x26D8 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_26D8.s")

// offset: 0x2888 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_2888.s")

// offset: 0x2A1C | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_2A1C.s")

// offset: 0x2B50 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_2B50.s")

// offset: 0x2C58 | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/65_options/dll_65_func_2C58.s")

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
