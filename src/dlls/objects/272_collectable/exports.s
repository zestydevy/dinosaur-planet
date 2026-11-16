.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword collectable_ctor
.dword collectable_dtor

# export table
/*0*/ .dword collectable_obj_Setup
/*1*/ .dword collectable_obj_Control
/*2*/ .dword collectable_obj_Update
/*3*/ .dword collectable_obj_Print
/*4*/ .dword collectable_obj_Free
/*5*/ .dword collectable_obj_GetModelFlags
/*6*/ .dword collectable_obj_GetDataSize
/*7*/ .dword collectable_IsCollected
/*8*/ .dword collectable_SetPauseState
/*9*/ .dword collectable_GetAreaValue
/*10*/ .dword collectable_SetVelocity
/*11*/ .dword collectable_SetVisibility
/*12*/ .dword collectable_GetVisibility
/*13*/ .dword collectable_SavePosition
