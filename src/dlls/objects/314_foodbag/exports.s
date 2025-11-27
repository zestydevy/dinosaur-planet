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
/*7*/ .dword Foodbag_func_590
/*8*/ .dword Foodbag_func_5B4
/*9*/ .dword Foodbag_func_5FC
/*10*/ .dword Foodbag_func_83C
/*11*/ .dword Foodbag_func_708
/*12*/ .dword Foodbag_func_7E0
/*13*/ .dword Foodbag_func_650
/*14*/ .dword Foodbag_func_A0C
/*15*/ .dword Foodbag_func_A1C
/*16*/ .dword Foodbag_get_anim_objectID_from_food_gamebit
