.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword HitAnimator_ctor
.dword HitAnimator_dtor

# export table
/*0*/ .dword HitAnimator_setup
/*1*/ .dword HitAnimator_control
/*2*/ .dword HitAnimator_update
/*3*/ .dword HitAnimator_print
/*4*/ .dword HitAnimator_free
/*5*/ .dword HitAnimator_get_model_flags
/*6*/ .dword HitAnimator_get_data_size
