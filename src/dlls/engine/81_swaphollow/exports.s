.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_81_ctor
.dword dll_81_dtor

# export table
/*0*/ .dword dll_81_func_18
/*1*/ .dword dll_81_func_A4
/*2*/ .dword dll_81_func_224
/*3*/ .dword dll_81_func_3C8
/*4*/ .dword dll_81_func_4DC
