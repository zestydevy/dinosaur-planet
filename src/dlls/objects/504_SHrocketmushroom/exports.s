.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHrocketmushroom_ctor
.dword SHrocketmushroom_dtor

# export table
/*0*/ .dword SHrocketmushroom_setup
/*1*/ .dword SHrocketmushroom_control
/*2*/ .dword SHrocketmushroom_update
/*3*/ .dword SHrocketmushroom_print
/*4*/ .dword SHrocketmushroom_free
/*5*/ .dword SHrocketmushroom_get_model_flags
/*6*/ .dword SHrocketmushroom_get_data_size
