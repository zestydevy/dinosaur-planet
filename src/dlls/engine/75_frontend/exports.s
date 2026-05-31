.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword frontend_ctor
.dword frontend_dtor

# export table
/*0*/ .dword frontend_func_E0
/*1*/ .dword frontend_func_1C4
/*2*/ .dword frontend_func_284
/*3*/ .dword frontend_func_354
/*4*/ .dword frontend_func_38C
/*5*/ .dword frontend_func_6A0
/*6*/ .dword frontend_func_77C
/*7*/ .dword frontend_func_790
/*8*/ .dword frontend_func_7C0
/*9*/ .dword frontend_func_7CC
/*10*/ .dword frontend_func_7E0
/*11*/ .dword frontend_func_7F4
