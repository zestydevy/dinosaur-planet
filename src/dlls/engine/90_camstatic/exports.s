.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_90_ctor
.dword dll_90_dtor

# export table
/*0*/ .dword dll_90_func_18
/*1*/ .dword dll_90_func_278
/*2*/ .dword dll_90_func_584
/*3*/ .dword dll_90_func_5C4
