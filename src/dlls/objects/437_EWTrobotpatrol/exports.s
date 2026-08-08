.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword EWTrobotpatrol_ctor
.dword EWTrobotpatrol_dtor

# export table
/*0*/ .dword EWTrobotpatrol_obj_Setup
/*1*/ .dword EWTrobotpatrol_obj_Control
/*2*/ .dword EWTrobotpatrol_obj_Update
/*3*/ .dword EWTrobotpatrol_obj_Print
/*4*/ .dword EWTrobotpatrol_obj_Free
/*5*/ .dword EWTrobotpatrol_obj_GetModelFlags
/*6*/ .dword EWTrobotpatrol_obj_GetDataSize
/*7*/ .dword EWTrobotpatrol_SetBase
/*8*/ .dword EWTrobotpatrol_MoveTo
