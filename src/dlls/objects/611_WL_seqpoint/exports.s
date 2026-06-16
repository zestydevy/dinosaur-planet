.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WL_seqpoint_ctor
.dword WL_seqpoint_dtor

# export table
/*0*/ .dword WL_seqpoint_setup
/*1*/ .dword WL_seqpoint_control
/*2*/ .dword WL_seqpoint_update
/*3*/ .dword WL_seqpoint_print
/*4*/ .dword WL_seqpoint_free
/*5*/ .dword WL_seqpoint_get_model_flags
/*6*/ .dword WL_seqpoint_get_data_size
