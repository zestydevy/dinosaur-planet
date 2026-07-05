.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword attentioncam_ctor
.dword attentioncam_dtor

# export table
/*0*/ .dword attentioncam_func_18
/*1*/ .dword attentioncam_func_848
/*2*/ .dword attentioncam_func_AC4
/*3*/ .dword attentioncam_func_B04
