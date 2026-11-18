.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camspellaim_ctor
.dword camspellaim_dtor

# export table
/*0*/ .dword camspellaim_Setup
/*1*/ .dword camspellaim_Control
/*2*/ .dword camspellaim_Free
/*3*/ .dword camspellaim_Func_2D4
