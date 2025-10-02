.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFPLift_ctor
.dword DFPLift_dtor

# export table
.dword DFPLift_create
.dword DFPLift_update
.dword DFPLift_func_7EC
.dword DFPLift_draw
.dword DFPLift_destroy
.dword DFPLift_func_8F8
.dword DFPLift_get_state_size
