.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WallAnimator_ctor
.dword WallAnimator_dtor

# export table
/*0*/ .dword WallAnimator_setup
/*1*/ .dword WallAnimator_control
/*2*/ .dword WallAnimator_update
/*3*/ .dword WallAnimator_print
/*4*/ .dword WallAnimator_free
/*5*/ .dword WallAnimator_get_model_flags
/*6*/ .dword WallAnimator_get_data_size
/*7*/ .dword WallAnimator_tick_dig
/*8*/ .dword WallAnimator_is_dig_finished
/*9*/ .dword WallAnimator_get_energy_cost
