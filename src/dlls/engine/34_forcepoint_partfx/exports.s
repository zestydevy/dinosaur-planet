.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword forcepointPartfx_ctor
.dword forcepointPartfx_dtor

# export table
/*0*/ .dword forcepointPartfx_Func_18
/*1*/ .dword forcepointPartfx_Spawn
/*2*/ .dword forcepointPartfx_Func_20
