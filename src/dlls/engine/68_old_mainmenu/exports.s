.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_68_ctor
.dword dll_68_dtor

# export table
/*0*/ .dword dll_68_func_C4
/*1*/ .dword dll_68_func_D0
/*2*/ .dword dll_68_func_D8
