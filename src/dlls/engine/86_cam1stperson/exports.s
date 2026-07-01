.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_86_ctor
.dword dll_86_dtor

# export table
/*0*/ .dword dll_86_func_18
/*1*/ .dword dll_86_func_4F8
/*2*/ .dword dll_86_func_A04
/*3*/ .dword dll_86_func_A64
