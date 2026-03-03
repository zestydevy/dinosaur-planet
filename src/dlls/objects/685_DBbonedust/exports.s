.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DBBoneDust_ctor
.dword DBBoneDust_dtor

# export table
/*0*/ .dword DBBoneDust_setup
/*1*/ .dword DBBoneDust_control
/*2*/ .dword DBBoneDust_update
/*3*/ .dword DBBoneDust_print
/*4*/ .dword DBBoneDust_free
/*5*/ .dword DBBoneDust_get_model_flags
/*6*/ .dword DBBoneDust_get_data_size
/*7*/ .dword DBBoneDust_is_hidden
/*8*/ .dword DBBoneDust_launch
