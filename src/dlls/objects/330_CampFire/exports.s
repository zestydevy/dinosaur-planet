.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword campfire_ctor
.dword campfire_dtor

# export table
/*0*/ .dword campfire_setup
/*1*/ .dword campfire_control
/*2*/ .dword campfire_update
/*3*/ .dword campfire_print
/*4*/ .dword campfire_free
/*5*/ .dword campfire_get_model_flags
/*6*/ .dword campfire_get_data_size
