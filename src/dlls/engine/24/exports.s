.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_24_ctor
.dword dll_24_dtor

# export table
.dword dll_24_func_6E8
.dword dll_24_func_564
.dword dll_24_func_C7C
.dword dll_24_func_174C
.dword dll_24_func_1CC8
.dword dll_24_func_1B28
.dword dll_24_func_24C
.dword dll_24_func_1C88
