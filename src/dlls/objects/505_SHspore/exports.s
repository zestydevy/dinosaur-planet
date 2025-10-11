.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHspore_ctor
.dword SHspore_dtor

# export table
.dword SHspore_setup
.dword SHspore_control
.dword SHspore_update
.dword SHspore_print
.dword SHspore_free
.dword SHspore_get_model_flags
.dword SHspore_get_data_size
