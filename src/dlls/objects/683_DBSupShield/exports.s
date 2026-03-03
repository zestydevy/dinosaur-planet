.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBSupShield_ctor
.dword DBSupShield_dtor

# export table
/*0*/ .dword DBSupShield_setup
/*1*/ .dword DBSupShield_control
/*2*/ .dword DBSupShield_update
/*3*/ .dword DBSupShield_print
/*4*/ .dword DBSupShield_free
/*5*/ .dword DBSupShield_get_model_flags
/*6*/ .dword DBSupShield_get_data_size
