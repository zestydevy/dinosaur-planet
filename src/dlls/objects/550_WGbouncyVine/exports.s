.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WGBouncyVine_ctor
.dword WGBouncyVine_dtor

# export table
/*0*/ .dword WGBouncyVine_setup
/*1*/ .dword WGBouncyVine_control
/*2*/ .dword WGBouncyVine_update
/*3*/ .dword WGBouncyVine_print
/*4*/ .dword WGBouncyVine_free
/*5*/ .dword WGBouncyVine_get_model_flags
/*6*/ .dword WGBouncyVine_get_data_size
