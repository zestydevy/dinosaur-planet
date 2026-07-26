.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_45_ctor
.dword dll_45_dtor

# export table
/*0*/ .dword dll_45_Func_18
/*1*/ .dword dll_45_Func_2C
/*2*/ .dword dll_45_Func_20
