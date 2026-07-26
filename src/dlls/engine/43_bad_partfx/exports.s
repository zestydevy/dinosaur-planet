.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword badPartfx_ctor
.dword badPartfx_dtor

# export table
/*0*/ .dword badPartfx_Func_18
/*1*/ .dword badPartfx_Spawn
/*2*/ .dword badPartfx_Func_20
