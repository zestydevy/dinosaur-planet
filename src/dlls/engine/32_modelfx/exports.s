.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_32_ctor
.dword dll_32_dtor

# export table
/*0*/ .dword dll_32_func_2C8
/*1*/ .dword dll_32_func_2D0
/*2*/ .dword dll_32_func_2DC
/*3*/ .dword dll_32_func_4B0
/*4*/ .dword dll_32_func_92C
/*5*/ .dword dll_32_func_1314
