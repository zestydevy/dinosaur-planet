.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camdrop_ctor
.dword camdrop_dtor

# export table
/*0*/ .dword camdrop_func_18
/*1*/ .dword camdrop_func_98
/*2*/ .dword camdrop_func_1A4
/*3*/ .dword camdrop_func_1E4
