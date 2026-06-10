.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFSH_Door2_ctor
.dword DFSH_Door2_dtor

# export table
/*0*/ .dword DFSH_Door2_setup
/*1*/ .dword DFSH_Door2_control
/*2*/ .dword DFSH_Door2_update
/*3*/ .dword DFSH_Door2_print
/*4*/ .dword DFSH_Door2_free
/*5*/ .dword DFSH_Door2_get_model_flags
/*6*/ .dword DFSH_Door2_get_data_size
