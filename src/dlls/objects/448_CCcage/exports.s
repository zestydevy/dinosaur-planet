.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCcage_ctor
.dword CCcage_dtor

# export table
/*0*/ .dword CCcage_setup
/*1*/ .dword CCcage_control
/*2*/ .dword CCcage_update
/*3*/ .dword CCcage_print
/*4*/ .dword CCcage_free
/*5*/ .dword CCcage_get_model_flags
/*6*/ .dword CCcage_get_data_size
