.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword mmpPartfx_ctor
.dword mmpPartfx_dtor

# export table
/*0*/ .dword mmpPartfx_Func_18
/*1*/ .dword mmpPartfx_Spawn
/*2*/ .dword mmpPartfx_Func_20
