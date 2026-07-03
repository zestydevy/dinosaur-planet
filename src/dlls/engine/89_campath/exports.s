.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword campath_ctor
.dword campath_dtor

# export table
/*0*/ .dword campath_func_18
/*1*/ .dword campath_func_588
/*2*/ .dword campath_func_DF0
/*3*/ .dword campath_func_E30
