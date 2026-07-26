.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_52_ctor
.dword dll_52_dtor

# export table
/*0*/ .dword dll_52_Func_18
/*1*/ .dword dll_52_Func_194
/*2*/ .dword dll_52_Func_20
