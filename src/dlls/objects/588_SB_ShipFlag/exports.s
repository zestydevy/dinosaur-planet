.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SB_ShipFlag_ctor
.dword SB_ShipFlag_dtor

# export table
/*0*/ .dword SB_ShipFlag_setup
/*1*/ .dword SB_ShipFlag_control
/*2*/ .dword SB_ShipFlag_update
/*3*/ .dword SB_ShipFlag_print
/*4*/ .dword SB_ShipFlag_free
/*5*/ .dword SB_ShipFlag_get_model_flags
/*6*/ .dword SB_ShipFlag_get_data_size
