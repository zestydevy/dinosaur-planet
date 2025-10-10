.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_782_ctor
.dword dll_782_dtor

# export table
.dword dll_782_func_18
.dword dll_782_func_58
.dword dll_782_func_F38
.dword dll_782_func_F44
.dword dll_782_func_F98
.dword dll_782_get_model_flags
.dword dll_782_get_state_size
