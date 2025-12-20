.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SB_ShipGun_ctor
.dword SB_ShipGun_dtor

# export table
/*0*/ .dword SB_ShipGun_setup
/*1*/ .dword SB_ShipGun_control
/*2*/ .dword SB_ShipGun_update
/*3*/ .dword SB_ShipGun_print
/*4*/ .dword SB_ShipGun_free
/*5*/ .dword SB_ShipGun_get_model_flags
/*6*/ .dword SB_ShipGun_get_data_size
