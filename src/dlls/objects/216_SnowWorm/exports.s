.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SnowWorm_ctor
.dword SnowWorm_dtor

# export table
/*0*/ .dword SnowWorm_setup
/*1*/ .dword SnowWorm_control
/*2*/ .dword SnowWorm_update
/*3*/ .dword SnowWorm_print
/*4*/ .dword SnowWorm_free
/*5*/ .dword SnowWorm_get_model_flags
/*6*/ .dword SnowWorm_get_data_size
/*7*/ .dword SnowWorm_get_anim_state
/*8*/ .dword SnowWorm_receive_message
