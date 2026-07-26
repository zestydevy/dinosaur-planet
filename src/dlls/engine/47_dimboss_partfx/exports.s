.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dimbossPartfx_ctor
.dword dimbossPartfx_dtor

# export table
/*0*/ .dword dimbossPartfx_Func_18
/*1*/ .dword dimbossPartfx_Spawn
/*2*/ .dword dimbossPartfx_Func_20
