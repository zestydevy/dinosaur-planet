.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_227_ctor
.dword dll_227_dtor

# export table
.dword dll_227_setup
.dword dll_227_control
.dword dll_227_update
.dword dll_227_print
.dword dll_227_free
.dword dll_227_get_model_flags
.dword dll_227_get_state_size
.dword dll_227_func_171C
.dword dll_227_func_172C
.dword dll_227_func_174C
.dword dll_227_func_17C0
.dword dll_227_func_1828
.dword dll_227_func_1840
