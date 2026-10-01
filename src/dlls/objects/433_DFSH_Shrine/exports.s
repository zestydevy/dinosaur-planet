.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFShrine_ctor
.dword DFShrine_dtor

# export table
/*0*/ .dword DFShrine_obj_Setup
/*1*/ .dword DFShrine_obj_Control
/*2*/ .dword DFShrine_obj_Update
/*3*/ .dword DFShrine_obj_Print
/*4*/ .dword DFShrine_obj_Free
/*5*/ .dword DFShrine_obj_GetModelFlags
/*6*/ .dword DFShrine_obj_GetDataSize
