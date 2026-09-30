.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ECSHshrine_ctor
.dword ECSHshrine_dtor

# export table
/*0*/ .dword ECSHshrine_obj_Setup
/*1*/ .dword ECSHshrine_obj_Control
/*2*/ .dword ECSHshrine_obj_Update
/*3*/ .dword ECSHshrine_obj_Print
/*4*/ .dword ECSHshrine_obj_Free
/*5*/ .dword ECSHshrine_obj_GetModelFlags
/*6*/ .dword ECSHshrine_obj_GetDataSize
/*7*/ .dword ECSHshrine_Func_1454
/*8*/ .dword ECSHshrine_GetCupCoords
/*9*/ .dword ECSHshrine_GetMinigameState
/*10*/ .dword ECSHshrine_SetCupCoords
/*11*/ .dword ECSHshrine_ChooseCup
