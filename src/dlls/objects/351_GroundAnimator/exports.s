.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword GroundAnimator_ctor
.dword GroundAnimator_dtor

# export table
/*0*/ .dword GroundAnimator_setup
/*1*/ .dword GroundAnimator_control
/*2*/ .dword GroundAnimator_update
/*3*/ .dword GroundAnimator_print
/*4*/ .dword GroundAnimator_free
/*5*/ .dword GroundAnimator_get_model_flags
/*6*/ .dword GroundAnimator_get_data_size
/*7*/ .dword GroundAnimator_tick_dig
/*8*/ .dword GroundAnimator_is_dig_finished
/*9*/ .dword GroundAnimator_get_magic_cave_index
