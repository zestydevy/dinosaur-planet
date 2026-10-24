#ifndef _SYS_GAME_OBJECTS_SETUPS_DFBARRELCREATOR_H
#define _SYS_GAME_OBJECTS_SETUPS_DFBARRELCREATOR_H

#include "game/objects/object.h"

// ctrl: 104 | size: 0x1c
typedef struct {
    ObjSetup base;
    u8 searchDistance;  //Creates a barrel if none are found inside this radius (stored divided by 4)
    s16 gamebitStop;    //Stops creating barrels if this gamebit is set
} DFBarrelCreator_Setup;

#endif
