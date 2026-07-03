.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camunk_ctor
.dword camunk_dtor

# export table
/*0*/ .dword camunk_func_18
/*1*/ .dword camunk_func_74
/*2*/ .dword camunk_func_250
/*3*/ .dword camunk_func_290
