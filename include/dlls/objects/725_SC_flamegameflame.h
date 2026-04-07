#ifndef _DLLS_725_H
#define _DLLS_725_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    u8 flameRange;          //Distance at which Kyte should use Flame command     
    s8 unk19;
    u16 kyteFlightCurveID;  //The curve path Kyte should follow when Flame is used
    s8 unk1C;
    s8 unk1D;
    s8 unk1E;
    u8 yaw;
} SCFlameGameFlame_Setup;

typedef struct {
    u8 unused0;
    s32 unk4;               //Set to 0 in setup, but otherwise unused. Maybe meant to store index of target in dKyteFlightCurveIDs etc?
    u8 flags;
    s16 gamebitActive;      //Gamebit must be set to enable the Flame target
    s16 gamebitFinished;    //Gamebit to set when Kyte has finished using Flame
} SCFlameGameFlame_Data;

typedef enum {
    SCFlameGameFlame_FLAG_None = 0,
    SCFlameGameFlame_FLAG_Kyte_is_Nearby = 1,           //Kyte is in range and should use Flame
    SCFlameGameFlame_FLAG_Target_in_View = 2,           //Player is looking towards target (print function used to determine this)              
    SCFlameGameFlame_FLAG_Player_Gave_Command = 4,      //Player used Flame in the inventory
    SCFlameGameFlame_FLAG_Finished = 8                  //Flame finished
} SCFlameGameFlame_Flags;

DLL_INTERFACE(DLL_725_FlameGameFlame) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);
    /*7*/ int (*func7)(Object* self, s32 arg1);
    /*8*/ s32 (*func8)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2);
    /*9*/ s32 (*func9)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2);
    /*10*/ s32 (*func10)(Object* self);
};

#endif // _DLLS_725_H

