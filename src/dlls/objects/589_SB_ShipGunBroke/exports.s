.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SB_ShipGunBroke_ctor
.dword SB_ShipGunBroke_dtor

# export table
/*0*/ .dword SB_ShipGunBroke_setup
/*1*/ .dword SB_ShipGunBroke_control
/*2*/ .dword SB_ShipGunBroke_update
/*3*/ .dword SB_ShipGunBroke_print
/*4*/ .dword SB_ShipGunBroke_free
/*5*/ .dword SB_ShipGunBroke_get_model_flags
/*6*/ .dword SB_ShipGunBroke_get_data_size
