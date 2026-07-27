.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_57_ctor
.dword dll_57_dtor

# export table
/*0*/ .dword dll_57_Func_18
/*1*/ .dword dll_57_Func_74
/*2*/ .dword dll_57_Func_190
/*3*/ .dword dll_57_Func_1E4
