.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camtalk_ctor
.dword camtalk_dtor

# export table
/*0*/ .dword camtalk_func_18
/*1*/ .dword camtalk_func_320
/*2*/ .dword camtalk_func_544
/*3*/ .dword camtalk_func_584
