.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dakPartfx_ctor
.dword dakPartfx_dtor

# export table
/*0*/ .dword dakPartfx_Func_18
/*1*/ .dword dakPartfx_Spawn
/*2*/ .dword dakPartfx_Func_20
