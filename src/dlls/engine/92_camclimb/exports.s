.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camclimb_ctor
.dword camclimb_dtor

# export table
/*0*/ .dword camclimb_setup
/*1*/ .dword camclimb_control
/*2*/ .dword camclimb_free
/*3*/ .dword camclimb_func_50C
