.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCgrub_ctor
.dword CCgrub_dtor

# export table
/*0*/ .dword CCgrub_setup
/*1*/ .dword CCgrub_control
/*2*/ .dword CCgrub_update
/*3*/ .dword CCgrub_print
/*4*/ .dword CCgrub_free
/*5*/ .dword CCgrub_get_model_flags
/*6*/ .dword CCgrub_get_data_size
