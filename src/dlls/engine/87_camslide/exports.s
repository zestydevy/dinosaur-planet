.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_87_ctor
.dword dll_87_dtor

# export table
/*0*/ .dword dll_87_func_18
/*1*/ .dword dll_87_func_98
/*2*/ .dword dll_87_func_3CC
/*3*/ .dword dll_87_func_40C
