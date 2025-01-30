.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_30_ctor
.dword dll_30_dtor

# export table
.dword dll_30_func_18
.dword dll_30_func_DC
.dword dll_30_func_368
.dword dll_30_func_390
.dword dll_30_func_3F8
.dword dll_30_func_454
