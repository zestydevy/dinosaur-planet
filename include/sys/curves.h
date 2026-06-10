#ifndef _SYS_CURVES_H__
#define _SYS_CURVES_H__

#include "game/objects/object.h"
#include "sys/math.h"

typedef f32 (*SplineFunc)(f32* spline, f32 t, f32* rate);
typedef void (*SplineConverterFunc)(f32* in, f32* out);

// Note: This is a curve-specific version of the base ObjSetup struct, with additions
typedef struct {
/*00*/ s16 objId;
/*02*/ u8 quarterSize;
/*03*/ u8 unk3;
/*04*/ union {
	struct {
	/*04*/ s8 unk4;
	/*05*/ s8 unk5;
	/*06*/ s8 unk6;
	/*07*/ s8 unk7;
	} base_type26;
    struct {
	/*04*/ u8 unk4;
	/*05*/ u8 unk5;
	/*06*/ u8 unk6;
	/*07*/ u8 unk7;
	} base_type24;
	struct {
	/*04*/ u16 unk4;
	/*06*/ s8 unk6;
	/*07*/ s8 unk7;
	} base_type22;
};
/*08*/ Vec3f pos;
/*14*/ u32 uID;
/*18*/ s8 unk18;
/*19*/ s8 curveType; // "CURVETYPE in default.dol"  2) KTrex, 3) RedEye, 1A) camera?, 1B) camera?, 1D) ThornTail, 1F) crawlSpace, 22) Kyte, 24) Tricky
/*1A*/ s8 unk1A;
/*1B*/ s8 unk1B;
/*1C*/ s32 links[4]; // "links" in default.dol
// TODO: struct split here?
/*2C*/ s8 unk2C; //yaw?
/*2D*/ s8 unk2D;
/*2E*/ u8 unk2E; 
/*2E*/ s8 unk2F; 
// TODO: some of this depends on the curve type
/*30*/ union {
    struct {
        /*30*/ s32 pad30;
        /*34*/ s16 unk34;
        /*36*/ s16 unk36;
        /*38*/ s16 unk38;
    } type8;
    struct {
        s32 pad30;
        s16 unk34;
    } type15;
    struct {
        u8 _unk30[0x34 - 0x30];
        u8 unk34;
    } type1D;
    struct {
        /*30*/ s16 unk30; // gameBit
        /*32*/ s16 usedBit; // gameBit
    } type22; // kyte?
    struct {
        s16 unk30; // gameBit
        s16 unk32; // gameBit
    } type24; // tricky?
    struct {
        /*30*/ s8 unk30; 
        /*31*/ s8 unk31;
        /*32*/ s8 unk32;
        /*33*/ s8 unk33;
        /*34*/ s8 unk34[4][4];
    } type26;
    struct {
        /*30*/ u8 unk30; 
        /*31*/ u8 unk31;
        /*32*/ u8 unk32;
        /*33*/ u8 unk33;
        /*34*/ u8 unk34;
    } unk;
};
} CurveSetup;

// NOTE: If the size of this struct changes, SnowHorn_Data must also be adjusted!
typedef struct {
    /* 0000 */ f32 unk0;
    /* 0004 */ f32 unk4; // curveProgress? (lerp t-value from 0 to 100?)
    /* 0008 */ f32 unk8;
    /* 000C */ f32 unkC;
    /* 0010 */ s32 unk10;
    /* 0014 */ f32 unk14[20];
    /* 0064 */ f32 unk64;
    /* 0068 */ Vec3f unk68; //lerp_result_x?
    /* 0074 */ f32 unk74; //some x component
    /* 0078 */ f32 unk78; //some y component
    /* 007C */ f32 unk7C; //some z component
    /* 0080 */ s32 unk80;
    /* 0084 */ f32* unk84;
    /* 0088 */ f32* unk88;
    /* 008C */ f32* unk8C;
    /* 0090 */ s32 numControlPoints;
    /* 0094 */ SplineFunc splineFunc;
    /* 0098 */ SplineConverterFunc splineConverterFunc;
    CurveSetup *unk9C;
    CurveSetup *unkA0;
    CurveSetup *unkA4;
    f32 unkA8[4];
    f32 unkB8[4];
    f32 unkC8[4];
    f32 unkD8[4];
    f32 unkE8[4];
    f32 unkF8[4];
} UnkCurvesStruct;

typedef struct Unk80006784 {
    f32 unk0[6];
} Unk80006784;

typedef struct Unk80006908 {
    Vec4f unk0;
    Vec4f unk10;
    f32 unk20;
} Unk80006908;

f32 curves_b_spline(f32* spline, f32 t, f32* rate);
void curves_b_spline_converter(f32* in, f32* out);
f32 curves_hermite(f32* spline, f32 t, f32* rate);
void curves_hermite_converter(f32* in, f32* out);
f32 curves_bezier(f32* spline, f32 t, f32* rate);
void curves_bezier_converter(f32* in, f32* out);
f32 curves_catmull_rom(f32* s, f32 t, f32* rate);
void curves_catmull_rom_converter(f32* in, f32* out);
f32 curves_linear(f32* s, f32 t, f32* rate);
void curves_linear_converter(f32* in, f32* out);

void curves_move(UnkCurvesStruct *arg0);
void curves_setup_move_network_curve(UnkCurvesStruct *arg0);
f32 curves_func_80006E04(Unk80006784 *, f32);
f32 curves_func_80006CFC(Unk80006784 *, f32, f32);
s32 curves_func_800053B0(UnkCurvesStruct* arg0, f32 arg1);
void curves_func_8000598C(f32* arg0, f32* arg1, f32* arg2, f32* arg3, f32* arg4, f32* arg5, s32 arg6, SplineConverterFunc arg7);

#endif
