.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword tricky_ctor
.dword tricky_dtor

# export table
/*0*/ .dword tricky_obj_Setup
/*1*/ .dword tricky_obj_Control
/*2*/ .dword tricky_obj_Update
/*3*/ .dword tricky_obj_Print
/*4*/ .dword tricky_obj_Free
/*5*/ .dword tricky_obj_GetModelFlags
/*6*/ .dword tricky_obj_GetDataSize
/*7*/ .dword tricky_sidekick_Func7
/*8*/ .dword tricky_sidekick_Func8
/*9*/ .dword tricky_sidekick_Func9
/*10*/ .dword tricky_sidekick_Func10
/*11*/ .dword tricky_sidekick_Func11
/*12*/ .dword tricky_sidekick_Func12
/*13*/ .dword tricky_sidekick_GetAvailableCommands
/*14*/ .dword tricky_sidekick_EnableCommand
/*15*/ .dword tricky_sidekick_GetBlueFoodCount
/*16*/ .dword tricky_sidekick_GetRedFoodCount
/*17*/ .dword tricky_sidekick_GetBlueAndRedFoodCount
/*18*/ .dword tricky_sidekick_SetRedFoodCount
/*19*/ .dword tricky_sidekick_Func19
/*20*/ .dword tricky_sidekick_Func20
/*21*/ .dword tricky_sidekick_Func21
/*22*/ .dword tricky_sidekick_Func22
/*23*/ .dword tricky_sidekick_Func23
/*24*/ .dword tricky_sidekick_Func24
/*25*/ .dword tricky_sidekick_Func25
/*26*/ .dword tricky_sidekick_Func26
