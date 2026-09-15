.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SCFlameGameFlame_ctor
.dword SCFlameGameFlame_dtor

# export table
/*0*/ .dword SCFlameGameFlame_obj_Setup
/*1*/ .dword SCFlameGameFlame_obj_Control
/*2*/ .dword SCFlameGameFlame_obj_Update
/*3*/ .dword SCFlameGameFlame_obj_Print
/*4*/ .dword SCFlameGameFlame_obj_Free
/*5*/ .dword SCFlameGameFlame_obj_GetModelFlags
/*6*/ .dword SCFlameGameFlame_obj_GetDataSize
/*7*/ .dword SCFlameGameFlame_KyteTarget_Interact
/*8*/ .dword SCFlameGameFlame_KyteTarget_Func_3A4
/*9*/ .dword SCFlameGameFlame_KyteTarget_Approach
/*10*/ .dword SCFlameGameFlame_KyteTarget_Func_3D4
