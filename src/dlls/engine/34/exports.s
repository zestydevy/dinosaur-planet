.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_34_ctor
.dword dll_34_dtor

# export table
/*0*/ .dword dll_34_Func_18
/*1*/ .dword dll_34_Spawn
/*2*/ .dword dll_34_Func_20
