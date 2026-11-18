.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword attentioncam_ctor
.dword attentioncam_dtor

# export table
/*0*/ .dword attentioncam_Setup
/*1*/ .dword attentioncam_Control
/*2*/ .dword attentioncam_Free
/*3*/ .dword attentioncam_Func_B04
