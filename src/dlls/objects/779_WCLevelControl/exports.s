.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WCLevelControl_ctor
.dword WCLevelControl_dtor

# export table
/*0*/ .dword WCLevelControl_setup
/*1*/ .dword WCLevelControl_control
/*2*/ .dword WCLevelControl_update
/*3*/ .dword WCLevelControl_print
/*4*/ .dword WCLevelControl_free
/*5*/ .dword WCLevelControl_get_model_flags
/*6*/ .dword WCLevelControl_get_data_size
/*7*/ .dword WCLevelControl_sun_puzzle_set_coords_from_grid_position
/*8*/ .dword WCLevelControl_sun_puzzle_set_grid_position_from_coords
/*9*/ .dword WCLevelControl_sun_puzzle_set_cell
/*10*/ .dword WCLevelControl_sun_puzzle_get_cell
/*11*/ .dword WCLevelControl_sun_puzzle_setup_position_hard
/*12*/ .dword WCLevelControl_sun_puzzle_setup_position_easy
/*13*/ .dword WCLevelControl_sun_puzzle_func_8C8
/*14*/ .dword WCLevelControl_moon_puzzle_set_coords_from_grid_position
/*15*/ .dword WCLevelControl_moon_puzzle_set_grid_position_from_coords
/*16*/ .dword WCLevelControl_moon_puzzle_set_cell
/*17*/ .dword WCLevelControl_moon_puzzle_get_cell
/*18*/ .dword WCLevelControl_moon_puzzle_setup_position_hard
/*19*/ .dword WCLevelControl_moon_puzzle_setup_position_easy
/*20*/ .dword WCLevelControl_moon_puzzle_func_100C
