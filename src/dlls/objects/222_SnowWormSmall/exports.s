.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SnowWormSmall_ctor
.dword SnowWormSmall_dtor

# export table
/*0*/ .dword SnowWormSmall_setup
/*1*/ .dword SnowWormSmall_control
/*2*/ .dword SnowWormSmall_update
/*3*/ .dword SnowWormSmall_print
/*4*/ .dword SnowWormSmall_free
/*5*/ .dword SnowWormSmall_get_model_flags
/*6*/ .dword SnowWormSmall_get_data_size
/*7*/ .dword SnowWormSmall_get_anim_state
/*8*/ .dword SnowWormSmall_receive_message
