#ifndef _SYS_GAME_OBJECTS_SETUPS_COLLECTABLE_H
#define _SYS_GAME_OBJECTS_SETUPS_COLLECTABLE_H

#include "game/objects/object.h"

// ctrl: 6 | size: 0x30
typedef struct {
/*00*/    ObjSetup base;
/*18*/    s8 unused18;
/*19*/    s8 messageArgBase;      //Base arg value used for sidekick-related collectables
/*1A*/    u8 scale;        //[DLL 272 "collectable": stored to Collectable_Data but unused]
                                    //[DLL 523 "SCcollectables": value for objHitInfo->unk52]
/*1B*/    u8 yaw8;
/*1C*/    s16 gamebitCollected;   //Set when collected: collectable vanishes when set
/*1E*/    s16 animMessage;
/*20*/    s8 unused20;
/*21*/    s8 unused21;    
/*22*/    u8 pitch8;
/*23*/    u8 roll8;
/*24*/    s16 gamebitSecondary;   //(Optional) [DLL 272 "collectable": show when set]
                                    //           [DLL 523 "SCcollectables": fall from tree]
/*26*/    u8 modelIdx;
/*27*/    u8 applyColourMultiplier;
/*28*/    u8 multiplyR;           //Colour multiplier for model
/*29*/    u8 multiplyG;           //Colour multiplier for model
/*2A*/    u8 multiplyB;           //Colour multiplier for model
/*2B*/    u8 unused2B;
/*2C*/    s16 gamebitCount;
/*2E*/    s16 mode;
} Collectable_Setup;

#endif
