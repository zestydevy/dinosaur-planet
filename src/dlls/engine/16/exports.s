.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_16_ctor
.dword dll_16_dtor

# export table
.dword dll_16_func_18
.dword dll_16_func_20
.dword dll_16_func_2C
