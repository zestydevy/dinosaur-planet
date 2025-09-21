.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_62_ctor
.dword dll_62_dtor

# export table
.dword dll_62_func_31C
.dword dll_62_func_764
.dword dll_62_func_76C
