.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camslide_ctor
.dword camslide_dtor

# export table
/*0*/ .dword camslide_func_18
/*1*/ .dword camslide_func_98
/*2*/ .dword camslide_func_3CC
/*3*/ .dword camslide_func_40C
