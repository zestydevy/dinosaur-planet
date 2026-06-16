.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword LanternFireFly_ctor
.dword LanternFireFly_dtor

# export table
/*0*/ .dword LanternFireFly_setup
/*1*/ .dword LanternFireFly_control
/*2*/ .dword LanternFireFly_update
/*3*/ .dword LanternFireFly_print
/*4*/ .dword LanternFireFly_free
/*5*/ .dword LanternFireFly_get_model_flags
/*6*/ .dword LanternFireFly_get_data_size
/*7*/ .dword LanternFireFly_set_next_spline_coord_by_player_position
/*8*/ .dword LanternFireFly_send
/*9*/ .dword LanternFireFly_set_home
