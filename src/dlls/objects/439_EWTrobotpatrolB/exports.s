.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword EWTrobotpatrolB_ctor
.dword EWTrobotpatrolB_dtor

# export table
/*0*/ .dword EWTrobotpatrolB_obj_Setup
/*1*/ .dword EWTrobotpatrolB_obj_Control
/*2*/ .dword EWTrobotpatrolB_obj_Update
/*3*/ .dword EWTrobotpatrolB_obj_Print
/*4*/ .dword EWTrobotpatrolB_obj_Free
/*5*/ .dword EWTrobotpatrolB_obj_GetModelFlags
/*6*/ .dword EWTrobotpatrolB_obj_GetDataSize
