.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_99_ctor
.dword dll_99_dtor

# export table
/*0*/ .dword dll_99_func_18
/*1*/ .dword dll_99_func_94
/*2*/ .dword dll_99_func_1CC
/*3*/ .dword dll_99_func_1D8
