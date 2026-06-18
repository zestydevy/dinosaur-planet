.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCSH_Shrine_ctor
.dword CCSH_Shrine_dtor

# export table
/*0*/ .dword CCSH_Shrine_setup
/*1*/ .dword CCSH_Shrine_control
/*2*/ .dword CCSH_Shrine_update
/*3*/ .dword CCSH_Shrine_print
/*4*/ .dword CCSH_Shrine_free
/*5*/ .dword CCSH_Shrine_get_model_flags
/*6*/ .dword CCSH_Shrine_get_data_size
