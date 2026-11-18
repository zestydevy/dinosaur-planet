.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFPLift_ctor
.dword DFPLift_dtor

# export table
.dword DFPLift_obj_Setup
.dword DFPLift_obj_Control
.dword DFPLift_obj_Update
.dword DFPLift_obj_Print
.dword DFPLift_obj_Free
.dword DFPLift_obj_GetModelFlags
.dword DFPLift_obj_GetDataSize
