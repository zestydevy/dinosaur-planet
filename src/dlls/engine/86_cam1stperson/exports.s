.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword cam1stperson_ctor
.dword cam1stperson_dtor

# export table
/*0*/ .dword cam1stperson_func_18
/*1*/ .dword cam1stperson_func_4F8
/*2*/ .dword cam1stperson_func_A04
/*3*/ .dword cam1stperson_func_A64
