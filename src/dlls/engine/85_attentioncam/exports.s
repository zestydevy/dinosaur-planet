.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_85_ctor
.dword dll_85_dtor

# export table
/*0*/ .dword dll_85_func_18
/*1*/ .dword dll_85_func_848
/*2*/ .dword dll_85_func_AC4
/*3*/ .dword dll_85_func_B04
