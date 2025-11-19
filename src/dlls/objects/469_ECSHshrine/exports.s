.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_469_ctor
.dword dll_469_dtor

# export table
/*0*/ .dword dll_469_setup
/*1*/ .dword dll_469_control
/*2*/ .dword dll_469_update
/*3*/ .dword dll_469_print
/*4*/ .dword dll_469_free
/*5*/ .dword dll_469_get_model_flags
/*6*/ .dword dll_469_get_data_size
/*7*/ .dword dll_469_func_1454
/*8*/ .dword dll_469_func_1484
/*9*/ .dword dll_469_func_14F4
/*10*/ .dword dll_469_func_152C
/*11*/ .dword dll_469_func_1590
