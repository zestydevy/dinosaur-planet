.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHmushroom_ctor
.dword SHmushroom_dtor

# export table
/*0*/ .dword SHmushroom_setup
/*1*/ .dword SHmushroom_control
/*2*/ .dword SHmushroom_update
/*3*/ .dword SHmushroom_print
/*4*/ .dword SHmushroom_free
/*5*/ .dword SHmushroom_get_model_flags
/*6*/ .dword SHmushroom_get_data_size
