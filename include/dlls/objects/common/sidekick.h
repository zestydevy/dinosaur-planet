#ifndef _DLLS_COMMON_SIDEKICK_H
#define _DLLS_COMMON_SIDEKICK_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"
#include "dlls/engine/27.h"
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
    Sidekick_Command_INDEX_5_Play     = 5,
    Sidekick_Command_COUNT
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
	/*7*/ s32 (*Func7)(s32 arg0);
	/*8*/ void (*Func8)(Object* self, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2, UNK_TYPE_32 arg3);
	/*9*/ void (*Func9)(Object* self, UNK_TYPE_32 arg1);
	/*10*/ void (*Func10)(Object* self, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2, UNK_TYPE_32 arg3);
	/*11*/ f32 (*Func11)(Object* self);
	/*12*/ DLL27_Data* (*Func12)(s32 arg0);
	/*13*/ s32 (*GetAvailableCommands)(Object* self); //Returns a bitfield of the sidekick's available commands (see `SidekickCommands`)
	/*14*/ void (*EnableCommand)(Object* self, s32 commandIndex); // Enables a sidekick command (e.g. allowing Flame when near vines).
	/*15*/ u8 (*GetBlueFoodCount)(Object* self);
	/*16*/ u8 (*GetRedFoodCount)(Object* self);
	/*17*/ s32 (*GetBlueAndRedFoodCount)(Object* self);
	/*18*/ void (*SetRedFoodCount)(Object* self, s32 amount);
	/*19*/ s32 (*Func19)(Object* self);
	/*20*/ void (*Func20)(Object* self, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2, UNK_TYPE_32 arg3);
	/*21*/ void (*Func21)(Object* self, s32 arg1, Object* arg2);
	/*22*/ void (*Func22)(Object* self, Object* arg1);
	/*23*/ void (*Func23)(Object* self);
	/*24*/ s32 (*Func24)(Object* self);
	/*25*/ s32 (*Func25)(Object* self);
	/*26*/ int (*Func26)(Object* self, s32* arg1);
};

#endif //_DLLS_COMMON_SIDEKICK_H
