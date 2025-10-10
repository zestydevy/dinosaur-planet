.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Duster_ctor
.dword Duster_dtor

# export table
.dword Duster_create
.dword Duster_update
.dword Duster_func_754
.dword Duster_draw
.dword Duster_destroy
.dword Duster_get_model_flags
.dword Duster_get_state_size
