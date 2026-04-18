#ifndef _DLLS_COMMON_SIDEKICK_H
#define _DLLS_COMMON_SIDEKICK_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dll_def.h"

#define NO_SIDEKICK_COMMAND -1
#define TOTAL_SIDEKICK_COMMANDS 6

#define INDEX_TO_FLAG(index) 1 << (index)

typedef enum {
    Sidekick_Command_INDEX_0_Heel     = 0,
    Sidekick_Command_INDEX_1_Find     = 1,
    Sidekick_Command_INDEX_2_Distract = 2,
    Sidekick_Command_INDEX_3_Guard    = 3, 
    Sidekick_Command_INDEX_4_Flame    = 4,
    Sidekick_Command_INDEX_5_Play     = 5
} SidekickCommandIndices;

typedef enum {
    Sidekick_Command_FLAG_01_Heel     = INDEX_TO_FLAG(Sidekick_Command_INDEX_0_Heel),
    Sidekick_Command_FLAG_02_Find     = INDEX_TO_FLAG(Sidekick_Command_INDEX_1_Find),
    Sidekick_Command_FLAG_04_Distract = INDEX_TO_FLAG(Sidekick_Command_INDEX_2_Distract),
    Sidekick_Command_FLAG_08_Guard    = INDEX_TO_FLAG(Sidekick_Command_INDEX_3_Guard),
    Sidekick_Command_FLAG_10_Flame    = INDEX_TO_FLAG(Sidekick_Command_INDEX_4_Flame),
    Sidekick_Command_FLAG_20_Play     = INDEX_TO_FLAG(Sidekick_Command_INDEX_5_Play)
} SidekickCommandFlags;


DLL_INTERFACE(DLL_ISidekick) {
	/*:*/ DLL_INTERFACE_BASE(DLL_IObject);
	/*7*/ UnknownDLLFunc func7;
	/*8*/ UnknownDLLFunc func8;
	/*9*/ UnknownDLLFunc func9;
	/*10*/ UnknownDLLFunc func10;
	/*11*/ UnknownDLLFunc func11;
	/*12*/ UnknownDLLFunc func12;
	/*13*/ s32 (*func13)(Object*);	//Returns a bitfield of the sidekick's available commands (see `SidekickCommands`)
	/*14*/ void (*func14)(Object* self, s32 gamebitID); //Show a sidekick command in the inventory (e.g. Flame available when near vines)
	/*15*/ s32 (*get_blue_food_count)(Object *); // How much blue food they currently have
	/*16*/ s32 (*get_red_food_count)(Object *);  // How much red food they currently have
	/*17*/ UnknownDLLFunc func17;
	/*18*/ void (*func18)(Object* self, s32);
	/*19*/ UnknownDLLFunc func19;
	/*20*/ UnknownDLLFunc func20;
	/*21*/ void (*func21)(Object *, s32, s32);
	/*22*/ void (*func22)(Object *, Object *);
	/*23*/ void (*func23)(Object *);
	/*24*/ s32 (*func24)(Object *);
	/*25*/ s32 (*func25)(Object*);
 	/*26*/ s32 (*func26)(Object*, s32*);
};

#endif //_DLLS_COMMON_SIDEKICK_H
