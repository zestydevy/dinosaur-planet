.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_23_ctor
.dword dll_23_dtor

# export table
.dword dll_23_func_24
.dword dll_23_func_30
.dword dll_23_func_18
.dword dll_23_func_3C
.dword dll_23_func_4C
.dword dll_23_func_60
.dword dll_23_func_6C
.dword dll_23_func_8C
