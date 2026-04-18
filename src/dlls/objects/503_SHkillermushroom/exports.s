.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHkillermushroom_ctor
.dword SHkillermushroom_dtor

# export table
/*0*/ .dword SHkillermushroom_setup
/*1*/ .dword SHkillermushroom_control
/*2*/ .dword SHkillermushroom_update
/*3*/ .dword SHkillermushroom_print
/*4*/ .dword SHkillermushroom_free
/*5*/ .dword SHkillermushroom_get_model_flags
/*6*/ .dword SHkillermushroom_get_data_size
