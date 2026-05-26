.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_75_ctor
.dword dll_75_dtor

# export table
/*0*/ .dword dll_75_func_E0
/*1*/ .dword dll_75_func_1C4
/*2*/ .dword dll_75_func_284
/*3*/ .dword dll_75_func_354
/*4*/ .dword dll_75_func_38C
/*5*/ .dword dll_75_func_6A0
/*6*/ .dword dll_75_func_77C
/*7*/ .dword dll_75_func_790
/*8*/ .dword dll_75_func_7C0
/*9*/ .dword dll_75_func_7CC
/*10*/ .dword dll_75_func_7E0
/*11*/ .dword dll_75_func_7F4
