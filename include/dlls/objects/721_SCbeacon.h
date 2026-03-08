#ifndef _DLLS_721_H
#define _DLLS_721_H

#include "PR/ultratypes.h"
#include "dll_def.h"
#include "types.h"
#include "game/objects/object.h"
#include "sys/curves.h"

typedef struct {
    ObjSetup base;
    u8 playerRange;
    u8 unused19;
    u16 kyteCurveID;
    s8 unused1C;
    s8 unused1D;
    s8 unused1E;
    u8 yaw;
} SCbeacon_Setup;

typedef struct {
    u8 state;
    u8 flags;                   //Tracks twigs being added to bowl, and LightAction use (when lit)
    u8 prevFlags;               //Value of flags on previous flame, for comparison
    s32 seqIdxPlaceTwigs;       //When carrying Tumbleweed near beacon (@bug: doesn't play?)
    s32 seqIdxLightTwigs;       //When Flame used
    CurveSetup* kyteCurve;      
    s16 gamebitTwigs;           //Set when twigs placed in bowl
    s16 gamebitLit;             //Set when beacon lit
    Object* unk14;              //Set to 0 in setup, but otherwise unused
} SCbeacon_Data;

typedef enum {
    SCbeacon_STATE_Initial = 0,         //Setup phase, restores state via gamebits
    SCbeacon_STATE_Bowl_Empty = 1,      //Waiting for Krystal to carry over a Tumbleweed
    SCbeacon_STATE_Twigs_in_Bowl = 2,   //Waiting for Kyte to use Flame
    SCbeacon_STATE_Lighting = 3,        //Kyte currently using Flame
    SCbeacon_STATE_Lit = 4              //Beacon lit
} SCbeacon_States;

typedef enum {
    SCbeacon_FLAG_0 = 0,
    SCbeacon_FLAG_Add_Tumbleweed = 1,
    SCbeacon_FLAG_Emit_Light = 2
} SCbeacon_Flags;

typedef enum {
    SCbeacon_MODEL_Bowl_Empty = 0,
    SCbeacon_MODEL_Twigs_in_Bowl = 1
} SCbeacon_ModelIndices;

typedef enum {
    SCbeacon_SEQIDX_Placing_Twigs = 0, //doesn't play?
    SCbeacon_SEQIDX_Lighting_Twigs = 1
} SCbeacon_SeqIndices;

typedef enum {
    SCbeacon_Near_Golden_Plains = 0x28CE,
    SCbeacon_Near_Pond_with_Pole = 0x28CF,
    SCbeacon_Near_Discovery_Falls = 0x28D0
} SCbeacon_UIDs;

DLL_INTERFACE(DLL_721_SCbeacon) {
    /*:*/ DLL_INTERFACE_BASE(DLL_IObject);

    /*7*/ int (*handle_kyte_flame_seqs)(Object* self, s32 finishLighting);     /*
    * Returns TRUE when the beacon is in its "lighting" state (emitting embers/smoke while Kyte is using Flame on it).
    * 
    * `finishLighting` == 0: advances beacon to "lighting" state, plays seq where Kyte begins lighting the beacon
    *
    * `finishLighting` == 1: advances beacon to "lit" state, and (if all beacons are lit) plays the DF entrance pool seq 
    */
    
    /*8*/ s32 (*func8)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2); //returns 0 (TO-DO: does anything call this?)
    /*9*/ s32 (*func9)(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2); //returns 0 (TO-DO: does anything call this?)
    /*10*/ s32 (*func10)(Object* self); //returns 2, called by Kyte's DLL when using Flame (TO-DO: what is this for?)
};

#endif // _DLLS_721_H

