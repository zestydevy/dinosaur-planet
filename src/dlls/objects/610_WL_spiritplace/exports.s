.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WL_spiritplace_ctor
.dword WL_spiritplace_dtor

# export table
.dword WL_spiritplace_setup
.dword WL_spiritplace_control
.dword WL_spiritplace_update
.dword WL_spiritplace_print
.dword WL_spiritplace_free
.dword WL_spiritplace_get_model_flags
.dword WL_spiritplace_get_data_size
