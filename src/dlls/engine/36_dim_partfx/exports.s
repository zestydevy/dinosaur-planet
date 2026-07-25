.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dimPartfx_ctor
.dword dimPartfx_dtor

# export table
/*0*/ .dword dimPartfx_Func_18
/*1*/ .dword dimPartfx_Spawn
/*2*/ .dword dimPartfx_Func_20
