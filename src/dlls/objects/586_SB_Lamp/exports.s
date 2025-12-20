.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SB_Lamp_ctor
.dword SB_Lamp_dtor

# export table
/*0*/ .dword SB_Lamp_setup
/*1*/ .dword SB_Lamp_control
/*2*/ .dword SB_Lamp_update
/*3*/ .dword SB_Lamp_print
/*4*/ .dword SB_Lamp_free
/*5*/ .dword SB_Lamp_get_model_flags
/*6*/ .dword SB_Lamp_get_data_size
