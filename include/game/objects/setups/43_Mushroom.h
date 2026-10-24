#ifndef _SYS_GAME_OBJECTS_SETUPS_MUSHROOM_H
#define _SYS_GAME_OBJECTS_SETUPS_MUSHROOM_H

#include "game/objects/object.h"

// ctrl: 43 | size: 0x24
typedef struct {
	ObjSetup base;
	u8 index;               //Values 4/5 have curve-following behaviour. Possibly meant as an index for the 10 White Mushrooms? Only those in SwapStone Hollow (Well) have unique values though: the surface ones are all set to 1.
	u8 fleeRange;           //Mushroom flees when player/sidekick inside this radius
	s16 gamebitCollected;   //Gamebit to set when collected, preventing mushroom from reappearing (different from the inventory/count gamebitID: see `objData->gamebitInventory`)
	u8 hopSpeed;            //Coefficient for hop speed(?) (0 to 255, mapping from 0.0 to 0.2)
	u8 jumpSpeed;           //Coefficient for jump speed   (0 to 255, mapping from 0.0 to 1.0)
	u8 unused1E;
	u8 alertRange;          //Mushroom enters alert state when player/sidekick inside this radius (also enables sidekick's Find command when sidekick inside radius)
	u8 modelIdx;            //Mushroom type (blue/white)
} Mushroom_Setup;

#endif
