.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WM_MovePlatform_ctor
.dword WM_MovePlatform_dtor

# export table
/*0*/ .dword WM_MovePlatform_setup
/*1*/ .dword WM_MovePlatform_control
/*2*/ .dword WM_MovePlatform_update
/*3*/ .dword WM_MovePlatform_print
/*4*/ .dword WM_MovePlatform_free
/*5*/ .dword WM_MovePlatform_get_model_flags
/*6*/ .dword WM_MovePlatform_get_data_size
