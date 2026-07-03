.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camshipbattle_ctor
.dword camshipbattle_dtor

# export table
/*0*/ .dword camshipbattle_setup
/*1*/ .dword camshipbattle_control
/*2*/ .dword camshipbattle_free
/*3*/ .dword camshipbattle_func_50C
