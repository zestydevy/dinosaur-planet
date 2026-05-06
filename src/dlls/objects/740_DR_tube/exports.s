.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DR_Tube_ctor
.dword DR_Tube_dtor

# export table
/*0*/ .dword DR_Tube_setup
/*1*/ .dword DR_Tube_control
/*2*/ .dword DR_Tube_update
/*3*/ .dword DR_Tube_print
/*4*/ .dword DR_Tube_free
/*5*/ .dword DR_Tube_get_model_flags
/*6*/ .dword DR_Tube_get_data_size
