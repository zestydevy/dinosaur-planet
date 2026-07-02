.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_95_ctor
.dword dll_95_dtor

# export table
/*0*/ .dword dll_95_func_18
/*1*/ .dword dll_95_func_320
/*2*/ .dword dll_95_func_544
/*3*/ .dword dll_95_func_584
