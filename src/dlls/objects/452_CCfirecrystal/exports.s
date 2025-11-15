.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CCfirecrystal_ctor
.dword CCfirecrystal_dtor

# export table
/*0*/ .dword CCfirecrystal_setup
/*1*/ .dword CCfirecrystal_control
/*2*/ .dword CCfirecrystal_update
/*3*/ .dword CCfirecrystal_print
/*4*/ .dword CCfirecrystal_free
/*5*/ .dword CCfirecrystal_get_model_flags
/*6*/ .dword CCfirecrystal_get_data_size
