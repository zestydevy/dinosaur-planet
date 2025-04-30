.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_63_ctor
.dword dll_63_dtor

# export table
.dword dll_63_func_220
.dword dll_63_func_5A0
.dword dll_63_func_5A8
