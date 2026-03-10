.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_426_ctor
.dword dll_426_dtor

# export table
/*0*/ .dword dll_426_setup
/*1*/ .dword dll_426_control
/*2*/ .dword dll_426_update
/*3*/ .dword dll_426_print
/*4*/ .dword dll_426_free
/*5*/ .dword dll_426_get_model_flags
/*6*/ .dword dll_426_get_data_size
