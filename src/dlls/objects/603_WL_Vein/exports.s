.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WMvein_ctor
.dword WMvein_dtor

# export table
/*0*/ .dword WMvein_setup
/*1*/ .dword WMvein_control
/*2*/ .dword WMvein_update
/*3*/ .dword WMvein_print
/*4*/ .dword WMvein_free
/*5*/ .dword WMvein_get_model_flags
/*6*/ .dword WMvein_get_data_size
