.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword rexPartfx_ctor
.dword rexPartfx_dtor

# export table
/*0*/ .dword rexPartfx_Func_18
/*1*/ .dword rexPartfx_Spawn
/*2*/ .dword rexPartfx_Func_20
