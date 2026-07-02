.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_102_ctor
.dword dll_102_dtor

# export table
/*0*/ .dword dll_102_func_18
/*1*/ .dword dll_102_func_138
/*2*/ .dword dll_102_func_61C
/*3*/ .dword dll_102_func_65C
