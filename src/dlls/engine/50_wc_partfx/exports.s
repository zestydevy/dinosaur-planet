.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword wcPartfx_ctor
.dword wcPartfx_dtor

# export table
/*0*/ .dword wcPartfx_Func_18
/*1*/ .dword wcPartfx_Spawn
/*2*/ .dword wcPartfx_Func_20
