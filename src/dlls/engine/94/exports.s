.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_94_ctor
.dword dll_94_dtor

# export table
.dword dll_94_setup
.dword dll_94_control
.dword dll_94_free
.dword dll_94_func_7C
