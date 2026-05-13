.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_53_ctor
.dword dll_53_dtor

# export table
/*0*/ .dword dll_53_func_4B8
/*1*/ .dword dll_53_func_B84
/*2*/ .dword dll_53_func_BA4
/*3*/ .dword dll_53_func_CDC
/*4*/ .dword dll_53_func_E9C
/*5*/ .dword dll_53_func_B90
/*6*/ .dword dll_53_func_1E70
/*7*/ .dword dll_53_func_1C0C
/*8*/ .dword dll_53_func_1DEC
/*9*/ .dword dll_53_func_1CC8
/*10*/ .dword dll_53_func_1130
/*11*/ .dword dll_53_func_14F4
/*12*/ .dword dll_53_func_1F1C
