.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword bwcPartfx_ctor
.dword bwcPartfx_dtor

# export table
/*0*/ .dword bwcPartfx_Func_18
/*1*/ .dword bwcPartfx_Spawn
/*2*/ .dword bwcPartfx_Func_20
