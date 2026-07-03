.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camcannon_ctor
.dword camcannon_dtor

# export table
/*0*/ .dword camcannon_func_18
/*1*/ .dword camcannon_func_94
/*2*/ .dword camcannon_func_1CC
/*3*/ .dword camcannon_func_1D8
