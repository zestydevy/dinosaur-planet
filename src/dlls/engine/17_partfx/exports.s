.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword partfx_ctor
.dword partfx_dtor

# export table
/*0*/ .dword partfx_func_398
/*1*/ .dword partfx_func_D74
/*2*/ .dword partfx_func_3A0
