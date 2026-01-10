#include "common.h"
#include "dlls/objects/210_player.h"
#include "prevent_bss_reordering.h"

/*0x0*/ static u32 _data_0[] = {
    0xfc121603, 0xfffffff8, 0xfc121603, 0xfffffff8
};
/*0x10*/ static u32 _data_10[] = {
    0xef182c00, 0x03024000, 0xef182c00, 0x00112008, 0xef182c00, 0x00112230, 0xef182c00, 0x00112038, 
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x00104a50, 0xef182c00, 0x001049d8, 
    0xef182c00, 0xcb024000, 0xef182c00, 0xc8112008, 0xef182c00, 0xc8112230, 0xef182c00, 0xc8112038, 
    0xef182c00, 0xc8104240, 0xef182c00, 0xc81041c8, 0xef182c00, 0xc8104a50, 0xef182c00, 0xc81049d8
};
/*0x90*/ static u32 _data_90[] = {
    0xef182c00, 0x03024000, 0xef182c00, 0x00112008, 0xef182c00, 0x00112230, 0xef182c00, 0x00112038, 
    0xef182c00, 0x00104340, 0xef182c00, 0x00104340, 0xef182c00, 0x00104b50, 0xef182c00, 0x00104b50, 
    0xef182c00, 0xcb024000, 0xef182c00, 0xc8112008, 0xef182c00, 0xc8112230, 0xef182c00, 0xc8112038, 
    0xef182c00, 0xc8104340, 0xef182c00, 0xc8104340, 0xef182c00, 0xc8104b50, 0xef182c00, 0xc8104b50
};
/*0x110*/ static u32 _data_110[] = {
    0xfc41ffff, 0xfffff638, 0xfc41ffff, 0xfffff638
};
/*0x120*/ static u32 _data_120[] = {
    0xef180c00, 0x03024000, 0xef180c00, 0x00112008, 0xef180c00, 0x00112230, 0xef180c00, 0x00112038, 
    0xef180c00, 0x00104240, 0xef180c00, 0x001041c8, 0xef180c00, 0x00104a50, 0xef180c00, 0x001049d8, 
    0xef180c00, 0xcb024000, 0xef180c00, 0xc8112008, 0xef180c00, 0xc8112230, 0xef180c00, 0xc8112038, 
    0xef180c00, 0xc8104240, 0xef180c00, 0xc81041c8, 0xef180c00, 0xc8104a50, 0xef180c00, 0xc81049d8
};
/*0x1A0*/ static u32 _data_1A0[] = {
    0xfc121803, 0xff0fffff, 0xfc121803, 0xff0fffff
};
/*0x1B0*/ static u32 _data_1B0[] = {
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x00104a50, 0xef182c00, 0x001049d8, 
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x00104a50, 0xef182c00, 0x001049d8, 
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x00104a50, 0xef182c00, 0x001049d8, 
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x00104a50, 0xef182c00, 0x001049d8
};
/*0x230*/ static u32 _data_230[] = {
    0xfc41c683, 0xff8fffff, 0xfc41c683, 0xff8fffff
};
/*0x240*/ static u32 _data_240[] = {
    0xef080c00, 0x0c184240, 0xef080c00, 0x005461c8, 0xef080c00, 0x00546a70, 0xef080c00, 0x005469f8, 
    0xef080c00, 0x00504240, 0xef080c00, 0x005041c8, 0xef080c00, 0x00504a50, 0xef080c00, 0x005049d8, 
    0xef080c00, 0x0c184240, 0xef080c00, 0x005461c8, 0xef080c00, 0x00546a70, 0xef080c00, 0x005469f8, 
    0xef080c00, 0x00504240, 0xef080c00, 0x005041c8, 0xef080c00, 0x00504a50, 0xef080c00, 0x005049d8
};
/*0x2C0*/ static u32 _data_2C0[] = {
    0xfc12160b, 0xfffffff8, 0xfc12160b, 0xfffffff8
};
/*0x2D0*/ static u32 _data_2D0[] = {
    0xef182c00, 0x03024000, 0xef182c00, 0x00112008, 0xef182c00, 0x00112230, 0xef182c00, 0x00112038, 
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x00104a50, 0xef182c00, 0x001049d8
};
/*0x310*/ static u32 _data_310[] = {
    0xfc45ffff, 0xfffff638, 0xfc45ffff, 0xfffff638
};
/*0x320*/ static u32 _data_320[] = {
    0xef180c00, 0x03024000, 0xef180c00, 0x00112008, 0xef180c00, 0x00112230, 0xef180c00, 0x00112038, 
    0xef180c00, 0x00104240, 0xef180c00, 0x001041c8, 0xef180c00, 0x00104a50, 0xef180c00, 0x001049d8
};
/*0x360*/ static u32 _data_360[] = {
    0xfc12166b, 0xf0fffe38, 0xfc12166b, 0xf0fffe38
};
/*0x370*/ static u32 _data_370[] = {
    0xef182c00, 0x03024000, 0xef182c00, 0x00112008, 0xef182c00, 0x00112230, 0xef182c00, 0x00112038, 
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x00104a50, 0xef182c00, 0x001049d8
};
/*0x3B0*/ static u32 _data_3B0[] = {
    0xfc35ffff, 0x4ffc7638, 0xfc35ffff, 0x4ffc7638
};
/*0x3C0*/ static u32 _data_3C0[] = {
    0xef180c00, 0x03024000, 0xef180c00, 0x00112008, 0xef180c00, 0x00112230, 0xef180c00, 0x00112038, 
    0xef180c00, 0x00104240, 0xef180c00, 0x001041c8, 0xef180c00, 0x00104a50, 0xef180c00, 0x001049d8
};
/*0x400*/ static u32 _data_400[] = {
    0xfc26a04d, 0x11409249, 0xfc26a004, 0x1f0c93ff
};
/*0x410*/ static u32 _data_410[] = {
    0xef192c00, 0x03024000, 0xef192c00, 0x00112008, 0xef192c00, 0x00112230, 0xef192c00, 0x00112038, 
    0xef192c00, 0x00104240, 0xef192c00, 0x001041c8, 0xef192c00, 0x00104a50, 0xef192c00, 0x001049d8, 
    0xef192c00, 0xcb024000, 0xef192c00, 0xc8112008, 0xef192c00, 0xc8112230, 0xef192c00, 0xc8112038, 
    0xef192c00, 0xc8104240, 0xef192c00, 0xc81041c8, 0xef192c00, 0xc8104a50, 0xef192c00, 0xc81049d8
};
/*0x490*/ static u32 _data_490[] = {
    0xfc22aa04, 0x1f0c93ff, 0xfc22aa04, 0x1f0c93ff
};
/*0x4A0*/ static u32 _data_4A0[] = {
    0xef182c00, 0x03024000, 0xef182c00, 0x00112008, 0xef182c00, 0x00112230, 0xef182c00, 0x00112038, 
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x00104a50, 0xef182c00, 0x001049d8, 
    0xef182c00, 0xcb024000, 0xef182c00, 0xc8112008, 0xef182c00, 0xc8112230, 0xef182c00, 0xc8112038, 
    0xef182c00, 0xc8104240, 0xef182c00, 0xc81041c8, 0xef182c00, 0xc8104a50, 0xef182c00, 0xc81049d8
};
/*0x520*/ static u32 _data_520[] = {
    0xfc22aa04, 0x1f1093ff, 0xfc22aa04, 0x1f1093ff
};
/*0x530*/ static u32 _data_530[] = {
    0xfc25a804, 0x1f0c93ff, 0xfc25a804, 0x1f0c93ff
};
/*0x540*/ static u32 _data_540[] = {
    0xfc25a803, 0x1f0c93ff, 0xfc25a803, 0x1f0c93ff
};
/*0x550*/ static u32 _data_550[] = {
    0xfc119623, 0xff2fffff, 0xfc1196ac, 0xf0fffe38
};
/*0x560*/ static u32 _data_560[] = {
    0xfc367ea0, 0x5f0ef3ff, 0xfc367ea0, 0x5f0ef3ff
};
/*0x570*/ static u32 _data_570[] = {
    0xef082c00, 0x00504240, 0xef082c00, 0x005041c8, 0xef082c00, 0x00553078, 0xef082c00, 0x005045d8, 
    0xef082c00, 0x00504240, 0xef082c00, 0x005041c8, 0xef082c00, 0x00553078, 0xef082c00, 0x005045d8, 
    0xef182c00, 0xc8104240, 0xef182c00, 0xc81041c8, 0xef182c00, 0xc8113078, 0xef182c00, 0xc81045d8, 
    0xef182c00, 0xc8104240, 0xef182c00, 0xc81041c8, 0xef182c00, 0xc81045f8, 0xef182c00, 0xc81045d8
};
/*0x5F0*/ static u32 _data_5F0[] = {
    0xef080c00, 0x00504240, 0xef080c00, 0x005041c8, 0xef080c00, 0x00553078, 0xef080c00, 0x005045d8, 
    0xef080c00, 0x00504240, 0xef080c00, 0x005041c8, 0xef080c00, 0x00553078, 0xef080c00, 0x005045d8, 
    0xef180c00, 0xc8104240, 0xef180c00, 0xc81041c8, 0xef180c00, 0xc8113078, 0xef180c00, 0xc81045d8, 
    0xef180c00, 0xc8104240, 0xef180c00, 0xc81041c8, 0xef180c00, 0xc81045f8, 0xef180c00, 0xc81045d8
};
/*0x670*/ static u32 _data_670[] = {
    0xef082c80, 0x00504240, 0xef082c80, 0x005041c8, 0xef082c80, 0x00553078, 0xef082c80, 0x00504b50, 
    0xef082c80, 0x00504240, 0xef082c80, 0x005041c8, 0xef082c80, 0x00553078, 0xef082c80, 0x00504b50, 
    0xef182c80, 0xc8104240, 0xef182c80, 0xc81041c8, 0xef182c80, 0xc8113078, 0xef182c80, 0xc8104b50, 
    0xef182c80, 0xc8104240, 0xef182c80, 0xc81041c8, 0xef182c80, 0xc81045f8, 0xef182c80, 0xc8104b50
};
/*0x6F0*/ static u32 _data_6F0[] = {
    0xfc22aa04, 0x1f0c93ff, 0xfc22aa04, 0x1f0c93ff
};
/*0x700*/ static u32 _data_700[] = {
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x00113078, 0xef182c00, 0x001045d8, 
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x001045f8, 0xef182c00, 0x001045d8, 
    0xef182c00, 0xc8104240, 0xef182c00, 0xc81041c8, 0xef182c00, 0xc8113078, 0xef182c00, 0xc81045d8, 
    0xef182c00, 0xc8104240, 0xef182c00, 0xc81041c8, 0xef182c00, 0xc81045f8, 0xef182c00, 0xc81045d8
};
/*0x780*/ static u32 _data_780[] = {
    0xef180c00, 0x00104240, 0xef180c00, 0x001041c8, 0xef180c00, 0x00113078, 0xef180c00, 0x001045d8, 
    0xef180c00, 0x00104240, 0xef180c00, 0x001041c8, 0xef180c00, 0x001045f8, 0xef180c00, 0x001045d8, 
    0xef180c00, 0xc8104240, 0xef180c00, 0xc81041c8, 0xef180c00, 0xc8113078, 0xef180c00, 0xc81045d8, 
    0xef180c00, 0xc8104240, 0xef180c00, 0xc81041c8, 0xef180c00, 0xc81045f8, 0xef180c00, 0xc81045d8
};
/*0x800*/ static u32 _data_800[] = {
    0xfc121603, 0xfffffff8, 0xfc121603, 0xfffffff8
};
/*0x810*/ static u32 _data_810[] = {
    0xef182c00, 0x00112e10, 0xef182c00, 0x00112d18, 0xef182c00, 0x00112e10, 0xef182c00, 0x00112d18, 
    0xef182c00, 0x00104e50, 0xef182c00, 0x00104dd8, 0xef182c00, 0x00104e50, 0xef182c00, 0x00104dd8, 
    0xef182c00, 0xc8112e10, 0xef182c00, 0xc8112d18, 0xef182c00, 0xc8112e10, 0xef182c00, 0xc8112d18, 
    0xef182c00, 0xc8104e50, 0xef182c00, 0xc8104dd8, 0xef182c00, 0xc8104e50, 0xef182c00, 0xc8104dd8
};
/*0x890*/ static u32 _data_890[] = {
    0xfc121603, 0xfffffff8, 0xfc121603, 0xfffffff8
};
/*0x8A0*/ static u32 _data_8A0[] = {
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x00111338, 0xef182c00, 0x00111038, 
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x00111338, 0xef182c00, 0x00111038, 
    0xef182c00, 0xc8104240, 0xef182c00, 0xc81041c8, 0xef182c00, 0xc8111338, 0xef182c00, 0xc8111038, 
    0xef182c00, 0xc8104240, 0xef182c00, 0xc81041c8, 0xef182c00, 0xc8111338, 0xef182c00, 0xc8111038
};
/*0x920*/ static u32 _data_920[] = {
    0xef182c00, 0xc8104240, 0xef182c00, 0xc81041c8, 0xef182c00, 0xc8113078, 0xef182c00, 0xc8105858, 
    0xef182c00, 0xc8104240, 0xef182c00, 0xc81041c8, 0xef182c00, 0xc8113078, 0xef182c00, 0xc8105858
};
/*0x960*/ static u32 _data_960[] = {
    0xfc121803, 0xff0fffff, 0xfc121803, 0xff0fffff
};
/*0x970*/ static u32 _data_970[] = {
    0xef182c00, 0x00104e50, 0xef182c00, 0x00104dd8, 0xef182c00, 0x00104e50, 0xef182c00, 0x00104dd8, 
    0xef182c00, 0x00104b50, 0xef182c00, 0x00104b50, 0xef182c00, 0x00104b50, 0xef182c00, 0x00104b50, 
    0xef182c00, 0x00104e50, 0xef182c00, 0x00104dd8, 0xef182c00, 0x00104e50, 0xef182c00, 0x00104dd8, 
    0xef182c00, 0x00104b50, 0xef182c00, 0x00104b50, 0xef182c00, 0x00104b50, 0xef182c00, 0x00104b50
};
/*0x9F0*/ static u32 _data_9F0[] = {
    0xfc26a004, 0x1f1093ff, 0xfc26a004, 0x1f1093ff
};
/*0xA00*/ static u32 _data_A00[] = {
    0xef192c00, 0x00104e50, 0xef192c00, 0x00104dd8, 0xef192c00, 0x00104e50, 0xef192c00, 0x00104dd8, 
    0xef192c00, 0x00104a50, 0xef192c00, 0x001049d8, 0xef192c00, 0x00104a50, 0xef192c00, 0x001049d8, 
    0xef192c00, 0x00104e50, 0xef192c00, 0x00104dd8, 0xef192c00, 0x00104e50, 0xef192c00, 0x00104dd8, 
    0xef192c00, 0x00104a50, 0xef192c00, 0x001049d8, 0xef192c00, 0x00104a50, 0xef192c00, 0x001049d8
};
/*0xA80*/ static u32 _data_A80[] = {
    0xfc121603, 0xff0fffff, 0xfc121603, 0xff0fffff
};
/*0xA90*/ static u32 _data_A90[] = {
    0xef182c00, 0x03024000, 0xef182c00, 0x00112248, 0xef182c00, 0x00112230, 0xef182c00, 0x00112278, 
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x00104a50, 0xef182c00, 0x001049d8, 
    0xef182c00, 0xcb024000, 0xef182c00, 0xc8112248, 0xef182c00, 0xc8112230, 0xef182c00, 0xc8112278, 
    0xef182c00, 0xc8104240, 0xef182c00, 0xc81041c8, 0xef182c00, 0xc8104a50, 0xef182c00, 0xc81049d8
};
/*0xB10*/ static u32 _data_B10[] = {
    0xfc26a004, 0x1f0c93ff, 0xfc26a004, 0x1f0c93ff
};
/*0xB20*/ static u32 _data_B20[] = {
    0xef192c00, 0x03024000, 0xef192c00, 0x00112248, 0xef192c00, 0x00112230, 0xef192c00, 0x00112278, 
    0xef192c00, 0x00104240, 0xef192c00, 0x001041c8, 0xef192c00, 0x00104a50, 0xef192c00, 0x001049d8, 
    0xef192c00, 0xcb024000, 0xef192c00, 0xc8112248, 0xef192c00, 0xc8112230, 0xef192c00, 0xc8112278, 
    0xef192c00, 0xc8104240, 0xef192c00, 0xc81041c8, 0xef192c00, 0xc8104a50, 0xef192c00, 0xc81049d8
};
/*0xBA0*/ static u32 _data_BA0[] = {
    0xfc55fe04, 0x1ffcfdfe, 0xfc55fe04, 0x1ffcfdfe
};
/*0xBB0*/ static u32 _data_BB0[] = {
    0xef182c00, 0x03024000, 0xef182c00, 0x00112008, 0xef182c00, 0x00112230, 0xef182c00, 0x00112038, 
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x00104a50, 0xef182c00, 0x001049d8, 
    0xef182c00, 0x03024000, 0xef182c00, 0x00112008, 0xef182c00, 0x00112230, 0xef182c00, 0x00112038, 
    0xef182c00, 0x00104240, 0xef182c00, 0x001041c8, 0xef182c00, 0x00104a50, 0xef182c00, 0x001049d8
};
/*0xC30*/ static u32 _data_C30 = (u32)&_data_0;
/*0xC34*/ static u32 _data_C34[] = {
    (u32)&_data_10, 0x0000000f, 0x00000000
};
/*0xC40*/ static u32 _data_C40 = (u32)&_data_1A0;
/*0xC44*/ static u32 _data_C44[] = {
    (u32)&_data_1B0, 0x00000007, 0x00000004
};
/*0xC50*/ static u32 _data_C50 = (u32)&_data_2C0;
/*0xC54*/ static u32 _data_C54[] = {
    (u32)&_data_2D0, 0x00000007, 0x00000000
};
/*0xC60*/ static u32 _data_C60 = (u32)&_data_360;
/*0xC64*/ static u32 _data_C64[] = {
    (u32)&_data_370, 0x00000007, 0x00000000
};
/*0xC70*/ static u32 _data_C70 = (u32)&_data_490;
/*0xC74*/ static u32 _data_C74[] = {
    (u32)&_data_4A0, 0x0000000f, 0x00000000
};
/*0xC80*/ static u32 _data_C80 = (u32)&_data_520;
/*0xC84*/ static u32 _data_C84[] = {
    (u32)&_data_4A0, 0x00000007, 0x00000004
};
/*0xC90*/ static u32 _data_C90 = (u32)&_data_530;
/*0xC94*/ static u32 _data_C94[] = {
    (u32)&_data_4A0, 0x00000007, 0x00000000
};
/*0xCA0*/ static u32 _data_CA0 = (u32)&_data_540;
/*0xCA4*/ static u32 _data_CA4[] = {
    (u32)&_data_4A0, 0x00000007, 0x00000000
};
/*0xCB0*/ static u32 _data_CB0 = (u32)&_data_800;
/*0xCB4*/ static u32 _data_CB4[] = {
    (u32)&_data_810, 0x0000000f, 0x00000002
};
/*0xCC0*/ static u32 _data_CC0 = (u32)&_data_800;
/*0xCC4*/ static u32 _data_CC4[] = {
    (u32)&_data_810, 0x0000000f, 0x00000002
};
/*0xCD0*/ static u32 _data_CD0 = (u32)&_data_800;
/*0xCD4*/ static u32 _data_CD4[] = {
    (u32)&_data_810, 0x0000000f, 0x00000002
};
/*0xCE0*/ static u32 _data_CE0 = (u32)&_data_800;
/*0xCE4*/ static u32 _data_CE4[] = {
    (u32)&_data_810, 0x0000000f, 0x00000002
};
/*0xCF0*/ static u32 _data_CF0 = (u32)&_data_490;
/*0xCF4*/ static u32 _data_CF4[] = {
    (u32)&_data_810, 0x0000000f, 0x00000002
};
/*0xD00*/ static u32 _data_D00 = (u32)&_data_520;
/*0xD04*/ static u32 _data_D04[] = {
    (u32)&_data_810, 0x0000000f, 0x00000006
};
/*0xD10*/ static u32 _data_D10 = (u32)&_data_530;
/*0xD14*/ static u32 _data_D14[] = {
    (u32)&_data_810, 0x0000000f, 0x00000002
};
/*0xD20*/ static u32 _data_D20 = (u32)&_data_540;
/*0xD24*/ static u32 _data_D24[] = {
    (u32)&_data_810, 0x0000000f, 0x00000002
};
/*0xD30*/ static u32 _data_D30 = (u32)&_data_890;
/*0xD34*/ static u32 _data_D34[] = {
    (u32)&_data_8A0, 0x0000000f, 0x00000000
};
/*0xD40*/ static u32 _data_D40 = (u32)&_data_890;
/*0xD44*/ static u32 _data_D44[] = {
    (u32)&_data_8A0, 0x0000000f, 0x00000000
};
/*0xD50*/ static u32 _data_D50 = (u32)&_data_890;
/*0xD54*/ static u32 _data_D54[] = {
    (u32)&_data_8A0, 0x0000000f, 0x00000000
};
/*0xD60*/ static u32 _data_D60 = (u32)&_data_890;
/*0xD64*/ static u32 _data_D64[] = {
    (u32)&_data_8A0, 0x0000000f, 0x00000000
};
/*0xD70*/ static u32 _data_D70 = (u32)&_data_490;
/*0xD74*/ static u32 _data_D74[] = {
    (u32)&_data_920, 0x00000007, 0x00000000
};
/*0xD80*/ static u32 _data_D80 = (u32)&_data_520;
/*0xD84*/ static u32 _data_D84[] = {
    (u32)&_data_920, 0x00000007, 0x00000004
};
/*0xD90*/ static u32 _data_D90 = (u32)&_data_530;
/*0xD94*/ static u32 _data_D94[] = {
    (u32)&_data_920, 0x00000007, 0x00000000
};
/*0xDA0*/ static u32 _data_DA0 = (u32)&_data_540;
/*0xDA4*/ static u32 _data_DA4[] = {
    (u32)&_data_920, 0x00000007, 0x00000000
};
/*0xDB0*/ static u32 _data_DB0 = (u32)&_data_A80;
/*0xDB4*/ static u32 _data_DB4[] = {
    (u32)&_data_A90, 0x0000000f, 0x00000000
};
/*0xDC0*/ static u32 _data_DC0 = (u32)&_data_1A0;
/*0xDC4*/ static u32 _data_DC4[] = {
    (u32)&_data_A90, 0x0000000f, 0x00000004
};
/*0xDD0*/ static u32 _data_DD0 = (u32)&_data_2C0;
/*0xDD4*/ static u32 _data_DD4[] = {
    (u32)&_data_A90, 0x0000000f, 0x00000000
};
/*0xDE0*/ static u32 _data_DE0 = (u32)&_data_360;
/*0xDE4*/ static u32 _data_DE4[] = {
    (u32)&_data_A90, 0x0000000f, 0x00000000
};
/*0xDF0*/ static u32 _data_DF0 = (u32)&_data_490;
/*0xDF4*/ static u32 _data_DF4[] = {
    (u32)&_data_A90, 0x0000000f, 0x00000000
};
/*0xE00*/ static u32 _data_E00 = (u32)&_data_520;
/*0xE04*/ static u32 _data_E04[] = {
    (u32)&_data_A90, 0x0000000f, 0x00000004
};
/*0xE10*/ static u32 _data_E10 = (u32)&_data_530;
/*0xE14*/ static u32 _data_E14[] = {
    (u32)&_data_A90, 0x0000000f, 0x00000000
};
/*0xE20*/ static u32 _data_E20 = (u32)&_data_540;
/*0xE24*/ static u32 _data_E24[] = {
    (u32)&_data_A90, 0x0000000f, 0x00000000
};
/*0xE30*/ static u32 _data_E30 = (u32)&_data_400;
/*0xE34*/ static u32 _data_E34[] = {
    (u32)&_data_410, 0x0000000f, 0x00000000
};
/*0xE40*/ static u32 _data_E40 = (u32)&_data_B10;
/*0xE44*/ static u32 _data_E44[] = {
    (u32)&_data_B20, 0x00000007, 0x00000000
};
/*0xE50*/ static u32 _data_E50 = (u32)&_data_960;
/*0xE54*/ static u32 _data_E54[] = {
    (u32)&_data_970, 0x00000007, 0x00000002
};
/*0xE60*/ static u32 _data_E60 = (u32)&_data_9F0;
/*0xE64*/ static u32 _data_E64[] = {
    (u32)&_data_A00, 0x00000007, 0x00000002
};
/*0xE70*/ static u32 _data_E70 = (u32)&_data_BA0;
/*0xE74*/ static u32 _data_E74[] = {
    (u32)&_data_BB0, 0x0000000b, 0x00000000
};
/*0xE80*/ static u32 _data_E80 = (u32)&_data_0;
/*0xE84*/ static u32 _data_E84[] = {
    (u32)&_data_90, 0x0000000f, 0x00000000
};
/*0xE90*/ static u32 _data_E90 = (u32)&_data_110;
/*0xE94*/ static u32 _data_E94[] = {
    (u32)&_data_120, 0x0000000f, 0x00000000
};
/*0xEA0*/ static u32 _data_EA0 = (u32)&_data_230;
/*0xEA4*/ static u32 _data_EA4[] = {
    (u32)&_data_240, 0x00000007, 0x00000004
};
/*0xEB0*/ static u32 _data_EB0 = (u32)&_data_310;
/*0xEB4*/ static u32 _data_EB4[] = {
    (u32)&_data_320, 0x00000007, 0x00000000
};
/*0xEC0*/ static u32 _data_EC0 = (u32)&_data_3B0;
/*0xEC4*/ static u32 _data_EC4[] = {
    (u32)&_data_3C0, 0x00000007, 0x00000000
};
/*0xED0*/ static u32 _data_ED0 = (u32)&_data_110;
/*0xED4*/ static u32 _data_ED4[] = {
    (u32)&_data_120, 0x0000000f, 0x00000000
};
/*0xEE0*/ static u32 _data_EE0 = (u32)&_data_230;
/*0xEE4*/ static u32 _data_EE4[] = {
    (u32)&_data_240, 0x00000007, 0x00000004
};
/*0xEF0*/ static u32 _data_EF0 = (u32)&_data_310;
/*0xEF4*/ static u32 _data_EF4[] = {
    (u32)&_data_320, 0x00000007, 0x00000000
};
/*0xF00*/ static u32 _data_F00 = (u32)&_data_3B0;
/*0xF04*/ static u32 _data_F04[] = {
    (u32)&_data_3C0, 0x00000007, 0x00000000
};
/*0xF10*/ static u32 _data_F10 = (u32)&_data_550;
/*0xF14*/ static u32 _data_F14[] = {
    (u32)&_data_570, 0x0000000f, 0x00000000
};
/*0xF20*/ static u32 _data_F20 = (u32)&_data_6F0;
/*0xF24*/ static u32 _data_F24[] = {
    (u32)&_data_700, 0x0000000f, 0x00000000
};
/*0xF30*/ static u32 _data_F30 = (u32)&_data_550;
/*0xF34*/ static u32 _data_F34[] = {
    (u32)&_data_5F0, 0x0000000f, 0x00000000
};
/*0xF40*/ static u32 _data_F40 = (u32)&_data_6F0;
/*0xF44*/ static u32 _data_F44[] = {
    (u32)&_data_780, 0x0000000f, 0x00000000
};
/*0xF50*/ static u32 _data_F50 = (u32)&_data_560;
/*0xF54*/ static u32 _data_F54[] = {
    (u32)&_data_570, 0x0000000f, 0x00000000
};
/*0xF60*/ static u32 _data_F60 = (u32)&_data_560;
/*0xF64*/ static u32 _data_F64[] = {
    (u32)&_data_670, 0x0000000f, 0x00000000, 0xf5101000, 0x00014050, 0xf2000000, 0x0007c07c, 0xf5100900, 
    0x01010441, 0xf2000000, 0x0103c03c, 0xf5100540, 0x0200c832, 0xf2000000, 0x0201c01c, 0xf5100350, 
    0x03008c23, 0xf2000000, 0x0300c00c, 0xf5101000, 0x00080050, 0xf2000000, 0x0007c07c, 0xf5100900, 
    0x01080441, 0xf2000000, 0x0103c03c, 0xf5100540, 0x02080832, 0xf2000000, 0x0201c01c, 0xf5100350, 
    0x03080c23, 0xf2000000, 0x0300c00c, 0xf5101000, 0x00014200, 0xf2000000, 0x0007c07c, 0xf5100900, 
    0x01010601, 0xf2000000, 0x0103c03c, 0xf5100540, 0x0200ca02, 0xf2000000, 0x0201c01c, 0xf5100350, 
    0x03008e03, 0xf2000000, 0x0300c00c, 0xf5100400, 0x00018050, 0xf2000000, 0x0007c07c, 0xf5100280, 
    0x01414441, 0xf2000000, 0x0103c03c, 0xf51002a0, 0x02810832, 0xf2000000, 0x0201c01c, 0xf51002a8, 
    0x03c0cc23, 0xf2000000, 0x0300c00c, 0xf5100400, 0x00080050, 0xf2000000, 0x0007c07c, 0xf5100280, 
    0x01480441, 0xf2000000, 0x0103c03c, 0xf51002a0, 0x02880832, 0xf2000000, 0x0201c01c, 0xf51002a8, 
    0x03c80c23, 0xf2000000, 0x0300c00c, 0xf5100400, 0x00018200, 0xf2000000, 0x0007c07c, 0xf5100280, 
    0x01414601, 0xf2000000, 0x0103c03c, 0xf51002a0, 0x02810a02, 0xf2000000, 0x0201c01c, 0xf51002a8, 
    0x03c0ce03, 0xf2000000, 0x0300c00c, 0xf5180800, 0x00010040, 0xf5180440, 0x0100c431, 0xf5180250, 
    0x02008822, 0xf5180254, 0x03008822, 0xf2000000, 0x0003c03c, 0xf2000000, 0x0001c01c, 0xf2000000, 
    0x0000c00c, 0xf2000000, 0x0000c00c
};
/*0x1130*/ static u32 _data_1130[] = {
    0xc4bb8000, 0x00000000, 0xc4bb8000, 0xc4bb8000, 0x00000000, 0x44bb8000, 0x44bb8000, 0x00000000, 
    0x44bb8000, 0x44bb8000, 0x00000000, 0xc4bb8000, 0xc4bb8000, 0x00000000, 0xc4bb8000, 0xc4bb8000, 
    0x00000000, 0x44bb8000, 0x44bb8000, 0x00000000, 0x44bb8000, 0x44bb8000, 0x00000000, 0xc4bb8000, 
    0xc4bb8000, 0x00000000, 0xc4bb8000, 0xc4bb8000, 0x00000000, 0x44bb8000, 0x44bb8000, 0x00000000, 
    0x44bb8000, 0x44bb8000, 0x00000000, 0xc4bb8000
};
/*0x11C0*/ static u32 _data_11C0[] = {
    0x00000000, 0xc4bb8000, 0xc4bb8000, 0x00000000, 0xc4bb8000, 0x44bb8000, 0x00000000, 0x44bb8000, 
    0x44bb8000, 0x00000000, 0x44bb8000, 0xc4bb8000, 0x00000000, 0xc4bb8000, 0xc4bb8000, 0x00000000, 
    0xc4bb8000, 0x44bb8000, 0x00000000, 0x44bb8000, 0x44bb8000, 0x00000000, 0x44bb8000, 0xc4bb8000, 
    0x00000000, 0xc4bb8000, 0xc4bb8000, 0x00000000, 0xc4bb8000, 0x44bb8000, 0x00000000, 0x44bb8000, 
    0x44bb8000, 0x00000000, 0x44bb8000, 0xc4bb8000
};
/*0x1250*/ static u32 _data_1250[] = {
    0xc4bb8000, 0xc4bb8000, 0x00000000, 0x44bb8000, 0xc4bb8000, 0x00000000, 0x44bb8000, 0x44bb8000, 
    0x00000000, 0xc4bb8000, 0x44bb8000, 0x00000000, 0xc4bb8000, 0xc4bb8000, 0x00000000, 0x44bb8000, 
    0xc4bb8000, 0x00000000, 0x44bb8000, 0x44bb8000, 0x00000000, 0xc4bb8000, 0x44bb8000, 0x00000000, 
    0xc4bb8000, 0xc4bb8000, 0x00000000, 0x44bb8000, 0xc4bb8000, 0x00000000, 0x44bb8000, 0x44bb8000, 
    0x00000000, 0xc4bb8000, 0x44bb8000, 0x00000000
};
/*0x12E0*/ static u32 _data_12E0[] = {
    0xfe0cfc7c, 0xfe0c0000, 0x00000000, 0xffffffff, 0x01f4fc7c, 0xfe0c0000, 0x01e00000, 0xffffffff, 
    0x01f4fc7c, 0x01f40000, 0x03e00000, 0xffffffff, 0xfe0cfc7c, 0x01f40000, 0x05e00000, 0xffffffff, 
    0xfe0cfc7c, 0xfe0c0000, 0x000003e0, 0xffffffff, 0x01f4fc7c, 0xfe0c0000, 0x01e003e0, 0xffffffff, 
    0x01f4fc7c, 0x01f40000, 0x03e003e0, 0xffffffff, 0xfe0cfc7c, 0x01f40000, 0x05e003e0, 0xffffffff, 
    0xfe0cfc7c, 0xfe0c0000, 0x000007e0, 0xffffffff, 0x01f4fc7c, 0xfe0c0000, 0x01e007e0, 0xffffffff, 
    0x01f4fc7c, 0x01f40000, 0x03e007e0, 0xffffffff, 0xfe0cfc7c, 0x01f40000, 0x05e007e0, 0xffffffff, 
    0xfe0cfc7c, 0xfe0c0000, 0x00000be0, 0xffffffff, 0x01f4fc7c, 0xfe0c0000, 0x01e00be0, 0xffffffff, 
    0x01f4fc7c, 0x01f40000, 0x03e00be0, 0xffffffff, 0xfe0cfc7c, 0x01f40000, 0x05e00be0, 0xffffffff, 
    0xfe0cfc7c, 0xfe0c0000, 0x00000fe0, 0xffffffff, 0x01f4fc7c, 0xfe0c0000, 0x01e00fe0, 0xffffffff, 
    0x01f4fc7c, 0x01f40000, 0x03e00fe0, 0xffffffff, 0xfe0cfc7c, 0x01f40000, 0x05e00fe0, 0xffffffff
};
/*0x1420*/ static u32 _data_1420[] = {
    0x00000405, 0x00000000, 0x00000000, 0x00000000, 0x00000501, 0x00000000, 0x00000000, 0x00000000, 
    0x00010506, 0x00000000, 0x00000000, 0x00000000, 0x00010602, 0x00000000, 0x00000000, 0x00000000, 
    0x00020607, 0x00000000, 0x00000000, 0x00000000, 0x00020703, 0x00000000, 0x00000000, 0x00000000, 
    0x00030704, 0x00000000, 0x00000000, 0x00000000, 0x00030400, 0x00000000, 0x00000000, 0x00000000, 
    0x00040809, 0x00000000, 0x00000000, 0x00000000, 0x00040905, 0x00000000, 0x00000000, 0x00000000, 
    0x0005090a, 0x00000000, 0x00000000, 0x00000000, 0x00050a06, 0x00000000, 0x00000000, 0x00000000, 
    0x00060a0b, 0x00000000, 0x00000000, 0x00000000, 0x00060b07, 0x00000000, 0x00000000, 0x00000000, 
    0x00070b08, 0x00000000, 0x00000000, 0x00000000, 0x00070804, 0x00000000, 0x00000000, 0x00000000, 
    0x00080c0d, 0x00000000, 0x00000000, 0x00000000, 0x00080d09, 0x00000000, 0x00000000, 0x00000000, 
    0x00090d0e, 0x00000000, 0x00000000, 0x00000000, 0x00090e0a, 0x00000000, 0x00000000, 0x00000000, 
    0x000a0e0f, 0x00000000, 0x00000000, 0x00000000, 0x000a0f0b, 0x00000000, 0x00000000, 0x00000000, 
    0x000b0f0c, 0x00000000, 0x00000000, 0x00000000, 0x000b0c08, 0x00000000, 0x00000000, 0x00000000, 
    0x000c1011, 0x00000000, 0x00000000, 0x00000000, 0x000c110d, 0x00000000, 0x00000000, 0x00000000, 
    0x000d1112, 0x00000000, 0x00000000, 0x00000000, 0x000d120e, 0x00000000, 0x00000000, 0x00000000, 
    0x000e1213, 0x00000000, 0x00000000, 0x00000000, 0x000e130f, 0x00000000, 0x00000000, 0x00000000, 
    0x000f1310, 0x00000000, 0x00000000, 0x00000000, 0x000f100c, 0x00000000, 0x00000000, 0x00000000, 
    0x00000201, 0x00000000, 0x00000000, 0x00000000, 0x00000203, 0x00000000, 0x00000000, 0x00000000, 
    0x00040605, 0x00000000, 0x00000000, 0x00000000, 0x00040607, 0x00000000, 0x00000000, 0x00000000, 
    0x00080a09, 0x00000000, 0x00000000, 0x00000000, 0x00080a0b, 0x00000000, 0x00000000, 0x00000000, 
    0x000c0e0d, 0x00000000, 0x00000000, 0x00000000, 0x000c0e0f, 0x00000000, 0x00000000, 0x00000000, 
    0x00101211, 0x00000000, 0x00000000, 0x00000000, 0x00101213, 0x00000000, 0x00000000, 0x00000000
};
/*0x16C0*/ static u32 _data_16C0[] = {
    0x01010101, 0x01010101, 0x01020202, 0x02020101, 0x00000000, 0x00000000, 0x00000000, 0x01010101, 
    0x01010000, 0x06000000
};
/*0x16E8*/ static u32 _data_16E8[] = {
    0x00010203, 0x04000506, 0x0708090a, 0x0b0d0d10, 0x11121314, 0x16171819, 0x1a0f1d20, 0x0f0f1c1c, 
    0x21000000
};
/*0x170C*/ static u32 _data_170C[] = {
    0x3f70a3d7, 0x3f28f5c3, 0x3efc6a7f, 0x3f116873, 0x3f049ba6, 0x3f000000, 0x3eed0e56, 0x3f1e353f, 
    0x3f049ba6, 0x3ee66666, 0x3f2353f8, 0x3f578d50, 0x3f19999a, 0x3ed374bc, 0x3f666666, 0x3f5cac08, 
    0x3f049ba6, 0x3f15c28f, 0x3e91eb85, 0x3eee147b, 0x3ec00000, 0x3f400000, 0x3f0a3d71, 0x3f1d70a4, 
    0x3e19999a, 0x3eea7efa, 0x3ecb4396, 0x3f400000, 0x3f6fdf3b, 0x3f71eb85, 0x3f400000, 0x3f400000, 
    0x3f5cac08, 0x3f726e98, 0x3f400000
};
/*0x1798*/ static u32 _data_1798[] = {
    0x3f400000, 0x3f800000, 0x3f45a1cb, 0x3f778d50, 0x3f778d50, 0x3f90a3d7, 0x3f3645a2, 0x3f84bc6a, 
    0x3f41cac1, 0x3ee66666, 0x3e3645a2, 0x3eed0e56, 0x3f19999a, 0x3ed374bc, 0x3f666666, 0x3f5cac08, 
    0x3f049ba6, 0x3f15c28f, 0x3f0e147b, 0x3eee147b, 0x3ec00000, 0x3f400000, 0x3f0a3d71, 0x3f1d70a4, 
    0x3f028f5c, 0x3eea7efa, 0x3ecb4396, 0x3f400000, 0x3ef9db23, 0x3f71eb85, 0x3f400000, 0x3f400000, 
    0x3f5cac08, 0x3f0ed917, 0x3f400000
};
/*0x1824*/ static u32 _data_1824 = 0x00000000;
/*0x1828*/ static u32 _data_1828 = 0x00000000;
/*0x182C*/ static u32 _data_182C[] = {
    0x00000000, 0x00000258
};
/*0x1834*/ static f32 _data_1834 = 0.0;
/*0x1838*/ static f32 _data_1838 = 10.0;
/*0x183C*/ static u32 _data_183C = 0x00000000;
/*0x1840*/ static u32 _data_1840 = 0x00000000;
/*0x1844*/ static u32 _data_1844 = 0x00000000;
/*0x1848*/ static u32 _data_1848 = 0x00000000;
/*0x184C*/ static u32 _data_184C[] = {
    0xbf800000, 0x00000000, 0x00000000
};
/*0x1858*/ static u32 _data_1858[] = {
    0x00000000, 0x00000000, 0x3f800000
};
/*0x1864*/ static s32 _data_1864 = 0;

/*0x0*/ static u8 _bss_0[0x1c];
/*0x1C*/ static u8 _bss_1C[0x4];

// offset: 0x0 | ctor
void dll_32_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/32/dll_32_ctor.s")

// offset: 0x218 | dtor
void dll_32_dtor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/32/dll_32_dtor.s")

// offset: 0x2C8 | func: 0 | export: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/32/dll_32_func_2C8.s")

// offset: 0x2D0 | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/32/dll_32_func_2D0.s")

// offset: 0x2DC | func: 2 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/32/dll_32_func_2DC.s")

// offset: 0x4B0 | func: 3 | export: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/32/dll_32_func_4B0.s")

// offset: 0x92C | func: 4 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/32/dll_32_func_92C.s")

// offset: 0x1314 | func: 5 | export: 5
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/32/dll_32_func_1314.s")
#else
/*static*/ s32 dll_32_func_1D3C(f32* a0, f32* a1, f32* a2);

// https://decomp.me/scratch/ocR7s
void dll_32_func_1314(Gfx** gdl, Mtx** mtxs, Object* a2) {
    s32 temp_t0;
    s32 temp_v0_3;
    SRT spD8;
    s32 var_s2;
    s32 var_v1;
    f32 normalized;
    f32 var_fs0;
    ModelFacebatch* temp_s0;
    ModelFacebatch* pad2;
    s32 pad;
    ModelInstance* temp_s3;
    f32 spB4;
    Vec3f spA8 = { -1.0f, 0.0f, 0.0f }; // _data_184C
    Vec3f sp9C = { 0.0f, 0.0f, 1.0f }; // _data_1858
    Camera* camera;
    Vtx* temp_a3;
    s32 sp90;
    s32 sp8C;
    Object* player;
    s16 temp_a2;
    MtxF *temp;

    sp90 = 0x15;\
    sp8C = 8;
    player = get_player();
    if (a2->modelInstIdx != 0) {
        return;
    }
    temp_s3 = a2->modelInsts[a2->modelInstIdx];
    spD8.yaw = 0;
    spD8.roll = 0;
    spD8.pitch = 0;
    spD8.scale = 1.0f;
    camera = get_camera();
    if (camera != NULL) {
        spA8.x = camera->srt.transl.x - a2->srt.transl.x;
        spA8.y = camera->srt.transl.y - a2->srt.transl.y;
        spA8.z = camera->srt.transl.z - a2->srt.transl.z;
        var_fs0 = SQ(spA8.f[0]) + SQ(spA8.f[1]) + SQ(spA8.f[2]);
        if (var_fs0 != 0) {
            var_fs0 = sqrtf(var_fs0);
        }
        spD8.yaw = 0;
        spA8.x /= var_fs0;
        spA8.y /= var_fs0;
        spA8.z /= var_fs0;
        spD8.yaw = a2->srt.yaw;
        spD8.transl.x = 0.0f;
        spD8.transl.y = 0.0f;
        spD8.transl.z = 0.0f;
        rotate_vec3(&spD8, sp9C.f);
        spD8.yaw = 0;
    }
    temp_v0_3 = dll_32_func_1D3C(spA8.f, sp9C.f, &spB4);
    if (temp_v0_3 != -1) {
        return;
    }

    if (((DLL_210_Player*)player->dll)->vtbl->func44(player) == 0) {
        if (temp_v0_3 == -1) {
            for (var_s2 = 0x13; var_s2 < sp90; var_s2++) {
                if ((a2->id != 0) || (var_s2 != 0x2A)) {
                    temp_s0 = &temp_s3->model->faces[var_s2];
                    temp_a2 = temp_s0->baseVertexID;
                    temp_a3 = &temp_s3->vertices[0][temp_a2];
                    temp_t0 = temp_s3->model->faces[var_s2 + 1].baseVertexID - temp_a2;
                    spD8.transl.x = 0.0f;
                    spD8.transl.y = -26.0f;
                    spD8.transl.z = 0.0f;

                    for (var_v1 = 0; var_v1 < temp_t0; var_v1++) {
                        spD8.transl.x += temp_a3[var_v1].n.ob[0];
                        spD8.transl.y += temp_a3[var_v1].n.ob[1];
                        spD8.transl.z += temp_a3[var_v1].n.ob[2];
                    }

                    spD8.scale = 1.0f;
                    spD8.transl.x /= temp_t0;
                    spD8.transl.y /= temp_t0;
                    spD8.transl.z /= temp_t0;
                    pad = temp_s0->jointID_A;
                    temp = (MtxF *) ((f32**)(temp_s3->matrices)[(temp_s3->unk34 & 1)] + (temp_s0->jointID_A << 4));
                    vec3_transform(temp,
                                   spD8.transl.x, spD8.transl.y, spD8.transl.z, 
                                   &spD8.transl.x, &spD8.transl.y, &spD8.transl.z);
                    spD8.transl.x += gWorldX;
                    spD8.transl.z += gWorldZ;
                    normalized = SQ(a2->speed.x) + SQ(a2->speed.y) + SQ(a2->speed.z);
                    if (normalized < 0.1f) {
                        gDLL_17_partfx->vtbl->spawn(a2, 0x19A, &spD8, 0x200000, -1, NULL);
                    } else {
                        gDLL_17_partfx->vtbl->spawn(a2, 0x19B, &spD8, 0x200000, -1, NULL);
                    }
                    gDLL_17_partfx->vtbl->spawn(a2, 0x199, &spD8, 0x200000, -1, NULL);
                }
            }
        }
    }
    for (var_s2 = sp8C; var_s2 < (sp8C + 1); var_s2++) {
        temp_s0 = &temp_s3->model->faces[var_s2];
        temp_a2 = temp_s0->baseVertexID;
        temp_a3 = &temp_s3->vertices[0][temp_a2];
        _data_1864 = 4;
        spD8.transl.x = 0.0f;
        spD8.transl.y = 10.0f;
        spD8.transl.z = -40.0f;
        spD8.transl.x = temp_a3[4].n.ob[0];
        spD8.transl.y = temp_a3[4].n.ob[1] + 2.0f;
        spD8.transl.z = temp_a3[4].n.ob[2] + 10.0f;
        spD8.transl.x += rand_next(-5, 5);
        spD8.transl.y += rand_next(-5, 5);
        spD8.transl.z += rand_next(-5, 5);
        spD8.scale = 1.0f;
        pad = temp_s0->jointID_A;
        temp = (MtxF *) &(((f32 **)temp_s3->matrices)[(temp_s3->unk34 & 1)][temp_s0->jointID_A << 4]);
        vec3_transform(temp, 
                       spD8.transl.x, spD8.transl.y, spD8.transl.z, 
                       &spD8.transl.x, &spD8.transl.y, &spD8.transl.z);
        spD8.transl.x += gWorldX;
        spD8.transl.z += gWorldZ;
        spD8.transl.x += (2.0f * a2->speed.x);
        spD8.transl.y += (2.0f * a2->speed.y);
        spD8.transl.z += (2.0f * a2->speed.z);
        normalized = SQ(a2->speed.x) + SQ(a2->speed.y) + SQ(a2->speed.z);
        if (normalized < 0.01f) {
            gDLL_17_partfx->vtbl->spawn(a2, 0x19D, &spD8, 0x200000, -1, NULL);
        }
        gDLL_17_partfx->vtbl->spawn(a2, 0x19C, &spD8, 0x200000, -1, NULL);
    }
}
#endif

// offset: 0x1D3C | func: 6
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/32/dll_32_func_1D3C.s")
#else
// https://decomp.me/scratch/os9k5
s32 dll_32_func_1D3C(f32* a0, f32* a1, f32* a2) {
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    s32 var_v1;

    var_v1 = 1;
    sp44 = (a0[0] * a1[0]) + (a0[1] * a1[1]) + (a0[2] * a1[2]);
    if (sp44 < 0.0f) {
        var_v1 = 1;
        sp3C = sqrtf((SQ(a1[0]) + SQ(a1[1]) + SQ(a1[2])) * (SQ(a0[0]) + SQ(a0[1]) + SQ(a0[2])));
        sp40 = sp44 / sp3C;
        *a2 = sp40;
        if (sp40 < -0.2f) {
            var_v1 = -1;
        }
    }
    return var_v1;
}
#endif

/*0x0*/ static const char str_0[] = "static float scal[]={";
/*0x18*/ static const char str_18[] = "%fF,\n";
/*0x20*/ static const char str_20[] = "};";
/*0x24*/ static const char str_24[] = "static float zscal[]={";
/*0x3C*/ static const char str_3C[] = "%fF,\n";
/*0x44*/ static const char str_44[] = "};";
