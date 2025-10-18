.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SB_Propeller_ctor
.dword SB_Propeller_dtor

# export table
/*0*/ .dword SB_Propeller_setup
/*1*/ .dword SB_Propeller_control
/*2*/ .dword SB_Propeller_update
/*3*/ .dword SB_Propeller_print
/*4*/ .dword SB_Propeller_free
/*5*/ .dword SB_Propeller_get_model_flags
/*6*/ .dword SB_Propeller_get_data_size
