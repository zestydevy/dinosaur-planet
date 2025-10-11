.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_782_ctor
.dword dll_782_dtor

# export table
.dword dll_782_setup
.dword dll_782_control
.dword dll_782_update
.dword dll_782_print
.dword dll_782_free
.dword dll_782_get_model_flags
.dword dll_782_get_state_size
