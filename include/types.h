#ifndef _TYPES_H_
#define _TYPES_H_

#include "PR/ultratypes.h"
#include "sys/math.h"
#include "dll_def.h"

typedef union {
    void (*asVoid)(void);
    void (*withOneArg)(s32);
    void (*withTwoArgs)(s32, s32);
    void (*withThreeArgs)(s32, s32, s32);
    void (*withFourArgs)(s32, s32, s32, s32);
    void (*withFiveArgs)(s32, s32, s32, s32, s32);
    void (*withSixArgs)(s32, s32, s32, s32, s32, s32);
    void (*withSevenArgs)(s32, s32, s32, s32, s32, s32, s32);
    u16 (*asVoidU16)(void);
    u16 (*withOneArgU16)(s32);
    u16 (*withTwoArgsU16)(s32, s32);
    u16 (*withThreeArgsU16)(s32, s32, s32);
    u16 (*withFourArgsU16)(s32, s32, s32, s32);
    u16 (*withFiveArgsU16)(s32, s32, s32, s32, s32);
    u16 (*withSixArgsU16)(s32, s32, s32, s32, s32, s32);
    u16 (*withSevenArgsU16)(s32, s32, s32, s32, s32, s32, s32);
    u32 (*asVoidS32)(void);
    s32 (*withOneArgS32)(s32);
    s32 (*withOneVoidArgS32)(void*);
    s32 (*withTwoArgsS32)(s32, s32);
    s32 (*withThreeArgsS32)(s32, s32, s32);
    s32 (*withFourArgsS32)(s32, s32, s32, s32);
    s32 (*withFiveArgsS32)(s32, s32, s32, s32, s32);
    s32 (*withSixArgsS32)(s32, s32, s32, s32, s32, s32);
    s32 (*withSevenArgsS32)(s32, s32, s32, s32, s32, s32, s32);
    f32 (*asVoidF32)(void);
    f32 (*withOneArgF32)(s32);
    f32 (*withOneVoidArgF32)(void*);
    f32 (*withTwoArgsF32)(s32, s32);
    f32 (*withTwoArgsF32Custom)(void*, f32);
    f32 (*withThreeArgsF32)(s32, s32, s32);
    f32 (*withFourArgsF32)(s32, s32, s32, s32);
    f32 (*withFiveArgsF32)(s32, s32, s32, s32, s32);
    f32 (*withSixArgsF32)(s32, s32, s32, s32, s32, s32);
    f32 (*withSevenArgsF32)(s32, s32, s32, s32, s32, s32, s32);
    void (*withThreeArgsCustom)(void *, void *, f32);
    void (*withThreeArgsCustom2)(void *, f32, f32);
    void (*withOneS32OneF32)(s32, f32);
    void (*withTwoArgsCustom)(void *, s32);
    void (*withTwoArgsCustom2)(void *, f32);
    void (*withFiveArgsCustom)(void *, f32, f32 *, f32 *, f32 *);
    void (*withFiveArgsCustom2)(void *, void *, f32, s32, f32);
    s32 (*withFiveArgsCustom3)(void *, void *, s8, f32 *, f32 *);
    void (*withFiveArgsCustom4)(void *, void *, f32, f32, f32);
    void (*withSixArgsCustom)(void *, s32, void *, s32, s32, s32);
    s32 (*withSevenArgsCustom)(void *, f32, f32, f32, void*, void*, void*);
    void (*objtype57Func7)(void *, void *, s32, void *, void *, void *, void *);
} UnknownDLLFunc;

// "Unknown" DLL interface to help match code calling DLLs where the exact
// DLL being called has not been determined
DLL_INTERFACE(DLL_Unknown) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ UnknownDLLFunc func[1]; // length varies
};

//used for PlayerPosBuffer and something else
struct Vec3_Int{
    Vec3f f;
    u32 i; //seems to be a 32-bit bool. (for player pos buffer it's a frame count)
};

#endif
