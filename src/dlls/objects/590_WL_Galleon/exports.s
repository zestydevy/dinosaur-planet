.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WLgalleon_ctor
.dword WLgalleon_dtor

# export table
/*0*/ .dword WLgalleon_setup
/*1*/ .dword WLgalleon_control
/*2*/ .dword WLgalleon_update
/*3*/ .dword WLgalleon_print
/*4*/ .dword WLgalleon_free
/*5*/ .dword WLgalleon_get_model_flags
/*6*/ .dword WLgalleon_get_data_size
