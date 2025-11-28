.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword Foodbag_ctor
.dword Foodbag_dtor

# export table
/*0*/ .dword Foodbag_setup
/*1*/ .dword Foodbag_control
/*2*/ .dword Foodbag_update
/*3*/ .dword Foodbag_print
/*4*/ .dword Foodbag_free
/*5*/ .dword Foodbag_get_model_flags
/*6*/ .dword Foodbag_get_data_size
/*7*/ .dword Foodbag_is_obtained
/*8*/ .dword Foodbag_get_nearest_placed_food_of_type
/*9*/ .dword Foodbag_destroy_placed_food
/*10*/ .dword Foodbag_set_capacity
/*11*/ .dword Foodbag_collect_food
/*12*/ .dword Foodbag_delete_food_by_gamebit
/*13*/ .dword Foodbag_set_eat_config
/*14*/ .dword Foodbag_get_capacity
/*15*/ .dword Foodbag_count_slots_occupied
/*16*/ .dword Foodbag_get_anim_objectID_from_food_gamebit
