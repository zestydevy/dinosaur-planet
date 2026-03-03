.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ExplodeAnimator_ctor
.dword ExplodeAnimator_dtor

# export table
/*0*/ .dword ExplodeAnimator_setup
/*1*/ .dword ExplodeAnimator_control
/*2*/ .dword ExplodeAnimator_update
/*3*/ .dword ExplodeAnimator_print
/*4*/ .dword ExplodeAnimator_free
/*5*/ .dword ExplodeAnimator_get_model_flags
/*6*/ .dword ExplodeAnimator_get_data_size
