.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_4_ctor
.dword dll_4_dtor

# export table
/*0*/ .dword dll_4_func_64
/*1*/ .dword dll_4_func_80
/*2*/ .dword dll_4_func_150
/*3*/ .dword dll_4_func_200
/*4*/ .dword dll_4_func_9B0
/*5*/ .dword dll_4_func_119C
/*6*/ .dword dll_4_func_15B4
/*7*/ .dword dll_4_func_15D0
/*8*/ .dword dll_4_func_1F60
/*9*/ .dword dll_4_func_EF0
/*10*/ .dword dll_4_func_EA4
/*11*/ .dword dll_4_func_FB4
/*12*/ .dword dll_4_func_1100
/*13*/ .dword dll_4_func_1028
/*14*/ .dword dll_4_func_FDC
