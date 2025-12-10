.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword collectable_ctor
.dword collectable_dtor

# export table
/*0*/ .dword collectable_setup
/*1*/ .dword collectable_control
/*2*/ .dword collectable_update
/*3*/ .dword collectable_print
/*4*/ .dword collectable_free
/*5*/ .dword collectable_get_model_flags
/*6*/ .dword collectable_get_data_size
/*7*/ .dword collectable_is_collected
/*8*/ .dword collectable_set_pause_state
/*9*/ .dword collectable_get_area_value
/*10*/ .dword collectable_set_speed
/*11*/ .dword collectable_set_visibility
/*12*/ .dword collectable_get_visibility
/*13*/ .dword collectable_save_position
