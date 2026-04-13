.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_598_ctor
.dword dll_598_dtor

# export table
/*0*/ .dword dll_598_setup
/*1*/ .dword dll_598_control
/*2*/ .dword dll_598_update
/*3*/ .dword dll_598_print
/*4*/ .dword dll_598_free
/*5*/ .dword dll_598_get_model_flags
/*6*/ .dword dll_598_get_data_size
