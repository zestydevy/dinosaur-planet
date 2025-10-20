.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword FElevControl_ctor
.dword FElevControl_dtor

# export table
/*0*/ .dword FElevControl_setup
/*1*/ .dword FElevControl_control
/*2*/ .dword FElevControl_update
/*3*/ .dword FElevControl_print
/*4*/ .dword FElevControl_free
/*5*/ .dword FElevControl_get_model_flags
/*6*/ .dword FElevControl_get_data_size
