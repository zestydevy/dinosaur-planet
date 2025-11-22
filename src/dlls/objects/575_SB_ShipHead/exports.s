.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SB_ShipHead_ctor
.dword SB_ShipHead_dtor

# export table
/*0*/ .dword SB_ShipHead_setup
/*1*/ .dword SB_ShipHead_control
/*2*/ .dword SB_ShipHead_update
/*3*/ .dword SB_ShipHead_print
/*4*/ .dword SB_ShipHead_free
/*5*/ .dword SB_ShipHead_get_model_flags
/*6*/ .dword SB_ShipHead_get_data_size
