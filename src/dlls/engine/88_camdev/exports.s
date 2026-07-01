.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_88_ctor
.dword dll_88_dtor

# export table
/*0*/ .dword dll_88_func_18
/*1*/ .dword dll_88_func_78
/*2*/ .dword dll_88_func_340
/*3*/ .dword dll_88_func_380
