.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword FXEmit_ctor
.dword FXEmit_dtor

# export table
/*0*/ .dword FXEmit_setup
/*1*/ .dword FXEmit_control
/*2*/ .dword FXEmit_update
/*3*/ .dword FXEmit_print
/*4*/ .dword FXEmit_free
/*5*/ .dword FXEmit_get_model_flags
/*6*/ .dword FXEmit_get_data_size
