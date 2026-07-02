.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_93_ctor
.dword dll_93_dtor

# export table
/*0*/ .dword dll_93_func_18
/*1*/ .dword dll_93_func_340
/*2*/ .dword dll_93_func_5E8
/*3*/ .dword dll_93_func_62C
