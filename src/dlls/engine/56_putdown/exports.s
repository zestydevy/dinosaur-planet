.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword putdown_ctor
.dword putdown_dtor

# export table
/*0*/ .dword putdown_place_food
/*1*/ .dword putdown_get_nearest_placed_food_of_type
/*2*/ .dword putdown_destroy_placed_food
/*3*/ .dword putdown_func_47C
/*4*/ .dword putdown_func_5E8
/*5*/ .dword putdown_func_730
/*6*/ .dword putdown_func_80C
/*7*/ .dword putdown_func_900
/*8*/ .dword putdown_update_food_quantity_gamebits
/*9*/ .dword putdown_func_AFC
