.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFScalesGalleon_ctor
.dword CFScalesGalleon_dtor

# export table
/*0*/ .dword CFScalesGalleon_setup
/*1*/ .dword CFScalesGalleon_control
/*2*/ .dword CFScalesGalleon_update
/*3*/ .dword CFScalesGalleon_print
/*4*/ .dword CFScalesGalleon_free
/*5*/ .dword CFScalesGalleon_get_model_flags
/*6*/ .dword CFScalesGalleon_get_data_size
