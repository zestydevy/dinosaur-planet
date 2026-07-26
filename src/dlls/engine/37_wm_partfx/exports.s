.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword wmPartfx_ctor
.dword wmPartfx_dtor

# export table
/*0*/ .dword wmPartfx_Func_18
/*1*/ .dword wmPartfx_Spawn
/*2*/ .dword wmPartfx_Func_20
