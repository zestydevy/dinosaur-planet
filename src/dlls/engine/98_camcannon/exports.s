.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camcannon_ctor
.dword camcannon_dtor

# export table
/*0*/ .dword camcannon_setup
/*1*/ .dword camcannon_control
/*2*/ .dword camcannon_free
/*3*/ .dword camcannon_func_1E8
