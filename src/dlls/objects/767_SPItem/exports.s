.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SPItem_ctor
.dword SPItem_dtor

# export table
/*0*/ .dword SPItem_setup
/*1*/ .dword SPItem_control
/*2*/ .dword SPItem_update
/*3*/ .dword SPItem_print
/*4*/ .dword SPItem_free
/*5*/ .dword SPItem_get_model_flags
/*6*/ .dword SPItem_get_data_size
