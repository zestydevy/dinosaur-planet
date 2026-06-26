.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CRSnowBike_ctor
.dword CRSnowBike_dtor

# export table
/*0*/ .dword CRSnowBike_setup
/*1*/ .dword CRSnowBike_control
/*2*/ .dword CRSnowBike_update
/*3*/ .dword CRSnowBike_print
/*4*/ .dword CRSnowBike_free
/*5*/ .dword CRSnowBike_get_model_flags
/*6*/ .dword CRSnowBike_get_data_size
/*7*/ .dword CRSnowBike_can_bike_be_mounted
/*8*/ .dword CRSnowBike_get_mount_side
/*9*/ .dword CRSnowBike_get_attach_position
/*10*/ .dword CRSnowBike_can_bike_be_dismounted
/*11*/ .dword CRSnowBike_get_dismount_side
/*12*/ .dword CRSnowBike_get_front_of_bike_coords
/*13*/ .dword CRSnowBike_get_state
/*14*/ .dword CRSnowBike_set_state
/*15*/ .dword CRSnowBike_get_turn_tvalue_and_direction
/*16*/ .dword CRSnowBike_get_speed_tvalue_and_max
/*17*/ .dword CRSnowBike_get_ranking
/*18*/ .dword CRSnowBike_sharpclaw_start_race
/*19*/ .dword CRSnowBike_func_1DB8
/*20*/ .dword CRSnowBike_func_4B30
