.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_103_ctor
.dword dll_103_dtor

# export table
/*0*/ .dword dll_103_func_18
/*1*/ .dword dll_103_func_98
/*2*/ .dword dll_103_func_1A4
/*3*/ .dword dll_103_func_1E4
