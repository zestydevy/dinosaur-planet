.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_17_ctor
.dword dll_17_dtor

# export table
/*0*/ .dword dll_17_func_398
/*1*/ .dword dll_17_func_D74
/*2*/ .dword dll_17_func_3A0
