.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFTreasSharpy_ctor
.dword CFTreasSharpy_dtor

# export table
/*0*/ .dword CFTreasSharpy_obj_Setup
/*1*/ .dword CFTreasSharpy_obj_Control
/*2*/ .dword CFTreasSharpy_obj_Update
/*3*/ .dword CFTreasSharpy_obj_Print
/*4*/ .dword CFTreasSharpy_obj_Free
/*5*/ .dword CFTreasSharpy_obj_GetModelFlags
/*6*/ .dword CFTreasSharpy_obj_GetDataSize
