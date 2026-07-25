.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camspellaim_ctor
.dword camspellaim_dtor

# export table
/*0*/ .dword camspellaim_func_18
/*1*/ .dword camspellaim_func_64
/*2*/ .dword camspellaim_func_2C8
/*3*/ .dword camspellaim_func_2D4
