.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_94_ctor
.dword dll_94_dtor

# export table
.dword dll_94_func_18
.dword dll_94_func_64
.dword dll_94_func_70
.dword dll_94_func_7C
