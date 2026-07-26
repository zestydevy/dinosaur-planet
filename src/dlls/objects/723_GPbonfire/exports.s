.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword GPbonfire_ctor
.dword GPbonfire_dtor

# export table
.dword GPbonfire_obj_Setup
.dword GPbonfire_obj_Control
.dword GPbonfire_obj_Update
.dword GPbonfire_obj_Print
.dword GPbonfire_obj_Free
.dword GPbonfire_obj_GetModelFlags
.dword GPbonfire_obj_GetDataSize
.dword GPbonfire_KyteTarget_Interact
.dword GPbonfire_KyteTarget_Func_A08
.dword GPbonfire_KyteTarget_Approach
.dword GPbonfire_KyteTarget_Func_A38
