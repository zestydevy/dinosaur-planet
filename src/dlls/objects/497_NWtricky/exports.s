.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword NWtricky_ctor
.dword NWtricky_dtor

# export table
/*0*/ .dword NWtricky_obj_Setup
/*1*/ .dword NWtricky_obj_Control
/*2*/ .dword NWtricky_obj_Update
/*3*/ .dword NWtricky_obj_Print
/*4*/ .dword NWtricky_obj_Free
/*5*/ .dword NWtricky_obj_GetModelFlags
/*6*/ .dword NWtricky_obj_GetDataSize
