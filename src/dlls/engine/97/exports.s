.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_97_ctor
.dword dll_97_dtor

# export table
/*0*/ .dword dll_97_func_18
/*1*/ .dword dll_97_func_74
/*2*/ .dword dll_97_func_250
/*3*/ .dword dll_97_func_290
