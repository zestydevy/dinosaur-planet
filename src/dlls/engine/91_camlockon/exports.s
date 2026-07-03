.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camlockon_ctor
.dword camlockon_dtor

# export table
/*0*/ .dword camlockon_func_18
/*1*/ .dword camlockon_func_1B8
/*2*/ .dword camlockon_func_B64
/*3*/ .dword camlockon_func_BB0
