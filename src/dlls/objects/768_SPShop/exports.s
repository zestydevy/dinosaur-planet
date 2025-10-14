.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SPShop_ctor
.dword SPShop_dtor

# export table
/*0*/ .dword SPShop_setup
/*1*/ .dword SPShop_control
/*2*/ .dword SPShop_update
/*3*/ .dword SPShop_print
/*4*/ .dword SPShop_free
/*5*/ .dword SPShop_get_model_flags
/*6*/ .dword SPShop_get_data_size
/*7*/ .dword SPShop_return_unk0
/*8*/ .dword SPShop_play_sequence
/*9*/ .dword SPShop_is_item_shown
/*10*/ .dword SPShop_is_item_hidden
/*11*/ .dword SPShop_get_minimum_price
/*12*/ .dword SPShop_get_special_price
/*13*/ .dword SPShop_get_initial_price
/*14*/ .dword SPShop_get_item_gametext_index
/*15*/ .dword SPShop_set_current_item_index
/*16*/ .dword SPShop_get_current_item_index
/*17*/ .dword SPShop_buy_item
/*18*/ .dword SPShop_func_884
/*19*/ .dword SPShop_func_89C
/*20*/ .dword SPShop_func_8C0
