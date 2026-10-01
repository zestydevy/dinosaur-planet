.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFlevelcontrol_ctor
.dword DFlevelcontrol_dtor

# export table
/*0*/ .dword DFlevelcontrol_obj_Setup
/*1*/ .dword DFlevelcontrol_obj_Control
/*2*/ .dword DFlevelcontrol_obj_Update
/*3*/ .dword DFlevelcontrol_obj_Print
/*4*/ .dword DFlevelcontrol_obj_Free
/*5*/ .dword DFlevelcontrol_obj_GetModelFlags
/*6*/ .dword DFlevelcontrol_obj_GetDataSize
