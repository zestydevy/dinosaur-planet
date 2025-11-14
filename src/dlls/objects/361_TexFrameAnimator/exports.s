.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword TexFrameAnimator_ctor
.dword TexFrameAnimator_dtor

# export table
/*0*/ .dword TexFrameAnimator_setup
/*1*/ .dword TexFrameAnimator_control
/*2*/ .dword TexFrameAnimator_update
/*3*/ .dword TexFrameAnimator_print
/*4*/ .dword TexFrameAnimator_free
/*5*/ .dword TexFrameAnimator_get_model_flags
/*6*/ .dword TexFrameAnimator_get_data_size
