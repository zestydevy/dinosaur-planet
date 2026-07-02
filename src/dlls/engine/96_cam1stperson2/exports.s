.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_96_ctor
.dword dll_96_dtor

# export table
/*0*/ .dword dll_96_func_18
/*1*/ .dword dll_96_func_4BC
/*2*/ .dword dll_96_func_93C
/*3*/ .dword dll_96_func_990
