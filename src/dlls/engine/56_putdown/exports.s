.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_56_ctor
.dword dll_56_dtor

# export table
/*0*/ .dword dll_56_func_18
/*1*/ .dword dll_56_func_1B4
/*2*/ .dword dll_56_func_3F0
/*3*/ .dword dll_56_func_47C
/*4*/ .dword dll_56_func_5E8
/*5*/ .dword dll_56_func_730
/*6*/ .dword dll_56_func_80C
/*7*/ .dword dll_56_func_900
/*8*/ .dword dll_56_func_9E0
/*9*/ .dword dll_56_func_AFC
