.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword clfPartfx_ctor
.dword clfPartfx_dtor

# export table
/*0*/ .dword clfPartfx_Func_18
/*1*/ .dword clfPartfx_Spawn
/*2*/ .dword clfPartfx_Func_20
