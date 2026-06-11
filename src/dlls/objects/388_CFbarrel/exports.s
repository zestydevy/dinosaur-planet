.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFbarrel_ctor
.dword CFbarrel_dtor

# export table
/*0*/ .dword CFbarrel_setup
/*1*/ .dword CFbarrel_control
/*2*/ .dword CFbarrel_update
/*3*/ .dword CFbarrel_print
/*4*/ .dword CFbarrel_free
/*5*/ .dword CFbarrel_get_model_flags
/*6*/ .dword CFbarrel_get_data_size
/*7*/ .dword CFbarrel_func_17F4
