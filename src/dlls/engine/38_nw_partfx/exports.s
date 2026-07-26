.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword nwPartfx_ctor
.dword nwPartfx_dtor

# export table
/*0*/ .dword nwPartfx_Func_18
/*1*/ .dword nwPartfx_Spawn
/*2*/ .dword nwPartfx_Func_20
