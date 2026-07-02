.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_91_ctor
.dword dll_91_dtor

# export table
/*0*/ .dword dll_91_func_18
/*1*/ .dword dll_91_func_1B8
/*2*/ .dword dll_91_func_B64
/*3*/ .dword dll_91_func_BB0
