.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFP_RotatePuzzle_ctor
.dword DFP_RotatePuzzle_dtor

# export table
/*0*/ .dword DFP_RotatePuzzle_obj_Setup
/*1*/ .dword DFP_RotatePuzzle_obj_Control
/*2*/ .dword DFP_RotatePuzzle_obj_Update
/*3*/ .dword DFP_RotatePuzzle_obj_Print
/*4*/ .dword DFP_RotatePuzzle_obj_Free
/*5*/ .dword DFP_RotatePuzzle_obj_GetModelFlags
/*6*/ .dword DFP_RotatePuzzle_obj_GetDataSize
