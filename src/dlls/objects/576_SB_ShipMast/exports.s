.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SB_ShipMast_ctor
.dword SB_ShipMast_dtor

# export table
/*0*/ .dword SB_ShipMast_setup
/*1*/ .dword SB_ShipMast_control
/*2*/ .dword SB_ShipMast_update
/*3*/ .dword SB_ShipMast_print
/*4*/ .dword SB_ShipMast_free
/*5*/ .dword SB_ShipMast_get_model_flags
/*6*/ .dword SB_ShipMast_get_data_size
