.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SideFoodbag_ctor
.dword SideFoodbag_dtor

# export table
/*0*/ .dword SideFoodbag_setup
/*1*/ .dword SideFoodbag_control
/*2*/ .dword SideFoodbag_update
/*3*/ .dword SideFoodbag_print
/*4*/ .dword SideFoodbag_free
/*5*/ .dword SideFoodbag_get_model_flags
/*6*/ .dword SideFoodbag_get_data_size
/*7*/ .dword SideFoodbag_is_obtained
/*8*/ .dword SideFoodbag_get_nearest_placed_food_of_type
/*9*/ .dword SideFoodbag_destroy_placed_food
/*10*/ .dword SideFoodbag_set_capacity
/*11*/ .dword SideFoodbag_collect_food
/*12*/ .dword SideFoodbag_delete_food_by_gamebit
