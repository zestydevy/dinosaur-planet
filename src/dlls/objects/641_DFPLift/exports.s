.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFPLift_ctor
.dword DFPLift_dtor

# export table
.dword DFPLift_setup
.dword DFPLift_control
.dword DFPLift_update
.dword DFPLift_print
.dword DFPLift_free
.dword DFPLift_get_model_flags
.dword DFPLift_get_state_size
