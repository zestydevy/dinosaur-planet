.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFSH_Door2Special_ctor
.dword DFSH_Door2Special_dtor

# export table
/*0*/ .dword DFSH_Door2Special_setup
/*1*/ .dword DFSH_Door2Special_control
/*2*/ .dword DFSH_Door2Special_update
/*3*/ .dword DFSH_Door2Special_print
/*4*/ .dword DFSH_Door2Special_free
/*5*/ .dword DFSH_Door2Special_get_model_flags
/*6*/ .dword DFSH_Door2Special_get_data_size
