.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Kyte_ctor
.dword Kyte_dtor

# export table
/*0*/ .dword Kyte_obj_Setup
/*1*/ .dword Kyte_obj_Control
/*2*/ .dword Kyte_obj_Update
/*3*/ .dword Kyte_obj_Print
/*4*/ .dword Kyte_obj_Free
/*5*/ .dword Kyte_obj_GetModelFlags
/*6*/ .dword Kyte_obj_GetDataSize
/*7*/ .dword Kyte_sidekick_Func7
/*8*/ .dword Kyte_sidekick_Func8
/*9*/ .dword Kyte_sidekick_Func9
/*10*/ .dword Kyte_sidekick_Func10
/*11*/ .dword Kyte_sidekick_Func11
/*12*/ .dword Kyte_sidekick_Func12
/*13*/ .dword Kyte_sidekick_GetAvailableCommands
/*14*/ .dword Kyte_sidekick_EnableCommand
/*15*/ .dword Kyte_sidekick_GetBlueFoodCount
/*16*/ .dword Kyte_sidekick_GetRedFoodCount
/*17*/ .dword Kyte_sidekick_GetBlueAndRedFoodCount
/*18*/ .dword Kyte_sidekick_SetRedFoodCount
/*19*/ .dword Kyte_sidekick_Func19
/*20*/ .dword Kyte_sidekick_Func20
/*21*/ .dword Kyte_sidekick_Func21
/*22*/ .dword Kyte_sidekick_Func22
/*23*/ .dword Kyte_sidekick_Func23
/*24*/ .dword Kyte_sidekick_Func24
/*25*/ .dword Kyte_sidekick_Func25
/*26*/ .dword Kyte_sidekick_Func26
