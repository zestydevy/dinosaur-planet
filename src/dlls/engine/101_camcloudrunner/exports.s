.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_101_ctor
.dword dll_101_dtor

# export table
/*0*/ .dword dll_101_func_18
/*1*/ .dword dll_101_func_17C
/*2*/ .dword dll_101_func_2C4
/*3*/ .dword dll_101_func_304
