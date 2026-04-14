.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword InfoPoint_ctor
.dword InfoPoint_dtor

# export table
/*0*/ .dword InfoPoint_setup
/*1*/ .dword InfoPoint_control
/*2*/ .dword InfoPoint_update
/*3*/ .dword InfoPoint_print
/*4*/ .dword InfoPoint_free
/*5*/ .dword InfoPoint_get_model_flags
/*6*/ .dword InfoPoint_get_data_size
