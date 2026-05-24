.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword BWlog_ctor
.dword BWlog_dtor

# export table
/*0*/ .dword BWlog_setup
/*1*/ .dword BWlog_control
/*2*/ .dword BWlog_update
/*3*/ .dword BWlog_print
/*4*/ .dword BWlog_free
/*5*/ .dword BWlog_get_model_flags
/*6*/ .dword BWlog_get_data_size
/*7*/ .dword BWlog_vehicle_can_mount
/*8*/ .dword BWlog_vehicle_get_mount_side
/*9*/ .dword BWlog_vehicle_get_rider_position
/*10*/ .dword BWlog_vehicle_can_dismount
/*11*/ .dword BWlog_vehicle_get_dismount_side
/*12*/ .dword BWlog_vehicle_get_camera_position
/*13*/ .dword BWlog_vehicle_get_mount_state
/*14*/ .dword BWlog_vehicle_set_mount_state
/*15*/ .dword BWlog_vehicle_get_player_anim
/*16*/ .dword BWlog_func_E48
/*17*/ .dword BWlog_vehicle_get_race_position
/*18*/ .dword BWlog_func_E80
/*19*/ .dword BWlog_vehicle_handle_rider_scale
/*20*/ .dword BWlog_func_E9C
