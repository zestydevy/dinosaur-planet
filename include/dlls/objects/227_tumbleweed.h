#ifndef _DLLS_227_H
#define _DLLS_227_H

#include "PR/ultratypes.h"
#include "dlls/engine/27.h"
#include "game/objects/object.h"
#include "dll_def.h"
#include "types.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s32 unk18;
/*1c*/ s32 unk1C;
/*20*/ s32 unk20;
/*24*/ s16 gamebitDropped;
} GoldenNugget_Setup;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 roll;
/*19*/ s8 pitch;
/*1A*/ s8 yaw;
/*1B*/ u8 carryingGold;     //Bitfield flags: carrying a Shiny Nugget when bit0 is set
/*1C*/ f32 interactRadius;  //Distance at which player can interact with tumbleweed
} Tumbleweed_Setup;

typedef struct {
/*000*/ DLL27_Data collision;       //Terrain/HITS data
/*260*/ u16 characterDistance;      //Distance to character Tumbleweed's fleeing from (player/sidekick)
/*262*/ u16 interactRadius;         //Distance at which player can interact with tumbleweed
/*264*/ f32 baseScale;              //100% scale for Tumbleweed
/*268*/ f32 timer;                  //Self-deletion timer, also used for scale-up speed when growing
/*26C*/ f32 twigSqueakTimer;        //Random interval between twigs' squeaks
/*270*/ u8 state;                   //State machine phase
/*271*/ u8 carryingGold;            //Bitfield flags: carrying a Shiny Nugget when bit0 is set
/*272*/ u8 flags;                   //Flags for creating leaf/dust particles, or starting deletion timer
/*273*/ s8 pad273;
/*274*/ s16 rollSpeed;              //Rotation speed
/*276*/ s16 pitchSpeed;             //Rotation speed
/*278*/ s16 yawSpeed;               //Rotation speed
/*27A*/ s16 pad27A;
/*27C*/ Object* goldenNugget;       //Reference to Shiny Nugget
/*280*/ Object* player;             //Reference to player
/*284*/ s16 goldDroppedGamebit;     //GamebitID to set when Shiny Nugget dropped
/*286*/ s16 pad286;
/*288*/ f32 homeX;                  //World coord that fleeing Tumbleweeds return to
/*28C*/ f32 homeZ;                  //World coord that fleeing Tumbleweeds return to
/*290*/ Vec3f* gravitateTarget;     //World coord for Tumbleweed to gravitate towards
/*294*/ s16 carryMessageArgLo;      //Lower halfword of player message argument when Tumbleweed carried
/*296*/ s16 carryMessageArgHi;      //Upper halfword of player message argument when Tumbleweed carried
/*298*/ u8 carryFlags;              //Flags for Tumbleweed twigs when being carried (lifted/dropped/etc.)
/*299*/ u8 beingCarried;            //Boolean for Tumbleweed twigs
} Tumbleweed_Data;

typedef enum {
    Tumbleweed_STATE_Growing_on_Tree = 0,
    Tumbleweed_STATE_Fall_from_Tree = 1,
    Tumbleweed_STATE_Rolling = 2,
    Tumbleweed_STATE_Twigs = 3,
    Tumbleweed_STATE_Expire = 4,
    Tumbleweed_STATE_Gravitate = 5,
    Tumbleweed_STATE_Eaten = 6
} Tumbleweed_States;

typedef enum {
    Tumbleweed_FLAG_None = 0,
    Tumbleweed_FLAG_Create_Leaves = 1,
    Tumbleweed_FLAG_Create_Dust = 2,
    Tumbleweed_FLAG_Expire = 4
} Tumbleweed_Flags;

typedef enum {
    Twig_FLAG_None = 0,
    Twig_FLAG_Lifted = 1,
    Twig_FLAG_Dropped = 2
} Twig_Carry_Flags;

DLL_INTERFACE(DLL_227_Tumbleweed) {
/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
/*07*/ u8 (*get_state)(Object* self);
/*08*/ void (*set_home)(Object* self, f32 x, f32 z);
/*09*/ void (*fall)(Object* self);
/*10*/ void (*gravitate_towards_point)(Object* self, Vec3f* point);
/*11*/ s32 (*is_gravitating)(Object* self);
/*12*/ void (*store_player)(Object* self, Object* player);
};

#endif //_DLLS_227_H
