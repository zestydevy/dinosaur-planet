.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword projlib_ctor
.dword projlib_dtor

# export table
/*0*/ .dword projlib_Func_18
/*1*/ .dword projlib_Func_304
/*2*/ .dword projlib_Func_580
