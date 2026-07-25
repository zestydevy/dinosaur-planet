.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camdev_ctor
.dword camdev_dtor

# export table
/*0*/ .dword camdev_func_18
/*1*/ .dword camdev_func_78
/*2*/ .dword camdev_func_340
/*3*/ .dword camdev_func_380
