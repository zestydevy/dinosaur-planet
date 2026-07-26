.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword bayPartfx_ctor
.dword bayPartfx_dtor

# export table
/*0*/ .dword bayPartfx_Func_18
/*1*/ .dword bayPartfx_Spawn
/*2*/ .dword bayPartfx_Func_20
