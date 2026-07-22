.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SnowHorn_ctor
.dword SnowHorn_dtor

# export table
.dword SnowHorn_obj_Setup
.dword SnowHorn_obj_Control
.dword SnowHorn_obj_Update
.dword SnowHorn_obj_Print
.dword SnowHorn_obj_Free
.dword SnowHorn_obj_GetModelFlags
.dword SnowHorn_obj_GetDataSize
