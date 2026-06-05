.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_73_ctor
.dword dll_73_dtor

# export table
/*0*/ .dword dll_73_func_6C
/*1*/ .dword dll_73_func_118
/*2*/ .dword dll_73_func_204
/*3*/ .dword dll_73_func_46C
/*4*/ .dword dll_73_func_540
/*5*/ .dword dll_73_func_55C
/*6*/ .dword dll_73_func_6D0
/*7*/ .dword dll_73_func_764
/*8*/ .dword dll_73_func_780
