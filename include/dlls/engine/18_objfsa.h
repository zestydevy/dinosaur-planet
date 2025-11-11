#ifndef _DLL_18_H
#define _DLL_18_H

#include "dll_def.h"
#include "dlls/engine/27.h"
#include "game/objects/object.h"
#include "types.h"

typedef enum {
    OBJFSA_FLAG_8000 = 0x8000 // Set after first iteration?
} ObjFSAFlags;

typedef struct {
/*0*/ u8 _unk0[2];
/*2*/ u16 soundID;
} ObjFSA_Func_11BC_Struct;

struct ObjFSA_Data;

/**
 * Callback to run for a given objfsa state.
 * Returns:
 * - zero: Keep current state.
 * - positive number: Switch to new state synchronously (state = return_value - 1)
 * - negative number: Switch to new state asynchronously (state = -return_value)
 *
 * Synchronous transitions will run immediately after this callback, within the same tick.
 * Asynchronous transitions will run on the next tick.
 */
typedef s32 (*ObjFSA_StateCallback)(Object *obj, struct ObjFSA_Data *fsa, f32 updateRate);
typedef void (*ObjFSA_ExitCallback)(Object *obj, struct ObjFSA_Data *fsa);

/**
 * objfsa ("object finite-state automata") implements finite-state machines for object related stuff.
 */

// size:0x34C
typedef struct ObjFSA_Data {
/*000*/ u32 flags;
/*004*/ DLL27_Data unk4;
/*264*/ s16 unk264;
/*266*/ s16 unk266;
/*268*/ s16 logicState; // index of logic state callback to run
/*26A*/ s16 prevLogicState;
/*26C*/ s16 animState; // index of anim state callback to run
/*26E*/ s16 prevAnimState;
/*270*/ s16 unk270;
        // Whether the current anim state was just entered 
        // (only true for the first call since transitioning to the state)
/*272*/ s8 enteredAnimState;
        // Whether the current logic state was just entered 
        // (only true for the first call since transitioning to the state)
/*273*/ s8 enteredLogicState;
/*274*/ Vec3f *unk274;
/*278*/ f32 unk278; // rate of turning (yaw only)?
/*27C*/ f32 unk27C; // rate of turning (yaw only)?
/*280*/ f32 unk280;
/*284*/ f32 unk284;
/*288*/ f32 unk288;
/*28C*/ f32 speed;
/*290*/ f32 unk290;
/*294*/ f32 unk294;
/*298*/ f32 animTickDelta; // How much the object's animation progresses per fsa tick.
/*29C*/ f32 unk29C;
/*2A0*/ f32 unk2A0;
/*2A4*/ f32 unk2A4;
/*2A8*/ s32 unk2A8;
/*2AC*/ f32 unk2AC;
/*2B0*/ f32 unk2B0;
/*2B4*/ f32 unk2B4;
/*2B8*/ f32 targetDist; // distance to target
/*2BC*/ u8 _unk2BC[0x2C8 - 0x2BC];
/*2C8*/ Object *target;
/*2CC*/ u8 _unk2CC[0x2EC - 0x2CC];
/*2EC*/ Vec3f unk2EC;
/*2F8*/ f32 unk2F8;
        // If set, runs when leaving the current anim state.
        // Is reset to null after being ran.
/*2FC*/ ObjFSA_ExitCallback animExitAction;
/*300*/ u8 _unk300[0x304 - 0x300];
/*304*/ s32 unk304;
/*308*/ u32 unk308;
/*30C*/ s32 unk30C;
/*310*/ s32 unk310;
/*314*/ u8 _unk314[0x322 - 0x314];
/*322*/ s16 logicStateTime; // time spent in current logic state (caps at 10000)
/*324*/ s16 unk324;
/*326*/ s16 unk326;
/*328*/ s16 unk328; // yaw in degrees (absolute value) (sorta, more to this)
/*32A*/ s16 unk32A; // yaw in degrees (sorta, more to this)
/*32C*/ s16 animStateTime; // time spent in current anim state (caps at 10000)
/*32E*/ s16 unk32E;
/*330*/ s32 unk330;
/*334*/ s32 unk334;
/*338*/ s8 unk338;
/*339*/ s8 unk339;
/*33A*/ s8 unk33A; // whether the current animation is finished?
/*33B*/ u8 _unk33B[0x33D - 0x33B];
/*33D*/ u8 unk33D;
/*33E*/ u8 unk33E;
/*33F*/ u8 unk33F;
/*340*/ s8 unk340; // bit 0 set when turning?
/*341*/ s8 unk341;
/*342*/ u8 unk342;
/*343*/ s8 unk343;
/*344*/ u8 _unk344[0x348 - 0x344];
/*348*/ s8 unk348;
/*349*/ u8 _unk349;
/*34A*/ u8 unk34A;
/*34B*/ s8 unk34B;
} ObjFSA_Data;

DLL_INTERFACE(DLL_18_objfsa) {
/*:*/ DLL_INTERFACE_BASE(DLL);
/*0*/ void (*func0)(Object *obj, ObjFSA_Data *data, s32 arg2, s32 arg3);
/*1*/ void (*tick)(Object* obj, ObjFSA_Data *data, f32 fsaUpdateRate, f32 arg3, ObjFSA_StateCallback *animStateCallbacks, ObjFSA_StateCallback *logicStateCallbacks);
/*2*/ void (*func2)(Object *obj, ObjFSA_Data *data, ObjFSA_StateCallback *callbacks);
/*3*/ void (*func3)(SRT *srt);
/*4*/ void (*set_anim_state)(Object *obj, ObjFSA_Data *data, s32 state);
/*5*/ void (*func5)(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
/*6*/ void (*func6)(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
/*7*/ void (*func7)(Object *obj, ObjFSA_Data *data, f32 updateRate, s32 arg3);
/*8*/ void (*func8)(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3, f32 arg4);
/*9*/ void (*func9)(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3);
/*10*/ void (*func10)(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3);
/*11*/ void (*func11)(Object *obj, ObjFSA_Data *data, f32 arg2, s32 arg3);
/*12*/ void (*func12)(Object *obj, ObjFSA_Data *data, s32 arg2, s32 arg3, ObjFSA_Func_11BC_Struct *arg4);
/*13*/ void (*func13)(Object *obj, ObjFSA_Data *data, s32 arg2, s32 arg3, ObjFSA_Func_11BC_Struct *arg4, f32 arg5, u8 volume);
/*14*/ void (*func14)(Object *obj, ObjFSA_Data *data, s32 arg2);
/*15*/ void (*func15)(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3);
/*16*/ void (*func16)(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3);
/*17*/ void (*func17)(Object *obj, ObjFSA_Data *data);
/*18*/ void (*func18)(Object *obj, ObjFSA_Data *data, s32 arg2, s32 arg3);
/*19*/ void (*func19)(Object *obj, ObjFSA_Data *data, s32 arg2, s32 arg3);
/*20*/ void (*func20)(Object *obj, ObjFSA_Data *data, s32 arg2, s32 arg3, u32 arg5, u32 arg6);
/*21*/ void (*func21)(Object *obj, ObjFSA_Data *data, s32 partfxID, s32 arg3, u32 arg4);
/*22*/ void (*func22)(ObjFSA_Data *data, void *arg1);
};

#endif // _DLL_18_H
