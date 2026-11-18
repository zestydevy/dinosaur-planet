.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword KamColumn_ctor
.dword KamColumn_dtor

# export table
/*0*/ .dword KamColumn_obj_Setup
/*1*/ .dword KamColumn_obj_Control
/*2*/ .dword KamColumn_obj_Update
/*3*/ .dword KamColumn_obj_Print
/*4*/ .dword KamColumn_obj_Free
/*5*/ .dword KamColumn_obj_GetModelFlags
/*6*/ .dword KamColumn_obj_GetDataSize
