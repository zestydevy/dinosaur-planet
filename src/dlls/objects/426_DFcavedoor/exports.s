.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFcavedoor_ctor
.dword DFcavedoor_dtor

# export table
/*0*/ .dword DFcavedoor_setup
/*1*/ .dword DFcavedoor_control
/*2*/ .dword DFcavedoor_update
/*3*/ .dword DFcavedoor_print
/*4*/ .dword DFcavedoor_free
/*5*/ .dword DFcavedoor_get_model_flags
/*6*/ .dword DFcavedoor_get_data_size
