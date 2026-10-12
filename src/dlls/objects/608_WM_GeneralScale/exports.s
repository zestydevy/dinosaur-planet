.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WMGeneralScale_ctor
.dword WMGeneralScale_dtor

# export table
/*0*/ .dword WMGeneralScale_obj_Setup
/*1*/ .dword WMGeneralScale_obj_Control
/*2*/ .dword WMGeneralScale_obj_Update
/*3*/ .dword WMGeneralScale_obj_Print
/*4*/ .dword WMGeneralScale_obj_Free
/*5*/ .dword WMGeneralScale_obj_GetModelFlags
/*6*/ .dword WMGeneralScale_obj_GetDataSize
