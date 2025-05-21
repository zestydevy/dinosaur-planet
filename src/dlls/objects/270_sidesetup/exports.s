.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_270_ctor
.dword dll_270_dtor

# export table
.dword dll_270_func_18
.dword dll_270_func_3C
.dword dll_270_func_48
.dword dll_270_func_54
.dword dll_270_func_6C
.dword dll_270_func_7C
.dword dll_270_func_8C
