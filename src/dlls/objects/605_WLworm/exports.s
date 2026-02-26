.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WLWorm_ctor
.dword WLWorm_dtor

# export table
/*0*/ .dword WLWorm_setup
/*1*/ .dword WLWorm_control
/*2*/ .dword WLWorm_update
/*3*/ .dword WLWorm_print
/*4*/ .dword WLWorm_free
/*5*/ .dword WLWorm_get_model_flags
/*6*/ .dword WLWorm_get_data_size
