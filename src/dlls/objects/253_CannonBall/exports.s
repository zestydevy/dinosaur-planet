.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CannonBall_ctor
.dword CannonBall_dtor

# export table
/*0*/ .dword CannonBall_setup
/*1*/ .dword CannonBall_control
/*2*/ .dword CannonBall_update
/*3*/ .dword CannonBall_print
/*4*/ .dword CannonBall_free
/*5*/ .dword CannonBall_get_model_flags
/*6*/ .dword CannonBall_get_data_size
