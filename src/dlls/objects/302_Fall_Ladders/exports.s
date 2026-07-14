.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Fall_Ladders_ctor
.dword Fall_Ladders_dtor

# export table
/*0*/ .dword Fall_Ladders_setup
/*1*/ .dword Fall_Ladders_control
/*2*/ .dword Fall_Ladders_update
/*3*/ .dword Fall_Ladders_print
/*4*/ .dword Fall_Ladders_free
/*5*/ .dword Fall_Ladders_get_model_flags
/*6*/ .dword Fall_Ladders_get_data_size
