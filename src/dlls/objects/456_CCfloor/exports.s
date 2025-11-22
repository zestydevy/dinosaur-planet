.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCfloor_ctor
.dword CCfloor_dtor

# export table
/*0*/ .dword CCfloor_setup
/*1*/ .dword CCfloor_control
/*2*/ .dword CCfloor_update
/*3*/ .dword CCfloor_print
/*4*/ .dword CCfloor_free
/*5*/ .dword CCfloor_get_model_flags
/*6*/ .dword CCfloor_get_data_size
