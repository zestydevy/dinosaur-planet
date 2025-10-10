.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_638_ctor
.dword dll_638_dtor

# export table
.dword dll_638_func_18
.dword dll_638_func_260
.dword dll_638_func_51C
.dword dll_638_func_528
.dword dll_638_func_540
.dword dll_638_get_model_flags
.dword dll_638_get_state_size
.dword dll_638_func_5E4
