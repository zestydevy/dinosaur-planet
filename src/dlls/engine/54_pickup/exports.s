.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword pickup_ctor
.dword pickup_dtor

# export table
/*0*/ .dword pickup_setup
/*1*/ .dword pickup_control
/*2*/ .dword pickup_should_print
/*3*/ .dword pickup_free
/*4*/ .dword pickup_get_state
/*5*/ .dword pickup_was_just_picked_up
/*6*/ .dword pickup_func_6C0
/*7*/ .dword pickup_set_gravity
/*8*/ .dword pickup_set_drop_disabled
/*9*/ .dword pickup_get_drop_disabled
/*10*/ .dword pickup_set_dont_save
/*11*/ .dword pickup_drop
