.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBPlatform_ctor
.dword DBPlatform_dtor

# export table
/*0*/ .dword DBPlatform_setup
/*1*/ .dword DBPlatform_control
/*2*/ .dword DBPlatform_update
/*3*/ .dword DBPlatform_print
/*4*/ .dword DBPlatform_free
/*5*/ .dword DBPlatform_get_model_flags
/*6*/ .dword DBPlatform_get_data_size
