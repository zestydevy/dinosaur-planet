#ifndef _DLL_33_H
#define _DLL_33_H

#include "dlls/engine/18_objfsa.h"
#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "segment_334F0.h"
#include "sys/curves.h"
#include "sys/voxmap.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
/*00*/ u8 _unk0[0x24 - 0x0];
/*24*/ s8 unk24;
/*25*/ u8 unk25;
/*26*/ s8 unk26;
/*27*/ s8 unk27;
} DLL33Data_34C;

// size:0x3FC
typedef struct {
/*000*/ ObjFSA_Data fsa;
/*34C*/ DLL33Data_34C unk34C;
/*374*/ SomeVoxmapAllocStruct unk374;
/*380*/ u8 _unk380[0x39C - 0x380];
/*39C*/ s16 unk39C;
/*39E*/ s16 unk39E;
/*3A0*/ s16 unk3A0;
/*3A2*/ s16 unk3A2;
/*3A4*/ s16 unk3A4;
/*3A6*/ s16 unk3A6;
/*3A8*/ u32 unk3A8;
/*3AC*/ u8 _unk3AC[0x3B0 - 0x3AC];
/*3B0*/ u8 unk3B0;
/*3B1*/ u8 _unk3B1;
/*3B2*/ u16 unk3B2;
/*3B4*/ s8 unk3B4;
/*3B5*/ u8 _unk3B5;
/*3B6*/ s16 unk3B6;
/*3B8*/ u8 unk3B8;
/*3B9*/ s8 unk3B9;
/*3BA*/ s8 _unk3BA;
/*3BB*/ s8 unk3BB;
/*3BC*/ Unk80032CF8 unk3BC;
/*3E0*/ s16 unk3E0;
/*3E2*/ u16 unk3E2;
/*3E4*/ f32 unk3E4;
/*3E8*/ f32 unk3E8;
/*3EC*/ f32 unk3EC;
/*3F0*/ u8 unk3F0;
/*3F1*/ u8 _unk3F1[0x3F4 - 0x3F1];
/*3F4*/ void *unk3F4; // pointer to data immediately following this struct
/*3F8*/ UnkCurvesStruct *unk3F8;
} DLL33_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 unk18;
/*1A*/ s16 unk1A;
/*1C*/ s16 unk1C;
/*1E*/ s16 unk1E;
/*20*/ s16 unk20;
/*22*/ s16 unk22;
/*24*/ u8 _unk24[0x27 - 0x24];
/*27*/ s8 unk27;
/*28*/ s8 unk28;
/*29*/ u8 unk29;
/*2A*/ s8 unk2A;
/*2B*/ u8 unk2B;
/*2C*/ s16 unk2C;
/*2E*/ s8 unk2E;
/*2F*/ u8 unk2F;
/*30*/ s16 unk30;
/*32*/ u8 unk32;
/*33*/ u8 pad33;
/*34*/ u16 unk34;
} DLL33_ObjSetup;

DLL_INTERFACE(DLL_33) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*ctor)(void *dll); // This DLL exports the ctor/dtor for some reason, don't call these
/*1*/ void (*dtor)(void *dll);
/*2*/ f32 (*func2)(Object* arg0, f32 arg1, f32 arg2, f32 arg3, Object* arg4);
/*3*/ void (*func3)(Object* arg0, DLL33_Data* arg1, u32 arg2, f32 arg3, f32 arg4);
/*4*/ void (*func4)(Object* arg0, Object* arg1, u8 arg2, s16* arg3, s16* arg4, s16* arg5);
/*5*/ s32 (*func5)(Object* arg0, DLL33_Data* arg1, f32 arg2) ;
/*6*/ s32 (*func6)(UNK_PTR *arg0, UNK_PTR *arg1);
/*7*/ u16 (*func7)(Object* arg0);
/*8*/ f32 (*func8)(Object* arg0);
/*9*/ void (*func9)(Object* arg0, ObjFSA_Data* arg1, DLL33Data_34C *arg2, s16 arg3, s8 *arg4, s16 arg5, s16 arg6, s32 arg7, s8 arg8);
/*10*/ void (*func10)(Object* arg0, DLL33_Data* arg1, f32 arg2, s8 arg3);
/*11*/ s32 (*func11)(Object* arg0, DLL33_Data* arg1, u8 arg2);
/*12*/ s32 (*func12)(Object* arg0, AnimObj_Data* arg1, DLL33_Data* arg2, ObjFSA_StateCallback *arg3, ObjFSA_StateCallback *arg4, s16 arg5);
/*13*/ s32 (*func13)(Object* arg0, DLL33_Data* arg1, ObjFSA_StateCallback *arg2, ObjFSA_StateCallback *arg3, s16 arg4, f32* arg5, f32* arg6, s32* arg7);
// TODO: arg count might be off
/*14*/ void (*func14)(Object* arg0, DLL33_Data* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
/*15*/ void (*func15)(Object* arg0, DLL33_Data* arg1, u8 arg2);
/*16*/ s32 (*func16)(Object* arg0, DLL33_Data* arg1, f32 arg2, s32 arg3);
/*17*/ Object *(*func17)(Object* arg0, DLL33_Data* arg1, f32 arg2, s32 arg3);
/*18*/ Object *(*func18)(Object* arg0, s32 arg1, s32 arg2, u8 arg3);
/*19*/ s32 (*func19)(Object* arg0, ObjFSA_Data* arg1, s32 arg2, s32 arg3, s32 *arg4, s8 *arg5, s16 arg6, u32* arg7, SRT* arg8);
/*20*/ s32 (*func20)(Object* arg0, ObjFSA_Data* arg1, DLL33Data_34C *arg2, s16 arg3, s8 *arg4, s16 arg5, s16 arg6, s16 arg7);
/*21*/ void (*func21)(Object* obj, DLL33_ObjSetup* setup, DLL33_Data* data, s32 arg3, s32 arg4, s32 arg5, u8 arg6, f32 arg7);
/*22*/ void (*func22)(Object* arg0, DLL33_Data* arg1);
/*23*/ f32 (*func23)(Object* arg0);
};

#endif // _DLL_33_H
