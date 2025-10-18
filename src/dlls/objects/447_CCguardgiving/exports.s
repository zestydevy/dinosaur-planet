.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCguardgiving_ctor
.dword CCguardgiving_dtor

# export table
.dword CCguardgiving_setup
.dword CCguardgiving_control
.dword CCguardgiving_update
.dword CCguardgiving_print
.dword CCguardgiving_free
.dword CCguardgiving_get_model_flags
.dword CCguardgiving_get_data_size
