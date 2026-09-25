.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ToggleSwitch_ctor
.dword ToggleSwitch_dtor

# export table
/*0*/ .dword ToggleSwitch_obj_Setup
/*1*/ .dword ToggleSwitch_obj_Control
/*2*/ .dword ToggleSwitch_obj_Update
/*3*/ .dword ToggleSwitch_obj_Print
/*4*/ .dword ToggleSwitch_obj_Free
/*5*/ .dword ToggleSwitch_obj_GetModelFlags
/*6*/ .dword ToggleSwitch_obj_GetDataSize
