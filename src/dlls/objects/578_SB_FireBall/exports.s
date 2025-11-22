.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SB_FireBall_ctor
.dword SB_FireBall_dtor

# export table
/*0*/ .dword SB_FireBall_setup
/*1*/ .dword SB_FireBall_control
/*2*/ .dword SB_FireBall_update
/*3*/ .dword SB_FireBall_print
/*4*/ .dword SB_FireBall_free
/*5*/ .dword SB_FireBall_get_model_flags
/*6*/ .dword SB_FireBall_get_data_size
