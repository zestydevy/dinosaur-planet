.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword VisAnimator_ctor
.dword VisAnimator_dtor

# export table
/*0*/ .dword VisAnimator_setup
/*1*/ .dword VisAnimator_control
/*2*/ .dword VisAnimator_update
/*3*/ .dword VisAnimator_print
/*4*/ .dword VisAnimator_free
/*5*/ .dword VisAnimator_get_model_flags
/*6*/ .dword VisAnimator_get_data_size
