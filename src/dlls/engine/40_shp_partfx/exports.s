.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword shpPartfx_ctor
.dword shpPartfx_dtor

# export table
/*0*/ .dword shpPartfx_Func_18
/*1*/ .dword shpPartfx_Spawn
/*2*/ .dword shpPartfx_Func_20
