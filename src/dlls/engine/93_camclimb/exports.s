.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camclimb_ctor
.dword camclimb_dtor

# export table
/*0*/ .dword camclimb_func_18
/*1*/ .dword camclimb_func_340
/*2*/ .dword camclimb_func_5E8
/*3*/ .dword camclimb_func_62C
