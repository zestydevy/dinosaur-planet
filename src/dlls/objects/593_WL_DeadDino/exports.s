.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WL_DeadDino_ctor
.dword WL_DeadDino_dtor

# export table
/*0*/ .dword WL_DeadDino_setup
/*1*/ .dword WL_DeadDino_control
/*2*/ .dword WL_DeadDino_update
/*3*/ .dword WL_DeadDino_print
/*4*/ .dword WL_DeadDino_free
/*5*/ .dword WL_DeadDino_get_model_flags
/*6*/ .dword WL_DeadDino_get_data_size
