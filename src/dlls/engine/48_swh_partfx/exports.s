.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword swhPartfx_ctor
.dword swhPartfx_dtor

# export table
/*0*/ .dword swhPartfx_Func_18
/*1*/ .dword swhPartfx_Spawn
/*2*/ .dword swhPartfx_Func_20
