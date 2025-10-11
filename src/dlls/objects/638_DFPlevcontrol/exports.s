.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_638_ctor
.dword dll_638_dtor

# export table
.dword dll_638_setup
.dword dll_638_control
.dword dll_638_update
.dword dll_638_print
.dword dll_638_free
.dword dll_638_get_model_flags
.dword dll_638_get_state_size
.dword dll_638_func_5E4
