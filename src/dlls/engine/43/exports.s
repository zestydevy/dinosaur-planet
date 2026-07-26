.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_43_ctor
.dword dll_43_dtor

# export table
/*0*/ .dword dll_43_Func_18
/*1*/ .dword dll_43_Func_194
/*2*/ .dword dll_43_Func_20
