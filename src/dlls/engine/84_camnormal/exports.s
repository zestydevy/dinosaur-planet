.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camnormal_ctor
.dword camnormal_dtor

# export table
/*0*/ .dword camnormal_func_A8
/*1*/ .dword camnormal_func_860
/*2*/ .dword camnormal_func_B70
/*3*/ .dword camnormal_func_BD4
/*4*/ .dword camnormal_func_ED4
/*5*/ .dword camnormal_func_11F0
/*6*/ .dword camnormal_func_12E8
/*7*/ .dword camnormal_func_19E8
/*8*/ .dword camnormal_func_1A58
