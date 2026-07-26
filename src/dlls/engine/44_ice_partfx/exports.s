.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword icePartfx_ctor
.dword icePartfx_dtor

# export table
/*0*/ .dword icePartfx_Func_18
/*1*/ .dword icePartfx_Spawn
/*2*/ .dword icePartfx_Func_20
