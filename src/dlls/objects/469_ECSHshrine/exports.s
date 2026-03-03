.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ECSHshrine_ctor
.dword ECSHshrine_dtor

# export table
/*0*/ .dword ECSHshrine_setup
/*1*/ .dword ECSHshrine_control
/*2*/ .dword ECSHshrine_update
/*3*/ .dword ECSHshrine_print
/*4*/ .dword ECSHshrine_free
/*5*/ .dword ECSHshrine_get_model_flags
/*6*/ .dword ECSHshrine_get_data_size
/*7*/ .dword ECSHshrine_func_1454
/*8*/ .dword ECSHshrine_get_cup_coords
/*9*/ .dword ECSHshrine_get_minigame_state
/*10*/ .dword ECSHshrine_set_cup_coords
/*11*/ .dword ECSHshrine_choose_cup
