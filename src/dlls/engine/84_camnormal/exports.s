.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_84_ctor
.dword dll_84_dtor

# export table
/*0*/ .dword dll_84_func_A8
/*1*/ .dword dll_84_func_860
/*2*/ .dword dll_84_func_B70
/*3*/ .dword dll_84_func_BD4
/*4*/ .dword dll_84_func_ED4
/*5*/ .dword dll_84_func_11F0
/*6*/ .dword dll_84_func_12E8
/*7*/ .dword dll_84_func_19E8
/*8*/ .dword dll_84_func_1A58
