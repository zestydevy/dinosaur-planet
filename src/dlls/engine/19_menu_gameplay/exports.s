.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_19_ctor
.dword dll_19_dtor

# export table
/*0*/ .dword dll_19_func_18
/*1*/ .dword dll_19_func_5C
/*2*/ .dword dll_19_func_A0
