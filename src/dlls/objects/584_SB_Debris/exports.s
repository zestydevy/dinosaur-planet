.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SB_Debris_ctor
.dword SB_Debris_dtor

# export table
/*0*/ .dword SB_Debris_setup
/*1*/ .dword SB_Debris_control
/*2*/ .dword SB_Debris_update
/*3*/ .dword SB_Debris_print
/*4*/ .dword SB_Debris_free
/*5*/ .dword SB_Debris_get_model_flags
/*6*/ .dword SB_Debris_get_data_size
