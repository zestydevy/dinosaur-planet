.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCCannonSwitch_ctor
.dword CCCannonSwitch_dtor

# export table
/*0*/ .dword CCCannonSwitch_obj_Setup
/*1*/ .dword CCCannonSwitch_obj_Control
/*2*/ .dword CCCannonSwitch_obj_Update
/*3*/ .dword CCCannonSwitch_obj_Print
/*4*/ .dword CCCannonSwitch_obj_Free
/*5*/ .dword CCCannonSwitch_obj_GetModelFlags
/*6*/ .dword CCCannonSwitch_obj_GetDataSize
