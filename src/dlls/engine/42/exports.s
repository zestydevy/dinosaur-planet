.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_42_ctor
.dword dll_42_dtor

# export table
/*0*/ .dword dll_42_Func_18
/*1*/ .dword dll_42_Func_194
/*2*/ .dword dll_42_Func_20
