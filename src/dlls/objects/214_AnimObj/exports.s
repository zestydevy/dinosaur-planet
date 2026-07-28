.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword AnimObj_ctor
.dword AnimObj_dtor

# export table
.dword AnimObj_obj_Setup
.dword AnimObj_obj_Control
.dword AnimObj_obj_Update
.dword AnimObj_obj_Print
.dword AnimObj_obj_Free
.dword AnimObj_obj_GetModelFlags
.dword AnimObj_obj_GetDataSize
