.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_70_ctor
.dword dll_70_dtor

# export table
/*0*/ .dword dll_70_func_18
/*1*/ .dword dll_70_func_24
/*2*/ .dword dll_70_func_2C
