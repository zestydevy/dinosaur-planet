.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword GPSH_Shrine_ctor
.dword GPSH_Shrine_dtor

# export table
/*0*/ .dword GPSH_Shrine_setup
/*1*/ .dword GPSH_Shrine_control
/*2*/ .dword GPSH_Shrine_update
/*3*/ .dword GPSH_Shrine_print
/*4*/ .dword GPSH_Shrine_free
/*5*/ .dword GPSH_Shrine_get_model_flags
/*6*/ .dword GPSH_Shrine_get_data_size
