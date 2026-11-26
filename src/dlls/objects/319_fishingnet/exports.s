.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword fishingnet_ctor
.dword fishingnet_dtor

# export table
/*0*/ .dword fishingnet_obj_Setup
/*1*/ .dword fishingnet_obj_Control
/*2*/ .dword fishingnet_obj_Update
/*3*/ .dword fishingnet_obj_Print
/*4*/ .dword fishingnet_obj_Free
/*5*/ .dword fishingnet_obj_GetModelFlags
/*6*/ .dword fishingnet_obj_GetDataSize
/*7*/ .dword fishingnet_SetVisibility
/*8*/ .dword fishingnet_Draw
/*9*/ .dword fishingnet_GetTransform
/*10*/ .dword fishingnet_State
/*11*/ .dword fishingnet_Func4D4
