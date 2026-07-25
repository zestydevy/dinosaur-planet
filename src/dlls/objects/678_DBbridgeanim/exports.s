.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBbridgeanim_ctor
.dword DBbridgeanim_dtor

# export table
/*0*/ .dword DBbridgeanim_setup
/*1*/ .dword DBbridgeanim_control
/*2*/ .dword DBbridgeanim_update
/*3*/ .dword DBbridgeanim_print
/*4*/ .dword DBbridgeanim_free
/*5*/ .dword DBbridgeanim_get_model_flags
/*6*/ .dword DBbridgeanim_get_data_size
