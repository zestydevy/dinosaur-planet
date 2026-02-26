.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword ECSHCup_ctor
.dword ECSHCup_dtor

# export table
/*0*/ .dword ECSHCup_setup
/*1*/ .dword ECSHCup_control
/*2*/ .dword ECSHCup_update
/*3*/ .dword ECSHCup_print
/*4*/ .dword ECSHCup_free
/*5*/ .dword ECSHCup_get_model_flags
/*6*/ .dword ECSHCup_get_data_size
