.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camcrawl_ctor
.dword camcrawl_dtor

# export table
/*0*/ .dword camcrawl_setup
/*1*/ .dword camcrawl_control
/*2*/ .dword camcrawl_free
/*3*/ .dword camcrawl_func_1E8
