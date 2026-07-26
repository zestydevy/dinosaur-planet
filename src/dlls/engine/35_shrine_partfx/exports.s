.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword shrinePartfx_ctor
.dword shrinePartfx_dtor

# export table
/*0*/ .dword shrinePartfx_Func_18
/*1*/ .dword shrinePartfx_Spawn
/*2*/ .dword shrinePartfx_Func_20
