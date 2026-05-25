.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_65_ctor
.dword dll_65_dtor

# export table
/*0*/ .dword dll_65_func_1FC
/*1*/ .dword dll_65_func_B84
/*2*/ .dword dll_65_func_B8C
