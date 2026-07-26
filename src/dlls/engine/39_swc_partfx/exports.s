.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword swcPartfx_ctor
.dword swcPartfx_dtor

# export table
/*0*/ .dword swcPartfx_Func_18
/*1*/ .dword swcPartfx_Spawn
/*2*/ .dword swcPartfx_Func_20
