.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camstatic_ctor
.dword camstatic_dtor

# export table
/*0*/ .dword camstatic_func_18
/*1*/ .dword camstatic_func_278
/*2*/ .dword camstatic_func_584
/*3*/ .dword camstatic_func_5C4
