.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword camcloudrunner_ctor
.dword camcloudrunner_dtor

# export table
/*0*/ .dword camcloudrunner_func_18
/*1*/ .dword camcloudrunner_func_17C
/*2*/ .dword camcloudrunner_func_2C4
/*3*/ .dword camcloudrunner_func_304
