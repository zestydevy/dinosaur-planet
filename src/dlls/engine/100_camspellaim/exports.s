.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_100_ctor
.dword dll_100_dtor

# export table
/*0*/ .dword dll_100_func_18
/*1*/ .dword dll_100_func_64
/*2*/ .dword dll_100_func_2C8
/*3*/ .dword dll_100_func_2D4
