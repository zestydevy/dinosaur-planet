.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dfPartfx_ctor
.dword dfPartfx_dtor

# export table
/*0*/ .dword dfPartfx_Func_18
/*1*/ .dword dfPartfx_Spawn
/*2*/ .dword dfPartfx_Func_20
