.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_89_ctor
.dword dll_89_dtor

# export table
/*0*/ .dword dll_89_func_18
/*1*/ .dword dll_89_func_588
/*2*/ .dword dll_89_func_DF0
/*3*/ .dword dll_89_func_E30
