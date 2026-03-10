.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword MagicDust_ctor
.dword MagicDust_dtor

# export table
/*0*/ .dword MagicDust_setup
/*1*/ .dword MagicDust_control
/*2*/ .dword MagicDust_update
/*3*/ .dword MagicDust_print
/*4*/ .dword MagicDust_free
/*5*/ .dword MagicDust_get_model_flags
/*6*/ .dword MagicDust_get_data_size
