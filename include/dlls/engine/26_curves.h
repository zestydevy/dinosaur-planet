#ifndef _DLLS_26_H
#define _DLLS_26_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
/*060*/ s32 unk00;
/*064*/ f32 curveProgress; //(lerp t-value from 0 to 100?)
/*068*/ s32 unk08;
/*06C*/ s32 unk0C;
/*070*/ s32 unk10;
/*074*/ s32 unk14;
/*078*/ s32 unk18;
/*07C*/ s32 unk1C;
/*080*/ s32 unk20;
/*084*/ s32 unk24;
/*088*/ s32 unk28;
/*08C*/ s32 unk2C;
/*090*/ s32 unk30;
/*094*/ s32 unk34;
/*098*/ s32 unk38;
/*09C*/ s32 unk3C;
/*0A0*/ s32 unk40;
/*0A4*/ s32 unk44;
/*0A8*/ s32 unk48;
/*0AC*/ s32 unk4C;
/*0B0*/ s32 unk50;
/*0B4*/ s32 unk54;
/*0B8*/ s32 unk58;
/*0BC*/ s32 unk5C;
/*0C0*/ s32 unk60;
/*0C4*/ s32 unk64;
/*0C8*/ f32 unk68; //lerp_result_x?
/*0CC*/ f32 unk6C; //lerp_result_y?
/*0D0*/ f32 unk70; //lerp_result_z?
/*0D4*/ f32 unk74; //some x component
/*0D8*/ f32 unk78; //some y component
/*0DC*/ f32 unk7C; //some z component
} UnkCurvesStruct60;

DLL_INTERFACE_BEGIN(26_curves)
    /*0*/ UnknownDLLFunc curves_func_18;
    /*1*/ UnknownDLLFunc curves_func_34;
    /*2*/ UnknownDLLFunc curves_func_10c;
    /*3*/ UnknownDLLFunc curves_func_1bc;
    /*4*/ UnknownDLLFunc curves_func_1e4;
    /*5*/ UnknownDLLFunc curves_func_3f00;
    /*6*/ UnknownDLLFunc curves_func_39c;
    /*7*/ UnknownDLLFunc curves_func_14f4;
    /*8*/ UnknownDLLFunc curves_func_174c;
    /*9*/ UnknownDLLFunc curves_func_17f4;
    /*10*/ UnknownDLLFunc curves_func_14c8;
    /*11*/ UnknownDLLFunc curves_func_1880;
    /*12*/ UnknownDLLFunc curves_func_1924;
    /*13*/ UnknownDLLFunc curves_func_19c8;
    /*14*/ UnknownDLLFunc curves_func_1b90;
    /*15*/ UnknownDLLFunc curves_func_218c;
    /*16*/ UnknownDLLFunc curves_func_21fc;
    /*17*/ UnknownDLLFunc curves_func_277c;
    /*18*/ UnknownDLLFunc curves_func_291c;
    /*19*/ UnknownDLLFunc curves_func_35ac;
    /*20*/ UnknownDLLFunc curves_func_438;
    /*21*/ UnknownDLLFunc curves_func_4f0;
    /*22*/ UnknownDLLFunc curves_func_854;
    /*23*/ UnknownDLLFunc curves_func_590;
    /*24*/ UnknownDLLFunc curves_func_374c;
    /*25*/ UnknownDLLFunc curves_func_3930;
    /*26*/ UnknownDLLFunc curves_func_a34;
    /*27*/ UnknownDLLFunc curves_func_b8c;
    /*28*/ UnknownDLLFunc curves_func_d8c;
    /*29*/ UnknownDLLFunc curves_func_e40;
    /*30*/ UnknownDLLFunc curves_func_2c20;
    /*31*/ UnknownDLLFunc curves_func_3018;
    /*32*/ UnknownDLLFunc curves_func_31e0;
    /*33*/ UnknownDLLFunc curves_func_32d8;
    /*34*/ s32 (*curves_func_4288)(UnkCurvesStruct60*, Object*, f32, s32*, s16);
    /*35*/ s32 (*curves_func_4704)(UnkCurvesStruct60*);
    /*36*/ UnknownDLLFunc curves_func_4cb4;
    /*37*/ UnknownDLLFunc curves_func_4fa8;
    /*38*/ UnknownDLLFunc curves_func_50e4;
    /*39*/ UnknownDLLFunc curves_func_5698;
    /*40*/ UnknownDLLFunc curves_func_577c;
    /*41*/ UnknownDLLFunc curves_func_5860;
DLL_INTERFACE_END()

#endif //_DLLS_26_H
