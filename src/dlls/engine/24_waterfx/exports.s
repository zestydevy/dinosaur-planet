.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword waterfx_ctor
.dword waterfx_dtor

# export table
.dword waterfx_func_6E8
.dword waterfx_func_564
.dword waterfx_func_C7C
.dword waterfx_func_174C
.dword waterfx_func_1CC8
.dword waterfx_func_1B28
.dword waterfx_func_24C
.dword waterfx_func_1C88
