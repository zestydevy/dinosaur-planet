.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_83_ctor
.dword dll_83_dtor

# export table
/*0*/ .dword dll_83_func_18
/*1*/ .dword dll_83_func_A4
